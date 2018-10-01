#include "undirected/bl_planarity_tester.hpp"
#include "undirected/bl_tree.hpp"
#include "undirected/bl_tree_node.hpp"
#include "undirected/bl_inline.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/bl_planarity_tester.cpp
 *
 * @brief It performs Booth & Lueker's planarity test with PQ-tree.
 */
namespace Wailea {

namespace Undirected {

using namespace std;


bool BLPlanarityTester::isPlanar(
    Graph&                  g,
    vector<node_list_it_t>& stOrder
) {

    // Up to K3. Already planar.
    if (g.numNodes() <= 3) {
        return true;
    }

    BLGraph                graphCopy;
    vector<node_list_it_t> stOrderCopy;

    copyInputGraph(g, stOrder, graphCopy, stOrderCopy);

    BLTree         pqTree;
    node_list_it_t attachmentNode = pqTree.makePAttachment();

    size_t index = 0;

    for (auto nit : stOrderCopy ) {
        auto& N = dynamic_cast<BLGraphNode&>(*(*nit));

        if (index > 0) {

            pqTree.initializeForOneIteration();

            vector<node_list_it_t> pertinentLeaves =
                                    fromGraphEdgesToPQLeaves(N.mIncomingEdges);

            node_list_it_t pertinentRoot;
            node_list_it_t virtualRoot =pqTree.nil();

            bool result = bubbleUp(pqTree, pertinentLeaves, virtualRoot);

            if (!result) {
                cleanUpInternalData();
                return false; // BubbleUp failed. Non-planar.
            }

            result = applyTemplates(pqTree, pertinentLeaves, pertinentRoot);

            if (!result) {                                   
                cleanUpInternalData();
                return false; // None of the templates matches. Non-planar.
            }

            attachmentNode = removePertinentTree(pqTree, pertinentRoot);

            if (!pqTree.isNil(virtualRoot)) {

                pqTree.removeVirtualRoot(virtualRoot);

            }

        }

        if (index < stOrderCopy.size() - 1) {

            pqTree.fanOutLeavesFromAttachment(attachmentNode,N.mOutgoingEdges);

        }

        index++;

    }

    cleanUpInternalData();

    return true;
}


bool BLPlanarityTester::isPlanarOldAlg(
    Graph&                  g,
    vector<node_list_it_t>& stOrder,
    size_t&                 index
) {

    BLGraph                graphCopy;
    vector<node_list_it_t> stOrderCopy;

    copyInputGraph(g, stOrder, graphCopy, stOrderCopy);

    BLTree         pqTree;
    node_list_it_t attachmentNode = pqTree.makePAttachment();

    index = 0;
    for (auto nit : stOrderCopy ) {
        auto& N = dynamic_cast<BLGraphNode&>(*(*nit));
        if (index > 0) {
            pqTree.initializeForOneIteration();
            vector<node_list_it_t> pertinentLeaves =
                                    fromGraphEdgesToPQLeaves(N.mIncomingEdges);
            node_list_it_t pertinentRoot;
            node_list_it_t virtualRoot =pqTree.nil();
            bool result = bubbleUp(pqTree, pertinentLeaves, virtualRoot);
            if (!result) {
                cleanUpInternalData();
                return false; // BubbleUp failed. Non-planar.
            }
            result = applyTemplatesOldAlg(
                                       pqTree, pertinentLeaves, pertinentRoot);
            if (!result) {                                   
                cleanUpInternalData();
                return false; // None of the templates matches. Non-planar.
            }
            attachmentNode = removePertinentTree(pqTree, pertinentRoot);
            if (!pqTree.isNil(virtualRoot)) {
                pqTree.removeVirtualRoot(virtualRoot);
            }
        }
        if (index < stOrderCopy.size() - 1) {
            pqTree.fanOutLeavesFromAttachment(attachmentNode,N.mOutgoingEdges);
        }
        index++;
    }
    cleanUpInternalData();
    return true;
}


bool BLPlanarityTester::findEmbedding(
    Graph&                  g,
    vector<node_list_it_t>& stOrder
) {

    // Up to K3. Already planar.
    if (g.numNodes() <= 3) {
        return true;
    }
    BLGraph                graphCopy;
    vector<node_list_it_t> stOrderCopy;
    copyInputGraph(g, stOrder, graphCopy, stOrderCopy);
    bool result;
    result = findEmbeddingFirstPass(graphCopy, stOrderCopy);
    if (!result) {
        cleanUpInternalData();
        return false;
    }
    result = findEmbeddingSecondPass(graphCopy, stOrderCopy);
    if (!result) {
        cleanUpInternalData();
        return false;
    }
    rearrangeIncidentEdges(g, graphCopy);
    cleanUpInternalData();
    return true;

}


/** @brief performns PQ-tree reductions from s to t, and find
 *         orientation-agnostic incoming edge ordering for each node.
 */
bool BLPlanarityTester::findEmbeddingFirstPass(
    BLGraph&                graphCopy,
    vector<node_list_it_t>& stOrderCopy
) {
#ifdef UNIT_TESTS2
    cerr << "\n\n\n==================================================\n";
    cerr <<       "=========          FIRST PASS          ===========\n";
    cerr <<       "==================================================\n";
#endif
    BLTree                 pqTree;
    pqTree.setCollectingEdges();
    node_list_it_t attachmentNode = pqTree.makePAttachment();
    size_t index = 0;

    for (auto nit : stOrderCopy ) {
        auto& N = dynamic_cast<BLGraphNode&>(*(*nit));
#ifdef UNIT_TESTS2
        {     
            cerr << "\n\n\n==================================================\n";
            cerr <<       "=========        Iteration: " << index + 1 << "        ===========\n";
            cerr <<       "==================================================\n";
            auto& GN = dynamic_cast<NumNode&>(N.IGBackwardLinkRef());

            cerr << "[" << GN.num() << "] In:[";
            for (auto eit : N.mIncomingEdges) {
                auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
                auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(N));
                auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
            cerr << GA.num() << ",";
            }
            cerr << "] Out:[";
            for (auto eit : N.mOutgoingEdges) {
                auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
                auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(N));
                auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
                cerr << GA.num() << ",";
            }
            cerr << "]\n";
        }
#endif
        if (index > 0) {
            pqTree.initializeForOneIteration();
            vector<node_list_it_t> pertinentLeaves =
                                    fromGraphEdgesToPQLeaves(N.mIncomingEdges);

            node_list_it_t pertinentRoot;
            node_list_it_t virtualRoot = pqTree.nil();

            bool result = bubbleUp(pqTree, pertinentLeaves, virtualRoot);
            if (!result) {
                return false;
            }
#ifdef UNIT_TESTS2
            cerr << "\nAfter bubbleUp\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
            result = applyTemplates(pqTree, pertinentLeaves, pertinentRoot);
            if (!result) {                                   
                return false;
            }
#ifdef UNIT_TESTS2
            cerr << "\nAfter applyTemplates\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
            collectEdgeOrdering(pqTree, pertinentRoot, N, INCOMING);
            attachmentNode = removePertinentTree(pqTree, pertinentRoot);
            if (!pqTree.isNil(virtualRoot)) {
                pqTree.removeVirtualRoot(virtualRoot);
            }
#ifdef UNIT_TESTS2
            cerr << "\nAfter removePertinentRoot\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
        }
        if (index < stOrderCopy.size() - 1) {
            pqTree.fanOutLeavesFromAttachment(attachmentNode,N.mOutgoingEdges);
#ifdef UNIT_TESTS2
            cerr << "\nAfter Fanning out\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
        }

#ifdef UNIT_TESTS2
        {     
            cerr << "Collected Edges: ";
            auto& GN = dynamic_cast<NumNode&>(N.IGBackwardLinkRef());

            cerr << "[" << GN.num() << "] In:[";
            for (auto eit : N.mIncomingEdgesOrdered) {
                auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
                auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(N));
                auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
                cerr << GA.num() << ",";
            }
            cerr << "]\n";
        }
#endif
        index++;
    }
    return true;
}


