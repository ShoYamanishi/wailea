#include "gtest/gtest.h"
#include "undirected/jts_bl_gmw_planarizer.hpp"
#include "undirected/jts_planarizer.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/bl_planarity_tester.hpp"

namespace Wailea {

namespace Undirected {

class JTS_BL_GMW_PlanarizerTests : public ::testing::Test {

  protected:

    JTS_BL_GMW_PlanarizerTests(){;};
    virtual ~JTS_BL_GMW_PlanarizerTests(){;};

    virtual void SetUp() {;};
    virtual void TearDown() {;};

    void findPlanarSubgraph(
        JTS_BL_GMW_Planarizer&  P,
        Graph&                  G,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    ){
        P.findPlanarSubgraph(G, planarSubgraph, removedEdges);
    }

    void maximalizePlanarSubgraph(
        JTS_BL_GMW_Planarizer&  P,
        Graph&                  G,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    ){
        P.maximalizePlanarSubgraph(G, planarSubgraph, removedEdges);
    }

    void reinsertRemovedEdges(
        JTS_BL_GMW_Planarizer&  P,
        Graph&                  G,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    ){
        P.reinsertRemovedEdges(G, planarSubgraph, removedEdges);
    }

};


/**  @brief tests k5
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test1) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    auto& e_01_05 = g_1.addEdge(make_unique<Edge>(), n_01, n_05);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_02_04 = g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    auto& e_02_05 = g_1.addEdge(make_unique<Edge>(), n_02, n_05);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    auto& e_03_05 = g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(), n_04, n_05);

    PlanarizableGraph      pg_1;
    vector<edge_list_it_t> removedEdges;
    JTS_BL_GMW_Planarizer  planarizer;
    findPlanarSubgraph(planarizer, g_1, pg_1, removedEdges);

    EXPECT_EQ(pg_1.numNodes(), 5);
    EXPECT_EQ(pg_1.numEdges(), 9);
    EXPECT_EQ(removedEdges.size(), 1);

    auto rit = removedEdges.begin();
    EXPECT_EQ(e_01_03.backIt(), *rit);

    auto peit = pg_1.edges().first;
    auto&pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_02.IGBackwardLink(), e_01_02.backIt());
    peit++;
    //auto& pe_01_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    //EXPECT_EQ(pe_01_03.IGBackwardLink(), e_01_03.backIt());
    //peit++;
    auto& pe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_04.IGBackwardLink(), e_01_04.backIt());
    peit++;
    auto& pe_01_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_05.IGBackwardLink(), e_01_05.backIt());
    peit++;
    auto& pe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_03.IGBackwardLink(), e_02_03.backIt());
    peit++;
    auto& pe_02_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_04.IGBackwardLink(), e_02_04.backIt());
    peit++;
    auto& pe_02_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_05.IGBackwardLink(), e_02_05.backIt());
    peit++;
    auto& pe_03_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_03_04.IGBackwardLink(), e_03_04.backIt());
    peit++;
    auto& pe_03_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_03_05.IGBackwardLink(), e_03_05.backIt());
    peit++;
    auto& pe_04_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_04_05.IGBackwardLink(), e_04_05.backIt());

    // pe_01_03 is removed by the jts planarizer.    
    // intentionaly remove pe_02_04 to test maximalizePlanarSubgraph().
    EXPECT_EQ(e_01_03.IGForwardLink(), pg_1.edges().second);
    EXPECT_EQ(e_01_03.utility(), JTSPlanarizer::kForwardTypeRemoved);

    e_02_04.setIGForwardLink(pg_1.edges().second);
    e_02_04.setUtility(JTSPlanarizer::kForwardTypeRemoved);
    pg_1.removeEdge(pe_02_04);

    removedEdges.push_back(e_02_04.backIt());

    maximalizePlanarSubgraph(planarizer, g_1, pg_1, removedEdges);

    EXPECT_EQ(pg_1.numNodes(), 5);
    EXPECT_EQ(pg_1.numEdges(), 9);
    EXPECT_EQ(removedEdges.size(), 1);

    rit = removedEdges.begin();
    EXPECT_EQ(e_02_04.backIt(), *rit);

    peit = pg_1.edges().first;
    auto&ppe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_01_02.IGBackwardLink(), e_01_02.backIt());
    peit++;
    auto& ppe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_01_04.IGBackwardLink(), e_01_04.backIt());
    peit++;
    auto& ppe_01_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_01_05.IGBackwardLink(), e_01_05.backIt());
    peit++;
    auto& ppe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_02_03.IGBackwardLink(), e_02_03.backIt());
    peit++;
    //auto& ppe_02_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    //EXPECT_EQ(ppe_02_04.IGBackwardLink(), e_02_04.backIt());
    //peit++;
    auto& ppe_02_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_02_05.IGBackwardLink(), e_02_05.backIt());
    peit++;
    auto& ppe_03_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_03_04.IGBackwardLink(), e_03_04.backIt());
    peit++;
    auto& ppe_03_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_03_05.IGBackwardLink(), e_03_05.backIt());
    peit++;
    auto& ppe_04_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_04_05.IGBackwardLink(), e_04_05.backIt());
    peit++;
    auto& ppe_01_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(ppe_01_03.IGBackwardLink(), e_01_03.backIt());

    EXPECT_EQ(e_02_04.IGForwardLink(), pg_1.edges().second);
    EXPECT_EQ(e_02_04.utility(), JTSPlanarizer::kForwardTypeRemoved);

    EXPECT_EQ(e_01_03.IGForwardLink(), ppe_01_03.backIt());
    EXPECT_EQ(e_01_03.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(ppe_01_03.IGBackwardLink(), e_01_03.backIt());

    reinsertRemovedEdges(planarizer, g_1, pg_1, removedEdges);

    EXPECT_EQ(pg_1.numNodes(), 6);
    EXPECT_EQ(pg_1.numEdges(), 12);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    pnit++;
    auto&pn_05 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(n_05.IGForwardLink(),   pn_05.backIt());
    EXPECT_EQ(pn_05.isVirtual(), false);
    pnit++;
    auto&pn_04 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(n_04.IGForwardLink(),   pn_04.backIt());
    EXPECT_EQ(pn_04.isVirtual(), false);
    pnit++;
    auto&pn_03 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(), false);
    pnit++;
    auto&pn_02 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    pnit++;
    auto&pn_06 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_06.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_06.isVirtual(), true);

    peit = pg_1.edges().first;
    auto&pppe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(pppe_01_02.isTerminalN1(), true);
    EXPECT_EQ(pppe_01_02.isTerminalN2(), true);
    peit++;
    auto& pppe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(pppe_01_04.isTerminalN1(), true);
    EXPECT_EQ(pppe_01_04.isTerminalN2(), true);
    peit++;
    auto& pppe_01_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_01_05.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(pppe_01_05.isTerminalN1(), true);
    EXPECT_EQ(pppe_01_05.isTerminalN2(), true);
    peit++;
    auto& pppe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(pppe_02_03.isTerminalN1(), true);
    EXPECT_EQ(pppe_02_03.isTerminalN2(), true);
    peit++;
    auto& pppe_02_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_02_05.IGBackwardLink(), e_02_05.backIt());
    EXPECT_EQ(pppe_02_05.isTerminalN1(), true);
    EXPECT_EQ(pppe_02_05.isTerminalN2(), true);
    peit++;
    auto& pppe_03_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_03_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(pppe_03_04.isTerminalN1(), true);
    EXPECT_EQ(pppe_03_04.isTerminalN2(), true);
    peit++;
    auto& pppe_03_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_03_05.IGBackwardLink(), e_03_05.backIt());
    EXPECT_EQ(pppe_03_05.isTerminalN1(), true);
    EXPECT_EQ(pppe_03_05.isTerminalN2(), true);
    peit++;
    auto& pppe_04_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_04_05.IGBackwardLink(), e_04_05.backIt());
    EXPECT_EQ(pppe_04_05.isTerminalN1(), true);
    EXPECT_EQ(pppe_04_05.isTerminalN2(), true);
    peit++;
    auto& pppe_01_06 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_01_06.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(), pppe_01_06.backIt());
    EXPECT_EQ(pppe_01_06.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pppe_01_06.incidentNode2().backIt(), pn_06.backIt());
    EXPECT_EQ(pppe_01_06.isTerminalN1(), true);
    EXPECT_EQ(pppe_01_06.isTerminalN2(), false);

    peit++;
    auto& pppe_06_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_06_03.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(pppe_06_03.incidentNode1().backIt(), pn_06.backIt());
    EXPECT_EQ(pppe_06_03.incidentNode2().backIt(), pn_03.backIt());
    EXPECT_EQ(pppe_06_03.isTerminalN1(), false);
    EXPECT_EQ(pppe_06_03.isTerminalN2(), true);

    EXPECT_EQ(pppe_01_06.chainN2(), pppe_06_03.backIt());
    EXPECT_EQ(pppe_06_03.chainN1(), pppe_01_06.backIt());

    peit++;
    auto& pppe_06_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_06_02.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(e_02_04.IGForwardLink(), pppe_06_02.backIt());
    EXPECT_EQ(pppe_06_02.incidentNode1().backIt(), pn_02.backIt());
    EXPECT_EQ(pppe_06_02.incidentNode2().backIt(), pn_06.backIt());
    EXPECT_EQ(pppe_06_02.isTerminalN1(), true);
    EXPECT_EQ(pppe_06_02.isTerminalN2(), false);
    peit++;
    auto& pppe_06_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pppe_06_04.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(pppe_06_04.incidentNode1().backIt(), pn_06.backIt());
    EXPECT_EQ(pppe_06_04.incidentNode2().backIt(), pn_04.backIt());
    EXPECT_EQ(pppe_06_04.isTerminalN1(), false);
    EXPECT_EQ(pppe_06_04.isTerminalN2(), true);

    EXPECT_EQ(pppe_06_02.chainN2(), pppe_06_04.backIt());
    EXPECT_EQ(pppe_06_04.chainN1(), pppe_06_02.backIt());

    auto bnit = pg_1.nodes().first;
    auto& S = dynamic_cast<PlanarizableNode&>(*(*bnit));
    bnit++;
    auto& T = dynamic_cast<PlanarizableNode&>(*(*bnit));

    STNumbering st;
    vector<node_list_it_t> stOrder = st.getBipolarOrientation(pg_1, S, T);
    BLPlanarityTester bl;
    bool isPlanar = bl.isPlanar(pg_1, stOrder);
    EXPECT_EQ(isPlanar, true);

}


/**  @brief tests k6
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test2) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(5));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(), n_01, n_05);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(), n_01, n_06);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(), n_02, n_05);
    /*auto& e_02_06 = */g_1.addEdge(make_unique<Edge>(), n_02, n_06);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_06 = */g_1.addEdge(make_unique<Edge>(), n_03, n_06);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    /*auto& e_04_06 = */g_1.addEdge(make_unique<Edge>(), n_04, n_06);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(), n_05, n_06);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeBiconnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(),  9);
    EXPECT_EQ(pg_1.numEdges(), 21);

    auto bnit = pg_1.nodes().first;
    auto& S = dynamic_cast<PlanarizableNode&>(*(*bnit));
    bnit++;
    auto& T = dynamic_cast<PlanarizableNode&>(*(*bnit));

    STNumbering st;
    vector<node_list_it_t> stOrder = st.getBipolarOrientation(pg_1, S, T);
    BLPlanarityTester bl;
    bool isPlanar = bl.isPlanar(pg_1, stOrder);
    EXPECT_EQ(isPlanar, true);

}


