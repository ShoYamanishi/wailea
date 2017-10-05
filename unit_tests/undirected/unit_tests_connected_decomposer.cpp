#include "gtest/gtest.h"
#include "undirected/connected_decomposer.hpp"

namespace Wailea {

namespace Undirected { 

class ConnectedDecomposerTests : public ::testing::Test {

  protected:

    ConnectedDecomposerTests(){;};
    virtual ~ConnectedDecomposerTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    bool checkNodes(
             vector<node_list_it_t>& nodes1, vector<node_list_it_t>& nodes2) {

        if (nodes1.size() != nodes2.size()){
            return false;
        }

        vector<int> counters;
        for (int i = 0; i < nodes1.size(); i++) {counters.push_back(0);}
        int index = 0;
        for (auto n1 : nodes1) {

            for (auto n2 : nodes2) {

                if (n1 == n2) {
                    counters[index] = counters[index] + 1;
                }
            }
            index++;
        }

        for (auto c1 : counters) {

            if (c1 != 1) {
                return false;
            }
        }
        return true;
    }    

    bool checkNodeSets(
             vector<vector<node_list_it_t> >& nodeSets1, 
             vector<vector<node_list_it_t> >& nodeSets2) {

        if (nodeSets1.size() != nodeSets2.size()){
            return false;
        }

        vector<int> counters;
        for (int i = 0; i < nodeSets1.size(); i++) {counters.push_back(0);}
        int index = 0;
        for (auto n1 : nodeSets1) {

            for (auto n2 : nodeSets2) {
                  
                if (checkNodes(n1,n2)) {
                    counters[index] = counters[index] + 1;
                }
            }
            index++;
        }

        for (auto c1 : counters) {

            if (c1 != 1) {
                return false;
            }
        }

        return true;
    }


};
 

TEST_F(ConnectedDecomposerTests, Test1) {

    Graph g;
    ConnectedDecomposer decomposer;

    vector< vector<node_list_it_t> > nodeSets;
    decomposer.decompose(g, nodeSets);
    EXPECT_EQ(nodeSets.size(), 0);

}


TEST_F(ConnectedDecomposerTests, Test2) {

    Graph g;

    auto& n_01 = g.addNode(make_unique<NumNode>(1));
    
    ConnectedDecomposer decomposer;

    vector< vector<node_list_it_t> > nodeSets;
    decomposer.decompose(g, nodeSets);
    EXPECT_EQ(nodeSets.size(), 1);
    EXPECT_EQ(nodeSets[0].size(), 1);
    EXPECT_EQ(nodeSets[0][0], n_01.backIt());

}


TEST_F(ConnectedDecomposerTests, Test3) {

    Graph g;

    auto& n_01 = g.addNode(make_unique<NumNode>(1));
    auto& n_02 = g.addNode(make_unique<NumNode>(2));
    /*auto& e_01_02 = */g.addEdge(make_unique<Edge>(),n_01, n_02);
    
    ConnectedDecomposer decomposer;

    vector< vector<node_list_it_t> > nodeSets;
    decomposer.decompose(g, nodeSets);
    EXPECT_EQ(nodeSets.size(), 1);
    vector<node_list_it_t> nodes_01;
    nodes_01.push_back(n_01.backIt());
    nodes_01.push_back(n_02.backIt());
    EXPECT_EQ(checkNodes(nodeSets[0], nodes_01), true);

}


TEST_F(ConnectedDecomposerTests, Test4) {

    Graph g;

    auto& n_01 = g.addNode(make_unique<NumNode>(1));
    auto& n_02 = g.addNode(make_unique<NumNode>(2));
    auto& n_03 = g.addNode(make_unique<NumNode>(3));
    auto& n_04 = g.addNode(make_unique<NumNode>(4));
    auto& n_05 = g.addNode(make_unique<NumNode>(5));
    auto& n_06 = g.addNode(make_unique<NumNode>(6));

    /*auto& e_01_02 = */g.addEdge(make_unique<Edge>(),n_01, n_02);
    /*auto& e_02_03 = */g.addEdge(make_unique<Edge>(),n_02, n_03);
    /*auto& e_01_03 = */g.addEdge(make_unique<Edge>(),n_01, n_03);

    /*auto& e_04_05 = */g.addEdge(make_unique<Edge>(),n_04, n_05);
    /*auto& e_05_06 = */g.addEdge(make_unique<Edge>(),n_05, n_06);
    
    ConnectedDecomposer decomposer;

    vector< vector<node_list_it_t> > nodeSets;
    decomposer.decompose(g, nodeSets);
    EXPECT_EQ(nodeSets.size(), 2);
    vector<node_list_it_t> nodes_01;
    nodes_01.push_back(n_01.backIt());
    nodes_01.push_back(n_02.backIt());
    nodes_01.push_back(n_03.backIt());
    EXPECT_EQ(checkNodes(nodeSets[0], nodes_01), true);

    vector<node_list_it_t> nodes_02;
    nodes_02.push_back(n_04.backIt());
    nodes_02.push_back(n_05.backIt());
    nodes_02.push_back(n_06.backIt());
    EXPECT_EQ(checkNodes(nodeSets[1], nodes_02), true);

}


TEST_F(ConnectedDecomposerTests, Test5) {

    Graph g;

    auto& n_01 = g.addNode(make_unique<NumNode>(1));
    auto& n_02 = g.addNode(make_unique<NumNode>(2));
    auto& n_03 = g.addNode(make_unique<NumNode>(3));
    auto& n_04 = g.addNode(make_unique<NumNode>(4));
    auto& n_05 = g.addNode(make_unique<NumNode>(5));
    auto& n_06 = g.addNode(make_unique<NumNode>(6));

    /*auto& e_01_02 = */g.addEdge(make_unique<Edge>(),n_01, n_02);
    /*auto& e_02_03 = */g.addEdge(make_unique<Edge>(),n_02, n_03);
    /*auto& e_01_03 = */g.addEdge(make_unique<Edge>(),n_01, n_03);

    /*auto& e_04_05 = */g.addEdge(make_unique<Edge>(),n_04, n_05);
    /*auto& e_05_06 = */g.addEdge(make_unique<Edge>(),n_05, n_06);
    
    ConnectedDecomposer decomposer;

    vector< vector<node_list_it_t> > nodeSets;
    decomposer.decompose(g, nodeSets);
    EXPECT_EQ(nodeSets.size(), 2);
    vector<node_list_it_t> nodes_01;
    nodes_01.push_back(n_01.backIt());
    nodes_01.push_back(n_02.backIt());
    nodes_01.push_back(n_03.backIt());
    vector<node_list_it_t> nodes_02;
    nodes_02.push_back(n_04.backIt());
    nodes_02.push_back(n_05.backIt());
    nodes_02.push_back(n_06.backIt());

    vector< vector<node_list_it_t> > nodeSetsExpected;
    nodeSetsExpected.push_back(std::move(nodes_02));
    nodeSetsExpected.push_back(std::move(nodes_01));

    EXPECT_EQ(checkNodeSets(nodeSets, nodeSetsExpected), true);

}


TEST_F(ConnectedDecomposerTests, Test6) {

    Graph g;

    auto& n_01 = g.addNode(make_unique<NumNode>(1));
    auto& n_02 = g.addNode(make_unique<NumNode>(2));
    auto& n_03 = g.addNode(make_unique<NumNode>(3));
    auto& n_04 = g.addNode(make_unique<NumNode>(4));
    auto& n_05 = g.addNode(make_unique<NumNode>(5));

    auto& n_06 = g.addNode(make_unique<NumNode>(6));
    auto& n_07 = g.addNode(make_unique<NumNode>(7));
    auto& n_08 = g.addNode(make_unique<NumNode>(8));
    auto& n_09 = g.addNode(make_unique<NumNode>(9));

    auto& n_10 = g.addNode(make_unique<NumNode>(10));
    auto& n_11 = g.addNode(make_unique<NumNode>(11));
    auto& n_12 = g.addNode(make_unique<NumNode>(12));
    auto& n_13 = g.addNode(make_unique<NumNode>(13));
    auto& n_14 = g.addNode(make_unique<NumNode>(14));
    auto& n_15 = g.addNode(make_unique<NumNode>(15));
    auto& n_16 = g.addNode(make_unique<NumNode>(16));

    auto& n_17 = g.addNode(make_unique<NumNode>(17));
    auto& n_18 = g.addNode(make_unique<NumNode>(18));
    auto& n_19 = g.addNode(make_unique<NumNode>(19));
    auto& n_20 = g.addNode(make_unique<NumNode>(20));

    /*auto& e_01_02 = */g.addEdge(make_unique<Edge>(),n_01, n_02);
    /*auto& e_02_05 = */g.addEdge(make_unique<Edge>(),n_02, n_05);
    /*auto& e_05_01 = */g.addEdge(make_unique<Edge>(),n_05, n_01);
    /*auto& e_04_01 = */g.addEdge(make_unique<Edge>(),n_04, n_01);
    /*auto& e_04_03 = */g.addEdge(make_unique<Edge>(),n_04, n_03);
    /*auto& e_06_07 = */g.addEdge(make_unique<Edge>(),n_06, n_07);
    /*auto& e_06_08 = */g.addEdge(make_unique<Edge>(),n_06, n_08);
    /*auto& e_07_08 = */g.addEdge(make_unique<Edge>(),n_07, n_08);
    /*auto& e_07_09 = */g.addEdge(make_unique<Edge>(),n_07, n_09);
    /*auto& e_10_11 = */g.addEdge(make_unique<Edge>(),n_10, n_11);
    /*auto& e_10_12 = */g.addEdge(make_unique<Edge>(),n_10, n_12);
    /*auto& e_11_13 = */g.addEdge(make_unique<Edge>(),n_11, n_13);
    /*auto& e_11_14 = */g.addEdge(make_unique<Edge>(),n_11, n_14);
    /*auto& e_12_15 = */g.addEdge(make_unique<Edge>(),n_12, n_15);
    /*auto& e_12_16 = */g.addEdge(make_unique<Edge>(),n_12, n_16);
    /*auto& e_17_19 = */g.addEdge(make_unique<Edge>(),n_17, n_19);
    /*auto& e_17_18 = */g.addEdge(make_unique<Edge>(),n_17, n_18);
    /*auto& e_17_20 = */g.addEdge(make_unique<Edge>(),n_17, n_20);
    /*auto& e_19_18 = */g.addEdge(make_unique<Edge>(),n_19, n_18);
    /*auto& e_19_20 = */g.addEdge(make_unique<Edge>(),n_19, n_20);
    /*auto& e_18_20 = */g.addEdge(make_unique<Edge>(),n_18, n_20);

    ConnectedDecomposer decomposer;

    vector< vector<node_list_it_t> > nodeSets;
    decomposer.decompose(g, nodeSets);
    EXPECT_EQ(nodeSets.size(), 4);
    vector<node_list_it_t> nodes_01;
    nodes_01.push_back(n_01.backIt());
    nodes_01.push_back(n_02.backIt());
    nodes_01.push_back(n_03.backIt());
    nodes_01.push_back(n_04.backIt());
    nodes_01.push_back(n_05.backIt());

    vector<node_list_it_t> nodes_02;
    nodes_02.push_back(n_06.backIt());
    nodes_02.push_back(n_07.backIt());
    nodes_02.push_back(n_08.backIt());
    nodes_02.push_back(n_09.backIt());

    vector<node_list_it_t> nodes_03;
    nodes_03.push_back(n_10.backIt());
    nodes_03.push_back(n_11.backIt());
    nodes_03.push_back(n_12.backIt());
    nodes_03.push_back(n_13.backIt());
    nodes_03.push_back(n_14.backIt());
    nodes_03.push_back(n_15.backIt());
    nodes_03.push_back(n_16.backIt());

    vector<node_list_it_t> nodes_04;
    nodes_04.push_back(n_17.backIt());
    nodes_04.push_back(n_18.backIt());
    nodes_04.push_back(n_19.backIt());
    nodes_04.push_back(n_20.backIt());

    vector< vector<node_list_it_t> > nodeSetsExpected;
    nodeSetsExpected.push_back(std::move(nodes_02));
    nodeSetsExpected.push_back(std::move(nodes_01));
    nodeSetsExpected.push_back(std::move(nodes_03));
    nodeSetsExpected.push_back(std::move(nodes_04));

    EXPECT_EQ(checkNodeSets(nodeSets, nodeSetsExpected), true);

}

} // namespace Undirected

} // namespace Wailea