/** @brief performns PQ-tree reductions from t to s, and find
 *         an oriented outgoing edge ordering and an orientation for the 
 *         incoming edge ordering per node.
 */
bool BLPlanarityTester::findEmbeddingSecondPass(
    BLGraph&                graphCopy,
    vector<node_list_it_t>& stOrderCopy
) {
#ifdef UNIT_TESTS2
    cerr << "\n\n\n==================================================\n";
    cerr <<       "=========          SECOND PASS         ===========\n";
    cerr <<       "==================================================\n";
#endif
    BLTree                 pqTree;
    pqTree.setCollectingEdges();
    // In the second pass, it keeps track of the orientation of all the Q-nodes

    pqTree.trackQFlippings();
    node_list_it_t attachmentNode = pqTree.makePAttachment();
    size_t index = 0;

    for (auto nItIt=stOrderCopy.rbegin(); nItIt!=stOrderCopy.rend(); nItIt++) {
        auto& N = dynamic_cast<BLGraphNode&>(*(*(*(nItIt))));
#ifdef UNIT_TESTS2
        {     
            cerr << "\n\n\n================================================\n";
            cerr <<       "=========        Iteration: " << index + 1 
                 << "        ===========\n";
            cerr <<       "================================================\n";

            auto& GN = dynamic_cast<NumNode&>(N.IGBackwardLinkRef());

            cerr << "[" << GN.num() << "] InOrdered:[";
            for (auto eit : N.mIncomingEdgesOrdered) {
                auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
                auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(N));
                auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
            cerr << GA.num() << ",";
            }
            cerr << "] Out:[";
            for (auto eit : N.mOutgoingEdges) {
                auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
                auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(N));
                auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
                cerr << GA.num() << ",";
            }
            cerr << "]\n";
        }
#endif

        if (index > 0) {
            pqTree.initializeForOneIteration();
            vector<node_list_it_t> pertinentLeaves =
                                    fromGraphEdgesToPQLeaves(N.mOutgoingEdges);
            node_list_it_t pertinentRoot;
            node_list_it_t virtualRoot = pqTree.nil();
            bool result = bubbleUp(pqTree, pertinentLeaves, virtualRoot);
            if (!result) {
                return false;
            }
#ifdef UNIT_TESTS2
            cerr << "\nAfter bubbleUp\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
            result = applyTemplates(pqTree, pertinentLeaves, pertinentRoot);
            if (!result) {                                   
                return false;
            }
#ifdef UNIT_TESTS2
            cerr << "\nAfter applyTemplates\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
            collectEdgeOrdering(pqTree, pertinentRoot, N, OUTGOING);
            attachmentNode = removePertinentTree(pqTree, pertinentRoot);
            if (!pqTree.isNil(virtualRoot)) {
                pqTree.removeVirtualRoot(virtualRoot);
            }
#ifdef UNIT_TESTS2
            cerr << "\nAfter removePertinentRoot\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
        }
        if (index < stOrderCopy.size() - 1) {

            pqTree.fanOutLeavesFromQAttachment(
                            attachmentNode,N.mIncomingEdgesOrdered,N.backIt());
#ifdef UNIT_TESTS2
            cerr << "\nAfter Fanning out\n";
            pqTree.printTree(cerr, pqTree.nil());
            cerr << "\n";
#endif
        }
        index++;


#ifdef UNIT_TESTS2
        {     
            cerr << "Collected Edges: ";
            auto& GN = dynamic_cast<NumNode&>(N.IGBackwardLinkRef());

            cerr << "[" << GN.num() << "] Out:[";
            for (auto eit : N.mOutgoingEdgesOrdered) {
                auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
                auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(N));
                auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
                cerr << GA.num() << ",";
            }
            cerr << "]\n";
        }
#endif
    }
    // At this point, pqTree consists of a single node of Q type.


#ifdef UNIT_TESTS2

    cerr << "\nFinal Tree\n";
    cerr << "num nodes: " << pqTree.numNodes() << "\n";
    pqTree.printTree(cerr, pqTree.nil());
    cerr << "\n";
#endif

    /*
     * Transfer all the orientation information to the lists in 
     * BLPlanarityTestser object.
     */
    auto& Q = pqTree.toNodeRef(pqTree.nodes().first);

    if (!pqTree.isNil(Q.mParent)) {
        cerr << "!!!ERROR Q's not root!!!\n";
    }

    auto& Q2 = pqTree.toNodeRef(pqTree.findRoot());

    if (Q.backIt()!=Q2.backIt()) {
        cerr << "!!!ERROR Q different from Q2!!!\n";
    }

    mOrientInNorm.clear();
    mOrientInReversed.clear();
    mOrientOutNorm.clear();
    mOrientOutReversed.clear();

    for (auto nit : Q.mOrientInNorm) {
        mOrientInNorm.push_back(nit);
    }
    for (auto nit : Q.mOrientInReversed) {
        mOrientInReversed.push_back(nit);
    }
    for (auto nit : Q.mOrientOutNorm) {
        mOrientOutNorm.push_back(nit);
    }
    for (auto nit : Q.mOrientOutReversed) {
        mOrientOutReversed.push_back(nit);
    }