/**  @brief tests k10
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test3) {

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

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(), n_01, n_05);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(), n_01, n_06);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(), n_01, n_07);
    /*auto& e_01_08 = */g_1.addEdge(make_unique<Edge>(), n_01, n_08);
    /*auto& e_01_09 = */g_1.addEdge(make_unique<Edge>(), n_01, n_09);
    /*auto& e_01_10 = */g_1.addEdge(make_unique<Edge>(), n_01, n_10);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(), n_02, n_05);
    /*auto& e_02_06 = */g_1.addEdge(make_unique<Edge>(), n_02, n_06);
    /*auto& e_02_07 = */g_1.addEdge(make_unique<Edge>(), n_02, n_07);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(), n_02, n_08);
    /*auto& e_02_09 = */g_1.addEdge(make_unique<Edge>(), n_02, n_09);
    /*auto& e_02_10 = */g_1.addEdge(make_unique<Edge>(), n_02, n_10);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    /*auto& e_03_06 = */g_1.addEdge(make_unique<Edge>(), n_03, n_06);
    /*auto& e_03_07 = */g_1.addEdge(make_unique<Edge>(), n_03, n_07);
    /*auto& e_03_08 = */g_1.addEdge(make_unique<Edge>(), n_03, n_08);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    /*auto& e_03_10 = */g_1.addEdge(make_unique<Edge>(), n_03, n_10);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    /*auto& e_04_06 = */g_1.addEdge(make_unique<Edge>(), n_04, n_06);
    /*auto& e_04_07 = */g_1.addEdge(make_unique<Edge>(), n_04, n_07);
    /*auto& e_04_08 = */g_1.addEdge(make_unique<Edge>(), n_04, n_08);
    /*auto& e_04_09 = */g_1.addEdge(make_unique<Edge>(), n_04, n_09);
    /*auto& e_04_10 = */g_1.addEdge(make_unique<Edge>(), n_04, n_10);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(), n_05, n_06);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    /*auto& e_05_10 = */g_1.addEdge(make_unique<Edge>(), n_05, n_10);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    /*auto& e_06_09 = */g_1.addEdge(make_unique<Edge>(), n_06, n_09);
    /*auto& e_06_10 = */g_1.addEdge(make_unique<Edge>(), n_06, n_10);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    /*auto& e_07_09 = */g_1.addEdge(make_unique<Edge>(), n_07, n_09);
    /*auto& e_07_10 = */g_1.addEdge(make_unique<Edge>(), n_07, n_10);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(), n_08, n_09);
    /*auto& e_08_10 = */g_1.addEdge(make_unique<Edge>(), n_08, n_10);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(), n_09, n_10);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeBiconnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 72);
    EXPECT_EQ(pg_1.numEdges(), 169);

    //cerr << "numNodes: " << pg_1.numNodes() << "\n";
    //cerr << "numEdges: " << pg_1.numEdges() << "\n";

    auto bnit = pg_1.nodes().first;
    auto& S = dynamic_cast<PlanarizableNode&>(*(*bnit));
    bnit++;
    auto& T = dynamic_cast<PlanarizableNode&>(*(*bnit));

    STNumbering st;
    vector<node_list_it_t> stOrder = st.getBipolarOrientation(pg_1, S, T);
    BLPlanarityTester bl;
    bool isPlanar = bl.isPlanar(pg_1, stOrder);
    EXPECT_EQ(isPlanar, true);

}

