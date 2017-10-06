#include "gtest/gtest.h"
#include "directed/critical_path.hpp"

namespace Wailea {

namespace Directed { 

class CriticalPathTests : public ::testing::Test {

  protected:  

    std::unique_ptr<DiGraph> generate_graph(){
        return std::make_unique<DiGraph>();
    }
 
    std::unique_ptr<CPDiNode> generate_node(long length) {
        return std::make_unique<CPDiNode>(length);
    }

    std::unique_ptr<CPDiEdge> generate_edge(long length) {
        return std::make_unique<CPDiEdge>(length);
    }

    CriticalPathTests(){;};
    virtual ~CriticalPathTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief realistic PERT 1.
 */
TEST_F(CriticalPathTests, Test1) {

    std::unique_ptr<DiGraph> gp1 = generate_graph();
    DiGraph&   g1 = *gp1;

    std::unique_ptr<CPDiNode> np0 = generate_node(0);
    CPDiNode& n0 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np0)));

    std::unique_ptr<CPDiNode> np1 = generate_node(6);
    CPDiNode& n1 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<CPDiNode> np2 = generate_node(4);
    CPDiNode& n2 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<CPDiNode> np3 = generate_node(2);
    CPDiNode& n3 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<CPDiNode> np4 = generate_node(6);
    CPDiNode& n4 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<CPDiNode> np5 = generate_node(3);
    CPDiNode& n5 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<CPDiNode> np6 = generate_node(4);
    CPDiNode& n6 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<CPDiNode> np7 = generate_node(3);
    CPDiNode& n7 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<CPDiNode> np8 = generate_node(2);
    CPDiNode& n8 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np8)));

    std::unique_ptr<CPDiNode> np9 = generate_node(4);
    CPDiNode& n9 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np9)));

    std::unique_ptr<CPDiNode> np10 = generate_node(4);
    CPDiNode& n10 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np10)));

    std::unique_ptr<CPDiNode> np11 = generate_node(1);
    CPDiNode& n11 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np11)));

    std::unique_ptr<CPDiNode> np12 = generate_node(1);
    CPDiNode& n12 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np12)));

    std::unique_ptr<CPDiNode> np13 = generate_node(4);
    CPDiNode& n13 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np13)));

    std::unique_ptr<CPDiNode> np14 = generate_node(0);
    CPDiNode& n14 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np14)));

    std::unique_ptr<CPDiEdge> ep0_1 = generate_edge(0);
    g1.addEdge(std::move(ep0_1), n0, n1);

    std::unique_ptr<CPDiEdge> ep1_2 = generate_edge(0);
    g1.addEdge(std::move(ep1_2), n1, n2);

    std::unique_ptr<CPDiEdge> ep2_3 = generate_edge(0);
    g1.addEdge(std::move(ep2_3), n2, n3);

    std::unique_ptr<CPDiEdge> ep3_7 = generate_edge(0);
    g1.addEdge(std::move(ep3_7), n3, n7);

    std::unique_ptr<CPDiEdge> ep1_4 = generate_edge(0);
    g1.addEdge(std::move(ep1_4), n1, n4);

    std::unique_ptr<CPDiEdge> ep4_7 = generate_edge(0);
    g1.addEdge(std::move(ep4_7), n4, n7);

    std::unique_ptr<CPDiEdge> ep1_5 = generate_edge(0);
    g1.addEdge(std::move(ep1_5), n1, n5);

    std::unique_ptr<CPDiEdge> ep5_6 = generate_edge(0);
    g1.addEdge(std::move(ep5_6), n5, n6);

    std::unique_ptr<CPDiEdge> ep6_7 = generate_edge(0);
    g1.addEdge(std::move(ep6_7), n6, n7);

    std::unique_ptr<CPDiEdge> ep7_8 = generate_edge(0);
    g1.addEdge(std::move(ep7_8), n7, n8);

    std::unique_ptr<CPDiEdge> ep8_9 = generate_edge(0);
    g1.addEdge(std::move(ep8_9), n8, n9);

    std::unique_ptr<CPDiEdge> ep9_14 = generate_edge(0);
    g1.addEdge(std::move(ep9_14), n9, n14);

    std::unique_ptr<CPDiEdge> ep7_10 = generate_edge(0);
    g1.addEdge(std::move(ep7_10), n7, n10);

    std::unique_ptr<CPDiEdge> ep10_11 = generate_edge(0);
    g1.addEdge(std::move(ep10_11), n10, n11);

    std::unique_ptr<CPDiEdge> ep11_14 = generate_edge(0);
    g1.addEdge(std::move(ep11_14), n11, n14);

    std::unique_ptr<CPDiEdge> ep6_12 = generate_edge(0);
    g1.addEdge(std::move(ep6_12), n6, n12);

    std::unique_ptr<CPDiEdge> ep12_13 = generate_edge(0);
    g1.addEdge(std::move(ep12_13), n12, n13);

    std::unique_ptr<CPDiEdge> ep13_14 = generate_edge(0);
    g1.addEdge(std::move(ep13_14), n13, n14);

    CriticalPath cp;
    cp.findCriticalPaths(g1);

    EXPECT_EQ(n0.earliestStart(),   0);
    EXPECT_EQ(n0.latestStart(),     0);
    EXPECT_EQ(n0.earliestFinish(),  0);
    EXPECT_EQ(n0.latestFinish(),    0);
    EXPECT_EQ(n1.earliestStart(),   0);
    EXPECT_EQ(n1.latestStart(),     0);
    EXPECT_EQ(n1.earliestFinish(),  6);
    EXPECT_EQ(n1.latestFinish(),    6);
    EXPECT_EQ(n2.earliestStart(),   6);
    EXPECT_EQ(n2.latestStart(),     7);
    EXPECT_EQ(n2.earliestFinish(), 10);
    EXPECT_EQ(n2.latestFinish(),   11);
    EXPECT_EQ(n3.earliestStart(),  10);
    EXPECT_EQ(n3.latestStart(),    11);
    EXPECT_EQ(n3.earliestFinish(), 12);
    EXPECT_EQ(n3.latestFinish(),   13);
    EXPECT_EQ(n4.earliestStart(),   6);
    EXPECT_EQ(n4.latestStart(),     7);
    EXPECT_EQ(n4.earliestFinish(), 12);
    EXPECT_EQ(n4.latestFinish(),   13);
    EXPECT_EQ(n5.earliestStart(),   6);
    EXPECT_EQ(n5.latestStart(),     6);
    EXPECT_EQ(n5.earliestFinish(),  9);
    EXPECT_EQ(n5.latestFinish(),    9);
    EXPECT_EQ(n6.earliestStart(),   9);
    EXPECT_EQ(n6.latestStart(),     9);
    EXPECT_EQ(n6.earliestFinish(), 13);
    EXPECT_EQ(n6.latestFinish(),   13);
    EXPECT_EQ(n7.earliestStart(),  13);
    EXPECT_EQ(n7.latestStart(),    13);
    EXPECT_EQ(n7.earliestFinish(), 16);
    EXPECT_EQ(n7.latestFinish(),   16);
    EXPECT_EQ(n8.earliestStart(),  16);
    EXPECT_EQ(n8.latestStart(),    16);
    EXPECT_EQ(n8.earliestFinish(), 18);
    EXPECT_EQ(n8.latestFinish(),   18);
    EXPECT_EQ(n9.earliestStart(),  18);
    EXPECT_EQ(n9.latestStart(),    18);
    EXPECT_EQ(n9.earliestFinish(), 22);
    EXPECT_EQ(n9.latestFinish(),   22);
    EXPECT_EQ(n10.earliestStart(), 16);
    EXPECT_EQ(n10.latestStart(),   17);
    EXPECT_EQ(n10.earliestFinish(),20);
    EXPECT_EQ(n10.latestFinish(),  21);
    EXPECT_EQ(n11.earliestStart(), 20);
    EXPECT_EQ(n11.latestStart(),   21);
    EXPECT_EQ(n11.earliestFinish(),21);
    EXPECT_EQ(n11.latestFinish(),  22);
    EXPECT_EQ(n12.earliestStart(), 13);
    EXPECT_EQ(n12.latestStart(),   17);
    EXPECT_EQ(n12.earliestFinish(),14);
    EXPECT_EQ(n12.latestFinish(),  18);
    EXPECT_EQ(n13.earliestStart(), 14);
    EXPECT_EQ(n13.latestStart(),   18);
    EXPECT_EQ(n13.earliestFinish(),18);
    EXPECT_EQ(n13.latestFinish(),  22);
    EXPECT_EQ(n14.earliestStart(), 22);
    EXPECT_EQ(n14.latestStart(),   22);
    EXPECT_EQ(n14.earliestFinish(),22);
    EXPECT_EQ(n14.latestFinish(),  22);

}



