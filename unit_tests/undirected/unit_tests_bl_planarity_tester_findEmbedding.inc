/**  @brief tests rearrangeIncidentEdges()
 */
TEST_F(BLPlanarityTesterTests, Test116) {


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

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    auto& e_01_06 = g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    auto& e_01_07 = g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    auto& e_01_10 = g_1.addEdge(make_unique<Edge>(),n_01,n_10);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_02_06 = g_1.addEdge(make_unique<Edge>(),n_02,n_06);
    auto& e_02_08 = g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    auto& e_02_09 = g_1.addEdge(make_unique<Edge>(),n_02,n_09);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_03_09 = g_1.addEdge(make_unique<Edge>(),n_03,n_09);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    auto& e_04_08 = g_1.addEdge(make_unique<Edge>(),n_04,n_08);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_05_08 = g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    auto& e_05_09 = g_1.addEdge(make_unique<Edge>(),n_05,n_09);
    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    auto& e_06_09 = g_1.addEdge(make_unique<Edge>(),n_06,n_09);
    auto& e_07_09 = g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    auto& e_07_10 = g_1.addEdge(make_unique<Edge>(),n_07,n_10);
    auto& e_08_10 = g_1.addEdge(make_unique<Edge>(),n_08,n_10);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);

    vector<node_list_it_t> stOrderSrc;
    stOrderSrc.push_back(n_01.backIt());
    stOrderSrc.push_back(n_02.backIt());
    stOrderSrc.push_back(n_03.backIt());
    stOrderSrc.push_back(n_04.backIt());
    stOrderSrc.push_back(n_05.backIt());
    stOrderSrc.push_back(n_06.backIt());
    stOrderSrc.push_back(n_07.backIt());
    stOrderSrc.push_back(n_08.backIt());
    stOrderSrc.push_back(n_09.backIt());
    stOrderSrc.push_back(n_10.backIt());

    BLGraph g_2;
    vector<node_list_it_t> stOrderDst;

    BLTree pqTree;
    BLPlanarityTester tester;

    copyInputGraph(tester, g_1,stOrderSrc, g_2, stOrderDst);

    auto nPair = g_2.nodes();
    auto nIt   = nPair.first;

    auto& nc_01 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_02 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_03 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_04 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_05 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_06 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_07 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_08 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_09 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_10 = dynamic_cast<BLGraphNode&>(*(*nIt++));

    auto ePair = g_2.edges();
    auto eIt   = ePair.first;

    auto& ec_01_02 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_04 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_07 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_03 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_03_04 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_03_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_04_05 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_04_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_06_07 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_06_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_07_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_07_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_08_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_09_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));


    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_02.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_04.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_06.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_07.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_10.backIt());

    nc_02.mIncomingEdgesOrdered.push_back(ec_01_02.backIt());

    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_03.backIt());
    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_06.backIt());
    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_08.backIt());
    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_09.backIt());

    nc_03.mIncomingEdgesOrdered.push_back(ec_02_03.backIt());

    nc_03.mOutgoingEdgesOrdered.push_back(ec_03_04.backIt());
    nc_03.mOutgoingEdgesOrdered.push_back(ec_03_09.backIt());

    nc_04.mIncomingEdgesOrdered.push_back(ec_01_04.backIt());
    nc_04.mIncomingEdgesOrdered.push_back(ec_03_04.backIt());

    nc_04.mOutgoingEdgesOrdered.push_back(ec_04_05.backIt());
    nc_04.mOutgoingEdgesOrdered.push_back(ec_04_08.backIt());

    nc_05.mIncomingEdgesOrdered.push_back(ec_04_05.backIt());

    nc_05.mOutgoingEdgesOrdered.push_back(ec_05_06.backIt());
    nc_05.mOutgoingEdgesOrdered.push_back(ec_05_08.backIt());
    nc_05.mOutgoingEdgesOrdered.push_back(ec_05_09.backIt());

    nc_06.mIncomingEdgesOrdered.push_back(ec_01_06.backIt());
    nc_06.mIncomingEdgesOrdered.push_back(ec_02_06.backIt());
    nc_06.mIncomingEdgesOrdered.push_back(ec_05_06.backIt());

    nc_06.mOutgoingEdgesOrdered.push_back(ec_06_07.backIt());
    nc_06.mOutgoingEdgesOrdered.push_back(ec_06_09.backIt());

    nc_07.mIncomingEdgesOrdered.push_back(ec_01_07.backIt());
    nc_07.mIncomingEdgesOrdered.push_back(ec_06_07.backIt());

    nc_07.mOutgoingEdgesOrdered.push_back(ec_07_09.backIt());
    nc_07.mOutgoingEdgesOrdered.push_back(ec_07_10.backIt());

    nc_08.mIncomingEdgesOrdered.push_back(ec_02_08.backIt());
    nc_08.mIncomingEdgesOrdered.push_back(ec_04_08.backIt());
    nc_08.mIncomingEdgesOrdered.push_back(ec_05_08.backIt());

    nc_08.mOutgoingEdgesOrdered.push_back(ec_08_10.backIt());

    nc_09.mIncomingEdgesOrdered.push_back(ec_02_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_03_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_05_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_06_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_07_09.backIt());

    nc_09.mOutgoingEdgesOrdered.push_back(ec_09_10.backIt());

    nc_10.mIncomingEdgesOrdered.push_back(ec_01_10.backIt());
    nc_10.mIncomingEdgesOrdered.push_back(ec_07_10.backIt());
    nc_10.mIncomingEdgesOrdered.push_back(ec_08_10.backIt());
    nc_10.mIncomingEdgesOrdered.push_back(ec_09_10.backIt());

    tester.mOrientInNorm.push_back(nc_01.backIt());
    tester.mOrientInNorm.push_back(nc_02.backIt());
    tester.mOrientInNorm.push_back(nc_03.backIt());
    tester.mOrientInNorm.push_back(nc_04.backIt());
    tester.mOrientInNorm.push_back(nc_05.backIt());
    tester.mOrientInNorm.push_back(nc_06.backIt());
    tester.mOrientInNorm.push_back(nc_07.backIt());
    tester.mOrientInNorm.push_back(nc_09.backIt());
    tester.mOrientInNorm.push_back(nc_10.backIt());

    tester.mOrientOutNorm.push_back(nc_01.backIt());
    tester.mOrientOutNorm.push_back(nc_02.backIt());
    tester.mOrientOutNorm.push_back(nc_03.backIt());
    tester.mOrientOutNorm.push_back(nc_04.backIt());
    tester.mOrientOutNorm.push_back(nc_05.backIt());
    tester.mOrientOutNorm.push_back(nc_06.backIt());
    tester.mOrientOutNorm.push_back(nc_07.backIt());
    tester.mOrientOutNorm.push_back(nc_09.backIt());
    tester.mOrientOutNorm.push_back(nc_10.backIt());

    rearrangeIncidentEdges(tester, g_1, g_2);

    EXPECT_EQ(nc_01.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_02.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_03.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_04.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_05.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_06.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_07.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_08.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_09.mIncomingEdgesInReverse, false);
    EXPECT_EQ(nc_10.mIncomingEdgesInReverse, false);

    EXPECT_EQ(nc_01.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_02.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_03.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_04.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_05.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_06.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_07.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_08.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_09.mOutgoingEdgesInReverse, false);
    EXPECT_EQ(nc_10.mOutgoingEdgesInReverse, false);

    EXPECT_EQ(n_01.degree(), 5);
    EXPECT_EQ(n_02.degree(), 5);
    EXPECT_EQ(n_03.degree(), 3);
    EXPECT_EQ(n_04.degree(), 4);
    EXPECT_EQ(n_05.degree(), 4);
    EXPECT_EQ(n_06.degree(), 5);
    EXPECT_EQ(n_07.degree(), 4);
    EXPECT_EQ(n_08.degree(), 4);
    EXPECT_EQ(n_09.degree(), 6);
    EXPECT_EQ(n_10.degree(), 4);

    auto incIt = n_01.incidentEdges().first;
    EXPECT_EQ(e_01_10.backIt(), *incIt++);
    EXPECT_EQ(e_01_07.backIt(), *incIt++);
    EXPECT_EQ(e_01_06.backIt(), *incIt++);
    EXPECT_EQ(e_01_04.backIt(), *incIt++);
    EXPECT_EQ(e_01_02.backIt(), *incIt++);

    incIt = n_02.incidentEdges().first;
    EXPECT_EQ(e_01_02.backIt(), *incIt++);
    EXPECT_EQ(e_02_09.backIt(), *incIt++);
    EXPECT_EQ(e_02_08.backIt(), *incIt++);
    EXPECT_EQ(e_02_06.backIt(), *incIt++);
    EXPECT_EQ(e_02_03.backIt(), *incIt++);

    incIt = n_03.incidentEdges().first;
    EXPECT_EQ(e_02_03.backIt(), *incIt++);
    EXPECT_EQ(e_03_09.backIt(), *incIt++);
    EXPECT_EQ(e_03_04.backIt(), *incIt++);


    incIt = n_04.incidentEdges().first;
    EXPECT_EQ(e_01_04.backIt(), *incIt++);
    EXPECT_EQ(e_03_04.backIt(), *incIt++);
    EXPECT_EQ(e_04_08.backIt(), *incIt++);
    EXPECT_EQ(e_04_05.backIt(), *incIt++);

    incIt = n_05.incidentEdges().first;
    EXPECT_EQ(e_04_05.backIt(), *incIt++);
    EXPECT_EQ(e_05_09.backIt(), *incIt++);
    EXPECT_EQ(e_05_08.backIt(), *incIt++);
    EXPECT_EQ(e_05_06.backIt(), *incIt++);

    incIt = n_06.incidentEdges().first;
    EXPECT_EQ(e_01_06.backIt(), *incIt++);
    EXPECT_EQ(e_02_06.backIt(), *incIt++);
    EXPECT_EQ(e_05_06.backIt(), *incIt++);
    EXPECT_EQ(e_06_09.backIt(), *incIt++);
    EXPECT_EQ(e_06_07.backIt(), *incIt++);


    incIt = n_07.incidentEdges().first;
    EXPECT_EQ(e_01_07.backIt(), *incIt++);
    EXPECT_EQ(e_06_07.backIt(), *incIt++);
    EXPECT_EQ(e_07_10.backIt(), *incIt++);
    EXPECT_EQ(e_07_09.backIt(), *incIt++);

    incIt = n_08.incidentEdges().first;
    EXPECT_EQ(e_02_08.backIt(), *incIt++);
    EXPECT_EQ(e_04_08.backIt(), *incIt++);
    EXPECT_EQ(e_05_08.backIt(), *incIt++);
    EXPECT_EQ(e_08_10.backIt(), *incIt++);

    incIt = n_09.incidentEdges().first;
    EXPECT_EQ(e_02_09.backIt(), *incIt++);
    EXPECT_EQ(e_03_09.backIt(), *incIt++);
    EXPECT_EQ(e_05_09.backIt(), *incIt++);
    EXPECT_EQ(e_06_09.backIt(), *incIt++);
    EXPECT_EQ(e_07_09.backIt(), *incIt++);
    EXPECT_EQ(e_09_10.backIt(), *incIt++);

    incIt = n_10.incidentEdges().first;
    EXPECT_EQ(e_01_10.backIt(), *incIt++);
    EXPECT_EQ(e_07_10.backIt(), *incIt++);
    EXPECT_EQ(e_08_10.backIt(), *incIt++);
    EXPECT_EQ(e_09_10.backIt(), *incIt++);

}