/**  @brief tests k0
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test4) {

    Graph g_1;

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 0);
    EXPECT_EQ(pg_1.numEdges(), 0);

    //cerr << "numNodes: " << pg_1.numNodes() << "\n";
    //cerr << "numEdges: " << pg_1.numEdges() << "\n";

}


/**  @brief tests k1
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test5) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 1);
    EXPECT_EQ(pg_1.numEdges(), 0);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);

}


/**  @brief tests k2
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test6) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 2);
    EXPECT_EQ(pg_1.numEdges(), 1);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(pn_01.utilitySize(), 0);
    pnit++;
    auto&pn_02 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(pn_02.utilitySize(), 0);

    auto peit = pg_1.edges().first;
    auto&pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(), pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.utilitySize(), 0);
    EXPECT_EQ(pe_01_02.isTerminalN1(), true);
    EXPECT_EQ(pe_01_02.isTerminalN2(), true);


}


/**  @brief tests k3
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test7) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_03_01 = g_1.addEdge(make_unique<Edge>(), n_03, n_01);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 3);
    EXPECT_EQ(pg_1.numEdges(), 3);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(pn_01.utilitySize(), 0);
    pnit++;
    auto&pn_02 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(pn_02.utilitySize(), 0);
    pnit++;
    auto&pn_03 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(), false);
    EXPECT_EQ(pn_03.utilitySize(), 0);

    auto peit = pg_1.edges().first;
    auto&pe_03_01 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_03_01.IGBackwardLink(), e_03_01.backIt());
    EXPECT_EQ(e_03_01.IGForwardLink(), pe_03_01.backIt());
    EXPECT_EQ(pe_03_01.utilitySize(), 0);
    EXPECT_EQ(pe_03_01.isTerminalN1(), true);
    EXPECT_EQ(pe_03_01.isTerminalN2(), true);

    peit++;
    auto&pe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(), pe_02_03.backIt());
    EXPECT_EQ(pe_02_03.utilitySize(), 0);
    EXPECT_EQ(pe_02_03.isTerminalN1(), true);
    EXPECT_EQ(pe_02_03.isTerminalN2(), true);

    peit++;
    auto&pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(), pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.utilitySize(), 0);
    EXPECT_EQ(pe_01_02.isTerminalN1(), true);
    EXPECT_EQ(pe_01_02.isTerminalN2(), true);


}


/**  @brief tests k4
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test8) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_02_04 = g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 4);
    EXPECT_EQ(pg_1.numEdges(), 6);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(pn_01.utilitySize(), 0);
    pnit++;
    auto&pn_02 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(pn_02.utilitySize(), 0);
    pnit++;
    auto&pn_03 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(), false);
    EXPECT_EQ(pn_03.utilitySize(), 0);
    pnit++;
    auto&pn_04 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(n_04.IGForwardLink(),   pn_04.backIt());
    EXPECT_EQ(pn_04.isVirtual(), false);
    EXPECT_EQ(pn_04.utilitySize(), 0);

    auto peit = pg_1.edges().first;
    auto&pe_02_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_04.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(e_02_04.IGForwardLink(), pe_02_04.backIt());
    EXPECT_EQ(pe_02_04.utilitySize(), 0);
    EXPECT_EQ(pe_02_04.isTerminalN1(), true);
    EXPECT_EQ(pe_02_04.isTerminalN2(), true);

    peit++;
    auto&pe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(e_01_04.IGForwardLink(), pe_01_04.backIt());
    EXPECT_EQ(pe_01_04.utilitySize(), 0);
    EXPECT_EQ(pe_01_04.isTerminalN1(), true);
    EXPECT_EQ(pe_01_04.isTerminalN2(), true);

    peit++;
    auto&pe_03_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_03_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(e_03_04.IGForwardLink(), pe_03_04.backIt());
    EXPECT_EQ(pe_03_04.utilitySize(), 0);
    EXPECT_EQ(pe_03_04.isTerminalN1(), true);
    EXPECT_EQ(pe_03_04.isTerminalN2(), true);

    peit++;
    auto&pe_01_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_03.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(), pe_01_03.backIt());
    EXPECT_EQ(pe_01_03.utilitySize(), 0);
    EXPECT_EQ(pe_01_03.isTerminalN1(), true);
    EXPECT_EQ(pe_01_03.isTerminalN2(), true);

    peit++;
    auto&pe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(), pe_02_03.backIt());
    EXPECT_EQ(pe_02_03.utilitySize(), 0);
    EXPECT_EQ(pe_02_03.isTerminalN1(), true);
    EXPECT_EQ(pe_02_03.isTerminalN2(), true);

    peit++;
    auto&pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(), pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.utilitySize(), 0);
    EXPECT_EQ(pe_01_02.isTerminalN1(), true);
    EXPECT_EQ(pe_01_02.isTerminalN2(), true);

}


/**  @brief tests k5
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test9) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    auto& e_01_05 = g_1.addEdge(make_unique<Edge>(), n_01, n_05);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_02_04 = g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    auto& e_02_05 = g_1.addEdge(make_unique<Edge>(), n_02, n_05);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    auto& e_03_05 = g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(), n_04, n_05);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 6);
    EXPECT_EQ(pg_1.numEdges(),12);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(pn_01.utilitySize(), 0);
    pnit++;
    auto&pn_02 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(pn_02.utilitySize(), 0);
    pnit++;
    auto&pn_03 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(), false);
    EXPECT_EQ(pn_03.utilitySize(), 0);
    pnit++;
    auto&pn_04 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(n_04.IGForwardLink(),   pn_04.backIt());
    EXPECT_EQ(pn_04.isVirtual(), false);
    EXPECT_EQ(pn_04.utilitySize(), 0);
    pnit++;
    auto&pn_05 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(n_05.IGForwardLink(),   pn_05.backIt());
    EXPECT_EQ(pn_05.isVirtual(), false);
    EXPECT_EQ(pn_05.utilitySize(), 0);
    pnit++;
    auto&pn_06 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_06.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_06.isVirtual(), true);
    EXPECT_EQ(pn_06.utilitySize(), 0);

    auto peit = pg_1.edges().first;
    auto&pe_03_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_03_05.IGBackwardLink(), e_03_05.backIt());
    EXPECT_EQ(e_03_05.IGForwardLink(), pe_03_05.backIt());
    EXPECT_EQ(pe_03_05.utilitySize(), 0);
    EXPECT_EQ(pe_03_05.isTerminalN1(), true);
    EXPECT_EQ(pe_03_05.isTerminalN2(), true);

    peit++;
    auto&pe_02_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_05.IGBackwardLink(), e_02_05.backIt());
    EXPECT_EQ(e_02_05.IGForwardLink(), pe_02_05.backIt());
    EXPECT_EQ(pe_02_05.utilitySize(), 0);
    EXPECT_EQ(pe_02_05.isTerminalN1(), true);
    EXPECT_EQ(pe_02_05.isTerminalN2(), true);

    peit++;
    auto&pe_01_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_05.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(e_01_05.IGForwardLink(), pe_01_05.backIt());
    EXPECT_EQ(pe_01_05.utilitySize(), 0);
    EXPECT_EQ(pe_01_05.isTerminalN1(), true);
    EXPECT_EQ(pe_01_05.isTerminalN2(), true);

    peit++;
    auto&pe_04_05 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_04_05.IGBackwardLink(), e_04_05.backIt());
    EXPECT_EQ(e_04_05.IGForwardLink(), pe_04_05.backIt());
    EXPECT_EQ(pe_04_05.utilitySize(), 0);
    EXPECT_EQ(pe_04_05.isTerminalN1(), true);
    EXPECT_EQ(pe_04_05.isTerminalN2(), true);

    peit++;
    auto&pe_02_04_1 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_04_1.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(pe_02_04_1.utilitySize(), 0);
    EXPECT_EQ(pe_02_04_1.incidentNode2().backIt(), pn_06.backIt());
    EXPECT_EQ(pe_02_04_1.isTerminalN1(), true);
    EXPECT_EQ(pe_02_04_1.isTerminalN2(), false);

    peit++;
    auto&pe_02_04_2 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_04_2.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(e_02_04.IGForwardLink(), pe_02_04_2.backIt());
    EXPECT_EQ(pe_02_04_2.utilitySize(), 0);
    EXPECT_EQ(pe_02_04_2.incidentNode1().backIt(), pn_06.backIt());
    EXPECT_EQ(pe_02_04_2.isTerminalN1(), false);
    EXPECT_EQ(pe_02_04_2.isTerminalN2(), true);

    EXPECT_EQ(pe_02_04_1.chainN2(), pe_02_04_2.backIt());
    EXPECT_EQ(pe_02_04_2.chainN1(), pe_02_04_1.backIt());

    peit++;
    auto&pe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(e_01_04.IGForwardLink(), pe_01_04.backIt());
    EXPECT_EQ(pe_01_04.utilitySize(), 0);
    EXPECT_EQ(pe_01_04.isTerminalN1(), true);
    EXPECT_EQ(pe_01_04.isTerminalN2(), true);
    peit++;
    auto&pe_03_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_03_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(e_03_04.IGForwardLink(), pe_03_04.backIt());
    EXPECT_EQ(pe_03_04.utilitySize(), 0);
    EXPECT_EQ(pe_03_04.isTerminalN1(), true);
    EXPECT_EQ(pe_03_04.isTerminalN2(), true);
    peit++;
    auto&pe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(), pe_02_03.backIt());
    EXPECT_EQ(pe_02_03.utilitySize(), 0);
    EXPECT_EQ(pe_02_03.isTerminalN1(), true);
    EXPECT_EQ(pe_02_03.isTerminalN2(), true);

    peit++;
    auto&pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(), pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.utilitySize(), 0);
    EXPECT_EQ(pe_01_02.isTerminalN1(), true);
    EXPECT_EQ(pe_01_02.isTerminalN2(), true);
    peit++;
    auto&pe_01_03_1 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_03_1.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(pe_01_03_1.utilitySize(), 0);
    EXPECT_EQ(pe_01_03_1.incidentNode2().backIt(), pn_06.backIt());
    EXPECT_EQ(pe_01_03_1.isTerminalN1(), true);
    EXPECT_EQ(pe_01_03_1.isTerminalN2(), false);
    peit++;
    auto&pe_01_03_2 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_03_2.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(), pe_01_03_2.backIt());
    EXPECT_EQ(pe_01_03_2.utilitySize(), 0);
    EXPECT_EQ(pe_01_03_2.incidentNode1().backIt(), pn_06.backIt());
    EXPECT_EQ(pe_01_03_2.isTerminalN1(), false);
    EXPECT_EQ(pe_01_03_2.isTerminalN2(), true);

    EXPECT_EQ(pe_01_03_1.chainN2(), pe_01_03_2.backIt());
    EXPECT_EQ(pe_01_03_2.chainN1(), pe_01_03_1.backIt());

}


/**  @brief tests tree
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test10) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);

    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;
    planarizer.planarizeConnectedGraph(g_1, pg_1);

    EXPECT_EQ(pg_1.numNodes(), 4);
    EXPECT_EQ(pg_1.numEdges(), 3);

    auto pnit = pg_1.nodes().first;
    auto&pn_01 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(pn_01.utilitySize(), 0);
    pnit++;
    auto&pn_02 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(pn_02.utilitySize(), 0);
    pnit++;
    auto&pn_03 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(), false);
    EXPECT_EQ(pn_03.utilitySize(), 0);
    pnit++;
    auto&pn_04 = dynamic_cast<PlanarizableNode&>(*(*(pnit)));
    EXPECT_EQ(pn_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(n_04.IGForwardLink(),   pn_04.backIt());
    EXPECT_EQ(pn_04.isVirtual(), false);
    EXPECT_EQ(pn_04.utilitySize(), 0);

    auto peit = pg_1.edges().first;
    auto&pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(), pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.utilitySize(), 0);
    EXPECT_EQ(pe_01_02.isTerminalN1(), true);
    EXPECT_EQ(pe_01_02.isTerminalN2(), true);
    peit++;
    auto&pe_01_03 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_03.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(), pe_01_03.backIt());
    EXPECT_EQ(pe_01_03.utilitySize(), 0);
    EXPECT_EQ(pe_01_03.isTerminalN1(), true);
    EXPECT_EQ(pe_01_03.isTerminalN2(), true);
    peit++;
    auto&pe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*(peit)));
    EXPECT_EQ(pe_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(e_01_04.IGForwardLink(), pe_01_04.backIt());
    EXPECT_EQ(pe_01_04.utilitySize(), 0);
    EXPECT_EQ(pe_01_04.isTerminalN1(), true);
    EXPECT_EQ(pe_01_04.isTerminalN2(), true);
    peit++;

}

static std::string findCorrespondingEdge(
    PlanarizableGraph& g,
    edge_list_it_t eit,
    bool&          head
) {
    head = false;
    long index = 0;
    std::string indices("");
    std::string space(" ");
    for (auto peit = g.edges().first; peit != g.edges().second; peit++) {

        auto &PE = dynamic_cast<PlanarizableEdge&>(*(*peit));

        if (PE.IGBackwardLink() == eit) {

            indices = indices + std::to_string(index) + space;

//            auto &E = dynamic_cast<Edge&>(PE.IGBackwardLinkRef());
//            if (E.IGForwardLink() == PE.backIt()) {
//                return index;
//            }

        }
      
        index++;

    }
    return indices;
}

/**  @brief tests k7
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test11) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    auto& e_01_05 = g_1.addEdge(make_unique<Edge>(), n_01, n_05);
    auto& e_01_06 = g_1.addEdge(make_unique<Edge>(), n_01, n_06);
    auto& e_01_07 = g_1.addEdge(make_unique<Edge>(), n_01, n_07);

    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_02_04 = g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    auto& e_02_05 = g_1.addEdge(make_unique<Edge>(), n_02, n_05);
    auto& e_02_06 = g_1.addEdge(make_unique<Edge>(), n_02, n_06);
    auto& e_02_07 = g_1.addEdge(make_unique<Edge>(), n_02, n_07);

    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    auto& e_03_05 = g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    auto& e_03_06 = g_1.addEdge(make_unique<Edge>(), n_03, n_06);
    auto& e_03_07 = g_1.addEdge(make_unique<Edge>(), n_03, n_07);

    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    auto& e_04_06 = g_1.addEdge(make_unique<Edge>(), n_04, n_06);
    auto& e_04_07 = g_1.addEdge(make_unique<Edge>(), n_04, n_07);

    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(), n_05, n_06);
    auto& e_05_07 = g_1.addEdge(make_unique<Edge>(), n_05, n_07);

    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(), n_06, n_07);


    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;

    planarizer.planarizeBiconnectedGraph(g_1, pg_1);

//    cerr << "Nodes: " << pg_1.numNodes() << "\n";
//    cerr << "Edges: " << pg_1.numEdges() << "\n";

    auto pnit = pg_1.nodes().first;
    
    auto& pn_01 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(),     false);
    pnit++;
    auto& pn_07 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_07.IGBackwardLink(), n_07.backIt());
    EXPECT_EQ(n_07.IGForwardLink(),   pn_07.backIt());
    EXPECT_EQ(pn_07.isVirtual(),     false);
    pnit++;
    auto& pn_06 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_06.IGBackwardLink(), n_06.backIt());
    EXPECT_EQ(n_06.IGForwardLink(),   pn_06.backIt());
    EXPECT_EQ(pn_06.isVirtual(),     false);
    pnit++;
    auto& pn_05 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(n_05.IGForwardLink(),   pn_05.backIt());
    EXPECT_EQ(pn_05.isVirtual(),     false);
    pnit++;
    auto& pn_04 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(n_04.IGForwardLink(),   pn_04.backIt());
    EXPECT_EQ(pn_04.isVirtual(),     false);
    pnit++;
    auto& pn_03 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(),     false);
    pnit++;
    auto& pn_02 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(),     false);
    pnit++;
    auto& pn_08 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_08.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_08.isVirtual(),     true);
    pnit++;
    auto& pn_09 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_09.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_09.isVirtual(),     true);
    pnit++;
    auto& pn_10 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_10.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_10.isVirtual(),     true);
    pnit++;
    auto& pn_11 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_11.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_11.isVirtual(),     true);
    pnit++;
    auto& pn_12 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_12.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_12.isVirtual(),     true);
    pnit++;
    auto& pn_13 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_13.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_13.isVirtual(),     true);
    pnit++;
    auto& pn_14 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_14.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_14.isVirtual(),     true);
    pnit++;
    auto& pn_15 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_15.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_15.isVirtual(),     true);
    pnit++;
    auto& pn_16 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    EXPECT_EQ(pn_16.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(pn_16.isVirtual(),     true);

    auto peit = pg_1.edges().first;
    bool head;
    auto idx  = findCorrespondingEdge(pg_1, e_01_02.backIt() ,head);

    auto& pe_01_02_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_02_1.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(pe_01_02_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_01_02_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_01_02_1.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pe_01_02_1.incidentNode2().backIt(), pn_02.backIt());

    peit++;
    auto& pe_01_06_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_06_1.IGBackwardLink(), e_01_06.backIt());
    EXPECT_EQ(pe_01_06_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_01_06_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_01_06_1.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pe_01_06_1.incidentNode2().backIt(), pn_06.backIt());

    peit++;
    auto& pe_01_07_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_07_1.IGBackwardLink(), e_01_07.backIt());
    EXPECT_EQ(pe_01_07_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_01_07_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_01_07_1.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pe_01_07_1.incidentNode2().backIt(), pn_07.backIt());

    peit++;
    auto& pe_02_03_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_03_1.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(pe_02_03_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_02_03_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_02_03_1.incidentNode1().backIt(), pn_02.backIt());
    EXPECT_EQ(pe_02_03_1.incidentNode2().backIt(), pn_03.backIt());

    peit++;
    auto& pe_02_04_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_04_1.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(pe_02_04_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_02_04_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_02_04_1.incidentNode1().backIt(), pn_02.backIt());
    EXPECT_EQ(pe_02_04_1.incidentNode2().backIt(), pn_13.backIt());

    peit++;
    auto& pe_02_04_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_04_2.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(pe_02_04_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_02_04_2.isTerminalN2(),   false);
    EXPECT_EQ(pe_02_04_2.incidentNode1().backIt(), pn_13.backIt());
    EXPECT_EQ(pe_02_04_2.incidentNode2().backIt(), pn_12.backIt());

    EXPECT_EQ(pe_02_04_1.chainN2(), pe_02_04_2.backIt());
    EXPECT_EQ(pe_02_04_2.chainN1(), pe_02_04_1.backIt());

    peit++;
    auto& pe_02_04_3 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_04_3.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(pe_02_04_3.isTerminalN1(),   false);
    EXPECT_EQ(pe_02_04_3.isTerminalN2(),    true);
    EXPECT_EQ(pe_02_04_3.incidentNode1().backIt(), pn_12.backIt());
    EXPECT_EQ(pe_02_04_3.incidentNode2().backIt(), pn_04.backIt());
    peit++;
    auto& pe_02_05_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_05_1.IGBackwardLink(), e_02_05.backIt());
    EXPECT_EQ(pe_02_05_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_02_05_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_02_05_1.incidentNode1().backIt(), pn_02.backIt());
    EXPECT_EQ(pe_02_05_1.incidentNode2().backIt(), pn_14.backIt());
    peit++;
    auto& pe_02_05_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_05_2.IGBackwardLink(), e_02_05.backIt());
    EXPECT_EQ(pe_02_05_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_02_05_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_02_05_2.incidentNode1().backIt(), pn_14.backIt());
    EXPECT_EQ(pe_02_05_2.incidentNode2().backIt(), pn_05.backIt());

    EXPECT_EQ(pe_02_05_1.chainN2(), pe_02_05_2.backIt());
    EXPECT_EQ(pe_02_05_2.chainN1(), pe_02_05_1.backIt());

    peit++;
    auto& pe_02_06_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_06_1.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(pe_02_06_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_02_06_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_02_06_1.incidentNode1().backIt(), pn_02.backIt());
    EXPECT_EQ(pe_02_06_1.incidentNode2().backIt(), pn_08.backIt());
    peit++;
    auto& pe_02_06_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_06_2.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(pe_02_06_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_02_06_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_02_06_2.incidentNode1().backIt(), pn_08.backIt());
    EXPECT_EQ(pe_02_06_2.incidentNode2().backIt(), pn_06.backIt());

    EXPECT_EQ(pe_02_06_1.chainN2(), pe_02_06_2.backIt());
    EXPECT_EQ(pe_02_06_2.chainN1(), pe_02_06_1.backIt());

    peit++;
    auto& pe_02_07_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_07_1.IGBackwardLink(), e_02_07.backIt());
    EXPECT_EQ(pe_02_07_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_02_07_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_02_07_1.incidentNode1().backIt(), pn_02.backIt());
    EXPECT_EQ(pe_02_07_1.incidentNode2().backIt(), pn_11.backIt());
    peit++;
    auto& pe_02_07_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_02_07_2.IGBackwardLink(), e_02_07.backIt());
    EXPECT_EQ(pe_02_07_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_02_07_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_02_07_2.incidentNode1().backIt(), pn_11.backIt());
    EXPECT_EQ(pe_02_07_2.incidentNode2().backIt(), pn_07.backIt());

    EXPECT_EQ(pe_02_07_1.chainN2(), pe_02_07_2.backIt());
    EXPECT_EQ(pe_02_07_2.chainN1(), pe_02_07_1.backIt());

    peit++;
    auto& pe_03_04_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_04_1.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(pe_03_04_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_03_04_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_03_04_1.incidentNode1().backIt(), pn_03.backIt());
    EXPECT_EQ(pe_03_04_1.incidentNode2().backIt(), pn_04.backIt());

    peit++;
    auto& pe_03_06_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_06_1.IGBackwardLink(), e_03_06.backIt());
    EXPECT_EQ(pe_03_06_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_03_06_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_03_06_1.incidentNode1().backIt(), pn_03.backIt());
    EXPECT_EQ(pe_03_06_1.incidentNode2().backIt(), pn_06.backIt());

    peit++;
    auto& pe_04_05_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_04_05_1.IGBackwardLink(), e_04_05.backIt());
    EXPECT_EQ(pe_04_05_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_04_05_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_04_05_1.incidentNode1().backIt(), pn_04.backIt());
    EXPECT_EQ(pe_04_05_1.incidentNode2().backIt(), pn_05.backIt());

    peit++;
    auto& pe_04_06_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_04_06_1.IGBackwardLink(), e_04_06.backIt());
    EXPECT_EQ(pe_04_06_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_04_06_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_04_06_1.incidentNode1().backIt(), pn_04.backIt());
    EXPECT_EQ(pe_04_06_1.incidentNode2().backIt(), pn_06.backIt());

    peit++;
    auto& pe_05_06_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_05_06_1.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(pe_05_06_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_05_06_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_05_06_1.incidentNode1().backIt(), pn_05.backIt());
    EXPECT_EQ(pe_05_06_1.incidentNode2().backIt(), pn_16.backIt());
    peit++;
    auto& pe_05_06_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_05_06_2.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(pe_05_06_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_05_06_2.isTerminalN2(),   false);
    EXPECT_EQ(pe_05_06_2.incidentNode1().backIt(), pn_16.backIt());
    EXPECT_EQ(pe_05_06_2.incidentNode2().backIt(), pn_10.backIt());
    peit++;
    auto& pe_05_06_3 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_05_06_3.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(pe_05_06_3.isTerminalN1(),   false);
    EXPECT_EQ(pe_05_06_3.isTerminalN2(),    true);
    EXPECT_EQ(pe_05_06_3.incidentNode1().backIt(), pn_10.backIt());
    EXPECT_EQ(pe_05_06_3.incidentNode2().backIt(), pn_06.backIt());

    EXPECT_EQ(pe_05_06_1.chainN2(), pe_05_06_2.backIt());
    EXPECT_EQ(pe_05_06_2.chainN1(), pe_05_06_1.backIt());

    EXPECT_EQ(pe_05_06_2.chainN2(), pe_05_06_3.backIt());
    EXPECT_EQ(pe_05_06_3.chainN1(), pe_05_06_2.backIt());//<-

    peit++;
    auto& pe_05_07_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_05_07_1.IGBackwardLink(), e_05_07.backIt());
    EXPECT_EQ(pe_05_07_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_05_07_1.isTerminalN2(),    true);
    EXPECT_EQ(pe_05_07_1.incidentNode1().backIt(), pn_05.backIt());
    EXPECT_EQ(pe_05_07_1.incidentNode2().backIt(), pn_07.backIt());

    peit++;
    auto& pe_06_07_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_06_07_1.IGBackwardLink(), e_06_07.backIt());
    EXPECT_EQ(pe_06_07_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_06_07_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_06_07_1.incidentNode1().backIt(), pn_06.backIt());
    EXPECT_EQ(pe_06_07_1.incidentNode2().backIt(), pn_09.backIt());
    peit++;
    auto& pe_06_07_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_06_07_2.IGBackwardLink(), e_06_07.backIt());
    EXPECT_EQ(pe_06_07_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_06_07_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_06_07_2.incidentNode1().backIt(), pn_09.backIt());
    EXPECT_EQ(pe_06_07_2.incidentNode2().backIt(), pn_07.backIt());

    EXPECT_EQ(pe_06_07_1.chainN2(), pe_06_07_2.backIt());
    EXPECT_EQ(pe_06_07_2.chainN1(), pe_06_07_1.backIt());

    peit++;
    auto& pe_01_03_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_03_1.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(pe_01_03_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_01_03_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_01_03_1.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pe_01_03_1.incidentNode2().backIt(), pn_08.backIt());

    peit++;
    auto& pe_01_03_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_03_2.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(pe_01_03_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_01_03_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_01_03_2.incidentNode1().backIt(), pn_08.backIt());
    EXPECT_EQ(pe_01_03_2.incidentNode2().backIt(), pn_03.backIt());

    EXPECT_EQ(pe_01_03_1.chainN2(), pe_01_03_2.backIt());
    EXPECT_EQ(pe_01_03_2.chainN1(), pe_01_03_1.backIt());

    peit++;
    auto& pe_01_04_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_04_1.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(pe_01_04_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_01_04_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_01_04_1.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pe_01_04_1.incidentNode2().backIt(), pn_09.backIt());
    peit++;
    auto& pe_01_04_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_04_2.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(pe_01_04_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_01_04_2.isTerminalN2(),   false);
    EXPECT_EQ(pe_01_04_2.incidentNode1().backIt(), pn_09.backIt());
    EXPECT_EQ(pe_01_04_2.incidentNode2().backIt(), pn_10.backIt());
    peit++;
    auto& pe_01_04_3 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_04_3.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(pe_01_04_3.isTerminalN1(),   false);
    EXPECT_EQ(pe_01_04_3.isTerminalN2(),    true);
    EXPECT_EQ(pe_01_04_3.incidentNode1().backIt(), pn_10.backIt());
    EXPECT_EQ(pe_01_04_3.incidentNode2().backIt(), pn_04.backIt());


    EXPECT_EQ(pe_01_04_1.chainN2(), pe_01_04_2.backIt());
    EXPECT_EQ(pe_01_04_2.chainN1(), pe_01_04_1.backIt());

    EXPECT_EQ(pe_01_04_2.chainN2(), pe_01_04_3.backIt());
    EXPECT_EQ(pe_01_04_3.chainN1(), pe_01_04_2.backIt());

    peit++;
    auto& pe_01_05_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_05_1.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(pe_01_05_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_01_05_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_01_05_1.incidentNode1().backIt(), pn_01.backIt());
    EXPECT_EQ(pe_01_05_1.incidentNode2().backIt(), pn_11.backIt());

    peit++;
    auto& pe_01_05_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_05_2.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(pe_01_05_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_01_05_2.isTerminalN2(),   false);
    EXPECT_EQ(pe_01_05_2.incidentNode1().backIt(), pn_11.backIt());
    EXPECT_EQ(pe_01_05_2.incidentNode2().backIt(), pn_15.backIt());

    peit++;
    auto& pe_01_05_3 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_01_05_3.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(pe_01_05_3.isTerminalN1(),   false);
    EXPECT_EQ(pe_01_05_3.isTerminalN2(),    true);
    EXPECT_EQ(pe_01_05_3.incidentNode1().backIt(), pn_15.backIt());
    EXPECT_EQ(pe_01_05_3.incidentNode2().backIt(), pn_05.backIt());


    EXPECT_EQ(pe_01_05_1.chainN2(), pe_01_05_2.backIt());//<-
    EXPECT_EQ(pe_01_05_2.chainN1(), pe_01_05_1.backIt());

    EXPECT_EQ(pe_01_05_2.chainN2(), pe_01_05_3.backIt());
    EXPECT_EQ(pe_01_05_3.chainN1(), pe_01_05_2.backIt());

    peit++;
    auto& pe_03_05_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_05_1.IGBackwardLink(), e_03_05.backIt());
    EXPECT_EQ(pe_03_05_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_03_05_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_03_05_1.incidentNode1().backIt(), pn_03.backIt());
    EXPECT_EQ(pe_03_05_1.incidentNode2().backIt(), pn_12.backIt());

    peit++;
    auto& pe_03_05_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_05_2.IGBackwardLink(), e_03_05.backIt());
    EXPECT_EQ(pe_03_05_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_03_05_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_03_05_2.incidentNode1().backIt(), pn_12.backIt());
    EXPECT_EQ(pe_03_05_2.incidentNode2().backIt(), pn_05.backIt());

    EXPECT_EQ(pe_03_05_1.chainN2(), pe_03_05_2.backIt());
    EXPECT_EQ(pe_03_05_2.chainN1(), pe_03_05_1.backIt());

    peit++;
    auto& pe_03_07_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_07_1.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(pe_03_07_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_03_07_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_03_07_1.incidentNode1().backIt(), pn_03.backIt());
    EXPECT_EQ(pe_03_07_1.incidentNode2().backIt(), pn_13.backIt());

    peit++;
    auto& pe_03_07_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_07_2.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(pe_03_07_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_03_07_2.isTerminalN2(),   false);
    EXPECT_EQ(pe_03_07_2.incidentNode1().backIt(), pn_13.backIt());
    EXPECT_EQ(pe_03_07_2.incidentNode2().backIt(), pn_14.backIt());
    peit++;
    auto& pe_03_07_3 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_07_3.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(pe_03_07_3.isTerminalN1(),   false);
    EXPECT_EQ(pe_03_07_3.isTerminalN2(),   false);
    EXPECT_EQ(pe_03_07_3.incidentNode1().backIt(), pn_14.backIt());
    EXPECT_EQ(pe_03_07_3.incidentNode2().backIt(), pn_15.backIt());

    peit++;
    auto& pe_03_07_4 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_03_07_4.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(pe_03_07_4.isTerminalN1(),   false);
    EXPECT_EQ(pe_03_07_4.isTerminalN2(),    true);
    EXPECT_EQ(pe_03_07_4.incidentNode1().backIt(), pn_15.backIt());
    EXPECT_EQ(pe_03_07_4.incidentNode2().backIt(), pn_07.backIt());
    peit++;

    EXPECT_EQ(pe_03_07_1.chainN2(), pe_03_07_2.backIt());
    EXPECT_EQ(pe_03_07_2.chainN1(), pe_03_07_1.backIt());

    EXPECT_EQ(pe_03_07_2.chainN2(), pe_03_07_3.backIt());
    EXPECT_EQ(pe_03_07_3.chainN1(), pe_03_07_2.backIt());

    EXPECT_EQ(pe_03_07_3.chainN2(), pe_03_07_4.backIt());
    EXPECT_EQ(pe_03_07_4.chainN1(), pe_03_07_3.backIt());

    auto& pe_04_07_1 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_04_07_1.IGBackwardLink(), e_04_07.backIt());
    EXPECT_EQ(pe_04_07_1.isTerminalN1(),    true);
    EXPECT_EQ(pe_04_07_1.isTerminalN2(),   false);
    EXPECT_EQ(pe_04_07_1.incidentNode1().backIt(), pn_04.backIt());
    EXPECT_EQ(pe_04_07_1.incidentNode2().backIt(), pn_16.backIt());
    peit++;
    auto& pe_04_07_2 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    EXPECT_EQ(pe_04_07_2.IGBackwardLink(), e_04_07.backIt());
    EXPECT_EQ(pe_04_07_2.isTerminalN1(),   false);
    EXPECT_EQ(pe_04_07_2.isTerminalN2(),    true);
    EXPECT_EQ(pe_04_07_2.incidentNode1().backIt(), pn_16.backIt());
    EXPECT_EQ(pe_04_07_2.incidentNode2().backIt(), pn_07.backIt());

    EXPECT_EQ(pe_04_07_1.chainN2(), pe_04_07_2.backIt());
    EXPECT_EQ(pe_04_07_2.chainN1(), pe_04_07_1.backIt());

    peit++;
    EXPECT_EQ(peit, pg_1.edges().second);

    auto bnit = pg_1.nodes().first;
    auto& S = dynamic_cast<PlanarizableNode&>(*(*bnit));
    bnit++;
    auto& T = dynamic_cast<PlanarizableNode&>(*(*bnit));

    STNumbering st;
    vector<node_list_it_t> stOrder = st.getBipolarOrientation(pg_1, S, T);

    BLPlanarityTester bl;
    bool isPlanar = bl.isPlanar(pg_1, stOrder);
    EXPECT_EQ(isPlanar, true);


/*
    auto& pe_01_02_1
    auto& pe_01_06_1
    auto& pe_01_07_1
    auto& pe_02_03_1
    auto& pe_02_04_1
    auto& pe_02_04_2
    auto& pe_02_04_3
    auto& pe_02_05_1
    auto& pe_02_05_2
    auto& pe_02_06_1
    auto& pe_02_06_2
    auto& pe_02_07_1
    auto& pe_02_07_2
    auto& pe_03_04_1
    auto& pe_03_06_1
    auto& pe_04_05_1
    auto& pe_04_06_1
    auto& pe_05_06_1
    auto& pe_05_06_2
    auto& pe_05_06_3
    auto& pe_05_07_1
    auto& pe_06_07_1
    auto& pe_06_07_2
    auto& pe_01_03_1
    auto& pe_01_03_2
    auto& pe_01_04_1
    auto& pe_01_04_2
    auto& pe_01_04_3
    auto& pe_01_05_1
    auto& pe_01_05_2
    auto& pe_01_05_3
    auto& pe_03_05_1
    auto& pe_03_05_2
    auto& pe_03_07_1
    auto& pe_03_07_2
    auto& pe_03_07_3
    auto& pe_03_07_4
    auto& pe_04_07_1
    auto& pe_04_07_2
*/




