#include "gtest/gtest.h"
#include "directed/gknv_crossings_reducer.hpp"

// Python script to generate a piece of code to generate a test graph.
//import fileinput
//nlist = []
//elist = []
//for line in fileinput.input():
//    fields = line.strip().split()
//    if len(fields)==2:
//        nsrc = int(fields[0])
//        ndst = int(fields[1])
//        nlist.append(nsrc)
//        nlist.append(ndst)
//        elist.append(('%03d' % nsrc , '%03d' % ndst))
//nmin = min(nlist)
//nmax = max(nlist)
//for i in range(nmin,nmax+1):
//    if i in nlist:
//        ns = '%03d' % i
//        print '    auto np' + ns + ' = std::make_unique<NumDiNode>(' + str(i) + ');'
//        print '    auto& N' + ns + ' = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np' + ns + ')));'
//        print ''
//
//for e in elist:
//    estr = e[0] + '_' + e[1]
//    print '    auto ep' + estr + ' = std::make_unique<DiEdge>();'
//    print '    //auto& E' + estr + ' = dynamic_cast<DiEdge&>('
//    print '        g01.addEdge(std::move(ep' + estr + '), N' + e[0] + ' , N' + e[1] + ');'
//    print ''


namespace Wailea {

namespace Directed { 

class GKNVcrossingsReducerTests : public ::testing::Test {

  protected:  

    GKNVcrossingsReducerTests(){;};
    virtual ~GKNVcrossingsReducerTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    void initialOrderByDFS(
        GKNVcrossingsReducer& r,
        vector<vector<node_list_it_t> >& ranks
    ){
        r.initialOrderByDFS(ranks);
    }

    long calculateNumberOfCrossings(
        GKNVcrossingsReducer&   r,
        vector<node_list_it_t>& leftNodes,
        vector<node_list_it_t>& rightNodes
    ) {
        return r.calculateNumberOfCrossings(leftNodes,rightNodes);
    } 


    void reorderRightSideByMedianHeuristic(
        GKNVcrossingsReducer&   r,
        vector<node_list_it_t>& leftNodes,
        vector<node_list_it_t>& rightNodes,
        vector<node_list_it_t>& newRightNodes
    ) {
        r.reorderRightSideByMedianHeuristic(
                                          leftNodes,rightNodes,newRightNodes);
    }

    void reorderLeftSideByMedianHeuristic(
        GKNVcrossingsReducer&   r,
        vector<node_list_it_t>& leftNodes,
        vector<node_list_it_t>& rightNodes,
        vector<node_list_it_t>& newLeftNodes
    ) {
        r.reorderLeftSideByMedianHeuristic(
                                          leftNodes,rightNodes,newLeftNodes);
    }


};


/**  @brief test initialOrderByDFS() empty graph
 */
TEST_F(GKNVcrossingsReducerTests, Test1) {

    DiGraph g01;
    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t>>ranks;

    initialOrderByDFS(reducer, ranks);
    EXPECT_EQ(ranks.size(),0);
}


/**  @brief test initialOrderByDFS() one node
 */
TEST_F(GKNVcrossingsReducerTests, Test2) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);

    initialOrderByDFS(reducer, ranks);

    EXPECT_EQ(ranks.size(),1);
    auto& outRank_00 = ranks[0];
    EXPECT_EQ(outRank_00.size(),1);
    EXPECT_EQ(outRank_00[0], N01.backIt());

}


/**  @brief test initialOrderByDFS() one edge
 */
TEST_F(GKNVcrossingsReducerTests, Test3) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto ep01_02 = std::make_unique<DiEdge>();

//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);

    initialOrderByDFS(reducer, ranks);

    EXPECT_EQ(ranks.size(),2);

    auto& outRank_00 = ranks[0];
    EXPECT_EQ(outRank_00.size(),1);
    EXPECT_EQ(outRank_00[0], N01.backIt());

    auto& outRank_01 = ranks[1];
    EXPECT_EQ(outRank_01.size(),1);
    EXPECT_EQ(outRank_01[0], N02.backIt());
}


/**  @brief test initialOrderByDFS() two edges
 */
TEST_F(GKNVcrossingsReducerTests, Test4) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep01_03 = std::make_unique<DiEdge>();
//    auto& E01_03 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());
    inRank_01.push_back(N03.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);

    initialOrderByDFS(reducer, ranks);

    EXPECT_EQ(ranks.size(),2);

    auto& outRank_00 = ranks[0];
    EXPECT_EQ(outRank_00.size(),1);
    EXPECT_EQ(outRank_00[0], N01.backIt());

    auto& outRank_01 = ranks[1];
    EXPECT_EQ(outRank_01.size(),2);
    EXPECT_EQ(outRank_01[0], N02.backIt());
    EXPECT_EQ(outRank_01[1], N03.backIt());
}


/**  @brief test initialOrderByDFS() two edges #2
 */
TEST_F(GKNVcrossingsReducerTests, Test5) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep03_02 = std::make_unique<DiEdge>();
//    auto& E03_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep03_02), N03, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());
    inRank_00.push_back(N03.backIt());

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);

    initialOrderByDFS(reducer, ranks);

    EXPECT_EQ(ranks.size(),2);

    auto& outRank_00 = ranks[0];
    EXPECT_EQ(outRank_00.size(),2);
    EXPECT_EQ(outRank_00[0], N01.backIt());
    EXPECT_EQ(outRank_00[1], N03.backIt());

    auto& outRank_01 = ranks[1];
    EXPECT_EQ(outRank_01.size(),1);
    EXPECT_EQ(outRank_01[0], N02.backIt());

}


/**  @brief test initialOrderByDFS() realistic
 */
