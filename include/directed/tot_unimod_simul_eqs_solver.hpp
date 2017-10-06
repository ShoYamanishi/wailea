#ifndef _WAILEA_DIRECTED_TOT_UNIMOD_SIMUL_EQS_SOLVER_HPP_
#define _WAILEA_DIRECTED_TOT_UNIMOD_SIMUL_EQS_SOLVER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file directed/tot_unimod_simul_eqs_solver.hpp
 *
 * @brief 
 */

namespace Wailea {

namespace Directed {

using namespace std;


/**
 *  @brief Simultaneous equations solver whose coefficient matrix is 
 *         represented by a regular totally unimodular matrix.
 *         It solves the problem by diagonalizing the matrix by Gaussian
 *         elimination.
 *         Since it is assumed to be totally unimodular, the coefficients
 *         are any of -1, 0, or 1, and operations do not involve multiplication
 *         or division.
 */     
class TotUnimodSimulEqsSolver {

  public:

    using base_t = char;

    TotUnimodSimulEqsSolver(size_t size);

    virtual ~TotUnimodSimulEqsSolver(){;}

    inline void setCoefficient(size_t i, size_t j, long val) {
        if (val > 1)  val =  1;
        if (val < -1) val = -1;
        mMatrix[i][j] = val;
    }

    inline void setRHS(size_t i, long val) { mRHS[i] = val; }
       
    void solve();

    inline long getSolution(size_t j) {return mRHS[j];}

  private:
    size_t                       mSize;
    vector<vector<base_t> >      mMatrix;
    vector<long>                 mRHS;

};


}// namespace Directed

}// namespace Wailea

#endif /*_WAILEA_DIRECTED_TOT_UNIMOD_SIMUL_EQS_SOLVER_HPP_*/
