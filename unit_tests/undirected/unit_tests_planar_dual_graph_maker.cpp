#include "gtest/gtest.h"
#include "undirected/planar_dual_graph_maker.hpp"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/st_numbering.hpp"

namespace Wailea {

namespace Undirected {


class PlanarDualGraphMakerTests : public ::testing::Test {

  protected:

    PlanarDualGraphMakerTests(){;};
    virtual ~PlanarDualGraphMakerTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief tests  K3
 */
TEST_F(PlanarDualGraphMakerTests, Test1) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_01 = g_1.addEdge(make_unique<Edge>(),n_03,n_01);

    vector<edge_list_it_t> inc_01;
    inc_01.push_back(e_01_02.backIt());
    inc_01.push_back(e_03_01.backIt());
    n_01.reorderIncidence(move(inc_01));
    vector<edge_list_it_t> inc_02;
    inc_02.push_back(e_02_03.backIt());
    inc_02.push_back(e_01_02.backIt());
    n_02.reorderIncidence(move(inc_02));
    vector<edge_list_it_t> inc_03;
    inc_03.push_back(e_03_01.backIt());
    inc_03.push_back(e_02_03.backIt());
    n_03.reorderIncidence(move(inc_03));

    PlanarDualGraphMaker maker;
    EmbeddedGraph emb;
    DualGraph     dual;

    maker.makeDualGraph(g_1, emb, dual);

    EXPECT_EQ(emb.numNodes(), 3);
    EXPECT_EQ(emb.numEdges(), 3);

    EXPECT_EQ(dual.numNodes(), 2);
    EXPECT_EQ(dual.numEdges(), 3);

    auto enit = emb.nodes().first;
    auto& EN1 = dynamic_cast<EmbeddedNode&>(*(*(enit)));
    EXPECT_EQ(EN1.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(), EN1.backIt());
    enit++;
    auto& EN2 = dynamic_cast<EmbeddedNode&>(*(*(enit))); 
    EXPECT_EQ(EN2.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(), EN2.backIt());
    enit++;
    auto& EN3 = dynamic_cast<EmbeddedNode&>(*(*(enit))); 
    EXPECT_EQ(EN3.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(), EN3.backIt());

    auto eeit = emb.edges().first;
    auto& EE1 = dynamic_cast<EmbeddedEdge&>(*(*(eeit)));
    EXPECT_EQ(EE1.IGBackwardLink(), e_01_02.backIt());
    eeit++;
    auto& EE2 = dynamic_cast<EmbeddedEdge&>(*(*(eeit))); 
    EXPECT_EQ(EE2.IGBackwardLink(), e_02_03.backIt());
    eeit++;
    auto& EE3 = dynamic_cast<EmbeddedEdge&>(*(*(eeit))); 
    EXPECT_EQ(EE3.IGBackwardLink(), e_03_01.backIt());

    EXPECT_EQ(EE1.mHalfEdge1.mEmbeddedEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mTheOtherHalfOn1, false);
    EXPECT_EQ(EE1.mHalfEdge1.mSrcNode, EN1.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mDstNode, EN2.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mPrevEdge, EE3.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mPrevHalfEdgeOn1, true);
    EXPECT_EQ(EE1.mHalfEdge1.mNextEdge, EE2.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mNextHalfEdgeOn1, true);

    EXPECT_EQ(EE1.mHalfEdge2.mEmbeddedEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mTheOtherHalfOn1, true);
    EXPECT_EQ(EE1.mHalfEdge2.mSrcNode, EN2.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mDstNode, EN1.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mPrevEdge, EE2.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mPrevHalfEdgeOn1, false);
    EXPECT_EQ(EE1.mHalfEdge2.mNextEdge, EE3.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mNextHalfEdgeOn1, false);

    EXPECT_EQ(EE2.mHalfEdge1.mEmbeddedEdge, EE2.backIt());
    EXPECT_EQ(EE2.mHalfEdge1.mTheOtherHalfOn1, false);
    EXPECT_EQ(EE2.mHalfEdge1.mSrcNode, EN2.backIt());
    EXPECT_EQ(EE2.mHalfEdge1.mDstNode, EN3.backIt());
    EXPECT_EQ(EE2.mHalfEdge1.mPrevEdge, EE1.backIt());
    EXPECT_EQ(EE2.mHalfEdge1.mPrevHalfEdgeOn1, true);
    EXPECT_EQ(EE2.mHalfEdge1.mNextEdge, EE3.backIt());
    EXPECT_EQ(EE2.mHalfEdge1.mNextHalfEdgeOn1, true);

