#include "gtest/gtest.h"
#include "directed/network_simplex.hpp"

namespace Wailea {

namespace Directed { 

class NetworkSimplexTests : public ::testing::Test {

  protected:  

    NetworkSimplexTests(){;};
    virtual ~NetworkSimplexTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief Empty graph
 */
TEST_F(NetworkSimplexTests, Test1) {

    DiGraph g01;
    NetworkSimplex  ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);

}


/**  @brief Graph with single node
 */
TEST_F(NetworkSimplexTests, Test2) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));
    N01.setB(0);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);
    EXPECT_EQ(N01.y(), 0);

}


/**  @brief Graph with single node invalid
 */
TEST_F(NetworkSimplexTests, Test3) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));
    N01.setB(-1);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), false);
}


/**  @brief Graph with two nodes
 */
TEST_F(NetworkSimplexTests, Test4) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>();
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));
    N01.setB(1);
    N02.setB(-1);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), false);
}


/**  @brief Graph with single edge
 */
TEST_F(NetworkSimplexTests, Test5) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>();
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));

    N01.setB(1);
    N02.setB(-1);
    E01_02.setC(1);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);
    EXPECT_EQ(E01_02.x(), 1);
    EXPECT_EQ(N01.y(), 0);
    EXPECT_EQ(N02.y(), 1);
}


/**  @brief Graph with single edge infeasible
 */
TEST_F(NetworkSimplexTests, Test6) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>();
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));

    N01.setB(-1);
    N02.setB(1);
    E01_02.setC(1);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), false);
//    EXPECT_EQ(E01_02.x(), -1);
}


/**  @brief Graph with single edge zero supply/demand
 */
TEST_F(NetworkSimplexTests, Test7) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>();
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));

    N01.setB(0);
    N02.setB(0);
    E01_02.setC(1);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);
    EXPECT_EQ(E01_02.x(), 0);
    EXPECT_EQ(N01.y(), 0);
    EXPECT_EQ(N02.y(), 1);
}


/**  @brief Graph with two serial edges feasible
 */
TEST_F(NetworkSimplexTests, Test8) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));
    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    N01.setB(2);
    N02.setB(0);
    N03.setB(-2);
    E01_02.setC(1);
    E02_03.setC(2);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);
    EXPECT_EQ(E01_02.x(), 2);
    EXPECT_EQ(E02_03.x(), 2);
    EXPECT_EQ(N01.y(), 0);
    EXPECT_EQ(N02.y(), 1);
    EXPECT_EQ(N03.y(), 3);
}


/**  @brief Graph with two serial edges infeasible
 */
TEST_F(NetworkSimplexTests, Test9) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>();
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>();
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));
    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    N01.setB(-1);
    N02.setB(0);
    N03.setB(1);
    E01_02.setC(1);
    E02_03.setC(2);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), false);
}


/**  @brief Graph with two serial edges zero supply/demand
 */
TEST_F(NetworkSimplexTests, Test10) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>();
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>();
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>();
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));
    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    N01.setB(0);
    N02.setB(0);
    N03.setB(0);
    E01_02.setC(1);
    E02_03.setC(2);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);
    EXPECT_EQ(E01_02.x(), 0);
    EXPECT_EQ(E02_03.x(), 0);
    EXPECT_EQ(N01.y(), 0);
    EXPECT_EQ(N02.y(), 1);
    EXPECT_EQ(N03.y(), 3);
}


/**  @brief Graph realistic feasible
 */
