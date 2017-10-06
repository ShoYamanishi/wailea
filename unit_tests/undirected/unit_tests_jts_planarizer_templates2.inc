


/**  @brief tests JTSTree::templateP7()
 */
TEST_F(JTSPlanarizerTests, Test55) {


    JTSTree& pqTree = getPQTree();

    //   P7 positive #1
    //
    //    ___Q22__                          ___Q22__
    //   |   |    |                        |    |   |
    // *L83*L82 _P26_____________    =>  *L83*L82  P26
    //        /  | \   \   \     \               /  |  \
    //      L70  |*L71 L72*L73    |           *L71 Q23*L73
    //        __Q23___       ____Q24___       ______|________________________
    //       /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //     L74 L75*L76*L77 L78 L79*L80*L81 *L81*L80 L79 L78 P27 L74 L75*L76*L77
    //                                                      / \
    //                                                    L70 L72
    initializeForOneIteration(pqTree);

    auto& L70 = createL(pqTree);
    auto& L71 = createL(pqTree);
    L71.mGeneration = pqTree.mGeneration;
    L71.mPertinentType = JTSTreeNode::Full;
    auto& L72 = createL(pqTree);
    auto& L73 = createL(pqTree);
    L73.mGeneration = pqTree.mGeneration;
    L73.mPertinentType = JTSTreeNode::Full;
    auto& L74 = createL(pqTree);
    auto& L75 = createL(pqTree);
    auto& L76 = createL(pqTree);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = JTSTreeNode::Full;
    auto& L77 = createL(pqTree);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = JTSTreeNode::Full;
    auto& L78 = createL(pqTree);
    auto& L79 = createL(pqTree);
    auto& L80 = createL(pqTree);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = JTSTreeNode::Full;
    auto& L81 = createL(pqTree);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Partial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mFullChildrenCount = 2;
    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = JTSTreeNode::Partial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mFullChildrenCount = 2;

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(L71.backIt());
    P26children.push_back(L72.backIt());
    P26children.push_back(L73.backIt());
    P26children.push_back(Q24.backIt());
    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 4;
    P26.mFullChildrenCount = 2;
    P26.mPartialChild1 = Q23.backIt();
    P26.mPartialChild2 = Q24.backIt();

    auto& L82 = createL(pqTree);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = JTSTreeNode::Full;

    auto& L83 = createL(pqTree);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(L83.backIt());
    Q22children.push_back(P26.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentChildrenCount = 3;

    EXPECT_EQ(templateP7(pqTree, P26), true);

    EXPECT_EQ(P26.mCDPartialChild, Q23.backIt());
    EXPECT_EQ(P26.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P26.mPertinentChildrenCount, 3);
    EXPECT_EQ(P26.mFullChildrenCount, 2);
    EXPECT_EQ(P26.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P26.mPartialChild2, pqTree.nil());

    EXPECT_EQ(P26.mChildren.size(), 3);
    auto cIt = P26.mChildren.begin();    
    EXPECT_EQ(Q23.backIt(), *cIt);
    EXPECT_EQ(Q23.mChildIt, cIt);
    EXPECT_EQ(Q23.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L71.backIt(), *cIt);
    EXPECT_EQ(L71.mChildIt, cIt);
    EXPECT_EQ(L71.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L73.backIt(), *cIt);
    EXPECT_EQ(L73.mChildIt, cIt);
    EXPECT_EQ(L73.mParent, P26.backIt());

    EXPECT_EQ(Q23.mChildren.size(), 9);
    cIt = Q23.mChildren.begin();
    EXPECT_EQ(L81.backIt(), *cIt);
    EXPECT_EQ(L81.mChildIt, cIt);
    EXPECT_EQ(L81.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L80.backIt(), *cIt);
    EXPECT_EQ(L80.mChildIt, cIt);
    EXPECT_EQ(L80.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L79.backIt(), *cIt);
    EXPECT_EQ(L79.mChildIt, cIt);
    EXPECT_EQ(L79.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L78.backIt(), *cIt);
    EXPECT_EQ(L78.mChildIt, cIt);
    EXPECT_EQ(L78.mParent, Q23.backIt());
    cIt++;
    auto& Q23_C05 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(Q23_C05.mChildIt, cIt);
    EXPECT_EQ(Q23_C05.mParent, Q23.backIt());

    cIt++;
    EXPECT_EQ(L74.backIt(), *cIt);
    EXPECT_EQ(L74.mChildIt, cIt);
    EXPECT_EQ(L74.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L75.backIt(), *cIt);
    EXPECT_EQ(L75.mChildIt, cIt);
    EXPECT_EQ(L75.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L76.backIt(), *cIt);
    EXPECT_EQ(L76.mChildIt, cIt);
    EXPECT_EQ(L76.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L77.backIt(), *cIt);
    EXPECT_EQ(L77.mChildIt, cIt);
    EXPECT_EQ(L77.mParent, Q23.backIt());

    EXPECT_EQ(Q23.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q23.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(Q23.mFullChildrenCount, 4);
    EXPECT_EQ(Q23.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mPartialChild2, pqTree.nil());

    cIt = Q23_C05.mChildren.begin();
    EXPECT_EQ(L70.backIt(), *cIt);
    EXPECT_EQ(L70.mChildIt, cIt);
    EXPECT_EQ(L70.mParent, Q23_C05.backIt());
    cIt++;
    EXPECT_EQ(L72.backIt(), *cIt);
    EXPECT_EQ(L72.mChildIt, cIt);
    EXPECT_EQ(L72.mParent, Q23_C05.backIt());
    EXPECT_EQ(Q23_C05.mFullChildrenCount, 0);
    EXPECT_EQ(Q23_C05.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23_C05.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q23_C05.mPertinentType, JTSTreeNode::Empty);
    EXPECT_EQ(Q23_C05.mNodeType, JTSTreeNode::PType);

    EXPECT_EQ(Q22.mCDPartialChild, P26.backIt());

}


/**  @brief tests JTSTree::templateP7()
 */
TEST_F(JTSPlanarizerTests, Test56) {


    JTSTree& pqTree = getPQTree();

    //   P7 positive #2
    //
    //    ___Q22__                          ___Q22__
    //   |   |    |                        |    |   |
    // *L83*L82 _P26_____________    =>  *L83*L82  P26
    //        /  | \       \     \               /  |  \
    //      L70  |*L71    *L73    |           *L71 Q23*L73
    //        __Q23___       ____Q24___       ______|________________________
    //       /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //    *L74*L75 L76 L77 L78 L79*L80*L81 *L74*L75 L76 L77 L70 L78 L79*L80*L81

    initializeForOneIteration(pqTree);

    auto& L70 = createL(pqTree);
    auto& L71 = createL(pqTree);
    L71.mGeneration = pqTree.mGeneration;
    L71.mPertinentType = JTSTreeNode::Full;
    auto& L73 = createL(pqTree);
    L73.mGeneration = pqTree.mGeneration;
    L73.mPertinentType = JTSTreeNode::Full;
    auto& L74 = createL(pqTree);
    L74.mGeneration = pqTree.mGeneration;
    L74.mPertinentType = JTSTreeNode::Full;
    auto& L75 = createL(pqTree);
    L75.mGeneration = pqTree.mGeneration;
    L75.mPertinentType = JTSTreeNode::Full;
    auto& L76 = createL(pqTree);
    auto& L77 = createL(pqTree);
    auto& L78 = createL(pqTree);
    auto& L79 = createL(pqTree);
    auto& L80 = createL(pqTree);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = JTSTreeNode::Full;
    auto& L81 = createL(pqTree);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Partial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mFullChildrenCount = 2;
    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = JTSTreeNode::Partial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mFullChildrenCount = 2;

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(L71.backIt());
    P26children.push_back(L73.backIt());
    P26children.push_back(Q24.backIt());
    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 4;
    P26.mFullChildrenCount = 2;
    P26.mPartialChild1 = Q23.backIt();
    P26.mPartialChild2 = Q24.backIt();

    auto& L82 = createL(pqTree);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = JTSTreeNode::Full;

    auto& L83 = createL(pqTree);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(L83.backIt());
    Q22children.push_back(P26.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentChildrenCount = 3;

    EXPECT_EQ(templateP7(pqTree, P26), true);

    EXPECT_EQ(P26.mCDPartialChild, Q23.backIt());
    EXPECT_EQ(P26.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P26.mPertinentChildrenCount, 3);
    EXPECT_EQ(P26.mFullChildrenCount, 2);
    EXPECT_EQ(P26.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P26.mPartialChild2, pqTree.nil());

    EXPECT_EQ(P26.mChildren.size(), 3);
    auto cIt = P26.mChildren.begin();    
    EXPECT_EQ(Q23.backIt(), *cIt);
    EXPECT_EQ(Q23.mChildIt, cIt);
    EXPECT_EQ(Q23.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L71.backIt(), *cIt);
    EXPECT_EQ(L71.mChildIt, cIt);
    EXPECT_EQ(L71.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L73.backIt(), *cIt);
    EXPECT_EQ(L73.mChildIt, cIt);
    EXPECT_EQ(L73.mParent, P26.backIt());

    EXPECT_EQ(Q23.mChildren.size(), 9);
    cIt = Q23.mChildren.begin();
    EXPECT_EQ(L74.backIt(), *cIt);
    EXPECT_EQ(L74.mChildIt, cIt);
    EXPECT_EQ(L74.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L75.backIt(), *cIt);
    EXPECT_EQ(L75.mChildIt, cIt);
    EXPECT_EQ(L75.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L76.backIt(), *cIt);
    EXPECT_EQ(L76.mChildIt, cIt);
    EXPECT_EQ(L76.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L77.backIt(), *cIt);
    EXPECT_EQ(L77.mChildIt, cIt);
    EXPECT_EQ(L77.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L70.backIt(), *cIt);
    EXPECT_EQ(L70.mChildIt, cIt);
    EXPECT_EQ(L70.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L78.backIt(), *cIt);
    EXPECT_EQ(L78.mChildIt, cIt);
    EXPECT_EQ(L78.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L79.backIt(), *cIt);
    EXPECT_EQ(L79.mChildIt, cIt);
    EXPECT_EQ(L79.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L80.backIt(), *cIt);
    EXPECT_EQ(L80.mChildIt, cIt);
    EXPECT_EQ(L80.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L81.backIt(), *cIt);
    EXPECT_EQ(L81.mChildIt, cIt);
    EXPECT_EQ(L81.mParent, Q23.backIt());

    EXPECT_EQ(Q23.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q23.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(Q23.mFullChildrenCount, 4);
    EXPECT_EQ(Q23.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mPartialChild2, pqTree.nil());

    EXPECT_EQ(Q22.mCDPartialChild, P26.backIt());

}


/**  @brief tests JTSTree::templateP7()
 */
TEST_F(JTSPlanarizerTests, Test57) {


    JTSTree& pqTree = getPQTree();

    //   P7 positive #3
    //
    //    ___Q22__                              ___Q22__
    //   |   |    |                            |    |   |
    // *L83*L82 _P26_____________         => *L83*L82  P26
    //           | \       \     \                   /  |  \
    //           |*L71    *L73    |               *L71 Q23*L73
    //        __Q23___       ____Q24___           ______|____________________
    //       /  /  \  \     /   /   \  \         |   |   |   |   |   |   |   |
    //     L74 L75*L76*L77*L78*L79 L80 L81     *L78*L79 L80 L81 L74 L75*L76*L77

    initializeForOneIteration(pqTree);

    auto& L71 = createL(pqTree);
    L71.mGeneration = pqTree.mGeneration;
    L71.mPertinentType = JTSTreeNode::Full;
    auto& L73 = createL(pqTree);
    L73.mGeneration = pqTree.mGeneration;
    L73.mPertinentType = JTSTreeNode::Full;
    auto& L74 = createL(pqTree);
    auto& L75 = createL(pqTree);
    auto& L76 = createL(pqTree);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = JTSTreeNode::Full;
    auto& L77 = createL(pqTree);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = JTSTreeNode::Full;
    auto& L78 = createL(pqTree);
    L78.mGeneration = pqTree.mGeneration;
    L78.mPertinentType = JTSTreeNode::Full;
    auto& L79 = createL(pqTree);
    L79.mGeneration = pqTree.mGeneration;
    L79.mPertinentType = JTSTreeNode::Full;
    auto& L80 = createL(pqTree);
    auto& L81 = createL(pqTree);
    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Partial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mFullChildrenCount = 2;
    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = JTSTreeNode::Partial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mFullChildrenCount = 2;

    list<node_list_it_t> P26children;
    P26children.push_back(Q23.backIt());
    P26children.push_back(L71.backIt());
    P26children.push_back(L73.backIt());
    P26children.push_back(Q24.backIt());
    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 4;
    P26.mFullChildrenCount = 2;
    P26.mPartialChild1 = Q23.backIt();
    P26.mPartialChild2 = Q24.backIt();

    auto& L82 = createL(pqTree);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = JTSTreeNode::Full;

    auto& L83 = createL(pqTree);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(L83.backIt());
    Q22children.push_back(P26.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentChildrenCount = 3;

    EXPECT_EQ(templateP7(pqTree, P26), true);

    EXPECT_EQ(P26.mCDPartialChild, Q23.backIt());
    EXPECT_EQ(P26.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P26.mPertinentChildrenCount, 3);
    EXPECT_EQ(P26.mFullChildrenCount, 2);
    EXPECT_EQ(P26.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P26.mPartialChild2, pqTree.nil());

    EXPECT_EQ(P26.mChildren.size(), 3);
    auto cIt = P26.mChildren.begin();    
    EXPECT_EQ(Q23.backIt(), *cIt);
    EXPECT_EQ(Q23.mChildIt, cIt);
    EXPECT_EQ(Q23.mParent, P26.backIt());
    cIt++;

    EXPECT_EQ(L71.backIt(), *cIt);
    EXPECT_EQ(L71.mChildIt, cIt);
    EXPECT_EQ(L71.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L73.backIt(), *cIt);
    EXPECT_EQ(L73.mChildIt, cIt);
    EXPECT_EQ(L73.mParent, P26.backIt());

    EXPECT_EQ(Q23.mChildren.size(), 8);
    cIt = Q23.mChildren.begin();

    EXPECT_EQ(L78.backIt(), *cIt);
    EXPECT_EQ(L78.mChildIt, cIt);
    EXPECT_EQ(L78.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L79.backIt(), *cIt);
    EXPECT_EQ(L79.mChildIt, cIt);
    EXPECT_EQ(L79.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L80.backIt(), *cIt);
    EXPECT_EQ(L80.mChildIt, cIt);
    EXPECT_EQ(L80.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L81.backIt(), *cIt);
    EXPECT_EQ(L81.mChildIt, cIt);
    EXPECT_EQ(L81.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L74.backIt(), *cIt);
    EXPECT_EQ(L74.mChildIt, cIt);
    EXPECT_EQ(L74.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L75.backIt(), *cIt);
    EXPECT_EQ(L75.mChildIt, cIt);
    EXPECT_EQ(L75.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L76.backIt(), *cIt);
    EXPECT_EQ(L76.mChildIt, cIt);
    EXPECT_EQ(L76.mParent, Q23.backIt());
    cIt++;
    EXPECT_EQ(L77.backIt(), *cIt);
    EXPECT_EQ(L77.mChildIt, cIt);
    EXPECT_EQ(L77.mParent, Q23.backIt());

    EXPECT_EQ(Q23.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q23.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(Q23.mFullChildrenCount, 4);
    EXPECT_EQ(Q23.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mPartialChild2, pqTree.nil());

    EXPECT_EQ(Q22.mCDPartialChild, P26.backIt());

}


/**  @brief tests JTSTree::templateP7()
 */
TEST_F(JTSPlanarizerTests, Test58) {


    JTSTree& pqTree = getPQTree();

    //   P7 positive #4
    //
    //    ___Q22__                          ___Q22__
    //   |   |    |                        |    |   |
    // *L83*L82 _P26_____________    =>  *L83*L82  P26
    //        /  |     \         \            ______|________________________ 
    //      L70  |     L72        |          |   |   |   |   |   |   |   |   |
    //        __Q23___       ____Q24___    *L81*L80 L79 L78 P27 L74 L75*L76*L77
    //       /  /  \  \     /   /   \  \                    / \
    //     L74 L75*L76*L77 L78 L79*L80*L81                L70 L72
    //                                           

    initializeForOneIteration(pqTree);

    auto& L70 = createL(pqTree);
    auto& L72 = createL(pqTree);
    auto& L74 = createL(pqTree);
    auto& L75 = createL(pqTree);
    auto& L76 = createL(pqTree);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = JTSTreeNode::Full;
    auto& L77 = createL(pqTree);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = JTSTreeNode::Full;
    auto& L78 = createL(pqTree);
    auto& L79 = createL(pqTree);
    auto& L80 = createL(pqTree);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = JTSTreeNode::Full;
    auto& L81 = createL(pqTree);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Partial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mFullChildrenCount = 2;
    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = JTSTreeNode::Partial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mFullChildrenCount = 2;

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(L72.backIt());
    P26children.push_back(Q24.backIt());
    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 2;
    P26.mFullChildrenCount = 0;
    P26.mPartialChild1 = Q23.backIt();
    P26.mPartialChild2 = Q24.backIt();

    auto& L82 = createL(pqTree);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = JTSTreeNode::Full;

    auto& L83 = createL(pqTree);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(L83.backIt());
    Q22children.push_back(P26.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentChildrenCount = 3;

    EXPECT_EQ(templateP7(pqTree, P26), true);

    //   P7 positive #4
    //
    //    ___Q22__                          ___Q22__
    //   |   |    |                        |    |   |
    // *L83*L82 _P26_____________    =>  *L83*L82  P26
    //        /  |     \         \            ______|________________________ 
    //      L70  |     L72        |          |   |   |   |   |   |   |   |   |
    //        __Q23___       ____Q24___    *L81*L80 L79 L78 P27 L74 L75*L76*L77
    //       /  /  \  \     /   /   \  \                    / \
    //     L74 L75*L76*L77 L78 L79*L80*L81                L70 L72
    //                                           

    EXPECT_EQ(P26.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(P26.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P26.mPertinentChildrenCount, 4);
    EXPECT_EQ(P26.mFullChildrenCount, 4);
    EXPECT_EQ(P26.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P26.mPartialChild2, pqTree.nil());

    EXPECT_EQ(P26.mChildren.size(), 9);
    auto cIt = P26.mChildren.begin();
    EXPECT_EQ(L81.backIt(), *cIt);
    EXPECT_EQ(L81.mChildIt, cIt);
    EXPECT_EQ(L81.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L80.backIt(), *cIt);
    EXPECT_EQ(L80.mChildIt, cIt);
    EXPECT_EQ(L80.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L79.backIt(), *cIt);
    EXPECT_EQ(L79.mChildIt, cIt);
    EXPECT_EQ(L79.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L78.backIt(), *cIt);
    EXPECT_EQ(L78.mChildIt, cIt);
    EXPECT_EQ(L78.mParent, P26.backIt());
    cIt++;
    auto& P26_C05 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P26_C05.mChildIt, cIt);
    EXPECT_EQ(P26_C05.mParent, P26.backIt());

    cIt++;
    EXPECT_EQ(L74.backIt(), *cIt);
    EXPECT_EQ(L74.mChildIt, cIt);
    EXPECT_EQ(L74.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L75.backIt(), *cIt);
    EXPECT_EQ(L75.mChildIt, cIt);
    EXPECT_EQ(L75.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L76.backIt(), *cIt);
    EXPECT_EQ(L76.mChildIt, cIt);
    EXPECT_EQ(L76.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L77.backIt(), *cIt);
    EXPECT_EQ(L77.mChildIt, cIt);
    EXPECT_EQ(L77.mParent, P26.backIt());

    EXPECT_EQ(P26.mPertinentChildrenCount, 4);
    EXPECT_EQ(P26.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P26.mFullChildrenCount, 4);
    EXPECT_EQ(P26.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P26.mPartialChild2, pqTree.nil());

    cIt = P26_C05.mChildren.begin();
    EXPECT_EQ(L70.backIt(), *cIt);
    EXPECT_EQ(L70.mChildIt, cIt);
    EXPECT_EQ(L70.mParent, P26_C05.backIt());
    cIt++;
    EXPECT_EQ(L72.backIt(), *cIt);
    EXPECT_EQ(L72.mChildIt, cIt);
    EXPECT_EQ(L72.mParent, P26_C05.backIt());
    EXPECT_EQ(P26_C05.mFullChildrenCount, 0);
    EXPECT_EQ(P26_C05.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P26_C05.mPartialChild2, pqTree.nil());
    EXPECT_EQ(P26_C05.mPertinentType, JTSTreeNode::Empty);
    EXPECT_EQ(P26_C05.mNodeType, JTSTreeNode::PType);

    EXPECT_EQ(Q22.mCDPartialChild, P26.backIt());

}


/**  @brief tests JTSTree::templateP8()
 */
TEST_F(JTSPlanarizerTests, Test59) {


    JTSTree& pqTree = getPQTree();

    //   P8 positive #1
    //
    //               P11
    //   _____________|________________
    //  |             |                |
    //*L12            |              *L13
    //               P10
    //      __________|____________
    //     |          |        |   |
    //   *L05   __ __Q08____ *L06*L07
    //         |    |   |   |
    //        L01* L02 L03*L04
    //                                           

    initializeForOneIteration(pqTree);

    auto& L01 = createL(pqTree);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;

    auto& L05 = createL(pqTree);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q08children;
    Q08children.push_back(L01.backIt());
    Q08children.push_back(L02.backIt());
    Q08children.push_back(L03.backIt());
    Q08children.push_back(L04.backIt());
    auto& Q08 = createQ(pqTree, Q08children);
    Q08.mGeneration = pqTree.mGeneration;
    Q08.mPertinentType = JTSTreeNode::CDPartial;
    Q08.mPertinentChildrenCount = 2;
    Q08.mFullChildrenCount = 2;

    auto& L08 = createL(pqTree);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P10children;
    P10children.push_back(L05.backIt());
    P10children.push_back(Q08.backIt());
    P10children.push_back(L06.backIt());
    P10children.push_back(L07.backIt());
    auto& P10 = createP(pqTree, P10children);
    P10.mCDPartialChild = Q08.backIt();
    P10.mGeneration = pqTree.mGeneration;
    P10.mPertinentType = JTSTreeNode::Partial;
    P10.mPertinentChildrenCount = 4;
    P10.mFullChildrenCount = 3;

    auto& L12 = createL(pqTree);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;

    auto& L13 = createL(pqTree);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P11children;
    P11children.push_back(L12.backIt());
    P11children.push_back(P10.backIt());
    P11children.push_back(L13.backIt());
    auto& P11 = createQ(pqTree, P11children);
    P11.mGeneration = pqTree.mGeneration;
    P11.mPertinentType = JTSTreeNode::Partial;
    P11.mPertinentChildrenCount = 3;
    P11.mFullChildrenCount = 2;

    EXPECT_EQ(templateP8(pqTree, P10), true);

    EXPECT_EQ(P10.mCDPartialChild, Q08.backIt());
    EXPECT_EQ(P10.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P10.mPertinentChildrenCount, 4);
    EXPECT_EQ(P10.mFullChildrenCount, 3);
    EXPECT_EQ(P10.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P10.mPartialChild2, pqTree.nil());

    EXPECT_EQ(P11.mCDPartialChild, P10.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test60) {


    JTSTree& pqTree = getPQTree();

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
    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

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
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);


    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test61) {


    JTSTree& pqTree = getPQTree();


    //     Q3
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

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

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
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    //     Q3
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

    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test62) {


    JTSTree& pqTree = getPQTree();

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
    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

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
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);


    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}



/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test63) {


    JTSTree& pqTree = getPQTree();

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
    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

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
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);


    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}



/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test64) {


    JTSTree& pqTree = getPQTree();

    //
    //                                 Q01
    //                                  |
    //       __________________________Q02__________________________
    //      |    |           |                     |         |      |
    //    *L01 *L02   ____ _Q03_____         _____Q04______ *L13  *L14
    //               |    |    |    |       |    |    |    |    
    //             *L03 *L04  L05  L06    *L09 *L10  L11  L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |    |    |    |    |         |    |    |    |    |    |
    //    *L01 *L02 *L03 *L04  L05  L06       L12  L11 *L10 *L09 *L13 *L14
    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test65) {


    JTSTree& pqTree = getPQTree();

    //
    //                                 Q01
    //                                  |
    //       __________________________Q02__________________________
    //      |    |           |                     |         |      |
    //    *L01 *L02   ____ _Q03_____         _____Q04______ *L13  *L14
    //               |    |    |    |       |    |    |    |    
    //             *L03 *L04  L05  L06     L09  L10 *L11 *L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |    |    |    |    |         |    |    |    |    |    |
    //    *L01 *L02 *L03 *L04  L05  L06       L09  L10 *L11 *L12 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test66) {


    JTSTree& pqTree = getPQTree();

    //
    //                                 Q01
    //                                  |
    //       __________________________Q02__________________________
    //      |    |           |                     |         |      |
    //    *L01 *L02   ____ _Q03_____         _____Q04______ *L13  *L14
    //               |    |    |    |       |    |    |    |    
    //              L03  L04 *L05 *L06    *L09 *L10  L11  L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |    |    |    |    |         |    |    |    |    |    |
    //    *L01 *L02 *L06 *L05  L04  L03       L12  L11 *L10 *L09 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}



/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test67) {


    JTSTree& pqTree = getPQTree();

    //
    //                                 Q01
    //                                  |
    //       __________________________Q02__________________________
    //      |    |           |                     |         |      |
    //    *L01 *L02   ____ _Q03_____         _____Q04______ *L13  *L14
    //               |    |    |    |       |    |    |    |    
    //              L03  L04 *L05 *L06     L09  L10 *L11 *L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                   Q01
    //                                    |
    //       ____________________________Q02____________________________
    //      |    |    |    |    |    |         |    |    |    |    |    |
    //    *L01 *L02 *L06 *L05  L04  L03       L09  L10 *L11 *L12 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 6;
    Q02.mFullChildrenCount = 4;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 8);
    EXPECT_EQ(Q02.mFullChildrenCount, 8);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L01.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L02.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test68) {


    JTSTree& pqTree = getPQTree();


    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________________
    //                       |         |    |          |         |      |
    //                ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |    
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_________________________________
    //                |    |    |    |    |    |    |    |    |    |    |    |
    //              *L03 *L04  L05  L06  L07  L08  L12  L11 *L10 *L09 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}



/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test69) {


    JTSTree& pqTree = getPQTree();


    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________________
    //                       |         |    |          |         |      |
    //                ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |    
    //             *L03 *L04  L05  L06         L09  L10 *L11 *L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_________________________________
    //                |    |    |    |    |    |    |    |    |    |    |    |
    //              *L03 *L04  L05  L06  L07  L08  L09  L10 *L11 *L12 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test70) {


    JTSTree& pqTree = getPQTree();


    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________________
    //                       |         |    |          |         |      |
    //                ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |    
    //              L03  L04 *L05 *L06        *L09 *L10  L11  L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_________________________________
    //                |    |    |    |    |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L07  L08  L12  L11 *L10 *L09 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test71) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________________
    //                       |         |    |          |         |      |
    //                ____ _Q03_____  L07  L08   _____Q04______ *L13  *L14
    //               |    |    |    |           |    |    |    |    
    //              L03  L04 *L05 *L06         L09  L10 *L11 *L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_________________________________
    //                |    |    |    |    |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L07  L08  L09  L10 *L11 *L12 *L13 *L14

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    L13.mGeneration    = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    Q02children.push_back(L13.backIt());
    Q02children.push_back(L14.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test72) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________
    //                       |         |    |          |
    //                ____ _Q03_____  L07  L08   _____Q04______
    //               |    |    |    |           |    |    |    |    
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_______________________
    //                |    |    |    |    |    |    |    |    |    |
    //              *L03 *L04  L05  L06  L07  L08  L12  L11 *L10 *L09

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 10);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test73) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________
    //                       |         |    |          |
    //                ____ _Q03_____  L07  L08   _____Q04______
    //               |    |    |    |           |    |    |    |    
    //             *L03 *L04  L05  L06         L09  L10 *L11 *L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_______________________
    //                |    |    |    |    |    |    |    |    |    |
    //              *L03 *L04  L05  L06  L07  L08  L09  L10 *L11 *L12

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 10);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test74) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________
    //                       |         |    |          |
    //                ____ _Q03_____  L07  L08   _____Q04______
    //               |    |    |    |           |    |    |    |    
    //              L03  L04 *L05 *L06        *L09 *L10  L11  L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_______________________
    //                |    |    |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L07  L08  L12  L11 *L10 *L09

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 10);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test75) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________
    //                       |         |    |          |
    //                ____ _Q03_____  L07  L08   _____Q04______
    //               |    |    |    |           |    |    |    |    
    //              L03  L04 *L05 *L06         L09  L10 *L11 *L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02_______________________
    //                |    |    |    |    |    |    |    |    |    |
    //              *L06 *L05  L04  L03  L07  L08  L09  L10 *L11 *L12

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 10);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test76) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___________
    //                       |                         |
    //                ____ _Q03_____             _____Q04______
    //               |    |    |    |           |    |    |    |    
    //             *L03 *L04  L05  L06        *L09 *L10  L11  L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02___________________
    //                |    |    |    |          |    |    |    |
    //              *L03 *L04  L05  L06        L12  L11 *L10 *L09

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test77) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___________
    //                       |                         |
    //                ____ _Q03_____             _____Q04______
    //               |    |    |    |           |    |    |    |    
    //             *L03 *L04  L05  L06         L09  L10 *L11 *L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02___________________
    //                |    |    |    |          |    |    |    |
    //              *L03 *L04  L05  L06        L09  L10 *L11 *L12

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration    = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test78) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___________
    //                       |                         |
    //                ____ _Q03_____             _____Q04______
    //               |    |    |    |           |    |    |    |    
    //              L03  L04 *L05 *L06        *L09 *L10  L11  L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02___________________
    //                |    |    |    |          |    |    |    |
    //              *L06 *L05  L04  L03        L12  L11 *L10 *L09

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}