/*
    cerr << "IDX 01_02: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_01_03.backIt() ,head);
    cerr << "IDX 01_03: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_01_04.backIt() ,head);
    cerr << "IDX 01_04: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_01_05.backIt() ,head);
    cerr << "IDX 01_05: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_01_06.backIt() ,head);
    cerr << "IDX 01_06: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_01_07.backIt() ,head);
    cerr << "IDX 01_07: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_02_03.backIt() ,head);
    cerr << "IDX 02_03: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_02_04.backIt() ,head);
    cerr << "IDX 02_04: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_02_05.backIt() ,head);
    cerr << "IDX 02_05: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_02_06.backIt() ,head);
    cerr << "IDX 02_06: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_02_07.backIt() ,head);
    cerr << "IDX 02_07: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_03_04.backIt() ,head);
    cerr << "IDX 03_04: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_03_05.backIt() ,head);
    cerr << "IDX 03_05: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_03_06.backIt() ,head);
    cerr << "IDX 03_06: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_03_07.backIt() ,head);
    cerr << "IDX 03_07: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_04_05.backIt() ,head);
    cerr << "IDX 04_05: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_04_06.backIt() ,head);
    cerr << "IDX 04_06: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_04_07.backIt() ,head);
    cerr << "IDX 04_07: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_05_06.backIt() ,head);
    cerr << "IDX 05_06: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_05_07.backIt() ,head);
    cerr << "IDX 05_07: " << idx << "\n";
    idx  = findCorrespondingEdge(pg_1, e_06_07.backIt() ,head);
    cerr << "IDX 06_07: " << idx << "\n";



IDX 01_02: 0 
IDX 01_03: 23 24 
IDX 01_04: 25 26 27 
IDX 01_05: 28 29 30 
IDX 01_06: 1 
IDX 01_07: 2 
IDX 02_03: 3 
IDX 02_04: 4 5 6 
IDX 02_05: 7 8 
IDX 02_06: 9 10 
IDX 02_07: 11 12 
IDX 03_04: 13 
IDX 03_05: 31 32 
IDX 03_06: 14 
IDX 03_07: 33 34 35 36 
IDX 04_05: 15 
IDX 04_06: 16 
IDX 04_07: 37 38 
IDX 05_06: 17 18 19 
IDX 05_07: 20 
IDX 06_07: 21 22 
*/


}




