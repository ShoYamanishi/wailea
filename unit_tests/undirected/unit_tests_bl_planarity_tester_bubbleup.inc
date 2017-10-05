
/**  @brief tests copyInputGraph()
 */
TEST_F(BLPlanarityTesterTests, Test1) {


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

    EXPECT_EQ(g_2.numNodes(), 10);
    EXPECT_EQ(g_2.numEdges(), 22);

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

    EXPECT_EQ(nc_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(nc_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(nc_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(nc_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(nc_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(nc_06.IGBackwardLink(), n_06.backIt());
    EXPECT_EQ(nc_07.IGBackwardLink(), n_07.backIt());
    EXPECT_EQ(nc_08.IGBackwardLink(), n_08.backIt());
    EXPECT_EQ(nc_09.IGBackwardLink(), n_09.backIt());
    EXPECT_EQ(nc_10.IGBackwardLink(), n_10.backIt());

    EXPECT_EQ(nc_01.mStNumber, 0);
    EXPECT_EQ(nc_02.mStNumber, 1);
    EXPECT_EQ(nc_03.mStNumber, 2);
    EXPECT_EQ(nc_04.mStNumber, 3);
    EXPECT_EQ(nc_05.mStNumber, 4);
    EXPECT_EQ(nc_06.mStNumber, 5);
    EXPECT_EQ(nc_07.mStNumber, 6);
    EXPECT_EQ(nc_08.mStNumber, 7);
    EXPECT_EQ(nc_09.mStNumber, 8);
    EXPECT_EQ(nc_10.mStNumber, 9);

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

    EXPECT_EQ(ec_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(ec_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(ec_01_06.IGBackwardLink(), e_01_06.backIt());
    EXPECT_EQ(ec_01_07.IGBackwardLink(), e_01_07.backIt());
    EXPECT_EQ(ec_01_10.IGBackwardLink(), e_01_10.backIt());
    EXPECT_EQ(ec_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(ec_02_06.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(ec_02_08.IGBackwardLink(), e_02_08.backIt());
    EXPECT_EQ(ec_02_09.IGBackwardLink(), e_02_09.backIt());
    EXPECT_EQ(ec_03_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(ec_03_09.IGBackwardLink(), e_03_09.backIt());
    EXPECT_EQ(ec_04_05.IGBackwardLink(), e_04_05.backIt());
    EXPECT_EQ(ec_04_08.IGBackwardLink(), e_04_08.backIt());
    EXPECT_EQ(ec_05_06.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(ec_05_08.IGBackwardLink(), e_05_08.backIt());
    EXPECT_EQ(ec_05_09.IGBackwardLink(), e_05_09.backIt());
    EXPECT_EQ(ec_06_07.IGBackwardLink(), e_06_07.backIt());
    EXPECT_EQ(ec_06_09.IGBackwardLink(), e_06_09.backIt());
    EXPECT_EQ(ec_07_09.IGBackwardLink(), e_07_09.backIt());
    EXPECT_EQ(ec_07_10.IGBackwardLink(), e_07_10.backIt());
    EXPECT_EQ(ec_08_10.IGBackwardLink(), e_08_10.backIt());
    EXPECT_EQ(ec_09_10.IGBackwardLink(), e_09_10.backIt());

    list<edge_list_it_t> nc_01_incoming;

    list<edge_list_it_t> nc_01_outgoing;
    nc_01_outgoing.push_back(ec_01_02.backIt());
    nc_01_outgoing.push_back(ec_01_04.backIt());
    nc_01_outgoing.push_back(ec_01_06.backIt());
    nc_01_outgoing.push_back(ec_01_07.backIt());
    nc_01_outgoing.push_back(ec_01_10.backIt());

    list<edge_list_it_t> nc_02_incoming;
    nc_02_incoming.push_back(ec_01_02.backIt());

    list<edge_list_it_t> nc_02_outgoing;
    nc_02_outgoing.push_back(ec_02_03.backIt());
    nc_02_outgoing.push_back(ec_02_06.backIt());
    nc_02_outgoing.push_back(ec_02_08.backIt());
    nc_02_outgoing.push_back(ec_02_09.backIt());

    list<edge_list_it_t> nc_03_incoming;
    nc_03_incoming.push_back(ec_02_03.backIt());

    list<edge_list_it_t> nc_03_outgoing;
    nc_03_outgoing.push_back(ec_03_04.backIt());
    nc_03_outgoing.push_back(ec_03_09.backIt());

    list<edge_list_it_t> nc_04_incoming;
    nc_04_incoming.push_back(ec_01_04.backIt());
    nc_04_incoming.push_back(ec_03_04.backIt());

    list<edge_list_it_t> nc_04_outgoing;
    nc_04_outgoing.push_back(ec_04_05.backIt());
    nc_04_outgoing.push_back(ec_04_08.backIt());

    list<edge_list_it_t> nc_05_incoming;
    nc_05_incoming.push_back(ec_04_05.backIt());

    list<edge_list_it_t> nc_05_outgoing;
    nc_05_outgoing.push_back(ec_05_06.backIt());
    nc_05_outgoing.push_back(ec_05_08.backIt());
    nc_05_outgoing.push_back(ec_05_09.backIt());

    list<edge_list_it_t> nc_06_incoming;
    nc_06_incoming.push_back(ec_01_06.backIt());
    nc_06_incoming.push_back(ec_02_06.backIt());
    nc_06_incoming.push_back(ec_05_06.backIt());

    list<edge_list_it_t> nc_06_outgoing;
    nc_06_outgoing.push_back(ec_06_07.backIt());
    nc_06_outgoing.push_back(ec_06_09.backIt());

    list<edge_list_it_t> nc_07_incoming;
    nc_07_incoming.push_back(ec_01_07.backIt());
    nc_07_incoming.push_back(ec_06_07.backIt());

    list<edge_list_it_t> nc_07_outgoing;
    nc_07_outgoing.push_back(ec_07_09.backIt());
    nc_07_outgoing.push_back(ec_07_10.backIt());

    list<edge_list_it_t> nc_08_incoming;
    nc_08_incoming.push_back(ec_02_08.backIt());
    nc_08_incoming.push_back(ec_04_08.backIt());
    nc_08_incoming.push_back(ec_05_08.backIt());

    list<edge_list_it_t> nc_08_outgoing;
    nc_08_outgoing.push_back(ec_08_10.backIt());

    list<edge_list_it_t> nc_09_incoming;
    nc_09_incoming.push_back(ec_02_09.backIt());
    nc_09_incoming.push_back(ec_03_09.backIt());
    nc_09_incoming.push_back(ec_05_09.backIt());
    nc_09_incoming.push_back(ec_06_09.backIt());
    nc_09_incoming.push_back(ec_07_09.backIt());

    list<edge_list_it_t> nc_09_outgoing;
    nc_09_outgoing.push_back(ec_09_10.backIt());

    list<edge_list_it_t> nc_10_incoming;
    nc_10_incoming.push_back(ec_01_10.backIt());
    nc_10_incoming.push_back(ec_07_10.backIt());
    nc_10_incoming.push_back(ec_08_10.backIt());
    nc_10_incoming.push_back(ec_09_10.backIt());

    list<edge_list_it_t> nc_10_outgoing;

    EXPECT_EQ(cmp_edges_unordered(nc_01.mIncomingEdges,nc_01_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_01.mOutgoingEdges,nc_01_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_02.mIncomingEdges,nc_02_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_02.mOutgoingEdges,nc_02_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_03.mIncomingEdges,nc_03_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_03.mOutgoingEdges,nc_03_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_04.mIncomingEdges,nc_04_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_04.mOutgoingEdges,nc_04_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_05.mIncomingEdges,nc_05_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_05.mOutgoingEdges,nc_05_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_06.mIncomingEdges,nc_06_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_06.mOutgoingEdges,nc_06_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_07.mIncomingEdges,nc_07_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_07.mOutgoingEdges,nc_07_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_08.mIncomingEdges,nc_08_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_08.mOutgoingEdges,nc_08_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_09.mIncomingEdges,nc_09_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_09.mOutgoingEdges,nc_09_outgoing),true);
    EXPECT_EQ(cmp_edges_unordered(nc_10.mIncomingEdges,nc_10_incoming),true);
    EXPECT_EQ(cmp_edges_unordered(nc_10.mOutgoingEdges,nc_10_outgoing),true);

    EXPECT_EQ(stOrderDst.size(), 10);

    vector<node_list_it_t>::iterator lit = stOrderDst.begin();
    EXPECT_EQ(nc_01.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_02.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_03.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_04.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_05.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_06.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_07.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_08.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_09.backIt(), *lit);
    lit++;
    EXPECT_EQ(nc_10.backIt(), *lit);

}


/**  @brief tests BLTree::makePAttachment();
 */
TEST_F(BLPlanarityTesterTests, Test2) {

    BLTree pqTree;
    auto nIt =  pqTree.makePAttachment();

    EXPECT_EQ(pqTree.numNodes(), 1);
    EXPECT_EQ(pqTree.numEdges(), 0);
    EXPECT_EQ(pqTree.nodes().first, nIt);
    BLTreeNode& att = dynamic_cast<BLTreeNode&>(*(*nIt));

    EXPECT_EQ(&(att.mTree),             &pqTree);
    EXPECT_EQ(att.mNodeType,            BLTreeNode::PType);
    EXPECT_EQ(att.mChildren.size(),     0);
    EXPECT_EQ(att.mChildrenCount,       0);
    EXPECT_EQ(att.mParent,              pqTree.nil());
    EXPECT_EQ(att.mEndChild1,           pqTree.nil());
    EXPECT_EQ(att.mEndChild2,           pqTree.nil());
    EXPECT_EQ(att.mSibling1,            pqTree.nil());
    EXPECT_EQ(att.mSibling2,            pqTree.nil());
    EXPECT_EQ(att.mParent,              pqTree.nil());
    EXPECT_EQ(att.mGeneration,          0);
    EXPECT_EQ(att.mUnblocked,           false);
    EXPECT_EQ(att.mBlocked,             false);
    EXPECT_EQ(att.mPertinentChildrenCount, 0);
    EXPECT_EQ(att.mPertinentChildrenCountProcessed, 0);
    EXPECT_EQ(att.mPertinentType,       BLTreeNode::PertinentUnknown);
    EXPECT_EQ(att.mPertinentLeavesCount,0);
    EXPECT_EQ(att.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(att.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(att.mCDPartialChild,      pqTree.nil());
    EXPECT_EQ(att.mFullChildren.size(), 0);

}


/**  @brief tests BLTree::fanOutLeavesFromAttachment()
 */
TEST_F(BLPlanarityTesterTests, Test3) {

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

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    /*auto& e_01_10 = */g_1.addEdge(make_unique<Edge>(),n_01,n_10);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_06 = */g_1.addEdge(make_unique<Edge>(),n_02,n_06);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    /*auto& e_02_09 = */g_1.addEdge(make_unique<Edge>(),n_02,n_09);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(),n_03,n_09);
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

    auto nIt2 =    pqTree.makePAttachment();
    auto& A  =     pqTree.toNodeRef(nIt2);
    pqTree.fanOutLeavesFromAttachment(A.backIt(), nc_01.mOutgoingEdges);

    EXPECT_EQ(parent_check(A),true);
    EXPECT_EQ(cross_check(pqTree, nc_01.mOutgoingEdges, A.mChildren),true);
    EXPECT_EQ(nc_01.mOutgoingEdges.size(), A.mChildrenCount);

}


/**  @brief tests BLTree::fanOutLeavesFromAttachment()
 */
TEST_F(BLPlanarityTesterTests, Test4) {

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

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    /*auto& e_01_10 = */g_1.addEdge(make_unique<Edge>(),n_01,n_10);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_06 = */g_1.addEdge(make_unique<Edge>(),n_02,n_06);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    /*auto& e_02_09 = */g_1.addEdge(make_unique<Edge>(),n_02,n_09);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_03_09 = */g_1.addEdge(make_unique<Edge>(),n_03,n_09);
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

    auto nIt2 =   pqTree.makePAttachment();
    auto& A  =    pqTree.toNodeRef(nIt2);

    fanOutLeavesFromDummyAttachment(pqTree, A, nc_01.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_02.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_03.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_04.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_05.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_06.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_07.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_08.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_09.mOutgoingEdges);
    fanOutLeavesFromDummyAttachment(pqTree, A, nc_10.mOutgoingEdges);


    vector<node_list_it_t> nc_01_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_01.mIncomingEdges);

    EXPECT_EQ(
        cross_check(pqTree, nc_01.mIncomingEdges, nc_01_pertinentLeaves),true);

    vector<node_list_it_t> nc_02_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_02.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_02.mIncomingEdges, nc_02_pertinentLeaves),true);

    vector<node_list_it_t> nc_03_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_03.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_03.mIncomingEdges, nc_03_pertinentLeaves),true);

    vector<node_list_it_t> nc_04_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_04.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_04.mIncomingEdges, nc_04_pertinentLeaves),true);

    vector<node_list_it_t> nc_05_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_05.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_05.mIncomingEdges, nc_05_pertinentLeaves),true);

    vector<node_list_it_t> nc_06_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_06.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_06.mIncomingEdges, nc_06_pertinentLeaves),true);

    vector<node_list_it_t> nc_07_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_07.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_07.mIncomingEdges, nc_07_pertinentLeaves),true);

    vector<node_list_it_t> nc_08_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_08.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_08.mIncomingEdges, nc_08_pertinentLeaves),true);

    vector<node_list_it_t> nc_09_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_09.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_09.mIncomingEdges, nc_09_pertinentLeaves),true);

    vector<node_list_it_t> nc_10_pertinentLeaves =
            fromGraphEdgesToPQLeaves(tester, nc_10.mIncomingEdges);

    EXPECT_EQ(cross_check(
                    pqTree, nc_10.mIncomingEdges, nc_10_pertinentLeaves),true);

}


