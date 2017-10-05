#include "gtest/gtest.h"
#include "directed/acyclic_ordering_finder.hpp"

namespace Wailea {

namespace Directed { 

class AcyclicOrderingFinderTests : public ::testing::Test {

  protected:  

    AcyclicOrderingFinderTests(){;};
    virtual ~AcyclicOrderingFinderTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    long findFeedbackEdges(AcyclicOrderingFinder& finder){
        return finder.findFeedbackEdges();
    };

    bool isFeedbackEdge(AOFEdge& e) { return e.mFeedbackEdge; }

    long findCyclicRanks(AcyclicOrderingFinder& finder, long n){
        return finder.findCyclicRanks(n);
    }

    long nodeCost(AOFNode& n) { return n.mCost; }

    void setRank(AOFNode& n, long r) { n.mRank = r; }

    vector<node_list_it_t> serializeCyclicRanks(
                                 AcyclicOrderingFinder& finder, long maxRank) {
        return finder.serializeCyclicRanks(maxRank);
    }

    size_t findMinimumRankGap(AcyclicOrderingFinder& finder,
                                         vector<node_list_it_t>& cyclicRanks) {
        return finder.findMinimumRankGap(cyclicRanks);
    }


};


/**  @brief step-by-step
 */
TEST_F(AcyclicOrderingFinderTests, Test1) {

    DiGraph g01;
 
    auto np01 = std::make_unique<AOFNode>();
    auto& N01 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np01)));
    N01.mDebugId = 1;
    auto np02 = std::make_unique<AOFNode>();
    auto& N02 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np02)));
    N02.mDebugId = 2;
    auto np03 = std::make_unique<AOFNode>();
    auto& N03 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np03)));
    N03.mDebugId = 3;
    auto np04 = std::make_unique<AOFNode>();
    auto& N04 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np04)));
    N04.mDebugId = 4;
    auto np05 = std::make_unique<AOFNode>();
    auto& N05 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np05)));
    N05.mDebugId = 5;

    auto ep01_02 = std::make_unique<AOFEdge>();
    auto& E01_02 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));
    E01_02.setCost(10);
    auto ep01_03 = std::make_unique<AOFEdge>();
    auto& E01_03 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));
    E01_03.setCost(8);
    auto ep04_01 = std::make_unique<AOFEdge>();
    auto& E04_01 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep04_01), N04, N01));
    E04_01.setCost(1);
    auto ep02_03 = std::make_unique<AOFEdge>();
    auto& E02_03 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));
    E02_03.setCost(2);
    auto ep03_04 = std::make_unique<AOFEdge>();
    auto& E03_04 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep03_04), N03, N04));
    E03_04.setCost(1);
    auto ep03_05 = std::make_unique<AOFEdge>();
    auto& E03_05 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep03_05), N03, N05));
    E03_05.setCost(4);
    auto ep04_05 = std::make_unique<AOFEdge>();
    auto& E04_05 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));
    E04_05.setCost(12);
    auto ep05_02 = std::make_unique<AOFEdge>();
    auto& E05_02 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep05_02), N05, N02));
    E05_02.setCost(7);

    AcyclicOrderingFinder finder(g01);

    long num_01 = findFeedbackEdges(finder);

    EXPECT_EQ(num_01, 2);
    EXPECT_EQ(isFeedbackEdge(E01_02), false);
    EXPECT_EQ(isFeedbackEdge(E01_03), false);
    EXPECT_EQ(isFeedbackEdge(E04_01), true);
    EXPECT_EQ(isFeedbackEdge(E02_03), false);
    EXPECT_EQ(isFeedbackEdge(E03_04), false);
    EXPECT_EQ(isFeedbackEdge(E03_05), false);
    EXPECT_EQ(isFeedbackEdge(E04_05), false);
    EXPECT_EQ(isFeedbackEdge(E05_02), true);

    long num_02 = findCyclicRanks(finder, num_01);

    EXPECT_EQ(num_02, 4);
    EXPECT_EQ(N01.rank(), 0);
    EXPECT_EQ(N02.rank(), 1);
    EXPECT_EQ(N03.rank(), 2);
    EXPECT_EQ(N04.rank(), 3);
    EXPECT_EQ(N05.rank(), 4);

    EXPECT_EQ(nodeCost(N01),  17);
    EXPECT_EQ(nodeCost(N02), -15);
    EXPECT_EQ(nodeCost(N03),  -5);
    EXPECT_EQ(nodeCost(N04),  12);
    EXPECT_EQ(nodeCost(N05),  -9);

    setRank(N01, 1);
    setRank(N02, 1);
    setRank(N03, 1);
    setRank(N04, 1);
    setRank(N05, 1);

    vector<node_list_it_t> vec_01 = serializeCyclicRanks(finder, num_02);

    EXPECT_EQ(N01.rank(), 4);
    EXPECT_EQ(N02.rank(), 0);
    EXPECT_EQ(N03.rank(), 2);
    EXPECT_EQ(N04.rank(), 3);
    EXPECT_EQ(N05.rank(), 1);

    EXPECT_EQ(vec_01.size(), 5);
    auto vit = vec_01.begin();
    EXPECT_EQ(*vit, N02.backIt());
    vit++;
    EXPECT_EQ(*vit, N05.backIt());
    vit++;
    EXPECT_EQ(*vit, N03.backIt());
    vit++;
    EXPECT_EQ(*vit, N04.backIt());
    vit++;
    EXPECT_EQ(*vit, N01.backIt());

    setRank(N01, 0);
    setRank(N02, 1);
    setRank(N03, 2);
    setRank(N04, 3);
    setRank(N05, 4);

    vector<node_list_it_t> vec_02;
    vec_02.push_back(N01.backIt());
    vec_02.push_back(N02.backIt());
    vec_02.push_back(N03.backIt());
    vec_02.push_back(N04.backIt());
    vec_02.push_back(N05.backIt());

    long num_03 = findMinimumRankGap(finder, vec_02);

    EXPECT_EQ(num_03, 3);

}