TEST_F(GKNVcrossingsReducerTests, Test6) {

#include "unit_tests/directed/unit_tests_gknv_crossings_reducer_test_graph_01.hpp"

    GKNVcrossingsReducer  reducer(g01);

    initialOrderByDFS(reducer, ranks);

    EXPECT_EQ(ranks.size(), 9);
    auto& outRank_00 = ranks[0];
    EXPECT_EQ(outRank_00.size(), 5);
    EXPECT_EQ(outRank_00[0], N024.backIt());
    EXPECT_EQ(outRank_00[1], N008.backIt());
    EXPECT_EQ(outRank_00[2], N035.backIt());
    EXPECT_EQ(outRank_00[3], N001.backIt());
    EXPECT_EQ(outRank_00[4], N030.backIt());

    auto& outRank_01 = ranks[1];
    EXPECT_EQ(outRank_01.size(), 10);
    EXPECT_EQ(outRank_01[0], N050.backIt());
    EXPECT_EQ(outRank_01[1], N051.backIt());
    EXPECT_EQ(outRank_01[2], N052.backIt());
    EXPECT_EQ(outRank_01[3], N036.backIt());
    EXPECT_EQ(outRank_01[4], N043.backIt());
    EXPECT_EQ(outRank_01[5], N002.backIt());
    EXPECT_EQ(outRank_01[6], N010.backIt());
    EXPECT_EQ(outRank_01[7], N053.backIt());
    EXPECT_EQ(outRank_01[8], N054.backIt());
    EXPECT_EQ(outRank_01[9], N037.backIt());

    auto& outRank_02 = ranks[2];
    EXPECT_EQ(outRank_02.size(), 20);
    EXPECT_EQ(outRank_02[0], N055.backIt());
    EXPECT_EQ(outRank_02[1], N025.backIt());
    EXPECT_EQ(outRank_02[2], N009.backIt());
    EXPECT_EQ(outRank_02[3], N056.backIt());
    EXPECT_EQ(outRank_02[4], N038.backIt());
    EXPECT_EQ(outRank_02[5], N040.backIt());
    EXPECT_EQ(outRank_02[6], N017.backIt());
    EXPECT_EQ(outRank_02[7], N059.backIt());
    EXPECT_EQ(outRank_02[8], N018.backIt());
    EXPECT_EQ(outRank_02[9], N060.backIt());
    EXPECT_EQ(outRank_02[10], N061.backIt());
    EXPECT_EQ(outRank_02[11], N013.backIt());
    EXPECT_EQ(outRank_02[12], N012.backIt());
    EXPECT_EQ(outRank_02[13], N062.backIt());
    EXPECT_EQ(outRank_02[14], N063.backIt());
    EXPECT_EQ(outRank_02[15], N064.backIt());
    EXPECT_EQ(outRank_02[16], N065.backIt());
    EXPECT_EQ(outRank_02[17], N066.backIt());
    EXPECT_EQ(outRank_02[18], N057.backIt());
    EXPECT_EQ(outRank_02[19], N058.backIt());

    auto& outRank_03 = ranks[3];
    EXPECT_EQ(outRank_03.size(), 19);
    EXPECT_EQ(outRank_03[0], N067.backIt());
    EXPECT_EQ(outRank_03[1], N068.backIt());
    EXPECT_EQ(outRank_03[2], N026.backIt());
    EXPECT_EQ(outRank_03[3], N069.backIt());
    EXPECT_EQ(outRank_03[4], N042.backIt());
    EXPECT_EQ(outRank_03[5], N019.backIt());
    EXPECT_EQ(outRank_03[6], N070.backIt());
    EXPECT_EQ(outRank_03[7], N003.backIt());
    EXPECT_EQ(outRank_03[8], N072.backIt());
    EXPECT_EQ(outRank_03[9], N016.backIt());
    EXPECT_EQ(outRank_03[10], N073.backIt());
    EXPECT_EQ(outRank_03[11], N074.backIt());
    EXPECT_EQ(outRank_03[12], N011.backIt());
    EXPECT_EQ(outRank_03[13], N014.backIt());
    EXPECT_EQ(outRank_03[14], N075.backIt());
    EXPECT_EQ(outRank_03[15], N076.backIt());
    EXPECT_EQ(outRank_03[16], N033.backIt());
    EXPECT_EQ(outRank_03[17], N039.backIt());
    EXPECT_EQ(outRank_03[18], N071.backIt());

    auto& outRank_04 = ranks[4];
    EXPECT_EQ(outRank_04.size(), 18);
    EXPECT_EQ(outRank_04[0], N077.backIt());
    EXPECT_EQ(outRank_04[1], N078.backIt());
    EXPECT_EQ(outRank_04[2], N004.backIt());
    EXPECT_EQ(outRank_04[3], N079.backIt());
    EXPECT_EQ(outRank_04[4], N021.backIt());
    EXPECT_EQ(outRank_04[5], N020.backIt());
    EXPECT_EQ(outRank_04[6], N028.backIt());
    EXPECT_EQ(outRank_04[7], N082.backIt());
    EXPECT_EQ(outRank_04[8], N081.backIt());
    EXPECT_EQ(outRank_04[9], N084.backIt());
    EXPECT_EQ(outRank_04[10], N083.backIt());
    EXPECT_EQ(outRank_04[11], N085.backIt());
    EXPECT_EQ(outRank_04[12], N086.backIt());
    EXPECT_EQ(outRank_04[13], N031.backIt());
    EXPECT_EQ(outRank_04[14], N034.backIt());
    EXPECT_EQ(outRank_04[15], N087.backIt());
    EXPECT_EQ(outRank_04[16], N080.backIt());
    EXPECT_EQ(outRank_04[17], N041.backIt());

    auto& outRank_05 = ranks[5];
    EXPECT_EQ(outRank_05.size(), 12);
    EXPECT_EQ(outRank_05[0], N027.backIt());
    EXPECT_EQ(outRank_05[1], N088.backIt());
    EXPECT_EQ(outRank_05[2], N005.backIt());
    EXPECT_EQ(outRank_05[3], N089.backIt());
    EXPECT_EQ(outRank_05[4], N022.backIt());
    EXPECT_EQ(outRank_05[5], N015.backIt());
    EXPECT_EQ(outRank_05[6], N029.backIt());
    EXPECT_EQ(outRank_05[7], N090.backIt());
    EXPECT_EQ(outRank_05[8], N091.backIt());
    EXPECT_EQ(outRank_05[9], N032.backIt());
    EXPECT_EQ(outRank_05[10], N092.backIt());
    EXPECT_EQ(outRank_05[11], N093.backIt());

    auto& outRank_06 = ranks[6];
    EXPECT_EQ(outRank_06.size(), 13);
    EXPECT_EQ(outRank_06[0], N094.backIt());
    EXPECT_EQ(outRank_06[1], N095.backIt());
    EXPECT_EQ(outRank_06[2], N097.backIt());
    EXPECT_EQ(outRank_06[3], N006.backIt());
    EXPECT_EQ(outRank_06[4], N023.backIt());
    EXPECT_EQ(outRank_06[5], N096.backIt());
    EXPECT_EQ(outRank_06[6], N098.backIt());
    EXPECT_EQ(outRank_06[7], N099.backIt());
    EXPECT_EQ(outRank_06[8], N101.backIt());
    EXPECT_EQ(outRank_06[9], N100.backIt());
    EXPECT_EQ(outRank_06[10], N102.backIt());
    EXPECT_EQ(outRank_06[11], N103.backIt());
    EXPECT_EQ(outRank_06[12], N104.backIt());

    auto& outRank_07 = ranks[7];
    EXPECT_EQ(outRank_07.size(), 14);
    EXPECT_EQ(outRank_07[0], N105.backIt());
    EXPECT_EQ(outRank_07[1], N106.backIt());
    EXPECT_EQ(outRank_07[2], N108.backIt());
    EXPECT_EQ(outRank_07[3], N007.backIt());
    EXPECT_EQ(outRank_07[4], N109.backIt());
    EXPECT_EQ(outRank_07[5], N111.backIt());
    EXPECT_EQ(outRank_07[6], N107.backIt());
    EXPECT_EQ(outRank_07[7], N110.backIt());
    EXPECT_EQ(outRank_07[8], N112.backIt());
    EXPECT_EQ(outRank_07[9], N115.backIt());
    EXPECT_EQ(outRank_07[10], N113.backIt());
    EXPECT_EQ(outRank_07[11], N114.backIt());
    EXPECT_EQ(outRank_07[12], N116.backIt());
    EXPECT_EQ(outRank_07[13], N117.backIt());

    auto& outRank_08 = ranks[8];
    EXPECT_EQ(outRank_08.size(), 5);
    EXPECT_EQ(outRank_08[0], N046.backIt());
    EXPECT_EQ(outRank_08[1], N044.backIt());
    EXPECT_EQ(outRank_08[2], N048.backIt());
    EXPECT_EQ(outRank_08[3], N045.backIt());
    EXPECT_EQ(outRank_08[4], N047.backIt());

}


