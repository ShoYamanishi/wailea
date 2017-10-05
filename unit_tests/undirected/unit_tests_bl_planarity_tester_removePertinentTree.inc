
/**  @brief tests BLPlanarityTester::removePertinentTree(): L
 */
TEST_F(BLPlanarityTesterTests, Test97) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;


    //       P01
    //    ____|____
    //   /    |    \
    //  L01 *L02  L03

    pqTree.initializeForOneIteration();


    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L03 = createL(pqTree, G);

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    /*auto& P01 = */createP(pqTree, P01children);

    auto pertinentRoot = L02.backIt();
    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    EXPECT_EQ(attIt, L02.backIt());
    EXPECT_EQ(L02.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(L02.mChildren.size(), 0);


}


/**  @brief tests BLPlanarityTester::removePertinentTree(): P
 */
TEST_F(BLPlanarityTesterTests, Test98) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    //       P02
    //   /    |    \
    //  L04 *P01  L05
    //    ____|____
    //   /    |    \
    // *L01 *L02 *L03

    pqTree.initializeForOneIteration();


    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration = pqTree.mGeneration;
    P01.mPertinentType = BLTreeNode::Full;
    P01.mPertinentChildrenCount = 3;

    createFullLink(L01.backIt(), P01.backIt());
    createFullLink(L02.backIt(), P01.backIt());
    createFullLink(L03.backIt(), P01.backIt());

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);

    list<node_list_it_t> P02children;
    P02children.push_back(L04.backIt());
    P02children.push_back(P01.backIt());
    P02children.push_back(L05.backIt());
    /*auto& P02 = */createP(pqTree, P02children);

    auto pertinentRoot = P01.backIt();

    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    EXPECT_EQ(attIt, P01.backIt());
    EXPECT_EQ(P01.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P01.mChildren.size(), 0);
    EXPECT_EQ(pqTree.numNodes(), 4);


}


/**  @brief tests BLPlanarityTester::removePertinentTree(): P
 */
