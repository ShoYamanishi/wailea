#include "gtest/gtest.h"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/bl_tree.hpp"
#include "undirected/bl_tree_node.hpp"
#include "undirected/bl_inline.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/planar_dual_graph_maker.hpp"
#include "undirected/random_planar_graph_generator.hpp"
#include "undirected/st_numbering.hpp"


namespace Wailea {

namespace Undirected {

class BLPlanarityTesterTestsForEmbedding : public ::testing::Test {

  protected:

    BLPlanarityTesterTestsForEmbedding(){;};
    virtual ~BLPlanarityTesterTestsForEmbedding(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    inline void copyInputGraph(
        BLPlanarityTester&      tester,
        Graph&                  srcGraph,
        vector<node_list_it_t>& stOrderSrc,
        BLGraph&                dstGraph,
        vector<node_list_it_t>& stOrderDst
    ) { tester.copyInputGraph(srcGraph, stOrderSrc, dstGraph, stOrderDst);}

    inline vector<node_list_it_t> fromGraphEdgesToPQLeaves(
        BLPlanarityTester&   tester,
        list<edge_list_it_t>& edgeList
    ) {return tester.fromGraphEdgesToPQLeaves(edgeList); }

    inline bool bubbleUp(
        BLPlanarityTester&      tester,
        BLTree&                 pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         virtualRoot
    ) {return tester.bubbleUp(pqTree,pertinentLeaves,virtualRoot);}

    inline void queueParentIfNecessaryBubbleUp(
        BLPlanarityTester&    tester,
        list<node_list_it_t>& Q,
        size_t&               Qsize,
        BLTree&               pqTree,
        node_list_it_t        parentIt,
        size_t&               offTheTop
    ){tester.queueParentIfNecessaryBubbleUp(Q,Qsize,pqTree,parentIt,offTheTop);}

    inline void tryUnblockingPertinentNodes(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        BLTreeNode&           X,
        size_t&               blockCount,
        list<node_list_it_t>& Q,
        size_t&               Qsize,
        size_t&               offTheTop
    ){tester.tryUnblockingPertinentNodes(pqTree,X,blockCount,Q,Qsize, offTheTop);}

    inline void unblockSiblings(
        BLPlanarityTester& tester,
        BLTree&            pqTree,
        BLTreeNode&        X,
        node_list_it_t     sibIt,
        size_t&            blockCount
    ){tester.unblockSiblings(pqTree,X,sibIt,blockCount);}

    inline node_list_it_t makeVirtualRootAndPutOrphansUnder(
        BLPlanarityTester& tester,
        BLTree&               pqTree,
        list<node_list_it_t>& offQ
    ){ return  tester.makeVirtualRootAndPutOrphansUnder(pqTree,offQ);}

    inline bool applyTemplates(
        BLPlanarityTester&      tester,
        BLTree&                 pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         pertinentRoot
    ){return tester.applyTemplates(pqTree,pertinentLeaves,pertinentRoot);}

    inline void queueParentIfNecessaryApplyTemplates(
        BLPlanarityTester&    tester,
        list<node_list_it_t>& Q,
        size_t&               Qsize,
        BLTree&               pqTree,
        BLTreeNode&           X
    ){return tester.queueParentIfNecessaryApplyTemplates(Q, Qsize, pqTree, X);}

    inline node_list_it_t removePertinentTree(
        BLPlanarityTester& tester,
        BLTree&            pqTree,
        node_list_it_t     pertinentRoot
    ){return tester.removePertinentTree(pqTree,pertinentRoot);}

    inline node_list_it_t removePertinentTreeCDPartial(
        BLPlanarityTester& tester,
        BLTree&            pqTree,
        BLTreeNode&        treeRoot
    ){return tester.removePertinentTreeCDPartial(pqTree,treeRoot);}

    inline node_list_it_t removePertinentTreeQType(
        BLPlanarityTester& tester,
        BLTree&            pqTree,
        BLTreeNode&        R
    ){return tester.removePertinentTreeQType(pqTree,R);}

    inline node_list_it_t removePertinentTreePType(
        BLPlanarityTester& tester,
        BLTree&            pqTree,
        BLTreeNode&        pertinentRoot
    ){return tester.removePertinentTreePType(pqTree,pertinentRoot);}

    inline node_list_it_t removePertinentTreeQTypeMiddle(
        BLPlanarityTester& tester,
        BLTree&            pqTree,
        BLTreeNode&        pertinentRoot
    ){return tester.removePertinentTreeQTypeMiddle(pqTree, pertinentRoot);}

    inline void removePertinentNodesAndDescendants(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        list<node_list_it_t>& topLevelNodes
    ){tester.removePertinentNodesAndDescendants(pqTree,topLevelNodes);}

    inline bool findEmbeddingFirstPass(
        BLPlanarityTester&      tester,
        BLGraph&                graphCopy,
        vector<node_list_it_t>& stOrderCopy
    ){return tester.findEmbeddingFirstPass(graphCopy,stOrderCopy);}

    inline bool findEmbeddingSecondPass(
        BLPlanarityTester&      tester,
        BLGraph&                graphCopy,
        vector<node_list_it_t>& stOrderCopy
    ){return tester.findEmbeddingSecondPass(graphCopy,stOrderCopy);}

    inline void collectEdgeOrdering(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        BLPlanarityTester::edgeCollectionType    collectionType
    ){tester.collectEdgeOrdering(
                             pqTree,pertinentRoot,graphNode,collectionType);}
    inline void collectEdgeOrderingCDPartialRoot(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        node_list_it_t        treeRoot,
        BLGraphNode&          graphNode,
        BLPlanarityTester::edgeCollectionType    collectionType
    ){tester.collectEdgeOrderingCDPartialRoot(
                                    pqTree,treeRoot,graphNode,collectionType);}

    inline void collectEdgeOrderingQTypeMiddle(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        BLPlanarityTester::edgeCollectionType    collectionType
    ){tester.collectEdgeOrderingQTypeMiddle(
                               pqTree,pertinentRoot,graphNode,collectionType);}

    inline void collectEdgeOrderingQType(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        BLPlanarityTester::edgeCollectionType    collectionType
    ){tester.collectEdgeOrderingQType(
                               pqTree,pertinentRoot,graphNode,collectionType);}

    inline void collectEdgeOrderingPType(
        BLPlanarityTester&    tester,
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        BLPlanarityTester::edgeCollectionType    collectionType
    ){tester.collectEdgeOrderingPType(
                               pqTree,pertinentRoot,graphNode,collectionType);}


    inline void collectEdgeOrderingFromTopLevelNodes(
        BLPlanarityTester&        tester,
        BLTree&                   pqTree,
        list<node_list_it_t>&     topPertinentNodes,
        list<edge_list_it_t>&     edgeList,
        BLPlanarityTester::scanDirectionType         scanDirection
    ){tester.collectEdgeOrderingFromTopLevelNodes(
                             pqTree,topPertinentNodes,edgeList,scanDirection);}

    inline void rearrangeIncidentEdges(
        BLPlanarityTester& tester,
        Graph&             graph,
        BLGraph&           graphCopy
    ){tester.rearrangeIncidentEdges(graph,graphCopy);}

    inline void cleanUpInternalData(
        BLPlanarityTester& tester
    ){tester.cleanUpInternalData();}



    inline void fanOutLeavesFromDummyAttachment(
        BLTree&              tr,
        BLTreeNode&          A,
        list<edge_list_it_t>& edgeList
    ) {
        for(auto eIt : edgeList) {

            auto& E = dynamic_cast<BLGraphEdge&>(*(*eIt));

            auto  np  = make_unique<BLTreeNode>(tr);
            auto  nIt = tr.addNode(std::move(np)).backIt();
            auto& N   = tr.toNodeRef(nIt);

            N.mNodeType  = BLTreeNode::LType;
            N.mGraphEdge = eIt;
            E.mPQLeaf    = nIt;
            N.linkToPTypeParent(A);
        }
    }


    inline  bool checkTreeNodes
           (BLTree& tree, size_t numL, size_t numP, size_t numQ, size_t numVR)
    {
        size_t cntL   = 0;
        size_t cntP   = 0;
        size_t cntQ   = 0;
        size_t cntVR  = 0;
        for (auto nIt = tree.nodes().first; nIt != tree.nodes().second; nIt++){
            auto& N = tree.toNodeRef(nIt);
            if(N.mNodeType == BLTreeNode::LType) cntL++;
            if(N.mNodeType == BLTreeNode::PType) cntP++;
            if(N.mNodeType == BLTreeNode::QType) cntQ++;
            if(N.mNodeType == BLTreeNode::VirtualRootType) cntVR++;
        }

        //cerr << "TreeNodes:" << cntL << ":" << cntP << ":" << cntQ << ":"
        //     << cntVR << "\n" ;
        if (cntL==numL&&cntP==numP&&cntQ==numQ&&cntVR==numVR) {
            return true;
        }
        else {
            return false;
        }
    }


    inline Node& createGraphNode(Graph& G) {
        auto  np    = make_unique<Node>();
        return G.addNode(std::move(np));
    }


    inline bool checkUnorderedList(
        list<node_list_it_t>& test1,
        list<node_list_it_t>& test2
    ) {
        if (test1.size()!=test2.size()) {
            return false;
        }
        vector<int> counters;

        for (int i = 0;i < test1.size();i++) {
            counters.push_back(0);
        }

        size_t index;
        for (auto lit1 : test1) {
            index = 0;
            for (auto lit2 : test2) {
                if (*lit1 == *lit2) {
                    counters[index] = counters[index] + 1;
                }
                index++;
            }
        }
        for (auto c : counters) {
            if (c != 1) {
                return false;
            }
        }
        return true;
    }


    inline bool checkOrientation(
        BLTreeNode& Q, 
        list<node_list_it_t>& testOrientInNorm,
        list<node_list_it_t>& testOrientInReversed,
        list<node_list_it_t>& testOrientOutNorm,
        list<node_list_it_t>& testOrientOutReversed
    ) {
        return checkUnorderedList(Q.mOrientInNorm, testOrientInNorm) &&
            checkUnorderedList(Q.mOrientInReversed, testOrientInReversed) &&
            checkUnorderedList(Q.mOrientOutNorm, testOrientOutNorm) &&
            checkUnorderedList(Q.mOrientOutReversed, testOrientOutReversed);
    }


    inline bool checkAssumedOrientation(
        BLTreeNode& Q, 
        list<node_list_it_t>& testOrientInNorm,
        list<node_list_it_t>& testOrientInReversed,
        list<node_list_it_t>& testOrientOutNorm,
        list<node_list_it_t>& testOrientOutReversed
    ) {
        return checkUnorderedList(Q.mAssumedOrientInNorm, testOrientInNorm) &&
        checkUnorderedList(Q.mAssumedOrientInReversed, testOrientInReversed) &&
        checkUnorderedList(Q.mAssumedOrientOutNorm, testOrientOutNorm) &&
        checkUnorderedList(Q.mAssumedOrientOutReversed, testOrientOutReversed);
    }

    inline void createFullLink(node_list_it_t nIt, node_list_it_t pIt) {
        auto& N = dynamic_cast<BLTreeNode&>(*(*nIt));
        auto& P = dynamic_cast<BLTreeNode&>(*(*pIt));
        N.createFullLink(P);
    }

    inline bool isInFullChildren(node_list_it_t cIt, node_list_it_t nIt) {
                     
        auto& C = dynamic_cast<BLTreeNode&>(*(*cIt));
        auto& N = dynamic_cast<BLTreeNode&>(*(*nIt));

        for (auto it2 = N.mFullChildren.begin(); 
                                       it2 != N.mFullChildren.end(); it2++) {
            if (*it2 == cIt && it2 == C.mFullChildrenIt) {
                return true;
            }
        }
        return false;
    }

    inline void printNode(BLGraphNode& BN, ostream& os) {
        auto& GN = dynamic_cast<NumNode&>(BN.IGBackwardLinkRef());

        os << "[" << GN.num() << "] In:[";
        for (auto eit : BN.mIncomingEdgesOrdered) {
            auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
            auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(BN));
            auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
            os << GA.num() << ",";
        }
        os << "] Out:[";
        for (auto eit : BN.mOutgoingEdges) {
            auto& GE = dynamic_cast<BLGraphEdge&>(*(*eit));
            auto& BA = dynamic_cast<BLGraphNode&>(GE.adjacentNode(BN));
            auto& GA = dynamic_cast<NumNode&>(BA.IGBackwardLinkRef());
            os << GA.num() << ",";
        }
        os << "]\n";
    }

};


static bool cmp_edges_unordered(
    list<edge_list_it_t>& cmp1,
    list<edge_list_it_t>& cmp2
) {
    if( cmp1.size() != cmp2.size() ){
        return false;
    }
    vector<int> checkFlags;
    for (size_t i = 0; i < cmp1.size(); i++) {
        checkFlags.push_back(0);
    }
    size_t index = 0;
    for (auto c1 : cmp1) {
        for (auto c2 : cmp2) {
            if (c1==c2) {
                 checkFlags[index]++;
            }
        }
        index++;
    }
    for (auto f : checkFlags) {
        if (f != 1) {
            return false;
        }
    }

    return true;
}


static bool cross_check(
    BLTree&                pqTree,
    list<edge_list_it_t>&  edges,
    list<node_list_it_t>& treeLeaves
) {
    if (edges.size() != treeLeaves.size()) {
        return false;
    }

    vector<int> checkFlags;
    for (size_t i = 0; i < edges.size(); i++) {
        checkFlags.push_back(0);
    }
    size_t index = 0;
    for (auto eIt : edges) {
        auto& e = dynamic_cast<BLGraphEdge&>(*(*eIt));
        for (auto tlIt : treeLeaves) {
            auto& tl = dynamic_cast<BLTreeNode&>(*(*tlIt));
            if (e.mPQLeaf == tlIt && tl.mGraphEdge == eIt) {
                checkFlags[index]++;
            }
        }
        index++;
    }

    for (auto f : checkFlags) {
        if (f != 1) {
            return false;
        }
    }

    for (auto tlIt : treeLeaves) {
        auto& T = dynamic_cast<BLTreeNode&>(*(*tlIt));
        if ( (T.mNodeType == BLTreeNode::LType) &&
             (T.mChildren.size() == 0) &&
             (T.mChildrenCount == 0) &&
             (T.mEndChild1 == pqTree.nil()) &&
             (T.mEndChild2 == pqTree.nil()) &&
             (T.mSibling1 == pqTree.nil()) &&
             (T.mSibling2 == pqTree.nil()) &&
             (T.mGeneration == 0) &&
             (T.mUnblocked == false) &&
             (T.mPertinentChildrenCount == 0) &&
             (T.mPertinentChildrenCountProcessed == 0) &&
             (T.mPertinentType == BLTreeNode::PertinentUnknown) &&
             (T.mSinglyPartialChild1 == pqTree.nil()) &&
             (T.mSinglyPartialChild2 == pqTree.nil()) &&
             (T.mCDPartialChild == pqTree.nil()) &&
             (T.mFullChildren.size() == 0) ) {
            ;
        }
        else {
            return false;
        }
    }

    return true;
}



static bool cross_check(
    BLTree&                pqTree,
    list<edge_list_it_t>&  edges,
    vector<node_list_it_t>& treeLeaves
) {
    if (edges.size() != treeLeaves.size()) {
        return false;
    }

    vector<int> checkFlags;
    for (size_t i = 0; i < edges.size(); i++) {
        checkFlags.push_back(0);
    }
    size_t index = 0;
    for (auto eIt : edges) {
        auto& e = dynamic_cast<BLGraphEdge&>(*(*eIt));
        for (auto tlIt : treeLeaves) {
            auto& tl = dynamic_cast<BLTreeNode&>(*(*tlIt));
            if (e.mPQLeaf == tlIt && tl.mGraphEdge == eIt) {
                checkFlags[index]++;
            }
        }
        index++;
    }

    for (auto f : checkFlags) {
        if (f != 1) {
            return false;
        }
    }

    for (auto tlIt : treeLeaves) {
        auto& T = dynamic_cast<BLTreeNode&>(*(*tlIt));
        if ( (T.mNodeType == BLTreeNode::LType) &&
             (T.mChildren.size() == 0) &&
             (T.mChildrenCount == 0) &&
             (T.mEndChild1 == pqTree.nil()) &&
             (T.mEndChild2 == pqTree.nil()) &&
             (T.mSibling1 == pqTree.nil()) &&
             (T.mSibling2 == pqTree.nil()) &&
             (T.mGeneration == 0) &&
             (T.mUnblocked == false) &&
             (T.mPertinentChildrenCount == 0) &&
             (T.mPertinentChildrenCountProcessed == 0) &&
             (T.mPertinentType == BLTreeNode::PertinentUnknown) &&
             (T.mSinglyPartialChild1 == pqTree.nil()) &&
             (T.mSinglyPartialChild2 == pqTree.nil()) &&
             (T.mCDPartialChild == pqTree.nil()) &&
             (T.mFullChildren.size() == 0) ) {
            ;
        }
        else {
            return false;
        }
    }

    return true;
}


static bool parent_check(BLTreeNode& N)
{
    for (auto cIt = N.mChildren.begin(); cIt != N.mChildren.end(); cIt++) {
        auto& C = dynamic_cast<BLTreeNode&>(*(*(*cIt)));
        if (C.mChildIt != cIt) {
            return false;
        }
        if (C.mParent != N.backIt()){
            return false;
        }
    }
    return true;
}


static BLTreeNode& createP(BLTree& pqTree, list<node_list_it_t>& children)
{
    auto  np    = make_unique<BLTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& P     = pqTree.toNodeRef(nIt);
    P.mNodeType = BLTreeNode::PType;
    for (auto cIt : children) {
        auto& C = dynamic_cast<BLTreeNode&>(*(*cIt));
        C.mChildIt = P.mChildren.insert(P.mChildren.end(),cIt);
        P.mChildrenCount++;
        C.mParent = nIt;
    }
    return P;
}


static BLTreeNode& createQ(BLTree& pqTree, list<node_list_it_t>& children)
{
    auto  np    = make_unique<BLTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& Q     = pqTree.toNodeRef(nIt);
    Q.mNodeType = BLTreeNode::QType;
    size_t index = 0;
    node_list_it_t prevIt = pqTree.nil();
    if (children.size() == 1) {
        auto& C = dynamic_cast<BLTreeNode&>(*(*(*(children.begin()))));
        Q.mEndChild1 = C.backIt();
        Q.mEndChild2 = C.backIt();
        C.mParent    = Q.backIt();
    }
    else {
        for (auto cIt : children) {
            auto& C = dynamic_cast<BLTreeNode&>(*(*cIt));

            if (index == 0) {
                Q.mEndChild1 = cIt;
                C.mParent    = nIt;
            }
            else if (index == children.size() -1) {
                Q.mEndChild2 = cIt;
                C.mParent    = nIt;
            }
            C.mSibling1 = prevIt;
            if (index > 0) {
               auto& P = dynamic_cast<BLTreeNode&>(*(*prevIt));
                P.mSibling2 = cIt;
            }
            prevIt = cIt;
            index++;
        }
    }
    return Q;
}


static BLTreeNode& createQext(
    BLTree& pqTree,
    list<node_list_it_t>& children,
    list<bool>&           orientations)
{
    auto  np    = make_unique<BLTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& Q     = pqTree.toNodeRef(nIt);
    Q.mNodeType = BLTreeNode::QType;
    list<bool>::iterator prevOit;
    list<bool>::iterator curOit = orientations.begin();

    size_t index = 0;
    node_list_it_t prevIt = pqTree.nil();
    if (children.size() == 1) {

        auto& C = dynamic_cast<BLTreeNode&>(*(*(*(children.begin()))));
        Q.mEndChild1 = C.backIt();
        Q.mEndChild2 = C.backIt();
        C.mParent    = Q.backIt();

    }
    else {

        for (auto cIt : children) {
            auto& C = dynamic_cast<BLTreeNode&>(*(*cIt));

            if (index == 0) {
                Q.mEndChild1 = cIt;
                C.mParent    = nIt;
            }
            else if (index == children.size() -1) {
                Q.mEndChild2 = cIt;
                C.mParent    = nIt;
            }
            if (*curOit) {
                C.mSibling1 = prevIt;
            }
            else {
                C.mSibling2 = prevIt;
            }
            if (index > 0) {
                auto& P = dynamic_cast<BLTreeNode&>(*(*prevIt));
                if (*prevOit) {
                    P.mSibling2 = cIt;
                }
                else {
                    P.mSibling1 = cIt;
                }
            }
            prevIt = cIt;
            index++;

            prevOit = curOit;
            curOit++;
        }

    }
    return Q;
}


static BLTreeNode& createVR(BLTree& pqTree)
{
    auto  np    = make_unique<BLTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& VR    = pqTree.toNodeRef(nIt);
    VR.mNodeType = BLTreeNode::VirtualRootType;
    return VR;
}


static BLTreeNode& createL(BLTree& pqTree, BLGraph& G)
{
    auto  np    = make_unique<BLTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& L     = pqTree.toNodeRef(nIt);
    L.mNodeType = BLTreeNode::LType;

    auto  np1   = make_unique<BLGraphNode>();
    auto& n1    = G.addNode(std::move(np1));
    auto  np2   = make_unique<BLGraphNode>();
    auto& n2    = G.addNode(std::move(np2));
    auto  ep    = make_unique<BLGraphEdge>();
    auto& e     = dynamic_cast<BLGraphEdge&>(G.addEdge(std::move(ep),n1, n2));
    e.mPQLeaf   = L.backIt();
    L.mGraphEdge= e.backIt();

    return L;
}


//static BLTreeNode& createL(BLTree& pqTree, BLGraphEdge& e)
//{
//    auto  np    = make_unique<BLTreeNode>(pqTree);
//    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
//    auto& L     = pqTree.toNodeRef(nIt);
//    L.mNodeType = BLTreeNode::LType;
//    e.mPQLeaf   = L.backIt();
//    L.mGraphEdge= e.backIt();
//    return L;
//}


static BLGraphNode& createBLGraphNode(BLGraph& G)
{
    auto  np    = make_unique<BLGraphNode>();
    auto  nIt   = G.addNode(std::move(np)).backIt();
    return dynamic_cast<BLGraphNode&>(*(*nIt));;
}


static bool circular_check(
            Node& N, list<node_list_it_t>& L2, bool& reversed)
{

    if (N.degree() != L2.size()) {
        return false;
    }
    if (N.degree() == 0) {
        return false;
    }

    auto startL1 = N.incidentEdges().second;
    for (auto L1It = N.incidentEdges().first;
                                    L1It != N.incidentEdges().second; L1It++) {

        auto& L1E = *(*(*L1It));
        auto& Adj = dynamic_cast<Node&>(L1E.adjacentNode(N));
        if (*(L2.begin()) == Adj.backIt()) {
            startL1 = L1It;
            break;
        }
    }
    if (startL1 == N.incidentEdges().second) {
        return false;
    }

    bool try_reverse = false;

    auto L1It = startL1;
    list<node_list_it_t>::iterator L2It = L2.begin();
    for (auto i = 0; i < N.degree(); i++) {
        auto& L1E = *(*(*L1It));
        auto& Adj = L1E.adjacentNode(N);
        if (Adj.backIt() != *L2It) {
            try_reverse = true;
            break;
        }
        L1It++;
        L2It++;
        if (L1It == N.incidentEdges().second) {
            L1It = N.incidentEdges().first;
        }
    }

    if (!try_reverse) {
        reversed = false;
        return true;
    }

    startL1 = N.incidentEdges().second;
    for (auto L1It = N.incidentEdges().first;
                                    L1It != N.incidentEdges().second; L1It++) {
        auto& L1E = *(*(*L1It));
        auto& Adj = L1E.adjacentNode(N);
        if (*(L2.rbegin()) == Adj.backIt() ) {
            startL1 = L1It;
            break;
        }
    }
    if (startL1 == N.incidentEdges().second) {
        return false;
    }

    L1It = startL1;
    list<node_list_it_t>::reverse_iterator L2rIt = L2.rbegin();
    for (auto i = 0; i < N.degree(); i++) {
        auto& L1E = *(*(*L1It));
        auto& Adj = L1E.adjacentNode(N);
        if (Adj.backIt() != *L2rIt) {
            return false;
        }
        L1It++;
        L2rIt++;
        if (L1It == N.incidentEdges().second) {
            L1It = N.incidentEdges().first;
        }
    }
    reversed = true;
    return true;
}


static bool areImmediateSiblings(BLTreeNode& n1, BLTreeNode& n2)
{
    return (n1.mSibling1 == n2.backIt() || n1.mSibling2 == n2.backIt()) &&
           (n2.mSibling1 == n1.backIt() || n2.mSibling2 == n1.backIt());
}


// /**  @brief tests rearrangeIncidentEdges()
//  */
// TEST_F(BLPlanarityTesterTestsForEmbedding, TestRepro_001) {
// 
// 
//     Graph g_1;
// 
//     auto& n_000 = g_1.addNode(make_unique<NumNode>(  0 ));
//     auto& n_001 = g_1.addNode(make_unique<NumNode>(  1 ));
//     auto& n_002 = g_1.addNode(make_unique<NumNode>(  2 ));
//     auto& n_003 = g_1.addNode(make_unique<NumNode>(  3 ));
//     auto& n_004 = g_1.addNode(make_unique<NumNode>(  4 ));
//     auto& n_005 = g_1.addNode(make_unique<NumNode>(  5 ));
//     auto& n_006 = g_1.addNode(make_unique<NumNode>(  6 ));
//     auto& n_007 = g_1.addNode(make_unique<NumNode>(  7 ));
//     auto& n_008 = g_1.addNode(make_unique<NumNode>(  8 ));
//     auto& n_009 = g_1.addNode(make_unique<NumNode>(  9 ));
//     auto& n_010 = g_1.addNode(make_unique<NumNode>( 10 ));
//     auto& n_011 = g_1.addNode(make_unique<NumNode>( 11 ));
//     auto& n_012 = g_1.addNode(make_unique<NumNode>( 12 ));
//     auto& n_013 = g_1.addNode(make_unique<NumNode>( 13 ));
//     auto& n_014 = g_1.addNode(make_unique<NumNode>( 14 ));
//     auto& n_015 = g_1.addNode(make_unique<NumNode>( 15 ));
//     auto& n_016 = g_1.addNode(make_unique<NumNode>( 16 ));
//     auto& n_017 = g_1.addNode(make_unique<NumNode>( 17 ));
//     auto& n_018 = g_1.addNode(make_unique<NumNode>( 18 ));
//     auto& n_019 = g_1.addNode(make_unique<NumNode>( 19 ));
//     auto& n_020 = g_1.addNode(make_unique<NumNode>( 20 ));
//     auto& n_021 = g_1.addNode(make_unique<NumNode>( 21 ));
//     auto& n_022 = g_1.addNode(make_unique<NumNode>( 22 ));
//     auto& n_023 = g_1.addNode(make_unique<NumNode>( 23 ));
//     auto& n_024 = g_1.addNode(make_unique<NumNode>( 24 ));
//     auto& n_025 = g_1.addNode(make_unique<NumNode>( 25 ));
//     auto& n_026 = g_1.addNode(make_unique<NumNode>( 26 ));
//     auto& n_027 = g_1.addNode(make_unique<NumNode>( 27 ));
//     auto& n_028 = g_1.addNode(make_unique<NumNode>( 28 ));
//     auto& n_029 = g_1.addNode(make_unique<NumNode>( 29 ));
//     auto& n_030 = g_1.addNode(make_unique<NumNode>( 30 ));
//     auto& n_031 = g_1.addNode(make_unique<NumNode>( 31 ));
//     auto& n_032 = g_1.addNode(make_unique<NumNode>( 32 ));
//     auto& n_033 = g_1.addNode(make_unique<NumNode>( 33 ));
//     auto& n_034 = g_1.addNode(make_unique<NumNode>( 34 ));
//     auto& n_035 = g_1.addNode(make_unique<NumNode>( 35 ));
//     auto& n_036 = g_1.addNode(make_unique<NumNode>( 36 ));
//     auto& n_037 = g_1.addNode(make_unique<NumNode>( 37 ));
//     auto& n_038 = g_1.addNode(make_unique<NumNode>( 38 ));
//     auto& n_039 = g_1.addNode(make_unique<NumNode>( 39 ));
//     auto& n_040 = g_1.addNode(make_unique<NumNode>( 40 ));
//     auto& n_041 = g_1.addNode(make_unique<NumNode>( 41 ));
//     auto& n_042 = g_1.addNode(make_unique<NumNode>( 42 ));
//     auto& n_043 = g_1.addNode(make_unique<NumNode>( 43 ));
//     auto& n_044 = g_1.addNode(make_unique<NumNode>( 44 ));
//     auto& n_045 = g_1.addNode(make_unique<NumNode>( 45 ));
//     auto& n_046 = g_1.addNode(make_unique<NumNode>( 46 ));
//     auto& n_047 = g_1.addNode(make_unique<NumNode>( 47 ));
//     auto& n_048 = g_1.addNode(make_unique<NumNode>( 48 ));
//     auto& n_049 = g_1.addNode(make_unique<NumNode>( 49 ));
//     auto& n_050 = g_1.addNode(make_unique<NumNode>( 50 ));
//     auto& n_051 = g_1.addNode(make_unique<NumNode>( 51 ));
//     auto& n_052 = g_1.addNode(make_unique<NumNode>( 52 ));
//     auto& n_053 = g_1.addNode(make_unique<NumNode>( 53 ));
//     auto& n_054 = g_1.addNode(make_unique<NumNode>( 54 ));
//     auto& n_055 = g_1.addNode(make_unique<NumNode>( 55 ));
//     auto& n_056 = g_1.addNode(make_unique<NumNode>( 56 ));
//     auto& n_057 = g_1.addNode(make_unique<NumNode>( 57 ));
//     auto& n_058 = g_1.addNode(make_unique<NumNode>( 58 ));
//     auto& n_059 = g_1.addNode(make_unique<NumNode>( 59 ));
//     auto& n_060 = g_1.addNode(make_unique<NumNode>( 60 ));
//     auto& n_061 = g_1.addNode(make_unique<NumNode>( 61 ));
//     auto& n_062 = g_1.addNode(make_unique<NumNode>( 62 ));
//     auto& n_063 = g_1.addNode(make_unique<NumNode>( 63 ));
//     auto& n_064 = g_1.addNode(make_unique<NumNode>( 64 ));
//     auto& n_065 = g_1.addNode(make_unique<NumNode>( 65 ));
//     auto& n_066 = g_1.addNode(make_unique<NumNode>( 66 ));
//     auto& n_067 = g_1.addNode(make_unique<NumNode>( 67 ));
//     auto& n_068 = g_1.addNode(make_unique<NumNode>( 68 ));
//     auto& n_069 = g_1.addNode(make_unique<NumNode>( 69 ));
//     auto& n_070 = g_1.addNode(make_unique<NumNode>( 70 ));
//     auto& n_071 = g_1.addNode(make_unique<NumNode>( 71 ));
//     auto& n_072 = g_1.addNode(make_unique<NumNode>( 72 ));
//     auto& n_073 = g_1.addNode(make_unique<NumNode>( 73 ));
//     auto& n_074 = g_1.addNode(make_unique<NumNode>( 74 ));
//     auto& n_075 = g_1.addNode(make_unique<NumNode>( 75 ));
//     auto& n_076 = g_1.addNode(make_unique<NumNode>( 76 ));
//     auto& n_077 = g_1.addNode(make_unique<NumNode>( 77 ));
//     auto& n_078 = g_1.addNode(make_unique<NumNode>( 78 ));
//     auto& n_079 = g_1.addNode(make_unique<NumNode>( 79 ));
//     auto& n_080 = g_1.addNode(make_unique<NumNode>( 80 ));
//     auto& n_081 = g_1.addNode(make_unique<NumNode>( 81 ));
//     auto& n_082 = g_1.addNode(make_unique<NumNode>( 82 ));
//     auto& n_083 = g_1.addNode(make_unique<NumNode>( 83 ));
//     auto& n_084 = g_1.addNode(make_unique<NumNode>( 84 ));
//     auto& n_085 = g_1.addNode(make_unique<NumNode>( 85 ));
//     auto& n_086 = g_1.addNode(make_unique<NumNode>( 86 ));
//     auto& n_087 = g_1.addNode(make_unique<NumNode>( 87 ));
// 
//     auto& e_063_061 = g_1.addEdge(make_unique<Edge>(), n_063, n_061 );
//     auto& e_032_057 = g_1.addEdge(make_unique<Edge>(), n_032, n_057 );
//     auto& e_068_014 = g_1.addEdge(make_unique<Edge>(), n_068, n_014 );
//     auto& e_072_073 = g_1.addEdge(make_unique<Edge>(), n_072, n_073 );
//     auto& e_002_070 = g_1.addEdge(make_unique<Edge>(), n_002, n_070 );
//     auto& e_034_035 = g_1.addEdge(make_unique<Edge>(), n_034, n_035 );
//     auto& e_023_026 = g_1.addEdge(make_unique<Edge>(), n_023, n_026 );
//     auto& e_048_020 = g_1.addEdge(make_unique<Edge>(), n_048, n_020 );
//     auto& e_036_034 = g_1.addEdge(make_unique<Edge>(), n_036, n_034 );
//     auto& e_005_008 = g_1.addEdge(make_unique<Edge>(), n_005, n_008 );
//     auto& e_066_060 = g_1.addEdge(make_unique<Edge>(), n_066, n_060 );
//     auto& e_056_032 = g_1.addEdge(make_unique<Edge>(), n_056, n_032 );
//     auto& e_047_056 = g_1.addEdge(make_unique<Edge>(), n_047, n_056 );
//     auto& e_003_086 = g_1.addEdge(make_unique<Edge>(), n_003, n_086 );
//     auto& e_076_077 = g_1.addEdge(make_unique<Edge>(), n_076, n_077 );
//     auto& e_029_053 = g_1.addEdge(make_unique<Edge>(), n_029, n_053 );
//     auto& e_085_003 = g_1.addEdge(make_unique<Edge>(), n_085, n_003 );
//     auto& e_025_026 = g_1.addEdge(make_unique<Edge>(), n_025, n_026 );
//     auto& e_061_062 = g_1.addEdge(make_unique<Edge>(), n_061, n_062 );
//     auto& e_014_001 = g_1.addEdge(make_unique<Edge>(), n_014, n_001 );
//     auto& e_072_070 = g_1.addEdge(make_unique<Edge>(), n_072, n_070 );
//     auto& e_022_065 = g_1.addEdge(make_unique<Edge>(), n_022, n_065 );
//     auto& e_027_028 = g_1.addEdge(make_unique<Edge>(), n_027, n_028 );
//     auto& e_051_011 = g_1.addEdge(make_unique<Edge>(), n_051, n_011 );
//     auto& e_041_039 = g_1.addEdge(make_unique<Edge>(), n_041, n_039 );
//     auto& e_050_055 = g_1.addEdge(make_unique<Edge>(), n_050, n_055 );
//     auto& e_070_009 = g_1.addEdge(make_unique<Edge>(), n_070, n_009 );
//     auto& e_002_006 = g_1.addEdge(make_unique<Edge>(), n_002, n_006 );
//     auto& e_083_080 = g_1.addEdge(make_unique<Edge>(), n_083, n_080 );
//     auto& e_026_054 = g_1.addEdge(make_unique<Edge>(), n_026, n_054 );
//     auto& e_030_029 = g_1.addEdge(make_unique<Edge>(), n_030, n_029 );
//     auto& e_049_048 = g_1.addEdge(make_unique<Edge>(), n_049, n_048 );
//     auto& e_011_061 = g_1.addEdge(make_unique<Edge>(), n_011, n_061 );
//     auto& e_009_039 = g_1.addEdge(make_unique<Edge>(), n_009, n_039 );
//     auto& e_000_001 = g_1.addEdge(make_unique<Edge>(), n_000, n_001 );
//     auto& e_021_015 = g_1.addEdge(make_unique<Edge>(), n_021, n_015 );
//     auto& e_041_042 = g_1.addEdge(make_unique<Edge>(), n_041, n_042 );
//     auto& e_000_052 = g_1.addEdge(make_unique<Edge>(), n_000, n_052 );
//     auto& e_066_063 = g_1.addEdge(make_unique<Edge>(), n_066, n_063 );
//     auto& e_037_034 = g_1.addEdge(make_unique<Edge>(), n_037, n_034 );
//     auto& e_043_035 = g_1.addEdge(make_unique<Edge>(), n_043, n_035 );
//     auto& e_038_035 = g_1.addEdge(make_unique<Edge>(), n_038, n_035 );
//     auto& e_053_054 = g_1.addEdge(make_unique<Edge>(), n_053, n_054 );
//     auto& e_083_084 = g_1.addEdge(make_unique<Edge>(), n_083, n_084 );
//     auto& e_085_012 = g_1.addEdge(make_unique<Edge>(), n_085, n_012 );
//     auto& e_015_016 = g_1.addEdge(make_unique<Edge>(), n_015, n_016 );
//     auto& e_002_080 = g_1.addEdge(make_unique<Edge>(), n_002, n_080 );
//     auto& e_032_031 = g_1.addEdge(make_unique<Edge>(), n_032, n_031 );
//     auto& e_069_011 = g_1.addEdge(make_unique<Edge>(), n_069, n_011 );
//     auto& e_051_016 = g_1.addEdge(make_unique<Edge>(), n_051, n_016 );
//     auto& e_000_014 = g_1.addEdge(make_unique<Edge>(), n_000, n_014 );
//     auto& e_041_040 = g_1.addEdge(make_unique<Edge>(), n_041, n_040 );
//     auto& e_030_055 = g_1.addEdge(make_unique<Edge>(), n_030, n_055 );
//     auto& e_059_058 = g_1.addEdge(make_unique<Edge>(), n_059, n_058 );
//     auto& e_050_053 = g_1.addEdge(make_unique<Edge>(), n_050, n_053 );
//     auto& e_000_049 = g_1.addEdge(make_unique<Edge>(), n_000, n_049 );
//     auto& e_004_084 = g_1.addEdge(make_unique<Edge>(), n_004, n_084 );
//     auto& e_052_049 = g_1.addEdge(make_unique<Edge>(), n_052, n_049 );
//     auto& e_030_027 = g_1.addEdge(make_unique<Edge>(), n_030, n_027 );
//     auto& e_019_018 = g_1.addEdge(make_unique<Edge>(), n_019, n_018 );
//     auto& e_032_058 = g_1.addEdge(make_unique<Edge>(), n_032, n_058 );
//     auto& e_078_076 = g_1.addEdge(make_unique<Edge>(), n_078, n_076 );
//     auto& e_020_021 = g_1.addEdge(make_unique<Edge>(), n_020, n_021 );
//     auto& e_009_040 = g_1.addEdge(make_unique<Edge>(), n_009, n_040 );
//     auto& e_018_016 = g_1.addEdge(make_unique<Edge>(), n_018, n_016 );
//     auto& e_048_021 = g_1.addEdge(make_unique<Edge>(), n_048, n_021 );
//     auto& e_055_047 = g_1.addEdge(make_unique<Edge>(), n_055, n_047 );
//     auto& e_025_023 = g_1.addEdge(make_unique<Edge>(), n_025, n_023 );
//     auto& e_029_026 = g_1.addEdge(make_unique<Edge>(), n_029, n_026 );
//     auto& e_080_082 = g_1.addEdge(make_unique<Edge>(), n_080, n_082 );
//     auto& e_010_009 = g_1.addEdge(make_unique<Edge>(), n_010, n_009 );
//     auto& e_000_033 = g_1.addEdge(make_unique<Edge>(), n_000, n_033 );
//     auto& e_005_073 = g_1.addEdge(make_unique<Edge>(), n_005, n_073 );
//     auto& e_005_004 = g_1.addEdge(make_unique<Edge>(), n_005, n_004 );
//     auto& e_036_009 = g_1.addEdge(make_unique<Edge>(), n_036, n_009 );
//     auto& e_044_046 = g_1.addEdge(make_unique<Edge>(), n_044, n_046 );
//     auto& e_007_001 = g_1.addEdge(make_unique<Edge>(), n_007, n_001 );
//     auto& e_005_070 = g_1.addEdge(make_unique<Edge>(), n_005, n_070 );
//     auto& e_011_066 = g_1.addEdge(make_unique<Edge>(), n_011, n_066 );
//     auto& e_072_009 = g_1.addEdge(make_unique<Edge>(), n_072, n_009 );
//     auto& e_087_003 = g_1.addEdge(make_unique<Edge>(), n_087, n_003 );
//     auto& e_075_079 = g_1.addEdge(make_unique<Edge>(), n_075, n_079 );
//     auto& e_023_024 = g_1.addEdge(make_unique<Edge>(), n_023, n_024 );
//     auto& e_059_066 = g_1.addEdge(make_unique<Edge>(), n_059, n_066 );
//     auto& e_054_021 = g_1.addEdge(make_unique<Edge>(), n_054, n_021 );
//     auto& e_045_011 = g_1.addEdge(make_unique<Edge>(), n_045, n_011 );
//     auto& e_043_039 = g_1.addEdge(make_unique<Edge>(), n_043, n_039 );
//     auto& e_004_006 = g_1.addEdge(make_unique<Edge>(), n_004, n_006 );
//     auto& e_004_083 = g_1.addEdge(make_unique<Edge>(), n_004, n_083 );
//     auto& e_085_086 = g_1.addEdge(make_unique<Edge>(), n_085, n_086 );
//     auto& e_007_008 = g_1.addEdge(make_unique<Edge>(), n_007, n_008 );
//     auto& e_005_007 = g_1.addEdge(make_unique<Edge>(), n_005, n_007 );
//     auto& e_038_034 = g_1.addEdge(make_unique<Edge>(), n_038, n_034 );
//     auto& e_059_060 = g_1.addEdge(make_unique<Edge>(), n_059, n_060 );
//     auto& e_074_009 = g_1.addEdge(make_unique<Edge>(), n_074, n_009 );
//     auto& e_022_067 = g_1.addEdge(make_unique<Edge>(), n_022, n_067 );
//     auto& e_001_003 = g_1.addEdge(make_unique<Edge>(), n_001, n_003 );
//     auto& e_023_031 = g_1.addEdge(make_unique<Edge>(), n_023, n_031 );
//     auto& e_028_029 = g_1.addEdge(make_unique<Edge>(), n_028, n_029 );
//     auto& e_078_077 = g_1.addEdge(make_unique<Edge>(), n_078, n_077 );
//     auto& e_004_080 = g_1.addEdge(make_unique<Edge>(), n_004, n_080 );
//     auto& e_060_021 = g_1.addEdge(make_unique<Edge>(), n_060, n_021 );
//     auto& e_055_000 = g_1.addEdge(make_unique<Edge>(), n_055, n_000 );
//     auto& e_011_014 = g_1.addEdge(make_unique<Edge>(), n_011, n_014 );
//     auto& e_081_080 = g_1.addEdge(make_unique<Edge>(), n_081, n_080 );
//     auto& e_020_017 = g_1.addEdge(make_unique<Edge>(), n_020, n_017 );
//     auto& e_068_011 = g_1.addEdge(make_unique<Edge>(), n_068, n_011 );
//     auto& e_017_015 = g_1.addEdge(make_unique<Edge>(), n_017, n_015 );
//     auto& e_049_050 = g_1.addEdge(make_unique<Edge>(), n_049, n_050 );
//     auto& e_077_079 = g_1.addEdge(make_unique<Edge>(), n_077, n_079 );
//     auto& e_001_086 = g_1.addEdge(make_unique<Edge>(), n_001, n_086 );
//     auto& e_051_019 = g_1.addEdge(make_unique<Edge>(), n_051, n_019 );
//     auto& e_058_033 = g_1.addEdge(make_unique<Edge>(), n_058, n_033 );
//     auto& e_008_076 = g_1.addEdge(make_unique<Edge>(), n_008, n_076 );
//     auto& e_019_020 = g_1.addEdge(make_unique<Edge>(), n_019, n_020 );
//     auto& e_027_025 = g_1.addEdge(make_unique<Edge>(), n_027, n_025 );
//     auto& e_000_050 = g_1.addEdge(make_unique<Edge>(), n_000, n_050 );
//     auto& e_069_070 = g_1.addEdge(make_unique<Edge>(), n_069, n_070 );
//     auto& e_009_011 = g_1.addEdge(make_unique<Edge>(), n_009, n_011 );
//     auto& e_058_011 = g_1.addEdge(make_unique<Edge>(), n_058, n_011 );
//     auto& e_010_000 = g_1.addEdge(make_unique<Edge>(), n_010, n_000 );
//     auto& e_033_011 = g_1.addEdge(make_unique<Edge>(), n_033, n_011 );
//     auto& e_039_040 = g_1.addEdge(make_unique<Edge>(), n_039, n_040 );
//     auto& e_015_022 = g_1.addEdge(make_unique<Edge>(), n_015, n_022 );
//     auto& e_063_011 = g_1.addEdge(make_unique<Edge>(), n_063, n_011 );
//     auto& e_018_017 = g_1.addEdge(make_unique<Edge>(), n_018, n_017 );
//     auto& e_033_047 = g_1.addEdge(make_unique<Edge>(), n_033, n_047 );
//     auto& e_024_057 = g_1.addEdge(make_unique<Edge>(), n_024, n_057 );
//     auto& e_031_059 = g_1.addEdge(make_unique<Edge>(), n_031, n_059 );
//     auto& e_053_048 = g_1.addEdge(make_unique<Edge>(), n_053, n_048 );
//     auto& e_002_003 = g_1.addEdge(make_unique<Edge>(), n_002, n_003 );
//     auto& e_004_002 = g_1.addEdge(make_unique<Edge>(), n_004, n_002 );
//     auto& e_051_052 = g_1.addEdge(make_unique<Edge>(), n_051, n_052 );
//     auto& e_030_057 = g_1.addEdge(make_unique<Edge>(), n_030, n_057 );
//     auto& e_005_072 = g_1.addEdge(make_unique<Edge>(), n_005, n_072 );
//     auto& e_011_062 = g_1.addEdge(make_unique<Edge>(), n_011, n_062 );
//     auto& e_045_046 = g_1.addEdge(make_unique<Edge>(), n_045, n_046 );
//     auto& e_085_001 = g_1.addEdge(make_unique<Edge>(), n_085, n_001 );
//     auto& e_047_032 = g_1.addEdge(make_unique<Edge>(), n_047, n_032 );
//     auto& e_052_019 = g_1.addEdge(make_unique<Edge>(), n_052, n_019 );
//     auto& e_067_016 = g_1.addEdge(make_unique<Edge>(), n_067, n_016 );
//     auto& e_023_027 = g_1.addEdge(make_unique<Edge>(), n_023, n_027 );
//     auto& e_028_025 = g_1.addEdge(make_unique<Edge>(), n_028, n_025 );
//     auto& e_051_062 = g_1.addEdge(make_unique<Edge>(), n_051, n_062 );
//     auto& e_005_009 = g_1.addEdge(make_unique<Edge>(), n_005, n_009 );
//     auto& e_037_036 = g_1.addEdge(make_unique<Edge>(), n_037, n_036 );
//     auto& e_044_033 = g_1.addEdge(make_unique<Edge>(), n_044, n_033 );
//     auto& e_009_001 = g_1.addEdge(make_unique<Edge>(), n_009, n_001 );
//     auto& e_005_006 = g_1.addEdge(make_unique<Edge>(), n_005, n_006 );
//     auto& e_012_071 = g_1.addEdge(make_unique<Edge>(), n_012, n_071 );
//     auto& e_027_057 = g_1.addEdge(make_unique<Edge>(), n_027, n_057 );
//     auto& e_031_026 = g_1.addEdge(make_unique<Edge>(), n_031, n_026 );
//     auto& e_023_032 = g_1.addEdge(make_unique<Edge>(), n_023, n_032 );
//     auto& e_064_063 = g_1.addEdge(make_unique<Edge>(), n_064, n_063 );
//     auto& e_017_016 = g_1.addEdge(make_unique<Edge>(), n_017, n_016 );
//     auto& e_030_056 = g_1.addEdge(make_unique<Edge>(), n_030, n_056 );
//     auto& e_002_071 = g_1.addEdge(make_unique<Edge>(), n_002, n_071 );
//     auto& e_075_074 = g_1.addEdge(make_unique<Edge>(), n_075, n_074 );
//     auto& e_070_071 = g_1.addEdge(make_unique<Edge>(), n_070, n_071 );
//     auto& e_059_021 = g_1.addEdge(make_unique<Edge>(), n_059, n_021 );
//     auto& e_075_008 = g_1.addEdge(make_unique<Edge>(), n_075, n_008 );
//     auto& e_016_062 = g_1.addEdge(make_unique<Edge>(), n_016, n_062 );
//     auto& e_004_081 = g_1.addEdge(make_unique<Edge>(), n_004, n_081 );
//     auto& e_035_011 = g_1.addEdge(make_unique<Edge>(), n_035, n_011 );
//     auto& e_056_057 = g_1.addEdge(make_unique<Edge>(), n_056, n_057 );
//     auto& e_030_028 = g_1.addEdge(make_unique<Edge>(), n_030, n_028 );
//     auto& e_059_011 = g_1.addEdge(make_unique<Edge>(), n_059, n_011 );
//     auto& e_012_087 = g_1.addEdge(make_unique<Edge>(), n_012, n_087 );
//     auto& e_022_021 = g_1.addEdge(make_unique<Edge>(), n_022, n_021 );
//     auto& e_013_011 = g_1.addEdge(make_unique<Edge>(), n_013, n_011 );
//     auto& e_069_009 = g_1.addEdge(make_unique<Edge>(), n_069, n_009 );
//     auto& e_051_018 = g_1.addEdge(make_unique<Edge>(), n_051, n_018 );
//     auto& e_033_035 = g_1.addEdge(make_unique<Edge>(), n_033, n_035 );
//     auto& e_008_077 = g_1.addEdge(make_unique<Edge>(), n_008, n_077 );
//     auto& e_070_006 = g_1.addEdge(make_unique<Edge>(), n_070, n_006 );
//     auto& e_027_024 = g_1.addEdge(make_unique<Edge>(), n_027, n_024 );
//     auto& e_000_051 = g_1.addEdge(make_unique<Edge>(), n_000, n_051 );
//     auto& e_045_044 = g_1.addEdge(make_unique<Edge>(), n_045, n_044 );
//     auto& e_054_059 = g_1.addEdge(make_unique<Edge>(), n_054, n_059 );
//     auto& e_064_016 = g_1.addEdge(make_unique<Edge>(), n_064, n_016 );
//     auto& e_060_064 = g_1.addEdge(make_unique<Edge>(), n_060, n_064 );
//     auto& e_000_041 = g_1.addEdge(make_unique<Edge>(), n_000, n_041 );
//     auto& e_065_064 = g_1.addEdge(make_unique<Edge>(), n_065, n_064 );
//     auto& e_011_012 = g_1.addEdge(make_unique<Edge>(), n_011, n_012 );
//     auto& e_081_082 = g_1.addEdge(make_unique<Edge>(), n_081, n_082 );
//     auto& e_013_001 = g_1.addEdge(make_unique<Edge>(), n_013, n_001 );
//     auto& e_033_046 = g_1.addEdge(make_unique<Edge>(), n_033, n_046 );
//     auto& e_042_039 = g_1.addEdge(make_unique<Edge>(), n_042, n_039 );
//     auto& e_008_079 = g_1.addEdge(make_unique<Edge>(), n_008, n_079 );
//     auto& e_024_032 = g_1.addEdge(make_unique<Edge>(), n_024, n_032 );
//     auto& e_061_016 = g_1.addEdge(make_unique<Edge>(), n_061, n_016 );
// 
//     vector<edge_list_it_t> edges_000;
//     edges_000.push_back(e_000_014.backIt());
//     edges_000.push_back(e_055_000.backIt());
//     edges_000.push_back(e_000_051.backIt());
//     edges_000.push_back(e_000_041.backIt());
//     edges_000.push_back(e_000_001.backIt());
//     edges_000.push_back(e_000_049.backIt());
//     edges_000.push_back(e_010_000.backIt());
//     edges_000.push_back(e_000_033.backIt());
//     edges_000.push_back(e_000_052.backIt());
//     edges_000.push_back(e_000_050.backIt());
//     n_000.reorderIncidence(std::move(edges_000));
// 
//     vector<edge_list_it_t> edges_001;
//     edges_001.push_back(e_014_001.backIt());
//     edges_001.push_back(e_001_003.backIt());
//     edges_001.push_back(e_001_086.backIt());
//     edges_001.push_back(e_085_001.backIt());
//     edges_001.push_back(e_000_001.backIt());
//     edges_001.push_back(e_009_001.backIt());
//     edges_001.push_back(e_013_001.backIt());
//     edges_001.push_back(e_007_001.backIt());
//     n_001.reorderIncidence(std::move(edges_001));
// 
//     vector<edge_list_it_t> edges_002;
//     edges_002.push_back(e_002_006.backIt());
//     edges_002.push_back(e_002_071.backIt());
//     edges_002.push_back(e_002_070.backIt());
//     edges_002.push_back(e_002_003.backIt());
//     edges_002.push_back(e_002_080.backIt());
//     edges_002.push_back(e_004_002.backIt());
//     n_002.reorderIncidence(std::move(edges_002));
// 
//     vector<edge_list_it_t> edges_003;
//     edges_003.push_back(e_087_003.backIt());
//     edges_003.push_back(e_001_003.backIt());
//     edges_003.push_back(e_003_086.backIt());
//     edges_003.push_back(e_002_003.backIt());
//     edges_003.push_back(e_085_003.backIt());
//     n_003.reorderIncidence(std::move(edges_003));
// 
//     vector<edge_list_it_t> edges_004;
//     edges_004.push_back(e_004_006.backIt());
//     edges_004.push_back(e_004_084.backIt());
//     edges_004.push_back(e_005_004.backIt());
//     edges_004.push_back(e_004_083.backIt());
//     edges_004.push_back(e_004_080.backIt());
//     edges_004.push_back(e_004_081.backIt());
//     edges_004.push_back(e_004_002.backIt());
//     n_004.reorderIncidence(std::move(edges_004));
// 
//     vector<edge_list_it_t> edges_005;
//     edges_005.push_back(e_005_008.backIt());
//     edges_005.push_back(e_005_006.backIt());
//     edges_005.push_back(e_005_004.backIt());
//     edges_005.push_back(e_005_009.backIt());
//     edges_005.push_back(e_005_007.backIt());
//     edges_005.push_back(e_005_070.backIt());
//     edges_005.push_back(e_005_072.backIt());
//     edges_005.push_back(e_005_073.backIt());
//     n_005.reorderIncidence(std::move(edges_005));
// 
//     vector<edge_list_it_t> edges_006;
//     edges_006.push_back(e_002_006.backIt());
//     edges_006.push_back(e_004_006.backIt());
//     edges_006.push_back(e_005_006.backIt());
//     edges_006.push_back(e_070_006.backIt());
//     n_006.reorderIncidence(std::move(edges_006));
// 
//     vector<edge_list_it_t> edges_007;
//     edges_007.push_back(e_005_007.backIt());
//     edges_007.push_back(e_007_008.backIt());
//     edges_007.push_back(e_007_001.backIt());
//     n_007.reorderIncidence(std::move(edges_007));
// 
//     vector<edge_list_it_t> edges_008;
//     edges_008.push_back(e_075_008.backIt());
//     edges_008.push_back(e_005_008.backIt());
//     edges_008.push_back(e_008_077.backIt());
//     edges_008.push_back(e_008_079.backIt());
//     edges_008.push_back(e_007_008.backIt());
//     edges_008.push_back(e_008_076.backIt());
//     n_008.reorderIncidence(std::move(edges_008));
// 
//     vector<edge_list_it_t> edges_009;
//     edges_009.push_back(e_074_009.backIt());
//     edges_009.push_back(e_069_009.backIt());
//     edges_009.push_back(e_072_009.backIt());
//     edges_009.push_back(e_010_009.backIt());
//     edges_009.push_back(e_005_009.backIt());
//     edges_009.push_back(e_009_001.backIt());
//     edges_009.push_back(e_009_011.backIt());
//     edges_009.push_back(e_036_009.backIt());
//     edges_009.push_back(e_070_009.backIt());
//     edges_009.push_back(e_009_039.backIt());
//     edges_009.push_back(e_009_040.backIt());
//     n_009.reorderIncidence(std::move(edges_009));
// 
//     vector<edge_list_it_t> edges_010;
//     edges_010.push_back(e_010_009.backIt());
//     edges_010.push_back(e_010_000.backIt());
//     n_010.reorderIncidence(std::move(edges_010));
// 
//     vector<edge_list_it_t> edges_011;
//     edges_011.push_back(e_068_011.backIt());
//     edges_011.push_back(e_011_062.backIt());
//     edges_011.push_back(e_033_011.backIt());
//     edges_011.push_back(e_059_011.backIt());
//     edges_011.push_back(e_011_014.backIt());
//     edges_011.push_back(e_035_011.backIt());
//     edges_011.push_back(e_045_011.backIt());
//     edges_011.push_back(e_051_011.backIt());
//     edges_011.push_back(e_011_061.backIt());
//     edges_011.push_back(e_063_011.backIt());
//     edges_011.push_back(e_009_011.backIt());
//     edges_011.push_back(e_069_011.backIt());
//     edges_011.push_back(e_013_011.backIt());
//     edges_011.push_back(e_011_066.backIt());
//     edges_011.push_back(e_058_011.backIt());
//     edges_011.push_back(e_011_012.backIt());
//     n_011.reorderIncidence(std::move(edges_011));
// 
//     vector<edge_list_it_t> edges_012;
//     edges_012.push_back(e_085_012.backIt());
//     edges_012.push_back(e_012_071.backIt());
//     edges_012.push_back(e_011_012.backIt());
//     edges_012.push_back(e_012_087.backIt());
//     n_012.reorderIncidence(std::move(edges_012));
// 
//     vector<edge_list_it_t> edges_013;
//     edges_013.push_back(e_013_011.backIt());
//     edges_013.push_back(e_013_001.backIt());
//     n_013.reorderIncidence(std::move(edges_013));
// 
//     vector<edge_list_it_t> edges_014;
//     edges_014.push_back(e_000_014.backIt());
//     edges_014.push_back(e_014_001.backIt());
//     edges_014.push_back(e_068_014.backIt());
//     edges_014.push_back(e_011_014.backIt());
//     n_014.reorderIncidence(std::move(edges_014));
// 
//     vector<edge_list_it_t> edges_015;
//     edges_015.push_back(e_017_015.backIt());
//     edges_015.push_back(e_015_022.backIt());
//     edges_015.push_back(e_021_015.backIt());
//     edges_015.push_back(e_015_016.backIt());
//     n_015.reorderIncidence(std::move(edges_015));
// 
//     vector<edge_list_it_t> edges_016;
//     edges_016.push_back(e_067_016.backIt());
//     edges_016.push_back(e_016_062.backIt());
//     edges_016.push_back(e_051_016.backIt());
//     edges_016.push_back(e_061_016.backIt());
//     edges_016.push_back(e_018_016.backIt());
//     edges_016.push_back(e_064_016.backIt());
//     edges_016.push_back(e_015_016.backIt());
//     edges_016.push_back(e_017_016.backIt());
//     n_016.reorderIncidence(std::move(edges_016));
// 
//     vector<edge_list_it_t> edges_017;
//     edges_017.push_back(e_018_017.backIt());
//     edges_017.push_back(e_017_015.backIt());
//     edges_017.push_back(e_020_017.backIt());
//     edges_017.push_back(e_017_016.backIt());
//     n_017.reorderIncidence(std::move(edges_017));
// 
//     vector<edge_list_it_t> edges_018;
//     edges_018.push_back(e_051_018.backIt());
//     edges_018.push_back(e_018_017.backIt());
//     edges_018.push_back(e_019_018.backIt());
//     edges_018.push_back(e_018_016.backIt());
//     n_018.reorderIncidence(std::move(edges_018));
// 
//     vector<edge_list_it_t> edges_019;
//     edges_019.push_back(e_051_019.backIt());
//     edges_019.push_back(e_019_020.backIt());
//     edges_019.push_back(e_052_019.backIt());
//     edges_019.push_back(e_019_018.backIt());
//     n_019.reorderIncidence(std::move(edges_019));
// 
//     vector<edge_list_it_t> edges_020;
//     edges_020.push_back(e_019_020.backIt());
//     edges_020.push_back(e_020_021.backIt());
//     edges_020.push_back(e_048_020.backIt());
//     edges_020.push_back(e_020_017.backIt());
//     n_020.reorderIncidence(std::move(edges_020));
// 
//     vector<edge_list_it_t> edges_021;
//     edges_021.push_back(e_054_021.backIt());
//     edges_021.push_back(e_048_021.backIt());
//     edges_021.push_back(e_059_021.backIt());
//     edges_021.push_back(e_060_021.backIt());
//     edges_021.push_back(e_022_021.backIt());
//     edges_021.push_back(e_020_021.backIt());
//     edges_021.push_back(e_021_015.backIt());
//     n_021.reorderIncidence(std::move(edges_021));
// 
//     vector<edge_list_it_t> edges_022;
//     edges_022.push_back(e_015_022.backIt());
//     edges_022.push_back(e_022_065.backIt());
//     edges_022.push_back(e_022_067.backIt());
//     edges_022.push_back(e_022_021.backIt());
//     n_022.reorderIncidence(std::move(edges_022));
// 
//     vector<edge_list_it_t> edges_023;
//     edges_023.push_back(e_023_032.backIt());
//     edges_023.push_back(e_023_026.backIt());
//     edges_023.push_back(e_023_027.backIt());
//     edges_023.push_back(e_023_031.backIt());
//     edges_023.push_back(e_025_023.backIt());
//     edges_023.push_back(e_023_024.backIt());
//     n_023.reorderIncidence(std::move(edges_023));
// 
//     vector<edge_list_it_t> edges_024;
//     edges_024.push_back(e_027_024.backIt());
//     edges_024.push_back(e_024_032.backIt());
//     edges_024.push_back(e_023_024.backIt());
//     edges_024.push_back(e_024_057.backIt());
//     n_024.reorderIncidence(std::move(edges_024));
// 
//     vector<edge_list_it_t> edges_025;
//     edges_025.push_back(e_028_025.backIt());
//     edges_025.push_back(e_025_023.backIt());
//     edges_025.push_back(e_027_025.backIt());
//     edges_025.push_back(e_025_026.backIt());
//     n_025.reorderIncidence(std::move(edges_025));
// 
//     vector<edge_list_it_t> edges_026;
//     edges_026.push_back(e_029_026.backIt());
//     edges_026.push_back(e_023_026.backIt());
//     edges_026.push_back(e_026_054.backIt());
//     edges_026.push_back(e_031_026.backIt());
//     edges_026.push_back(e_025_026.backIt());
//     n_026.reorderIncidence(std::move(edges_026));
// 
//     vector<edge_list_it_t> edges_027;
//     edges_027.push_back(e_030_027.backIt());
//     edges_027.push_back(e_027_024.backIt());
//     edges_027.push_back(e_023_027.backIt());
//     edges_027.push_back(e_027_057.backIt());
//     edges_027.push_back(e_027_028.backIt());
//     edges_027.push_back(e_027_025.backIt());
//     n_027.reorderIncidence(std::move(edges_027));
// 
//     vector<edge_list_it_t> edges_028;
//     edges_028.push_back(e_030_028.backIt());
//     edges_028.push_back(e_028_025.backIt());
//     edges_028.push_back(e_027_028.backIt());
//     edges_028.push_back(e_028_029.backIt());
//     n_028.reorderIncidence(std::move(edges_028));
// 
//     vector<edge_list_it_t> edges_029;
//     edges_029.push_back(e_030_029.backIt());
//     edges_029.push_back(e_029_026.backIt());
//     edges_029.push_back(e_028_029.backIt());
//     edges_029.push_back(e_029_053.backIt());
//     n_029.reorderIncidence(std::move(edges_029));
// 
//     vector<edge_list_it_t> edges_030;
//     edges_030.push_back(e_030_027.backIt());
//     edges_030.push_back(e_030_055.backIt());
//     edges_030.push_back(e_030_029.backIt());
//     edges_030.push_back(e_030_057.backIt());
//     edges_030.push_back(e_030_028.backIt());
//     edges_030.push_back(e_030_056.backIt());
//     n_030.reorderIncidence(std::move(edges_030));
// 
//     vector<edge_list_it_t> edges_031;
//     edges_031.push_back(e_023_031.backIt());
//     edges_031.push_back(e_031_059.backIt());
//     edges_031.push_back(e_032_031.backIt());
//     edges_031.push_back(e_031_026.backIt());
//     n_031.reorderIncidence(std::move(edges_031));
// 
//     vector<edge_list_it_t> edges_032;
//     edges_032.push_back(e_023_032.backIt());
//     edges_032.push_back(e_024_032.backIt());
//     edges_032.push_back(e_032_058.backIt());
//     edges_032.push_back(e_032_031.backIt());
//     edges_032.push_back(e_047_032.backIt());
//     edges_032.push_back(e_056_032.backIt());
//     edges_032.push_back(e_032_057.backIt());
//     n_032.reorderIncidence(std::move(edges_032));
// 
//     vector<edge_list_it_t> edges_033;
//     edges_033.push_back(e_033_011.backIt());
//     edges_033.push_back(e_033_047.backIt());
//     edges_033.push_back(e_033_046.backIt());
//     edges_033.push_back(e_033_035.backIt());
//     edges_033.push_back(e_044_033.backIt());
//     edges_033.push_back(e_000_033.backIt());
//     edges_033.push_back(e_058_033.backIt());
//     n_033.reorderIncidence(std::move(edges_033));
// 
//     vector<edge_list_it_t> edges_034;
//     edges_034.push_back(e_037_034.backIt());
//     edges_034.push_back(e_038_034.backIt());
//     edges_034.push_back(e_036_034.backIt());
//     edges_034.push_back(e_034_035.backIt());
//     n_034.reorderIncidence(std::move(edges_034));
// 
//     vector<edge_list_it_t> edges_035;
//     edges_035.push_back(e_043_035.backIt());
//     edges_035.push_back(e_035_011.backIt());
//     edges_035.push_back(e_033_035.backIt());
//     edges_035.push_back(e_038_035.backIt());
//     edges_035.push_back(e_034_035.backIt());
//     n_035.reorderIncidence(std::move(edges_035));
// 
//     vector<edge_list_it_t> edges_036;
//     edges_036.push_back(e_037_036.backIt());
//     edges_036.push_back(e_036_034.backIt());
//     edges_036.push_back(e_036_009.backIt());
//     n_036.reorderIncidence(std::move(edges_036));
// 
//     vector<edge_list_it_t> edges_037;
//     edges_037.push_back(e_037_034.backIt());
//     edges_037.push_back(e_037_036.backIt());
//     n_037.reorderIncidence(std::move(edges_037));
// 
//     vector<edge_list_it_t> edges_038;
//     edges_038.push_back(e_038_034.backIt());
//     edges_038.push_back(e_038_035.backIt());
//     n_038.reorderIncidence(std::move(edges_038));
// 
//     vector<edge_list_it_t> edges_039;
//     edges_039.push_back(e_041_039.backIt());
//     edges_039.push_back(e_043_039.backIt());
//     edges_039.push_back(e_039_040.backIt());
//     edges_039.push_back(e_042_039.backIt());
//     edges_039.push_back(e_009_039.backIt());
//     n_039.reorderIncidence(std::move(edges_039));
// 
//     vector<edge_list_it_t> edges_040;
//     edges_040.push_back(e_039_040.backIt());
//     edges_040.push_back(e_009_040.backIt());
//     edges_040.push_back(e_041_040.backIt());
//     n_040.reorderIncidence(std::move(edges_040));
// 
//     vector<edge_list_it_t> edges_041;
//     edges_041.push_back(e_041_039.backIt());
//     edges_041.push_back(e_000_041.backIt());
//     edges_041.push_back(e_041_042.backIt());
//     edges_041.push_back(e_041_040.backIt());
//     n_041.reorderIncidence(std::move(edges_041));
// 
//     vector<edge_list_it_t> edges_042;
//     edges_042.push_back(e_042_039.backIt());
//     edges_042.push_back(e_041_042.backIt());
//     n_042.reorderIncidence(std::move(edges_042));
// 
//     vector<edge_list_it_t> edges_043;
//     edges_043.push_back(e_043_035.backIt());
//     edges_043.push_back(e_043_039.backIt());
//     n_043.reorderIncidence(std::move(edges_043));
// 
//     vector<edge_list_it_t> edges_044;
//     edges_044.push_back(e_044_046.backIt());
//     edges_044.push_back(e_045_044.backIt());
//     edges_044.push_back(e_044_033.backIt());
//     n_044.reorderIncidence(std::move(edges_044));
// 
//     vector<edge_list_it_t> edges_045;
//     edges_045.push_back(e_045_046.backIt());
//     edges_045.push_back(e_045_011.backIt());
//     edges_045.push_back(e_045_044.backIt());
//     n_045.reorderIncidence(std::move(edges_045));
// 
//     vector<edge_list_it_t> edges_046;
//     edges_046.push_back(e_044_046.backIt());
//     edges_046.push_back(e_033_046.backIt());
//     edges_046.push_back(e_045_046.backIt());
//     n_046.reorderIncidence(std::move(edges_046));
// 
//     vector<edge_list_it_t> edges_047;
//     edges_047.push_back(e_033_047.backIt());
//     edges_047.push_back(e_047_056.backIt());
//     edges_047.push_back(e_055_047.backIt());
//     edges_047.push_back(e_047_032.backIt());
//     n_047.reorderIncidence(std::move(edges_047));
// 
//     vector<edge_list_it_t> edges_048;
//     edges_048.push_back(e_049_048.backIt());
//     edges_048.push_back(e_048_021.backIt());
//     edges_048.push_back(e_053_048.backIt());
//     edges_048.push_back(e_048_020.backIt());
//     n_048.reorderIncidence(std::move(edges_048));
// 
//     vector<edge_list_it_t> edges_049;
//     edges_049.push_back(e_000_049.backIt());
//     edges_049.push_back(e_049_048.backIt());
//     edges_049.push_back(e_052_049.backIt());
//     edges_049.push_back(e_049_050.backIt());
//     n_049.reorderIncidence(std::move(edges_049));
// 
//     vector<edge_list_it_t> edges_050;
//     edges_050.push_back(e_000_050.backIt());
//     edges_050.push_back(e_050_053.backIt());
//     edges_050.push_back(e_049_050.backIt());
//     edges_050.push_back(e_050_055.backIt());
//     n_050.reorderIncidence(std::move(edges_050));
// 
//     vector<edge_list_it_t> edges_051;
//     edges_051.push_back(e_051_018.backIt());
//     edges_051.push_back(e_000_051.backIt());
//     edges_051.push_back(e_051_016.backIt());
//     edges_051.push_back(e_051_062.backIt());
//     edges_051.push_back(e_051_011.backIt());
//     edges_051.push_back(e_051_019.backIt());
//     edges_051.push_back(e_051_052.backIt());
//     n_051.reorderIncidence(std::move(edges_051));
// 
//     vector<edge_list_it_t> edges_052;
//     edges_052.push_back(e_000_052.backIt());
//     edges_052.push_back(e_052_019.backIt());
//     edges_052.push_back(e_051_052.backIt());
//     edges_052.push_back(e_052_049.backIt());
//     n_052.reorderIncidence(std::move(edges_052));
// 
//     vector<edge_list_it_t> edges_053;
//     edges_053.push_back(e_050_053.backIt());
//     edges_053.push_back(e_053_054.backIt());
//     edges_053.push_back(e_029_053.backIt());
//     edges_053.push_back(e_053_048.backIt());
//     n_053.reorderIncidence(std::move(edges_053));
// 
//     vector<edge_list_it_t> edges_054;
//     edges_054.push_back(e_026_054.backIt());
//     edges_054.push_back(e_054_021.backIt());
//     edges_054.push_back(e_053_054.backIt());
//     edges_054.push_back(e_054_059.backIt());
//     n_054.reorderIncidence(std::move(edges_054));
// 
//     vector<edge_list_it_t> edges_055;
//     edges_055.push_back(e_030_055.backIt());
//     edges_055.push_back(e_055_000.backIt());
//     edges_055.push_back(e_050_055.backIt());
//     edges_055.push_back(e_055_047.backIt());
//     n_055.reorderIncidence(std::move(edges_055));
// 
//     vector<edge_list_it_t> edges_056;
//     edges_056.push_back(e_047_056.backIt());
//     edges_056.push_back(e_056_057.backIt());
//     edges_056.push_back(e_030_056.backIt());
//     edges_056.push_back(e_056_032.backIt());
//     n_056.reorderIncidence(std::move(edges_056));
// 
//     vector<edge_list_it_t> edges_057;
//     edges_057.push_back(e_056_057.backIt());
//     edges_057.push_back(e_030_057.backIt());
//     edges_057.push_back(e_027_057.backIt());
//     edges_057.push_back(e_024_057.backIt());
//     edges_057.push_back(e_032_057.backIt());
//     n_057.reorderIncidence(std::move(edges_057));
// 
//     vector<edge_list_it_t> edges_058;
//     edges_058.push_back(e_032_058.backIt());
//     edges_058.push_back(e_058_011.backIt());
//     edges_058.push_back(e_059_058.backIt());
//     edges_058.push_back(e_058_033.backIt());
//     n_058.reorderIncidence(std::move(edges_058));
// 
//     vector<edge_list_it_t> edges_059;
//     edges_059.push_back(e_059_011.backIt());
//     edges_059.push_back(e_059_021.backIt());
//     edges_059.push_back(e_031_059.backIt());
//     edges_059.push_back(e_059_060.backIt());
//     edges_059.push_back(e_059_066.backIt());
//     edges_059.push_back(e_054_059.backIt());
//     edges_059.push_back(e_059_058.backIt());
//     n_059.reorderIncidence(std::move(edges_059));
// 
//     vector<edge_list_it_t> edges_060;
//     edges_060.push_back(e_066_060.backIt());
//     edges_060.push_back(e_060_021.backIt());
//     edges_060.push_back(e_059_060.backIt());
//     edges_060.push_back(e_060_064.backIt());
//     n_060.reorderIncidence(std::move(edges_060));
// 
//     vector<edge_list_it_t> edges_061;
//     edges_061.push_back(e_011_061.backIt());
//     edges_061.push_back(e_061_016.backIt());
//     edges_061.push_back(e_063_061.backIt());
//     edges_061.push_back(e_061_062.backIt());
//     n_061.reorderIncidence(std::move(edges_061));
// 
//     vector<edge_list_it_t> edges_062;
//     edges_062.push_back(e_011_062.backIt());
//     edges_062.push_back(e_016_062.backIt());
//     edges_062.push_back(e_051_062.backIt());
//     edges_062.push_back(e_061_062.backIt());
//     n_062.reorderIncidence(std::move(edges_062));
// 
//     vector<edge_list_it_t> edges_063;
//     edges_063.push_back(e_064_063.backIt());
//     edges_063.push_back(e_063_011.backIt());
//     edges_063.push_back(e_066_063.backIt());
//     edges_063.push_back(e_063_061.backIt());
//     n_063.reorderIncidence(std::move(edges_063));
// 
//     vector<edge_list_it_t> edges_064;
//     edges_064.push_back(e_065_064.backIt());
//     edges_064.push_back(e_064_063.backIt());
//     edges_064.push_back(e_060_064.backIt());
//     edges_064.push_back(e_064_016.backIt());
//     n_064.reorderIncidence(std::move(edges_064));
// 
//     vector<edge_list_it_t> edges_065;
//     edges_065.push_back(e_022_065.backIt());
//     edges_065.push_back(e_065_064.backIt());
//     n_065.reorderIncidence(std::move(edges_065));
// 
//     vector<edge_list_it_t> edges_066;
//     edges_066.push_back(e_011_066.backIt());
//     edges_066.push_back(e_066_060.backIt());
//     edges_066.push_back(e_059_066.backIt());
//     edges_066.push_back(e_066_063.backIt());
//     n_066.reorderIncidence(std::move(edges_066));
// 
//     vector<edge_list_it_t> edges_067;
//     edges_067.push_back(e_067_016.backIt());
//     edges_067.push_back(e_022_067.backIt());
//     n_067.reorderIncidence(std::move(edges_067));
// 
//     vector<edge_list_it_t> edges_068;
//     edges_068.push_back(e_068_011.backIt());
//     edges_068.push_back(e_068_014.backIt());
//     n_068.reorderIncidence(std::move(edges_068));
// 
//     vector<edge_list_it_t> edges_069;
//     edges_069.push_back(e_069_070.backIt());
//     edges_069.push_back(e_069_009.backIt());
//     edges_069.push_back(e_069_011.backIt());
//     n_069.reorderIncidence(std::move(edges_069));
// 
//     vector<edge_list_it_t> edges_070;
//     edges_070.push_back(e_069_070.backIt());
//     edges_070.push_back(e_002_070.backIt());
//     edges_070.push_back(e_070_006.backIt());
//     edges_070.push_back(e_005_070.backIt());
//     edges_070.push_back(e_070_009.backIt());
//     edges_070.push_back(e_072_070.backIt());
//     edges_070.push_back(e_070_071.backIt());
//     n_070.reorderIncidence(std::move(edges_070));
// 
//     vector<edge_list_it_t> edges_071;
//     edges_071.push_back(e_002_071.backIt());
//     edges_071.push_back(e_012_071.backIt());
//     edges_071.push_back(e_070_071.backIt());
//     n_071.reorderIncidence(std::move(edges_071));
// 
//     vector<edge_list_it_t> edges_072;
//     edges_072.push_back(e_072_009.backIt());
//     edges_072.push_back(e_072_073.backIt());
//     edges_072.push_back(e_005_072.backIt());
//     edges_072.push_back(e_072_070.backIt());
//     n_072.reorderIncidence(std::move(edges_072));
// 
//     vector<edge_list_it_t> edges_073;
//     edges_073.push_back(e_072_073.backIt());
//     edges_073.push_back(e_005_073.backIt());
//     n_073.reorderIncidence(std::move(edges_073));
// 
//     vector<edge_list_it_t> edges_074;
//     edges_074.push_back(e_074_009.backIt());
//     edges_074.push_back(e_075_074.backIt());
//     n_074.reorderIncidence(std::move(edges_074));
// 
//     vector<edge_list_it_t> edges_075;
//     edges_075.push_back(e_075_008.backIt());
//     edges_075.push_back(e_075_079.backIt());
//     edges_075.push_back(e_075_074.backIt());
//     n_075.reorderIncidence(std::move(edges_075));
// 
//     vector<edge_list_it_t> edges_076;
//     edges_076.push_back(e_078_076.backIt());
//     edges_076.push_back(e_008_076.backIt());
//     edges_076.push_back(e_076_077.backIt());
//     n_076.reorderIncidence(std::move(edges_076));
// 
//     vector<edge_list_it_t> edges_077;
//     edges_077.push_back(e_008_077.backIt());
//     edges_077.push_back(e_078_077.backIt());
//     edges_077.push_back(e_077_079.backIt());
//     edges_077.push_back(e_076_077.backIt());
//     n_077.reorderIncidence(std::move(edges_077));
// 
//     vector<edge_list_it_t> edges_078;
//     edges_078.push_back(e_078_076.backIt());
//     edges_078.push_back(e_078_077.backIt());
//     n_078.reorderIncidence(std::move(edges_078));
// 
//     vector<edge_list_it_t> edges_079;
//     edges_079.push_back(e_008_079.backIt());
//     edges_079.push_back(e_075_079.backIt());
//     edges_079.push_back(e_077_079.backIt());
//     n_079.reorderIncidence(std::move(edges_079));
// 
//     vector<edge_list_it_t> edges_080;
//     edges_080.push_back(e_081_080.backIt());
//     edges_080.push_back(e_080_082.backIt());
//     edges_080.push_back(e_004_080.backIt());
//     edges_080.push_back(e_083_080.backIt());
//     edges_080.push_back(e_002_080.backIt());
//     n_080.reorderIncidence(std::move(edges_080));
// 
//     vector<edge_list_it_t> edges_081;
//     edges_081.push_back(e_081_080.backIt());
//     edges_081.push_back(e_081_082.backIt());
//     edges_081.push_back(e_004_081.backIt());
//     n_081.reorderIncidence(std::move(edges_081));
// 
//     vector<edge_list_it_t> edges_082;
//     edges_082.push_back(e_080_082.backIt());
//     edges_082.push_back(e_081_082.backIt());
//     n_082.reorderIncidence(std::move(edges_082));
// 
//     vector<edge_list_it_t> edges_083;
//     edges_083.push_back(e_083_084.backIt());
//     edges_083.push_back(e_004_083.backIt());
//     edges_083.push_back(e_083_080.backIt());
//     n_083.reorderIncidence(std::move(edges_083));
// 
//     vector<edge_list_it_t> edges_084;
//     edges_084.push_back(e_004_084.backIt());
//     edges_084.push_back(e_083_084.backIt());
//     n_084.reorderIncidence(std::move(edges_084));
// 
//     vector<edge_list_it_t> edges_085;
//     edges_085.push_back(e_085_086.backIt());
//     edges_085.push_back(e_085_001.backIt());
//     edges_085.push_back(e_085_012.backIt());
//     edges_085.push_back(e_085_003.backIt());
//     n_085.reorderIncidence(std::move(edges_085));
// 
//     vector<edge_list_it_t> edges_086;
//     edges_086.push_back(e_001_086.backIt());
//     edges_086.push_back(e_085_086.backIt());
//     edges_086.push_back(e_003_086.backIt());
//     n_086.reorderIncidence(std::move(edges_086));
// 
//     vector<edge_list_it_t> edges_087;
//     edges_087.push_back(e_087_003.backIt());
//     edges_087.push_back(e_012_087.backIt());
//     n_087.reorderIncidence(std::move(edges_087));
// 
//     vector<node_list_it_t> stOrder;
//     stOrder.push_back(n_000.backIt());
//     stOrder.push_back(n_052.backIt());
//     stOrder.push_back(n_049.backIt());
//     stOrder.push_back(n_048.backIt());
//     stOrder.push_back(n_053.backIt());
//     stOrder.push_back(n_050.backIt());
//     stOrder.push_back(n_055.backIt());
//     stOrder.push_back(n_047.backIt());
//     stOrder.push_back(n_056.backIt());
//     stOrder.push_back(n_057.backIt());
//     stOrder.push_back(n_010.backIt());
//     stOrder.push_back(n_009.backIt());
//     stOrder.push_back(n_036.backIt());
//     stOrder.push_back(n_037.backIt());
//     stOrder.push_back(n_034.backIt());
//     stOrder.push_back(n_038.backIt());
//     stOrder.push_back(n_074.backIt());
//     stOrder.push_back(n_075.backIt());
//     stOrder.push_back(n_079.backIt());
//     stOrder.push_back(n_077.backIt());
//     stOrder.push_back(n_078.backIt());
//     stOrder.push_back(n_076.backIt());
//     stOrder.push_back(n_008.backIt());
//     stOrder.push_back(n_005.backIt());
//     stOrder.push_back(n_007.backIt());
//     stOrder.push_back(n_006.backIt());
//     stOrder.push_back(n_004.backIt());
//     stOrder.push_back(n_084.backIt());
//     stOrder.push_back(n_083.backIt());
//     stOrder.push_back(n_081.backIt());
//     stOrder.push_back(n_082.backIt());
//     stOrder.push_back(n_080.backIt());
//     stOrder.push_back(n_002.backIt());
//     stOrder.push_back(n_071.backIt());
//     stOrder.push_back(n_073.backIt());
//     stOrder.push_back(n_072.backIt());
//     stOrder.push_back(n_070.backIt());
//     stOrder.push_back(n_069.backIt());
//     stOrder.push_back(n_012.backIt());
//     stOrder.push_back(n_085.backIt());
//     stOrder.push_back(n_086.backIt());
//     stOrder.push_back(n_087.backIt());
//     stOrder.push_back(n_003.backIt());
//     stOrder.push_back(n_040.backIt());
//     stOrder.push_back(n_041.backIt());
//     stOrder.push_back(n_042.backIt());
//     stOrder.push_back(n_039.backIt());
//     stOrder.push_back(n_043.backIt());
//     stOrder.push_back(n_035.backIt());
//     stOrder.push_back(n_033.backIt());
//     stOrder.push_back(n_046.backIt());
//     stOrder.push_back(n_044.backIt());
//     stOrder.push_back(n_045.backIt());
//     stOrder.push_back(n_058.backIt());
//     stOrder.push_back(n_059.backIt());
//     stOrder.push_back(n_060.backIt());
//     stOrder.push_back(n_066.backIt());
//     stOrder.push_back(n_063.backIt());
//     stOrder.push_back(n_061.backIt());
//     stOrder.push_back(n_064.backIt());
//     stOrder.push_back(n_065.backIt());
//     stOrder.push_back(n_031.backIt());
//     stOrder.push_back(n_023.backIt());
//     stOrder.push_back(n_025.backIt());
//     stOrder.push_back(n_028.backIt());
//     stOrder.push_back(n_032.backIt());
//     stOrder.push_back(n_024.backIt());
//     stOrder.push_back(n_027.backIt());
//     stOrder.push_back(n_030.backIt());
//     stOrder.push_back(n_029.backIt());
//     stOrder.push_back(n_026.backIt());
//     stOrder.push_back(n_054.backIt());
//     stOrder.push_back(n_021.backIt());
//     stOrder.push_back(n_020.backIt());
//     stOrder.push_back(n_019.backIt());
//     stOrder.push_back(n_051.backIt());
//     stOrder.push_back(n_018.backIt());
//     stOrder.push_back(n_017.backIt());
//     stOrder.push_back(n_015.backIt());
//     stOrder.push_back(n_022.backIt());
//     stOrder.push_back(n_067.backIt());
//     stOrder.push_back(n_016.backIt());
//     stOrder.push_back(n_062.backIt());
//     stOrder.push_back(n_011.backIt());
//     stOrder.push_back(n_013.backIt());
//     stOrder.push_back(n_068.backIt());
//     stOrder.push_back(n_014.backIt());
//     stOrder.push_back(n_001.backIt());
// 
//     BLPlanarityTester tester01;
//     auto res01 =  tester01.findEmbedding(g_1, stOrder);
//     if (res01) {
//         cerr << "The graph is planar #1.\n";
//     }
//     else{
//         cerr << "The graph is planar.#1\n";
//     }
// 
//     BLPlanarityTester tester02;
//     auto res02 =  tester02.isPlanar(g_1, stOrder);
//     if (res02) {
//         cerr << "The graph is planar. #2\n";
//     }
//     else{
//         cerr << "The graph is planar. #2\n";
//     }
// 
//     PlanarDualGraphMaker maker;
//     EmbeddedGraph emb;
//     DualGraph     dual;
//     maker.makeDualGraph(g_1, emb, dual);
// 
//     auto nPair = g_1.nodes();
//     for (auto nit = nPair.first; nit != nPair.second; nit++) {
//         auto& N = dynamic_cast<NumNode&>(*(*nit));
//         cerr << N.num() << " :";
//         for (auto iit  = N.incidentEdges().first;
//                   iit != N.incidentEdges().second; iit++) {
//             auto& E = *(*(*iit));
//             auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
//             cerr << " " << A.num();
// 
//         }
//         cerr << "\n";
//     }
// }
// 
// 
// /**  @brief tests rearrangeIncidentEdges()
//  */
// TEST_F(BLPlanarityTesterTestsForEmbedding, TestRepro_002) {
// 
//     Graph g_1;
// 
//     auto& n_000 = g_1.addNode(make_unique<NumNode>(  0 ));
//     auto& n_001 = g_1.addNode(make_unique<NumNode>(  1 ));
//     auto& n_002 = g_1.addNode(make_unique<NumNode>(  2 ));
//     auto& n_003 = g_1.addNode(make_unique<NumNode>(  3 ));
//     auto& n_004 = g_1.addNode(make_unique<NumNode>(  4 ));
//     auto& n_005 = g_1.addNode(make_unique<NumNode>(  5 ));
//     auto& n_006 = g_1.addNode(make_unique<NumNode>(  6 ));
//     auto& n_007 = g_1.addNode(make_unique<NumNode>(  7 ));
//     auto& n_008 = g_1.addNode(make_unique<NumNode>(  8 ));
//     auto& n_009 = g_1.addNode(make_unique<NumNode>(  9 ));
//     auto& n_010 = g_1.addNode(make_unique<NumNode>( 10 ));
//     auto& n_011 = g_1.addNode(make_unique<NumNode>( 11 ));
//     auto& n_012 = g_1.addNode(make_unique<NumNode>( 12 ));
//     auto& n_013 = g_1.addNode(make_unique<NumNode>( 13 ));
//     auto& n_014 = g_1.addNode(make_unique<NumNode>( 14 ));
//     auto& n_015 = g_1.addNode(make_unique<NumNode>( 15 ));
//     auto& n_016 = g_1.addNode(make_unique<NumNode>( 16 ));
//     auto& n_017 = g_1.addNode(make_unique<NumNode>( 17 ));
//     auto& n_018 = g_1.addNode(make_unique<NumNode>( 18 ));
//     auto& n_019 = g_1.addNode(make_unique<NumNode>( 19 ));
//     auto& n_020 = g_1.addNode(make_unique<NumNode>( 20 ));
//     auto& n_021 = g_1.addNode(make_unique<NumNode>( 21 ));
//     auto& n_022 = g_1.addNode(make_unique<NumNode>( 22 ));
//     auto& n_023 = g_1.addNode(make_unique<NumNode>( 23 ));
//     auto& n_024 = g_1.addNode(make_unique<NumNode>( 24 ));
//     auto& n_025 = g_1.addNode(make_unique<NumNode>( 25 ));
//     auto& n_026 = g_1.addNode(make_unique<NumNode>( 26 ));
//     auto& n_027 = g_1.addNode(make_unique<NumNode>( 27 ));
//     auto& n_028 = g_1.addNode(make_unique<NumNode>( 28 ));
//     auto& n_029 = g_1.addNode(make_unique<NumNode>( 29 ));
//     auto& n_030 = g_1.addNode(make_unique<NumNode>( 30 ));
//     auto& n_031 = g_1.addNode(make_unique<NumNode>( 31 ));
//     auto& n_032 = g_1.addNode(make_unique<NumNode>( 32 ));
//     auto& n_033 = g_1.addNode(make_unique<NumNode>( 33 ));
//     auto& n_034 = g_1.addNode(make_unique<NumNode>( 34 ));
//     auto& n_035 = g_1.addNode(make_unique<NumNode>( 35 ));
//     auto& n_036 = g_1.addNode(make_unique<NumNode>( 36 ));
//     auto& n_037 = g_1.addNode(make_unique<NumNode>( 37 ));
//     auto& n_038 = g_1.addNode(make_unique<NumNode>( 38 ));
//     auto& n_039 = g_1.addNode(make_unique<NumNode>( 39 ));
//     auto& n_040 = g_1.addNode(make_unique<NumNode>( 40 ));
//     auto& n_041 = g_1.addNode(make_unique<NumNode>( 41 ));
//     auto& n_042 = g_1.addNode(make_unique<NumNode>( 42 ));
//     auto& n_043 = g_1.addNode(make_unique<NumNode>( 43 ));
//     auto& n_044 = g_1.addNode(make_unique<NumNode>( 44 ));
//     auto& n_045 = g_1.addNode(make_unique<NumNode>( 45 ));
//     auto& n_046 = g_1.addNode(make_unique<NumNode>( 46 ));
//     auto& n_047 = g_1.addNode(make_unique<NumNode>( 47 ));
//     auto& n_048 = g_1.addNode(make_unique<NumNode>( 48 ));
//     auto& n_049 = g_1.addNode(make_unique<NumNode>( 49 ));
//     auto& n_050 = g_1.addNode(make_unique<NumNode>( 50 ));
//     auto& n_051 = g_1.addNode(make_unique<NumNode>( 51 ));
//     auto& n_052 = g_1.addNode(make_unique<NumNode>( 52 ));
//     auto& n_053 = g_1.addNode(make_unique<NumNode>( 53 ));
//     auto& n_054 = g_1.addNode(make_unique<NumNode>( 54 ));
//     auto& n_055 = g_1.addNode(make_unique<NumNode>( 55 ));
//     auto& n_056 = g_1.addNode(make_unique<NumNode>( 56 ));
//     auto& n_057 = g_1.addNode(make_unique<NumNode>( 57 ));
//     auto& n_058 = g_1.addNode(make_unique<NumNode>( 58 ));
//     auto& n_059 = g_1.addNode(make_unique<NumNode>( 59 ));
//     auto& n_060 = g_1.addNode(make_unique<NumNode>( 60 ));
//     auto& n_061 = g_1.addNode(make_unique<NumNode>( 61 ));
//     auto& n_062 = g_1.addNode(make_unique<NumNode>( 62 ));
//     auto& n_063 = g_1.addNode(make_unique<NumNode>( 63 ));
//     auto& n_064 = g_1.addNode(make_unique<NumNode>( 64 ));
//     auto& n_065 = g_1.addNode(make_unique<NumNode>( 65 ));
//     auto& n_066 = g_1.addNode(make_unique<NumNode>( 66 ));
//     auto& n_067 = g_1.addNode(make_unique<NumNode>( 67 ));
//     auto& n_068 = g_1.addNode(make_unique<NumNode>( 68 ));
//     auto& n_069 = g_1.addNode(make_unique<NumNode>( 69 ));
//     auto& n_070 = g_1.addNode(make_unique<NumNode>( 70 ));
//     auto& n_071 = g_1.addNode(make_unique<NumNode>( 71 ));
//     auto& n_072 = g_1.addNode(make_unique<NumNode>( 72 ));
//     auto& n_073 = g_1.addNode(make_unique<NumNode>( 73 ));
//     auto& n_074 = g_1.addNode(make_unique<NumNode>( 74 ));
//     auto& n_075 = g_1.addNode(make_unique<NumNode>( 75 ));
//     auto& n_076 = g_1.addNode(make_unique<NumNode>( 76 ));
//     auto& n_077 = g_1.addNode(make_unique<NumNode>( 77 ));
//     auto& n_078 = g_1.addNode(make_unique<NumNode>( 78 ));
//     auto& n_079 = g_1.addNode(make_unique<NumNode>( 79 ));
//     auto& n_080 = g_1.addNode(make_unique<NumNode>( 80 ));
//     auto& n_081 = g_1.addNode(make_unique<NumNode>( 81 ));
//     auto& n_082 = g_1.addNode(make_unique<NumNode>( 82 ));
//     auto& n_083 = g_1.addNode(make_unique<NumNode>( 83 ));
//     auto& n_084 = g_1.addNode(make_unique<NumNode>( 84 ));
//     auto& n_085 = g_1.addNode(make_unique<NumNode>( 85 ));
//     auto& n_086 = g_1.addNode(make_unique<NumNode>( 86 ));
//     auto& n_087 = g_1.addNode(make_unique<NumNode>( 87 ));
//     auto& n_088 = g_1.addNode(make_unique<NumNode>( 88 ));
//     auto& n_089 = g_1.addNode(make_unique<NumNode>( 89 ));
//     auto& n_090 = g_1.addNode(make_unique<NumNode>( 90 ));
//     auto& n_091 = g_1.addNode(make_unique<NumNode>( 91 ));
//     auto& n_092 = g_1.addNode(make_unique<NumNode>( 92 ));
//     auto& n_093 = g_1.addNode(make_unique<NumNode>( 93 ));
//     auto& n_094 = g_1.addNode(make_unique<NumNode>( 94 ));
//     auto& n_095 = g_1.addNode(make_unique<NumNode>( 95 ));
//     auto& n_096 = g_1.addNode(make_unique<NumNode>( 96 ));
//     auto& n_097 = g_1.addNode(make_unique<NumNode>( 97 ));
//     auto& n_098 = g_1.addNode(make_unique<NumNode>( 98 ));
//     auto& n_099 = g_1.addNode(make_unique<NumNode>( 99 ));
//     auto& n_100 = g_1.addNode(make_unique<NumNode>(100 ));
//     auto& n_101 = g_1.addNode(make_unique<NumNode>(101 ));
//     auto& n_102 = g_1.addNode(make_unique<NumNode>(102 ));
//     auto& n_103 = g_1.addNode(make_unique<NumNode>(103 ));
//     auto& n_104 = g_1.addNode(make_unique<NumNode>(104 ));
//     auto& n_105 = g_1.addNode(make_unique<NumNode>(105 ));
//     auto& n_106 = g_1.addNode(make_unique<NumNode>(106 ));
//     auto& n_107 = g_1.addNode(make_unique<NumNode>(107 ));
//     auto& n_108 = g_1.addNode(make_unique<NumNode>(108 ));
//     auto& n_109 = g_1.addNode(make_unique<NumNode>(109 ));
//     auto& n_110 = g_1.addNode(make_unique<NumNode>(110 ));
//     auto& n_111 = g_1.addNode(make_unique<NumNode>(111 ));
//     auto& n_112 = g_1.addNode(make_unique<NumNode>(112 ));
//     auto& n_113 = g_1.addNode(make_unique<NumNode>(113 ));
//     auto& n_114 = g_1.addNode(make_unique<NumNode>(114 ));
//     auto& n_115 = g_1.addNode(make_unique<NumNode>(115 ));
//     auto& n_116 = g_1.addNode(make_unique<NumNode>(116 ));
//     auto& n_117 = g_1.addNode(make_unique<NumNode>(117 ));
//     auto& n_118 = g_1.addNode(make_unique<NumNode>(118 ));
//     auto& n_119 = g_1.addNode(make_unique<NumNode>(119 ));
//     auto& n_120 = g_1.addNode(make_unique<NumNode>(120 ));
//     auto& n_121 = g_1.addNode(make_unique<NumNode>(121 ));
//     auto& n_122 = g_1.addNode(make_unique<NumNode>(122 ));
//     auto& n_123 = g_1.addNode(make_unique<NumNode>(123 ));
//     auto& n_124 = g_1.addNode(make_unique<NumNode>(124 ));
//     auto& n_125 = g_1.addNode(make_unique<NumNode>(125 ));
//     auto& n_126 = g_1.addNode(make_unique<NumNode>(126 ));
//     auto& n_127 = g_1.addNode(make_unique<NumNode>(127 ));
//     auto& n_128 = g_1.addNode(make_unique<NumNode>(128 ));
//     auto& n_129 = g_1.addNode(make_unique<NumNode>(129 ));
//     auto& n_130 = g_1.addNode(make_unique<NumNode>(130 ));
//     auto& n_131 = g_1.addNode(make_unique<NumNode>(131 ));
//     auto& n_132 = g_1.addNode(make_unique<NumNode>(132 ));
//     auto& n_133 = g_1.addNode(make_unique<NumNode>(133 ));
//     auto& n_134 = g_1.addNode(make_unique<NumNode>(134 ));
//     auto& n_135 = g_1.addNode(make_unique<NumNode>(135 ));
//     auto& n_136 = g_1.addNode(make_unique<NumNode>(136 ));
//     auto& n_137 = g_1.addNode(make_unique<NumNode>(137 ));
//     auto& n_138 = g_1.addNode(make_unique<NumNode>(138 ));
//     auto& n_139 = g_1.addNode(make_unique<NumNode>(139 ));
//     auto& n_140 = g_1.addNode(make_unique<NumNode>(140 ));
//     auto& n_141 = g_1.addNode(make_unique<NumNode>(141 ));
//     auto& n_142 = g_1.addNode(make_unique<NumNode>(142 ));
// 
//     auto& e_051_040 = g_1.addEdge(make_unique<Edge>(), n_051, n_040 );
//     auto& e_116_026 = g_1.addEdge(make_unique<Edge>(), n_116, n_026 );
//     auto& e_048_043 = g_1.addEdge(make_unique<Edge>(), n_048, n_043 );
//     auto& e_097_035 = g_1.addEdge(make_unique<Edge>(), n_097, n_035 );
//     auto& e_054_055 = g_1.addEdge(make_unique<Edge>(), n_054, n_055 );
//     auto& e_116_115 = g_1.addEdge(make_unique<Edge>(), n_116, n_115 );
//     auto& e_100_106 = g_1.addEdge(make_unique<Edge>(), n_100, n_106 );
//     auto& e_137_034 = g_1.addEdge(make_unique<Edge>(), n_137, n_034 );
//     auto& e_088_132 = g_1.addEdge(make_unique<Edge>(), n_088, n_132 );
//     auto& e_054_058 = g_1.addEdge(make_unique<Edge>(), n_054, n_058 );
//     auto& e_062_087 = g_1.addEdge(make_unique<Edge>(), n_062, n_087 );
//     auto& e_026_028 = g_1.addEdge(make_unique<Edge>(), n_026, n_028 );
//     auto& e_040_041 = g_1.addEdge(make_unique<Edge>(), n_040, n_041 );
//     auto& e_104_105 = g_1.addEdge(make_unique<Edge>(), n_104, n_105 );
//     auto& e_103_104 = g_1.addEdge(make_unique<Edge>(), n_103, n_104 );
//     auto& e_032_031 = g_1.addEdge(make_unique<Edge>(), n_032, n_031 );
//     auto& e_053_081 = g_1.addEdge(make_unique<Edge>(), n_053, n_081 );
//     auto& e_086_087 = g_1.addEdge(make_unique<Edge>(), n_086, n_087 );
//     auto& e_085_131 = g_1.addEdge(make_unique<Edge>(), n_085, n_131 );
//     auto& e_024_117 = g_1.addEdge(make_unique<Edge>(), n_024, n_117 );
//     auto& e_032_002 = g_1.addEdge(make_unique<Edge>(), n_032, n_002 );
//     auto& e_022_117 = g_1.addEdge(make_unique<Edge>(), n_022, n_117 );
//     auto& e_111_113 = g_1.addEdge(make_unique<Edge>(), n_111, n_113 );
//     auto& e_010_008 = g_1.addEdge(make_unique<Edge>(), n_010, n_008 );
//     auto& e_077_076 = g_1.addEdge(make_unique<Edge>(), n_077, n_076 );
//     auto& e_115_028 = g_1.addEdge(make_unique<Edge>(), n_115, n_028 );
//     auto& e_082_057 = g_1.addEdge(make_unique<Edge>(), n_082, n_057 );
//     auto& e_142_005 = g_1.addEdge(make_unique<Edge>(), n_142, n_005 );
//     auto& e_105_111 = g_1.addEdge(make_unique<Edge>(), n_105, n_111 );
//     auto& e_084_085 = g_1.addEdge(make_unique<Edge>(), n_084, n_085 );
//     auto& e_051_039 = g_1.addEdge(make_unique<Edge>(), n_051, n_039 );
//     auto& e_007_006 = g_1.addEdge(make_unique<Edge>(), n_007, n_006 );
//     auto& e_072_096 = g_1.addEdge(make_unique<Edge>(), n_072, n_096 );
//     auto& e_098_107 = g_1.addEdge(make_unique<Edge>(), n_098, n_107 );
//     auto& e_141_016 = g_1.addEdge(make_unique<Edge>(), n_141, n_016 );
//     auto& e_061_062 = g_1.addEdge(make_unique<Edge>(), n_061, n_062 );
//     auto& e_123_042 = g_1.addEdge(make_unique<Edge>(), n_123, n_042 );
//     auto& e_038_002 = g_1.addEdge(make_unique<Edge>(), n_038, n_002 );
//     auto& e_121_065 = g_1.addEdge(make_unique<Edge>(), n_121, n_065 );
//     auto& e_052_136 = g_1.addEdge(make_unique<Edge>(), n_052, n_136 );
//     auto& e_040_038 = g_1.addEdge(make_unique<Edge>(), n_040, n_038 );
//     auto& e_013_020 = g_1.addEdge(make_unique<Edge>(), n_013, n_020 );
//     auto& e_091_092 = g_1.addEdge(make_unique<Edge>(), n_091, n_092 );
//     auto& e_017_142 = g_1.addEdge(make_unique<Edge>(), n_017, n_142 );
//     auto& e_034_036 = g_1.addEdge(make_unique<Edge>(), n_034, n_036 );
//     auto& e_023_025 = g_1.addEdge(make_unique<Edge>(), n_023, n_025 );
//     auto& e_082_055 = g_1.addEdge(make_unique<Edge>(), n_082, n_055 );
//     auto& e_033_034 = g_1.addEdge(make_unique<Edge>(), n_033, n_034 );
//     auto& e_002_006 = g_1.addEdge(make_unique<Edge>(), n_002, n_006 );
//     auto& e_099_022 = g_1.addEdge(make_unique<Edge>(), n_099, n_022 );
//     auto& e_037_038 = g_1.addEdge(make_unique<Edge>(), n_037, n_038 );
//     auto& e_101_103 = g_1.addEdge(make_unique<Edge>(), n_101, n_103 );
//     auto& e_097_098 = g_1.addEdge(make_unique<Edge>(), n_097, n_098 );
//     auto& e_093_063 = g_1.addEdge(make_unique<Edge>(), n_093, n_063 );
//     auto& e_124_073 = g_1.addEdge(make_unique<Edge>(), n_124, n_073 );
//     auto& e_053_136 = g_1.addEdge(make_unique<Edge>(), n_053, n_136 );
//     auto& e_139_035 = g_1.addEdge(make_unique<Edge>(), n_139, n_035 );
//     auto& e_075_045 = g_1.addEdge(make_unique<Edge>(), n_075, n_045 );
//     auto& e_107_108 = g_1.addEdge(make_unique<Edge>(), n_107, n_108 );
//     auto& e_014_015 = g_1.addEdge(make_unique<Edge>(), n_014, n_015 );
//     auto& e_122_036 = g_1.addEdge(make_unique<Edge>(), n_122, n_036 );
//     auto& e_020_019 = g_1.addEdge(make_unique<Edge>(), n_020, n_019 );
//     auto& e_057_085 = g_1.addEdge(make_unique<Edge>(), n_057, n_085 );
//     auto& e_100_105 = g_1.addEdge(make_unique<Edge>(), n_100, n_105 );
//     auto& e_049_048 = g_1.addEdge(make_unique<Edge>(), n_049, n_048 );
//     auto& e_064_061 = g_1.addEdge(make_unique<Edge>(), n_064, n_061 );
//     auto& e_036_094 = g_1.addEdge(make_unique<Edge>(), n_036, n_094 );
//     auto& e_000_001 = g_1.addEdge(make_unique<Edge>(), n_000, n_001 );
//     auto& e_013_023 = g_1.addEdge(make_unique<Edge>(), n_013, n_023 );
//     auto& e_070_068 = g_1.addEdge(make_unique<Edge>(), n_070, n_068 );
//     auto& e_106_105 = g_1.addEdge(make_unique<Edge>(), n_106, n_105 );
//     auto& e_008_065 = g_1.addEdge(make_unique<Edge>(), n_008, n_065 );
//     auto& e_082_056 = g_1.addEdge(make_unique<Edge>(), n_082, n_056 );
//     auto& e_082_133 = g_1.addEdge(make_unique<Edge>(), n_082, n_133 );
//     auto& e_061_063 = g_1.addEdge(make_unique<Edge>(), n_061, n_063 );
//     auto& e_060_086 = g_1.addEdge(make_unique<Edge>(), n_060, n_086 );
//     auto& e_000_108 = g_1.addEdge(make_unique<Edge>(), n_000, n_108 );
//     auto& e_053_054 = g_1.addEdge(make_unique<Edge>(), n_053, n_054 );
//     auto& e_022_021 = g_1.addEdge(make_unique<Edge>(), n_022, n_021 );
//     auto& e_031_002 = g_1.addEdge(make_unique<Edge>(), n_031, n_002 );
//     auto& e_122_042 = g_1.addEdge(make_unique<Edge>(), n_122, n_042 );
//     auto& e_120_113 = g_1.addEdge(make_unique<Edge>(), n_120, n_113 );
//     auto& e_053_049 = g_1.addEdge(make_unique<Edge>(), n_053, n_049 );
//     auto& e_057_054 = g_1.addEdge(make_unique<Edge>(), n_057, n_054 );
//     auto& e_103_102 = g_1.addEdge(make_unique<Edge>(), n_103, n_102 );
//     auto& e_137_139 = g_1.addEdge(make_unique<Edge>(), n_137, n_139 );
//     auto& e_022_023 = g_1.addEdge(make_unique<Edge>(), n_022, n_023 );
//     auto& e_030_032 = g_1.addEdge(make_unique<Edge>(), n_030, n_032 );
//     auto& e_126_127 = g_1.addEdge(make_unique<Edge>(), n_126, n_127 );
//     auto& e_076_045 = g_1.addEdge(make_unique<Edge>(), n_076, n_045 );
//     auto& e_079_128 = g_1.addEdge(make_unique<Edge>(), n_079, n_128 );
//     auto& e_031_037 = g_1.addEdge(make_unique<Edge>(), n_031, n_037 );
//     auto& e_069_068 = g_1.addEdge(make_unique<Edge>(), n_069, n_068 );
//     auto& e_002_001 = g_1.addEdge(make_unique<Edge>(), n_002, n_001 );
//     auto& e_008_009 = g_1.addEdge(make_unique<Edge>(), n_008, n_009 );
//     auto& e_075_074 = g_1.addEdge(make_unique<Edge>(), n_075, n_074 );
//     auto& e_082_134 = g_1.addEdge(make_unique<Edge>(), n_082, n_134 );
//     auto& e_082_084 = g_1.addEdge(make_unique<Edge>(), n_082, n_084 );
//     auto& e_043_046 = g_1.addEdge(make_unique<Edge>(), n_043, n_046 );
//     auto& e_041_050 = g_1.addEdge(make_unique<Edge>(), n_041, n_050 );
//     auto& e_095_094 = g_1.addEdge(make_unique<Edge>(), n_095, n_094 );
//     auto& e_117_099 = g_1.addEdge(make_unique<Edge>(), n_117, n_099 );
//     auto& e_052_049 = g_1.addEdge(make_unique<Edge>(), n_052, n_049 );
//     auto& e_081_080 = g_1.addEdge(make_unique<Edge>(), n_081, n_080 );
//     auto& e_007_002 = g_1.addEdge(make_unique<Edge>(), n_007, n_002 );
//     auto& e_047_049 = g_1.addEdge(make_unique<Edge>(), n_047, n_049 );
//     auto& e_011_010 = g_1.addEdge(make_unique<Edge>(), n_011, n_010 );
//     auto& e_094_044 = g_1.addEdge(make_unique<Edge>(), n_094, n_044 );
//     auto& e_066_064 = g_1.addEdge(make_unique<Edge>(), n_066, n_064 );
//     auto& e_012_013 = g_1.addEdge(make_unique<Edge>(), n_012, n_013 );
//     auto& e_057_086 = g_1.addEdge(make_unique<Edge>(), n_057, n_086 );
//     auto& e_018_005 = g_1.addEdge(make_unique<Edge>(), n_018, n_005 );
//     auto& e_111_110 = g_1.addEdge(make_unique<Edge>(), n_111, n_110 );
//     auto& e_090_084 = g_1.addEdge(make_unique<Edge>(), n_090, n_084 );
//     auto& e_135_002 = g_1.addEdge(make_unique<Edge>(), n_135, n_002 );
//     auto& e_003_006 = g_1.addEdge(make_unique<Edge>(), n_003, n_006 );
//     auto& e_049_077 = g_1.addEdge(make_unique<Edge>(), n_049, n_077 );
//     auto& e_050_042 = g_1.addEdge(make_unique<Edge>(), n_050, n_042 );
//     auto& e_036_097 = g_1.addEdge(make_unique<Edge>(), n_036, n_097 );
//     auto& e_010_009 = g_1.addEdge(make_unique<Edge>(), n_010, n_009 );
//     auto& e_009_007 = g_1.addEdge(make_unique<Edge>(), n_009, n_007 );
//     auto& e_130_129 = g_1.addEdge(make_unique<Edge>(), n_130, n_129 );
//     auto& e_090_085 = g_1.addEdge(make_unique<Edge>(), n_090, n_085 );
//     auto& e_053_079 = g_1.addEdge(make_unique<Edge>(), n_053, n_079 );
//     auto& e_052_054 = g_1.addEdge(make_unique<Edge>(), n_052, n_054 );
//     auto& e_044_123 = g_1.addEdge(make_unique<Edge>(), n_044, n_123 );
//     auto& e_016_004 = g_1.addEdge(make_unique<Edge>(), n_016, n_004 );
//     auto& e_054_049 = g_1.addEdge(make_unique<Edge>(), n_054, n_049 );
//     auto& e_045_047 = g_1.addEdge(make_unique<Edge>(), n_045, n_047 );
//     auto& e_079_129 = g_1.addEdge(make_unique<Edge>(), n_079, n_129 );
//     auto& e_086_090 = g_1.addEdge(make_unique<Edge>(), n_086, n_090 );
//     auto& e_059_049 = g_1.addEdge(make_unique<Edge>(), n_059, n_049 );
//     auto& e_022_070 = g_1.addEdge(make_unique<Edge>(), n_022, n_070 );
//     auto& e_020_005 = g_1.addEdge(make_unique<Edge>(), n_020, n_005 );
//     auto& e_017_141 = g_1.addEdge(make_unique<Edge>(), n_017, n_141 );
//     auto& e_129_128 = g_1.addEdge(make_unique<Edge>(), n_129, n_128 );
//     auto& e_084_091 = g_1.addEdge(make_unique<Edge>(), n_084, n_091 );
//     auto& e_023_024 = g_1.addEdge(make_unique<Edge>(), n_023, n_024 );
//     auto& e_029_016 = g_1.addEdge(make_unique<Edge>(), n_029, n_016 );
//     auto& e_003_005 = g_1.addEdge(make_unique<Edge>(), n_003, n_005 );
//     auto& e_092_063 = g_1.addEdge(make_unique<Edge>(), n_092, n_063 );
//     auto& e_135_061 = g_1.addEdge(make_unique<Edge>(), n_135, n_061 );
//     auto& e_038_039 = g_1.addEdge(make_unique<Edge>(), n_038, n_039 );
//     auto& e_078_080 = g_1.addEdge(make_unique<Edge>(), n_078, n_080 );
//     auto& e_002_065 = g_1.addEdge(make_unique<Edge>(), n_002, n_065 );
//     auto& e_034_031 = g_1.addEdge(make_unique<Edge>(), n_034, n_031 );
//     auto& e_080_077 = g_1.addEdge(make_unique<Edge>(), n_080, n_077 );
//     auto& e_063_089 = g_1.addEdge(make_unique<Edge>(), n_063, n_089 );
//     auto& e_030_002 = g_1.addEdge(make_unique<Edge>(), n_030, n_002 );
//     auto& e_059_051 = g_1.addEdge(make_unique<Edge>(), n_059, n_051 );
//     auto& e_079_127 = g_1.addEdge(make_unique<Edge>(), n_079, n_127 );
//     auto& e_057_058 = g_1.addEdge(make_unique<Edge>(), n_057, n_058 );
//     auto& e_110_000 = g_1.addEdge(make_unique<Edge>(), n_110, n_000 );
//     auto& e_025_020 = g_1.addEdge(make_unique<Edge>(), n_025, n_020 );
//     auto& e_072_022 = g_1.addEdge(make_unique<Edge>(), n_072, n_022 );
//     auto& e_039_060 = g_1.addEdge(make_unique<Edge>(), n_039, n_060 );
//     auto& e_073_069 = g_1.addEdge(make_unique<Edge>(), n_073, n_069 );
//     auto& e_036_095 = g_1.addEdge(make_unique<Edge>(), n_036, n_095 );
//     auto& e_098_100 = g_1.addEdge(make_unique<Edge>(), n_098, n_100 );
//     auto& e_105_102 = g_1.addEdge(make_unique<Edge>(), n_105, n_102 );
//     auto& e_000_002 = g_1.addEdge(make_unique<Edge>(), n_000, n_002 );
//     auto& e_108_035 = g_1.addEdge(make_unique<Edge>(), n_108, n_035 );
//     auto& e_139_138 = g_1.addEdge(make_unique<Edge>(), n_139, n_138 );
//     auto& e_028_018 = g_1.addEdge(make_unique<Edge>(), n_028, n_018 );
//     auto& e_077_047 = g_1.addEdge(make_unique<Edge>(), n_077, n_047 );
//     auto& e_042_043 = g_1.addEdge(make_unique<Edge>(), n_042, n_043 );
//     auto& e_022_013 = g_1.addEdge(make_unique<Edge>(), n_022, n_013 );
//     auto& e_095_073 = g_1.addEdge(make_unique<Edge>(), n_095, n_073 );
//     auto& e_036_037 = g_1.addEdge(make_unique<Edge>(), n_036, n_037 );
//     auto& e_119_118 = g_1.addEdge(make_unique<Edge>(), n_119, n_118 );
//     auto& e_021_011 = g_1.addEdge(make_unique<Edge>(), n_021, n_011 );
//     auto& e_000_109 = g_1.addEdge(make_unique<Edge>(), n_000, n_109 );
//     auto& e_121_002 = g_1.addEdge(make_unique<Edge>(), n_121, n_002 );
//     auto& e_113_115 = g_1.addEdge(make_unique<Edge>(), n_113, n_115 );
//     auto& e_082_083 = g_1.addEdge(make_unique<Edge>(), n_082, n_083 );
//     auto& e_133_085 = g_1.addEdge(make_unique<Edge>(), n_133, n_085 );
//     auto& e_096_095 = g_1.addEdge(make_unique<Edge>(), n_096, n_095 );
//     auto& e_056_054 = g_1.addEdge(make_unique<Edge>(), n_056, n_054 );
//     auto& e_047_046 = g_1.addEdge(make_unique<Edge>(), n_047, n_046 );
//     auto& e_037_002 = g_1.addEdge(make_unique<Edge>(), n_037, n_002 );
//     auto& e_062_088 = g_1.addEdge(make_unique<Edge>(), n_062, n_088 );
//     auto& e_068_011 = g_1.addEdge(make_unique<Edge>(), n_068, n_011 );
//     auto& e_000_028 = g_1.addEdge(make_unique<Edge>(), n_000, n_028 );
//     auto& e_049_050 = g_1.addEdge(make_unique<Edge>(), n_049, n_050 );
//     auto& e_119_120 = g_1.addEdge(make_unique<Edge>(), n_119, n_120 );
//     auto& e_115_114 = g_1.addEdge(make_unique<Edge>(), n_115, n_114 );
//     auto& e_120_112 = g_1.addEdge(make_unique<Edge>(), n_120, n_112 );
//     auto& e_101_106 = g_1.addEdge(make_unique<Edge>(), n_101, n_106 );
//     auto& e_017_028 = g_1.addEdge(make_unique<Edge>(), n_017, n_028 );
//     auto& e_053_052 = g_1.addEdge(make_unique<Edge>(), n_053, n_052 );
//     auto& e_013_025 = g_1.addEdge(make_unique<Edge>(), n_013, n_025 );
//     auto& e_035_033 = g_1.addEdge(make_unique<Edge>(), n_035, n_033 );
//     auto& e_112_113 = g_1.addEdge(make_unique<Edge>(), n_112, n_113 );
//     auto& e_073_075 = g_1.addEdge(make_unique<Edge>(), n_073, n_075 );
//     auto& e_041_049 = g_1.addEdge(make_unique<Edge>(), n_041, n_049 );
//     auto& e_096_036 = g_1.addEdge(make_unique<Edge>(), n_096, n_036 );
//     auto& e_044_042 = g_1.addEdge(make_unique<Edge>(), n_044, n_042 );
//     auto& e_022_096 = g_1.addEdge(make_unique<Edge>(), n_022, n_096 );
//     auto& e_087_089 = g_1.addEdge(make_unique<Edge>(), n_087, n_089 );
//     auto& e_125_049 = g_1.addEdge(make_unique<Edge>(), n_125, n_049 );
//     auto& e_049_040 = g_1.addEdge(make_unique<Edge>(), n_049, n_040 );
//     auto& e_106_102 = g_1.addEdge(make_unique<Edge>(), n_106, n_102 );
//     auto& e_012_014 = g_1.addEdge(make_unique<Edge>(), n_012, n_014 );
//     auto& e_066_008 = g_1.addEdge(make_unique<Edge>(), n_066, n_008 );
//     auto& e_060_058 = g_1.addEdge(make_unique<Edge>(), n_060, n_058 );
//     auto& e_013_015 = g_1.addEdge(make_unique<Edge>(), n_013, n_015 );
//     auto& e_078_079 = g_1.addEdge(make_unique<Edge>(), n_078, n_079 );
//     auto& e_027_019 = g_1.addEdge(make_unique<Edge>(), n_027, n_019 );
//     auto& e_018_017 = g_1.addEdge(make_unique<Edge>(), n_018, n_017 );
//     auto& e_071_069 = g_1.addEdge(make_unique<Edge>(), n_071, n_069 );
//     auto& e_002_003 = g_1.addEdge(make_unique<Edge>(), n_002, n_003 );
//     auto& e_126_125 = g_1.addEdge(make_unique<Edge>(), n_126, n_125 );
//     auto& e_082_085 = g_1.addEdge(make_unique<Edge>(), n_082, n_085 );
//     auto& e_021_070 = g_1.addEdge(make_unique<Edge>(), n_021, n_070 );
//     auto& e_004_002 = g_1.addEdge(make_unique<Edge>(), n_004, n_002 );
//     auto& e_080_083 = g_1.addEdge(make_unique<Edge>(), n_080, n_083 );
//     auto& e_101_119 = g_1.addEdge(make_unique<Edge>(), n_101, n_119 );
//     auto& e_067_066 = g_1.addEdge(make_unique<Edge>(), n_067, n_066 );
//     auto& e_053_078 = g_1.addEdge(make_unique<Edge>(), n_053, n_078 );
//     auto& e_057_090 = g_1.addEdge(make_unique<Edge>(), n_057, n_090 );
//     auto& e_112_103 = g_1.addEdge(make_unique<Edge>(), n_112, n_103 );
//     auto& e_016_005 = g_1.addEdge(make_unique<Edge>(), n_016, n_005 );
//     auto& e_088_060 = g_1.addEdge(make_unique<Edge>(), n_088, n_060 );
//     auto& e_045_046 = g_1.addEdge(make_unique<Edge>(), n_045, n_046 );
//     auto& e_027_028 = g_1.addEdge(make_unique<Edge>(), n_027, n_028 );
//     auto& e_067_010 = g_1.addEdge(make_unique<Edge>(), n_067, n_010 );
//     auto& e_014_007 = g_1.addEdge(make_unique<Edge>(), n_014, n_007 );
//     auto& e_013_005 = g_1.addEdge(make_unique<Edge>(), n_013, n_005 );
//     auto& e_019_005 = g_1.addEdge(make_unique<Edge>(), n_019, n_005 );
//     auto& e_094_075 = g_1.addEdge(make_unique<Edge>(), n_094, n_075 );
//     auto& e_050_048 = g_1.addEdge(make_unique<Edge>(), n_050, n_048 );
//     auto& e_015_006 = g_1.addEdge(make_unique<Edge>(), n_015, n_006 );
//     auto& e_022_071 = g_1.addEdge(make_unique<Edge>(), n_022, n_071 );
//     auto& e_068_067 = g_1.addEdge(make_unique<Edge>(), n_068, n_067 );
//     auto& e_109_030 = g_1.addEdge(make_unique<Edge>(), n_109, n_030 );
//     auto& e_091_063 = g_1.addEdge(make_unique<Edge>(), n_091, n_063 );
//     auto& e_041_036 = g_1.addEdge(make_unique<Edge>(), n_041, n_036 );
//     auto& e_057_056 = g_1.addEdge(make_unique<Edge>(), n_057, n_056 );
//     auto& e_071_072 = g_1.addEdge(make_unique<Edge>(), n_071, n_072 );
//     auto& e_009_012 = g_1.addEdge(make_unique<Edge>(), n_009, n_012 );
//     auto& e_029_028 = g_1.addEdge(make_unique<Edge>(), n_029, n_028 );
//     auto& e_110_114 = g_1.addEdge(make_unique<Edge>(), n_110, n_114 );
//     auto& e_126_079 = g_1.addEdge(make_unique<Edge>(), n_126, n_079 );
//     auto& e_107_110 = g_1.addEdge(make_unique<Edge>(), n_107, n_110 );
//     auto& e_053_055 = g_1.addEdge(make_unique<Edge>(), n_053, n_055 );
//     auto& e_055_081 = g_1.addEdge(make_unique<Edge>(), n_055, n_081 );
//     auto& e_005_006 = g_1.addEdge(make_unique<Edge>(), n_005, n_006 );
//     auto& e_103_120 = g_1.addEdge(make_unique<Edge>(), n_103, n_120 );
//     auto& e_024_026 = g_1.addEdge(make_unique<Edge>(), n_024, n_026 );
//     auto& e_054_051 = g_1.addEdge(make_unique<Edge>(), n_054, n_051 );
//     auto& e_133_134 = g_1.addEdge(make_unique<Edge>(), n_133, n_134 );
//     auto& e_025_027 = g_1.addEdge(make_unique<Edge>(), n_025, n_027 );
//     auto& e_064_063 = g_1.addEdge(make_unique<Edge>(), n_064, n_063 );
//     auto& e_001_029 = g_1.addEdge(make_unique<Edge>(), n_001, n_029 );
//     auto& e_002_088 = g_1.addEdge(make_unique<Edge>(), n_002, n_088 );
//     auto& e_033_032 = g_1.addEdge(make_unique<Edge>(), n_033, n_032 );
//     auto& e_015_005 = g_1.addEdge(make_unique<Edge>(), n_015, n_005 );
//     auto& e_013_021 = g_1.addEdge(make_unique<Edge>(), n_013, n_021 );
//     auto& e_092_093 = g_1.addEdge(make_unique<Edge>(), n_092, n_093 );
//     auto& e_101_099 = g_1.addEdge(make_unique<Edge>(), n_101, n_099 );
//     auto& e_114_028 = g_1.addEdge(make_unique<Edge>(), n_114, n_028 );
//     auto& e_059_054 = g_1.addEdge(make_unique<Edge>(), n_059, n_054 );
//     auto& e_070_071 = g_1.addEdge(make_unique<Edge>(), n_070, n_071 );
//     auto& e_084_131 = g_1.addEdge(make_unique<Edge>(), n_084, n_131 );
//     auto& e_128_127 = g_1.addEdge(make_unique<Edge>(), n_128, n_127 );
//     auto& e_108_109 = g_1.addEdge(make_unique<Edge>(), n_108, n_109 );
//     auto& e_112_104 = g_1.addEdge(make_unique<Edge>(), n_112, n_104 );
//     auto& e_056_055 = g_1.addEdge(make_unique<Edge>(), n_056, n_055 );
//     auto& e_074_067 = g_1.addEdge(make_unique<Edge>(), n_074, n_067 );
//     auto& e_124_072 = g_1.addEdge(make_unique<Edge>(), n_124, n_072 );
//     auto& e_112_105 = g_1.addEdge(make_unique<Edge>(), n_112, n_105 );
//     auto& e_105_107 = g_1.addEdge(make_unique<Edge>(), n_105, n_107 );
//     auto& e_118_116 = g_1.addEdge(make_unique<Edge>(), n_118, n_116 );
//     auto& e_104_102 = g_1.addEdge(make_unique<Edge>(), n_104, n_102 );
//     auto& e_062_089 = g_1.addEdge(make_unique<Edge>(), n_062, n_089 );
//     auto& e_076_092 = g_1.addEdge(make_unique<Edge>(), n_076, n_092 );
//     auto& e_000_029 = g_1.addEdge(make_unique<Edge>(), n_000, n_029 );
//     auto& e_118_113 = g_1.addEdge(make_unique<Edge>(), n_118, n_113 );
//     auto& e_039_002 = g_1.addEdge(make_unique<Edge>(), n_039, n_002 );
//     auto& e_140_030 = g_1.addEdge(make_unique<Edge>(), n_140, n_030 );
//     auto& e_117_116 = g_1.addEdge(make_unique<Edge>(), n_117, n_116 );
//     auto& e_035_098 = g_1.addEdge(make_unique<Edge>(), n_035, n_098 );
//     auto& e_022_024 = g_1.addEdge(make_unique<Edge>(), n_022, n_024 );
//     auto& e_109_033 = g_1.addEdge(make_unique<Edge>(), n_109, n_033 );
//     auto& e_101_102 = g_1.addEdge(make_unique<Edge>(), n_101, n_102 );
//     auto& e_035_138 = g_1.addEdge(make_unique<Edge>(), n_035, n_138 );
//     auto& e_019_018 = g_1.addEdge(make_unique<Edge>(), n_019, n_018 );
//     auto& e_000_140 = g_1.addEdge(make_unique<Edge>(), n_000, n_140 );
//     auto& e_137_138 = g_1.addEdge(make_unique<Edge>(), n_137, n_138 );
//     auto& e_062_063 = g_1.addEdge(make_unique<Edge>(), n_062, n_063 );
//     auto& e_001_004 = g_1.addEdge(make_unique<Edge>(), n_001, n_004 );
//     auto& e_044_043 = g_1.addEdge(make_unique<Edge>(), n_044, n_043 );
//     auto& e_048_046 = g_1.addEdge(make_unique<Edge>(), n_048, n_046 );
//     auto& e_022_097 = g_1.addEdge(make_unique<Edge>(), n_022, n_097 );
//     auto& e_011_012 = g_1.addEdge(make_unique<Edge>(), n_011, n_012 );
//     auto& e_069_074 = g_1.addEdge(make_unique<Edge>(), n_069, n_074 );
//     auto& e_035_036 = g_1.addEdge(make_unique<Edge>(), n_035, n_036 );
//     auto& e_081_082 = g_1.addEdge(make_unique<Edge>(), n_081, n_082 );
//     auto& e_114_000 = g_1.addEdge(make_unique<Edge>(), n_114, n_000 );
//     auto& e_026_027 = g_1.addEdge(make_unique<Edge>(), n_026, n_027 );
//     auto& e_112_111 = g_1.addEdge(make_unique<Edge>(), n_112, n_111 );
//     auto& e_013_014 = g_1.addEdge(make_unique<Edge>(), n_013, n_014 );
//     auto& e_099_100 = g_1.addEdge(make_unique<Edge>(), n_099, n_100 );
//     auto& e_083_091 = g_1.addEdge(make_unique<Edge>(), n_083, n_091 );
//     auto& e_065_064 = g_1.addEdge(make_unique<Edge>(), n_065, n_064 );
//     auto& e_130_128 = g_1.addEdge(make_unique<Edge>(), n_130, n_128 );
//     auto& e_132_087 = g_1.addEdge(make_unique<Edge>(), n_132, n_087 );
//     auto& e_083_076 = g_1.addEdge(make_unique<Edge>(), n_083, n_076 );
//     auto& e_063_087 = g_1.addEdge(make_unique<Edge>(), n_063, n_087 );
//     auto& e_004_003 = g_1.addEdge(make_unique<Edge>(), n_004, n_003 );
//     auto& e_101_118 = g_1.addEdge(make_unique<Edge>(), n_101, n_118 );
//     auto& e_093_066 = g_1.addEdge(make_unique<Edge>(), n_093, n_066 );
//     auto& e_074_093 = g_1.addEdge(make_unique<Edge>(), n_074, n_093 );
//     auto& e_103_119 = g_1.addEdge(make_unique<Edge>(), n_103, n_119 );
// 
//     vector<edge_list_it_t> edges_000;
//     edges_000.push_back(e_000_002.backIt());
//     edges_000.push_back(e_000_140.backIt());
//     edges_000.push_back(e_000_028.backIt());
//     edges_000.push_back(e_110_000.backIt());
//     edges_000.push_back(e_000_029.backIt());
//     edges_000.push_back(e_000_001.backIt());
//     edges_000.push_back(e_114_000.backIt());
//     edges_000.push_back(e_000_108.backIt());
//     edges_000.push_back(e_000_109.backIt());
//     n_000.reorderIncidence(std::move(edges_000));
// 
//     vector<edge_list_it_t> edges_001;
//     edges_001.push_back(e_002_001.backIt());
//     edges_001.push_back(e_001_029.backIt());
//     edges_001.push_back(e_000_001.backIt());
//     edges_001.push_back(e_001_004.backIt());
//     n_001.reorderIncidence(std::move(edges_001));
// 
//     vector<edge_list_it_t> edges_002;
//     edges_002.push_back(e_121_002.backIt());
//     edges_002.push_back(e_002_006.backIt());
//     edges_002.push_back(e_032_002.backIt());
//     edges_002.push_back(e_000_002.backIt());
//     edges_002.push_back(e_002_065.backIt());
//     edges_002.push_back(e_037_002.backIt());
//     edges_002.push_back(e_031_002.backIt());
//     edges_002.push_back(e_007_002.backIt());
//     edges_002.push_back(e_002_003.backIt());
//     edges_002.push_back(e_004_002.backIt());
//     edges_002.push_back(e_038_002.backIt());
//     edges_002.push_back(e_039_002.backIt());
//     edges_002.push_back(e_135_002.backIt());
//     edges_002.push_back(e_002_001.backIt());
//     edges_002.push_back(e_030_002.backIt());
//     edges_002.push_back(e_002_088.backIt());
//     n_002.reorderIncidence(std::move(edges_002));
// 
//     vector<edge_list_it_t> edges_003;
//     edges_003.push_back(e_002_003.backIt());
//     edges_003.push_back(e_003_005.backIt());
//     edges_003.push_back(e_004_003.backIt());
//     edges_003.push_back(e_003_006.backIt());
//     n_003.reorderIncidence(std::move(edges_003));
// 
//     vector<edge_list_it_t> edges_004;
//     edges_004.push_back(e_016_004.backIt());
//     edges_004.push_back(e_004_002.backIt());
//     edges_004.push_back(e_001_004.backIt());
//     edges_004.push_back(e_004_003.backIt());
//     n_004.reorderIncidence(std::move(edges_004));
// 
//     vector<edge_list_it_t> edges_005;
//     edges_005.push_back(e_142_005.backIt());
//     edges_005.push_back(e_005_006.backIt());
//     edges_005.push_back(e_013_005.backIt());
//     edges_005.push_back(e_003_005.backIt());
//     edges_005.push_back(e_020_005.backIt());
//     edges_005.push_back(e_016_005.backIt());
//     edges_005.push_back(e_018_005.backIt());
//     edges_005.push_back(e_019_005.backIt());
//     edges_005.push_back(e_015_005.backIt());
//     n_005.reorderIncidence(std::move(edges_005));
// 
//     vector<edge_list_it_t> edges_006;
//     edges_006.push_back(e_002_006.backIt());
//     edges_006.push_back(e_005_006.backIt());
//     edges_006.push_back(e_015_006.backIt());
//     edges_006.push_back(e_003_006.backIt());
//     edges_006.push_back(e_007_006.backIt());
//     n_006.reorderIncidence(std::move(edges_006));
// 
//     vector<edge_list_it_t> edges_007;
//     edges_007.push_back(e_014_007.backIt());
//     edges_007.push_back(e_007_002.backIt());
//     edges_007.push_back(e_009_007.backIt());
//     edges_007.push_back(e_007_006.backIt());
//     n_007.reorderIncidence(std::move(edges_007));
// 
//     vector<edge_list_it_t> edges_008;
//     edges_008.push_back(e_010_008.backIt());
//     edges_008.push_back(e_008_065.backIt());
//     edges_008.push_back(e_066_008.backIt());
//     edges_008.push_back(e_008_009.backIt());
//     n_008.reorderIncidence(std::move(edges_008));
// 
//     vector<edge_list_it_t> edges_009;
//     edges_009.push_back(e_010_009.backIt());
//     edges_009.push_back(e_009_007.backIt());
//     edges_009.push_back(e_008_009.backIt());
//     edges_009.push_back(e_009_012.backIt());
//     n_009.reorderIncidence(std::move(edges_009));
// 
//     vector<edge_list_it_t> edges_010;
//     edges_010.push_back(e_011_010.backIt());
//     edges_010.push_back(e_010_008.backIt());
//     edges_010.push_back(e_067_010.backIt());
//     edges_010.push_back(e_010_009.backIt());
//     n_010.reorderIncidence(std::move(edges_010));
// 
//     vector<edge_list_it_t> edges_011;
//     edges_011.push_back(e_021_011.backIt());
//     edges_011.push_back(e_011_010.backIt());
//     edges_011.push_back(e_068_011.backIt());
//     edges_011.push_back(e_011_012.backIt());
//     n_011.reorderIncidence(std::move(edges_011));
// 
//     vector<edge_list_it_t> edges_012;
//     edges_012.push_back(e_011_012.backIt());
//     edges_012.push_back(e_012_014.backIt());
//     edges_012.push_back(e_009_012.backIt());
//     edges_012.push_back(e_012_013.backIt());
//     n_012.reorderIncidence(std::move(edges_012));
// 
//     vector<edge_list_it_t> edges_013;
//     edges_013.push_back(e_013_020.backIt());
//     edges_013.push_back(e_022_013.backIt());
//     edges_013.push_back(e_013_005.backIt());
//     edges_013.push_back(e_013_015.backIt());
//     edges_013.push_back(e_013_014.backIt());
//     edges_013.push_back(e_013_025.backIt());
//     edges_013.push_back(e_013_023.backIt());
//     edges_013.push_back(e_013_021.backIt());
//     edges_013.push_back(e_012_013.backIt());
//     n_013.reorderIncidence(std::move(edges_013));
// 
//     vector<edge_list_it_t> edges_014;
//     edges_014.push_back(e_013_014.backIt());
//     edges_014.push_back(e_014_007.backIt());
//     edges_014.push_back(e_012_014.backIt());
//     edges_014.push_back(e_014_015.backIt());
//     n_014.reorderIncidence(std::move(edges_014));
// 
//     vector<edge_list_it_t> edges_015;
//     edges_015.push_back(e_013_015.backIt());
//     edges_015.push_back(e_015_006.backIt());
//     edges_015.push_back(e_014_015.backIt());
//     edges_015.push_back(e_015_005.backIt());
//     n_015.reorderIncidence(std::move(edges_015));
// 
//     vector<edge_list_it_t> edges_016;
//     edges_016.push_back(e_141_016.backIt());
//     edges_016.push_back(e_016_004.backIt());
//     edges_016.push_back(e_029_016.backIt());
//     edges_016.push_back(e_016_005.backIt());
//     n_016.reorderIncidence(std::move(edges_016));
// 
//     vector<edge_list_it_t> edges_017;
//     edges_017.push_back(e_018_017.backIt());
//     edges_017.push_back(e_017_141.backIt());
//     edges_017.push_back(e_017_142.backIt());
//     edges_017.push_back(e_017_028.backIt());
//     n_017.reorderIncidence(std::move(edges_017));
// 
//     vector<edge_list_it_t> edges_018;
//     edges_018.push_back(e_019_018.backIt());
//     edges_018.push_back(e_018_017.backIt());
//     edges_018.push_back(e_028_018.backIt());
//     edges_018.push_back(e_018_005.backIt());
//     n_018.reorderIncidence(std::move(edges_018));
// 
//     vector<edge_list_it_t> edges_019;
//     edges_019.push_back(e_020_019.backIt());
//     edges_019.push_back(e_019_018.backIt());
//     edges_019.push_back(e_027_019.backIt());
//     edges_019.push_back(e_019_005.backIt());
//     n_019.reorderIncidence(std::move(edges_019));
// 
//     vector<edge_list_it_t> edges_020;
//     edges_020.push_back(e_013_020.backIt());
//     edges_020.push_back(e_020_019.backIt());
//     edges_020.push_back(e_025_020.backIt());
//     edges_020.push_back(e_020_005.backIt());
//     n_020.reorderIncidence(std::move(edges_020));
// 
//     vector<edge_list_it_t> edges_021;
//     edges_021.push_back(e_022_021.backIt());
//     edges_021.push_back(e_021_011.backIt());
//     edges_021.push_back(e_013_021.backIt());
//     edges_021.push_back(e_021_070.backIt());
//     n_021.reorderIncidence(std::move(edges_021));
// 
//     vector<edge_list_it_t> edges_022;
//     edges_022.push_back(e_022_021.backIt());
//     edges_022.push_back(e_099_022.backIt());
//     edges_022.push_back(e_022_013.backIt());
//     edges_022.push_back(e_022_096.backIt());
//     edges_022.push_back(e_022_071.backIt());
//     edges_022.push_back(e_022_024.backIt());
//     edges_022.push_back(e_072_022.backIt());
//     edges_022.push_back(e_022_097.backIt());
//     edges_022.push_back(e_022_023.backIt());
//     edges_022.push_back(e_022_117.backIt());
//     edges_022.push_back(e_022_070.backIt());
//     n_022.reorderIncidence(std::move(edges_022));
// 
//     vector<edge_list_it_t> edges_023;
//     edges_023.push_back(e_022_023.backIt());
//     edges_023.push_back(e_023_025.backIt());
//     edges_023.push_back(e_013_023.backIt());
//     edges_023.push_back(e_023_024.backIt());
//     n_023.reorderIncidence(std::move(edges_023));
// 
//     vector<edge_list_it_t> edges_024;
//     edges_024.push_back(e_022_024.backIt());
//     edges_024.push_back(e_024_026.backIt());
//     edges_024.push_back(e_023_024.backIt());
//     edges_024.push_back(e_024_117.backIt());
//     n_024.reorderIncidence(std::move(edges_024));
// 
//     vector<edge_list_it_t> edges_025;
//     edges_025.push_back(e_013_025.backIt());
//     edges_025.push_back(e_025_027.backIt());
//     edges_025.push_back(e_023_025.backIt());
//     edges_025.push_back(e_025_020.backIt());
//     n_025.reorderIncidence(std::move(edges_025));
// 
//     vector<edge_list_it_t> edges_026;
//     edges_026.push_back(e_024_026.backIt());
//     edges_026.push_back(e_026_028.backIt());
//     edges_026.push_back(e_116_026.backIt());
//     edges_026.push_back(e_026_027.backIt());
//     n_026.reorderIncidence(std::move(edges_026));
// 
//     vector<edge_list_it_t> edges_027;
//     edges_027.push_back(e_025_027.backIt());
//     edges_027.push_back(e_027_028.backIt());
//     edges_027.push_back(e_026_027.backIt());
//     edges_027.push_back(e_027_019.backIt());
//     n_027.reorderIncidence(std::move(edges_027));
// 
//     vector<edge_list_it_t> edges_028;
//     edges_028.push_back(e_115_028.backIt());
//     edges_028.push_back(e_026_028.backIt());
//     edges_028.push_back(e_000_028.backIt());
//     edges_028.push_back(e_029_028.backIt());
//     edges_028.push_back(e_017_028.backIt());
//     edges_028.push_back(e_027_028.backIt());
//     edges_028.push_back(e_028_018.backIt());
//     edges_028.push_back(e_114_028.backIt());
//     n_028.reorderIncidence(std::move(edges_028));
// 
//     vector<edge_list_it_t> edges_029;
//     edges_029.push_back(e_001_029.backIt());
//     edges_029.push_back(e_029_028.backIt());
//     edges_029.push_back(e_000_029.backIt());
//     edges_029.push_back(e_029_016.backIt());
//     n_029.reorderIncidence(std::move(edges_029));
// 
//     vector<edge_list_it_t> edges_030;
//     edges_030.push_back(e_109_030.backIt());
//     edges_030.push_back(e_030_002.backIt());
//     edges_030.push_back(e_140_030.backIt());
//     edges_030.push_back(e_030_032.backIt());
//     n_030.reorderIncidence(std::move(edges_030));
// 
//     vector<edge_list_it_t> edges_031;
//     edges_031.push_back(e_034_031.backIt());
//     edges_031.push_back(e_031_002.backIt());
//     edges_031.push_back(e_032_031.backIt());
//     edges_031.push_back(e_031_037.backIt());
//     n_031.reorderIncidence(std::move(edges_031));
// 
//     vector<edge_list_it_t> edges_032;
//     edges_032.push_back(e_033_032.backIt());
//     edges_032.push_back(e_032_002.backIt());
//     edges_032.push_back(e_030_032.backIt());
//     edges_032.push_back(e_032_031.backIt());
//     n_032.reorderIncidence(std::move(edges_032));
// 
//     vector<edge_list_it_t> edges_033;
//     edges_033.push_back(e_035_033.backIt());
//     edges_033.push_back(e_033_032.backIt());
//     edges_033.push_back(e_109_033.backIt());
//     edges_033.push_back(e_033_034.backIt());
//     n_033.reorderIncidence(std::move(edges_033));
// 
//     vector<edge_list_it_t> edges_034;
//     edges_034.push_back(e_137_034.backIt());
//     edges_034.push_back(e_034_031.backIt());
//     edges_034.push_back(e_033_034.backIt());
//     edges_034.push_back(e_034_036.backIt());
//     n_034.reorderIncidence(std::move(edges_034));
// 
//     vector<edge_list_it_t> edges_035;
//     edges_035.push_back(e_035_033.backIt());
//     edges_035.push_back(e_035_098.backIt());
//     edges_035.push_back(e_035_138.backIt());
//     edges_035.push_back(e_035_036.backIt());
//     edges_035.push_back(e_139_035.backIt());
//     edges_035.push_back(e_097_035.backIt());
//     edges_035.push_back(e_108_035.backIt());
//     n_035.reorderIncidence(std::move(edges_035));
// 
//     vector<edge_list_it_t> edges_036;
//     edges_036.push_back(e_122_036.backIt());
//     edges_036.push_back(e_036_037.backIt());
//     edges_036.push_back(e_035_036.backIt());
//     edges_036.push_back(e_034_036.backIt());
//     edges_036.push_back(e_036_097.backIt());
//     edges_036.push_back(e_036_094.backIt());
//     edges_036.push_back(e_096_036.backIt());
//     edges_036.push_back(e_036_095.backIt());
//     edges_036.push_back(e_041_036.backIt());
//     n_036.reorderIncidence(std::move(edges_036));
// 
//     vector<edge_list_it_t> edges_037;
//     edges_037.push_back(e_036_037.backIt());
//     edges_037.push_back(e_037_002.backIt());
//     edges_037.push_back(e_031_037.backIt());
//     edges_037.push_back(e_037_038.backIt());
//     n_037.reorderIncidence(std::move(edges_037));
// 
//     vector<edge_list_it_t> edges_038;
//     edges_038.push_back(e_040_038.backIt());
//     edges_038.push_back(e_038_002.backIt());
//     edges_038.push_back(e_037_038.backIt());
//     edges_038.push_back(e_038_039.backIt());
//     n_038.reorderIncidence(std::move(edges_038));
// 
//     vector<edge_list_it_t> edges_039;
//     edges_039.push_back(e_051_039.backIt());
//     edges_039.push_back(e_039_002.backIt());
//     edges_039.push_back(e_038_039.backIt());
//     edges_039.push_back(e_039_060.backIt());
//     n_039.reorderIncidence(std::move(edges_039));
// 
//     vector<edge_list_it_t> edges_040;
//     edges_040.push_back(e_049_040.backIt());
//     edges_040.push_back(e_040_038.backIt());
//     edges_040.push_back(e_051_040.backIt());
//     edges_040.push_back(e_040_041.backIt());
//     n_040.reorderIncidence(std::move(edges_040));
// 
//     vector<edge_list_it_t> edges_041;
//     edges_041.push_back(e_041_049.backIt());
//     edges_041.push_back(e_040_041.backIt());
//     edges_041.push_back(e_041_050.backIt());
//     edges_041.push_back(e_041_036.backIt());
//     n_041.reorderIncidence(std::move(edges_041));
// 
//     vector<edge_list_it_t> edges_042;
//     edges_042.push_back(e_044_042.backIt());
//     edges_042.push_back(e_122_042.backIt());
//     edges_042.push_back(e_042_043.backIt());
//     edges_042.push_back(e_123_042.backIt());
//     edges_042.push_back(e_050_042.backIt());
//     n_042.reorderIncidence(std::move(edges_042));
// 
//     vector<edge_list_it_t> edges_043;
//     edges_043.push_back(e_042_043.backIt());
//     edges_043.push_back(e_043_046.backIt());
//     edges_043.push_back(e_048_043.backIt());
//     edges_043.push_back(e_044_043.backIt());
//     n_043.reorderIncidence(std::move(edges_043));
// 
//     vector<edge_list_it_t> edges_044;
//     edges_044.push_back(e_044_042.backIt());
//     edges_044.push_back(e_094_044.backIt());
//     edges_044.push_back(e_044_123.backIt());
//     edges_044.push_back(e_044_043.backIt());
//     n_044.reorderIncidence(std::move(edges_044));
// 
//     vector<edge_list_it_t> edges_045;
//     edges_045.push_back(e_075_045.backIt());
//     edges_045.push_back(e_045_047.backIt());
//     edges_045.push_back(e_076_045.backIt());
//     edges_045.push_back(e_045_046.backIt());
//     n_045.reorderIncidence(std::move(edges_045));
// 
//     vector<edge_list_it_t> edges_046;
//     edges_046.push_back(e_043_046.backIt());
//     edges_046.push_back(e_047_046.backIt());
//     edges_046.push_back(e_045_046.backIt());
//     edges_046.push_back(e_048_046.backIt());
//     n_046.reorderIncidence(std::move(edges_046));
// 
//     vector<edge_list_it_t> edges_047;
//     edges_047.push_back(e_045_047.backIt());
//     edges_047.push_back(e_047_049.backIt());
//     edges_047.push_back(e_077_047.backIt());
//     edges_047.push_back(e_047_046.backIt());
//     n_047.reorderIncidence(std::move(edges_047));
// 
//     vector<edge_list_it_t> edges_048;
//     edges_048.push_back(e_049_048.backIt());
//     edges_048.push_back(e_048_043.backIt());
//     edges_048.push_back(e_050_048.backIt());
//     edges_048.push_back(e_048_046.backIt());
//     n_048.reorderIncidence(std::move(edges_048));
// 
//     vector<edge_list_it_t> edges_049;
//     edges_049.push_back(e_125_049.backIt());
//     edges_049.push_back(e_059_049.backIt());
//     edges_049.push_back(e_052_049.backIt());
//     edges_049.push_back(e_047_049.backIt());
//     edges_049.push_back(e_053_049.backIt());
//     edges_049.push_back(e_049_077.backIt());
//     edges_049.push_back(e_049_040.backIt());
//     edges_049.push_back(e_041_049.backIt());
//     edges_049.push_back(e_054_049.backIt());
//     edges_049.push_back(e_049_050.backIt());
//     edges_049.push_back(e_049_048.backIt());
//     n_049.reorderIncidence(std::move(edges_049));
// 
//     vector<edge_list_it_t> edges_050;
//     edges_050.push_back(e_049_050.backIt());
//     edges_050.push_back(e_050_042.backIt());
//     edges_050.push_back(e_041_050.backIt());
//     edges_050.push_back(e_050_048.backIt());
//     n_050.reorderIncidence(std::move(edges_050));
// 
//     vector<edge_list_it_t> edges_051;
//     edges_051.push_back(e_059_051.backIt());
//     edges_051.push_back(e_051_039.backIt());
//     edges_051.push_back(e_054_051.backIt());
//     edges_051.push_back(e_051_040.backIt());
//     n_051.reorderIncidence(std::move(edges_051));
// 
//     vector<edge_list_it_t> edges_052;
//     edges_052.push_back(e_052_049.backIt());
//     edges_052.push_back(e_052_136.backIt());
//     edges_052.push_back(e_053_052.backIt());
//     edges_052.push_back(e_052_054.backIt());
//     n_052.reorderIncidence(std::move(edges_052));
// 
//     vector<edge_list_it_t> edges_053;
//     edges_053.push_back(e_053_079.backIt());
//     edges_053.push_back(e_053_055.backIt());
//     edges_053.push_back(e_053_081.backIt());
//     edges_053.push_back(e_053_049.backIt());
//     edges_053.push_back(e_053_078.backIt());
//     edges_053.push_back(e_053_054.backIt());
//     edges_053.push_back(e_053_052.backIt());
//     edges_053.push_back(e_053_136.backIt());
//     n_053.reorderIncidence(std::move(edges_053));
// 
//     vector<edge_list_it_t> edges_054;
//     edges_054.push_back(e_059_054.backIt());
//     edges_054.push_back(e_057_054.backIt());
//     edges_054.push_back(e_054_055.backIt());
//     edges_054.push_back(e_053_054.backIt());
//     edges_054.push_back(e_054_049.backIt());
//     edges_054.push_back(e_052_054.backIt());
//     edges_054.push_back(e_054_058.backIt());
//     edges_054.push_back(e_056_054.backIt());
//     edges_054.push_back(e_054_051.backIt());
//     n_054.reorderIncidence(std::move(edges_054));
// 
//     vector<edge_list_it_t> edges_055;
//     edges_055.push_back(e_056_055.backIt());
//     edges_055.push_back(e_082_055.backIt());
//     edges_055.push_back(e_053_055.backIt());
//     edges_055.push_back(e_054_055.backIt());
//     edges_055.push_back(e_055_081.backIt());
//     n_055.reorderIncidence(std::move(edges_055));
// 
//     vector<edge_list_it_t> edges_056;
//     edges_056.push_back(e_057_056.backIt());
//     edges_056.push_back(e_056_055.backIt());
//     edges_056.push_back(e_082_056.backIt());
//     edges_056.push_back(e_056_054.backIt());
//     n_056.reorderIncidence(std::move(edges_056));
// 
//     vector<edge_list_it_t> edges_057;
//     edges_057.push_back(e_057_056.backIt());
//     edges_057.push_back(e_057_058.backIt());
//     edges_057.push_back(e_057_054.backIt());
//     edges_057.push_back(e_057_086.backIt());
//     edges_057.push_back(e_057_085.backIt());
//     edges_057.push_back(e_082_057.backIt());
//     edges_057.push_back(e_057_090.backIt());
//     n_057.reorderIncidence(std::move(edges_057));
// 
//     vector<edge_list_it_t> edges_058;
//     edges_058.push_back(e_057_058.backIt());
//     edges_058.push_back(e_060_058.backIt());
//     edges_058.push_back(e_054_058.backIt());
//     n_058.reorderIncidence(std::move(edges_058));
// 
//     vector<edge_list_it_t> edges_059;
//     edges_059.push_back(e_059_054.backIt());
//     edges_059.push_back(e_059_049.backIt());
//     edges_059.push_back(e_059_051.backIt());
//     n_059.reorderIncidence(std::move(edges_059));
// 
//     vector<edge_list_it_t> edges_060;
//     edges_060.push_back(e_088_060.backIt());
//     edges_060.push_back(e_060_058.backIt());
//     edges_060.push_back(e_039_060.backIt());
//     edges_060.push_back(e_060_086.backIt());
//     n_060.reorderIncidence(std::move(edges_060));
// 
//     vector<edge_list_it_t> edges_061;
//     edges_061.push_back(e_135_061.backIt());
//     edges_061.push_back(e_061_063.backIt());
//     edges_061.push_back(e_064_061.backIt());
//     edges_061.push_back(e_061_062.backIt());
//     n_061.reorderIncidence(std::move(edges_061));
// 
//     vector<edge_list_it_t> edges_062;
//     edges_062.push_back(e_062_089.backIt());
//     edges_062.push_back(e_062_063.backIt());
//     edges_062.push_back(e_062_088.backIt());
//     edges_062.push_back(e_062_087.backIt());
//     edges_062.push_back(e_061_062.backIt());
//     n_062.reorderIncidence(std::move(edges_062));
// 
//     vector<edge_list_it_t> edges_063;
//     edges_063.push_back(e_064_063.backIt());
//     edges_063.push_back(e_063_087.backIt());
//     edges_063.push_back(e_063_089.backIt());
//     edges_063.push_back(e_062_063.backIt());
//     edges_063.push_back(e_093_063.backIt());
//     edges_063.push_back(e_092_063.backIt());
//     edges_063.push_back(e_061_063.backIt());
//     edges_063.push_back(e_091_063.backIt());
//     n_063.reorderIncidence(std::move(edges_063));
// 
//     vector<edge_list_it_t> edges_064;
//     edges_064.push_back(e_065_064.backIt());
//     edges_064.push_back(e_064_063.backIt());
//     edges_064.push_back(e_066_064.backIt());
//     edges_064.push_back(e_064_061.backIt());
//     n_064.reorderIncidence(std::move(edges_064));
// 
//     vector<edge_list_it_t> edges_065;
//     edges_065.push_back(e_008_065.backIt());
//     edges_065.push_back(e_065_064.backIt());
//     edges_065.push_back(e_121_065.backIt());
//     edges_065.push_back(e_002_065.backIt());
//     n_065.reorderIncidence(std::move(edges_065));
// 
//     vector<edge_list_it_t> edges_066;
//     edges_066.push_back(e_067_066.backIt());
//     edges_066.push_back(e_066_064.backIt());
//     edges_066.push_back(e_093_066.backIt());
//     edges_066.push_back(e_066_008.backIt());
//     n_066.reorderIncidence(std::move(edges_066));
// 
//     vector<edge_list_it_t> edges_067;
//     edges_067.push_back(e_074_067.backIt());
//     edges_067.push_back(e_067_010.backIt());
//     edges_067.push_back(e_068_067.backIt());
//     edges_067.push_back(e_067_066.backIt());
//     n_067.reorderIncidence(std::move(edges_067));
// 
//     vector<edge_list_it_t> edges_068;
//     edges_068.push_back(e_069_068.backIt());
//     edges_068.push_back(e_068_011.backIt());
//     edges_068.push_back(e_070_068.backIt());
//     edges_068.push_back(e_068_067.backIt());
//     n_068.reorderIncidence(std::move(edges_068));
// 
//     vector<edge_list_it_t> edges_069;
//     edges_069.push_back(e_071_069.backIt());
//     edges_069.push_back(e_069_074.backIt());
//     edges_069.push_back(e_073_069.backIt());
//     edges_069.push_back(e_069_068.backIt());
//     n_069.reorderIncidence(std::move(edges_069));
// 
//     vector<edge_list_it_t> edges_070;
//     edges_070.push_back(e_021_070.backIt());
//     edges_070.push_back(e_070_071.backIt());
//     edges_070.push_back(e_022_070.backIt());
//     edges_070.push_back(e_070_068.backIt());
//     n_070.reorderIncidence(std::move(edges_070));
// 
//     vector<edge_list_it_t> edges_071;
//     edges_071.push_back(e_022_071.backIt());
//     edges_071.push_back(e_071_069.backIt());
//     edges_071.push_back(e_070_071.backIt());
//     edges_071.push_back(e_071_072.backIt());
//     n_071.reorderIncidence(std::move(edges_071));
// 
//     vector<edge_list_it_t> edges_072;
//     edges_072.push_back(e_124_072.backIt());
//     edges_072.push_back(e_072_022.backIt());
//     edges_072.push_back(e_071_072.backIt());
//     edges_072.push_back(e_072_096.backIt());
//     n_072.reorderIncidence(std::move(edges_072));
// 
//     vector<edge_list_it_t> edges_073;
//     edges_073.push_back(e_124_073.backIt());
//     edges_073.push_back(e_073_075.backIt());
//     edges_073.push_back(e_095_073.backIt());
//     edges_073.push_back(e_073_069.backIt());
//     n_073.reorderIncidence(std::move(edges_073));
// 
//     vector<edge_list_it_t> edges_074;
//     edges_074.push_back(e_069_074.backIt());
//     edges_074.push_back(e_074_093.backIt());
//     edges_074.push_back(e_075_074.backIt());
//     edges_074.push_back(e_074_067.backIt());
//     n_074.reorderIncidence(std::move(edges_074));
// 
//     vector<edge_list_it_t> edges_075;
//     edges_075.push_back(e_073_075.backIt());
//     edges_075.push_back(e_075_045.backIt());
//     edges_075.push_back(e_094_075.backIt());
//     edges_075.push_back(e_075_074.backIt());
//     n_075.reorderIncidence(std::move(edges_075));
// 
//     vector<edge_list_it_t> edges_076;
//     edges_076.push_back(e_077_076.backIt());
//     edges_076.push_back(e_076_092.backIt());
//     edges_076.push_back(e_083_076.backIt());
//     edges_076.push_back(e_076_045.backIt());
//     n_076.reorderIncidence(std::move(edges_076));
// 
//     vector<edge_list_it_t> edges_077;
//     edges_077.push_back(e_049_077.backIt());
//     edges_077.push_back(e_077_076.backIt());
//     edges_077.push_back(e_080_077.backIt());
//     edges_077.push_back(e_077_047.backIt());
//     n_077.reorderIncidence(std::move(edges_077));
// 
//     vector<edge_list_it_t> edges_078;
//     edges_078.push_back(e_053_078.backIt());
//     edges_078.push_back(e_078_079.backIt());
//     edges_078.push_back(e_078_080.backIt());
//     n_078.reorderIncidence(std::move(edges_078));
// 
//     vector<edge_list_it_t> edges_079;
//     edges_079.push_back(e_126_079.backIt());
//     edges_079.push_back(e_053_079.backIt());
//     edges_079.push_back(e_079_128.backIt());
//     edges_079.push_back(e_079_127.backIt());
//     edges_079.push_back(e_078_079.backIt());
//     edges_079.push_back(e_079_129.backIt());
//     n_079.reorderIncidence(std::move(edges_079));
// 
//     vector<edge_list_it_t> edges_080;
//     edges_080.push_back(e_078_080.backIt());
//     edges_080.push_back(e_080_083.backIt());
//     edges_080.push_back(e_081_080.backIt());
//     edges_080.push_back(e_080_077.backIt());
//     n_080.reorderIncidence(std::move(edges_080));
// 
//     vector<edge_list_it_t> edges_081;
//     edges_081.push_back(e_053_081.backIt());
//     edges_081.push_back(e_081_082.backIt());
//     edges_081.push_back(e_055_081.backIt());
//     edges_081.push_back(e_081_080.backIt());
//     n_081.reorderIncidence(std::move(edges_081));
// 
//     vector<edge_list_it_t> edges_082;
//     edges_082.push_back(e_082_055.backIt());
//     edges_082.push_back(e_082_134.backIt());
//     edges_082.push_back(e_081_082.backIt());
//     edges_082.push_back(e_082_133.backIt());
//     edges_082.push_back(e_082_085.backIt());
//     edges_082.push_back(e_082_084.backIt());
//     edges_082.push_back(e_082_057.backIt());
//     edges_082.push_back(e_082_056.backIt());
//     edges_082.push_back(e_082_083.backIt());
//     n_082.reorderIncidence(std::move(edges_082));
// 
//     vector<edge_list_it_t> edges_083;
//     edges_083.push_back(e_080_083.backIt());
//     edges_083.push_back(e_083_091.backIt());
//     edges_083.push_back(e_082_083.backIt());
//     edges_083.push_back(e_083_076.backIt());
//     n_083.reorderIncidence(std::move(edges_083));
// 
//     vector<edge_list_it_t> edges_084;
//     edges_084.push_back(e_084_085.backIt());
//     edges_084.push_back(e_084_131.backIt());
//     edges_084.push_back(e_082_084.backIt());
//     edges_084.push_back(e_084_091.backIt());
//     edges_084.push_back(e_090_084.backIt());
//     n_084.reorderIncidence(std::move(edges_084));
// 
//     vector<edge_list_it_t> edges_085;
//     edges_085.push_back(e_084_085.backIt());
//     edges_085.push_back(e_085_131.backIt());
//     edges_085.push_back(e_082_085.backIt());
//     edges_085.push_back(e_133_085.backIt());
//     edges_085.push_back(e_057_085.backIt());
//     edges_085.push_back(e_090_085.backIt());
//     n_085.reorderIncidence(std::move(edges_085));
// 
//     vector<edge_list_it_t> edges_086;
//     edges_086.push_back(e_057_086.backIt());
//     edges_086.push_back(e_086_087.backIt());
//     edges_086.push_back(e_060_086.backIt());
//     edges_086.push_back(e_086_090.backIt());
//     n_086.reorderIncidence(std::move(edges_086));
// 
//     vector<edge_list_it_t> edges_087;
//     edges_087.push_back(e_132_087.backIt());
//     edges_087.push_back(e_063_087.backIt());
//     edges_087.push_back(e_087_089.backIt());
//     edges_087.push_back(e_086_087.backIt());
//     edges_087.push_back(e_062_087.backIt());
//     n_087.reorderIncidence(std::move(edges_087));
// 
//     vector<edge_list_it_t> edges_088;
//     edges_088.push_back(e_062_088.backIt());
//     edges_088.push_back(e_088_060.backIt());
//     edges_088.push_back(e_002_088.backIt());
//     edges_088.push_back(e_088_132.backIt());
//     n_088.reorderIncidence(std::move(edges_088));
// 
//     vector<edge_list_it_t> edges_089;
//     edges_089.push_back(e_063_089.backIt());
//     edges_089.push_back(e_062_089.backIt());
//     edges_089.push_back(e_087_089.backIt());
//     n_089.reorderIncidence(std::move(edges_089));
// 
//     vector<edge_list_it_t> edges_090;
//     edges_090.push_back(e_086_090.backIt());
//     edges_090.push_back(e_090_085.backIt());
//     edges_090.push_back(e_057_090.backIt());
//     edges_090.push_back(e_090_084.backIt());
//     n_090.reorderIncidence(std::move(edges_090));
// 
//     vector<edge_list_it_t> edges_091;
//     edges_091.push_back(e_083_091.backIt());
//     edges_091.push_back(e_091_063.backIt());
//     edges_091.push_back(e_084_091.backIt());
//     edges_091.push_back(e_091_092.backIt());
//     n_091.reorderIncidence(std::move(edges_091));
// 
//     vector<edge_list_it_t> edges_092;
//     edges_092.push_back(e_076_092.backIt());
//     edges_092.push_back(e_092_063.backIt());
//     edges_092.push_back(e_091_092.backIt());
//     edges_092.push_back(e_092_093.backIt());
//     n_092.reorderIncidence(std::move(edges_092));
// 
//     vector<edge_list_it_t> edges_093;
//     edges_093.push_back(e_074_093.backIt());
//     edges_093.push_back(e_093_063.backIt());
//     edges_093.push_back(e_092_093.backIt());
//     edges_093.push_back(e_093_066.backIt());
//     n_093.reorderIncidence(std::move(edges_093));
// 
//     vector<edge_list_it_t> edges_094;
//     edges_094.push_back(e_095_094.backIt());
//     edges_094.push_back(e_094_044.backIt());
//     edges_094.push_back(e_036_094.backIt());
//     edges_094.push_back(e_094_075.backIt());
//     n_094.reorderIncidence(std::move(edges_094));
// 
//     vector<edge_list_it_t> edges_095;
//     edges_095.push_back(e_096_095.backIt());
//     edges_095.push_back(e_095_094.backIt());
//     edges_095.push_back(e_036_095.backIt());
//     edges_095.push_back(e_095_073.backIt());
//     n_095.reorderIncidence(std::move(edges_095));
// 
//     vector<edge_list_it_t> edges_096;
//     edges_096.push_back(e_022_096.backIt());
//     edges_096.push_back(e_096_095.backIt());
//     edges_096.push_back(e_072_096.backIt());
//     edges_096.push_back(e_096_036.backIt());
//     n_096.reorderIncidence(std::move(edges_096));
// 
//     vector<edge_list_it_t> edges_097;
//     edges_097.push_back(e_022_097.backIt());
//     edges_097.push_back(e_097_035.backIt());
//     edges_097.push_back(e_036_097.backIt());
//     edges_097.push_back(e_097_098.backIt());
//     n_097.reorderIncidence(std::move(edges_097));
// 
//     vector<edge_list_it_t> edges_098;
//     edges_098.push_back(e_035_098.backIt());
//     edges_098.push_back(e_098_100.backIt());
//     edges_098.push_back(e_097_098.backIt());
//     edges_098.push_back(e_098_107.backIt());
//     n_098.reorderIncidence(std::move(edges_098));
// 
//     vector<edge_list_it_t> edges_099;
//     edges_099.push_back(e_101_099.backIt());
//     edges_099.push_back(e_099_022.backIt());
//     edges_099.push_back(e_117_099.backIt());
//     edges_099.push_back(e_099_100.backIt());
//     n_099.reorderIncidence(std::move(edges_099));
// 
//     vector<edge_list_it_t> edges_100;
//     edges_100.push_back(e_098_100.backIt());
//     edges_100.push_back(e_100_106.backIt());
//     edges_100.push_back(e_099_100.backIt());
//     edges_100.push_back(e_100_105.backIt());
//     n_100.reorderIncidence(std::move(edges_100));
// 
//     vector<edge_list_it_t> edges_101;
//     edges_101.push_back(e_101_103.backIt());
//     edges_101.push_back(e_101_099.backIt());
//     edges_101.push_back(e_101_118.backIt());
//     edges_101.push_back(e_101_102.backIt());
//     edges_101.push_back(e_101_119.backIt());
//     edges_101.push_back(e_101_106.backIt());
//     n_101.reorderIncidence(std::move(edges_101));
// 
//     vector<edge_list_it_t> edges_102;
//     edges_102.push_back(e_106_102.backIt());
//     edges_102.push_back(e_101_102.backIt());
//     edges_102.push_back(e_103_102.backIt());
//     edges_102.push_back(e_104_102.backIt());
//     edges_102.push_back(e_105_102.backIt());
//     n_102.reorderIncidence(std::move(edges_102));
// 
//     vector<edge_list_it_t> edges_103;
//     edges_103.push_back(e_101_103.backIt());
//     edges_103.push_back(e_103_104.backIt());
//     edges_103.push_back(e_112_103.backIt());
//     edges_103.push_back(e_103_102.backIt());
//     edges_103.push_back(e_103_119.backIt());
//     edges_103.push_back(e_103_120.backIt());
//     n_103.reorderIncidence(std::move(edges_103));
// 
//     vector<edge_list_it_t> edges_104;
//     edges_104.push_back(e_103_104.backIt());
//     edges_104.push_back(e_104_105.backIt());
//     edges_104.push_back(e_112_104.backIt());
//     edges_104.push_back(e_104_102.backIt());
//     n_104.reorderIncidence(std::move(edges_104));
// 
//     vector<edge_list_it_t> edges_105;
//     edges_105.push_back(e_112_105.backIt());
//     edges_105.push_back(e_104_105.backIt());
//     edges_105.push_back(e_105_107.backIt());
//     edges_105.push_back(e_105_111.backIt());
//     edges_105.push_back(e_100_105.backIt());
//     edges_105.push_back(e_106_105.backIt());
//     edges_105.push_back(e_105_102.backIt());
//     n_105.reorderIncidence(std::move(edges_105));
// 
//     vector<edge_list_it_t> edges_106;
//     edges_106.push_back(e_100_106.backIt());
//     edges_106.push_back(e_106_102.backIt());
//     edges_106.push_back(e_101_106.backIt());
//     edges_106.push_back(e_106_105.backIt());
//     n_106.reorderIncidence(std::move(edges_106));
// 
//     vector<edge_list_it_t> edges_107;
//     edges_107.push_back(e_105_107.backIt());
//     edges_107.push_back(e_107_108.backIt());
//     edges_107.push_back(e_098_107.backIt());
//     edges_107.push_back(e_107_110.backIt());
//     n_107.reorderIncidence(std::move(edges_107));
// 
//     vector<edge_list_it_t> edges_108;
//     edges_108.push_back(e_107_108.backIt());
//     edges_108.push_back(e_108_109.backIt());
//     edges_108.push_back(e_000_108.backIt());
//     edges_108.push_back(e_108_035.backIt());
//     n_108.reorderIncidence(std::move(edges_108));
// 
//     vector<edge_list_it_t> edges_109;
//     edges_109.push_back(e_108_109.backIt());
//     edges_109.push_back(e_109_030.backIt());
//     edges_109.push_back(e_000_109.backIt());
//     edges_109.push_back(e_109_033.backIt());
//     n_109.reorderIncidence(std::move(edges_109));
// 
//     vector<edge_list_it_t> edges_110;
//     edges_110.push_back(e_111_110.backIt());
//     edges_110.push_back(e_110_000.backIt());
//     edges_110.push_back(e_107_110.backIt());
//     edges_110.push_back(e_110_114.backIt());
//     n_110.reorderIncidence(std::move(edges_110));
// 
//     vector<edge_list_it_t> edges_111;
//     edges_111.push_back(e_112_111.backIt());
//     edges_111.push_back(e_111_110.backIt());
//     edges_111.push_back(e_105_111.backIt());
//     edges_111.push_back(e_111_113.backIt());
//     n_111.reorderIncidence(std::move(edges_111));
// 
//     vector<edge_list_it_t> edges_112;
//     edges_112.push_back(e_112_105.backIt());
//     edges_112.push_back(e_112_113.backIt());
//     edges_112.push_back(e_112_103.backIt());
//     edges_112.push_back(e_112_111.backIt());
//     edges_112.push_back(e_120_112.backIt());
//     edges_112.push_back(e_112_104.backIt());
//     n_112.reorderIncidence(std::move(edges_112));
// 
//     vector<edge_list_it_t> edges_113;
//     edges_113.push_back(e_118_113.backIt());
//     edges_113.push_back(e_112_113.backIt());
//     edges_113.push_back(e_113_115.backIt());
//     edges_113.push_back(e_111_113.backIt());
//     edges_113.push_back(e_120_113.backIt());
//     n_113.reorderIncidence(std::move(edges_113));
// 
//     vector<edge_list_it_t> edges_114;
//     edges_114.push_back(e_115_114.backIt());
//     edges_114.push_back(e_114_000.backIt());
//     edges_114.push_back(e_110_114.backIt());
//     edges_114.push_back(e_114_028.backIt());
//     n_114.reorderIncidence(std::move(edges_114));
// 
//     vector<edge_list_it_t> edges_115;
//     edges_115.push_back(e_113_115.backIt());
//     edges_115.push_back(e_115_028.backIt());
//     edges_115.push_back(e_116_115.backIt());
//     edges_115.push_back(e_115_114.backIt());
//     n_115.reorderIncidence(std::move(edges_115));
// 
//     vector<edge_list_it_t> edges_116;
//     edges_116.push_back(e_117_116.backIt());
//     edges_116.push_back(e_116_115.backIt());
//     edges_116.push_back(e_118_116.backIt());
//     edges_116.push_back(e_116_026.backIt());
//     n_116.reorderIncidence(std::move(edges_116));
// 
//     vector<edge_list_it_t> edges_117;
//     edges_117.push_back(e_022_117.backIt());
//     edges_117.push_back(e_117_116.backIt());
//     edges_117.push_back(e_024_117.backIt());
//     edges_117.push_back(e_117_099.backIt());
//     n_117.reorderIncidence(std::move(edges_117));
// 
//     vector<edge_list_it_t> edges_118;
//     edges_118.push_back(e_101_118.backIt());
//     edges_118.push_back(e_118_113.backIt());
//     edges_118.push_back(e_119_118.backIt());
//     edges_118.push_back(e_118_116.backIt());
//     n_118.reorderIncidence(std::move(edges_118));
// 
//     vector<edge_list_it_t> edges_119;
//     edges_119.push_back(e_101_119.backIt());
//     edges_119.push_back(e_119_120.backIt());
//     edges_119.push_back(e_103_119.backIt());
//     edges_119.push_back(e_119_118.backIt());
//     n_119.reorderIncidence(std::move(edges_119));
// 
//     vector<edge_list_it_t> edges_120;
//     edges_120.push_back(e_119_120.backIt());
//     edges_120.push_back(e_120_112.backIt());
//     edges_120.push_back(e_103_120.backIt());
//     edges_120.push_back(e_120_113.backIt());
//     n_120.reorderIncidence(std::move(edges_120));
// 
//     vector<edge_list_it_t> edges_121;
//     edges_121.push_back(e_121_002.backIt());
//     edges_121.push_back(e_121_065.backIt());
//     n_121.reorderIncidence(std::move(edges_121));
// 
//     vector<edge_list_it_t> edges_122;
//     edges_122.push_back(e_122_036.backIt());
//     edges_122.push_back(e_122_042.backIt());
//     n_122.reorderIncidence(std::move(edges_122));
// 
//     vector<edge_list_it_t> edges_123;
//     edges_123.push_back(e_123_042.backIt());
//     edges_123.push_back(e_044_123.backIt());
//     n_123.reorderIncidence(std::move(edges_123));
// 
//     vector<edge_list_it_t> edges_124;
//     edges_124.push_back(e_124_073.backIt());
//     edges_124.push_back(e_124_072.backIt());
//     n_124.reorderIncidence(std::move(edges_124));
// 
//     vector<edge_list_it_t> edges_125;
//     edges_125.push_back(e_125_049.backIt());
//     edges_125.push_back(e_126_125.backIt());
//     n_125.reorderIncidence(std::move(edges_125));
// 
//     vector<edge_list_it_t> edges_126;
//     edges_126.push_back(e_126_079.backIt());
//     edges_126.push_back(e_126_127.backIt());
//     edges_126.push_back(e_126_125.backIt());
//     n_126.reorderIncidence(std::move(edges_126));
// 
//     vector<edge_list_it_t> edges_127;
//     edges_127.push_back(e_079_127.backIt());
//     edges_127.push_back(e_126_127.backIt());
//     edges_127.push_back(e_128_127.backIt());
//     n_127.reorderIncidence(std::move(edges_127));
// 
//     vector<edge_list_it_t> edges_128;
//     edges_128.push_back(e_079_128.backIt());
//     edges_128.push_back(e_130_128.backIt());
//     edges_128.push_back(e_128_127.backIt());
//     edges_128.push_back(e_129_128.backIt());
//     n_128.reorderIncidence(std::move(edges_128));
// 
//     vector<edge_list_it_t> edges_129;
//     edges_129.push_back(e_130_129.backIt());
//     edges_129.push_back(e_079_129.backIt());
//     edges_129.push_back(e_129_128.backIt());
//     n_129.reorderIncidence(std::move(edges_129));
// 
//     vector<edge_list_it_t> edges_130;
//     edges_130.push_back(e_130_129.backIt());
//     edges_130.push_back(e_130_128.backIt());
//     n_130.reorderIncidence(std::move(edges_130));
// 
//     vector<edge_list_it_t> edges_131;
//     edges_131.push_back(e_085_131.backIt());
//     edges_131.push_back(e_084_131.backIt());
//     n_131.reorderIncidence(std::move(edges_131));
// 
//     vector<edge_list_it_t> edges_132;
//     edges_132.push_back(e_132_087.backIt());
//     edges_132.push_back(e_088_132.backIt());
//     n_132.reorderIncidence(std::move(edges_132));
// 
//     vector<edge_list_it_t> edges_133;
//     edges_133.push_back(e_133_134.backIt());
//     edges_133.push_back(e_082_133.backIt());
//     edges_133.push_back(e_133_085.backIt());
//     n_133.reorderIncidence(std::move(edges_133));
// 
//     vector<edge_list_it_t> edges_134;
//     edges_134.push_back(e_082_134.backIt());
//     edges_134.push_back(e_133_134.backIt());
//     n_134.reorderIncidence(std::move(edges_134));
// 
//     vector<edge_list_it_t> edges_135;
//     edges_135.push_back(e_135_002.backIt());
//     edges_135.push_back(e_135_061.backIt());
//     n_135.reorderIncidence(std::move(edges_135));
// 
//     vector<edge_list_it_t> edges_136;
//     edges_136.push_back(e_052_136.backIt());
//     edges_136.push_back(e_053_136.backIt());
//     n_136.reorderIncidence(std::move(edges_136));
// 
//     vector<edge_list_it_t> edges_137;
//     edges_137.push_back(e_137_138.backIt());
//     edges_137.push_back(e_137_034.backIt());
//     edges_137.push_back(e_137_139.backIt());
//     n_137.reorderIncidence(std::move(edges_137));
// 
//     vector<edge_list_it_t> edges_138;
//     edges_138.push_back(e_139_138.backIt());
//     edges_138.push_back(e_035_138.backIt());
//     edges_138.push_back(e_137_138.backIt());
//     n_138.reorderIncidence(std::move(edges_138));
// 
//     vector<edge_list_it_t> edges_139;
//     edges_139.push_back(e_139_138.backIt());
//     edges_139.push_back(e_137_139.backIt());
//     edges_139.push_back(e_139_035.backIt());
//     n_139.reorderIncidence(std::move(edges_139));
// 
//     vector<edge_list_it_t> edges_140;
//     edges_140.push_back(e_000_140.backIt());
//     edges_140.push_back(e_140_030.backIt());
//     n_140.reorderIncidence(std::move(edges_140));
// 
//     vector<edge_list_it_t> edges_141;
//     edges_141.push_back(e_017_141.backIt());
//     edges_141.push_back(e_141_016.backIt());
//     n_141.reorderIncidence(std::move(edges_141));
// 
//     vector<edge_list_it_t> edges_142;
//     edges_142.push_back(e_142_005.backIt());
//     edges_142.push_back(e_017_142.backIt());
//     n_142.reorderIncidence(std::move(edges_142));
// 
// 
//     vector<node_list_it_t> stOrder;
//     stOrder.push_back(n_000.backIt());
//     stOrder.push_back(n_140.backIt());
//     stOrder.push_back(n_030.backIt());
//     stOrder.push_back(n_032.backIt());
//     stOrder.push_back(n_033.backIt());
//     stOrder.push_back(n_035.backIt());
//     stOrder.push_back(n_098.backIt());
//     stOrder.push_back(n_097.backIt());
//     stOrder.push_back(n_036.backIt());
//     stOrder.push_back(n_122.backIt());
//     stOrder.push_back(n_042.backIt());
//     stOrder.push_back(n_123.backIt());
//     stOrder.push_back(n_044.backIt());
//     stOrder.push_back(n_094.backIt());
//     stOrder.push_back(n_095.backIt());
//     stOrder.push_back(n_096.backIt());
//     stOrder.push_back(n_072.backIt());
//     stOrder.push_back(n_124.backIt());
//     stOrder.push_back(n_073.backIt());
//     stOrder.push_back(n_075.backIt());
//     stOrder.push_back(n_045.backIt());
//     stOrder.push_back(n_047.backIt());
//     stOrder.push_back(n_049.backIt());
//     stOrder.push_back(n_125.backIt());
//     stOrder.push_back(n_126.backIt());
//     stOrder.push_back(n_127.backIt());
//     stOrder.push_back(n_128.backIt());
//     stOrder.push_back(n_130.backIt());
//     stOrder.push_back(n_129.backIt());
//     stOrder.push_back(n_079.backIt());
//     stOrder.push_back(n_053.backIt());
//     stOrder.push_back(n_055.backIt());
//     stOrder.push_back(n_056.backIt());
//     stOrder.push_back(n_057.backIt());
//     stOrder.push_back(n_058.backIt());
//     stOrder.push_back(n_060.backIt());
//     stOrder.push_back(n_039.backIt());
//     stOrder.push_back(n_136.backIt());
//     stOrder.push_back(n_052.backIt());
//     stOrder.push_back(n_054.backIt());
//     stOrder.push_back(n_059.backIt());
//     stOrder.push_back(n_051.backIt());
//     stOrder.push_back(n_046.backIt());
//     stOrder.push_back(n_043.backIt());
//     stOrder.push_back(n_048.backIt());
//     stOrder.push_back(n_050.backIt());
//     stOrder.push_back(n_041.backIt());
//     stOrder.push_back(n_040.backIt());
//     stOrder.push_back(n_038.backIt());
//     stOrder.push_back(n_037.backIt());
//     stOrder.push_back(n_139.backIt());
//     stOrder.push_back(n_138.backIt());
//     stOrder.push_back(n_137.backIt());
//     stOrder.push_back(n_034.backIt());
//     stOrder.push_back(n_031.backIt());
//     stOrder.push_back(n_088.backIt());
//     stOrder.push_back(n_062.backIt());
//     stOrder.push_back(n_089.backIt());
//     stOrder.push_back(n_063.backIt());
//     stOrder.push_back(n_064.backIt());
//     stOrder.push_back(n_061.backIt());
//     stOrder.push_back(n_135.backIt());
//     stOrder.push_back(n_066.backIt());
//     stOrder.push_back(n_067.backIt());
//     stOrder.push_back(n_078.backIt());
//     stOrder.push_back(n_081.backIt());
//     stOrder.push_back(n_082.backIt());
//     stOrder.push_back(n_134.backIt());
//     stOrder.push_back(n_133.backIt());
//     stOrder.push_back(n_132.backIt());
//     stOrder.push_back(n_087.backIt());
//     stOrder.push_back(n_086.backIt());
//     stOrder.push_back(n_090.backIt());
//     stOrder.push_back(n_085.backIt());
//     stOrder.push_back(n_131.backIt());
//     stOrder.push_back(n_084.backIt());
//     stOrder.push_back(n_091.backIt());
//     stOrder.push_back(n_083.backIt());
//     stOrder.push_back(n_080.backIt());
//     stOrder.push_back(n_077.backIt());
//     stOrder.push_back(n_076.backIt());
//     stOrder.push_back(n_092.backIt());
//     stOrder.push_back(n_093.backIt());
//     stOrder.push_back(n_074.backIt());
//     stOrder.push_back(n_069.backIt());
//     stOrder.push_back(n_071.backIt());
//     stOrder.push_back(n_070.backIt());
//     stOrder.push_back(n_068.backIt());
//     stOrder.push_back(n_100.backIt());
//     stOrder.push_back(n_106.backIt());
//     stOrder.push_back(n_102.backIt());
//     stOrder.push_back(n_109.backIt());
//     stOrder.push_back(n_108.backIt());
//     stOrder.push_back(n_107.backIt());
//     stOrder.push_back(n_110.backIt());
//     stOrder.push_back(n_111.backIt());
//     stOrder.push_back(n_114.backIt());
//     stOrder.push_back(n_115.backIt());
//     stOrder.push_back(n_029.backIt());
//     stOrder.push_back(n_016.backIt());
//     stOrder.push_back(n_141.backIt());
//     stOrder.push_back(n_017.backIt());
//     stOrder.push_back(n_018.backIt());
//     stOrder.push_back(n_019.backIt());
//     stOrder.push_back(n_020.backIt());
//     stOrder.push_back(n_013.backIt());
//     stOrder.push_back(n_025.backIt());
//     stOrder.push_back(n_023.backIt());
//     stOrder.push_back(n_027.backIt());
//     stOrder.push_back(n_142.backIt());
//     stOrder.push_back(n_005.backIt());
//     stOrder.push_back(n_006.backIt());
//     stOrder.push_back(n_003.backIt());
//     stOrder.push_back(n_004.backIt());
//     stOrder.push_back(n_015.backIt());
//     stOrder.push_back(n_014.backIt());
//     stOrder.push_back(n_007.backIt());
//     stOrder.push_back(n_009.backIt());
//     stOrder.push_back(n_012.backIt());
//     stOrder.push_back(n_028.backIt());
//     stOrder.push_back(n_026.backIt());
//     stOrder.push_back(n_024.backIt());
//     stOrder.push_back(n_117.backIt());
//     stOrder.push_back(n_116.backIt());
//     stOrder.push_back(n_118.backIt());
//     stOrder.push_back(n_119.backIt());
//     stOrder.push_back(n_120.backIt());
//     stOrder.push_back(n_113.backIt());
//     stOrder.push_back(n_112.backIt());
//     stOrder.push_back(n_105.backIt());
//     stOrder.push_back(n_104.backIt());
//     stOrder.push_back(n_103.backIt());
//     stOrder.push_back(n_101.backIt());
//     stOrder.push_back(n_099.backIt());
//     stOrder.push_back(n_022.backIt());
//     stOrder.push_back(n_021.backIt());
//     stOrder.push_back(n_011.backIt());
//     stOrder.push_back(n_010.backIt());
//     stOrder.push_back(n_008.backIt());
//     stOrder.push_back(n_065.backIt());
//     stOrder.push_back(n_121.backIt());
//     stOrder.push_back(n_002.backIt());
//     stOrder.push_back(n_001.backIt());
// 
//     BLPlanarityTester tester01;
//     auto res01 =  tester01.findEmbedding(g_1, stOrder);
//     if (res01) {
//         cerr << "The graph is planar. #1\n";
//     }
//     else{
//         cerr << "The graph is planar. #1\n";
//     }
// 
//     BLPlanarityTester tester02;
//     auto res02 =  tester02.isPlanar(g_1, stOrder);
//     if (res02) {
//         cerr << "The graph is planar. #2\n";
//     }
//     else{
//         cerr << "The graph is planar. #2\n";
//     }
// 
//     PlanarDualGraphMaker maker;
//     EmbeddedGraph emb;
//     DualGraph     dual;
//     maker.makeDualGraph(g_1, emb, dual);
// 
//     auto nPair = g_1.nodes();
//     for (auto nit = nPair.first; nit != nPair.second; nit++) {
//         auto& N = dynamic_cast<NumNode&>(*(*nit));
//         cerr << N.num() << " :";
//         for (auto iit  = N.incidentEdges().first;
//                   iit != N.incidentEdges().second; iit++) {
//             auto& E = *(*(*iit));
//             auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
//             cerr << " " << A.num();
// 
//         }
//         cerr << "\n";
//     }
//     
// 
// 
// }
// 
// 
// /**  @brief tests rearrangeIncidentEdges()
//  */
// TEST_F(BLPlanarityTesterTestsForEmbedding, TestRepro_003) {
// 
// 
//     Graph g_1;
// 
//     auto& n_000 = g_1.addNode(make_unique<NumNode>(0 ));
//     auto& n_001 = g_1.addNode(make_unique<NumNode>(1 ));
//     auto& n_002 = g_1.addNode(make_unique<NumNode>(2 ));
//     auto& n_003 = g_1.addNode(make_unique<NumNode>(3 ));
//     auto& n_004 = g_1.addNode(make_unique<NumNode>(4 ));
//     auto& n_005 = g_1.addNode(make_unique<NumNode>(5 ));
//     auto& n_006 = g_1.addNode(make_unique<NumNode>(6 ));
//     auto& n_007 = g_1.addNode(make_unique<NumNode>(7 ));
//     auto& n_008 = g_1.addNode(make_unique<NumNode>(8 ));
//     auto& n_009 = g_1.addNode(make_unique<NumNode>(9 ));
//     auto& n_010 = g_1.addNode(make_unique<NumNode>(10 ));
//     auto& n_011 = g_1.addNode(make_unique<NumNode>(11 ));
//     auto& n_012 = g_1.addNode(make_unique<NumNode>(12 ));
//     auto& n_013 = g_1.addNode(make_unique<NumNode>(13 ));
//     auto& n_014 = g_1.addNode(make_unique<NumNode>(14 ));
//     auto& n_015 = g_1.addNode(make_unique<NumNode>(15 ));
//     auto& n_016 = g_1.addNode(make_unique<NumNode>(16 ));
//     auto& n_017 = g_1.addNode(make_unique<NumNode>(17 ));
//     auto& n_018 = g_1.addNode(make_unique<NumNode>(18 ));
//     auto& n_019 = g_1.addNode(make_unique<NumNode>(19 ));
//     auto& n_020 = g_1.addNode(make_unique<NumNode>(20 ));
//     auto& n_021 = g_1.addNode(make_unique<NumNode>(21 ));
//     auto& n_022 = g_1.addNode(make_unique<NumNode>(22 ));
//     auto& n_023 = g_1.addNode(make_unique<NumNode>(23 ));
//     auto& n_024 = g_1.addNode(make_unique<NumNode>(24 ));
//     auto& n_025 = g_1.addNode(make_unique<NumNode>(25 ));
//     auto& n_026 = g_1.addNode(make_unique<NumNode>(26 ));
//     auto& n_027 = g_1.addNode(make_unique<NumNode>(27 ));
//     auto& n_028 = g_1.addNode(make_unique<NumNode>(28 ));
//     auto& n_029 = g_1.addNode(make_unique<NumNode>(29 ));
//     auto& n_030 = g_1.addNode(make_unique<NumNode>(30 ));
//     auto& n_031 = g_1.addNode(make_unique<NumNode>(31 ));
//     auto& n_032 = g_1.addNode(make_unique<NumNode>(32 ));
//     auto& n_033 = g_1.addNode(make_unique<NumNode>(33 ));
//     auto& n_034 = g_1.addNode(make_unique<NumNode>(34 ));
//     auto& n_035 = g_1.addNode(make_unique<NumNode>(35 ));
//     auto& n_036 = g_1.addNode(make_unique<NumNode>(36 ));
//     auto& n_037 = g_1.addNode(make_unique<NumNode>(37 ));
//     auto& n_038 = g_1.addNode(make_unique<NumNode>(38 ));
//     auto& n_039 = g_1.addNode(make_unique<NumNode>(39 ));
//     auto& n_040 = g_1.addNode(make_unique<NumNode>(40 ));
//     auto& n_041 = g_1.addNode(make_unique<NumNode>(41 ));
//     auto& n_042 = g_1.addNode(make_unique<NumNode>(42 ));
//     auto& n_043 = g_1.addNode(make_unique<NumNode>(43 ));
//     auto& n_044 = g_1.addNode(make_unique<NumNode>(44 ));
//     auto& n_045 = g_1.addNode(make_unique<NumNode>(45 ));
//     auto& n_046 = g_1.addNode(make_unique<NumNode>(46 ));
//     auto& n_047 = g_1.addNode(make_unique<NumNode>(47 ));
//     auto& n_048 = g_1.addNode(make_unique<NumNode>(48 ));
//     auto& n_049 = g_1.addNode(make_unique<NumNode>(49 ));
//     auto& n_050 = g_1.addNode(make_unique<NumNode>(50 ));
//     auto& n_051 = g_1.addNode(make_unique<NumNode>(51 ));
//     auto& n_052 = g_1.addNode(make_unique<NumNode>(52 ));
//     auto& n_053 = g_1.addNode(make_unique<NumNode>(53 ));
//     auto& n_054 = g_1.addNode(make_unique<NumNode>(54 ));
//     auto& n_055 = g_1.addNode(make_unique<NumNode>(55 ));
//     auto& n_056 = g_1.addNode(make_unique<NumNode>(56 ));
//     auto& n_057 = g_1.addNode(make_unique<NumNode>(57 ));
//     auto& n_058 = g_1.addNode(make_unique<NumNode>(58 ));
//     auto& n_059 = g_1.addNode(make_unique<NumNode>(59 ));
//     auto& n_060 = g_1.addNode(make_unique<NumNode>(60 ));
//     auto& n_061 = g_1.addNode(make_unique<NumNode>(61 ));
//     auto& n_062 = g_1.addNode(make_unique<NumNode>(62 ));
//     auto& n_063 = g_1.addNode(make_unique<NumNode>(63 ));
//     auto& n_064 = g_1.addNode(make_unique<NumNode>(64 ));
//     auto& n_065 = g_1.addNode(make_unique<NumNode>(65 ));
//     auto& n_066 = g_1.addNode(make_unique<NumNode>(66 ));
//     auto& n_067 = g_1.addNode(make_unique<NumNode>(67 ));
//     auto& n_068 = g_1.addNode(make_unique<NumNode>(68 ));
//     auto& n_069 = g_1.addNode(make_unique<NumNode>(69 ));
//     auto& n_070 = g_1.addNode(make_unique<NumNode>(70 ));
//     auto& n_071 = g_1.addNode(make_unique<NumNode>(71 ));
//     auto& n_072 = g_1.addNode(make_unique<NumNode>(72 ));
//     auto& n_073 = g_1.addNode(make_unique<NumNode>(73 ));
//     auto& n_074 = g_1.addNode(make_unique<NumNode>(74 ));
//     auto& n_075 = g_1.addNode(make_unique<NumNode>(75 ));
//     auto& n_076 = g_1.addNode(make_unique<NumNode>(76 ));
//     auto& n_077 = g_1.addNode(make_unique<NumNode>(77 ));
//     auto& n_078 = g_1.addNode(make_unique<NumNode>(78 ));
//     auto& n_079 = g_1.addNode(make_unique<NumNode>(79 ));
//     auto& n_080 = g_1.addNode(make_unique<NumNode>(80 ));
//     auto& n_081 = g_1.addNode(make_unique<NumNode>(81 ));
//     auto& n_082 = g_1.addNode(make_unique<NumNode>(82 ));
//     auto& n_083 = g_1.addNode(make_unique<NumNode>(83 ));
//     auto& n_084 = g_1.addNode(make_unique<NumNode>(84 ));
//     auto& n_085 = g_1.addNode(make_unique<NumNode>(85 ));
//     auto& n_086 = g_1.addNode(make_unique<NumNode>(86 ));
//     auto& n_087 = g_1.addNode(make_unique<NumNode>(87 ));
//     auto& n_088 = g_1.addNode(make_unique<NumNode>(88 ));
//     auto& n_089 = g_1.addNode(make_unique<NumNode>(89 ));
//     auto& n_090 = g_1.addNode(make_unique<NumNode>(90 ));
//     auto& n_091 = g_1.addNode(make_unique<NumNode>(91 ));
//     auto& n_092 = g_1.addNode(make_unique<NumNode>(92 ));
//     auto& n_093 = g_1.addNode(make_unique<NumNode>(93 ));
//     auto& n_094 = g_1.addNode(make_unique<NumNode>(94 ));
//     auto& n_095 = g_1.addNode(make_unique<NumNode>(95 ));
//     auto& n_096 = g_1.addNode(make_unique<NumNode>(96 ));
//     auto& n_097 = g_1.addNode(make_unique<NumNode>(97 ));
//     auto& n_098 = g_1.addNode(make_unique<NumNode>(98 ));
//     auto& n_099 = g_1.addNode(make_unique<NumNode>(99 ));
//     auto& n_100 = g_1.addNode(make_unique<NumNode>(100 ));
//     auto& n_101 = g_1.addNode(make_unique<NumNode>(101 ));
//     auto& n_102 = g_1.addNode(make_unique<NumNode>(102 ));
//     auto& n_103 = g_1.addNode(make_unique<NumNode>(103 ));
//     auto& n_104 = g_1.addNode(make_unique<NumNode>(104 ));
//     auto& n_105 = g_1.addNode(make_unique<NumNode>(105 ));
//     auto& n_106 = g_1.addNode(make_unique<NumNode>(106 ));
//     auto& n_107 = g_1.addNode(make_unique<NumNode>(107 ));
//     auto& n_108 = g_1.addNode(make_unique<NumNode>(108 ));
//     auto& n_109 = g_1.addNode(make_unique<NumNode>(109 ));
//     auto& n_110 = g_1.addNode(make_unique<NumNode>(110 ));
//     auto& n_111 = g_1.addNode(make_unique<NumNode>(111 ));
//     auto& n_112 = g_1.addNode(make_unique<NumNode>(112 ));
//     auto& n_113 = g_1.addNode(make_unique<NumNode>(113 ));
//     auto& n_114 = g_1.addNode(make_unique<NumNode>(114 ));
//     auto& n_115 = g_1.addNode(make_unique<NumNode>(115 ));
//     auto& n_116 = g_1.addNode(make_unique<NumNode>(116 ));
//     auto& n_117 = g_1.addNode(make_unique<NumNode>(117 ));
//     auto& n_118 = g_1.addNode(make_unique<NumNode>(118 ));
//     auto& n_119 = g_1.addNode(make_unique<NumNode>(119 ));
//     auto& n_120 = g_1.addNode(make_unique<NumNode>(120 ));
//     auto& n_121 = g_1.addNode(make_unique<NumNode>(121 ));
//     auto& n_122 = g_1.addNode(make_unique<NumNode>(122 ));
//     auto& n_123 = g_1.addNode(make_unique<NumNode>(123 ));
//     auto& n_124 = g_1.addNode(make_unique<NumNode>(124 ));
//     auto& n_125 = g_1.addNode(make_unique<NumNode>(125 ));
//     auto& n_126 = g_1.addNode(make_unique<NumNode>(126 ));
//     auto& n_127 = g_1.addNode(make_unique<NumNode>(127 ));
//     auto& n_128 = g_1.addNode(make_unique<NumNode>(128 ));
//     auto& n_129 = g_1.addNode(make_unique<NumNode>(129 ));
//     auto& n_130 = g_1.addNode(make_unique<NumNode>(130 ));
//     auto& n_131 = g_1.addNode(make_unique<NumNode>(131 ));
//     auto& n_132 = g_1.addNode(make_unique<NumNode>(132 ));
//     auto& n_133 = g_1.addNode(make_unique<NumNode>(133 ));
//     auto& n_134 = g_1.addNode(make_unique<NumNode>(134 ));
//     auto& n_135 = g_1.addNode(make_unique<NumNode>(135 ));
//     auto& n_136 = g_1.addNode(make_unique<NumNode>(136 ));
//     auto& n_137 = g_1.addNode(make_unique<NumNode>(137 ));
//     auto& n_138 = g_1.addNode(make_unique<NumNode>(138 ));
//     auto& n_139 = g_1.addNode(make_unique<NumNode>(139 ));
//     auto& n_140 = g_1.addNode(make_unique<NumNode>(140 ));
//     auto& n_141 = g_1.addNode(make_unique<NumNode>(141 ));
// 
//     auto& e_041_013 = g_1.addEdge(make_unique<Edge>(), n_041, n_013 );
//     auto& e_099_097 = g_1.addEdge(make_unique<Edge>(), n_099, n_097 );
//     auto& e_070_103 = g_1.addEdge(make_unique<Edge>(), n_070, n_103 );
//     auto& e_138_001 = g_1.addEdge(make_unique<Edge>(), n_138, n_001 );
//     auto& e_098_099 = g_1.addEdge(make_unique<Edge>(), n_098, n_099 );
//     auto& e_070_058 = g_1.addEdge(make_unique<Edge>(), n_070, n_058 );
//     auto& e_055_054 = g_1.addEdge(make_unique<Edge>(), n_055, n_054 );
//     auto& e_082_087 = g_1.addEdge(make_unique<Edge>(), n_082, n_087 );
//     auto& e_026_028 = g_1.addEdge(make_unique<Edge>(), n_026, n_028 );
//     auto& e_022_019 = g_1.addEdge(make_unique<Edge>(), n_022, n_019 );
//     auto& e_082_112 = g_1.addEdge(make_unique<Edge>(), n_082, n_112 );
//     auto& e_003_114 = g_1.addEdge(make_unique<Edge>(), n_003, n_114 );
//     auto& e_085_111 = g_1.addEdge(make_unique<Edge>(), n_085, n_111 );
//     auto& e_094_095 = g_1.addEdge(make_unique<Edge>(), n_094, n_095 );
//     auto& e_135_132 = g_1.addEdge(make_unique<Edge>(), n_135, n_132 );
//     auto& e_137_004 = g_1.addEdge(make_unique<Edge>(), n_137, n_004 );
//     auto& e_045_009 = g_1.addEdge(make_unique<Edge>(), n_045, n_009 );
//     auto& e_132_031 = g_1.addEdge(make_unique<Edge>(), n_132, n_031 );
//     auto& e_129_126 = g_1.addEdge(make_unique<Edge>(), n_129, n_126 );
//     auto& e_032_002 = g_1.addEdge(make_unique<Edge>(), n_032, n_002 );
//     auto& e_038_037 = g_1.addEdge(make_unique<Edge>(), n_038, n_037 );
//     auto& e_004_000 = g_1.addEdge(make_unique<Edge>(), n_004, n_000 );
//     auto& e_105_090 = g_1.addEdge(make_unique<Edge>(), n_105, n_090 );
//     auto& e_098_096 = g_1.addEdge(make_unique<Edge>(), n_098, n_096 );
//     auto& e_051_050 = g_1.addEdge(make_unique<Edge>(), n_051, n_050 );
//     auto& e_082_118 = g_1.addEdge(make_unique<Edge>(), n_082, n_118 );
//     auto& e_056_061 = g_1.addEdge(make_unique<Edge>(), n_056, n_061 );
//     auto& e_095_135 = g_1.addEdge(make_unique<Edge>(), n_095, n_135 );
//     auto& e_086_109 = g_1.addEdge(make_unique<Edge>(), n_086, n_109 );
//     auto& e_085_105 = g_1.addEdge(make_unique<Edge>(), n_085, n_105 );
//     auto& e_129_128 = g_1.addEdge(make_unique<Edge>(), n_129, n_128 );
//     auto& e_115_116 = g_1.addEdge(make_unique<Edge>(), n_115, n_116 );
//     auto& e_007_006 = g_1.addEdge(make_unique<Edge>(), n_007, n_006 );
//     auto& e_028_035 = g_1.addEdge(make_unique<Edge>(), n_028, n_035 );
//     auto& e_050_039 = g_1.addEdge(make_unique<Edge>(), n_050, n_039 );
//     auto& e_028_029 = g_1.addEdge(make_unique<Edge>(), n_028, n_029 );
//     auto& e_115_002 = g_1.addEdge(make_unique<Edge>(), n_115, n_002 );
//     auto& e_131_006 = g_1.addEdge(make_unique<Edge>(), n_131, n_006 );
//     auto& e_117_070 = g_1.addEdge(make_unique<Edge>(), n_117, n_070 );
//     auto& e_136_006 = g_1.addEdge(make_unique<Edge>(), n_136, n_006 );
//     auto& e_093_092 = g_1.addEdge(make_unique<Edge>(), n_093, n_092 );
//     auto& e_052_127 = g_1.addEdge(make_unique<Edge>(), n_052, n_127 );
//     auto& e_042_041 = g_1.addEdge(make_unique<Edge>(), n_042, n_041 );
//     auto& e_075_077 = g_1.addEdge(make_unique<Edge>(), n_075, n_077 );
//     auto& e_000_001 = g_1.addEdge(make_unique<Edge>(), n_000, n_001 );
//     auto& e_061_060 = g_1.addEdge(make_unique<Edge>(), n_061, n_060 );
//     auto& e_139_113 = g_1.addEdge(make_unique<Edge>(), n_139, n_113 );
//     auto& e_024_023 = g_1.addEdge(make_unique<Edge>(), n_024, n_023 );
//     auto& e_024_039 = g_1.addEdge(make_unique<Edge>(), n_024, n_039 );
//     auto& e_003_141 = g_1.addEdge(make_unique<Edge>(), n_003, n_141 );
//     auto& e_116_114 = g_1.addEdge(make_unique<Edge>(), n_116, n_114 );
//     auto& e_126_002 = g_1.addEdge(make_unique<Edge>(), n_126, n_002 );
//     auto& e_022_048 = g_1.addEdge(make_unique<Edge>(), n_022, n_048 );
//     auto& e_107_086 = g_1.addEdge(make_unique<Edge>(), n_107, n_086 );
//     auto& e_086_110 = g_1.addEdge(make_unique<Edge>(), n_086, n_110 );
//     auto& e_101_089 = g_1.addEdge(make_unique<Edge>(), n_101, n_089 );
//     auto& e_007_005 = g_1.addEdge(make_unique<Edge>(), n_007, n_005 );
//     auto& e_047_040 = g_1.addEdge(make_unique<Edge>(), n_047, n_040 );
//     auto& e_104_103 = g_1.addEdge(make_unique<Edge>(), n_104, n_103 );
//     auto& e_097_135 = g_1.addEdge(make_unique<Edge>(), n_097, n_135 );
//     auto& e_050_040 = g_1.addEdge(make_unique<Edge>(), n_050, n_040 );
//     auto& e_072_070 = g_1.addEdge(make_unique<Edge>(), n_072, n_070 );
//     auto& e_021_018 = g_1.addEdge(make_unique<Edge>(), n_021, n_018 );
//     auto& e_069_067 = g_1.addEdge(make_unique<Edge>(), n_069, n_067 );
//     auto& e_017_018 = g_1.addEdge(make_unique<Edge>(), n_017, n_018 );
//     auto& e_048_024 = g_1.addEdge(make_unique<Edge>(), n_048, n_024 );
//     auto& e_104_088 = g_1.addEdge(make_unique<Edge>(), n_104, n_088 );
//     auto& e_094_000 = g_1.addEdge(make_unique<Edge>(), n_094, n_000 );
//     auto& e_092_091 = g_1.addEdge(make_unique<Edge>(), n_092, n_091 );
//     auto& e_137_002 = g_1.addEdge(make_unique<Edge>(), n_137, n_002 );
//     auto& e_103_102 = g_1.addEdge(make_unique<Edge>(), n_103, n_102 );
//     auto& e_019_049 = g_1.addEdge(make_unique<Edge>(), n_019, n_049 );
//     auto& e_139_003 = g_1.addEdge(make_unique<Edge>(), n_139, n_003 );
//     auto& e_058_066 = g_1.addEdge(make_unique<Edge>(), n_058, n_066 );
//     auto& e_005_014 = g_1.addEdge(make_unique<Edge>(), n_005, n_014 );
//     auto& e_011_008 = g_1.addEdge(make_unique<Edge>(), n_011, n_008 );
//     auto& e_085_084 = g_1.addEdge(make_unique<Edge>(), n_085, n_084 );
//     auto& e_060_065 = g_1.addEdge(make_unique<Edge>(), n_060, n_065 );
//     auto& e_040_048 = g_1.addEdge(make_unique<Edge>(), n_040, n_048 );
//     auto& e_056_055 = g_1.addEdge(make_unique<Edge>(), n_056, n_055 );
//     auto& e_036_002 = g_1.addEdge(make_unique<Edge>(), n_036, n_002 );
//     auto& e_059_066 = g_1.addEdge(make_unique<Edge>(), n_059, n_066 );
//     auto& e_129_127 = g_1.addEdge(make_unique<Edge>(), n_129, n_127 );
//     auto& e_015_016 = g_1.addEdge(make_unique<Edge>(), n_015, n_016 );
//     auto& e_055_050 = g_1.addEdge(make_unique<Edge>(), n_055, n_050 );
//     auto& e_045_093 = g_1.addEdge(make_unique<Edge>(), n_045, n_093 );
//     auto& e_028_037 = g_1.addEdge(make_unique<Edge>(), n_028, n_037 );
//     auto& e_023_026 = g_1.addEdge(make_unique<Edge>(), n_023, n_026 );
//     auto& e_134_132 = g_1.addEdge(make_unique<Edge>(), n_134, n_132 );
//     auto& e_022_047 = g_1.addEdge(make_unique<Edge>(), n_022, n_047 );
//     auto& e_081_107 = g_1.addEdge(make_unique<Edge>(), n_081, n_107 );
//     auto& e_045_042 = g_1.addEdge(make_unique<Edge>(), n_045, n_042 );
//     auto& e_069_068 = g_1.addEdge(make_unique<Edge>(), n_069, n_068 );
//     auto& e_126_125 = g_1.addEdge(make_unique<Edge>(), n_126, n_125 );
//     auto& e_092_098 = g_1.addEdge(make_unique<Edge>(), n_092, n_098 );
//     auto& e_081_120 = g_1.addEdge(make_unique<Edge>(), n_081, n_120 );
//     auto& e_070_066 = g_1.addEdge(make_unique<Edge>(), n_070, n_066 );
//     auto& e_043_046 = g_1.addEdge(make_unique<Edge>(), n_043, n_046 );
//     auto& e_124_058 = g_1.addEdge(make_unique<Edge>(), n_124, n_058 );
//     auto& e_084_111 = g_1.addEdge(make_unique<Edge>(), n_084, n_111 );
//     auto& e_041_012 = g_1.addEdge(make_unique<Edge>(), n_041, n_012 );
//     auto& e_064_092 = g_1.addEdge(make_unique<Edge>(), n_064, n_092 );
//     auto& e_021_046 = g_1.addEdge(make_unique<Edge>(), n_021, n_046 );
//     auto& e_054_052 = g_1.addEdge(make_unique<Edge>(), n_054, n_052 );
//     auto& e_045_044 = g_1.addEdge(make_unique<Edge>(), n_045, n_044 );
//     auto& e_015_023 = g_1.addEdge(make_unique<Edge>(), n_015, n_023 );
//     auto& e_078_076 = g_1.addEdge(make_unique<Edge>(), n_078, n_076 );
//     auto& e_020_021 = g_1.addEdge(make_unique<Edge>(), n_020, n_021 );
//     auto& e_088_102 = g_1.addEdge(make_unique<Edge>(), n_088, n_102 );
//     auto& e_091_089 = g_1.addEdge(make_unique<Edge>(), n_091, n_089 );
//     auto& e_123_070 = g_1.addEdge(make_unique<Edge>(), n_123, n_070 );
//     auto& e_131_031 = g_1.addEdge(make_unique<Edge>(), n_131, n_031 );
//     auto& e_081_118 = g_1.addEdge(make_unique<Edge>(), n_081, n_118 );
//     auto& e_026_025 = g_1.addEdge(make_unique<Edge>(), n_026, n_025 );
//     auto& e_080_079 = g_1.addEdge(make_unique<Edge>(), n_080, n_079 );
//     auto& e_118_070 = g_1.addEdge(make_unique<Edge>(), n_118, n_070 );
//     auto& e_008_006 = g_1.addEdge(make_unique<Edge>(), n_008, n_006 );
//     auto& e_014_034 = g_1.addEdge(make_unique<Edge>(), n_014, n_034 );
//     auto& e_117_118 = g_1.addEdge(make_unique<Edge>(), n_117, n_118 );
//     auto& e_045_100 = g_1.addEdge(make_unique<Edge>(), n_045, n_100 );
//     auto& e_099_133 = g_1.addEdge(make_unique<Edge>(), n_099, n_133 );
//     auto& e_091_096 = g_1.addEdge(make_unique<Edge>(), n_091, n_096 );
//     auto& e_096_097 = g_1.addEdge(make_unique<Edge>(), n_096, n_097 );
//     auto& e_077_076 = g_1.addEdge(make_unique<Edge>(), n_077, n_076 );
//     auto& e_075_076 = g_1.addEdge(make_unique<Edge>(), n_075, n_076 );
//     auto& e_060_059 = g_1.addEdge(make_unique<Edge>(), n_060, n_059 );
//     auto& e_106_104 = g_1.addEdge(make_unique<Edge>(), n_106, n_104 );
//     auto& e_081_070 = g_1.addEdge(make_unique<Edge>(), n_081, n_070 );
//     auto& e_109_084 = g_1.addEdge(make_unique<Edge>(), n_109, n_084 );
//     auto& e_012_007 = g_1.addEdge(make_unique<Edge>(), n_012, n_007 );
//     auto& e_019_017 = g_1.addEdge(make_unique<Edge>(), n_019, n_017 );
//     auto& e_125_053 = g_1.addEdge(make_unique<Edge>(), n_125, n_053 );
//     auto& e_046_027 = g_1.addEdge(make_unique<Edge>(), n_046, n_027 );
//     auto& e_029_033 = g_1.addEdge(make_unique<Edge>(), n_029, n_033 );
//     auto& e_072_071 = g_1.addEdge(make_unique<Edge>(), n_072, n_071 );
//     auto& e_123_069 = g_1.addEdge(make_unique<Edge>(), n_123, n_069 );
//     auto& e_132_002 = g_1.addEdge(make_unique<Edge>(), n_132, n_002 );
//     auto& e_083_084 = g_1.addEdge(make_unique<Edge>(), n_083, n_084 );
//     auto& e_009_134 = g_1.addEdge(make_unique<Edge>(), n_009, n_134 );
//     auto& e_052_125 = g_1.addEdge(make_unique<Edge>(), n_052, n_125 );
//     auto& e_102_067 = g_1.addEdge(make_unique<Edge>(), n_102, n_067 );
//     auto& e_052_056 = g_1.addEdge(make_unique<Edge>(), n_052, n_056 );
//     auto& e_077_078 = g_1.addEdge(make_unique<Edge>(), n_077, n_078 );
//     auto& e_067_068 = g_1.addEdge(make_unique<Edge>(), n_067, n_068 );
//     auto& e_022_049 = g_1.addEdge(make_unique<Edge>(), n_022, n_049 );
//     auto& e_065_091 = g_1.addEdge(make_unique<Edge>(), n_065, n_091 );
//     auto& e_057_058 = g_1.addEdge(make_unique<Edge>(), n_057, n_058 );
//     auto& e_114_112 = g_1.addEdge(make_unique<Edge>(), n_114, n_112 );
//     auto& e_103_069 = g_1.addEdge(make_unique<Edge>(), n_103, n_069 );
//     auto& e_122_070 = g_1.addEdge(make_unique<Edge>(), n_122, n_070 );
//     auto& e_039_038 = g_1.addEdge(make_unique<Edge>(), n_039, n_038 );
//     auto& e_109_110 = g_1.addEdge(make_unique<Edge>(), n_109, n_110 );
//     auto& e_112_083 = g_1.addEdge(make_unique<Edge>(), n_112, n_083 );
//     auto& e_053_115 = g_1.addEdge(make_unique<Edge>(), n_053, n_115 );
//     auto& e_010_099 = g_1.addEdge(make_unique<Edge>(), n_010, n_099 );
//     auto& e_066_068 = g_1.addEdge(make_unique<Edge>(), n_066, n_068 );
//     auto& e_023_018 = g_1.addEdge(make_unique<Edge>(), n_023, n_018 );
//     auto& e_097_095 = g_1.addEdge(make_unique<Edge>(), n_097, n_095 );
//     auto& e_082_119 = g_1.addEdge(make_unique<Edge>(), n_082, n_119 );
//     auto& e_133_009 = g_1.addEdge(make_unique<Edge>(), n_133, n_009 );
//     auto& e_086_087 = g_1.addEdge(make_unique<Edge>(), n_086, n_087 );
//     auto& e_027_025 = g_1.addEdge(make_unique<Edge>(), n_027, n_025 );
//     auto& e_042_043 = g_1.addEdge(make_unique<Edge>(), n_042, n_043 );
//     auto& e_081_108 = g_1.addEdge(make_unique<Edge>(), n_081, n_108 );
//     auto& e_039_026 = g_1.addEdge(make_unique<Edge>(), n_039, n_026 );
//     auto& e_095_004 = g_1.addEdge(make_unique<Edge>(), n_095, n_004 );
//     auto& e_024_049 = g_1.addEdge(make_unique<Edge>(), n_024, n_049 );
//     auto& e_009_008 = g_1.addEdge(make_unique<Edge>(), n_009, n_008 );
//     auto& e_100_093 = g_1.addEdge(make_unique<Edge>(), n_100, n_093 );
//     auto& e_062_061 = g_1.addEdge(make_unique<Edge>(), n_062, n_061 );
//     auto& e_030_031 = g_1.addEdge(make_unique<Edge>(), n_030, n_031 );
//     auto& e_029_013 = g_1.addEdge(make_unique<Edge>(), n_029, n_013 );
//     auto& e_035_029 = g_1.addEdge(make_unique<Edge>(), n_035, n_029 );
//     auto& e_015_019 = g_1.addEdge(make_unique<Edge>(), n_015, n_019 );
//     auto& e_073_056 = g_1.addEdge(make_unique<Edge>(), n_073, n_056 );
//     auto& e_063_062 = g_1.addEdge(make_unique<Edge>(), n_063, n_062 );
//     auto& e_020_017 = g_1.addEdge(make_unique<Edge>(), n_020, n_017 );
//     auto& e_028_038 = g_1.addEdge(make_unique<Edge>(), n_028, n_038 );
//     auto& e_009_044 = g_1.addEdge(make_unique<Edge>(), n_009, n_044 );
//     auto& e_017_015 = g_1.addEdge(make_unique<Edge>(), n_017, n_015 );
//     auto& e_022_020 = g_1.addEdge(make_unique<Edge>(), n_022, n_020 );
//     auto& e_077_079 = g_1.addEdge(make_unique<Edge>(), n_077, n_079 );
//     auto& e_108_107 = g_1.addEdge(make_unique<Edge>(), n_108, n_107 );
//     auto& e_010_100 = g_1.addEdge(make_unique<Edge>(), n_010, n_100 );
//     auto& e_035_032 = g_1.addEdge(make_unique<Edge>(), n_035, n_032 );
//     auto& e_081_106 = g_1.addEdge(make_unique<Edge>(), n_081, n_106 );
//     auto& e_019_020 = g_1.addEdge(make_unique<Edge>(), n_019, n_020 );
//     auto& e_087_118 = g_1.addEdge(make_unique<Edge>(), n_087, n_118 );
//     auto& e_002_030 = g_1.addEdge(make_unique<Edge>(), n_002, n_030 );
//     auto& e_018_027 = g_1.addEdge(make_unique<Edge>(), n_018, n_027 );
//     auto& e_080_078 = g_1.addEdge(make_unique<Edge>(), n_080, n_078 );
//     auto& e_134_131 = g_1.addEdge(make_unique<Edge>(), n_134, n_131 );
//     auto& e_120_121 = g_1.addEdge(make_unique<Edge>(), n_120, n_121 );
//     auto& e_028_002 = g_1.addEdge(make_unique<Edge>(), n_028, n_002 );
//     auto& e_009_011 = g_1.addEdge(make_unique<Edge>(), n_009, n_011 );
//     auto& e_033_006 = g_1.addEdge(make_unique<Edge>(), n_033, n_006 );
//     auto& e_028_013 = g_1.addEdge(make_unique<Edge>(), n_028, n_013 );
//     auto& e_125_002 = g_1.addEdge(make_unique<Edge>(), n_125, n_002 );
//     auto& e_124_056 = g_1.addEdge(make_unique<Edge>(), n_124, n_056 );
//     auto& e_056_053 = g_1.addEdge(make_unique<Edge>(), n_056, n_053 );
//     auto& e_082_084 = g_1.addEdge(make_unique<Edge>(), n_082, n_084 );
//     auto& e_086_085 = g_1.addEdge(make_unique<Edge>(), n_086, n_085 );
//     auto& e_045_043 = g_1.addEdge(make_unique<Edge>(), n_045, n_043 );
//     auto& e_114_119 = g_1.addEdge(make_unique<Edge>(), n_114, n_119 );
//     auto& e_040_024 = g_1.addEdge(make_unique<Edge>(), n_040, n_024 );
//     auto& e_054_056 = g_1.addEdge(make_unique<Edge>(), n_054, n_056 );
//     auto& e_116_002 = g_1.addEdge(make_unique<Edge>(), n_116, n_002 );
//     auto& e_088_105 = g_1.addEdge(make_unique<Edge>(), n_088, n_105 );
//     auto& e_101_090 = g_1.addEdge(make_unique<Edge>(), n_101, n_090 );
//     auto& e_071_056 = g_1.addEdge(make_unique<Edge>(), n_071, n_056 );
//     auto& e_082_083 = g_1.addEdge(make_unique<Edge>(), n_082, n_083 );
//     auto& e_034_033 = g_1.addEdge(make_unique<Edge>(), n_034, n_033 );
//     auto& e_087_108 = g_1.addEdge(make_unique<Edge>(), n_087, n_108 );
//     auto& e_135_133 = g_1.addEdge(make_unique<Edge>(), n_135, n_133 );
//     auto& e_002_003 = g_1.addEdge(make_unique<Edge>(), n_002, n_003 );
//     auto& e_008_007 = g_1.addEdge(make_unique<Edge>(), n_008, n_007 );
//     auto& e_044_042 = g_1.addEdge(make_unique<Edge>(), n_044, n_042 );
//     auto& e_102_101 = g_1.addEdge(make_unique<Edge>(), n_102, n_101 );
//     auto& e_120_118 = g_1.addEdge(make_unique<Edge>(), n_120, n_118 );
//     auto& e_079_078 = g_1.addEdge(make_unique<Edge>(), n_079, n_078 );
//     auto& e_009_006 = g_1.addEdge(make_unique<Edge>(), n_009, n_006 );
//     auto& e_130_036 = g_1.addEdge(make_unique<Edge>(), n_130, n_036 );
//     auto& e_030_006 = g_1.addEdge(make_unique<Edge>(), n_030, n_006 );
//     auto& e_112_113 = g_1.addEdge(make_unique<Edge>(), n_112, n_113 );
//     auto& e_138_090 = g_1.addEdge(make_unique<Edge>(), n_138, n_090 );
//     auto& e_015_024 = g_1.addEdge(make_unique<Edge>(), n_015, n_024 );
//     auto& e_019_016 = g_1.addEdge(make_unique<Edge>(), n_019, n_016 );
//     auto& e_118_108 = g_1.addEdge(make_unique<Edge>(), n_118, n_108 );
//     auto& e_067_065 = g_1.addEdge(make_unique<Edge>(), n_067, n_065 );
//     auto& e_006_031 = g_1.addEdge(make_unique<Edge>(), n_006, n_031 );
//     auto& e_013_005 = g_1.addEdge(make_unique<Edge>(), n_013, n_005 );
//     auto& e_061_064 = g_1.addEdge(make_unique<Edge>(), n_061, n_064 );
//     auto& e_025_013 = g_1.addEdge(make_unique<Edge>(), n_025, n_013 );
//     auto& e_075_122 = g_1.addEdge(make_unique<Edge>(), n_075, n_122 );
//     auto& e_044_011 = g_1.addEdge(make_unique<Edge>(), n_044, n_011 );
//     auto& e_113_140 = g_1.addEdge(make_unique<Edge>(), n_113, n_140 );
//     auto& e_032_030 = g_1.addEdge(make_unique<Edge>(), n_032, n_030 );
//     auto& e_083_085 = g_1.addEdge(make_unique<Edge>(), n_083, n_085 );
//     auto& e_045_010 = g_1.addEdge(make_unique<Edge>(), n_045, n_010 );
//     auto& e_002_004 = g_1.addEdge(make_unique<Edge>(), n_002, n_004 );
//     auto& e_089_094 = g_1.addEdge(make_unique<Edge>(), n_089, n_094 );
//     auto& e_016_049 = g_1.addEdge(make_unique<Edge>(), n_016, n_049 );
//     auto& e_037_036 = g_1.addEdge(make_unique<Edge>(), n_037, n_036 );
//     auto& e_118_119 = g_1.addEdge(make_unique<Edge>(), n_118, n_119 );
//     auto& e_051_054 = g_1.addEdge(make_unique<Edge>(), n_051, n_054 );
//     auto& e_056_060 = g_1.addEdge(make_unique<Edge>(), n_056, n_060 );
//     auto& e_005_006 = g_1.addEdge(make_unique<Edge>(), n_005, n_006 );
//     auto& e_086_108 = g_1.addEdge(make_unique<Edge>(), n_086, n_108 );
//     auto& e_090_140 = g_1.addEdge(make_unique<Edge>(), n_090, n_140 );
//     auto& e_100_098 = g_1.addEdge(make_unique<Edge>(), n_100, n_098 );
//     auto& e_014_013 = g_1.addEdge(make_unique<Edge>(), n_014, n_013 );
//     auto& e_139_090 = g_1.addEdge(make_unique<Edge>(), n_139, n_090 );
//     auto& e_047_045 = g_1.addEdge(make_unique<Edge>(), n_047, n_045 );
//     auto& e_064_063 = g_1.addEdge(make_unique<Edge>(), n_064, n_063 );
//     auto& e_045_062 = g_1.addEdge(make_unique<Edge>(), n_045, n_062 );
//     auto& e_106_077 = g_1.addEdge(make_unique<Edge>(), n_106, n_077 );
//     auto& e_033_032 = g_1.addEdge(make_unique<Edge>(), n_033, n_032 );
//     auto& e_001_139 = g_1.addEdge(make_unique<Edge>(), n_001, n_139 );
//     auto& e_086_088 = g_1.addEdge(make_unique<Edge>(), n_086, n_088 );
//     auto& e_001_002 = g_1.addEdge(make_unique<Edge>(), n_001, n_002 );
//     auto& e_074_072 = g_1.addEdge(make_unique<Edge>(), n_074, n_072 );
//     auto& e_117_116 = g_1.addEdge(make_unique<Edge>(), n_117, n_116 );
//     auto& e_016_024 = g_1.addEdge(make_unique<Edge>(), n_016, n_024 );
//     auto& e_128_127 = g_1.addEdge(make_unique<Edge>(), n_128, n_127 );
//     auto& e_074_071 = g_1.addEdge(make_unique<Edge>(), n_074, n_071 );
//     auto& e_053_002 = g_1.addEdge(make_unique<Edge>(), n_053, n_002 );
//     auto& e_105_101 = g_1.addEdge(make_unique<Edge>(), n_105, n_101 );
//     auto& e_089_090 = g_1.addEdge(make_unique<Edge>(), n_089, n_090 );
//     auto& e_028_130 = g_1.addEdge(make_unique<Edge>(), n_028, n_130 );
//     auto& e_037_052 = g_1.addEdge(make_unique<Edge>(), n_037, n_052 );
//     auto& e_086_082 = g_1.addEdge(make_unique<Edge>(), n_086, n_082 );
//     auto& e_002_035 = g_1.addEdge(make_unique<Edge>(), n_002, n_035 );
//     auto& e_015_018 = g_1.addEdge(make_unique<Edge>(), n_015, n_018 );
//     auto& e_139_140 = g_1.addEdge(make_unique<Edge>(), n_139, n_140 );
//     auto& e_022_021 = g_1.addEdge(make_unique<Edge>(), n_022, n_021 );
//     auto& e_073_071 = g_1.addEdge(make_unique<Edge>(), n_073, n_071 );
//     auto& e_070_115 = g_1.addEdge(make_unique<Edge>(), n_070, n_115 );
//     auto& e_038_051 = g_1.addEdge(make_unique<Edge>(), n_038, n_051 );
//     auto& e_059_058 = g_1.addEdge(make_unique<Edge>(), n_059, n_058 );
//     auto& e_093_063 = g_1.addEdge(make_unique<Edge>(), n_093, n_063 );
//     auto& e_086_084 = g_1.addEdge(make_unique<Edge>(), n_086, n_084 );
//     auto& e_002_031 = g_1.addEdge(make_unique<Edge>(), n_002, n_031 );
//     auto& e_133_134 = g_1.addEdge(make_unique<Edge>(), n_133, n_134 );
//     auto& e_009_131 = g_1.addEdge(make_unique<Edge>(), n_009, n_131 );
//     auto& e_141_113 = g_1.addEdge(make_unique<Edge>(), n_141, n_113 );
//     auto& e_027_013 = g_1.addEdge(make_unique<Edge>(), n_027, n_013 );
//     auto& e_009_010 = g_1.addEdge(make_unique<Edge>(), n_009, n_010 );
//     auto& e_063_045 = g_1.addEdge(make_unique<Edge>(), n_063, n_045 );
//     auto& e_052_051 = g_1.addEdge(make_unique<Edge>(), n_052, n_051 );
//     auto& e_081_077 = g_1.addEdge(make_unique<Edge>(), n_081, n_077 );
//     auto& e_046_041 = g_1.addEdge(make_unique<Edge>(), n_046, n_041 );
//     auto& e_062_056 = g_1.addEdge(make_unique<Edge>(), n_062, n_056 );
//     auto& e_011_012 = g_1.addEdge(make_unique<Edge>(), n_011, n_012 );
//     auto& e_025_028 = g_1.addEdge(make_unique<Edge>(), n_025, n_028 );
//     auto& e_000_090 = g_1.addEdge(make_unique<Edge>(), n_000, n_090 );
//     auto& e_048_049 = g_1.addEdge(make_unique<Edge>(), n_048, n_049 );
//     auto& e_045_055 = g_1.addEdge(make_unique<Edge>(), n_045, n_055 );
//     auto& e_128_052 = g_1.addEdge(make_unique<Edge>(), n_128, n_052 );
//     auto& e_107_104 = g_1.addEdge(make_unique<Edge>(), n_107, n_104 );
//     auto& e_120_070 = g_1.addEdge(make_unique<Edge>(), n_120, n_070 );
//     auto& e_059_057 = g_1.addEdge(make_unique<Edge>(), n_059, n_057 );
//     auto& e_096_094 = g_1.addEdge(make_unique<Edge>(), n_096, n_094 );
//     auto& e_036_126 = g_1.addEdge(make_unique<Edge>(), n_036, n_126 );
//     auto& e_065_064 = g_1.addEdge(make_unique<Edge>(), n_065, n_064 );
//     auto& e_012_013 = g_1.addEdge(make_unique<Edge>(), n_012, n_013 );
//     auto& e_081_121 = g_1.addEdge(make_unique<Edge>(), n_081, n_121 );
//     auto& e_090_113 = g_1.addEdge(make_unique<Edge>(), n_090, n_113 );
//     auto& e_043_047 = g_1.addEdge(make_unique<Edge>(), n_043, n_047 );
//     auto& e_014_136 = g_1.addEdge(make_unique<Edge>(), n_014, n_136 );
// 
//     vector<edge_list_it_t> edges_000;
//     edges_000.push_back(e_094_000.backIt());
//     edges_000.push_back(e_000_090.backIt());
//     edges_000.push_back(e_000_001.backIt());
//     edges_000.push_back(e_004_000.backIt());
//     n_000.reorderIncidence(std::move(edges_000));
// 
//     vector<edge_list_it_t> edges_001;
//     edges_001.push_back(e_001_002.backIt());
//     edges_001.push_back(e_000_001.backIt());
//     edges_001.push_back(e_138_001.backIt());
//     edges_001.push_back(e_001_139.backIt());
//     n_001.reorderIncidence(std::move(edges_001));
// 
//     vector<edge_list_it_t> edges_002;
//     edges_002.push_back(e_116_002.backIt());
//     edges_002.push_back(e_115_002.backIt());
//     edges_002.push_back(e_053_002.backIt());
//     edges_002.push_back(e_125_002.backIt());
//     edges_002.push_back(e_126_002.backIt());
//     edges_002.push_back(e_036_002.backIt());
//     edges_002.push_back(e_028_002.backIt());
//     edges_002.push_back(e_002_035.backIt());
//     edges_002.push_back(e_032_002.backIt());
//     edges_002.push_back(e_002_030.backIt());
//     edges_002.push_back(e_002_031.backIt());
//     edges_002.push_back(e_132_002.backIt());
//     edges_002.push_back(e_137_002.backIt());
//     edges_002.push_back(e_002_004.backIt());
//     edges_002.push_back(e_001_002.backIt());
//     edges_002.push_back(e_002_003.backIt());
//     n_002.reorderIncidence(std::move(edges_002));
// 
//     vector<edge_list_it_t> edges_003;
//     edges_003.push_back(e_003_114.backIt());
//     edges_003.push_back(e_002_003.backIt());
//     edges_003.push_back(e_139_003.backIt());
//     edges_003.push_back(e_003_141.backIt());
//     n_003.reorderIncidence(std::move(edges_003));
// 
//     vector<edge_list_it_t> edges_004;
//     edges_004.push_back(e_004_000.backIt());
//     edges_004.push_back(e_002_004.backIt());
//     edges_004.push_back(e_137_004.backIt());
//     edges_004.push_back(e_095_004.backIt());
//     n_004.reorderIncidence(std::move(edges_004));
// 
//     vector<edge_list_it_t> edges_005;
//     edges_005.push_back(e_013_005.backIt());
//     edges_005.push_back(e_007_005.backIt());
//     edges_005.push_back(e_005_006.backIt());
//     edges_005.push_back(e_005_014.backIt());
//     n_005.reorderIncidence(std::move(edges_005));
// 
//     vector<edge_list_it_t> edges_006;
//     edges_006.push_back(e_030_006.backIt());
//     edges_006.push_back(e_033_006.backIt());
//     edges_006.push_back(e_136_006.backIt());
//     edges_006.push_back(e_005_006.backIt());
//     edges_006.push_back(e_007_006.backIt());
//     edges_006.push_back(e_008_006.backIt());
//     edges_006.push_back(e_009_006.backIt());
//     edges_006.push_back(e_131_006.backIt());
//     edges_006.push_back(e_006_031.backIt());
//     n_006.reorderIncidence(std::move(edges_006));
// 
//     vector<edge_list_it_t> edges_007;
//     edges_007.push_back(e_012_007.backIt());
//     edges_007.push_back(e_008_007.backIt());
//     edges_007.push_back(e_007_006.backIt());
//     edges_007.push_back(e_007_005.backIt());
//     n_007.reorderIncidence(std::move(edges_007));
// 
//     vector<edge_list_it_t> edges_008;
//     edges_008.push_back(e_011_008.backIt());
//     edges_008.push_back(e_009_008.backIt());
//     edges_008.push_back(e_008_006.backIt());
//     edges_008.push_back(e_008_007.backIt());
//     n_008.reorderIncidence(std::move(edges_008));
// 
//     vector<edge_list_it_t> edges_009;
//     edges_009.push_back(e_009_011.backIt());
//     edges_009.push_back(e_009_044.backIt());
//     edges_009.push_back(e_045_009.backIt());
//     edges_009.push_back(e_009_010.backIt());
//     edges_009.push_back(e_133_009.backIt());
//     edges_009.push_back(e_009_134.backIt());
//     edges_009.push_back(e_009_131.backIt());
//     edges_009.push_back(e_009_006.backIt());
//     edges_009.push_back(e_009_008.backIt());
//     n_009.reorderIncidence(std::move(edges_009));
// 
//     vector<edge_list_it_t> edges_010;
//     edges_010.push_back(e_010_100.backIt());
//     edges_010.push_back(e_010_099.backIt());
//     edges_010.push_back(e_009_010.backIt());
//     edges_010.push_back(e_045_010.backIt());
//     n_010.reorderIncidence(std::move(edges_010));
// 
//     vector<edge_list_it_t> edges_011;
//     edges_011.push_back(e_044_011.backIt());
//     edges_011.push_back(e_009_011.backIt());
//     edges_011.push_back(e_011_008.backIt());
//     edges_011.push_back(e_011_012.backIt());
//     n_011.reorderIncidence(std::move(edges_011));
// 
//     vector<edge_list_it_t> edges_012;
//     edges_012.push_back(e_012_013.backIt());
//     edges_012.push_back(e_041_012.backIt());
//     edges_012.push_back(e_011_012.backIt());
//     edges_012.push_back(e_012_007.backIt());
//     n_012.reorderIncidence(std::move(edges_012));
// 
//     vector<edge_list_it_t> edges_013;
//     edges_013.push_back(e_027_013.backIt());
//     edges_013.push_back(e_041_013.backIt());
//     edges_013.push_back(e_012_013.backIt());
//     edges_013.push_back(e_013_005.backIt());
//     edges_013.push_back(e_014_013.backIt());
//     edges_013.push_back(e_029_013.backIt());
//     edges_013.push_back(e_028_013.backIt());
//     edges_013.push_back(e_025_013.backIt());
//     n_013.reorderIncidence(std::move(edges_013));
// 
//     vector<edge_list_it_t> edges_014;
//     edges_014.push_back(e_014_034.backIt());
//     edges_014.push_back(e_014_013.backIt());
//     edges_014.push_back(e_005_014.backIt());
//     edges_014.push_back(e_014_136.backIt());
//     n_014.reorderIncidence(std::move(edges_014));
// 
//     vector<edge_list_it_t> edges_015;
//     edges_015.push_back(e_015_024.backIt());
//     edges_015.push_back(e_015_016.backIt());
//     edges_015.push_back(e_015_019.backIt());
//     edges_015.push_back(e_017_015.backIt());
//     edges_015.push_back(e_015_018.backIt());
//     edges_015.push_back(e_015_023.backIt());
//     n_015.reorderIncidence(std::move(edges_015));
// 
//     vector<edge_list_it_t> edges_016;
//     edges_016.push_back(e_019_016.backIt());
//     edges_016.push_back(e_015_016.backIt());
//     edges_016.push_back(e_016_024.backIt());
//     edges_016.push_back(e_016_049.backIt());
//     n_016.reorderIncidence(std::move(edges_016));
// 
//     vector<edge_list_it_t> edges_017;
//     edges_017.push_back(e_017_018.backIt());
//     edges_017.push_back(e_017_015.backIt());
//     edges_017.push_back(e_019_017.backIt());
//     edges_017.push_back(e_020_017.backIt());
//     n_017.reorderIncidence(std::move(edges_017));
// 
//     vector<edge_list_it_t> edges_018;
//     edges_018.push_back(e_015_018.backIt());
//     edges_018.push_back(e_017_018.backIt());
//     edges_018.push_back(e_021_018.backIt());
//     edges_018.push_back(e_018_027.backIt());
//     edges_018.push_back(e_023_018.backIt());
//     n_018.reorderIncidence(std::move(edges_018));
// 
//     vector<edge_list_it_t> edges_019;
//     edges_019.push_back(e_022_019.backIt());
//     edges_019.push_back(e_019_020.backIt());
//     edges_019.push_back(e_019_017.backIt());
//     edges_019.push_back(e_015_019.backIt());
//     edges_019.push_back(e_019_016.backIt());
//     edges_019.push_back(e_019_049.backIt());
//     n_019.reorderIncidence(std::move(edges_019));
// 
//     vector<edge_list_it_t> edges_020;
//     edges_020.push_back(e_022_020.backIt());
//     edges_020.push_back(e_020_021.backIt());
//     edges_020.push_back(e_020_017.backIt());
//     edges_020.push_back(e_019_020.backIt());
//     n_020.reorderIncidence(std::move(edges_020));
// 
//     vector<edge_list_it_t> edges_021;
//     edges_021.push_back(e_021_018.backIt());
//     edges_021.push_back(e_020_021.backIt());
//     edges_021.push_back(e_022_021.backIt());
//     edges_021.push_back(e_021_046.backIt());
//     n_021.reorderIncidence(std::move(edges_021));
// 
//     vector<edge_list_it_t> edges_022;
//     edges_022.push_back(e_022_021.backIt());
//     edges_022.push_back(e_022_020.backIt());
//     edges_022.push_back(e_022_019.backIt());
//     edges_022.push_back(e_022_049.backIt());
//     edges_022.push_back(e_022_048.backIt());
//     edges_022.push_back(e_022_047.backIt());
//     n_022.reorderIncidence(std::move(edges_022));
// 
//     vector<edge_list_it_t> edges_023;
//     edges_023.push_back(e_024_023.backIt());
//     edges_023.push_back(e_015_023.backIt());
//     edges_023.push_back(e_023_018.backIt());
//     edges_023.push_back(e_023_026.backIt());
//     n_023.reorderIncidence(std::move(edges_023));
// 
//     vector<edge_list_it_t> edges_024;
//     edges_024.push_back(e_024_039.backIt());
//     edges_024.push_back(e_040_024.backIt());
//     edges_024.push_back(e_048_024.backIt());
//     edges_024.push_back(e_024_049.backIt());
//     edges_024.push_back(e_016_024.backIt());
//     edges_024.push_back(e_015_024.backIt());
//     edges_024.push_back(e_024_023.backIt());
//     n_024.reorderIncidence(std::move(edges_024));
// 
//     vector<edge_list_it_t> edges_025;
//     edges_025.push_back(e_027_025.backIt());
//     edges_025.push_back(e_025_013.backIt());
//     edges_025.push_back(e_025_028.backIt());
//     edges_025.push_back(e_026_025.backIt());
//     n_025.reorderIncidence(std::move(edges_025));
// 
//     vector<edge_list_it_t> edges_026;
//     edges_026.push_back(e_039_026.backIt());
//     edges_026.push_back(e_023_026.backIt());
//     edges_026.push_back(e_026_025.backIt());
//     edges_026.push_back(e_026_028.backIt());
//     n_026.reorderIncidence(std::move(edges_026));
// 
//     vector<edge_list_it_t> edges_027;
//     edges_027.push_back(e_018_027.backIt());
//     edges_027.push_back(e_046_027.backIt());
//     edges_027.push_back(e_027_013.backIt());
//     edges_027.push_back(e_027_025.backIt());
//     n_027.reorderIncidence(std::move(edges_027));
// 
//     vector<edge_list_it_t> edges_028;
//     edges_028.push_back(e_028_130.backIt());
//     edges_028.push_back(e_028_037.backIt());
//     edges_028.push_back(e_028_038.backIt());
//     edges_028.push_back(e_026_028.backIt());
//     edges_028.push_back(e_025_028.backIt());
//     edges_028.push_back(e_028_013.backIt());
//     edges_028.push_back(e_028_029.backIt());
//     edges_028.push_back(e_028_035.backIt());
//     edges_028.push_back(e_028_002.backIt());
//     n_028.reorderIncidence(std::move(edges_028));
// 
//     vector<edge_list_it_t> edges_029;
//     edges_029.push_back(e_028_029.backIt());
//     edges_029.push_back(e_029_013.backIt());
//     edges_029.push_back(e_029_033.backIt());
//     edges_029.push_back(e_035_029.backIt());
//     n_029.reorderIncidence(std::move(edges_029));
// 
//     vector<edge_list_it_t> edges_030;
//     edges_030.push_back(e_032_030.backIt());
//     edges_030.push_back(e_030_006.backIt());
//     edges_030.push_back(e_030_031.backIt());
//     edges_030.push_back(e_002_030.backIt());
//     n_030.reorderIncidence(std::move(edges_030));
// 
//     vector<edge_list_it_t> edges_031;
//     edges_031.push_back(e_030_031.backIt());
//     edges_031.push_back(e_006_031.backIt());
//     edges_031.push_back(e_131_031.backIt());
//     edges_031.push_back(e_132_031.backIt());
//     edges_031.push_back(e_002_031.backIt());
//     n_031.reorderIncidence(std::move(edges_031));
// 
//     vector<edge_list_it_t> edges_032;
//     edges_032.push_back(e_035_032.backIt());
//     edges_032.push_back(e_033_032.backIt());
//     edges_032.push_back(e_032_030.backIt());
//     edges_032.push_back(e_032_002.backIt());
//     n_032.reorderIncidence(std::move(edges_032));
// 
//     vector<edge_list_it_t> edges_033;
//     edges_033.push_back(e_033_032.backIt());
//     edges_033.push_back(e_029_033.backIt());
//     edges_033.push_back(e_034_033.backIt());
//     edges_033.push_back(e_033_006.backIt());
//     n_033.reorderIncidence(std::move(edges_033));
// 
//     vector<edge_list_it_t> edges_034;
//     edges_034.push_back(e_034_033.backIt());
//     edges_034.push_back(e_014_034.backIt());
//     n_034.reorderIncidence(std::move(edges_034));
// 
//     vector<edge_list_it_t> edges_035;
//     edges_035.push_back(e_028_035.backIt());
//     edges_035.push_back(e_035_029.backIt());
//     edges_035.push_back(e_035_032.backIt());
//     edges_035.push_back(e_002_035.backIt());
//     n_035.reorderIncidence(std::move(edges_035));
// 
//     vector<edge_list_it_t> edges_036;
//     edges_036.push_back(e_036_126.backIt());
//     edges_036.push_back(e_037_036.backIt());
//     edges_036.push_back(e_130_036.backIt());
//     edges_036.push_back(e_036_002.backIt());
//     n_036.reorderIncidence(std::move(edges_036));
// 
//     vector<edge_list_it_t> edges_037;
//     edges_037.push_back(e_037_036.backIt());
//     edges_037.push_back(e_037_052.backIt());
//     edges_037.push_back(e_038_037.backIt());
//     edges_037.push_back(e_028_037.backIt());
//     n_037.reorderIncidence(std::move(edges_037));
// 
//     vector<edge_list_it_t> edges_038;
//     edges_038.push_back(e_038_037.backIt());
//     edges_038.push_back(e_038_051.backIt());
//     edges_038.push_back(e_039_038.backIt());
//     edges_038.push_back(e_028_038.backIt());
//     n_038.reorderIncidence(std::move(edges_038));
// 
//     vector<edge_list_it_t> edges_039;
//     edges_039.push_back(e_039_038.backIt());
//     edges_039.push_back(e_050_039.backIt());
//     edges_039.push_back(e_024_039.backIt());
//     edges_039.push_back(e_039_026.backIt());
//     n_039.reorderIncidence(std::move(edges_039));
// 
//     vector<edge_list_it_t> edges_040;
//     edges_040.push_back(e_040_048.backIt());
//     edges_040.push_back(e_040_024.backIt());
//     edges_040.push_back(e_050_040.backIt());
//     edges_040.push_back(e_047_040.backIt());
//     n_040.reorderIncidence(std::move(edges_040));
// 
//     vector<edge_list_it_t> edges_041;
//     edges_041.push_back(e_041_013.backIt());
//     edges_041.push_back(e_046_041.backIt());
//     edges_041.push_back(e_042_041.backIt());
//     edges_041.push_back(e_041_012.backIt());
//     n_041.reorderIncidence(std::move(edges_041));
// 
//     vector<edge_list_it_t> edges_042;
//     edges_042.push_back(e_042_041.backIt());
//     edges_042.push_back(e_042_043.backIt());
//     edges_042.push_back(e_045_042.backIt());
//     edges_042.push_back(e_044_042.backIt());
//     n_042.reorderIncidence(std::move(edges_042));
// 
//     vector<edge_list_it_t> edges_043;
//     edges_043.push_back(e_043_047.backIt());
//     edges_043.push_back(e_045_043.backIt());
//     edges_043.push_back(e_042_043.backIt());
//     edges_043.push_back(e_043_046.backIt());
//     n_043.reorderIncidence(std::move(edges_043));
// 
//     vector<edge_list_it_t> edges_044;
//     edges_044.push_back(e_044_042.backIt());
//     edges_044.push_back(e_045_044.backIt());
//     edges_044.push_back(e_009_044.backIt());
//     edges_044.push_back(e_044_011.backIt());
//     n_044.reorderIncidence(std::move(edges_044));
// 
//     vector<edge_list_it_t> edges_045;
//     edges_045.push_back(e_045_044.backIt());
//     edges_045.push_back(e_045_042.backIt());
//     edges_045.push_back(e_045_043.backIt());
//     edges_045.push_back(e_047_045.backIt());
//     edges_045.push_back(e_045_055.backIt());
//     edges_045.push_back(e_045_062.backIt());
//     edges_045.push_back(e_063_045.backIt());
//     edges_045.push_back(e_045_093.backIt());
//     edges_045.push_back(e_045_100.backIt());
//     edges_045.push_back(e_045_010.backIt());
//     edges_045.push_back(e_045_009.backIt());
//     n_045.reorderIncidence(std::move(edges_045));
// 
//     vector<edge_list_it_t> edges_046;
//     edges_046.push_back(e_021_046.backIt());
//     edges_046.push_back(e_043_046.backIt());
//     edges_046.push_back(e_046_041.backIt());
//     edges_046.push_back(e_046_027.backIt());
//     n_046.reorderIncidence(std::move(edges_046));
// 
//     vector<edge_list_it_t> edges_047;
//     edges_047.push_back(e_022_047.backIt());
//     edges_047.push_back(e_047_040.backIt());
//     edges_047.push_back(e_047_045.backIt());
//     edges_047.push_back(e_043_047.backIt());
//     n_047.reorderIncidence(std::move(edges_047));
// 
//     vector<edge_list_it_t> edges_048;
//     edges_048.push_back(e_022_048.backIt());
//     edges_048.push_back(e_048_049.backIt());
//     edges_048.push_back(e_048_024.backIt());
//     edges_048.push_back(e_040_048.backIt());
//     n_048.reorderIncidence(std::move(edges_048));
// 
//     vector<edge_list_it_t> edges_049;
//     edges_049.push_back(e_022_049.backIt());
//     edges_049.push_back(e_019_049.backIt());
//     edges_049.push_back(e_016_049.backIt());
//     edges_049.push_back(e_024_049.backIt());
//     edges_049.push_back(e_048_049.backIt());
//     n_049.reorderIncidence(std::move(edges_049));
// 
//     vector<edge_list_it_t> edges_050;
//     edges_050.push_back(e_050_040.backIt());
//     edges_050.push_back(e_050_039.backIt());
//     edges_050.push_back(e_051_050.backIt());
//     edges_050.push_back(e_055_050.backIt());
//     n_050.reorderIncidence(std::move(edges_050));
// 
//     vector<edge_list_it_t> edges_051;
//     edges_051.push_back(e_051_050.backIt());
//     edges_051.push_back(e_038_051.backIt());
//     edges_051.push_back(e_052_051.backIt());
//     edges_051.push_back(e_051_054.backIt());
//     n_051.reorderIncidence(std::move(edges_051));
// 
//     vector<edge_list_it_t> edges_052;
//     edges_052.push_back(e_052_056.backIt());
//     edges_052.push_back(e_054_052.backIt());
//     edges_052.push_back(e_052_051.backIt());
//     edges_052.push_back(e_037_052.backIt());
//     edges_052.push_back(e_128_052.backIt());
//     edges_052.push_back(e_052_127.backIt());
//     edges_052.push_back(e_052_125.backIt());
//     n_052.reorderIncidence(std::move(edges_052));
// 
//     vector<edge_list_it_t> edges_053;
//     edges_053.push_back(e_053_115.backIt());
//     edges_053.push_back(e_056_053.backIt());
//     edges_053.push_back(e_125_053.backIt());
//     edges_053.push_back(e_053_002.backIt());
//     n_053.reorderIncidence(std::move(edges_053));
// 
//     vector<edge_list_it_t> edges_054;
//     edges_054.push_back(e_051_054.backIt());
//     edges_054.push_back(e_054_052.backIt());
//     edges_054.push_back(e_054_056.backIt());
//     edges_054.push_back(e_055_054.backIt());
//     n_054.reorderIncidence(std::move(edges_054));
// 
//     vector<edge_list_it_t> edges_055;
//     edges_055.push_back(e_055_050.backIt());
//     edges_055.push_back(e_055_054.backIt());
//     edges_055.push_back(e_056_055.backIt());
//     edges_055.push_back(e_045_055.backIt());
//     n_055.reorderIncidence(std::move(edges_055));
// 
//     vector<edge_list_it_t> edges_056;
//     edges_056.push_back(e_056_053.backIt());
//     edges_056.push_back(e_071_056.backIt());
//     edges_056.push_back(e_073_056.backIt());
//     edges_056.push_back(e_124_056.backIt());
//     edges_056.push_back(e_056_060.backIt());
//     edges_056.push_back(e_056_061.backIt());
//     edges_056.push_back(e_062_056.backIt());
//     edges_056.push_back(e_056_055.backIt());
//     edges_056.push_back(e_054_056.backIt());
//     edges_056.push_back(e_052_056.backIt());
//     n_056.reorderIncidence(std::move(edges_056));
// 
//     vector<edge_list_it_t> edges_057;
//     edges_057.push_back(e_057_058.backIt());
//     edges_057.push_back(e_059_057.backIt());
//     n_057.reorderIncidence(std::move(edges_057));
// 
//     vector<edge_list_it_t> edges_058;
//     edges_058.push_back(e_124_058.backIt());
//     edges_058.push_back(e_070_058.backIt());
//     edges_058.push_back(e_058_066.backIt());
//     edges_058.push_back(e_057_058.backIt());
//     edges_058.push_back(e_059_058.backIt());
//     n_058.reorderIncidence(std::move(edges_058));
// 
//     vector<edge_list_it_t> edges_059;
//     edges_059.push_back(e_059_058.backIt());
//     edges_059.push_back(e_059_057.backIt());
//     edges_059.push_back(e_059_066.backIt());
//     edges_059.push_back(e_060_059.backIt());
//     n_059.reorderIncidence(std::move(edges_059));
// 
//     vector<edge_list_it_t> edges_060;
//     edges_060.push_back(e_056_060.backIt());
//     edges_060.push_back(e_060_059.backIt());
//     edges_060.push_back(e_060_065.backIt());
//     edges_060.push_back(e_061_060.backIt());
//     n_060.reorderIncidence(std::move(edges_060));
// 
//     vector<edge_list_it_t> edges_061;
//     edges_061.push_back(e_056_061.backIt());
//     edges_061.push_back(e_061_060.backIt());
//     edges_061.push_back(e_061_064.backIt());
//     edges_061.push_back(e_062_061.backIt());
//     n_061.reorderIncidence(std::move(edges_061));
// 
//     vector<edge_list_it_t> edges_062;
//     edges_062.push_back(e_062_056.backIt());
//     edges_062.push_back(e_062_061.backIt());
//     edges_062.push_back(e_063_062.backIt());
//     edges_062.push_back(e_045_062.backIt());
//     n_062.reorderIncidence(std::move(edges_062));
// 
//     vector<edge_list_it_t> edges_063;
//     edges_063.push_back(e_093_063.backIt());
//     edges_063.push_back(e_063_045.backIt());
//     edges_063.push_back(e_063_062.backIt());
//     edges_063.push_back(e_064_063.backIt());
//     n_063.reorderIncidence(std::move(edges_063));
// 
//     vector<edge_list_it_t> edges_064;
//     edges_064.push_back(e_064_092.backIt());
//     edges_064.push_back(e_064_063.backIt());
//     edges_064.push_back(e_061_064.backIt());
//     edges_064.push_back(e_065_064.backIt());
//     n_064.reorderIncidence(std::move(edges_064));
// 
//     vector<edge_list_it_t> edges_065;
//     edges_065.push_back(e_065_091.backIt());
//     edges_065.push_back(e_065_064.backIt());
//     edges_065.push_back(e_060_065.backIt());
//     edges_065.push_back(e_067_065.backIt());
//     n_065.reorderIncidence(std::move(edges_065));
// 
//     vector<edge_list_it_t> edges_066;
//     edges_066.push_back(e_058_066.backIt());
//     edges_066.push_back(e_070_066.backIt());
//     edges_066.push_back(e_066_068.backIt());
//     edges_066.push_back(e_059_066.backIt());
//     n_066.reorderIncidence(std::move(edges_066));
// 
//     vector<edge_list_it_t> edges_067;
//     edges_067.push_back(e_067_065.backIt());
//     edges_067.push_back(e_067_068.backIt());
//     edges_067.push_back(e_069_067.backIt());
//     edges_067.push_back(e_102_067.backIt());
//     n_067.reorderIncidence(std::move(edges_067));
// 
//     vector<edge_list_it_t> edges_068;
//     edges_068.push_back(e_069_068.backIt());
//     edges_068.push_back(e_067_068.backIt());
//     edges_068.push_back(e_066_068.backIt());
//     n_068.reorderIncidence(std::move(edges_068));
// 
//     vector<edge_list_it_t> edges_069;
//     edges_069.push_back(e_069_067.backIt());
//     edges_069.push_back(e_069_068.backIt());
//     edges_069.push_back(e_123_069.backIt());
//     edges_069.push_back(e_103_069.backIt());
//     n_069.reorderIncidence(std::move(edges_069));
// 
//     vector<edge_list_it_t> edges_070;
//     edges_070.push_back(e_123_070.backIt());
//     edges_070.push_back(e_070_066.backIt());
//     edges_070.push_back(e_070_058.backIt());
//     edges_070.push_back(e_072_070.backIt());
//     edges_070.push_back(e_070_115.backIt());
//     edges_070.push_back(e_117_070.backIt());
//     edges_070.push_back(e_118_070.backIt());
//     edges_070.push_back(e_120_070.backIt());
//     edges_070.push_back(e_081_070.backIt());
//     edges_070.push_back(e_122_070.backIt());
//     edges_070.push_back(e_070_103.backIt());
//     n_070.reorderIncidence(std::move(edges_070));
// 
//     vector<edge_list_it_t> edges_071;
//     edges_071.push_back(e_074_071.backIt());
//     edges_071.push_back(e_072_071.backIt());
//     edges_071.push_back(e_073_071.backIt());
//     edges_071.push_back(e_071_056.backIt());
//     n_071.reorderIncidence(std::move(edges_071));
// 
//     vector<edge_list_it_t> edges_072;
//     edges_072.push_back(e_072_070.backIt());
//     edges_072.push_back(e_072_071.backIt());
//     edges_072.push_back(e_074_072.backIt());
//     n_072.reorderIncidence(std::move(edges_072));
// 
//     vector<edge_list_it_t> edges_073;
//     edges_073.push_back(e_073_071.backIt());
//     edges_073.push_back(e_073_056.backIt());
//     n_073.reorderIncidence(std::move(edges_073));
// 
//     vector<edge_list_it_t> edges_074;
//     edges_074.push_back(e_074_072.backIt());
//     edges_074.push_back(e_074_071.backIt());
//     n_074.reorderIncidence(std::move(edges_074));
// 
//     vector<edge_list_it_t> edges_075;
//     edges_075.push_back(e_075_122.backIt());
//     edges_075.push_back(e_075_076.backIt());
//     edges_075.push_back(e_075_077.backIt());
//     n_075.reorderIncidence(std::move(edges_075));
// 
//     vector<edge_list_it_t> edges_076;
//     edges_076.push_back(e_075_076.backIt());
//     edges_076.push_back(e_078_076.backIt());
//     edges_076.push_back(e_077_076.backIt());
//     n_076.reorderIncidence(std::move(edges_076));
// 
//     vector<edge_list_it_t> edges_077;
//     edges_077.push_back(e_075_077.backIt());
//     edges_077.push_back(e_077_076.backIt());
//     edges_077.push_back(e_077_078.backIt());
//     edges_077.push_back(e_077_079.backIt());
//     edges_077.push_back(e_081_077.backIt());
//     edges_077.push_back(e_106_077.backIt());
//     n_077.reorderIncidence(std::move(edges_077));
// 
//     vector<edge_list_it_t> edges_078;
//     edges_078.push_back(e_078_076.backIt());
//     edges_078.push_back(e_079_078.backIt());
//     edges_078.push_back(e_080_078.backIt());
//     edges_078.push_back(e_077_078.backIt());
//     n_078.reorderIncidence(std::move(edges_078));
// 
//     vector<edge_list_it_t> edges_079;
//     edges_079.push_back(e_080_079.backIt());
//     edges_079.push_back(e_079_078.backIt());
//     edges_079.push_back(e_077_079.backIt());
//     n_079.reorderIncidence(std::move(edges_079));
// 
//     vector<edge_list_it_t> edges_080;
//     edges_080.push_back(e_080_078.backIt());
//     edges_080.push_back(e_080_079.backIt());
//     n_080.reorderIncidence(std::move(edges_080));
// 
//     vector<edge_list_it_t> edges_081;
//     edges_081.push_back(e_081_077.backIt());
//     edges_081.push_back(e_081_070.backIt());
//     edges_081.push_back(e_081_120.backIt());
//     edges_081.push_back(e_081_121.backIt());
//     edges_081.push_back(e_081_118.backIt());
//     edges_081.push_back(e_081_108.backIt());
//     edges_081.push_back(e_081_107.backIt());
//     edges_081.push_back(e_081_106.backIt());
//     n_081.reorderIncidence(std::move(edges_081));
// 
//     vector<edge_list_it_t> edges_082;
//     edges_082.push_back(e_082_087.backIt());
//     edges_082.push_back(e_082_118.backIt());
//     edges_082.push_back(e_082_119.backIt());
//     edges_082.push_back(e_082_112.backIt());
//     edges_082.push_back(e_082_083.backIt());
//     edges_082.push_back(e_082_084.backIt());
//     edges_082.push_back(e_086_082.backIt());
//     n_082.reorderIncidence(std::move(edges_082));
// 
//     vector<edge_list_it_t> edges_083;
//     edges_083.push_back(e_082_083.backIt());
//     edges_083.push_back(e_112_083.backIt());
//     edges_083.push_back(e_083_085.backIt());
//     edges_083.push_back(e_083_084.backIt());
//     n_083.reorderIncidence(std::move(edges_083));
// 
//     vector<edge_list_it_t> edges_084;
//     edges_084.push_back(e_082_084.backIt());
//     edges_084.push_back(e_083_084.backIt());
//     edges_084.push_back(e_084_111.backIt());
//     edges_084.push_back(e_085_084.backIt());
//     edges_084.push_back(e_109_084.backIt());
//     edges_084.push_back(e_086_084.backIt());
//     n_084.reorderIncidence(std::move(edges_084));
// 
//     vector<edge_list_it_t> edges_085;
//     edges_085.push_back(e_085_084.backIt());
//     edges_085.push_back(e_085_111.backIt());
//     edges_085.push_back(e_083_085.backIt());
//     edges_085.push_back(e_085_105.backIt());
//     edges_085.push_back(e_086_085.backIt());
//     n_085.reorderIncidence(std::move(edges_085));
// 
//     vector<edge_list_it_t> edges_086;
//     edges_086.push_back(e_086_108.backIt());
//     edges_086.push_back(e_086_087.backIt());
//     edges_086.push_back(e_086_082.backIt());
//     edges_086.push_back(e_086_084.backIt());
//     edges_086.push_back(e_086_110.backIt());
//     edges_086.push_back(e_086_109.backIt());
//     edges_086.push_back(e_086_085.backIt());
//     edges_086.push_back(e_086_088.backIt());
//     edges_086.push_back(e_107_086.backIt());
//     n_086.reorderIncidence(std::move(edges_086));
// 
//     vector<edge_list_it_t> edges_087;
//     edges_087.push_back(e_087_108.backIt());
//     edges_087.push_back(e_087_118.backIt());
//     edges_087.push_back(e_082_087.backIt());
//     edges_087.push_back(e_086_087.backIt());
//     n_087.reorderIncidence(std::move(edges_087));
// 
//     vector<edge_list_it_t> edges_088;
//     edges_088.push_back(e_104_088.backIt());
//     edges_088.push_back(e_086_088.backIt());
//     edges_088.push_back(e_088_102.backIt());
//     edges_088.push_back(e_088_105.backIt());
//     n_088.reorderIncidence(std::move(edges_088));
// 
//     vector<edge_list_it_t> edges_089;
//     edges_089.push_back(e_089_094.backIt());
//     edges_089.push_back(e_091_089.backIt());
//     edges_089.push_back(e_101_089.backIt());
//     edges_089.push_back(e_089_090.backIt());
//     n_089.reorderIncidence(std::move(edges_089));
// 
//     vector<edge_list_it_t> edges_090;
//     edges_090.push_back(e_089_090.backIt());
//     edges_090.push_back(e_101_090.backIt());
//     edges_090.push_back(e_105_090.backIt());
//     edges_090.push_back(e_090_113.backIt());
//     edges_090.push_back(e_090_140.backIt());
//     edges_090.push_back(e_139_090.backIt());
//     edges_090.push_back(e_138_090.backIt());
//     edges_090.push_back(e_000_090.backIt());
//     n_090.reorderIncidence(std::move(edges_090));
// 
//     vector<edge_list_it_t> edges_091;
//     edges_091.push_back(e_091_096.backIt());
//     edges_091.push_back(e_092_091.backIt());
//     edges_091.push_back(e_065_091.backIt());
//     edges_091.push_back(e_091_089.backIt());
//     n_091.reorderIncidence(std::move(edges_091));
// 
//     vector<edge_list_it_t> edges_092;
//     edges_092.push_back(e_092_091.backIt());
//     edges_092.push_back(e_092_098.backIt());
//     edges_092.push_back(e_093_092.backIt());
//     edges_092.push_back(e_064_092.backIt());
//     n_092.reorderIncidence(std::move(edges_092));
// 
//     vector<edge_list_it_t> edges_093;
//     edges_093.push_back(e_093_092.backIt());
//     edges_093.push_back(e_100_093.backIt());
//     edges_093.push_back(e_045_093.backIt());
//     edges_093.push_back(e_093_063.backIt());
//     n_093.reorderIncidence(std::move(edges_093));
// 
//     vector<edge_list_it_t> edges_094;
//     edges_094.push_back(e_094_095.backIt());
//     edges_094.push_back(e_096_094.backIt());
//     edges_094.push_back(e_089_094.backIt());
//     edges_094.push_back(e_094_000.backIt());
//     n_094.reorderIncidence(std::move(edges_094));
// 
//     vector<edge_list_it_t> edges_095;
//     edges_095.push_back(e_095_004.backIt());
//     edges_095.push_back(e_095_135.backIt());
//     edges_095.push_back(e_097_095.backIt());
//     edges_095.push_back(e_094_095.backIt());
//     n_095.reorderIncidence(std::move(edges_095));
// 
//     vector<edge_list_it_t> edges_096;
//     edges_096.push_back(e_096_094.backIt());
//     edges_096.push_back(e_096_097.backIt());
//     edges_096.push_back(e_098_096.backIt());
//     edges_096.push_back(e_091_096.backIt());
//     n_096.reorderIncidence(std::move(edges_096));
// 
//     vector<edge_list_it_t> edges_097;
//     edges_097.push_back(e_096_097.backIt());
//     edges_097.push_back(e_097_095.backIt());
//     edges_097.push_back(e_097_135.backIt());
//     edges_097.push_back(e_099_097.backIt());
//     n_097.reorderIncidence(std::move(edges_097));
// 
//     vector<edge_list_it_t> edges_098;
//     edges_098.push_back(e_100_098.backIt());
//     edges_098.push_back(e_092_098.backIt());
//     edges_098.push_back(e_098_096.backIt());
//     edges_098.push_back(e_098_099.backIt());
//     n_098.reorderIncidence(std::move(edges_098));
// 
//     vector<edge_list_it_t> edges_099;
//     edges_099.push_back(e_098_099.backIt());
//     edges_099.push_back(e_099_097.backIt());
//     edges_099.push_back(e_099_133.backIt());
//     edges_099.push_back(e_010_099.backIt());
//     n_099.reorderIncidence(std::move(edges_099));
// 
//     vector<edge_list_it_t> edges_100;
//     edges_100.push_back(e_100_093.backIt());
//     edges_100.push_back(e_100_098.backIt());
//     edges_100.push_back(e_010_100.backIt());
//     edges_100.push_back(e_045_100.backIt());
//     n_100.reorderIncidence(std::move(edges_100));
// 
//     vector<edge_list_it_t> edges_101;
//     edges_101.push_back(e_102_101.backIt());
//     edges_101.push_back(e_105_101.backIt());
//     edges_101.push_back(e_101_090.backIt());
//     edges_101.push_back(e_101_089.backIt());
//     n_101.reorderIncidence(std::move(edges_101));
// 
//     vector<edge_list_it_t> edges_102;
//     edges_102.push_back(e_102_067.backIt());
//     edges_102.push_back(e_103_102.backIt());
//     edges_102.push_back(e_088_102.backIt());
//     edges_102.push_back(e_102_101.backIt());
//     n_102.reorderIncidence(std::move(edges_102));
// 
//     vector<edge_list_it_t> edges_103;
//     edges_103.push_back(e_103_069.backIt());
//     edges_103.push_back(e_070_103.backIt());
//     edges_103.push_back(e_104_103.backIt());
//     edges_103.push_back(e_103_102.backIt());
//     n_103.reorderIncidence(std::move(edges_103));
// 
//     vector<edge_list_it_t> edges_104;
//     edges_104.push_back(e_106_104.backIt());
//     edges_104.push_back(e_107_104.backIt());
//     edges_104.push_back(e_104_088.backIt());
//     edges_104.push_back(e_104_103.backIt());
//     n_104.reorderIncidence(std::move(edges_104));
// 
//     vector<edge_list_it_t> edges_105;
//     edges_105.push_back(e_088_105.backIt());
//     edges_105.push_back(e_085_105.backIt());
//     edges_105.push_back(e_105_101.backIt());
//     edges_105.push_back(e_105_090.backIt());
//     n_105.reorderIncidence(std::move(edges_105));
// 
//     vector<edge_list_it_t> edges_106;
//     edges_106.push_back(e_106_077.backIt());
//     edges_106.push_back(e_081_106.backIt());
//     edges_106.push_back(e_106_104.backIt());
//     n_106.reorderIncidence(std::move(edges_106));
// 
//     vector<edge_list_it_t> edges_107;
//     edges_107.push_back(e_081_107.backIt());
//     edges_107.push_back(e_108_107.backIt());
//     edges_107.push_back(e_107_086.backIt());
//     edges_107.push_back(e_107_104.backIt());
//     n_107.reorderIncidence(std::move(edges_107));
// 
//     vector<edge_list_it_t> edges_108;
//     edges_108.push_back(e_081_108.backIt());
//     edges_108.push_back(e_118_108.backIt());
//     edges_108.push_back(e_087_108.backIt());
//     edges_108.push_back(e_086_108.backIt());
//     edges_108.push_back(e_108_107.backIt());
//     n_108.reorderIncidence(std::move(edges_108));
// 
//     vector<edge_list_it_t> edges_109;
//     edges_109.push_back(e_109_084.backIt());
//     edges_109.push_back(e_086_109.backIt());
//     edges_109.push_back(e_109_110.backIt());
//     n_109.reorderIncidence(std::move(edges_109));
// 
//     vector<edge_list_it_t> edges_110;
//     edges_110.push_back(e_109_110.backIt());
//     edges_110.push_back(e_086_110.backIt());
//     n_110.reorderIncidence(std::move(edges_110));
// 
//     vector<edge_list_it_t> edges_111;
//     edges_111.push_back(e_084_111.backIt());
//     edges_111.push_back(e_085_111.backIt());
//     n_111.reorderIncidence(std::move(edges_111));
// 
//     vector<edge_list_it_t> edges_112;
//     edges_112.push_back(e_082_112.backIt());
//     edges_112.push_back(e_114_112.backIt());
//     edges_112.push_back(e_112_113.backIt());
//     edges_112.push_back(e_112_083.backIt());
//     n_112.reorderIncidence(std::move(edges_112));
// 
//     vector<edge_list_it_t> edges_113;
//     edges_113.push_back(e_112_113.backIt());
//     edges_113.push_back(e_141_113.backIt());
//     edges_113.push_back(e_139_113.backIt());
//     edges_113.push_back(e_113_140.backIt());
//     edges_113.push_back(e_090_113.backIt());
//     n_113.reorderIncidence(std::move(edges_113));
// 
//     vector<edge_list_it_t> edges_114;
//     edges_114.push_back(e_114_119.backIt());
//     edges_114.push_back(e_116_114.backIt());
//     edges_114.push_back(e_003_114.backIt());
//     edges_114.push_back(e_114_112.backIt());
//     n_114.reorderIncidence(std::move(edges_114));
// 
//     vector<edge_list_it_t> edges_115;
//     edges_115.push_back(e_115_116.backIt());
//     edges_115.push_back(e_070_115.backIt());
//     edges_115.push_back(e_053_115.backIt());
//     edges_115.push_back(e_115_002.backIt());
//     n_115.reorderIncidence(std::move(edges_115));
// 
//     vector<edge_list_it_t> edges_116;
//     edges_116.push_back(e_116_114.backIt());
//     edges_116.push_back(e_117_116.backIt());
//     edges_116.push_back(e_115_116.backIt());
//     edges_116.push_back(e_116_002.backIt());
//     n_116.reorderIncidence(std::move(edges_116));
// 
//     vector<edge_list_it_t> edges_117;
//     edges_117.push_back(e_117_118.backIt());
//     edges_117.push_back(e_117_070.backIt());
//     edges_117.push_back(e_117_116.backIt());
//     n_117.reorderIncidence(std::move(edges_117));
// 
//     vector<edge_list_it_t> edges_118;
//     edges_118.push_back(e_118_119.backIt());
//     edges_118.push_back(e_082_118.backIt());
//     edges_118.push_back(e_087_118.backIt());
//     edges_118.push_back(e_118_108.backIt());
//     edges_118.push_back(e_081_118.backIt());
//     edges_118.push_back(e_120_118.backIt());
//     edges_118.push_back(e_118_070.backIt());
//     edges_118.push_back(e_117_118.backIt());
//     n_118.reorderIncidence(std::move(edges_118));
// 
//     vector<edge_list_it_t> edges_119;
//     edges_119.push_back(e_082_119.backIt());
//     edges_119.push_back(e_118_119.backIt());
//     edges_119.push_back(e_114_119.backIt());
//     n_119.reorderIncidence(std::move(edges_119));
// 
//     vector<edge_list_it_t> edges_120;
//     edges_120.push_back(e_120_121.backIt());
//     edges_120.push_back(e_081_120.backIt());
//     edges_120.push_back(e_120_070.backIt());
//     edges_120.push_back(e_120_118.backIt());
//     n_120.reorderIncidence(std::move(edges_120));
// 
//     vector<edge_list_it_t> edges_121;
//     edges_121.push_back(e_081_121.backIt());
//     edges_121.push_back(e_120_121.backIt());
//     n_121.reorderIncidence(std::move(edges_121));
// 
//     vector<edge_list_it_t> edges_122;
//     edges_122.push_back(e_122_070.backIt());
//     edges_122.push_back(e_075_122.backIt());
//     n_122.reorderIncidence(std::move(edges_122));
// 
//     vector<edge_list_it_t> edges_123;
//     edges_123.push_back(e_123_069.backIt());
//     edges_123.push_back(e_123_070.backIt());
//     n_123.reorderIncidence(std::move(edges_123));
// 
//     vector<edge_list_it_t> edges_124;
//     edges_124.push_back(e_124_056.backIt());
//     edges_124.push_back(e_124_058.backIt());
//     n_124.reorderIncidence(std::move(edges_124));
// 
//     vector<edge_list_it_t> edges_125;
//     edges_125.push_back(e_125_053.backIt());
//     edges_125.push_back(e_052_125.backIt());
//     edges_125.push_back(e_126_125.backIt());
//     edges_125.push_back(e_125_002.backIt());
//     n_125.reorderIncidence(std::move(edges_125));
// 
//     vector<edge_list_it_t> edges_126;
//     edges_126.push_back(e_126_125.backIt());
//     edges_126.push_back(e_129_126.backIt());
//     edges_126.push_back(e_036_126.backIt());
//     edges_126.push_back(e_126_002.backIt());
//     n_126.reorderIncidence(std::move(edges_126));
// 
//     vector<edge_list_it_t> edges_127;
//     edges_127.push_back(e_052_127.backIt());
//     edges_127.push_back(e_128_127.backIt());
//     edges_127.push_back(e_129_127.backIt());
//     n_127.reorderIncidence(std::move(edges_127));
// 
//     vector<edge_list_it_t> edges_128;
//     edges_128.push_back(e_128_052.backIt());
//     edges_128.push_back(e_129_128.backIt());
//     edges_128.push_back(e_128_127.backIt());
//     n_128.reorderIncidence(std::move(edges_128));
// 
//     vector<edge_list_it_t> edges_129;
//     edges_129.push_back(e_129_127.backIt());
//     edges_129.push_back(e_129_128.backIt());
//     edges_129.push_back(e_129_126.backIt());
//     n_129.reorderIncidence(std::move(edges_129));
// 
//     vector<edge_list_it_t> edges_130;
//     edges_130.push_back(e_130_036.backIt());
//     edges_130.push_back(e_028_130.backIt());
//     n_130.reorderIncidence(std::move(edges_130));
// 
//     vector<edge_list_it_t> edges_131;
//     edges_131.push_back(e_134_131.backIt());
//     edges_131.push_back(e_131_031.backIt());
//     edges_131.push_back(e_131_006.backIt());
//     edges_131.push_back(e_009_131.backIt());
//     n_131.reorderIncidence(std::move(edges_131));
// 
//     vector<edge_list_it_t> edges_132;
//     edges_132.push_back(e_135_132.backIt());
//     edges_132.push_back(e_132_002.backIt());
//     edges_132.push_back(e_132_031.backIt());
//     edges_132.push_back(e_134_132.backIt());
//     n_132.reorderIncidence(std::move(edges_132));
// 
//     vector<edge_list_it_t> edges_133;
//     edges_133.push_back(e_135_133.backIt());
//     edges_133.push_back(e_133_134.backIt());
//     edges_133.push_back(e_133_009.backIt());
//     edges_133.push_back(e_099_133.backIt());
//     n_133.reorderIncidence(std::move(edges_133));
// 
//     vector<edge_list_it_t> edges_134;
//     edges_134.push_back(e_134_132.backIt());
//     edges_134.push_back(e_134_131.backIt());
//     edges_134.push_back(e_009_134.backIt());
//     edges_134.push_back(e_133_134.backIt());
//     n_134.reorderIncidence(std::move(edges_134));
// 
//     vector<edge_list_it_t> edges_135;
//     edges_135.push_back(e_095_135.backIt());
//     edges_135.push_back(e_135_132.backIt());
//     edges_135.push_back(e_135_133.backIt());
//     edges_135.push_back(e_097_135.backIt());
//     n_135.reorderIncidence(std::move(edges_135));
// 
//     vector<edge_list_it_t> edges_136;
//     edges_136.push_back(e_014_136.backIt());
//     edges_136.push_back(e_136_006.backIt());
//     n_136.reorderIncidence(std::move(edges_136));
// 
//     vector<edge_list_it_t> edges_137;
//     edges_137.push_back(e_137_004.backIt());
//     edges_137.push_back(e_137_002.backIt());
//     n_137.reorderIncidence(std::move(edges_137));
// 
//     vector<edge_list_it_t> edges_138;
//     edges_138.push_back(e_138_001.backIt());
//     edges_138.push_back(e_138_090.backIt());
//     n_138.reorderIncidence(std::move(edges_138));
// 
//     vector<edge_list_it_t> edges_139;
//     edges_139.push_back(e_001_139.backIt());
//     edges_139.push_back(e_139_090.backIt());
//     edges_139.push_back(e_139_140.backIt());
//     edges_139.push_back(e_139_113.backIt());
//     edges_139.push_back(e_139_003.backIt());
//     n_139.reorderIncidence(std::move(edges_139));
// 
//     vector<edge_list_it_t> edges_140;
//     edges_140.push_back(e_139_140.backIt());
//     edges_140.push_back(e_090_140.backIt());
//     edges_140.push_back(e_113_140.backIt());
//     n_140.reorderIncidence(std::move(edges_140));
// 
//     vector<edge_list_it_t> edges_141;
//     edges_141.push_back(e_003_141.backIt());
//     edges_141.push_back(e_141_113.backIt());
//     n_141.reorderIncidence(std::move(edges_141));
// 
// 
//     vector<node_list_it_t> stOrder;
//     stOrder.push_back(n_004.backIt());
//     stOrder.push_back(n_137.backIt());
//     stOrder.push_back(n_095.backIt());
//     stOrder.push_back(n_135.backIt());
//     stOrder.push_back(n_132.backIt());
//     stOrder.push_back(n_134.backIt());
//     stOrder.push_back(n_131.backIt());
//     stOrder.push_back(n_133.backIt());
//     stOrder.push_back(n_094.backIt());
//     stOrder.push_back(n_096.backIt());
//     stOrder.push_back(n_091.backIt());
//     stOrder.push_back(n_092.backIt());
//     stOrder.push_back(n_093.backIt());
//     stOrder.push_back(n_063.backIt());
//     stOrder.push_back(n_064.backIt());
//     stOrder.push_back(n_065.backIt());
//     stOrder.push_back(n_067.backIt());
//     stOrder.push_back(n_069.backIt());
//     stOrder.push_back(n_123.backIt());
//     stOrder.push_back(n_070.backIt());
//     stOrder.push_back(n_122.backIt());
//     stOrder.push_back(n_075.backIt());
//     stOrder.push_back(n_076.backIt());
//     stOrder.push_back(n_078.backIt());
//     stOrder.push_back(n_080.backIt());
//     stOrder.push_back(n_079.backIt());
//     stOrder.push_back(n_077.backIt());
//     stOrder.push_back(n_081.backIt());
//     stOrder.push_back(n_108.backIt());
//     stOrder.push_back(n_087.backIt());
//     stOrder.push_back(n_082.backIt());
//     stOrder.push_back(n_119.backIt());
//     stOrder.push_back(n_114.backIt());
//     stOrder.push_back(n_112.backIt());
//     stOrder.push_back(n_083.backIt());
//     stOrder.push_back(n_084.backIt());
//     stOrder.push_back(n_111.backIt());
//     stOrder.push_back(n_085.backIt());
//     stOrder.push_back(n_109.backIt());
//     stOrder.push_back(n_110.backIt());
//     stOrder.push_back(n_086.backIt());
//     stOrder.push_back(n_107.backIt());
//     stOrder.push_back(n_106.backIt());
//     stOrder.push_back(n_104.backIt());
//     stOrder.push_back(n_088.backIt());
//     stOrder.push_back(n_105.backIt());
//     stOrder.push_back(n_103.backIt());
//     stOrder.push_back(n_102.backIt());
//     stOrder.push_back(n_101.backIt());
//     stOrder.push_back(n_089.backIt());
//     stOrder.push_back(n_121.backIt());
//     stOrder.push_back(n_120.backIt());
//     stOrder.push_back(n_118.backIt());
//     stOrder.push_back(n_117.backIt());
//     stOrder.push_back(n_116.backIt());
//     stOrder.push_back(n_115.backIt());
//     stOrder.push_back(n_053.backIt());
//     stOrder.push_back(n_072.backIt());
//     stOrder.push_back(n_074.backIt());
//     stOrder.push_back(n_071.backIt());
//     stOrder.push_back(n_073.backIt());
//     stOrder.push_back(n_056.backIt()); // 056
//     stOrder.push_back(n_052.backIt()); // <= 052
//     stOrder.push_back(n_125.backIt()); // 125
//     stOrder.push_back(n_128.backIt()); // 128
//     stOrder.push_back(n_127.backIt()); // 127
//     stOrder.push_back(n_129.backIt()); // 129
//     stOrder.push_back(n_126.backIt()); // 126
//     stOrder.push_back(n_036.backIt());
//     stOrder.push_back(n_037.backIt()); // 037
//     stOrder.push_back(n_038.backIt());
//     stOrder.push_back(n_039.backIt());
//     stOrder.push_back(n_024.backIt());
//     stOrder.push_back(n_015.backIt());
//     stOrder.push_back(n_018.backIt());
//     stOrder.push_back(n_021.backIt());
//     stOrder.push_back(n_022.backIt());
//     stOrder.push_back(n_017.backIt());
//     stOrder.push_back(n_020.backIt());
//     stOrder.push_back(n_019.backIt());
//     stOrder.push_back(n_016.backIt());
//     stOrder.push_back(n_049.backIt());
//     stOrder.push_back(n_048.backIt());
//     stOrder.push_back(n_040.backIt());
//     stOrder.push_back(n_047.backIt());
//     stOrder.push_back(n_043.backIt());
//     stOrder.push_back(n_046.backIt());
//     stOrder.push_back(n_027.backIt());
//     stOrder.push_back(n_013.backIt());
//     stOrder.push_back(n_130.backIt());
//     stOrder.push_back(n_023.backIt());
//     stOrder.push_back(n_025.backIt());
//     stOrder.push_back(n_026.backIt());
//     stOrder.push_back(n_028.backIt());
//     stOrder.push_back(n_029.backIt());
//     stOrder.push_back(n_035.backIt());
//     stOrder.push_back(n_032.backIt());
//     stOrder.push_back(n_030.backIt());
//     stOrder.push_back(n_033.backIt());
//     stOrder.push_back(n_034.backIt());
//     stOrder.push_back(n_041.backIt());
//     stOrder.push_back(n_042.backIt());
//     stOrder.push_back(n_044.backIt());
//     stOrder.push_back(n_011.backIt());
//     stOrder.push_back(n_012.backIt());
//     stOrder.push_back(n_050.backIt());
//     stOrder.push_back(n_051.backIt()); // 051
//     stOrder.push_back(n_054.backIt()); // 054
//     stOrder.push_back(n_055.backIt());
//     stOrder.push_back(n_124.backIt());
//     stOrder.push_back(n_058.backIt());
//     stOrder.push_back(n_057.backIt());
//     stOrder.push_back(n_068.backIt());
//     stOrder.push_back(n_066.backIt());
//     stOrder.push_back(n_059.backIt());
//     stOrder.push_back(n_060.backIt());
//     stOrder.push_back(n_061.backIt());
//     stOrder.push_back(n_062.backIt());
//     stOrder.push_back(n_100.backIt());
//     stOrder.push_back(n_098.backIt());
//     stOrder.push_back(n_097.backIt());
//     stOrder.push_back(n_099.backIt());
//     stOrder.push_back(n_010.backIt());
//     stOrder.push_back(n_045.backIt());
//     stOrder.push_back(n_009.backIt());
//     stOrder.push_back(n_008.backIt());
//     stOrder.push_back(n_007.backIt());
//     stOrder.push_back(n_005.backIt());
//     stOrder.push_back(n_014.backIt());
//     stOrder.push_back(n_136.backIt());
//     stOrder.push_back(n_006.backIt());
//     stOrder.push_back(n_031.backIt());
//     stOrder.push_back(n_002.backIt());
//     stOrder.push_back(n_001.backIt());
//     stOrder.push_back(n_138.backIt());
//     stOrder.push_back(n_139.backIt());
//     stOrder.push_back(n_140.backIt());
//     stOrder.push_back(n_003.backIt());
//     stOrder.push_back(n_141.backIt());
//     stOrder.push_back(n_113.backIt());
//     stOrder.push_back(n_090.backIt());
//     stOrder.push_back(n_000.backIt());
// 
//     BLPlanarityTester tester01;
//     auto res01 =  tester01.findEmbedding(g_1, stOrder);
//     if (res01) {
//         cerr << "The graph is planar. #1\n";
//     }
//     else{
//         cerr << "The graph is planar. #1\n";
//     }
// 
//     BLPlanarityTester tester02;
//     auto res02 =  tester02.isPlanar(g_1, stOrder);
//     if (res02) {
//         cerr << "The graph is planar. #2\n";
//     }
//     else{
//         cerr << "The graph is planar. #2\n";
//     }
// 
//     PlanarDualGraphMaker maker;
//     EmbeddedGraph emb;
//     DualGraph     dual;
//     maker.makeDualGraph(g_1, emb, dual);
// 
//     auto nPair = g_1.nodes();
//     for (auto nit = nPair.first; nit != nPair.second; nit++) {
//         auto& N = dynamic_cast<NumNode&>(*(*nit));
//         cerr << N.num() << " :";
//         for (auto iit  = N.incidentEdges().first;
//                   iit != N.incidentEdges().second; iit++) {
//             auto& E = *(*(*iit));
//             auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
//             cerr << " " << A.num();
// 
//         }
//         cerr << "\n";
//     }
// 
// }
// 
// 
// 
// /**  @brief tests rearrangeIncidentEdges()
//  */
// TEST_F(BLPlanarityTesterTestsForEmbedding, TestRepro_004) {
// 
// 
//     Graph g_1;
// 
//     auto& n_000 = g_1.addNode(make_unique<NumNode>(  0 ));
//     auto& n_001 = g_1.addNode(make_unique<NumNode>(  1 ));
//     auto& n_002 = g_1.addNode(make_unique<NumNode>(  2 ));
//     auto& n_003 = g_1.addNode(make_unique<NumNode>(  3 ));
//     auto& n_004 = g_1.addNode(make_unique<NumNode>(  4 ));
//     auto& n_005 = g_1.addNode(make_unique<NumNode>(  5 ));
//     auto& n_006 = g_1.addNode(make_unique<NumNode>(  6 ));
//     auto& n_007 = g_1.addNode(make_unique<NumNode>(  7 ));
//     auto& n_008 = g_1.addNode(make_unique<NumNode>(  8 ));
//     auto& n_009 = g_1.addNode(make_unique<NumNode>(  9 ));
//     auto& n_010 = g_1.addNode(make_unique<NumNode>( 10 ));
//     auto& n_011 = g_1.addNode(make_unique<NumNode>( 11 ));
//     auto& n_012 = g_1.addNode(make_unique<NumNode>( 12 ));
//     auto& n_013 = g_1.addNode(make_unique<NumNode>( 13 ));
//     auto& n_014 = g_1.addNode(make_unique<NumNode>( 14 ));
//     auto& n_015 = g_1.addNode(make_unique<NumNode>( 15 ));
//     auto& n_016 = g_1.addNode(make_unique<NumNode>( 16 ));
//     auto& n_017 = g_1.addNode(make_unique<NumNode>( 17 ));
//     auto& n_018 = g_1.addNode(make_unique<NumNode>( 18 ));
//     auto& n_019 = g_1.addNode(make_unique<NumNode>( 19 ));
//     auto& n_020 = g_1.addNode(make_unique<NumNode>( 20 ));
//     auto& n_021 = g_1.addNode(make_unique<NumNode>( 21 ));
//     auto& n_022 = g_1.addNode(make_unique<NumNode>( 22 ));
//     auto& n_023 = g_1.addNode(make_unique<NumNode>( 23 ));
//     auto& n_024 = g_1.addNode(make_unique<NumNode>( 24 ));
//     auto& n_025 = g_1.addNode(make_unique<NumNode>( 25 ));
//     auto& n_026 = g_1.addNode(make_unique<NumNode>( 26 ));
//     auto& n_027 = g_1.addNode(make_unique<NumNode>( 27 ));
//     auto& n_028 = g_1.addNode(make_unique<NumNode>( 28 ));
//     auto& n_029 = g_1.addNode(make_unique<NumNode>( 29 ));
//     auto& n_030 = g_1.addNode(make_unique<NumNode>( 30 ));
//     auto& n_031 = g_1.addNode(make_unique<NumNode>( 31 ));
//     auto& n_032 = g_1.addNode(make_unique<NumNode>( 32 ));
//     auto& n_033 = g_1.addNode(make_unique<NumNode>( 33 ));
//     auto& n_034 = g_1.addNode(make_unique<NumNode>( 34 ));
//     auto& n_035 = g_1.addNode(make_unique<NumNode>( 35 ));
//     auto& n_036 = g_1.addNode(make_unique<NumNode>( 36 ));
//     auto& n_037 = g_1.addNode(make_unique<NumNode>( 37 ));
//     auto& n_038 = g_1.addNode(make_unique<NumNode>( 38 ));
//     auto& n_039 = g_1.addNode(make_unique<NumNode>( 39 ));
//     auto& n_040 = g_1.addNode(make_unique<NumNode>( 40 ));
//     auto& n_041 = g_1.addNode(make_unique<NumNode>( 41 ));
//     auto& n_042 = g_1.addNode(make_unique<NumNode>( 42 ));
//     auto& n_043 = g_1.addNode(make_unique<NumNode>( 43 ));
//     auto& n_044 = g_1.addNode(make_unique<NumNode>( 44 ));
//     auto& n_045 = g_1.addNode(make_unique<NumNode>( 45 ));
//     auto& n_046 = g_1.addNode(make_unique<NumNode>( 46 ));
//     auto& n_047 = g_1.addNode(make_unique<NumNode>( 47 ));
//     auto& n_048 = g_1.addNode(make_unique<NumNode>( 48 ));
//     auto& n_049 = g_1.addNode(make_unique<NumNode>( 49 ));
//     auto& n_050 = g_1.addNode(make_unique<NumNode>( 50 ));
//     auto& n_051 = g_1.addNode(make_unique<NumNode>( 51 ));
//     auto& n_052 = g_1.addNode(make_unique<NumNode>( 52 ));
//     auto& n_053 = g_1.addNode(make_unique<NumNode>( 53 ));
//     auto& n_054 = g_1.addNode(make_unique<NumNode>( 54 ));
//     auto& n_055 = g_1.addNode(make_unique<NumNode>( 55 ));
//     auto& n_056 = g_1.addNode(make_unique<NumNode>( 56 ));
//     auto& n_057 = g_1.addNode(make_unique<NumNode>( 57 ));
//     auto& n_058 = g_1.addNode(make_unique<NumNode>( 58 ));
//     auto& n_059 = g_1.addNode(make_unique<NumNode>( 59 ));
//     auto& n_060 = g_1.addNode(make_unique<NumNode>( 60 ));
//     auto& n_061 = g_1.addNode(make_unique<NumNode>( 61 ));
//     auto& n_062 = g_1.addNode(make_unique<NumNode>( 62 ));
//     auto& n_063 = g_1.addNode(make_unique<NumNode>( 63 ));
//     auto& n_064 = g_1.addNode(make_unique<NumNode>( 64 ));
//     auto& n_065 = g_1.addNode(make_unique<NumNode>( 65 ));
//     auto& n_066 = g_1.addNode(make_unique<NumNode>( 66 ));
//     auto& n_067 = g_1.addNode(make_unique<NumNode>( 67 ));
//     auto& n_068 = g_1.addNode(make_unique<NumNode>( 68 ));
//     auto& n_069 = g_1.addNode(make_unique<NumNode>( 69 ));
//     auto& n_070 = g_1.addNode(make_unique<NumNode>( 70 ));
//     auto& n_071 = g_1.addNode(make_unique<NumNode>( 71 ));
//     auto& n_072 = g_1.addNode(make_unique<NumNode>( 72 ));
//     auto& n_073 = g_1.addNode(make_unique<NumNode>( 73 ));
//     auto& n_074 = g_1.addNode(make_unique<NumNode>( 74 ));
//     auto& n_075 = g_1.addNode(make_unique<NumNode>( 75 ));
//     auto& n_076 = g_1.addNode(make_unique<NumNode>( 76 ));
//     auto& n_077 = g_1.addNode(make_unique<NumNode>( 77 ));
//     auto& n_078 = g_1.addNode(make_unique<NumNode>( 78 ));
//     auto& n_079 = g_1.addNode(make_unique<NumNode>( 79 ));
//     auto& n_080 = g_1.addNode(make_unique<NumNode>( 80 ));
//     auto& n_081 = g_1.addNode(make_unique<NumNode>( 81 ));
//     auto& n_082 = g_1.addNode(make_unique<NumNode>( 82 ));
//     auto& n_083 = g_1.addNode(make_unique<NumNode>( 83 ));
//     auto& n_084 = g_1.addNode(make_unique<NumNode>( 84 ));
//     auto& n_085 = g_1.addNode(make_unique<NumNode>( 85 ));
//     auto& n_086 = g_1.addNode(make_unique<NumNode>( 86 ));
//     auto& n_087 = g_1.addNode(make_unique<NumNode>( 87 ));
//     auto& n_088 = g_1.addNode(make_unique<NumNode>( 88 ));
//     auto& n_089 = g_1.addNode(make_unique<NumNode>( 89 ));
//     auto& n_090 = g_1.addNode(make_unique<NumNode>( 90 ));
//     auto& n_091 = g_1.addNode(make_unique<NumNode>( 91 ));
//     auto& n_092 = g_1.addNode(make_unique<NumNode>( 92 ));
//     auto& n_093 = g_1.addNode(make_unique<NumNode>( 93 ));
//     auto& n_094 = g_1.addNode(make_unique<NumNode>( 94 ));
//     auto& n_095 = g_1.addNode(make_unique<NumNode>( 95 ));
//     auto& n_096 = g_1.addNode(make_unique<NumNode>( 96 ));
//     auto& n_097 = g_1.addNode(make_unique<NumNode>( 97 ));
//     auto& n_098 = g_1.addNode(make_unique<NumNode>( 98 ));
//     auto& n_099 = g_1.addNode(make_unique<NumNode>( 99 ));
//     auto& n_100 = g_1.addNode(make_unique<NumNode>(100 ));
// 
//     auto& e_014_074 = g_1.addEdge(make_unique<Edge>(), n_014, n_074 );
//     auto& e_071_032 = g_1.addEdge(make_unique<Edge>(), n_071, n_032 );
//     auto& e_055_054 = g_1.addEdge(make_unique<Edge>(), n_055, n_054 );
//     auto& e_050_049 = g_1.addEdge(make_unique<Edge>(), n_050, n_049 );
//     auto& e_040_041 = g_1.addEdge(make_unique<Edge>(), n_040, n_041 );
//     auto& e_018_019 = g_1.addEdge(make_unique<Edge>(), n_018, n_019 );
//     auto& e_032_031 = g_1.addEdge(make_unique<Edge>(), n_032, n_031 );
//     auto& e_003_007 = g_1.addEdge(make_unique<Edge>(), n_003, n_007 );
//     auto& e_100_001 = g_1.addEdge(make_unique<Edge>(), n_100, n_001 );
//     auto& e_049_048 = g_1.addEdge(make_unique<Edge>(), n_049, n_048 );
//     auto& e_038_037 = g_1.addEdge(make_unique<Edge>(), n_038, n_037 );
//     auto& e_096_097 = g_1.addEdge(make_unique<Edge>(), n_096, n_097 );
//     auto& e_051_050 = g_1.addEdge(make_unique<Edge>(), n_051, n_050 );
//     auto& e_042_062 = g_1.addEdge(make_unique<Edge>(), n_042, n_062 );
//     auto& e_081_009 = g_1.addEdge(make_unique<Edge>(), n_081, n_009 );
//     auto& e_003_086 = g_1.addEdge(make_unique<Edge>(), n_003, n_086 );
//     auto& e_068_050 = g_1.addEdge(make_unique<Edge>(), n_068, n_050 );
//     auto& e_000_083 = g_1.addEdge(make_unique<Edge>(), n_000, n_083 );
//     auto& e_039_045 = g_1.addEdge(make_unique<Edge>(), n_039, n_045 );
//     auto& e_055_008 = g_1.addEdge(make_unique<Edge>(), n_055, n_008 );
//     auto& e_012_006 = g_1.addEdge(make_unique<Edge>(), n_012, n_006 );
//     auto& e_025_026 = g_1.addEdge(make_unique<Edge>(), n_025, n_026 );
//     auto& e_065_008 = g_1.addEdge(make_unique<Edge>(), n_065, n_008 );
//     auto& e_050_070 = g_1.addEdge(make_unique<Edge>(), n_050, n_070 );
//     auto& e_028_046 = g_1.addEdge(make_unique<Edge>(), n_028, n_046 );
//     auto& e_082_083 = g_1.addEdge(make_unique<Edge>(), n_082, n_083 );
//     auto& e_000_004 = g_1.addEdge(make_unique<Edge>(), n_000, n_004 );
//     auto& e_013_020 = g_1.addEdge(make_unique<Edge>(), n_013, n_020 );
//     auto& e_042_041 = g_1.addEdge(make_unique<Edge>(), n_042, n_041 );
//     auto& e_064_042 = g_1.addEdge(make_unique<Edge>(), n_064, n_042 );
//     auto& e_017_018 = g_1.addEdge(make_unique<Edge>(), n_017, n_018 );
//     auto& e_003_002 = g_1.addEdge(make_unique<Edge>(), n_003, n_002 );
//     auto& e_042_038 = g_1.addEdge(make_unique<Edge>(), n_042, n_038 );
//     auto& e_088_087 = g_1.addEdge(make_unique<Edge>(), n_088, n_087 );
//     auto& e_083_004 = g_1.addEdge(make_unique<Edge>(), n_083, n_004 );
//     auto& e_048_034 = g_1.addEdge(make_unique<Edge>(), n_048, n_034 );
//     auto& e_030_029 = g_1.addEdge(make_unique<Edge>(), n_030, n_029 );
//     auto& e_003_085 = g_1.addEdge(make_unique<Edge>(), n_003, n_085 );
//     auto& e_020_019 = g_1.addEdge(make_unique<Edge>(), n_020, n_019 );
//     auto& e_019_013 = g_1.addEdge(make_unique<Edge>(), n_019, n_013 );
//     auto& e_015_014 = g_1.addEdge(make_unique<Edge>(), n_015, n_014 );
//     auto& e_040_016 = g_1.addEdge(make_unique<Edge>(), n_040, n_016 );
//     auto& e_000_001 = g_1.addEdge(make_unique<Edge>(), n_000, n_001 );
//     auto& e_068_070 = g_1.addEdge(make_unique<Edge>(), n_068, n_070 );
//     auto& e_069_068 = g_1.addEdge(make_unique<Edge>(), n_069, n_068 );
//     auto& e_071_019 = g_1.addEdge(make_unique<Edge>(), n_071, n_019 );
//     auto& e_083_084 = g_1.addEdge(make_unique<Edge>(), n_083, n_084 );
//     auto& e_042_052 = g_1.addEdge(make_unique<Edge>(), n_042, n_052 );
//     auto& e_082_009 = g_1.addEdge(make_unique<Edge>(), n_082, n_009 );
//     auto& e_034_035 = g_1.addEdge(make_unique<Edge>(), n_034, n_035 );
//     auto& e_085_083 = g_1.addEdge(make_unique<Edge>(), n_085, n_083 );
//     auto& e_000_093 = g_1.addEdge(make_unique<Edge>(), n_000, n_093 );
//     auto& e_021_024 = g_1.addEdge(make_unique<Edge>(), n_021, n_024 );
//     auto& e_062_006 = g_1.addEdge(make_unique<Edge>(), n_062, n_006 );
//     auto& e_020_077 = g_1.addEdge(make_unique<Edge>(), n_020, n_077 );
//     auto& e_042_063 = g_1.addEdge(make_unique<Edge>(), n_042, n_063 );
//     auto& e_091_090 = g_1.addEdge(make_unique<Edge>(), n_091, n_090 );
//     auto& e_015_013 = g_1.addEdge(make_unique<Edge>(), n_015, n_013 );
//     auto& e_073_014 = g_1.addEdge(make_unique<Edge>(), n_073, n_014 );
//     auto& e_018_015 = g_1.addEdge(make_unique<Edge>(), n_018, n_015 );
//     auto& e_032_019 = g_1.addEdge(make_unique<Edge>(), n_032, n_019 );
//     auto& e_036_046 = g_1.addEdge(make_unique<Edge>(), n_036, n_046 );
//     auto& e_044_019 = g_1.addEdge(make_unique<Edge>(), n_044, n_019 );
//     auto& e_008_009 = g_1.addEdge(make_unique<Edge>(), n_008, n_009 );
//     auto& e_060_061 = g_1.addEdge(make_unique<Edge>(), n_060, n_061 );
//     auto& e_025_047 = g_1.addEdge(make_unique<Edge>(), n_025, n_047 );
//     auto& e_032_009 = g_1.addEdge(make_unique<Edge>(), n_032, n_009 );
//     auto& e_053_052 = g_1.addEdge(make_unique<Edge>(), n_053, n_052 );
//     auto& e_001_099 = g_1.addEdge(make_unique<Edge>(), n_001, n_099 );
//     auto& e_058_059 = g_1.addEdge(make_unique<Edge>(), n_058, n_059 );
//     auto& e_046_030 = g_1.addEdge(make_unique<Edge>(), n_046, n_030 );
//     auto& e_044_071 = g_1.addEdge(make_unique<Edge>(), n_044, n_071 );
//     auto& e_091_089 = g_1.addEdge(make_unique<Edge>(), n_091, n_089 );
//     auto& e_063_079 = g_1.addEdge(make_unique<Edge>(), n_063, n_079 );
//     auto& e_037_019 = g_1.addEdge(make_unique<Edge>(), n_037, n_019 );
//     auto& e_035_032 = g_1.addEdge(make_unique<Edge>(), n_035, n_032 );
//     auto& e_023_021 = g_1.addEdge(make_unique<Edge>(), n_023, n_021 );
//     auto& e_040_079 = g_1.addEdge(make_unique<Edge>(), n_040, n_079 );
//     auto& e_028_027 = g_1.addEdge(make_unique<Edge>(), n_028, n_027 );
//     auto& e_008_006 = g_1.addEdge(make_unique<Edge>(), n_008, n_006 );
//     auto& e_051_053 = g_1.addEdge(make_unique<Edge>(), n_051, n_053 );
//     auto& e_083_008 = g_1.addEdge(make_unique<Edge>(), n_083, n_008 );
//     auto& e_040_014 = g_1.addEdge(make_unique<Edge>(), n_040, n_014 );
//     auto& e_077_076 = g_1.addEdge(make_unique<Edge>(), n_077, n_076 );
//     auto& e_056_055 = g_1.addEdge(make_unique<Edge>(), n_056, n_055 );
//     auto& e_000_084 = g_1.addEdge(make_unique<Edge>(), n_000, n_084 );
//     auto& e_011_009 = g_1.addEdge(make_unique<Edge>(), n_011, n_009 );
//     auto& e_033_049 = g_1.addEdge(make_unique<Edge>(), n_033, n_049 );
//     auto& e_006_099 = g_1.addEdge(make_unique<Edge>(), n_006, n_099 );
//     auto& e_028_047 = g_1.addEdge(make_unique<Edge>(), n_028, n_047 );
//     auto& e_041_017 = g_1.addEdge(make_unique<Edge>(), n_041, n_017 );
//     auto& e_060_058 = g_1.addEdge(make_unique<Edge>(), n_060, n_058 );
//     auto& e_023_024 = g_1.addEdge(make_unique<Edge>(), n_023, n_024 );
//     auto& e_003_005 = g_1.addEdge(make_unique<Edge>(), n_003, n_005 );
//     auto& e_063_012 = g_1.addEdge(make_unique<Edge>(), n_063, n_012 );
//     auto& e_038_039 = g_1.addEdge(make_unique<Edge>(), n_038, n_039 );
//     auto& e_088_007 = g_1.addEdge(make_unique<Edge>(), n_088, n_007 );
//     auto& e_085_086 = g_1.addEdge(make_unique<Edge>(), n_085, n_086 );
//     auto& e_028_025 = g_1.addEdge(make_unique<Edge>(), n_028, n_025 );
//     auto& e_006_001 = g_1.addEdge(make_unique<Edge>(), n_006, n_001 );
//     auto& e_005_007 = g_1.addEdge(make_unique<Edge>(), n_005, n_007 );
//     auto& e_053_066 = g_1.addEdge(make_unique<Edge>(), n_053, n_066 );
//     auto& e_085_008 = g_1.addEdge(make_unique<Edge>(), n_085, n_008 );
//     auto& e_090_092 = g_1.addEdge(make_unique<Edge>(), n_090, n_092 );
//     auto& e_027_043 = g_1.addEdge(make_unique<Edge>(), n_027, n_043 );
//     auto& e_040_017 = g_1.addEdge(make_unique<Edge>(), n_040, n_017 );
//     auto& e_064_062 = g_1.addEdge(make_unique<Edge>(), n_064, n_062 );
//     auto& e_008_058 = g_1.addEdge(make_unique<Edge>(), n_008, n_058 );
//     auto& e_022_030 = g_1.addEdge(make_unique<Edge>(), n_022, n_030 );
//     auto& e_093_095 = g_1.addEdge(make_unique<Edge>(), n_093, n_095 );
//     auto& e_066_060 = g_1.addEdge(make_unique<Edge>(), n_066, n_060 );
//     auto& e_031_050 = g_1.addEdge(make_unique<Edge>(), n_031, n_050 );
//     auto& e_075_009 = g_1.addEdge(make_unique<Edge>(), n_075, n_009 );
//     auto& e_040_074 = g_1.addEdge(make_unique<Edge>(), n_040, n_074 );
//     auto& e_009_074 = g_1.addEdge(make_unique<Edge>(), n_009, n_074 );
//     auto& e_002_096 = g_1.addEdge(make_unique<Edge>(), n_002, n_096 );
//     auto& e_049_051 = g_1.addEdge(make_unique<Edge>(), n_049, n_051 );
//     auto& e_046_047 = g_1.addEdge(make_unique<Edge>(), n_046, n_047 );
//     auto& e_003_083 = g_1.addEdge(make_unique<Edge>(), n_003, n_083 );
//     auto& e_039_043 = g_1.addEdge(make_unique<Edge>(), n_039, n_043 );
//     auto& e_016_014 = g_1.addEdge(make_unique<Edge>(), n_016, n_014 );
//     auto& e_050_053 = g_1.addEdge(make_unique<Edge>(), n_050, n_053 );
//     auto& e_021_030 = g_1.addEdge(make_unique<Edge>(), n_021, n_030 );
//     auto& e_036_030 = g_1.addEdge(make_unique<Edge>(), n_036, n_030 );
//     auto& e_025_023 = g_1.addEdge(make_unique<Edge>(), n_025, n_023 );
//     auto& e_032_033 = g_1.addEdge(make_unique<Edge>(), n_032, n_033 );
//     auto& e_057_054 = g_1.addEdge(make_unique<Edge>(), n_057, n_054 );
//     auto& e_056_054 = g_1.addEdge(make_unique<Edge>(), n_056, n_054 );
//     auto& e_010_100 = g_1.addEdge(make_unique<Edge>(), n_010, n_100 );
//     auto& e_007_090 = g_1.addEdge(make_unique<Edge>(), n_007, n_090 );
//     auto& e_041_038 = g_1.addEdge(make_unique<Edge>(), n_041, n_038 );
//     auto& e_048_036 = g_1.addEdge(make_unique<Edge>(), n_048, n_036 );
//     auto& e_098_010 = g_1.addEdge(make_unique<Edge>(), n_098, n_010 );
//     auto& e_053_009 = g_1.addEdge(make_unique<Edge>(), n_053, n_009 );
//     auto& e_081_012 = g_1.addEdge(make_unique<Edge>(), n_081, n_012 );
//     auto& e_079_009 = g_1.addEdge(make_unique<Edge>(), n_079, n_009 );
//     auto& e_032_072 = g_1.addEdge(make_unique<Edge>(), n_032, n_072 );
//     auto& e_054_053 = g_1.addEdge(make_unique<Edge>(), n_054, n_053 );
//     auto& e_020_080 = g_1.addEdge(make_unique<Edge>(), n_020, n_080 );
//     auto& e_028_026 = g_1.addEdge(make_unique<Edge>(), n_028, n_026 );
//     auto& e_061_058 = g_1.addEdge(make_unique<Edge>(), n_061, n_058 );
//     auto& e_030_034 = g_1.addEdge(make_unique<Edge>(), n_030, n_034 );
//     auto& e_037_018 = g_1.addEdge(make_unique<Edge>(), n_037, n_018 );
//     auto& e_034_033 = g_1.addEdge(make_unique<Edge>(), n_034, n_033 );
//     auto& e_007_089 = g_1.addEdge(make_unique<Edge>(), n_007, n_089 );
//     auto& e_072_019 = g_1.addEdge(make_unique<Edge>(), n_072, n_019 );
//     auto& e_050_009 = g_1.addEdge(make_unique<Edge>(), n_050, n_009 );
//     auto& e_024_044 = g_1.addEdge(make_unique<Edge>(), n_024, n_044 );
//     auto& e_073_074 = g_1.addEdge(make_unique<Edge>(), n_073, n_074 );
//     auto& e_080_014 = g_1.addEdge(make_unique<Edge>(), n_080, n_014 );
//     auto& e_069_070 = g_1.addEdge(make_unique<Edge>(), n_069, n_070 );
//     auto& e_043_044 = g_1.addEdge(make_unique<Edge>(), n_043, n_044 );
//     auto& e_078_075 = g_1.addEdge(make_unique<Edge>(), n_078, n_075 );
//     auto& e_022_047 = g_1.addEdge(make_unique<Edge>(), n_022, n_047 );
//     auto& e_021_029 = g_1.addEdge(make_unique<Edge>(), n_021, n_029 );
//     auto& e_021_022 = g_1.addEdge(make_unique<Edge>(), n_021, n_022 );
//     auto& e_060_059 = g_1.addEdge(make_unique<Edge>(), n_060, n_059 );
//     auto& e_032_078 = g_1.addEdge(make_unique<Edge>(), n_032, n_078 );
//     auto& e_007_092 = g_1.addEdge(make_unique<Edge>(), n_007, n_092 );
//     auto& e_026_023 = g_1.addEdge(make_unique<Edge>(), n_026, n_023 );
//     auto& e_003_004 = g_1.addEdge(make_unique<Edge>(), n_003, n_004 );
//     auto& e_002_004 = g_1.addEdge(make_unique<Edge>(), n_002, n_004 );
//     auto& e_009_012 = g_1.addEdge(make_unique<Edge>(), n_009, n_012 );
//     auto& e_033_031 = g_1.addEdge(make_unique<Edge>(), n_033, n_031 );
//     auto& e_009_078 = g_1.addEdge(make_unique<Edge>(), n_009, n_078 );
//     auto& e_076_014 = g_1.addEdge(make_unique<Edge>(), n_076, n_014 );
//     auto& e_031_009 = g_1.addEdge(make_unique<Edge>(), n_031, n_009 );
//     auto& e_005_006 = g_1.addEdge(make_unique<Edge>(), n_005, n_006 );
//     auto& e_045_036 = g_1.addEdge(make_unique<Edge>(), n_045, n_036 );
//     auto& e_066_064 = g_1.addEdge(make_unique<Edge>(), n_066, n_064 );
//     auto& e_098_099 = g_1.addEdge(make_unique<Edge>(), n_098, n_099 );
//     auto& e_029_035 = g_1.addEdge(make_unique<Edge>(), n_029, n_035 );
//     auto& e_028_045 = g_1.addEdge(make_unique<Edge>(), n_028, n_045 );
//     auto& e_030_047 = g_1.addEdge(make_unique<Edge>(), n_030, n_047 );
//     auto& e_008_059 = g_1.addEdge(make_unique<Edge>(), n_008, n_059 );
//     auto& e_017_016 = g_1.addEdge(make_unique<Edge>(), n_017, n_016 );
//     auto& e_042_040 = g_1.addEdge(make_unique<Edge>(), n_042, n_040 );
//     auto& e_098_006 = g_1.addEdge(make_unique<Edge>(), n_098, n_006 );
//     auto& e_043_037 = g_1.addEdge(make_unique<Edge>(), n_043, n_037 );
//     auto& e_042_039 = g_1.addEdge(make_unique<Edge>(), n_042, n_039 );
//     auto& e_057_053 = g_1.addEdge(make_unique<Edge>(), n_057, n_053 );
//     auto& e_009_073 = g_1.addEdge(make_unique<Edge>(), n_009, n_073 );
//     auto& e_002_097 = g_1.addEdge(make_unique<Edge>(), n_002, n_097 );
//     auto& e_067_053 = g_1.addEdge(make_unique<Edge>(), n_067, n_053 );
//     auto& e_089_090 = g_1.addEdge(make_unique<Edge>(), n_089, n_090 );
//     auto& e_010_084 = g_1.addEdge(make_unique<Edge>(), n_010, n_084 );
//     auto& e_016_015 = g_1.addEdge(make_unique<Edge>(), n_016, n_015 );
//     auto& e_096_093 = g_1.addEdge(make_unique<Edge>(), n_096, n_093 );
//     auto& e_025_022 = g_1.addEdge(make_unique<Edge>(), n_025, n_022 );
//     auto& e_069_009 = g_1.addEdge(make_unique<Edge>(), n_069, n_009 );
//     auto& e_002_093 = g_1.addEdge(make_unique<Edge>(), n_002, n_093 );
//     auto& e_088_092 = g_1.addEdge(make_unique<Edge>(), n_088, n_092 );
//     auto& e_027_024 = g_1.addEdge(make_unique<Edge>(), n_027, n_024 );
//     auto& e_087_008 = g_1.addEdge(make_unique<Edge>(), n_087, n_008 );
//     auto& e_075_073 = g_1.addEdge(make_unique<Edge>(), n_075, n_073 );
//     auto& e_079_074 = g_1.addEdge(make_unique<Edge>(), n_079, n_074 );
//     auto& e_009_010 = g_1.addEdge(make_unique<Edge>(), n_009, n_010 );
//     auto& e_062_063 = g_1.addEdge(make_unique<Edge>(), n_062, n_063 );
//     auto& e_094_093 = g_1.addEdge(make_unique<Edge>(), n_094, n_093 );
//     auto& e_052_051 = g_1.addEdge(make_unique<Edge>(), n_052, n_051 );
//     auto& e_076_075 = g_1.addEdge(make_unique<Edge>(), n_076, n_075 );
//     auto& e_082_008 = g_1.addEdge(make_unique<Edge>(), n_082, n_008 );
//     auto& e_065_064 = g_1.addEdge(make_unique<Edge>(), n_065, n_064 );
//     auto& e_045_042 = g_1.addEdge(make_unique<Edge>(), n_045, n_042 );
//     auto& e_042_066 = g_1.addEdge(make_unique<Edge>(), n_042, n_066 );
//     auto& e_021_025 = g_1.addEdge(make_unique<Edge>(), n_021, n_025 );
//     auto& e_026_027 = g_1.addEdge(make_unique<Edge>(), n_026, n_027 );
//     auto& e_072_076 = g_1.addEdge(make_unique<Edge>(), n_072, n_076 );
//     auto& e_013_014 = g_1.addEdge(make_unique<Edge>(), n_013, n_014 );
//     auto& e_002_094 = g_1.addEdge(make_unique<Edge>(), n_002, n_094 );
//     auto& e_098_001 = g_1.addEdge(make_unique<Edge>(), n_098, n_001 );
//     auto& e_003_008 = g_1.addEdge(make_unique<Edge>(), n_003, n_008 );
//     auto& e_002_000 = g_1.addEdge(make_unique<Edge>(), n_002, n_000 );
//     auto& e_029_024 = g_1.addEdge(make_unique<Edge>(), n_029, n_024 );
//     auto& e_094_095 = g_1.addEdge(make_unique<Edge>(), n_094, n_095 );
//     auto& e_035_071 = g_1.addEdge(make_unique<Edge>(), n_035, n_071 );
//     auto& e_067_058 = g_1.addEdge(make_unique<Edge>(), n_067, n_058 );
//     auto& e_078_072 = g_1.addEdge(make_unique<Edge>(), n_078, n_072 );
//     auto& e_011_006 = g_1.addEdge(make_unique<Edge>(), n_011, n_006 );
//     auto& e_052_048 = g_1.addEdge(make_unique<Edge>(), n_052, n_048 );
// 
//     vector<edge_list_it_t> edges_000;
//     edges_000.push_back(e_002_000.backIt());
//     edges_000.push_back(e_000_093.backIt());
//     edges_000.push_back(e_000_001.backIt());
//     edges_000.push_back(e_000_084.backIt());
//     edges_000.push_back(e_000_083.backIt());
//     edges_000.push_back(e_000_004.backIt());
//     n_000.reorderIncidence(std::move(edges_000));
// 
//     vector<edge_list_it_t> edges_001;
//     edges_001.push_back(e_000_001.backIt());
//     edges_001.push_back(e_006_001.backIt());
//     edges_001.push_back(e_001_099.backIt());
//     edges_001.push_back(e_098_001.backIt());
//     edges_001.push_back(e_100_001.backIt());
//     n_001.reorderIncidence(std::move(edges_001));
// 
//     vector<edge_list_it_t> edges_002;
//     edges_002.push_back(e_003_002.backIt());
//     edges_002.push_back(e_002_093.backIt());
//     edges_002.push_back(e_002_097.backIt());
//     edges_002.push_back(e_002_096.backIt());
//     edges_002.push_back(e_002_094.backIt());
//     edges_002.push_back(e_002_000.backIt());
//     edges_002.push_back(e_002_004.backIt());
//     n_002.reorderIncidence(std::move(edges_002));
// 
//     vector<edge_list_it_t> edges_003;
//     edges_003.push_back(e_003_083.backIt());
//     edges_003.push_back(e_003_086.backIt());
//     edges_003.push_back(e_003_085.backIt());
//     edges_003.push_back(e_003_008.backIt());
//     edges_003.push_back(e_003_007.backIt());
//     edges_003.push_back(e_003_005.backIt());
//     edges_003.push_back(e_003_002.backIt());
//     edges_003.push_back(e_003_004.backIt());
//     n_003.reorderIncidence(std::move(edges_003));
// 
//     vector<edge_list_it_t> edges_004;
//     edges_004.push_back(e_000_004.backIt());
//     edges_004.push_back(e_083_004.backIt());
//     edges_004.push_back(e_003_004.backIt());
//     edges_004.push_back(e_002_004.backIt());
//     n_004.reorderIncidence(std::move(edges_004));
// 
//     vector<edge_list_it_t> edges_005;
//     edges_005.push_back(e_005_006.backIt());
//     edges_005.push_back(e_003_005.backIt());
//     edges_005.push_back(e_005_007.backIt());
//     n_005.reorderIncidence(std::move(edges_005));
// 
//     vector<edge_list_it_t> edges_006;
//     edges_006.push_back(e_006_001.backIt());
//     edges_006.push_back(e_005_006.backIt());
//     edges_006.push_back(e_008_006.backIt());
//     edges_006.push_back(e_062_006.backIt());
//     edges_006.push_back(e_012_006.backIt());
//     edges_006.push_back(e_011_006.backIt());
//     edges_006.push_back(e_098_006.backIt());
//     edges_006.push_back(e_006_099.backIt());
//     n_006.reorderIncidence(std::move(edges_006));
// 
//     vector<edge_list_it_t> edges_007;
//     edges_007.push_back(e_007_089.backIt());
//     edges_007.push_back(e_007_090.backIt());
//     edges_007.push_back(e_007_092.backIt());
//     edges_007.push_back(e_088_007.backIt());
//     edges_007.push_back(e_005_007.backIt());
//     edges_007.push_back(e_003_007.backIt());
//     n_007.reorderIncidence(std::move(edges_007));
// 
//     vector<edge_list_it_t> edges_008;
//     edges_008.push_back(e_085_008.backIt());
//     edges_008.push_back(e_083_008.backIt());
//     edges_008.push_back(e_082_008.backIt());
//     edges_008.push_back(e_008_009.backIt());
//     edges_008.push_back(e_055_008.backIt());
//     edges_008.push_back(e_008_058.backIt());
//     edges_008.push_back(e_008_059.backIt());
//     edges_008.push_back(e_065_008.backIt());
//     edges_008.push_back(e_008_006.backIt());
//     edges_008.push_back(e_087_008.backIt());
//     edges_008.push_back(e_003_008.backIt());
//     n_008.reorderIncidence(std::move(edges_008));
// 
//     vector<edge_list_it_t> edges_009;
//     edges_009.push_back(e_011_009.backIt());
//     edges_009.push_back(e_009_012.backIt());
//     edges_009.push_back(e_081_009.backIt());
//     edges_009.push_back(e_079_009.backIt());
//     edges_009.push_back(e_009_074.backIt());
//     edges_009.push_back(e_009_073.backIt());
//     edges_009.push_back(e_075_009.backIt());
//     edges_009.push_back(e_009_078.backIt());
//     edges_009.push_back(e_032_009.backIt());
//     edges_009.push_back(e_031_009.backIt());
//     edges_009.push_back(e_069_009.backIt());
//     edges_009.push_back(e_050_009.backIt());
//     edges_009.push_back(e_053_009.backIt());
//     edges_009.push_back(e_008_009.backIt());
//     edges_009.push_back(e_082_009.backIt());
//     edges_009.push_back(e_009_010.backIt());
//     n_009.reorderIncidence(std::move(edges_009));
// 
//     vector<edge_list_it_t> edges_010;
//     edges_010.push_back(e_010_100.backIt());
//     edges_010.push_back(e_098_010.backIt());
//     edges_010.push_back(e_009_010.backIt());
//     edges_010.push_back(e_010_084.backIt());
//     n_010.reorderIncidence(std::move(edges_010));
// 
//     vector<edge_list_it_t> edges_011;
//     edges_011.push_back(e_011_006.backIt());
//     edges_011.push_back(e_011_009.backIt());
//     n_011.reorderIncidence(std::move(edges_011));
// 
//     vector<edge_list_it_t> edges_012;
//     edges_012.push_back(e_012_006.backIt());
//     edges_012.push_back(e_063_012.backIt());
//     edges_012.push_back(e_081_012.backIt());
//     edges_012.push_back(e_009_012.backIt());
//     n_012.reorderIncidence(std::move(edges_012));
// 
//     vector<edge_list_it_t> edges_013;
//     edges_013.push_back(e_015_013.backIt());
//     edges_013.push_back(e_019_013.backIt());
//     edges_013.push_back(e_013_020.backIt());
//     edges_013.push_back(e_013_014.backIt());
//     n_013.reorderIncidence(std::move(edges_013));
// 
//     vector<edge_list_it_t> edges_014;
//     edges_014.push_back(e_040_014.backIt());
//     edges_014.push_back(e_016_014.backIt());
//     edges_014.push_back(e_015_014.backIt());
//     edges_014.push_back(e_013_014.backIt());
//     edges_014.push_back(e_080_014.backIt());
//     edges_014.push_back(e_076_014.backIt());
//     edges_014.push_back(e_073_014.backIt());
//     edges_014.push_back(e_014_074.backIt());
//     n_014.reorderIncidence(std::move(edges_014));
// 
//     vector<edge_list_it_t> edges_015;
//     edges_015.push_back(e_016_015.backIt());
//     edges_015.push_back(e_018_015.backIt());
//     edges_015.push_back(e_015_013.backIt());
//     edges_015.push_back(e_015_014.backIt());
//     n_015.reorderIncidence(std::move(edges_015));
// 
//     vector<edge_list_it_t> edges_016;
//     edges_016.push_back(e_040_016.backIt());
//     edges_016.push_back(e_017_016.backIt());
//     edges_016.push_back(e_016_015.backIt());
//     edges_016.push_back(e_016_014.backIt());
//     n_016.reorderIncidence(std::move(edges_016));
// 
//     vector<edge_list_it_t> edges_017;
//     edges_017.push_back(e_041_017.backIt());
//     edges_017.push_back(e_017_018.backIt());
//     edges_017.push_back(e_017_016.backIt());
//     edges_017.push_back(e_040_017.backIt());
//     n_017.reorderIncidence(std::move(edges_017));
// 
//     vector<edge_list_it_t> edges_018;
//     edges_018.push_back(e_037_018.backIt());
//     edges_018.push_back(e_018_019.backIt());
//     edges_018.push_back(e_018_015.backIt());
//     edges_018.push_back(e_017_018.backIt());
//     n_018.reorderIncidence(std::move(edges_018));
// 
//     vector<edge_list_it_t> edges_019;
//     edges_019.push_back(e_037_019.backIt());
//     edges_019.push_back(e_044_019.backIt());
//     edges_019.push_back(e_071_019.backIt());
//     edges_019.push_back(e_032_019.backIt());
//     edges_019.push_back(e_072_019.backIt());
//     edges_019.push_back(e_020_019.backIt());
//     edges_019.push_back(e_019_013.backIt());
//     edges_019.push_back(e_018_019.backIt());
//     n_019.reorderIncidence(std::move(edges_019));
// 
//     vector<edge_list_it_t> edges_020;
//     edges_020.push_back(e_013_020.backIt());
//     edges_020.push_back(e_020_019.backIt());
//     edges_020.push_back(e_020_077.backIt());
//     edges_020.push_back(e_020_080.backIt());
//     n_020.reorderIncidence(std::move(edges_020));
// 
//     vector<edge_list_it_t> edges_021;
//     edges_021.push_back(e_021_025.backIt());
//     edges_021.push_back(e_021_022.backIt());
//     edges_021.push_back(e_021_030.backIt());
//     edges_021.push_back(e_021_029.backIt());
//     edges_021.push_back(e_021_024.backIt());
//     edges_021.push_back(e_023_021.backIt());
//     n_021.reorderIncidence(std::move(edges_021));
// 
//     vector<edge_list_it_t> edges_022;
//     edges_022.push_back(e_025_022.backIt());
//     edges_022.push_back(e_022_047.backIt());
//     edges_022.push_back(e_022_030.backIt());
//     edges_022.push_back(e_021_022.backIt());
//     n_022.reorderIncidence(std::move(edges_022));
// 
//     vector<edge_list_it_t> edges_023;
//     edges_023.push_back(e_025_023.backIt());
//     edges_023.push_back(e_023_021.backIt());
//     edges_023.push_back(e_023_024.backIt());
//     edges_023.push_back(e_026_023.backIt());
//     n_023.reorderIncidence(std::move(edges_023));
// 
//     vector<edge_list_it_t> edges_024;
//     edges_024.push_back(e_021_024.backIt());
//     edges_024.push_back(e_029_024.backIt());
//     edges_024.push_back(e_024_044.backIt());
//     edges_024.push_back(e_027_024.backIt());
//     edges_024.push_back(e_023_024.backIt());
//     n_024.reorderIncidence(std::move(edges_024));
// 
//     vector<edge_list_it_t> edges_025;
//     edges_025.push_back(e_028_025.backIt());
//     edges_025.push_back(e_025_047.backIt());
//     edges_025.push_back(e_025_022.backIt());
//     edges_025.push_back(e_021_025.backIt());
//     edges_025.push_back(e_025_023.backIt());
//     edges_025.push_back(e_025_026.backIt());
//     n_025.reorderIncidence(std::move(edges_025));
// 
//     vector<edge_list_it_t> edges_026;
//     edges_026.push_back(e_028_026.backIt());
//     edges_026.push_back(e_025_026.backIt());
//     edges_026.push_back(e_026_023.backIt());
//     edges_026.push_back(e_026_027.backIt());
//     n_026.reorderIncidence(std::move(edges_026));
// 
//     vector<edge_list_it_t> edges_027;
//     edges_027.push_back(e_028_027.backIt());
//     edges_027.push_back(e_026_027.backIt());
//     edges_027.push_back(e_027_024.backIt());
//     edges_027.push_back(e_027_043.backIt());
//     n_027.reorderIncidence(std::move(edges_027));
// 
//     vector<edge_list_it_t> edges_028;
//     edges_028.push_back(e_028_045.backIt());
//     edges_028.push_back(e_028_046.backIt());
//     edges_028.push_back(e_028_047.backIt());
//     edges_028.push_back(e_028_025.backIt());
//     edges_028.push_back(e_028_026.backIt());
//     edges_028.push_back(e_028_027.backIt());
//     n_028.reorderIncidence(std::move(edges_028));
// 
//     vector<edge_list_it_t> edges_029;
//     edges_029.push_back(e_029_024.backIt());
//     edges_029.push_back(e_021_029.backIt());
//     edges_029.push_back(e_030_029.backIt());
//     edges_029.push_back(e_029_035.backIt());
//     n_029.reorderIncidence(std::move(edges_029));
// 
//     vector<edge_list_it_t> edges_030;
//     edges_030.push_back(e_022_030.backIt());
//     edges_030.push_back(e_030_047.backIt());
//     edges_030.push_back(e_046_030.backIt());
//     edges_030.push_back(e_036_030.backIt());
//     edges_030.push_back(e_030_034.backIt());
//     edges_030.push_back(e_030_029.backIt());
//     edges_030.push_back(e_021_030.backIt());
//     n_030.reorderIncidence(std::move(edges_030));
// 
//     vector<edge_list_it_t> edges_031;
//     edges_031.push_back(e_031_050.backIt());
//     edges_031.push_back(e_031_009.backIt());
//     edges_031.push_back(e_032_031.backIt());
//     edges_031.push_back(e_033_031.backIt());
//     n_031.reorderIncidence(std::move(edges_031));
// 
//     vector<edge_list_it_t> edges_032;
//     edges_032.push_back(e_032_019.backIt());
//     edges_032.push_back(e_071_032.backIt());
//     edges_032.push_back(e_035_032.backIt());
//     edges_032.push_back(e_032_033.backIt());
//     edges_032.push_back(e_032_031.backIt());
//     edges_032.push_back(e_032_009.backIt());
//     edges_032.push_back(e_032_078.backIt());
//     edges_032.push_back(e_032_072.backIt());
//     n_032.reorderIncidence(std::move(edges_032));
// 
//     vector<edge_list_it_t> edges_033;
//     edges_033.push_back(e_033_049.backIt());
//     edges_033.push_back(e_033_031.backIt());
//     edges_033.push_back(e_032_033.backIt());
//     edges_033.push_back(e_034_033.backIt());
//     n_033.reorderIncidence(std::move(edges_033));
// 
//     vector<edge_list_it_t> edges_034;
//     edges_034.push_back(e_034_033.backIt());
//     edges_034.push_back(e_034_035.backIt());
//     edges_034.push_back(e_030_034.backIt());
//     edges_034.push_back(e_048_034.backIt());
//     n_034.reorderIncidence(std::move(edges_034));
// 
//     vector<edge_list_it_t> edges_035;
//     edges_035.push_back(e_035_032.backIt());
//     edges_035.push_back(e_035_071.backIt());
//     edges_035.push_back(e_029_035.backIt());
//     edges_035.push_back(e_034_035.backIt());
//     n_035.reorderIncidence(std::move(edges_035));
// 
//     vector<edge_list_it_t> edges_036;
//     edges_036.push_back(e_036_030.backIt());
//     edges_036.push_back(e_036_046.backIt());
//     edges_036.push_back(e_045_036.backIt());
//     edges_036.push_back(e_048_036.backIt());
//     n_036.reorderIncidence(std::move(edges_036));
// 
//     vector<edge_list_it_t> edges_037;
//     edges_037.push_back(e_038_037.backIt());
//     edges_037.push_back(e_043_037.backIt());
//     edges_037.push_back(e_037_019.backIt());
//     edges_037.push_back(e_037_018.backIt());
//     n_037.reorderIncidence(std::move(edges_037));
// 
//     vector<edge_list_it_t> edges_038;
//     edges_038.push_back(e_042_038.backIt());
//     edges_038.push_back(e_038_039.backIt());
//     edges_038.push_back(e_038_037.backIt());
//     edges_038.push_back(e_041_038.backIt());
//     n_038.reorderIncidence(std::move(edges_038));
// 
//     vector<edge_list_it_t> edges_039;
//     edges_039.push_back(e_042_039.backIt());
//     edges_039.push_back(e_039_045.backIt());
//     edges_039.push_back(e_039_043.backIt());
//     edges_039.push_back(e_038_039.backIt());
//     n_039.reorderIncidence(std::move(edges_039));
// 
//     vector<edge_list_it_t> edges_040;
//     edges_040.push_back(e_040_079.backIt());
//     edges_040.push_back(e_042_040.backIt());
//     edges_040.push_back(e_040_041.backIt());
//     edges_040.push_back(e_040_017.backIt());
//     edges_040.push_back(e_040_016.backIt());
//     edges_040.push_back(e_040_014.backIt());
//     edges_040.push_back(e_040_074.backIt());
//     n_040.reorderIncidence(std::move(edges_040));
// 
//     vector<edge_list_it_t> edges_041;
//     edges_041.push_back(e_042_041.backIt());
//     edges_041.push_back(e_041_038.backIt());
//     edges_041.push_back(e_041_017.backIt());
//     edges_041.push_back(e_040_041.backIt());
//     n_041.reorderIncidence(std::move(edges_041));
// 
//     vector<edge_list_it_t> edges_042;
//     edges_042.push_back(e_042_063.backIt());
//     edges_042.push_back(e_042_062.backIt());
//     edges_042.push_back(e_064_042.backIt());
//     edges_042.push_back(e_042_066.backIt());
//     edges_042.push_back(e_042_052.backIt());
//     edges_042.push_back(e_045_042.backIt());
//     edges_042.push_back(e_042_039.backIt());
//     edges_042.push_back(e_042_038.backIt());
//     edges_042.push_back(e_042_041.backIt());
//     edges_042.push_back(e_042_040.backIt());
//     n_042.reorderIncidence(std::move(edges_042));
// 
//     vector<edge_list_it_t> edges_043;
//     edges_043.push_back(e_027_043.backIt());
//     edges_043.push_back(e_043_044.backIt());
//     edges_043.push_back(e_043_037.backIt());
//     edges_043.push_back(e_039_043.backIt());
//     n_043.reorderIncidence(std::move(edges_043));
// 
//     vector<edge_list_it_t> edges_044;
//     edges_044.push_back(e_044_019.backIt());
//     edges_044.push_back(e_043_044.backIt());
//     edges_044.push_back(e_024_044.backIt());
//     edges_044.push_back(e_044_071.backIt());
//     n_044.reorderIncidence(std::move(edges_044));
// 
//     vector<edge_list_it_t> edges_045;
//     edges_045.push_back(e_045_042.backIt());
//     edges_045.push_back(e_045_036.backIt());
//     edges_045.push_back(e_028_045.backIt());
//     edges_045.push_back(e_039_045.backIt());
//     n_045.reorderIncidence(std::move(edges_045));
// 
//     vector<edge_list_it_t> edges_046;
//     edges_046.push_back(e_046_030.backIt());
//     edges_046.push_back(e_046_047.backIt());
//     edges_046.push_back(e_028_046.backIt());
//     edges_046.push_back(e_036_046.backIt());
//     n_046.reorderIncidence(std::move(edges_046));
// 
//     vector<edge_list_it_t> edges_047;
//     edges_047.push_back(e_030_047.backIt());
//     edges_047.push_back(e_022_047.backIt());
//     edges_047.push_back(e_025_047.backIt());
//     edges_047.push_back(e_028_047.backIt());
//     edges_047.push_back(e_046_047.backIt());
//     n_047.reorderIncidence(std::move(edges_047));
// 
//     vector<edge_list_it_t> edges_048;
//     edges_048.push_back(e_049_048.backIt());
//     edges_048.push_back(e_048_034.backIt());
//     edges_048.push_back(e_048_036.backIt());
//     edges_048.push_back(e_052_048.backIt());
//     n_048.reorderIncidence(std::move(edges_048));
// 
//     vector<edge_list_it_t> edges_049;
//     edges_049.push_back(e_050_049.backIt());
//     edges_049.push_back(e_033_049.backIt());
//     edges_049.push_back(e_049_048.backIt());
//     edges_049.push_back(e_049_051.backIt());
//     n_049.reorderIncidence(std::move(edges_049));
// 
//     vector<edge_list_it_t> edges_050;
//     edges_050.push_back(e_068_050.backIt());
//     edges_050.push_back(e_050_070.backIt());
//     edges_050.push_back(e_050_009.backIt());
//     edges_050.push_back(e_031_050.backIt());
//     edges_050.push_back(e_050_049.backIt());
//     edges_050.push_back(e_051_050.backIt());
//     edges_050.push_back(e_050_053.backIt());
//     n_050.reorderIncidence(std::move(edges_050));
// 
//     vector<edge_list_it_t> edges_051;
//     edges_051.push_back(e_051_050.backIt());
//     edges_051.push_back(e_049_051.backIt());
//     edges_051.push_back(e_052_051.backIt());
//     edges_051.push_back(e_051_053.backIt());
//     n_051.reorderIncidence(std::move(edges_051));
// 
//     vector<edge_list_it_t> edges_052;
//     edges_052.push_back(e_052_048.backIt());
//     edges_052.push_back(e_042_052.backIt());
//     edges_052.push_back(e_053_052.backIt());
//     edges_052.push_back(e_052_051.backIt());
//     n_052.reorderIncidence(std::move(edges_052));
// 
//     vector<edge_list_it_t> edges_053;
//     edges_053.push_back(e_053_009.backIt());
//     edges_053.push_back(e_050_053.backIt());
//     edges_053.push_back(e_051_053.backIt());
//     edges_053.push_back(e_053_052.backIt());
//     edges_053.push_back(e_053_066.backIt());
//     edges_053.push_back(e_067_053.backIt());
//     edges_053.push_back(e_057_053.backIt());
//     edges_053.push_back(e_054_053.backIt());
//     n_053.reorderIncidence(std::move(edges_053));
// 
//     vector<edge_list_it_t> edges_054;
//     edges_054.push_back(e_054_053.backIt());
//     edges_054.push_back(e_057_054.backIt());
//     edges_054.push_back(e_056_054.backIt());
//     edges_054.push_back(e_055_054.backIt());
//     n_054.reorderIncidence(std::move(edges_054));
// 
//     vector<edge_list_it_t> edges_055;
//     edges_055.push_back(e_055_054.backIt());
//     edges_055.push_back(e_056_055.backIt());
//     edges_055.push_back(e_055_008.backIt());
//     n_055.reorderIncidence(std::move(edges_055));
// 
//     vector<edge_list_it_t> edges_056;
//     edges_056.push_back(e_056_054.backIt());
//     edges_056.push_back(e_056_055.backIt());
//     n_056.reorderIncidence(std::move(edges_056));
// 
//     vector<edge_list_it_t> edges_057;
//     edges_057.push_back(e_057_053.backIt());
//     edges_057.push_back(e_057_054.backIt());
//     n_057.reorderIncidence(std::move(edges_057));
// 
//     vector<edge_list_it_t> edges_058;
//     edges_058.push_back(e_067_058.backIt());
//     edges_058.push_back(e_061_058.backIt());
//     edges_058.push_back(e_060_058.backIt());
//     edges_058.push_back(e_058_059.backIt());
//     edges_058.push_back(e_008_058.backIt());
//     n_058.reorderIncidence(std::move(edges_058));
// 
//     vector<edge_list_it_t> edges_059;
//     edges_059.push_back(e_058_059.backIt());
//     edges_059.push_back(e_060_059.backIt());
//     edges_059.push_back(e_008_059.backIt());
//     n_059.reorderIncidence(std::move(edges_059));
// 
//     vector<edge_list_it_t> edges_060;
//     edges_060.push_back(e_060_061.backIt());
//     edges_060.push_back(e_060_058.backIt());
//     edges_060.push_back(e_066_060.backIt());
//     edges_060.push_back(e_060_059.backIt());
//     n_060.reorderIncidence(std::move(edges_060));
// 
//     vector<edge_list_it_t> edges_061;
//     edges_061.push_back(e_061_058.backIt());
//     edges_061.push_back(e_060_061.backIt());
//     n_061.reorderIncidence(std::move(edges_061));
// 
//     vector<edge_list_it_t> edges_062;
//     edges_062.push_back(e_042_062.backIt());
//     edges_062.push_back(e_062_063.backIt());
//     edges_062.push_back(e_062_006.backIt());
//     edges_062.push_back(e_064_062.backIt());
//     n_062.reorderIncidence(std::move(edges_062));
// 
//     vector<edge_list_it_t> edges_063;
//     edges_063.push_back(e_063_012.backIt());
//     edges_063.push_back(e_062_063.backIt());
//     edges_063.push_back(e_042_063.backIt());
//     edges_063.push_back(e_063_079.backIt());
//     n_063.reorderIncidence(std::move(edges_063));
// 
//     vector<edge_list_it_t> edges_064;
//     edges_064.push_back(e_066_064.backIt());
//     edges_064.push_back(e_064_042.backIt());
//     edges_064.push_back(e_064_062.backIt());
//     edges_064.push_back(e_065_064.backIt());
//     n_064.reorderIncidence(std::move(edges_064));
// 
//     vector<edge_list_it_t> edges_065;
//     edges_065.push_back(e_065_064.backIt());
//     edges_065.push_back(e_065_008.backIt());
//     n_065.reorderIncidence(std::move(edges_065));
// 
//     vector<edge_list_it_t> edges_066;
//     edges_066.push_back(e_066_060.backIt());
//     edges_066.push_back(e_053_066.backIt());
//     edges_066.push_back(e_042_066.backIt());
//     edges_066.push_back(e_066_064.backIt());
//     n_066.reorderIncidence(std::move(edges_066));
// 
//     vector<edge_list_it_t> edges_067;
//     edges_067.push_back(e_067_053.backIt());
//     edges_067.push_back(e_067_058.backIt());
//     n_067.reorderIncidence(std::move(edges_067));
// 
//     vector<edge_list_it_t> edges_068;
//     edges_068.push_back(e_069_068.backIt());
//     edges_068.push_back(e_068_070.backIt());
//     edges_068.push_back(e_068_050.backIt());
//     n_068.reorderIncidence(std::move(edges_068));
// 
//     vector<edge_list_it_t> edges_069;
//     edges_069.push_back(e_069_009.backIt());
//     edges_069.push_back(e_069_070.backIt());
//     edges_069.push_back(e_069_068.backIt());
//     n_069.reorderIncidence(std::move(edges_069));
// 
//     vector<edge_list_it_t> edges_070;
//     edges_070.push_back(e_068_070.backIt());
//     edges_070.push_back(e_069_070.backIt());
//     edges_070.push_back(e_050_070.backIt());
//     n_070.reorderIncidence(std::move(edges_070));
// 
//     vector<edge_list_it_t> edges_071;
//     edges_071.push_back(e_071_019.backIt());
//     edges_071.push_back(e_044_071.backIt());
//     edges_071.push_back(e_035_071.backIt());
//     edges_071.push_back(e_071_032.backIt());
//     n_071.reorderIncidence(std::move(edges_071));
// 
//     vector<edge_list_it_t> edges_072;
//     edges_072.push_back(e_072_019.backIt());
//     edges_072.push_back(e_032_072.backIt());
//     edges_072.push_back(e_078_072.backIt());
//     edges_072.push_back(e_072_076.backIt());
//     n_072.reorderIncidence(std::move(edges_072));
// 
//     vector<edge_list_it_t> edges_073;
//     edges_073.push_back(e_075_073.backIt());
//     edges_073.push_back(e_009_073.backIt());
//     edges_073.push_back(e_073_074.backIt());
//     edges_073.push_back(e_073_014.backIt());
//     n_073.reorderIncidence(std::move(edges_073));
// 
//     vector<edge_list_it_t> edges_074;
//     edges_074.push_back(e_079_074.backIt());
//     edges_074.push_back(e_040_074.backIt());
//     edges_074.push_back(e_014_074.backIt());
//     edges_074.push_back(e_073_074.backIt());
//     edges_074.push_back(e_009_074.backIt());
//     n_074.reorderIncidence(std::move(edges_074));
// 
//     vector<edge_list_it_t> edges_075;
//     edges_075.push_back(e_078_075.backIt());
//     edges_075.push_back(e_075_009.backIt());
//     edges_075.push_back(e_075_073.backIt());
//     edges_075.push_back(e_076_075.backIt());
//     n_075.reorderIncidence(std::move(edges_075));
// 
//     vector<edge_list_it_t> edges_076;
//     edges_076.push_back(e_072_076.backIt());
//     edges_076.push_back(e_076_075.backIt());
//     edges_076.push_back(e_076_014.backIt());
//     edges_076.push_back(e_077_076.backIt());
//     n_076.reorderIncidence(std::move(edges_076));
// 
//     vector<edge_list_it_t> edges_077;
//     edges_077.push_back(e_077_076.backIt());
//     edges_077.push_back(e_020_077.backIt());
//     n_077.reorderIncidence(std::move(edges_077));
// 
//     vector<edge_list_it_t> edges_078;
//     edges_078.push_back(e_078_072.backIt());
//     edges_078.push_back(e_032_078.backIt());
//     edges_078.push_back(e_009_078.backIt());
//     edges_078.push_back(e_078_075.backIt());
//     n_078.reorderIncidence(std::move(edges_078));
// 
//     vector<edge_list_it_t> edges_079;
//     edges_079.push_back(e_063_079.backIt());
//     edges_079.push_back(e_040_079.backIt());
//     edges_079.push_back(e_079_074.backIt());
//     edges_079.push_back(e_079_009.backIt());
//     n_079.reorderIncidence(std::move(edges_079));
// 
//     vector<edge_list_it_t> edges_080;
//     edges_080.push_back(e_020_080.backIt());
//     edges_080.push_back(e_080_014.backIt());
//     n_080.reorderIncidence(std::move(edges_080));
// 
//     vector<edge_list_it_t> edges_081;
//     edges_081.push_back(e_081_012.backIt());
//     edges_081.push_back(e_081_009.backIt());
//     n_081.reorderIncidence(std::move(edges_081));
// 
//     vector<edge_list_it_t> edges_082;
//     edges_082.push_back(e_082_083.backIt());
//     edges_082.push_back(e_082_009.backIt());
//     edges_082.push_back(e_082_008.backIt());
//     n_082.reorderIncidence(std::move(edges_082));
// 
//     vector<edge_list_it_t> edges_083;
//     edges_083.push_back(e_000_083.backIt());
//     edges_083.push_back(e_083_084.backIt());
//     edges_083.push_back(e_082_083.backIt());
//     edges_083.push_back(e_083_008.backIt());
//     edges_083.push_back(e_085_083.backIt());
//     edges_083.push_back(e_003_083.backIt());
//     edges_083.push_back(e_083_004.backIt());
//     n_083.reorderIncidence(std::move(edges_083));
// 
//     vector<edge_list_it_t> edges_084;
//     edges_084.push_back(e_000_084.backIt());
//     edges_084.push_back(e_010_084.backIt());
//     edges_084.push_back(e_083_084.backIt());
//     n_084.reorderIncidence(std::move(edges_084));
// 
//     vector<edge_list_it_t> edges_085;
//     edges_085.push_back(e_085_083.backIt());
//     edges_085.push_back(e_085_008.backIt());
//     edges_085.push_back(e_003_085.backIt());
//     edges_085.push_back(e_085_086.backIt());
//     n_085.reorderIncidence(std::move(edges_085));
// 
//     vector<edge_list_it_t> edges_086;
//     edges_086.push_back(e_085_086.backIt());
//     edges_086.push_back(e_003_086.backIt());
//     n_086.reorderIncidence(std::move(edges_086));
// 
//     vector<edge_list_it_t> edges_087;
//     edges_087.push_back(e_087_008.backIt());
//     edges_087.push_back(e_088_087.backIt());
//     n_087.reorderIncidence(std::move(edges_087));
// 
//     vector<edge_list_it_t> edges_088;
//     edges_088.push_back(e_088_087.backIt());
//     edges_088.push_back(e_088_007.backIt());
//     edges_088.push_back(e_088_092.backIt());
//     n_088.reorderIncidence(std::move(edges_088));
// 
//     vector<edge_list_it_t> edges_089;
//     edges_089.push_back(e_091_089.backIt());
//     edges_089.push_back(e_089_090.backIt());
//     edges_089.push_back(e_007_089.backIt());
//     n_089.reorderIncidence(std::move(edges_089));
// 
//     vector<edge_list_it_t> edges_090;
//     edges_090.push_back(e_090_092.backIt());
//     edges_090.push_back(e_007_090.backIt());
//     edges_090.push_back(e_089_090.backIt());
//     edges_090.push_back(e_091_090.backIt());
//     n_090.reorderIncidence(std::move(edges_090));
// 
//     vector<edge_list_it_t> edges_091;
//     edges_091.push_back(e_091_090.backIt());
//     edges_091.push_back(e_091_089.backIt());
//     n_091.reorderIncidence(std::move(edges_091));
// 
//     vector<edge_list_it_t> edges_092;
//     edges_092.push_back(e_088_092.backIt());
//     edges_092.push_back(e_007_092.backIt());
//     edges_092.push_back(e_090_092.backIt());
//     n_092.reorderIncidence(std::move(edges_092));
// 
//     vector<edge_list_it_t> edges_093;
//     edges_093.push_back(e_000_093.backIt());
//     edges_093.push_back(e_093_095.backIt());
//     edges_093.push_back(e_094_093.backIt());
//     edges_093.push_back(e_096_093.backIt());
//     edges_093.push_back(e_002_093.backIt());
//     n_093.reorderIncidence(std::move(edges_093));
// 
//     vector<edge_list_it_t> edges_094;
//     edges_094.push_back(e_094_093.backIt());
//     edges_094.push_back(e_094_095.backIt());
//     edges_094.push_back(e_002_094.backIt());
//     n_094.reorderIncidence(std::move(edges_094));
// 
//     vector<edge_list_it_t> edges_095;
//     edges_095.push_back(e_093_095.backIt());
//     edges_095.push_back(e_094_095.backIt());
//     n_095.reorderIncidence(std::move(edges_095));
// 
//     vector<edge_list_it_t> edges_096;
//     edges_096.push_back(e_096_093.backIt());
//     edges_096.push_back(e_002_096.backIt());
//     edges_096.push_back(e_096_097.backIt());
//     n_096.reorderIncidence(std::move(edges_096));
// 
//     vector<edge_list_it_t> edges_097;
//     edges_097.push_back(e_096_097.backIt());
//     edges_097.push_back(e_002_097.backIt());
//     n_097.reorderIncidence(std::move(edges_097));
// 
//     vector<edge_list_it_t> edges_098;
//     edges_098.push_back(e_098_001.backIt());
//     edges_098.push_back(e_098_099.backIt());
//     edges_098.push_back(e_098_006.backIt());
//     edges_098.push_back(e_098_010.backIt());
//     n_098.reorderIncidence(std::move(edges_098));
// 
//     vector<edge_list_it_t> edges_099;
//     edges_099.push_back(e_001_099.backIt());
//     edges_099.push_back(e_006_099.backIt());
//     edges_099.push_back(e_098_099.backIt());
//     n_099.reorderIncidence(std::move(edges_099));
// 
//     vector<edge_list_it_t> edges_100;
//     edges_100.push_back(e_100_001.backIt());
//     edges_100.push_back(e_010_100.backIt());
//     n_100.reorderIncidence(std::move(edges_100));
// 
// 
//     vector<node_list_it_t> stOrder;
//     stOrder.push_back(n_000.backIt());
//     stOrder.push_back(n_093.backIt());
//     stOrder.push_back(n_095.backIt());
//     stOrder.push_back(n_094.backIt());
//     stOrder.push_back(n_096.backIt());
//     stOrder.push_back(n_097.backIt());
//     stOrder.push_back(n_001.backIt());
//     stOrder.push_back(n_100.backIt());
//     stOrder.push_back(n_006.backIt());
//     stOrder.push_back(n_005.backIt());
//     stOrder.push_back(n_012.backIt());
//     stOrder.push_back(n_063.backIt());
//     stOrder.push_back(n_042.backIt());
//     stOrder.push_back(n_045.backIt());
//     stOrder.push_back(n_028.backIt());
//     stOrder.push_back(n_025.backIt());
//     stOrder.push_back(n_022.backIt());
//     stOrder.push_back(n_030.backIt());
//     stOrder.push_back(n_021.backIt());
//     stOrder.push_back(n_024.backIt());
//     stOrder.push_back(n_023.backIt());
//     stOrder.push_back(n_026.backIt());
//     stOrder.push_back(n_027.backIt());
//     stOrder.push_back(n_043.backIt());
//     stOrder.push_back(n_039.backIt());
//     stOrder.push_back(n_038.backIt());
//     stOrder.push_back(n_037.backIt());
//     stOrder.push_back(n_019.backIt());
//     stOrder.push_back(n_044.backIt());
//     stOrder.push_back(n_071.backIt());
//     stOrder.push_back(n_032.backIt());
//     stOrder.push_back(n_081.backIt());
//     stOrder.push_back(n_079.backIt());
//     stOrder.push_back(n_018.backIt());
//     stOrder.push_back(n_041.backIt());
//     stOrder.push_back(n_017.backIt());
//     stOrder.push_back(n_040.backIt());
//     stOrder.push_back(n_016.backIt());
//     stOrder.push_back(n_015.backIt());
//     stOrder.push_back(n_013.backIt());
//     stOrder.push_back(n_072.backIt());
//     stOrder.push_back(n_078.backIt());
//     stOrder.push_back(n_075.backIt());
//     stOrder.push_back(n_073.backIt());
//     stOrder.push_back(n_076.backIt());
//     stOrder.push_back(n_077.backIt());
//     stOrder.push_back(n_020.backIt());
//     stOrder.push_back(n_080.backIt());
//     stOrder.push_back(n_014.backIt());
//     stOrder.push_back(n_074.backIt());
//     stOrder.push_back(n_011.backIt());
//     stOrder.push_back(n_009.backIt());
//     stOrder.push_back(n_069.backIt());
//     stOrder.push_back(n_068.backIt());
//     stOrder.push_back(n_070.backIt());
//     stOrder.push_back(n_050.backIt());
//     stOrder.push_back(n_049.backIt());
//     stOrder.push_back(n_047.backIt());
//     stOrder.push_back(n_046.backIt());
//     stOrder.push_back(n_036.backIt());
//     stOrder.push_back(n_031.backIt());
//     stOrder.push_back(n_033.backIt());
//     stOrder.push_back(n_029.backIt());
//     stOrder.push_back(n_035.backIt());
//     stOrder.push_back(n_034.backIt());
//     stOrder.push_back(n_048.backIt());
//     stOrder.push_back(n_052.backIt());
//     stOrder.push_back(n_051.backIt());
//     stOrder.push_back(n_053.backIt());
//     stOrder.push_back(n_057.backIt());
//     stOrder.push_back(n_054.backIt());
//     stOrder.push_back(n_056.backIt());
//     stOrder.push_back(n_055.backIt());
//     stOrder.push_back(n_067.backIt());
//     stOrder.push_back(n_058.backIt());
//     stOrder.push_back(n_061.backIt());
//     stOrder.push_back(n_060.backIt());
//     stOrder.push_back(n_059.backIt());
//     stOrder.push_back(n_066.backIt());
//     stOrder.push_back(n_062.backIt());
//     stOrder.push_back(n_064.backIt());
//     stOrder.push_back(n_065.backIt());
//     stOrder.push_back(n_099.backIt());
//     stOrder.push_back(n_098.backIt());
//     stOrder.push_back(n_010.backIt());
//     stOrder.push_back(n_084.backIt());
//     stOrder.push_back(n_083.backIt());
//     stOrder.push_back(n_085.backIt());
//     stOrder.push_back(n_086.backIt());
//     stOrder.push_back(n_082.backIt());
//     stOrder.push_back(n_008.backIt());
//     stOrder.push_back(n_087.backIt());
//     stOrder.push_back(n_088.backIt());
//     stOrder.push_back(n_092.backIt());
//     stOrder.push_back(n_090.backIt());
//     stOrder.push_back(n_091.backIt());
//     stOrder.push_back(n_089.backIt());
//     stOrder.push_back(n_007.backIt());
//     stOrder.push_back(n_003.backIt());
//     stOrder.push_back(n_002.backIt());
//     stOrder.push_back(n_004.backIt());
// 
// 
// 
//     BLPlanarityTester tester01;
//     auto res01 =  tester01.findEmbedding(g_1, stOrder);
//     if (res01) {
//         cerr << "The graph is planar. #1\n";
//     }
//     else{
//         cerr << "The graph is planar. #1\n";
//     }
// 
//     BLPlanarityTester tester02;
//     auto res02 =  tester02.isPlanar(g_1, stOrder);
//     if (res02) {
//         cerr << "The graph is planar. #2\n";
//     }
//     else{
//         cerr << "The graph is planar. #2\n";
//     }
// 
//     PlanarDualGraphMaker maker;
//     EmbeddedGraph emb;
//     DualGraph     dual;
//     maker.makeDualGraph(g_1, emb, dual);
// 
//     auto nPair = g_1.nodes();
//     for (auto nit = nPair.first; nit != nPair.second; nit++) {
//         auto& N = dynamic_cast<NumNode&>(*(*nit));
//         cerr << N.num() << " :";
//         for (auto iit  = N.incidentEdges().first;
//                   iit != N.incidentEdges().second; iit++) {
//             auto& E = *(*(*iit));
//             auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
//             cerr << " " << A.num();
// 
//         }
//         cerr << "\n";
//     }
// 
// }
// 
// 
// /**  @brief tests rearrangeIncidentEdges()
//  */
// TEST_F(BLPlanarityTesterTestsForEmbedding, TestRepro_005) {
// 
//     Graph g_1;
// 
//     auto& n_000 = g_1.addNode(make_unique<NumNode>(  0 ));
//     auto& n_001 = g_1.addNode(make_unique<NumNode>(  1 ));
//     auto& n_002 = g_1.addNode(make_unique<NumNode>(  2 ));
//     auto& n_003 = g_1.addNode(make_unique<NumNode>(  3 ));
//     auto& n_004 = g_1.addNode(make_unique<NumNode>(  4 ));
//     auto& n_005 = g_1.addNode(make_unique<NumNode>(  5 ));
//     auto& n_006 = g_1.addNode(make_unique<NumNode>(  6 ));
//     auto& n_007 = g_1.addNode(make_unique<NumNode>(  7 ));
//     auto& n_008 = g_1.addNode(make_unique<NumNode>(  8 ));
//     auto& n_009 = g_1.addNode(make_unique<NumNode>(  9 ));
//     auto& n_010 = g_1.addNode(make_unique<NumNode>( 10 ));
//     auto& n_011 = g_1.addNode(make_unique<NumNode>( 11 ));
//     auto& n_012 = g_1.addNode(make_unique<NumNode>( 12 ));
//     auto& n_013 = g_1.addNode(make_unique<NumNode>( 13 ));
//     auto& n_014 = g_1.addNode(make_unique<NumNode>( 14 ));
//     auto& n_015 = g_1.addNode(make_unique<NumNode>( 15 ));
//     auto& n_016 = g_1.addNode(make_unique<NumNode>( 16 ));
//     auto& n_017 = g_1.addNode(make_unique<NumNode>( 17 ));
//     auto& n_018 = g_1.addNode(make_unique<NumNode>( 18 ));
//     auto& n_019 = g_1.addNode(make_unique<NumNode>( 19 ));
//     auto& n_020 = g_1.addNode(make_unique<NumNode>( 20 ));
//     auto& n_021 = g_1.addNode(make_unique<NumNode>( 21 ));
//     auto& n_022 = g_1.addNode(make_unique<NumNode>( 22 ));
//     auto& n_023 = g_1.addNode(make_unique<NumNode>( 23 ));
//     auto& n_024 = g_1.addNode(make_unique<NumNode>( 24 ));
//     auto& n_025 = g_1.addNode(make_unique<NumNode>( 25 ));
//     auto& n_026 = g_1.addNode(make_unique<NumNode>( 26 ));
//     auto& n_027 = g_1.addNode(make_unique<NumNode>( 27 ));
//     auto& n_028 = g_1.addNode(make_unique<NumNode>( 28 ));
//     auto& n_029 = g_1.addNode(make_unique<NumNode>( 29 ));
//     auto& n_030 = g_1.addNode(make_unique<NumNode>( 30 ));
//     auto& n_031 = g_1.addNode(make_unique<NumNode>( 31 ));
//     auto& n_032 = g_1.addNode(make_unique<NumNode>( 32 ));
//     auto& n_033 = g_1.addNode(make_unique<NumNode>( 33 ));
//     auto& n_034 = g_1.addNode(make_unique<NumNode>( 34 ));
//     auto& n_035 = g_1.addNode(make_unique<NumNode>( 35 ));
//     auto& n_036 = g_1.addNode(make_unique<NumNode>( 36 ));
//     auto& n_037 = g_1.addNode(make_unique<NumNode>( 37 ));
//     auto& n_038 = g_1.addNode(make_unique<NumNode>( 38 ));
//     auto& n_039 = g_1.addNode(make_unique<NumNode>( 39 ));
//     auto& n_040 = g_1.addNode(make_unique<NumNode>( 40 ));
//     auto& n_041 = g_1.addNode(make_unique<NumNode>( 41 ));
//     auto& n_042 = g_1.addNode(make_unique<NumNode>( 42 ));
//     auto& n_043 = g_1.addNode(make_unique<NumNode>( 43 ));
//     auto& n_044 = g_1.addNode(make_unique<NumNode>( 44 ));
//     auto& n_045 = g_1.addNode(make_unique<NumNode>( 45 ));
//     auto& n_046 = g_1.addNode(make_unique<NumNode>( 46 ));
//     auto& n_047 = g_1.addNode(make_unique<NumNode>( 47 ));
//     auto& n_048 = g_1.addNode(make_unique<NumNode>( 48 ));
//     auto& n_049 = g_1.addNode(make_unique<NumNode>( 49 ));
//     auto& n_050 = g_1.addNode(make_unique<NumNode>( 50 ));
//     auto& n_051 = g_1.addNode(make_unique<NumNode>( 51 ));
//     auto& n_052 = g_1.addNode(make_unique<NumNode>( 52 ));
//     auto& n_053 = g_1.addNode(make_unique<NumNode>( 53 ));
//     auto& n_054 = g_1.addNode(make_unique<NumNode>( 54 ));
//     auto& n_055 = g_1.addNode(make_unique<NumNode>( 55 ));
//     auto& n_056 = g_1.addNode(make_unique<NumNode>( 56 ));
//     auto& n_057 = g_1.addNode(make_unique<NumNode>( 57 ));
//     auto& n_058 = g_1.addNode(make_unique<NumNode>( 58 ));
//     auto& n_059 = g_1.addNode(make_unique<NumNode>( 59 ));
//     auto& n_060 = g_1.addNode(make_unique<NumNode>( 60 ));
//     auto& n_061 = g_1.addNode(make_unique<NumNode>( 61 ));
//     auto& n_062 = g_1.addNode(make_unique<NumNode>( 62 ));
//     auto& n_063 = g_1.addNode(make_unique<NumNode>( 63 ));
//     auto& n_064 = g_1.addNode(make_unique<NumNode>( 64 ));
//     auto& n_065 = g_1.addNode(make_unique<NumNode>( 65 ));
//     auto& n_066 = g_1.addNode(make_unique<NumNode>( 66 ));
//     auto& n_067 = g_1.addNode(make_unique<NumNode>( 67 ));
//     auto& n_068 = g_1.addNode(make_unique<NumNode>( 68 ));
//     auto& n_069 = g_1.addNode(make_unique<NumNode>( 69 ));
//     auto& n_070 = g_1.addNode(make_unique<NumNode>( 70 ));
//     auto& n_071 = g_1.addNode(make_unique<NumNode>( 71 ));
//     auto& n_072 = g_1.addNode(make_unique<NumNode>( 72 ));
//     auto& n_073 = g_1.addNode(make_unique<NumNode>( 73 ));
//     auto& n_074 = g_1.addNode(make_unique<NumNode>( 74 ));
//     auto& n_075 = g_1.addNode(make_unique<NumNode>( 75 ));
//     auto& n_076 = g_1.addNode(make_unique<NumNode>( 76 ));
//     auto& n_077 = g_1.addNode(make_unique<NumNode>( 77 ));
//     auto& n_078 = g_1.addNode(make_unique<NumNode>( 78 ));
//     auto& n_079 = g_1.addNode(make_unique<NumNode>( 79 ));
//     auto& n_080 = g_1.addNode(make_unique<NumNode>( 80 ));
//     auto& n_081 = g_1.addNode(make_unique<NumNode>( 81 ));
//     auto& n_082 = g_1.addNode(make_unique<NumNode>( 82 ));
//     auto& n_083 = g_1.addNode(make_unique<NumNode>( 83 ));
//     auto& n_084 = g_1.addNode(make_unique<NumNode>( 84 ));
//     auto& n_085 = g_1.addNode(make_unique<NumNode>( 85 ));
//     auto& n_086 = g_1.addNode(make_unique<NumNode>( 86 ));
//     auto& n_087 = g_1.addNode(make_unique<NumNode>( 87 ));
//     auto& n_088 = g_1.addNode(make_unique<NumNode>( 88 ));
//     auto& n_089 = g_1.addNode(make_unique<NumNode>( 89 ));
//     auto& n_090 = g_1.addNode(make_unique<NumNode>( 90 ));
//     auto& n_091 = g_1.addNode(make_unique<NumNode>( 91 ));
//     auto& n_092 = g_1.addNode(make_unique<NumNode>( 92 ));
//     auto& n_093 = g_1.addNode(make_unique<NumNode>( 93 ));
//     auto& n_094 = g_1.addNode(make_unique<NumNode>( 94 ));
//     auto& n_095 = g_1.addNode(make_unique<NumNode>( 95 ));
//     auto& n_096 = g_1.addNode(make_unique<NumNode>( 96 ));
//     auto& n_097 = g_1.addNode(make_unique<NumNode>( 97 ));
//     auto& n_098 = g_1.addNode(make_unique<NumNode>( 98 ));
//     auto& n_099 = g_1.addNode(make_unique<NumNode>( 99 ));
//     auto& n_100 = g_1.addNode(make_unique<NumNode>(100 ));
//     auto& n_101 = g_1.addNode(make_unique<NumNode>(101 ));
//     auto& n_102 = g_1.addNode(make_unique<NumNode>(102 ));
//     auto& n_103 = g_1.addNode(make_unique<NumNode>(103 ));
//     auto& n_104 = g_1.addNode(make_unique<NumNode>(104 ));
//     auto& n_105 = g_1.addNode(make_unique<NumNode>(105 ));
//     auto& n_106 = g_1.addNode(make_unique<NumNode>(106 ));
//     auto& n_107 = g_1.addNode(make_unique<NumNode>(107 ));
//     auto& n_108 = g_1.addNode(make_unique<NumNode>(108 ));
//     auto& n_109 = g_1.addNode(make_unique<NumNode>(109 ));
//     auto& n_110 = g_1.addNode(make_unique<NumNode>(110 ));
//     auto& n_111 = g_1.addNode(make_unique<NumNode>(111 ));
//     auto& n_112 = g_1.addNode(make_unique<NumNode>(112 ));
//     auto& n_113 = g_1.addNode(make_unique<NumNode>(113 ));
//     auto& n_114 = g_1.addNode(make_unique<NumNode>(114 ));
//     auto& n_115 = g_1.addNode(make_unique<NumNode>(115 ));
//     auto& n_116 = g_1.addNode(make_unique<NumNode>(116 ));
//     auto& n_117 = g_1.addNode(make_unique<NumNode>(117 ));
//     auto& n_118 = g_1.addNode(make_unique<NumNode>(118 ));
//     auto& n_119 = g_1.addNode(make_unique<NumNode>(119 ));
//     auto& n_120 = g_1.addNode(make_unique<NumNode>(120 ));
//     auto& n_121 = g_1.addNode(make_unique<NumNode>(121 ));
//     auto& n_122 = g_1.addNode(make_unique<NumNode>(122 ));
//     auto& n_123 = g_1.addNode(make_unique<NumNode>(123 ));
//     auto& n_124 = g_1.addNode(make_unique<NumNode>(124 ));
//     auto& n_125 = g_1.addNode(make_unique<NumNode>(125 ));
//     auto& n_126 = g_1.addNode(make_unique<NumNode>(126 ));
//     auto& n_127 = g_1.addNode(make_unique<NumNode>(127 ));
//     auto& n_128 = g_1.addNode(make_unique<NumNode>(128 ));
//     auto& n_129 = g_1.addNode(make_unique<NumNode>(129 ));
//     auto& n_130 = g_1.addNode(make_unique<NumNode>(130 ));
//     auto& n_131 = g_1.addNode(make_unique<NumNode>(131 ));
//     auto& n_132 = g_1.addNode(make_unique<NumNode>(132 ));
//     auto& n_133 = g_1.addNode(make_unique<NumNode>(133 ));
//     auto& n_134 = g_1.addNode(make_unique<NumNode>(134 ));
//     auto& n_135 = g_1.addNode(make_unique<NumNode>(135 ));
//     auto& n_136 = g_1.addNode(make_unique<NumNode>(136 ));
//     auto& n_137 = g_1.addNode(make_unique<NumNode>(137 ));
//     auto& n_138 = g_1.addNode(make_unique<NumNode>(138 ));
//     auto& n_139 = g_1.addNode(make_unique<NumNode>(139 ));
//     auto& n_140 = g_1.addNode(make_unique<NumNode>(140 ));
//     auto& n_141 = g_1.addNode(make_unique<NumNode>(141 ));
// 
//     auto& e_041_013 = g_1.addEdge(make_unique<Edge>(), n_041, n_013 );
//     auto& e_099_097 = g_1.addEdge(make_unique<Edge>(), n_099, n_097 );
//     auto& e_070_103 = g_1.addEdge(make_unique<Edge>(), n_070, n_103 );
//     auto& e_138_001 = g_1.addEdge(make_unique<Edge>(), n_138, n_001 );
//     auto& e_098_099 = g_1.addEdge(make_unique<Edge>(), n_098, n_099 );
//     auto& e_070_058 = g_1.addEdge(make_unique<Edge>(), n_070, n_058 );
//     auto& e_055_054 = g_1.addEdge(make_unique<Edge>(), n_055, n_054 );
//     auto& e_082_087 = g_1.addEdge(make_unique<Edge>(), n_082, n_087 );
//     auto& e_026_028 = g_1.addEdge(make_unique<Edge>(), n_026, n_028 );
//     auto& e_022_019 = g_1.addEdge(make_unique<Edge>(), n_022, n_019 );
//     auto& e_082_112 = g_1.addEdge(make_unique<Edge>(), n_082, n_112 );
//     auto& e_003_114 = g_1.addEdge(make_unique<Edge>(), n_003, n_114 );
//     auto& e_085_111 = g_1.addEdge(make_unique<Edge>(), n_085, n_111 );
//     auto& e_094_095 = g_1.addEdge(make_unique<Edge>(), n_094, n_095 );
//     auto& e_135_132 = g_1.addEdge(make_unique<Edge>(), n_135, n_132 );
//     auto& e_137_004 = g_1.addEdge(make_unique<Edge>(), n_137, n_004 );
//     auto& e_045_009 = g_1.addEdge(make_unique<Edge>(), n_045, n_009 );
//     auto& e_132_031 = g_1.addEdge(make_unique<Edge>(), n_132, n_031 );
//     auto& e_129_126 = g_1.addEdge(make_unique<Edge>(), n_129, n_126 );
//     auto& e_032_002 = g_1.addEdge(make_unique<Edge>(), n_032, n_002 );
//     auto& e_038_037 = g_1.addEdge(make_unique<Edge>(), n_038, n_037 );
//     auto& e_004_000 = g_1.addEdge(make_unique<Edge>(), n_004, n_000 );
//     auto& e_105_090 = g_1.addEdge(make_unique<Edge>(), n_105, n_090 );
//     auto& e_098_096 = g_1.addEdge(make_unique<Edge>(), n_098, n_096 );
//     auto& e_051_050 = g_1.addEdge(make_unique<Edge>(), n_051, n_050 );
//     auto& e_082_118 = g_1.addEdge(make_unique<Edge>(), n_082, n_118 );
//     auto& e_056_061 = g_1.addEdge(make_unique<Edge>(), n_056, n_061 );
//     auto& e_095_135 = g_1.addEdge(make_unique<Edge>(), n_095, n_135 );
//     auto& e_086_109 = g_1.addEdge(make_unique<Edge>(), n_086, n_109 );
//     auto& e_085_105 = g_1.addEdge(make_unique<Edge>(), n_085, n_105 );
//     auto& e_129_128 = g_1.addEdge(make_unique<Edge>(), n_129, n_128 );
//     auto& e_115_116 = g_1.addEdge(make_unique<Edge>(), n_115, n_116 );
//     auto& e_007_006 = g_1.addEdge(make_unique<Edge>(), n_007, n_006 );
//     auto& e_028_035 = g_1.addEdge(make_unique<Edge>(), n_028, n_035 );
//     auto& e_050_039 = g_1.addEdge(make_unique<Edge>(), n_050, n_039 );
//     auto& e_028_029 = g_1.addEdge(make_unique<Edge>(), n_028, n_029 );
//     auto& e_115_002 = g_1.addEdge(make_unique<Edge>(), n_115, n_002 );
//     auto& e_131_006 = g_1.addEdge(make_unique<Edge>(), n_131, n_006 );
//     auto& e_117_070 = g_1.addEdge(make_unique<Edge>(), n_117, n_070 );
//     auto& e_136_006 = g_1.addEdge(make_unique<Edge>(), n_136, n_006 );
//     auto& e_093_092 = g_1.addEdge(make_unique<Edge>(), n_093, n_092 );
//     auto& e_052_127 = g_1.addEdge(make_unique<Edge>(), n_052, n_127 );
//     auto& e_042_041 = g_1.addEdge(make_unique<Edge>(), n_042, n_041 );
//     auto& e_075_077 = g_1.addEdge(make_unique<Edge>(), n_075, n_077 );
//     auto& e_000_001 = g_1.addEdge(make_unique<Edge>(), n_000, n_001 );
//     auto& e_061_060 = g_1.addEdge(make_unique<Edge>(), n_061, n_060 );
//     auto& e_139_113 = g_1.addEdge(make_unique<Edge>(), n_139, n_113 );
//     auto& e_024_023 = g_1.addEdge(make_unique<Edge>(), n_024, n_023 );
//     auto& e_024_039 = g_1.addEdge(make_unique<Edge>(), n_024, n_039 );
//     auto& e_003_141 = g_1.addEdge(make_unique<Edge>(), n_003, n_141 );
//     auto& e_116_114 = g_1.addEdge(make_unique<Edge>(), n_116, n_114 );
//     auto& e_126_002 = g_1.addEdge(make_unique<Edge>(), n_126, n_002 );
//     auto& e_022_048 = g_1.addEdge(make_unique<Edge>(), n_022, n_048 );
//     auto& e_107_086 = g_1.addEdge(make_unique<Edge>(), n_107, n_086 );
//     auto& e_086_110 = g_1.addEdge(make_unique<Edge>(), n_086, n_110 );
//     auto& e_101_089 = g_1.addEdge(make_unique<Edge>(), n_101, n_089 );
//     auto& e_007_005 = g_1.addEdge(make_unique<Edge>(), n_007, n_005 );
//     auto& e_047_040 = g_1.addEdge(make_unique<Edge>(), n_047, n_040 );
//     auto& e_104_103 = g_1.addEdge(make_unique<Edge>(), n_104, n_103 );
//     auto& e_097_135 = g_1.addEdge(make_unique<Edge>(), n_097, n_135 );
//     auto& e_050_040 = g_1.addEdge(make_unique<Edge>(), n_050, n_040 );
//     auto& e_072_070 = g_1.addEdge(make_unique<Edge>(), n_072, n_070 );
//     auto& e_021_018 = g_1.addEdge(make_unique<Edge>(), n_021, n_018 );
//     auto& e_069_067 = g_1.addEdge(make_unique<Edge>(), n_069, n_067 );
//     auto& e_017_018 = g_1.addEdge(make_unique<Edge>(), n_017, n_018 );
//     auto& e_048_024 = g_1.addEdge(make_unique<Edge>(), n_048, n_024 );
//     auto& e_104_088 = g_1.addEdge(make_unique<Edge>(), n_104, n_088 );
//     auto& e_094_000 = g_1.addEdge(make_unique<Edge>(), n_094, n_000 );
//     auto& e_092_091 = g_1.addEdge(make_unique<Edge>(), n_092, n_091 );
//     auto& e_137_002 = g_1.addEdge(make_unique<Edge>(), n_137, n_002 );
//     auto& e_103_102 = g_1.addEdge(make_unique<Edge>(), n_103, n_102 );
//     auto& e_019_049 = g_1.addEdge(make_unique<Edge>(), n_019, n_049 );
//     auto& e_139_003 = g_1.addEdge(make_unique<Edge>(), n_139, n_003 );
//     auto& e_058_066 = g_1.addEdge(make_unique<Edge>(), n_058, n_066 );
//     auto& e_005_014 = g_1.addEdge(make_unique<Edge>(), n_005, n_014 );
//     auto& e_011_008 = g_1.addEdge(make_unique<Edge>(), n_011, n_008 );
//     auto& e_085_084 = g_1.addEdge(make_unique<Edge>(), n_085, n_084 );
//     auto& e_060_065 = g_1.addEdge(make_unique<Edge>(), n_060, n_065 );
//     auto& e_040_048 = g_1.addEdge(make_unique<Edge>(), n_040, n_048 );
//     auto& e_056_055 = g_1.addEdge(make_unique<Edge>(), n_056, n_055 );
//     auto& e_036_002 = g_1.addEdge(make_unique<Edge>(), n_036, n_002 );
//     auto& e_059_066 = g_1.addEdge(make_unique<Edge>(), n_059, n_066 );
//     auto& e_129_127 = g_1.addEdge(make_unique<Edge>(), n_129, n_127 );
//     auto& e_015_016 = g_1.addEdge(make_unique<Edge>(), n_015, n_016 );
//     auto& e_055_050 = g_1.addEdge(make_unique<Edge>(), n_055, n_050 );
//     auto& e_045_093 = g_1.addEdge(make_unique<Edge>(), n_045, n_093 );
//     auto& e_028_037 = g_1.addEdge(make_unique<Edge>(), n_028, n_037 );
//     auto& e_023_026 = g_1.addEdge(make_unique<Edge>(), n_023, n_026 );
//     auto& e_134_132 = g_1.addEdge(make_unique<Edge>(), n_134, n_132 );
//     auto& e_022_047 = g_1.addEdge(make_unique<Edge>(), n_022, n_047 );
//     auto& e_081_107 = g_1.addEdge(make_unique<Edge>(), n_081, n_107 );
//     auto& e_045_042 = g_1.addEdge(make_unique<Edge>(), n_045, n_042 );
//     auto& e_069_068 = g_1.addEdge(make_unique<Edge>(), n_069, n_068 );
//     auto& e_126_125 = g_1.addEdge(make_unique<Edge>(), n_126, n_125 );
//     auto& e_092_098 = g_1.addEdge(make_unique<Edge>(), n_092, n_098 );
//     auto& e_081_120 = g_1.addEdge(make_unique<Edge>(), n_081, n_120 );
//     auto& e_070_066 = g_1.addEdge(make_unique<Edge>(), n_070, n_066 );
//     auto& e_043_046 = g_1.addEdge(make_unique<Edge>(), n_043, n_046 );
//     auto& e_124_058 = g_1.addEdge(make_unique<Edge>(), n_124, n_058 );
//     auto& e_084_111 = g_1.addEdge(make_unique<Edge>(), n_084, n_111 );
//     auto& e_041_012 = g_1.addEdge(make_unique<Edge>(), n_041, n_012 );
//     auto& e_064_092 = g_1.addEdge(make_unique<Edge>(), n_064, n_092 );
//     auto& e_021_046 = g_1.addEdge(make_unique<Edge>(), n_021, n_046 );
//     auto& e_054_052 = g_1.addEdge(make_unique<Edge>(), n_054, n_052 );
//     auto& e_045_044 = g_1.addEdge(make_unique<Edge>(), n_045, n_044 );
//     auto& e_015_023 = g_1.addEdge(make_unique<Edge>(), n_015, n_023 );
//     auto& e_078_076 = g_1.addEdge(make_unique<Edge>(), n_078, n_076 );
//     auto& e_020_021 = g_1.addEdge(make_unique<Edge>(), n_020, n_021 );
//     auto& e_088_102 = g_1.addEdge(make_unique<Edge>(), n_088, n_102 );
//     auto& e_091_089 = g_1.addEdge(make_unique<Edge>(), n_091, n_089 );
//     auto& e_123_070 = g_1.addEdge(make_unique<Edge>(), n_123, n_070 );
//     auto& e_131_031 = g_1.addEdge(make_unique<Edge>(), n_131, n_031 );
//     auto& e_081_118 = g_1.addEdge(make_unique<Edge>(), n_081, n_118 );
//     auto& e_026_025 = g_1.addEdge(make_unique<Edge>(), n_026, n_025 );
//     auto& e_080_079 = g_1.addEdge(make_unique<Edge>(), n_080, n_079 );
//     auto& e_118_070 = g_1.addEdge(make_unique<Edge>(), n_118, n_070 );
//     auto& e_008_006 = g_1.addEdge(make_unique<Edge>(), n_008, n_006 );
//     auto& e_014_034 = g_1.addEdge(make_unique<Edge>(), n_014, n_034 );
//     auto& e_117_118 = g_1.addEdge(make_unique<Edge>(), n_117, n_118 );
//     auto& e_045_100 = g_1.addEdge(make_unique<Edge>(), n_045, n_100 );
//     auto& e_099_133 = g_1.addEdge(make_unique<Edge>(), n_099, n_133 );
//     auto& e_091_096 = g_1.addEdge(make_unique<Edge>(), n_091, n_096 );
//     auto& e_096_097 = g_1.addEdge(make_unique<Edge>(), n_096, n_097 );
//     auto& e_077_076 = g_1.addEdge(make_unique<Edge>(), n_077, n_076 );
//     auto& e_075_076 = g_1.addEdge(make_unique<Edge>(), n_075, n_076 );
//     auto& e_060_059 = g_1.addEdge(make_unique<Edge>(), n_060, n_059 );
//     auto& e_106_104 = g_1.addEdge(make_unique<Edge>(), n_106, n_104 );
//     auto& e_081_070 = g_1.addEdge(make_unique<Edge>(), n_081, n_070 );
//     auto& e_109_084 = g_1.addEdge(make_unique<Edge>(), n_109, n_084 );
//     auto& e_012_007 = g_1.addEdge(make_unique<Edge>(), n_012, n_007 );
//     auto& e_019_017 = g_1.addEdge(make_unique<Edge>(), n_019, n_017 );
//     auto& e_125_053 = g_1.addEdge(make_unique<Edge>(), n_125, n_053 );
//     auto& e_046_027 = g_1.addEdge(make_unique<Edge>(), n_046, n_027 );
//     auto& e_029_033 = g_1.addEdge(make_unique<Edge>(), n_029, n_033 );
//     auto& e_072_071 = g_1.addEdge(make_unique<Edge>(), n_072, n_071 );
//     auto& e_123_069 = g_1.addEdge(make_unique<Edge>(), n_123, n_069 );
//     auto& e_132_002 = g_1.addEdge(make_unique<Edge>(), n_132, n_002 );
//     auto& e_083_084 = g_1.addEdge(make_unique<Edge>(), n_083, n_084 );
//     auto& e_009_134 = g_1.addEdge(make_unique<Edge>(), n_009, n_134 );
//     auto& e_052_125 = g_1.addEdge(make_unique<Edge>(), n_052, n_125 );
//     auto& e_102_067 = g_1.addEdge(make_unique<Edge>(), n_102, n_067 );
//     auto& e_052_056 = g_1.addEdge(make_unique<Edge>(), n_052, n_056 );
//     auto& e_077_078 = g_1.addEdge(make_unique<Edge>(), n_077, n_078 );
//     auto& e_067_068 = g_1.addEdge(make_unique<Edge>(), n_067, n_068 );
//     auto& e_022_049 = g_1.addEdge(make_unique<Edge>(), n_022, n_049 );
//     auto& e_065_091 = g_1.addEdge(make_unique<Edge>(), n_065, n_091 );
//     auto& e_057_058 = g_1.addEdge(make_unique<Edge>(), n_057, n_058 );
//     auto& e_114_112 = g_1.addEdge(make_unique<Edge>(), n_114, n_112 );
//     auto& e_103_069 = g_1.addEdge(make_unique<Edge>(), n_103, n_069 );
//     auto& e_122_070 = g_1.addEdge(make_unique<Edge>(), n_122, n_070 );
//     auto& e_039_038 = g_1.addEdge(make_unique<Edge>(), n_039, n_038 );
//     auto& e_109_110 = g_1.addEdge(make_unique<Edge>(), n_109, n_110 );
//     auto& e_112_083 = g_1.addEdge(make_unique<Edge>(), n_112, n_083 );
//     auto& e_053_115 = g_1.addEdge(make_unique<Edge>(), n_053, n_115 );
//     auto& e_010_099 = g_1.addEdge(make_unique<Edge>(), n_010, n_099 );
//     auto& e_066_068 = g_1.addEdge(make_unique<Edge>(), n_066, n_068 );
//     auto& e_023_018 = g_1.addEdge(make_unique<Edge>(), n_023, n_018 );
//     auto& e_097_095 = g_1.addEdge(make_unique<Edge>(), n_097, n_095 );
//     auto& e_082_119 = g_1.addEdge(make_unique<Edge>(), n_082, n_119 );
//     auto& e_133_009 = g_1.addEdge(make_unique<Edge>(), n_133, n_009 );
//     auto& e_086_087 = g_1.addEdge(make_unique<Edge>(), n_086, n_087 );
//     auto& e_027_025 = g_1.addEdge(make_unique<Edge>(), n_027, n_025 );
//     auto& e_042_043 = g_1.addEdge(make_unique<Edge>(), n_042, n_043 );
//     auto& e_081_108 = g_1.addEdge(make_unique<Edge>(), n_081, n_108 );
//     auto& e_039_026 = g_1.addEdge(make_unique<Edge>(), n_039, n_026 );
//     auto& e_095_004 = g_1.addEdge(make_unique<Edge>(), n_095, n_004 );
//     auto& e_024_049 = g_1.addEdge(make_unique<Edge>(), n_024, n_049 );
//     auto& e_009_008 = g_1.addEdge(make_unique<Edge>(), n_009, n_008 );
//     auto& e_100_093 = g_1.addEdge(make_unique<Edge>(), n_100, n_093 );
//     auto& e_062_061 = g_1.addEdge(make_unique<Edge>(), n_062, n_061 );
//     auto& e_030_031 = g_1.addEdge(make_unique<Edge>(), n_030, n_031 );
//     auto& e_029_013 = g_1.addEdge(make_unique<Edge>(), n_029, n_013 );
//     auto& e_035_029 = g_1.addEdge(make_unique<Edge>(), n_035, n_029 );
//     auto& e_015_019 = g_1.addEdge(make_unique<Edge>(), n_015, n_019 );
//     auto& e_073_056 = g_1.addEdge(make_unique<Edge>(), n_073, n_056 );
//     auto& e_063_062 = g_1.addEdge(make_unique<Edge>(), n_063, n_062 );
//     auto& e_020_017 = g_1.addEdge(make_unique<Edge>(), n_020, n_017 );
//     auto& e_028_038 = g_1.addEdge(make_unique<Edge>(), n_028, n_038 );
//     auto& e_009_044 = g_1.addEdge(make_unique<Edge>(), n_009, n_044 );
//     auto& e_017_015 = g_1.addEdge(make_unique<Edge>(), n_017, n_015 );
//     auto& e_022_020 = g_1.addEdge(make_unique<Edge>(), n_022, n_020 );
//     auto& e_077_079 = g_1.addEdge(make_unique<Edge>(), n_077, n_079 );
//     auto& e_108_107 = g_1.addEdge(make_unique<Edge>(), n_108, n_107 );
//     auto& e_010_100 = g_1.addEdge(make_unique<Edge>(), n_010, n_100 );
//     auto& e_035_032 = g_1.addEdge(make_unique<Edge>(), n_035, n_032 );
//     auto& e_081_106 = g_1.addEdge(make_unique<Edge>(), n_081, n_106 );
//     auto& e_019_020 = g_1.addEdge(make_unique<Edge>(), n_019, n_020 );
//     auto& e_087_118 = g_1.addEdge(make_unique<Edge>(), n_087, n_118 );
//     auto& e_002_030 = g_1.addEdge(make_unique<Edge>(), n_002, n_030 );
//     auto& e_018_027 = g_1.addEdge(make_unique<Edge>(), n_018, n_027 );
//     auto& e_080_078 = g_1.addEdge(make_unique<Edge>(), n_080, n_078 );
//     auto& e_134_131 = g_1.addEdge(make_unique<Edge>(), n_134, n_131 );
//     auto& e_120_121 = g_1.addEdge(make_unique<Edge>(), n_120, n_121 );
//     auto& e_028_002 = g_1.addEdge(make_unique<Edge>(), n_028, n_002 );
//     auto& e_009_011 = g_1.addEdge(make_unique<Edge>(), n_009, n_011 );
//     auto& e_033_006 = g_1.addEdge(make_unique<Edge>(), n_033, n_006 );
//     auto& e_028_013 = g_1.addEdge(make_unique<Edge>(), n_028, n_013 );
//     auto& e_125_002 = g_1.addEdge(make_unique<Edge>(), n_125, n_002 );
//     auto& e_124_056 = g_1.addEdge(make_unique<Edge>(), n_124, n_056 );
//     auto& e_056_053 = g_1.addEdge(make_unique<Edge>(), n_056, n_053 );
//     auto& e_082_084 = g_1.addEdge(make_unique<Edge>(), n_082, n_084 );
//     auto& e_086_085 = g_1.addEdge(make_unique<Edge>(), n_086, n_085 );
//     auto& e_045_043 = g_1.addEdge(make_unique<Edge>(), n_045, n_043 );
//     auto& e_114_119 = g_1.addEdge(make_unique<Edge>(), n_114, n_119 );
//     auto& e_040_024 = g_1.addEdge(make_unique<Edge>(), n_040, n_024 );
//     auto& e_054_056 = g_1.addEdge(make_unique<Edge>(), n_054, n_056 );
//     auto& e_116_002 = g_1.addEdge(make_unique<Edge>(), n_116, n_002 );
//     auto& e_088_105 = g_1.addEdge(make_unique<Edge>(), n_088, n_105 );
//     auto& e_101_090 = g_1.addEdge(make_unique<Edge>(), n_101, n_090 );
//     auto& e_071_056 = g_1.addEdge(make_unique<Edge>(), n_071, n_056 );
//     auto& e_082_083 = g_1.addEdge(make_unique<Edge>(), n_082, n_083 );
//     auto& e_034_033 = g_1.addEdge(make_unique<Edge>(), n_034, n_033 );
//     auto& e_087_108 = g_1.addEdge(make_unique<Edge>(), n_087, n_108 );
//     auto& e_135_133 = g_1.addEdge(make_unique<Edge>(), n_135, n_133 );
//     auto& e_002_003 = g_1.addEdge(make_unique<Edge>(), n_002, n_003 );
//     auto& e_008_007 = g_1.addEdge(make_unique<Edge>(), n_008, n_007 );
//     auto& e_044_042 = g_1.addEdge(make_unique<Edge>(), n_044, n_042 );
//     auto& e_102_101 = g_1.addEdge(make_unique<Edge>(), n_102, n_101 );
//     auto& e_120_118 = g_1.addEdge(make_unique<Edge>(), n_120, n_118 );
//     auto& e_079_078 = g_1.addEdge(make_unique<Edge>(), n_079, n_078 );
//     auto& e_009_006 = g_1.addEdge(make_unique<Edge>(), n_009, n_006 );
//     auto& e_130_036 = g_1.addEdge(make_unique<Edge>(), n_130, n_036 );
//     auto& e_030_006 = g_1.addEdge(make_unique<Edge>(), n_030, n_006 );
//     auto& e_112_113 = g_1.addEdge(make_unique<Edge>(), n_112, n_113 );
//     auto& e_138_090 = g_1.addEdge(make_unique<Edge>(), n_138, n_090 );
//     auto& e_015_024 = g_1.addEdge(make_unique<Edge>(), n_015, n_024 );
//     auto& e_019_016 = g_1.addEdge(make_unique<Edge>(), n_019, n_016 );
//     auto& e_118_108 = g_1.addEdge(make_unique<Edge>(), n_118, n_108 );
//     auto& e_067_065 = g_1.addEdge(make_unique<Edge>(), n_067, n_065 );
//     auto& e_006_031 = g_1.addEdge(make_unique<Edge>(), n_006, n_031 );
//     auto& e_013_005 = g_1.addEdge(make_unique<Edge>(), n_013, n_005 );
//     auto& e_061_064 = g_1.addEdge(make_unique<Edge>(), n_061, n_064 );
//     auto& e_025_013 = g_1.addEdge(make_unique<Edge>(), n_025, n_013 );
//     auto& e_075_122 = g_1.addEdge(make_unique<Edge>(), n_075, n_122 );
//     auto& e_044_011 = g_1.addEdge(make_unique<Edge>(), n_044, n_011 );
//     auto& e_113_140 = g_1.addEdge(make_unique<Edge>(), n_113, n_140 );
//     auto& e_032_030 = g_1.addEdge(make_unique<Edge>(), n_032, n_030 );
//     auto& e_083_085 = g_1.addEdge(make_unique<Edge>(), n_083, n_085 );
//     auto& e_045_010 = g_1.addEdge(make_unique<Edge>(), n_045, n_010 );
//     auto& e_002_004 = g_1.addEdge(make_unique<Edge>(), n_002, n_004 );
//     auto& e_089_094 = g_1.addEdge(make_unique<Edge>(), n_089, n_094 );
//     auto& e_016_049 = g_1.addEdge(make_unique<Edge>(), n_016, n_049 );
//     auto& e_037_036 = g_1.addEdge(make_unique<Edge>(), n_037, n_036 );
//     auto& e_118_119 = g_1.addEdge(make_unique<Edge>(), n_118, n_119 );
//     auto& e_051_054 = g_1.addEdge(make_unique<Edge>(), n_051, n_054 );
//     auto& e_056_060 = g_1.addEdge(make_unique<Edge>(), n_056, n_060 );
//     auto& e_005_006 = g_1.addEdge(make_unique<Edge>(), n_005, n_006 );
//     auto& e_086_108 = g_1.addEdge(make_unique<Edge>(), n_086, n_108 );
//     auto& e_090_140 = g_1.addEdge(make_unique<Edge>(), n_090, n_140 );
//     auto& e_100_098 = g_1.addEdge(make_unique<Edge>(), n_100, n_098 );
//     auto& e_014_013 = g_1.addEdge(make_unique<Edge>(), n_014, n_013 );
//     auto& e_139_090 = g_1.addEdge(make_unique<Edge>(), n_139, n_090 );
//     auto& e_047_045 = g_1.addEdge(make_unique<Edge>(), n_047, n_045 );
//     auto& e_064_063 = g_1.addEdge(make_unique<Edge>(), n_064, n_063 );
//     auto& e_045_062 = g_1.addEdge(make_unique<Edge>(), n_045, n_062 );
//     auto& e_106_077 = g_1.addEdge(make_unique<Edge>(), n_106, n_077 );
//     auto& e_033_032 = g_1.addEdge(make_unique<Edge>(), n_033, n_032 );
//     auto& e_001_139 = g_1.addEdge(make_unique<Edge>(), n_001, n_139 );
//     auto& e_086_088 = g_1.addEdge(make_unique<Edge>(), n_086, n_088 );
//     auto& e_001_002 = g_1.addEdge(make_unique<Edge>(), n_001, n_002 );
//     auto& e_074_072 = g_1.addEdge(make_unique<Edge>(), n_074, n_072 );
//     auto& e_117_116 = g_1.addEdge(make_unique<Edge>(), n_117, n_116 );
//     auto& e_016_024 = g_1.addEdge(make_unique<Edge>(), n_016, n_024 );
//     auto& e_128_127 = g_1.addEdge(make_unique<Edge>(), n_128, n_127 );
//     auto& e_074_071 = g_1.addEdge(make_unique<Edge>(), n_074, n_071 );
//     auto& e_053_002 = g_1.addEdge(make_unique<Edge>(), n_053, n_002 );
//     auto& e_105_101 = g_1.addEdge(make_unique<Edge>(), n_105, n_101 );
//     auto& e_089_090 = g_1.addEdge(make_unique<Edge>(), n_089, n_090 );
//     auto& e_028_130 = g_1.addEdge(make_unique<Edge>(), n_028, n_130 );
//     auto& e_037_052 = g_1.addEdge(make_unique<Edge>(), n_037, n_052 );
//     auto& e_086_082 = g_1.addEdge(make_unique<Edge>(), n_086, n_082 );
//     auto& e_002_035 = g_1.addEdge(make_unique<Edge>(), n_002, n_035 );
//     auto& e_015_018 = g_1.addEdge(make_unique<Edge>(), n_015, n_018 );
//     auto& e_139_140 = g_1.addEdge(make_unique<Edge>(), n_139, n_140 );
//     auto& e_022_021 = g_1.addEdge(make_unique<Edge>(), n_022, n_021 );
//     auto& e_073_071 = g_1.addEdge(make_unique<Edge>(), n_073, n_071 );
//     auto& e_070_115 = g_1.addEdge(make_unique<Edge>(), n_070, n_115 );
//     auto& e_038_051 = g_1.addEdge(make_unique<Edge>(), n_038, n_051 );
//     auto& e_059_058 = g_1.addEdge(make_unique<Edge>(), n_059, n_058 );
//     auto& e_093_063 = g_1.addEdge(make_unique<Edge>(), n_093, n_063 );
//     auto& e_086_084 = g_1.addEdge(make_unique<Edge>(), n_086, n_084 );
//     auto& e_002_031 = g_1.addEdge(make_unique<Edge>(), n_002, n_031 );
//     auto& e_133_134 = g_1.addEdge(make_unique<Edge>(), n_133, n_134 );
//     auto& e_009_131 = g_1.addEdge(make_unique<Edge>(), n_009, n_131 );
//     auto& e_141_113 = g_1.addEdge(make_unique<Edge>(), n_141, n_113 );
//     auto& e_027_013 = g_1.addEdge(make_unique<Edge>(), n_027, n_013 );
//     auto& e_009_010 = g_1.addEdge(make_unique<Edge>(), n_009, n_010 );
//     auto& e_063_045 = g_1.addEdge(make_unique<Edge>(), n_063, n_045 );
//     auto& e_052_051 = g_1.addEdge(make_unique<Edge>(), n_052, n_051 );
//     auto& e_081_077 = g_1.addEdge(make_unique<Edge>(), n_081, n_077 );
//     auto& e_046_041 = g_1.addEdge(make_unique<Edge>(), n_046, n_041 );
//     auto& e_062_056 = g_1.addEdge(make_unique<Edge>(), n_062, n_056 );
//     auto& e_011_012 = g_1.addEdge(make_unique<Edge>(), n_011, n_012 );
//     auto& e_025_028 = g_1.addEdge(make_unique<Edge>(), n_025, n_028 );
//     auto& e_000_090 = g_1.addEdge(make_unique<Edge>(), n_000, n_090 );
//     auto& e_048_049 = g_1.addEdge(make_unique<Edge>(), n_048, n_049 );
//     auto& e_045_055 = g_1.addEdge(make_unique<Edge>(), n_045, n_055 );
//     auto& e_128_052 = g_1.addEdge(make_unique<Edge>(), n_128, n_052 );
//     auto& e_107_104 = g_1.addEdge(make_unique<Edge>(), n_107, n_104 );
//     auto& e_120_070 = g_1.addEdge(make_unique<Edge>(), n_120, n_070 );
//     auto& e_059_057 = g_1.addEdge(make_unique<Edge>(), n_059, n_057 );
//     auto& e_096_094 = g_1.addEdge(make_unique<Edge>(), n_096, n_094 );
//     auto& e_036_126 = g_1.addEdge(make_unique<Edge>(), n_036, n_126 );
//     auto& e_065_064 = g_1.addEdge(make_unique<Edge>(), n_065, n_064 );
//     auto& e_012_013 = g_1.addEdge(make_unique<Edge>(), n_012, n_013 );
//     auto& e_081_121 = g_1.addEdge(make_unique<Edge>(), n_081, n_121 );
//     auto& e_090_113 = g_1.addEdge(make_unique<Edge>(), n_090, n_113 );
//     auto& e_043_047 = g_1.addEdge(make_unique<Edge>(), n_043, n_047 );
//     auto& e_014_136 = g_1.addEdge(make_unique<Edge>(), n_014, n_136 );
// 
//     vector<edge_list_it_t> edges_000;
//     edges_000.push_back(e_094_000.backIt());
//     edges_000.push_back(e_000_090.backIt());
//     edges_000.push_back(e_000_001.backIt());
//     edges_000.push_back(e_004_000.backIt());
//     n_000.reorderIncidence(std::move(edges_000));
// 
//     vector<edge_list_it_t> edges_001;
//     edges_001.push_back(e_001_002.backIt());
//     edges_001.push_back(e_000_001.backIt());
//     edges_001.push_back(e_138_001.backIt());
//     edges_001.push_back(e_001_139.backIt());
//     n_001.reorderIncidence(std::move(edges_001));
// 
//     vector<edge_list_it_t> edges_002;
//     edges_002.push_back(e_116_002.backIt());
//     edges_002.push_back(e_115_002.backIt());
//     edges_002.push_back(e_053_002.backIt());
//     edges_002.push_back(e_125_002.backIt());
//     edges_002.push_back(e_126_002.backIt());
//     edges_002.push_back(e_036_002.backIt());
//     edges_002.push_back(e_028_002.backIt());
//     edges_002.push_back(e_002_035.backIt());
//     edges_002.push_back(e_032_002.backIt());
//     edges_002.push_back(e_002_030.backIt());
//     edges_002.push_back(e_002_031.backIt());
//     edges_002.push_back(e_132_002.backIt());
//     edges_002.push_back(e_137_002.backIt());
//     edges_002.push_back(e_002_004.backIt());
//     edges_002.push_back(e_001_002.backIt());
//     edges_002.push_back(e_002_003.backIt());
//     n_002.reorderIncidence(std::move(edges_002));
// 
//     vector<edge_list_it_t> edges_003;
//     edges_003.push_back(e_003_114.backIt());
//     edges_003.push_back(e_002_003.backIt());
//     edges_003.push_back(e_139_003.backIt());
//     edges_003.push_back(e_003_141.backIt());
//     n_003.reorderIncidence(std::move(edges_003));
// 
//     vector<edge_list_it_t> edges_004;
//     edges_004.push_back(e_004_000.backIt());
//     edges_004.push_back(e_002_004.backIt());
//     edges_004.push_back(e_137_004.backIt());
//     edges_004.push_back(e_095_004.backIt());
//     n_004.reorderIncidence(std::move(edges_004));
// 
//     vector<edge_list_it_t> edges_005;
//     edges_005.push_back(e_013_005.backIt());
//     edges_005.push_back(e_007_005.backIt());
//     edges_005.push_back(e_005_006.backIt());
//     edges_005.push_back(e_005_014.backIt());
//     n_005.reorderIncidence(std::move(edges_005));
// 
//     vector<edge_list_it_t> edges_006;
//     edges_006.push_back(e_030_006.backIt());
//     edges_006.push_back(e_033_006.backIt());
//     edges_006.push_back(e_136_006.backIt());
//     edges_006.push_back(e_005_006.backIt());
//     edges_006.push_back(e_007_006.backIt());
//     edges_006.push_back(e_008_006.backIt());
//     edges_006.push_back(e_009_006.backIt());
//     edges_006.push_back(e_131_006.backIt());
//     edges_006.push_back(e_006_031.backIt());
//     n_006.reorderIncidence(std::move(edges_006));
// 
//     vector<edge_list_it_t> edges_007;
//     edges_007.push_back(e_012_007.backIt());
//     edges_007.push_back(e_008_007.backIt());
//     edges_007.push_back(e_007_006.backIt());
//     edges_007.push_back(e_007_005.backIt());
//     n_007.reorderIncidence(std::move(edges_007));
// 
//     vector<edge_list_it_t> edges_008;
//     edges_008.push_back(e_011_008.backIt());
//     edges_008.push_back(e_009_008.backIt());
//     edges_008.push_back(e_008_006.backIt());
//     edges_008.push_back(e_008_007.backIt());
//     n_008.reorderIncidence(std::move(edges_008));
// 
//     vector<edge_list_it_t> edges_009;
//     edges_009.push_back(e_009_011.backIt());
//     edges_009.push_back(e_009_044.backIt());
//     edges_009.push_back(e_045_009.backIt());
//     edges_009.push_back(e_009_010.backIt());
//     edges_009.push_back(e_133_009.backIt());
//     edges_009.push_back(e_009_134.backIt());
//     edges_009.push_back(e_009_131.backIt());
//     edges_009.push_back(e_009_006.backIt());
//     edges_009.push_back(e_009_008.backIt());
//     n_009.reorderIncidence(std::move(edges_009));
// 
//     vector<edge_list_it_t> edges_010;
//     edges_010.push_back(e_010_100.backIt());
//     edges_010.push_back(e_010_099.backIt());
//     edges_010.push_back(e_009_010.backIt());
//     edges_010.push_back(e_045_010.backIt());
//     n_010.reorderIncidence(std::move(edges_010));
// 
//     vector<edge_list_it_t> edges_011;
//     edges_011.push_back(e_044_011.backIt());
//     edges_011.push_back(e_009_011.backIt());
//     edges_011.push_back(e_011_008.backIt());
//     edges_011.push_back(e_011_012.backIt());
//     n_011.reorderIncidence(std::move(edges_011));
// 
//     vector<edge_list_it_t> edges_012;
//     edges_012.push_back(e_012_013.backIt());
//     edges_012.push_back(e_041_012.backIt());
//     edges_012.push_back(e_011_012.backIt());
//     edges_012.push_back(e_012_007.backIt());
//     n_012.reorderIncidence(std::move(edges_012));
// 
//     vector<edge_list_it_t> edges_013;
//     edges_013.push_back(e_027_013.backIt());
//     edges_013.push_back(e_041_013.backIt());
//     edges_013.push_back(e_012_013.backIt());
//     edges_013.push_back(e_013_005.backIt());
//     edges_013.push_back(e_014_013.backIt());
//     edges_013.push_back(e_029_013.backIt());
//     edges_013.push_back(e_028_013.backIt());
//     edges_013.push_back(e_025_013.backIt());
//     n_013.reorderIncidence(std::move(edges_013));
// 
//     vector<edge_list_it_t> edges_014;
//     edges_014.push_back(e_014_034.backIt());
//     edges_014.push_back(e_014_013.backIt());
//     edges_014.push_back(e_005_014.backIt());
//     edges_014.push_back(e_014_136.backIt());
//     n_014.reorderIncidence(std::move(edges_014));
// 
//     vector<edge_list_it_t> edges_015;
//     edges_015.push_back(e_015_024.backIt());
//     edges_015.push_back(e_015_016.backIt());
//     edges_015.push_back(e_015_019.backIt());
//     edges_015.push_back(e_017_015.backIt());
//     edges_015.push_back(e_015_018.backIt());
//     edges_015.push_back(e_015_023.backIt());
//     n_015.reorderIncidence(std::move(edges_015));
// 
//     vector<edge_list_it_t> edges_016;
//     edges_016.push_back(e_019_016.backIt());
//     edges_016.push_back(e_015_016.backIt());
//     edges_016.push_back(e_016_024.backIt());
//     edges_016.push_back(e_016_049.backIt());
//     n_016.reorderIncidence(std::move(edges_016));
// 
//     vector<edge_list_it_t> edges_017;
//     edges_017.push_back(e_017_018.backIt());
//     edges_017.push_back(e_017_015.backIt());
//     edges_017.push_back(e_019_017.backIt());
//     edges_017.push_back(e_020_017.backIt());
//     n_017.reorderIncidence(std::move(edges_017));
// 
//     vector<edge_list_it_t> edges_018;
//     edges_018.push_back(e_015_018.backIt());
//     edges_018.push_back(e_017_018.backIt());
//     edges_018.push_back(e_021_018.backIt());
//     edges_018.push_back(e_018_027.backIt());
//     edges_018.push_back(e_023_018.backIt());
//     n_018.reorderIncidence(std::move(edges_018));
// 
//     vector<edge_list_it_t> edges_019;
//     edges_019.push_back(e_022_019.backIt());
//     edges_019.push_back(e_019_020.backIt());
//     edges_019.push_back(e_019_017.backIt());
//     edges_019.push_back(e_015_019.backIt());
//     edges_019.push_back(e_019_016.backIt());
//     edges_019.push_back(e_019_049.backIt());
//     n_019.reorderIncidence(std::move(edges_019));
// 
//     vector<edge_list_it_t> edges_020;
//     edges_020.push_back(e_022_020.backIt());
//     edges_020.push_back(e_020_021.backIt());
//     edges_020.push_back(e_020_017.backIt());
//     edges_020.push_back(e_019_020.backIt());
//     n_020.reorderIncidence(std::move(edges_020));
// 
//     vector<edge_list_it_t> edges_021;
//     edges_021.push_back(e_021_018.backIt());
//     edges_021.push_back(e_020_021.backIt());
//     edges_021.push_back(e_022_021.backIt());
//     edges_021.push_back(e_021_046.backIt());
//     n_021.reorderIncidence(std::move(edges_021));
// 
//     vector<edge_list_it_t> edges_022;
//     edges_022.push_back(e_022_021.backIt());
//     edges_022.push_back(e_022_020.backIt());
//     edges_022.push_back(e_022_019.backIt());
//     edges_022.push_back(e_022_049.backIt());
//     edges_022.push_back(e_022_048.backIt());
//     edges_022.push_back(e_022_047.backIt());
//     n_022.reorderIncidence(std::move(edges_022));
// 
//     vector<edge_list_it_t> edges_023;
//     edges_023.push_back(e_024_023.backIt());
//     edges_023.push_back(e_015_023.backIt());
//     edges_023.push_back(e_023_018.backIt());
//     edges_023.push_back(e_023_026.backIt());
//     n_023.reorderIncidence(std::move(edges_023));
// 
//     vector<edge_list_it_t> edges_024;
//     edges_024.push_back(e_024_039.backIt());
//     edges_024.push_back(e_040_024.backIt());
//     edges_024.push_back(e_048_024.backIt());
//     edges_024.push_back(e_024_049.backIt());
//     edges_024.push_back(e_016_024.backIt());
//     edges_024.push_back(e_015_024.backIt());
//     edges_024.push_back(e_024_023.backIt());
//     n_024.reorderIncidence(std::move(edges_024));
// 
//     vector<edge_list_it_t> edges_025;
//     edges_025.push_back(e_027_025.backIt());
//     edges_025.push_back(e_025_013.backIt());
//     edges_025.push_back(e_025_028.backIt());
//     edges_025.push_back(e_026_025.backIt());
//     n_025.reorderIncidence(std::move(edges_025));
// 
//     vector<edge_list_it_t> edges_026;
//     edges_026.push_back(e_039_026.backIt());
//     edges_026.push_back(e_023_026.backIt());
//     edges_026.push_back(e_026_025.backIt());
//     edges_026.push_back(e_026_028.backIt());
//     n_026.reorderIncidence(std::move(edges_026));
// 
//     vector<edge_list_it_t> edges_027;
//     edges_027.push_back(e_018_027.backIt());
//     edges_027.push_back(e_046_027.backIt());
//     edges_027.push_back(e_027_013.backIt());
//     edges_027.push_back(e_027_025.backIt());
//     n_027.reorderIncidence(std::move(edges_027));
// 
//     vector<edge_list_it_t> edges_028;
//     edges_028.push_back(e_028_130.backIt());
//     edges_028.push_back(e_028_037.backIt());
//     edges_028.push_back(e_028_038.backIt());
//     edges_028.push_back(e_026_028.backIt());
//     edges_028.push_back(e_025_028.backIt());
//     edges_028.push_back(e_028_013.backIt());
//     edges_028.push_back(e_028_029.backIt());
//     edges_028.push_back(e_028_035.backIt());
//     edges_028.push_back(e_028_002.backIt());
//     n_028.reorderIncidence(std::move(edges_028));
// 
//     vector<edge_list_it_t> edges_029;
//     edges_029.push_back(e_028_029.backIt());
//     edges_029.push_back(e_029_013.backIt());
//     edges_029.push_back(e_029_033.backIt());
//     edges_029.push_back(e_035_029.backIt());
//     n_029.reorderIncidence(std::move(edges_029));
// 
//     vector<edge_list_it_t> edges_030;
//     edges_030.push_back(e_032_030.backIt());
//     edges_030.push_back(e_030_006.backIt());
//     edges_030.push_back(e_030_031.backIt());
//     edges_030.push_back(e_002_030.backIt());
//     n_030.reorderIncidence(std::move(edges_030));
// 
//     vector<edge_list_it_t> edges_031;
//     edges_031.push_back(e_030_031.backIt());
//     edges_031.push_back(e_006_031.backIt());
//     edges_031.push_back(e_131_031.backIt());
//     edges_031.push_back(e_132_031.backIt());
//     edges_031.push_back(e_002_031.backIt());
//     n_031.reorderIncidence(std::move(edges_031));
// 
//     vector<edge_list_it_t> edges_032;
//     edges_032.push_back(e_035_032.backIt());
//     edges_032.push_back(e_033_032.backIt());
//     edges_032.push_back(e_032_030.backIt());
//     edges_032.push_back(e_032_002.backIt());
//     n_032.reorderIncidence(std::move(edges_032));
// 
//     vector<edge_list_it_t> edges_033;
//     edges_033.push_back(e_033_032.backIt());
//     edges_033.push_back(e_029_033.backIt());
//     edges_033.push_back(e_034_033.backIt());
//     edges_033.push_back(e_033_006.backIt());
//     n_033.reorderIncidence(std::move(edges_033));
// 
//     vector<edge_list_it_t> edges_034;
//     edges_034.push_back(e_034_033.backIt());
//     edges_034.push_back(e_014_034.backIt());
//     n_034.reorderIncidence(std::move(edges_034));
// 
//     vector<edge_list_it_t> edges_035;
//     edges_035.push_back(e_028_035.backIt());
//     edges_035.push_back(e_035_029.backIt());
//     edges_035.push_back(e_035_032.backIt());
//     edges_035.push_back(e_002_035.backIt());
//     n_035.reorderIncidence(std::move(edges_035));
// 
//     vector<edge_list_it_t> edges_036;
//     edges_036.push_back(e_036_126.backIt());
//     edges_036.push_back(e_037_036.backIt());
//     edges_036.push_back(e_130_036.backIt());
//     edges_036.push_back(e_036_002.backIt());
//     n_036.reorderIncidence(std::move(edges_036));
// 
//     vector<edge_list_it_t> edges_037;
//     edges_037.push_back(e_037_036.backIt());
//     edges_037.push_back(e_037_052.backIt());
//     edges_037.push_back(e_038_037.backIt());
//     edges_037.push_back(e_028_037.backIt());
//     n_037.reorderIncidence(std::move(edges_037));
// 
//     vector<edge_list_it_t> edges_038;
//     edges_038.push_back(e_038_037.backIt());
//     edges_038.push_back(e_038_051.backIt());
//     edges_038.push_back(e_039_038.backIt());
//     edges_038.push_back(e_028_038.backIt());
//     n_038.reorderIncidence(std::move(edges_038));
// 
//     vector<edge_list_it_t> edges_039;
//     edges_039.push_back(e_039_038.backIt());
//     edges_039.push_back(e_050_039.backIt());
//     edges_039.push_back(e_024_039.backIt());
//     edges_039.push_back(e_039_026.backIt());
//     n_039.reorderIncidence(std::move(edges_039));
// 
//     vector<edge_list_it_t> edges_040;
//     edges_040.push_back(e_040_048.backIt());
//     edges_040.push_back(e_040_024.backIt());
//     edges_040.push_back(e_050_040.backIt());
//     edges_040.push_back(e_047_040.backIt());
//     n_040.reorderIncidence(std::move(edges_040));
// 
//     vector<edge_list_it_t> edges_041;
//     edges_041.push_back(e_041_013.backIt());
//     edges_041.push_back(e_046_041.backIt());
//     edges_041.push_back(e_042_041.backIt());
//     edges_041.push_back(e_041_012.backIt());
//     n_041.reorderIncidence(std::move(edges_041));
// 
//     vector<edge_list_it_t> edges_042;
//     edges_042.push_back(e_042_041.backIt());
//     edges_042.push_back(e_042_043.backIt());
//     edges_042.push_back(e_045_042.backIt());
//     edges_042.push_back(e_044_042.backIt());
//     n_042.reorderIncidence(std::move(edges_042));
// 
//     vector<edge_list_it_t> edges_043;
//     edges_043.push_back(e_043_047.backIt());
//     edges_043.push_back(e_045_043.backIt());
//     edges_043.push_back(e_042_043.backIt());
//     edges_043.push_back(e_043_046.backIt());
//     n_043.reorderIncidence(std::move(edges_043));
// 
//     vector<edge_list_it_t> edges_044;
//     edges_044.push_back(e_044_042.backIt());
//     edges_044.push_back(e_045_044.backIt());
//     edges_044.push_back(e_009_044.backIt());
//     edges_044.push_back(e_044_011.backIt());
//     n_044.reorderIncidence(std::move(edges_044));
// 
//     vector<edge_list_it_t> edges_045;
//     edges_045.push_back(e_045_044.backIt());
//     edges_045.push_back(e_045_042.backIt());
//     edges_045.push_back(e_045_043.backIt());
//     edges_045.push_back(e_047_045.backIt());
//     edges_045.push_back(e_045_055.backIt());
//     edges_045.push_back(e_045_062.backIt());
//     edges_045.push_back(e_063_045.backIt());
//     edges_045.push_back(e_045_093.backIt());
//     edges_045.push_back(e_045_100.backIt());
//     edges_045.push_back(e_045_010.backIt());
//     edges_045.push_back(e_045_009.backIt());
//     n_045.reorderIncidence(std::move(edges_045));
// 
//     vector<edge_list_it_t> edges_046;
//     edges_046.push_back(e_021_046.backIt());
//     edges_046.push_back(e_043_046.backIt());
//     edges_046.push_back(e_046_041.backIt());
//     edges_046.push_back(e_046_027.backIt());
//     n_046.reorderIncidence(std::move(edges_046));
// 
//     vector<edge_list_it_t> edges_047;
//     edges_047.push_back(e_022_047.backIt());
//     edges_047.push_back(e_047_040.backIt());
//     edges_047.push_back(e_047_045.backIt());
//     edges_047.push_back(e_043_047.backIt());
//     n_047.reorderIncidence(std::move(edges_047));
// 
//     vector<edge_list_it_t> edges_048;
//     edges_048.push_back(e_022_048.backIt());
//     edges_048.push_back(e_048_049.backIt());
//     edges_048.push_back(e_048_024.backIt());
//     edges_048.push_back(e_040_048.backIt());
//     n_048.reorderIncidence(std::move(edges_048));
// 
//     vector<edge_list_it_t> edges_049;
//     edges_049.push_back(e_022_049.backIt());
//     edges_049.push_back(e_019_049.backIt());
//     edges_049.push_back(e_016_049.backIt());
//     edges_049.push_back(e_024_049.backIt());
//     edges_049.push_back(e_048_049.backIt());
//     n_049.reorderIncidence(std::move(edges_049));
// 
//     vector<edge_list_it_t> edges_050;
//     edges_050.push_back(e_050_040.backIt());
//     edges_050.push_back(e_050_039.backIt());
//     edges_050.push_back(e_051_050.backIt());
//     edges_050.push_back(e_055_050.backIt());
//     n_050.reorderIncidence(std::move(edges_050));
// 
//     vector<edge_list_it_t> edges_051;
//     edges_051.push_back(e_051_050.backIt());
//     edges_051.push_back(e_038_051.backIt());
//     edges_051.push_back(e_052_051.backIt());
//     edges_051.push_back(e_051_054.backIt());
//     n_051.reorderIncidence(std::move(edges_051));
// 
//     vector<edge_list_it_t> edges_052;
//     edges_052.push_back(e_052_056.backIt());
//     edges_052.push_back(e_054_052.backIt());
//     edges_052.push_back(e_052_051.backIt());
//     edges_052.push_back(e_037_052.backIt());
//     edges_052.push_back(e_128_052.backIt());
//     edges_052.push_back(e_052_127.backIt());
//     edges_052.push_back(e_052_125.backIt());
//     n_052.reorderIncidence(std::move(edges_052));
// 
//     vector<edge_list_it_t> edges_053;
//     edges_053.push_back(e_053_115.backIt());
//     edges_053.push_back(e_056_053.backIt());
//     edges_053.push_back(e_125_053.backIt());
//     edges_053.push_back(e_053_002.backIt());
//     n_053.reorderIncidence(std::move(edges_053));
// 
//     vector<edge_list_it_t> edges_054;
//     edges_054.push_back(e_051_054.backIt());
//     edges_054.push_back(e_054_052.backIt());
//     edges_054.push_back(e_054_056.backIt());
//     edges_054.push_back(e_055_054.backIt());
//     n_054.reorderIncidence(std::move(edges_054));
// 
//     vector<edge_list_it_t> edges_055;
//     edges_055.push_back(e_055_050.backIt());
//     edges_055.push_back(e_055_054.backIt());
//     edges_055.push_back(e_056_055.backIt());
//     edges_055.push_back(e_045_055.backIt());
//     n_055.reorderIncidence(std::move(edges_055));
// 
//     vector<edge_list_it_t> edges_056;
//     edges_056.push_back(e_056_053.backIt());
//     edges_056.push_back(e_073_056.backIt());
//     edges_056.push_back(e_071_056.backIt());
//     edges_056.push_back(e_124_056.backIt());
//     edges_056.push_back(e_056_060.backIt());
//     edges_056.push_back(e_056_061.backIt());
//     edges_056.push_back(e_062_056.backIt());
//     edges_056.push_back(e_056_055.backIt());
//     edges_056.push_back(e_054_056.backIt());
//     edges_056.push_back(e_052_056.backIt());
//     n_056.reorderIncidence(std::move(edges_056));
// 
//     vector<edge_list_it_t> edges_057;
//     edges_057.push_back(e_057_058.backIt());
//     edges_057.push_back(e_059_057.backIt());
//     n_057.reorderIncidence(std::move(edges_057));
// 
//     vector<edge_list_it_t> edges_058;
//     edges_058.push_back(e_124_058.backIt());
//     edges_058.push_back(e_070_058.backIt());
//     edges_058.push_back(e_058_066.backIt());
//     edges_058.push_back(e_059_058.backIt());
//     edges_058.push_back(e_057_058.backIt());
//     n_058.reorderIncidence(std::move(edges_058));
// 
//     vector<edge_list_it_t> edges_059;
//     edges_059.push_back(e_059_057.backIt());
//     edges_059.push_back(e_059_058.backIt());
//     edges_059.push_back(e_059_066.backIt());
//     edges_059.push_back(e_060_059.backIt());
//     n_059.reorderIncidence(std::move(edges_059));
// 
//     vector<edge_list_it_t> edges_060;
//     edges_060.push_back(e_056_060.backIt());
//     edges_060.push_back(e_060_059.backIt());
//     edges_060.push_back(e_060_065.backIt());
//     edges_060.push_back(e_061_060.backIt());
//     n_060.reorderIncidence(std::move(edges_060));
// 
//     vector<edge_list_it_t> edges_061;
//     edges_061.push_back(e_056_061.backIt());
//     edges_061.push_back(e_061_060.backIt());
//     edges_061.push_back(e_061_064.backIt());
//     edges_061.push_back(e_062_061.backIt());
//     n_061.reorderIncidence(std::move(edges_061));
// 
//     vector<edge_list_it_t> edges_062;
//     edges_062.push_back(e_062_056.backIt());
//     edges_062.push_back(e_062_061.backIt());
//     edges_062.push_back(e_063_062.backIt());
//     edges_062.push_back(e_045_062.backIt());
//     n_062.reorderIncidence(std::move(edges_062));
// 
//     vector<edge_list_it_t> edges_063;
//     edges_063.push_back(e_093_063.backIt());
//     edges_063.push_back(e_063_045.backIt());
//     edges_063.push_back(e_063_062.backIt());
//     edges_063.push_back(e_064_063.backIt());
//     n_063.reorderIncidence(std::move(edges_063));
// 
//     vector<edge_list_it_t> edges_064;
//     edges_064.push_back(e_064_092.backIt());
//     edges_064.push_back(e_064_063.backIt());
//     edges_064.push_back(e_061_064.backIt());
//     edges_064.push_back(e_065_064.backIt());
//     n_064.reorderIncidence(std::move(edges_064));
// 
//     vector<edge_list_it_t> edges_065;
//     edges_065.push_back(e_065_091.backIt());
//     edges_065.push_back(e_065_064.backIt());
//     edges_065.push_back(e_060_065.backIt());
//     edges_065.push_back(e_067_065.backIt());
//     n_065.reorderIncidence(std::move(edges_065));
// 
//     vector<edge_list_it_t> edges_066;
//     edges_066.push_back(e_058_066.backIt());
//     edges_066.push_back(e_070_066.backIt());
//     edges_066.push_back(e_066_068.backIt());
//     edges_066.push_back(e_059_066.backIt());
//     n_066.reorderIncidence(std::move(edges_066));
// 
//     vector<edge_list_it_t> edges_067;
//     edges_067.push_back(e_067_065.backIt());
//     edges_067.push_back(e_067_068.backIt());
//     edges_067.push_back(e_069_067.backIt());
//     edges_067.push_back(e_102_067.backIt());
//     n_067.reorderIncidence(std::move(edges_067));
// 
//     vector<edge_list_it_t> edges_068;
//     edges_068.push_back(e_069_068.backIt());
//     edges_068.push_back(e_067_068.backIt());
//     edges_068.push_back(e_066_068.backIt());
//     n_068.reorderIncidence(std::move(edges_068));
// 
//     vector<edge_list_it_t> edges_069;
//     edges_069.push_back(e_069_067.backIt());
//     edges_069.push_back(e_069_068.backIt());
//     edges_069.push_back(e_123_069.backIt());
//     edges_069.push_back(e_103_069.backIt());
//     n_069.reorderIncidence(std::move(edges_069));
// 
//     vector<edge_list_it_t> edges_070;
//     edges_070.push_back(e_123_070.backIt());
//     edges_070.push_back(e_070_066.backIt());
//     edges_070.push_back(e_070_058.backIt());
//     edges_070.push_back(e_072_070.backIt());
//     edges_070.push_back(e_070_115.backIt());
//     edges_070.push_back(e_117_070.backIt());
//     edges_070.push_back(e_118_070.backIt());
//     edges_070.push_back(e_120_070.backIt());
//     edges_070.push_back(e_081_070.backIt());
//     edges_070.push_back(e_122_070.backIt());
//     edges_070.push_back(e_070_103.backIt());
//     n_070.reorderIncidence(std::move(edges_070));
// 
//     vector<edge_list_it_t> edges_071;
//     edges_071.push_back(e_074_071.backIt());
//     edges_071.push_back(e_072_071.backIt());
//     edges_071.push_back(e_071_056.backIt());
//     edges_071.push_back(e_073_071.backIt());
//     n_071.reorderIncidence(std::move(edges_071));
// 
//     vector<edge_list_it_t> edges_072;
//     edges_072.push_back(e_072_070.backIt());
//     edges_072.push_back(e_072_071.backIt());
//     edges_072.push_back(e_074_072.backIt());
//     n_072.reorderIncidence(std::move(edges_072));
// 
//     vector<edge_list_it_t> edges_073;
//     edges_073.push_back(e_073_071.backIt());
//     edges_073.push_back(e_073_056.backIt());
//     n_073.reorderIncidence(std::move(edges_073));
// 
//     vector<edge_list_it_t> edges_074;
//     edges_074.push_back(e_074_072.backIt());
//     edges_074.push_back(e_074_071.backIt());
//     n_074.reorderIncidence(std::move(edges_074));
// 
//     vector<edge_list_it_t> edges_075;
//     edges_075.push_back(e_075_122.backIt());
//     edges_075.push_back(e_075_076.backIt());
//     edges_075.push_back(e_075_077.backIt());
//     n_075.reorderIncidence(std::move(edges_075));
// 
//     vector<edge_list_it_t> edges_076;
//     edges_076.push_back(e_075_076.backIt());
//     edges_076.push_back(e_078_076.backIt());
//     edges_076.push_back(e_077_076.backIt());
//     n_076.reorderIncidence(std::move(edges_076));
// 
//     vector<edge_list_it_t> edges_077;
//     edges_077.push_back(e_075_077.backIt());
//     edges_077.push_back(e_077_076.backIt());
//     edges_077.push_back(e_077_078.backIt());
//     edges_077.push_back(e_077_079.backIt());
//     edges_077.push_back(e_081_077.backIt());
//     edges_077.push_back(e_106_077.backIt());
//     n_077.reorderIncidence(std::move(edges_077));
// 
//     vector<edge_list_it_t> edges_078;
//     edges_078.push_back(e_078_076.backIt());
//     edges_078.push_back(e_079_078.backIt());
//     edges_078.push_back(e_080_078.backIt());
//     edges_078.push_back(e_077_078.backIt());
//     n_078.reorderIncidence(std::move(edges_078));
// 
//     vector<edge_list_it_t> edges_079;
//     edges_079.push_back(e_080_079.backIt());
//     edges_079.push_back(e_079_078.backIt());
//     edges_079.push_back(e_077_079.backIt());
//     n_079.reorderIncidence(std::move(edges_079));
// 
//     vector<edge_list_it_t> edges_080;
//     edges_080.push_back(e_080_078.backIt());
//     edges_080.push_back(e_080_079.backIt());
//     n_080.reorderIncidence(std::move(edges_080));
// 
//     vector<edge_list_it_t> edges_081;
//     edges_081.push_back(e_081_077.backIt());
//     edges_081.push_back(e_081_070.backIt());
//     edges_081.push_back(e_081_121.backIt());
//     edges_081.push_back(e_081_120.backIt());
//     edges_081.push_back(e_081_118.backIt());
//     edges_081.push_back(e_081_108.backIt());
//     edges_081.push_back(e_081_107.backIt());
//     edges_081.push_back(e_081_106.backIt());
//     n_081.reorderIncidence(std::move(edges_081));
// 
//     vector<edge_list_it_t> edges_082;
//     edges_082.push_back(e_082_087.backIt());
//     edges_082.push_back(e_082_118.backIt());
//     edges_082.push_back(e_082_119.backIt());
//     edges_082.push_back(e_082_112.backIt());
//     edges_082.push_back(e_082_083.backIt());
//     edges_082.push_back(e_082_084.backIt());
//     edges_082.push_back(e_086_082.backIt());
//     n_082.reorderIncidence(std::move(edges_082));
// 
//     vector<edge_list_it_t> edges_083;
//     edges_083.push_back(e_082_083.backIt());
//     edges_083.push_back(e_112_083.backIt());
//     edges_083.push_back(e_083_085.backIt());
//     edges_083.push_back(e_083_084.backIt());
//     n_083.reorderIncidence(std::move(edges_083));
// 
//     vector<edge_list_it_t> edges_084;
//     edges_084.push_back(e_082_084.backIt());
//     edges_084.push_back(e_083_084.backIt());
//     edges_084.push_back(e_085_084.backIt());
//     edges_084.push_back(e_084_111.backIt());
//     edges_084.push_back(e_109_084.backIt());
//     edges_084.push_back(e_086_084.backIt());
//     n_084.reorderIncidence(std::move(edges_084));
// 
//     vector<edge_list_it_t> edges_085;
//     edges_085.push_back(e_085_111.backIt());
//     edges_085.push_back(e_085_084.backIt());
//     edges_085.push_back(e_083_085.backIt());
//     edges_085.push_back(e_085_105.backIt());
//     edges_085.push_back(e_086_085.backIt());
//     n_085.reorderIncidence(std::move(edges_085));
// 
//     vector<edge_list_it_t> edges_086;
//     edges_086.push_back(e_086_108.backIt());
//     edges_086.push_back(e_086_087.backIt());
//     edges_086.push_back(e_086_082.backIt());
//     edges_086.push_back(e_086_084.backIt());
//     edges_086.push_back(e_086_110.backIt());
//     edges_086.push_back(e_086_109.backIt());
//     edges_086.push_back(e_086_085.backIt());
//     edges_086.push_back(e_086_088.backIt());
//     edges_086.push_back(e_107_086.backIt());
//     n_086.reorderIncidence(std::move(edges_086));
// 
//     vector<edge_list_it_t> edges_087;
//     edges_087.push_back(e_087_108.backIt());
//     edges_087.push_back(e_087_118.backIt());
//     edges_087.push_back(e_082_087.backIt());
//     edges_087.push_back(e_086_087.backIt());
//     n_087.reorderIncidence(std::move(edges_087));
// 
//     vector<edge_list_it_t> edges_088;
//     edges_088.push_back(e_104_088.backIt());
//     edges_088.push_back(e_086_088.backIt());
//     edges_088.push_back(e_088_102.backIt());
//     edges_088.push_back(e_088_105.backIt());
//     n_088.reorderIncidence(std::move(edges_088));
// 
//     vector<edge_list_it_t> edges_089;
//     edges_089.push_back(e_089_094.backIt());
//     edges_089.push_back(e_091_089.backIt());
//     edges_089.push_back(e_101_089.backIt());
//     edges_089.push_back(e_089_090.backIt());
//     n_089.reorderIncidence(std::move(edges_089));
// 
//     vector<edge_list_it_t> edges_090;
//     edges_090.push_back(e_089_090.backIt());
//     edges_090.push_back(e_101_090.backIt());
//     edges_090.push_back(e_105_090.backIt());
//     edges_090.push_back(e_090_113.backIt());
//     edges_090.push_back(e_090_140.backIt());
//     edges_090.push_back(e_139_090.backIt());
//     edges_090.push_back(e_138_090.backIt());
//     edges_090.push_back(e_000_090.backIt());
//     n_090.reorderIncidence(std::move(edges_090));
// 
//     vector<edge_list_it_t> edges_091;
//     edges_091.push_back(e_091_096.backIt());
//     edges_091.push_back(e_092_091.backIt());
//     edges_091.push_back(e_065_091.backIt());
//     edges_091.push_back(e_091_089.backIt());
//     n_091.reorderIncidence(std::move(edges_091));
// 
//     vector<edge_list_it_t> edges_092;
//     edges_092.push_back(e_092_091.backIt());
//     edges_092.push_back(e_092_098.backIt());
//     edges_092.push_back(e_093_092.backIt());
//     edges_092.push_back(e_064_092.backIt());
//     n_092.reorderIncidence(std::move(edges_092));
// 
//     vector<edge_list_it_t> edges_093;
//     edges_093.push_back(e_093_092.backIt());
//     edges_093.push_back(e_100_093.backIt());
//     edges_093.push_back(e_045_093.backIt());
//     edges_093.push_back(e_093_063.backIt());
//     n_093.reorderIncidence(std::move(edges_093));
// 
//     vector<edge_list_it_t> edges_094;
//     edges_094.push_back(e_094_095.backIt());
//     edges_094.push_back(e_096_094.backIt());
//     edges_094.push_back(e_089_094.backIt());
//     edges_094.push_back(e_094_000.backIt());
//     n_094.reorderIncidence(std::move(edges_094));
// 
//     vector<edge_list_it_t> edges_095;
//     edges_095.push_back(e_095_004.backIt());
//     edges_095.push_back(e_095_135.backIt());
//     edges_095.push_back(e_097_095.backIt());
//     edges_095.push_back(e_094_095.backIt());
//     n_095.reorderIncidence(std::move(edges_095));
// 
//     vector<edge_list_it_t> edges_096;
//     edges_096.push_back(e_096_094.backIt());
//     edges_096.push_back(e_096_097.backIt());
//     edges_096.push_back(e_098_096.backIt());
//     edges_096.push_back(e_091_096.backIt());
//     n_096.reorderIncidence(std::move(edges_096));
// 
//     vector<edge_list_it_t> edges_097;
//     edges_097.push_back(e_096_097.backIt());
//     edges_097.push_back(e_097_095.backIt());
//     edges_097.push_back(e_097_135.backIt());
//     edges_097.push_back(e_099_097.backIt());
//     n_097.reorderIncidence(std::move(edges_097));
// 
//     vector<edge_list_it_t> edges_098;
//     edges_098.push_back(e_100_098.backIt());
//     edges_098.push_back(e_092_098.backIt());
//     edges_098.push_back(e_098_096.backIt());
//     edges_098.push_back(e_098_099.backIt());
//     n_098.reorderIncidence(std::move(edges_098));
// 
//     vector<edge_list_it_t> edges_099;
//     edges_099.push_back(e_098_099.backIt());
//     edges_099.push_back(e_099_097.backIt());
//     edges_099.push_back(e_099_133.backIt());
//     edges_099.push_back(e_010_099.backIt());
//     n_099.reorderIncidence(std::move(edges_099));
// 
//     vector<edge_list_it_t> edges_100;
//     edges_100.push_back(e_100_093.backIt());
//     edges_100.push_back(e_100_098.backIt());
//     edges_100.push_back(e_010_100.backIt());
//     edges_100.push_back(e_045_100.backIt());
//     n_100.reorderIncidence(std::move(edges_100));
// 
//     vector<edge_list_it_t> edges_101;
//     edges_101.push_back(e_102_101.backIt());
//     edges_101.push_back(e_105_101.backIt());
//     edges_101.push_back(e_101_090.backIt());
//     edges_101.push_back(e_101_089.backIt());
//     n_101.reorderIncidence(std::move(edges_101));
// 
//     vector<edge_list_it_t> edges_102;
//     edges_102.push_back(e_102_067.backIt());
//     edges_102.push_back(e_103_102.backIt());
//     edges_102.push_back(e_088_102.backIt());
//     edges_102.push_back(e_102_101.backIt());
//     n_102.reorderIncidence(std::move(edges_102));
// 
//     vector<edge_list_it_t> edges_103;
//     edges_103.push_back(e_103_069.backIt());
//     edges_103.push_back(e_070_103.backIt());
//     edges_103.push_back(e_104_103.backIt());
//     edges_103.push_back(e_103_102.backIt());
//     n_103.reorderIncidence(std::move(edges_103));
// 
//     vector<edge_list_it_t> edges_104;
//     edges_104.push_back(e_106_104.backIt());
//     edges_104.push_back(e_107_104.backIt());
//     edges_104.push_back(e_104_088.backIt());
//     edges_104.push_back(e_104_103.backIt());
//     n_104.reorderIncidence(std::move(edges_104));
// 
//     vector<edge_list_it_t> edges_105;
//     edges_105.push_back(e_088_105.backIt());
//     edges_105.push_back(e_085_105.backIt());
//     edges_105.push_back(e_105_101.backIt());
//     edges_105.push_back(e_105_090.backIt());
//     n_105.reorderIncidence(std::move(edges_105));
// 
//     vector<edge_list_it_t> edges_106;
//     edges_106.push_back(e_106_077.backIt());
//     edges_106.push_back(e_081_106.backIt());
//     edges_106.push_back(e_106_104.backIt());
//     n_106.reorderIncidence(std::move(edges_106));
// 
//     vector<edge_list_it_t> edges_107;
//     edges_107.push_back(e_081_107.backIt());
//     edges_107.push_back(e_108_107.backIt());
//     edges_107.push_back(e_107_086.backIt());
//     edges_107.push_back(e_107_104.backIt());
//     n_107.reorderIncidence(std::move(edges_107));
// 
//     vector<edge_list_it_t> edges_108;
//     edges_108.push_back(e_081_108.backIt());
//     edges_108.push_back(e_118_108.backIt());
//     edges_108.push_back(e_087_108.backIt());
//     edges_108.push_back(e_086_108.backIt());
//     edges_108.push_back(e_108_107.backIt());
//     n_108.reorderIncidence(std::move(edges_108));
// 
//     vector<edge_list_it_t> edges_109;
//     edges_109.push_back(e_109_084.backIt());
//     edges_109.push_back(e_086_109.backIt());
//     edges_109.push_back(e_109_110.backIt());
//     n_109.reorderIncidence(std::move(edges_109));
// 
//     vector<edge_list_it_t> edges_110;
//     edges_110.push_back(e_109_110.backIt());
//     edges_110.push_back(e_086_110.backIt());
//     n_110.reorderIncidence(std::move(edges_110));
// 
//     vector<edge_list_it_t> edges_111;
//     edges_111.push_back(e_084_111.backIt());
//     edges_111.push_back(e_085_111.backIt());
//     n_111.reorderIncidence(std::move(edges_111));
// 
//     vector<edge_list_it_t> edges_112;
//     edges_112.push_back(e_082_112.backIt());
//     edges_112.push_back(e_114_112.backIt());
//     edges_112.push_back(e_112_113.backIt());
//     edges_112.push_back(e_112_083.backIt());
//     n_112.reorderIncidence(std::move(edges_112));
// 
//     vector<edge_list_it_t> edges_113;
//     edges_113.push_back(e_112_113.backIt());
//     edges_113.push_back(e_141_113.backIt());
//     edges_113.push_back(e_139_113.backIt());
//     edges_113.push_back(e_113_140.backIt());
//     edges_113.push_back(e_090_113.backIt());
//     n_113.reorderIncidence(std::move(edges_113));
// 
//     vector<edge_list_it_t> edges_114;
//     edges_114.push_back(e_114_119.backIt());
//     edges_114.push_back(e_116_114.backIt());
//     edges_114.push_back(e_003_114.backIt());
//     edges_114.push_back(e_114_112.backIt());
//     n_114.reorderIncidence(std::move(edges_114));
// 
//     vector<edge_list_it_t> edges_115;
//     edges_115.push_back(e_115_116.backIt());
//     edges_115.push_back(e_070_115.backIt());
//     edges_115.push_back(e_053_115.backIt());
//     edges_115.push_back(e_115_002.backIt());
//     n_115.reorderIncidence(std::move(edges_115));
// 
//     vector<edge_list_it_t> edges_116;
//     edges_116.push_back(e_116_114.backIt());
//     edges_116.push_back(e_117_116.backIt());
//     edges_116.push_back(e_115_116.backIt());
//     edges_116.push_back(e_116_002.backIt());
//     n_116.reorderIncidence(std::move(edges_116));
// 
//     vector<edge_list_it_t> edges_117;
//     edges_117.push_back(e_117_118.backIt());
//     edges_117.push_back(e_117_070.backIt());
//     edges_117.push_back(e_117_116.backIt());
//     n_117.reorderIncidence(std::move(edges_117));
// 
//     vector<edge_list_it_t> edges_118;
//     edges_118.push_back(e_118_119.backIt());
//     edges_118.push_back(e_082_118.backIt());
//     edges_118.push_back(e_087_118.backIt());
//     edges_118.push_back(e_118_108.backIt());
//     edges_118.push_back(e_081_118.backIt());
//     edges_118.push_back(e_120_118.backIt());
//     edges_118.push_back(e_118_070.backIt());
//     edges_118.push_back(e_117_118.backIt());
//     n_118.reorderIncidence(std::move(edges_118));
// 
//     vector<edge_list_it_t> edges_119;
//     edges_119.push_back(e_082_119.backIt());
//     edges_119.push_back(e_118_119.backIt());
//     edges_119.push_back(e_114_119.backIt());
//     n_119.reorderIncidence(std::move(edges_119));
// 
//     vector<edge_list_it_t> edges_120;
//     edges_120.push_back(e_081_120.backIt());
//     edges_120.push_back(e_120_121.backIt());
//     edges_120.push_back(e_120_070.backIt());
//     edges_120.push_back(e_120_118.backIt());
//     n_120.reorderIncidence(std::move(edges_120));
// 
//     vector<edge_list_it_t> edges_121;
//     edges_121.push_back(e_081_121.backIt());
//     edges_121.push_back(e_120_121.backIt());
//     n_121.reorderIncidence(std::move(edges_121));
// 
//     vector<edge_list_it_t> edges_122;
//     edges_122.push_back(e_122_070.backIt());
//     edges_122.push_back(e_075_122.backIt());
//     n_122.reorderIncidence(std::move(edges_122));
// 
//     vector<edge_list_it_t> edges_123;
//     edges_123.push_back(e_123_069.backIt());
//     edges_123.push_back(e_123_070.backIt());
//     n_123.reorderIncidence(std::move(edges_123));
// 
//     vector<edge_list_it_t> edges_124;
//     edges_124.push_back(e_124_056.backIt());
//     edges_124.push_back(e_124_058.backIt());
//     n_124.reorderIncidence(std::move(edges_124));
// 
//     vector<edge_list_it_t> edges_125;
//     edges_125.push_back(e_125_053.backIt());
//     edges_125.push_back(e_052_125.backIt());
//     edges_125.push_back(e_126_125.backIt());
//     edges_125.push_back(e_125_002.backIt());
//     n_125.reorderIncidence(std::move(edges_125));
// 
//     vector<edge_list_it_t> edges_126;
//     edges_126.push_back(e_126_125.backIt());
//     edges_126.push_back(e_129_126.backIt());
//     edges_126.push_back(e_036_126.backIt());
//     edges_126.push_back(e_126_002.backIt());
//     n_126.reorderIncidence(std::move(edges_126));
// 
//     vector<edge_list_it_t> edges_127;
//     edges_127.push_back(e_052_127.backIt());
//     edges_127.push_back(e_128_127.backIt());
//     edges_127.push_back(e_129_127.backIt());
//     n_127.reorderIncidence(std::move(edges_127));
// 
//     vector<edge_list_it_t> edges_128;
//     edges_128.push_back(e_128_052.backIt());
//     edges_128.push_back(e_129_128.backIt());
//     edges_128.push_back(e_128_127.backIt());
//     n_128.reorderIncidence(std::move(edges_128));
// 
//     vector<edge_list_it_t> edges_129;
//     edges_129.push_back(e_129_127.backIt());
//     edges_129.push_back(e_129_128.backIt());
//     edges_129.push_back(e_129_126.backIt());
//     n_129.reorderIncidence(std::move(edges_129));
// 
//     vector<edge_list_it_t> edges_130;
//     edges_130.push_back(e_130_036.backIt());
//     edges_130.push_back(e_028_130.backIt());
//     n_130.reorderIncidence(std::move(edges_130));
// 
//     vector<edge_list_it_t> edges_131;
//     edges_131.push_back(e_134_131.backIt());
//     edges_131.push_back(e_131_031.backIt());
//     edges_131.push_back(e_131_006.backIt());
//     edges_131.push_back(e_009_131.backIt());
//     n_131.reorderIncidence(std::move(edges_131));
// 
//     vector<edge_list_it_t> edges_132;
//     edges_132.push_back(e_135_132.backIt());
//     edges_132.push_back(e_132_002.backIt());
//     edges_132.push_back(e_132_031.backIt());
//     edges_132.push_back(e_134_132.backIt());
//     n_132.reorderIncidence(std::move(edges_132));
// 
//     vector<edge_list_it_t> edges_133;
//     edges_133.push_back(e_135_133.backIt());
//     edges_133.push_back(e_133_134.backIt());
//     edges_133.push_back(e_133_009.backIt());
//     edges_133.push_back(e_099_133.backIt());
//     n_133.reorderIncidence(std::move(edges_133));
// 
//     vector<edge_list_it_t> edges_134;
//     edges_134.push_back(e_134_132.backIt());
//     edges_134.push_back(e_134_131.backIt());
//     edges_134.push_back(e_009_134.backIt());
//     edges_134.push_back(e_133_134.backIt());
//     n_134.reorderIncidence(std::move(edges_134));
// 
//     vector<edge_list_it_t> edges_135;
//     edges_135.push_back(e_095_135.backIt());
//     edges_135.push_back(e_135_132.backIt());
//     edges_135.push_back(e_135_133.backIt());
//     edges_135.push_back(e_097_135.backIt());
//     n_135.reorderIncidence(std::move(edges_135));
// 
//     vector<edge_list_it_t> edges_136;
//     edges_136.push_back(e_014_136.backIt());
//     edges_136.push_back(e_136_006.backIt());
//     n_136.reorderIncidence(std::move(edges_136));
// 
//     vector<edge_list_it_t> edges_137;
//     edges_137.push_back(e_137_004.backIt());
//     edges_137.push_back(e_137_002.backIt());
//     n_137.reorderIncidence(std::move(edges_137));
// 
//     vector<edge_list_it_t> edges_138;
//     edges_138.push_back(e_138_001.backIt());
//     edges_138.push_back(e_138_090.backIt());
//     n_138.reorderIncidence(std::move(edges_138));
// 
//     vector<edge_list_it_t> edges_139;
//     edges_139.push_back(e_001_139.backIt());
//     edges_139.push_back(e_139_090.backIt());
//     edges_139.push_back(e_139_140.backIt());
//     edges_139.push_back(e_139_113.backIt());
//     edges_139.push_back(e_139_003.backIt());
//     n_139.reorderIncidence(std::move(edges_139));
// 
//     vector<edge_list_it_t> edges_140;
//     edges_140.push_back(e_139_140.backIt());
//     edges_140.push_back(e_090_140.backIt());
//     edges_140.push_back(e_113_140.backIt());
//     n_140.reorderIncidence(std::move(edges_140));
// 
//     vector<edge_list_it_t> edges_141;
//     edges_141.push_back(e_003_141.backIt());
//     edges_141.push_back(e_141_113.backIt());
//     n_141.reorderIncidence(std::move(edges_141));
// 
// 
//     vector<node_list_it_t> stOrder;
//     stOrder.push_back(n_004.backIt());
//     stOrder.push_back(n_137.backIt());
//     stOrder.push_back(n_095.backIt());
//     stOrder.push_back(n_135.backIt());
//     stOrder.push_back(n_132.backIt());
//     stOrder.push_back(n_134.backIt());
//     stOrder.push_back(n_131.backIt());
//     stOrder.push_back(n_133.backIt());
//     stOrder.push_back(n_094.backIt());
//     stOrder.push_back(n_096.backIt());
//     stOrder.push_back(n_091.backIt());
//     stOrder.push_back(n_092.backIt());
//     stOrder.push_back(n_093.backIt());
//     stOrder.push_back(n_063.backIt());
//     stOrder.push_back(n_064.backIt());
//     stOrder.push_back(n_065.backIt());
//     stOrder.push_back(n_067.backIt());
//     stOrder.push_back(n_069.backIt());
//     stOrder.push_back(n_123.backIt());
//     stOrder.push_back(n_070.backIt());
//     stOrder.push_back(n_122.backIt());
//     stOrder.push_back(n_075.backIt());
//     stOrder.push_back(n_076.backIt());
//     stOrder.push_back(n_078.backIt());
//     stOrder.push_back(n_080.backIt());
//     stOrder.push_back(n_079.backIt());
//     stOrder.push_back(n_077.backIt());
//     stOrder.push_back(n_081.backIt());
//     stOrder.push_back(n_108.backIt());
//     stOrder.push_back(n_087.backIt());
//     stOrder.push_back(n_082.backIt());
//     stOrder.push_back(n_119.backIt());
//     stOrder.push_back(n_114.backIt());
//     stOrder.push_back(n_112.backIt());
//     stOrder.push_back(n_083.backIt());
//     stOrder.push_back(n_084.backIt());
//     stOrder.push_back(n_111.backIt());
//     stOrder.push_back(n_085.backIt());
//     stOrder.push_back(n_109.backIt());
//     stOrder.push_back(n_110.backIt());
//     stOrder.push_back(n_086.backIt());
//     stOrder.push_back(n_107.backIt());
//     stOrder.push_back(n_106.backIt());
//     stOrder.push_back(n_104.backIt());
//     stOrder.push_back(n_088.backIt());
//     stOrder.push_back(n_105.backIt());
//     stOrder.push_back(n_103.backIt());
//     stOrder.push_back(n_102.backIt());
//     stOrder.push_back(n_101.backIt());
//     stOrder.push_back(n_089.backIt());
//     stOrder.push_back(n_121.backIt());
//     stOrder.push_back(n_120.backIt());
//     stOrder.push_back(n_118.backIt());
//     stOrder.push_back(n_117.backIt());
//     stOrder.push_back(n_116.backIt());
//     stOrder.push_back(n_115.backIt());
//     stOrder.push_back(n_053.backIt());
//     stOrder.push_back(n_072.backIt());
//     stOrder.push_back(n_074.backIt());
//     stOrder.push_back(n_071.backIt());
//     stOrder.push_back(n_073.backIt());
//     stOrder.push_back(n_056.backIt());
//     stOrder.push_back(n_052.backIt());
//     stOrder.push_back(n_125.backIt());
//     stOrder.push_back(n_128.backIt());
//     stOrder.push_back(n_127.backIt());
//     stOrder.push_back(n_129.backIt());
//     stOrder.push_back(n_126.backIt());
//     stOrder.push_back(n_036.backIt());
//     stOrder.push_back(n_037.backIt());
//     stOrder.push_back(n_038.backIt());
//     stOrder.push_back(n_039.backIt());
//     stOrder.push_back(n_024.backIt());
//     stOrder.push_back(n_015.backIt());
//     stOrder.push_back(n_018.backIt());
//     stOrder.push_back(n_021.backIt());
//     stOrder.push_back(n_022.backIt());
//     stOrder.push_back(n_017.backIt());
//     stOrder.push_back(n_020.backIt());
//     stOrder.push_back(n_019.backIt());
//     stOrder.push_back(n_016.backIt());
//     stOrder.push_back(n_049.backIt());
//     stOrder.push_back(n_048.backIt());
//     stOrder.push_back(n_040.backIt());
//     stOrder.push_back(n_047.backIt());
//     stOrder.push_back(n_043.backIt());
//     stOrder.push_back(n_046.backIt());
//     stOrder.push_back(n_027.backIt());
//     stOrder.push_back(n_013.backIt());
//     stOrder.push_back(n_130.backIt());
//     stOrder.push_back(n_023.backIt());
//     stOrder.push_back(n_025.backIt());
//     stOrder.push_back(n_026.backIt());
//     stOrder.push_back(n_028.backIt());
//     stOrder.push_back(n_029.backIt());
//     stOrder.push_back(n_035.backIt());
//     stOrder.push_back(n_032.backIt());
//     stOrder.push_back(n_030.backIt());
//     stOrder.push_back(n_033.backIt());
//     stOrder.push_back(n_034.backIt());
//     stOrder.push_back(n_041.backIt());
//     stOrder.push_back(n_042.backIt());
//     stOrder.push_back(n_044.backIt());
//     stOrder.push_back(n_011.backIt());
//     stOrder.push_back(n_012.backIt());
//     stOrder.push_back(n_050.backIt());
//     stOrder.push_back(n_051.backIt());
//     stOrder.push_back(n_054.backIt());
//     stOrder.push_back(n_055.backIt());
//     stOrder.push_back(n_124.backIt());
//     stOrder.push_back(n_058.backIt());
//     stOrder.push_back(n_057.backIt());
//     stOrder.push_back(n_068.backIt());
//     stOrder.push_back(n_066.backIt());
//     stOrder.push_back(n_059.backIt());
//     stOrder.push_back(n_060.backIt());
//     stOrder.push_back(n_061.backIt());
//     stOrder.push_back(n_062.backIt());
//     stOrder.push_back(n_100.backIt());
//     stOrder.push_back(n_098.backIt());
//     stOrder.push_back(n_097.backIt());
//     stOrder.push_back(n_099.backIt());
//     stOrder.push_back(n_010.backIt());
//     stOrder.push_back(n_045.backIt());
//     stOrder.push_back(n_009.backIt());
//     stOrder.push_back(n_008.backIt());
//     stOrder.push_back(n_007.backIt());
//     stOrder.push_back(n_005.backIt());
//     stOrder.push_back(n_014.backIt());
//     stOrder.push_back(n_136.backIt());
//     stOrder.push_back(n_006.backIt());
//     stOrder.push_back(n_031.backIt());
//     stOrder.push_back(n_002.backIt());
//     stOrder.push_back(n_001.backIt());
//     stOrder.push_back(n_138.backIt());
//     stOrder.push_back(n_139.backIt());
//     stOrder.push_back(n_140.backIt());
//     stOrder.push_back(n_003.backIt());
//     stOrder.push_back(n_141.backIt());
//     stOrder.push_back(n_113.backIt());
//     stOrder.push_back(n_090.backIt());
//     stOrder.push_back(n_000.backIt());
// 
//     BLPlanarityTester tester01;
//     auto res01 =  tester01.findEmbedding(g_1, stOrder);
//     if (res01) {
//         cerr << "The graph is planar. #1\n";
//     }
//     else{
//         cerr << "The graph is planar. #1\n";
//     }
// 
//     BLPlanarityTester tester02;
//     auto res02 =  tester02.isPlanar(g_1, stOrder);
//     if (res02) {
//         cerr << "The graph is planar. #2\n";
//     }
//     else{
//         cerr << "The graph is planar. #2\n";
//     }
// 
//     PlanarDualGraphMaker maker;
//     EmbeddedGraph emb;
//     DualGraph     dual;
//     maker.makeDualGraph(g_1, emb, dual);
// 
//     auto nPair = g_1.nodes();
//     for (auto nit = nPair.first; nit != nPair.second; nit++) {
//         auto& N = dynamic_cast<NumNode&>(*(*nit));
//         cerr << N.num() << " :";
//         for (auto iit  = N.incidentEdges().first;
//                   iit != N.incidentEdges().second; iit++) {
//             auto& E = *(*(*iit));
//             auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
//             cerr << " " << A.num();
// 
//         }
//         cerr << "\n";
//     }
// 
// }
// 



/**  @brief DFS for an empty graph.
 */
TEST_F(BLPlanarityTesterTestsForEmbedding, Test1) {

    RandomPlanarGraphGenerator gen;
//    gen.generateGraphs(20,20,0.4);
    gen.generateGraphs(6,6,0.4);
    cerr << "Number of graphs: " << gen.numGraphs() << "\n";
    for (size_t i = 0; i < gen.numGraphs(); i++ ) {
        if (gen.numVertices(i) > 4) {
            cerr << "Graph[" << i << "]\n";
            cerr << "    Number of nodes: " << gen.numVertices(i) << "\n";
            vector<pair<size_t,size_t> >& edgePairs = gen.edges(i);

            Graph g;
            vector<node_list_it_t> nodes;

            for (size_t j = 1; j <= gen.numVertices(i); j++) {
                auto& N = g.addNode(make_unique<NumNode>(j));
                nodes.push_back(N.backIt());
            }

            cerr << "    Number of edges: " << edgePairs.size() << "\n";
            for (auto& p : edgePairs) {
            cerr << "    {" << p.first << "," << p.second << "}\n";
                auto& N1 = dynamic_cast<NumNode&>(*(*(nodes[p.first-1])));
                auto& N2 = dynamic_cast<NumNode&>(*(*(nodes[p.second-1])));
                g.addEdge(make_unique<Edge>(), N1, N2);
            }
            cerr << "\n";


            vector<node_list_it_t> TList;
            for (auto tIt = g.nodes().first; tIt != g.nodes().second; tIt++){
                TList.push_back(tIt);
            }

            size_t success = 0;
            size_t failure = 0;
            size_t numTrials = 0;
            size_t maxNumTrials = 100000;
//            for (size_t i = 0; i < g.numNodes() && numTrials < maxNumTrials; i++ ) {
//                for (size_t j = 0; j < g.numNodes() && numTrials < maxNumTrials; j++ ) {
//                    if (i == j) {
//                        continue;
//                    }
                    
              for (auto eit = g.edges().first; eit != g.edges().second; eit++) {
                  auto& E = *(*eit);
                  auto& N1 = dynamic_cast<NumNode&>(E.incidentNode1());
                  auto& N2 = dynamic_cast<NumNode&>(E.incidentNode2());
                  auto i = N1.num();
                  auto j = N2.num();
                    cerr << "Testing [" << i << "][" << j << "]\n";
                    STNumbering st;
                    vector<node_list_it_t> st_list =
//                         st.getBipolarOrientation(g, **TList[i], **TList[j]);
                           st.getBipolarOrientation(g, N1, N2);
                    cerr << "ST-Ordering: ";
                    for (auto stit : st_list) {
                        auto& N = dynamic_cast<NumNode&>(*(*stit));
                        cerr << N.num() << " :";
                    }
                    cerr << "\n";

                    BLPlanarityTester tester;
                    size_t index = 0;

                    bool planar = tester.isPlanar(g, st_list);

                    if (!planar) {
                        cerr << "Non planar at iteration: " << index << "\n";
                        failure++;
                    }
                    else {
                        cerr << "Planar .\n";
                        success++;
                    }

                    BLPlanarityTester tester2;
                    auto res01 =  tester2.findEmbedding(g, st_list);
                    if (res01) {
                        cerr << "The graph is planar. #1\n";
                    }
                    else {
                        cerr << "The graph is planar. #1\n";
                    }

                    PlanarDualGraphMaker maker;
                    EmbeddedGraph emb;
                    DualGraph     dual;
                    maker.makeDualGraph(g, emb, dual);
 
                    auto nPair = g.nodes();
                    for (auto nit = nPair.first; nit != nPair.second; nit++) {
                        auto& N = dynamic_cast<NumNode&>(*(*nit));
                        cerr << N.num() << " :";
                        for (auto iit  = N.incidentEdges().first;
                                  iit != N.incidentEdges().second; iit++) {
                            auto& E = *(*(*iit));
                            auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
                            cerr << " " << A.num();
 
                        }
                        cerr << "\n";
                    }
                    numTrials++;
                }
//            }
        }
    }

}


} // namespace Undirected

} // namespace Wailea