/**  @brief tests JTSTree::templateQ4()
 */
TEST_F(JTSPlanarizerTests, Test79) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___________
    //                       |                         |
    //                ____ _Q03_____             _____Q04______
    //               |    |    |    |           |    |    |    |    
    //              L03  L04 *L05 *L06         L09  L10 *L11 *L12
    // 
    //                                  _||_
    //                                  \  /
    //                                   \/ 
    //
    //                                   Q01
    //                                    |
    //                 __________________Q02___________________
    //                |    |    |    |          |    |    |    |
    //              *L06 *L05  L04  L03        L09  L10 *L11 *L12

    initializeForOneIteration(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    L05.mGeneration    = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q03children;
    Q03children.push_back(L03.backIt());
    Q03children.push_back(L04.backIt());
    Q03children.push_back(L05.backIt());
    Q03children.push_back(L06.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration    = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mPertinentChildrenCount = 2;
    Q03.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L09.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    Q04children.push_back(L12.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration    = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mPertinentChildrenCount = 2;
    Q04.mFullChildrenCount = 2;

    list<node_list_it_t> Q02children;
    Q02children.push_back(Q03.backIt());
    Q02children.push_back(Q04.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ4(pqTree, Q02), true);

    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::CDPartial);

    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L03.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);

    EXPECT_EQ(Q01.mCDPartialChild, Q02.backIt());

}



