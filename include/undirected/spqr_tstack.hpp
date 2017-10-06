#ifndef _WAILEA_UNDIRECTED_SPQR_TSTACK_HPP_
#define _WAILEA_UNDIRECTED_SPQR_TSTACK_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"
#include "undirected/spqr_dfs_elements.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqr_tstack.hpp
 *
 * @brief TSTACK implemenation used in spqr_proc3.
 *     
 * @remarks  this corresponds to TSTACK used in "PathSearch(v)" in 
 *           Gutwenger, Carsten; Mutzel Petra (2001), "A linear time 
 *           implementation of SPQR trees", Proc. 8th International Symposium
 *           on Graph Drawing (GD 2000), Lecture Nodes in Computer Science 
 *           1984, Springer-Verlag, pp. 77-90, doi:10.1007/3-540-44541-2_8   
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

/** @class TStack
 *
 *  @brief 
 *
 */ 


class TStack {

  public:

    inline TStack() noexcept;
    inline virtual ~TStack() noexcept;

    inline void push(size_t h, size_t a, size_t b);
    inline void pop() noexcept ;
    inline void pushEOS();
    inline void updateForTreeArc(size_t newH, size_t newA, size_t newB);
    inline void updateForFrond(size_t v, size_t w) ;
    inline void popUntilHigherH(size_t v, size_t high_v) noexcept;
    inline void popUntilAndIncludingEOS() noexcept;
    inline bool isEmpty() const noexcept;
    inline size_t h() const noexcept;
    inline size_t a() const noexcept;
    inline size_t b() const noexcept;

  private:

    vector<size_t> mA;
    vector<size_t> mB;
    vector<size_t> mH;

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif
        
};


inline TStack::TStack() noexcept {;}


inline TStack::~TStack() noexcept {;}


inline bool TStack::isEmpty() const noexcept {

    return a() == 0;

}

inline void TStack::push(size_t h, size_t a, size_t b){
    mH.push_back(h);
    mA.push_back(a);
    mB.push_back(b);

}


inline void TStack::pop() noexcept {
    if(mH.size()==0){
        return;
    }
    mH.pop_back();
    mA.pop_back();
    mB.pop_back();

}


inline void TStack::pushEOS(){
    mH.push_back(0);
    mA.push_back(0);
    mB.push_back(0);

}


inline void TStack::updateForTreeArc(size_t newH, size_t newA, size_t newB) {
    size_t lastB = newB;        
    size_t maxH  = newH;

    while ( a() > newA ) {

        lastB = b();
        maxH  = max(maxH, h());

        mA.pop_back();
        mB.pop_back();
        mH.pop_back();

    }
    mH.push_back(maxH);
    mA.push_back(newA);
    mB.push_back(lastB);

    mH.push_back(0);
    mA.push_back(0);
    mB.push_back(0);
}


inline void TStack::updateForFrond(size_t v, size_t w) {

    size_t lastB = v;
    size_t maxH  = v;
    while ( a() > w ) {

        lastB = b();
        maxH  = max(maxH, h());

        mA.pop_back();
        mB.pop_back();
        mH.pop_back();

    }

    mH.push_back(maxH);
    mA.push_back(w);
    mB.push_back(lastB);

}


inline void TStack::popUntilHigherH(size_t v, size_t high_v) noexcept {

    while ( a() != 0 && a() != v && b() != v && h() < high_v) {

        mA.pop_back();
        mB.pop_back();
        mH.pop_back();

    }
}


inline void TStack::popUntilAndIncludingEOS() noexcept {

    while (mA.size() > 0) {

        if (a() == 0){
            break;
        }

        mA.pop_back();
        mB.pop_back();
        mH.pop_back();

    }

    if (mA.size() > 0) {

        mA.pop_back();
        mB.pop_back();
        mH.pop_back();

    }

}


inline size_t TStack::h() const noexcept {

    if (mH.size()>0) {
        return *(mH.rbegin());
    }
    else {
        return 0;
    }

}


inline size_t TStack::a() const noexcept {

    if (mA.size()>0) {
        return *(mA.rbegin());
    }
    else {
        return 0;
    }

}


inline size_t TStack::b() const noexcept {

    if (mB.size()>0) {
        return *(mB.rbegin());
    }
    else {
        return 0;
    }

}

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQR_TSTACK_HPP_*/