#ifdef UNIT_TESTS2
    cerr << "mOrientInNorm: ";
    for (auto a : mOrientInNorm) {
        auto& BN = dynamic_cast<BLGraphNode&>(*(*a));
        auto& GN = dynamic_cast<NumNode&>(BN.IGBackwardLinkRef());
        cerr << GN.num() << ", ";
    }
    cerr << "\n";
    cerr << "mOrientInReversed: ";
    for (auto a : mOrientInReversed) {
        auto& BN = dynamic_cast<BLGraphNode&>(*(*a));
        auto& GN = dynamic_cast<NumNode&>(BN.IGBackwardLinkRef());
        cerr << GN.num() << ", ";
    }
    cerr << "\n";

    cerr << "mOrientOutNorm: ";
    for (auto a : mOrientOutNorm) {
        auto& BN = dynamic_cast<BLGraphNode&>(*(*a));
        auto& GN = dynamic_cast<NumNode&>(BN.IGBackwardLinkRef());
        cerr << GN.num() << ", ";
    }
    cerr << "\n";
    cerr << "mOrientOutReversed: ";
    for (auto a : mOrientOutReversed) {
        auto& BN = dynamic_cast<BLGraphNode&>(*(*a));
        auto& GN = dynamic_cast<NumNode&>(BN.IGBackwardLinkRef());
        cerr << GN.num() << ", ";
    }
    cerr << "\n";
#endif                    
    return true;
}


void BLPlanarityTester::rearrangeIncidentEdges(
    Graph&    graph,
    BLGraph&  graphCopy
) {
    for (auto nit : mOrientInReversed) {
        auto& N = dynamic_cast<BLGraphNode&>(*(*nit));
        N.mIncomingEdgesInReverse = true;
    }
    for (auto nit : mOrientOutReversed) {
        auto& N = dynamic_cast<BLGraphNode&>(*(*nit));
        N.mOutgoingEdgesInReverse = true;
    }
    for (auto nit = graphCopy.nodes().first;
                                      nit != graphCopy.nodes().second; nit++) {
        auto& NCopy = dynamic_cast<BLGraphNode&>(*(*nit));
        auto& NOrg  = dynamic_cast<Node&>(NCopy.IGBackwardLinkRef());
        vector<edge_list_it_t> rearrangedEdges;
        // Concatenate mIncomingEdgesOrdered and mOutgoingEdgesOrdered
        // into rearrangedEdges. Note that one of them must be in reversed 
        // order to make it a circular ordering.

        if (NCopy.mIncomingEdgesInReverse) {
            for (auto eit = NCopy.mIncomingEdgesOrdered.rbegin();
                           eit != NCopy.mIncomingEdgesOrdered.rend(); eit++) {
                auto& ECopy = dynamic_cast<BLGraphEdge&>(*(*(*(eit))));
                rearrangedEdges.push_back(ECopy.IGBackwardLink());
            }

        }
        else {        
            for (auto eit = NCopy.mIncomingEdgesOrdered.begin();
                           eit != NCopy.mIncomingEdgesOrdered.end(); eit++) {
                auto& ECopy = dynamic_cast<BLGraphEdge&>(*(*(*(eit))));
                rearrangedEdges.push_back(ECopy.IGBackwardLink());
            }
        }
        if (!NCopy.mOutgoingEdgesInReverse) {
            for (auto eit = NCopy.mOutgoingEdgesOrdered.rbegin();
                           eit != NCopy.mOutgoingEdgesOrdered.rend(); eit++) {
                auto& ECopy = dynamic_cast<BLGraphEdge&>(*(*(*(eit))));
                rearrangedEdges.push_back(ECopy.IGBackwardLink());
            }
        }
        else {        
            for (auto eit = NCopy.mOutgoingEdgesOrdered.begin();
                           eit != NCopy.mOutgoingEdgesOrdered.end(); eit++) {
                auto& ECopy = dynamic_cast<BLGraphEdge&>(*(*(*(eit))));
                rearrangedEdges.push_back(ECopy.IGBackwardLink());
            }
        }
        NOrg.reorderIncidence(std::move(rearrangedEdges));
    }
}


void BLPlanarityTester::copyInputGraph(
    Graph&                  src,
    vector<node_list_it_t>& stOrderSrc,
    BLGraph&                dst,
    vector<node_list_it_t>& stOrderDst
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

        auto np = make_unique<BLGraphNode>();
        np->pushIGBackwardLink(*nItIt);
        np->mStNumber = stNum++;
        nodePairs.push_back(make_pair(*nItIt,std::move(np)));

    }

    auto eitPair = src.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {

        auto ep = make_unique<BLGraphEdge>();
        ep->pushIGBackwardLink(eit);
        edgePairs.push_back(make_pair(eit,std::move(ep)));

    }

    src.copySubgraph(nodePairs, edgePairs, dst);

    for (auto nit = dst.nodes().first; nit != dst.nodes().second; nit++) {

        auto& n = dynamic_cast<BLGraphNode&>(*(*nit));
        stOrderDst[n.mStNumber] = nit;
        for (auto iit = n.incidentEdges().first;
                                      iit != n.incidentEdges().second; iit++) {
            auto& e = dynamic_cast<BLGraphEdge&>(*(*(*iit)));
            auto& a = dynamic_cast<BLGraphNode&>(e.adjacentNode(n));
            if (n.mStNumber < a.mStNumber) {
                n.mOutgoingEdges.push_back(e.backIt());
            }
            else {
                n.mIncomingEdges.push_back(e.backIt());
            }
            n.mIncomingEdgesInReverse = false;
            n.mOutgoingEdgesInReverse = false;
        }
    }

    dst.mOriginal = &src;

}


void BLPlanarityTester::cleanUpInternalData()
{
    mOrientInNorm.clear();
    mOrientInReversed.clear();
    mOrientOutNorm.clear();
    mOrientOutReversed.clear();
}