/**  @brief tests rearrangeIncidentEdges() reversed
 */
TEST_F(BLPlanarityTesterTests, Test117) {


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

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    auto& e_01_06 = g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    auto& e_01_07 = g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    auto& e_01_10 = g_1.addEdge(make_unique<Edge>(),n_01,n_10);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_02_06 = g_1.addEdge(make_unique<Edge>(),n_02,n_06);
    auto& e_02_08 = g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    auto& e_02_09 = g_1.addEdge(make_unique<Edge>(),n_02,n_09);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_03_09 = g_1.addEdge(make_unique<Edge>(),n_03,n_09);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    auto& e_04_08 = g_1.addEdge(make_unique<Edge>(),n_04,n_08);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_05_08 = g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    auto& e_05_09 = g_1.addEdge(make_unique<Edge>(),n_05,n_09);
    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    auto& e_06_09 = g_1.addEdge(make_unique<Edge>(),n_06,n_09);
    auto& e_07_09 = g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    auto& e_07_10 = g_1.addEdge(make_unique<Edge>(),n_07,n_10);
    auto& e_08_10 = g_1.addEdge(make_unique<Edge>(),n_08,n_10);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);

    vector<node_list_it_t> stOrderSrc;
    stOrderSrc.push_back(n_01.backIt());
    stOrderSrc.push_back(n_02.backIt());
    stOrderSrc.push_back(n_03.backIt());
    stOrderSrc.push_back(n_04.backIt());
    stOrderSrc.push_back(n_05.backIt());
    stOrderSrc.push_back(n_06.backIt());
    stOrderSrc.push_back(n_07.backIt());
    stOrderSrc.push_back(n_08.backIt());
    stOrderSrc.push_back(n_09.backIt());
    stOrderSrc.push_back(n_10.backIt());

    BLGraph g_2;
    vector<node_list_it_t> stOrderDst;

    BLTree pqTree;
    BLPlanarityTester tester;

    copyInputGraph(tester, g_1,stOrderSrc, g_2, stOrderDst);

    auto nPair = g_2.nodes();
    auto nIt   = nPair.first;

    auto& nc_01 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_02 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_03 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_04 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_05 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_06 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_07 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_08 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_09 = dynamic_cast<BLGraphNode&>(*(*nIt++));
    auto& nc_10 = dynamic_cast<BLGraphNode&>(*(*nIt++));

    auto ePair = g_2.edges();
    auto eIt   = ePair.first;

    auto& ec_01_02 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_04 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_07 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_03 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_03_04 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_03_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_04_05 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_04_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_06_07 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_06_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_07_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_07_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_08_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_09_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));


    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_02.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_04.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_06.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_07.backIt());
    nc_01.mOutgoingEdgesOrdered.push_back(ec_01_10.backIt());

    nc_02.mIncomingEdgesOrdered.push_back(ec_01_02.backIt());

    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_03.backIt());
    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_06.backIt());
    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_08.backIt());
    nc_02.mOutgoingEdgesOrdered.push_back(ec_02_09.backIt());

    nc_03.mIncomingEdgesOrdered.push_back(ec_02_03.backIt());

    nc_03.mOutgoingEdgesOrdered.push_back(ec_03_04.backIt());
    nc_03.mOutgoingEdgesOrdered.push_back(ec_03_09.backIt());

    nc_04.mIncomingEdgesOrdered.push_back(ec_01_04.backIt());
    nc_04.mIncomingEdgesOrdered.push_back(ec_03_04.backIt());

    nc_04.mOutgoingEdgesOrdered.push_back(ec_04_05.backIt());
    nc_04.mOutgoingEdgesOrdered.push_back(ec_04_08.backIt());

    nc_05.mIncomingEdgesOrdered.push_back(ec_04_05.backIt());

    nc_05.mOutgoingEdgesOrdered.push_back(ec_05_06.backIt());
    nc_05.mOutgoingEdgesOrdered.push_back(ec_05_08.backIt());
    nc_05.mOutgoingEdgesOrdered.push_back(ec_05_09.backIt());

    nc_06.mIncomingEdgesOrdered.push_back(ec_01_06.backIt());
    nc_06.mIncomingEdgesOrdered.push_back(ec_02_06.backIt());
    nc_06.mIncomingEdgesOrdered.push_back(ec_05_06.backIt());

    nc_06.mOutgoingEdgesOrdered.push_back(ec_06_07.backIt());
    nc_06.mOutgoingEdgesOrdered.push_back(ec_06_09.backIt());

    nc_07.mIncomingEdgesOrdered.push_back(ec_01_07.backIt());
    nc_07.mIncomingEdgesOrdered.push_back(ec_06_07.backIt());

    nc_07.mOutgoingEdgesOrdered.push_back(ec_07_09.backIt());
    nc_07.mOutgoingEdgesOrdered.push_back(ec_07_10.backIt());

    nc_08.mIncomingEdgesOrdered.push_back(ec_02_08.backIt());
    nc_08.mIncomingEdgesOrdered.push_back(ec_04_08.backIt());
    nc_08.mIncomingEdgesOrdered.push_back(ec_05_08.backIt());

    nc_08.mOutgoingEdgesOrdered.push_back(ec_08_10.backIt());

    nc_09.mIncomingEdgesOrdered.push_back(ec_02_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_03_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_05_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_06_09.backIt());
    nc_09.mIncomingEdgesOrdered.push_back(ec_07_09.backIt());

    nc_09.mOutgoingEdgesOrdered.push_back(ec_09_10.backIt());

    nc_10.mIncomingEdgesOrdered.push_back(ec_01_10.backIt());
    nc_10.mIncomingEdgesOrdered.push_back(ec_07_10.backIt());
    nc_10.mIncomingEdgesOrdered.push_back(ec_08_10.backIt());
    nc_10.mIncomingEdgesOrdered.push_back(ec_09_10.backIt());


    tester.mOrientInReversed.push_back(nc_01.backIt());
    tester.mOrientInReversed.push_back(nc_02.backIt());
    tester.mOrientInReversed.push_back(nc_03.backIt());
    tester.mOrientInReversed.push_back(nc_04.backIt());
    tester.mOrientInReversed.push_back(nc_05.backIt());
    tester.mOrientInReversed.push_back(nc_06.backIt());
    tester.mOrientInReversed.push_back(nc_07.backIt());
    tester.mOrientInReversed.push_back(nc_08.backIt());
    tester.mOrientInReversed.push_back(nc_09.backIt());
    tester.mOrientInReversed.push_back(nc_10.backIt());

    tester.mOrientOutReversed.push_back(nc_01.backIt());
    tester.mOrientOutReversed.push_back(nc_02.backIt());
    tester.mOrientOutReversed.push_back(nc_03.backIt());
    tester.mOrientOutReversed.push_back(nc_04.backIt());
    tester.mOrientOutReversed.push_back(nc_05.backIt());
    tester.mOrientOutReversed.push_back(nc_06.backIt());
    tester.mOrientOutReversed.push_back(nc_07.backIt());
    tester.mOrientOutReversed.push_back(nc_08.backIt());
    tester.mOrientOutReversed.push_back(nc_09.backIt());
    tester.mOrientOutReversed.push_back(nc_10.backIt());

    rearrangeIncidentEdges(tester, g_1, g_2);

    EXPECT_EQ(nc_01.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_02.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_03.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_04.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_05.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_06.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_07.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_08.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_09.mIncomingEdgesInReverse, true);
    EXPECT_EQ(nc_10.mIncomingEdgesInReverse, true);

    EXPECT_EQ(nc_01.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_02.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_03.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_04.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_05.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_06.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_07.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_08.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_09.mOutgoingEdgesInReverse, true);
    EXPECT_EQ(nc_10.mOutgoingEdgesInReverse, true);

    EXPECT_EQ(n_01.degree(), 5);
    EXPECT_EQ(n_02.degree(), 5);
    EXPECT_EQ(n_03.degree(), 3);
    EXPECT_EQ(n_04.degree(), 4);
    EXPECT_EQ(n_05.degree(), 4);
    EXPECT_EQ(n_06.degree(), 5);
    EXPECT_EQ(n_07.degree(), 4);
    EXPECT_EQ(n_08.degree(), 4);
    EXPECT_EQ(n_09.degree(), 6);
    EXPECT_EQ(n_10.degree(), 4);

    auto incIt = n_01.incidentEdges().first;
    EXPECT_EQ(e_01_02.backIt(), *incIt++);
    EXPECT_EQ(e_01_04.backIt(), *incIt++);
    EXPECT_EQ(e_01_06.backIt(), *incIt++);
    EXPECT_EQ(e_01_07.backIt(), *incIt++);
    EXPECT_EQ(e_01_10.backIt(), *incIt++);

    incIt = n_02.incidentEdges().first;
    EXPECT_EQ(e_01_02.backIt(), *incIt++);
    EXPECT_EQ(e_02_03.backIt(), *incIt++);
    EXPECT_EQ(e_02_06.backIt(), *incIt++);
    EXPECT_EQ(e_02_08.backIt(), *incIt++);
    EXPECT_EQ(e_02_09.backIt(), *incIt++);

    incIt = n_03.incidentEdges().first;
    EXPECT_EQ(e_02_03.backIt(), *incIt++);
    EXPECT_EQ(e_03_04.backIt(), *incIt++);
    EXPECT_EQ(e_03_09.backIt(), *incIt++);


    incIt = n_04.incidentEdges().first;
    EXPECT_EQ(e_03_04.backIt(), *incIt++);
    EXPECT_EQ(e_01_04.backIt(), *incIt++);
    EXPECT_EQ(e_04_05.backIt(), *incIt++);
    EXPECT_EQ(e_04_08.backIt(), *incIt++);

    incIt = n_05.incidentEdges().first;
    EXPECT_EQ(e_04_05.backIt(), *incIt++);
    EXPECT_EQ(e_05_06.backIt(), *incIt++);
    EXPECT_EQ(e_05_08.backIt(), *incIt++);
    EXPECT_EQ(e_05_09.backIt(), *incIt++);

    incIt = n_06.incidentEdges().first;
    EXPECT_EQ(e_05_06.backIt(), *incIt++);
    EXPECT_EQ(e_02_06.backIt(), *incIt++);
    EXPECT_EQ(e_01_06.backIt(), *incIt++);
    EXPECT_EQ(e_06_07.backIt(), *incIt++);
    EXPECT_EQ(e_06_09.backIt(), *incIt++);

    incIt = n_07.incidentEdges().first;
    EXPECT_EQ(e_06_07.backIt(), *incIt++);
    EXPECT_EQ(e_01_07.backIt(), *incIt++);
    EXPECT_EQ(e_07_09.backIt(), *incIt++);
    EXPECT_EQ(e_07_10.backIt(), *incIt++);

    incIt = n_08.incidentEdges().first;
    EXPECT_EQ(e_05_08.backIt(), *incIt++);
    EXPECT_EQ(e_04_08.backIt(), *incIt++);
    EXPECT_EQ(e_02_08.backIt(), *incIt++);
    EXPECT_EQ(e_08_10.backIt(), *incIt++);

    incIt = n_09.incidentEdges().first;
    EXPECT_EQ(e_07_09.backIt(), *incIt++);
    EXPECT_EQ(e_06_09.backIt(), *incIt++);
    EXPECT_EQ(e_05_09.backIt(), *incIt++);
    EXPECT_EQ(e_03_09.backIt(), *incIt++);
    EXPECT_EQ(e_02_09.backIt(), *incIt++);
    EXPECT_EQ(e_09_10.backIt(), *incIt++);

    incIt = n_10.incidentEdges().first;
    EXPECT_EQ(e_09_10.backIt(), *incIt++);
    EXPECT_EQ(e_08_10.backIt(), *incIt++);
    EXPECT_EQ(e_07_10.backIt(), *incIt++);
    EXPECT_EQ(e_01_10.backIt(), *incIt++);

}

