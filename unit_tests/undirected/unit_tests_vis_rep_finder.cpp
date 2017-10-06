#include "gtest/gtest.h"
#include "undirected/base.hpp"
#include "undirected/embedded_bctree.hpp"
#include "undirected/vis_rep_finder.hpp"
#include "unit_tests/undirected/vis_rep_test_graph_generator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Wailea {

namespace Undirected { 

class VisRepFinderTests : public ::testing::Test {

  protected:  

    VisRepFinderTests(){;};
    virtual ~VisRepFinderTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief
 */
TEST_F(VisRepFinderTests, EmptyGraph) {

    VRRectLabelGraph g1;

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 0);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();
    ebcT.findGeometricEmbedding();

    EXPECT_THROW(ebcT.root(), std::invalid_argument);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),0);
    EXPECT_EQ(expT.numEdges(),0);
    EXPECT_THROW(expT.root(), std::invalid_argument);
    VisRepFinder vr(ebcT);
    vr.find();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, SingleNode) {

    VRRectLabelGraph g1;
    auto  np  = std::make_unique<VRRectLabelNode>(
                                     100.0, 300.0);
    auto& N1  = dynamic_cast<VRRectLabelNode&>(g1.addNode(std::move(np)));

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 1);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();
    ebcT.findGeometricEmbedding();

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),1);
    EXPECT_EQ(expT.numEdges(),0);
    VisRepFinder vr(ebcT);
    vr.find();

    EXPECT_EQ(N1.y(),      150.0);
    EXPECT_EQ(N1.xLeft(),   50.0);
    EXPECT_EQ(N1.xRight(),  50.0);
    EXPECT_EQ(g1.width(),  100.0);
    EXPECT_EQ(g1.height(), 300.0);

}


/**  @brief
 */
TEST_F(VisRepFinderTests, SingleEdge1) {

    VRRectLabelGraph g1;
    g1.setHorizontalGap(10.0);
    g1.setVerticalGap(15.0);
    auto  np1 = std::make_unique<VRRectLabelNode>(
                                     100.0, 300.0);
    auto& N1  = dynamic_cast<VRRectLabelNode&>(g1.addNode(std::move(np1)));

    auto  np2 = std::make_unique<VRRectLabelNode>(
                                     400.0, 500.0);
    auto& N2  = dynamic_cast<VRRectLabelNode&>(g1.addNode(std::move(np2)));


    auto  ep1 = std::make_unique<VRRectLabelEdge>();
    auto& E01_02 = dynamic_cast<VRRectLabelEdge&>(
                                           g1.addEdge(std::move(ep1), N1, N2));

    E01_02.setLabelNode1Side(VRRectLabelEdge::POS_CENTER, 100.0, 100.0);
    E01_02.setLabelNode2Side(
                         VRRectLabelEdge::POS_COUNTER_CLOCKWISE, 100.0, 100.0);
    E01_02.setLabelMiddle(VRRectLabelEdge::POS_CLOCKWISE, 100.0, 100.0);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 1);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();
    auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcT.nodes().first)));
    EXPECT_EQ(ETN.embeddedGraph().numNodes(), 2);
    EXPECT_EQ(ETN.embeddedGraph().numEdges(), 1);
    EXPECT_EQ(ETN.dualGraph().numNodes(), 1);
    EXPECT_EQ(ETN.dualGraph().numEdges(), 1);

    auto& topN =  dynamic_cast<EmbeddedNode&>(
                                      *(*(ETN.embeddedGraph().nodes().first)));
    auto& BN1  =  dynamic_cast<BlockNode&>(topN.IGBackwardLinkRef());
    EXPECT_EQ(BN1.IGBackwardLink(), N1.backIt());
    auto outFIt = ETN.dualGraph().nodes().first;

    ebcT.findGeometricEmbedding(ebcT.nodes().first, outFIt, topN.backIt());
    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),1);
    EXPECT_EQ(expT.numEdges(),0);
    VisRepFinder vr(ebcT);
    vr.find();
    EXPECT_EQ(N1.y(),       980.0);
    EXPECT_EQ(N2.y(),       250.0);
    EXPECT_EQ(g1.height(), 1130.0);
    EXPECT_EQ(N1.xLeft(),     0.0);
    EXPECT_EQ(N1.xRight(),  400.0);
    EXPECT_EQ(N2.xLeft(),     0.0);
    EXPECT_EQ(N2.xRight(),  400.0);
    EXPECT_EQ(g1.width(),   400.0);
    EXPECT_EQ(E01_02.x(),   200.0);
    EXPECT_EQ(E01_02.y1(),  980.0);
    EXPECT_EQ(E01_02.y2(),  250.0);

}


