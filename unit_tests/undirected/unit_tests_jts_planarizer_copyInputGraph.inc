/**  @brief tests JTSPlanarizer::copyInputGraph().
 */
TEST_F(JTSPlanarizerTests, Test1) {

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

    JTSGraph g_2;
    vector<node_list_it_t> stOrderDst;

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;

    copyInputGraph(planarizer, g_1,stOrderSrc, g_2, stOrderDst, pqTree);

    EXPECT_EQ(g_2.numNodes(), 10);
    EXPECT_EQ(g_2.numEdges(), 22);

    auto nPair = g_2.nodes();
    auto nIt   = nPair.first;

    auto& nc_01 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_02 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_03 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_04 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_05 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_06 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_07 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_08 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_09 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_10 = dynamic_cast<JTSGraphNode&>(*(*nIt++));

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

    auto ePair = g_2.edges();
    auto eIt   = ePair.first;

    auto& ec_01_02 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_01_04 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_01_06 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_01_07 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_01_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_02_03 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_02_06 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_02_08 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_02_09 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_03_04 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_03_09 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_04_05 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_04_08 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_05_06 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_05_08 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_05_09 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_06_07 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_06_09 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_07_09 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_07_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_08_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_09_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));


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

    EXPECT_EQ(ec_01_02.mRemoved, false);
    EXPECT_EQ(ec_01_04.mRemoved, false);
    EXPECT_EQ(ec_01_06.mRemoved, false);
    EXPECT_EQ(ec_01_07.mRemoved, false);
    EXPECT_EQ(ec_01_10.mRemoved, false);
    EXPECT_EQ(ec_02_03.mRemoved, false);
    EXPECT_EQ(ec_02_06.mRemoved, false);
    EXPECT_EQ(ec_02_08.mRemoved, false);
    EXPECT_EQ(ec_02_09.mRemoved, false);
    EXPECT_EQ(ec_03_04.mRemoved, false);
    EXPECT_EQ(ec_03_09.mRemoved, false);
    EXPECT_EQ(ec_04_05.mRemoved, false);
    EXPECT_EQ(ec_04_08.mRemoved, false);
    EXPECT_EQ(ec_05_06.mRemoved, false);
    EXPECT_EQ(ec_05_08.mRemoved, false);
    EXPECT_EQ(ec_05_09.mRemoved, false);
    EXPECT_EQ(ec_06_07.mRemoved, false);
    EXPECT_EQ(ec_06_09.mRemoved, false);
    EXPECT_EQ(ec_07_09.mRemoved, false);
    EXPECT_EQ(ec_07_10.mRemoved, false);
    EXPECT_EQ(ec_08_10.mRemoved, false);
    EXPECT_EQ(ec_09_10.mRemoved, false);

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


/**  @brief tests JTSTree::makeInitialAttachmentP();
 */
TEST_F(JTSPlanarizerTests, Test2) {

    JTSTree& pqTree = getPQTree();
    auto nIt =    makeInitialAttachmentP(pqTree);

    EXPECT_EQ(pqTree.numNodes(), 1);
    EXPECT_EQ(pqTree.numEdges(), 0);
    EXPECT_EQ(pqTree.nodes().first, nIt);
    JTSTreeNode& att = dynamic_cast<JTSTreeNode&>(*(*nIt));

    EXPECT_EQ(&(att.mTree), &pqTree);
    EXPECT_EQ(att.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(att.mChildren.size(), 0);
    EXPECT_EQ(att.mParent, pqTree.nil());

}


/**  @brief tests JTSTree::fanOutLeavesFromAttachment()
 */
TEST_F(JTSPlanarizerTests, Test3) {

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

    JTSGraph g_2;
    vector<node_list_it_t> stOrderDst;

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;

    copyInputGraph(planarizer, g_1,stOrderSrc, g_2, stOrderDst, pqTree);

    auto nPair = g_2.nodes();
    auto nIt   = nPair.first;

    auto& nc_01 = dynamic_cast<JTSGraphNode&>(*(*nIt++));

    auto nIt2 =    makeInitialAttachmentP(pqTree);
    auto& A  =    toNodeRef(pqTree, nIt2);

    fanOutLeavesFromAttachment(pqTree, A, nc_01.mOutgoingEdges);

    EXPECT_EQ(parent_check(A),true);
    EXPECT_EQ(cross_check(nc_01.mOutgoingEdges, A.mChildren),true);
    
}


/**  @brief tests JTSTree::fanOutLeavesFromAttachment()
 */
TEST_F(JTSPlanarizerTests, Test4) {

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

    JTSGraph g_2;
    vector<node_list_it_t> stOrderDst;

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;

    copyInputGraph(planarizer, g_1,stOrderSrc, g_2, stOrderDst, pqTree);

    auto nPair = g_2.nodes();
    auto nIt   = nPair.first;

    auto& nc_01 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_02 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_03 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_04 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_05 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_06 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_07 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_08 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_09 = dynamic_cast<JTSGraphNode&>(*(*nIt++));
    auto& nc_10 = dynamic_cast<JTSGraphNode&>(*(*nIt++));

    auto nIt2 =    makeInitialAttachmentP(pqTree);
    auto& A  =    toNodeRef(pqTree, nIt2);

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
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_01.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_01.mIncomingEdges, nc_01_pertinentLeaves),true);

    vector<node_list_it_t> nc_02_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_02.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_02.mIncomingEdges, nc_02_pertinentLeaves),true);

    vector<node_list_it_t> nc_03_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_03.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_03.mIncomingEdges, nc_03_pertinentLeaves),true);

    vector<node_list_it_t> nc_04_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_04.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_04.mIncomingEdges, nc_04_pertinentLeaves),true);

    vector<node_list_it_t> nc_05_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_05.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_05.mIncomingEdges, nc_05_pertinentLeaves),true);

    vector<node_list_it_t> nc_06_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_06.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_06.mIncomingEdges, nc_06_pertinentLeaves),true);

    vector<node_list_it_t> nc_07_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_07.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_07.mIncomingEdges, nc_07_pertinentLeaves),true);

    vector<node_list_it_t> nc_08_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_08.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_08.mIncomingEdges, nc_08_pertinentLeaves),true);

    vector<node_list_it_t> nc_09_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_09.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_09.mIncomingEdges, nc_09_pertinentLeaves),true);

    vector<node_list_it_t> nc_10_pertinentLeaves =
            fromGraphEdgesToPQLeaves(planarizer, pqTree, nc_10.mIncomingEdges);
    EXPECT_EQ(cross_check(nc_10.mIncomingEdges, nc_10_pertinentLeaves),true);

}