/**  @brief tests findEmbeddingFirstPass()
 */
TEST_F(BLPlanarityTesterTests, Test118) {

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

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    /*auto& e_01_10 = */g_1.addEdge(make_unique<Edge>(),n_01,n_10);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_04_08 = */g_1.addEdge(make_unique<Edge>(),n_04,n_08);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(),n_05,n_09);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    /*auto& e_06_09 = */g_1.addEdge(make_unique<Edge>(),n_06,n_09);
    /*auto& e_07_09 = */g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    /*auto& e_07_10 = */g_1.addEdge(make_unique<Edge>(),n_07,n_10);
    /*auto& e_08_10 = */g_1.addEdge(make_unique<Edge>(),n_08,n_10);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(),n_09,n_10);

    vector<node_list_it_t> stOrderSrc;
//    stOrderSrc.push_back(n_01.backIt());
//    stOrderSrc.push_back(n_10.backIt());
//    stOrderSrc.push_back(n_08.backIt());
//    stOrderSrc.push_back(n_09.backIt());
//    stOrderSrc.push_back(n_07.backIt());
//    stOrderSrc.push_back(n_06.backIt());
//    stOrderSrc.push_back(n_05.backIt());
//    stOrderSrc.push_back(n_04.backIt());
//    stOrderSrc.push_back(n_03.backIt());
//    stOrderSrc.push_back(n_02.backIt());

    stOrderSrc.push_back(n_01.backIt());
    stOrderSrc.push_back(n_02.backIt());
    stOrderSrc.push_back(n_03.backIt());
    stOrderSrc.push_back(n_04.backIt());
    stOrderSrc.push_back(n_05.backIt());
    stOrderSrc.push_back(n_06.backIt());
    stOrderSrc.push_back(n_07.backIt());
    stOrderSrc.push_back(n_08.backIt());
    stOrderSrc.push_back(n_09.backIt());
    stOrderSrc.push_back(n_10.backIt());

    BLGraph g_2;
    vector<node_list_it_t> stOrderDst;

    BLTree pqTree;
    BLPlanarityTester tester;

    copyInputGraph(tester, g_1, stOrderSrc, g_2, stOrderDst);

//    auto nPair = g_2.nodes();
//    auto nIt   = nPair.first;

