#include "gtest/gtest.h"
#include "undirected/spqr_decomposer.hpp"

namespace Wailea {

namespace Undirected { 

using DExplorer = DFSExplorer <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DStrategy = DFSStrategy <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DGraph    = DFSGraph    <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DNode     = DFSNode     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DEdge     = DFSEdge     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;

class SPQRProc2Tests : public ::testing::Test {

  protected:  

    SPQRProc2Tests(){;};
    virtual ~SPQRProc2Tests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    node_list_it_t copyInputGraph(SPQRDecomposer& decomp,
                            Graph& src, Graph& dst, node_list_it_t startNode){
        return decomp.copyInputGraph(src,dst,startNode);
    }

    void sortAdjacency(SPQRDecomposer& decomp, Graph& g){
        decomp.sortAdjacency(g);
    }

};

/**  @brief tests on the frmous representative graph 1.
 */
TEST_F(SPQRProc2Tests, Test1) {

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

    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                      (nc_01));

    sortAdjacency(decomposer, g_2);

    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);


    auto eit = g_2.edges().first;
    auto& ec_01_02 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_02_03 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_03_13 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_13_01 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_13_02 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_03_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_04_05 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_05_06 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_06_07 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_07_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_07_05 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_06_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_05_08 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_08_01 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_08_09 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_09_10 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_10_12 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_12_01 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_12_08 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_12_09 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_10_11 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_11_08 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_11_09 = dynamic_cast<DEdge&>(*(*eit++));

    auto& nc_02 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_03 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_04 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_05 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_06 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_07 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_08 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_09 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_10 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_11 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_12 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_13 = dynamic_cast<DNode&>(*(*nit++));