/**  @brief tests JTSTree::templateQ5()
 */
TEST_F(JTSPlanarizerTests, Test80) {


    JTSTree& pqTree = getPQTree();

    //   P8 positive #1
    //
    //               P11
    //   _____________|________________
    //  |             |                |
    //*L12            |              *L13
    //               Q10
    //      __________|____________
    //     |          |        |   |
    //   *L05   __ __Q08____ *L06*L07
    //         |    |   |   |
    //        L01* L02 L03*L04
    //                                           

    initializeForOneIteration(pqTree);

    auto& L01 = createL(pqTree);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;

    auto& L05 = createL(pqTree);
    L05.mGeneration = pqTree.mGeneration;
    L05.mPertinentType = JTSTreeNode::Full;
    auto& L06 = createL(pqTree);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q08children;
    Q08children.push_back(L01.backIt());
    Q08children.push_back(L02.backIt());
    Q08children.push_back(L03.backIt());
    Q08children.push_back(L04.backIt());
    auto& Q08 = createQ(pqTree, Q08children);
    Q08.mGeneration = pqTree.mGeneration;
    Q08.mPertinentType = JTSTreeNode::CDPartial;
    Q08.mPertinentChildrenCount = 2;
    Q08.mFullChildrenCount = 2;

    auto& L08 = createL(pqTree);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q10children;
    Q10children.push_back(L05.backIt());
    Q10children.push_back(Q08.backIt());
    Q10children.push_back(L06.backIt());
    Q10children.push_back(L07.backIt());
    auto& Q10 = createQ(pqTree, Q10children);
    Q10.mCDPartialChild = Q08.backIt();
    Q10.mGeneration = pqTree.mGeneration;
    Q10.mPertinentType = JTSTreeNode::Partial;
    Q10.mPertinentChildrenCount = 4;
    Q10.mFullChildrenCount = 3;

    auto& L12 = createL(pqTree);
    L12.mGeneration = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;

    auto& L13 = createL(pqTree);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P11children;
    P11children.push_back(L12.backIt());
    P11children.push_back(Q10.backIt());
    P11children.push_back(L13.backIt());
    auto& P11 = createQ(pqTree, P11children);
    P11.mGeneration = pqTree.mGeneration;
    P11.mPertinentType = JTSTreeNode::Partial;
    P11.mPertinentChildrenCount = 3;
    P11.mFullChildrenCount = 2;

    EXPECT_EQ(templateQ5(pqTree, Q10), true);

    EXPECT_EQ(Q10.mCDPartialChild, Q08.backIt());
    EXPECT_EQ(Q10.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(Q10.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q10.mFullChildrenCount, 3);
    EXPECT_EQ(Q10.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q10.mPartialChild2, pqTree.nil());

    EXPECT_EQ(P11.mCDPartialChild, Q10.backIt());

}



/**  @Brief tests JTSPlanarizer::appliyTemplates()  extra
 */
TEST_F(JTSPlanarizerTests, Test81) {
    //          P1
    //     ______|____
    //    /           \
    // *{8,2}         Q4
    //          _______|___________________________________
    //         |            |    |                         |
    //      *{3,2}         P28 {19,18}                    P13
    //                   ___|___                      _____|______
    //                  /       \                    /     |      \
    //              {24,18}    P30                 P6 {13,12} {13,18}
    //                        __|___           ____|_____
    //                       /       \        /    |     \
    //                    {23,18}    Q32  *{7,2} {7,6} {7,12}
    //                        ________|____
    //                       |             |
    //                      P39          {11,6}
    //                    ___|___
    //                   /       \
    //                {17,12}  {17,18}

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    initializeForOneIteration(pqTree);

    auto& L17_12 = createL(pqTree);
    auto& L17_18 = createL(pqTree);

    list<node_list_it_t> P39children;
    P39children.push_back(L17_12.backIt());
    P39children.push_back(L17_18.backIt());
    auto& P39 = createP(pqTree, P39children);

    auto& L11_06 = createL(pqTree);

    list<node_list_it_t> Q32children;
    Q32children.push_back(P39.backIt());
    Q32children.push_back(L11_06.backIt());
    auto& Q32 = createP(pqTree, Q32children);

    auto& L23_18 = createL(pqTree);

    list<node_list_it_t> P30children;
    P30children.push_back(L23_18.backIt());
    P30children.push_back(Q32.backIt());
    auto& P30 = createP(pqTree, P30children);

    auto& L24_18 = createL(pqTree);

    list<node_list_it_t> P28children;
    P28children.push_back(L24_18.backIt());
    P28children.push_back(P30.backIt());
    auto& P28 = createP(pqTree, P28children);

    auto& L07_02 = createL(pqTree);
    auto& L07_06 = createL(pqTree);
    auto& L07_12 = createL(pqTree);

    list<node_list_it_t> P06children;
    P06children.push_back(L07_02.backIt());
    P06children.push_back(L07_06.backIt());
    P06children.push_back(L07_12.backIt());
    auto& P06 = createP(pqTree, P06children);
    auto& L13_12 = createL(pqTree);
    auto& L13_18 = createL(pqTree);

    list<node_list_it_t> P13children;
    P13children.push_back(P06.backIt());
    P13children.push_back(L13_12.backIt());
    P13children.push_back(L13_18.backIt());
    auto& P13 = createP(pqTree, P13children);
    auto& L03_02 = createL(pqTree);
    auto& L19_18 = createL(pqTree);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L03_02.backIt());
    Q04children.push_back(P28.backIt());
    Q04children.push_back(L19_18.backIt());
    Q04children.push_back(P13.backIt());
    auto& Q04 = createQ(pqTree, Q04children);

    auto& L08_02 = createL(pqTree);

    list<node_list_it_t> P01children;
    P01children.push_back(L08_02.backIt());
    P01children.push_back(Q04.backIt());
    auto& P01 = createP(pqTree, P01children);

    vector<node_list_it_t> pertinentLeaves;
    pertinentLeaves.push_back(L08_02.backIt());
    pertinentLeaves.push_back(L07_02.backIt());
    pertinentLeaves.push_back(L03_02.backIt());

    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);

    root = applyTemplates(planarizer, pqTree, pertinentLeaves, P01.backIt());

    EXPECT_EQ(root, P01.backIt());                         

    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(P01.mPertinentChildrenCount, 2);
    EXPECT_EQ(P01.mFullChildrenCount, 1);
    EXPECT_EQ(P01.mCDPartialChild, Q04.backIt());
    EXPECT_EQ(P01.mChildren.size(), 2);
    auto cIt = P01.mChildren.begin();
    EXPECT_EQ(*cIt, L08_02.backIt());
    cIt++;
    EXPECT_EQ(*cIt, Q04.backIt());
    EXPECT_EQ(Q04.mPertinentType, JTSTreeNode::CDPartial);
    EXPECT_EQ(Q04.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q04.mFullChildrenCount, 2);
    EXPECT_EQ(Q04.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q04.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q04.mChildren.size(), 6);
    cIt = Q04.mChildren.begin();
    EXPECT_EQ(*cIt, L03_02.backIt());
    cIt++;
    EXPECT_EQ(*cIt, P28.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L19_18.backIt());
    cIt++;
    auto& P102 = pqTree.toNodeRef(*cIt);
    cIt++;
    auto& P103 = pqTree.toNodeRef(*cIt);
    cIt++;
    EXPECT_EQ(*cIt, L07_02.backIt());

    EXPECT_EQ(P102.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(P102.mPertinentType, JTSTreeNode::Empty);
    EXPECT_EQ(P102.mChildren.size(), 2);
    cIt = P102.mChildren.begin();
    EXPECT_EQ(*cIt, L13_12.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L13_18.backIt());

    EXPECT_EQ(P103.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(P103.mPertinentType, JTSTreeNode::Empty);
    EXPECT_EQ(P103.mChildren.size(), 2);
    cIt = P103.mChildren.begin();
    EXPECT_EQ(*cIt, L07_06.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L07_12.backIt());


}


/**  @brief tests JTSPlanarizer::removePertinentTree() extra
 */
TEST_F(JTSPlanarizerTests, Test82) {

    Graph g_1;

    JTSPlanarizer planarizer;

    JTSTree& pqTree = getPQTree();

    //
    //    _P01_____
    //   /  |      \
    //*L01*Q01____ *L02
    //    / | \   \
    //   /  |  \   \
    //*L03*L04*Q02*L05
    //        / | \
    //    *L06 L07*L08
    //

    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration    = pqTree.mGeneration;
    L01.mPertinentType = JTSTreeNode::Full;
    auto& L02 = createL(pqTree);
    L02.mGeneration    = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    L03.mGeneration    = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
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
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q02children;
    Q02children.push_back(L06.backIt());
    Q02children.push_back(L07.backIt());
    Q02children.push_back(L08.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration    = pqTree.mGeneration;
    Q02.mPertinentType = JTSTreeNode::CDPartial;
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 2;

    list<node_list_it_t> Q01children;
    Q01children.push_back(L03.backIt());
    Q01children.push_back(L04.backIt());
    Q01children.push_back(Q02.backIt());
    Q01children.push_back(L05.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration    = pqTree.mGeneration;
    Q01.mPertinentType = JTSTreeNode::CDPartial;
    Q01.mPertinentChildrenCount = 4;
    Q01.mFullChildrenCount = 3;
    Q01.mCDPartialChild = Q02.backIt();

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(Q01.backIt());
    P01children.push_back(L02.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration    = pqTree.mGeneration;
    P01.mPertinentType = JTSTreeNode::CDPartial;
    P01.mPertinentChildrenCount = 3;
    P01.mFullChildrenCount = 2;
    P01.mCDPartialChild = Q01.backIt();

    auto att = removePertinentTree(planarizer, pqTree, P01.backIt());
    auto& A = pqTree.toNodeRef(att);
    EXPECT_EQ(A.mParent, Q02.backIt());
    EXPECT_EQ(Q02.mChildren.size(), 2);
    auto cIt = Q02.mChildren.begin();
    EXPECT_EQ(*cIt, L07.backIt());
    cIt++;
    EXPECT_EQ(*cIt, A.backIt());

    EXPECT_EQ(pqTree.numNodes(), 3);

}


/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for random planar graph with all the combinations of {s,t}.
 */
TEST_F(JTSPlanarizerTests, Test83) {

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

    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
        for (size_t j = 0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//            cerr << "Testing [" << i << "][" << j << "]\n";
            STNumbering st;
            vector<node_list_it_t> st_list =
                         st.getBipolarOrientation(g_1, **TList[i], **TList[j]);
            JTSPlanarizer          planarizer;
            PlanarizableGraph      planarSubgraph;
            vector<edge_list_it_t> removedEdges;
            planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);
            EXPECT_EQ(removedEdges.size(), 0);
        }
    }
}



/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for K8.
 */
TEST_F(JTSPlanarizerTests, Test84) {

    Graph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));
    auto& n_07 = g_1.addNode(make_unique<NumNode>(7));
    auto& n_08 = g_1.addNode(make_unique<NumNode>(8));

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

    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
        for (size_t j = 0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//            cerr << "Testing [" << i << "][" << j << "]\n";
            STNumbering st;
            vector<node_list_it_t> st_list =
                         st.getBipolarOrientation(g_1, **TList[i], **TList[j]);
            JTSPlanarizer          planarizer;
            PlanarizableGraph      planarSubgraph;
            vector<edge_list_it_t> removedEdges;
            planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

            EXPECT_GE(removedEdges.size(), 10);

//            for (auto eit = planarSubgraph.edges().first; 
//                               eit != planarSubgraph.edges().second; eit++) {
//                auto& CE = dynamic_cast<Edge&>(*(*(eit)));
//                auto& OE = dynamic_cast<Edge&>(*(*(CE.mOriginal)));
//                auto& N1 = dynamic_cast<NumNode&>(OE.incidentNode1());
//                auto& N2 = dynamic_cast<NumNode&>(OE.incidentNode2());
//                cerr << "{" << N1.num() << "," << N2.num() << "}\n";
//            }

        }
    }
}