/**  @brief test calculateNumberOfCrossings()
 */
TEST_F(GKNVcrossingsReducerTests, Test7) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));


    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;

    vector<node_list_it_t>inRank_01;

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);


    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);

    EXPECT_EQ(num_01,0);

}


/**  @brief test calculateNumberOfCrossings()
 */
TEST_F(GKNVcrossingsReducerTests, Test8) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));


    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());

    vector<node_list_it_t>inRank_01;

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);


    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);

    EXPECT_EQ(num_01,0);

}


/**  @brief test calculateNumberOfCrossings()
 */
TEST_F(GKNVcrossingsReducerTests, Test9) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));


    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);


    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);

    EXPECT_EQ(num_01,0);

}


/**  @brief test calculateNumberOfCrossings()
 */
TEST_F(GKNVcrossingsReducerTests, Test10) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));


    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);


    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);

    EXPECT_EQ(num_01,0);

}


/**  @brief test calculateNumberOfCrossings()
 */
TEST_F(GKNVcrossingsReducerTests, Test11) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NumDiNode>(4);
    auto& N04 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np04)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep03_04 = std::make_unique<DiEdge>();
//    auto& E03_04 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep03_04), N03, N04);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N01.backIt());
    inRank_00.push_back(N03.backIt());

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());
    inRank_01.push_back(N04.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);

    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);

    EXPECT_EQ(num_01,0);

}


/**  @brief test calculateNumberOfCrossings()
 */
TEST_F(GKNVcrossingsReducerTests, Test12) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NumDiNode>(4);
    auto& N04 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np04)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep03_04 = std::make_unique<DiEdge>();
//    auto& E03_04 = dynamic_cast<DiEdge&>(
                                    g01.addEdge(std::move(ep03_04), N03, N04);

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t>inRank_00;
    inRank_00.push_back(N03.backIt());
    inRank_00.push_back(N01.backIt());

    vector<node_list_it_t>inRank_01;
    inRank_01.push_back(N02.backIt());
    inRank_01.push_back(N04.backIt());

    vector<vector<node_list_it_t>>ranks;
    ranks.push_back(inRank_00);
    ranks.push_back(inRank_01);

    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);

    EXPECT_EQ(num_01,1);

}


/**  @brief test calculateNumberOfCrossings() realistic
 */
TEST_F(GKNVcrossingsReducerTests, Test13) {

#include "unit_tests/directed/unit_tests_gknv_crossings_reducer_test_graph_01.hpp"

    GKNVcrossingsReducer  reducer(g01);

    auto num_01 = calculateNumberOfCrossings(reducer, inRank_00, inRank_01);
    EXPECT_EQ(num_01,0);
    auto num_02 = calculateNumberOfCrossings(reducer, inRank_01, inRank_02);
    EXPECT_EQ(num_02,1);
    auto num_03 = calculateNumberOfCrossings(reducer, inRank_02, inRank_03);
    EXPECT_EQ(num_03,9);
    auto num_04 = calculateNumberOfCrossings(reducer, inRank_03, inRank_04);
    EXPECT_EQ(num_04,18);
    auto num_05 = calculateNumberOfCrossings(reducer, inRank_04, inRank_05);
    EXPECT_EQ(num_05,10);
    auto num_06 = calculateNumberOfCrossings(reducer, inRank_05, inRank_06);
    EXPECT_EQ(num_06,6);
    auto num_07 = calculateNumberOfCrossings(reducer, inRank_06, inRank_07);
    EXPECT_EQ(num_07,2);
    auto num_08 = calculateNumberOfCrossings(reducer, inRank_07, inRank_08);
    EXPECT_EQ(num_08,12);

}


/**  @brief test reorderRightSideByMedianHeuristic()
 */
