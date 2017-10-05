/**  @brief tests BLPlanarizer::templateQ1()
 */
TEST_F(BLPlanarityTesterTests, Test33){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);


    //     Q1 positive
    //
    //      Q01
    //       |
    //      Q02
    //    /  |  \
    // *L01*L02*L03

    pqTree.initializeForOneIteration();

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;
    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;
    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(L03.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 3;
    Q02.mPertinentLeavesCount = 3;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L03.backIt(), Q02.backIt());

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);

    EXPECT_EQ(pqTree.templateQ1(Q02, BLTree::NOT_FINAL_REDUCTION),true);

    // Check Q01
    list<node_list_it_t> tmpList;
    tmpList.push_back(Q02.backIt());
    EXPECT_EQ(checkUnorderedList(Q01.mFullChildren,tmpList),true);

    // Check Q02
    EXPECT_EQ(Q02.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(Q02.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q02.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L01.backIt());
    tmpList2.push_back(L02.backIt());
    tmpList2.push_back(L03.backIt());
    EXPECT_EQ(checkUnorderedList(Q02.mFullChildren,tmpList2),true);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 3);
    EXPECT_EQ(Q02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(Q02.backIt(), Q01.backIt()), true);
    EXPECT_EQ(Q01.mFullChildren.size(), 1);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test34){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #1
    //
    //                Q03                                   Q03
    //                 |                                     |
    //    ____________Q04___________           _____________Q04______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    // *L08*L09   ____Q05____  L10 L11      *L08 *L09 *L07 *L06 L05 L04 L10 L11
    //           |   |   |   |
    //          L04 L05*L06*L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L06.backIt(), Q05.backIt());
    createFullLink(L07.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q05.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q05.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q05.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q05.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;

    auto &track_15 = createBLGraphNode(G);
    auto &track_16 = createBLGraphNode(G);
    auto &track_17 = createBLGraphNode(G);
    auto &track_18 = createBLGraphNode(G);
    L08.mAssumedOrientInNorm.push_back(track_15.backIt());
    L08.mAssumedOrientInReversed.push_back(track_16.backIt());
    L08.mAssumedOrientOutNorm.push_back(track_17.backIt());
    L08.mAssumedOrientOutReversed.push_back(track_18.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;

    auto &track_19 = createBLGraphNode(G);
    auto &track_20 = createBLGraphNode(G);
    auto &track_21 = createBLGraphNode(G);
    auto &track_22 = createBLGraphNode(G);
    L09.mAssumedOrientInNorm.push_back(track_19.backIt());
    L09.mAssumedOrientInReversed.push_back(track_20.backIt());
    L09.mAssumedOrientOutNorm.push_back(track_21.backIt());
    L09.mAssumedOrientOutReversed.push_back(track_22.backIt());

    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    list<bool> Q04orientations;
    Q04orientations.push_back(false);
    Q04orientations.push_back(true);
    Q04orientations.push_back(false);
    Q04orientations.push_back(true);
    Q04orientations.push_back(true);
    auto& Q04 = createQext(pqTree, Q04children, Q04orientations);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;

    createFullLink(L08.backIt(), Q04.backIt());
    createFullLink(L09.backIt(), Q04.backIt());

    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut)
                                                                        ,true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L08.backIt());
    tmpList.push_back(L09.backIt());
    tmpList.push_back(L07.backIt());
    tmpList.push_back(L06.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L08.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());

    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_12.backIt());
    expectedOrientInReversed.push_back(track_11.backIt());
    expectedOrientOutNorm.push_back(track_14.backIt());
    expectedOrientOutReversed.push_back(track_13.backIt());

    expectedOrientInNorm.push_back(track_16.backIt());
    expectedOrientInReversed.push_back(track_15.backIt());
    expectedOrientOutNorm.push_back(track_18.backIt());
    expectedOrientOutReversed.push_back(track_17.backIt());

    expectedOrientInNorm.push_back(track_19.backIt());
    expectedOrientInReversed.push_back(track_20.backIt());
    expectedOrientOutNorm.push_back(track_21.backIt());
    expectedOrientOutReversed.push_back(track_22.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test35){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #2
    //
    //                Q03                                   Q03
    //                 |                                     |
    //    ____________Q04___________           _____________Q04______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    //  L08 L09   ____Q05____ *L10*L11       L08  L09  L04  L05*L06*L07*L10*L11
    //           |   |   |   |
    //          L04 L05*L06*L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L06.backIt(), Q05.backIt());
    createFullLink(L07.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q05.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q05.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q05.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q05.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;

    auto &track_15 = createBLGraphNode(G);
    auto &track_16 = createBLGraphNode(G);
    auto &track_17 = createBLGraphNode(G);
    auto &track_18 = createBLGraphNode(G);
    L10.mAssumedOrientInNorm.push_back(track_15.backIt());
    L10.mAssumedOrientInReversed.push_back(track_16.backIt());
    L10.mAssumedOrientOutNorm.push_back(track_17.backIt());
    L10.mAssumedOrientOutReversed.push_back(track_18.backIt());

    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;

    auto &track_19 = createBLGraphNode(G);
    auto &track_20 = createBLGraphNode(G);
    auto &track_21 = createBLGraphNode(G);
    auto &track_22 = createBLGraphNode(G);
    L11.mAssumedOrientInNorm.push_back(track_19.backIt());
    L11.mAssumedOrientInReversed.push_back(track_20.backIt());
    L11.mAssumedOrientOutNorm.push_back(track_21.backIt());
    L11.mAssumedOrientOutReversed.push_back(track_22.backIt());

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    list<bool> Q04orientations;
    Q04orientations.push_back(true);
    Q04orientations.push_back(false);
    Q04orientations.push_back(true);
    Q04orientations.push_back(false);
    Q04orientations.push_back(true);

    auto& Q04 = createQext(pqTree, Q04children, Q04orientations);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;

    createFullLink(L10.backIt(), Q04.backIt());
    createFullLink(L11.backIt(), Q04.backIt());

    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut)
                                                                        ,true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());


    list<node_list_it_t> tmpList;
    tmpList.push_back(L06.backIt());
    tmpList.push_back(L07.backIt());
    tmpList.push_back(L10.backIt());
    tmpList.push_back(L11.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L08.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());

    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_16.backIt());
    expectedOrientInReversed.push_back(track_15.backIt());
    expectedOrientOutNorm.push_back(track_18.backIt());
    expectedOrientOutReversed.push_back(track_17.backIt());

    expectedOrientInNorm.push_back(track_19.backIt());
    expectedOrientInReversed.push_back(track_20.backIt());
    expectedOrientOutNorm.push_back(track_21.backIt());
    expectedOrientOutReversed.push_back(track_22.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test36){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #3
    //
    //                Q03                                   Q03
    //                 |                                     |
    //    ____________Q04___________           _____________Q04______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    //  L08 L09   ____Q05____ *L10*L11       L08  L09  L07  L06*L05*L04*L10*L11
    //           |   |   |   |
    //         *L04*L05 L06 L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L04.backIt(), Q05.backIt());
    createFullLink(L05.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;

    createFullLink(L10.backIt(), Q04.backIt());
    createFullLink(L11.backIt(), Q04.backIt());

    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut)
                                                                        ,true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());


    list<node_list_it_t> tmpList;
    tmpList.push_back(L05.backIt());
    tmpList.push_back(L04.backIt());
    tmpList.push_back(L10.backIt());
    tmpList.push_back(L11.backIt());

    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L08.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());

    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test37){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #4
    //
    //                Q03                                   Q03
    //                 |                                     |
    //    ____________Q04___________           _____________Q04______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    // *L08*L09   ____Q05____  L10 L11      *L08 *L09 *L04 *L05 L06 L07 L10 L11
    //           |   |   |   |
    //         *L04*L05 L06 L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L04.backIt(), Q05.backIt());
    createFullLink(L05.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;

    createFullLink(L08.backIt(), Q04.backIt());
    createFullLink(L09.backIt(), Q04.backIt());

    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut)
                                                                        ,true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L05.backIt());
    tmpList.push_back(L04.backIt());
    tmpList.push_back(L08.backIt());
    tmpList.push_back(L09.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L08.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());

    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test38){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #5
    //
    //                Q03                               Q03
    //                 |                                 |
    //                Q04___________           _________Q04________
    //                 |        |   |   =>    |    |   |   |   |   |
    //            ____Q05____  L10 L11      *L04 *L05 L06 L07 L10 L11
    //           |   |   |   |
    //         *L04*L05 L06 L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L04.backIt(), Q05.backIt());
    createFullLink(L05.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 1;
    Q04.mPertinentLeavesCount = 2;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L04.backIt());
    tmpList.push_back(L05.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L04.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());

    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(Q04.mFullChildren.size(), 2);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test39){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #6
    //
    //                Q03                               Q03
    //                 |                                 |
    //     ___________Q04                      _________Q04________
    //    |   |        |                =>    |    |   |   |   |   |
    //   L08 L09  ____Q05____               *L04 *L05 L06 L07 L09 L08
    //           |   |   |   |
    //         *L04*L05 L06 L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L04.backIt(), Q05.backIt());
    createFullLink(L05.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 1;
    Q04.mPertinentLeavesCount = 2;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L04.backIt());
    tmpList.push_back(L05.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L04.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());

    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(Q04.mFullChildren.size(), 2);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test40){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #7
    //
    //                Q03                               Q03
    //                 |                                 |
    //                Q04__________            _________Q04________
    //                 |       |   |    =>    |    |   |   |   |   |
    //            ____Q05____ L08 L09        L09  L08 L04 L05 L06 L07
    //           |   |   |   |
    //          L04 L05*L06*L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L06.backIt(), Q05.backIt());
    createFullLink(L07.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 1;
    Q04.mPertinentLeavesCount = 2;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L06.backIt());
    tmpList.push_back(L07.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L09.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());

    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 2);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F(BLPlanarityTesterTests, Test41){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #8
    //
    //                Q03                               Q03
    //                 |                                 |
    //     ___________Q04                      _________Q04________
    //    |   |        |                =>    |    |   |   |   |   |
    //   L08 L09  ____Q05____                L08  L09 L04 L05*L06*L07
    //           |   |   |   |
    //          L04 L05*L06*L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L06.backIt(), Q05.backIt());
    createFullLink(L07.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 1;
    Q04.mPertinentLeavesCount = 2;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L06.backIt());
    tmpList.push_back(L07.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L08.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());

    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 2);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F (BLPlanarityTesterTests, Test42) {

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #9
    //
    //                Q03                              Q03
    //                 |                                |
    //                Q04__________            ________Q04________
    //                 |       |   |    =>    |   |   |   |   |   |
    //            ____Q05____*L08*L09        L04 L05*L06*L07*L08*L09
    //           |   |   |   |
    //          L04 L05*L06*L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L06.backIt(), Q05.backIt());
    createFullLink(L07.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    createFullLink(L08.backIt(), Q04.backIt());
    createFullLink(L09.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());


    list<node_list_it_t> tmpList;
    tmpList.push_back(L06.backIt());
    tmpList.push_back(L07.backIt());
    tmpList.push_back(L08.backIt());
    tmpList.push_back(L09.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L04.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F (BLPlanarityTesterTests, Test43) {

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #10
    //
    //                Q03                              Q03
    //                 |                                |
    //      __________Q04                      ________Q04________
    //     |   |       |                =>    |   |   |   |   |   |
    //   *L08*L09 ____Q05____                L04 L05*L06*L07*L09*L08
    //           |   |   |   |
    //          L04 L05*L06*L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L06.backIt(), Q05.backIt());
    createFullLink(L07.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    createFullLink(L08.backIt(), Q04.backIt());
    createFullLink(L09.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L06.backIt());
    tmpList.push_back(L07.backIt());
    tmpList.push_back(L09.backIt());
    tmpList.push_back(L08.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L04.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());

    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F (BLPlanarityTesterTests, Test44) {

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #11
    //
    //                Q03                                Q03
    //                 |                                  |
    //                Q04___________             ________Q04________
    //                 |        |   |     =>    |   |   |   |   |   |
    //            ____Q05____ *L08*L09         L07 L06*L05*L04*L08*L09
    //           |   |   |   |
    //         *L04*L05 L06 L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L04.backIt(), Q05.backIt());
    createFullLink(L05.backIt(), Q05.backIt());
 
    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    createFullLink(L08.backIt(), Q04.backIt());
    createFullLink(L09.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L05.backIt());
    tmpList.push_back(L04.backIt());
    tmpList.push_back(L08.backIt());
    tmpList.push_back(L09.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L07.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q04.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F (BLPlanarityTesterTests, Test45) {

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #12
    //
    //                Q03                                Q03
    //                 |                                  |
    //     ___________Q04                        ________Q04________
    //    |   |        |                  =>    |   |   |   |   |   |
    //  *L08*L09  ____Q05____                  L07 L06*L05*L04*L09*L08
    //           |   |   |   |
    //         *L04*L05 L06 L07

    pqTree.initializeForOneIteration();

    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::SinglyPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L04.backIt(), Q05.backIt());
    createFullLink(L05.backIt(), Q05.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q05.mOrientInNorm.push_back(track_01.backIt());
    Q05.mOrientInReversed.push_back(track_02.backIt());
    Q05.mOrientOutNorm.push_back(track_03.backIt());
    Q05.mOrientOutReversed.push_back(track_04.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mPertinentLeavesCount = 4;
    Q04.mSinglyPartialChild1 = Q05.backIt();

    createFullLink(L08.backIt(), Q04.backIt());
    createFullLink(L09.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q04, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);   

    // Check Q03
    EXPECT_EQ(Q03.mSinglyPartialChild1, Q04.backIt());

    // Check Q04
    EXPECT_EQ(Q04.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L05.backIt());
    tmpList.push_back(L04.backIt());
    tmpList.push_back(L09.backIt());
    tmpList.push_back(L08.backIt());
    EXPECT_EQ(checkUnorderedList(Q04.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q04.mEndChild1==L07.backIt())?Q04.mEndChild1:Q04.mEndChild2;
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());

    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkOrientation(Q04,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q04.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), Q04.backIt()), true);
    EXPECT_EQ(Q04.mFullChildren.size(), 4);
    EXPECT_EQ(Q03.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);
}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F (BLPlanarityTesterTests, Test46) {

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #14
    //
    //           Q01                     Q01
    //            |                       |
    //       ____Q02____              ___Q02_____
    //      |   |   |   |  =>        |   |   |   |
    //     L01 L02*L03*L04          L01  L02*L03*L04

    pqTree.initializeForOneIteration();

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(L03.backIt());
    Q02children.push_back(L04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q02.backIt());
    createFullLink(L04.backIt(), Q02.backIt());

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);

    // Check Q01
    EXPECT_EQ(Q01.mSinglyPartialChild1, Q02.backIt());

    // Check Q02
    EXPECT_EQ(Q02.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q02.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q02.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L03.backIt());
    tmpList.push_back(L04.backIt());
    EXPECT_EQ(checkUnorderedList(Q02.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q02.mEndChild1==L01.backIt())?Q02.mEndChild1:Q02.mEndChild2;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);

    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 2);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ2()
 */
TEST_F (BLPlanarityTesterTests, Test47) {

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //     Q2 positive #13
    //
    //            Q01                      Q01
    //             |                        |
    //       _____Q02____              ____Q02_____
    //      |    |   |   |  =>        |    |   |   |
    //    *L01 *L02 L03 L04         *L01 *L02 L03 L04

    pqTree.initializeForOneIteration();

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;
    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;
    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(L03.backIt());
    Q02children.push_back(L04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 2;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ2(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                         true);
    EXPECT_EQ(earlyOut, false);

    // Check Q01
    EXPECT_EQ(Q01.mSinglyPartialChild1, Q02.backIt());

    // Check Q02
    EXPECT_EQ(Q02.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q02.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q02.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L01.backIt());
    tmpList.push_back(L02.backIt());
    EXPECT_EQ(checkUnorderedList(Q02.mFullChildren,tmpList),true);

    auto prevIt = pqTree.nil();
    auto curIt = (Q02.mEndChild1==L01.backIt())?Q02.mEndChild1:Q02.mEndChild2;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);


    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 2);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test49){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //     L01  L02   ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
    //               |    |    |    |           |    |    |    |
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L07 *L08 *L09 *L10  L11  L12  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;
    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;

    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //     L01  L02   ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
    //               |    |    |    |           |    |    |    |
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L07 *L08 *L09 *L10  L11  L12  L13  L14


    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    L07.mParent = VR.backIt();
    L08.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 4;
    VR.mPertinentLeavesCount = 6;

    createFullLink(L07.backIt(), VR.backIt());
    createFullLink(L08.backIt(), VR.backIt());

    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_02.backIt());
    expectedAssumedOrientInNorm.push_back(track_05.backIt());
    expectedAssumedOrientInReversed.push_back(track_01.backIt());
    expectedAssumedOrientInReversed.push_back(track_06.backIt());
    expectedAssumedOrientOutNorm.push_back(track_04.backIt());
    expectedAssumedOrientOutNorm.push_back(track_07.backIt());
    expectedAssumedOrientOutReversed.push_back(track_03.backIt());
    expectedAssumedOrientOutReversed.push_back(track_08.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), VR.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), VR.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), VR.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), VR.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 6);
    EXPECT_EQ(VR.mFullChildrenSet, false);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test50){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #1 EEPFFPEE
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //     L01  L02   ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
    //               |    |    |    |           |    |    |    |
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L07 *L08 *L09 *L10  L11  L12  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    auto &track_15 = createBLGraphNode(G);
    auto &track_16 = createBLGraphNode(G);
    auto &track_17 = createBLGraphNode(G);
    auto &track_18 = createBLGraphNode(G);
    L07.mAssumedOrientInNorm.push_back(track_15.backIt());
    L07.mAssumedOrientInReversed.push_back(track_16.backIt());
    L07.mAssumedOrientOutNorm.push_back(track_17.backIt());
    L07.mAssumedOrientOutReversed.push_back(track_18.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;

    auto &track_19 = createBLGraphNode(G);
    auto &track_20 = createBLGraphNode(G);
    auto &track_21 = createBLGraphNode(G);
    auto &track_22 = createBLGraphNode(G);
    L08.mAssumedOrientInNorm.push_back(track_19.backIt());
    L08.mAssumedOrientInReversed.push_back(track_20.backIt());
    L08.mAssumedOrientOutNorm.push_back(track_21.backIt());
    L08.mAssumedOrientOutReversed.push_back(track_22.backIt());

    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    L07.mParent = VR.backIt();
    L08.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 4;
    VR.mPertinentLeavesCount = 6;

    createFullLink(L07.backIt(), VR.backIt());
    createFullLink(L08.backIt(), VR.backIt());

    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_01.backIt());
    expectedAssumedOrientInNorm.push_back(track_06.backIt());
    expectedAssumedOrientInReversed.push_back(track_02.backIt());
    expectedAssumedOrientInReversed.push_back(track_05.backIt());
    expectedAssumedOrientOutNorm.push_back(track_03.backIt());
    expectedAssumedOrientOutNorm.push_back(track_08.backIt());
    expectedAssumedOrientOutReversed.push_back(track_04.backIt());
    expectedAssumedOrientOutReversed.push_back(track_07.backIt());

    expectedAssumedOrientInNorm.push_back(track_12.backIt());
    expectedAssumedOrientInReversed.push_back(track_11.backIt());
    expectedAssumedOrientOutNorm.push_back(track_14.backIt());
    expectedAssumedOrientOutReversed.push_back(track_13.backIt());

    expectedAssumedOrientInNorm.push_back(track_15.backIt());
    expectedAssumedOrientInReversed.push_back(track_16.backIt());
    expectedAssumedOrientOutNorm.push_back(track_17.backIt());
    expectedAssumedOrientOutReversed.push_back(track_18.backIt());

    expectedAssumedOrientInNorm.push_back(track_20.backIt());
    expectedAssumedOrientInReversed.push_back(track_19.backIt());
    expectedAssumedOrientOutNorm.push_back(track_22.backIt());
    expectedAssumedOrientOutReversed.push_back(track_21.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());


    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), VR.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), VR.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), VR.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), VR.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 6);


}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test51){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #2 EEPFFPEE
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //     L01  L02   ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
    //               |    |    |    |           |    |    |    |
    //             *L03 *L04  L05  L06         L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L07 *L08 *L12 *L11  L10  L09  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    auto &track_15 = createBLGraphNode(G);
    auto &track_16 = createBLGraphNode(G);
    auto &track_17 = createBLGraphNode(G);
    auto &track_18 = createBLGraphNode(G);
    L07.mAssumedOrientInNorm.push_back(track_15.backIt());
    L07.mAssumedOrientInReversed.push_back(track_16.backIt());
    L07.mAssumedOrientOutNorm.push_back(track_17.backIt());
    L07.mAssumedOrientOutReversed.push_back(track_18.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;

    auto &track_19 = createBLGraphNode(G);
    auto &track_20 = createBLGraphNode(G);
    auto &track_21 = createBLGraphNode(G);
    auto &track_22 = createBLGraphNode(G);
    L08.mAssumedOrientInNorm.push_back(track_19.backIt());
    L08.mAssumedOrientInReversed.push_back(track_20.backIt());
    L08.mAssumedOrientOutNorm.push_back(track_21.backIt());
    L08.mAssumedOrientOutReversed.push_back(track_22.backIt());

    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    L07.mParent = VR.backIt();
    L08.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 4;
    VR.mPertinentLeavesCount = 6;

    createFullLink(L07.backIt(), VR.backIt());
    createFullLink(L08.backIt(), VR.backIt());

    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_02.backIt());
    expectedAssumedOrientInReversed.push_back(track_01.backIt());
    expectedAssumedOrientOutNorm.push_back(track_04.backIt());
    expectedAssumedOrientOutReversed.push_back(track_03.backIt());

    expectedAssumedOrientInNorm.push_back(track_06.backIt());
    expectedAssumedOrientInReversed.push_back(track_05.backIt());
    expectedAssumedOrientOutNorm.push_back(track_08.backIt());
    expectedAssumedOrientOutReversed.push_back(track_07.backIt());

    expectedAssumedOrientInNorm.push_back(track_12.backIt());
    expectedAssumedOrientInReversed.push_back(track_11.backIt());
    expectedAssumedOrientOutNorm.push_back(track_14.backIt());
    expectedAssumedOrientOutReversed.push_back(track_13.backIt());

    expectedAssumedOrientInNorm.push_back(track_15.backIt());
    expectedAssumedOrientInReversed.push_back(track_16.backIt());
    expectedAssumedOrientOutNorm.push_back(track_17.backIt());
    expectedAssumedOrientOutReversed.push_back(track_18.backIt());

    expectedAssumedOrientInNorm.push_back(track_19.backIt());
    expectedAssumedOrientInReversed.push_back(track_20.backIt());
    expectedAssumedOrientOutNorm.push_back(track_21.backIt());
    expectedAssumedOrientOutReversed.push_back(track_22.backIt());

    expectedAssumedOrientInNorm.push_back(track_24.backIt());
    expectedAssumedOrientInReversed.push_back(track_23.backIt());
    expectedAssumedOrientOutNorm.push_back(track_26.backIt());
    expectedAssumedOrientOutReversed.push_back(track_25.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), VR.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), VR.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), VR.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), VR.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 6);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test52){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #3 EEPFFPEE
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //     L01  L02   ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
    //               |    |    |    |           |    |    |    |
    //              L03  L04 *L05 *L06        *L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L07 *L08 *L09 *L10  L11  L12  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    auto &track_15 = createBLGraphNode(G);
    auto &track_16 = createBLGraphNode(G);
    auto &track_17 = createBLGraphNode(G);
    auto &track_18 = createBLGraphNode(G);
    L07.mAssumedOrientInNorm.push_back(track_15.backIt());
    L07.mAssumedOrientInReversed.push_back(track_16.backIt());
    L07.mAssumedOrientOutNorm.push_back(track_17.backIt());
    L07.mAssumedOrientOutReversed.push_back(track_18.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;

    auto &track_19 = createBLGraphNode(G);
    auto &track_20 = createBLGraphNode(G);
    auto &track_21 = createBLGraphNode(G);
    auto &track_22 = createBLGraphNode(G);
    L08.mAssumedOrientInNorm.push_back(track_19.backIt());
    L08.mAssumedOrientInReversed.push_back(track_20.backIt());
    L08.mAssumedOrientOutNorm.push_back(track_21.backIt());
    L08.mAssumedOrientOutReversed.push_back(track_22.backIt());

    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    L07.mParent = VR.backIt();
    L08.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 4;
    VR.mPertinentLeavesCount = 6;

    createFullLink(L07.backIt(), VR.backIt());
    createFullLink(L08.backIt(), VR.backIt());

    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_01.backIt());
    expectedAssumedOrientInReversed.push_back(track_02.backIt());
    expectedAssumedOrientOutNorm.push_back(track_03.backIt());
    expectedAssumedOrientOutReversed.push_back(track_04.backIt());

    expectedAssumedOrientInNorm.push_back(track_05.backIt());
    expectedAssumedOrientInReversed.push_back(track_06.backIt());
    expectedAssumedOrientOutNorm.push_back(track_07.backIt());
    expectedAssumedOrientOutReversed.push_back(track_08.backIt());

    expectedAssumedOrientInNorm.push_back(track_11.backIt());
    expectedAssumedOrientInReversed.push_back(track_12.backIt());
    expectedAssumedOrientOutNorm.push_back(track_13.backIt());
    expectedAssumedOrientOutReversed.push_back(track_14.backIt());

    expectedAssumedOrientInNorm.push_back(track_15.backIt());
    expectedAssumedOrientInReversed.push_back(track_16.backIt());
    expectedAssumedOrientOutNorm.push_back(track_17.backIt());
    expectedAssumedOrientOutReversed.push_back(track_18.backIt());

    expectedAssumedOrientInNorm.push_back(track_19.backIt());
    expectedAssumedOrientInReversed.push_back(track_20.backIt());
    expectedAssumedOrientOutNorm.push_back(track_21.backIt());
    expectedAssumedOrientOutReversed.push_back(track_22.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), VR.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), VR.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), VR.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), VR.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 6);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test53){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #4 EEPFFPEE
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //     L01  L02   ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
    //               |    |    |    |           |    |    |    |
    //              L03  L04 *L05 *L06         L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L07 *L08 *L12 *L11  L10  L09  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L07 = createL(pqTree, G);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = BLTreeNode::Full;
    L07.mPertinentLeavesCount = 1;

    auto &track_15 = createBLGraphNode(G);
    auto &track_16 = createBLGraphNode(G);
    auto &track_17 = createBLGraphNode(G);
    auto &track_18 = createBLGraphNode(G);
    L07.mAssumedOrientInNorm.push_back(track_15.backIt());
    L07.mAssumedOrientInReversed.push_back(track_16.backIt());
    L07.mAssumedOrientOutNorm.push_back(track_17.backIt());
    L07.mAssumedOrientOutReversed.push_back(track_18.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;

    auto &track_19 = createBLGraphNode(G);
    auto &track_20 = createBLGraphNode(G);
    auto &track_21 = createBLGraphNode(G);
    auto &track_22 = createBLGraphNode(G);
    L08.mAssumedOrientInNorm.push_back(track_19.backIt());
    L08.mAssumedOrientInReversed.push_back(track_20.backIt());
    L08.mAssumedOrientOutNorm.push_back(track_21.backIt());
    L08.mAssumedOrientOutReversed.push_back(track_22.backIt());

    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    L07.mParent = VR.backIt();
    L08.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 4;
    VR.mPertinentLeavesCount = 6;

    createFullLink(L07.backIt(), VR.backIt());
    createFullLink(L08.backIt(), VR.backIt());

    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L07.isFull(), true);
    EXPECT_EQ(L08.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_01.backIt());
    expectedAssumedOrientInReversed.push_back(track_02.backIt());
    expectedAssumedOrientOutNorm.push_back(track_03.backIt());
    expectedAssumedOrientOutReversed.push_back(track_04.backIt());

    expectedAssumedOrientInNorm.push_back(track_06.backIt());
    expectedAssumedOrientInReversed.push_back(track_05.backIt());
    expectedAssumedOrientOutNorm.push_back(track_08.backIt());
    expectedAssumedOrientOutReversed.push_back(track_07.backIt());

    expectedAssumedOrientInNorm.push_back(track_11.backIt());
    expectedAssumedOrientInReversed.push_back(track_12.backIt());
    expectedAssumedOrientOutNorm.push_back(track_13.backIt());
    expectedAssumedOrientOutReversed.push_back(track_14.backIt());

    expectedAssumedOrientInNorm.push_back(track_15.backIt());
    expectedAssumedOrientInReversed.push_back(track_16.backIt());
    expectedAssumedOrientOutNorm.push_back(track_17.backIt());
    expectedAssumedOrientOutReversed.push_back(track_18.backIt());

    expectedAssumedOrientInNorm.push_back(track_19.backIt());
    expectedAssumedOrientInReversed.push_back(track_20.backIt());
    expectedAssumedOrientOutNorm.push_back(track_21.backIt());
    expectedAssumedOrientOutReversed.push_back(track_22.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), VR.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), VR.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L07.backIt(), VR.backIt()), true);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), VR.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 6);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test54){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #5 EEPPEE
    //                               Q01
    //                                | VR
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_______________________
    //      |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L12 *L11  L10  L09  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);

    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 2;
    VR.mPertinentLeavesCount = 4;
    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_01.backIt());
    expectedAssumedOrientInReversed.push_back(track_02.backIt());
    expectedAssumedOrientOutNorm.push_back(track_03.backIt());
    expectedAssumedOrientOutReversed.push_back(track_04.backIt());

    expectedAssumedOrientInNorm.push_back(track_06.backIt());
    expectedAssumedOrientInReversed.push_back(track_05.backIt());
    expectedAssumedOrientOutNorm.push_back(track_08.backIt());
    expectedAssumedOrientOutReversed.push_back(track_07.backIt());

    expectedAssumedOrientInNorm.push_back(track_11.backIt());
    expectedAssumedOrientInReversed.push_back(track_12.backIt());
    expectedAssumedOrientOutNorm.push_back(track_13.backIt());
    expectedAssumedOrientOutReversed.push_back(track_14.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), VR.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), VR.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 4);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test55){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #6 EEPPEE
    //                               Q01
    //                                | VR
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06*L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_______________________
    //      |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L09 *L19  L11  L12  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 2;
    VR.mPertinentLeavesCount = 4;
    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_01.backIt());
    expectedAssumedOrientInReversed.push_back(track_02.backIt());
    expectedAssumedOrientOutNorm.push_back(track_03.backIt());
    expectedAssumedOrientOutReversed.push_back(track_04.backIt());

    expectedAssumedOrientInNorm.push_back(track_05.backIt());
    expectedAssumedOrientInReversed.push_back(track_06.backIt());
    expectedAssumedOrientOutNorm.push_back(track_07.backIt());
    expectedAssumedOrientOutReversed.push_back(track_08.backIt());

    expectedAssumedOrientInNorm.push_back(track_11.backIt());
    expectedAssumedOrientInReversed.push_back(track_12.backIt());
    expectedAssumedOrientOutNorm.push_back(track_13.backIt());
    expectedAssumedOrientOutReversed.push_back(track_14.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), VR.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), VR.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 4);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test56){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #7 EEPPEE
    //                               Q01
    //                                | VR
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |
    //             *L03 *L04  L05  L06 L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_______________________
    //      |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L12 *L11  L10  L09  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 2;
    VR.mPertinentLeavesCount = 4;
    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_02.backIt());
    expectedAssumedOrientInReversed.push_back(track_01.backIt());
    expectedAssumedOrientOutNorm.push_back(track_04.backIt());
    expectedAssumedOrientOutReversed.push_back(track_03.backIt());

    expectedAssumedOrientInNorm.push_back(track_06.backIt());
    expectedAssumedOrientInReversed.push_back(track_05.backIt());
    expectedAssumedOrientOutNorm.push_back(track_08.backIt());
    expectedAssumedOrientOutReversed.push_back(track_07.backIt());

    expectedAssumedOrientInNorm.push_back(track_11.backIt());
    expectedAssumedOrientInReversed.push_back(track_12.backIt());
    expectedAssumedOrientOutNorm.push_back(track_13.backIt());
    expectedAssumedOrientOutReversed.push_back(track_14.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), VR.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), VR.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 4);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test57){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #8 EEPPEE
    //                               Q01
    //                                | VR
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |
    //             *L03 *L04  L05  L06*L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    | VR
    //       ____________________________Q02_______________________
    //      |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L09 *L10  L11  L12  L13  L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    auto& L14 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    auto& VR = createVR(pqTree);
    Q03.mParent = VR.backIt();
    Q04.mParent = VR.backIt();
    VR.mPertinentChildrenCount = 2;
    VR.mPertinentLeavesCount = 4;
    VR.mSinglyPartialChild1 = Q03.backIt();
    VR.mSinglyPartialChild2 = Q04.backIt();

    EXPECT_EQ(pqTree.templateQ3(VR) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L13.isEmpty(), true);
    EXPECT_EQ(L14.isEmpty(), true);

    list<node_list_it_t> expectedAssumedOrientInNorm;
    list<node_list_it_t> expectedAssumedOrientInReversed;
    list<node_list_it_t> expectedAssumedOrientOutNorm;
    list<node_list_it_t> expectedAssumedOrientOutReversed;

    expectedAssumedOrientInNorm.push_back(track_02.backIt());
    expectedAssumedOrientInReversed.push_back(track_01.backIt());
    expectedAssumedOrientOutNorm.push_back(track_04.backIt());
    expectedAssumedOrientOutReversed.push_back(track_03.backIt());

    expectedAssumedOrientInNorm.push_back(track_05.backIt());
    expectedAssumedOrientInReversed.push_back(track_06.backIt());
    expectedAssumedOrientOutNorm.push_back(track_07.backIt());
    expectedAssumedOrientOutReversed.push_back(track_08.backIt());

    expectedAssumedOrientInNorm.push_back(track_11.backIt());
    expectedAssumedOrientInReversed.push_back(track_12.backIt());
    expectedAssumedOrientOutNorm.push_back(track_13.backIt());
    expectedAssumedOrientOutReversed.push_back(track_14.backIt());

    expectedAssumedOrientInNorm.push_back(track_23.backIt());
    expectedAssumedOrientInReversed.push_back(track_24.backIt());
    expectedAssumedOrientOutNorm.push_back(track_25.backIt());
    expectedAssumedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkAssumedOrientation(L13,
        expectedAssumedOrientInNorm, expectedAssumedOrientInReversed,
        expectedAssumedOrientOutNorm,expectedAssumedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), VR.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), VR.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), VR.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), VR.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, false);
    EXPECT_EQ(L14.mFullChildrenSet, false);
    EXPECT_EQ(VR.mFullChildren.size(), 4);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test58){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #9 EEPP
    //                               Q01
    //                                |
    //       ________________________Q02_______
    //      |    |           |                 |
    //     L01  L02   ____ _Q03_____     _____Q04______
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_____________
    //      |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L12 *L11  L19  L09

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;


    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);


    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);

    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test59){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #10 PPEE
    //                        Q01
    //                         |
    //                 _______Q02_______________________
    //                |                 |          |    |
    //         ______Q03_____     _____Q04______  L01  L02
    //        |    |    |    |   |    |    |    |
    //       L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________________
    //      |    |    |    |    |    |    |    |   |   |
    //     L03  L04 *L05 *L06 *L12 *L11  L19  L09 L01 L02

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);


    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());

    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test60){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #11 PPEE
    //                        Q01
    //                         |
    //                 _______Q02_______________________
    //                |                 |          |    |
    //         ______Q03_____     _____Q04______  L01  L02
    //        |    |    |    |   |    |    |    |
    //       L03  L04 *L05 *L06*L09 *L10  L11  L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________________
    //      |    |    |    |    |    |    |    |   |   |
    //     L03  L04 *L05 *L06 *L09 *L10  L11  L12 L01 L02

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);


    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());

    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test61){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #12 PPEE
    //                        Q01
    //                         |
    //                 _______Q02_______________________
    //                |                 |          |    |
    //         ______Q03_____     _____Q04______  L01  L02
    //        |    |    |    |   |    |    |    |
    //      *L03 *L04  L05  L06 L09  L10 *L11 *L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________________
    //      |    |    |    |    |    |    |    |   |   |
    //     L06  L05 *L04 *L03 *L12 *L11  L10  L09 L01 L02

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);


    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());


    // BLTree::templateQ3() positive #11 PPEE
    //                        Q01
    //                         |
    //                 _______Q02_______________________
    //                |                 |          |    |
    //         ______Q03_____     _____Q04______  L01  L02
    //        |    |    |    |   |    |    |    |
    //      *L03 *L04  L05  L06 L09  L10 *L11 *L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________________
    //      |    |    |    |    |    |    |    |   |   |
    //     L06  L05 *L04 *L03 *L12 *L11  L10  L09 L01 L02

    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test62){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #12 PPEE
    //                        Q01
    //                         |
    //                 _______Q02_______________________
    //                |                 |          |    |
    //         ______Q03_____     _____Q04______  L01  L02
    //        |    |    |    |   |    |    |    |
    //      *L03 *L04  L05  L06*L09 *L10  L11  L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________________
    //      |    |    |    |    |    |    |    |   |   |
    //     L06  L05 *L04 *L03 *L09 *L10  L11  L12 L01 L02

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    auto& L01 = createL(pqTree, G);
    auto& L02 = createL(pqTree, G);

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);


    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());

    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L01.isEmpty(), true);
    EXPECT_EQ(L02.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test63){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #13 PP
    //                        Q01
    //                         |
    //                 _______Q02_______
    //                |                 |
    //         ______Q03_____     _____Q04______
    //        |    |    |    |   |    |    |    |
    //       L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________
    //      |    |    |    |    |    |    |    |
    //     L03  L04 *L05 *L06 *L12 *L11  L10  L09

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test64){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #14 PP
    //                        Q01
    //                         |
    //                 _______Q02_______
    //                |                 |
    //         ______Q03_____     _____Q04______
    //        |    |    |    |   |    |    |    |
    //       L03  L04 *L05 *L06*L09 *L10  L11  L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________
    //      |    |    |    |    |    |    |    |
    //     L03  L04 *L05 *L06 *L09 *L10  L11  L12

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());

    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test65){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #15 PP
    //                        Q01
    //                         |
    //                 _______Q02_______
    //                |                 |
    //         ______Q03_____     _____Q04______
    //        |    |    |    |   |    |    |    |
    //      *L03 *L04  L05  L06 L09  L10 *L11 *L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________
    //      |    |    |    |    |    |    |    |
    //     L06  L05 *L04 *L03 *L12 *L11  L10  L09

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);


    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ3()
 */