/**  @brief tests k7
 */
TEST_F(JTS_BL_GMW_PlanarizerTests, Test12) {

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

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(), n_01, n_04);
    auto& e_01_05 = g_1.addEdge(make_unique<Edge>(), n_01, n_05);
    auto& e_01_06 = g_1.addEdge(make_unique<Edge>(), n_01, n_06);
    auto& e_01_07 = g_1.addEdge(make_unique<Edge>(), n_01, n_07);
    auto& e_01_08 = g_1.addEdge(make_unique<Edge>(), n_01, n_08);
    auto& e_01_09 = g_1.addEdge(make_unique<Edge>(), n_01, n_09);
    auto& e_01_10 = g_1.addEdge(make_unique<Edge>(), n_01, n_10);

    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_02_04 = g_1.addEdge(make_unique<Edge>(), n_02, n_04);
    auto& e_02_05 = g_1.addEdge(make_unique<Edge>(), n_02, n_05);
    auto& e_02_06 = g_1.addEdge(make_unique<Edge>(), n_02, n_06);
    auto& e_02_07 = g_1.addEdge(make_unique<Edge>(), n_02, n_07);
    auto& e_02_08 = g_1.addEdge(make_unique<Edge>(), n_02, n_08);
    auto& e_02_09 = g_1.addEdge(make_unique<Edge>(), n_02, n_09);
    auto& e_02_10 = g_1.addEdge(make_unique<Edge>(), n_02, n_10);

    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    auto& e_03_05 = g_1.addEdge(make_unique<Edge>(), n_03, n_05);
    auto& e_03_06 = g_1.addEdge(make_unique<Edge>(), n_03, n_06);
    auto& e_03_07 = g_1.addEdge(make_unique<Edge>(), n_03, n_07);
    auto& e_03_08 = g_1.addEdge(make_unique<Edge>(), n_03, n_08);
    auto& e_03_09 = g_1.addEdge(make_unique<Edge>(), n_03, n_09);
    auto& e_03_10 = g_1.addEdge(make_unique<Edge>(), n_03, n_10);

    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    auto& e_04_06 = g_1.addEdge(make_unique<Edge>(), n_04, n_06);
    auto& e_04_07 = g_1.addEdge(make_unique<Edge>(), n_04, n_07);
    auto& e_04_08 = g_1.addEdge(make_unique<Edge>(), n_04, n_08);
    auto& e_04_09 = g_1.addEdge(make_unique<Edge>(), n_04, n_09);
    auto& e_04_10 = g_1.addEdge(make_unique<Edge>(), n_04, n_10);

    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(), n_05, n_06);
    auto& e_05_07 = g_1.addEdge(make_unique<Edge>(), n_05, n_07);
    auto& e_05_08 = g_1.addEdge(make_unique<Edge>(), n_05, n_08);
    auto& e_05_09 = g_1.addEdge(make_unique<Edge>(), n_05, n_09);
    auto& e_05_10 = g_1.addEdge(make_unique<Edge>(), n_05, n_10);

    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(), n_06, n_07);
    auto& e_06_08 = g_1.addEdge(make_unique<Edge>(), n_06, n_08);
    auto& e_06_09 = g_1.addEdge(make_unique<Edge>(), n_06, n_09);
    auto& e_06_10 = g_1.addEdge(make_unique<Edge>(), n_06, n_10);

    auto& e_07_08 = g_1.addEdge(make_unique<Edge>(), n_07, n_08);
    auto& e_07_09 = g_1.addEdge(make_unique<Edge>(), n_07, n_09);
    auto& e_07_10 = g_1.addEdge(make_unique<Edge>(), n_07, n_10);

    auto& e_08_09 = g_1.addEdge(make_unique<Edge>(), n_08, n_09);
    auto& e_08_10 = g_1.addEdge(make_unique<Edge>(), n_08, n_10);

    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(), n_09, n_10);


    PlanarizableGraph      pg_1;
    JTS_BL_GMW_Planarizer  planarizer;

    planarizer.planarizeConnectedGraph(g_1, pg_1);

//    cerr << "Nodes: " << pg_1.numNodes() << "\n";
//    cerr << "Edges: " << pg_1.numEdges() << "\n";

    auto bnit = pg_1.nodes().first;
    auto& S = dynamic_cast<PlanarizableNode&>(*(*bnit));
    bnit++;
    auto& T = dynamic_cast<PlanarizableNode&>(*(*bnit));

    STNumbering st;
    vector<node_list_it_t> stOrder = st.getBipolarOrientation(pg_1, S, T);

    BLPlanarityTester bl;
    bool isPlanar = bl.isPlanar(pg_1, stOrder);
    EXPECT_EQ(isPlanar, true);

}

} // namespace Undirected

} // namespace Wailea

