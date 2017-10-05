#include "gtest/gtest.h"
#include "undirected/gmw_bc_path.hpp"

namespace Wailea {

namespace Undirected {

class GMWBCPathTests : public ::testing::Test {

  protected:

    GMWBCPathTests(){;};
    virtual ~GMWBCPathTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief tests minimal : single edge
 */
TEST_F(GMWBCPathTests, Test1) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_01.backIt(), n_02.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_BOTH_IN_THE_SAME_BLOCK);

}


/**  @brief tests  *n1 - *n2 - n3
 */
TEST_F(GMWBCPathTests, Test2) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_01.backIt(), n_02.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_CUT_VERTEX);

}


/**  @brief tests  n1 - *n2 - *n3
 */
TEST_F(GMWBCPathTests, Test3) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_02.backIt(), n_03.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_CUT_VERTEX_N2_IS_BLOCK);

}


/**  @brief tests *n1 - n2 - *n3
 */
TEST_F(GMWBCPathTests, Test4) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_01.backIt(), n_03.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK);

}


/**  @brief tests triangle
 */
TEST_F(GMWBCPathTests, Test5) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_01.backIt(), n_03.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_BOTH_IN_THE_SAME_BLOCK);

}


/**  @brief tests triangle - triangle - triangle - triangle
 */
TEST_F(GMWBCPathTests, Test6) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);

    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    /*auto& e_05_03 = */g_1.addEdge(make_unique<Edge>(), n_05, n_03);

    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(), n_05, n_06);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_07_05 = */g_1.addEdge(make_unique<Edge>(), n_07, n_05);

    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(), n_08, n_09);
    /*auto& e_09_07 = */g_1.addEdge(make_unique<Edge>(), n_09, n_07);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_03.backIt(), n_07.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_BOTH_CUT_VERTICES);

}


/**  @brief tests triangle - triangle - triangle - triangle
 */
TEST_F(GMWBCPathTests, Test7) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);

    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    /*auto& e_05_03 = */g_1.addEdge(make_unique<Edge>(), n_05, n_03);

    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(), n_05, n_06);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_07_05 = */g_1.addEdge(make_unique<Edge>(), n_07, n_05);

    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(), n_08, n_09);
    /*auto& e_09_07 = */g_1.addEdge(make_unique<Edge>(), n_09, n_07);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_01.backIt(), n_07.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_CUT_VERTEX);

}


/**  @brief tests triangle - triangle - triangle - triangle
 */
TEST_F(GMWBCPathTests, Test8) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);

    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    /*auto& e_05_03 = */g_1.addEdge(make_unique<Edge>(), n_05, n_03);

    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(), n_05, n_06);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_07_05 = */g_1.addEdge(make_unique<Edge>(), n_07, n_05);

    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(), n_08, n_09);
    /*auto& e_09_07 = */g_1.addEdge(make_unique<Edge>(), n_09, n_07);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_02.backIt(), n_08.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK);

}


/**  @brief tests 
 */
TEST_F(GMWBCPathTests, Test9) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(10));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(21));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    /*auto& e_06_05 = */g_1.addEdge(make_unique<Edge>(), n_06, n_05);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(), n_09, n_13);
    /*auto& e_10_13 = */g_1.addEdge(make_unique<Edge>(), n_10, n_13);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */g_1.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_14_15 = */g_1.addEdge(make_unique<Edge>(), n_14, n_15);
    /*auto& e_15_16 = */g_1.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_15_19 = */g_1.addEdge(make_unique<Edge>(), n_15, n_19);
    /*auto& e_16_19 = */g_1.addEdge(make_unique<Edge>(), n_16, n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(), n_16, n_17);
    /*auto& e_17_18 = */g_1.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_16_18 = */g_1.addEdge(make_unique<Edge>(), n_16, n_18);
    /*auto& e_19_20 = */g_1.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */g_1.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_20_21 = */g_1.addEdge(make_unique<Edge>(), n_20, n_21);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_03.backIt(), n_19.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_BOTH_CUT_VERTICES);

}


/**  @brief tests 
 */
