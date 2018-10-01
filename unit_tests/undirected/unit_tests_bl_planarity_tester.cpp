#include "gtest/gtest.h"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/bl_tree.hpp"
#include "undirected/bl_tree_node.hpp"
#include "undirected/bl_inline.hpp"
#include "undirected/st_numbering.hpp"

namespace Wailea {

namespace Undirected {

class BLPlanarityTesterTests : public ::testing::Test {

  protected:

    BLPlanarityTesterTests(){;};
    virtual ~BLPlanarityTesterTests(){;};
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


#include "unit_tests/undirected/unit_tests_bl_planarity_tester_bubbleup.inc"

#include "unit_tests/undirected/unit_tests_bl_planarity_tester_templateP.inc"

#include "unit_tests/undirected/unit_tests_bl_planarity_tester_templateQ.inc"

//#include "unit_tests/undirected/unit_tests_bl_planarity_tester_collectEdgeOrdering.inc"

#include "unit_tests/undirected/unit_tests_bl_planarity_tester_removePertinentTree.inc"

#include "unit_tests/undirected/unit_tests_bl_planarity_tester_isPlanar.inc"

//#include "unit_tests/undirected/unit_tests_bl_planarity_tester_findEmbedding.inc"

} // namespace Undirected

} // namespace Wailea