TEST_F(GKNVcrossingsReducerTests, Test14) {

#include "unit_tests/directed/unit_tests_gknv_crossings_reducer_test_graph_01.hpp"

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t> outRank;

    reorderRightSideByMedianHeuristic(reducer, inRank_00, inRank_01, outRank);

    EXPECT_EQ(outRank.size(), 10);
    EXPECT_EQ(outRank[0], N050.backIt());
    EXPECT_EQ(outRank[1], N051.backIt());
    EXPECT_EQ(outRank[2], N052.backIt());
    EXPECT_EQ(outRank[3], N036.backIt());
    EXPECT_EQ(outRank[4], N043.backIt());
    EXPECT_EQ(outRank[5], N037.backIt());
    EXPECT_EQ(outRank[6], N002.backIt());
    EXPECT_EQ(outRank[7], N010.backIt());
    EXPECT_EQ(outRank[8], N053.backIt());
    EXPECT_EQ(outRank[9], N054.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_01, inRank_02, outRank);

    EXPECT_EQ(outRank.size(), 20);
    EXPECT_EQ(outRank[ 0], N055.backIt());
    EXPECT_EQ(outRank[ 1], N025.backIt());
    EXPECT_EQ(outRank[ 2], N009.backIt());
    EXPECT_EQ(outRank[ 3], N056.backIt());
    EXPECT_EQ(outRank[ 4], N038.backIt());
    EXPECT_EQ(outRank[ 5], N040.backIt());
    EXPECT_EQ(outRank[ 6], N057.backIt());
    EXPECT_EQ(outRank[ 7], N058.backIt());
    EXPECT_EQ(outRank[ 8], N017.backIt());
    EXPECT_EQ(outRank[ 9], N059.backIt());
    EXPECT_EQ(outRank[10], N018.backIt());
    EXPECT_EQ(outRank[11], N060.backIt());
    EXPECT_EQ(outRank[12], N061.backIt());
    EXPECT_EQ(outRank[13], N013.backIt());
    EXPECT_EQ(outRank[14], N012.backIt());
    EXPECT_EQ(outRank[15], N062.backIt());
    EXPECT_EQ(outRank[16], N063.backIt());
    EXPECT_EQ(outRank[17], N064.backIt());
    EXPECT_EQ(outRank[18], N065.backIt());
    EXPECT_EQ(outRank[19], N066.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_02, inRank_03, outRank);

    EXPECT_EQ(outRank.size(), 19);
    EXPECT_EQ(outRank[ 0], N067.backIt());
    EXPECT_EQ(outRank[ 1], N068.backIt());
    EXPECT_EQ(outRank[ 2], N026.backIt());
    EXPECT_EQ(outRank[ 3], N069.backIt());
    EXPECT_EQ(outRank[ 4], N042.backIt());
    EXPECT_EQ(outRank[ 5], N070.backIt());
    EXPECT_EQ(outRank[ 6], N039.backIt());
    EXPECT_EQ(outRank[ 7], N071.backIt());
    EXPECT_EQ(outRank[ 8], N019.backIt());
    EXPECT_EQ(outRank[ 9], N003.backIt());
    EXPECT_EQ(outRank[10], N072.backIt());
    EXPECT_EQ(outRank[11], N016.backIt());
    EXPECT_EQ(outRank[12], N073.backIt());
    EXPECT_EQ(outRank[13], N074.backIt());
    EXPECT_EQ(outRank[14], N011.backIt());
    EXPECT_EQ(outRank[15], N014.backIt());
    EXPECT_EQ(outRank[16], N075.backIt());
    EXPECT_EQ(outRank[17], N076.backIt());
    EXPECT_EQ(outRank[18], N033.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_03, inRank_04, outRank);

    EXPECT_EQ(outRank.size(), 18);
    EXPECT_EQ(outRank[ 0], N077.backIt());
    EXPECT_EQ(outRank[ 1], N078.backIt());
    EXPECT_EQ(outRank[ 2], N079.backIt());
    EXPECT_EQ(outRank[ 3], N004.backIt());
    EXPECT_EQ(outRank[ 4], N080.backIt());
    EXPECT_EQ(outRank[ 5], N021.backIt());
    EXPECT_EQ(outRank[ 6], N020.backIt());
    EXPECT_EQ(outRank[ 7], N028.backIt());
    EXPECT_EQ(outRank[ 8], N041.backIt());
    EXPECT_EQ(outRank[ 9], N082.backIt());
    EXPECT_EQ(outRank[10], N081.backIt());
    EXPECT_EQ(outRank[11], N084.backIt());
    EXPECT_EQ(outRank[12], N083.backIt());
    EXPECT_EQ(outRank[13], N085.backIt());
    EXPECT_EQ(outRank[14], N086.backIt());
    EXPECT_EQ(outRank[15], N031.backIt());
    EXPECT_EQ(outRank[16], N034.backIt());
    EXPECT_EQ(outRank[17], N087.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_04, inRank_05, outRank);

    EXPECT_EQ(outRank.size(), 12);
    EXPECT_EQ(outRank[ 0], N027.backIt());
    EXPECT_EQ(outRank[ 1], N088.backIt());
    EXPECT_EQ(outRank[ 2], N089.backIt());
    EXPECT_EQ(outRank[ 3], N005.backIt());
    EXPECT_EQ(outRank[ 4], N022.backIt());
    EXPECT_EQ(outRank[ 5], N015.backIt());
    EXPECT_EQ(outRank[ 6], N029.backIt());
    EXPECT_EQ(outRank[ 7], N090.backIt());
    EXPECT_EQ(outRank[ 8], N091.backIt());
    EXPECT_EQ(outRank[ 9], N032.backIt());
    EXPECT_EQ(outRank[10], N092.backIt());
    EXPECT_EQ(outRank[11], N093.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_05, inRank_06, outRank);

    EXPECT_EQ(outRank.size(), 13);
    EXPECT_EQ(outRank[ 0], N094.backIt());
    EXPECT_EQ(outRank[ 1], N095.backIt());
    EXPECT_EQ(outRank[ 2], N096.backIt());
    EXPECT_EQ(outRank[ 3], N097.backIt());
    EXPECT_EQ(outRank[ 4], N006.backIt());
    EXPECT_EQ(outRank[ 5], N098.backIt());
    EXPECT_EQ(outRank[ 6], N023.backIt());
    EXPECT_EQ(outRank[ 7], N099.backIt());
    EXPECT_EQ(outRank[ 8], N101.backIt());
    EXPECT_EQ(outRank[ 9], N100.backIt());
    EXPECT_EQ(outRank[10], N102.backIt());
    EXPECT_EQ(outRank[11], N103.backIt());
    EXPECT_EQ(outRank[12], N104.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_06, inRank_07, outRank);

    EXPECT_EQ(outRank.size(), 14);
    EXPECT_EQ(outRank[ 0], N105.backIt());
    EXPECT_EQ(outRank[ 1], N106.backIt());
    EXPECT_EQ(outRank[ 2], N107.backIt());
    EXPECT_EQ(outRank[ 3], N108.backIt());
    EXPECT_EQ(outRank[ 4], N007.backIt());
    EXPECT_EQ(outRank[ 5], N110.backIt());
    EXPECT_EQ(outRank[ 6], N109.backIt());
    EXPECT_EQ(outRank[ 7], N111.backIt());
    EXPECT_EQ(outRank[ 8], N112.backIt());
    EXPECT_EQ(outRank[ 9], N113.backIt());
    EXPECT_EQ(outRank[10], N115.backIt());
    EXPECT_EQ(outRank[11], N114.backIt());
    EXPECT_EQ(outRank[12], N116.backIt());
    EXPECT_EQ(outRank[13], N117.backIt());

    reorderRightSideByMedianHeuristic(reducer, inRank_07, inRank_08, outRank);

    EXPECT_EQ(outRank.size(), 5);
    EXPECT_EQ(outRank[ 0], N046.backIt());
    EXPECT_EQ(outRank[ 1], N045.backIt());
    EXPECT_EQ(outRank[ 2], N048.backIt());
    EXPECT_EQ(outRank[ 3], N044.backIt());
    EXPECT_EQ(outRank[ 4], N047.backIt());

}