/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for dodeca hedron
 */
TEST_F(JTSPlanarizerTests, Test85) {

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

    vector<node_list_it_t> TList;
    for (auto tIt = g_1.nodes().first; tIt != g_1.nodes().second; tIt++){
        TList.push_back(tIt);
    }

    for (size_t i = 0; i < g_1.numNodes() ; i++ ) {
        for (size_t j = 0; j < g_1.numNodes() ; j++ ) {
            if (i == j) {
                continue;
            }
//            cerr << "Testing [" << i << "][" << j << "]\n";
            STNumbering st;
            vector<node_list_it_t> st_list =
                         st.getBipolarOrientation(g_1, **TList[i], **TList[j]);
            JTSPlanarizer          planarizer;
            PlanarizableGraph      planarSubgraph;
            vector<edge_list_it_t> removedEdges;
            planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

            EXPECT_EQ(removedEdges.size(), 0);

        }
    }
}


/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for k0
 */
TEST_F(JTSPlanarizerTests, Test86) {

    Graph g_1;

    vector<node_list_it_t> st_list;
    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;
    planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 0);
    EXPECT_EQ(planarSubgraph.numEdges(), 0);
    EXPECT_EQ(removedEdges.size(), 0);

}


/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for k1
 */
TEST_F(JTSPlanarizerTests, Test87) {

    Graph g_1;
    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));

    vector<node_list_it_t> st_list;
    st_list.push_back(n_01.backIt());
    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;
    planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 1);
    EXPECT_EQ(planarSubgraph.numEdges(), 0);
    EXPECT_EQ(removedEdges.size(), 0);
    auto nit = planarSubgraph.nodes().first;
    auto& sn_01 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_01.isVirtual(), false);
    EXPECT_EQ(sn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   sn_01.backIt());

}