/**  @brief
 */
TEST_F(VisRepFinderTests, SingleEdge2) {

    VRRectLabelGraph g1;
    g1.setHorizontalGap(10.0);
    g1.setVerticalGap(15.0);
    auto  np1 = std::make_unique<VRRectLabelNode>(
                                     100.0, 300.0);
    auto& N1  = dynamic_cast<VRRectLabelNode&>(g1.addNode(std::move(np1)));

    auto  np2 = std::make_unique<VRRectLabelNode>(
                                     400.0, 500.0);
    auto& N2  = dynamic_cast<VRRectLabelNode&>(g1.addNode(std::move(np2)));


    auto  ep1 = std::make_unique<VRRectLabelEdge>();
    auto& E01_02 = dynamic_cast<VRRectLabelEdge&>(g1.addEdge(std::move(ep1), N1, N2));

    E01_02.setLabelNode1Side(VRRectLabelEdge::POS_CENTER, 100.0, 100.0);
    E01_02.setLabelNode2Side(VRRectLabelEdge::POS_COUNTER_CLOCKWISE, 100.0, 100.0);
    E01_02.setLabelMiddle(VRRectLabelEdge::POS_CLOCKWISE, 500.0, 100.0);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 1);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();
    auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcT.nodes().first)));
    EXPECT_EQ(ETN.embeddedGraph().numNodes(), 2);
    EXPECT_EQ(ETN.embeddedGraph().numEdges(), 1);
    EXPECT_EQ(ETN.dualGraph().numNodes(), 1);
    EXPECT_EQ(ETN.dualGraph().numEdges(), 1);

    auto nit = ETN.embeddedGraph().nodes().first;
    auto& TN1 =  dynamic_cast<EmbeddedNode&>(*(*nit));
    auto& BN1  =  dynamic_cast<BlockNode&>(TN1.IGBackwardLinkRef());
    EXPECT_EQ(BN1.IGBackwardLink(), N1.backIt());
    nit++;
    auto& TN2 =  dynamic_cast<EmbeddedNode&>(*(*nit));
    auto& BN2  =  dynamic_cast<BlockNode&>(TN2.IGBackwardLinkRef());
    EXPECT_EQ(BN2.IGBackwardLink(), N2.backIt());

    auto outFIt = ETN.dualGraph().nodes().first;

    ebcT.findGeometricEmbedding(ebcT.nodes().first, outFIt, TN2.backIt());
    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),1);
    EXPECT_EQ(expT.numEdges(),0);
    VisRepFinder vr(ebcT);
    vr.find();
    EXPECT_EQ(N1.y(),       150.0);
    EXPECT_EQ(N2.y(),       880.0);
    EXPECT_EQ(g1.height(), 1130.0);
    EXPECT_EQ(N1.xLeft(),     0.0);
    EXPECT_EQ(N1.xRight(),  600.0);
    EXPECT_EQ(N2.xLeft(),     0.0);
    EXPECT_EQ(N2.xRight(),  600.0);
    EXPECT_EQ(g1.width(),   600.0);
    EXPECT_EQ(E01_02.x(),   100.0);
    EXPECT_EQ(E01_02.y1(),  150.0);
    EXPECT_EQ(E01_02.y2(),  880.0);

}


/**  @brief
 */
