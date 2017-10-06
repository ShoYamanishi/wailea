#include "gtest/gtest.h"
#include "undirected/planar_insertion_path_finder.hpp"

namespace Wailea {

namespace Undirected {

class PlanarInsertionPathFinderTests : public ::testing::Test {

  protected:

    PlanarInsertionPathFinderTests(){;};
    virtual ~PlanarInsertionPathFinderTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief tests 3-cycle (minimal biconnected graph)
 */
TEST_F(PlanarInsertionPathFinderTests, Test1) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);

    PlanarInsertionPathFinder finder(g_1);

    list<edge_list_it_t> edges = finder.findInsertionPath(
        n_01.backIt(), 
        g_1.edges().second,
        n_02.backIt(), 
        g_1.edges().second
    );

    EXPECT_EQ(edges.size(), 0);

}


/**  @brief tests 
 */
TEST_F(PlanarInsertionPathFinderTests, Test2) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_04_01 = */g_1.addEdge(make_unique<Edge>(), n_04, n_01);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(), n_01, n_03);

    PlanarInsertionPathFinder finder(g_1);

    list<edge_list_it_t> edges = finder.findInsertionPath(
        n_02.backIt(), 
        g_1.edges().second,
        n_04.backIt(), 
        g_1.edges().second
    );

    EXPECT_EQ(edges.size(), 0);

}


/**  @brief tests  K5 -1
 */
TEST_F(PlanarInsertionPathFinderTests, Test3) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    /*auto& e_05_01 = */g_1.addEdge(make_unique<Edge>(), n_05, n_01);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);

    PlanarInsertionPathFinder finder(g_1);

    list<edge_list_it_t> edges = finder.findInsertionPath(
        n_02.backIt(), 
        g_1.edges().second,
        n_05.backIt(), 
        g_1.edges().second
    );

    EXPECT_EQ(edges.size(), 1);
    auto eit = *(edges.begin());
    EXPECT_EQ( eit==e_01_04.backIt() ||
               eit==e_03_04.backIt() ||
               eit==e_01_03.backIt(), true );

}


/**  @brief tests  realistic rigid node-node
 */
TEST_F(PlanarInsertionPathFinderTests, Test4) {


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


    /*auto& e_01_05 =   */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_01_06 =   */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_02 =   */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_06_15 =   g_1.addEdge(make_unique<Edge>(),n_06,n_15);
    /*auto& e_06_11 =   */g_1.addEdge(make_unique<Edge>(),n_06,n_11);
    /*auto& e_05_10 =   */g_1.addEdge(make_unique<Edge>(),n_05,n_10);
    /*auto& e_02_07 =   */g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    /*auto& e_10_15 =   */g_1.addEdge(make_unique<Edge>(),n_10,n_15);
    /*auto& e_07_11 =   */g_1.addEdge(make_unique<Edge>(),n_07,n_11);
    /*auto& e_15_20 =   */g_1.addEdge(make_unique<Edge>(),n_15,n_20);
    auto& e_11_16 =   g_1.addEdge(make_unique<Edge>(),n_11,n_16);
    /*auto& e_20_16 =   */g_1.addEdge(make_unique<Edge>(),n_20,n_16);
    /*auto& e_05_04 =   */g_1.addEdge(make_unique<Edge>(),n_05,n_04);
    /*auto& e_10_14 =   */g_1.addEdge(make_unique<Edge>(),n_10,n_14);
    /*auto& e_20_19 =   */g_1.addEdge(make_unique<Edge>(),n_20,n_19);
    /*auto& e_16_17 =   */g_1.addEdge(make_unique<Edge>(),n_16,n_17);
    /*auto& e_07_12 =   */g_1.addEdge(make_unique<Edge>(),n_07,n_12);
    /*auto& e_02_03 =   */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_14_19 =   */g_1.addEdge(make_unique<Edge>(),n_14,n_19);
    /*auto& e_14_09 =   */g_1.addEdge(make_unique<Edge>(),n_14,n_09);
    /*auto& e_18_19 =   */g_1.addEdge(make_unique<Edge>(),n_18,n_19);
    /*auto& e_18_17 =   */g_1.addEdge(make_unique<Edge>(),n_18,n_17);
    /*auto& e_12_17 =   */g_1.addEdge(make_unique<Edge>(),n_12,n_17);
    /*auto& e_12_08 =   */g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    /*auto& e_13_18 =   */g_1.addEdge(make_unique<Edge>(),n_13,n_18);
    /*auto& e_09_13 =   */g_1.addEdge(make_unique<Edge>(),n_09,n_13);
    /*auto& e_13_08 =   */g_1.addEdge(make_unique<Edge>(),n_13,n_08);
    /*auto& e_03_08 =   */g_1.addEdge(make_unique<Edge>(),n_03,n_08);
    /*auto& e_04_09 =   */g_1.addEdge(make_unique<Edge>(),n_04,n_09);
    /*auto& e_04_03 =   */g_1.addEdge(make_unique<Edge>(),n_04,n_03);

    PlanarInsertionPathFinder finder(g_1);

    list<edge_list_it_t> edges = finder.findInsertionPath(
        n_17.backIt(), 
        g_1.edges().second,
        n_05.backIt(), 
        g_1.edges().second
    );

    EXPECT_EQ(edges.size(), 2);
    auto eit = edges.begin();
    EXPECT_EQ(*eit, e_11_16.backIt());
    eit++;
    EXPECT_EQ(*eit, e_06_15.backIt());
}