    EXPECT_EQ(EE2.mHalfEdge2.mEmbeddedEdge, EE2.backIt());
    EXPECT_EQ(EE2.mHalfEdge2.mTheOtherHalfOn1, true);
    EXPECT_EQ(EE2.mHalfEdge2.mSrcNode, EN3.backIt());
    EXPECT_EQ(EE2.mHalfEdge2.mDstNode, EN2.backIt());
    EXPECT_EQ(EE2.mHalfEdge2.mPrevEdge, EE3.backIt());
    EXPECT_EQ(EE2.mHalfEdge2.mPrevHalfEdgeOn1, false);
    EXPECT_EQ(EE2.mHalfEdge2.mNextEdge, EE1.backIt());
    EXPECT_EQ(EE2.mHalfEdge2.mNextHalfEdgeOn1, false);

    EXPECT_EQ(EE3.mHalfEdge1.mEmbeddedEdge, EE3.backIt());
    EXPECT_EQ(EE3.mHalfEdge1.mTheOtherHalfOn1, false);
    EXPECT_EQ(EE3.mHalfEdge1.mSrcNode, EN3.backIt());
    EXPECT_EQ(EE3.mHalfEdge1.mDstNode, EN1.backIt());
    EXPECT_EQ(EE3.mHalfEdge1.mPrevEdge, EE2.backIt());
    EXPECT_EQ(EE3.mHalfEdge1.mPrevHalfEdgeOn1, true);
    EXPECT_EQ(EE3.mHalfEdge1.mNextEdge, EE1.backIt());
    EXPECT_EQ(EE3.mHalfEdge1.mNextHalfEdgeOn1, true);

    EXPECT_EQ(EE3.mHalfEdge2.mEmbeddedEdge, EE3.backIt());
    EXPECT_EQ(EE3.mHalfEdge2.mTheOtherHalfOn1, true);
    EXPECT_EQ(EE3.mHalfEdge2.mSrcNode, EN1.backIt());
    EXPECT_EQ(EE3.mHalfEdge2.mDstNode, EN3.backIt());
    EXPECT_EQ(EE3.mHalfEdge2.mPrevEdge, EE1.backIt());
    EXPECT_EQ(EE3.mHalfEdge2.mPrevHalfEdgeOn1, false);
    EXPECT_EQ(EE3.mHalfEdge2.mNextEdge, EE2.backIt());
    EXPECT_EQ(EE3.mHalfEdge2.mNextHalfEdgeOn1, false);

    auto eniit = EN1.incidentEdges().first;
    auto& EN1_I1 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN1_I1.backIt(), EE1.backIt());
    eniit++;
    auto& EN1_I2 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN1_I2.backIt(), EE3.backIt());