TEST_F(VisRepFinderTests, TwoSerialEdges01) {

    VisRepTestGraphParser p;
    p.parseSpec(
              "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_01.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
    vr.find();
    EXPECT_EQ(g.inspect(), true);
}


/**  @brief
 */
TEST_F(VisRepFinderTests, TwoParallelEdges01) {

    VisRepTestGraphParser p;
    p.parseSpec(
              "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_02.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
    vr.find();
    EXPECT_EQ(g.inspect(), true);
}


/**  @brief
 */
TEST_F(VisRepFinderTests, RealisticTree01) {

    VisRepTestGraphParser p;
    p.parseSpec(
              "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_03.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV01) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_04.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV01_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_04_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV01_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_04_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV01_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_04_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV01_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_04_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV02) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_05.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV02_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_05_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV02_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_05_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV02_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_05_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceTopCV02_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_05_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();

}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV01) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_06.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV01_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_06_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV01_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_06_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV01_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_06_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV01_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_06_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV02) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_07.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV02_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_07_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV02_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_07_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV02_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_07_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV02_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_07_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV03) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_08.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV03_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_08_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV03_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_08_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV03_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_08_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV03_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_08_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV04) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_09.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV04_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_09_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV04_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_09_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV04_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_09_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceTopCV04_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_09_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV01) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_10.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV01_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_10_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV01_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_10_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV01_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_10_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV01_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_10_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV02) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_11.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV02_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_11_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV02_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_11_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV02_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_11_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV02_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_11_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV03) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_12.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV03_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_12_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV03_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_12_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV03_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_12_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV03_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_12_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}



/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV04) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_13.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV04_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_13_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV04_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_13_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV04_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_13_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceLeftCV04_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_13_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV05) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_14.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV05_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_14_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV05_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_14_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV05_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_14_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV05_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_14_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV06) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_15.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV06_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_15_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV06_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_15_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV06_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_15_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV06_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_15_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV07) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_16.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV07_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_16_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV07_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_16_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV07_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_16_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV07_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_16_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV08) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_17.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV08_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_17_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV08_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_17_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV08_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_17_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceLeftCV08_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_17_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV01) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_18.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV01_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_18_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV01_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_18_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV01_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_18_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV01_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_18_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV02) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_19.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV02_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_19_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV02_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_19_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV02_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_19_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV02_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_19_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV03) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_20.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV03_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_20_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV03_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_20_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV03_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_20_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV03_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_20_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV04) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_21.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV04_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_21_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV04_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_21_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV04_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_21_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV04_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_21_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV05) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_22.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV05_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_22_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV05_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_22_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV05_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_22_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV05_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_22_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV06) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_23.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV06_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_23_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV06_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_23_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV06_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_23_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV06_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_23_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV07) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_24.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV07_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_24_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV07_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_24_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV07_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_24_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV07_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_24_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV08) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_25.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV08_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_25_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV08_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_25_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV08_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_25_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceRightCV08_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_25_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV01) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_26.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV01_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_26_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV01_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_26_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV01_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_26_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV01_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_26_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV02) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_27.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV02_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_27_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV02_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_27_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV02_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_27_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, OuterFaceBottomCV02_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_27_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV01) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_28.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV01_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_28_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV01_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_28_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV01_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_28_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV01_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_28_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV02) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_29.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV02_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_29_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV02_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_29_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV02_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_29_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV02_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_29_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV03) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_30.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV03_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_30_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV03_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_30_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV03_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_30_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV03_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_30_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV04) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_31.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV04_nest1) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_31_nest1.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV04_nest2) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_31_nest2.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV04_nest3) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_31_nest3.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}


/**  @brief
 */
TEST_F(VisRepFinderTests, InnerFaceBottomCV04_nest4) {

    VisRepTestGraphParser p;
    p.parseSpec(
             "./unit_tests/undirected/vis_rep_patterns/vr_test_pattern_31_nest4.txt");
    VisRepTestGraphGenerator g(p);
    g.generateGraphs();
    VisRepFinder vr(g.getEBCTree());
//    vr.mDebugPrint = true;
    vr.find();
    EXPECT_EQ(g.inspect(), true);
//    g.emitUnitTestExpectation();
//    g.debugPrint();
}

} // namespace Undirected

} // namespace Wailea