TEST_F(BLPlanarityTesterTests, Test99) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    //     __Q14___
    //    |   |    |
    //  L15 *P01  L16
    //    ____|____
    //   /    |    \
    // *L02 *P03 *L04
    //      / | \
    //   *L05*L06*Q07
    //            /|\
    //       *P08*L09*L10
    //       / | \
    //   *L11*L12*L13


    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::Full;
    Q07.mPertinentChildrenCount = 3;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::Full;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());
    createFullLink(Q07.backIt(), P03.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P01children;
    P01children.push_back(L02.backIt());
    P01children.push_back(P03.backIt());
    P01children.push_back(L04.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration = pqTree.mGeneration;
    P01.mPertinentType = BLTreeNode::Full;
    P01.mPertinentChildrenCount = 3;

    createFullLink(L02.backIt(), P01.backIt());
    createFullLink(P03.backIt(), P01.backIt());
    createFullLink(L04.backIt(), P01.backIt());

    auto& L15 = createL(pqTree, G);
    L15.mGeneration = pqTree.mGeneration;
    L15.mPertinentType = BLTreeNode::Full;

    auto& L16 = createL(pqTree, G);
    L16.mGeneration = pqTree.mGeneration;
    L16.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(P01.backIt());
    Q14children.push_back(L16.backIt());
    /*auto& Q14 = */createP(pqTree, P01children);

    auto pertinentRoot = P01.backIt();

    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    EXPECT_EQ(attIt, P01.backIt());
    EXPECT_EQ(P01.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P01.mChildren.size(), 0);
    EXPECT_EQ(pqTree.numNodes(), 4);

    EXPECT_EQ(tester.mOrientInNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_01.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_02.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_03.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_04.backIt());


}


/**  @brief tests BLPlanarityTester::removePertinentTree(): Q
 */
TEST_F(BLPlanarityTesterTests, Test100) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();


    //         ___Q19___
    //        |    |    |
    //      *Q01  L20  L21
    //    ____|_______________________
    //   |    |    |           |      |
    // *L02 *P03 *L04       _*Q14__ *L18
    //      / | \          |   |   |
    //   *L05*L06*Q07    *L15*L16*L17
    //            /|\
    //       *P08*L09*L10
    //       / | \
    //   *L11*L12*L13

    pqTree.initializeForOneIteration();

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::Full;
    Q07.mPertinentChildrenCount = 3;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::Full;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());
    createFullLink(Q07.backIt(), P03.backIt());

    auto& L15 = createL(pqTree, G);
    L15.mGeneration = pqTree.mGeneration;
    L15.mPertinentType = BLTreeNode::Full;
    auto& L16 = createL(pqTree, G);
    L16.mGeneration = pqTree.mGeneration;
    L16.mPertinentType = BLTreeNode::Full;
    auto& L17 = createL(pqTree, G);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = BLTreeNode::Full;
    Q14.mPertinentChildrenCount = 3;

    createFullLink(L15.backIt(), Q14.backIt());
    createFullLink(L16.backIt(), Q14.backIt());
    createFullLink(L17.backIt(), Q14.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);
    L18.mGeneration = pqTree.mGeneration;
    L18.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = BLTreeNode::Full;
    Q01.mPertinentChildrenCount = 5;

    createFullLink(L02.backIt(), Q01.backIt());
    createFullLink(P03.backIt(), Q01.backIt());
    createFullLink(L04.backIt(), Q01.backIt());
    createFullLink(Q14.backIt(), Q01.backIt());
    createFullLink(L18.backIt(), Q01.backIt());

    auto& L20 = createL(pqTree, G);
    L20.mGeneration = pqTree.mGeneration;
    L20.mPertinentType = BLTreeNode::Full;

    auto& L21 = createL(pqTree, G);
    L21.mGeneration = pqTree.mGeneration;
    L21.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q19children;
    Q19children.push_back(Q01.backIt());
    Q19children.push_back(L20.backIt());
    Q19children.push_back(L21.backIt());
    auto& Q19 = createQ(pqTree, Q19children);

    auto pertinentRoot = Q01.backIt();

    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    EXPECT_EQ(attIt, Q01.backIt());
    EXPECT_EQ(Q01.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(Q01.mChildren.size(), 0);
    EXPECT_EQ(pqTree.numNodes(), 4);
    EXPECT_EQ(Q19.mEndChild1, Q01.backIt());
    EXPECT_EQ(Q01.mParent, Q19.backIt());

    EXPECT_EQ(tester.mOrientInNorm.size(),2);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_05.backIt());
    EXPECT_EQ(*(tester.mOrientInNorm.rbegin()),track_01.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),2);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_06.backIt());
    EXPECT_EQ(*(tester.mOrientInReversed.rbegin()),track_02.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),2);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_07.backIt());
    EXPECT_EQ(*(tester.mOrientOutNorm.rbegin()),track_03.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),2);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_08.backIt());
    EXPECT_EQ(*(tester.mOrientOutReversed.rbegin()),track_04.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): Q
 */