vector<node_list_it_t> BLPlanarityTester::fromGraphEdgesToPQLeaves(
    list<edge_list_it_t>& edgeList
) {
    vector<node_list_it_t> leaves;
    for(auto eit : edgeList) {
        auto& e = dynamic_cast<BLGraphEdge&>(*(*eit));
        //auto& GN1 = dynamic_cast<BLGraphNode&>(e.incidentNode1());
        //auto& GN2 = dynamic_cast<BLGraphNode&>(e.incidentNode2());
        //auto& N1 = dynamic_cast<NumNode&>(GN1.IGBackwardLinkRef());
        //auto& N2 = dynamic_cast<NumNode&>(GN2.IGBackwardLinkRef());
        //cerr << "Edge: [" << N1.num() << "," << N2.num() << "]\n";
        leaves.push_back(e.mPQLeaf);
    }
    return leaves; // rvo
}


/** @brief BubbleUp() of [BL76]
 *         - It identifies a super-set of pertinent subtree.
 *         - It sets the pertinent children count for each pertinent node.
 */
bool BLPlanarityTester::bubbleUp(
    BLTree&                 pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t&         virtualRoot
) {
    // State transition of the pertinent nodes
    // 
    //   1. "NonPertinent"       : mGeneration  < pqTree.mGeneration
    //          || 
    //         _||_  [Queued & resetForBubbleUp()]
    //         \  /
    //          \/
    //   2. "Pertinent & onQueue": mGeneration == pqTree.mGeneration
    //          ||                 mBlocked    == false
    //         _||_  [Dequeued]    mUnblocked  == false
    //         \  / 
    //          \/   
    //   3. "Blocked"            : mGeneration == pqTree.mGeneration
    //          ||                 mBlocked    == true
    //         _||_  [unblock]     mUnblocked  == false
    //         \  /
    //          \/
    //   4. "Unblocked"          : mGeneration == pqTree.mGeneration
    //                             mBlocked    == false
    //                             mUnblocked  == true

    // There is only one pertinent leaf.
    // It is the pertinent root by itself.
    // This is to avoid generating unnecessary virtual root below.
    if (pertinentLeaves.size() == 1) {
        auto& X = pqTree.toNodeRef(*(pertinentLeaves.begin()));
        X.resetForBubbleUp();
        virtualRoot = pqTree.nil();
        return true;
    }
    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;
    size_t               Qsize = 0;
    for (auto leafIt : pertinentLeaves) {
        pqTree.toNodeRef(leafIt).resetForBubbleUp();
        Q.push_back(leafIt);
        Qsize++;
    }
    virtualRoot = pqTree.nil();
    // Number of blocks of consecutive children whose Q-node parents
    // have not been found.
    size_t blockCount   = 0;

    // One indicates the tree root has been found and processed, and it
    // coincides with the pertinent root.
    size_t offTheTop    = 0;

    // Keeps all the pertinent nodes that have been taken off the queue.
    // Used to find blocked nodes for virtual root.
    list<node_list_it_t> offQ;
    while (Qsize + blockCount + offTheTop > 1) {
        auto& X  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();
        Qsize--;
        offQ.push_back(X.backIt());
        tryUnblockingPertinentNodes(pqTree, X, blockCount, Q, Qsize, offTheTop);
        if (Q.size() == 0) {
            if (blockCount == 1 && offTheTop == 0) {
                // All the pertinent nodes have been discovered but
                // the consecutive pertinent children of the pertinent root
                // have not discovered its parent.
                // Make a virtual parent for the orphans.

                virtualRoot = makeVirtualRootAndPutOrphansUnder(pqTree, offQ);
                blockCount  = 0;  
            }
            else if (blockCount > 1 || (blockCount == 1 && offTheTop == 1)) {
                // The pertinent leaves can't be arranged consecutively in
                // one of the following conditions.
                // 1. If there are multiple blocks
                // 2. If the tree root has been found, but there is still a 
                //    blocked node.

                virtualRoot = pqTree.nil();
                return false;
            }
        }
    }
    return true;
}


void BLPlanarityTester::queueParentIfNecessaryBubbleUp(
    list<node_list_it_t>& Q,
    size_t&               Qsize,
    BLTree&               pqTree, 
    node_list_it_t        parentIt,
    size_t&               offTheTop
) {
    // This function assumes parentIt points to 
    // a valid node or nil().

    if (pqTree.isNil(parentIt)) {
        // X is the root of the PQ-tree.
        offTheTop = 1;
    }
    else {
        auto& P = pqTree.toNodeRef(parentIt);
        if (!(P.hasBeenQueuedForBubbleUp())) {
            P.resetForBubbleUp();
            Q.push_back(parentIt);
            Qsize++;
        }
    }
}


void BLPlanarityTester::tryUnblockingPertinentNodes(
    BLTree&               pqTree,
    BLTreeNode&           X,
    size_t&               blockCount,
    list<node_list_it_t>& Q,
    size_t&               Qsize,
    size_t&               offTheTop
) {

    if (!pqTree.isNil(X.mSibling1)) {
        // X has sibling 1.
        auto& sib1 = pqTree.toNodeRef(X.mSibling1);

        if (!pqTree.isNil(X.mSibling2)) {
            // X has both sibling 1 & 2.
            // X does not know its parent.
            auto& sib2 = pqTree.toNodeRef(X.mSibling2);
            
            if (sib1.isUnblocked()) {

                // X has sibling1 unblocked.
                X.unblock(sib1.mParent);
                // try to unlbock the side of sibling2.
                unblockSiblings(pqTree, X, X.mSibling2, blockCount);

            }
            else if (sib2.isUnblocked()) {

                // X has sibling2 unblocked.
                X.unblock(sib2.mParent);
                // try to unblock the side of sibling1.
                unblockSiblings(pqTree, X, X.mSibling1, blockCount);

            }
            else {

                // X has no unblocked immediate sibling. It is blocked.
                X.block();

                if (sib1.isBlocked() && sib2.isBlocked()){

                    // Both sib 1 & 2 blocked. Concatenating those two to make
                    // a single longer block.
                    blockCount--;

                }
                else if (((!sib1.isPertinent())||sib1.isOnQueue()) &&
                         ((!sib2.isPertinent())||sib2.isOnQueue())   ){

                    // Either sib 1 or 2 have not been explored.
                    // the current block itself is a block.
                    blockCount++;

                }
            }
        }
        else {
            // X has sibling1 only. X already knows its parent.
            queueParentIfNecessaryBubbleUp(Q, Qsize, pqTree, X.mParent, offTheTop);
            X.unblock();

            // try to unblock the siblings.
            unblockSiblings(pqTree, X, X.mSibling1, blockCount);
        }
    }
    else if (!pqTree.isNil(X.mSibling2)) {
        // X has sibling 2 only. X already knows its parent.
        queueParentIfNecessaryBubbleUp(Q, Qsize, pqTree, X.mParent, offTheTop);
        X.unblock();

        // try to unblock the siblings.
        unblockSiblings(pqTree, X, X.mSibling2, blockCount);
    }
    else {
        // X has no sibling. It's a child of a P-node.
        queueParentIfNecessaryBubbleUp(Q, Qsize, pqTree, X.mParent, offTheTop);
        X.unblock();
    }

}