    eniit = EN2.incidentEdges().first;
    auto& EN2_I1 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN2_I1.backIt(), EE2.backIt());
    eniit++;
    auto& EN2_I2 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN2_I2.backIt(), EE1.backIt());

    eniit = EN3.incidentEdges().first;
    auto& EN3_I1 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN3_I1.backIt(), EE3.backIt());
    eniit++;
    auto& EN3_I2 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN3_I2.backIt(), EE2.backIt());


    auto deit = dual.edges().first;
    auto& DE1 = dynamic_cast<DualEdge&>(*(*(deit)));
    EXPECT_EQ(DE1.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(DE1.mEmbeddedEdge, EE1.backIt());

    deit++;
    auto& DE2 = dynamic_cast<DualEdge&>(*(*(deit)));
    EXPECT_EQ(DE2.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(DE2.mEmbeddedEdge, EE2.backIt());

    deit++;

    auto& DE3 = dynamic_cast<DualEdge&>(*(*(deit)));
    EXPECT_EQ(DE3.IGBackwardLink(), e_03_01.backIt());
    EXPECT_EQ(DE3.mEmbeddedEdge, EE3.backIt());

    auto dnit = dual.nodes().first;
    auto& F1 = dynamic_cast<EmbeddedFace&>(*(*(dnit)));
    EXPECT_EQ(F1.mCycleEdges.size(), 3);
    EXPECT_EQ(F1.mCycleHalfEdgesOn1.size(), 3);
    auto fcit1 = F1.mCycleEdges.begin();
    auto fcit2 = F1.mCycleHalfEdgesOn1.begin();
    auto fciit3 = F1.incidentEdges().first;
    EXPECT_EQ(*fcit1,  EE1.backIt());
    EXPECT_EQ(*fcit2,  true);
    EXPECT_EQ(*fciit3, DE1.backIt());
    fcit1++;fcit2++;fciit3++;
    EXPECT_EQ(*fcit1, EE2.backIt());
    EXPECT_EQ(*fcit2, true);
    EXPECT_EQ(*fciit3, DE2.backIt());
    fcit1++;fcit2++;fciit3++;
    EXPECT_EQ(*fcit1, EE3.backIt());
    EXPECT_EQ(*fcit2, true);
    EXPECT_EQ(*fciit3, DE3.backIt());

    dnit++;
    auto& F2 = dynamic_cast<EmbeddedFace&>(*(*(dnit))); 
    EXPECT_EQ(F2.mCycleEdges.size(), 3);
    EXPECT_EQ(F2.mCycleHalfEdgesOn1.size(), 3);
    fcit1 = F2.mCycleEdges.begin();
    fcit2 = F2.mCycleHalfEdgesOn1.begin();
    fciit3 = F2.incidentEdges().first;
    EXPECT_EQ(*fcit1, EE3.backIt());
    EXPECT_EQ(*fcit2, false);
    EXPECT_EQ(*fciit3, DE3.backIt());
    fcit1++;fcit2++;fciit3++;
    EXPECT_EQ(*fcit1, EE2.backIt());
    EXPECT_EQ(*fcit2, false);
    EXPECT_EQ(*fciit3, DE2.backIt());
    fcit1++;fcit2++;fciit3++;
    EXPECT_EQ(*fcit1, EE1.backIt());
    EXPECT_EQ(*fcit2, false);
    EXPECT_EQ(*fciit3, DE1.backIt());

}


/**  @brief tests dodecahedron
 */
TEST_F(PlanarDualGraphMakerTests, Test2) {

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

    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_06_15 = */g_1.addEdge(make_unique<Edge>(),n_06,n_15);
    /*auto& e_06_11 = */g_1.addEdge(make_unique<Edge>(),n_06,n_11);
    /*auto& e_05_10 = */g_1.addEdge(make_unique<Edge>(),n_05,n_10);
    /*auto& e_02_07 = */g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    /*auto& e_10_15 = */g_1.addEdge(make_unique<Edge>(),n_10,n_15);
    /*auto& e_07_11 = */g_1.addEdge(make_unique<Edge>(),n_07,n_11);
    /*auto& e_15_20 = */g_1.addEdge(make_unique<Edge>(),n_15,n_20);
    /*auto& e_11_16 = */g_1.addEdge(make_unique<Edge>(),n_11,n_16);
    /*auto& e_20_16 = */g_1.addEdge(make_unique<Edge>(),n_20,n_16);
    /*auto& e_05_04 = */g_1.addEdge(make_unique<Edge>(),n_05,n_04);
    /*auto& e_10_14 = */g_1.addEdge(make_unique<Edge>(),n_10,n_14);
    /*auto& e_20_19 = */g_1.addEdge(make_unique<Edge>(),n_20,n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(),n_16,n_17);
    /*auto& e_07_12 = */g_1.addEdge(make_unique<Edge>(),n_07,n_12);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_14_19 = */g_1.addEdge(make_unique<Edge>(),n_14,n_19);
    /*auto& e_14_09 = */g_1.addEdge(make_unique<Edge>(),n_14,n_09);
    /*auto& e_18_19 = */g_1.addEdge(make_unique<Edge>(),n_18,n_19);
    /*auto& e_18_17 = */g_1.addEdge(make_unique<Edge>(),n_18,n_17);
    /*auto& e_12_17 = */g_1.addEdge(make_unique<Edge>(),n_12,n_17);
    /*auto& e_12_08 = */g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    /*auto& e_13_18 = */g_1.addEdge(make_unique<Edge>(),n_13,n_18);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(),n_09,n_13);
    /*auto& e_13_08 = */g_1.addEdge(make_unique<Edge>(),n_13,n_08);
    /*auto& e_03_08 = */g_1.addEdge(make_unique<Edge>(),n_03,n_08);
    /*auto& e_04_09 = */g_1.addEdge(make_unique<Edge>(),n_04,n_09);
    /*auto& e_04_03 = */g_1.addEdge(make_unique<Edge>(),n_04,n_03);

    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
        for (size_t j = 0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//            cerr << "Testing [" << i << "][" << j << "]\n";                   
            STNumbering st;
            vector<node_list_it_t> st_list =
                       st.getBipolarOrientation(g_1, **TList[i], **TList[j]);
            BLPlanarityTester tester;
            tester.findEmbedding(g_1, st_list);

            PlanarDualGraphMaker maker;
            EmbeddedGraph emb;
            DualGraph     dual;
            maker.makeDualGraph(g_1, emb, dual);

            EXPECT_EQ(emb.numNodes(), 20);
            EXPECT_EQ(emb.numEdges(), 30);
            EXPECT_EQ(dual.numNodes(), 12);
            EXPECT_EQ(dual.numEdges(), 30);

        }
    }
}


