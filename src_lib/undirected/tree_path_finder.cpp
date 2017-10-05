#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/tree_path_finder.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/st_numbering.cpp
 *
 * @brief finding st-numbers of a simple graph between
 *        two particular nodes s and t. The numbers are implicitly given as
 *        a node list of bipolar orientation between s and t.
 *        If the given graph has {s,t}, it must be biconnected.
 *        If the given graph G does not have {s,t}, then G + {s,t} must be
 *        biconnected.  
 *
 */
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @class TreePathFinderStrategy
 *
 *  @brief
 */
class TreePathFinderStrategy : 
                         public DFSStrategy<Graph, TPF_DFS_Node, TPF_DFS_Edge> 
{

  public:

    inline TreePathFinderStrategy(Graph& tree):mTree(tree){;}
    inline virtual ~TreePathFinderStrategy(){;}
    void setEndNode(DNode& n);
    list<edge_list_it_t>&& getEdges();
    list<node_list_it_t>&& getNodes();


    void enterNode(DNode& n) noexcept override;
    void beforeVisitingChild(DNode& n, DEdge& c) noexcept override;

  private:
    Graph&               mTree;
    node_list_it_t       mEnd;
    list<edge_list_it_t> mEdges;
    list<node_list_it_t> mNodes;

#ifdef UNIT_TESTS
  friend class TreePathFinderTests;
#endif

};


void TreePathFinderStrategy::setEndNode(DNode& n)
{
    mEnd = n.backIt();
}


void TreePathFinderStrategy::enterNode(DNode& v) noexcept
{

    if (mEnd==v.backIt()) {

        mNodes.push_front(v.backIt());

        // Reached the other end node.
        auto nit = v.backIt();
        auto eit = v.mBackEdge;
        while (eit !=mTree.edges().second) {

            mEdges.push_front(eit);
            auto& N     = dynamic_cast<DNode&>(*(*nit));
            auto& E     = dynamic_cast<DEdge&>(*(*eit));
            auto& Nprev = dynamic_cast<DNode&>(E.adjacentNode(N));

            eit = Nprev.mBackEdge;
            nit = Nprev.backIt();
            if (nit != mTree.nodes().second) {
                mNodes.push_front(nit);
            }

        }

        setEarlyOut();

    }
}


void TreePathFinderStrategy::beforeVisitingChild(DNode& v, DEdge& vw) noexcept
{
    auto& W = dynamic_cast<TPF_DFS_Node&>(vw.adjacentNode(v));
    W.mBackEdge = vw.backIt();
}


list<edge_list_it_t>&& TreePathFinderStrategy::getEdges()
{
    return std::move(mEdges);
}


list<node_list_it_t>&& TreePathFinderStrategy::getNodes()
{
    return std::move(mNodes);
}


void TreePathFinder::copyInputGraph(
    Graph&          Gorg,
    node_list_it_t  Sorg,
    node_list_it_t  Torg,

    Graph&          Gcopy, 
    node_list_it_t& Scopy,
    node_list_it_t& Tcopy

) {


    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    auto nitPair = Gorg.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto np = make_unique<DNode>();
	np->pushIGBackwardLink(nit);

        nodePairs.push_back(make_pair(nit,std::move(np)));
    }
    auto eitPair = Gorg.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto ep = make_unique<DEdge>();
	ep->pushIGBackwardLink(eit);
	edgePairs.push_back(make_pair(eit,std::move(ep)));
    }

    Gorg.copySubgraph(nodePairs, edgePairs, Gcopy);

    for (auto nit = Gcopy.nodes().first; nit != Gcopy.nodes().second; nit++) {
        auto& N = dynamic_cast<DNode&>(*(*nit));

        N.mBackEdge = Gcopy.edges().second;
                                                                    
        if (N.IGBackwardLink() == Sorg) {
            Scopy = nit;
        }

        if (N.IGBackwardLink() == Torg) {
            Tcopy = nit;
        }
    }

}

void TreePathFinder::findPath(

        Graph&                T,
        Node&                 s,
        Node&                 t,
        list<node_list_it_t>& nodes,
        list<edge_list_it_t>& edges

) {

    DFSGraph<Graph,TPF_DFS_Node,TPF_DFS_Edge> Tcopy;
    node_list_it_t n1It;
    node_list_it_t n2It;

    copyInputGraph(T, s.backIt(), t.backIt(), Tcopy, n1It, n2It);

    auto& N1copy = dynamic_cast<DNode&>(*(*n1It));
    auto& N2copy = dynamic_cast<DNode&>(*(*n2It));

    TreePathFinderStrategy strategy(Tcopy);
    DFSExplorer<Graph,TPF_DFS_Node,TPF_DFS_Edge> explorer(Tcopy, strategy);
    strategy.setEndNode(N2copy);
    explorer.reset(Tcopy);
    explorer.run(N1copy);

    list<node_list_it_t> nodesCopy = strategy.getNodes();
    nodes.clear();
    for (auto nit : nodesCopy) {

        auto& N = dynamic_cast<DNode&>(*(*nit));
        nodes.push_back(N.IGBackwardLink());

    }

    list<edge_list_it_t> edgesCopy = strategy.getEdges();
    edges.clear();
    for (auto eit : edgesCopy) {

        auto& E = dynamic_cast<DEdge&>(*(*eit));
        edges.push_back(E.IGBackwardLink());

    }

    return;
}

}// namespace Undirected

}// namespace Wailea
