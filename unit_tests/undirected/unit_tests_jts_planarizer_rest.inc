
/**  @brief tests JTSPlanarizer::appliyTemplates()
 */
TEST_F(JTSPlanarizerTests, Test37) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //                               P01
    //                              /   \
    //                            Q01  L1_5
    //                          /     \
    //                         P02     P03
    //                        /  \     /  \
    //                   *L2_4  L2_5 *L3_4 L3_5
    //
    //                               _||_
    //                               \  /
    //                                \/
    //
    //                            ____P01___
    //                           /          \
    //                  _______Q01_______  L1_5
    //                 |     |     |     |
    //               L2_5 *L2_4 *L3_4  L3_5
    // 
    initializeForOneIteration(pqTree);
    auto& L1_5 = createL(pqTree);
    auto& L2_4 = createL(pqTree);
    L2_4.mGeneration    = pqTree.mGeneration;
    auto& L2_5 = createL(pqTree);
    auto& L3_4 = createL(pqTree);
    L3_4.mGeneration    = pqTree.mGeneration;
    auto& L3_5 = createL(pqTree);

    list<node_list_it_t> P02children;
    P02children.push_back(L2_4.backIt());
    P02children.push_back(L2_5.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration    = pqTree.mGeneration;
    P02.mPertinentChildrenCount = 1;

    list<node_list_it_t> P03children;
    P03children.push_back(L3_4.backIt());
    P03children.push_back(L3_5.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration    = pqTree.mGeneration;
    P03.mPertinentChildrenCount = 1;

    list<node_list_it_t> Q01children;
    Q01children.push_back(P02.backIt());
    Q01children.push_back(P03.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 2;

    list<node_list_it_t> P01children;
    P01children.push_back(Q01.backIt());
    P01children.push_back(L1_5.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mPertinentChildrenCount = 0;

    vector<node_list_it_t> pertinentLeaves;
    pertinentLeaves.push_back(L2_4.backIt());
    pertinentLeaves.push_back(L3_4.backIt());

    auto root = applyTemplates(
                         planarizer, pqTree, pertinentLeaves, Q01.backIt());
    EXPECT_EQ(root, Q01.backIt());
    EXPECT_EQ(P01.mChildren.size(),2);
    EXPECT_LT(P01.mGeneration,pqTree.mGeneration);
    auto cIt = P01.mChildren.begin();
    EXPECT_EQ(Q01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L1_5.backIt(), *cIt);
    EXPECT_EQ(Q01.mChildren.size(),4);
    EXPECT_EQ(Q01.mGeneration,pqTree.mGeneration);
    cIt = Q01.mChildren.begin();
    EXPECT_EQ(L2_5.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L2_4.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L3_4.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L3_5.backIt(), *cIt);

}


/**  @brief tests JTSPlanarizer::appliyTemplates()
 */
TEST_F(JTSPlanarizerTests, Test38) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    // 
    //              _P01_
    //             /     \
    //        ___Q01___  L1_5
    //       |    |    |
    //     L2_5 L4_5 L3_5
    //
    //             _||_
    //             \  /
    //              \/
    //
    //              _P01_
    //             /     \
    //        ___Q01___  L1_5
    //       |    |    |
    //     L2_5 L4_5 L3_5
    //

    initializeForOneIteration(pqTree);
    auto& L2_5 = createL(pqTree);
    L2_5.mGeneration    = pqTree.mGeneration;
    auto& L4_5 = createL(pqTree);
    L4_5.mGeneration    = pqTree.mGeneration;
    auto& L3_5 = createL(pqTree);
    L3_5.mGeneration    = pqTree.mGeneration;
    auto& L1_5 = createL(pqTree);
    L1_5.mGeneration    = pqTree.mGeneration;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L2_5.backIt());
    Q01children.push_back(L4_5.backIt());
    Q01children.push_back(L3_5.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 3;

    list<node_list_it_t> P01children;
    P01children.push_back(Q01.backIt());
    P01children.push_back(L1_5.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration    = pqTree.mGeneration;
    P01.mPertinentChildrenCount = 2;

    vector<node_list_it_t> pertinentLeaves;
    pertinentLeaves.push_back(L2_5.backIt());
    pertinentLeaves.push_back(L4_5.backIt());
    pertinentLeaves.push_back(L3_5.backIt());
    pertinentLeaves.push_back(L1_5.backIt());

    auto root = applyTemplates(
                    planarizer, pqTree, pertinentLeaves, P01.backIt());
    EXPECT_EQ(root, P01.backIt());
    auto cIt = P01.mChildren.begin();
    EXPECT_EQ(Q01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L1_5.backIt(), *cIt);
    EXPECT_EQ(P01.mPertinentChildrenCount,2);
    EXPECT_EQ(P01.mFullChildrenCount,2);

    cIt = Q01.mChildren.begin();
    EXPECT_EQ(L2_5.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L4_5.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L3_5.backIt(), *cIt);
    EXPECT_EQ(Q01.mPertinentChildrenCount,3);
    EXPECT_EQ(Q01.mFullChildrenCount,3);
    EXPECT_EQ(Q01.mPertinentType, JTSTreeNode::Full);

}




/**  @brief tests JTSPlanarizer::appliyTemplates()
 */
TEST_F(JTSPlanarizerTests, Test39) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_
    //   /  |  \
    // L01*L02 L03
    //

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    vector<node_list_it_t> pertinentLeaves;
    pertinentLeaves.push_back(L02.backIt());

    auto root = applyTemplates(
                    planarizer, pqTree, pertinentLeaves, L02.backIt());
    EXPECT_EQ(root, L02.backIt());
    EXPECT_EQ(L02.mPertinentType, JTSTreeNode::Full);


}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test40) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_
    //   /  |  \
    // L01*L02 L03
    //

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, L02.backIt());
    EXPECT_EQ(L02.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(att, L02.backIt());

}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test41) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_
    //   /  |  \
    // L01*P02 L02
    //   /  |  \
    //*L03*L04*L05

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P02children;
    P02children.push_back(L03.backIt());
    P02children.push_back(L04.backIt());
    P02children.push_back(L05.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration    = pqTree.mGeneration;
    P02.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(P02.backIt());
    P01children.push_back(L02.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, P02.backIt());
    EXPECT_EQ(P02.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(att, P02.backIt());
    EXPECT_EQ(P02.mChildren.size(), 0);

}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test42) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_
    //   /  |  \
    // L01*Q02 L02
    //   /  |  \
    //*L03*L04*L05

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P02children;
    P02children.push_back(L03.backIt());
    P02children.push_back(L04.backIt());
    P02children.push_back(L05.backIt());
    auto& P02 = createQ(pqTree, P02children);
    P02.mGeneration    = pqTree.mGeneration;
    P02.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(P02.backIt());
    P01children.push_back(L02.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, P02.backIt());
    EXPECT_EQ(P02.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(att, P02.backIt());
    EXPECT_EQ(P02.mChildren.size(), 0);

}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test43) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_____
    //   /  |      \
    // L01*Q01____  L02
    //    / | \   \
    //   /  |  \   \
    // L03 L04*L05*L06 

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(Q01.backIt());
    P01children.push_back(L02.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, Q01.backIt());
    EXPECT_EQ(Q01.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(Q01.mChildren.size(), 3);
    auto cIt = Q01.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(att, *cIt);
    auto& A = pqTree.toNodeRef(att);
    EXPECT_EQ(A.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(A.mChildren.size(),  0);

}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test44) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_____
    //   /  |      \
    // L01*Q01____  L02
    //    / | \   \
    //   /  |  \   \
    //*L03*L04 L05 L06 

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(Q01.backIt());
    P01children.push_back(L02.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, Q01.backIt());
    EXPECT_EQ(Q01.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(Q01.mChildren.size(), 3);
    auto cIt = Q01.mChildren.begin();
    EXPECT_EQ(att, *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);

    auto& A = pqTree.toNodeRef(att);
    EXPECT_EQ(A.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(A.mChildren.size(),  0);

}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test45) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_____
    //   /  |      \
    // L01*Q01____  L02
    //    / | \   \
    //   /  |  \   \
    // L03*L04*L05 L06 

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(Q01.backIt());
    P01children.push_back(L02.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, Q01.backIt());
    EXPECT_EQ(Q01.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(Q01.mChildren.size(), 3);
    auto cIt = Q01.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(att, *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);

    auto& A = pqTree.toNodeRef(att);
    EXPECT_EQ(A.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(A.mChildren.size(),  0);

}


/**  @brief tests JTSPlanarizer::removePertinentTree()
 */
TEST_F(JTSPlanarizerTests, Test46) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_____
    //   /  |      \
    // L01*Q01____  L02
    //    / | \   \
    //   /  |  \   \
    // L03*L04*P02*L05 
    //        / | \
    //    *L06*L07*L08   
    //
    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L06.backIt());
    P02children.push_back(L08.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration    = pqTree.mGeneration;
    P02.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(P02.backIt());
    Q01children.push_back(L05.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(Q01.backIt());
    P01children.push_back(L02.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto att = removePertinentTree(planarizer, pqTree, Q01.backIt());
    EXPECT_EQ(Q01.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(Q01.mChildren.size(), 2);
    auto cIt = Q01.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(att, *cIt);

    auto& A = pqTree.toNodeRef(att);
    EXPECT_EQ(A.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(A.mChildren.size(),  0);

}


/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges()
 */
TEST_F(JTSPlanarizerTests, Test47) {

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

/*
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
*/

    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    makePlanarSubgraphAndRemovedEdges(
                             planarizer, g_1, g_2, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 10);

    EXPECT_EQ(planarSubgraph.numEdges(), 22);
    EXPECT_EQ(removedEdges.size(), 0);

    auto nPair = planarSubgraph.nodes();
    auto nIt   = nPair.first;

    auto& np_01 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_02 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_03 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_04 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_05 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_06 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_07 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_08 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_09 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_10 = dynamic_cast<Node&>(*(*nIt++));
    EXPECT_EQ(np_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(np_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(np_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(np_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(np_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(np_06.IGBackwardLink(), n_06.backIt());
    EXPECT_EQ(np_07.IGBackwardLink(), n_07.backIt());
    EXPECT_EQ(np_08.IGBackwardLink(), n_08.backIt());
    EXPECT_EQ(np_09.IGBackwardLink(), n_09.backIt());
    EXPECT_EQ(np_10.IGBackwardLink(), n_10.backIt());

    auto ePair = planarSubgraph.edges();
    auto eIt   = ePair.first;

    auto& ep_01_02 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_01_04 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_01_06 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_01_07 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_01_10 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_03 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_06 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_08 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_03_04 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_03_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_04_05 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_04_08 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_05_06 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_05_08 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_05_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_06_07 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_06_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_07_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_07_10 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_08_10 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_09_10 = dynamic_cast<Edge&>(*(*eIt++));

    EXPECT_EQ(ep_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(ep_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(ep_01_06.IGBackwardLink(), e_01_06.backIt());
    EXPECT_EQ(ep_01_07.IGBackwardLink(), e_01_07.backIt());
    EXPECT_EQ(ep_01_10.IGBackwardLink(), e_01_10.backIt());
    EXPECT_EQ(ep_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(ep_02_06.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(ep_02_08.IGBackwardLink(), e_02_08.backIt());
    EXPECT_EQ(ep_02_09.IGBackwardLink(), e_02_09.backIt());
    EXPECT_EQ(ep_03_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(ep_03_09.IGBackwardLink(), e_03_09.backIt());
    EXPECT_EQ(ep_04_05.IGBackwardLink(), e_04_05.backIt());
    EXPECT_EQ(ep_04_08.IGBackwardLink(), e_04_08.backIt());
    EXPECT_EQ(ep_05_06.IGBackwardLink(), e_05_06.backIt());
    EXPECT_EQ(ep_05_08.IGBackwardLink(), e_05_08.backIt());
    EXPECT_EQ(ep_05_09.IGBackwardLink(), e_05_09.backIt());
    EXPECT_EQ(ep_06_07.IGBackwardLink(), e_06_07.backIt());
    EXPECT_EQ(ep_06_09.IGBackwardLink(), e_06_09.backIt());
    EXPECT_EQ(ep_07_09.IGBackwardLink(), e_07_09.backIt());
    EXPECT_EQ(ep_07_10.IGBackwardLink(), e_07_10.backIt());
    EXPECT_EQ(ep_08_10.IGBackwardLink(), e_08_10.backIt());
    EXPECT_EQ(ep_09_10.IGBackwardLink(), e_09_10.backIt());

    EXPECT_EQ(e_01_02.IGForwardLink(), ep_01_02.backIt());
    EXPECT_EQ(e_01_04.IGForwardLink(), ep_01_04.backIt());
    EXPECT_EQ(e_01_06.IGForwardLink(), ep_01_06.backIt());
    EXPECT_EQ(e_01_07.IGForwardLink(), ep_01_07.backIt());
    EXPECT_EQ(e_01_10.IGForwardLink(), ep_01_10.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(), ep_02_03.backIt());
    EXPECT_EQ(e_02_06.IGForwardLink(), ep_02_06.backIt());
    EXPECT_EQ(e_02_08.IGForwardLink(), ep_02_08.backIt());
    EXPECT_EQ(e_02_09.IGForwardLink(), ep_02_09.backIt());
    EXPECT_EQ(e_03_04.IGForwardLink(), ep_03_04.backIt());
    EXPECT_EQ(e_03_09.IGForwardLink(), ep_03_09.backIt());
    EXPECT_EQ(e_04_05.IGForwardLink(), ep_04_05.backIt());
    EXPECT_EQ(e_04_08.IGForwardLink(), ep_04_08.backIt());
    EXPECT_EQ(e_05_06.IGForwardLink(), ep_05_06.backIt());
    EXPECT_EQ(e_05_08.IGForwardLink(), ep_05_08.backIt());
    EXPECT_EQ(e_05_09.IGForwardLink(), ep_05_09.backIt());
    EXPECT_EQ(e_06_07.IGForwardLink(), ep_06_07.backIt());
    EXPECT_EQ(e_06_09.IGForwardLink(), ep_06_09.backIt());
    EXPECT_EQ(e_07_09.IGForwardLink(), ep_07_09.backIt());
    EXPECT_EQ(e_07_10.IGForwardLink(), ep_07_10.backIt());
    EXPECT_EQ(e_08_10.IGForwardLink(), ep_08_10.backIt());
    EXPECT_EQ(e_09_10.IGForwardLink(), ep_09_10.backIt());

    EXPECT_EQ(e_01_02.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_01_04.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_01_06.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_01_07.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_01_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_03.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_06.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_08.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_03_04.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_03_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_04_05.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_04_08.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_05_06.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_05_08.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_05_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_06_07.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_06_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_07_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_07_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_08_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_09_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);


}



/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges()
 */
TEST_F(JTSPlanarizerTests, Test48) {

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


    auto nPair = g_2.nodes();
    auto nIt   = nPair.first;

/*
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
*/

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

    ec_01_02.mRemoved = true;
    ec_01_04.mRemoved = false;
    ec_01_06.mRemoved = false;
    ec_01_07.mRemoved = false;
    ec_01_10.mRemoved = true;
    ec_02_03.mRemoved = false;
    ec_02_06.mRemoved = false;
    ec_02_08.mRemoved = true;
    ec_02_09.mRemoved = false;
    ec_03_04.mRemoved = true;
    ec_03_09.mRemoved = false;
    ec_04_05.mRemoved = true;
    ec_04_08.mRemoved = true;
    ec_05_06.mRemoved = true;
    ec_05_08.mRemoved = false;
    ec_05_09.mRemoved = false;
    ec_06_07.mRemoved = false;
    ec_06_09.mRemoved = true;
    ec_07_09.mRemoved = true;
    ec_07_10.mRemoved = false;
    ec_08_10.mRemoved = false;
    ec_09_10.mRemoved = false;


    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    makePlanarSubgraphAndRemovedEdges(
                             planarizer, g_1, g_2, planarSubgraph, removedEdges);


    EXPECT_EQ(planarSubgraph.numNodes(), 10);

    EXPECT_EQ(planarSubgraph.numEdges(), 13);
    EXPECT_EQ(removedEdges.size(), 9);

    nPair = planarSubgraph.nodes();
    nIt   = nPair.first;

    auto& np_01 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_02 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_03 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_04 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_05 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_06 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_07 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_08 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_09 = dynamic_cast<Node&>(*(*nIt++));
    auto& np_10 = dynamic_cast<Node&>(*(*nIt++));

    EXPECT_EQ(np_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(np_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(np_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(np_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(np_05.IGBackwardLink(), n_05.backIt());
    EXPECT_EQ(np_06.IGBackwardLink(), n_06.backIt());
    EXPECT_EQ(np_07.IGBackwardLink(), n_07.backIt());
    EXPECT_EQ(np_08.IGBackwardLink(), n_08.backIt());
    EXPECT_EQ(np_09.IGBackwardLink(), n_09.backIt());
    EXPECT_EQ(np_10.IGBackwardLink(), n_10.backIt());

    ePair = planarSubgraph.edges();
    eIt   = ePair.first;

    auto& ep_01_04 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_01_06 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_01_07 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_03 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_06 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_02_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_03_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_05_08 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_05_09 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_06_07 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_07_10 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_08_10 = dynamic_cast<Edge&>(*(*eIt++));
    auto& ep_09_10 = dynamic_cast<Edge&>(*(*eIt++));

    EXPECT_EQ(ep_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(ep_01_06.IGBackwardLink(), e_01_06.backIt());
    EXPECT_EQ(ep_01_07.IGBackwardLink(), e_01_07.backIt());
    EXPECT_EQ(ep_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(ep_02_06.IGBackwardLink(), e_02_06.backIt());
    EXPECT_EQ(ep_02_09.IGBackwardLink(), e_02_09.backIt());
    EXPECT_EQ(ep_03_09.IGBackwardLink(), e_03_09.backIt());
    EXPECT_EQ(ep_05_08.IGBackwardLink(), e_05_08.backIt());
    EXPECT_EQ(ep_05_09.IGBackwardLink(), e_05_09.backIt());
    EXPECT_EQ(ep_06_07.IGBackwardLink(), e_06_07.backIt());
    EXPECT_EQ(ep_07_10.IGBackwardLink(), e_07_10.backIt());
    EXPECT_EQ(ep_08_10.IGBackwardLink(), e_08_10.backIt());
    EXPECT_EQ(ep_09_10.IGBackwardLink(), e_09_10.backIt());

    EXPECT_EQ(e_01_04.IGForwardLink(), ep_01_04.backIt());
    EXPECT_EQ(e_01_06.IGForwardLink(), ep_01_06.backIt());
    EXPECT_EQ(e_01_07.IGForwardLink(), ep_01_07.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(), ep_02_03.backIt());
    EXPECT_EQ(e_02_06.IGForwardLink(), ep_02_06.backIt());
    EXPECT_EQ(e_02_09.IGForwardLink(), ep_02_09.backIt());
    EXPECT_EQ(e_03_09.IGForwardLink(), ep_03_09.backIt());
    EXPECT_EQ(e_05_08.IGForwardLink(), ep_05_08.backIt());
    EXPECT_EQ(e_05_09.IGForwardLink(), ep_05_09.backIt());
    EXPECT_EQ(e_06_07.IGForwardLink(), ep_06_07.backIt());
    EXPECT_EQ(e_07_10.IGForwardLink(), ep_07_10.backIt());
    EXPECT_EQ(e_08_10.IGForwardLink(), ep_08_10.backIt());
    EXPECT_EQ(e_09_10.IGForwardLink(), ep_09_10.backIt());

    EXPECT_EQ(e_01_04.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_01_06.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_01_07.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_03.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_06.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_02_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_03_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_05_08.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_05_09.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_06_07.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_07_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_08_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    EXPECT_EQ(e_09_10.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);

    auto rit = removedEdges.begin();

    auto& er_01_02 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_01_10 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_02_08 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_03_04 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_04_05 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_04_08 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_05_06 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_06_09 = dynamic_cast<Edge&>(*(*(*rit++)));
    auto& er_07_09 = dynamic_cast<Edge&>(*(*(*rit++)));

    EXPECT_EQ(er_01_02.backIt(), e_01_02.backIt());
    EXPECT_EQ(er_01_10.backIt(), e_01_10.backIt());
    EXPECT_EQ(er_02_08.backIt(), e_02_08.backIt());
    EXPECT_EQ(er_03_04.backIt(), e_03_04.backIt());
    EXPECT_EQ(er_04_05.backIt(), e_04_05.backIt());
    EXPECT_EQ(er_04_08.backIt(), e_04_08.backIt());
    EXPECT_EQ(er_05_06.backIt(), e_05_06.backIt());
    EXPECT_EQ(er_06_09.backIt(), e_06_09.backIt());
    EXPECT_EQ(er_07_09.backIt(), e_07_09.backIt());

    EXPECT_EQ(e_01_02.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_01_10.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_02_08.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_03_04.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_04_05.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_04_08.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_05_06.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_06_09.IGForwardLink(), planarSubgraph.edges().second);
    EXPECT_EQ(e_07_09.IGForwardLink(), planarSubgraph.edges().second);

    EXPECT_EQ(e_01_02.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_01_10.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_02_08.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_03_04.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_04_05.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_04_08.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_05_06.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_06_09.utility(), JTSPlanarizer::kForwardTypeRemoved);
    EXPECT_EQ(e_07_09.utility(), JTSPlanarizer::kForwardTypeRemoved);

}

/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges() in steps for planar graph.
 */
TEST_F(JTSPlanarizerTests, Test49) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());
    auto& n_05 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03,n_05);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());

    JTSPlanarizer  planarizer;
    JTSTree&       pqTree = getPQTree();

    JTSGraph                graphCopy;
    vector<node_list_it_t>  stOrderCopy;

    copyInputGraph(planarizer, g_1, stOrder, graphCopy, stOrderCopy, pqTree);

    node_list_it_t attachmentNode = pqTree.makeInitialAttachmentP();

    size_t index = 0;

    auto nItIt =  stOrderCopy.begin();
    auto& N1 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pqTree.fanOutLeavesFromAttachment(
                          pqTree.toNodeRef(attachmentNode), N1.mOutgoingEdges);
                          
    index++;
    nItIt++;

    auto& N2 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    node_list_it_t pertinentRoot =
                                 bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    vector<node_list_it_t> newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    pertinentRoot = 
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N2.mOutgoingEdges);
                          
    index++;
    nItIt++;

    auto& N3 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);
                          
    pertinentRoot = 
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);


    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N3.mOutgoingEdges);

    index++;
    nItIt++;

    auto& N4 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N4.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N4.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N4.mOutgoingEdges);

    index++;
    nItIt++;

    auto& N5 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N5.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N5.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    EXPECT_EQ(pqTree.numNodes(), 1);

    auto eCopyPair = graphCopy.edges();
    size_t tmp_index = 0;
    for (auto eCopyIt = eCopyPair.first; eCopyIt != eCopyPair.second;
                                                      eCopyIt++, tmp_index++) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eCopyIt));
        EXPECT_EQ(E.mRemoved, false);
    }

    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    makePlanarSubgraphAndRemovedEdges(
                          planarizer, g_1, graphCopy, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 5);
    EXPECT_EQ(planarSubgraph.numEdges(), 9);
    EXPECT_EQ(removedEdges.size(), 0);

}


/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges() in steps
 *          for a non-planar graph.
 */

TEST_F(JTSPlanarizerTests, Test50) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());
    auto& n_05 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03,n_05);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());

    JTSPlanarizer  planarizer;
    JTSTree&       pqTree = getPQTree();

    JTSGraph                graphCopy;
    vector<node_list_it_t>  stOrderCopy;

    copyInputGraph(planarizer, g_1, stOrder, graphCopy, stOrderCopy, pqTree);

    //auto eitPair = graphCopy.edges();
    //for(auto eit = eitPair.first; eit != eitPair.second; eit++) {
    //    auto& E = dynamic_cast<JTSGraphEdge&>(*(*eit));
    //    cerr << "E: " << (unsigned long long)(&E) << "\n";
   // }


    node_list_it_t attachmentNode = pqTree.makeInitialAttachmentP();

    size_t index = 0;

    auto nItIt =  stOrderCopy.begin();
    auto& N1 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pqTree.fanOutLeavesFromAttachment(
                         pqTree.toNodeRef(attachmentNode), N1.mOutgoingEdges);
                          
    index++;
    nItIt++;

    auto& N2 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    node_list_it_t pertinentRoot =
                                 bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    vector<node_list_it_t> newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N2.mOutgoingEdges);
                          
    index++;
    nItIt++;

    auto& N3 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);
                          
    pertinentRoot = 
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);


    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N3.mOutgoingEdges);

    index++;
    nItIt++;

    auto& N4 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N4.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N4.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N4.mOutgoingEdges);

    index++;
    nItIt++;

    auto& N5 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N5.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N5.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);

    EXPECT_EQ(pqTree.numNodes(), 1);

    auto eCopyPair = graphCopy.edges();
    size_t numRemoved = 0;
    for (auto eCopyIt = eCopyPair.first; eCopyIt != eCopyPair.second;
                                                                   eCopyIt++) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eCopyIt));
        if (E.mRemoved) {
            //cerr << "Removed: " << (unsigned long long)(&E) << "\n";
            numRemoved++;
        }
    }
    EXPECT_EQ(numRemoved, 1);

    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    makePlanarSubgraphAndRemovedEdges(
                          planarizer, g_1, graphCopy, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 5);
    EXPECT_EQ(planarSubgraph.numEdges(), 9);
    EXPECT_EQ(removedEdges.size(), 1);

}