/**  @brief test reorderLeftSideByMedianHeuristic()
 */
TEST_F(GKNVcrossingsReducerTests, Test15) {

#include "unit_tests/directed/unit_tests_gknv_crossings_reducer_test_graph_01.hpp"

    GKNVcrossingsReducer  reducer(g01);

    vector<node_list_it_t> outRank;

    reorderLeftSideByMedianHeuristic(reducer, inRank_07, inRank_08, outRank);

    EXPECT_EQ(outRank.size(), 14);
    EXPECT_EQ(outRank[ 0], N105.backIt());
    EXPECT_EQ(outRank[ 1], N109.backIt());
    EXPECT_EQ(outRank[ 2], N007.backIt());
    EXPECT_EQ(outRank[ 3], N108.backIt());
    EXPECT_EQ(outRank[ 4], N110.backIt());
    EXPECT_EQ(outRank[ 5], N106.backIt());
    EXPECT_EQ(outRank[ 6], N107.backIt());
    EXPECT_EQ(outRank[ 7], N111.backIt());
    EXPECT_EQ(outRank[ 8], N112.backIt());
    EXPECT_EQ(outRank[ 9], N113.backIt());
    EXPECT_EQ(outRank[10], N114.backIt());
    EXPECT_EQ(outRank[11], N116.backIt());
    EXPECT_EQ(outRank[12], N115.backIt());
    EXPECT_EQ(outRank[13], N117.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_06, inRank_07, outRank);

    EXPECT_EQ(outRank.size(), 13);
    EXPECT_EQ(outRank[ 0], N094.backIt());
    EXPECT_EQ(outRank[ 1], N095.backIt());
    EXPECT_EQ(outRank[ 2], N096.backIt());
    EXPECT_EQ(outRank[ 3], N097.backIt());
    EXPECT_EQ(outRank[ 4], N006.backIt());
    EXPECT_EQ(outRank[ 5], N098.backIt());
    EXPECT_EQ(outRank[ 6], N023.backIt());
    EXPECT_EQ(outRank[ 7], N099.backIt());
    EXPECT_EQ(outRank[ 8], N100.backIt());
    EXPECT_EQ(outRank[ 9], N102.backIt());
    EXPECT_EQ(outRank[10], N101.backIt());
    EXPECT_EQ(outRank[11], N103.backIt());
    EXPECT_EQ(outRank[12], N104.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_05, inRank_06, outRank);

    EXPECT_EQ(outRank.size(), 12);
    EXPECT_EQ(outRank[ 0], N027.backIt());
    EXPECT_EQ(outRank[ 1], N088.backIt());
    EXPECT_EQ(outRank[ 2], N089.backIt());
    EXPECT_EQ(outRank[ 3], N005.backIt());
    EXPECT_EQ(outRank[ 4], N022.backIt());
    EXPECT_EQ(outRank[ 5], N032.backIt());
    EXPECT_EQ(outRank[ 6], N015.backIt());
    EXPECT_EQ(outRank[ 7], N090.backIt());
    EXPECT_EQ(outRank[ 8], N029.backIt());
    EXPECT_EQ(outRank[ 9], N091.backIt());
    EXPECT_EQ(outRank[10], N092.backIt());
    EXPECT_EQ(outRank[11], N093.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_04, inRank_05, outRank);

    EXPECT_EQ(outRank.size(), 18);
    EXPECT_EQ(outRank[ 0], N077.backIt());
    EXPECT_EQ(outRank[ 1], N078.backIt());
    EXPECT_EQ(outRank[ 2], N079.backIt());
    EXPECT_EQ(outRank[ 3], N004.backIt());
    EXPECT_EQ(outRank[ 4], N021.backIt());
    EXPECT_EQ(outRank[ 5], N080.backIt());
    EXPECT_EQ(outRank[ 6], N020.backIt());
    EXPECT_EQ(outRank[ 7], N081.backIt());
    EXPECT_EQ(outRank[ 8], N085.backIt());
    EXPECT_EQ(outRank[ 9], N028.backIt());
    EXPECT_EQ(outRank[10], N041.backIt());
    EXPECT_EQ(outRank[11], N082.backIt());
    EXPECT_EQ(outRank[12], N083.backIt());
    EXPECT_EQ(outRank[13], N034.backIt());
    EXPECT_EQ(outRank[14], N084.backIt());
    EXPECT_EQ(outRank[15], N086.backIt());
    EXPECT_EQ(outRank[16], N031.backIt());
    EXPECT_EQ(outRank[17], N087.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_03, inRank_04, outRank);

    EXPECT_EQ(outRank.size(), 19);
    EXPECT_EQ(outRank[ 0], N067.backIt());
    EXPECT_EQ(outRank[ 1], N068.backIt());
    EXPECT_EQ(outRank[ 2], N069.backIt());
    EXPECT_EQ(outRank[ 3], N026.backIt());
    EXPECT_EQ(outRank[ 4], N042.backIt());
    EXPECT_EQ(outRank[ 5], N070.backIt());
    EXPECT_EQ(outRank[ 6], N003.backIt());
    EXPECT_EQ(outRank[ 7], N011.backIt());
    EXPECT_EQ(outRank[ 8], N039.backIt());
    EXPECT_EQ(outRank[ 9], N019.backIt());
    EXPECT_EQ(outRank[10], N071.backIt());
    EXPECT_EQ(outRank[11], N016.backIt());
    EXPECT_EQ(outRank[12], N072.backIt());
    EXPECT_EQ(outRank[13], N074.backIt());
    EXPECT_EQ(outRank[14], N073.backIt());
    EXPECT_EQ(outRank[15], N014.backIt());
    EXPECT_EQ(outRank[16], N075.backIt());
    EXPECT_EQ(outRank[17], N076.backIt());
    EXPECT_EQ(outRank[18], N033.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_02, inRank_03, outRank);

    EXPECT_EQ(outRank.size(), 20);
    EXPECT_EQ(outRank[ 0], N055.backIt());
    EXPECT_EQ(outRank[ 1], N025.backIt());
    EXPECT_EQ(outRank[ 2], N009.backIt());
    EXPECT_EQ(outRank[ 3], N038.backIt());
    EXPECT_EQ(outRank[ 4], N057.backIt());
    EXPECT_EQ(outRank[ 5], N056.backIt());
    EXPECT_EQ(outRank[ 6], N040.backIt());
    EXPECT_EQ(outRank[ 7], N017.backIt());
    EXPECT_EQ(outRank[ 8], N013.backIt());
    EXPECT_EQ(outRank[ 9], N058.backIt());
    EXPECT_EQ(outRank[10], N059.backIt());
    EXPECT_EQ(outRank[11], N018.backIt());
    EXPECT_EQ(outRank[12], N060.backIt());
    EXPECT_EQ(outRank[13], N061.backIt());
    EXPECT_EQ(outRank[14], N012.backIt());
    EXPECT_EQ(outRank[15], N062.backIt());
    EXPECT_EQ(outRank[16], N063.backIt());
    EXPECT_EQ(outRank[17], N064.backIt());
    EXPECT_EQ(outRank[18], N065.backIt());
    EXPECT_EQ(outRank[19], N066.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_01, inRank_02, outRank);

    EXPECT_EQ(outRank.size(), 10);
    EXPECT_EQ(outRank[ 0], N050.backIt());
    EXPECT_EQ(outRank[ 1], N051.backIt());
    EXPECT_EQ(outRank[ 2], N052.backIt());
    EXPECT_EQ(outRank[ 3], N036.backIt());
    EXPECT_EQ(outRank[ 4], N043.backIt());
    EXPECT_EQ(outRank[ 5], N037.backIt());
    EXPECT_EQ(outRank[ 6], N002.backIt());
    EXPECT_EQ(outRank[ 7], N010.backIt());
    EXPECT_EQ(outRank[ 8], N053.backIt());
    EXPECT_EQ(outRank[ 9], N054.backIt());

    reorderLeftSideByMedianHeuristic(reducer, inRank_00, inRank_01, outRank);

    EXPECT_EQ(outRank.size(), 5);
    EXPECT_EQ(outRank[ 0], N024.backIt());
    EXPECT_EQ(outRank[ 1], N008.backIt());
    EXPECT_EQ(outRank[ 2], N035.backIt());
    EXPECT_EQ(outRank[ 3], N001.backIt());
    EXPECT_EQ(outRank[ 4], N030.backIt());

}