    EXPECT_EQ(nc_01.degree(), 4);
    auto lit = nc_01.incidentEdges().first;
    auto& nc_01_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_1, &ec_01_02);
    auto& nc_01_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_2, &ec_13_01);
    auto& nc_01_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_3, &ec_08_01);
    auto& nc_01_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_4, &ec_12_01);

    EXPECT_EQ(nc_02.degree(), 3);
    lit = nc_02.incidentEdges().first;
    auto& nc_02_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_1, &ec_01_02);
    auto& nc_02_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_2, &ec_02_03);
    auto& nc_02_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_3, &ec_13_02);

    EXPECT_EQ(nc_03.degree(), 3);
    lit = nc_03.incidentEdges().first;
    auto& nc_03_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_1, &ec_03_13);
    auto& nc_03_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_2, &ec_02_03);
    auto& nc_03_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_3, &ec_03_04);

    EXPECT_EQ(nc_13.degree(), 3);
    lit = nc_13.incidentEdges().first;
    auto& nc_13_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_1, &ec_03_13);
    auto& nc_13_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_2, &ec_13_01);
    auto& nc_13_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_3, &ec_13_02);

    EXPECT_EQ(nc_04.degree(), 4);
    lit = nc_04.incidentEdges().first;
    auto& nc_04_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_1, &ec_03_04);
    auto& nc_04_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_2, &ec_04_05);
    auto& nc_04_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_3, &ec_07_04);
    auto& nc_04_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_4, &ec_06_04);

    EXPECT_EQ(nc_05.degree(), 4);
    lit = nc_05.incidentEdges().first;
    auto& nc_05_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_1, &ec_04_05);
    auto& nc_05_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_2, &ec_05_08);
    auto& nc_05_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_3, &ec_05_06);
    auto& nc_05_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_4, &ec_07_05);

    EXPECT_EQ(nc_06.degree(), 3);
    lit = nc_06.incidentEdges().first;
    auto& nc_06_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_1, &ec_06_07);
    auto& nc_06_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_2, &ec_05_06);
    auto& nc_06_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_3, &ec_06_04);

    EXPECT_EQ(nc_07.degree(), 3);
    lit = nc_07.incidentEdges().first;
    auto& nc_07_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_1, &ec_06_07);
    auto& nc_07_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_2, &ec_07_04);
    auto& nc_07_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_3, &ec_07_05);

    EXPECT_EQ(nc_08.degree(), 5);
    lit = nc_08.incidentEdges().first;
    auto& nc_08_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_1, &ec_05_08);
    auto& nc_08_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_2, &ec_08_09);
    auto& nc_08_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_3, &ec_08_01);
    auto& nc_08_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_4, &ec_12_08);
    auto& nc_08_5 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_5, &ec_11_08);

    EXPECT_EQ(nc_09.degree(), 4);
    lit = nc_09.incidentEdges().first;
    auto& nc_09_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_1, &ec_09_10);
    auto& nc_09_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_2, &ec_08_09);
    auto& nc_09_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_3, &ec_12_09);
    auto& nc_09_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_4, &ec_11_09);

    EXPECT_EQ(nc_10.degree(), 3);
    lit = nc_10.incidentEdges().first;
    auto& nc_10_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_1, &ec_09_10);
    auto& nc_10_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_2, &ec_10_12);
    auto& nc_10_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_3, &ec_10_11);

    EXPECT_EQ(nc_11.degree(), 3);
    lit = nc_11.incidentEdges().first;
    auto& nc_11_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_1, &ec_10_11);
    auto& nc_11_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_2, &ec_11_08);
    auto& nc_11_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_3, &ec_11_09);

    EXPECT_EQ(nc_12.degree(), 4);
    lit = nc_12.incidentEdges().first;
    auto& nc_12_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_1, &ec_10_12);
    auto& nc_12_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_2, &ec_12_01);
    auto& nc_12_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_3, &ec_12_08);
    auto& nc_12_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_4, &ec_12_09);

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    EXPECT_EQ(nc_01.mNum, 1);
    EXPECT_EQ(nc_02.mNum, 2);
    EXPECT_EQ(nc_03.mNum, 3);
    EXPECT_EQ(nc_13.mNum,13);
    EXPECT_EQ(nc_04.mNum, 4);
    EXPECT_EQ(nc_05.mNum, 5);
    EXPECT_EQ(nc_06.mNum, 6);
    EXPECT_EQ(nc_07.mNum, 7);
    EXPECT_EQ(nc_08.mNum, 8);
    EXPECT_EQ(nc_09.mNum, 9);
    EXPECT_EQ(nc_10.mNum,10);
    EXPECT_EQ(nc_12.mNum,12);
    EXPECT_EQ(nc_11.mNum,11);

    EXPECT_EQ(nc_01.mLowPt1, 1);
    EXPECT_EQ(nc_02.mLowPt1, 1);
    EXPECT_EQ(nc_03.mLowPt1, 1);
    EXPECT_EQ(nc_13.mLowPt1, 1);
    EXPECT_EQ(nc_04.mLowPt1, 1);
    EXPECT_EQ(nc_05.mLowPt1, 1);
    EXPECT_EQ(nc_06.mLowPt1, 4);
    EXPECT_EQ(nc_07.mLowPt1, 4);
    EXPECT_EQ(nc_08.mLowPt1, 1);
    EXPECT_EQ(nc_09.mLowPt1, 1);
    EXPECT_EQ(nc_10.mLowPt1, 1);
    EXPECT_EQ(nc_12.mLowPt1, 1);
    EXPECT_EQ(nc_11.mLowPt1, 8);

    EXPECT_EQ(nc_01.mLowPt2, 1);
    EXPECT_EQ(nc_02.mLowPt2, 2);
    EXPECT_EQ(nc_03.mLowPt2, 2);
    EXPECT_EQ(nc_13.mLowPt2, 2);
    EXPECT_EQ(nc_04.mLowPt2, 4);
    EXPECT_EQ(nc_05.mLowPt2, 4);
    EXPECT_EQ(nc_06.mLowPt2, 5);
    EXPECT_EQ(nc_07.mLowPt2, 5);
    EXPECT_EQ(nc_08.mLowPt2, 8);
    EXPECT_EQ(nc_09.mLowPt2, 8);
    EXPECT_EQ(nc_10.mLowPt2, 8);
    EXPECT_EQ(nc_12.mLowPt2, 8);
    EXPECT_EQ(nc_11.mLowPt2, 9);

    EXPECT_EQ(nc_01.mHighPoints.size(),3);
    auto  nc_01_h01it = nc_01.mHighPoints.begin();
    auto& nc_01_h01   = dynamic_cast<DNode&>(*(*(*nc_01_h01it)));
    EXPECT_EQ(&nc_13, &nc_01_h01);
    auto  nc_01_h02it = nc_01.mHighPoints.begin();
    nc_01_h02it++;
    auto& nc_01_h02   = dynamic_cast<DNode&>(*(*(*nc_01_h02it)));
    EXPECT_EQ(&nc_12, &nc_01_h02);
    auto  nc_01_h03it = nc_01.mHighPoints.begin();
    nc_01_h03it++;nc_01_h03it++;
    auto& nc_01_h03   = dynamic_cast<DNode&>(*(*(*nc_01_h03it)));
    EXPECT_EQ(&nc_08, &nc_01_h03);

    EXPECT_EQ(nc_02.mHighPoints.size(),1);
    auto  nc_02_h01it = nc_02.mHighPoints.begin();
    auto& nc_02_h01   = dynamic_cast<DNode&>(*(*(*nc_02_h01it)));
    EXPECT_EQ(&nc_13, &nc_02_h01);

    EXPECT_EQ(nc_03.mHighPoints.size(),0);

    EXPECT_EQ(nc_04.mHighPoints.size(),2);
    auto  nc_04_h01it = nc_04.mHighPoints.begin();
    auto& nc_04_h01   = dynamic_cast<DNode&>(*(*(*nc_04_h01it)));
    EXPECT_EQ(&nc_07, &nc_04_h01);
    auto  nc_04_h02it = nc_04.mHighPoints.begin();
    nc_04_h02it++;
    auto& nc_04_h02   = dynamic_cast<DNode&>(*(*(*nc_04_h02it)));
    EXPECT_EQ(&nc_06, &nc_04_h02);

    EXPECT_EQ(nc_05.mHighPoints.size(),1);
    auto  nc_05_h01it = nc_05.mHighPoints.begin();
    auto& nc_05_h01   = dynamic_cast<DNode&>(*(*(*nc_05_h01it)));
    EXPECT_EQ(&nc_07, &nc_05_h01);

    EXPECT_EQ(nc_06.mHighPoints.size(),0);

    EXPECT_EQ(nc_07.mHighPoints.size(),0);

    EXPECT_EQ(nc_08.mHighPoints.size(),2);
    auto  nc_08_h01it = nc_08.mHighPoints.begin();
    auto& nc_08_h01   = dynamic_cast<DNode&>(*(*(*nc_08_h01it)));
    EXPECT_EQ(&nc_12, &nc_08_h01);
    auto  nc_08_h02it = nc_08.mHighPoints.begin();
    nc_08_h02it++;
    auto& nc_08_h02   = dynamic_cast<DNode&>(*(*(*nc_08_h02it)));
    EXPECT_EQ(&nc_11, &nc_08_h02);

    EXPECT_EQ(nc_09.mHighPoints.size(),2);
    auto  nc_09_h01it = nc_09.mHighPoints.begin();
    auto& nc_09_h01   = dynamic_cast<DNode&>(*(*(*nc_09_h01it)));
    EXPECT_EQ(&nc_12, &nc_09_h01);
    auto  nc_09_h02it = nc_09.mHighPoints.begin();
    nc_09_h02it++;
    auto& nc_09_h02   = dynamic_cast<DNode&>(*(*(*nc_09_h02it)));
    EXPECT_EQ(&nc_11, &nc_09_h02);

    EXPECT_EQ(nc_10.mHighPoints.size(),0);

    EXPECT_EQ(nc_11.mHighPoints.size(),0);

    EXPECT_EQ(nc_12.mHighPoints.size(),0);

    EXPECT_EQ(nc_01.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_01.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_02.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_02.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_03.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_03.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_13.mHighPointReferences.size(),2);
    EXPECT_EQ(nc_13.mHighPointIterators.size(),2);
    auto nc_13_hr = nc_13.mHighPointReferences.begin();
    EXPECT_EQ(nc_01.mNum, *nc_13_hr);
    nc_13_hr++;
    EXPECT_EQ(nc_02.mNum, *nc_13_hr);
    auto nc_13_hit = nc_13.mHighPointIterators.begin();
    EXPECT_EQ(nc_01_h01it, *nc_13_hit);
    nc_13_hit++;
    EXPECT_EQ(nc_02_h01it, *nc_13_hit);

    EXPECT_EQ(nc_04.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_04.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_05.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_05.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_06.mHighPointReferences.size(),1);
    EXPECT_EQ(nc_06.mHighPointIterators.size(),1);
    auto nc_06_hr = nc_06.mHighPointReferences.begin();
    EXPECT_EQ(nc_04.mNum, *nc_06_hr);
    auto nc_06_hit = nc_06.mHighPointIterators.begin();
    EXPECT_EQ(nc_04_h02it, *nc_06_hit);

    EXPECT_EQ(nc_07.mHighPointReferences.size(),2);
    EXPECT_EQ(nc_07.mHighPointIterators.size(),2);
    auto nc_07_hr = nc_07.mHighPointReferences.begin();
    EXPECT_EQ(nc_04.mNum, *nc_07_hr);
    nc_07_hr++;
    EXPECT_EQ(nc_05.mNum, *nc_07_hr);
    auto nc_07_hit = nc_07.mHighPointIterators.begin();
    EXPECT_EQ(nc_04_h01it, *nc_07_hit);
    nc_07_hit++;
    EXPECT_EQ(nc_05_h01it, *nc_07_hit);

    EXPECT_EQ(nc_08.mHighPointReferences.size(),1);
    EXPECT_EQ(nc_08.mHighPointIterators.size(),1);
    auto nc_08_hr = nc_08.mHighPointReferences.begin();
    EXPECT_EQ(nc_01.mNum, *nc_08_hr);
    auto nc_08_hit = nc_08.mHighPointIterators.begin();
    EXPECT_EQ(nc_01_h03it, *nc_08_hit);

    EXPECT_EQ(nc_09.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_09.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_10.mHighPointReferences.size(),0);
    EXPECT_EQ(nc_10.mHighPointIterators.size(),0);

    EXPECT_EQ(nc_12.mHighPointReferences.size(),3);
    EXPECT_EQ(nc_12.mHighPointIterators.size(),3);
    auto nc_12_hr = nc_12.mHighPointReferences.begin();
    EXPECT_EQ(nc_01.mNum, *nc_12_hr);
    nc_12_hr++;
    EXPECT_EQ(nc_08.mNum, *nc_12_hr);
    nc_12_hr++;
    EXPECT_EQ(nc_09.mNum, *nc_12_hr);
    auto nc_12_hit = nc_12.mHighPointIterators.begin();
    EXPECT_EQ(nc_01_h02it, *nc_12_hit);
    nc_12_hit++;
    EXPECT_EQ(nc_08_h01it, *nc_12_hit);
    nc_12_hit++;
    EXPECT_EQ(nc_09_h01it, *nc_12_hit);

    EXPECT_EQ(nc_11.mHighPointReferences.size(),2);
    EXPECT_EQ(nc_11.mHighPointIterators.size(),2);
    auto nc_11_hr = nc_11.mHighPointReferences.begin();
    EXPECT_EQ(nc_08.mNum, *nc_11_hr);
    nc_11_hr++;
    EXPECT_EQ(nc_09.mNum, *nc_11_hr);
    auto nc_11_hit = nc_11.mHighPointIterators.begin();
    EXPECT_EQ(nc_08_h02it, *nc_11_hit);
    nc_11_hit++;
    EXPECT_EQ(nc_09_h02it, *nc_11_hit);


}

} // namespace Undirected

} // namespace Wailea