/**  @brief tests minimal graph K2
 */
TEST_F(PlanarDualGraphMakerTests, Test3) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);

    PlanarDualGraphMaker maker;
    EmbeddedGraph emb;
    DualGraph     dual;

    maker.makeDualGraph(g_1, emb, dual);

    EXPECT_EQ(emb.numNodes(), 2);
    EXPECT_EQ(emb.numEdges(), 1);

    EXPECT_EQ(dual.numNodes(), 1);
    EXPECT_EQ(dual.numEdges(), 1);

    auto enit = emb.nodes().first;
    auto& EN1 = dynamic_cast<EmbeddedNode&>(*(*(enit)));
    EXPECT_EQ(EN1.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(), EN1.backIt());
    enit++;
    auto& EN2 = dynamic_cast<EmbeddedNode&>(*(*(enit))); 
    EXPECT_EQ(EN2.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(), EN2.backIt());

    auto eeit = emb.edges().first;
    auto& EE1 = dynamic_cast<EmbeddedEdge&>(*(*(eeit)));
    EXPECT_EQ(EE1.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(), EE1.backIt());

    EXPECT_EQ(EE1.mHalfEdge1.mEmbeddedEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mTheOtherHalfOn1, false);
    EXPECT_EQ(EE1.mHalfEdge1.mSrcNode, EN1.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mDstNode, EN2.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mPrevEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mPrevHalfEdgeOn1, false);
    EXPECT_EQ(EE1.mHalfEdge1.mNextEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge1.mNextHalfEdgeOn1, false);

    EXPECT_EQ(EE1.mHalfEdge2.mEmbeddedEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mTheOtherHalfOn1, true);
    EXPECT_EQ(EE1.mHalfEdge2.mSrcNode, EN2.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mDstNode, EN1.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mPrevEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mPrevHalfEdgeOn1, true);
    EXPECT_EQ(EE1.mHalfEdge2.mNextEdge, EE1.backIt());
    EXPECT_EQ(EE1.mHalfEdge2.mNextHalfEdgeOn1, true);


    auto eniit = EN1.incidentEdges().first;
    auto& EN1_I1 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN1_I1.backIt(), EE1.backIt());

    eniit = EN2.incidentEdges().first;
    auto& EN2_I1 = dynamic_cast<EmbeddedEdge&>(*(*(*eniit)));
    EXPECT_EQ(EN2_I1.backIt(), EE1.backIt());


    auto deit = dual.edges().first;
    auto& DE1 = dynamic_cast<DualEdge&>(*(*(deit)));
    EXPECT_EQ(DE1.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(DE1.mEmbeddedEdge, EE1.backIt());

    auto dnit = dual.nodes().first;
    auto& F1 = dynamic_cast<EmbeddedFace&>(*(*(dnit)));
    EXPECT_EQ(F1.mCycleEdges.size(), 2);
    EXPECT_EQ(F1.mCycleHalfEdgesOn1.size(), 2);

    auto fcit1 = F1.mCycleEdges.begin();
    auto fcit2 = F1.mCycleHalfEdgesOn1.begin();
    auto fciit3 = F1.incidentEdges().first;
    EXPECT_EQ(*fcit1,  EE1.backIt());
    EXPECT_EQ(*fcit2,  true);
    EXPECT_EQ(*fciit3, DE1.backIt());
    fcit1++;fcit2++;fciit3++;
    EXPECT_EQ(*fcit1, EE1.backIt());
    EXPECT_EQ(*fcit2, false);
    EXPECT_EQ(*fciit3, DE1.backIt());
}