/**  @brief test reduce()
 */
TEST_F(GKNVcrossingsReducerTests, Test16) {

#include "unit_tests/directed/unit_tests_gknv_crossings_reducer_test_graph_01.hpp"

    GKNVcrossingsReducer  reducer(g01);
    reducer.reduce(ranks, 100);

    EXPECT_EQ(ranks.size(), 9);
    auto& outRank_00 = ranks[0];
    EXPECT_EQ(outRank_00.size(), 5);
    EXPECT_EQ(outRank_00[0], N024.backIt());
    EXPECT_EQ(outRank_00[1], N008.backIt());
    EXPECT_EQ(outRank_00[2], N035.backIt());
    EXPECT_EQ(outRank_00[3], N001.backIt());
    EXPECT_EQ(outRank_00[4], N030.backIt());

    auto& outRank_01 = ranks[1];
    EXPECT_EQ(outRank_01.size(), 10);
    EXPECT_EQ(outRank_01[0], N050.backIt());
    EXPECT_EQ(outRank_01[1], N051.backIt());
    EXPECT_EQ(outRank_01[2], N052.backIt());
    EXPECT_EQ(outRank_01[3], N036.backIt());
    EXPECT_EQ(outRank_01[4], N002.backIt());
    EXPECT_EQ(outRank_01[5], N043.backIt());
    EXPECT_EQ(outRank_01[6], N010.backIt());
    EXPECT_EQ(outRank_01[7], N053.backIt());
    EXPECT_EQ(outRank_01[8], N054.backIt());
    EXPECT_EQ(outRank_01[9], N037.backIt());
    auto& outRank_02 = ranks[2];
    EXPECT_EQ(outRank_02.size(), 20);
    EXPECT_EQ(outRank_02[0], N055.backIt());
    EXPECT_EQ(outRank_02[1], N025.backIt());
    EXPECT_EQ(outRank_02[2], N009.backIt());
    EXPECT_EQ(outRank_02[3], N056.backIt());
    EXPECT_EQ(outRank_02[4], N017.backIt());
    EXPECT_EQ(outRank_02[5], N059.backIt());
    EXPECT_EQ(outRank_02[6], N018.backIt());
    EXPECT_EQ(outRank_02[7], N060.backIt());
    EXPECT_EQ(outRank_02[8], N061.backIt());
    EXPECT_EQ(outRank_02[9], N040.backIt());
    EXPECT_EQ(outRank_02[10], N013.backIt());
    EXPECT_EQ(outRank_02[11], N038.backIt());
    EXPECT_EQ(outRank_02[12], N062.backIt());
    EXPECT_EQ(outRank_02[13], N012.backIt());
    EXPECT_EQ(outRank_02[14], N063.backIt());
    EXPECT_EQ(outRank_02[15], N064.backIt());
    EXPECT_EQ(outRank_02[16], N065.backIt());
    EXPECT_EQ(outRank_02[17], N066.backIt());
    EXPECT_EQ(outRank_02[18], N057.backIt());
    EXPECT_EQ(outRank_02[19], N058.backIt());

    auto& outRank_03 = ranks[3];
    EXPECT_EQ(outRank_03.size(), 19);
    EXPECT_EQ(outRank_03[0], N067.backIt());
    EXPECT_EQ(outRank_03[1], N068.backIt());
    EXPECT_EQ(outRank_03[2], N026.backIt());
    EXPECT_EQ(outRank_03[3], N069.backIt());
    EXPECT_EQ(outRank_03[4], N042.backIt());
    EXPECT_EQ(outRank_03[5], N019.backIt());
    EXPECT_EQ(outRank_03[6], N003.backIt());
    EXPECT_EQ(outRank_03[7], N070.backIt());
    EXPECT_EQ(outRank_03[8], N072.backIt());
    EXPECT_EQ(outRank_03[9], N016.backIt());
    EXPECT_EQ(outRank_03[10], N073.backIt());
    EXPECT_EQ(outRank_03[11], N011.backIt());
    EXPECT_EQ(outRank_03[12], N074.backIt());
    EXPECT_EQ(outRank_03[13], N014.backIt());
    EXPECT_EQ(outRank_03[14], N075.backIt());
    EXPECT_EQ(outRank_03[15], N076.backIt());
    EXPECT_EQ(outRank_03[16], N039.backIt());
    EXPECT_EQ(outRank_03[17], N033.backIt());
    EXPECT_EQ(outRank_03[18], N071.backIt());

    auto& outRank_04 = ranks[4];
    EXPECT_EQ(outRank_04.size(), 18);
    EXPECT_EQ(outRank_04[0], N077.backIt());
    EXPECT_EQ(outRank_04[1], N078.backIt());
    EXPECT_EQ(outRank_04[2], N079.backIt());
    EXPECT_EQ(outRank_04[3], N021.backIt());
    EXPECT_EQ(outRank_04[4], N020.backIt());
    EXPECT_EQ(outRank_04[5], N028.backIt());
    EXPECT_EQ(outRank_04[6], N004.backIt());
    EXPECT_EQ(outRank_04[7], N082.backIt());
    EXPECT_EQ(outRank_04[8], N081.backIt());
    EXPECT_EQ(outRank_04[9], N084.backIt());
    EXPECT_EQ(outRank_04[10], N083.backIt());
    EXPECT_EQ(outRank_04[11], N085.backIt());
    EXPECT_EQ(outRank_04[12], N086.backIt());
    EXPECT_EQ(outRank_04[13], N080.backIt());
    EXPECT_EQ(outRank_04[14], N034.backIt());
    EXPECT_EQ(outRank_04[15], N031.backIt());
    EXPECT_EQ(outRank_04[16], N087.backIt());
    EXPECT_EQ(outRank_04[17], N041.backIt());

    auto& outRank_05 = ranks[5];
    EXPECT_EQ(outRank_05.size(), 12);
    EXPECT_EQ(outRank_05[0], N027.backIt());
    EXPECT_EQ(outRank_05[1], N088.backIt());
    EXPECT_EQ(outRank_05[2], N005.backIt());
    EXPECT_EQ(outRank_05[3], N089.backIt());
    EXPECT_EQ(outRank_05[4], N022.backIt());
    EXPECT_EQ(outRank_05[5], N015.backIt());
    EXPECT_EQ(outRank_05[6], N090.backIt());
    EXPECT_EQ(outRank_05[7], N029.backIt());
    EXPECT_EQ(outRank_05[8], N091.backIt());
    EXPECT_EQ(outRank_05[9], N032.backIt());
    EXPECT_EQ(outRank_05[10], N092.backIt());
    EXPECT_EQ(outRank_05[11], N093.backIt());

    auto& outRank_06 = ranks[6];
    EXPECT_EQ(outRank_06.size(), 13);
    EXPECT_EQ(outRank_06[0], N094.backIt());
    EXPECT_EQ(outRank_06[1], N095.backIt());
    EXPECT_EQ(outRank_06[2], N006.backIt());
    EXPECT_EQ(outRank_06[3], N097.backIt());
    EXPECT_EQ(outRank_06[4], N023.backIt());
    EXPECT_EQ(outRank_06[5], N096.backIt());
    EXPECT_EQ(outRank_06[6], N098.backIt());
    EXPECT_EQ(outRank_06[7], N099.backIt());
    EXPECT_EQ(outRank_06[8], N100.backIt());
    EXPECT_EQ(outRank_06[9], N101.backIt());
    EXPECT_EQ(outRank_06[10], N102.backIt());
    EXPECT_EQ(outRank_06[11], N103.backIt());
    EXPECT_EQ(outRank_06[12], N104.backIt());

    auto& outRank_07 = ranks[7];
    EXPECT_EQ(outRank_07.size(), 14);
    EXPECT_EQ(outRank_07[0], N105.backIt());
    EXPECT_EQ(outRank_07[1], N106.backIt());
    EXPECT_EQ(outRank_07[2], N007.backIt());
    EXPECT_EQ(outRank_07[3], N108.backIt());
    EXPECT_EQ(outRank_07[4], N109.backIt());
    EXPECT_EQ(outRank_07[5], N111.backIt());
    EXPECT_EQ(outRank_07[6], N107.backIt());
    EXPECT_EQ(outRank_07[7], N110.backIt());
    EXPECT_EQ(outRank_07[8], N112.backIt());
    EXPECT_EQ(outRank_07[9], N113.backIt());
    EXPECT_EQ(outRank_07[10], N115.backIt());
    EXPECT_EQ(outRank_07[11], N114.backIt());
    EXPECT_EQ(outRank_07[12], N116.backIt());
    EXPECT_EQ(outRank_07[13], N117.backIt());

    auto& outRank_08 = ranks[8];
    EXPECT_EQ(outRank_08.size(), 5);
    EXPECT_EQ(outRank_08[0], N046.backIt());
    EXPECT_EQ(outRank_08[1], N045.backIt());
    EXPECT_EQ(outRank_08[2], N048.backIt());
    EXPECT_EQ(outRank_08[3], N044.backIt());
    EXPECT_EQ(outRank_08[4], N047.backIt());

//    long index = 0;
//    for (auto& rank : ranks) {
//        cerr << "Rank " << index << ":";
//        for (auto nit : rank) {
//            auto& N = dynamic_cast<NumDiNode&>(*(*nit));
//            cerr << N.num() << " ";
//        }
//        cerr << "\n";
//        index++;
//    }

//    auto nPair = g01.nodes();
//    for (auto nit = nPair.first ; nit != nPair.second; nit++) {
//        auto& N = dynamic_cast<NumDiNode&>(*(*nit));
//
//        auto iPairIn = N.incidentEdgesIn();
//        cerr << "Node " << N.num() << " In: ";
//        for (auto iit = iPairIn.first; iit != iPairIn.second; iit++) {
//            auto& E = dynamic_cast<DiEdge&>(*(*(*iit)));
//            auto& A = dynamic_cast<NumDiNode&>(E.adjacentNode(N));
//            cerr << A.num() << " ";
//        }
//        cerr << "\n";
//
//        auto iPairOut = N.incidentEdgesOut();
//        cerr << "Node " << N.num() << " Out: ";
//        for (auto iit = iPairOut.first; iit != iPairOut.second; iit++) {
//            auto& E = dynamic_cast<DiEdge&>(*(*(*iit)));
//            auto& A = dynamic_cast<NumDiNode&>(E.adjacentNode(N));
//            cerr << A.num() << " ";
//        }
//        cerr << "\n";
//    }

}