//    auto& nc_01 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_02 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_03 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_04 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_05 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_06 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_07 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_08 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_09 = dynamic_cast<BLGraphNode&>(*(*nIt++));
//    auto& nc_10 = dynamic_cast<BLGraphNode&>(*(*nIt++));

    auto ePair = g_2.edges();
    auto eIt   = ePair.first;

    auto& ec_01_02 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_04 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_07 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_01_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_03 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_02_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_03_04 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_04_05 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_04_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_06 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_08 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_05_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_06_07 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_06_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_07_09 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_07_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_08_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));
    auto& ec_09_10 = dynamic_cast<BLGraphEdge&>(*(*eIt++));


    auto nit = stOrderDst.begin();
    size_t index = 0;
    node_list_it_t attachmentNode = pqTree.makePAttachment();


    // Iteration 1
    auto &n01 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.fanOutLeavesFromAttachment(attachmentNode,n01.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 2
    auto &n02 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
                     fromGraphEdgesToPQLeaves(tester, n02.mIncomingEdges);

    node_list_it_t pertinentRoot;
    node_list_it_t virtualRoot;

    bool result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n02, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n02.mIncomingEdgesOrdered.size(), 1);
    auto geIt = n02.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_01_02.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n02.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 3
    auto &n03 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n03.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n03, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n03.mIncomingEdgesOrdered.size(), 1);
    geIt = n03.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_02_03.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n03.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 4
    auto &n04 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n04.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n04, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n04.mIncomingEdgesOrdered.size(), 2);
    geIt = n04.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_01_04.backIt(), *geIt++);
    EXPECT_EQ(ec_03_04.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n04.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 5
    auto &n05 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n05.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n05, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n05.mIncomingEdgesOrdered.size(), 1);
    geIt = n05.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_04_05.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n05.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 6
    auto &n06 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n06.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n06, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n06.mIncomingEdgesOrdered.size(), 2);
    geIt = n06.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_01_06.backIt(), *geIt++);
    EXPECT_EQ(ec_05_06.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n06.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 7
    auto &n07 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n07.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n07, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n07.mIncomingEdgesOrdered.size(), 2);
    geIt = n07.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_01_07.backIt(), *geIt++);
    EXPECT_EQ(ec_06_07.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n07.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 8
    auto &n08 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n08.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n08, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n08.mIncomingEdgesOrdered.size(), 3);
    geIt = n08.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_05_08.backIt(), *geIt++);
    EXPECT_EQ(ec_04_08.backIt(), *geIt++);
    EXPECT_EQ(ec_02_08.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n08.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 9

    auto &n09 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n09.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n09, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n09.mIncomingEdgesOrdered.size(), 3);
    geIt = n09.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_05_09.backIt(), *geIt++);
    EXPECT_EQ(ec_06_09.backIt(), *geIt++);
    EXPECT_EQ(ec_07_09.backIt(), *geIt++);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n09.mOutgoingEdges);

    index++;
    nit++;


    // Iteration 10

    auto &n10 = dynamic_cast<BLGraphNode&>(*(*(*nit)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n10.mIncomingEdges);

    result = bubbleUp(tester,pqTree, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
    collectEdgeOrdering(tester,
                     pqTree, pertinentRoot, n10, BLPlanarityTester::INCOMING);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);
    if (!pqTree.isNil(virtualRoot)) {
        pqTree.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(n10.mIncomingEdgesOrdered.size(), 4);
    geIt = n10.mIncomingEdgesOrdered.begin();
    EXPECT_EQ(ec_07_10.backIt(), *geIt++);
    EXPECT_EQ(ec_09_10.backIt(), *geIt++);
    EXPECT_EQ(ec_08_10.backIt(), *geIt++);
    EXPECT_EQ(ec_01_10.backIt(), *geIt++);

    index++;
    nit++;

    // Second Pass

    BLTree pqTree2;

    pqTree2.trackQFlippings();

    attachmentNode = pqTree2.makePAttachment();

    index = 0;
    auto nit2 = stOrderDst.rbegin();

    // Iteration 1.
//    cerr << "Iteration 1\n";
    auto& m10 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m10, cerr);

    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m10.mIncomingEdgesOrdered, m10.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    index++;
    nit2++;


    // Iteration 2.
//    cerr << "Iteration 2\n";
    auto& m09 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m09, cerr);

    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m09.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m09, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m09.mIncomingEdgesOrdered, m09.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m09.mOutgoingEdgesOrdered.size(), 1);
    geIt = m09.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_09_10.backIt(), *geIt++);


    index++;
    nit2++;


    // Iteration 3.
//    cerr << "Iteration 3\n";
    auto& m08 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m08, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m08.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m08, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m08.mIncomingEdgesOrdered, m08.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);


    EXPECT_EQ(m08.mOutgoingEdgesOrdered.size(), 1);
    geIt = m08.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_08_10.backIt(), *geIt++);

    index++;
    nit2++;


    // Iteration 4.
//    cerr << "Iteration 4\n";
    auto& m07 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m07, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m07.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m07, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m07.mIncomingEdgesOrdered, m07.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m07.mOutgoingEdgesOrdered.size(), 2);
    geIt = m07.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_07_10.backIt(), *geIt++);
    EXPECT_EQ(ec_07_09.backIt(), *geIt++);

    index++;
    nit2++;


    // Iteration 5.
//    cerr << "Iteration 5\n";
    auto& m06 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m06, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m06.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m06, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m06.mIncomingEdgesOrdered, m06.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m06.mOutgoingEdgesOrdered.size(), 2);
    geIt = m06.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_06_07.backIt(), *geIt++);
    EXPECT_EQ(ec_06_09.backIt(), *geIt++);

    index++;
    nit2++;


    // Iteration 6.
//    cerr << "Iteration 6\n";
    auto& m05 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m05, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m05.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);
//    cerr << "applyTemplates\n";
    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);

//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m05, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m05.mIncomingEdgesOrdered, m05.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m05.mOutgoingEdgesOrdered.size(), 3);
    geIt = m05.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_05_08.backIt(), *geIt++);
    EXPECT_EQ(ec_05_09.backIt(), *geIt++);
    EXPECT_EQ(ec_05_06.backIt(), *geIt++);
    index++;
    nit2++;


    // Iteration 7.
//    cerr << "Iteration 7\n";
    auto& m04 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m04, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m04.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m04, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m04.mIncomingEdgesOrdered, m04.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m04.mOutgoingEdgesOrdered.size(), 2);
    geIt = m04.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_04_05.backIt(), *geIt++);
    EXPECT_EQ(ec_04_08.backIt(), *geIt++);

    index++;
    nit2++;


    // Iteration 8.
//    cerr << "Iteration 8\n";
    auto& m03 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m03, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m03.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);


    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m03, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m03.mIncomingEdgesOrdered, m03.backIt());

//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m03.mOutgoingEdgesOrdered.size(), 1);
    geIt = m03.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_03_04.backIt(), *geIt++);

    index++;
    nit2++;


    // Iteration 9.
//    cerr << "Iteration 9\n";
    auto& m02 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m02, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m02.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m02, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }
    pqTree2.fanOutLeavesFromQAttachment(
                     attachmentNode, m02.mIncomingEdgesOrdered, m02.backIt());
//    cerr << "fanout\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    EXPECT_EQ(m02.mOutgoingEdgesOrdered.size(), 2);
    geIt = m02.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_02_03.backIt(), *geIt++);
    EXPECT_EQ(ec_02_08.backIt(), *geIt++);

    index++;
    nit2++;


    // Iteration 10.
//    cerr << "Iteration 10\n";
    auto& m01 = dynamic_cast<BLGraphNode&>(*(*(*(nit2))));
//    printNode(m01, cerr);
    pqTree2.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, m01.mOutgoingEdges);

    result = bubbleUp(tester, pqTree2, pertinentLeaves, virtualRoot);
    EXPECT_EQ(result, true);
//    cerr << "bubbleUp\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    result = applyTemplates(tester, pqTree2, pertinentLeaves, pertinentRoot);
    EXPECT_EQ(result, true);
//    cerr << "applyTemplates\n";
//    pqTree2.printTree(cerr, pertinentRoot);

    collectEdgeOrdering(
              tester,pqTree2, pertinentRoot, m01, BLPlanarityTester::OUTGOING);

    attachmentNode = removePertinentTree(tester, pqTree2, pertinentRoot);

    if (!pqTree2.isNil(virtualRoot)) {
        pqTree2.removeVirtualRoot(virtualRoot);
    }

    EXPECT_EQ(m01.mOutgoingEdgesOrdered.size(), 5);
    geIt = m01.mOutgoingEdgesOrdered.begin();
    EXPECT_EQ(ec_01_07.backIt(), *geIt++);
    EXPECT_EQ(ec_01_06.backIt(), *geIt++);
    EXPECT_EQ(ec_01_04.backIt(), *geIt++);
    EXPECT_EQ(ec_01_02.backIt(), *geIt++);
    EXPECT_EQ(ec_01_10.backIt(), *geIt++);

//    pqTree2.printTree(cerr, pqTree2.nil());

    auto& Q = pqTree2.toNodeRef(pqTree2.nodes().first);

    tester.mOrientInNorm.splice(tester.mOrientInNorm.end(), Q.mOrientInNorm);
    tester.mOrientInReversed.splice(tester.mOrientInReversed.end(), 
                                                          Q.mOrientInReversed);
    tester.mOrientOutNorm.splice(tester.mOrientOutNorm.end(),Q.mOrientOutNorm);
    tester.mOrientOutReversed.splice(tester.mOrientOutReversed.end(),
                                                         Q.mOrientOutReversed);
    tester.rearrangeIncidentEdges(g_1, g_2);

