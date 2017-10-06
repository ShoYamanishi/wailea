#include "gtest/gtest.h"
#include "undirected/tree_path_finder.hpp"

namespace Wailea {

namespace Undirected {

class TreePathFinderTests : public ::testing::Test {

  protected:

    TreePathFinderTests(){;};
    virtual ~TreePathFinderTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief tests single node
 */
TEST_F(TreePathFinderTests, Test1) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));

    TreePathFinder finder;

    list<node_list_it_t> nodes;
    list<edge_list_it_t> edges;
    finder.findPath(g_1, n_01, n_01, nodes, edges);
    EXPECT_EQ(edges.size(), 0);
    EXPECT_EQ(nodes.size(), 1);
    EXPECT_EQ(*(nodes.begin()), n_01.backIt());

}


/**  @brief tests
 */
TEST_F(TreePathFinderTests, Test2) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);

    TreePathFinder finder;

    list<node_list_it_t> nodes;
    list<edge_list_it_t> edges;
    finder.findPath(g_1, n_01, n_02, nodes, edges);

    EXPECT_EQ(edges.size(), 1);
    auto eit = edges.begin();
    EXPECT_EQ(*eit, e_01_02.backIt());

    EXPECT_EQ(nodes.size(), 2);
    auto nit = nodes.begin();
    EXPECT_EQ(*nit, n_01.backIt());
    nit++;
    EXPECT_EQ(*nit, n_02.backIt());

}


/**  @brief tests
 */
TEST_F(TreePathFinderTests, Test3) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);


    TreePathFinder finder;

    list<node_list_it_t> nodes;
    list<edge_list_it_t> edges;

    finder.findPath(g_1, n_01, n_03, nodes, edges);

    EXPECT_EQ(edges.size(), 2);
    auto eit = edges.begin();
    EXPECT_EQ(*eit, e_01_02.backIt());
    eit++;
    EXPECT_EQ(*eit, e_02_03.backIt());

    EXPECT_EQ(nodes.size(), 3);
    auto nit = nodes.begin();
    EXPECT_EQ(*nit, n_01.backIt());
    nit++;
    EXPECT_EQ(*nit, n_02.backIt());
    nit++;
    EXPECT_EQ(*nit, n_03.backIt());

}

/**  @brief tests
 */
TEST_F(TreePathFinderTests, Test4) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(),n_01,n_03);

    TreePathFinder finder;

    list<node_list_it_t> nodes;
    list<edge_list_it_t> edges;


    finder.findPath(g_1, n_01, n_03, nodes, edges);
    EXPECT_EQ(edges.size(), 1);
    auto eit = edges.begin();
    EXPECT_EQ(*eit, e_01_03.backIt());

    EXPECT_EQ(nodes.size(), 2);
    auto nit = nodes.begin();
    EXPECT_EQ(*nit, n_01.backIt());
    nit++;
    EXPECT_EQ(*nit, n_03.backIt());

}

/**  @brief tests
 */
TEST_F(TreePathFinderTests, Test5) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(),n_01,n_03);

    TreePathFinder finder;

    list<edge_list_it_t> edges;
    list<node_list_it_t> nodes;
    finder.findPath(g_1, n_03, n_02, nodes, edges);

    EXPECT_EQ(edges.size(), 2);
    auto eit = edges.begin();
    EXPECT_EQ(*eit, e_01_03.backIt());
    eit++;
    EXPECT_EQ(*eit, e_01_02.backIt());

    EXPECT_EQ(nodes.size(), 3);
    auto nit = nodes.begin();
    EXPECT_EQ(*nit, n_03.backIt());
    nit++;
    EXPECT_EQ(*nit, n_01.backIt());
    nit++;
    EXPECT_EQ(*nit, n_02.backIt());

}



/**  @brief tests : realistic tree
 */