/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges() in steps for a planar graph.
 */

TEST_F(JTSPlanarizerTests, Test51) {

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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());
    stOrder.push_back(n_09.backIt());
    stOrder.push_back(n_10.backIt());

    JTSPlanarizer  planarizer;
    JTSTree&       pqTree = getPQTree();

    JTSGraph                graphCopy;
    vector<node_list_it_t>  stOrderCopy;

    copyInputGraph(planarizer, g_1, stOrder, graphCopy, stOrderCopy, pqTree);

//    auto eitPair = graphCopy.edges();
//    for(auto eit = eitPair.first; eit != eitPair.second; eit++) {
//        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eit));
//        cerr << "E: " << hex << (unsigned long long)(&E) << "\n";
//    }

    node_list_it_t attachmentNode = pqTree.makeInitialAttachmentP();
    //cerr << "\nInitial attachment\n";
    //pqTree.printTree(cerr);

    size_t index = 0;

    auto nItIt =  stOrderCopy.begin();
    auto& N1 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pqTree.fanOutLeavesFromAttachment(
                         pqTree.toNodeRef(attachmentNode), N1.mOutgoingEdges);
    //cerr << "\nFan out N1\n";
    //pqTree.printTree(cerr);
                          
    index++;
    nItIt++;

    auto& N2 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
              fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    node_list_it_t pertinentRoot =
                                 bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                          planarizer, pqTree, pertinentLeaves, pertinentRoot);

    vector<node_list_it_t> newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N2\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N2\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N2.mOutgoingEdges);
    //cerr << "\nFan out N2\n";
    //pqTree.printTree(cerr);
                          
    index++;
    nItIt++;

    auto& N3 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N3\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N3\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N3.mOutgoingEdges);
    //cerr << "\nFan out N3\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N4 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N4.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                          planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
              fromGraphEdgesToPQLeaves(planarizer, pqTree, N4.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N4\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N4\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N4.mOutgoingEdges);
    //cerr << "\nFan out N4\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N5 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N5.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N5.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N5\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N5\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N5.mOutgoingEdges);
    //cerr << "\nFan out N5\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N6 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N6.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N6.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N6\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N6\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N6.mOutgoingEdges);
    //cerr << "\nFan out N6\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N7 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N7.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N7.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N7\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N7\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N7.mOutgoingEdges);
    //cerr << "\nFan out N7\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N8 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N8.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);


    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N8.mIncomingEdges);

    //auto& TMP1 = pqTree.toNodeRef(pertinentRoot);
    //cerr << "Pertinent Root:" << hex << (unsigned long long)(&TMP1) << "\n";

    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);


    //cerr << "\nApply templates N8\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N8\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N8.mOutgoingEdges);
    //cerr << "\nFan out N8\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N9 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N9.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N9.mIncomingEdges);
                          
    pertinentRoot = 
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N9\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N9\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N9.mOutgoingEdges);
    //cerr << "\nFan out N9\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N10 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
              fromGraphEdgesToPQLeaves(planarizer, pqTree, N10.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    newPertinentLeaves =
              fromGraphEdgesToPQLeaves(planarizer, pqTree, N10.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N10\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N10\n";
    //pqTree.printTree(cerr);

    EXPECT_EQ(pqTree.numNodes(), 1);

    auto eCopyPair = graphCopy.edges();
    size_t numRemoved = 0;
    for (auto eCopyIt = eCopyPair.first; eCopyIt != eCopyPair.second; 
                                                                   eCopyIt++) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eCopyIt));
        if (E.mRemoved) {
            cerr << "Removed: " << (unsigned long long)(&E) << "\n";
            numRemoved++;
        }
    }
    EXPECT_EQ(numRemoved, 0);

    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    makePlanarSubgraphAndRemovedEdges(
                          planarizer, g_1, graphCopy, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 10);
    EXPECT_EQ(planarSubgraph.numEdges(), 19);
    EXPECT_EQ(removedEdges.size(),  0);

}