/**  @brief tests minimal graph K1
 */
TEST_F(PlanarDualGraphMakerTests, Test4) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));

    PlanarDualGraphMaker maker;
    EmbeddedGraph emb;
    DualGraph     dual;

    maker.makeDualGraph(g_1, emb, dual);

    EXPECT_EQ(emb.numNodes(), 1);
    EXPECT_EQ(emb.numEdges(), 0);

    EXPECT_EQ(dual.numNodes(), 1);
    EXPECT_EQ(dual.numEdges(), 0);

    auto enit = emb.nodes().first;
    auto& EN1 = dynamic_cast<EmbeddedNode&>(*(*(enit)));
    EXPECT_EQ(EN1.degree(),  0);
    EXPECT_EQ(EN1.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(), EN1.backIt());

    auto fit = dual.nodes().first;
    auto& F1 = dynamic_cast<EmbeddedFace&>(*(*(fit)));
    EXPECT_EQ(F1.mCycleEdges.size(),  0);
    EXPECT_EQ(F1.mCycleHalfEdgesOn1.size(),  0);

}


/**  @brief tests minimal graph K0
 */
TEST_F(PlanarDualGraphMakerTests, Test5) {

    Graph g_1;

    PlanarDualGraphMaker maker;
    EmbeddedGraph emb;
    DualGraph     dual;

    maker.makeDualGraph(g_1, emb, dual);

    EXPECT_EQ(emb.numNodes(), 0);
    EXPECT_EQ(emb.numEdges(), 0);

    EXPECT_EQ(dual.numNodes(), 0);
    EXPECT_EQ(dual.numEdges(), 0);


}


/**  @brief tests small planar
 */