/**  @brief realistic PERT 2.
 */
TEST_F(CriticalPathTests, Test2) {

    std::unique_ptr<DiGraph> gp1 = generate_graph();
    DiGraph&   g1 = *gp1;

    std::unique_ptr<CPDiNode> np1 = generate_node(0);
    CPDiNode& n1 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<CPDiNode> np2 = generate_node(1);
    CPDiNode& n2 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<CPDiNode> np3 = generate_node(15);
    CPDiNode& n3 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<CPDiNode> np4 = generate_node(2);
    CPDiNode& n4 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<CPDiNode> np5 = generate_node(1);
    CPDiNode& n5 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<CPDiNode> np6 = generate_node(1);
    CPDiNode& n6 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<CPDiNode> np7 = generate_node(0);
    CPDiNode& n7 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<CPDiNode> np9 = generate_node(5);
    CPDiNode& n9 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np9)));

    std::unique_ptr<CPDiNode> np10 = generate_node(2);
    CPDiNode& n10 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np10)));

    std::unique_ptr<CPDiNode> np12 = generate_node(1);
    CPDiNode& n12 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np12)));

    std::unique_ptr<CPDiNode> np13 = generate_node(0);
    CPDiNode& n13 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np13)));

    std::unique_ptr<CPDiNode> np14 = generate_node(30);
    CPDiNode& n14 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np14)));

    std::unique_ptr<CPDiNode> np15 = generate_node(3);
    CPDiNode& n15 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np15)));

    std::unique_ptr<CPDiNode> np16 = generate_node(15);
    CPDiNode& n16 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np16)));

    std::unique_ptr<CPDiNode> np17 = generate_node(1);
    CPDiNode& n17 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np17)));

    std::unique_ptr<CPDiNode> np18 = generate_node(5);
    CPDiNode& n18 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np18)));

    std::unique_ptr<CPDiNode> np19 = generate_node(1);
    CPDiNode& n19 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np19)));

    std::unique_ptr<CPDiNode> np20 = generate_node(5);
    CPDiNode& n20 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np20)));

    std::unique_ptr<CPDiNode> np21 = generate_node(2);
    CPDiNode& n21 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np21)));

    std::unique_ptr<CPDiNode> np22 = generate_node(0);
    CPDiNode& n22 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np22)));

    std::unique_ptr<CPDiNode> np23 = generate_node(1);
    CPDiNode& n23 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np23)));

    std::unique_ptr<CPDiNode> np24 = generate_node(2);
    CPDiNode& n24 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np24)));

    std::unique_ptr<CPDiNode> np25 = generate_node(0);
    CPDiNode& n25 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np25)));

    std::unique_ptr<CPDiNode> np26 = generate_node(1);
    CPDiNode& n26 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np26)));

    std::unique_ptr<CPDiNode> np27 = generate_node(1);
    CPDiNode& n27 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np27)));

    std::unique_ptr<CPDiNode> np28 = generate_node(2);
    CPDiNode& n28 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np28)));

    std::unique_ptr<CPDiNode> np29 = generate_node(0);
    CPDiNode& n29 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np29)));

    std::unique_ptr<CPDiEdge> ep1_2 = generate_edge(0);
    g1.addEdge(std::move(ep1_2), n1, n2);

    std::unique_ptr<CPDiEdge> ep1_3 = generate_edge(0);
    g1.addEdge(std::move(ep1_3), n1, n3);

    std::unique_ptr<CPDiEdge> ep1_16 = generate_edge(0);
    g1.addEdge(std::move(ep1_16), n1, n16);

    std::unique_ptr<CPDiEdge> ep2_26 = generate_edge(0);
    g1.addEdge(std::move(ep2_26), n2, n26);

    std::unique_ptr<CPDiEdge> ep3_4 = generate_edge(0);
    g1.addEdge(std::move(ep3_4), n3, n4);

    std::unique_ptr<CPDiEdge> ep3_9 = generate_edge(0);
    g1.addEdge(std::move(ep3_9), n3, n9);

    std::unique_ptr<CPDiEdge> ep4_27 = generate_edge(0);
    g1.addEdge(std::move(ep4_27), n4, n27);

    std::unique_ptr<CPDiEdge> ep4_5 = generate_edge(0);
    g1.addEdge(std::move(ep4_5), n4, n5);

    std::unique_ptr<CPDiEdge> ep5_6 = generate_edge(0);
    g1.addEdge(std::move(ep5_6), n5, n6);

    std::unique_ptr<CPDiEdge> ep6_7 = generate_edge(0);
    g1.addEdge(std::move(ep6_7), n6, n7);

    std::unique_ptr<CPDiEdge> ep7_9 = generate_edge(0);
    g1.addEdge(std::move(ep7_9), n7, n9);

    std::unique_ptr<CPDiEdge> ep9_10 = generate_edge(0);
    g1.addEdge(std::move(ep9_10), n9, n10);

    std::unique_ptr<CPDiEdge> ep10_12 = generate_edge(0);
    g1.addEdge(std::move(ep10_12), n10, n12);

    std::unique_ptr<CPDiEdge> ep12_13 = generate_edge(0);
    g1.addEdge(std::move(ep12_13), n12, n13);

    std::unique_ptr<CPDiEdge> ep13_14 = generate_edge(0);
    g1.addEdge(std::move(ep13_14), n13, n14);

    std::unique_ptr<CPDiEdge> ep14_15 = generate_edge(0);
    g1.addEdge(std::move(ep14_15), n14, n15);

    std::unique_ptr<CPDiEdge> ep15_28 = generate_edge(0);
    g1.addEdge(std::move(ep15_28), n15, n28);

    std::unique_ptr<CPDiEdge> ep15_17 = generate_edge(0);
    g1.addEdge(std::move(ep15_17), n15, n17);

    std::unique_ptr<CPDiEdge> ep16_15 = generate_edge(0);
    g1.addEdge(std::move(ep16_15), n16, n15);

    std::unique_ptr<CPDiEdge> ep17_18 = generate_edge(0);
    g1.addEdge(std::move(ep17_18), n17, n18);

    std::unique_ptr<CPDiEdge> ep18_19 = generate_edge(0);
    g1.addEdge(std::move(ep18_19), n18, n19);

    std::unique_ptr<CPDiEdge> ep19_20 = generate_edge(0);
    g1.addEdge(std::move(ep19_20), n19, n20);

    std::unique_ptr<CPDiEdge> ep20_21 = generate_edge(0);
    g1.addEdge(std::move(ep20_21), n20, n21);

    std::unique_ptr<CPDiEdge> ep21_22 = generate_edge(0);
    g1.addEdge(std::move(ep21_22), n21, n22);

    std::unique_ptr<CPDiEdge> ep22_23 = generate_edge(0);
    g1.addEdge(std::move(ep22_23), n22, n23);

    std::unique_ptr<CPDiEdge> ep22_24 = generate_edge(0);
    g1.addEdge(std::move(ep22_24), n22, n24);

    std::unique_ptr<CPDiEdge> ep23_25 = generate_edge(0);
    g1.addEdge(std::move(ep23_25), n23, n25);

    std::unique_ptr<CPDiEdge> ep24_29 = generate_edge(0);
    g1.addEdge(std::move(ep24_29), n24, n29);

    std::unique_ptr<CPDiEdge> ep25_29 = generate_edge(0);
    g1.addEdge(std::move(ep25_29), n25, n29);

    std::unique_ptr<CPDiEdge> ep26_29 = generate_edge(0);
    g1.addEdge(std::move(ep26_29), n26, n29);

    std::unique_ptr<CPDiEdge> ep27_29 = generate_edge(0);
    g1.addEdge(std::move(ep27_29), n27, n29);

    std::unique_ptr<CPDiEdge> ep28_29 = generate_edge(0);
    g1.addEdge(std::move(ep28_29), n28, n29);


    CriticalPath cp;
    cp.findCriticalPaths(g1);

    EXPECT_EQ(n1.earliestStart(),   0);
    EXPECT_EQ(n1.latestStart(),     0);
    EXPECT_EQ(n1.earliestFinish(),  0);
    EXPECT_EQ(n1.latestFinish(),    0);
    EXPECT_EQ(n2.earliestStart(),   0);
    EXPECT_EQ(n2.latestStart(),     74);
    EXPECT_EQ(n2.earliestFinish(),  1);
    EXPECT_EQ(n2.latestFinish(),    75);
    EXPECT_EQ(n3.earliestStart(),   0);
    EXPECT_EQ(n3.latestStart(),     0);
    EXPECT_EQ(n3.earliestFinish(),  15);
    EXPECT_EQ(n3.latestFinish(),    15);
    EXPECT_EQ(n4.earliestStart(),   15);
    EXPECT_EQ(n4.latestStart(),     15);
    EXPECT_EQ(n4.earliestFinish(),  17);
    EXPECT_EQ(n4.latestFinish(),    17);
    EXPECT_EQ(n5.earliestStart(),   17);
    EXPECT_EQ(n5.latestStart(),     17);
    EXPECT_EQ(n5.earliestFinish(),  18);
    EXPECT_EQ(n5.latestFinish(),    18);
    EXPECT_EQ(n6.earliestStart(),   18);
    EXPECT_EQ(n6.latestStart(),     18);
    EXPECT_EQ(n6.earliestFinish(),  19);
    EXPECT_EQ(n6.latestFinish(),    19);
    EXPECT_EQ(n7.earliestStart(),   19);
    EXPECT_EQ(n7.latestStart(),     19);
    EXPECT_EQ(n7.earliestFinish(),  19);
    EXPECT_EQ(n7.latestFinish(),    19);
    EXPECT_EQ(n9.earliestStart(),   19);
    EXPECT_EQ(n9.latestStart(),     19);
    EXPECT_EQ(n9.earliestFinish(),  24);
    EXPECT_EQ(n9.latestFinish(),    24);
    EXPECT_EQ(n10.earliestStart(),  24);
    EXPECT_EQ(n10.latestStart(),    24);
    EXPECT_EQ(n10.earliestFinish(), 26);
    EXPECT_EQ(n10.latestFinish(),   26);
    EXPECT_EQ(n12.earliestStart(),  26);
    EXPECT_EQ(n12.latestStart(),    26);
    EXPECT_EQ(n12.earliestFinish(), 27);
    EXPECT_EQ(n12.latestFinish(),   27);
    EXPECT_EQ(n13.earliestStart(),  27);
    EXPECT_EQ(n13.latestStart(),    27);
    EXPECT_EQ(n13.earliestFinish(), 27);
    EXPECT_EQ(n13.latestFinish(),   27);
    EXPECT_EQ(n14.earliestStart(),  27);
    EXPECT_EQ(n14.latestStart(),    27);
    EXPECT_EQ(n14.earliestFinish(), 57);
    EXPECT_EQ(n14.latestFinish(),   57);
    EXPECT_EQ(n15.earliestStart(),  57);
    EXPECT_EQ(n15.latestStart(),    57);
    EXPECT_EQ(n15.earliestFinish(), 60);
    EXPECT_EQ(n15.latestFinish(),   60);
    EXPECT_EQ(n16.earliestStart(),  0);
    EXPECT_EQ(n16.latestStart(),    42);
    EXPECT_EQ(n16.earliestFinish(), 15);
    EXPECT_EQ(n16.latestFinish(),   57);
    EXPECT_EQ(n17.earliestStart(),  60);
    EXPECT_EQ(n17.latestStart(),    60);
    EXPECT_EQ(n17.earliestFinish(), 61);
    EXPECT_EQ(n17.latestFinish(),   61);
    EXPECT_EQ(n18.earliestStart(),  61);
    EXPECT_EQ(n18.latestStart(),    61);
    EXPECT_EQ(n18.earliestFinish(), 66);
    EXPECT_EQ(n18.latestFinish(),   66);
    EXPECT_EQ(n19.earliestStart(),  66);
    EXPECT_EQ(n19.latestStart(),    66);
    EXPECT_EQ(n19.earliestFinish(), 67);
    EXPECT_EQ(n19.latestFinish(),   67);
    EXPECT_EQ(n20.earliestStart(),  67);
    EXPECT_EQ(n20.latestStart(),    67);
    EXPECT_EQ(n20.earliestFinish(), 72);
    EXPECT_EQ(n20.latestFinish(),   72);
    EXPECT_EQ(n21.earliestStart(),  72);
    EXPECT_EQ(n21.latestStart(),    72);
    EXPECT_EQ(n21.earliestFinish(), 74);
    EXPECT_EQ(n21.latestFinish(),   74);
    EXPECT_EQ(n22.earliestStart(),  74);
    EXPECT_EQ(n22.latestStart(),    74);
    EXPECT_EQ(n22.earliestFinish(), 74);
    EXPECT_EQ(n22.latestFinish(),   74);
    EXPECT_EQ(n23.earliestStart(),  74);
    EXPECT_EQ(n23.latestStart(),    75);
    EXPECT_EQ(n23.earliestFinish(), 75);
    EXPECT_EQ(n23.latestFinish(),   76);
    EXPECT_EQ(n24.earliestStart(),  74);
    EXPECT_EQ(n24.latestStart(),    74);
    EXPECT_EQ(n24.earliestFinish(), 76);
    EXPECT_EQ(n24.latestFinish(),   76);
    EXPECT_EQ(n25.earliestStart(),  75);
    EXPECT_EQ(n25.latestStart(),    76);
    EXPECT_EQ(n25.earliestFinish(), 75);
    EXPECT_EQ(n25.latestFinish(),   76);
    EXPECT_EQ(n26.earliestStart(),  1);
    EXPECT_EQ(n26.latestStart(),    75);
    EXPECT_EQ(n26.earliestFinish(), 2);
    EXPECT_EQ(n26.latestFinish(),   76);
    EXPECT_EQ(n27.earliestStart(),  17);
    EXPECT_EQ(n27.latestStart(),    75);
    EXPECT_EQ(n27.earliestFinish(), 18);
    EXPECT_EQ(n27.latestFinish(),   76);
    EXPECT_EQ(n28.earliestStart(),  60);
    EXPECT_EQ(n28.latestStart(),    74);
    EXPECT_EQ(n28.earliestFinish(), 62);
    EXPECT_EQ(n28.latestFinish(),   76);
    EXPECT_EQ(n29.earliestStart(),  76);
    EXPECT_EQ(n29.latestStart(),    76);
    EXPECT_EQ(n29.earliestFinish(), 76);
    EXPECT_EQ(n29.latestFinish(),   76);

}