void BLPlanarityTester::unblockSiblings(
    BLTree&        pqTree,
    BLTreeNode&    X,
    node_list_it_t sibIt,
    size_t&        blockCount
) {
    auto  prevIt = X.backIt();
    auto  curIt  = sibIt;

    // From the immediate sibling of X onward, unblock the consecutive
    // blocked siblings.
    bool  blockRemoved = false;

    while (!pqTree.isNil(curIt)) {

        auto& C = pqTree.toNodeRef(curIt);

        if (C.isBlocked()) {
            C.unblock(X.mParent); 
            blockRemoved = true;
        }
        else {
            break;
        }

        pqTree.advanceSib(prevIt, curIt);
    }

    if (blockRemoved) {
        blockCount--;
    }
}


/** @brief creates a temporary virtual root for the pertinent orphans,
 *         and have them points to the virtual root.
 *
 *  @param pqTree  (in): PQ-tree 
 *
 *  @param offQ    (in): list of pertinent nodes expored in bubbleUp().
 *                       It contains both blocked and unblocked nodes.
 *
 *  @return   pointer to the virtual root node.
 *
 *  @remarks  The virtual node does not know it children, but all the
 *            blocked children now point to it.
 */
node_list_it_t BLPlanarityTester::makeVirtualRootAndPutOrphansUnder(
    BLTree&               pqTree,
    list<node_list_it_t>& offQ
) {

    // Scan through offQ and find a blocked node.
    node_list_it_t xIt = pqTree.nil();
    for (auto nIt : offQ) {
        auto& N = pqTree.toNodeRef(nIt);
        if (N.isBlocked()) {
            xIt = nIt;
            break;
        }
    }

    // X is a blocked node.
    auto& X =  pqTree.toNodeRef(xIt);

    // Make a virtual root.
    auto  vpp  = make_unique<BLTreeNode>(pqTree);
    auto& VP   = dynamic_cast<BLTreeNode&>(pqTree.addNode(std::move(vpp)));

    VP.mNodeType = BLTreeNode::VirtualRootType;
    VP.mGeneration = pqTree.mGeneration;

    // Explore the sibling chain from X in both directions.

    auto  prevIt = X.mSibling1;
    auto  curIt  = X.backIt();
    while (!pqTree.isNil(curIt)) {

        auto& C = pqTree.toNodeRef(curIt);
        if (C.isBlocked()) {

            C.unblock(VP.backIt()); 
        }
        else {

            break;
        }
        pqTree.advanceSib(prevIt, curIt);
    }
    prevIt = X.backIt();
    curIt  = X.mSibling1;

    while (!pqTree.isNil(curIt)) {

        auto& C = pqTree.toNodeRef(curIt);
        if (C.isBlocked()) {

            C.unblock(VP.backIt()); 
        }
        else {

            break;
        }
        pqTree.advanceSib(prevIt, curIt);
    }

    return VP.backIt();
}


/** @brief performs REDUCE in [BL76].
 *         - It arranges the pertinent leaves or non-pertinent leaves
 *           consecutively by appliying series of templates.
 *         - It finds the pertinent root.
 *         - It classifies the pertinent nodes into 4 types:
 *               Full
 *               Singly partial
 *               Doubly partial
 *               Complementarily doubly partial (CDPartial)
 *         - The parent is dequeued only after all of its children are
 *           dequeued and processed.
 *         - The pertinent root is found by tracking the number of 
 *           pertinent descendants in mPertinentLeavesCount.
 *           The node whose mPertinentLeavesCount matches the number of
 *           pertinent leaves first is the pertinent root.
 *         It also finds the lowest Complementarily doubly partial node
 *         if the non-pertinent leaves, instead of pertinent leaves are
 *         consecutively arranged.
 */
bool BLPlanarityTester::applyTemplates(
    BLTree&                 pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t&         pertinentRoot
) {
    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;
    size_t               Qsize = 0;
    for (auto leafIt : pertinentLeaves) {
        auto& L  = pqTree.toNodeRef(leafIt);
        L.mPertinentLeavesCount = 1;
        Q.push_back(leafIt);
        Qsize++;
    }
    while (Qsize > 0) {
        auto& X  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();
        Qsize--;
        if (X.mPertinentLeavesCount < pertinentLeaves.size()) {
            // X is not the pertinent root.
            if (!pqTree.isNil(X.mParent)) {
                auto& P = pqTree.toNodeRef(X.mParent);
                P.mPertinentLeavesCount += X.mPertinentLeavesCount;
            }
            queueParentIfNecessaryApplyTemplates(Q, Qsize, pqTree, X);
            /** @remarks on use of earlyOut
             *           earlyOut is used to detect non-planarity condition
             *           at an early stage. If a template finds more than two
             *           singly partial children, more than one complementarily
             *           doubly partial children under one parent, neither 
             *           pertinent leaves or non-pertinent leaves can be 
             *           arranged consecutively. In this case, the template
             *           returns true to avoid trying other remaining templates
             *           but instead sets earlyOut true to notify 
             *           applyTemplates() of the violating condition so that
             *           it can terminate the reduction and returns false.
             */                 

            bool earlyOut = false;
            if (!pqTree.templateL1(X, BLTree::NOT_FINAL_REDUCTION))
            if (!pqTree.templateP1(X, BLTree::NOT_FINAL_REDUCTION))
            if (!pqTree.templateP3(X, earlyOut))
            if (!pqTree.templateP5(X, earlyOut))
            if (!pqTree.templateP7(X, earlyOut))
            if (!pqTree.templateP8(X, BLTree::NOT_FINAL_REDUCTION, earlyOut))
            if (!pqTree.templateQ1(X, BLTree::NOT_FINAL_REDUCTION))
            if (!pqTree.templateQ2(X, BLTree::NOT_FINAL_REDUCTION, earlyOut))
            if (!pqTree.templateQ4(X, BLTree::NOT_FINAL_REDUCTION, earlyOut))
            if (!pqTree.templateQ5(X, BLTree::NOT_FINAL_REDUCTION, earlyOut)) {
                return false;
            }
            if (earlyOut) {
                return false;
            }
        }
        else {
            // X is the pertinent root.
            // P4 or P6 may change the pertinent root below.
            pertinentRoot = X.backIt();
            bool dummyEarlyOut = false;

            if (!pqTree.templateL1(X, BLTree::FINAL_REDUCTION))
            if (!pqTree.templateP1(X, BLTree::FINAL_REDUCTION))
            if (!pqTree.templateP2(X, pertinentRoot))
            if (!pqTree.templateP4(X, pertinentRoot))
            if (!pqTree.templateP6(X, pertinentRoot))
            if (!pqTree.templateP8(X, BLTree::FINAL_REDUCTION, dummyEarlyOut))
            if (!pqTree.templateQ1(X, BLTree::FINAL_REDUCTION))
            if (!pqTree.templateQ2(X, BLTree::FINAL_REDUCTION, dummyEarlyOut))
            if (!pqTree.templateQ3(X))
            if (!pqTree.templateQ4(X, BLTree::FINAL_REDUCTION, dummyEarlyOut))
            if (!pqTree.templateQ5(X, BLTree::FINAL_REDUCTION, dummyEarlyOut))
            {
                return false;
            }
        }
    }
    return true;
}