/**  @brief tests  realistic rigid edge-edge
 */
TEST_F(PlanarInsertionPathFinderTests, Test5) {


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

    /*auto& e_01_05 =   */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_01_06 =   */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_02 =   */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_06_15 =   */g_1.addEdge(make_unique<Edge>(),n_06,n_15);
    auto& e_06_11 =   g_1.addEdge(make_unique<Edge>(),n_06,n_11);
    /*auto& e_05_10 =   */g_1.addEdge(make_unique<Edge>(),n_05,n_10);
    /*auto& e_02_07 =   */g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    /*auto& e_10_15 =   */g_1.addEdge(make_unique<Edge>(),n_10,n_15);
    /*auto& e_07_11 =   */g_1.addEdge(make_unique<Edge>(),n_07,n_11);
    auto& e_15_20 =   g_1.addEdge(make_unique<Edge>(),n_15,n_20);
    /*auto& e_11_16 =   */g_1.addEdge(make_unique<Edge>(),n_11,n_16);
    /*auto& e_20_16 =   */g_1.addEdge(make_unique<Edge>(),n_20,n_16);
    /*auto& e_05_04 =   */g_1.addEdge(make_unique<Edge>(),n_05,n_04);
    /*auto& e_10_14 =   */g_1.addEdge(make_unique<Edge>(),n_10,n_14);
    /*auto& e_20_19 =   */g_1.addEdge(make_unique<Edge>(),n_20,n_19);
    /*auto& e_16_17 =   */g_1.addEdge(make_unique<Edge>(),n_16,n_17);
    /*auto& e_07_12 =   */g_1.addEdge(make_unique<Edge>(),n_07,n_12);
    /*auto& e_02_03 =   */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_14_19 =   g_1.addEdge(make_unique<Edge>(),n_14,n_19);
    /*auto& e_14_09 =   */g_1.addEdge(make_unique<Edge>(),n_14,n_09);
    /*auto& e_18_19 =   */g_1.addEdge(make_unique<Edge>(),n_18,n_19);
    /*auto& e_18_17 =   */g_1.addEdge(make_unique<Edge>(),n_18,n_17);
    /*auto& e_12_17 =   */g_1.addEdge(make_unique<Edge>(),n_12,n_17);
    /*auto& e_12_08 =   */g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    /*auto& e_13_18 =   */g_1.addEdge(make_unique<Edge>(),n_13,n_18);
    auto& e_09_13 =   g_1.addEdge(make_unique<Edge>(),n_09,n_13);
    /*auto& e_13_08 =   */g_1.addEdge(make_unique<Edge>(),n_13,n_08);
    /*auto& e_03_08 =   */g_1.addEdge(make_unique<Edge>(),n_03,n_08);
    /*auto& e_04_09 =   */g_1.addEdge(make_unique<Edge>(),n_04,n_09);
    /*auto& e_04_03 =   */g_1.addEdge(make_unique<Edge>(),n_04,n_03);

    PlanarInsertionPathFinder finder(g_1);

    list<edge_list_it_t> edges = finder.findInsertionPath(
        g_1.nodes().second,
        e_09_13.backIt(),
        g_1.nodes().second,
        e_06_11.backIt()
    );

    EXPECT_EQ(edges.size(), 2);
    auto eit = edges.begin();
    EXPECT_EQ(*eit, e_14_19.backIt());
    eit++;
    EXPECT_EQ(*eit, e_15_20.backIt());
}

} // namespace Undirected

} // namespace Wailea

