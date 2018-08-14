#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/spqr_decomposer.hpp"


/**
 * @file undirected/spqrtree.cpp
 *
 * @brief triconnected decomposition and SPQR-tree.
 *     
 * @reference:
 * [1] Gutwenger, Carsten; Mutzel Petra (2001), "A linear time implementation
 *     of SPQR trees", Proc. 8th International Symposium on Graph Drawing
 *     (GD 2000), Lecture Nodes in Computer Science 1984, Springer-Verlag,
 *     pp. 77-90, doi:10.1007/3-540-44541-2_8
 *
 * [2] Hopcroft, John; Tarjan, Robert (1973), "Dividing a graph into 
 *     triconnected components', SIAM Journal on Computing 2 (3): 135-158,
 *     doi:10.1137/0202012
 */
namespace Wailea {

namespace Undirected { 

using namespace std;


SPQRTree SPQRDecomposer::decompose(Graph& g, Node& s)
{
    DFSGraph<Graph,SPQR_DFS_Node,SPQR_DFS_Edge> gCopy;
    auto sCopy = copyInputGraph(g, gCopy, s.backIt());

    {
        SPQRStrategyProc1 strategy1;
        DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge> 
                                                 mExplorer1(gCopy,strategy1);
        mExplorer1.reset(gCopy);
        mExplorer1.run(dynamic_cast<
                             DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                  (*(*sCopy)));
    }
    sortAdjacency(gCopy);
    {
        SPQRStrategyProc2 strategy2;
        DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge> 
                                                 mExplorer2(gCopy,strategy2);
        strategy2.prepareNodesForDFS(gCopy);
        mExplorer2.reset(gCopy);
        mExplorer2.run(dynamic_cast<
                         DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>
                                                                  (*(*sCopy)));
    }
    // Find the initial separation components and form a tree.

    SPQRTree     spqrTree(g);
    {

        SPQRStrategyProc3 strategy3(gCopy, spqrTree);

        DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge> 
                                                 mExplorer3(gCopy,strategy3);

        strategy3.prepareNodesAndEdges(gCopy);

        mExplorer3.reset(gCopy);

        mExplorer3.run(dynamic_cast<
                        DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                  (*(*sCopy)));

    }

    spqrTree.determineTypes();


    // Merging adjacent S and P nodes.
    {
        SPQRStrategyProc4 strategy4(spqrTree);
        DFSGraph<Graph, SPQR_PROC4_Node, SPQR_PROC4_Edge> tCopy;
        copySPQRTree(spqrTree, tCopy);
        auto nit =  tCopy.nodes().first;
        DFSExplorer<Graph, SPQR_PROC4_Node, SPQR_PROC4_Edge> 
                                                 mExplorer4(tCopy, strategy4);
        mExplorer4.reset(tCopy);
        mExplorer4.run(dynamic_cast<
                  DFSNode<Graph, SPQR_PROC4_Node, SPQR_PROC4_Edge>&>(*(*nit)));
    }
    // Making forward links for the edges in the original graph.
    {
        auto tnPair = spqrTree.nodes();
        for (auto tnit = tnPair.first; tnit != tnPair.second; tnit++) {
            auto& TN = dynamic_cast<SPQRTreeNode&>(*(*tnit));
            auto& C  = TN.component();
            utility_t edgeType = (TN.type()==SPQRTreeNode::PType)?
                                     SPQRTree::kForwardEdgeTypeSeparatingEdge:
                                     SPQRTree::kForwardEdgeTypeOrdinaryEdge;
            auto cePair = C.edges();
            for (auto ceit = cePair.first; ceit != cePair.second; ceit++) {
                auto& CE = dynamic_cast<SPQRComponentEdge&>(*(*ceit));
                if (CE.type() == SPQRComponentEdge::OrdinaryType) {
                    CE.IGBackwardLinkRef().pushIGForwardLink(ceit);
                    CE.IGBackwardLinkRef().pushUtility(edgeType);
                }
            }
        }
    }
    return spqrTree; // rvo
}


node_list_it_t SPQRDecomposer::copyInputGraph(
    Graph&         src,
    Graph&         dst,
    node_list_it_t startNode
){
    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;
    Node* startNodeCopyGraph;
    auto nitPair = src.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto np = make_unique<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
        np->pushIGBackwardLink(nit);
        if (nit == startNode) {
            startNodeCopyGraph = &(*np);
        }
        nodePairs.push_back(make_pair(nit,std::move(np)));
    }
    auto eitPair = src.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto ep = make_unique<DFSEdge<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
        ep->pushIGBackwardLink(eit);
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }
    src.copySubgraph(nodePairs, edgePairs, dst);
    return startNodeCopyGraph->backIt();
}


