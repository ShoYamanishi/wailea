#include "gtest/gtest.h"
#include "undirected/spqr_decomposer.hpp"

namespace Wailea {

namespace Undirected { 

using DExplorer = DFSExplorer <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DStrategy = DFSStrategy <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DGraph    = DFSGraph    <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DNode     = DFSNode     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DEdge     = DFSEdge     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;

class SPQRProc1Tests : public ::testing::Test {

  protected:  

    SPQRProc1Tests(){;};
    virtual ~SPQRProc1Tests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    node_list_it_t copyInputGraph(SPQRDecomposer& decomp,
                            Graph& src, Graph& dst, node_list_it_t startNode){
        return decomp.copyInputGraph(src,dst,startNode);
    }

};

/**  @brief tests on the frmous representative graph 1.
 */
TEST_F(SPQRProc1Tests, Test1) {

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
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                      (nc_01));
    nit++;
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


    auto eit = g_2.edges().first;
    auto& ec_01_02 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_02_03 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_03_13 = dynamic_cast<DEdge&>(*(*eit++));
    /*auto& ec_13_01 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    /*auto& ec_13_02 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    auto& ec_03_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_04_05 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_05_06 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_06_07 = dynamic_cast<DEdge&>(*(*eit++));
    /*auto& ec_07_04 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    /*auto& ec_07_05 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    /*auto& ec_06_04 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    auto& ec_05_08 = dynamic_cast<DEdge&>(*(*eit++));
    /*auto& ec_08_01 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    auto& ec_08_09 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_09_10 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_10_12 = dynamic_cast<DEdge&>(*(*eit++));
    /*auto& ec_12_01 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    /*auto& ec_12_08 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    /*auto& ec_12_09 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    auto& ec_10_11 = dynamic_cast<DEdge&>(*(*eit++));
    /*auto& ec_11_08 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;
    /*auto& ec_11_09 = dynamic_cast<DEdge&>(*(* */eit++/*))*/;


    EXPECT_EQ(nc_01.mNum, 1);
    EXPECT_EQ(nc_02.mNum, 2);
    EXPECT_EQ(nc_03.mNum, 3);
    EXPECT_EQ(nc_13.mNum, 4);
    EXPECT_EQ(nc_04.mNum, 5);
    EXPECT_EQ(nc_05.mNum, 6);
    EXPECT_EQ(nc_06.mNum, 7);
    EXPECT_EQ(nc_07.mNum, 8);
    EXPECT_EQ(nc_08.mNum, 9);
    EXPECT_EQ(nc_09.mNum,10);
    EXPECT_EQ(nc_10.mNum,11);
    EXPECT_EQ(nc_12.mNum,12);
    EXPECT_EQ(nc_11.mNum,13);

    EXPECT_EQ(nc_01.mLowPt1, 1);
    EXPECT_EQ(nc_02.mLowPt1, 1);
    EXPECT_EQ(nc_03.mLowPt1, 1);
    EXPECT_EQ(nc_13.mLowPt1, 1);
    EXPECT_EQ(nc_04.mLowPt1, 1);
    EXPECT_EQ(nc_05.mLowPt1, 1);
    EXPECT_EQ(nc_06.mLowPt1, 5);
    EXPECT_EQ(nc_07.mLowPt1, 5);
    EXPECT_EQ(nc_08.mLowPt1, 1);
    EXPECT_EQ(nc_09.mLowPt1, 1);
    EXPECT_EQ(nc_10.mLowPt1, 1);
    EXPECT_EQ(nc_12.mLowPt1, 1);
    EXPECT_EQ(nc_11.mLowPt1, 9);

    EXPECT_EQ(nc_01.mLowPt2, 1);
    EXPECT_EQ(nc_02.mLowPt2, 2);
    EXPECT_EQ(nc_03.mLowPt2, 2);
    EXPECT_EQ(nc_13.mLowPt2, 2);
    EXPECT_EQ(nc_04.mLowPt2, 5);
    EXPECT_EQ(nc_05.mLowPt2, 5);
    EXPECT_EQ(nc_06.mLowPt2, 6);
    EXPECT_EQ(nc_07.mLowPt2, 6);
    EXPECT_EQ(nc_08.mLowPt2, 9);
    EXPECT_EQ(nc_09.mLowPt2, 9);
    EXPECT_EQ(nc_10.mLowPt2, 9);
    EXPECT_EQ(nc_12.mLowPt2, 9);
    EXPECT_EQ(nc_11.mLowPt2, 10);

    EXPECT_EQ(nc_01.mNd, 13);
    EXPECT_EQ(nc_02.mNd, 12);
    EXPECT_EQ(nc_03.mNd, 11);
    EXPECT_EQ(nc_13.mNd,  1);
    EXPECT_EQ(nc_04.mNd,  9);
    EXPECT_EQ(nc_05.mNd,  8);
    EXPECT_EQ(nc_06.mNd,  2);
    EXPECT_EQ(nc_07.mNd,  1);
    EXPECT_EQ(nc_08.mNd,  5);
    EXPECT_EQ(nc_09.mNd,  4);
    EXPECT_EQ(nc_10.mNd,  3);
    EXPECT_EQ(nc_11.mNd,  1);
    EXPECT_EQ(nc_12.mNd,  1);

    EXPECT_EQ(nc_01.mParentEdge, g_2.edges().second);
    EXPECT_EQ(nc_02.mParentEdge, ec_01_02.backIt());
    EXPECT_EQ(nc_03.mParentEdge, ec_02_03.backIt());
    EXPECT_EQ(nc_13.mParentEdge, ec_03_13.backIt());
    EXPECT_EQ(nc_04.mParentEdge, ec_03_04.backIt());
    EXPECT_EQ(nc_05.mParentEdge, ec_04_05.backIt());
    EXPECT_EQ(nc_06.mParentEdge, ec_05_06.backIt());
    EXPECT_EQ(nc_07.mParentEdge, ec_06_07.backIt());
    EXPECT_EQ(nc_08.mParentEdge, ec_05_08.backIt());
    EXPECT_EQ(nc_09.mParentEdge, ec_08_09.backIt());
    EXPECT_EQ(nc_10.mParentEdge, ec_09_10.backIt());
    EXPECT_EQ(nc_12.mParentEdge, ec_10_12.backIt());
    EXPECT_EQ(nc_11.mParentEdge, ec_10_11.backIt());

}

} // namespace Undirected

} // namespace Wailea