/**  @brief test initialOrderByDFS() empty
 */
TEST_F(GKNVcrossingsReducerTests, Test17) {

    DiGraph g01;

//    auto np01 = std::make_unique<NumDiNode>(1);
//    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

//    auto np02 = std::make_unique<NumDiNode>(2);
//    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

//    auto np03 = std::make_unique<NumDiNode>(3);
//    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

//    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
//                                  g01.addEdge(std::move(ep01_02), N01, N02);

//    auto ep01_03 = std::make_unique<DiEdge>();
//    auto& E01_03 = dynamic_cast<DiEdge&>(
//                                  g01.addEdge(std::move(ep01_03), N01, N03);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;

    reducer.reduce(ranks, 100);
    
}


/**  @brief test initialOrderByDFS() one node
 */
TEST_F(GKNVcrossingsReducerTests, Test18) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

//    auto np02 = std::make_unique<NumDiNode>(2);
//    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

//    auto np03 = std::make_unique<NumDiNode>(3);
//    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

//    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
//                                  g01.addEdge(std::move(ep01_02), N01, N02);

//    auto ep01_03 = std::make_unique<DiEdge>();
//    auto& E01_03 = dynamic_cast<DiEdge&>(
//                                  g01.addEdge(std::move(ep01_03), N01, N03);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;
    vector<node_list_it_t> rank_00;
    rank_00.push_back(N01.backIt());
    ranks.push_back(rank_00);

    reducer.reduce(ranks, 100);
    
}