bool BLPlanarityTester::applyTemplatesOldAlg(
    BLTree&                 pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t&         pertinentRoot
) {

    // FIFO queue of the pertinent nodes.
    list<node_list_it_t> Q;
    size_t               Qsize = 0;
    for (auto leafIt : pertinentLeaves) {

        auto& L  = pqTree.toNodeRef(leafIt);
        L.mPertinentLeavesCount = 1;
        Q.push_back(leafIt);
        Qsize++;
    }

    while (Qsize > 0) {

        auto& X  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();
        Qsize--;
        if (X.mPertinentLeavesCount < pertinentLeaves.size()) {
            // X is not the pertinent root.

            if (!pqTree.isNil(X.mParent)) {

                auto& P = pqTree.toNodeRef(X.mParent);
                P.mPertinentLeavesCount += X.mPertinentLeavesCount;
            }

            queueParentIfNecessaryApplyTemplates(Q, Qsize, pqTree, X);

            /** @remarks on use of earlyOut
             *           earlyOut is used to detect non-planarity condition
             *           at an early stage. If a template finds more than two
             *           singly partial children, more than one complementarily
             *           doubly partial children under one parent, neither 
             *           pertinent leaves or non-pertinent leaves can be 
             *           arranged consecutively. In this case, the template
             *           returns true to avoid trying other remaining templates
             *           but instead sets earlyOut true to notify 
             *           applyTemplates() of the violating condition so that
             *           it can terminate the reduction and returns false.
             */                 

            bool earlyOut = false;
            if (!pqTree.templateL1(X, BLTree::NOT_FINAL_REDUCTION))
            if (!pqTree.templateP1(X, BLTree::NOT_FINAL_REDUCTION))
            if (!pqTree.templateP3(X, earlyOut))
            if (!pqTree.templateP5(X, earlyOut))
            if (!pqTree.templateQ1(X, BLTree::NOT_FINAL_REDUCTION))
            if (!pqTree.templateQ2(X, BLTree::NOT_FINAL_REDUCTION, earlyOut)) {
                return false;
            }

            if (earlyOut) {
                return false;
            }
        }
        else {

            // X is the pertinent root.
            // P4 or P6 may change the pertinent root below.
            pertinentRoot = X.backIt();

            bool dummyEarlyOut = false;

            if (!pqTree.templateL1(X, BLTree::FINAL_REDUCTION))
            if (!pqTree.templateP1(X, BLTree::FINAL_REDUCTION))
            if (!pqTree.templateP2(X, pertinentRoot))
            if (!pqTree.templateP4(X, pertinentRoot))
            if (!pqTree.templateP6(X, pertinentRoot))
            if (!pqTree.templateQ1(X, BLTree::FINAL_REDUCTION))
            if (!pqTree.templateQ2(X, BLTree::FINAL_REDUCTION, dummyEarlyOut))
            if (!pqTree.templateQ3(X))
            {
                return false;
            }

        }

    }

    return true;
}



void BLPlanarityTester::queueParentIfNecessaryApplyTemplates(
    list<node_list_it_t>& Q,
    size_t&               Qsize,
    BLTree&               pqTree,
    BLTreeNode&           X
) {
    /** If the node has a parent, and the latter's children have been all 
     * dequeued, then put the parent to the queue.
     */

    if (!(pqTree.isNil(X.mParent))){

        auto& P = pqTree.toNodeRef(X.mParent);

        P.mPertinentChildrenCountProcessed++;

        if(P.mPertinentChildrenCountProcessed == P.mPertinentChildrenCount) {

            Q.push_back(X.mParent);
            Qsize++;

        }
    }
}


/** @brief remove the pertinent subtree from PQ-tree and returns the
 *         attachment node from which new leaves are fanned out.
 *
 *  @param  pqTree        (in): PQ-tree
 *
 *  @param  pertinentRoot (in): Pertinent root
 *
 *  @return attachment node
 */
node_list_it_t BLPlanarityTester::removePertinentTree(
    BLTree&               pqTree,
    node_list_it_t        pertinentRoot
) {

    /* This is a dispatching function to forward the call 
     * depending on  the node type.
     */

    auto& R  = pqTree.toNodeRef(pertinentRoot);

    if (!pqTree.isNil(pqTree.CDPartialRoot())) {

        return removePertinentTreeCDPartial(pqTree, R);

    }
    else if (R.nodeType()==BLTreeNode::LType) {

        R.resetToPAttachment();

        return R.backIt();

    }
    else if (R.nodeType()==BLTreeNode::PType) {

        return removePertinentTreePType(pqTree, R);

    }
    else if (R.nodeType()==BLTreeNode::QType) {

        return removePertinentTreeQType(pqTree, R);

    }
    else { //(nodeType()==BLTreeNode::VirtualRootType)

        return removePertinentTreeQTypeMiddle(pqTree, R);

    }

}