TEST_F(BLPlanarityTesterTests, Test101) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //    __Q21___
    //   |   |    |
    //  L19 L20 *Q01
    //        ____|_______________________
    //       |    |    |           |      |
    //     *L02 *P03  L04       __Q14__  L18
    //          / | \          |   |   |
    //       *L05*L06*Q07     L15 L16 L17
    //                /|\
    //           *P08*L09*L10
    //           / | \
    //       *L11*L12*L13

    pqTree.initializeForOneIteration();

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::Full;
    Q07.mPertinentChildrenCount = 3;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::Full;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());
    createFullLink(Q07.backIt(), P03.backIt());

    auto& L15 = createL(pqTree, G);
    auto& L16 = createL(pqTree, G);
    auto& L17 = createL(pqTree, G);

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);


    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);

    auto& L18 = createL(pqTree, G);

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = BLTreeNode::SinglyPartial;
    Q01.mPertinentChildrenCount = 2;

    createFullLink(L02.backIt(), Q01.backIt());
    createFullLink(P03.backIt(), Q01.backIt());

    auto& L19 = createL(pqTree, G);
    auto& L20 = createL(pqTree, G);

    list<node_list_it_t> Q21children;
    Q21children.push_back(L19.backIt());
    Q21children.push_back(L20.backIt());
    Q21children.push_back(Q01.backIt());
    /*auto& Q21 = */createQ(pqTree, Q21children);

    auto pertinentRoot = Q01.backIt();
    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    auto &Att = pqTree.toNodeRef(attIt);
    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(Q01.mEndChild1, attIt);
    EXPECT_EQ(Att.mSibling1 == L04.backIt()||Att.mSibling2 == L04.backIt(),
                                                                       true);
    EXPECT_EQ(Att.mParent, Q01.backIt());

    EXPECT_EQ(pqTree.numNodes(), 11);

    EXPECT_EQ(tester.mOrientInNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_01.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_02.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_03.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_04.backIt());

}


/**  @brief tests BLPlanarityTester::removePertinentTree(): Q
 */
TEST_F(BLPlanarityTesterTests, Test102) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01
    //    ____|_______________________
    //   |    |    |           |      |
    //  L02  P03 *L04       _*Q14__ *L18
    //      / | \          |   |   |
    //    L05 L06 Q07    *L15*L16*L17
    //            /|\
    //        P08 L09 L10
    //       / | \
    //    L11 L12 L13

    pqTree.initializeForOneIteration();

    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);

    auto& L15 = createL(pqTree, G);
    L15.mGeneration = pqTree.mGeneration;
    L15.mPertinentType = BLTreeNode::Full;
    auto& L16 = createL(pqTree, G);
    L16.mGeneration = pqTree.mGeneration;
    L16.mPertinentType = BLTreeNode::Full;
    auto& L17 = createL(pqTree, G);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = BLTreeNode::Full;
    Q14.mPertinentChildrenCount = 3;

    createFullLink(L15.backIt(), Q14.backIt());
    createFullLink(L16.backIt(), Q14.backIt());
    createFullLink(L17.backIt(), Q14.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);
    L18.mGeneration = pqTree.mGeneration;
    L18.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = BLTreeNode::Full;
    Q01.mPertinentChildrenCount = 3;

    createFullLink(L04.backIt(), Q01.backIt());
    createFullLink(Q14.backIt(), Q01.backIt());
    createFullLink(L18.backIt(), Q01.backIt());

    auto pertinentRoot = Q01.backIt();
    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    auto &Att = pqTree.toNodeRef(attIt);
    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(Q01.mEndChild2, attIt);
    EXPECT_EQ(Att.mSibling1 == P03.backIt()||Att.mSibling2 == P03.backIt(),
                                                                       true);
    EXPECT_EQ(Att.mParent, Q01.backIt());
    EXPECT_EQ(pqTree.numNodes(), 13);

    EXPECT_EQ(tester.mOrientInNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_05.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_06.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_07.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_08.backIt());

}


/**  @brief tests BLPlanarityTester::removePertinentTree(): VR
 */