/**  @brief multi source and dest with edge delay
 */
TEST_F(CriticalPathTests, Test3) {

    std::unique_ptr<DiGraph> gp1 = generate_graph();
    DiGraph&   g1 = *gp1;

    std::unique_ptr<CPDiNode> np1 = generate_node(0);
    CPDiNode& n1 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<CPDiNode> np2 = generate_node(0);
    CPDiNode& n2 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<CPDiNode> np3 = generate_node(0);
    CPDiNode& n3 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<CPDiNode> np4 = generate_node(0);
    CPDiNode& n4 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<CPDiNode> np5 = generate_node(0);
    CPDiNode& n5 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<CPDiNode> np6 = generate_node(0);
    CPDiNode& n6 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<CPDiNode> np7 = generate_node(0);
    CPDiNode& n7 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<CPDiNode> np8 = generate_node(0);
    CPDiNode& n8 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np8)));

    std::unique_ptr<CPDiNode> np9 = generate_node(0);
    CPDiNode& n9 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np9)));

    std::unique_ptr<CPDiNode> np10 = generate_node(0);
    CPDiNode& n10 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np10)));

    std::unique_ptr<CPDiNode> np11 = generate_node(0);
    CPDiNode& n11 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np11)));

    std::unique_ptr<CPDiNode> np12 = generate_node(0);
    CPDiNode& n12 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np12)));

    std::unique_ptr<CPDiNode> np13 = generate_node(0);
    CPDiNode& n13 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np13)));

    std::unique_ptr<CPDiNode> np14 = generate_node(0);
    CPDiNode& n14 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np14)));

    std::unique_ptr<CPDiNode> np15 = generate_node(0);
    CPDiNode& n15 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np15)));

    std::unique_ptr<CPDiNode> np16 = generate_node(0);
    CPDiNode& n16 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np16)));

    std::unique_ptr<CPDiNode> np17 = generate_node(0);
    CPDiNode& n17 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np17)));

    std::unique_ptr<CPDiNode> np18 = generate_node(0);
    CPDiNode& n18 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np18)));

    std::unique_ptr<CPDiNode> np19 = generate_node(0);
    CPDiNode& n19 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np19)));

    std::unique_ptr<CPDiNode> np20 = generate_node(0);
    CPDiNode& n20 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np20)));

    std::unique_ptr<CPDiNode> np21 = generate_node(0);
    CPDiNode& n21 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np21)));

    std::unique_ptr<CPDiNode> np22 = generate_node(0);
    CPDiNode& n22 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np22)));

    std::unique_ptr<CPDiNode> np23 = generate_node(0);
    CPDiNode& n23 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np23)));

    std::unique_ptr<CPDiNode> np24 = generate_node(0);
    CPDiNode& n24 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np24)));

    std::unique_ptr<CPDiNode> np25 = generate_node(0);
    CPDiNode& n25 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np25)));

    std::unique_ptr<CPDiNode> np26 = generate_node(0);
    CPDiNode& n26 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np26)));

    std::unique_ptr<CPDiNode> np27 = generate_node(0);
    CPDiNode& n27 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np27)));

    std::unique_ptr<CPDiNode> np28 = generate_node(0);
    CPDiNode& n28 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np28)));

    std::unique_ptr<CPDiNode> np29 = generate_node(0);
    CPDiNode& n29 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np29)));

    std::unique_ptr<CPDiNode> np30 = generate_node(0);
    CPDiNode& n30 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np30)));

    std::unique_ptr<CPDiNode> np31 = generate_node(0);
    CPDiNode& n31 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np31)));

    std::unique_ptr<CPDiNode> np32 = generate_node(0);
    CPDiNode& n32 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np32)));

    std::unique_ptr<CPDiNode> np33 = generate_node(0);
    CPDiNode& n33 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np33)));

    std::unique_ptr<CPDiNode> np34 = generate_node(0);
    CPDiNode& n34 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np34)));

    std::unique_ptr<CPDiNode> np35 = generate_node(0);
    CPDiNode& n35 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np35)));

    std::unique_ptr<CPDiNode> np36 = generate_node(0);
    CPDiNode& n36 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np36)));

    std::unique_ptr<CPDiNode> np37 = generate_node(0);
    CPDiNode& n37 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np37)));

    std::unique_ptr<CPDiNode> np38 = generate_node(0);
    CPDiNode& n38 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np38)));

    std::unique_ptr<CPDiNode> np39 = generate_node(0);
    CPDiNode& n39 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np39)));

    std::unique_ptr<CPDiNode> np40 = generate_node(0);
    CPDiNode& n40 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np40)));

    std::unique_ptr<CPDiNode> np41 = generate_node(0);
    CPDiNode& n41 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np41)));

    std::unique_ptr<CPDiNode> np42 = generate_node(0);
    CPDiNode& n42 = dynamic_cast<CPDiNode&>(g1.addNode(std::move(np42)));

    std::unique_ptr<CPDiEdge> ep1_2 = generate_edge(1);
    g1.addEdge(std::move(ep1_2), n1, n2);

    std::unique_ptr<CPDiEdge> ep1_3 = generate_edge(1);
    g1.addEdge(std::move(ep1_3), n1, n3);

    std::unique_ptr<CPDiEdge> ep2_4 = generate_edge(1);
    g1.addEdge(std::move(ep2_4), n2, n4);

    std::unique_ptr<CPDiEdge> ep2_5 = generate_edge(1);
    g1.addEdge(std::move(ep2_5), n2, n5);

    std::unique_ptr<CPDiEdge> ep2_6 = generate_edge(1);
    g1.addEdge(std::move(ep2_6), n2, n6);

    std::unique_ptr<CPDiEdge> ep2_7 = generate_edge(1);
    g1.addEdge(std::move(ep2_7), n2, n7);

    std::unique_ptr<CPDiEdge> ep2_8 = generate_edge(1);
    g1.addEdge(std::move(ep2_8), n2, n8);

    std::unique_ptr<CPDiEdge> ep3_9 = generate_edge(1);
    g1.addEdge(std::move(ep3_9), n3, n9);

    std::unique_ptr<CPDiEdge> ep3_10 = generate_edge(1);
    g1.addEdge(std::move(ep3_10), n3, n10);

    std::unique_ptr<CPDiEdge> ep4_30 = generate_edge(1);
    g1.addEdge(std::move(ep4_30), n4, n30);

    std::unique_ptr<CPDiEdge> ep5_11 = generate_edge(1);
    g1.addEdge(std::move(ep5_11), n5, n11);

    std::unique_ptr<CPDiEdge> ep5_12 = generate_edge(1);
    g1.addEdge(std::move(ep5_12), n5, n12);

    std::unique_ptr<CPDiEdge> ep9_12 = generate_edge(1);
    g1.addEdge(std::move(ep9_12), n9, n12);

    std::unique_ptr<CPDiEdge> ep10_13 = generate_edge(1);
    g1.addEdge(std::move(ep10_13), n10, n13);

    std::unique_ptr<CPDiEdge> ep10_14 = generate_edge(1);
    g1.addEdge(std::move(ep10_14), n10, n14);

    std::unique_ptr<CPDiEdge> ep11_21 = generate_edge(1);
    g1.addEdge(std::move(ep11_21), n11, n21);

    std::unique_ptr<CPDiEdge> ep11_15 = generate_edge(1);
    g1.addEdge(std::move(ep11_15), n11, n15);

    std::unique_ptr<CPDiEdge> ep11_16 = generate_edge(1);
    g1.addEdge(std::move(ep11_16), n11, n16);

    std::unique_ptr<CPDiEdge> ep11_17 = generate_edge(1);
    g1.addEdge(std::move(ep11_17), n11, n17);

    std::unique_ptr<CPDiEdge> ep11_33 = generate_edge(1);
    g1.addEdge(std::move(ep11_33), n11, n33);

    std::unique_ptr<CPDiEdge> ep12_23 = generate_edge(1);
    g1.addEdge(std::move(ep12_23), n12, n23);

    std::unique_ptr<CPDiEdge> ep13_18 = generate_edge(1);
    g1.addEdge(std::move(ep13_18), n13, n18);

    std::unique_ptr<CPDiEdge> ep14_20 = generate_edge(1);
    g1.addEdge(std::move(ep14_20), n14, n20);

    std::unique_ptr<CPDiEdge> ep17_22 = generate_edge(1);
    g1.addEdge(std::move(ep17_22), n17, n22);

    std::unique_ptr<CPDiEdge> ep18_23 = generate_edge(1);
    g1.addEdge(std::move(ep18_23), n18, n23);

    std::unique_ptr<CPDiEdge> ep19_23 = generate_edge(1);
    g1.addEdge(std::move(ep19_23), n19, n23);

    std::unique_ptr<CPDiEdge> ep20_24 = generate_edge(1);
    g1.addEdge(std::move(ep20_24), n20, n24);

    std::unique_ptr<CPDiEdge> ep21_35 = generate_edge(1);
    g1.addEdge(std::move(ep21_35), n21, n35);

    std::unique_ptr<CPDiEdge> ep22_25 = generate_edge(1);
    g1.addEdge(std::move(ep22_25), n22, n25);

    std::unique_ptr<CPDiEdge> ep23_29 = generate_edge(1);
    g1.addEdge(std::move(ep23_29), n23, n29);

    std::unique_ptr<CPDiEdge> ep24_29 = generate_edge(1);
    g1.addEdge(std::move(ep24_29), n24, n29);

    std::unique_ptr<CPDiEdge> ep24_26 = generate_edge(1);
    g1.addEdge(std::move(ep24_26), n24, n26);

    std::unique_ptr<CPDiEdge> ep24_27 = generate_edge(1);
    g1.addEdge(std::move(ep24_27), n24, n27);

    std::unique_ptr<CPDiEdge> ep25_28 = generate_edge(1);
    g1.addEdge(std::move(ep25_28), n25, n28);

    std::unique_ptr<CPDiEdge> ep26_29 = generate_edge(1);
    g1.addEdge(std::move(ep26_29), n26, n29);

    std::unique_ptr<CPDiEdge> ep28_31 = generate_edge(1);
    g1.addEdge(std::move(ep28_31), n28, n31);

    std::unique_ptr<CPDiEdge> ep28_32 = generate_edge(1);
    g1.addEdge(std::move(ep28_32), n28, n32);

    std::unique_ptr<CPDiEdge> ep28_33 = generate_edge(1);
    g1.addEdge(std::move(ep28_33), n28, n33);

    std::unique_ptr<CPDiEdge> ep29_34 = generate_edge(1);
    g1.addEdge(std::move(ep29_34), n29, n34);

    std::unique_ptr<CPDiEdge> ep30_31 = generate_edge(1);
    g1.addEdge(std::move(ep30_31), n30, n31);

    std::unique_ptr<CPDiEdge> ep31_35 = generate_edge(1);
    g1.addEdge(std::move(ep31_35), n31, n35);

    std::unique_ptr<CPDiEdge> ep31_36 = generate_edge(1);
    g1.addEdge(std::move(ep31_36), n31, n36);

    std::unique_ptr<CPDiEdge> ep32_37 = generate_edge(1);
    g1.addEdge(std::move(ep32_37), n32, n37);

    std::unique_ptr<CPDiEdge> ep32_38 = generate_edge(1);
    g1.addEdge(std::move(ep32_38), n32, n38);

    std::unique_ptr<CPDiEdge> ep33_39 = generate_edge(1);
    g1.addEdge(std::move(ep33_39), n33, n39);

    std::unique_ptr<CPDiEdge> ep34_40 = generate_edge(1);
    g1.addEdge(std::move(ep34_40), n34, n40);

    std::unique_ptr<CPDiEdge> ep40_41 = generate_edge(1);
    g1.addEdge(std::move(ep40_41), n40, n41);

    std::unique_ptr<CPDiEdge> ep42_25 = generate_edge(1);
    g1.addEdge(std::move(ep42_25), n42, n25);

    std::unique_ptr<CPDiEdge> ep42_31 = generate_edge(1);
    g1.addEdge(std::move(ep42_31), n42, n31);


    CriticalPath cp;
    cp.findCriticalPaths(g1);

    EXPECT_EQ(n1.earliestStart(),   0);
    EXPECT_EQ(n1.latestStart(),     0);
    EXPECT_EQ(n1.earliestFinish(),  0);
    EXPECT_EQ(n1.latestFinish(),    0);

    EXPECT_EQ(n2.earliestStart(),   1);
    EXPECT_EQ(n2.latestStart(),     1);
    EXPECT_EQ(n2.earliestFinish(),  1);
    EXPECT_EQ(n2.latestFinish(),    1);

    EXPECT_EQ(n3.earliestStart(),   1);
    EXPECT_EQ(n3.latestStart(),     1);
    EXPECT_EQ(n3.earliestFinish(),  1);
    EXPECT_EQ(n3.latestFinish(),    1);

    EXPECT_EQ(n4.earliestStart(),   2);
    EXPECT_EQ(n4.latestStart(),     6);
    EXPECT_EQ(n4.earliestFinish(),  2);
    EXPECT_EQ(n4.latestFinish(),    6);

    EXPECT_EQ(n5.earliestStart(),   2);
    EXPECT_EQ(n5.latestStart(),     2);
    EXPECT_EQ(n5.earliestFinish(),  2);
    EXPECT_EQ(n5.latestFinish(),    2);

    EXPECT_EQ(n6.earliestStart(),   2);
    EXPECT_EQ(n6.latestStart(),     2);
    EXPECT_EQ(n6.earliestFinish(),  2);
    EXPECT_EQ(n6.latestFinish(),    2);

    EXPECT_EQ(n7.earliestStart(),   2);
    EXPECT_EQ(n7.latestStart(),     2);
    EXPECT_EQ(n7.earliestFinish(),  2);
    EXPECT_EQ(n7.latestFinish(),    2);

    EXPECT_EQ(n8.earliestStart(),   2);
    EXPECT_EQ(n8.latestStart(),     2);
    EXPECT_EQ(n8.earliestFinish(),  2);
    EXPECT_EQ(n8.latestFinish(),    2);

    EXPECT_EQ(n9.earliestStart(),   2);
    EXPECT_EQ(n9.latestStart(),     4);
    EXPECT_EQ(n9.earliestFinish(),  2);
    EXPECT_EQ(n9.latestFinish(),    4);

    EXPECT_EQ(n10.earliestStart(),   2);
    EXPECT_EQ(n10.latestStart(),     2);
    EXPECT_EQ(n10.earliestFinish(),  2);
    EXPECT_EQ(n10.latestFinish(),    2);

    EXPECT_EQ(n11.earliestStart(),   3);
    EXPECT_EQ(n11.latestStart(),     3);
    EXPECT_EQ(n11.earliestFinish(),  3);
    EXPECT_EQ(n11.latestFinish(),    3);

    EXPECT_EQ(n12.earliestStart(),   3);
    EXPECT_EQ(n12.latestStart(),     5);
    EXPECT_EQ(n12.earliestFinish(),  3);
    EXPECT_EQ(n12.latestFinish(),    5);

    EXPECT_EQ(n13.earliestStart(),   3);
    EXPECT_EQ(n13.latestStart(),     4);
    EXPECT_EQ(n13.earliestFinish(),  3);
    EXPECT_EQ(n13.latestFinish(),    4);

    EXPECT_EQ(n14.earliestStart(),   3);
    EXPECT_EQ(n14.latestStart(),     3);
    EXPECT_EQ(n14.earliestFinish(),  3);
    EXPECT_EQ(n14.latestFinish(),    3);

    EXPECT_EQ(n15.earliestStart(),   4);
    EXPECT_EQ(n15.latestStart(),     4);
    EXPECT_EQ(n15.earliestFinish(),  4);
    EXPECT_EQ(n15.latestFinish(),    4);

    EXPECT_EQ(n16.earliestStart(),   4);
    EXPECT_EQ(n16.latestStart(),     4);
    EXPECT_EQ(n16.earliestFinish(),  4);
    EXPECT_EQ(n16.latestFinish(),    4);

    EXPECT_EQ(n17.earliestStart(),   4);
    EXPECT_EQ(n17.latestStart(),     4);
    EXPECT_EQ(n17.earliestFinish(),  4);
    EXPECT_EQ(n17.latestFinish(),    4);

    EXPECT_EQ(n18.earliestStart(),   4);
    EXPECT_EQ(n18.latestStart(),     5);
    EXPECT_EQ(n18.earliestFinish(),  4);
    EXPECT_EQ(n18.latestFinish(),    5);

    EXPECT_EQ(n19.earliestStart(),   5);
    EXPECT_EQ(n19.latestStart(),     5);
    EXPECT_EQ(n19.earliestFinish(),  5);
    EXPECT_EQ(n19.latestFinish(),    5);

    EXPECT_EQ(n20.earliestStart(),   4);
    EXPECT_EQ(n20.latestStart(),     4);
    EXPECT_EQ(n20.earliestFinish(),  4);
    EXPECT_EQ(n20.latestFinish(),    4);

    EXPECT_EQ(n21.earliestStart(),   4);
    EXPECT_EQ(n21.latestStart(),     8);
    EXPECT_EQ(n21.earliestFinish(),  4);
    EXPECT_EQ(n21.latestFinish(),    8);

    EXPECT_EQ(n22.earliestStart(),   5);
    EXPECT_EQ(n22.latestStart(),     5);
    EXPECT_EQ(n22.earliestFinish(),  5);
    EXPECT_EQ(n22.latestFinish(),    5);

    EXPECT_EQ(n23.earliestStart(),   6);
    EXPECT_EQ(n23.latestStart(),     6);
    EXPECT_EQ(n23.earliestFinish(),  6);
    EXPECT_EQ(n23.latestFinish(),    6);

    EXPECT_EQ(n24.earliestStart(),   5);
    EXPECT_EQ(n24.latestStart(),     5);
    EXPECT_EQ(n24.earliestFinish(),  5);
    EXPECT_EQ(n24.latestFinish(),    5);

    EXPECT_EQ(n25.earliestStart(),   6);
    EXPECT_EQ(n25.latestStart(),     6);
    EXPECT_EQ(n25.earliestFinish(),  6);
    EXPECT_EQ(n25.latestFinish(),    6);

    EXPECT_EQ(n26.earliestStart(),   6);
    EXPECT_EQ(n26.latestStart(),     6);
    EXPECT_EQ(n26.earliestFinish(),  6);
    EXPECT_EQ(n26.latestFinish(),    6);

    EXPECT_EQ(n27.earliestStart(),   6);
    EXPECT_EQ(n27.latestStart(),     6);
    EXPECT_EQ(n27.earliestFinish(),  6);
    EXPECT_EQ(n27.latestFinish(),    6);

    EXPECT_EQ(n28.earliestStart(),   7);
    EXPECT_EQ(n28.latestStart(),     7);
    EXPECT_EQ(n28.earliestFinish(),  7);
    EXPECT_EQ(n28.latestFinish(),    7);

    EXPECT_EQ(n29.earliestStart(),   7);
    EXPECT_EQ(n29.latestStart(),     7);
    EXPECT_EQ(n29.earliestFinish(),  7);
    EXPECT_EQ(n29.latestFinish(),    7);

    EXPECT_EQ(n30.earliestStart(),   3);
    EXPECT_EQ(n30.latestStart(),     7);
    EXPECT_EQ(n30.earliestFinish(),  3);
    EXPECT_EQ(n30.latestFinish(),    7);

    EXPECT_EQ(n31.earliestStart(),   8);
    EXPECT_EQ(n31.latestStart(),     8);
    EXPECT_EQ(n31.earliestFinish(),  8);
    EXPECT_EQ(n31.latestFinish(),    8);

    EXPECT_EQ(n32.earliestStart(),   8);
    EXPECT_EQ(n32.latestStart(),     8);
    EXPECT_EQ(n32.earliestFinish(),  8);
    EXPECT_EQ(n32.latestFinish(),    8);

    EXPECT_EQ(n33.earliestStart(),   8);
    EXPECT_EQ(n33.latestStart(),     8);
    EXPECT_EQ(n33.earliestFinish(),  8);
    EXPECT_EQ(n33.latestFinish(),    8);

    EXPECT_EQ(n34.earliestStart(),   8);
    EXPECT_EQ(n34.latestStart(),     8);
    EXPECT_EQ(n34.earliestFinish(),  8);
    EXPECT_EQ(n34.latestFinish(),    8);

    EXPECT_EQ(n35.earliestStart(),   9);
    EXPECT_EQ(n35.latestStart(),     9);
    EXPECT_EQ(n35.earliestFinish(),  9);
    EXPECT_EQ(n35.latestFinish(),    9);

    EXPECT_EQ(n36.earliestStart(),   9);
    EXPECT_EQ(n36.latestStart(),     9);
    EXPECT_EQ(n36.earliestFinish(),  9);
    EXPECT_EQ(n36.latestFinish(),    9);

    EXPECT_EQ(n37.earliestStart(),   9);
    EXPECT_EQ(n37.latestStart(),     9);
    EXPECT_EQ(n37.earliestFinish(),  9);
    EXPECT_EQ(n37.latestFinish(),    9);

    EXPECT_EQ(n38.earliestStart(),   9);
    EXPECT_EQ(n38.latestStart(),     9);
    EXPECT_EQ(n38.earliestFinish(),  9);
    EXPECT_EQ(n38.latestFinish(),    9);

    EXPECT_EQ(n39.earliestStart(),   9);
    EXPECT_EQ(n39.latestStart(),     9);
    EXPECT_EQ(n39.earliestFinish(),  9);
    EXPECT_EQ(n39.latestFinish(),    9);

    EXPECT_EQ(n40.earliestStart(),   9);
    EXPECT_EQ(n40.latestStart(),     9);
    EXPECT_EQ(n40.earliestFinish(),  9);
    EXPECT_EQ(n40.latestFinish(),    9);

    EXPECT_EQ(n41.earliestStart(),   10);
    EXPECT_EQ(n41.latestStart(),     10);
    EXPECT_EQ(n41.earliestFinish(),  10);
    EXPECT_EQ(n41.latestFinish(),    10);

    EXPECT_EQ(n42.earliestStart(),   5);
    EXPECT_EQ(n42.latestStart(),     5);
    EXPECT_EQ(n42.earliestFinish(),  5);
    EXPECT_EQ(n42.latestFinish(),    5);

}

} // namespace Directed

} // namespace Wailea