TEST_F(NetworkSimplexTests, Test11) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto np06 = std::make_unique<NetworkSimplexNode>(6);
    auto& N06 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np06)));

    auto np07 = std::make_unique<NetworkSimplexNode>(7);
    auto& N07 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np07)));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep01_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_05), N01, N05));

    auto ep02_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_01), N02, N01));

    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep02_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_05), N02, N05));

    auto ep04_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_02), N04, N02));

    auto ep04_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));

    auto ep06_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_01), N06, N01));

    auto ep06_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_02), N06, N02));

    auto ep06_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_03), N06, N03));

    auto ep06_07 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_07 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_07), N06, N07));

    auto ep07_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_02), N07, N02));

    auto ep07_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_05), N07, N05));

    N01.setB(0);
    N02.setB(0);
    N03.setB(-6);
    N04.setB(-6);
    N05.setB(-2);
    N06.setB(9);
    N07.setB(5);

    E01_03.setC(48);
    E01_04.setC(28);
    E01_05.setC(10);
    E02_01.setC(4);
    E02_03.setC(65);
    E02_05.setC(7);
    E04_02.setC(38);
    E04_05.setC(15);
    E06_01.setC(56);
    E06_02.setC(48);
    E06_03.setC(108);
    E06_07.setC(24);
    E07_02.setC(33);
    E07_05.setC(19);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);

    EXPECT_EQ(N01.y(),   0);
    EXPECT_EQ(N02.y(),  -4);
    EXPECT_EQ(N03.y(),  48);
    EXPECT_EQ(N04.y(),  28);
    EXPECT_EQ(N05.y(), -18);
    EXPECT_EQ(N06.y(), -52);
    EXPECT_EQ(N07.y(), -37);

    EXPECT_EQ(E01_03.x(),   6);
    EXPECT_EQ(E01_04.x(),   6);
    EXPECT_EQ(E01_05.x(),   0);
    EXPECT_EQ(E02_01.x(),  12);
    EXPECT_EQ(E02_03.x(),   0);
    EXPECT_EQ(E02_05.x(),   0);
    EXPECT_EQ(E04_02.x(),   0);
    EXPECT_EQ(E04_05.x(),   0);
    EXPECT_EQ(E06_01.x(),   0);
    EXPECT_EQ(E06_02.x(),   9);
    EXPECT_EQ(E06_03.x(),   0);
    EXPECT_EQ(E06_07.x(),   0);
    EXPECT_EQ(E07_02.x(),   3);
    EXPECT_EQ(E07_05.x(),   2);

}


/**  @brief Graph realistic infeasible
 */
TEST_F(NetworkSimplexTests, Test12) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto np06 = std::make_unique<NetworkSimplexNode>(6);
    auto& N06 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np06)));

    auto np07 = std::make_unique<NetworkSimplexNode>(7);
    auto& N07 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np07)));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep01_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_05), N01, N05));

    auto ep02_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_01), N02, N01));

    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep02_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_05), N02, N05));

    auto ep04_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_02), N04, N02));

    auto ep04_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));

    auto ep06_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_01), N06, N01));

    auto ep06_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_02), N06, N02));

    auto ep06_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_03), N06, N03));

    auto ep06_07 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_07 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_07), N06, N07));

    auto ep07_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_02), N07, N02));

    auto ep07_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_05), N07, N05));

    N01.setB(0);
    N02.setB(0);
    N03.setB(6);
    N04.setB(6);
    N05.setB(2);
    N06.setB(-9);
    N07.setB(-5);

    E01_03.setC(48);
    E01_04.setC(28);
    E01_05.setC(10);
    E02_01.setC(4);
    E02_03.setC(65);
    E02_05.setC(7);
    E04_02.setC(38);
    E04_05.setC(15);
    E06_01.setC(56);
    E06_02.setC(48);
    E06_03.setC(108);
    E06_07.setC(24);
    E07_02.setC(33);
    E07_05.setC(19);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), false);

}


/**  @brief Graph realistic zero supply/demand
 */
