#include "gtest/gtest.h"
#include "undirected/planarizable_graph.hpp"

namespace Wailea {

namespace Undirected {

class PlanarizableGraphTests : public ::testing::Test {

  protected:

    PlanarizableGraphTests(){;};
    virtual ~PlanarizableGraphTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief tests k0
 */
TEST_F(PlanarizableGraphTests, Test1) {

    Graph g_1;


    vector<node_list_it_t> Nlist;
    vector<edge_list_it_t> Elist;

    PlanarizableGraph pg_1;
    pg_1.createFrom(g_1, Nlist, Elist);
    EXPECT_EQ(pg_1.numNodes(), 0);
    EXPECT_EQ(pg_1.numEdges(), 0);

}


/**  @brief tests k1
 */
TEST_F(PlanarizableGraphTests, Test2) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));

    vector<node_list_it_t> Nlist;
    vector<edge_list_it_t> Elist;
    Nlist.push_back(n_01.backIt());

    PlanarizableGraph pg_1;
    pg_1.createFrom(g_1, Nlist, Elist);
    EXPECT_EQ(pg_1.numNodes(), 1);
    EXPECT_EQ(pg_1.numEdges(), 0);

    auto pnit = pg_1.nodes().first;
    auto& pn_01 = dynamic_cast<PlanarizableNode&>(*(*pnit));

    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
}


/**  @brief tests k2
 */
TEST_F(PlanarizableGraphTests, Test3) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);

    vector<node_list_it_t> Nlist;
    vector<edge_list_it_t> Elist;
    Nlist.push_back(n_01.backIt());
    Nlist.push_back(n_02.backIt());
    Elist.push_back(e_01_02.backIt());

    PlanarizableGraph pg_1;
    pg_1.createFrom(g_1, Nlist, Elist);
    EXPECT_EQ(pg_1.numNodes(), 2);
    EXPECT_EQ(pg_1.numEdges(), 1);

    auto pnit = pg_1.nodes().first;
    auto& pn_01 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_02 = dynamic_cast<PlanarizableNode&>(*(*pnit));

    auto peit = pg_1.edges().first;
    auto& pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*peit));

    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(e_01_02.IGForwardLink(),    pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.IGBackwardLink(),  e_01_02.backIt());

}


/**  @brief tests k3
 */
