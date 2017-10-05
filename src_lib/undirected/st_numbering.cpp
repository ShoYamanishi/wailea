#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/st_numbering.hpp"

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


/** @class STStrategyProc1
 *
 *  @brief the first DFS in which the palm-tree is formed and
 *         the pre, and low numbers  are calculated for each node.
 */
class STStrategyProc1 : public DFSStrategy<Graph, ST_DFS_Node, ST_DFS_Edge> {

  public:
    STStrategyProc1():mI(0){;}
    inline virtual ~STStrategyProc1(){;}

    // The following 4 functions are the hook points to DFSExplorer
    void enterNode(DNode& n) noexcept override;
    void processFrond(DNode& n, DEdge& c) noexcept override;
    void beforeVisitingChild(DNode& n, DEdge& c) noexcept override;
    void afterVisitingChild(DNode& n, DEdge& c) noexcept override;

  private:

    /**  @brief the last number assigned to the node.
    */
    size_t  mI;

#ifdef UNIT_TESTS
  friend class STNumberingTests;
#endif

};


void STStrategyProc1::enterNode(DNode& v) noexcept
{
    mI++;
    v.mPre = mI;
    v.mLow = mI;
    

    if (v.mPre == 1) {
        v.mParentEdge = v.graph().edges().second;
    }
}


void STStrategyProc1::processFrond(DNode& v,DEdge& vw) noexcept
{
    auto& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    vw.mType   = ST_DFS_Edge::FrondType;

    if (w.mPre < v.mLow) {
        v.mLow = w.mPre;
    }
}


void STStrategyProc1::beforeVisitingChild(DNode& v, DEdge& vw) noexcept
{
    vw.mType = ST_DFS_Edge::TreeArcType;
}


void STStrategyProc1::afterVisitingChild(DNode& v, DEdge& vw) noexcept
{

    auto& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    if (w.mLow < v.mLow) {
        v.mLow = w.mLow;
    }

    w.mParentEdge = vw.backIt();
}


/** @class STStrategyProc2
 *
 *  @brief the second DFS in which the bipolar orientation list L is formed.
 */
class STStrategyProc2 : public DFSStrategy<Graph, ST_DFS_Node, ST_DFS_Edge> {
                    

  public:
    STStrategyProc2(){;}
    inline virtual ~STStrategyProc2(){;}

    void prepareNodesAndEdges(Graph &g, DNode& s, DNode&t);

    // The following function is the hook points to DFSExplorer
    void enterNode(DNode& n) noexcept override;

    vector<node_list_it_t> getL() const;

  private:

    /** @brief create a map from mPre to the iterator to its node.
     *
     *  @param g (in): The input graph that has ST_DFS_Nodes with mPre set.
     */
    void createNodeMap(Graph &g);


    /** @brief mapping from pre number to the node iterator.
     */
    vector<node_list_it_t> mNMap;


    /**  @brief the bipolar orientation list.
     */
    list<node_list_it_t> mL;


    /**  @brief the source node
     */
    node_list_it_t       mNodeS;


    /**  @brief the destination node
     */
    node_list_it_t       mNodeT;


#ifdef UNIT_TESTS
  friend class STNumberingTests;
#endif

};


void STStrategyProc2::prepareNodesAndEdges(Graph& g, DNode& s, DNode& t)
{
    mNodeS = s.backIt();
    mNodeT = t.backIt();

    s.mSignPlus = false;
    s.mItForL = mL.insert(mL.end(),mNodeS);
    t.mItForL = mL.insert(mL.end(),mNodeT);

    createNodeMap(g);
}


void STStrategyProc2::enterNode(DNode& v) noexcept
{
    if (v.backIt()==mNodeS || v.backIt()==mNodeT) {
        return;
    }

    auto& pLow    = dynamic_cast<DNode&>(*(*mNMap[v.mLow]));
    auto& vParent = dynamic_cast<DNode&>((*(v.mParentEdge))->adjacentNode(v));

    if (pLow.mSignPlus) {

        auto insersionPoint = vParent.mItForL;
        insersionPoint++;
        v.mItForL = mL.insert(insersionPoint, v.backIt());
        vParent.mSignPlus = false;

    }
    else {

        v.mItForL = mL.insert(vParent.mItForL, v.backIt());
        vParent.mSignPlus = true;

    }
}