TEST_F(BLPlanarityTesterTests, Test103) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01  VR
    //    ____|_______________________
    //   |    |    |           |      |
    //  L02 *P03 *L04       _*Q14__  L18
    //      / | \          |   |   |
    //   *L05*L06*Q07    *L15*L16*L17
    //            /|\
    //       *P08*L09*L10
    //       / | \
    //   *L11*L12*L13

    pqTree.initializeForOneIteration();

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::Full;
    Q07.mPertinentChildrenCount = 3;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::Full;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());
    createFullLink(Q07.backIt(), P03.backIt());

    auto& L15 = createL(pqTree, G);
    L15.mGeneration = pqTree.mGeneration;
    L15.mPertinentType = BLTreeNode::Full;
    auto& L16 = createL(pqTree, G);
    L16.mGeneration = pqTree.mGeneration;
    L16.mPertinentType = BLTreeNode::Full;
    auto& L17 = createL(pqTree, G);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = BLTreeNode::Full;
    Q14.mPertinentChildrenCount = 3;

    createFullLink(L15.backIt(), Q14.backIt());
    createFullLink(L16.backIt(), Q14.backIt());
    createFullLink(L17.backIt(), Q14.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    /*auto& Q01 = */createQ(pqTree, Q01children);

    auto& VR = createVR(pqTree);

    VR.mGeneration = pqTree.mGeneration;
    VR.mPertinentType = BLTreeNode::Full;
    VR.mPertinentChildrenCount = 3;

    createFullLink(P03.backIt(), VR.backIt());
    createFullLink(L04.backIt(), VR.backIt());
    createFullLink(Q14.backIt(), VR.backIt());

    auto pertinentRoot = VR.backIt();

    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    auto &Att = pqTree.toNodeRef(attIt);
    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(areImmediateSiblings(L02, Att), true);
    EXPECT_EQ(areImmediateSiblings(L18, Att), true);

    EXPECT_EQ(pqTree.numNodes(), 5);

    EXPECT_EQ(tester.mOrientInNorm.size(),2);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_05.backIt());
    EXPECT_EQ(*(tester.mOrientInNorm.rbegin()),track_01.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),2);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_06.backIt());
    EXPECT_EQ(*(tester.mOrientInReversed.rbegin()),track_02.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),2);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_07.backIt());
    EXPECT_EQ(*(tester.mOrientOutNorm.rbegin()),track_03.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),2);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_08.backIt());
    EXPECT_EQ(*(tester.mOrientOutReversed.rbegin()),track_04.backIt());
}


/**  @brief tests BLPlanarityTester::removePertinentTree(): VR
 */
TEST_F(BLPlanarityTesterTests, Test104) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01  VR
    //    ____|_______________________
    //   |    |    |           |      |
    //  L02 *P03 *L04       __Q14__  L18
    //      / | \          |   |   |
    //   *L05*L06*Q07     L15 L16 L17
    //            /|\
    //       *P08*L09*L10
    //       / | \
    //   *L11*L12*L13

    pqTree.initializeForOneIteration();

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::Full;
    Q07.mPertinentChildrenCount = 3;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::Full;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());
    createFullLink(Q07.backIt(), P03.backIt());

    auto& L15 = createL(pqTree, G);
    auto& L16 = createL(pqTree, G);
    auto& L17 = createL(pqTree, G);
    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());

    list<bool> Q01orientations;
    Q01orientations.push_back(true);
    Q01orientations.push_back(false);
    Q01orientations.push_back(true);
    Q01orientations.push_back(false);
    Q01orientations.push_back(true);

    /*auto& Q01 = */createQext(pqTree, Q01children, Q01orientations);

    auto& VR = createVR(pqTree);

    VR.mGeneration = pqTree.mGeneration;
    VR.mPertinentType = BLTreeNode::Full;
    VR.mPertinentChildrenCount = 2;

    createFullLink(P03.backIt(), VR.backIt());
    createFullLink(L04.backIt(), VR.backIt());

    auto pertinentRoot = VR.backIt();
    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    auto &Att = pqTree.toNodeRef(attIt);
    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(areImmediateSiblings(L02, Att), true);
    EXPECT_EQ(areImmediateSiblings(Q14, Att), true);

    EXPECT_EQ(pqTree.numNodes(), 9);

    EXPECT_EQ(tester.mOrientInNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_01.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_02.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_03.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_04.backIt());

}


/**  @brief tests BLPlanarityTester::removePertinentTree(): CDPartial
 */