//    list <node_list_it_t> NodeList;
//    NodeList.push_back(n_01.backIt());
//    NodeList.push_back(n_02.backIt());
//    NodeList.push_back(n_03.backIt());
//    NodeList.push_back(n_04.backIt());
//    NodeList.push_back(n_05.backIt());
//    NodeList.push_back(n_06.backIt());
//    NodeList.push_back(n_07.backIt());
//    NodeList.push_back(n_08.backIt());
//    NodeList.push_back(n_09.backIt());
//    NodeList.push_back(n_10.backIt());
//    for (auto nIt : NodeList) {
//        auto &N = dynamic_cast<NumNode&>(*(*nIt));
//        cerr << "[" << N.num() << "]:";
//        for (auto ait = N.incidentEdges().first;
//                                 ait != N.incidentEdges().second; ait++) { 
//            auto& E = dynamic_cast<Edge&>(*(*(*ait)));
//            auto& A = dynamic_cast<NumNode&>(E.adjacentNode(N));
//            cerr << A.num() << ",";
//        }
//        cerr << "\n";
//    }

    list<node_list_it_t> embed_n01;
    embed_n01.push_back(n_10.backIt());
    embed_n01.push_back(n_02.backIt());
    embed_n01.push_back(n_04.backIt());
    embed_n01.push_back(n_06.backIt());
    embed_n01.push_back(n_07.backIt());

    list<node_list_it_t> embed_n02;
    embed_n02.push_back(n_01.backIt());
    embed_n02.push_back(n_08.backIt());
    embed_n02.push_back(n_03.backIt());

    list<node_list_it_t> embed_n03;
    embed_n03.push_back(n_02.backIt());
    embed_n03.push_back(n_04.backIt());

    list<node_list_it_t> embed_n04;
    embed_n04.push_back(n_01.backIt());
    embed_n04.push_back(n_03.backIt());
    embed_n04.push_back(n_08.backIt());
    embed_n04.push_back(n_05.backIt());

    list<node_list_it_t> embed_n05;
    embed_n05.push_back(n_04.backIt());
    embed_n05.push_back(n_08.backIt());
    embed_n05.push_back(n_09.backIt());
    embed_n05.push_back(n_06.backIt());

    list<node_list_it_t> embed_n06;
    embed_n06.push_back(n_01.backIt());
    embed_n06.push_back(n_05.backIt());
    embed_n06.push_back(n_09.backIt());
    embed_n06.push_back(n_07.backIt());

    list<node_list_it_t> embed_n07;
    embed_n07.push_back(n_01.backIt());
    embed_n07.push_back(n_06.backIt());
    embed_n07.push_back(n_09.backIt());
    embed_n07.push_back(n_10.backIt());

    list<node_list_it_t> embed_n08;
    embed_n08.push_back(n_05.backIt());
    embed_n08.push_back(n_04.backIt());
    embed_n08.push_back(n_02.backIt());
    embed_n08.push_back(n_10.backIt());

    list<node_list_it_t> embed_n09;
    embed_n09.push_back(n_07.backIt());
    embed_n09.push_back(n_06.backIt());
    embed_n09.push_back(n_05.backIt());
    embed_n09.push_back(n_10.backIt());

    list<node_list_it_t> embed_n10;
    embed_n10.push_back(n_07.backIt());
    embed_n10.push_back(n_09.backIt());
    embed_n10.push_back(n_08.backIt());
    embed_n10.push_back(n_01.backIt());

    bool n01_flipped;
    EXPECT_EQ(circular_check(n_01, embed_n01, n01_flipped), true);
    bool n02_flipped;
    EXPECT_EQ(circular_check(n_02, embed_n02, n02_flipped), true);
    bool n03_flipped;
    EXPECT_EQ(circular_check(n_03, embed_n03, n03_flipped), true);
    bool n04_flipped;
    EXPECT_EQ(circular_check(n_04, embed_n04, n04_flipped), true);
    bool n05_flipped;
    EXPECT_EQ(circular_check(n_05, embed_n05, n05_flipped), true);
    bool n06_flipped;
    EXPECT_EQ(circular_check(n_06, embed_n06, n06_flipped), true);
    bool n07_flipped;
    EXPECT_EQ(circular_check(n_07, embed_n07, n07_flipped), true);
    bool n08_flipped;
    EXPECT_EQ(circular_check(n_08, embed_n08, n08_flipped), true);
    bool n09_flipped;
    EXPECT_EQ(circular_check(n_09, embed_n09, n09_flipped), true);
    bool n10_flipped;
    EXPECT_EQ(circular_check(n_10, embed_n10, n10_flipped), true);

   EXPECT_EQ((n01_flipped&&
              n02_flipped&&
//              n03_flipped&&
              n04_flipped&&
              n05_flipped&&
              n06_flipped&&
              n07_flipped&&
              n08_flipped&&
              n09_flipped&&
              n10_flipped   )||
             (!n01_flipped&&
              !n02_flipped&&
//              !n03_flipped&&
              !n04_flipped&&
              !n05_flipped&&
              !n06_flipped&&
              !n07_flipped&&
              !n08_flipped&&
              !n09_flipped&&
              !n10_flipped   ), true);


}


/**  @brief tests findEmbedding() for a planar graph with N=10
 */
TEST_F(BLPlanarityTesterTests, Test119) {

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

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    /*auto& e_01_10 = */g_1.addEdge(make_unique<Edge>(),n_01,n_10);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_04_08 = */g_1.addEdge(make_unique<Edge>(),n_04,n_08);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    /*auto& e_05_09 = */g_1.addEdge(make_unique<Edge>(),n_05,n_09);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    /*auto& e_06_09 = */g_1.addEdge(make_unique<Edge>(),n_06,n_09);
    /*auto& e_07_09 = */g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    /*auto& e_07_10 = */g_1.addEdge(make_unique<Edge>(),n_07,n_10);
    /*auto& e_08_10 = */g_1.addEdge(make_unique<Edge>(),n_08,n_10);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(),n_09,n_10);

    list<node_list_it_t> embed_n01;
    embed_n01.push_back(n_10.backIt());
    embed_n01.push_back(n_02.backIt());
    embed_n01.push_back(n_04.backIt());
    embed_n01.push_back(n_06.backIt());
    embed_n01.push_back(n_07.backIt());

    list<node_list_it_t> embed_n02;
    embed_n02.push_back(n_01.backIt());
    embed_n02.push_back(n_08.backIt());
    embed_n02.push_back(n_03.backIt());

    list<node_list_it_t> embed_n03;
    embed_n03.push_back(n_02.backIt());
    embed_n03.push_back(n_04.backIt());

    list<node_list_it_t> embed_n04;
    embed_n04.push_back(n_01.backIt());
    embed_n04.push_back(n_03.backIt());
    embed_n04.push_back(n_08.backIt());
    embed_n04.push_back(n_05.backIt());

    list<node_list_it_t> embed_n05;
    embed_n05.push_back(n_04.backIt());
    embed_n05.push_back(n_08.backIt());
    embed_n05.push_back(n_09.backIt());
    embed_n05.push_back(n_06.backIt());

    list<node_list_it_t> embed_n06;
    embed_n06.push_back(n_01.backIt());
    embed_n06.push_back(n_05.backIt());
    embed_n06.push_back(n_09.backIt());
    embed_n06.push_back(n_07.backIt());

    list<node_list_it_t> embed_n07;
    embed_n07.push_back(n_01.backIt());
    embed_n07.push_back(n_06.backIt());
    embed_n07.push_back(n_09.backIt());
    embed_n07.push_back(n_10.backIt());

    list<node_list_it_t> embed_n08;
    embed_n08.push_back(n_05.backIt());
    embed_n08.push_back(n_04.backIt());
    embed_n08.push_back(n_02.backIt());
    embed_n08.push_back(n_10.backIt());

    list<node_list_it_t> embed_n09;
    embed_n09.push_back(n_07.backIt());
    embed_n09.push_back(n_06.backIt());
    embed_n09.push_back(n_05.backIt());
    embed_n09.push_back(n_10.backIt());

    list<node_list_it_t> embed_n10;
    embed_n10.push_back(n_07.backIt());
    embed_n10.push_back(n_09.backIt());
    embed_n10.push_back(n_08.backIt());
    embed_n10.push_back(n_01.backIt());




    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
      for (size_t j = 0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//          cerr << "Testing [" << i << "][" << j << "]\n";

            STNumbering st;
            vector<node_list_it_t> st_list =
                       st.getBipolarOrientation(g_1, **TList[i], **TList[j]);

//            cerr << "st: ";
//            for (auto it : st_list) {
//
//                auto& N = dynamic_cast<NumNode&>(*(*it));
//                cerr << N.num() << ",";
//            }
//            cerr << "\n";

            BLPlanarityTester tester;
            EXPECT_EQ(tester.findEmbedding(g_1, st_list), true);

            bool n01_flipped;
            EXPECT_EQ(circular_check(n_01, embed_n01, n01_flipped), true);
            bool n02_flipped;
            EXPECT_EQ(circular_check(n_02, embed_n02, n02_flipped), true);
            bool n03_flipped;
            EXPECT_EQ(circular_check(n_03, embed_n03, n03_flipped), true);
            bool n04_flipped;
            EXPECT_EQ(circular_check(n_04, embed_n04, n04_flipped), true);
            bool n05_flipped;
            EXPECT_EQ(circular_check(n_05, embed_n05, n05_flipped), true);
            bool n06_flipped;
            EXPECT_EQ(circular_check(n_06, embed_n06, n06_flipped), true);
            bool n07_flipped;
            EXPECT_EQ(circular_check(n_07, embed_n07, n07_flipped), true);
            bool n08_flipped;
            EXPECT_EQ(circular_check(n_08, embed_n08, n08_flipped), true);
            bool n09_flipped;
            EXPECT_EQ(circular_check(n_09, embed_n09, n09_flipped), true);
            bool n10_flipped;
            EXPECT_EQ(circular_check(n_10, embed_n10, n10_flipped), true);

            EXPECT_EQ((n01_flipped&&
              n02_flipped&&
//              n03_flipped&&
              n04_flipped&&
              n05_flipped&&
              n06_flipped&&
              n07_flipped&&
              n08_flipped&&
              n09_flipped&&
              n10_flipped   )||
             (!n01_flipped&&
              !n02_flipped&&
//              !n03_flipped&&
              !n04_flipped&&
              !n05_flipped&&
              !n06_flipped&&
              !n07_flipped&&
              !n08_flipped&&
              !n09_flipped&&
              !n10_flipped   ), true);
        }
    }
}


/**  @brief tests findEmbedding() for a planar graph with N=10
 */