node_list_it_t BLPlanarityTester::removePertinentTreeCDPartial(
    BLTree&      pqTree,
    BLTreeNode&  treeRoot
)
{

    list<node_list_it_t> topLevelNodes;
    /* Starting from the tree root down to the lowest CDPartial node,
     * Decompose the pertinent tree into full trees of depth 2.
     *
     *        CD1                   CD1     CD2         CD(n-1)
     *    / /  |  \ \      =>      // \\   // \\   ... // \\
     *   F F  CD2  F F           F F F F F F F F     F F F F
     *    / /  |  \ \            
     *   F F  CD3  F F
     *         .
     *         .
     *       CDPR(Q) <-Lowest(pqTree.CDPartialRoot())
     *  _______|_____       newTreeRoot
     * | | | | | | | |        
     * F F F E E E F F
     */
    auto layerIt = treeRoot.backIt();
    while (!pqTree.isNil(layerIt)) {
        auto& L = pqTree.toNodeRef(layerIt);
        if (pqTree.isNil(L.mCDPartialChild)) {

            break;
        }

        auto& CDP = pqTree.toNodeRef(L.mCDPartialChild);

        topLevelNodes.push_back(layerIt);
        layerIt = L.mCDPartialChild;
        
        if (L.nodeType() == BLTreeNode::QType) {

            CDP.unlinkFromQTypeParent(layerIt);
        }
        else if (L.nodeType() == BLTreeNode::PType) {

            CDP.unlinkFromPTypeParent();
        }

    }
    auto& newTreeRoot = pqTree.toNodeRef(pqTree.CDPartialRoot());

    if (pqTree.isTrackingQFlippings()) {

        newTreeRoot.mOrientInNorm.splice(
            newTreeRoot.mOrientInNorm.end(), 
            treeRoot.mOrientInNorm               );

        newTreeRoot.mOrientInReversed.splice(
            newTreeRoot.mOrientInReversed.end(),
            treeRoot.mOrientInReversed           );

        newTreeRoot.mOrientOutNorm.splice(
            newTreeRoot.mOrientOutNorm.end(), 
            treeRoot.mOrientOutNorm              );

        newTreeRoot.mOrientOutReversed.splice(
            newTreeRoot.mOrientOutReversed.end(),
            treeRoot.mOrientOutReversed          );

    }

    // Remove the full children from End1.
    auto prevIt  = pqTree.nil();
    auto curIt   = newTreeRoot.mEndChild1;


    while (!pqTree.isNil(curIt)) {

        auto& C = pqTree.toNodeRef(curIt);
        if (!C.isFull()) {
            break;
        }
        topLevelNodes.push_back(curIt);
        pqTree.advanceSib(prevIt, curIt);
    }

    auto& emptyEnd1 = pqTree.toNodeRef(curIt);

    pqTree.unlinkSiblings(emptyEnd1, pqTree.toNodeRef(prevIt)) ;

    emptyEnd1.mParent = newTreeRoot.backIt();

    newTreeRoot.mEndChild1 = curIt;

    // Remove the full children from End2.
    prevIt = pqTree.nil();
    curIt  = newTreeRoot.mEndChild2;

    while (!pqTree.isNil(curIt)) {
        auto& C = pqTree.toNodeRef(curIt);
        if (!C.isFull()) {
            break;
        }
        topLevelNodes.push_back(curIt);
        pqTree.advanceSib(prevIt, curIt);
    }

    auto& emptyEnd2 = pqTree.toNodeRef(curIt);
    bool linkOnSib2;
    if (emptyEnd2.mSibling2 == prevIt) {
        linkOnSib2 = true;
    }
    else {
        linkOnSib2 = false;
    }
    pqTree.unlinkSiblings(emptyEnd2, pqTree.toNodeRef(prevIt)) ;

    /* At this point there may be only one (empty) child left under
     * newTreeRoot. It is OK to leave it temporarily as a new attachment
     * node will be added under it below.
     *
     *   newTreeRoot(Q)
     *          |
     *     End1_|_End2
     *      |       
     *      E - E - E(emptyEnd2)
     */


    // Remove the pertinent nodes from the tree root.
    removePertinentNodesAndDescendants(pqTree, topLevelNodes);

    // Make a new attachment.
    auto  aIt = pqTree.makePAttachment();
    auto& A   = pqTree.toNodeRef(aIt);

    // Preserving the natural direction of CEmpty when linking.
    if (linkOnSib2) {
        emptyEnd2.mSibling2 = A.backIt();
    }
    else {
        emptyEnd2.mSibling1 = A.backIt();
    }
    A.mSibling1 = emptyEnd2.backIt();
    A.mParent = newTreeRoot.backIt();
    newTreeRoot.mEndChild2 = aIt;

    /*
     *   newTreeRoot(Q)
     *          |
     *     End1_|_____End2
     *      |           |
     *      E - E - E - A
     */


    return aIt;
}


node_list_it_t BLPlanarityTester::removePertinentTreeQTypeMiddle(
    BLTree&               pqTree,
    BLTreeNode&           R
) {
    /*
     *            Real Root(Uknown)          Real Root
     *              |                            | 
     * EndX_________|__________EndY  =>   EndX___|____EndY
     *  |               R(VR)   |          |           |
     *  |               |       |          |           |
     *  E - E - F - F - F - F - E          E - E - A - E
     *              
     * 
     */

    /*
     * Starting from FullChild, explore the sibling chain and 
     * find the boundary nodes.
     */
    auto& StartNode = pqTree.toNodeRef(*(R.mFullChildren.rbegin()));
    auto curIt  = StartNode.backIt();
    auto prevIt = StartNode.mSibling1;

    while (!pqTree.isNil(curIt)) {
        auto& C = pqTree.toNodeRef(curIt);
        if (!C.isFull()) {
            break;
        }
        pqTree.advanceSib(prevIt, curIt);
    }

    auto& EmptyEnd1 = pqTree.toNodeRef(curIt); 
    auto& FullEnd1  = pqTree.toNodeRef(prevIt);
    bool EmptyEnd1LinkOnSib1;
    if (EmptyEnd1.mSibling1 == FullEnd1.backIt()) {
        EmptyEnd1LinkOnSib1 = true;
    }
    else {
        EmptyEnd1LinkOnSib1 = false;
    }

    std::swap(prevIt, curIt);

    list<node_list_it_t> topLevelNodes;

    while (!pqTree.isNil(curIt)) {
        auto& C = pqTree.toNodeRef(curIt);
        if (!C.isFull()) {
            break;
        }
        topLevelNodes.push_back(curIt);
        pqTree.advanceSib(prevIt, curIt);
    }

    auto& EmptyEnd2 = pqTree.toNodeRef(curIt); 
    auto& FullEnd2  = pqTree.toNodeRef(prevIt);
    bool EmptyEnd2LinkOnSib1;
    if (EmptyEnd2.mSibling1 == FullEnd2.backIt()) {
        EmptyEnd2LinkOnSib1 = true;
    }
    else {
        EmptyEnd2LinkOnSib1 = false;
    }

    pqTree.unlinkSiblings(EmptyEnd1, FullEnd1);
    pqTree.unlinkSiblings(EmptyEnd2, FullEnd2);

    auto  aIt = pqTree.makePAttachment();
    auto& A   = pqTree.toNodeRef(aIt);

    if (EmptyEnd1LinkOnSib1) {
        EmptyEnd1.mSibling1 = A.backIt();
    }
    else {
        EmptyEnd1.mSibling2 = A.backIt();
    }
    A.mSibling1 = EmptyEnd1.backIt();

    if (EmptyEnd2LinkOnSib1) {
        EmptyEnd2.mSibling1 = A.backIt();
    }
    else {
        EmptyEnd2.mSibling2 = A.backIt();
    }
    A.mSibling2 = EmptyEnd2.backIt();

    removePertinentNodesAndDescendants(pqTree, topLevelNodes);

    return aIt;
}


