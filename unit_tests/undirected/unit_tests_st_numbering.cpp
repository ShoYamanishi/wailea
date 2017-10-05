#include "gtest/gtest.h"
#include "undirected/st_numbering.hpp"

namespace Wailea {

namespace Undirected { 

using DNode     = DFSNode     <Graph, ST_DFS_Node, ST_DFS_Edge>;
using DEdge     = DFSEdge     <Graph, ST_DFS_Node, ST_DFS_Edge>;
using DExplorer = DFSExplorer <Graph, ST_DFS_Node, ST_DFS_Edge>;
using DStrategy = DFSStrategy <Graph, ST_DFS_Node, ST_DFS_Edge>;
using DGraph    = DFSGraph    <Graph, ST_DFS_Node, ST_DFS_Edge>;

class STNumberingTests : public ::testing::Test {

  protected:  

    STNumberingTests(){;};
    virtual ~STNumberingTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};
};


/**  @brief test pattern 1 with detailed check on the bipolar orientation list.
 */
TEST_F(STNumberingTests, Test1) {

    Graph g_1;

    auto& n_t = g_1.addNode(make_unique<Node>());
    auto& n_g = g_1.addNode(make_unique<Node>());
    auto& n_h = g_1.addNode(make_unique<Node>());
    auto& n_f = g_1.addNode(make_unique<Node>());
    auto& n_c = g_1.addNode(make_unique<Node>());
    auto& n_b = g_1.addNode(make_unique<Node>());
    auto& n_a = g_1.addNode(make_unique<Node>());
    auto& n_e = g_1.addNode(make_unique<Node>());
    auto& n_d = g_1.addNode(make_unique<Node>());
    auto& n_s = g_1.addNode(make_unique<Node>());

    /*auto& e_t_g = */g_1.addEdge(make_unique<Edge>(),n_t, n_g);
    /*auto& e_g_h = */g_1.addEdge(make_unique<Edge>(),n_g, n_h);
    /*auto& e_h_t = */g_1.addEdge(make_unique<Edge>(),n_h, n_t);
    /*auto& e_g_f = */g_1.addEdge(make_unique<Edge>(),n_g, n_f);
    /*auto& e_f_b = */g_1.addEdge(make_unique<Edge>(),n_f, n_b);
    /*auto& e_b_a = */g_1.addEdge(make_unique<Edge>(),n_b, n_a);
    /*auto& e_a_e = */g_1.addEdge(make_unique<Edge>(),n_a, n_e);
    /*auto& e_e_g = */g_1.addEdge(make_unique<Edge>(),n_e, n_g);
    /*auto& e_a_s = */g_1.addEdge(make_unique<Edge>(),n_a, n_s);
    /*auto& e_b_s = */g_1.addEdge(make_unique<Edge>(),n_b, n_s);
    /*auto& e_f_c = */g_1.addEdge(make_unique<Edge>(),n_f, n_c);
    /*auto& e_c_d = */g_1.addEdge(make_unique<Edge>(),n_c, n_d);
    /*auto& e_d_g = */g_1.addEdge(make_unique<Edge>(),n_d, n_g);
    /*auto& e_d_s = */g_1.addEdge(make_unique<Edge>(),n_d, n_s);
    /*auto& e_c_s = */g_1.addEdge(make_unique<Edge>(),n_c, n_s);
    /*auto& e_s_t = */g_1.addEdge(make_unique<Edge>(),n_s, n_t);

    STNumbering st;
    vector<node_list_it_t> nlist = 
    st.getBipolarOrientation(g_1, n_s, n_t);
    EXPECT_EQ(nlist.size(), 10);
    auto nit = nlist.begin();
    auto& n01 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n02 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n03 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n04 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n05 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n06 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n07 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n08 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n09 = dynamic_cast<Node&>(*(*(*nit++)));
    auto& n10 = dynamic_cast<Node&>(*(*(*nit++)));
    EXPECT_EQ(&n01, &n_s);
    EXPECT_EQ(&n02, &n_a);
    EXPECT_EQ(&n03, &n_e);
    EXPECT_EQ(&n04, &n_b);
    EXPECT_EQ(&n05, &n_d);
    EXPECT_EQ(&n06, &n_c);
    EXPECT_EQ(&n07, &n_f);
    EXPECT_EQ(&n08, &n_g);
    EXPECT_EQ(&n09, &n_h);
    EXPECT_EQ(&n10, &n_t);

}


static size_t getIndexForNode(vector<node_list_it_t>&nlist, Node&nCmp)
{
    size_t index = 1;
    for (auto nit : nlist) {
        auto& n = dynamic_cast<Node&>(*(*nit));
        if (&n == &nCmp) {
            return index;
        }
        index++;
    }
    return index;
}


static bool testOrientation(Graph& g_1, vector<node_list_it_t>& nlist) {

    if(nlist.size() !=g_1.numNodes()){
        return false;
    }
    size_t index = 1;
    for (auto nit1 : nlist) {
        auto& n1 = dynamic_cast<Node&>(*(*nit1));
        
        if (index==1) {
            for (auto eit = n1.incidentEdges().first; 
                      eit != n1.incidentEdges().second;  eit++) {
                auto& e = dynamic_cast<Edge&>(*(*(*eit)));
                auto&n2 = e.adjacentNode(n1);
                size_t n2_index = getIndexForNode(nlist, n2);
                if(n2_index < index || n2_index >g_1.numNodes()){
                    return false;
                }
            }
        }
        else if (index == g_1.numNodes()) {

            for (auto eit = n1.incidentEdges().first; 
                      eit != n1.incidentEdges().second;  eit++) {
                auto& e = dynamic_cast<Edge&>(*(*(*eit)));
                auto&n2 = e.adjacentNode(n1);
                size_t n2_index = getIndexForNode(nlist, n2);
                if(n2_index < 1 || n2_index > index) {
                    return false;
                }
            }

        }
        else {

            bool lower = false;
            bool upper = false;

            for (auto eit = n1.incidentEdges().first; 
                      eit != n1.incidentEdges().second;  eit++) {
                auto& e = dynamic_cast<Edge&>(*(*(*eit)));
                auto&n2 = e.adjacentNode(n1);
                size_t n2_index = getIndexForNode(nlist, n2);
                if(n2_index < 1 || n2_index > g_1.numNodes()){
                    return false;
                }
                else if (index < n2_index) {
                    upper = true;
                }
                else if (index > n2_index) {
                    lower = true;
                }
            }
            if (lower==false ||upper==false){
                return false;
            }
        }

        index++;

    }
    return true;
}


/**  @brief test pattern 2 with multiple s t pairs.
 */
TEST_F(STNumberingTests, Test2) {

    Graph g_1;

    auto& n_t = g_1.addNode(make_unique<Node>());
    auto& n_g = g_1.addNode(make_unique<Node>());
    auto& n_h = g_1.addNode(make_unique<Node>());
    auto& n_f = g_1.addNode(make_unique<Node>());
    auto& n_c = g_1.addNode(make_unique<Node>());
    auto& n_b = g_1.addNode(make_unique<Node>());
    auto& n_a = g_1.addNode(make_unique<Node>());
    auto& n_e = g_1.addNode(make_unique<Node>());
    auto& n_d = g_1.addNode(make_unique<Node>());
    auto& n_s = g_1.addNode(make_unique<Node>());

    /*auto& e_t_g = */g_1.addEdge(make_unique<Edge>(),n_t, n_g);
    /*auto& e_g_h = */g_1.addEdge(make_unique<Edge>(),n_g, n_h);
    /*auto& e_h_t = */g_1.addEdge(make_unique<Edge>(),n_h, n_t);
    /*auto& e_g_f = */g_1.addEdge(make_unique<Edge>(),n_g, n_f);
    /*auto& e_f_b = */g_1.addEdge(make_unique<Edge>(),n_f, n_b);
    /*auto& e_b_a = */g_1.addEdge(make_unique<Edge>(),n_b, n_a);
    /*auto& e_a_e = */g_1.addEdge(make_unique<Edge>(),n_a, n_e);
    /*auto& e_e_g = */g_1.addEdge(make_unique<Edge>(),n_e, n_g);
    /*auto& e_a_s = */g_1.addEdge(make_unique<Edge>(),n_a, n_s);
    /*auto& e_b_s = */g_1.addEdge(make_unique<Edge>(),n_b, n_s);
    /*auto& e_f_c = */g_1.addEdge(make_unique<Edge>(),n_f, n_c);
    /*auto& e_c_d = */g_1.addEdge(make_unique<Edge>(),n_c, n_d);
    /*auto& e_d_g = */g_1.addEdge(make_unique<Edge>(),n_d, n_g);
    /*auto& e_d_s = */g_1.addEdge(make_unique<Edge>(),n_d, n_s);
    /*auto& e_c_s = */g_1.addEdge(make_unique<Edge>(),n_c, n_s);
    /*auto& e_s_t = */g_1.addEdge(make_unique<Edge>(),n_s, n_t);


    for (auto eit = g_1.edges().first;  eit != g_1.edges().second; eit++) {
        auto& e  = dynamic_cast<Edge&>(*(*eit));
        auto& n1 = e.incidentNode1();
        auto& n2 = e.incidentNode2();
        STNumbering st1;
        vector<node_list_it_t> nlist1
            = st1.getBipolarOrientation(g_1, n1, n2);
        EXPECT_EQ(testOrientation(g_1, nlist1),true);

        STNumbering st2;
        vector<node_list_it_t> nlist2
            = st2.getBipolarOrientation(g_1, n2, n1);
        EXPECT_EQ(testOrientation(g_1, nlist2),true);

    }

}



/**  @brief test pattern 3 with multiple s t pairs.
 */
TEST_F(STNumberingTests, Test3) {

    Graph g_1;

    auto& n_A = g_1.addNode(make_unique<Node>());
    auto& n_B = g_1.addNode(make_unique<Node>());
    auto& n_C = g_1.addNode(make_unique<Node>());
    auto& n_D = g_1.addNode(make_unique<Node>());
    auto& n_E = g_1.addNode(make_unique<Node>());
    auto& n_F = g_1.addNode(make_unique<Node>());
    auto& n_G = g_1.addNode(make_unique<Node>());
    auto& n_H = g_1.addNode(make_unique<Node>());

    /*auto& e_A_B = */g_1.addEdge(make_unique<Edge>(),n_A, n_B);
    /*auto& e_A_C = */g_1.addEdge(make_unique<Edge>(),n_A, n_C);
    /*auto& e_B_C = */g_1.addEdge(make_unique<Edge>(),n_B, n_C);
    /*auto& e_B_E = */g_1.addEdge(make_unique<Edge>(),n_B, n_E);
    /*auto& e_B_F = */g_1.addEdge(make_unique<Edge>(),n_B, n_F);
    /*auto& e_B_G = */g_1.addEdge(make_unique<Edge>(),n_B, n_G);
    /*auto& e_C_D = */g_1.addEdge(make_unique<Edge>(),n_C, n_D);
    /*auto& e_C_E = */g_1.addEdge(make_unique<Edge>(),n_C, n_E);
    /*auto& e_D_E = */g_1.addEdge(make_unique<Edge>(),n_D, n_E);
    /*auto& e_E_G = */g_1.addEdge(make_unique<Edge>(),n_E, n_G);
    /*auto& e_E_H = */g_1.addEdge(make_unique<Edge>(),n_E, n_H);
    /*auto& e_F_G = */g_1.addEdge(make_unique<Edge>(),n_F, n_G);
    /*auto& e_G_H = */g_1.addEdge(make_unique<Edge>(),n_G, n_H);

    for (auto eit = g_1.edges().first;  eit != g_1.edges().second; eit++) {
        auto& e  = dynamic_cast<Edge&>(*(*eit));
        auto& n1 = e.incidentNode1();
        auto& n2 = e.incidentNode2();
        STNumbering st1;
        vector<node_list_it_t> nlist1
            = st1.getBipolarOrientation(g_1, n1, n2);
        EXPECT_EQ(testOrientation(g_1, nlist1),true);

        STNumbering st2;
        vector<node_list_it_t> nlist2
            = st2.getBipolarOrientation(g_1, n2, n1);
        EXPECT_EQ(testOrientation(g_1, nlist2),true);

    }

}


/**  @brief test pattern 4 with multiple s t pairs.
 */
TEST_F(STNumberingTests, Test4) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());
    auto& n_05 = g_1.addNode(make_unique<Node>());
    auto& n_06 = g_1.addNode(make_unique<Node>());
    auto& n_07 = g_1.addNode(make_unique<Node>());
    auto& n_08 = g_1.addNode(make_unique<Node>());
    auto& n_09 = g_1.addNode(make_unique<Node>());
    auto& n_10 = g_1.addNode(make_unique<Node>());
    auto& n_11 = g_1.addNode(make_unique<Node>());
    auto& n_12 = g_1.addNode(make_unique<Node>());
    auto& n_13 = g_1.addNode(make_unique<Node>());
    auto& n_14 = g_1.addNode(make_unique<Node>());
    auto& n_15 = g_1.addNode(make_unique<Node>());
    auto& n_16 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01, n_02);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01, n_07);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01, n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02, n_04);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02, n_08);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03, n_05);
    /*auto& e_04_06 = */g_1.addEdge(make_unique<Edge>(),n_04, n_06);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05, n_06);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(),n_05, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(),n_06, n_08);
    /*auto& e_07_09 = */g_1.addEdge(make_unique<Edge>(),n_07, n_09);
    /*auto& e_08_13 = */g_1.addEdge(make_unique<Edge>(),n_08, n_13);
    /*auto& e_08_15 = */g_1.addEdge(make_unique<Edge>(),n_08, n_15);
    /*auto& e_13_15 = */g_1.addEdge(make_unique<Edge>(),n_13, n_15);
    /*auto& e_15_16 = */g_1.addEdge(make_unique<Edge>(),n_15, n_16);
    /*auto& e_13_14 = */g_1.addEdge(make_unique<Edge>(),n_13, n_14);
    /*auto& e_10_16 = */g_1.addEdge(make_unique<Edge>(),n_10, n_16);
    /*auto& e_14_16 = */g_1.addEdge(make_unique<Edge>(),n_14, n_16);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(),n_09, n_10);
    /*auto& e_10_14 = */g_1.addEdge(make_unique<Edge>(),n_10, n_14);
    /*auto& e_09_11 = */g_1.addEdge(make_unique<Edge>(),n_09, n_11);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10, n_11);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(),n_11, n_12);
    /*auto& e_09_12 = */g_1.addEdge(make_unique<Edge>(),n_09, n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(),n_10, n_12);

    for (auto eit = g_1.edges().first;  eit != g_1.edges().second; eit++) {
        auto& e  = dynamic_cast<Edge&>(*(*eit));
        auto& n1 = e.incidentNode1();
        auto& n2 = e.incidentNode2();
        STNumbering st1;
        vector<node_list_it_t> nlist1
            = st1.getBipolarOrientation(g_1, n1, n2);
        EXPECT_EQ(testOrientation(g_1, nlist1),true);

        STNumbering st2;
        vector<node_list_it_t> nlist2
            = st2.getBipolarOrientation(g_1, n2, n1);
        EXPECT_EQ(testOrientation(g_1, nlist2),true);

    }

}