/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges() in steps
 *          for a non-planar graph.
 */
TEST_F(JTSPlanarizerTests, Test52) {

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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());
    stOrder.push_back(n_09.backIt());
    stOrder.push_back(n_10.backIt());

    JTSPlanarizer  planarizer;
    JTSTree&       pqTree = getPQTree();

    JTSGraph                graphCopy;
    vector<node_list_it_t>  stOrderCopy;

    copyInputGraph(planarizer, g_1, stOrder, graphCopy, stOrderCopy, pqTree);

//    auto eitPair = graphCopy.edges();
//    for(auto eit = eitPair.first; eit != eitPair.second; eit++) {
//        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eit));
//        cerr << "E: " << hex << (unsigned long long)(&E) << "\n";
//    }

//    auto nitPair = graphCopy.nodes();
//    for(auto nit = nitPair.first; nit != nitPair.second; nit++) {
//        auto& N = dynamic_cast<JTSGraphNode&>(*(*nit));
//        cerr << "N: " << hex << (unsigned long long)(&N) << "\n";
//    }

    node_list_it_t attachmentNode = pqTree.makeInitialAttachmentP();
    //cerr << "\nInitial attachment\n";
    //pqTree.printTree(cerr);

    size_t index = 0;

    auto nItIt =  stOrderCopy.begin();
    auto& N1 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pqTree.fanOutLeavesFromAttachment(
                         pqTree.toNodeRef(attachmentNode), N1.mOutgoingEdges);
    //cerr << "\nFan out N1\n";
    //pqTree.printTree(cerr);
                          
    index++;
    nItIt++;

    auto& N2 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
              fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    node_list_it_t pertinentRoot =
                                bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N2\n";
    //pqTree.printTree(cerr);

    vector<node_list_it_t> newPertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N2.mIncomingEdges);

    pertinentRoot = 
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N2\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N2\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N2.mOutgoingEdges);
    //cerr << "\nFan out N2\n";
    //pqTree.printTree(cerr);
                          
    index++;
    nItIt++;

    auto& N3 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N3\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = 
               fromGraphEdgesToPQLeaves(planarizer, pqTree, N3.mIncomingEdges);
                          
    pertinentRoot =
         applyTemplates(planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N3\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N3\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N3.mOutgoingEdges);
    //cerr << "\nFan out N3\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N4 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N4.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N4\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N4.mIncomingEdges);
                          
    pertinentRoot = applyTemplates(
                        planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N4\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N4\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N4.mOutgoingEdges);
    //cerr << "\nFan out N4\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N5 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N5.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N5\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N5.mIncomingEdges);
                          
    pertinentRoot = applyTemplates(
                        planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N5\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N5\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N5.mOutgoingEdges);
    //cerr << "\nFan out N5\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N6 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N6.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N6\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N6.mIncomingEdges);
                          
    pertinentRoot = applyTemplates(
                        planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N6\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N6\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N6.mOutgoingEdges);
    //cerr << "\nFan out N6\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N7 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N7.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                          planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N7\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                       planarizer, pqTree, N7.mIncomingEdges);
                          
    pertinentRoot = applyTemplates(
                       planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N7\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N7\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                 pqTree, pqTree.toNodeRef(attachmentNode), N7.mOutgoingEdges);
    //cerr << "\nFan out N7\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N8 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N8.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);


    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N8\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N8.mIncomingEdges);

    //auto& TMP1 = pqTree.toNodeRef(pertinentRoot);
    //cerr << "Pertinent Root:" << hex << (unsigned long long)(&TMP1) << "\n";

    pertinentRoot = applyTemplates(
                        planarizer, pqTree, newPertinentLeaves, pertinentRoot);


    //cerr << "\nApply templates N8\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N8\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N8.mOutgoingEdges);
    //cerr << "\nFan out N8\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N9 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N9.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                           planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N9\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                        planarizer, pqTree, N9.mIncomingEdges);

    pertinentRoot = applyTemplates(
                        planarizer, pqTree, newPertinentLeaves, pertinentRoot);

    //cerr << "\nApply templates N9\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N9\n";
    //pqTree.printTree(cerr);

    fanOutLeavesFromAttachment(
                  pqTree, pqTree.toNodeRef(attachmentNode), N9.mOutgoingEdges);
    //cerr << "\nFan out N9\n";
    //pqTree.printTree(cerr);

    index++;
    nItIt++;

    auto& N10 = dynamic_cast<JTSGraphNode&>(*(**(nItIt)));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(
                                      planarizer, pqTree, N10.mIncomingEdges);

    pertinentRoot = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, pertinentRoot);

    determineWHABCNodeTypes(planarizer, pqTree, pertinentRoot);

    removeNonconsecutiveNodes(
                          planarizer, pqTree, pertinentLeaves, pertinentRoot);

    //cerr << "\nRemove Nonconsecutive N10\n";
    //pqTree.printTree(cerr);

    newPertinentLeaves = fromGraphEdgesToPQLeaves(
                                       planarizer, pqTree, N10.mIncomingEdges);
                          
    pertinentRoot = applyTemplates(
                        planarizer, pqTree, newPertinentLeaves, pertinentRoot);
    //cerr << "\nApply templates N10\n";
    //pqTree.printTree(cerr);

    attachmentNode = removePertinentTree(planarizer, pqTree, pertinentRoot);
    //cerr << "\nRemove subtree N10\n";
    //pqTree.printTree(cerr);

    EXPECT_EQ(pqTree.numNodes(), 1);

    auto eCopyPair = graphCopy.edges();
    size_t numRemoved = 0;
    for (auto eCopyIt = eCopyPair.first; eCopyIt != eCopyPair.second;
                                                                 eCopyIt++) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eCopyIt));
        if (E.mRemoved) {
            //cerr << "Removed: " << (unsigned long long)(&E) << "\n";
            numRemoved++;
        }
    }
    EXPECT_EQ(numRemoved, 2);

    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    makePlanarSubgraphAndRemovedEdges(
                          planarizer, g_1, graphCopy, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 10);
    EXPECT_EQ(planarSubgraph.numEdges(), 20);
    EXPECT_EQ(removedEdges.size(),  2);

}