/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for k2
 */
TEST_F(JTSPlanarizerTests, Test88) {

    Graph g_1;
    auto& n_01    = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02    = g_1.addNode(make_unique<NumNode>(2));
    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);

    vector<node_list_it_t> st_list;
    st_list.push_back(n_01.backIt());
    st_list.push_back(n_02.backIt());
    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;
    planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 2);
    EXPECT_EQ(planarSubgraph.numEdges(), 1);
    EXPECT_EQ(removedEdges.size(), 0);
    auto nit = planarSubgraph.nodes().first;
    auto& sn_01 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_01.isVirtual(), false);
    EXPECT_EQ(sn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   sn_01.backIt());
    nit++;
    auto& sn_02 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_02.isVirtual(), false);
    EXPECT_EQ(sn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   sn_02.backIt());

    auto eit = planarSubgraph.edges().first;
    auto& se_01_02 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_01_02.isTerminalN1(),   true);
    EXPECT_EQ(se_01_02.isTerminalN2(),   true);
    EXPECT_EQ(se_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(),   se_01_02.backIt());
    EXPECT_EQ(e_01_02.utility(),  JTSPlanarizer::kForwardTypeInPlanarSubgraph);


}


/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for k3
 */
TEST_F(JTSPlanarizerTests, Test89) {

    Graph g_1;
    auto& n_01    = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02    = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03    = g_1.addNode(make_unique<NumNode>(3));
    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_01 = g_1.addEdge(make_unique<Edge>(),n_03,n_01);

    vector<node_list_it_t> st_list;
    st_list.push_back(n_01.backIt());
    st_list.push_back(n_02.backIt());
    st_list.push_back(n_03.backIt());
    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;
    planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 3);
    EXPECT_EQ(planarSubgraph.numEdges(), 3);
    EXPECT_EQ(removedEdges.size(), 0);
    auto nit = planarSubgraph.nodes().first;
    auto& sn_01 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_01.isVirtual(), false);
    EXPECT_EQ(sn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   sn_01.backIt());
    nit++;
    auto& sn_02 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_02.isVirtual(), false);
    EXPECT_EQ(sn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   sn_02.backIt());
    nit++;
    auto& sn_03 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_03.isVirtual(), false);
    EXPECT_EQ(sn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   sn_03.backIt());

    auto eit = planarSubgraph.edges().first;
    auto& se_01_02 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_01_02.isTerminalN1(),   true);
    EXPECT_EQ(se_01_02.isTerminalN2(),   true);
    EXPECT_EQ(se_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(),   se_01_02.backIt());
    EXPECT_EQ(e_01_02.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_02_03 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_02_03.isTerminalN1(),   true);
    EXPECT_EQ(se_02_03.isTerminalN2(),   true);
    EXPECT_EQ(se_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(),   se_02_03.backIt());
    EXPECT_EQ(e_02_03.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_03_01 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_03_01.isTerminalN1(),   true);
    EXPECT_EQ(se_03_01.isTerminalN2(),   true);
    EXPECT_EQ(se_03_01.IGBackwardLink(), e_03_01.backIt());
    EXPECT_EQ(e_03_01.IGForwardLink(),   se_03_01.backIt());
    EXPECT_EQ(e_03_01.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);

}