/**  @brief test pattern 5 with multiple s t pairs.
 */
TEST_F(STNumberingTests, Test5) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());
    auto& n_05 = g_1.addNode(make_unique<Node>());
    auto& n_06 = g_1.addNode(make_unique<Node>());
    auto& n_07 = g_1.addNode(make_unique<Node>());
    auto& n_08 = g_1.addNode(make_unique<Node>());
    auto& n_09 = g_1.addNode(make_unique<Node>());
    auto& n_10 = g_1.addNode(make_unique<Node>());
    auto& n_11 = g_1.addNode(make_unique<Node>());
    auto& n_12 = g_1.addNode(make_unique<Node>());
    auto& n_13 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_03_13 = */g_1.addEdge(make_unique<Edge>(),n_03,n_13);
    /*auto& e_13_01 = */g_1.addEdge(make_unique<Edge>(),n_13,n_01);
    /*auto& e_13_02 = */g_1.addEdge(make_unique<Edge>(),n_13,n_02);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    /*auto& e_07_04 = */g_1.addEdge(make_unique<Edge>(),n_07,n_04);
    /*auto& e_07_05 = */g_1.addEdge(make_unique<Edge>(),n_07,n_05);
    /*auto& e_06_04 = */g_1.addEdge(make_unique<Edge>(),n_06,n_04);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    /*auto& e_08_01 = */g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    /*auto& e_12_01 = */g_1.addEdge(make_unique<Edge>(),n_12,n_01);
    /*auto& e_12_08 = */g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    /*auto& e_12_09 = */g_1.addEdge(make_unique<Edge>(),n_12,n_09);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    /*auto& e_11_08 = */g_1.addEdge(make_unique<Edge>(),n_11,n_08);
    /*auto& e_11_09 = */g_1.addEdge(make_unique<Edge>(),n_11,n_09);

    for (auto eit = g_1.edges().first;  eit != g_1.edges().second; eit++) {
        auto& e  = dynamic_cast<Edge&>(*(*eit));
        auto& n1 = e.incidentNode1();
        auto& n2 = e.incidentNode2();
        STNumbering st1;
        vector<node_list_it_t> nlist1
            = st1.getBipolarOrientation(g_1, n1, n2);
        EXPECT_EQ(testOrientation(g_1, nlist1),true);

        STNumbering st2;
        vector<node_list_it_t> nlist2
            = st2.getBipolarOrientation(g_1, n2, n1);
        EXPECT_EQ(testOrientation(g_1, nlist2),true);

    }

}

} // namespace Undirected

} // namespace Wailea