/**  @brief realistic 01
 */
TEST_F(AcyclicOrderingFinderTests, Test2) {

    DiGraph g01;
 
    auto np01 = std::make_unique<AOFNode>();
    auto& N01 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np01)));
    N01.mDebugId = 1;
    auto np02 = std::make_unique<AOFNode>();
    auto& N02 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np02)));
    N02.mDebugId = 2;
    auto np03 = std::make_unique<AOFNode>();
    auto& N03 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np03)));
    N03.mDebugId = 3;
    auto np04 = std::make_unique<AOFNode>();
    auto& N04 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np04)));
    N04.mDebugId = 4;
    auto np05 = std::make_unique<AOFNode>();
    auto& N05 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np05)));
    N05.mDebugId = 5;

    auto ep01_02 = std::make_unique<AOFEdge>();
    auto& E01_02 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));
    E01_02.setCost(10);
    auto ep01_03 = std::make_unique<AOFEdge>();
    auto& E01_03 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));
    E01_03.setCost(8);
    auto ep04_01 = std::make_unique<AOFEdge>();
    auto& E04_01 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep04_01), N04, N01));
    E04_01.setCost(1);
    auto ep02_03 = std::make_unique<AOFEdge>();
    auto& E02_03 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));
    E02_03.setCost(2);
    auto ep03_04 = std::make_unique<AOFEdge>();
    auto& E03_04 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep03_04), N03, N04));
    E03_04.setCost(1);
    auto ep03_05 = std::make_unique<AOFEdge>();
    auto& E03_05 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep03_05), N03, N05));
    E03_05.setCost(4);
    auto ep04_05 = std::make_unique<AOFEdge>();
    auto& E04_05 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));
    E04_05.setCost(12);
    auto ep05_02 = std::make_unique<AOFEdge>();
    auto& E05_02 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep05_02), N05, N02));
    E05_02.setCost(7);

    AcyclicOrderingFinder finder(g01);

    vector<node_list_it_t> vec_01 = finder.find();

    auto vit = vec_01.begin();
    EXPECT_EQ(*vit, N04.backIt());
    vit++;
    EXPECT_EQ(*vit, N05.backIt());
    vit++;
    EXPECT_EQ(*vit, N01.backIt());
    vit++;
    EXPECT_EQ(*vit, N02.backIt());
    vit++;
    EXPECT_EQ(*vit, N03.backIt());
    
}


