#include "gtest/gtest.h"
#include "directed/sugiyama_digraph.hpp"

namespace Wailea {

namespace Directed { 

class SugiyamaDiGraphTests : public ::testing::Test {

  protected:  

    SugiyamaDiGraphTests(){;};
    virtual ~SugiyamaDiGraphTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief test empty graph
 */
TEST_F(SugiyamaDiGraphTests, Test1) {

    DiGraph g01;
    SugiyamaDiGraph g02(g01);

    vector<node_list_it_t> vec_00;
    g02.generateFromAcyclicOrdering(vec_00);

    vector<vector<node_list_it_t> > ranks_01;
    vector<vector<node_list_it_t> > ranks_02 = 
                                     g02.rankNodesAndAddVirtualNodes(ranks_01);
    EXPECT_EQ(ranks_02.size(), 0);

    g02.setPositionsForEachRank(ranks_02);

    EXPECT_EQ(g02.numNodes(), 0);
    EXPECT_EQ(g02.numEdges(), 0);

//    auto np01 = std::make_unique<NumDiNode>(1);
//    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

//    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
//                                    g01.addEdge(std::move(ep01_02), N01, N02);

}


/**  @brief test one node
 */
TEST_F(SugiyamaDiGraphTests, Test2) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    SugiyamaDiGraph g02(g01);

    EXPECT_EQ(g02.numNodes(), 0);
    EXPECT_EQ(g02.numEdges(), 0);

    vector<node_list_it_t> vec_00;
    vec_00.push_back(N01.backIt());
    g02.generateFromAcyclicOrdering(vec_00);

    EXPECT_EQ(g02.numNodes(), 1);
    EXPECT_EQ(g02.numEdges(), 0);
    
    auto snit = g02.nodes().first;
    auto& SN01 = dynamic_cast<SugiyamaDiNode&>(*(*snit));

    EXPECT_EQ(SN01.IGBackwardLink(), N01.backIt());
    EXPECT_EQ(N01.IGForwardLink(), SN01.backIt());

    vector<vector<node_list_it_t> > ranks_01;
    vector<node_list_it_t> vec_01;
    vec_01.push_back(SN01.backIt());
    ranks_01.push_back(vec_01);

    vector<vector<node_list_it_t> > ranks_02 = 
                                   g02.rankNodesAndAddVirtualNodes(ranks_01);

    EXPECT_EQ(ranks_02.size(), 1);    

    auto& vec_02 = ranks_02[0];
    EXPECT_EQ(vec_02.size(), 1);
    EXPECT_EQ(vec_02[0], SN01.backIt());

    g02.setPositionsForEachRank(ranks_02);

    EXPECT_EQ(SN01.rank(), 0);
    EXPECT_EQ(SN01.pos(),  0);

//    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
//                                  g01.addEdge(std::move(ep01_02), N01, N02);

}


/**  @brief test one edge
 */
TEST_F(SugiyamaDiGraphTests, Test3) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto ep01_02 = std::make_unique<DiEdge>();
    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));

    SugiyamaDiGraph g02(g01);

    EXPECT_EQ(g02.numNodes(), 0);
    EXPECT_EQ(g02.numEdges(), 0);

    vector<node_list_it_t> vec_00;
    vec_00.push_back(N01.backIt());
    vec_00.push_back(N02.backIt());
    g02.generateFromAcyclicOrdering(vec_00);

    EXPECT_EQ(g02.numNodes(), 2);
    EXPECT_EQ(g02.numEdges(), 1);
    
    auto snit = g02.nodes().first;
    auto& SN01 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN02 = dynamic_cast<SugiyamaDiNode&>(*(*snit));

    EXPECT_EQ(SN01.IGBackwardLink(), N01.backIt());
    EXPECT_EQ(N01.IGForwardLink(), SN01.backIt());

    EXPECT_EQ(SN02.IGBackwardLink(), N02.backIt());
    EXPECT_EQ(N02.IGForwardLink(), SN02.backIt());

    auto seit = g02.edges().first;
    auto& SE01_02 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));

    EXPECT_EQ(SE01_02.IGBackwardLink(), E01_02.backIt());
    EXPECT_EQ(E01_02.IGForwardLink(), SE01_02.backIt());

    vector<vector<node_list_it_t> > ranks_01;
    vector<node_list_it_t> vec_01;
    vec_01.push_back(SN01.backIt());
    ranks_01.push_back(vec_01);
    vector<node_list_it_t> vec_02;
    vec_02.push_back(SN02.backIt());
    ranks_01.push_back(vec_02);

    vector<vector<node_list_it_t> > ranks_02 = 
                                   g02.rankNodesAndAddVirtualNodes(ranks_01);

    EXPECT_EQ(ranks_02.size(), 2);

    auto& vec_03 = ranks_02[0];
    EXPECT_EQ(vec_03.size(), 1);
    EXPECT_EQ(vec_03[0], SN01.backIt());

    auto& vec_04 = ranks_02[1];
    EXPECT_EQ(vec_04.size(), 1);
    EXPECT_EQ(vec_04[0], SN02.backIt());

    EXPECT_EQ(SN01.rank(), 0);
    EXPECT_EQ(SN02.rank(), 1);

    g02.setPositionsForEachRank(ranks_02);

    EXPECT_EQ(SN01.pos(),  0);
    EXPECT_EQ(SN01.pos(),  0);

}


/**  @brief test realistic
 */
