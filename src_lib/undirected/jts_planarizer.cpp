#include "undirected/jts_planarizer.hpp"
#include "undirected/jts_tree.hpp"
#include "undirected/jts_tree_node.hpp"
#include "undirected/jts_inline.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/jts_planarizer.cpp
 *
 * @brief It performs Jayakumar, Thurasiraman, Swamy's PLANARIZE based on
 *        an augumented PQ-tree.
 */
namespace Wailea {

namespace Undirected {

using namespace std;


const utility_t   JTSPlanarizer::kForwardTypeInPlanarSubgraph = 0;
const utility_t   JTSPlanarizer::kForwardTypeRemoved          = 1;


/** @remarks
 *
 *  Implementation notes
 *
 *  1. Create an internal copy using PQGraph[Node,Edge].
 *  2. Find incoming and outgoing edges for each node.
 *  3. for each graph node in the st-ordering
 *    if not the first node:
 *        Bubble-up bottom up.
 *        Find W, H, A, and CD values for each tree node.
 *        Determine types top-down.
 *        Remove tree nodes and mark removed graph edges bottom-up.
 *        Apply templates bottom up.
 *        Reduce the consecutive pertinent leaves into an attachment node.
 *    fi
 *    if not the last node:
 *        Fan-out tree leaves for the outgoing edges from the attachment
 *    fi
 *
 *  4. Create a copy of graph in Graph[Node,Edge] for the planar
 *     subgraph and another for removed edges.
 */
void JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph(
    Graph&                  g,
    vector<node_list_it_t>& stOrder,
    PlanarizableGraph&      planarSubgraph,
    vector<edge_list_it_t>& removedEdges
) {

    JTSGraph               graphCopy;
    vector<node_list_it_t> stOrderCopy;
    JTSTree                pqTree;


    if (g.numNodes() <= 4) {
        // Already planar. Also handle k0, k1, and k2.
        handlek0tok4(g, planarSubgraph);
        return;
    }

    copyInputGraph(g, stOrder, graphCopy, stOrderCopy, pqTree);

    node_list_it_t attachmentNode = pqTree.makeInitialAttachmentP();

    size_t index = 0;

    for (auto nit : stOrderCopy ) {

        auto& n = dynamic_cast<JTSGraphNode&>(*(*nit));

//cerr << "\n\nIteration: " << index << "\n";


        if (index > 0) {

            // Reduction
            pqTree.initializeForOneIteration();

            vector<node_list_it_t> pertinentLeaves =
                            fromGraphEdgesToPQLeaves(pqTree, n.mIncomingEdges);

            node_list_it_t pertinentRoot =
                                    bubbleUp(pqTree, pertinentLeaves);

//cerr << "\n\nAfter bubbleUp PertRoot: [" << pqTree.toNodeRef(pertinentRoot).mNodeNum << "]\n";
//pqTree.printTree(cerr);

            findWHA(pqTree, pertinentLeaves, pertinentRoot);

            determineWHABCNodeTypes(pqTree, pertinentRoot);

//cerr << "\n\nAfter WHAB\n";
//pqTree.printTree(cerr);

            removeNonconsecutiveNodes(pqTree, pertinentLeaves, pertinentRoot);

//cerr << "\n\nAfter removeNonconsecutiveNodes PertRoot: [" << pqTree.toNodeRef(pertinentRoot).mNodeNum << "]\n";
//pqTree.printTree(cerr);

            vector<node_list_it_t> newPertinentLeaves =
                            fromGraphEdgesToPQLeaves(pqTree, n.mIncomingEdges);

            pertinentRoot = applyTemplates(
                                    pqTree, newPertinentLeaves, pertinentRoot);

//cerr << "\n\nAfter applyTemplates PertRoot: [" << pqTree.toNodeRef(pertinentRoot).mNodeNum << "]\n";
//pqTree.printTree(cerr);

            attachmentNode = removePertinentTree(pqTree, pertinentRoot);

//cerr << "\n\nAfter removePertinentTree Att:[" << pqTree.toNodeRef(attachmentNode).mNodeNum << "]\n";
//pqTree.printTree(cerr);

        }

        if (index < stOrderCopy.size() - 1) {

            // Fan-out.
            pqTree.fanOutLeavesFromAttachment(
                          pqTree.toNodeRef(attachmentNode), n.mOutgoingEdges);
//cerr << "\n\nAfter Fanout\n";
//pqTree.printTree(cerr);

        }

        index++;

    }

    makePlanarSubgraphAndRemovedEdges(g, graphCopy, planarSubgraph, removedEdges);

}


void JTSPlanarizer::copyInputGraph(
    Graph&                  src,
    vector<node_list_it_t>& stOrderSrc,
    JTSGraph&               dst,
    vector<node_list_it_t>& stOrderDst,
    JTSTree&                pqTree
) {

    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    stOrderDst.clear();
    for (size_t st = 0; st < stOrderSrc.size() ; st++) {

        node_list_it_t tmpIt;
        stOrderDst.push_back(tmpIt);

    }

    size_t stNum = 0;
    for (auto nItIt = stOrderSrc.begin(); nItIt != stOrderSrc.end(); nItIt++) {

        auto np = make_unique<JTSGraphNode>();
        np->pushIGBackwardLink(*nItIt);
        np->mStNumber = stNum++;
        nodePairs.push_back(make_pair(*nItIt,std::move(np)));

    }

    auto eitPair = src.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {

        auto ep = make_unique<JTSGraphEdge>();
        ep->pushIGBackwardLink(eit);
        ep->mRemoved  = false;
        edgePairs.push_back(make_pair(eit,std::move(ep)));

    }

    src.copySubgraph(nodePairs, edgePairs, dst);

    for (auto nit = dst.nodes().first; nit != dst.nodes().second; nit++) {

        auto& n = dynamic_cast<JTSGraphNode&>(*(*nit));
        stOrderDst[n.mStNumber] = nit;
        for (auto iit = n.incidentEdges().first;
                                      iit != n.incidentEdges().second; iit++) {
            auto& e = dynamic_cast<JTSGraphEdge&>(*(*(*iit)));
            auto& a = dynamic_cast<JTSGraphNode&>(e.adjacentNode(n));
            if (n.mStNumber < a.mStNumber) {
                n.mOutgoingEdges.push_back(e.backIt());
            }
            else {
                n.mIncomingEdges.push_back(e.backIt());
            }
        }
    }

    dst.mOriginal = &src;
}


vector<node_list_it_t> JTSPlanarizer::fromGraphEdgesToPQLeaves(
    JTSTree&              pqTree,
    list<edge_list_it_t>& edgeList
) {

    vector<node_list_it_t> leaves;

    for(auto eit : edgeList) {

        auto& e = dynamic_cast<JTSGraphEdge&>(*(*eit));

        if (!e.mRemoved) {

            leaves.push_back(e.mPQLeaf);
        }
    }

    return leaves; // rvo
}


node_list_it_t JTSPlanarizer::bubbleUp(
    JTSTree&                pqTree,
    vector<node_list_it_t>& pertinentLeaves
) {

    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;
    node_list_it_t       pertinentAncestor;

    for (auto leafIt : pertinentLeaves) {

        pqTree.toNodeRef(leafIt).resetForBubbleUp();

        Q.push_back(leafIt);

    }

    size_t leavesAccountedFor = pertinentLeaves.size();
    while (Q.size() > 0) {

        auto& X  = pqTree.toNodeRef(*(Q.begin()));

        if (X.mPertinentChildrenCount > 0) {

            leavesAccountedFor -= (X.mPertinentChildrenCount - 1);

        }

        Q.pop_front();

        if (Q.size() == 0 && leavesAccountedFor == 1) {

            break;
        }

        queueParentIfNecessaryBubbleUp(Q, pqTree, X);

        X.incrementPertinentChildrenCountOfParent();

    }

    return pqTree.findPertinentRoot(*(pertinentLeaves.begin()));
}


void JTSPlanarizer::findWHA(
    JTSTree&                pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t          pertinentRoot
) {
    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;

    for (auto leafIt : pertinentLeaves) {

        Q.push_back(leafIt);

    }

    while (Q.size() > 0) {

        auto& X = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();
        if(X.backIt() != pertinentRoot) {
            queueParentIfNecessaryCountingUp(Q, pqTree, X);
        }

        X.calculateWHAC();

    }
}


void JTSPlanarizer::determineWHABCNodeTypes(
    JTSTree&       pqTree,
    node_list_it_t pertinentRoot
) {

    auto& P = pqTree.toNodeRef(pertinentRoot);
    P.determineRootNodeType();

    list<node_list_it_t> Q;    // FIFO queue of the pertinent nodes.

    Q.push_back(pertinentRoot);
    while (Q.size() > 0) {

        auto& X  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();
        X.determineWHABCTypeForChildren();
        for (auto cIt : X.mChildren) {
            auto& C = pqTree.toNodeRef(cIt);
            if (C.isPertinent()) {
                Q.push_back(cIt);
            }
        }
    }
}


void JTSPlanarizer::removeNonconsecutiveNodes(
    JTSTree&                pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t&         pertinentRoot
) {

    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;

    vector<edge_list_it_t> graphEdges;

    for (auto leafIt : pertinentLeaves) {
        auto& L = pqTree.toNodeRef(leafIt);
        Q.push_back(leafIt);
        graphEdges.push_back(L.mGraphEdge);
    }

    while (Q.size() > 0) {
        auto& X  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();
        if(X.backIt() != pertinentRoot) {
            queueParentIfNecessaryCountingDown(Q, pqTree, X);
        }
        pqTree.removePertinentNodeIfNecessary(X);
    }

    for (auto eIt : graphEdges) {

        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eIt));