void STStrategyProc2::createNodeMap(Graph &g)
{
    size_t numNodes = g.numNodes();
    mNMap.reserve(numNodes+1);
    node_list_it_t blankIt;
    for (size_t i = 0; i < numNodes + 1; i++) {
        mNMap.push_back(blankIt);
    }

    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {
        ST_DFS_Node& n = dynamic_cast<ST_DFS_Node&>(*(*nit));
        mNMap[n.mPre] = n.backIt();
    }
}


vector<node_list_it_t> STStrategyProc2::getL() const
{
    vector<node_list_it_t> vecReturned;
    for (auto it = mL.begin(); it != mL.end(); it++) {
        auto& n = dynamic_cast<DNode&>(*(*(*it)));
        vecReturned.push_back(n.mOriginal);
    }
    return vecReturned; // rvo
}


void STNumbering::copyInputGraph(
    Graph& src, Graph& dst,
    node_list_it_t srcIt, node_list_it_t& srcCopyIt,
    node_list_it_t dstIt, node_list_it_t& dstCopyIt )
{
    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    auto nitPair = src.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto np = make_unique<DNode>();
	np->mOriginal = nit;
        nodePairs.push_back(make_pair(nit,std::move(np)));
    }
    auto eitPair = src.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto ep = make_unique<DEdge>();
	ep->mOriginal = eit;
	edgePairs.push_back(make_pair(eit,std::move(ep)));
    }
    src.copySubgraph(nodePairs, edgePairs, dst);

    for (auto nit = dst.nodes().first; nit != dst.nodes().second; nit++) {
        auto& n = dynamic_cast<DNode&>(*(*nit));
                                                                    
        if (n.mOriginal == srcIt) {
            srcCopyIt = nit;
        }
        if (n.mOriginal == dstIt) {
            dstCopyIt = nit;
        }
    }

    bool edgeSTFound = false;
    DNode& srcCopy = dynamic_cast<DNode&>(*(*srcCopyIt));
    DNode& dstCopy = dynamic_cast<DNode&>(*(*dstCopyIt));
    for (auto iit = srcCopy.incidentEdges().first; 
                               iit != srcCopy.incidentEdges().second; iit++) {
        auto& e = dynamic_cast<DEdge&>(*(*(*iit)));
        auto& vAdj = dynamic_cast<DNode&>(e.adjacentNode(srcCopy));
        if (&vAdj == &dstCopy) {
            // Move the edge {s,t} to the front of the incidence lists.
            dst.moveEdge(e, srcCopy, srcCopy.incidentEdges().first,
                            dstCopy, dstCopy.incidentEdges().first );
            edgeSTFound = true;
            break;

        }
    }
    if (edgeSTFound == false) {
        // Add {s,t} to the front of the incidence lists of s and t.
        auto ep = make_unique<DEdge>();
	ep->mOriginal = dst.edges().second;
        dst.addEdge(std::move(ep), srcCopy, dstCopy,
                srcCopy.incidentEdges().first, dstCopy.incidentEdges().first );
    }
}


vector<node_list_it_t> STNumbering::getBipolarOrientation(
                                                  Graph& g, Node& s, Node& t)
{
    DFSGraph<Graph,ST_DFS_Node,ST_DFS_Edge> gCopy;
    node_list_it_t srcCopyIt;
    node_list_it_t dstCopyIt;

    copyInputGraph(g, gCopy, s.backIt(), srcCopyIt, t.backIt(), dstCopyIt);

    auto& srcCopy = dynamic_cast<DNode&>(*(*srcCopyIt));
    auto& dstCopy = dynamic_cast<DNode&>(*(*dstCopyIt));

    STStrategyProc1 strategy1;
    DFSExplorer<Graph,ST_DFS_Node,ST_DFS_Edge> mExplorer1(gCopy,strategy1);
    mExplorer1.reset(gCopy);
    mExplorer1.run(srcCopy);

    STStrategyProc2 strategy2;
    DFSExplorer<Graph,ST_DFS_Node,ST_DFS_Edge> mExplorer2(gCopy,strategy2);
    strategy2.prepareNodesAndEdges(gCopy, srcCopy, dstCopy);
    mExplorer2.reset(gCopy);
    mExplorer2.run(srcCopy);

    return strategy2.getL();

}

}// namespace Undirected

}// namespace Wailea