TEST_F(PlanarizableGraphTests, Test4) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);

    vector<node_list_it_t> Nlist;
    vector<edge_list_it_t> Elist;
    Nlist.push_back(n_01.backIt());
    Nlist.push_back(n_02.backIt());
    Nlist.push_back(n_03.backIt());
    Elist.push_back(e_01_02.backIt());
    Elist.push_back(e_01_03.backIt());
    Elist.push_back(e_02_03.backIt());

    PlanarizableGraph pg_1;
    pg_1.createFrom(g_1, Nlist, Elist);

    EXPECT_EQ(pg_1.numNodes(), 3);
    EXPECT_EQ(pg_1.numEdges(), 3);
    auto pnit = pg_1.nodes().first;
    auto& pn_01 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_02 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_03 = dynamic_cast<PlanarizableNode&>(*(*pnit));

    auto peit = pg_1.edges().first;
    auto& pe_01_02 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    peit++;
    auto& pe_01_03 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    peit++;
    auto& pe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*peit));

    EXPECT_EQ(n_01.IGForwardLink(),   pn_01.backIt());
    EXPECT_EQ(pn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(n_02.IGForwardLink(),   pn_02.backIt());
    EXPECT_EQ(pn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(n_03.IGForwardLink(),   pn_03.backIt());
    EXPECT_EQ(pn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(pn_03.isVirtual(), false);

    EXPECT_EQ(e_01_02.IGForwardLink(),    pe_01_02.backIt());
    EXPECT_EQ(pe_01_02.IGBackwardLink(),  e_01_02.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(),    pe_01_03.backIt());
    EXPECT_EQ(pe_01_03.IGBackwardLink(),  e_01_03.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(),    pe_02_03.backIt());
    EXPECT_EQ(pe_02_03.IGBackwardLink(),  e_02_03.backIt());

}


/**  @brief tests insertEdge()
 */
TEST_F(PlanarizableGraphTests, Test5) {

//        __________   __________
//       |      ____\1/____      |
//       |     |     |     |     |
//       |     |     |     |     |
//       3-----4-----5-----6-----7
//       |     |     |     |     |
//       |     |     |     |     |
//       |     |_____2_____|     |
//       |__________/ \__________|
//
// Insert {3,7} through {2,4}, {2,5}, and {2,6}
//             
//                 _| |_
//                 \   /
//                  \_/ 
//        __________   __________
//       |      ____\1/____      |
//       |     |     |     |     |
//       |     |     |     |     |
//       3-----4-----5-----6-----7
//       |\    |     |     |    /|
//       | \   |     |     |   / |
//       |  \  |     |     |  /  |
//       |   \ |     |     | /   |
//       |    -v1----v2----v3    |
//       |     |     |     |     |
//       |     |_____2_____|     |
//       |__________/ \__________|

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));

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
    auto& e_03_07 = g_1.addEdge(make_unique<Edge>(), n_03, n_07);

    vector<node_list_it_t> Nlist;
    vector<edge_list_it_t> Elist;
    Nlist.push_back(n_01.backIt());
    Nlist.push_back(n_02.backIt());
    Nlist.push_back(n_03.backIt());
    Nlist.push_back(n_04.backIt());
    Nlist.push_back(n_05.backIt());
    Nlist.push_back(n_06.backIt());
    Nlist.push_back(n_07.backIt());

    Elist.push_back(e_01_03.backIt());
    Elist.push_back(e_01_04.backIt());
    Elist.push_back(e_01_05.backIt());
    Elist.push_back(e_01_06.backIt());
    Elist.push_back(e_01_07.backIt());
    Elist.push_back(e_02_03.backIt());
    Elist.push_back(e_02_04.backIt());
    Elist.push_back(e_02_05.backIt());
    Elist.push_back(e_02_06.backIt());
    Elist.push_back(e_02_07.backIt());

    PlanarizableGraph pg_1;
    pg_1.createFrom(g_1, Nlist, Elist);
    e_03_07.pushIGForwardLink(pg_1.edges().second);

    EXPECT_EQ(pg_1.numNodes(),  7);
    EXPECT_EQ(pg_1.numEdges(), 10);
    auto pnit = pg_1.nodes().first;
    auto& pn_01 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_02 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_03 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_04 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_05 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_06 = dynamic_cast<PlanarizableNode&>(*(*pnit));
    pnit++;
    auto& pn_07 = dynamic_cast<PlanarizableNode&>(*(*pnit));

    auto peit = pg_1.edges().first;
    /*auto& pe_01_03 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/
    peit++;
    /*auto& pe_01_04 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/
    peit++;
    /*auto& pe_01_05 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/
    peit++;
    /*auto& pe_01_06 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/
    peit++;
    /*auto& pe_01_07 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/
    peit++;
    /*auto& pe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/
    peit++;
    auto& pe_02_04 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    peit++;
    auto& pe_02_05 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    peit++;
    auto& pe_02_06 = dynamic_cast<PlanarizableEdge&>(*(*peit));
    peit++;
    /*auto& pe_02_07 = dynamic_cast<PlanarizableEdge&>(*(*peit));*/

    list<edge_list_it_t> insertionPath;
    insertionPath.push_back(pe_02_04.backIt());
    insertionPath.push_back(pe_02_05.backIt());
    insertionPath.push_back(pe_02_06.backIt());

    vector<edge_ptr_t> removedEdges = pg_1.insertEdge(
        insertionPath,
        pn_03,
        pn_07,
        e_03_07.backIt()
    );

    EXPECT_EQ(pg_1.numNodes(), 10);
    EXPECT_EQ(pg_1.numEdges(), 17);



    EXPECT_EQ(removedEdges.size(), 3);
    auto rmit = removedEdges.begin();
    EXPECT_EQ(&(*(*rmit)), &pe_02_04);
    rmit++;
    EXPECT_EQ(&(*(*rmit)), &pe_02_05);
    rmit++;
    EXPECT_EQ(&(*(*rmit)), &pe_02_06);

    EXPECT_EQ(insertionPath.size(), 4);
    auto ipit = insertionPath.begin();
    auto& ie_01 = dynamic_cast<PlanarizableEdge&>(*(*(*ipit)));
    EXPECT_EQ(ie_01.incidentNode1().backIt(), pn_03.backIt());
    EXPECT_EQ(ie_01.isTerminalN1(), true);
    EXPECT_EQ(ie_01.isTerminalN2(), false);


    ipit++;
    auto& ie_02 = dynamic_cast<PlanarizableEdge&>(*(*(*ipit)));
    EXPECT_EQ(ie_02.isTerminalN1(), false);
    EXPECT_EQ(ie_02.isTerminalN2(), false);

    ipit++;
    auto& ie_03 = dynamic_cast<PlanarizableEdge&>(*(*(*ipit)));
    EXPECT_EQ(ie_03.isTerminalN1(), false);
    EXPECT_EQ(ie_03.isTerminalN2(), false);

    ipit++;
    auto& ie_04 = dynamic_cast<PlanarizableEdge&>(*(*(*ipit)));
    EXPECT_EQ(ie_04.incidentNode2().backIt(), pn_07.backIt());
    EXPECT_EQ(ie_04.isTerminalN1(), false);
    EXPECT_EQ(ie_04.isTerminalN2(), true);


    EXPECT_EQ(ie_01.chainN2(),ie_02.backIt()); 
    EXPECT_EQ(ie_02.chainN1(),ie_01.backIt()); 
    EXPECT_EQ(ie_02.chainN2(),ie_03.backIt()); 
    EXPECT_EQ(ie_03.chainN1(),ie_02.backIt()); 
    EXPECT_EQ(ie_03.chainN2(),ie_04.backIt()); 
    EXPECT_EQ(ie_04.chainN1(),ie_03.backIt()); 

    auto& v_01 = dynamic_cast<PlanarizableNode&>(ie_01.incidentNode2());
    auto& v_02 = dynamic_cast<PlanarizableNode&>(ie_02.incidentNode2());
    auto& v_03 = dynamic_cast<PlanarizableNode&>(ie_03.incidentNode2());
    EXPECT_EQ(ie_02.incidentNode1().backIt(), v_01.backIt());
    EXPECT_EQ(ie_03.incidentNode1().backIt(), v_02.backIt());
    EXPECT_EQ(ie_04.incidentNode1().backIt(), v_03.backIt());

    EXPECT_EQ(v_01.degree(), 4);
    EXPECT_EQ(v_02.degree(), 4);
    EXPECT_EQ(v_03.degree(), 4);

    auto iit = v_01.incidentEdges().first;
    auto& v_01_e_01 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_01_a_01 = dynamic_cast<PlanarizableNode&>(v_01_e_01.adjacentNode(v_01));
    EXPECT_EQ(v_01_a_01.backIt(), pn_02.backIt());
    EXPECT_EQ(v_01_e_01.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(e_02_04.IGForwardLink(), v_01_e_01.backIt());
    EXPECT_EQ(v_01_e_01.isTerminalN1(), true);
    EXPECT_EQ(v_01_e_01.isTerminalN2(), false);

    iit++;
    auto& v_01_e_02 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_01_a_02 = dynamic_cast<PlanarizableNode&>(v_01_e_02.adjacentNode(v_01));
    EXPECT_EQ(v_01_e_02.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(v_01_a_02.backIt(), pn_04.backIt());
    EXPECT_EQ(v_01_e_02.isTerminalN1(), false);
    EXPECT_EQ(v_01_e_02.isTerminalN2(), true);

    EXPECT_EQ(v_01_e_01.chainN2(), v_01_e_02.backIt());
    EXPECT_EQ(v_01_e_02.chainN1(), v_01_e_01.backIt());

    iit++;
    auto& v_01_e_03 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_01_a_03 = dynamic_cast<PlanarizableNode&>(v_01_e_03.adjacentNode(v_01));
    EXPECT_EQ(v_01_a_03.backIt(), pn_03.backIt());
    EXPECT_EQ(v_01_e_03.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(e_03_07.IGForwardLink(), v_01_e_03.backIt());
    EXPECT_EQ(v_01_e_03.isTerminalN1(), true);
    EXPECT_EQ(v_01_e_03.isTerminalN2(), false);

    iit++;
    auto& v_01_e_04 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_01_a_04 = dynamic_cast<PlanarizableNode&>(v_01_e_04.adjacentNode(v_01));
    EXPECT_EQ(v_01_a_04.backIt(), v_02.backIt());
    EXPECT_EQ(v_01_e_04.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(v_01_e_04.isTerminalN1(), false);
    EXPECT_EQ(v_01_e_04.isTerminalN2(), false);


    iit = v_02.incidentEdges().first;
    auto& v_02_e_01 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_02_a_01 = dynamic_cast<PlanarizableNode&>(v_02_e_01.adjacentNode(v_02));
    EXPECT_EQ(v_02_a_01.backIt(), pn_02.backIt());
    EXPECT_EQ(v_02_e_01.IGBackwardLink(), e_02_05.backIt());
    EXPECT_EQ(e_02_05.IGForwardLink(), v_02_e_01.backIt());
    EXPECT_EQ(v_02_e_01.isTerminalN1(), true);
    EXPECT_EQ(v_02_e_01.isTerminalN2(), false);

    iit++;
    auto& v_02_e_02 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_02_a_02 = dynamic_cast<PlanarizableNode&>(v_02_e_02.adjacentNode(v_02));
    EXPECT_EQ(v_02_a_02.backIt(), pn_05.backIt());
    EXPECT_EQ(v_02_e_02.IGBackwardLink(), e_02_05.backIt());
    EXPECT_EQ(v_02_e_02.isTerminalN1(), false);
    EXPECT_EQ(v_02_e_02.isTerminalN2(), true);

    EXPECT_EQ(v_02_e_01.chainN2(), v_02_e_02.backIt());
    EXPECT_EQ(v_02_e_02.chainN1(), v_02_e_01.backIt());

    iit++;
    auto& v_02_e_03 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_02_a_03 = dynamic_cast<PlanarizableNode&>(v_02_e_03.adjacentNode(v_02));
    EXPECT_EQ(v_02_a_03.backIt(), v_01.backIt());
    EXPECT_EQ(v_02_e_03.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(v_02_e_03.isTerminalN1(), false);
    EXPECT_EQ(v_02_e_03.isTerminalN2(), false);

    iit++;
    auto& v_02_e_04 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_02_a_04 = dynamic_cast<PlanarizableNode&>(v_02_e_04.adjacentNode(v_02));
    EXPECT_EQ(v_02_a_04.backIt(), v_03.backIt());
    EXPECT_EQ(v_02_e_04.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(v_02_e_04.isTerminalN1(), false);
    EXPECT_EQ(v_02_e_04.isTerminalN2(), false);

    iit = v_03.incidentEdges().first;
    auto& v_03_e_01 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_03_a_01 = dynamic_cast<PlanarizableNode&>(v_03_e_01.adjacentNode(v_03));
    EXPECT_EQ(v_03_a_01.backIt(), pn_02.backIt());
    EXPECT_EQ(v_03_e_01.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(e_02_06.IGForwardLink(), v_03_e_01.backIt());
    EXPECT_EQ(v_03_e_01.isTerminalN1(), true);
    EXPECT_EQ(v_03_e_01.isTerminalN2(), false);

    iit++;
    auto& v_03_e_02 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_03_a_02 = dynamic_cast<PlanarizableNode&>(v_03_e_02.adjacentNode(v_03));
    EXPECT_EQ(v_03_a_02.backIt(), pn_06.backIt());
    EXPECT_EQ(v_03_e_02.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(v_03_e_02.isTerminalN1(), false);
    EXPECT_EQ(v_03_e_02.isTerminalN2(), true);

    EXPECT_EQ(v_03_e_01.chainN2(), v_03_e_02.backIt());
    EXPECT_EQ(v_03_e_02.chainN1(), v_03_e_01.backIt());

    iit++;
    auto& v_03_e_03 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_03_a_03 = dynamic_cast<PlanarizableNode&>(v_03_e_03.adjacentNode(v_03));
    EXPECT_EQ(v_03_a_03.backIt(), v_02.backIt());
    EXPECT_EQ(v_03_e_03.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(v_03_e_03.isTerminalN1(), false);
    EXPECT_EQ(v_03_e_03.isTerminalN2(), false);

    iit++;
    auto& v_03_e_04 = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
    auto& v_03_a_04 = dynamic_cast<PlanarizableNode&>(v_03_e_04.adjacentNode(v_03));
    EXPECT_EQ(v_03_a_04.backIt(), pn_07.backIt());
    EXPECT_EQ(v_03_e_04.IGBackwardLink(), e_03_07.backIt());
    EXPECT_EQ(v_03_e_04.isTerminalN1(), false);
    EXPECT_EQ(v_03_e_04.isTerminalN2(), true);

    EXPECT_EQ(v_01_e_03.chainN2(), v_01_e_04.backIt());
    EXPECT_EQ(v_01_e_04.chainN1(), v_01_e_03.backIt());
    EXPECT_EQ(v_01_e_04.backIt(),  v_02_e_03.backIt());
    EXPECT_EQ(v_02_e_03.chainN2(), v_02_e_04.backIt());
    EXPECT_EQ(v_02_e_04.chainN1(), v_02_e_03.backIt());
    EXPECT_EQ(v_02_e_04.backIt(), v_03_e_03.backIt());
    EXPECT_EQ(v_03_e_03.chainN2(), v_03_e_04.backIt());
    EXPECT_EQ(v_03_e_04.chainN1(), v_03_e_03.backIt());

    EXPECT_EQ(pn_01.isVirtual(), false);
    EXPECT_EQ(pn_02.isVirtual(), false);
    EXPECT_EQ(pn_03.isVirtual(), false);
    EXPECT_EQ(pn_04.isVirtual(), false);
    EXPECT_EQ(pn_05.isVirtual(), false);
    EXPECT_EQ(pn_06.isVirtual(), false);
    EXPECT_EQ(pn_07.isVirtual(), false);
    EXPECT_EQ(v_01.isVirtual(),  true);
    EXPECT_EQ(v_02.isVirtual(),  true);
    EXPECT_EQ(v_03.isVirtual(),  true);

    EXPECT_EQ(v_01.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(v_02.IGBackwardLink(), pg_1.nodes().second);
    EXPECT_EQ(v_03.IGBackwardLink(), pg_1.nodes().second);

}


/**  @brief tests removeUnnecessaryVirtualNodes()
 */
TEST_F(PlanarizableGraphTests, Test6) {

//  Original Graph       Planarized Graph             Expected
//    3-------2       3-----------------2          3-------------------2
//    |       |        \               /           |                   |
//    |       |         \   V11   V10 /            |                   |
//    |       |          \ /       \ /             |                   |
//    4-------1          V12        V9        V13-V12-V11          V10-V9-V8
//     \     /           / \       / \             |                   |
//      \   /          V13  \     /   V8           |                   |
//       \ /                 \   /                 |                   |
//        5                   \ /                  |                   |
//                             V17            =>   |                   |
//                            / \                  |                   |
//                           /   \                 |                   |
//                     V14  /     \   V7           |                   |
//                       \ /       \ /             |                   |
//                       V15        V6        V14-V15-V16           V5-V6-V7
//                       / \       / \             |                   |
//                      /   V16  V5   \            |                   |
//                     /               \           |                   |
//                    /                 \          |                   |
//                   4-------------------1         4-------------------1
//
// 1-2 -> 1-6-17-9-2
// 2-3 -> 2-3
// 3-4 -> 3-12-17-15-4
// 4-1 -> 4 1
// X-X -> 5-6-7
// X-X -> 8-9-10
// X-X -> 11-12-13
// X-X -> 14-15-16

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(), n_02, n_03);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(), n_03, n_04);
    auto& e_04_01 = g_1.addEdge(make_unique<Edge>(), n_04, n_01);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(), n_04, n_05);
    auto& e_01_05 = g_1.addEdge(make_unique<Edge>(), n_01, n_05);

    PlanarizableGraph pg_1;

    auto& pn_01 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_02 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_03 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_04 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_05 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_06 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_07 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_08 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_09 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_10 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_11 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_12 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_13 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_14 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_15 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_16 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));
    auto& pn_17 = dynamic_cast<PlanarizableNode&>(
                               pg_1.addNode(make_unique<PlanarizableNode>()));

    auto& pe_01_06 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_01,pn_06));
    auto& pe_06_17 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_06,pn_17));
    auto& pe_17_09 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_17,pn_09));
    auto& pe_09_02 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_09,pn_02));
    auto& pe_02_03 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_02,pn_03));
    auto& pe_03_12 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_03,pn_12));
    auto& pe_12_17 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_12,pn_17));
    auto& pe_17_15 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_17,pn_15));
    auto& pe_15_04 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_15,pn_04));
    auto& pe_04_01 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_04,pn_01));
    auto& pe_05_06 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_05,pn_06));
    auto& pe_06_07 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_06,pn_07));
    auto& pe_08_09 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_08,pn_09));
    auto& pe_09_10 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_09,pn_10));
    auto& pe_11_12 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_11,pn_12));
    auto& pe_12_13 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_12,pn_13));
    auto& pe_14_15 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_14,pn_15));
    auto& pe_15_16 = dynamic_cast<PlanarizableEdge&>(
                   pg_1.addEdge(make_unique<PlanarizableEdge>(), pn_15,pn_16));

    vector<edge_list_it_t> inc_pn_06;
    inc_pn_06.push_back(pe_01_06.backIt());
    inc_pn_06.push_back(pe_06_07.backIt());
    inc_pn_06.push_back(pe_06_17.backIt());
    inc_pn_06.push_back(pe_05_06.backIt());
    pn_06.reorderIncidence(std::move(inc_pn_06));

    vector<edge_list_it_t> inc_pn_09;
    inc_pn_09.push_back(pe_08_09.backIt());
    inc_pn_09.push_back(pe_09_02.backIt());
    inc_pn_09.push_back(pe_09_10.backIt());
    inc_pn_09.push_back(pe_17_09.backIt());
    pn_09.reorderIncidence(std::move(inc_pn_09));

    vector<edge_list_it_t> inc_pn_12;
    inc_pn_12.push_back(pe_03_12.backIt());
    inc_pn_12.push_back(pe_12_13.backIt());
    inc_pn_12.push_back(pe_12_17.backIt());
    inc_pn_12.push_back(pe_11_12.backIt());
    pn_12.reorderIncidence(std::move(inc_pn_12));

    vector<edge_list_it_t> inc_pn_15;
    inc_pn_15.push_back(pe_15_04.backIt());
    inc_pn_15.push_back(pe_15_16.backIt());
    inc_pn_15.push_back(pe_17_15.backIt());
    inc_pn_15.push_back(pe_14_15.backIt());
    pn_15.reorderIncidence(std::move(inc_pn_15));

    vector<edge_list_it_t> inc_pn_17;
    inc_pn_17.push_back(pe_06_17.backIt());
    inc_pn_17.push_back(pe_17_09.backIt());
    inc_pn_17.push_back(pe_12_17.backIt());
    inc_pn_17.push_back(pe_17_15.backIt());
    pn_17.reorderIncidence(std::move(inc_pn_17));


    n_01.pushIGForwardLink(pn_01.backIt());
    pn_01.pushIGBackwardLink(n_01.backIt());

    n_02.pushIGForwardLink(pn_02.backIt());
    pn_02.pushIGBackwardLink(n_02.backIt());
    n_03.pushIGForwardLink(pn_03.backIt());
    pn_03.pushIGBackwardLink(n_03.backIt());
    n_04.pushIGForwardLink(pn_04.backIt());
    pn_04.pushIGBackwardLink(n_04.backIt());

    pn_06.setVirtual(true);
    pn_09.setVirtual(true);
    pn_12.setVirtual(true);
    pn_15.setVirtual(true);
    pn_17.setVirtual(true);

    e_01_02.pushIGForwardLink(pe_01_06.backIt());

    pe_01_06.pushIGBackwardLink(e_01_02.backIt());
    pe_06_17.pushIGBackwardLink(e_01_02.backIt());
    pe_17_09.pushIGBackwardLink(e_01_02.backIt());
    pe_09_02.pushIGBackwardLink(e_01_02.backIt());

    pe_01_06.setChainN2(pe_06_17.backIt());

    pe_06_17.setChainN1(pe_01_06.backIt());
    pe_06_17.setChainN2(pe_17_09.backIt());

    pe_17_09.setChainN1(pe_06_17.backIt());
    pe_17_09.setChainN2(pe_09_02.backIt());

    pe_09_02.setChainN1(pe_17_09.backIt());

    pe_02_03.pushIGBackwardLink(e_02_03.backIt());
    e_02_03.pushIGForwardLink(pe_02_03.backIt());

    e_03_04.pushIGForwardLink(pe_03_12.backIt());

    pe_03_12.pushIGBackwardLink(e_03_04.backIt());
    pe_12_17.pushIGBackwardLink(e_03_04.backIt());
    pe_17_15.pushIGBackwardLink(e_03_04.backIt());
    pe_15_04.pushIGBackwardLink(e_03_04.backIt());

    pe_03_12.setChainN2(pe_12_17.backIt());

    pe_12_17.setChainN1(pe_03_12.backIt());
    pe_12_17.setChainN2(pe_17_15.backIt());

    pe_17_15.setChainN1(pe_12_17.backIt());
    pe_17_15.setChainN2(pe_15_04.backIt());

    pe_15_04.setChainN1(pe_17_15.backIt());

    pe_04_01.pushIGBackwardLink(e_04_01.backIt());
    e_04_01.pushIGForwardLink(pe_04_01.backIt());

    pe_05_06.setChainN2(pe_06_07.backIt());
    pe_06_07.setChainN1(pe_05_06.backIt());

    pe_05_06.pushIGBackwardLink(e_01_05.backIt());
    pe_06_07.pushIGBackwardLink(e_01_05.backIt());

    pe_08_09.setChainN2(pe_09_10.backIt());
    pe_09_10.setChainN1(pe_08_09.backIt());

    pe_08_09.pushIGBackwardLink(e_01_05.backIt());
    pe_09_10.pushIGBackwardLink(e_01_05.backIt());

    pe_11_12.setChainN2(pe_12_13.backIt());
    pe_12_13.setChainN1(pe_11_12.backIt());

    pe_11_12.pushIGBackwardLink(e_01_05.backIt());
    pe_12_13.pushIGBackwardLink(e_01_05.backIt());

    pe_14_15.setChainN2(pe_15_16.backIt());
    pe_15_16.setChainN1(pe_14_15.backIt());

    pe_14_15.pushIGBackwardLink(e_01_05.backIt());
    pe_15_16.pushIGBackwardLink(e_01_05.backIt());

    e_01_05.pushIGForwardLink(pe_15_16.backIt());

    auto edgeTuples = pg_1.removeUnnecessaryVirtualNodes();

    EXPECT_EQ(pg_1.numNodes(), 16);
    EXPECT_EQ(pg_1.numEdges(), 16);

    auto nit = pg_1.nodes().first;    
    auto& ppn_01 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_02 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_03 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_04 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_05 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_06 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_07 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_08 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_09 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_10 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_11 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_12 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_13 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_14 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_15 = dynamic_cast<PlanarizableNode&>(*(*nit++));
    auto& ppn_16 = dynamic_cast<PlanarizableNode&>(*(*nit++));

    auto eit = pg_1.edges().first;
    auto& ppe_01_06 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_09_02 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_02_03 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_03_12 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_15_04 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_04_01 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_05_06 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_06_07 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_08_09 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_09_10 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_11_12 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_12_13 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_14_15 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_15_16 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_06_09 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   
    auto& ppe_12_15 = dynamic_cast<PlanarizableEdge&>(*(*eit++));   

    EXPECT_EQ(ppe_01_06.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(ppe_09_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(ppe_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(ppe_03_12.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(ppe_15_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(ppe_04_01.IGBackwardLink(), e_04_01.backIt());
    EXPECT_EQ(ppe_05_06.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_06_07.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_08_09.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_09_10.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_11_12.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_12_13.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_14_15.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_15_16.IGBackwardLink(), e_01_05.backIt());
    EXPECT_EQ(ppe_06_09.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(ppe_12_15.IGBackwardLink(), e_03_04.backIt());

    EXPECT_EQ(e_01_02.IGForwardLink(), ppe_01_06.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(), ppe_02_03.backIt());
    EXPECT_EQ(e_03_04.IGForwardLink(), ppe_03_12.backIt());
    EXPECT_EQ(e_04_01.IGForwardLink(), ppe_04_01.backIt());

    EXPECT_EQ(ppe_01_06.isTerminalN1(), true);
    EXPECT_EQ(ppe_01_06.isTerminalN2(), false);
    EXPECT_EQ(ppe_01_06.chainN2(), ppe_06_09.backIt());

    EXPECT_EQ(ppe_06_09.isTerminalN1(), false);
    EXPECT_EQ(ppe_06_09.chainN1(), ppe_01_06.backIt());
    EXPECT_EQ(ppe_06_09.isTerminalN2(), false);
    EXPECT_EQ(ppe_06_09.chainN2(), ppe_09_02.backIt());
    EXPECT_EQ(ppe_09_02.isTerminalN1(), false);
    EXPECT_EQ(ppe_09_02.chainN1(), ppe_06_09.backIt());
    EXPECT_EQ(ppe_09_02.isTerminalN2(), true);

    EXPECT_EQ(ppe_05_06.isTerminalN1(), true);
    EXPECT_EQ(ppe_05_06.isTerminalN2(), false);
    EXPECT_EQ(ppe_05_06.chainN2(), ppe_06_07.backIt());

    EXPECT_EQ(ppe_06_07.isTerminalN1(), false);
    EXPECT_EQ(ppe_06_07.chainN1(), ppe_05_06.backIt());
    EXPECT_EQ(ppe_06_07.isTerminalN2(), true);

    EXPECT_EQ(ppe_03_12.isTerminalN1(), true);
    EXPECT_EQ(ppe_03_12.isTerminalN2(), false);
    EXPECT_EQ(ppe_03_12.chainN2(), ppe_12_15.backIt());

    EXPECT_EQ(ppe_12_15.isTerminalN1(), false);
    EXPECT_EQ(ppe_12_15.chainN1(), ppe_03_12.backIt());
    EXPECT_EQ(ppe_12_15.isTerminalN2(), false);
    EXPECT_EQ(ppe_12_15.chainN2(), ppe_15_04.backIt());

    EXPECT_EQ(ppe_15_04.isTerminalN1(), false);
    EXPECT_EQ(ppe_15_04.chainN1(), ppe_12_15.backIt());
    EXPECT_EQ(ppe_15_04.isTerminalN2(), true);


    auto iit = pn_06.incidentEdges().first;
    EXPECT_EQ(*iit, ppe_01_06.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_06_07.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_06_09.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_05_06.backIt());

    iit = pn_09.incidentEdges().first;
    EXPECT_EQ(*iit, ppe_08_09.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_09_02.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_09_10.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_06_09.backIt());

    iit = pn_12.incidentEdges().first;
    EXPECT_EQ(*iit, ppe_03_12.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_12_13.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_12_15.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_11_12.backIt());

    iit = pn_15.incidentEdges().first;
    EXPECT_EQ(*iit, ppe_15_04.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_15_16.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_12_15.backIt());
    iit++;
    EXPECT_EQ(*iit, ppe_14_15.backIt());

    EXPECT_EQ(edgeTuples.size(), 2);

    auto& tuple1 = edgeTuples[0];
    auto tpl1ep1 = std::move(get<0>(tuple1));
    auto tpl1ep2 = std::move(get<1>(tuple1));
    auto tpl1eit = get<2>(tuple1);

    auto& tuple2 = edgeTuples[1];
    auto tpl2ep1 = std::move(get<0>(tuple2));
    auto tpl2ep2 = std::move(get<1>(tuple2));
    auto tpl2eit = get<2>(tuple2);

    EXPECT_EQ(ppe_06_09.backIt(), tpl1eit);
    EXPECT_EQ(&pe_06_17, &(*(tpl1ep1)));
    EXPECT_EQ(&pe_17_09, &(*(tpl1ep2)));

    EXPECT_EQ(ppe_12_15.backIt(), tpl2eit);
    EXPECT_EQ(&pe_12_17, &(*(tpl2ep1)));
    EXPECT_EQ(&pe_17_15, &(*(tpl2ep2)));

}

} // namespace Undirected

} // namespace Wailea