/**  @brief tests JTSPlanarizer::findUpwardPlanarSingleSourceSubgraph()
 *          for k4
 */
TEST_F(JTSPlanarizerTests, Test90) {

    Graph g_1;
    auto& n_01    = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02    = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03    = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04    = g_1.addNode(make_unique<NumNode>(4));
    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_01_03 = g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    auto& e_01_04 = g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_02_04 = g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);

    vector<node_list_it_t> st_list;
    st_list.push_back(n_01.backIt());
    st_list.push_back(n_02.backIt());
    st_list.push_back(n_03.backIt());
    st_list.push_back(n_04.backIt());
    JTSPlanarizer          planarizer;
    PlanarizableGraph      planarSubgraph;
    vector<edge_list_it_t> removedEdges;
    planarizer.findUpwardPlanarSingleSourceSubgraph(
                                   g_1, st_list, planarSubgraph, removedEdges);

    EXPECT_EQ(planarSubgraph.numNodes(), 4);
    EXPECT_EQ(planarSubgraph.numEdges(), 6);
    EXPECT_EQ(removedEdges.size(), 0);
    auto nit = planarSubgraph.nodes().first;
    auto& sn_01 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_01.isVirtual(), false);
    EXPECT_EQ(sn_01.IGBackwardLink(), n_01.backIt());
    EXPECT_EQ(n_01.IGForwardLink(),   sn_01.backIt());
    nit++;
    auto& sn_02 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_02.isVirtual(), false);
    EXPECT_EQ(sn_02.IGBackwardLink(), n_02.backIt());
    EXPECT_EQ(n_02.IGForwardLink(),   sn_02.backIt());
    nit++;
    auto& sn_03 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_03.isVirtual(), false);
    EXPECT_EQ(sn_03.IGBackwardLink(), n_03.backIt());
    EXPECT_EQ(n_03.IGForwardLink(),   sn_03.backIt());
    nit++;
    auto& sn_04 = dynamic_cast<PlanarizableNode&>(*(*nit));
    EXPECT_EQ(sn_04.isVirtual(), false);
    EXPECT_EQ(sn_04.IGBackwardLink(), n_04.backIt());
    EXPECT_EQ(n_04.IGForwardLink(),   sn_04.backIt());

    auto eit = planarSubgraph.edges().first;
    auto& se_01_02 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_01_02.isTerminalN1(),   true);
    EXPECT_EQ(se_01_02.isTerminalN2(),   true);
    EXPECT_EQ(se_01_02.IGBackwardLink(), e_01_02.backIt());
    EXPECT_EQ(e_01_02.IGForwardLink(),   se_01_02.backIt());
    EXPECT_EQ(e_01_02.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_01_03 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_01_03.isTerminalN1(),   true);
    EXPECT_EQ(se_01_03.isTerminalN2(),   true);
    EXPECT_EQ(se_01_03.IGBackwardLink(), e_01_03.backIt());
    EXPECT_EQ(e_01_03.IGForwardLink(),   se_01_03.backIt());
    EXPECT_EQ(e_01_03.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_01_04 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_01_04.isTerminalN1(),   true);
    EXPECT_EQ(se_01_04.isTerminalN2(),   true);
    EXPECT_EQ(se_01_04.IGBackwardLink(), e_01_04.backIt());
    EXPECT_EQ(e_01_04.IGForwardLink(),   se_01_04.backIt());
    EXPECT_EQ(e_01_04.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_02_03 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_02_03.isTerminalN1(),   true);
    EXPECT_EQ(se_02_03.isTerminalN2(),   true);
    EXPECT_EQ(se_02_03.IGBackwardLink(), e_02_03.backIt());
    EXPECT_EQ(e_02_03.IGForwardLink(),   se_02_03.backIt());
    EXPECT_EQ(e_02_03.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_02_04 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_02_04.isTerminalN1(),   true);
    EXPECT_EQ(se_02_04.isTerminalN2(),   true);
    EXPECT_EQ(se_02_04.IGBackwardLink(), e_02_04.backIt());
    EXPECT_EQ(e_02_04.IGForwardLink(),   se_02_04.backIt());
    EXPECT_EQ(e_02_04.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);
    eit++;
    auto& se_03_04 = dynamic_cast<PlanarizableEdge&>(*(*eit));
    EXPECT_EQ(se_03_04.isTerminalN1(),   true);
    EXPECT_EQ(se_03_04.isTerminalN2(),   true);
    EXPECT_EQ(se_03_04.IGBackwardLink(), e_03_04.backIt());
    EXPECT_EQ(e_03_04.IGForwardLink(),   se_03_04.backIt());
    EXPECT_EQ(e_03_04.utility(), JTSPlanarizer::kForwardTypeInPlanarSubgraph);

}
