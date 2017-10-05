#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/spqr_proc1.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqr_proc1.cpp
 *
 * @brief triconnected decomposition procedure 1.
 *     
 * @remarks  this corresponds to "Step 1" in  Hopcroft, John; Tarjan, Robert
 *           (1973), "Dividing a graph into  triconnected components', 
 *           SIAM Journal on Computing 2 (3): 135-158, doi:10.1137/0202012
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

void SPQRStrategyProc1::enterNode(DNode& v) noexcept
{
    mI++;
    v.mNum    = mI;
    v.mLowPt1 = mI;
    v.mLowPt2 = mI;
    v.mNd     = 1;

    if (v.mNum == 1) {
        v.mParentEdge = v.graph().edges().second;
    }
}


void SPQRStrategyProc1::processFrond(DNode& v,DEdge& vw) noexcept
{
    auto& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    vw.mType   = SPQR_DFS_Edge::FrondType;

    if (w.mNum < v.mLowPt1) {

        v.mLowPt2 = v.mLowPt1;
        v.mLowPt1 = w.mNum;

    }
    else if (w.mNum > v.mLowPt1) {

        v.mLowPt2 = min(v.mLowPt2, w.mNum);

    }
}


void SPQRStrategyProc1::beforeVisitingChild(DNode& v, DEdge& vw) noexcept
{
    vw.mType = SPQR_DFS_Edge::TreeArcType;
}


void SPQRStrategyProc1::afterVisitingChild(DNode& v, DEdge& vw) noexcept
{

    auto& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    if (w.mLowPt1 < v.mLowPt1) {

        v.mLowPt2 = min(v.mLowPt1,w.mLowPt2);
        v.mLowPt1 = w.mLowPt1;

    }
    else if (w.mLowPt1 == v.mLowPt1) {

        v.mLowPt2 = min(v.mLowPt2, w.mLowPt2);

    }
    else { 

        v.mLowPt2 = min (v.mLowPt2, w.mLowPt1);

    }

    (v.mNd) += (w.mNd);

    w.mParentEdge = vw.backIt();
}


}// namespace Undirected

}// namespace Wailea