TEST_F(PlanarDualGraphMakerTests, Test6) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    auto& e_01_05 = g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_02 = g_1.addEdge(make_unique<Edge>(),n_04,n_02);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_06_02 = g_1.addEdge(make_unique<Edge>(),n_06,n_02);

    vector<edge_list_it_t> n_01_inc;
    n_01_inc.push_back(e_01_02.backIt());
    n_01_inc.push_back(e_01_03.backIt());
    n_01_inc.push_back(e_01_05.backIt());
    n_01.reorderIncidence(std::move(n_01_inc));

    vector<edge_list_it_t> n_02_inc;
    n_02_inc.push_back(e_06_02.backIt());
    n_02_inc.push_back(e_04_02.backIt());
    n_02_inc.push_back(e_01_02.backIt());
    n_02.reorderIncidence(std::move(n_02_inc));

    PlanarDualGraphMaker maker;
    EmbeddedGraph emb;
    DualGraph     dual;

    maker.makeDualGraph(g_1, emb, dual);

    EXPECT_EQ(emb.numNodes(), 6);
    EXPECT_EQ(emb.numEdges(), 7);

    EXPECT_EQ(dual.numNodes(), 3);
    EXPECT_EQ(dual.numEdges(), 7);

    auto enit = emb.nodes().first;
    auto& en_01 = dynamic_cast<EmbeddedNode&>(*(*enit));
    enit++;
    auto& en_02 = dynamic_cast<EmbeddedNode&>(*(*enit));
    enit++;
    auto& en_03 = dynamic_cast<EmbeddedNode&>(*(*enit));
    enit++;
    auto& en_04 = dynamic_cast<EmbeddedNode&>(*(*enit));
    enit++;
    auto& en_05 = dynamic_cast<EmbeddedNode&>(*(*enit));
    enit++;
    auto& en_06 = dynamic_cast<EmbeddedNode&>(*(*enit));

    auto eeit = emb.edges().first;
    auto& ee_01 = dynamic_cast<EmbeddedEdge&>(*(*eeit));
    eeit++;
    auto& ee_02 = dynamic_cast<EmbeddedEdge&>(*(*eeit));
    eeit++;
    auto& ee_03 = dynamic_cast<EmbeddedEdge&>(*(*eeit));
    eeit++;
    auto& ee_04 = dynamic_cast<EmbeddedEdge&>(*(*eeit));
    eeit++;
    auto& ee_05 = dynamic_cast<EmbeddedEdge&>(*(*eeit));
    eeit++;
    auto& ee_06 = dynamic_cast<EmbeddedEdge&>(*(*eeit));
    eeit++;
    auto& ee_07 = dynamic_cast<EmbeddedEdge&>(*(*eeit));

    auto fit = dual.nodes().first;
    auto& f_01 = dynamic_cast<EmbeddedFace&>(*(*fit));
    fit++;
    auto& f_02 = dynamic_cast<EmbeddedFace&>(*(*fit));
    fit++;
    auto& f_03 = dynamic_cast<EmbeddedFace&>(*(*fit));

    auto deit = dual.edges().first;
    auto& de_01 = dynamic_cast<DualEdge&>(*(*deit));
    deit++;
    auto& de_02 = dynamic_cast<DualEdge&>(*(*deit));
    deit++;
    auto& de_03 = dynamic_cast<DualEdge&>(*(*deit));
    deit++;
    auto& de_04 = dynamic_cast<DualEdge&>(*(*deit));
    deit++;
    auto& de_05 = dynamic_cast<DualEdge&>(*(*deit));
    deit++;
    auto& de_06 = dynamic_cast<DualEdge&>(*(*deit));
    deit++;
    auto& de_07 = dynamic_cast<DualEdge&>(*(*deit));

    EXPECT_EQ(en_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(en_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(en_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(en_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(en_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(en_06.IGBackwardLink(), n_06.backIt());

    EXPECT_EQ(n_01.IGForwardLink(), en_01.backIt());
    EXPECT_EQ(n_02.IGForwardLink(), en_02.backIt());
    EXPECT_EQ(n_03.IGForwardLink(), en_03.backIt());
    EXPECT_EQ(n_04.IGForwardLink(), en_04.backIt());
    EXPECT_EQ(n_05.IGForwardLink(), en_05.backIt());
    EXPECT_EQ(n_06.IGForwardLink(), en_06.backIt());

    EXPECT_EQ(ee_01.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(ee_02.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(ee_03.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ee_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(ee_05.IGBackwardLink(), e_04_02.backIt());
    EXPECT_EQ(ee_06.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(ee_07.IGBackwardLink(), e_06_02.backIt());

    EXPECT_EQ(e_01_02.IGForwardLink(), ee_01.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(), ee_02.backIt());
    EXPECT_EQ(e_01_05.IGForwardLink(), ee_03.backIt());
    EXPECT_EQ(e_03_04.IGForwardLink(), ee_04.backIt());
    EXPECT_EQ(e_04_02.IGForwardLink(), ee_05.backIt());
    EXPECT_EQ(e_05_06.IGForwardLink(), ee_06.backIt());
    EXPECT_EQ(e_06_02.IGForwardLink(), ee_07.backIt());

    EXPECT_EQ(de_01.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(de_02.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(de_03.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(de_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(de_05.IGBackwardLink(), e_04_02.backIt());
    EXPECT_EQ(de_06.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(de_07.IGBackwardLink(), e_06_02.backIt());

    EXPECT_EQ(ee_01.mDualEdge, de_01.backIt());
    EXPECT_EQ(ee_02.mDualEdge, de_02.backIt());
    EXPECT_EQ(ee_03.mDualEdge, de_03.backIt());
    EXPECT_EQ(ee_04.mDualEdge, de_04.backIt());
    EXPECT_EQ(ee_05.mDualEdge, de_05.backIt());
    EXPECT_EQ(ee_06.mDualEdge, de_06.backIt());
    EXPECT_EQ(ee_07.mDualEdge, de_07.backIt());

    EXPECT_EQ(de_01.mEmbeddedEdge, ee_01.backIt());
    EXPECT_EQ(de_02.mEmbeddedEdge, ee_02.backIt());
    EXPECT_EQ(de_03.mEmbeddedEdge, ee_03.backIt());
    EXPECT_EQ(de_04.mEmbeddedEdge, ee_04.backIt());
    EXPECT_EQ(de_05.mEmbeddedEdge, ee_05.backIt());
    EXPECT_EQ(de_06.mEmbeddedEdge, ee_06.backIt());
    EXPECT_EQ(de_07.mEmbeddedEdge, ee_07.backIt());

    EXPECT_EQ(f_01.mCycleEdges.size(), 4);
    EXPECT_EQ(f_01.mCycleHalfEdgesOn1.size(), 4);
    EXPECT_EQ(f_02.mCycleEdges.size(), 6);
    EXPECT_EQ(f_02.mCycleHalfEdgesOn1.size(), 6);
    EXPECT_EQ(f_03.mCycleEdges.size(), 4);
    EXPECT_EQ(f_03.mCycleHalfEdgesOn1.size(), 4);

    EXPECT_EQ(ee_01.mHalfEdge1.mEmbeddedEdge, ee_01.backIt());
    EXPECT_EQ(ee_01.mHalfEdge1.mTheOtherHalfOn1, false);
    EXPECT_EQ(ee_01.mHalfEdge1.mEmbeddedFace, f_01.backIt());
    EXPECT_EQ(ee_01.mHalfEdge1.mSrcNode,  en_01.backIt());
    EXPECT_EQ(ee_01.mHalfEdge1.mDstNode,  en_02.backIt());
    EXPECT_EQ(ee_01.mHalfEdge1.mPrevEdge, ee_02.backIt());// 01-03
    EXPECT_EQ(ee_01.mHalfEdge1.mPrevHalfEdgeOn1, false);
    EXPECT_EQ(ee_01.mHalfEdge1.mNextEdge, ee_05.backIt());// 02-04
    EXPECT_EQ(ee_01.mHalfEdge1.mNextHalfEdgeOn1, false);

    auto eniit = en_01.incidentEdges().first;
    EXPECT_EQ(*eniit, ee_01.backIt());
    eniit++;
    EXPECT_EQ(*eniit, ee_02.backIt());
    eniit++;
    EXPECT_EQ(*eniit, ee_03.backIt());

    eniit = en_02.incidentEdges().first;
    EXPECT_EQ(*eniit, ee_07.backIt());
    eniit++;
    EXPECT_EQ(*eniit, ee_05.backIt());
    eniit++;
    EXPECT_EQ(*eniit, ee_01.backIt());

    auto fcit = f_01.mCycleEdges.begin();
    auto fbit = f_01.mCycleHalfEdgesOn1.begin();
    EXPECT_EQ(*fcit, ee_01.backIt()); //e_01_02
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_05.backIt()); //e_04_02
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_04.backIt()); //e_03_04
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_02.backIt()); //e_01_03

    auto fiit = f_01.incidentEdges().first;
    EXPECT_EQ(*fiit, de_01.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_05.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_04.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_02.backIt());
    
    fcit = f_02.mCycleEdges.begin();
    fbit = f_02.mCycleHalfEdgesOn1.begin();
    EXPECT_EQ(*fcit, ee_02.backIt()); // 01-03
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_04.backIt()); // 03-04
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_05.backIt()); // 04-02
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_07.backIt()); // 02-06
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_06.backIt()); // 06-05
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_03.backIt()); // 05-01
    fcit++;fbit++;

    fiit = f_02.incidentEdges().first;
    EXPECT_EQ(*fiit, de_02.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_04.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_05.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_07.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_06.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_03.backIt());
    fiit++;

    
    fcit = f_03.mCycleEdges.begin();
    fbit = f_03.mCycleHalfEdgesOn1.begin();
    EXPECT_EQ(*fcit, ee_03.backIt()); // 01-05
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_06.backIt()); // 05-06
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_07.backIt()); // 06-02
    fcit++;fbit++;
    EXPECT_EQ(*fcit, ee_01.backIt()); // 02-01

    fiit = f_03.incidentEdges().first;
    EXPECT_EQ(*fiit, de_03.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_06.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_07.backIt());
    fiit++;
    EXPECT_EQ(*fiit, de_01.backIt());

}

} // namespace Undirected

} // namespace Wailea