TEST_F(BLPlanarityTesterTests, Test120) {

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
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));
    auto& n_21 = g_1.addNode(make_unique<NumNode>(21));
    auto& n_22 = g_1.addNode(make_unique<NumNode>(22));
    auto& n_23 = g_1.addNode(make_unique<NumNode>(23));
    auto& n_24 = g_1.addNode(make_unique<NumNode>(24));
    auto& n_25 = g_1.addNode(make_unique<NumNode>(25));

    g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    g_1.addEdge(make_unique<Edge>(),n_04,n_09);
    g_1.addEdge(make_unique<Edge>(),n_04,n_10);
    g_1.addEdge(make_unique<Edge>(),n_05,n_03);
    g_1.addEdge(make_unique<Edge>(),n_05,n_10);
    g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    g_1.addEdge(make_unique<Edge>(),n_06,n_11);
    g_1.addEdge(make_unique<Edge>(),n_06,n_12);
    g_1.addEdge(make_unique<Edge>(),n_07,n_12);
    g_1.addEdge(make_unique<Edge>(),n_07,n_13);
    g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    g_1.addEdge(make_unique<Edge>(),n_08,n_13);
    g_1.addEdge(make_unique<Edge>(),n_08,n_14);
    g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    g_1.addEdge(make_unique<Edge>(),n_09,n_15);
    g_1.addEdge(make_unique<Edge>(),n_10,n_15);
    g_1.addEdge(make_unique<Edge>(),n_11,n_16);
    g_1.addEdge(make_unique<Edge>(),n_11,n_17);
    g_1.addEdge(make_unique<Edge>(),n_12,n_13);
    g_1.addEdge(make_unique<Edge>(),n_12,n_17);
    g_1.addEdge(make_unique<Edge>(),n_12,n_18);
    g_1.addEdge(make_unique<Edge>(),n_13,n_14);
    g_1.addEdge(make_unique<Edge>(),n_13,n_18);
    g_1.addEdge(make_unique<Edge>(),n_14,n_15);
    g_1.addEdge(make_unique<Edge>(),n_14,n_19);
    g_1.addEdge(make_unique<Edge>(),n_14,n_20);
    g_1.addEdge(make_unique<Edge>(),n_15,n_20);
    g_1.addEdge(make_unique<Edge>(),n_16,n_17);
    g_1.addEdge(make_unique<Edge>(),n_16,n_21);
    g_1.addEdge(make_unique<Edge>(),n_16,n_22);
    g_1.addEdge(make_unique<Edge>(),n_17,n_18);
    g_1.addEdge(make_unique<Edge>(),n_18,n_19);
    g_1.addEdge(make_unique<Edge>(),n_18,n_23);
    g_1.addEdge(make_unique<Edge>(),n_18,n_24);
    g_1.addEdge(make_unique<Edge>(),n_19,n_20);
    g_1.addEdge(make_unique<Edge>(),n_19,n_24);
    g_1.addEdge(make_unique<Edge>(),n_19,n_25);
    g_1.addEdge(make_unique<Edge>(),n_20,n_25);
    g_1.addEdge(make_unique<Edge>(),n_21,n_17);
    g_1.addEdge(make_unique<Edge>(),n_21,n_22);
    g_1.addEdge(make_unique<Edge>(),n_22,n_23);
    g_1.addEdge(make_unique<Edge>(),n_23,n_24);
    g_1.addEdge(make_unique<Edge>(),n_24,n_25);


    list<node_list_it_t> al_n1;
    al_n1.push_back(n_06.backIt());
    al_n1.push_back(n_02.backIt());

    list<node_list_it_t> al_n2;
    al_n2.push_back(n_01.backIt());
    al_n2.push_back(n_03.backIt());
    al_n2.push_back(n_08.backIt());
    al_n2.push_back(n_07.backIt());

    list<node_list_it_t> al_n3;
    al_n3.push_back(n_04.backIt());
    al_n3.push_back(n_02.backIt());
    al_n3.push_back(n_05.backIt());

    list<node_list_it_t> al_n4;
    al_n4.push_back(n_05.backIt());
    al_n4.push_back(n_10.backIt());
    al_n4.push_back(n_09.backIt());
    al_n4.push_back(n_03.backIt());

    list<node_list_it_t> al_n5;
    al_n5.push_back(n_10.backIt());
    al_n5.push_back(n_04.backIt());
    al_n5.push_back(n_03.backIt());

    list<node_list_it_t> al_n6;
    al_n6.push_back(n_12.backIt());
    al_n6.push_back(n_11.backIt());
    al_n6.push_back(n_01.backIt());
    al_n6.push_back(n_07.backIt());

    list<node_list_it_t> al_n7;
    al_n7.push_back(n_12.backIt());
    al_n7.push_back(n_06.backIt());
    al_n7.push_back(n_02.backIt());
    al_n7.push_back(n_13.backIt());

    list<node_list_it_t> al_n8;
    al_n8.push_back(n_09.backIt());
    al_n8.push_back(n_14.backIt());
    al_n8.push_back(n_13.backIt());
    al_n8.push_back(n_02.backIt());

    list<node_list_it_t> al_n9;
    al_n9.push_back(n_04.backIt());
    al_n9.push_back(n_10.backIt());
    al_n9.push_back(n_15.backIt());
    al_n9.push_back(n_08.backIt());

    list<node_list_it_t> al_n10;
    al_n10.push_back(n_05.backIt());
    al_n10.push_back(n_15.backIt());
    al_n10.push_back(n_09.backIt());
    al_n10.push_back(n_04.backIt());

    list<node_list_it_t> al_n11;
    al_n11.push_back(n_17.backIt());
    al_n11.push_back(n_16.backIt());
    al_n11.push_back(n_06.backIt());

    list<node_list_it_t> al_n12;
    al_n12.push_back(n_18.backIt());
    al_n12.push_back(n_17.backIt());
    al_n12.push_back(n_06.backIt());
    al_n12.push_back(n_07.backIt());
    al_n12.push_back(n_13.backIt());

    list<node_list_it_t> al_n13;
    al_n13.push_back(n_08.backIt());
    al_n13.push_back(n_14.backIt());
    al_n13.push_back(n_18.backIt());
    al_n13.push_back(n_12.backIt());
    al_n13.push_back(n_07.backIt());

    list<node_list_it_t> al_n14;
    al_n14.push_back(n_08.backIt());
    al_n14.push_back(n_15.backIt());
    al_n14.push_back(n_20.backIt());
    al_n14.push_back(n_19.backIt());
    al_n14.push_back(n_13.backIt());

    list<node_list_it_t> al_n15;
    al_n15.push_back(n_10.backIt());
    al_n15.push_back(n_20.backIt());
    al_n15.push_back(n_14.backIt());
    al_n15.push_back(n_09.backIt());

    list<node_list_it_t> al_n16;
    al_n16.push_back(n_17.backIt());
    al_n16.push_back(n_21.backIt());
    al_n16.push_back(n_22.backIt());
    al_n16.push_back(n_11.backIt());

    list<node_list_it_t> al_n17;
    al_n17.push_back(n_12.backIt());
    al_n17.push_back(n_18.backIt());
    al_n17.push_back(n_21.backIt());
    al_n17.push_back(n_16.backIt());
    al_n17.push_back(n_11.backIt());

    list<node_list_it_t> al_n18;
    al_n18.push_back(n_19.backIt());
    al_n18.push_back(n_24.backIt());
    al_n18.push_back(n_23.backIt());
    al_n18.push_back(n_17.backIt());
    al_n18.push_back(n_12.backIt());
    al_n18.push_back(n_13.backIt());

    list<node_list_it_t> al_n19;
    al_n19.push_back(n_14.backIt());
    al_n19.push_back(n_20.backIt());
    al_n19.push_back(n_25.backIt());
    al_n19.push_back(n_24.backIt());
    al_n19.push_back(n_18.backIt());

    list<node_list_it_t> al_n20;
    al_n20.push_back(n_15.backIt());
    al_n20.push_back(n_25.backIt());
    al_n20.push_back(n_19.backIt());
    al_n20.push_back(n_14.backIt());

    list<node_list_it_t> al_n21;
    al_n21.push_back(n_22.backIt());
    al_n21.push_back(n_16.backIt());
    al_n21.push_back(n_17.backIt());

    list<node_list_it_t> al_n22;
    al_n22.push_back(n_23.backIt());
    al_n22.push_back(n_16.backIt());
    al_n22.push_back(n_21.backIt());

    list<node_list_it_t> al_n23;
    al_n23.push_back(n_24.backIt());
    al_n23.push_back(n_22.backIt());
    al_n23.push_back(n_18.backIt());

    list<node_list_it_t> al_n24;
    al_n24.push_back(n_25.backIt());
    al_n24.push_back(n_23.backIt());
    al_n24.push_back(n_18.backIt());
    al_n24.push_back(n_19.backIt());

    list<node_list_it_t> al_n25;
    al_n25.push_back(n_20.backIt());
    al_n25.push_back(n_24.backIt());
    al_n25.push_back(n_19.backIt());

    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
        for (size_t j =  0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//            cerr << "Testing [" << i << "][" << j << "]\n";
            STNumbering st;
            vector<node_list_it_t> st_list =
                       st.getBipolarOrientation(g_1, **TList[i], **TList[j]);
//            cerr << "STNum size: [" << st_list.size() << "]\n";

//            cerr << "STNum:\n";
//            for (auto sit : st_list) {
//                auto& S = dynamic_cast<NumNode&>(*(*(sit)));
//                cerr << S.num() << "\n";
//            }

            BLPlanarityTester tester;
            EXPECT_EQ(tester.findEmbedding(g_1, st_list), true);

            bool n1_flipped;
            EXPECT_EQ(circular_check(n_01, al_n1, n1_flipped), true);
//            if (n1_flipped) {cerr << "n1\n";}
            bool n2_flipped;
            EXPECT_EQ(circular_check(n_02, al_n2, n2_flipped), true);
//            if (n2_flipped) {cerr << "n2\n";}
            bool n3_flipped;
            EXPECT_EQ(circular_check(n_03, al_n3, n3_flipped), true);
//            if (n3_flipped) {cerr << "n3\n";}
            bool n4_flipped;
            EXPECT_EQ(circular_check(n_04, al_n4, n4_flipped), true);
//            if (n4_flipped) {cerr << "n4\n";}
            bool n5_flipped;
            EXPECT_EQ(circular_check(n_05, al_n5, n5_flipped), true);
//            if (n5_flipped) {cerr << "n5\n";}
            bool n6_flipped;
            EXPECT_EQ(circular_check(n_06, al_n6, n6_flipped), true);
//            if (n6_flipped) {cerr << "n6\n";}
            bool n7_flipped;
            EXPECT_EQ(circular_check(n_07, al_n7, n7_flipped), true);
//            if (n7_flipped) {cerr << "n7\n";}
            bool n8_flipped;
            EXPECT_EQ(circular_check(n_08, al_n8, n8_flipped), true);
//            if (n8_flipped) {cerr << "n8\n";}
            bool n9_flipped;
            EXPECT_EQ(circular_check(n_09, al_n9, n9_flipped), true);
//            if (n9_flipped) {cerr << "n9\n";}
            bool n10_flipped;
            EXPECT_EQ(circular_check(n_10, al_n10, n10_flipped), true);
//            if (n10_flipped) {cerr << "n10\n";}
            bool n11_flipped;
            EXPECT_EQ(circular_check(n_11, al_n11, n11_flipped), true);
//            if (n11_flipped) {cerr << "n11\n";}
            bool n12_flipped;
            EXPECT_EQ(circular_check(n_12, al_n12, n12_flipped), true);
//            if (n12_flipped) {cerr << "n12\n";}
            bool n13_flipped;
            EXPECT_EQ(circular_check(n_13, al_n13, n13_flipped), true);
//            if (n13_flipped) {cerr << "n13\n";}
            bool n14_flipped;
            EXPECT_EQ(circular_check(n_14, al_n14, n14_flipped), true);
//            if (n14_flipped) {cerr << "n14\n";}
            bool n15_flipped;
            EXPECT_EQ(circular_check(n_15, al_n15, n15_flipped), true);
//            if (n15_flipped) {cerr << "n15\n";}
            bool n16_flipped;
            EXPECT_EQ(circular_check(n_16, al_n16, n16_flipped), true);
//            if (n16_flipped) {cerr << "n16\n";}
            bool n17_flipped;
            EXPECT_EQ(circular_check(n_17, al_n17, n17_flipped), true);
//            if (n17_flipped) {cerr << "n17\n";}
            bool n18_flipped;
            EXPECT_EQ(circular_check(n_18, al_n18, n18_flipped), true);
//            if (n18_flipped) {cerr << "n18\n";}
            bool n19_flipped;
            EXPECT_EQ(circular_check(n_19, al_n19, n19_flipped), true);
//            if (n19_flipped) {cerr << "n19\n";}
            bool n20_flipped;
            EXPECT_EQ(circular_check(n_20, al_n20, n20_flipped), true);
//            if (n20_flipped) {cerr << "n20\n";}
            bool n21_flipped;
            EXPECT_EQ(circular_check(n_21, al_n21, n21_flipped), true);
//            if (n21_flipped) {cerr << "n21\n";}
            bool n22_flipped;
            EXPECT_EQ(circular_check(n_22, al_n22, n22_flipped), true);
//            if (n22_flipped) {cerr << "n22\n";}
            bool n23_flipped;
            EXPECT_EQ(circular_check(n_23, al_n23, n23_flipped), true);
//            if (n23_flipped) {cerr << "n23\n";}
            bool n24_flipped;
            EXPECT_EQ(circular_check(n_24, al_n24, n24_flipped), true);
//            if (n24_flipped) {cerr << "n24\n";}
            bool n25_flipped;
            EXPECT_EQ(circular_check(n_25, al_n25, n25_flipped), true);
//            if (n25_flipped) {cerr << "n25\n";}

            EXPECT_EQ((
               n2_flipped&&
               n3_flipped&&
               n4_flipped&&
               n5_flipped&&
               n6_flipped&&
               n7_flipped&&
               n8_flipped&&
               n9_flipped&&
               n10_flipped&&
               n11_flipped&&
               n12_flipped&&
               n13_flipped&&
               n14_flipped&&
               n15_flipped&&
               n16_flipped&&
               n17_flipped&&
               n18_flipped&&
               n19_flipped&&
               n20_flipped&&
               n21_flipped&&
               n22_flipped&&
               n23_flipped&&
               n24_flipped&&
               n25_flipped   )||
              (!n2_flipped&&
               !n3_flipped&&
               !n4_flipped&&
               !n5_flipped&&
               !n6_flipped&&
               !n7_flipped&&
               !n8_flipped&&
               !n9_flipped&&
               !n10_flipped&&
               !n11_flipped&&
               !n12_flipped&&
               !n13_flipped&&
               !n14_flipped&&
               !n15_flipped&&
               !n16_flipped&&
               !n17_flipped&&
               !n18_flipped&&
               !n19_flipped&&
               !n20_flipped&&
               !n21_flipped&&
               !n22_flipped&&
               !n23_flipped&&
               !n24_flipped&&
               !n25_flipped   ), true);

        }
    }