TEST_F(TreePathFinderTests, Test6) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_22 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_23 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_24 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_25 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_26 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_27 = g_1.addNode(make_unique<NumNode>(3));

    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_12 = g_1.addEdge(make_unique<Edge>(),n_04,n_12);
    auto& e_05_07 = g_1.addEdge(make_unique<Edge>(),n_05,n_07);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    auto& e_07_08 = g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    auto& e_08_12 = g_1.addEdge(make_unique<Edge>(),n_08,n_12);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(),n_11,n_12);
    auto& e_12_13 = g_1.addEdge(make_unique<Edge>(),n_12,n_13);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(),n_13,n_14);
    /*auto& e_14_15 = */g_1.addEdge(make_unique<Edge>(),n_14,n_15);
    /*auto& e_13_16 = */g_1.addEdge(make_unique<Edge>(),n_13,n_16);
    auto& e_13_17 = g_1.addEdge(make_unique<Edge>(),n_13,n_17);
    /*auto& e_12_21 = */g_1.addEdge(make_unique<Edge>(),n_12,n_21);
    /*auto& e_11_18 = */g_1.addEdge(make_unique<Edge>(),n_11,n_18);
    /*auto& e_18_19 = */g_1.addEdge(make_unique<Edge>(),n_18,n_19);
    /*auto& e_18_20 = */g_1.addEdge(make_unique<Edge>(),n_18,n_20);
    /*auto& e_21_22 = */g_1.addEdge(make_unique<Edge>(),n_21,n_22);
    /*auto& e_21_23 = */g_1.addEdge(make_unique<Edge>(),n_21,n_23);
    auto& e_17_24 = g_1.addEdge(make_unique<Edge>(),n_17,n_24);
    /*auto& e_24_25 = */g_1.addEdge(make_unique<Edge>(),n_24,n_25);
    /*auto& e_24_26 = */g_1.addEdge(make_unique<Edge>(),n_24,n_26);
    auto& e_24_27 = g_1.addEdge(make_unique<Edge>(),n_24,n_27);

    TreePathFinder finder;

    list<node_list_it_t> nodes_01;
    list<edge_list_it_t> edges_01;

    finder.findPath(g_1, n_08, n_27, nodes_01, edges_01);

    EXPECT_EQ(edges_01.size(), 5);
    auto eit = edges_01.begin();
    EXPECT_EQ(*eit, e_08_12.backIt());
    eit++;
    EXPECT_EQ(*eit, e_12_13.backIt());
    eit++;
    EXPECT_EQ(*eit, e_13_17.backIt());
    eit++;
    EXPECT_EQ(*eit, e_17_24.backIt());
    eit++;
    EXPECT_EQ(*eit, e_24_27.backIt());

    EXPECT_EQ(nodes_01.size(), 6);
    auto nit = nodes_01.begin();
    EXPECT_EQ(*nit, n_08.backIt());
    nit++;
    EXPECT_EQ(*nit, n_12.backIt());
    nit++;
    EXPECT_EQ(*nit, n_13.backIt());
    nit++;
    EXPECT_EQ(*nit, n_17.backIt());
    nit++;
    EXPECT_EQ(*nit, n_24.backIt());
    nit++;
    EXPECT_EQ(*nit, n_27.backIt());

    list<node_list_it_t> nodes_02;
    list<edge_list_it_t> edges_02;

    finder.findPath(g_1, n_27, n_08, nodes_02, edges_02);
    EXPECT_EQ(edges_02.size(), 5);
    eit = edges_02.begin();
    EXPECT_EQ(*eit, e_24_27.backIt());
    eit++;
    EXPECT_EQ(*eit, e_17_24.backIt());
    eit++;
    EXPECT_EQ(*eit, e_13_17.backIt());
    eit++;
    EXPECT_EQ(*eit, e_12_13.backIt());
    eit++;
    EXPECT_EQ(*eit, e_08_12.backIt());

    EXPECT_EQ(nodes_02.size(), 6);
    nit = nodes_02.begin();
    EXPECT_EQ(*nit, n_27.backIt());
    nit++;
    EXPECT_EQ(*nit, n_24.backIt());
    nit++;
    EXPECT_EQ(*nit, n_17.backIt());
    nit++;
    EXPECT_EQ(*nit, n_13.backIt());
    nit++;
    EXPECT_EQ(*nit, n_12.backIt());
    nit++;
    EXPECT_EQ(*nit, n_08.backIt());

    list<node_list_it_t> nodes_03;
    list<edge_list_it_t> edges_03;

    finder.findPath(g_1, n_04, n_12, nodes_03, edges_03);

    EXPECT_EQ(edges_03.size(), 1);
    eit = edges_03.begin();
    EXPECT_EQ(*eit, e_04_12.backIt());

    EXPECT_EQ(nodes_03.size(), 2);
    nit = nodes_03.begin();
    EXPECT_EQ(*nit, n_04.backIt());
    nit++;
    EXPECT_EQ(*nit, n_12.backIt());

    list<node_list_it_t> nodes_04;
    list<edge_list_it_t> edges_04;

    finder.findPath(g_1, n_05, n_27, nodes_04, edges_04);
    EXPECT_EQ(edges_04.size(), 7);
    eit = edges_04.begin();
    EXPECT_EQ(*eit, e_05_07.backIt());
    eit++;
    EXPECT_EQ(*eit, e_07_08.backIt());
    eit++;
    EXPECT_EQ(*eit, e_08_12.backIt());
    eit++;
    EXPECT_EQ(*eit, e_12_13.backIt());
    eit++;
    EXPECT_EQ(*eit, e_13_17.backIt());
    eit++;
    EXPECT_EQ(*eit, e_17_24.backIt());
    eit++;
    EXPECT_EQ(*eit, e_24_27.backIt());

    EXPECT_EQ(nodes_04.size(), 8);
    nit = nodes_04.begin();
    EXPECT_EQ(*nit, n_05.backIt());
    nit++;
    EXPECT_EQ(*nit, n_07.backIt());
    nit++;
    EXPECT_EQ(*nit, n_08.backIt());
    nit++;
    EXPECT_EQ(*nit, n_12.backIt());
    nit++;
    EXPECT_EQ(*nit, n_13.backIt());
    nit++;
    EXPECT_EQ(*nit, n_17.backIt());
    nit++;
    EXPECT_EQ(*nit, n_24.backIt());
    nit++;
    EXPECT_EQ(*nit, n_27.backIt());

    // The same node.
    list<node_list_it_t> nodes_05;
    list<edge_list_it_t> edges_05;

    finder.findPath(g_1, n_05, n_05, nodes_05, edges_05);
    EXPECT_EQ(edges_05.size(), 0);
    EXPECT_EQ(nodes_05.size(), 1);
    nit = nodes_05.begin();
    EXPECT_EQ(*nit, n_05.backIt());

}

} // namespace Undirected

} // namespace Wailea