        if (!E.mRemoved) {

            pertinentRoot = pqTree.findPertinentRoot(E.mPQLeaf);
            break;

        }

    }

}


node_list_it_t JTSPlanarizer::applyTemplates(
    JTSTree&                pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t          pertinentRoot
) {

    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;

    node_list_it_t pr = pertinentRoot;

    for (auto leafIt : pertinentLeaves) {
        Q.push_back(leafIt);
    }

    while (Q.size() > 0) {

        auto& X  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();

        if (X.backIt() != pertinentRoot) {

            queueParentIfNecessaryCountingUp(Q, pqTree, X);

            if (!pqTree.templateL1(X))
            if (!pqTree.templateP1(X))
            if (!pqTree.templateP3(X))
            if (!pqTree.templateP5(X))
            if (!pqTree.templateP7(X))
            if (!pqTree.templateP8(X))
            if (!pqTree.templateQ1(X))
            if (!pqTree.templateQ2(X))
            if (!pqTree.templateQ4(X))
            pqTree.templateQ5(X);
        }
        else {
            // Root.
            if (!pqTree.templateL1(X))
            if (!pqTree.templateP1(X))
            if (!pqTree.templateP2(X, pr))
            if (!pqTree.templateP4(X, pr))
            if (!pqTree.templateP6(X, pr))
            if (!pqTree.templateP8(X))
            if (!pqTree.templateQ1(X))
            if (!pqTree.templateQ2(X))
            if (!pqTree.templateQ3(X))
            if (!pqTree.templateQ4(X))
            pqTree.templateQ5(X);
        }
    }

    return pr;
}