TEST_F(SugiyamaDiGraphTests, Test4) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NumDiNode>(4);
    auto& N04 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NumDiNode>(5);
    auto& N05 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np05)));

    auto np06 = std::make_unique<NumDiNode>(6);
    auto& N06 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np06)));

    auto np07 = std::make_unique<NumDiNode>(7);
    auto& N07 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np07)));

    auto ep01_03 = std::make_unique<DiEdge>();
    auto& E01_03 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<DiEdge>();
    auto& E01_04 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep01_05 = std::make_unique<DiEdge>();
    auto& E01_05 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_05), N01, N05));

    auto ep02_01 = std::make_unique<DiEdge>();
    auto& E02_01 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep02_01), N02, N01));

    auto ep02_03 = std::make_unique<DiEdge>();
    auto& E02_03 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep02_05 = std::make_unique<DiEdge>();
    auto& E02_05 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep02_05), N02, N05));

    auto ep04_02 = std::make_unique<DiEdge>();
    auto& E04_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep04_02), N04, N02));

    auto ep04_05 = std::make_unique<DiEdge>();
    auto& E04_05 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));

    auto ep06_01 = std::make_unique<DiEdge>();
    auto& E06_01 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep06_01), N06, N01));

    auto ep06_02 = std::make_unique<DiEdge>();
    auto& E06_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep06_02), N06, N02));

    auto ep06_03 = std::make_unique<DiEdge>();
    auto& E06_03 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep06_03), N06, N03));

    auto ep06_07 = std::make_unique<DiEdge>();
    auto& E06_07 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep06_07), N06, N07));

    auto ep07_02 = std::make_unique<DiEdge>();
    auto& E07_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep07_02), N07, N02));

    auto ep07_05 = std::make_unique<DiEdge>();
    auto& E07_05 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep07_05), N07, N05));

    SugiyamaDiGraph g02(g01);

    EXPECT_EQ(g02.numNodes(), 0);
    EXPECT_EQ(g02.numEdges(), 0);

    vector<node_list_it_t> vec_00;
    vec_00.push_back(N01.backIt());
    vec_00.push_back(N02.backIt());
    vec_00.push_back(N03.backIt());
    vec_00.push_back(N04.backIt());
    vec_00.push_back(N05.backIt());
    vec_00.push_back(N06.backIt());
    vec_00.push_back(N07.backIt());
    g02.generateFromAcyclicOrdering(vec_00);

    EXPECT_EQ(g02.numNodes(), 7);
    EXPECT_EQ(g02.numEdges(), 14);
    
    auto snit = g02.nodes().first;
    auto& SN01 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN02 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN03 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN04 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN05 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN06 = dynamic_cast<SugiyamaDiNode&>(*(*snit));
    snit++;
    auto& SN07 = dynamic_cast<SugiyamaDiNode&>(*(*snit));

    EXPECT_EQ(SN01.IGBackwardLink(), N01.backIt());
    EXPECT_EQ(N01.IGForwardLink(), SN01.backIt());

    EXPECT_EQ(SN02.IGBackwardLink(), N02.backIt());
    EXPECT_EQ(N02.IGForwardLink(), SN02.backIt());

    EXPECT_EQ(SN03.IGBackwardLink(), N03.backIt());
    EXPECT_EQ(N03.IGForwardLink(), SN03.backIt());

    EXPECT_EQ(SN04.IGBackwardLink(), N04.backIt());
    EXPECT_EQ(N04.IGForwardLink(), SN04.backIt());

    EXPECT_EQ(SN05.IGBackwardLink(), N05.backIt());
    EXPECT_EQ(N05.IGForwardLink(), SN05.backIt());

    EXPECT_EQ(SN06.IGBackwardLink(), N06.backIt());
    EXPECT_EQ(N06.IGForwardLink(), SN06.backIt());

    EXPECT_EQ(SN07.IGBackwardLink(), N07.backIt());
    EXPECT_EQ(N07.IGForwardLink(), SN07.backIt());

    auto seit = g02.edges().first;
    auto& SE01_03 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE01_04 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE01_05 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE02_01 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE02_03 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE02_05 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE04_02 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE04_05 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE06_01 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE06_02 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE06_03 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE06_07 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE07_02 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));
    seit++;
    auto& SE07_05 = dynamic_cast<SugiyamaDiEdge&>(*(*seit));

    EXPECT_EQ(SE01_03.IGBackwardLink(), E01_03.backIt());
    EXPECT_EQ(E01_03.IGForwardLink(), SE01_03.backIt());
    EXPECT_EQ(SE01_03.isFlipped(), false);
    EXPECT_EQ(SE01_03.incidentNodeSrc().backIt(), SN01.backIt());
    EXPECT_EQ(SE01_03.incidentNodeDst().backIt(), SN03.backIt());

    EXPECT_EQ(SE01_04.IGBackwardLink(), E01_04.backIt());
    EXPECT_EQ(E01_04.IGForwardLink(), SE01_04.backIt());
    EXPECT_EQ(SE01_04.isFlipped(), false);
    EXPECT_EQ(SE01_04.incidentNodeSrc().backIt(), SN01.backIt());
    EXPECT_EQ(SE01_04.incidentNodeDst().backIt(), SN04.backIt());

    EXPECT_EQ(SE01_05.IGBackwardLink(), E01_05.backIt());
    EXPECT_EQ(E01_05.IGForwardLink(), SE01_05.backIt());
    EXPECT_EQ(SE01_05.isFlipped(), false);
    EXPECT_EQ(SE01_05.incidentNodeSrc().backIt(), SN01.backIt());
    EXPECT_EQ(SE01_05.incidentNodeDst().backIt(), SN05.backIt());

    EXPECT_EQ(SE02_01.IGBackwardLink(), E02_01.backIt());
    EXPECT_EQ(E02_01.IGForwardLink(), SE02_01.backIt());
    EXPECT_EQ(SE02_01.isFlipped(), true);
    EXPECT_EQ(SE02_01.incidentNodeSrc().backIt(), SN01.backIt());
    EXPECT_EQ(SE02_01.incidentNodeDst().backIt(), SN02.backIt());

    EXPECT_EQ(SE02_03.IGBackwardLink(), E02_03.backIt());
    EXPECT_EQ(E02_03.IGForwardLink(), SE02_03.backIt());
    EXPECT_EQ(SE02_03.isFlipped(), false);
    EXPECT_EQ(SE02_03.incidentNodeSrc().backIt(), SN02.backIt());
    EXPECT_EQ(SE02_03.incidentNodeDst().backIt(), SN03.backIt());

    EXPECT_EQ(SE02_05.IGBackwardLink(), E02_05.backIt());
    EXPECT_EQ(E02_05.IGForwardLink(), SE02_05.backIt());
    EXPECT_EQ(SE02_05.isFlipped(), false);
    EXPECT_EQ(SE02_05.incidentNodeSrc().backIt(), SN02.backIt());
    EXPECT_EQ(SE02_05.incidentNodeDst().backIt(), SN05.backIt());

    EXPECT_EQ(SE04_02.IGBackwardLink(), E04_02.backIt());
    EXPECT_EQ(E04_02.IGForwardLink(), SE04_02.backIt());
    EXPECT_EQ(SE04_02.isFlipped(), true);
    EXPECT_EQ(SE04_02.incidentNodeSrc().backIt(), SN02.backIt());
    EXPECT_EQ(SE04_02.incidentNodeDst().backIt(), SN04.backIt());

    EXPECT_EQ(SE04_05.IGBackwardLink(), E04_05.backIt());
    EXPECT_EQ(E04_05.IGForwardLink(), SE04_05.backIt());
    EXPECT_EQ(SE04_05.isFlipped(), false);
    EXPECT_EQ(SE04_05.incidentNodeSrc().backIt(), SN04.backIt());
    EXPECT_EQ(SE04_05.incidentNodeDst().backIt(), SN05.backIt());

    EXPECT_EQ(SE06_01.IGBackwardLink(), E06_01.backIt());
    EXPECT_EQ(E06_01.IGForwardLink(), SE06_01.backIt());
    EXPECT_EQ(SE06_01.isFlipped(), true);
    EXPECT_EQ(SE06_01.incidentNodeSrc().backIt(), SN01.backIt());
    EXPECT_EQ(SE06_01.incidentNodeDst().backIt(), SN06.backIt());

    EXPECT_EQ(SE06_02.IGBackwardLink(), E06_02.backIt());
    EXPECT_EQ(E06_02.IGForwardLink(), SE06_02.backIt());
    EXPECT_EQ(SE06_02.isFlipped(), true);
    EXPECT_EQ(SE06_02.incidentNodeSrc().backIt(), SN02.backIt());
    EXPECT_EQ(SE06_02.incidentNodeDst().backIt(), SN06.backIt());

    EXPECT_EQ(SE06_03.IGBackwardLink(), E06_03.backIt());
    EXPECT_EQ(E06_03.IGForwardLink(), SE06_03.backIt());
    EXPECT_EQ(SE06_03.isFlipped(), true);
    EXPECT_EQ(SE06_03.incidentNodeSrc().backIt(), SN03.backIt());
    EXPECT_EQ(SE06_03.incidentNodeDst().backIt(), SN06.backIt());

    EXPECT_EQ(SE06_07.IGBackwardLink(), E06_07.backIt());
    EXPECT_EQ(E06_07.IGForwardLink(), SE06_07.backIt());
    EXPECT_EQ(SE06_07.isFlipped(), false);
    EXPECT_EQ(SE06_07.incidentNodeSrc().backIt(), SN06.backIt());
    EXPECT_EQ(SE06_07.incidentNodeDst().backIt(), SN07.backIt());

    EXPECT_EQ(SE07_02.IGBackwardLink(), E07_02.backIt());
    EXPECT_EQ(E07_02.IGForwardLink(), SE07_02.backIt());
    EXPECT_EQ(SE07_02.isFlipped(), true);
    EXPECT_EQ(SE07_02.incidentNodeSrc().backIt(), SN02.backIt());
    EXPECT_EQ(SE07_02.incidentNodeDst().backIt(), SN07.backIt());

    EXPECT_EQ(SE07_05.IGBackwardLink(), E07_05.backIt());
    EXPECT_EQ(E07_05.IGForwardLink(), SE07_05.backIt());
    EXPECT_EQ(SE07_05.isFlipped(), true);
    EXPECT_EQ(SE07_05.incidentNodeSrc().backIt(), SN05.backIt());
    EXPECT_EQ(SE07_05.incidentNodeDst().backIt(), SN07.backIt());

    vector<vector<node_list_it_t> > ranks_01;
    vector<node_list_it_t> vec_01;
    vec_01.push_back(SN01.backIt());
    ranks_01.push_back(vec_01);
    vector<node_list_it_t> vec_02;
    vec_02.push_back(SN02.backIt());
    ranks_01.push_back(vec_02);
    vector<node_list_it_t> vec_03;
    vec_03.push_back(SN03.backIt());
    vec_03.push_back(SN04.backIt());
    ranks_01.push_back(vec_03);
    vector<node_list_it_t> vec_04;
    vec_04.push_back(SN05.backIt());
    vec_04.push_back(SN06.backIt());
    ranks_01.push_back(vec_04);
    vector<node_list_it_t> vec_05;
    vec_05.push_back(SN07.backIt());
    ranks_01.push_back(vec_05);

    vector<vector<node_list_it_t> > ranks_02 = 
                                     g02.rankNodesAndAddVirtualNodes(ranks_01);

    EXPECT_EQ(g02.numNodes(), 17);
    EXPECT_EQ(g02.numEdges(), 24);

    auto tnit = g02.nodes().first;
    auto& TN01 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TN02 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TN03 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TN04 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TN05 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TN06 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TN07 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV01 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV02 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV03 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV04 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV05 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV06 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV07 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV08 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV09 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));
    tnit++;
    auto& TV10 = dynamic_cast<SugiyamaDiNode&>(*(*tnit));

    EXPECT_EQ(TN01.backIt(), SN01.backIt());
    EXPECT_EQ(TN01.isVirtual(), false);
    EXPECT_EQ(TN01.rank(), 0);
    EXPECT_EQ(TN02.backIt(), SN02.backIt());
    EXPECT_EQ(TN02.isVirtual(), false);
    EXPECT_EQ(TN02.rank(), 1);
    EXPECT_EQ(TN03.backIt(), SN03.backIt());
    EXPECT_EQ(TN03.isVirtual(), false);
    EXPECT_EQ(TN03.rank(), 2);
    EXPECT_EQ(TN04.backIt(), SN04.backIt());
    EXPECT_EQ(TN04.isVirtual(), false);
    EXPECT_EQ(TN04.rank(), 2);
    EXPECT_EQ(TN05.backIt(), SN05.backIt());
    EXPECT_EQ(TN05.isVirtual(), false);
    EXPECT_EQ(TN05.rank(), 3);
    EXPECT_EQ(TN06.backIt(), SN06.backIt());
    EXPECT_EQ(TN06.isVirtual(), false);
    EXPECT_EQ(TN06.rank(), 3);
    EXPECT_EQ(TN07.backIt(), SN07.backIt());
    EXPECT_EQ(TN07.isVirtual(), false);
    EXPECT_EQ(TN07.rank(), 4);

    EXPECT_EQ(TV01.isVirtual(), true);
    EXPECT_EQ(TV01.rank(), 1);
    EXPECT_EQ(TV02.isVirtual(), true);
    EXPECT_EQ(TV02.rank(), 1);
    EXPECT_EQ(TV03.isVirtual(), true);
    EXPECT_EQ(TV03.rank(), 1);
    EXPECT_EQ(TV04.isVirtual(), true);
    EXPECT_EQ(TV04.rank(), 2);
    EXPECT_EQ(TV05.isVirtual(), true);
    EXPECT_EQ(TV05.rank(), 2);
    EXPECT_EQ(TV06.isVirtual(), true);
    EXPECT_EQ(TV06.rank(), 1);
    EXPECT_EQ(TV07.isVirtual(), true);
    EXPECT_EQ(TV07.rank(), 2);
    EXPECT_EQ(TV08.isVirtual(), true);
    EXPECT_EQ(TV08.rank(), 2);
    EXPECT_EQ(TV09.isVirtual(), true);
    EXPECT_EQ(TV09.rank(), 2);
    EXPECT_EQ(TV10.isVirtual(), true);
    EXPECT_EQ(TV10.rank(), 3);


    auto teit = g02.edges().first;
    auto& TE01 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE02 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE03 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE04 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE05 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE06 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE07 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE08 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE09 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE10 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE11 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE12 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE13 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE14 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE15 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE16 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE17 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE18 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE19 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE20 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE21 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE22 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE23 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));
    teit++;
    auto& TE24 = dynamic_cast<SugiyamaDiEdge&>(*(*teit));

    EXPECT_EQ(TE01.IGBackwardLink(), E02_01.backIt());
    EXPECT_EQ(E02_01.IGForwardLink(), TE01.backIt());
    EXPECT_EQ(TE01.isFlipped(), true);
    EXPECT_EQ(TE01.isTerminalNsrc(), true);
    EXPECT_EQ(TE01.isTerminalNdst(), true);
    EXPECT_EQ(TE01.incidentNodeSrc().backIt(), TN01.backIt());
    EXPECT_EQ(TE01.incidentNodeDst().backIt(), TN02.backIt());

    EXPECT_EQ(TE02.IGBackwardLink(), E02_03.backIt());
    EXPECT_EQ(E02_03.IGForwardLink(), TE02.backIt());
    EXPECT_EQ(TE02.isFlipped(), false);
    EXPECT_EQ(TE02.isTerminalNsrc(), true);
    EXPECT_EQ(TE02.isTerminalNdst(), true);
    EXPECT_EQ(TE02.incidentNodeSrc().backIt(), TN02.backIt());
    EXPECT_EQ(TE02.incidentNodeDst().backIt(), TN03.backIt());

    EXPECT_EQ(TE03.IGBackwardLink(), E04_02.backIt());
    EXPECT_EQ(E04_02.IGForwardLink(), TE03.backIt());
    EXPECT_EQ(TE03.isFlipped(), true);
    EXPECT_EQ(TE03.isTerminalNsrc(), true);
    EXPECT_EQ(TE03.isTerminalNdst(), true);
    EXPECT_EQ(TE03.incidentNodeSrc().backIt(), TN02.backIt());
    EXPECT_EQ(TE03.incidentNodeDst().backIt(), TN04.backIt());

    EXPECT_EQ(TE04.IGBackwardLink(), E04_05.backIt());
    EXPECT_EQ(E04_05.IGForwardLink(), TE04.backIt());
    EXPECT_EQ(TE04.isFlipped(), false);
    EXPECT_EQ(TE04.isTerminalNsrc(), true);
    EXPECT_EQ(TE04.isTerminalNdst(), true);
    EXPECT_EQ(TE04.incidentNodeSrc().backIt(), TN04.backIt());
    EXPECT_EQ(TE04.incidentNodeDst().backIt(), TN05.backIt());

    EXPECT_EQ(TE05.IGBackwardLink(), E06_03.backIt());
    EXPECT_EQ(E06_03.IGForwardLink(), TE05.backIt());
    EXPECT_EQ(TE05.isFlipped(), true);
    EXPECT_EQ(TE05.isTerminalNsrc(), true);
    EXPECT_EQ(TE05.isTerminalNdst(), true);
    EXPECT_EQ(TE05.incidentNodeSrc().backIt(), TN03.backIt());
    EXPECT_EQ(TE05.incidentNodeDst().backIt(), TN06.backIt());

    EXPECT_EQ(TE06.IGBackwardLink(), E06_07.backIt());
    EXPECT_EQ(E06_07.IGForwardLink(), TE06.backIt());
    EXPECT_EQ(TE06.isFlipped(), false);
    EXPECT_EQ(TE06.isTerminalNsrc(), true);
    EXPECT_EQ(TE06.isTerminalNdst(), true);
    EXPECT_EQ(TE06.incidentNodeSrc().backIt(), TN06.backIt());
    EXPECT_EQ(TE06.incidentNodeDst().backIt(), TN07.backIt());

    EXPECT_EQ(TE07.IGBackwardLink(), E07_05.backIt());
    EXPECT_EQ(E07_05.IGForwardLink(), TE07.backIt());
    EXPECT_EQ(TE07.isFlipped(), true);
    EXPECT_EQ(TE07.isTerminalNsrc(), true);
    EXPECT_EQ(TE07.isTerminalNdst(), true);
    EXPECT_EQ(TE07.incidentNodeSrc().backIt(), TN05.backIt());
    EXPECT_EQ(TE07.incidentNodeDst().backIt(), TN07.backIt());

    EXPECT_EQ(TE08.IGBackwardLink(), E01_03.backIt());
    EXPECT_EQ(E01_03.IGForwardLink(), TE08.backIt());
    EXPECT_EQ(TE08.isFlipped(), false);
    EXPECT_EQ(TE08.isTerminalNsrc(), true);
    EXPECT_EQ(TE08.isTerminalNdst(), false);
    EXPECT_EQ(TE08.chainNdst(), TE09.backIt());
    EXPECT_EQ(TE08.incidentNodeSrc().backIt(), TN01.backIt());
    EXPECT_EQ(TE08.incidentNodeDst().backIt(), TV01.backIt());

    EXPECT_EQ(TE09.IGBackwardLink(), E01_03.backIt());
    EXPECT_EQ(TE09.isFlipped(), false);
    EXPECT_EQ(TE09.isTerminalNsrc(), false);
    EXPECT_EQ(TE09.chainNsrc(), TE08.backIt());
    EXPECT_EQ(TE09.isTerminalNdst(), true);
    EXPECT_EQ(TE09.incidentNodeSrc().backIt(), TV01.backIt());
    EXPECT_EQ(TE09.incidentNodeDst().backIt(), TN03.backIt());

    EXPECT_EQ(TE10.IGBackwardLink(), E01_04.backIt());
    EXPECT_EQ(E01_04.IGForwardLink(), TE10.backIt());
    EXPECT_EQ(TE10.isFlipped(), false);
    EXPECT_EQ(TE10.isTerminalNsrc(), true);
    EXPECT_EQ(TE10.isTerminalNdst(), false);
    EXPECT_EQ(TE10.chainNdst(), TE11.backIt());
    EXPECT_EQ(TE10.incidentNodeSrc().backIt(), TN01.backIt());
    EXPECT_EQ(TE10.incidentNodeDst().backIt(), TV02.backIt());

    EXPECT_EQ(TE11.IGBackwardLink(), E01_04.backIt());
    EXPECT_EQ(TE11.isFlipped(), false);
    EXPECT_EQ(TE11.isTerminalNsrc(), false);
    EXPECT_EQ(TE11.chainNsrc(), TE10.backIt());
    EXPECT_EQ(TE11.isTerminalNdst(), true);
    EXPECT_EQ(TE11.incidentNodeSrc().backIt(), TV02.backIt());
    EXPECT_EQ(TE11.incidentNodeDst().backIt(), TN04.backIt());

    EXPECT_EQ(TE12.IGBackwardLink(), E01_05.backIt());
    EXPECT_EQ(E01_05.IGForwardLink(), TE12.backIt());
    EXPECT_EQ(TE12.isFlipped(), false);
    EXPECT_EQ(TE12.isTerminalNsrc(), true);
    EXPECT_EQ(TE12.isTerminalNdst(), false);
    EXPECT_EQ(TE12.chainNdst(), TE13.backIt());
    EXPECT_EQ(TE12.incidentNodeSrc().backIt(), TN01.backIt());
    EXPECT_EQ(TE12.incidentNodeDst().backIt(), TV03.backIt());

    EXPECT_EQ(TE13.IGBackwardLink(), E01_05.backIt());
    EXPECT_EQ(TE13.isFlipped(), false);
    EXPECT_EQ(TE13.isTerminalNsrc(), false);
    EXPECT_EQ(TE13.chainNsrc(), TE12.backIt());
    EXPECT_EQ(TE13.isTerminalNdst(), false);
    EXPECT_EQ(TE13.chainNdst(), TE14.backIt());
    EXPECT_EQ(TE13.incidentNodeSrc().backIt(), TV03.backIt());
    EXPECT_EQ(TE13.incidentNodeDst().backIt(), TV04.backIt());

    EXPECT_EQ(TE14.IGBackwardLink(), E01_05.backIt());
    EXPECT_EQ(TE14.isFlipped(), false);
    EXPECT_EQ(TE14.isTerminalNsrc(), false);
    EXPECT_EQ(TE14.chainNsrc(), TE13.backIt());
    EXPECT_EQ(TE14.isTerminalNdst(), true);
    EXPECT_EQ(TE14.incidentNodeSrc().backIt(), TV04.backIt());
    EXPECT_EQ(TE14.incidentNodeDst().backIt(), TN05.backIt());

    EXPECT_EQ(TE15.IGBackwardLink(), E02_05.backIt());
    EXPECT_EQ(E02_05.IGForwardLink(), TE15.backIt());
    EXPECT_EQ(TE15.isFlipped(), false);
    EXPECT_EQ(TE15.isTerminalNsrc(), true);
    EXPECT_EQ(TE15.isTerminalNdst(), false);
    EXPECT_EQ(TE15.chainNdst(), TE16.backIt());
    EXPECT_EQ(TE15.incidentNodeSrc().backIt(), TN02.backIt());
    EXPECT_EQ(TE15.incidentNodeDst().backIt(), TV05.backIt());

    EXPECT_EQ(TE16.IGBackwardLink(), E02_05.backIt());
    EXPECT_EQ(TE16.isFlipped(), false);
    EXPECT_EQ(TE16.isTerminalNsrc(), false);
    EXPECT_EQ(TE16.chainNsrc(), TE15.backIt());
    EXPECT_EQ(TE16.isTerminalNdst(), true);
    EXPECT_EQ(TE16.incidentNodeSrc().backIt(), TV05.backIt());
    EXPECT_EQ(TE16.incidentNodeDst().backIt(), TN05.backIt());

    EXPECT_EQ(TE17.IGBackwardLink(), E06_01.backIt());
    EXPECT_EQ(TE17.isFlipped(), true);
    EXPECT_EQ(TE17.isTerminalNsrc(), true);
    EXPECT_EQ(TE17.isTerminalNdst(), false);
    EXPECT_EQ(TE17.chainNdst(), TE18.backIt());
    EXPECT_EQ(TE17.incidentNodeSrc().backIt(), TN01.backIt());
    EXPECT_EQ(TE17.incidentNodeDst().backIt(), TV06.backIt());

    EXPECT_EQ(TE18.IGBackwardLink(), E06_01.backIt());
    EXPECT_EQ(TE18.isFlipped(), true);
    EXPECT_EQ(TE18.isTerminalNsrc(), false);
    EXPECT_EQ(TE18.chainNsrc(), TE17.backIt());
    EXPECT_EQ(TE18.isTerminalNdst(), false);
    EXPECT_EQ(TE18.chainNdst(), TE19.backIt());
    EXPECT_EQ(TE18.incidentNodeSrc().backIt(), TV06.backIt());
    EXPECT_EQ(TE18.incidentNodeDst().backIt(), TV07.backIt());

    EXPECT_EQ(TE19.IGBackwardLink(), E06_01.backIt());
    EXPECT_EQ(E06_01.IGForwardLink(), TE19.backIt());
    EXPECT_EQ(TE19.isFlipped(), true);
    EXPECT_EQ(TE19.isTerminalNsrc(), false);
    EXPECT_EQ(TE19.chainNsrc(), TE18.backIt());
    EXPECT_EQ(TE19.isTerminalNdst(), true);
    EXPECT_EQ(TE19.incidentNodeSrc().backIt(), TV07.backIt());
    EXPECT_EQ(TE19.incidentNodeDst().backIt(), TN06.backIt());

    EXPECT_EQ(TE20.IGBackwardLink(), E06_02.backIt());
    EXPECT_EQ(TE20.isFlipped(), true);
    EXPECT_EQ(TE20.isTerminalNsrc(), true);
    EXPECT_EQ(TE20.isTerminalNdst(), false);
    EXPECT_EQ(TE20.chainNdst(), TE21.backIt());
    EXPECT_EQ(TE20.incidentNodeSrc().backIt(), TN02.backIt());
    EXPECT_EQ(TE20.incidentNodeDst().backIt(), TV08.backIt());

    EXPECT_EQ(TE21.IGBackwardLink(), E06_02.backIt());
    EXPECT_EQ(E06_02.IGForwardLink(), TE21.backIt());
    EXPECT_EQ(TE21.isFlipped(), true);
    EXPECT_EQ(TE21.isTerminalNsrc(), false);
    EXPECT_EQ(TE21.chainNsrc(), TE20.backIt());
    EXPECT_EQ(TE21.isTerminalNdst(), true);
    EXPECT_EQ(TE21.incidentNodeSrc().backIt(), TV08.backIt());
    EXPECT_EQ(TE21.incidentNodeDst().backIt(), TN06.backIt());

    EXPECT_EQ(TE22.IGBackwardLink(), E07_02.backIt());
    EXPECT_EQ(TE22.isFlipped(), true);
    EXPECT_EQ(TE22.isTerminalNsrc(), true);
    EXPECT_EQ(TE22.isTerminalNdst(), false);
    EXPECT_EQ(TE22.chainNdst(), TE23.backIt());
    EXPECT_EQ(TE22.incidentNodeSrc().backIt(), TN02.backIt());
    EXPECT_EQ(TE22.incidentNodeDst().backIt(), TV09.backIt());

    EXPECT_EQ(TE23.IGBackwardLink(), E07_02.backIt());
    EXPECT_EQ(TE23.isFlipped(), true);
    EXPECT_EQ(TE23.isTerminalNsrc(), false);
    EXPECT_EQ(TE23.chainNsrc(), TE22.backIt());
    EXPECT_EQ(TE23.isTerminalNdst(), false);
    EXPECT_EQ(TE23.chainNdst(), TE24.backIt());
    EXPECT_EQ(TE23.incidentNodeSrc().backIt(), TV09.backIt());
    EXPECT_EQ(TE23.incidentNodeDst().backIt(), TV10.backIt());

    EXPECT_EQ(TE24.IGBackwardLink(), E07_02.backIt());
    EXPECT_EQ(E07_02.IGForwardLink(), TE24.backIt());
    EXPECT_EQ(TE24.isFlipped(), true);
    EXPECT_EQ(TE24.isTerminalNsrc(), false);
    EXPECT_EQ(TE24.chainNsrc(), TE23.backIt());
    EXPECT_EQ(TE24.isTerminalNdst(), true);
    EXPECT_EQ(TE24.incidentNodeSrc().backIt(), TV10.backIt());
    EXPECT_EQ(TE24.incidentNodeDst().backIt(), TN07.backIt());

    EXPECT_EQ(ranks_02.size(), 5);
    auto& vec_10 = ranks_02[0];
    EXPECT_EQ(vec_10.size(), 1);
    EXPECT_EQ(vec_10[0], TN01.backIt());

    auto& vec_11 = ranks_02[1];
    EXPECT_EQ(vec_11.size(), 5);
    EXPECT_EQ(vec_11[0], TN02.backIt());
    EXPECT_EQ(vec_11[1], TV01.backIt());
    EXPECT_EQ(vec_11[2], TV02.backIt());
    EXPECT_EQ(vec_11[3], TV03.backIt());
    EXPECT_EQ(vec_11[4], TV06.backIt());

    auto& vec_12 = ranks_02[2];
    EXPECT_EQ(vec_12.size(), 7);
    EXPECT_EQ(vec_12[0], TN03.backIt());
    EXPECT_EQ(vec_12[1], TN04.backIt());
    EXPECT_EQ(vec_12[2], TV04.backIt());
    EXPECT_EQ(vec_12[3], TV05.backIt());
    EXPECT_EQ(vec_12[4], TV07.backIt());
    EXPECT_EQ(vec_12[5], TV08.backIt());
    EXPECT_EQ(vec_12[6], TV09.backIt());

    auto& vec_13 = ranks_02[3];
    EXPECT_EQ(vec_13.size(), 3);
    EXPECT_EQ(vec_13[0], TN05.backIt());
    EXPECT_EQ(vec_13[1], TN06.backIt());
    EXPECT_EQ(vec_13[2], TV10.backIt());

    auto& vec_14 = ranks_02[4];
    EXPECT_EQ(vec_14.size(), 1);
    EXPECT_EQ(vec_14[0], TN07.backIt());

    g02.setPositionsForEachRank(ranks_02);

    EXPECT_EQ(TN01.rank(),  0);
    EXPECT_EQ(TN02.rank(),  1);
    EXPECT_EQ(TN03.rank(),  2);
    EXPECT_EQ(TN04.rank(),  2);
    EXPECT_EQ(TN05.rank(),  3);
    EXPECT_EQ(TN06.rank(),  3);
    EXPECT_EQ(TN07.rank(),  4);
    EXPECT_EQ(TV01.rank(),  1);
    EXPECT_EQ(TV02.rank(),  1);
    EXPECT_EQ(TV03.rank(),  1);
    EXPECT_EQ(TV04.rank(),  2);
    EXPECT_EQ(TV05.rank(),  2);
    EXPECT_EQ(TV06.rank(),  1);
    EXPECT_EQ(TV07.rank(),  2);
    EXPECT_EQ(TV08.rank(),  2);
    EXPECT_EQ(TV09.rank(),  2);
    EXPECT_EQ(TV10.rank(),  3);

    EXPECT_EQ(TN01.pos(),  0);
    EXPECT_EQ(TN02.pos(),  0);
    EXPECT_EQ(TN03.pos(),  0);
    EXPECT_EQ(TN04.pos(),  1);
    EXPECT_EQ(TN05.pos(),  0);
    EXPECT_EQ(TN06.pos(),  1);
    EXPECT_EQ(TN07.pos(),  0);
    EXPECT_EQ(TV01.pos(),  1);
    EXPECT_EQ(TV02.pos(),  2);
    EXPECT_EQ(TV03.pos(),  3);
    EXPECT_EQ(TV04.pos(),  2);
    EXPECT_EQ(TV05.pos(),  3);
    EXPECT_EQ(TV06.pos(),  4);
    EXPECT_EQ(TV07.pos(),  4);
    EXPECT_EQ(TV08.pos(),  5);
    EXPECT_EQ(TV09.pos(),  6);
    EXPECT_EQ(TV10.pos(),  2);

    EXPECT_EQ(TN01.degreeIn(), 0);
    EXPECT_EQ(TN01.degreeOut(), 5);
    auto iit = TN01.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE17.backIt());
    iit++;
    EXPECT_EQ(*iit, TE12.backIt());
    iit++;
    EXPECT_EQ(*iit, TE10.backIt());
    iit++;
    EXPECT_EQ(*iit, TE08.backIt());
    iit++;
    EXPECT_EQ(*iit, TE01.backIt());

    EXPECT_EQ(TN02.degreeIn(),  1);
    iit = TN02.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE01.backIt());
    EXPECT_EQ(TN02.degreeOut(), 5);
    iit = TN02.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE22.backIt());
    iit++;
    EXPECT_EQ(*iit, TE20.backIt());
    iit++;
    EXPECT_EQ(*iit, TE15.backIt());
    iit++;
    EXPECT_EQ(*iit, TE03.backIt());
    iit++;
    EXPECT_EQ(*iit, TE02.backIt());

    EXPECT_EQ(TN03.degreeIn(),  2);
    iit = TN03.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE02.backIt());
    iit++;
    EXPECT_EQ(*iit, TE09.backIt());
    EXPECT_EQ(TN03.degreeOut(),  1);
    iit = TN03.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE05.backIt());

    EXPECT_EQ(TN04.degreeIn(),  2);
    iit = TN04.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE03.backIt());
    iit++;
    EXPECT_EQ(*iit, TE11.backIt());
    EXPECT_EQ(TN04.degreeOut(),  1);
    iit = TN04.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE04.backIt());

    EXPECT_EQ(TN05.degreeIn(),  3);
    iit = TN05.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE04.backIt());
    iit++;
    EXPECT_EQ(*iit, TE14.backIt());
    iit++;
    EXPECT_EQ(*iit, TE16.backIt());
    EXPECT_EQ(TN05.degreeOut(), 1);
    iit = TN05.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE07.backIt());

    EXPECT_EQ(TN06.degreeIn(),  3);
    iit = TN06.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE05.backIt());
    iit++;
    EXPECT_EQ(*iit, TE19.backIt());
    iit++;
    EXPECT_EQ(*iit, TE21.backIt());
    EXPECT_EQ(TN06.degreeOut(), 1);
    iit = TN06.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE06.backIt());

    EXPECT_EQ(TN07.degreeIn(),  3);
    iit = TN07.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE07.backIt());
    iit++;
    EXPECT_EQ(*iit, TE06.backIt());
    iit++;
    EXPECT_EQ(*iit, TE24.backIt());
    EXPECT_EQ(TN07.degreeOut(), 0);

    EXPECT_EQ(TV01.degreeIn(),  1);
    iit = TV01.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE08.backIt());
    EXPECT_EQ(TV01.degreeOut(), 1);
    iit = TV01.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE09.backIt());

    EXPECT_EQ(TV02.degreeIn(),  1);
    iit = TV02.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE10.backIt());
    EXPECT_EQ(TV02.degreeOut(), 1);
    iit = TV02.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE11.backIt());

    EXPECT_EQ(TV03.degreeIn(),  1);
    iit = TV03.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE12.backIt());
    EXPECT_EQ(TV03.degreeOut(), 1);
    iit = TV03.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE13.backIt());

    EXPECT_EQ(TV04.degreeIn(),  1);
    iit = TV04.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE13.backIt());
    EXPECT_EQ(TV04.degreeOut(), 1);
    iit = TV04.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE14.backIt());

    EXPECT_EQ(TV05.degreeIn(),  1);
    iit = TV05.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE15.backIt());
    EXPECT_EQ(TV05.degreeOut(), 1);
    iit = TV05.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE16.backIt());

    EXPECT_EQ(TV06.degreeIn(),  1);
    iit = TV06.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE17.backIt());
    EXPECT_EQ(TV06.degreeOut(), 1);
    iit = TV06.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE18.backIt());

    EXPECT_EQ(TV07.degreeIn(),  1);
    iit = TV07.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE18.backIt());
    EXPECT_EQ(TV07.degreeOut(), 1);
    iit = TV07.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE19.backIt());

    EXPECT_EQ(TV08.degreeIn(),  1);
    iit = TV08.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE20.backIt());
    EXPECT_EQ(TV08.degreeOut(), 1);
    iit = TV08.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE21.backIt());

    EXPECT_EQ(TV09.degreeIn(),  1);
    iit = TV09.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE22.backIt());
    EXPECT_EQ(TV09.degreeOut(), 1);
    iit = TV09.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE23.backIt());

    EXPECT_EQ(TV10.degreeIn(),  1);
    iit = TV10.incidentEdgesIn().first;
    EXPECT_EQ(*iit, TE23.backIt());
    EXPECT_EQ(TV10.degreeOut(), 1);
    iit = TV10.incidentEdgesOut().first;
    EXPECT_EQ(*iit, TE24.backIt());

}

} // namespace Directed

} // namespace Wailea