void SPQRDecomposer::calculatePhi(SPQR_DFS_Edge&e)
{
    SPQR_DFS_Node& n1 = dynamic_cast<SPQR_DFS_Node&>(e.incidentNode1());
    SPQR_DFS_Node& n2 = dynamic_cast<SPQR_DFS_Node&>(e.incidentNode2());
    if (e.mType == SPQR_DFS_Edge::TreeArcType) {
        if (n1.mNum < n2.mNum) {
            // n1->n2
            if (n2.mLowPt2 < n1.mNum) {
                e.mPhi = 3 * n2.mLowPt1;
            }
            else {
                e.mPhi = 3 * n2.mLowPt1 + 2;
            }
        }
        else {
            // n2->n1
            if (n1.mLowPt2 < n2.mNum) {
                e.mPhi = 3 * n1.mLowPt1;
            }
            else {
                e.mPhi = 3 * n1.mLowPt1 + 2;
            }
        }
    }
    else { // SPQR_DFS_Edge::FrondType
        if (n1.mNum < n2.mNum) {
            // n2[_/>n1
            e.mPhi = 3 * n1.mNum + 1;
        }
        else {
            // n1[_/>n2
            e.mPhi = 3 * n2.mNum + 1;
        }
    }
}


void SPQRDecomposer::sortAdjacency(Graph& g)
{
    vector<vector<edge_list_it_t>> bucket;
    bucket.reserve(g.numNodes()*3+1);
    for (size_t i = 0; i < g.numNodes() * 3 + 1; i++) {
        vector<edge_list_it_t> e;
        bucket.push_back(e);
    }
    for (auto eit = g.edges().first; eit != g.edges().second; eit++) {
        SPQR_DFS_Edge& e = dynamic_cast<SPQR_DFS_Edge&>(*(*eit));
        calculatePhi(e);
        bucket[e.mPhi].push_back(e.backIt());
    }
    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {
        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*nit));
        n.mOrderedAdjacencyType1.clear();
        n.mOrderedAdjacencyType1Back.clear();
        n.mOrderedAdjacencyType2.clear();
        n.mOrderedAdjacencyType2Back.clear();
    }
    for (auto& innerVec : bucket) {
        for (auto eit : innerVec) {
            SPQR_DFS_Edge& e  = dynamic_cast<SPQR_DFS_Edge&>(*(*eit));
            SPQR_DFS_Node& n1 = dynamic_cast<SPQR_DFS_Node&>(
                                                            e.incidentNode1());
            SPQR_DFS_Node& n2 = dynamic_cast<SPQR_DFS_Node&>(
                                                            e.incidentNode2());
            if (e.mType == SPQR_DFS_Edge::TreeArcType) {
                n1.mOrderedAdjacencyType1.push_back(e.backIt());
                n2.mOrderedAdjacencyType1.push_back(e.backIt());
                n1.mOrderedAdjacencyType2.push_back(e.backIt());
                n2.mOrderedAdjacencyType2.push_back(e.backIt());
            }
            else { // e.type() == SPQR_DFS_Node::FrondType
                n1.mOrderedAdjacencyType1Back.push_back(e.backIt());
                n2.mOrderedAdjacencyType1Back.push_back(e.backIt());
                if (n1.mNum < n2.mNum) {
                    n1.mOrderedAdjacencyType2Back.push_back(e.backIt());
                    n2.mOrderedAdjacencyType2.push_back(e.backIt());
                }
                else { // n1.mNum > n2.mNum
                    n2.mOrderedAdjacencyType2Back.push_back(e.backIt());
                    n1.mOrderedAdjacencyType2.push_back(e.backIt());
                }
            }
        }
    }
    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {
        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*nit));
        n.mOrderedAdjacencyType1.insert(
                                   std::end(n.mOrderedAdjacencyType1), 
                                   std::begin(n.mOrderedAdjacencyType1Back),
                                   std::end(n.mOrderedAdjacencyType1Back)   );
        n.mOrderedAdjacencyType2.insert(
                                   std::end(n.mOrderedAdjacencyType2), 
                                   std::begin(n.mOrderedAdjacencyType2Back),
                                   std::end(n.mOrderedAdjacencyType2Back)   );
        n.mOrderedAdjacencyType1Back.clear();
        n.mOrderedAdjacencyType2Back.clear();
    }
}


SPQR_PROC4_Node::type SPQRDecomposer::convertNodeType(node_list_it_t nit)
{
    SPQRTreeNode& tn = dynamic_cast<SPQRTreeNode&>(*(*nit));
    switch (tn.type()) {
      case SPQRTreeNode::RType:
          return SPQR_PROC4_Node::RType;

      case SPQRTreeNode::SType:
          return SPQR_PROC4_Node::SType;

      case SPQRTreeNode::PType:
          return SPQR_PROC4_Node::PType;

      default:
          return SPQR_PROC4_Node::Unknown;

    }
    return SPQR_PROC4_Node::Unknown;
}


void SPQRDecomposer::copySPQRTree(SPQRTree& tree, Graph& copiedGraph)
{
    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;
    auto nitPair = tree.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto np = make_unique<DFSNode<
                                  Graph, SPQR_PROC4_Node, SPQR_PROC4_Edge>>();
        np->pushIGBackwardLink(nit);
        np->mType = convertNodeType(nit);
        nodePairs.push_back(make_pair(nit,std::move(np)));
    }
    auto eitPair = tree.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto ep = make_unique<DFSEdge<
                                  Graph, SPQR_PROC4_Node, SPQR_PROC4_Edge>>();
        ep->pushIGBackwardLink(eit);
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }
    tree.copySubgraph(nodePairs, edgePairs, copiedGraph);
}


}// namespace Undirected

}// namespace Wailea