TEST_F(NetworkSimplexTests, Test13) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto np06 = std::make_unique<NetworkSimplexNode>(6);
    auto& N06 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np06)));

    auto np07 = std::make_unique<NetworkSimplexNode>(7);
    auto& N07 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np07)));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep01_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_05), N01, N05));

    auto ep02_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_01), N02, N01));

    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep02_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_05), N02, N05));

    auto ep04_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_02), N04, N02));

    auto ep04_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));

    auto ep06_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_01), N06, N01));

    auto ep06_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_02), N06, N02));

    auto ep06_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_03), N06, N03));

    auto ep06_07 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_07 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_07), N06, N07));

    auto ep07_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_02), N07, N02));

    auto ep07_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_05), N07, N05));

    N01.setB(0);
    N02.setB(0);
    N03.setB(0);
    N04.setB(0);
    N05.setB(0);
    N06.setB(0);
    N07.setB(0);

    E01_03.setC(48);
    E01_04.setC(28);
    E01_05.setC(10);
    E02_01.setC(4);
    E02_03.setC(65);
    E02_05.setC(7);
    E04_02.setC(38);
    E04_05.setC(15);
    E06_01.setC(56);
    E06_02.setC(48);
    E06_03.setC(108);
    E06_07.setC(24);
    E07_02.setC(33);
    E07_05.setC(19);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);



    EXPECT_EQ(N01.y(),   0);
    EXPECT_EQ(N02.y(),  -4);
    EXPECT_EQ(N03.y(),  48);
    EXPECT_EQ(N04.y(),  28);
    EXPECT_EQ(N05.y(), -18);
    EXPECT_EQ(N06.y(), -52);
    EXPECT_EQ(N07.y(), -37);

    EXPECT_EQ(E01_03.x(),   0);
    EXPECT_EQ(E01_04.x(),   0);
    EXPECT_EQ(E01_05.x(),   0);
    EXPECT_EQ(E02_01.x(),   0);
    EXPECT_EQ(E02_03.x(),   0);
    EXPECT_EQ(E02_05.x(),   0);
    EXPECT_EQ(E04_02.x(),   0);
    EXPECT_EQ(E04_05.x(),   0);
    EXPECT_EQ(E06_01.x(),   0);
    EXPECT_EQ(E06_02.x(),   0);
    EXPECT_EQ(E06_03.x(),   0);
    EXPECT_EQ(E06_07.x(),   0);
    EXPECT_EQ(E07_02.x(),   0);
    EXPECT_EQ(E07_05.x(),   0);

}


/**  @brief Graph realistic spanning tree
 */
TEST_F(NetworkSimplexTests, Test14) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto np06 = std::make_unique<NetworkSimplexNode>(6);
    auto& N06 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np06)));

    auto np07 = std::make_unique<NetworkSimplexNode>(7);
    auto& N07 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np07)));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep02_01 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_01 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_01), N02, N01));

    auto ep06_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E06_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep06_02), N06, N02));

    auto ep07_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_02), N07, N02));

    auto ep07_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E07_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep07_05), N07, N05));

    N01.setB(0);
    N02.setB(0);
    N03.setB(-6);
    N04.setB(-6);
    N05.setB(-2);
    N06.setB(9);
    N07.setB(5);

    E01_03.setC(48);
    E01_04.setC(28);
    E02_01.setC(4);
    E06_02.setC(48);
    E07_02.setC(33);
    E07_05.setC(19);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);

    EXPECT_EQ(N01.y(),   0);
    EXPECT_EQ(N02.y(),  -4);
    EXPECT_EQ(N03.y(),  48);
    EXPECT_EQ(N04.y(),  28);
    EXPECT_EQ(N05.y(), -18);
    EXPECT_EQ(N06.y(), -52);
    EXPECT_EQ(N07.y(), -37);

    EXPECT_EQ(E01_03.x(),   6);
    EXPECT_EQ(E01_04.x(),   6);
    EXPECT_EQ(E02_01.x(),  12);
    EXPECT_EQ(E06_02.x(),   9);
    EXPECT_EQ(E07_02.x(),   3);
    EXPECT_EQ(E07_05.x(),   2);

}



/**  @brief Graph realistic
 */
TEST_F(NetworkSimplexTests, Test15) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep03_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E03_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep03_04), N03, N04));

    auto ep03_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E03_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep03_05), N03, N05));

    auto ep04_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));

    auto ep05_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E05_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep05_02), N05, N02));

    N01.setB(10);
    N02.setB(4);
    N03.setB(0);
    N04.setB(-6);
    N05.setB(-8);

    E01_02.setC(10);
    E01_03.setC( 8);
    E01_04.setC( 1);
    E02_03.setC( 2);
    E03_04.setC( 1);
    E03_05.setC( 4);
    E04_05.setC(12);
    E05_02.setC( 7);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);


    EXPECT_EQ(N01.y(),   0);
    EXPECT_EQ(N02.y(),   6);
    EXPECT_EQ(N03.y(),   8);
    EXPECT_EQ(N04.y(),   1);
    EXPECT_EQ(N05.y(),  12);

    EXPECT_EQ(E01_02.x(),   0);
    EXPECT_EQ(E01_03.x(),   4);
    EXPECT_EQ(E01_04.x(),   6);
    EXPECT_EQ(E02_03.x(),   4);
    EXPECT_EQ(E03_04.x(),   0);
    EXPECT_EQ(E03_05.x(),   8);
    EXPECT_EQ(E04_05.x(),   0);
    EXPECT_EQ(E05_02.x(),   0);

}