TEST_F(GMWBCPathTests, Test10) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(10));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(21));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    /*auto& e_06_05 = */g_1.addEdge(make_unique<Edge>(), n_06, n_05);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(), n_09, n_13);
    /*auto& e_10_13 = */g_1.addEdge(make_unique<Edge>(), n_10, n_13);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */g_1.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_14_15 = */g_1.addEdge(make_unique<Edge>(), n_14, n_15);
    /*auto& e_15_16 = */g_1.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_15_19 = */g_1.addEdge(make_unique<Edge>(), n_15, n_19);
    /*auto& e_16_19 = */g_1.addEdge(make_unique<Edge>(), n_16, n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(), n_16, n_17);
    /*auto& e_17_18 = */g_1.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_16_18 = */g_1.addEdge(make_unique<Edge>(), n_16, n_18);
    /*auto& e_19_20 = */g_1.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */g_1.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_20_21 = */g_1.addEdge(make_unique<Edge>(), n_20, n_21);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_06.backIt(), n_08.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_BOTH_IN_THE_SAME_BLOCK);

    node_list_it_t bnit_01;
    node_list_it_t bnit_02;

    Block& b_01 = bcPath.getSingleBlock(bnit_01, bnit_02);

    EXPECT_EQ(b_01.numNodes(),4);
    EXPECT_EQ(b_01.numEdges(),6);

    size_t nodesFound = 0;
    for (auto bnit = b_01.nodes().first; bnit != b_01.nodes().second; bnit++) {
        if (bnit == bnit_01 || bnit == bnit_02) {
            nodesFound++;
        }
    }
    EXPECT_EQ(nodesFound, 2);

    auto& BN_01 = dynamic_cast<BlockNode&>(*(*bnit_01));
    EXPECT_EQ(BN_01.IGBackwardLink(), n_06.backIt());
    auto& BN_02 = dynamic_cast<BlockNode&>(*(*bnit_02));
    EXPECT_EQ(BN_02.IGBackwardLink(), n_08.backIt());

}


/**  @brief tests 
 */
TEST_F(GMWBCPathTests, Test11) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(10));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(21));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    /*auto& e_06_05 = */g_1.addEdge(make_unique<Edge>(), n_06, n_05);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(), n_09, n_13);
    /*auto& e_10_13 = */g_1.addEdge(make_unique<Edge>(), n_10, n_13);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */g_1.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_14_15 = */g_1.addEdge(make_unique<Edge>(), n_14, n_15);
    /*auto& e_15_16 = */g_1.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_15_19 = */g_1.addEdge(make_unique<Edge>(), n_15, n_19);
    /*auto& e_16_19 = */g_1.addEdge(make_unique<Edge>(), n_16, n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(), n_16, n_17);
    /*auto& e_17_18 = */g_1.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_16_18 = */g_1.addEdge(make_unique<Edge>(), n_16, n_18);
    /*auto& e_19_20 = */g_1.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */g_1.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_20_21 = */g_1.addEdge(make_unique<Edge>(), n_20, n_21);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_18.backIt(), n_08.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK);

    node_list_it_t bnit_01;
    node_list_it_t bnit_02;

    Block& b_01 = bcPath.getBlock1(bnit_01, bnit_02);

    EXPECT_EQ(b_01.numNodes(),3);
    EXPECT_EQ(b_01.numEdges(),3);

    size_t nodesFound = 0;
    for (auto bnit = b_01.nodes().first; bnit != b_01.nodes().second; bnit++) {
        if (bnit == bnit_01 || bnit == bnit_02) {
            nodesFound++;
        }
    }
    EXPECT_EQ(nodesFound, 2);

    auto& BN_01 = dynamic_cast<BlockNode&>(*(*bnit_01));
    EXPECT_EQ(BN_01.IGBackwardLink(), n_18.backIt());
    auto& BN_02 = dynamic_cast<BlockNode&>(*(*bnit_02));
    EXPECT_EQ(BN_02.IGBackwardLink(), n_16.backIt());

    node_list_it_t bnit_03;
    node_list_it_t bnit_04;

    Block& b_02 = bcPath.getBlock2(bnit_03, bnit_04);

    EXPECT_EQ(b_02.numNodes(),4);
    EXPECT_EQ(b_02.numEdges(),6);

    nodesFound = 0;
    for (auto bnit = b_02.nodes().first; bnit != b_02.nodes().second; bnit++) {
        if (bnit == bnit_03 || bnit == bnit_04) {
            nodesFound++;
        }
    }
    EXPECT_EQ(nodesFound, 2);

    auto& BN_03 = dynamic_cast<BlockNode&>(*(*bnit_03));
    EXPECT_EQ(BN_03.IGBackwardLink(), n_05.backIt());
    auto& BN_04 = dynamic_cast<BlockNode&>(*(*bnit_04));
    EXPECT_EQ(BN_04.IGBackwardLink(), n_08.backIt());

}


