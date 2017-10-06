#include "gtest/gtest.h"
#include "directed/tot_unimod_simul_eqs_solver.hpp"

namespace Wailea {

namespace Directed {

 
class TotUnimodSimulEqsSolverTests : public ::testing::Test {
 
  protected:

    TotUnimodSimulEqsSolverTests(){;}
    virtual ~TotUnimodSimulEqsSolverTests(){;}

    virtual void SetUp() {;};
    virtual void TearDown() {;};

};
 

TEST_F(TotUnimodSimulEqsSolverTests, Test1) {

// 1 -1  0  0 -1  x1  3
//-1  1 -1  0  0  x2  5
// 0 -1  1 -1  0  x3  7
// 0  0 -1  1 -1  x4  11
//-1  0  0 -1  1  x5  13

    TotUnimodSimulEqsSolver solver(5);
    solver.setCoefficient(0, 0,  1);
    solver.setCoefficient(0, 1, -1);
    solver.setCoefficient(0, 4, -1);
    solver.setCoefficient(1, 0, -1);
    solver.setCoefficient(1, 1,  1);
    solver.setCoefficient(1, 2, -1);
    solver.setCoefficient(2, 1, -1);
    solver.setCoefficient(2, 2,  1);
    solver.setCoefficient(2, 3, -1);
    solver.setCoefficient(3, 2, -1);
    solver.setCoefficient(3, 3,  1);
    solver.setCoefficient(3, 4, -1);
    solver.setCoefficient(4, 0, -1);
    solver.setCoefficient(4, 3, -1);
    solver.setCoefficient(4, 4,  1);

    solver.setRHS(0,  3);
    solver.setRHS(1,  5);
    solver.setRHS(2,  7);
    solver.setRHS(3, 11);
    solver.setRHS(4, 13);

    solver.solve();

    EXPECT_EQ(solver.getSolution(0), -15);
    EXPECT_EQ(solver.getSolution(1), -19);
    EXPECT_EQ(solver.getSolution(2),  -9);
    EXPECT_EQ(solver.getSolution(3),   3);
    EXPECT_EQ(solver.getSolution(4),   1);

}


TEST_F(TotUnimodSimulEqsSolverTests, Test2) {

// 0  1  0  0  0  x1  17
// 0  1  1  1  1  x2  19
// 1  0  1  1  1  x3  23
// 1  0  0  1  0  x4  29
// 1  0  0  0  1  x5  31

    TotUnimodSimulEqsSolver solver(5);
    solver.setCoefficient(0, 1,  1);
    solver.setCoefficient(1, 1,  1);
    solver.setCoefficient(1, 2,  1);
    solver.setCoefficient(1, 3,  1);
    solver.setCoefficient(1, 4,  1);
    solver.setCoefficient(2, 0,  1);
    solver.setCoefficient(2, 2,  1);
    solver.setCoefficient(2, 3,  1);
    solver.setCoefficient(2, 4,  1);
    solver.setCoefficient(3, 0,  1);
    solver.setCoefficient(3, 3,  1);
    solver.setCoefficient(4, 0,  1);
    solver.setCoefficient(4, 4,  1);


    solver.setRHS(0, 17);
    solver.setRHS(1, 19);
    solver.setRHS(2, 23);
    solver.setRHS(3, 29);
    solver.setRHS(4, 31);

    solver.solve();

    EXPECT_EQ(solver.getSolution(0),  21);
    EXPECT_EQ(solver.getSolution(1),  17);
    EXPECT_EQ(solver.getSolution(2), -16);
    EXPECT_EQ(solver.getSolution(3),   8);
    EXPECT_EQ(solver.getSolution(4),  10);

}


TEST_F(TotUnimodSimulEqsSolverTests, Test3) {

//  1 -1 -1  0   x1  31
// -1  0  0  1   x2  37
//  0  1  0 -1   x3  41
//  0  0  1 -1   x4  43

    TotUnimodSimulEqsSolver solver(4);
    solver.setCoefficient(0, 0,  1);
    solver.setCoefficient(0, 1, -1);
    solver.setCoefficient(0, 2, -1);
    solver.setCoefficient(1, 0, -1);
    solver.setCoefficient(1, 3,  1);
    solver.setCoefficient(2, 1,  1);
    solver.setCoefficient(2, 3, -1);
    solver.setCoefficient(3, 2,  1);
    solver.setCoefficient(3, 3, -1);


    solver.setRHS(0, 31);
    solver.setRHS(1, 37);
    solver.setRHS(2, 41);
    solver.setRHS(3, 43);

    solver.solve();

    EXPECT_EQ(solver.getSolution(0), -189);
    EXPECT_EQ(solver.getSolution(1), -111);
    EXPECT_EQ(solver.getSolution(2), -109);
    EXPECT_EQ(solver.getSolution(3), -152);

}

} // namespace Directed

} // namespace Wailea
