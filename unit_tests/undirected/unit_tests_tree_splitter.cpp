#include "gtest/gtest.h"
#include "undirected/tree_splitter.hpp"

namespace Wailea {

namespace Undirected {

class TreeSplitterTests : public ::testing::Test {

  protected:

    TreeSplitterTests(){;};
    virtual ~TreeSplitterTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    bool checkCompNodes(list<node_list_it_t> L1,list<node_list_it_t> L2) {
        if (L1.size() != L1.size()) {
            return false;
        }
        vector<size_t> counts;        
        for (size_t i = 0; i < L1.size(); i++ ) {
            counts.push_back(0);
        }
        size_t index = 0;
        for (auto l1 : L1) {
            for (auto l2 : L2) {
                if (l1 == l2) {
                    counts[index] = counts[index] + 1;
                }
            }
            index++;
        }
        for (auto c : counts) {
            if (c != 1) {
                return false;
            }
        }
        return true;
    }

};


/**  @brief tests single node
 */
TEST_F(TreeSplitterTests, Test1) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));

    TreeSplitter splitter(g_1);

    list<edge_list_it_t> cut;
    splitter.prepareTree(cut);

    list<node_list_it_t> comp1;
    comp1 = splitter.findComponent(n_01.backIt());
    EXPECT_EQ(comp1.size(), 1);
    EXPECT_EQ(*(comp1.begin()), n_01.backIt());
    splitter.cleanUpTree();
}


/**  @brief tests single node
 */
TEST_F(TreeSplitterTests, Test2) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);

    TreeSplitter splitter(g_1);

    list<edge_list_it_t> cut;
    cut.push_back(e_01_02.backIt());
    splitter.prepareTree(cut);

    list<node_list_it_t> comp1;
    list<node_list_it_t> comp2;
    comp1 = splitter.findComponent(n_01.backIt());
    EXPECT_EQ(comp1.size(), 1);
    EXPECT_EQ(*(comp1.begin()), n_01.backIt());
    comp2 = splitter.findComponent(n_02.backIt());
    EXPECT_EQ(comp2.size(), 1);
    auto nit = comp2.begin();
    EXPECT_EQ(*nit, n_02.backIt());

    splitter.cleanUpTree();
}


/**  @brief tests single node
 */
TEST_F(TreeSplitterTests, Test3) {

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

    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_06_05 = */g_1.addEdge(make_unique<Edge>(),n_06,n_05);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(),n_05,n_07);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    /*auto& e_07_09 = */g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    auto& e_09_15 = g_1.addEdge(make_unique<Edge>(),n_09,n_15);
    /*auto& e_15_13 =*/g_1.addEdge(make_unique<Edge>(),n_15,n_13);
    /*auto& e_15_14 =*/g_1.addEdge(make_unique<Edge>(),n_15,n_14);

    TreeSplitter splitter(g_1);

    list<edge_list_it_t> cut;
    cut.push_back(e_04_05.backIt());
    cut.push_back(e_09_15.backIt());
    splitter.prepareTree(cut);

    list<node_list_it_t> comp1;
    list<node_list_it_t> comp2;
    list<node_list_it_t> comp3;
    comp1 = splitter.findComponent(n_01.backIt());
    list<node_list_it_t> comp1Nodes;
    comp1Nodes.push_back(n_01.backIt());;
    comp1Nodes.push_back(n_02.backIt());;
    comp1Nodes.push_back(n_03.backIt());;
    comp1Nodes.push_back(n_04.backIt());;
    EXPECT_EQ(checkCompNodes(comp1, comp1Nodes), true);

    comp2 = splitter.findComponent(n_07.backIt());
    list<node_list_it_t> comp2Nodes;
    comp2Nodes.push_back(n_05.backIt());;
    comp2Nodes.push_back(n_06.backIt());;
    comp2Nodes.push_back(n_07.backIt());;
    comp2Nodes.push_back(n_08.backIt());;
    comp2Nodes.push_back(n_09.backIt());;
    comp2Nodes.push_back(n_10.backIt());;
    comp2Nodes.push_back(n_11.backIt());;
    comp2Nodes.push_back(n_12.backIt());;
    EXPECT_EQ(checkCompNodes(comp2, comp2Nodes), true);

    comp3 = splitter.findComponent(n_15.backIt());
    list<node_list_it_t> comp3Nodes;
    comp3Nodes.push_back(n_15.backIt());;
    comp3Nodes.push_back(n_13.backIt());;
    comp3Nodes.push_back(n_14.backIt());;
    EXPECT_EQ(checkCompNodes(comp3, comp3Nodes), true);

    splitter.cleanUpTree();
}