TEST_F(BLPlanarityTesterTests, Test105) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01
    //    ____|_______________________
    //   |    |    |           |      |
    // *L02 *P03 *L04       _*Q14__ *L18
    //      / | \          |   |   |
    //   *L05*L06*Q07    *L15*L16*L17
    //            /|\
    //       *P08 L09*L10
    //       / | \
    //   *L11*L12*L13

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);

    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::CDPartial;
    Q07.mPertinentChildrenCount = 2;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::CDPartial;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());

    P03.mCDPartialChild = Q07.backIt();

    auto& L15 = createL(pqTree, G);
    L15.mGeneration = pqTree.mGeneration;
    L15.mPertinentType = BLTreeNode::Full;
    auto& L16 = createL(pqTree, G);
    L16.mGeneration = pqTree.mGeneration;
    L16.mPertinentType = BLTreeNode::Full;
    auto& L17 = createL(pqTree, G);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = BLTreeNode::Full;
    Q14.mPertinentChildrenCount = 3;

    createFullLink(L15.backIt(), Q14.backIt());
    createFullLink(L16.backIt(), Q14.backIt());
    createFullLink(L17.backIt(), Q14.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);
    L18.mGeneration = pqTree.mGeneration;
    L18.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = BLTreeNode::CDPartial;
    Q01.mPertinentChildrenCount = 5;

    createFullLink(L02.backIt(), Q01.backIt());
    createFullLink(L04.backIt(), Q01.backIt());
    createFullLink(Q14.backIt(), Q01.backIt());
    createFullLink(L18.backIt(), Q01.backIt());

    Q01.mCDPartialChild = P03.backIt();

    pqTree.mCDPartialRoot = Q07.backIt();
    auto pertinentRoot = Q01.backIt();
    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);
    auto &Att = pqTree.toNodeRef(attIt);

    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(areImmediateSiblings(L09, Att), true);
    EXPECT_EQ(Att.mParent, Q07.backIt());

    EXPECT_EQ(pqTree.numNodes(), 3);
    EXPECT_EQ(tester.mOrientInNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_05.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_06.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_07.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_08.backIt());

}


/**  @brief tests BLPlanarityTester::removePertinentTree(): CDPartial
 */
TEST_F(BLPlanarityTesterTests, Test106) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01
    //    ____|_______________________
    //   |    |    |           |      |
    // *L02 *P03 *L04       _*Q14__ *L18
    //      / | \          |   |   |
    //   *L05*L06*Q07    *L15 L16*L17
    //            /|\
    //       *P08*L09*L10
    //       / | \
    //   *L11*L12*L13


    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentType = BLTreeNode::Full;
    P08.mPertinentChildrenCount = 3;

    createFullLink(L11.backIt(), P08.backIt());
    createFullLink(L12.backIt(), P08.backIt());
    createFullLink(L13.backIt(), P08.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;

    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::Full;
    Q07.mPertinentChildrenCount = 3;

    createFullLink(P08.backIt(), Q07.backIt());
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentType = BLTreeNode::Full;
    P03.mPertinentChildrenCount = 3;

    createFullLink(L05.backIt(), P03.backIt());
    createFullLink(L06.backIt(), P03.backIt());
    createFullLink(Q07.backIt(), P03.backIt());

    auto& L15 = createL(pqTree, G);
    L15.mGeneration = pqTree.mGeneration;
    L15.mPertinentType = BLTreeNode::Full;
    auto& L16 = createL(pqTree, G);

    auto& L17 = createL(pqTree, G);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = BLTreeNode::CDPartial;
    Q14.mPertinentChildrenCount = 2;

    createFullLink(L15.backIt(), Q14.backIt());
    createFullLink(L17.backIt(), Q14.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);
    L18.mGeneration = pqTree.mGeneration;
    L18.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = BLTreeNode::CDPartial;
    Q01.mPertinentChildrenCount = 5;
    Q01.mCDPartialChild = Q14.backIt();

    createFullLink(L02.backIt(), Q01.backIt());
    createFullLink(P03.backIt(), Q01.backIt());
    createFullLink(L04.backIt(), Q01.backIt());
    createFullLink(L18.backIt(), Q01.backIt());

    pqTree.mCDPartialRoot = Q14.backIt();
    auto pertinentRoot = Q01.backIt();

    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    auto &Att = pqTree.toNodeRef(attIt);

    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(areImmediateSiblings(L16, Att), true);
    EXPECT_EQ(Att.mParent, Q14.backIt());

    EXPECT_EQ(pqTree.numNodes(), 3);

    EXPECT_EQ(tester.mOrientInNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientInNorm.begin()),track_01.backIt());
    EXPECT_EQ(tester.mOrientInReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientInReversed.begin()),track_02.backIt());
    EXPECT_EQ(tester.mOrientOutNorm.size(),1);
    EXPECT_EQ(*(tester.mOrientOutNorm.begin()),track_03.backIt());
    EXPECT_EQ(tester.mOrientOutReversed.size(),1);
    EXPECT_EQ(*(tester.mOrientOutReversed.begin()),track_04.backIt());

}