/**  @brief test initialOrderByDFS() one edge
 */
TEST_F(GKNVcrossingsReducerTests, Test19) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

//    auto np03 = std::make_unique<NumDiNode>(3);
//    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep01_02), N01, N02);

//    auto ep01_03 = std::make_unique<DiEdge>();
//    auto& E01_03 = dynamic_cast<DiEdge&>(
//                                  g01.addEdge(std::move(ep01_03), N01, N03);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;
    vector<node_list_it_t> rank_00;
    rank_00.push_back(N01.backIt());

    vector<node_list_it_t> rank_01;
    rank_01.push_back(N02.backIt());
    ranks.push_back(rank_00);
    ranks.push_back(rank_01);

    reducer.reduce(ranks, 100);
    
}


/**  @brief test initialOrderByDFS() two edges
 */
TEST_F(GKNVcrossingsReducerTests, Test20) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep01_03 = std::make_unique<DiEdge>();
//    auto& E01_03 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep01_03), N01, N03);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;
    vector<node_list_it_t> rank_00;
    rank_00.push_back(N01.backIt());

    vector<node_list_it_t> rank_01;
    rank_01.push_back(N02.backIt());
    rank_01.push_back(N03.backIt());
    ranks.push_back(rank_00);
    ranks.push_back(rank_01);

    reducer.reduce(ranks, 100);
    
}


/**  @brief test initialOrderByDFS() two edges
 */
TEST_F(GKNVcrossingsReducerTests, Test21) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep03_02 = std::make_unique<DiEdge>();
//    auto& E03_02 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep03_02), N03, N02);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;
    vector<node_list_it_t> rank_00;
    rank_00.push_back(N01.backIt());
    rank_00.push_back(N03.backIt());

    vector<node_list_it_t> rank_01;
    rank_01.push_back(N02.backIt());

    ranks.push_back(rank_00);
    ranks.push_back(rank_01);

    reducer.reduce(ranks, 100);
    
}


/**  @brief test initialOrderByDFS() two edges
 */
TEST_F(GKNVcrossingsReducerTests, Test22) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NumDiNode>(4);
    auto& N04 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np04)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep03_04 = std::make_unique<DiEdge>();
//    auto& E03_04 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep03_04), N03, N04);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;
    vector<node_list_it_t> rank_00;
    rank_00.push_back(N01.backIt());
    rank_00.push_back(N03.backIt());

    vector<node_list_it_t> rank_01;
    rank_01.push_back(N02.backIt());
    rank_01.push_back(N04.backIt());

    ranks.push_back(rank_00);
    ranks.push_back(rank_01);

    reducer.reduce(ranks, 100);
    
}


/**  @brief test initialOrderByDFS() two edges
 */
TEST_F(GKNVcrossingsReducerTests, Test23) {

    DiGraph g01;

    auto np01 = std::make_unique<NumDiNode>(1);
    auto& N01 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NumDiNode>(2);
    auto& N02 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NumDiNode>(3);
    auto& N03 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NumDiNode>(4);
    auto& N04 = dynamic_cast<NumDiNode&>(g01.addNode(std::move(np04)));

    auto ep01_02 = std::make_unique<DiEdge>();
//    auto& E01_02 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep01_02), N01, N02);

    auto ep03_04 = std::make_unique<DiEdge>();
//    auto& E03_04 = dynamic_cast<DiEdge&>(
                                  g01.addEdge(std::move(ep03_04), N03, N04);

    GKNVcrossingsReducer  reducer(g01);

    vector<vector<node_list_it_t> > ranks;
    vector<node_list_it_t> rank_00;
    rank_00.push_back(N01.backIt());
    rank_00.push_back(N03.backIt());

    vector<node_list_it_t> rank_01;
    rank_01.push_back(N04.backIt());
    rank_01.push_back(N02.backIt());

    ranks.push_back(rank_00);
    ranks.push_back(rank_01);

    reducer.reduce(ranks, 100);
    
}

} // namespace Directed

} // namespace Wailea