/**  @brief tests single node
 */
TEST_F(TreeSplitterTests, Test4) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));
    auto& n_09 = g_1.addNode(make_unique<NumNode>(9));
    auto& n_10 = g_1.addNode(make_unique<NumNode>(10));
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));

    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    auto& e_05_07 = g_1.addEdge(make_unique<Edge>(),n_05,n_07);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    auto& e_07_09 = g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    auto& e_09_15 = g_1.addEdge(make_unique<Edge>(),n_09,n_15);
    /*auto& e_15_13 =*/g_1.addEdge(make_unique<Edge>(),n_15,n_13);
    /*auto& e_15_14 =*/g_1.addEdge(make_unique<Edge>(),n_15,n_14);

    TreeSplitter splitter(g_1);

    list<edge_list_it_t> cut;
    cut.push_back(e_04_05.backIt());
    cut.push_back(e_05_07.backIt());
    cut.push_back(e_07_09.backIt());
    cut.push_back(e_09_10.backIt());
    cut.push_back(e_09_15.backIt());
    splitter.prepareTree(cut);

    list<node_list_it_t> comp1;
    list<node_list_it_t> comp2;
    list<node_list_it_t> comp3;
    list<node_list_it_t> comp4;
    list<node_list_it_t> comp5;
    list<node_list_it_t> comp6;

    comp1 = splitter.findComponent(n_01.backIt());
    list<node_list_it_t> comp1Nodes;
    comp1Nodes.push_back(n_01.backIt());;
    comp1Nodes.push_back(n_02.backIt());;
    comp1Nodes.push_back(n_03.backIt());;
    comp1Nodes.push_back(n_04.backIt());;
    EXPECT_EQ(checkCompNodes(comp1, comp1Nodes), true);

    comp2 = splitter.findComponent(n_05.backIt());
    list<node_list_it_t> comp2Nodes;
    comp2Nodes.push_back(n_05.backIt());;
    EXPECT_EQ(checkCompNodes(comp2, comp2Nodes), true);

    comp3 = splitter.findComponent(n_07.backIt());
    list<node_list_it_t> comp3Nodes;
    comp3Nodes.push_back(n_07.backIt());;
    comp3Nodes.push_back(n_08.backIt());;
    EXPECT_EQ(checkCompNodes(comp3, comp3Nodes), true);

    comp4 = splitter.findComponent(n_09.backIt());
    list<node_list_it_t> comp4Nodes;
    comp4Nodes.push_back(n_09.backIt());;
    EXPECT_EQ(checkCompNodes(comp4, comp4Nodes), true);

    comp5 = splitter.findComponent(n_10.backIt());
    list<node_list_it_t> comp5Nodes;
    comp5Nodes.push_back(n_10.backIt());;
    comp5Nodes.push_back(n_11.backIt());;
    comp5Nodes.push_back(n_12.backIt());;
    EXPECT_EQ(checkCompNodes(comp5, comp5Nodes), true);

    comp6 = splitter.findComponent(n_15.backIt());
    list<node_list_it_t> comp6Nodes;
    comp6Nodes.push_back(n_15.backIt());;
    comp6Nodes.push_back(n_13.backIt());;
    comp6Nodes.push_back(n_14.backIt());;
    EXPECT_EQ(checkCompNodes(comp6, comp6Nodes), true);

    splitter.cleanUpTree();
}

} // namespace Undirected

} // namespace Wailea