/**  @brief realistic 02
 */
TEST_F(AcyclicOrderingFinderTests, Test3) {

    DiGraph g01;

    auto np01 = std::make_unique<AOFNode>();
    auto& N01 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np01)));
    N01.mDebugId = 1;
    auto np02 = std::make_unique<AOFNode>();
    auto& N02 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np02)));
    N02.mDebugId = 2;
    auto np03 = std::make_unique<AOFNode>();
    auto& N03 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np03)));
    N03.mDebugId = 3;
    auto np04 = std::make_unique<AOFNode>();
    auto& N04 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np04)));
    N04.mDebugId = 4;
    auto np05 = std::make_unique<AOFNode>();
    auto& N05 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np05)));
    N05.mDebugId = 5;
    auto np06 = std::make_unique<AOFNode>();
    auto& N06 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np06)));
    N06.mDebugId = 5;

    auto ep01_03 = std::make_unique<AOFEdge>();
    auto& E01_03 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));
    E01_03.setCost(2);
    auto ep02_01 = std::make_unique<AOFEdge>();
    auto& E02_01 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep02_01), N02, N01));
    E02_01.setCost(2);
    auto ep02_04 = std::make_unique<AOFEdge>();
    auto& E02_04 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep02_04), N02, N04));
    E02_04.setCost(5);
    auto ep03_02 = std::make_unique<AOFEdge>();
    auto& E03_02 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep03_02), N03, N02));
    E03_02.setCost(5);
    auto ep03_05 = std::make_unique<AOFEdge>();
    auto& E03_05 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep03_05), N03, N05));
    E03_05.setCost(4);
    auto ep05_04 = std::make_unique<AOFEdge>();
    auto& E05_04 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep05_04), N05, N04));
    E05_04.setCost(3);
    auto ep06_04 = std::make_unique<AOFEdge>();
    auto& E06_04 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep06_04), N06, N04));
    E06_04.setCost(1);
    auto ep06_05 = std::make_unique<AOFEdge>();
    auto& E06_05 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep06_05), N06, N05));
    E06_05.setCost(6);

    AcyclicOrderingFinder finder(g01);

    long num_01 = findFeedbackEdges(finder);

    EXPECT_EQ(num_01, 1);
    EXPECT_EQ(isFeedbackEdge(E01_03), false);
    EXPECT_EQ(isFeedbackEdge(E02_01), true);
    EXPECT_EQ(isFeedbackEdge(E02_04), false);
    EXPECT_EQ(isFeedbackEdge(E03_02), false);
    EXPECT_EQ(isFeedbackEdge(E03_05), false);
    EXPECT_EQ(isFeedbackEdge(E05_04), false);
    EXPECT_EQ(isFeedbackEdge(E06_04), false);
    EXPECT_EQ(isFeedbackEdge(E06_05), false);

    long num_02 = findCyclicRanks(finder, num_01);

    EXPECT_EQ(num_02, 3);
    EXPECT_EQ(N01.rank(), 0);
    EXPECT_EQ(N02.rank(), 2);
    EXPECT_EQ(N03.rank(), 1);
    EXPECT_EQ(N04.rank(), 3);
    EXPECT_EQ(N05.rank(), 2);
    EXPECT_EQ(N06.rank(), 1);

    vector<node_list_it_t> vec_01 = serializeCyclicRanks(finder, num_02);

    EXPECT_EQ(N01.rank(), 0);
    EXPECT_EQ(N02.rank(), 4);
    EXPECT_EQ(N03.rank(), 1);
    EXPECT_EQ(N04.rank(), 5);
    EXPECT_EQ(N05.rank(), 3);
    EXPECT_EQ(N06.rank(), 2);

    EXPECT_EQ(vec_01.size(), 6);
    auto vit = vec_01.begin();
    EXPECT_EQ(*vit, N01.backIt());
    vit++;
    EXPECT_EQ(*vit, N03.backIt());
    vit++;
    EXPECT_EQ(*vit, N06.backIt());
    vit++;
    EXPECT_EQ(*vit, N05.backIt());
    vit++;
    EXPECT_EQ(*vit, N02.backIt());
    vit++;
    EXPECT_EQ(*vit, N04.backIt());

    long num_03 = findMinimumRankGap(finder, vec_01);

    EXPECT_EQ(num_03, 0);

    vector<node_list_it_t> vec_02 = finder.find();

    vit = vec_02.begin();
    EXPECT_EQ(*vit, N01.backIt());
    vit++;
    EXPECT_EQ(*vit, N03.backIt());
    vit++;
    EXPECT_EQ(*vit, N06.backIt());
    vit++;
    EXPECT_EQ(*vit, N05.backIt());
    vit++;
    EXPECT_EQ(*vit, N02.backIt());
    vit++;
    EXPECT_EQ(*vit, N04.backIt());
    
}