/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges()
 *          for a non-planar graph.
 */
TEST_F(JTSPlanarizerTests, Test53) {

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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());
    stOrder.push_back(n_09.backIt());
    stOrder.push_back(n_10.backIt());

    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    planarizer.findUpwardPlanarSingleSourceSubgraph(
                                 g_1, stOrder, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 10);
    EXPECT_EQ(planarSubgraph.numEdges(), 20);
    EXPECT_EQ(removedEdges.size(),  2);

}


/**  @brief tests JTSPlanarizer::makePlanarSubgraphAndRemovedEdges() for K8
 */
TEST_F(JTSPlanarizerTests, Test54) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());
    auto& n_05 = g_1.addNode(make_unique<Node>());
    auto& n_06 = g_1.addNode(make_unique<Node>());
    auto& n_07 = g_1.addNode(make_unique<Node>());
    auto& n_08 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    /*auto& e_01_08 = */g_1.addEdge(make_unique<Edge>(),n_01,n_08);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    /*auto& e_02_06 = */g_1.addEdge(make_unique<Edge>(),n_02,n_06);
    /*auto& e_02_07 = */g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03,n_05);
    /*auto& e_03_06 = */g_1.addEdge(make_unique<Edge>(),n_03,n_06);
    /*auto& e_03_07 = */g_1.addEdge(make_unique<Edge>(),n_03,n_07);
    /*auto& e_03_08 = */g_1.addEdge(make_unique<Edge>(),n_03,n_08);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_04_06 = */g_1.addEdge(make_unique<Edge>(),n_04,n_06);
    /*auto& e_04_07 = */g_1.addEdge(make_unique<Edge>(),n_04,n_07);
    /*auto& e_04_08 = */g_1.addEdge(make_unique<Edge>(),n_04,n_08);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(),n_05,n_07);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(),n_06,n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(),n_07,n_08);


    //auto eitPair = g_1.edges();
    //for (auto eit = eitPair.first; eit != eitPair.second; eit++){
    //    auto& E = dynamic_cast<Edge&>(*(*eit));
    //    cerr << "E:" << (unsigned long long)(&E) << "\n";
    //}
    //cerr << "\n\n";

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());

    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;

    planarizer.findUpwardPlanarSingleSourceSubgraph(
                              g_1, stOrder, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 8);
    EXPECT_EQ(planarSubgraph.numEdges(), 18);
    EXPECT_EQ(removedEdges.size(),  10);
    //eitPair = removedEdges.edges();
    //for (auto eit = eitPair.first; eit != eitPair.second; eit++){
    //    auto& E1 = dynamic_cast<Edge&>(*(*eit));
    //    auto& E2 = dynamic_cast<Edge&>(*(*(E1.mOriginal)));
    //    cerr << "E2:" << (unsigned long long)(&E2) << "\n";
    //}

}