/**  @brief tests BLPlanarityTester::removePertinentTree(): CDPartial
 */
TEST_F(BLPlanarityTesterTests, Test107) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01
    //    ____|_______________________
    //   |    |    |           |      |
    // *L02  P03  L04       _ Q14__ *L18
    //      / | \          |   |   |
    //    L05 L06 Q07     L15 L16 L17
    //            /|\
    //        P08 L09 L10
    //       / | \
    //    L11 L12 L13

    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);

    list<node_list_it_t> P08children;
    P08children.push_back(L11.backIt());
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    auto& P08 = createP(pqTree, P08children);

    auto& L09 = createL(pqTree, G);

    auto& L10 = createL(pqTree, G);

    list<node_list_it_t> Q07children;
    Q07children.push_back(P08.backIt());
    Q07children.push_back(L09.backIt());
    Q07children.push_back(L10.backIt());
    auto& Q07 = createQ(pqTree, Q07children);

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q07.mOrientInNorm.push_back(track_01.backIt());
    Q07.mOrientInReversed.push_back(track_02.backIt());
    Q07.mOrientOutNorm.push_back(track_03.backIt());
    Q07.mOrientOutReversed.push_back(track_04.backIt());

    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(Q07.backIt());
    auto& P03 = createP(pqTree, P03children);

    auto& L15 = createL(pqTree, G);
    auto& L16 = createL(pqTree, G);
    auto& L17 = createL(pqTree, G);

    list<node_list_it_t> Q14children;
    Q14children.push_back(L15.backIt());
    Q14children.push_back(L16.backIt());
    Q14children.push_back(L17.backIt());
    auto& Q14 = createQ(pqTree, Q14children);

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q14.mOrientInNorm.push_back(track_05.backIt());
    Q14.mOrientInReversed.push_back(track_06.backIt());
    Q14.mOrientOutNorm.push_back(track_07.backIt());
    Q14.mOrientOutReversed.push_back(track_08.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);

    auto& L18 = createL(pqTree, G);
    L18.mGeneration = pqTree.mGeneration;
    L18.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L02.backIt());
    Q01children.push_back(P03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q14.backIt());
    Q01children.push_back(L18.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = BLTreeNode::CDPartial;
    Q01.mPertinentChildrenCount = 2;

    createFullLink(L02.backIt(), Q01.backIt());
    createFullLink(L18.backIt(), Q01.backIt());

    pqTree.mCDPartialRoot = Q01.backIt();

    auto pertinentRoot = Q01.backIt();

    auto attIt = removePertinentTree(tester, pqTree, pertinentRoot);

    auto &Att = pqTree.toNodeRef(attIt);

    EXPECT_EQ(Att.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(areImmediateSiblings(Q14, Att), true);
    EXPECT_EQ(pqTree.numNodes(), 17);
    EXPECT_EQ(Att.mParent, Q01.backIt());

    EXPECT_EQ(tester.mOrientInNorm.size(),0);
    EXPECT_EQ(tester.mOrientInReversed.size(),0);
    EXPECT_EQ(tester.mOrientOutNorm.size(),0);
    EXPECT_EQ(tester.mOrientOutReversed.size(),0);
}