node_list_it_t JTSPlanarizer::removePertinentTree(
    JTSTree&       pqTree,
    node_list_it_t pertinentRoot
) {

    auto& R  = pqTree.toNodeRef(pertinentRoot);

    if (R.mNodeType==JTSTreeNode::LType) {

        // Turning this node to P attachment.
        pqTree.resetNodeToPAttachment(R);
        return R.backIt();
    }
    else if (R.mPertinentType == JTSTreeNode::CDPartial) {

        // From the tree root down to the lowest CD partial root 
        // remove all the nodes.
        
        while (true) {

            auto& C = pqTree.toNodeRef(pertinentRoot);

            if (pqTree.isNil(C.mCDPartialChild)) {

                for (auto fIt = C.mChildren.begin(); 
                                             fIt != C.mChildren.end();) {

                    auto& GC = pqTree.toNodeRef(*fIt);

                    fIt++;
                    if (GC.isFull()) {

                        C.mChildren.erase(GC.mChildIt);
                        GC.mParent = pqTree.nil();
                        pqTree.removeNodeAndDescendants(GC);
                    }
                    else {

                        break;
                    }
                }

                // We can't use reverse iterator as erase() uses the forward
                // iterator which breaks the integrity of reverse iterator.
                auto fIt = C.mChildren.end();
                fIt--;
                while (fIt != C.mChildren.begin()) {


                    auto& GC = pqTree.toNodeRef(*fIt);

                    fIt--;

                    if (GC.isFull()) {

                        C.mChildren.erase(GC.mChildIt);
                        GC.mParent = pqTree.nil();
                        pqTree.removeNodeAndDescendants(GC);
                    }
                    else {

                        break;

                    }
                }

                return  pqTree.makePAttachmentBefore(C, C.mChildren.end());

            }
            else {

                auto& PC = pqTree.toNodeRef(C.mCDPartialChild);
                pertinentRoot = C.mCDPartialChild;
                C.mChildren.erase(PC.mChildIt);
                PC.mParent = pqTree.nil();
                pqTree.removeNodeAndDescendants(C);
            }
        }

        return pqTree.nil(); // Not reachable.

    }
    else {

        // There are multiple full pertinent children under it.
        // Remove children.

        list<node_list_it_t>::iterator newPbefore = R.mChildren.end();
        bool inPertinent = false;

        for(auto cIt = R.mChildren.begin(); cIt != R.mChildren.end();) {

            auto& C = pqTree.toNodeRef(*cIt);

            cIt++;

            if (inPertinent) {

                if (C.isFull()) {

                    pqTree.removeNodeAndDescendants(C);

                }
                else {

                    newPbefore = cIt;
                    newPbefore--;

                    break;
                }
            }
            else {

                if (C.isFull()) {

                    inPertinent = true;

                    pqTree.removeNodeAndDescendants(C);


                }
            }
        }

        if (R.mChildren.size()==0) {

            // Make R the new attachment.
            pqTree.resetNodeToPAttachment(R);
            return R.backIt();
        }
        else {

            // Make a new P node and insert it before newPbefore.
            return  pqTree.makePAttachmentBefore(R, newPbefore);
        }
    }
}


