#include "gtest/gtest.h"
#include "undirected/gmw_edge_inserter.hpp"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/bl_tree.hpp"
#include "undirected/bl_tree_node.hpp"
#include "undirected/bl_inline.hpp"
#include "undirected/st_numbering.hpp"

namespace Wailea {

namespace Undirected {

class GMWEdgeInserterTests : public ::testing::Test {

  protected:

    GMWEdgeInserterTests(){;};
    virtual ~GMWEdgeInserterTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief Tests GMWSkeleton::generateSkeleton()
 */
TEST_F(GMWEdgeInserterTests, Test1) {

    GMWEdgeInserter inserter;

    // Create underlying block.
    Graph G01;

    auto& n_01 = G01.addNode(make_unique<NumNode>(1));
    auto& n_02 = G01.addNode(make_unique<NumNode>(2));
    auto& n_03 = G01.addNode(make_unique<NumNode>(3));
    auto& n_04 = G01.addNode(make_unique<NumNode>(4));
    auto& n_05 = G01.addNode(make_unique<NumNode>(5));
    auto& n_06 = G01.addNode(make_unique<NumNode>(6));
    auto& n_07 = G01.addNode(make_unique<NumNode>(7));
    auto& n_08 = G01.addNode(make_unique<NumNode>(8));
    auto& n_09 = G01.addNode(make_unique<NumNode>(9));
    auto& n_10 = G01.addNode(make_unique<NumNode>(10));
    auto& n_11 = G01.addNode(make_unique<NumNode>(11));
    auto& n_12 = G01.addNode(make_unique<NumNode>(12));
    auto& n_13 = G01.addNode(make_unique<NumNode>(13));
    auto& n_14 = G01.addNode(make_unique<NumNode>(14));
    auto& n_15 = G01.addNode(make_unique<NumNode>(15));
    auto& n_16 = G01.addNode(make_unique<NumNode>(16));
    auto& n_17 = G01.addNode(make_unique<NumNode>(17));
    auto& n_18 = G01.addNode(make_unique<NumNode>(18));
    auto& n_19 = G01.addNode(make_unique<NumNode>(19));
    auto& n_20 = G01.addNode(make_unique<NumNode>(20));
    auto& n_21 = G01.addNode(make_unique<NumNode>(21));
    auto& n_22 = G01.addNode(make_unique<NumNode>(22));
//    auto& n_23 = G01.addNode(make_unique<NumNode>(23));

    /*auto& e_01_02 = */G01.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_01_03 = */G01.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_07 = G01.addEdge(make_unique<Edge>(), n_01, n_07);
    /*auto& e_02_04 = */G01.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_02_08 = */G01.addEdge(make_unique<Edge>(), n_02, n_08);
    /*auto& e_03_04 = */G01.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */G01.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_04_06 = */G01.addEdge(make_unique<Edge>(), n_04, n_06);

    /*auto& e_05_06 = */G01.addEdge(make_unique<Edge>(), n_05, n_06);

//    auto& e_05_23 = G01.addEdge(make_unique<Edge>(), n_05, n_23);
//    auto& e_23_06 = G01.addEdge(make_unique<Edge>(), n_23, n_06);
//    auto& e_03_23 = G01.addEdge(make_unique<Edge>(), n_03, n_23);
//    auto& e_23_19 = G01.addEdge(make_unique<Edge>(), n_23, n_19);

    /*auto& e_05_07 = */G01.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */G01.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_09 = */G01.addEdge(make_unique<Edge>(), n_07, n_09);
    /*auto& e_08_15 = */G01.addEdge(make_unique<Edge>(), n_08, n_15);
    /*auto& e_08_13 = */G01.addEdge(make_unique<Edge>(), n_08, n_13);
    /*auto& e_13_14 = */G01.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */G01.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_15_16 = */G01.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_14_10 = */G01.addEdge(make_unique<Edge>(), n_14, n_10);
    /*auto& e_14_16 = */G01.addEdge(make_unique<Edge>(), n_14, n_16);
    /*auto& e_10_16 = */G01.addEdge(make_unique<Edge>(), n_10, n_16);
    /*auto& e_09_10 = */G01.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_11 = */G01.addEdge(make_unique<Edge>(), n_09, n_11);
    /*auto& e_10_11 = */G01.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_09_12 = */G01.addEdge(make_unique<Edge>(), n_09, n_12);
    /*auto& e_11_12 = */G01.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */G01.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_10_17 = */G01.addEdge(make_unique<Edge>(), n_10, n_17);
    /*auto& e_10_18 = */G01.addEdge(make_unique<Edge>(), n_10, n_18);
    /*auto& e_12_17 = */G01.addEdge(make_unique<Edge>(), n_12, n_17);
    /*auto& e_17_18 = */G01.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_12_18 = */G01.addEdge(make_unique<Edge>(), n_12, n_18);
    /*auto& e_12_21 = */G01.addEdge(make_unique<Edge>(), n_12, n_21);
    /*auto& e_12_19 = */G01.addEdge(make_unique<Edge>(), n_12, n_19);
    /*auto& e_19_20 = */G01.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */G01.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_18_20 = */G01.addEdge(make_unique<Edge>(), n_18, n_20);
    /*auto& e_18_22 = */G01.addEdge(make_unique<Edge>(), n_18, n_22);
    /*auto& e_20_22 = */G01.addEdge(make_unique<Edge>(), n_20, n_22);
    /*auto& e_21_22 = */G01.addEdge(make_unique<Edge>(), n_21, n_22);

//    STNumbering st;
//    vector<node_list_it_t> st_list =
//                  st.getBipolarOrientation(G01, n_01, n_22);
//    BLPlanarityTester tester;
//    EXPECT_EQ(tester.isPlanar(G01, st_list), true);

    inserter.findInsertionPath(G01, n_03, n_19);
    const list<edge_list_it_t>& insertionPath = inserter.getPath();
    EXPECT_EQ(insertionPath.size(), 1);
    EXPECT_EQ(*(insertionPath.begin()), e_01_07.backIt());

}



/**  @brief Tests GMWSkeleton::generateSkeleton()
 *          Check if the path find in Test 1 between
 *          n_03 and n_19 makes a planar graph.
 */
TEST_F(GMWEdgeInserterTests, Test1_aux) {

    GMWEdgeInserter inserter;

    // Create underlying block.
    Graph G01;

    auto& n_01 = G01.addNode(make_unique<NumNode>(1));
    auto& n_02 = G01.addNode(make_unique<NumNode>(2));
    auto& n_03 = G01.addNode(make_unique<NumNode>(3));
    auto& n_04 = G01.addNode(make_unique<NumNode>(4));
    auto& n_05 = G01.addNode(make_unique<NumNode>(5));
    auto& n_06 = G01.addNode(make_unique<NumNode>(6));
    auto& n_07 = G01.addNode(make_unique<NumNode>(7));
    auto& n_08 = G01.addNode(make_unique<NumNode>(8));
    auto& n_09 = G01.addNode(make_unique<NumNode>(9));
    auto& n_10 = G01.addNode(make_unique<NumNode>(10));
    auto& n_11 = G01.addNode(make_unique<NumNode>(11));
    auto& n_12 = G01.addNode(make_unique<NumNode>(12));
    auto& n_13 = G01.addNode(make_unique<NumNode>(13));
    auto& n_14 = G01.addNode(make_unique<NumNode>(14));
    auto& n_15 = G01.addNode(make_unique<NumNode>(15));
    auto& n_16 = G01.addNode(make_unique<NumNode>(16));
    auto& n_17 = G01.addNode(make_unique<NumNode>(17));
    auto& n_18 = G01.addNode(make_unique<NumNode>(18));
    auto& n_19 = G01.addNode(make_unique<NumNode>(19));
    auto& n_20 = G01.addNode(make_unique<NumNode>(20));
    auto& n_21 = G01.addNode(make_unique<NumNode>(21));
    auto& n_22 = G01.addNode(make_unique<NumNode>(22));

    // n_23 is a virtual node between 1 and 7.
    auto& n_23 = G01.addNode(make_unique<NumNode>(23));

    /*auto& e_01_02 = */G01.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_01_03 = */G01.addEdge(make_unique<Edge>(), n_01, n_03);

    //Split (1,7) into (1,23) & (23,7)
    //Add   (3,23) and (23,19)

    /*auto& e_01_23 = */G01.addEdge(make_unique<Edge>(), n_01, n_23);
    /*auto& e_23_07 = */G01.addEdge(make_unique<Edge>(), n_23, n_07);
    /*auto& e_03_23 = */G01.addEdge(make_unique<Edge>(), n_03, n_23);
    /*auto& e_19_23 = */G01.addEdge(make_unique<Edge>(), n_19, n_23);
    
    /*auto& e_02_04 = */G01.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_02_08 = */G01.addEdge(make_unique<Edge>(), n_02, n_08);
    /*auto& e_03_04 = */G01.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */G01.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_04_06 = */G01.addEdge(make_unique<Edge>(), n_04, n_06);
    /*auto& e_05_06 = */G01.addEdge(make_unique<Edge>(), n_05, n_06);

    /*auto& e_05_07 = */G01.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_06_08 = */G01.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_07_09 = */G01.addEdge(make_unique<Edge>(), n_07, n_09);
    /*auto& e_08_15 = */G01.addEdge(make_unique<Edge>(), n_08, n_15);
    /*auto& e_08_13 = */G01.addEdge(make_unique<Edge>(), n_08, n_13);
    /*auto& e_13_14 = */G01.addEdge(make_unique<Edge>(), n_13, n_14);
    /*auto& e_13_15 = */G01.addEdge(make_unique<Edge>(), n_13, n_15);
    /*auto& e_15_16 = */G01.addEdge(make_unique<Edge>(), n_15, n_16);
    /*auto& e_14_10 = */G01.addEdge(make_unique<Edge>(), n_14, n_10);
    /*auto& e_14_16 = */G01.addEdge(make_unique<Edge>(), n_14, n_16);
    /*auto& e_10_16 = */G01.addEdge(make_unique<Edge>(), n_10, n_16);
    /*auto& e_09_10 = */G01.addEdge(make_unique<Edge>(), n_09, n_10);
    /*auto& e_09_11 = */G01.addEdge(make_unique<Edge>(), n_09, n_11);
    /*auto& e_10_11 = */G01.addEdge(make_unique<Edge>(), n_10, n_11);
    /*auto& e_09_12 = */G01.addEdge(make_unique<Edge>(), n_09, n_12);
    /*auto& e_11_12 = */G01.addEdge(make_unique<Edge>(), n_11, n_12);
    /*auto& e_10_12 = */G01.addEdge(make_unique<Edge>(), n_10, n_12);
    /*auto& e_10_17 = */G01.addEdge(make_unique<Edge>(), n_10, n_17);
    /*auto& e_10_18 = */G01.addEdge(make_unique<Edge>(), n_10, n_18);
    /*auto& e_12_17 = */G01.addEdge(make_unique<Edge>(), n_12, n_17);
    /*auto& e_17_18 = */G01.addEdge(make_unique<Edge>(), n_17, n_18);
    /*auto& e_12_18 = */G01.addEdge(make_unique<Edge>(), n_12, n_18);
    /*auto& e_12_21 = */G01.addEdge(make_unique<Edge>(), n_12, n_21);
    /*auto& e_12_19 = */G01.addEdge(make_unique<Edge>(), n_12, n_19);
    /*auto& e_19_20 = */G01.addEdge(make_unique<Edge>(), n_19, n_20);
    /*auto& e_19_21 = */G01.addEdge(make_unique<Edge>(), n_19, n_21);
    /*auto& e_18_20 = */G01.addEdge(make_unique<Edge>(), n_18, n_20);
    /*auto& e_18_22 = */G01.addEdge(make_unique<Edge>(), n_18, n_22);
    /*auto& e_20_22 = */G01.addEdge(make_unique<Edge>(), n_20, n_22);
    /*auto& e_21_22 = */G01.addEdge(make_unique<Edge>(), n_21, n_22);

    STNumbering st;
    vector<node_list_it_t> st_list =
                  st.getBipolarOrientation(G01, n_01, n_22);
    BLPlanarityTester tester;
    EXPECT_EQ(tester.isPlanar(G01, st_list), true);


}

} // namespace Undirected

} // namespace Wailea

