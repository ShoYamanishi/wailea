#include "directed/tot_unimod_simul_eqs_solver.hpp"

/**
 * @file directed/tot_unimod_simul_eqs_solver.cpp
 *
 * @brief 
 */

namespace Wailea {

namespace Directed {

using namespace std;

TotUnimodSimulEqsSolver::TotUnimodSimulEqsSolver(size_t size) {

    mSize = size;

    for (size_t i = 0; i < size; i++) {
        vector<base_t> row;
        for (size_t j = 0; j < size; j++) {
            row.push_back(0);
        }
        mMatrix.push_back(std::move(row));
        mRHS.push_back(0);
   }
}


void TotUnimodSimulEqsSolver::solve() {

    vector<pair<size_t,size_t> > swapHistory;

    for (size_t i = 0; i < mSize - 1; i++) {

        // Swap two columns so that [i][i] becomes non-zero.
        size_t colStart;
        for (colStart = i; colStart < mSize; colStart++) {
            if (mMatrix[i][colStart] != 0){
                break;
            }
        }
        if (i != colStart) {
            for (auto i2 = 0; i2 < mSize; i2++) {
                std::swap(mMatrix[i2][i], mMatrix[i2][colStart]);
            }
            swapHistory.push_back(make_pair(i, colStart));
        }

        // Make the matrix upper diagonal.
        for (auto i2 = i+1; i2 < mSize; i2++) {
            if (mMatrix[i2][i] != 0) {
                if (mMatrix[i][i]==mMatrix[i2][i]) {
                    for (auto k = i; k < mSize; k++) {
                        mMatrix[i2][k] -= mMatrix[i][k];
                    }
                    mRHS[i2] -= mRHS[i];
                }
                else {
                    for (auto k = i; k < mSize; k++) {
                        mMatrix[i2][k] += mMatrix[i][k];
                    }
                    mRHS[i2] += mRHS[i];
                }
            }
        }

    }

    // Make the matrix diagonal.
    for (size_t i = mSize - 1; i > 0; i--) {
        for (auto  i2 = 0 ; i2 < i; i2++) {
            if (mMatrix[i2][i] !=0) {
                if (mMatrix[i][i] == mMatrix[i2][i]) {
                    mRHS[i2] -= mRHS[i];
                }
                else {
                    mRHS[i2] += mRHS[i];
                }
            }
        }
    }

    // Make the matrix identity
    for (size_t i = 0 ; i < mSize; i++) {
        if (mMatrix[i][i] < 0) {
            mRHS[i] = -1 * mRHS[i];
        }
    }

    // Apply the swappings.
    for (auto rit = swapHistory.rbegin(); rit != swapHistory.rend(); rit++) {
        auto p = *rit;
        std::swap(mRHS[p.first], mRHS[p.second]);
    }

}

}// namespace Directed

}// namespace Wailea