//    for (auto nIt = g_1.nodes().first; nIt != g_1.nodes().second; nIt++) {
//        auto& N = dynamic_cast<NumNode&>(*(*(nIt)));
//        N.printAdjacency(std::cerr);
//    }
}


/**  @brief tests findEmbedding() for a planar graph with N=20 (dodeca hedron)
 */
TEST_F(BLPlanarityTesterTests, Test121) {

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
    auto& n_11 = g_1.addNode(make_unique<NumNode>(11));
    auto& n_12 = g_1.addNode(make_unique<NumNode>(12));
    auto& n_13 = g_1.addNode(make_unique<NumNode>(13));
    auto& n_14 = g_1.addNode(make_unique<NumNode>(14));
    auto& n_15 = g_1.addNode(make_unique<NumNode>(15));
    auto& n_16 = g_1.addNode(make_unique<NumNode>(16));
    auto& n_17 = g_1.addNode(make_unique<NumNode>(17));
    auto& n_18 = g_1.addNode(make_unique<NumNode>(18));
    auto& n_19 = g_1.addNode(make_unique<NumNode>(19));
    auto& n_20 = g_1.addNode(make_unique<NumNode>(20));

    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_06_15 = */g_1.addEdge(make_unique<Edge>(),n_06,n_15);
    /*auto& e_06_11 = */g_1.addEdge(make_unique<Edge>(),n_06,n_11);
    /*auto& e_05_10 = */g_1.addEdge(make_unique<Edge>(),n_05,n_10);
    /*auto& e_02_07 = */g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    /*auto& e_10_15 = */g_1.addEdge(make_unique<Edge>(),n_10,n_15);
    /*auto& e_07_11 = */g_1.addEdge(make_unique<Edge>(),n_07,n_11);
    /*auto& e_15_20 = */g_1.addEdge(make_unique<Edge>(),n_15,n_20);
    /*auto& e_11_16 = */g_1.addEdge(make_unique<Edge>(),n_11,n_16);
    /*auto& e_20_16 = */g_1.addEdge(make_unique<Edge>(),n_20,n_16);
    /*auto& e_05_04 = */g_1.addEdge(make_unique<Edge>(),n_05,n_04);
    /*auto& e_10_14 = */g_1.addEdge(make_unique<Edge>(),n_10,n_14);
    /*auto& e_20_19 = */g_1.addEdge(make_unique<Edge>(),n_20,n_19);
    /*auto& e_16_17 = */g_1.addEdge(make_unique<Edge>(),n_16,n_17);
    /*auto& e_07_12 = */g_1.addEdge(make_unique<Edge>(),n_07,n_12);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_14_19 = */g_1.addEdge(make_unique<Edge>(),n_14,n_19);
    /*auto& e_14_09 = */g_1.addEdge(make_unique<Edge>(),n_14,n_09);
    /*auto& e_18_19 = */g_1.addEdge(make_unique<Edge>(),n_18,n_19);
    /*auto& e_18_17 = */g_1.addEdge(make_unique<Edge>(),n_18,n_17);
    /*auto& e_12_17 = */g_1.addEdge(make_unique<Edge>(),n_12,n_17);
    /*auto& e_12_08 = */g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    /*auto& e_13_18 = */g_1.addEdge(make_unique<Edge>(),n_13,n_18);
    /*auto& e_09_13 = */g_1.addEdge(make_unique<Edge>(),n_09,n_13);
    /*auto& e_13_08 = */g_1.addEdge(make_unique<Edge>(),n_13,n_08);
    /*auto& e_03_08 = */g_1.addEdge(make_unique<Edge>(),n_03,n_08);
    /*auto& e_04_09 = */g_1.addEdge(make_unique<Edge>(),n_04,n_09);
    /*auto& e_04_03 = */g_1.addEdge(make_unique<Edge>(),n_04,n_03);

    list<node_list_it_t> al_n01;
    al_n01.push_back(n_02.backIt());
    al_n01.push_back(n_06.backIt());
    al_n01.push_back(n_05.backIt());

    list<node_list_it_t> al_n02;
    al_n02.push_back(n_03.backIt());
    al_n02.push_back(n_07.backIt());
    al_n02.push_back(n_01.backIt());

    list<node_list_it_t> al_n03;
    al_n03.push_back(n_04.backIt());
    al_n03.push_back(n_08.backIt());
    al_n03.push_back(n_02.backIt());

    list<node_list_it_t> al_n04;
    al_n04.push_back(n_05.backIt());
    al_n04.push_back(n_09.backIt());
    al_n04.push_back(n_03.backIt());

    list<node_list_it_t> al_n05;
    al_n05.push_back(n_01.backIt());
    al_n05.push_back(n_10.backIt());
    al_n05.push_back(n_04.backIt());

    list<node_list_it_t> al_n06;
    al_n06.push_back(n_01.backIt());
    al_n06.push_back(n_11.backIt());
    al_n06.push_back(n_15.backIt());

    list<node_list_it_t> al_n07;
    al_n07.push_back(n_02.backIt());
    al_n07.push_back(n_12.backIt());
    al_n07.push_back(n_11.backIt());

    list<node_list_it_t> al_n08;
    al_n08.push_back(n_12.backIt());
    al_n08.push_back(n_03.backIt());
    al_n08.push_back(n_13.backIt());

    list<node_list_it_t> al_n09;
    al_n09.push_back(n_13.backIt());
    al_n09.push_back(n_04.backIt());
    al_n09.push_back(n_14.backIt());

    list<node_list_it_t> al_n10;
    al_n10.push_back(n_15.backIt());
    al_n10.push_back(n_14.backIt());
    al_n10.push_back(n_05.backIt());

    list<node_list_it_t> al_n11;
    al_n11.push_back(n_07.backIt());
    al_n11.push_back(n_16.backIt());
    al_n11.push_back(n_06.backIt());

    list<node_list_it_t> al_n12;
    al_n12.push_back(n_08.backIt());
    al_n12.push_back(n_17.backIt());
    al_n12.push_back(n_07.backIt());

    list<node_list_it_t> al_n13;
    al_n13.push_back(n_08.backIt());
    al_n13.push_back(n_09.backIt());
    al_n13.push_back(n_18.backIt());

    list<node_list_it_t> al_n14;
    al_n14.push_back(n_09.backIt());
    al_n14.push_back(n_10.backIt());
    al_n14.push_back(n_19.backIt());

    list<node_list_it_t> al_n15;
    al_n15.push_back(n_10.backIt());
    al_n15.push_back(n_06.backIt());
    al_n15.push_back(n_20.backIt());

    list<node_list_it_t> al_n16;
    al_n16.push_back(n_11.backIt());
    al_n16.push_back(n_17.backIt());
    al_n16.push_back(n_20.backIt());

    list<node_list_it_t> al_n17;
    al_n17.push_back(n_12.backIt());
    al_n17.push_back(n_18.backIt());
    al_n17.push_back(n_16.backIt());

    list<node_list_it_t> al_n18;
    al_n18.push_back(n_17.backIt());
    al_n18.push_back(n_13.backIt());
    al_n18.push_back(n_19.backIt());

    list<node_list_it_t> al_n19;
    al_n19.push_back(n_20.backIt());
    al_n19.push_back(n_18.backIt());
    al_n19.push_back(n_14.backIt());

    list<node_list_it_t> al_n20;
    al_n20.push_back(n_16.backIt());
    al_n20.push_back(n_19.backIt());
    al_n20.push_back(n_15.backIt());


    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
        for (size_t j =  0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//            cerr << "Testing [" << i << "][" << j << "]\n";
            STNumbering st;
            vector<node_list_it_t> st_list =
                       st.getBipolarOrientation(g_1, **TList[i], **TList[j]);
//            cerr << "STNum size: [" << st_list.size() << "]\n";

//            cerr << "STNum:\n";
//            for (auto sit : st_list) {
//                auto& S = dynamic_cast<NumNode&>(*(*(sit)));
//                cerr << S.num() << "\n";
//            }

            BLPlanarityTester tester;
            EXPECT_EQ(tester.findEmbedding(g_1, st_list), true);

            bool n01_flipped;
            EXPECT_EQ(circular_check(n_01, al_n01, n01_flipped), true);
//            if (n01_flipped) {cerr << "n1\n";}
            bool n02_flipped;
            EXPECT_EQ(circular_check(n_02, al_n02, n02_flipped), true);
//            if (n2_flipped) {cerr << "n2\n";}
            bool n03_flipped;
            EXPECT_EQ(circular_check(n_03, al_n03, n03_flipped), true);
//            if (n03_flipped) {cerr << "n3\n";}
            bool n04_flipped;
            EXPECT_EQ(circular_check(n_04, al_n04, n04_flipped), true);
//            if (n4_flipped) {cerr << "n4\n";}
            bool n05_flipped;
            EXPECT_EQ(circular_check(n_05, al_n05, n05_flipped), true);
//            if (n05_flipped) {cerr << "n5\n";}
            bool n06_flipped;
            EXPECT_EQ(circular_check(n_06, al_n06, n06_flipped), true);
//            if (n06_flipped) {cerr << "n6\n";}
            bool n07_flipped;
            EXPECT_EQ(circular_check(n_07, al_n07, n07_flipped), true);
//            if (n07_flipped) {cerr << "n7\n";}
            bool n08_flipped;
            EXPECT_EQ(circular_check(n_08, al_n08, n08_flipped), true);
//            if (n08_flipped) {cerr << "n8\n";}
            bool n09_flipped;
            EXPECT_EQ(circular_check(n_09, al_n09, n09_flipped), true);
//            if (n9_flipped) {cerr << "n9\n";}
            bool n10_flipped;
            EXPECT_EQ(circular_check(n_10, al_n10, n10_flipped), true);
//            if (n10_flipped) {cerr << "n10\n";}
            bool n11_flipped;
            EXPECT_EQ(circular_check(n_11, al_n11, n11_flipped), true);
//            if (n11_flipped) {cerr << "n11\n";}
            bool n12_flipped;
            EXPECT_EQ(circular_check(n_12, al_n12, n12_flipped), true);
//            if (n12_flipped) {cerr << "n12\n";}
            bool n13_flipped;
            EXPECT_EQ(circular_check(n_13, al_n13, n13_flipped), true);
//            if (n13_flipped) {cerr << "n13\n";}
            bool n14_flipped;
            EXPECT_EQ(circular_check(n_14, al_n14, n14_flipped), true);
//            if (n14_flipped) {cerr << "n14\n";}
            bool n15_flipped;
            EXPECT_EQ(circular_check(n_15, al_n15, n15_flipped), true);
//            if (n15_flipped) {cerr << "n15\n";}
            bool n16_flipped;
            EXPECT_EQ(circular_check(n_16, al_n16, n16_flipped), true);
//            if (n16_flipped) {cerr << "n16\n";}
            bool n17_flipped;
            EXPECT_EQ(circular_check(n_17, al_n17, n17_flipped), true);
//            if (n17_flipped) {cerr << "n17\n";}
            bool n18_flipped;
            EXPECT_EQ(circular_check(n_18, al_n18, n18_flipped), true);
//            if (n18_flipped) {cerr << "n18\n";}
            bool n19_flipped;
            EXPECT_EQ(circular_check(n_19, al_n19, n19_flipped), true);
//            if (n19_flipped) {cerr << "n19\n";}
            bool n20_flipped;
            EXPECT_EQ(circular_check(n_20, al_n20, n20_flipped), true);
//            if (n20_flipped) {cerr << "n20\n";}

            EXPECT_EQ((
               n01_flipped&&
               n02_flipped&&
               n03_flipped&&
               n04_flipped&&
               n05_flipped&&
               n06_flipped&&
               n07_flipped&&
               n08_flipped&&
               n09_flipped&&
               n10_flipped&&
               n11_flipped&&
               n12_flipped&&
               n13_flipped&&
               n14_flipped&&
               n15_flipped&&
               n16_flipped&&
               n17_flipped&&
               n18_flipped&&
               n19_flipped&&
               n20_flipped   )||
              (!n01_flipped&&
               !n02_flipped&&
               !n03_flipped&&
               !n04_flipped&&
               !n05_flipped&&
               !n06_flipped&&
               !n07_flipped&&
               !n08_flipped&&
               !n09_flipped&&
               !n10_flipped&&
               !n11_flipped&&
               !n12_flipped&&
               !n13_flipped&&
               !n14_flipped&&
               !n15_flipped&&
               !n16_flipped&&
               !n17_flipped&&
               !n18_flipped&&
               !n19_flipped&&
               !n20_flipped   ), true);
        }
    }
}