/**  @brief tests 
 */
TEST_F(GMWBCPathTests, Test12) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(10));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(21));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    /*auto& e_06_05 = */g_1.addEdge(make_unique<Edge>(), n_06, n_05);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(), n_09, n_13);
    /*auto& e_10_13 = */g_1.addEdge(make_unique<Edge>(), n_10, n_13);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */g_1.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_14_15 = */g_1.addEdge(make_unique<Edge>(), n_14, n_15);
    /*auto& e_15_16 = */g_1.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_15_19 = */g_1.addEdge(make_unique<Edge>(), n_15, n_19);
    /*auto& e_16_19 = */g_1.addEdge(make_unique<Edge>(), n_16, n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(), n_16, n_17);
    /*auto& e_17_18 = */g_1.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_16_18 = */g_1.addEdge(make_unique<Edge>(), n_16, n_18);
    /*auto& e_19_20 = */g_1.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */g_1.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_20_21 = */g_1.addEdge(make_unique<Edge>(), n_20, n_21);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_07.backIt(), n_10.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_CUT_VERTEX);

    node_list_it_t bnit_01;
    node_list_it_t bnit_02;

    Block& b_01 = bcPath.getBlock1(bnit_01, bnit_02);

    EXPECT_EQ(b_01.numNodes(),4);
    EXPECT_EQ(b_01.numEdges(),6);

    size_t nodesFound = 0;
    for (auto bnit = b_01.nodes().first; bnit != b_01.nodes().second; bnit++) {
        if (bnit == bnit_01 || bnit == bnit_02) {
            nodesFound++;
        }
    }
    EXPECT_EQ(nodesFound, 2);

    auto& BN_01 = dynamic_cast<BlockNode&>(*(*bnit_01));
    EXPECT_EQ(BN_01.IGBackwardLink(), n_07.backIt());
    auto& BN_02 = dynamic_cast<BlockNode&>(*(*bnit_02));
    EXPECT_EQ(BN_02.IGBackwardLink(), n_05.backIt());

}


/**  @brief tests 
 */
TEST_F(GMWBCPathTests, Test13) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(10));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(21));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_03_01 = */g_1.addEdge(make_unique<Edge>(), n_03, n_01);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    /*auto& e_06_05 = */g_1.addEdge(make_unique<Edge>(), n_06, n_05);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(), n_09, n_13);
    /*auto& e_10_13 = */g_1.addEdge(make_unique<Edge>(), n_10, n_13);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */g_1.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_14_15 = */g_1.addEdge(make_unique<Edge>(), n_14, n_15);
    /*auto& e_15_16 = */g_1.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_15_19 = */g_1.addEdge(make_unique<Edge>(), n_15, n_19);
    /*auto& e_16_19 = */g_1.addEdge(make_unique<Edge>(), n_16, n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(), n_16, n_17);
    /*auto& e_17_18 = */g_1.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_16_18 = */g_1.addEdge(make_unique<Edge>(), n_16, n_18);
    /*auto& e_19_20 = */g_1.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */g_1.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_20_21 = */g_1.addEdge(make_unique<Edge>(), n_20, n_21);

    GMWBCPath bcPath(g_1);
    bcPath.processConnectedGraph(n_05.backIt(), n_20.backIt());
    GMWBCPath::pathType type = bcPath.getPathType();
    EXPECT_EQ(type, GMWBCPath::BC_PATH_N1_IS_CUT_VERTEX_N2_IS_BLOCK);

    node_list_it_t bnit_01;
    node_list_it_t bnit_02;

    Block& b_01 = bcPath.getBlock2(bnit_01, bnit_02);

    EXPECT_EQ(b_01.numNodes(),3);
    EXPECT_EQ(b_01.numEdges(),3);

    size_t nodesFound = 0;
    for (auto bnit = b_01.nodes().first; bnit != b_01.nodes().second; bnit++) {
        if (bnit == bnit_01 || bnit == bnit_02) {
            nodesFound++;
        }
    }
    EXPECT_EQ(nodesFound, 2);

    auto& BN_01 = dynamic_cast<BlockNode&>(*(*bnit_01));
    EXPECT_EQ(BN_01.IGBackwardLink(), n_19.backIt());
    auto& BN_02 = dynamic_cast<BlockNode&>(*(*bnit_02));
    EXPECT_EQ(BN_02.IGBackwardLink(), n_20.backIt());

}

} // namespace Undirected

} // namespace Wailea