node_list_it_t BLPlanarityTester::removePertinentTreeQType(
    BLTree&               pqTree,
    BLTreeNode&           R
) {

    auto prevIt =  pqTree.nil();
    node_list_it_t curIt;
    bool           End1Full;

    if (pqTree.toNodeRef(R.mEndChild1).isFull()) {
        curIt  = R.mEndChild1;
        End1Full = true;
    }
    else if (pqTree.toNodeRef(R.mEndChild2).isFull()) {
        curIt  = R.mEndChild2;
        End1Full = false;
    }
    else {
        // Pertinent nodes are in the middle of sibling chain.
        return removePertinentTreeQTypeMiddle(pqTree, R);
    }
    list<node_list_it_t> topLevelNodes;

    while (!pqTree.isNil(curIt)) {

        auto& C = pqTree.toNodeRef(curIt);

        if (!C.isFull()) {
            break;
        }

        topLevelNodes.push_back(curIt);
        pqTree.advanceSib(prevIt, curIt);

    }

    if (curIt == pqTree.nil()) {

        /* All the children are full make R the new attachment.
         *              R                            A
         * EndX_________|__________EndY  => 
         *  |                       |
         *  F - F - F - F - F - F - F
         */
        removePertinentNodesAndDescendants(pqTree, topLevelNodes);
        R.resetToPAttachment();
        return R.backIt();
    
    }
    else {

        /* If R is singly partial:
         *
         *              R                            R
         * EndX_________|__________EndY  =>   EndX___|____EndY
         *  |                       |          |           |
         *  E - E - E - F - F - F - F          E - E - E - A
         */
        auto& CFull  = pqTree.toNodeRef(prevIt);
        auto& CEmpty = pqTree.toNodeRef(curIt);

        bool linkOnSib2;
        if (CEmpty.mSibling2 == prevIt) {
            linkOnSib2 = true;
        }
        else {
            linkOnSib2 = false;
        }

        pqTree.unlinkSiblings(CFull, CEmpty);

        auto& Att = pqTree.toNodeRef(pqTree.makePAttachment()); 
        // Preserving the natural direction of CEmpty when linking.
        if (linkOnSib2) {
            CEmpty.mSibling2 = Att.backIt();
        }
        else {
            CEmpty.mSibling1 = Att.backIt();
        }
        Att.mSibling1 = CEmpty.backIt();

        if (End1Full) {
            R.mEndChild1 = Att.backIt();
        }
        else {
            R.mEndChild2 = Att.backIt();
        }
        Att.mParent = R.backIt();

        removePertinentNodesAndDescendants(pqTree, topLevelNodes);

        return Att.backIt();
    }

}


node_list_it_t BLPlanarityTester::removePertinentTreePType(
    BLTree&               pqTree,
    BLTreeNode&           R
) {

    // PType. All the children are full.
    removePertinentNodesAndDescendants(pqTree, R.mChildren);
    R.resetToPAttachment();
    return R.backIt();

}


void BLPlanarityTester::removePertinentNodesAndDescendants(
    BLTree&               pqTree,
    list<node_list_it_t>& topLevelNodes
) {

    list<node_list_it_t> Q(topLevelNodes);

    while (Q.size() > 0) {

        auto& C  = pqTree.toNodeRef(*(Q.begin()));
        Q.pop_front();

        C.discardOldFullLink();
        C.clearFullChildren();

        if (C.nodeType() == BLTreeNode::PType) {

            Q.insert(Q.end(),C.mChildren.begin(), C.mChildren.end());
        }
        else if (C.nodeType() == BLTreeNode::QType) {
            auto prevIt = pqTree.nil();
            auto curIt  = C.mEndChild1;
            while (!pqTree.isNil(curIt)) {

                Q.push_back(curIt);
                pqTree.advanceSib(prevIt, curIt);
            }

        }

        pqTree.removeNode(C);
    }
}


/** @brief explores the pertinent tree in which the pertinent leaves are 
 *         (circularly) consecutively arranged in pre-traversal DFS, and 
 *         collects the graph edges in the natural order.
 *         It is assumed that all the pertinent nodes have correct pertinent
 *         type.
 */
void BLPlanarityTester::collectEdgeOrdering(
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        edgeCollectionType    collectionType
) {

    /* This is a dispatching function to forward the call 
     * depending on  the node type.
     */
    auto& R  = pqTree.toNodeRef(pertinentRoot);    
    list<edge_list_it_t>& edgeList = (collectionType==INCOMING)?
                                         graphNode.mIncomingEdgesOrdered:
                                         graphNode.mOutgoingEdgesOrdered;
    edgeList.clear();
    for (auto eit : R.mCollectedEdgesSide2) {
        edgeList.push_back(eit);
    }
    R.mCollectedEdgesSide2.clear();
    for (auto eit : R.mCollectedEdges) {
        edgeList.push_back(eit);
    }
    R.mCollectedEdges.clear();
    if (pqTree.isTrackingQFlippings()) {
        if ( R.nodeType() == BLTreeNode::VirtualRootType ) { 
            auto& NextSib = pqTree.toNodeRef(pqTree.mVirtualNextSib);
            if (pqTree.mVirtualNextSibAssumeReversed) {
                NextSib.mAssumedOrientOutReversed.push_back(graphNode.backIt());
            }
            else {
                NextSib.mAssumedOrientOutNorm.push_back(graphNode.backIt());
            }
        }
        else {
            // The orientation of the edges collected will be tracked
            // in this root Q-node.
            R.mOrientOutNorm.push_back(graphNode.backIt());
        }
    }
}


}// namespace Undirected

}// namespace Wailea