/**  @brief tests BLPlanarizer::bubbleUp() #1
 */
TEST_F(BLPlanarityTesterTests, Test5) {

    BLPlanarityTester tester;
    BLTree pqTree;
    BLGraph G;
    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L03 = createL(pqTree, G);

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);

    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    auto& L13 = createL(pqTree, G);
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree, G);

    auto& L15 = createL(pqTree, G);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());

    auto& Q01 = createQ(pqTree, Q01children);

    auto& L16 = createL(pqTree, G);
    auto& L17 = createL(pqTree, G);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree, G);
    auto& L19 = createL(pqTree, G);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree, G);

    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree, G);
    auto& L24 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree, G);

    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    auto& P04 = createP(pqTree, P04children);

    vector<node_list_it_t> pertinentLeaves;

    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12

    pqTree.initializeForOneIteration();

    pertinentLeaves.push_back(L01.backIt());

    node_list_it_t virtualRoot;

    auto result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);



    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02*L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
          
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01 L14  L15*Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03 *L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L04.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, false);
    EXPECT_EQ(virtualRoot, pqTree.nil());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01*L14 *L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03 *L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    auto& VR = pqTree.toNodeRef(virtualRoot);
    EXPECT_EQ(&(VR.mTree), &(pqTree));
    EXPECT_EQ(VR.mNodeType, BLTreeNode::VirtualRootType);
    EXPECT_EQ(VR.mChildren.size(), 0);
    EXPECT_EQ(VR.mChildrenCount, 0);
    EXPECT_EQ(VR.mEndChild1, pqTree.nil());
    EXPECT_EQ(VR.mEndChild2, pqTree.nil());
    EXPECT_EQ(VR.mSibling1, pqTree.nil());
    EXPECT_EQ(VR.mSibling2, pqTree.nil());
    EXPECT_EQ(VR.mParent, pqTree.nil());
    EXPECT_EQ(VR.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(VR.mBlocked, false);
    EXPECT_EQ(VR.mUnblocked, false);
    EXPECT_EQ(VR.mPertinentChildrenCount, 4);
    EXPECT_EQ(VR.mPertinentChildrenCountProcessed, 0);
    EXPECT_EQ(VR.mPertinentType, BLTreeNode::PertinentUnknown);
    EXPECT_EQ(VR.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(VR.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(VR.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(VR.mFullChildren.size(), 0);


    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13 P01*L14  L15 Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03 *L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L16.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, false);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                        *P04
    //                       /  |  \
    //                     L24*Q05*L25
    //                       /  |  \
    //                      L22 |*L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L23.backIt());
    pertinentLeaves.push_back(L25.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________  L21 _______*Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02*L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L20.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01 L14  L15*Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03 *L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L16.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, false);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15*Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03 *L04*L05*L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                        *P04
    //                       /  |  \
    //                    *L24 Q05*L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______ Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, false);
    EXPECT_EQ(virtualRoot, pqTree.nil());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01 L14  L15*Q01 L16  L17*P02 L18  L19*Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02*L03 *L04*L05*L06 *L07*L08*L09 *L10*L11*L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L10.backIt());
    pertinentLeaves.push_back(L11.backIt());
    pertinentLeaves.push_back(L12.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, false);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L10.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L11.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);



    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________  L21 _______*Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01*L14 *L15 Q01*L16 *L17 P02*L18 *L19*Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02*L03 *L04*L05*L06 *L07*L08*L09 *L10*L11*L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L10.backIt());
    pertinentLeaves.push_back(L11.backIt());
    pertinentLeaves.push_back(L12.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L18.backIt());
    pertinentLeaves.push_back(L19.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L10.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L11.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L18.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);



    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________  L21 _______*Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13*P01*L14 *L15*Q01*L16 *L17*P02*L18 *L19*Q02*L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02*L03 *L04*L05*L06 *L07*L08*L09 *L10*L11*L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L10.backIt());
    pertinentLeaves.push_back(L11.backIt());
    pertinentLeaves.push_back(L12.backIt());
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L18.backIt());
    pertinentLeaves.push_back(L19.backIt());
    pertinentLeaves.push_back(L20.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L10.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L11.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L18.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L19.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   ________Q03_________ *L21 _______*Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14 *L15 Q01 L16 *L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L21.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, false);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 _______*Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13 P01 L14  L15 Q01 L16 *L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L21.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);

    //                        *P04
    //                       /  |  \
    //                    *L24*Q05 L25
    //                       /  |  \
    //                     *L22 | L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15*Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05*L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L21.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L24.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L24.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                        *P04
    //                       /  |  \
    //                    *L24*Q05*L25
    //                       /  |  \
    //                     *L22 |  L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());
    pertinentLeaves.push_back(L22.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                        *P04
    //                       /  |  \
    //                    *L24*Q05*L25
    //                       /  |  \
    //                     *L22 | *L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                        *P04
    //                       /  |  \
    //                    *L24*Q05*L25
    //                       /  |  \
    //                     *L22 | *L23
    //             ___________*P03__________
    //            /             |           \
    //   ________Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());
    pertinentLeaves.push_back(L21.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                        *P04
    //                       /  |  \
    //                    *L24*Q05*L25
    //                       /  |  \
    //                     *L22 | *L23
    //             ___________*P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 _______*Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13*P01*L14 *L15*Q01*L16 *L17 P02 L18  L19 Q02*L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02*L03 *L04*L05*L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    pqTree.initializeForOneIteration();
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L20.backIt());
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());
    pertinentLeaves.push_back(L21.backIt());

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);

}


/**  @brief tests BLPlanarizer::bubbleUp() edge case.
 */
TEST_F(BLPlanarityTesterTests, Test6) {

    BLTree pqTree;
    BLPlanarityTester tester;
    BLGraph G;
    // Create a simulated PQ-tree.
    //
    //                         L01

    auto& L01 = createL(pqTree, G);

    vector<node_list_it_t> pertinentLeaves;

    //                        *L01
    pqTree.initializeForOneIteration();

    pertinentLeaves.push_back(L01.backIt());

    node_list_it_t pertinentRoot;
    node_list_it_t virtualRoot;

    auto result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);

}