void JTSPlanarizer::queueParentIfNecessaryBubbleUp(
    list<node_list_it_t>& Q,
    JTSTree&              pqTree,
    JTSTreeNode&          X
) {
    if (!(pqTree.isNil(X.mParent))){
        auto& P = pqTree.toNodeRef(X.mParent);
        if (!P.isPertinent()) {
            P.resetForBubbleUp();
            Q.push_back(X.mParent);
        }
    }
}


void JTSPlanarizer::queueParentIfNecessaryCountingUp(
    list<node_list_it_t>& Q,
    JTSTree&              pqTree,
    JTSTreeNode&          X
) {

    if (!(pqTree.isNil(X.mParent))){
        auto& P = pqTree.toNodeRef(X.mParent);
        P.mPertinentChildrenCountProcessed++;
        if(P.mPertinentChildrenCountProcessed==P.mPertinentChildrenCount) {
            Q.push_back(X.mParent);
        }
    }
}


void JTSPlanarizer::queueParentIfNecessaryCountingDown(
    list<node_list_it_t>& Q,
    JTSTree&              pqTree,
    JTSTreeNode&          X
) {
    if (!(pqTree.isNil(X.mParent))){
        auto& P = pqTree.toNodeRef(X.mParent);
        P.mPertinentChildrenCountProcessed--;
        if(P.mPertinentChildrenCountProcessed==0) {
            Q.push_back(X.mParent);
        }
    }
}


void JTSPlanarizer::makePlanarSubgraphAndRemovedEdges(
    Graph&                  originalGraph,
    JTSGraph&               internalGraph,
    PlanarizableGraph&      planarSubgraph,
    vector<edge_list_it_t>& removedEdges
) {

    vector<node_list_it_t> nodeList;
    vector<edge_list_it_t> edgeList;

    auto nitPair = internalGraph.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto& N = dynamic_cast<JTSGraphNode&>(*(*nit));
        nodeList.push_back(N.IGBackwardLink());
    }

    auto eitPair = internalGraph.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eit));
        if (E.mRemoved) {
            removedEdges.push_back(E.IGBackwardLink());
            E.IGBackwardLinkRef().pushUtility(kForwardTypeRemoved);
            E.IGBackwardLinkRef().pushIGForwardLink(
                                              planarSubgraph.edges().second);
        }
        else {
            edgeList.push_back(E.IGBackwardLink());
            E.IGBackwardLinkRef().pushUtility(kForwardTypeInPlanarSubgraph);
        }
    }

    planarSubgraph.createFrom(originalGraph, nodeList, edgeList);

}


void JTSPlanarizer::handlek0tok4(
    Graph&                  originalGraph,
    PlanarizableGraph&      planarSubgraph
) {

    vector<node_list_it_t> nodeList;
    vector<edge_list_it_t> edgeList;

    auto nitPair = originalGraph.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        nodeList.push_back(nit);
    }

    auto eitPair = originalGraph.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto& E = dynamic_cast<Edge&>(*(*eit));
        E.pushUtility(kForwardTypeInPlanarSubgraph);
        edgeList.push_back(eit);
    }

    planarSubgraph.createFrom(originalGraph, nodeList, edgeList);

}

}// namespace Undirected

}// namespace Wailea