/**  @brief empty graph
 */
TEST_F(AcyclicOrderingFinderTests, Test4) {

    DiGraph g01;

    AcyclicOrderingFinder finder(g01);

    long num_01 = findFeedbackEdges(finder);

    EXPECT_EQ(num_01, 0);

    long num_02 = findCyclicRanks(finder, num_01);

    EXPECT_EQ(num_02, 0);

    vector<node_list_it_t> vec_01 = serializeCyclicRanks(finder, num_02);

    EXPECT_EQ(vec_01.size(), 0);

    long num_03 = findMinimumRankGap(finder, vec_01);

    EXPECT_EQ(num_03, 0);

    vector<node_list_it_t> vec_02 = finder.find();
    EXPECT_EQ(vec_02.size(), 0);
    
}


/**  @brief single node
 */
TEST_F(AcyclicOrderingFinderTests, Test5) {

    DiGraph g01;

    auto np01 = std::make_unique<AOFNode>();
    auto& N01 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np01)));
    N01.mDebugId = 1;

    AcyclicOrderingFinder finder(g01);

    long num_01 = findFeedbackEdges(finder);

    EXPECT_EQ(num_01, 0);

    long num_02 = findCyclicRanks(finder, num_01);

    EXPECT_EQ(num_02, 0);

    vector<node_list_it_t> vec_01 = serializeCyclicRanks(finder, num_02);

    EXPECT_EQ(vec_01.size(), 1);
    EXPECT_EQ(*(vec_01.begin()), N01.backIt());

    long num_03 = findMinimumRankGap(finder, vec_01);

    EXPECT_EQ(num_03, 0);

    vector<node_list_it_t> vec_02 = finder.find();
    EXPECT_EQ(vec_02.size(), 1);
    EXPECT_EQ(*(vec_02.begin()), N01.backIt());    
}



/**  @brief single edge
 */
TEST_F(AcyclicOrderingFinderTests, Test6) {

    DiGraph g01;

    auto np01 = std::make_unique<AOFNode>();
    auto& N01 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np01)));
    N01.mDebugId = 1;

    auto np02 = std::make_unique<AOFNode>();
    auto& N02 = dynamic_cast<AOFNode&>(g01.addNode(std::move(np02)));
    N02.mDebugId = 1;

    auto ep01_02 = std::make_unique<AOFEdge>();
    auto& E01_02 = dynamic_cast<AOFEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));
    E01_02.setCost(1);

    AcyclicOrderingFinder finder(g01);

    long num_01 = findFeedbackEdges(finder);

    EXPECT_EQ(num_01, 0);

    long num_02 = findCyclicRanks(finder, num_01);

    EXPECT_EQ(num_02, 1);

    vector<node_list_it_t> vec_01 = serializeCyclicRanks(finder, num_02);

    EXPECT_EQ(vec_01.size(), 2);
    auto vit = vec_01.begin();
    EXPECT_EQ(*vit, N01.backIt());
    vit++;
    EXPECT_EQ(*vit, N02.backIt());

    long num_03 = findMinimumRankGap(finder, vec_01);

    EXPECT_EQ(num_03, 0);

    vector<node_list_it_t> vec_02 = finder.find();
    EXPECT_EQ(vec_02.size(), 2);
    vit = vec_02.begin();
    EXPECT_EQ(*vit, N01.backIt());
    vit++;
    EXPECT_EQ(*vit, N02.backIt());

}

} // namespace Directed

} // namespace Wailea