/**  @brief Graph realistic zero supply/demand
 */
TEST_F(NetworkSimplexTests, Test16) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto ep01_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_02), N01, N02));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep03_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E03_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep03_04), N03, N04));

    auto ep03_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E03_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep03_05), N03, N05));

    auto ep04_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E04_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep04_05), N04, N05));

    auto ep05_02 = std::make_unique<NetworkSimplexEdge>();
    auto& E05_02 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep05_02), N05, N02));

    N01.setB(0);
    N02.setB(0);
    N03.setB(0);
    N04.setB(0);
    N05.setB(0);

    E01_02.setC(10);
    E01_03.setC( 8);
    E01_04.setC( 1);
    E02_03.setC( 2);
    E03_04.setC( 1);
    E03_05.setC( 4);
    E04_05.setC(12);
    E05_02.setC( 7);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);

    EXPECT_EQ(N01.y(),   0);
    EXPECT_EQ(N02.y(),  10);
    EXPECT_EQ(N03.y(),   8);
    EXPECT_EQ(N04.y(),   1);
    EXPECT_EQ(N05.y(),   3);

    EXPECT_EQ(E01_02.x(),   0);
    EXPECT_EQ(E01_03.x(),   0);
    EXPECT_EQ(E01_04.x(),   0);
    EXPECT_EQ(E02_03.x(),   0);
    EXPECT_EQ(E03_04.x(),   0);
    EXPECT_EQ(E03_05.x(),   0);
    EXPECT_EQ(E04_05.x(),   0);
    EXPECT_EQ(E05_02.x(),   0);

}


/**  @brief Graph realistic zero supply/demand
 */
TEST_F(NetworkSimplexTests, Test17) {

    DiGraph g01;
 
    auto np01 = std::make_unique<NetworkSimplexNode>(1);
    auto& N01 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np01)));

    auto np02 = std::make_unique<NetworkSimplexNode>(2);
    auto& N02 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np02)));

    auto np03 = std::make_unique<NetworkSimplexNode>(3);
    auto& N03 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np03)));

    auto np04 = std::make_unique<NetworkSimplexNode>(4);
    auto& N04 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np04)));

    auto np05 = std::make_unique<NetworkSimplexNode>(5);
    auto& N05 = dynamic_cast<NetworkSimplexNode&>(
                                                g01.addNode(std::move(np05)));

    auto ep01_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_03), N01, N03));

    auto ep01_04 = std::make_unique<NetworkSimplexEdge>();
    auto& E01_04 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep01_04), N01, N04));

    auto ep02_03 = std::make_unique<NetworkSimplexEdge>();
    auto& E02_03 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep02_03), N02, N03));

    auto ep03_05 = std::make_unique<NetworkSimplexEdge>();
    auto& E03_05 = dynamic_cast<NetworkSimplexEdge&>(
                                    g01.addEdge(std::move(ep03_05), N03, N05));

    N01.setB(10);
    N02.setB( 4);
    N03.setB(0);
    N04.setB(-6);
    N05.setB(-8);

    E01_03.setC( 8);
    E01_04.setC( 1);
    E02_03.setC( 2);
    E03_05.setC( 4);

    NetworkSimplex ns(g01);
    EXPECT_EQ(ns.checkValidity(), true);
    EXPECT_EQ(ns.solve(), true);


    EXPECT_EQ(N01.y(),   0);
    EXPECT_EQ(N02.y(),   6);
    EXPECT_EQ(N03.y(),   8);
    EXPECT_EQ(N04.y(),   1);
    EXPECT_EQ(N05.y(),  12);

    EXPECT_EQ(E01_03.x(),   4);
    EXPECT_EQ(E01_04.x(),   6);
    EXPECT_EQ(E02_03.x(),   4);
    EXPECT_EQ(E03_05.x(),   8);

}

} // namespace Directed

} // namespace Wailea
