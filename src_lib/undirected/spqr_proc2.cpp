#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/spqr_proc2.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file undirected/spqr_proc2.cpp
 *
 * @brief triconnected decomposition procedure 2.
 *     
 * @remarks  this corresponds to "Step 3" in  Hopcroft, John; Tarjan, Robert
 *           (1973), "Dividing a graph into  triconnected components', 
 *           SIAM Journal on Computing 2 (3): 135-158, doi:10.1137/0202012
 */
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @brief initialize the nodes by reordering the incidence lists
 *         and clearing the high point data memeters.
 *         It also sets mV to the number of nodes in the graph.
 */
void SPQRStrategyProc2::prepareNodesForDFS(Graph &g) noexcept
{
    auto itPairs = g.nodes();
    for (auto it = itPairs.first; it != itPairs.second; it++) {

        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*it));
        n.reorderIncidence(move(n.mOrderedAdjacencyType1));
        n.mOrderedAdjacencyType1.clear();
	n.mHighPoints.clear();
	n.mHighPointReferences.clear();
	n.mHighPointIterators.clear();

    }

    mV = g.numNodes();

}


void SPQRStrategyProc2::enterNode(DNode& v) noexcept
{

    v.mNum = mV - v.mNd + 1;
    v.mLowPt1 = v.mNum;
    v.mLowPt2 = v.mNum;

}


void SPQRStrategyProc2::processFrond(DNode& v,DEdge& vw)
{
    auto& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    auto it = w.mHighPoints.insert(w.mHighPoints.end(),v.backIt());
    v.mHighPointIterators.push_back(it);
    v.mHighPointReferences.push_back(w.mNum);

    if (w.mNum < v.mLowPt1) {

        v.mLowPt2 = v.mLowPt1;
        v.mLowPt1 = w.mNum;

    }
    else if (w.mNum > v.mLowPt1) {

        v.mLowPt2 = min(v.mLowPt2, w.mNum);

    }
}


void SPQRStrategyProc2::afterVisitingChild(DNode& v, DEdge& vw) noexcept
{
    auto& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    mV--;
    if (w.mLowPt1 < v.mLowPt1) {

        v.mLowPt2 = min(v.mLowPt1, w.mLowPt2);
        v.mLowPt1 = w.mLowPt1;

    }
    else if (w.mLowPt1 == v.mLowPt1) {

        v.mLowPt2 = min(v.mLowPt2, w.mLowPt2);

    }
    else {

        v.mLowPt2 = min(v.mLowPt2, w.mLowPt1);

    }
}

}// namespace Undirected

}// namespace Wailea