TEST_F(BLPlanarityTesterTests, Test66){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    // BLTree::templateQ3() positive #16 PP
    //                        Q01
    //                         |
    //                 _______Q02_______
    //                |                 |
    //         ______Q03_____     _____Q04______
    //        |    |    |    |   |    |    |    |
    //      *L03 *L04  L05  L06*L09 *L10  L11  L12
    //
    //                           _||_
    //                           \  /
    //                            \/
    //
    //                            Q01
    //                             |
    //       _____________________Q02__________
    //      |    |    |    |    |    |    |    |
    //     L06  L05 *L04 *L03 *L09 *L10  L11  L12

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_11 = createBLGraphNode(G);
    auto &track_12 = createBLGraphNode(G);
    auto &track_13 = createBLGraphNode(G);
    auto &track_14 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_11.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_12.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_13.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_14.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_23 = createBLGraphNode(G);
    auto &track_24 = createBLGraphNode(G);
    auto &track_25 = createBLGraphNode(G);
    auto &track_26 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_23.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_24.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_25.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_26.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    EXPECT_EQ(pqTree.templateQ3(Q02) ,true);

    // Check Q02
    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());

    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_11.backIt());
    expectedOrientInReversed.push_back(track_12.backIt());
    expectedOrientOutNorm.push_back(track_13.backIt());
    expectedOrientOutReversed.push_back(track_14.backIt());

    expectedOrientInNorm.push_back(track_23.backIt());
    expectedOrientInReversed.push_back(track_24.backIt());
    expectedOrientOutNorm.push_back(track_25.backIt());
    expectedOrientOutReversed.push_back(track_26.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test67){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //    *L01 *L02   ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //    *L01 *L02 *L03 *L04  L05  L06  L07  L08  L12  L11 *L10 *L09 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 6;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 8);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test68){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //    *L01 *L02   ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |
    //             *L03 *L04  L05  L06         L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //    *L01 *L02 *L03 *L04  L05  L06  L07  L08  L09  L10 *L11 *L12 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 6;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 8);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test69){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //    *L01 *L02   ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |
    //              L03  L04 *L05 *L06        *L09 *L10  L11  L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //    *L01 *L02 *L06 *L05  L04  L03  L07  L08  L12  L11 *L10 *L09 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 6;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 8);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test70){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |           |         |    |          |         |      |
    //    *L01 *L02   ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |
    //              L03  L04 *L05 *L06         L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //    *L01 *L02 *L06 *L05  L04  L03  L07  L08  L09  L10 *L11 *L12 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 6;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 8);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test71){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________
    //      |    |           |                 |         |      |
    //    *L01 *L02   ____ _Q03_____     _____Q04______ *L13  *L14
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //       ________________________Q02___________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |
    //    *L01 *L02 *L06 *L05  L04  L03  L09  L10 *L11 *L12 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 6;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 8);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test72){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02___
    //      |    |           |                 |
    //    *L01 *L02   ____ _Q03_____     _____Q04______
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06*L09 *L10  L11  L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //       ________________________Q02_________________
    //      |    |    |    |    |    |    |    |    |    |
    //    *L01 *L02 *L06 *L05  L04  L03  L12  L11 *L10 *L09

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;
  
    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 4;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 6);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test73){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________
    //                       |                 |         |      |
    //                ____ _Q03_____     _____Q04______ *L13  *L14
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //                 _____________Q02____________________________
    //                |    |    |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L09  L10 *L11 *L12 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 4;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 6);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test74){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___
    //                       |                 |
    //                ____ _Q03_____     _____Q04______
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06 L09  L10 *L11 *L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //                 _____________Q02__________________
    //                |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L09  L10 *L11 *L12

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());

    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test75){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___
    //                       |                 |
    //                ____ _Q03_____     _____Q04______
    //               |    |    |    |   |    |    |    |
    //              L03  L04 *L05 *L06*L09 *L10  L11  L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //                 _____________Q02__________________
    //                |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L12  L11 *L10 *L09

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
   pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test76){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___
    //                       |                 |
    //                ____ _Q03_____     _____Q04______
    //               |    |    |    |   |    |    |    |
    //             *L03 *L04  L05  L06*L09 *L10  L11  L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //                 _____________Q02__________________
    //                |    |    |    |    |    |    |    |
    //              *L03 *L04  L05  L06  L12  L11 *L10 *L09

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), Q04.backIt());
    createFullLink(L10.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());

    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L12.isEmpty(), true);
    EXPECT_EQ(L11.isEmpty(), true);
    EXPECT_EQ(L10.isFull(), true);
    EXPECT_EQ(L09.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L12.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L10.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test77){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___
    //                       |                 |
    //                ____ _Q03_____     _____Q04______
    //               |    |    |    |   |    |    |    |
    //             *L03 *L04  L05  L06 L09  L10 *L11 *L12
    //
    //                             _||_
    //                             \  /
    //                              \/
    //
    //                              Q01
    //                               |
    //                 _____________Q02__________________
    //                |    |    |    |    |    |    |    |
    //              *L03 *L04  L05  L06  L09  L10 *L11 *L12

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L04.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 2;
    Q02.mPertinentLeavesCount = 4;
    Q02.mSinglyPartialChild1 = Q03.backIt();
    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());

    EXPECT_EQ(L03.isFull(), true);
    EXPECT_EQ(L04.isFull(), true);
    EXPECT_EQ(L05.isEmpty(), true);
    EXPECT_EQ(L06.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test78){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_______________
    //      |    |           |         |    |       |      |
    //    *L01 *L02   ____ _Q03_____  L07  L08    *L13  *L14
    //               |    |    |    |
    //              L03  L04 *L05 *L06
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02______________
    //      |    |    |    |    |    |    |    |     |    |
    //    *L01 *L02 *L06 *L05  L04  L03  L07  L08  *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 5;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 6);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}



/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test79){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_______________________
    //      |    |                |    |          |         |      |
    //    *L01 *L02              L07  L08   _____Q04______ *L13  *L14
    //                                     |    |    |    |
    //                                    L09  L10 *L11 *L12
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________
    //      |    |   |    |    |    |    |    |    |    |
    //    *L01 *L02 L07  L08  L09  L10 *L11 *L12 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L09 = createL(pqTree, G);
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    auto& L12 = createL(pqTree, G);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = BLTreeNode::Full;
    L12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = BLTreeNode::SinglyPartial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mPertinentLeavesCount = 2;

    createFullLink(L11.backIt(), Q04.backIt());
    createFullLink(L12.backIt(), Q04.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q04.mOrientInNorm.push_back(track_05.backIt());
    Q04.mOrientInReversed.push_back(track_06.backIt());
    Q04.mOrientOutNorm.push_back(track_07.backIt());
    Q04.mOrientOutReversed.push_back(track_08.backIt());

    auto &track_Q04_1 = createBLGraphNode(G);
    auto &track_Q04_2 = createBLGraphNode(G);
    auto &track_Q04_3 = createBLGraphNode(G);
    auto &track_Q04_4 = createBLGraphNode(G);
    Q04.mAssumedOrientInNorm.push_back(track_Q04_1.backIt());
    Q04.mAssumedOrientInReversed.push_back(track_Q04_2.backIt());
    Q04.mAssumedOrientOutNorm.push_back(track_Q04_3.backIt());
    Q04.mAssumedOrientOutReversed.push_back(track_Q04_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 5;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q04.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L10.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L11.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L12.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L09.isEmpty(), true);
    EXPECT_EQ(L10.isEmpty(), true);
    EXPECT_EQ(L11.isFull(), true);
    EXPECT_EQ(L12.isFull(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_05.backIt());
    expectedOrientInReversed.push_back(track_06.backIt());
    expectedOrientOutNorm.push_back(track_07.backIt());
    expectedOrientOutReversed.push_back(track_08.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q04_2.backIt());
    expectedOrientInReversed.push_back(track_Q04_1.backIt());
    expectedOrientOutNorm.push_back(track_Q04_4.backIt());
    expectedOrientOutReversed.push_back(track_Q04_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L09.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L12.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L12.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 6);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test80){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_________
    //      |    |           |                |      |
    //    *L01 *L02   ____ _Q03_____        *L13  *L14
    //               |    |    |    |
    //              L03  L04 *L05 *L06
    //
    //                                  _||_
    //                                  \  /
    //                                   \/
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02_______
    //      |    |    |    |    |    |        |    |
    //    *L01 *L02 *L06 *L05  L04  L03     *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = BLTreeNode::Full;
    L05.mPertinentLeavesCount = 1;
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::SinglyPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L05.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 5;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mSinglyPartialChild1 = Q03.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L05.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L06.isFull(), true);
    EXPECT_EQ(L05.isFull(), true);
    EXPECT_EQ(L04.isEmpty(), true);
    EXPECT_EQ(L03.isEmpty(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_01.backIt());
    expectedOrientOutNorm.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_03.backIt());

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L05.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L05.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 6);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test81){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                      Q01
    //                       |
    //       _______________Q02_______________
    //      |    |        |    |       |      |
    //    *L01 *L02      L07  L08    *L13  *L14
    //
    //
    //                     _||_
    //                     \  /
    //                      \/
    //
    //                      Q01
    //                       |
    //       _______________Q02______
    //      |    |    |    |    |    |
    //    *L01 *L02  L07  L08 *L13 *L14

    pqTree.initializeForOneIteration();

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L07 = createL(pqTree, G);
    auto& L08 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 4;
    Q02.mPertinentLeavesCount = 4;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ4(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L08.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(L07.isEmpty(), true);
    EXPECT_EQ(L08.isEmpty(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L07.mFullChildrenSet, false);
    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}



/**  @brief tests BLPlanarizer::templateQ4()
 */
TEST_F(BLPlanarityTesterTests, Test82){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //
    //                      Q01
    //                       |
    //       _______________Q02________________
    //      |    |           |           |     |
    //    *L01 *L02   ____ _Q03_____   *L13  *L14
    //               |    |    |    |  
    //             *L03  L04  L05 *L06
    //
    //                     _||_
    //                     \  /
    //                      \/
    //
    //                      Q01
    //                       |
    //       _______________Q02________________
    //      |    |           |           |     |
    //    *L01 *L02   ____ _Q03_____   *L13  *L14
    //               |    |    |    |  
    //             *L03  L04  L05 *L06

    pqTree.initializeForOneIteration();

    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    L03.mPertinentLeavesCount = 1;
    auto& L04 = createL(pqTree, G);
    auto& L05 = createL(pqTree, G);
    auto& L06 = createL(pqTree, G);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = BLTreeNode::Full;
    L06.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = BLTreeNode::CDPartial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mPertinentLeavesCount = 2;

    createFullLink(L03.backIt(), Q03.backIt());
    createFullLink(L06.backIt(), Q03.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q03.mOrientInNorm.push_back(track_01.backIt());
    Q03.mOrientInReversed.push_back(track_02.backIt());
    Q03.mOrientOutNorm.push_back(track_03.backIt());
    Q03.mOrientOutReversed.push_back(track_04.backIt());

    auto &track_Q03_1 = createBLGraphNode(G);
    auto &track_Q03_2 = createBLGraphNode(G);
    auto &track_Q03_3 = createBLGraphNode(G);
    auto &track_Q03_4 = createBLGraphNode(G);
    Q03.mAssumedOrientInNorm.push_back(track_Q03_1.backIt());
    Q03.mAssumedOrientInReversed.push_back(track_Q03_2.backIt());
    Q03.mAssumedOrientOutNorm.push_back(track_Q03_3.backIt());
    Q03.mAssumedOrientOutReversed.push_back(track_Q03_4.backIt());

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;

    auto &track_L01_1 = createBLGraphNode(G);
    auto &track_L01_2 = createBLGraphNode(G);
    auto &track_L01_3 = createBLGraphNode(G);
    auto &track_L01_4 = createBLGraphNode(G);
    L01.mAssumedOrientInNorm.push_back(track_L01_1.backIt());
    L01.mAssumedOrientInReversed.push_back(track_L01_2.backIt());
    L01.mAssumedOrientOutNorm.push_back(track_L01_3.backIt());
    L01.mAssumedOrientOutReversed.push_back(track_L01_4.backIt());

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    L02.mPertinentLeavesCount = 1;

    auto &track_L02_1 = createBLGraphNode(G);
    auto &track_L02_2 = createBLGraphNode(G);
    auto &track_L02_3 = createBLGraphNode(G);
    auto &track_L02_4 = createBLGraphNode(G);
    L02.mAssumedOrientInNorm.push_back(track_L02_1.backIt());
    L02.mAssumedOrientInReversed.push_back(track_L02_2.backIt());
    L02.mAssumedOrientOutNorm.push_back(track_L02_3.backIt());
    L02.mAssumedOrientOutReversed.push_back(track_L02_4.backIt());

    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;

    auto &track_L13_1 = createBLGraphNode(G);
    auto &track_L13_2 = createBLGraphNode(G);
    auto &track_L13_3 = createBLGraphNode(G);
    auto &track_L13_4 = createBLGraphNode(G);
    L13.mAssumedOrientInNorm.push_back(track_L13_1.backIt());
    L13.mAssumedOrientInReversed.push_back(track_L13_2.backIt());
    L13.mAssumedOrientOutNorm.push_back(track_L13_3.backIt());
    L13.mAssumedOrientOutReversed.push_back(track_L13_4.backIt());

    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;

    auto &track_L14_1 = createBLGraphNode(G);
    auto &track_L14_2 = createBLGraphNode(G);
    auto &track_L14_3 = createBLGraphNode(G);
    auto &track_L14_4 = createBLGraphNode(G);
    L14.mAssumedOrientInNorm.push_back(track_L14_1.backIt());
    L14.mAssumedOrientInReversed.push_back(track_L14_2.backIt());
    L14.mAssumedOrientOutNorm.push_back(track_L14_3.backIt());
    L14.mAssumedOrientOutReversed.push_back(track_L14_4.backIt());

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());

    list<bool> Q02orientations;
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);
    Q02orientations.push_back(true);
    Q02orientations.push_back(true);
    Q02orientations.push_back(false);

    auto& Q02 = createQext(pqTree, Q02children, Q02orientations);
    Q02.mPertinentChildrenCount = 5;
    Q02.mPertinentLeavesCount = 6;

    createFullLink(L01.backIt(), Q02.backIt());
    createFullLink(L02.backIt(), Q02.backIt());
    createFullLink(L13.backIt(), Q02.backIt());
    createFullLink(L14.backIt(), Q02.backIt());

    Q02.mCDPartialChild = Q03.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;

    L02.mParent = Q02.backIt();
    Q03.mParent = Q02.backIt();
    L13.mParent = Q02.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateQ5(Q02, BLTree::NOT_FINAL_REDUCTION, earlyOut) ,
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

    EXPECT_EQ(Q02.mPertinentType, BLTreeNode::CDPartial);

    auto prevIt = pqTree.nil();
    auto curIt  = Q02.mEndChild1;
    EXPECT_EQ(curIt, L01.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L02.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, Q03.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L13.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L14.backIt());

    EXPECT_EQ(L01.isFull(), true);
    EXPECT_EQ(L02.isFull(), true);
    EXPECT_EQ(Q03.isCDPartial(), true);
    EXPECT_EQ(L13.isFull(), true);
    EXPECT_EQ(L14.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_L01_1.backIt());
    expectedOrientInReversed.push_back(track_L01_2.backIt());
    expectedOrientOutNorm.push_back(track_L01_3.backIt());
    expectedOrientOutReversed.push_back(track_L01_4.backIt());

    expectedOrientInNorm.push_back(track_L02_2.backIt());
    expectedOrientInReversed.push_back(track_L02_1.backIt());
    expectedOrientOutNorm.push_back(track_L02_4.backIt());
    expectedOrientOutReversed.push_back(track_L02_3.backIt());

    expectedOrientInNorm.push_back(track_Q03_1.backIt());
    expectedOrientInReversed.push_back(track_Q03_2.backIt());
    expectedOrientOutNorm.push_back(track_Q03_3.backIt());
    expectedOrientOutReversed.push_back(track_Q03_4.backIt());

    expectedOrientInNorm.push_back(track_L13_1.backIt());
    expectedOrientInReversed.push_back(track_L13_2.backIt());
    expectedOrientOutNorm.push_back(track_L13_3.backIt());
    expectedOrientOutReversed.push_back(track_L13_4.backIt());

    expectedOrientInNorm.push_back(track_L14_2.backIt());
    expectedOrientInReversed.push_back(track_L14_1.backIt());
    expectedOrientOutNorm.push_back(track_L14_4.backIt());
    expectedOrientOutReversed.push_back(track_L14_3.backIt());

    EXPECT_EQ(checkOrientation(Q02,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,expectedOrientOutReversed), true);


    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), Q03.backIt()), true);
    EXPECT_EQ(L04.mFullChildrenSet, false);
    EXPECT_EQ(L05.mFullChildrenSet, false);
    EXPECT_EQ(L06.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L06.backIt(), Q03.backIt()), true);
    EXPECT_EQ(Q03.mFullChildren.size(), 2);

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q03.mFullChildrenSet, false);
    EXPECT_EQ(L13.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L13.backIt(), Q02.backIt()), true);
    EXPECT_EQ(L14.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L14.backIt(), Q02.backIt()), true);
    EXPECT_EQ(Q02.mFullChildren.size(), 4);
    EXPECT_EQ(Q02.mFullChildrenSet, false);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

}

