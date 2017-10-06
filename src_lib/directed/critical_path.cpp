#include "directed/critical_path.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file directed/critical_path.cpp
 *
 * @brief 
 * 
 * @details
 *    - It allows multiple sink and source nodes.
 *    - It allows edges to have delay value which can be used
 *      as the minimum delay after the source node's task finishied
 *      and before the destination node starts.
 */

namespace Wailea {

namespace Directed { 

using namespace std;

using namespace Wailea::Undirected;


void CriticalPath::findCriticalPaths(DiGraph& D) {

    findTerminals(D);

    resetForForwardPath1(D);

    long latestStart = performForwardPath(D);

    resetForBackwardPath(D, latestStart);

    performBackwardPath(D);

    resetForForwardPath2(D);

    performForwardPath(D);

}


void CriticalPath::findTerminals(DiGraph& D) {
    for (auto nit = D.nodes().first; nit != D.nodes().second; nit++) {
        auto& N = dynamic_cast<CPDiNode&>(*(*nit));
        if (N.degreeIn() == 0) {
             mSources.push_back(nit);
        }
        if (N.degreeOut() == 0) {
             mDestinations.push_back(nit);
        }
    }
}


void CriticalPath::resetForForwardPath1(DiGraph& D) {
    for (auto nit = D.nodes().first; nit != D.nodes().second; nit++) {
        auto& N = dynamic_cast<CPDiNode&>(*(*nit));
        N.mNumNeighborsProcessed = 0;
        N.mEarliestStart = 0;
    }
}


void CriticalPath::resetForForwardPath2(DiGraph& D) {
    for (auto nit = D.nodes().first; nit != D.nodes().second; nit++) {
        auto& N = dynamic_cast<CPDiNode&>(*(*nit));
        N.mNumNeighborsProcessed = 0;
        if (N.degreeIn() > 0) {
            N.mEarliestStart = 0;
        } 
        else {
            N.mEarliestStart = N.mLatestStart;
        }
    }
}


void CriticalPath::resetForBackwardPath(DiGraph& D, long latestStart) {
    for (auto nit = D.nodes().first; nit != D.nodes().second; nit++) {
        auto& N = dynamic_cast<CPDiNode&>(*(*nit));
        N.mNumNeighborsProcessed = 0;
        if (N.degreeOut() > 0) {
            N.mLatestStart = latestStart;
        } 
        else {
            N.mLatestStart = N.mEarliestStart;
        }
    }
}


long CriticalPath::performForwardPath(DiGraph& D) {

    list<node_list_it_t> Q(mSources.begin(), mSources.end());
    long maxEarliestStart = 0;
    while (Q.size() > 0) {    
        auto nit = *(Q.rbegin());
        Q.pop_back();
        auto& N = dynamic_cast<CPDiNode&>(*(*nit));
        for (auto eit = N.incidentEdgesOut().first; 
                                   eit != N.incidentEdgesOut().second; eit++) {
            auto& E = dynamic_cast<CPDiEdge&>(*(*(*eit)));
            auto& A = dynamic_cast<CPDiNode&>(E.adjacentNode(N));
            long sum = N.mEarliestStart + N.mLength + E.mLength;
            if (A.mEarliestStart < sum) {
                A.mEarliestStart = sum;
            }
            if (maxEarliestStart < sum) {
                maxEarliestStart = sum;
            }
            A.mNumNeighborsProcessed++;
            if (A.mNumNeighborsProcessed == A.degreeIn()) {
                Q.push_front(A.backIt());
            }
        }
    }
    return maxEarliestStart;
}


void CriticalPath::performBackwardPath(DiGraph& D) {

    list<node_list_it_t> Q(mDestinations.begin(), mDestinations.end());
    while (Q.size() > 0) {    
        auto nit = *(Q.rbegin());
        Q.pop_back();
        auto& N = dynamic_cast<CPDiNode&>(*(*nit));
        for (auto eit = N.incidentEdgesIn().first; 
                                   eit != N.incidentEdgesIn().second; eit++) {
            auto& E = dynamic_cast<CPDiEdge&>(*(*(*eit)));
            auto& A = dynamic_cast<CPDiNode&>(E.adjacentNode(N));
            long sum = N.mLatestStart - (A.mLength + E.mLength);
            if (A.mLatestStart > sum) {
                A.mLatestStart = sum;
            }
            A.mNumNeighborsProcessed++;
            if (A.mNumNeighborsProcessed == A.degreeOut()) {
                Q.push_front(A.backIt());
            }
        }
    }
}

}// namespace Directed

}// namespace Wailea
