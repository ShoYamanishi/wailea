

/**  @brief tests BLPlanarizer::templateL1() root
 */
TEST_F(BLPlanarityTesterTests, Test7){

    BLTree pqTree;
    BLGraph G;

    //    L1 positive
    //      P01
    //      |
    //     *L01
    pqTree.initializeForOneIteration();
    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentLeavesCount = 1;
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    auto& P01 = createP(pqTree, P01children);

    EXPECT_EQ(pqTree.templateL1(L01, BLTree::FINAL_REDUCTION), true);
    EXPECT_EQ(L01.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P01.mFullChildren.size(), 0);
    EXPECT_EQ(L01.mFullChildrenSet, false);
    EXPECT_EQ(isInFullChildren(L01.backIt(), P01.backIt()), false);

}



/**  @brief tests BLPlanarizer::templateL1() not root
 */
TEST_F(BLPlanarityTesterTests, Test8){

    BLTree pqTree;
    BLGraph G;

    //    L1 positive
    //     *P01
    //      |
    //     *L01
    pqTree.initializeForOneIteration();
    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentLeavesCount = 1;
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration = pqTree.mGeneration;
    P01.mPertinentLeavesCount = 1;

    EXPECT_EQ(pqTree.templateL1(L01, BLTree::NOT_FINAL_REDUCTION), true);
    EXPECT_EQ(L01.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P01.mFullChildren.size(), 1);
    EXPECT_EQ(*(P01.mFullChildren.begin()), L01.backIt());
    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), P01.backIt()), true);
}


/**  @brief tests BLPlanarizer::templateP1()
 */
TEST_F(BLPlanarityTesterTests, Test9){

    BLTree pqTree;
    BLGraph G;

    //    P1 positive
    //      Q01           Q01
    //      |             |
    //      P02     =>   *P02
    //     /|\           /|\
    //*L02*L03*L04   *L02*L03*L04
    pqTree.initializeForOneIteration();
    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    list<node_list_it_t> P02children;
    P02children.push_back(L02.backIt());
    P02children.push_back(L03.backIt());
    P02children.push_back(L04.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration = pqTree.mGeneration;
    P02.mPertinentChildrenCount = 3;
    P02.mPertinentLeavesCount = 3;

    createFullLink(L02.backIt(), P02.backIt());
    createFullLink(L03.backIt(), P02.backIt());
    createFullLink(L04.backIt(), P02.backIt());

    list<node_list_it_t> Q01children;
    Q01children.push_back(P02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(pqTree.templateP1(P02, BLTree::FINAL_REDUCTION), true);
    EXPECT_EQ(P02.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(Q01.mFullChildren.size(), 0);

    EXPECT_EQ(P02.mFullChildren.size(), 3);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), P02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), P02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), P02.backIt()), true);

}


/**  @brief tests BLPlanarizer::templateP1() not root
 */
TEST_F(BLPlanarityTesterTests, Test10){

    BLTree pqTree;
    BLGraph G;

    //    P1 positive
    //      Q01           Q01
    //      |             |
    //      P02     =>   *P02
    //     /|\           /|\
    //*L02*L03*L04   *L02*L03*L04
    pqTree.initializeForOneIteration();
    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L03 = createL(pqTree, G);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    list<node_list_it_t> P02children;
    P02children.push_back(L02.backIt());
    P02children.push_back(L03.backIt());
    P02children.push_back(L04.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration = pqTree.mGeneration;
    P02.mPertinentChildrenCount = 3;

    createFullLink(L02.backIt(), P02.backIt());
    createFullLink(L03.backIt(), P02.backIt());
    createFullLink(L04.backIt(), P02.backIt());

    P02.mPertinentLeavesCount = 3;

    list<node_list_it_t> Q01children;
    Q01children.push_back(P02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(pqTree.templateP1(P02, BLTree::NOT_FINAL_REDUCTION), true);
    EXPECT_EQ(P02.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(Q01.mFullChildren.size(), 1);

    EXPECT_EQ(P02.mFullChildren.size(), 3);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), P02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L03.backIt(), P02.backIt()), true);
    EXPECT_EQ(L03.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), P02.backIt()), true);
    EXPECT_EQ(P02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(P02.backIt(), Q01.backIt()), true);

}


/**  @brief tests BLPlanarizer::templateP3() #1
 */
TEST_F(BLPlanarityTesterTests, Test11){

    BLTree pqTree;
    BLGraph G;


    //   P3 positive #1
    //
    //      Q03             Q03
    //       |               |
    //      P05       =>   Q_N1
    //     /| |\          /     \
    //L08*L09 L10*L11   P05    *P07
    //                  / \     / \
    //                L08 L10 *L09  *L11
    pqTree.initializeForOneIteration();
    auto& L08 = createL(pqTree, G);
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;
    auto& L10 = createL(pqTree, G);
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;
    list<node_list_it_t> P05children;
    P05children.push_back(L08.backIt());
    P05children.push_back(L09.backIt());
    P05children.push_back(L10.backIt());
    P05children.push_back(L11.backIt());
    auto& P05 = createP(pqTree, P05children);
    P05.mGeneration = pqTree.mGeneration;
    P05.mPertinentChildrenCount = 2;
    P05.mPertinentLeavesCount = 2;

    createFullLink(L09.backIt(), P05.backIt());
    createFullLink(L11.backIt(), P05.backIt());

    list<node_list_it_t> Q03children;
    Q03children.push_back(P05.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP3(P05, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(P05.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P05.mPertinentType, BLTreeNode::Empty);
    EXPECT_EQ(P05.mPertinentChildrenCount, 0);
    EXPECT_EQ(P05.mFullChildren.size(),  0);
    EXPECT_EQ(P05.mPertinentLeavesCount, 0);
    EXPECT_EQ(P05.mChildren.size(),  2);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L08.backIt());
    tmpList.push_back(L10.backIt());
    EXPECT_EQ(checkUnorderedList(P05.mChildren, tmpList),true);


    EXPECT_EQ(Q03.mFullChildren.size(), 0);
    auto& Q_N1 = pqTree.toNodeRef(Q03.mSinglyPartialChild1);

    auto& P07 = (Q_N1.mEndChild2==P05.backIt())?
                pqTree.toNodeRef(Q_N1.mEndChild1):
                pqTree.toNodeRef(Q_N1.mEndChild2);

    EXPECT_EQ( (P05.mSibling1==P07.backIt()&&
                P05.mSibling2==pqTree.nil()  )||
               (P05.mSibling2==P07.backIt()&&
                P05.mSibling1==pqTree.nil()  ),true);
    EXPECT_EQ( (P07.mSibling1==P05.backIt()&&
                P07.mSibling2==pqTree.nil()  )||
               (P07.mSibling2==P05.backIt()&&
                P07.mSibling1==pqTree.nil()  ),true);

    EXPECT_EQ(P07.mPertinentType==BLTreeNode::Full&&
              P05.mPertinentType==BLTreeNode::Empty  , true);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L09.backIt());
    tmpList2.push_back(L11.backIt());
    EXPECT_EQ(checkUnorderedList(P07.mChildren, tmpList2),true);

    EXPECT_EQ(P07.mChildren.size(), 2);
    auto cIt = P07.mChildren.begin();
    auto& P07_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P07_C01.mChildIt,cIt);
    EXPECT_EQ(P07_C01.mParent, P07.backIt());
    EXPECT_EQ(P07_C01.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P07_C01.mSibling1, pqTree.nil());
    EXPECT_EQ(P07_C01.mSibling2, pqTree.nil());
    cIt++;
    auto& P07_C02 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P07_C02.mChildIt,cIt);
    EXPECT_EQ(P07_C02.mParent, P07.backIt());
    EXPECT_EQ(P07_C02.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P07_C02.mSibling1, pqTree.nil());
    EXPECT_EQ(P07_C02.mSibling2, pqTree.nil());

    cIt = P05.mChildren.begin();
    auto& P05_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P05_C01.mChildIt,cIt);
    EXPECT_EQ(P05_C01.mParent, P05.backIt());
    EXPECT_EQ(P05_C01.mPertinentType==BLTreeNode::Empty ||
              P05_C01.mGeneration < pqTree.mGeneration    ,true  );
    EXPECT_EQ(P05_C01.mSibling1, pqTree.nil());
    EXPECT_EQ(P05_C01.mSibling2, pqTree.nil());
    cIt++;
    auto& P05_C02 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P05_C02.mChildIt,cIt);
    EXPECT_EQ(P05_C02.mParent, P05.backIt());
    EXPECT_EQ(P05_C02.mPertinentType==BLTreeNode::Empty ||
              P05_C02.mGeneration < pqTree.mGeneration    ,true  );
    EXPECT_EQ(P05_C02.mSibling1, pqTree.nil());
    EXPECT_EQ(P05_C02.mSibling2, pqTree.nil());


    EXPECT_EQ(P07.mFullChildren.size(), 2);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), P07.backIt()), true);
    EXPECT_EQ(L11.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L11.backIt(), P07.backIt()), true);

    EXPECT_EQ(L08.mFullChildrenSet, false);
    EXPECT_EQ(L10.mFullChildrenSet, false);
    EXPECT_EQ(P05.mFullChildren.size(), 0);
    EXPECT_EQ(isInFullChildren(P07.backIt(), Q_N1.backIt()), true);
    EXPECT_EQ(Q_N1.mFullChildren.size(), 1);
    EXPECT_EQ(Q_N1.mFullChildrenSet, false);
    EXPECT_EQ(Q03.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateP3() #2
 */
TEST_F(BLPlanarityTesterTests, Test12){

    BLTree pqTree;
    BLGraph G;


    //   P3 positive #2
    //
    //      Q04             Q04
    //       |               |
    //      P08       =>   Q(P08)
    //     /| \             / \
    // L12*L13*L14       L12 *P09
    //                        / \
    //                     *L13 *L14
    pqTree.initializeForOneIteration();
    auto& L12 = createL(pqTree, G);
    auto& L13 = createL(pqTree, G);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = BLTreeNode::Full;
    L13.mPertinentLeavesCount = 1;
    auto& L14 = createL(pqTree, G);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = BLTreeNode::Full;
    L14.mPertinentLeavesCount = 1;
    list<node_list_it_t> P08children;
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    P08children.push_back(L14.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentChildrenCount = 2;
    P08.mPertinentLeavesCount = 2;

    createFullLink(L13.backIt(), P08.backIt());
    createFullLink(L14.backIt(), P08.backIt());

    list<node_list_it_t> Q04children;
    Q04children.push_back(P08.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 1;
    bool earlyOut;
    EXPECT_EQ(pqTree.templateP3(P08, earlyOut), true);
    EXPECT_EQ(earlyOut, false);
    EXPECT_EQ(P08.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(P08.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q04.mSinglyPartialChild1, P08.backIt());
    EXPECT_EQ(Q04.mFullChildren.size(), 0);
    EXPECT_EQ(P08.mPertinentLeavesCount, 2);

    EXPECT_EQ(P08.mChildren.size(), 0);
    auto& P08_C1 = pqTree.toNodeRef(P08.mEndChild1);
    auto& P08_C2 = pqTree.toNodeRef(P08.mEndChild2);
    EXPECT_EQ( (P08_C1.mSibling1==P08_C2.backIt()&&
                P08_C1.mSibling2==pqTree.nil()  )||
               (P08_C1.mSibling2==P08_C2.backIt()&&
                P08_C1.mSibling1==pqTree.nil()  ),true);
    EXPECT_EQ( (P08_C2.mSibling1==P08_C1.backIt()&&
                P08_C2.mSibling2==pqTree.nil()  )||
               (P08_C2.mSibling2==P08_C1.backIt()&&
                P08_C2.mSibling1==pqTree.nil()  ),true);

    EXPECT_EQ(((P08_C1.mPertinentType==BLTreeNode::Full&&
                (P08_C2.mPertinentType==BLTreeNode::Empty||
                 P08_C2.mGeneration<pqTree.mGeneration)    )||
               (P08_C2.mPertinentType==BLTreeNode::Full&&
                (P08_C1.mPertinentType==BLTreeNode::Empty||
                 P08_C1.mGeneration<pqTree.mGeneration)    ) ), true);

    if (P08_C1.mPertinentType==BLTreeNode::Full) {

        EXPECT_EQ(P08_C1.mChildren.size(), 2);
        auto cIt = P08_C1.mChildren.begin();
        auto& P08_C1_C01 = pqTree.toNodeRef(*cIt);
        EXPECT_EQ(P08_C1_C01.mChildIt,cIt);
        EXPECT_EQ(P08_C1_C01.mParent, P08_C1.backIt());
        EXPECT_EQ(P08_C1_C01.mPertinentType, BLTreeNode::Full);
        EXPECT_EQ(P08_C1_C01.mSibling1, pqTree.nil());
        EXPECT_EQ(P08_C1_C01.mSibling2, pqTree.nil());
        cIt++;
        auto& P08_C1_C02 = pqTree.toNodeRef(*cIt);
        EXPECT_EQ(P08_C1_C02.mChildIt,cIt);
        EXPECT_EQ(P08_C1_C02.mParent, P08_C1.backIt());
        EXPECT_EQ(P08_C1_C02.mPertinentType, BLTreeNode::Full);
        EXPECT_EQ(P08_C1_C02.mSibling1, pqTree.nil());
        EXPECT_EQ(P08_C1_C02.mSibling2, pqTree.nil());
        EXPECT_EQ( (&P08_C1_C01==&L13)&&(&P08_C1_C02==&L14) ||
                   (&P08_C1_C02==&L13)&&(&P08_C1_C01==&L14)   , true);

        EXPECT_EQ(&P08_C2, &L12);
        EXPECT_EQ(L12.mNodeType , BLTreeNode::LType);
        EXPECT_EQ(L12.mPertinentType==BLTreeNode::Empty ||
                  L12.mGeneration<pqTree.mGeneration, true);

        EXPECT_EQ(L12.mFullChildrenSet, false);
        EXPECT_EQ(isInFullChildren(L12.backIt(), P08.backIt()), false);
        EXPECT_EQ(L13.mFullChildrenSet, true);
        EXPECT_EQ(L14.mFullChildrenSet, true);
        EXPECT_EQ(isInFullChildren(L13.backIt(), P08_C1.backIt()), true);
        EXPECT_EQ(isInFullChildren(L14.backIt(), P08_C1.backIt()), true);
        EXPECT_EQ(P08_C1.mFullChildren.size(), 2);

        EXPECT_EQ(P08_C1.mFullChildrenSet, true);
        EXPECT_EQ(isInFullChildren(P08_C1.backIt(), P08.backIt()), true);
        EXPECT_EQ(P08.mFullChildren.size(), 1);
        EXPECT_EQ(P08.mFullChildrenSet, false);
        EXPECT_EQ(isInFullChildren(P08.backIt(), Q04.backIt()), false);
        EXPECT_EQ(Q04.mFullChildren.size(), 0);
        EXPECT_EQ(Q04.mFullChildrenSet, false);

    }
    else {

        EXPECT_EQ(P08_C2.mChildren.size(), 2);
        auto cIt = P08_C2.mChildren.begin();
        auto& P08_C2_C01 = pqTree.toNodeRef(*cIt);
        EXPECT_EQ(P08_C2_C01.mChildIt,cIt);
        EXPECT_EQ(P08_C2_C01.mParent, P08_C2.backIt());
        EXPECT_EQ(P08_C2_C01.mPertinentType, BLTreeNode::Full);
        EXPECT_EQ(P08_C2_C01.mSibling1, pqTree.nil());
        EXPECT_EQ(P08_C2_C01.mSibling2, pqTree.nil());
        cIt++;
        auto& P08_C2_C02 = pqTree.toNodeRef(*cIt);
        EXPECT_EQ(P08_C2_C02.mChildIt,cIt);
        EXPECT_EQ(P08_C2_C02.mParent, P08_C2.backIt());
        EXPECT_EQ(P08_C2_C02.mPertinentType, BLTreeNode::Full);
        EXPECT_EQ(P08_C2_C02.mSibling1, pqTree.nil());
        EXPECT_EQ(P08_C2_C02.mSibling2, pqTree.nil());
        EXPECT_EQ( (&P08_C2_C01==&L13)&&(&P08_C2_C02==&L14) ||
                   (&P08_C2_C02==&L13)&&(&P08_C2_C01==&L14)   , true);

        EXPECT_EQ(&P08_C1, &L12);
        EXPECT_EQ(L12.mNodeType , BLTreeNode::LType);
        EXPECT_EQ(L12.mPertinentType==BLTreeNode::Empty ||
                  L12.mGeneration<pqTree.mGeneration, true);

        EXPECT_EQ(L12.mFullChildrenSet, false);
        EXPECT_EQ(isInFullChildren(L12.backIt(), P08.backIt()), false);
        EXPECT_EQ(L13.mFullChildrenSet, true);
        EXPECT_EQ(L14.mFullChildrenSet, true);
        EXPECT_EQ(isInFullChildren(L13.backIt(), P08_C2.backIt()), true);
        EXPECT_EQ(isInFullChildren(L14.backIt(), P08_C2.backIt()), true);
        EXPECT_EQ(P08_C2.mFullChildren.size(), 2);

        EXPECT_EQ(P08_C2.mFullChildrenSet, true);
        EXPECT_EQ(isInFullChildren(P08_C2.backIt(), P08.backIt()), true);
        EXPECT_EQ(P08.mFullChildren.size(), 1);
        EXPECT_EQ(P08.mFullChildrenSet, false);
        EXPECT_EQ(isInFullChildren(P08.backIt(), Q04.backIt()), false);
        EXPECT_EQ(Q04.mFullChildren.size(), 0);
        EXPECT_EQ(Q04.mFullChildrenSet, false);

    }
}



/**  @brief tests BLPlanarizer::templateP3() #3
 */
TEST_F(BLPlanarityTesterTests, Test13){

    BLTree pqTree;
    BLGraph G;

    //   P3 positive #3
    //
    //      Q05             Q05
    //       |               |
    //      P10     =>     Q_N1
    //     /| \             / \
    // L15 L16 *L17      *L17  P10
    //                        / \
    //                      L15 L16
    pqTree.initializeForOneIteration();
    auto& L15 = createL(pqTree, G);
    auto& L16 = createL(pqTree, G);
    auto& L17 = createL(pqTree, G);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = BLTreeNode::Full;
    L17.mPertinentLeavesCount = 1;

    list<node_list_it_t> P10children;
    P10children.push_back(L15.backIt());
    P10children.push_back(L16.backIt());
    P10children.push_back(L17.backIt());
    auto& P10 = createP(pqTree, P10children);
    P10.mGeneration = pqTree.mGeneration;
    P10.mPertinentChildrenCount = 1;
    P10.mPertinentLeavesCount = 1;    

    createFullLink(L17.backIt(), P10.backIt());

    P10.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(P10.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentChildrenCount = 1;
    bool earlyOut;
    EXPECT_EQ(pqTree.templateP3(P10,  earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q05.mFullChildren.size(), 0);
    auto& Q_N1 = pqTree.toNodeRef(Q05.mSinglyPartialChild1);
    EXPECT_EQ(Q_N1.mPertinentType ,BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q_N1.mPertinentChildrenCount ,1);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L17.backIt());
    EXPECT_EQ(checkUnorderedList(Q_N1.mFullChildren, tmpList), true);
    if (Q_N1.mEndChild1 == L17.backIt()) {
        EXPECT_EQ(Q_N1.mEndChild2, P10.backIt());
    }
    else {
        EXPECT_EQ(Q_N1.mEndChild1, P10.backIt());
    }

    EXPECT_EQ(P10.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P10.mPertinentType, BLTreeNode::Empty);
    EXPECT_EQ(P10.mFullChildren.size(), 0);
    EXPECT_EQ(P10.mPertinentLeavesCount, 0);
    EXPECT_EQ(P10.mChildren.size(), 2);
    EXPECT_EQ(P10.mParent, Q_N1.backIt());

    EXPECT_EQ(Q05.mSinglyPartialChild1, Q_N1.backIt());
    EXPECT_EQ(Q05.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q05.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q05.mFullChildren.size(), 0);

    auto& Q_N1_C1 = pqTree.toNodeRef(Q_N1.mEndChild1);
    auto& Q_N1_C2 = pqTree.toNodeRef(Q_N1.mEndChild2);
    EXPECT_EQ( (Q_N1_C1.mSibling1==Q_N1_C2.backIt()&&
                Q_N1_C1.mSibling2==pqTree.nil()  )||
               (Q_N1_C1.mSibling2==Q_N1_C2.backIt()&&
                Q_N1_C1.mSibling1==pqTree.nil()  ),true);

    EXPECT_EQ( (Q_N1_C2.mSibling1==Q_N1_C1.backIt()&&
                Q_N1_C2.mSibling2==pqTree.nil()  )||
               (Q_N1_C2.mSibling2==Q_N1_C1.backIt()&&
                Q_N1_C2.mSibling1==pqTree.nil()  ),true);

    
    EXPECT_EQ((Q_N1_C1.backIt()==L17.backIt()&&
               Q_N1_C2.backIt()==P10.backIt()   )||
              (Q_N1_C1.backIt()==L17.backIt()&&
               Q_N1_C2.backIt()==P10.backIt()   ), true);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L15.backIt());
    tmpList2.push_back(L16.backIt());
    EXPECT_EQ(checkUnorderedList(P10.mChildren, tmpList2), true);
    EXPECT_EQ(P10.mFullChildren.size(), 0);
    EXPECT_EQ(L15.mParent, P10.backIt());
    EXPECT_EQ(L16.mParent, P10.backIt());
    EXPECT_EQ(L15.mPertinentType==BLTreeNode::Empty ||
              L15.mGeneration < pqTree.mGeneration    ,true  );
    EXPECT_EQ(L16.mPertinentType==BLTreeNode::Empty ||
              L16.mGeneration < pqTree.mGeneration    ,true  );

    EXPECT_EQ(L15.mFullChildrenSet, false);
    EXPECT_EQ(L16.mFullChildrenSet, false);
    EXPECT_EQ(P10.mFullChildrenSet, false);
    EXPECT_EQ(L17.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L17.backIt(), Q_N1.backIt()), true);
    EXPECT_EQ(Q_N1.mFullChildren.size(), 1);
    EXPECT_EQ(Q_N1.mFullChildrenSet, false);

}


/**  @brief tests BLPlanarizer::templateP3() #4
 */
TEST_F(BLPlanarityTesterTests, Test14){

    BLTree pqTree;
    BLGraph G;

    //   P3 positive #4
    //
    //      Q06             Q06
    //       |               |
    //      P12     =>     Q(P12)
    //      / \             / \
    //    L18 *L19       L18  *L19

    pqTree.initializeForOneIteration();
    auto& L18 = createL(pqTree, G);
    auto& L19 = createL(pqTree, G);
    L19.mGeneration = pqTree.mGeneration;
    L19.mPertinentType = BLTreeNode::Full;
    L19.mPertinentLeavesCount = 1;
    list<node_list_it_t> P12children;
    P12children.push_back(L18.backIt());
    P12children.push_back(L19.backIt());
    auto& P12 = createP(pqTree, P12children);
    P12.mGeneration = pqTree.mGeneration;
    P12.mPertinentChildrenCount = 1;

    createFullLink(L19.backIt(), P12.backIt());

    P12.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q06children;
    Q06children.push_back(P12.backIt());
    auto& Q06 = createQ(pqTree, Q06children);
    Q06.mGeneration = pqTree.mGeneration;
    Q06.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP3(P12, earlyOut), true);
    EXPECT_EQ(earlyOut, false);
    EXPECT_EQ(P12.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(P12.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(P12.mPertinentLeavesCount, 1);
    EXPECT_EQ(Q06.mSinglyPartialChild1, P12.backIt());
    EXPECT_EQ(Q06.mSinglyPartialChild2, pqTree.nil());

    EXPECT_EQ(P12.mChildren.size(), 0);
    auto& P12_C1 = pqTree.toNodeRef(P12.mEndChild1);
    auto& P12_C2 = pqTree.toNodeRef(P12.mEndChild2);
    EXPECT_EQ( (P12_C1.mSibling1==P12_C2.backIt()&&
                P12_C1.mSibling2==pqTree.nil()  )||
               (P12_C1.mSibling2==P12_C2.backIt()&&
                P12_C1.mSibling1==pqTree.nil()  ),true);
    EXPECT_EQ( (P12_C2.mSibling1==P12_C1.backIt()&&
                P12_C2.mSibling2==pqTree.nil()  )||
               (P12_C2.mSibling2==P12_C1.backIt()&&
                P12_C2.mSibling1==pqTree.nil()  ),true);

    EXPECT_EQ(((P12_C1.mPertinentType==BLTreeNode::Full&&
                (P12_C2.mPertinentType==BLTreeNode::Empty||
                 P12_C2.mGeneration<pqTree.mGeneration      ))||
               (P12_C2.mPertinentType==BLTreeNode::Full&&
                (P12_C1.mPertinentType==BLTreeNode::Empty||
                 P12_C1.mGeneration<pqTree.mGeneration      )) ), true);
    if (P12_C1.mPertinentType==BLTreeNode::Full) {

        EXPECT_EQ(&P12_C1, &L19);
        EXPECT_EQ(*(P12.mFullChildren.begin()), L19.backIt());
        EXPECT_EQ(P12_C1.mNodeType, BLTreeNode::LType);
        EXPECT_EQ(P12_C1.mPertinentType, BLTreeNode::Full);

        EXPECT_EQ(&P12_C2, &L18);
        EXPECT_EQ(P12_C2.mNodeType, BLTreeNode::LType);
        EXPECT_EQ(P12_C2.mPertinentType==BLTreeNode::Empty||
                  P12_C2.mGeneration < pqTree.mGeneration, true);

    }
    else {

        EXPECT_EQ(&P12_C2, &L19);
        EXPECT_EQ(*(P12.mFullChildren.begin()), L19.backIt());
        EXPECT_EQ(P12_C2.mNodeType, BLTreeNode::LType);
        EXPECT_EQ(P12_C2.mPertinentType, BLTreeNode::Full);

        EXPECT_EQ(&P12_C1, &L18);
        EXPECT_EQ(P12_C1.mNodeType, BLTreeNode::LType);
        EXPECT_EQ(P12_C1.mPertinentType==BLTreeNode::Empty||
                  P12_C1.mGeneration < pqTree.mGeneration, true);

    }

    EXPECT_EQ(L18.mFullChildrenSet, false);
    EXPECT_EQ(L19.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L19.backIt(), P12.backIt()), true);
    EXPECT_EQ(P12.mFullChildren.size(), 1);
    EXPECT_EQ(P12.mFullChildrenSet, false);


}


/**  @brief tests BLPlanarizer::templateP3() #5 early out
 */
TEST_F(BLPlanarityTesterTests, Test15){

    BLTree pqTree;
    BLGraph G;

    //   P3 early out
    //
    //          Q07
    //        /  |  \
    //     *P13 P14*P15
    //      / \
    //    L20 *L21

    pqTree.initializeForOneIteration();
    auto& L20 = createL(pqTree, G);
    auto& L21 = createL(pqTree, G);
    L21.mGeneration = pqTree.mGeneration;
    L21.mPertinentType = BLTreeNode::Full;
    list<node_list_it_t> P13children;
    P13children.push_back(L20.backIt());
    P13children.push_back(L21.backIt());
    auto& P13 = createP(pqTree, P13children);
    P13.mGeneration = pqTree.mGeneration;
    P13.mPertinentChildrenCount = 1;
    createFullLink(L21.backIt(), P13.backIt());

    list<node_list_it_t> P14children;
    auto& P14 = createP(pqTree, P14children);
    P14.mGeneration = pqTree.mGeneration;
    P14.mPertinentChildrenCount = 1;

    list<node_list_it_t> P15children;
    auto& P15 = createP(pqTree, P15children);
    P15.mGeneration = pqTree.mGeneration;
    P15.mPertinentChildrenCount = 1;

    list<node_list_it_t> Q07children;
    Q07children.push_back(P13.backIt());
    Q07children.push_back(P14.backIt());
    Q07children.push_back(P15.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentChildrenCount = 3;
    Q07.mSinglyPartialChild1 = P14.backIt();
    Q07.mSinglyPartialChild2 = P15.backIt();
    bool earlyOut;
    EXPECT_EQ(pqTree.templateP3(P13, earlyOut), true);
    EXPECT_EQ(earlyOut, true);

}


/**  @brief tests BLPlanarizer::templateP4()
 */
TEST_F(BLPlanarityTesterTests, Test16){

    BLTree pqTree;
    BLGraph G;

    //   P4 positive #1
    //
    //         Q06                      Q06
    //          |                        |
    //      ___P12_____       =>      __P12
    //     /  /  \  \  \             /  /  \
    //  L18 L19   \*L20*L21       L18 L19   \
    //             \                         \
    //        ___ Q07____             ______Q07________
    //       |   |   |   |           |     |   |   |   |
    //     *L22*L23 L24 L25        Q07_C1*L22*L23 L24 L25
    //                              /\
    //                           *L20*L21
    pqTree.initializeForOneIteration();
    auto& L18 = createL(pqTree, G);
    auto& L19 = createL(pqTree, G);
    auto& L20 = createL(pqTree, G);
    L20.mGeneration = pqTree.mGeneration;
    L20.mPertinentType = BLTreeNode::Full;
    L20.mPertinentLeavesCount = 1;
    auto& L21 = createL(pqTree, G);
    L21.mGeneration = pqTree.mGeneration;
    L21.mPertinentType = BLTreeNode::Full;
    L21.mPertinentLeavesCount = 1;
    auto& L22 = createL(pqTree, G);
    L22.mGeneration = pqTree.mGeneration;
    L22.mPertinentType = BLTreeNode::Full;
    L22.mPertinentLeavesCount = 1;
    auto& L23 = createL(pqTree, G);
    L23.mGeneration = pqTree.mGeneration;
    L23.mPertinentType = BLTreeNode::Full;
    L23.mPertinentLeavesCount = 1;
    auto& L24 = createL(pqTree, G);
    auto& L25 = createL(pqTree, G);
    list<node_list_it_t> Q07children;
    Q07children.push_back(L22.backIt());
    Q07children.push_back(L23.backIt());
    Q07children.push_back(L24.backIt());
    Q07children.push_back(L25.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = BLTreeNode::SinglyPartial;
    Q07.mPertinentChildrenCount = 2;

    createFullLink(L22.backIt(), Q07.backIt());
    createFullLink(L23.backIt(), Q07.backIt());

    Q07.mPertinentLeavesCount = 2;

    list<node_list_it_t> P12children;
    P12children.push_back(L18.backIt());
    P12children.push_back(L19.backIt());
    P12children.push_back(Q07.backIt());
    P12children.push_back(L20.backIt());
    P12children.push_back(L21.backIt());
    auto& P12 = createP(pqTree, P12children);
    P12.mGeneration = pqTree.mGeneration;
    P12.mPertinentChildrenCount = 3;
    P12.mSinglyPartialChild1 = Q07.backIt();

    createFullLink(L20.backIt(), P12.backIt());
    createFullLink(L21.backIt(), P12.backIt());

    P12.mPertinentLeavesCount = 4;

    list<node_list_it_t> Q06children;
    Q06children.push_back(P12.backIt());
    auto& Q06 = createQ(pqTree, Q06children);

    node_list_it_t root;
    EXPECT_EQ(pqTree.templateP4(P12, root), true);
    EXPECT_EQ(Q06.mFullChildren.size(), 0);
    EXPECT_EQ(Q06.mPertinentLeavesCount, 0);
    EXPECT_EQ(Q06.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q06.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q06.mCDPartialChild, pqTree.nil());

    EXPECT_EQ(root, Q07.backIt());

    EXPECT_EQ(P12.mChildren.size(),3);
    auto cIt = P12.mChildren.begin();
    EXPECT_EQ(*cIt, L18.backIt());
    EXPECT_EQ(L18.mChildIt, cIt);
    EXPECT_EQ(L18.mParent, P12.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L19.backIt());
    EXPECT_EQ(L19.mChildIt, cIt);
    EXPECT_EQ(L19.mParent, P12.backIt());
    cIt++;

    EXPECT_EQ(*cIt, Q07.backIt());
    EXPECT_EQ(Q07.mChildIt, cIt);
    EXPECT_EQ(Q07.mParent, P12.backIt());

    EXPECT_EQ(Q07.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q07.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q07.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q07.mPertinentLeavesCount, 4);
    EXPECT_EQ(Q07.mFullChildren.size(), 3);

    EXPECT_EQ(Q07.mChildren.size(),0);
    node_list_it_t prevIt = pqTree.nil();
    node_list_it_t curIt;
    auto& Q07_E1 = pqTree.toNodeRef(Q07.mEndChild1);
    if (&Q07_E1 == &L25) {
        curIt = Q07.mEndChild2;
    }
    else {
        curIt = Q07.mEndChild1;
    }
    auto& Q07_C01 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(Q07_C01.mParent, Q07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    auto& Q07_C02 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q07_C03 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q07_C04 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q07_C05 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(Q07_C05.mParent, Q07.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, pqTree.nil());

    EXPECT_EQ(Q07_C01.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(Q07_C01.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(Q07_C01.mChildren.size(), 2);
    EXPECT_EQ(Q07_C01.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q07_C01.mFullChildren.size(), 2);
    EXPECT_EQ(
              ( *(Q07_C01.mFullChildren.begin())==L20.backIt()&&
                *(Q07_C01.mFullChildren.rbegin())==L21.backIt())||
              ( *(Q07_C01.mFullChildren.begin())==L21.backIt()&&
                *(Q07_C01.mFullChildren.rbegin())==L20.backIt()), true);

    EXPECT_EQ(Q07_C01.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q07_C01.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q07_C01.mCDPartialChild, pqTree.nil());
    cIt = Q07_C01.mChildren.begin();
    EXPECT_EQ(*cIt, L20.backIt());
    EXPECT_EQ(L20.mChildIt, cIt);
    EXPECT_EQ(L20.mParent, Q07_C01.backIt());
    EXPECT_EQ(L20.mSibling1, pqTree.nil());
    EXPECT_EQ(L20.mSibling2, pqTree.nil());
    cIt++;
    EXPECT_EQ(*cIt, L21.backIt());
    EXPECT_EQ(L21.mChildIt, cIt);
    EXPECT_EQ(L21.mParent, Q07_C01.backIt());
    EXPECT_EQ(L21.mSibling1, pqTree.nil());
    EXPECT_EQ(L21.mSibling2, pqTree.nil());

    EXPECT_EQ(&Q07_C02, &L22);
    EXPECT_EQ(L22.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L22.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(&Q07_C03, &L23);
    EXPECT_EQ(L23.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L23.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(&Q07_C04, &L24);
    EXPECT_EQ(L24.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L24.isEmpty(), true);
    EXPECT_EQ(&Q07_C05, &L25);
    EXPECT_EQ(L25.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L25.isEmpty(), true);

    EXPECT_EQ(L20.mFullChildrenSet, true);
    EXPECT_EQ(L21.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L20.backIt(), Q07_C01.backIt()), true);
    EXPECT_EQ(isInFullChildren(L21.backIt(), Q07_C01.backIt()), true);
    EXPECT_EQ(Q07_C01.mFullChildren.size(), 2);
    EXPECT_EQ(Q07_C01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(Q07_C01.backIt(), Q07.backIt()), true);
    EXPECT_EQ(isInFullChildren(L22.backIt(), Q07.backIt()), true);
    EXPECT_EQ(isInFullChildren(L23.backIt(), Q07.backIt()), true);
    EXPECT_EQ(Q07.mFullChildren.size(), 3);
    EXPECT_EQ(Q07.mFullChildrenSet, false);
    EXPECT_EQ(L18.mFullChildrenSet, false);
    EXPECT_EQ(L19.mFullChildrenSet, false);
    EXPECT_EQ(P12.mFullChildren.size(), 0);
    EXPECT_EQ(P12.mFullChildrenSet, false);
    EXPECT_EQ(Q06.mFullChildren.size(), 0);
    EXPECT_EQ(Q06.mFullChildrenSet, false);
}


/**  @brief tests BLPlanarizer::templateP4()
 */
TEST_F(BLPlanarityTesterTests, Test17){

    BLTree pqTree;
    BLGraph G;

    //   P4 positive #2
    //
    //         Q08                      Q08
    //          |                        |
    //      ___P14__          =>      __P14
    //     /  /  \  \                /  /  \
    //  L26 L27   \*L28            L26 L27  \
    //        ____Q09____             ______Q09______
    //       |   |   |   |           |   |   |   |   |
    //     *L29*L30 L31 L32        *L28*L29*L30 L31 L32
    pqTree.initializeForOneIteration();
    auto& L26 = createL(pqTree, G);
    auto& L27 = createL(pqTree, G);
    auto& L28 = createL(pqTree, G);
    L28.mGeneration = pqTree.mGeneration;
    L28.mPertinentType = BLTreeNode::Full;
    L28.mPertinentLeavesCount = 1;
    auto& L29 = createL(pqTree, G);
    L29.mGeneration = pqTree.mGeneration;
    L29.mPertinentType = BLTreeNode::Full;
    L29.mPertinentLeavesCount = 1;
    auto& L30 = createL(pqTree, G);
    L30.mGeneration = pqTree.mGeneration;
    L30.mPertinentType = BLTreeNode::Full;
    L30.mPertinentLeavesCount = 1;
    auto& L31 = createL(pqTree, G);
    auto& L32 = createL(pqTree, G);
    list<node_list_it_t> Q09children;
    Q09children.push_back(L29.backIt());
    Q09children.push_back(L30.backIt());
    Q09children.push_back(L31.backIt());
    Q09children.push_back(L32.backIt());
    auto& Q09 = createQ(pqTree, Q09children);
    Q09.mGeneration = pqTree.mGeneration;
    Q09.mPertinentType = BLTreeNode::SinglyPartial;
    Q09.mPertinentChildrenCount = 2;
    Q09.mPertinentLeavesCount = 2;

    createFullLink(L29.backIt(), Q09.backIt());
    createFullLink(L30.backIt(), Q09.backIt());

    list<node_list_it_t> P14children;
    P14children.push_back(L26.backIt());
    P14children.push_back(L27.backIt());
    P14children.push_back(Q09.backIt());
    P14children.push_back(L28.backIt());
    auto& P14 = createP(pqTree, P14children);
    P14.mGeneration = pqTree.mGeneration;
    P14.mPertinentChildrenCount = 2;
    P14.mPertinentLeavesCount = 3;

    P14.mSinglyPartialChild1 = Q09.backIt();

    createFullLink(L28.backIt(), P14.backIt());

    node_list_it_t root;
    EXPECT_EQ(pqTree.templateP4(P14, root), true);
    EXPECT_EQ(root, Q09.backIt());

    EXPECT_EQ(P14.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(P14.mSinglyPartialChild2, pqTree.nil());

    auto cIt = P14.mChildren.begin();
    EXPECT_EQ(*cIt, L26.backIt());
    EXPECT_EQ(L26.mChildIt, cIt);
    EXPECT_EQ(L26.mParent, P14.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L27.backIt());
    EXPECT_EQ(L27.mChildIt, cIt);
    EXPECT_EQ(L27.mParent, P14.backIt());
    cIt++;
    EXPECT_EQ(*cIt, Q09.backIt());
    EXPECT_EQ(Q09.mChildIt, cIt);
    EXPECT_EQ(Q09.mParent, P14.backIt());
    EXPECT_EQ(Q09.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q09.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q09.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q09.mPertinentLeavesCount, 3);
    EXPECT_EQ(Q09.mChildren.size(),0);
    list<node_list_it_t> tmpList;
    tmpList.push_back(L28.backIt());
    tmpList.push_back(L29.backIt());
    tmpList.push_back(L30.backIt());
    EXPECT_EQ(checkUnorderedList(Q09.mFullChildren,tmpList),true);
    EXPECT_EQ(Q09.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q09.mSinglyPartialChild2, pqTree.nil());

    auto prevIt = pqTree.nil();
    node_list_it_t curIt;
    auto& Q09_E1 = pqTree.toNodeRef(Q09.mEndChild1);
    if (&Q09_E1 == &L28) {
        curIt = Q09.mEndChild1;
    }
    else {
        curIt = Q09.mEndChild2;
    }
    auto& Q09_C01 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(Q09_C01.mParent, Q09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    auto& Q09_C02 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q09_C03 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q09_C04 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q09_C05 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(Q09_C05.mParent, Q09.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, pqTree.nil());

    EXPECT_EQ(&Q09_C01, &L28);
    EXPECT_EQ(L28.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L28.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(&Q09_C02, &L29);
    EXPECT_EQ(L29.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L29.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(&Q09_C03, &L30);
    EXPECT_EQ(L30.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L30.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(&Q09_C04, &L31);
    EXPECT_EQ(L31.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L31.isEmpty(), true);
    EXPECT_EQ(&Q09_C05, &L32);
    EXPECT_EQ(L32.mNodeType, BLTreeNode::LType);
    EXPECT_EQ(L32.isEmpty(), true);

    EXPECT_EQ(L28.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L28.backIt(), Q09.backIt()), true);
    EXPECT_EQ(L29.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L29.backIt(), Q09.backIt()), true);
    EXPECT_EQ(L30.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L30.backIt(), Q09.backIt()), true);
    EXPECT_EQ(L31.mFullChildrenSet, false);
    EXPECT_EQ(isInFullChildren(L31.backIt(), Q09.backIt()), false);
    EXPECT_EQ(L32.mFullChildrenSet, false);
    EXPECT_EQ(isInFullChildren(L32.backIt(), Q09.backIt()), false);
    EXPECT_EQ(Q09.mFullChildren.size(), 3);

    EXPECT_EQ(L26.mFullChildrenSet, false);
    EXPECT_EQ(L27.mFullChildrenSet, false);
    EXPECT_EQ(P14.mFullChildren.size(), 0);
    EXPECT_EQ(P14.mFullChildrenSet, false);

}


/**  @brief tests BLPlanarizer::templateP4()
 */
TEST_F(BLPlanarityTesterTests, Test18){

    BLTree pqTree;
    BLGraph G;

    //   P4 positive #3
    //
    //     ____Q10                       ___Q10
    //    |     |                       |    |
    //   L39   P15            =>       L39__Q11______
    //     /  /  \                     / |   |   |   |
    // *L33*L34   \                 P16*L35*L36 L37 L38
    //        ___Q11_____           / \
    //       |   |   |   |       *L33 *L34
    //     *L35*L36 L37 L38
    pqTree.initializeForOneIteration();
    auto& L35 = createL(pqTree, G);
    L35.mGeneration = pqTree.mGeneration;
    L35.mPertinentType = BLTreeNode::Full;
    L35.mPertinentLeavesCount = 1;
    auto& L36 = createL(pqTree, G);
    L36.mGeneration = pqTree.mGeneration;
    L36.mPertinentType = BLTreeNode::Full;
    L36.mPertinentLeavesCount = 1;
    auto& L37 = createL(pqTree, G);
    auto& L38 = createL(pqTree, G);

    list<node_list_it_t> Q11children;
    Q11children.push_back(L35.backIt());
    Q11children.push_back(L36.backIt());
    Q11children.push_back(L37.backIt());
    Q11children.push_back(L38.backIt());
    auto& Q11 = createQ(pqTree, Q11children);
    Q11.mGeneration = pqTree.mGeneration;
    Q11.mPertinentType = BLTreeNode::SinglyPartial;
    Q11.mPertinentChildrenCount = 2;
    Q11.mPertinentLeavesCount = 2;

    createFullLink(L35.backIt(), Q11.backIt());
    createFullLink(L36.backIt(), Q11.backIt());

    auto& L33 = createL(pqTree, G);
    L33.mGeneration = pqTree.mGeneration;
    L33.mPertinentType = BLTreeNode::Full;
    L33.mPertinentLeavesCount = 1;
    auto& L34 = createL(pqTree, G);
    L34.mGeneration = pqTree.mGeneration;
    L34.mPertinentType = BLTreeNode::Full;
    L34.mPertinentLeavesCount = 1;

    list<node_list_it_t> P15children;
    P15children.push_back(L33.backIt());
    P15children.push_back(L34.backIt());
    P15children.push_back(Q11.backIt());
    auto& P15 = createP(pqTree, P15children);
    P15.mGeneration = pqTree.mGeneration;
    P15.mPertinentChildrenCount = 3;
    P15.mPertinentLeavesCount = 4;

    createFullLink(L33.backIt(), P15.backIt());
    createFullLink(L34.backIt(), P15.backIt());

    P15.mSinglyPartialChild1 = Q11.backIt();

    auto& L39 = createL(pqTree, G);

    list<node_list_it_t> Q10children;
    Q10children.push_back(L39.backIt());
    Q10children.push_back(P15.backIt());
    auto& Q10 = createQ(pqTree, Q10children);

    node_list_it_t root;
    EXPECT_EQ(pqTree.templateP4(P15, root), true);
    EXPECT_EQ(root, Q11.backIt());

    EXPECT_EQ(Q11.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q11.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q11.mChildren.size(), 0);
    EXPECT_EQ(Q11.mParent, Q10.backIt());

    EXPECT_EQ(Q11.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q11.mPertinentLeavesCount, 4);
    EXPECT_EQ(Q11.mFullChildren.size(), 3);
    auto prevIt = pqTree.nil();
    node_list_it_t curIt;

    if (pqTree.toNodeRef(Q11.mEndChild1).isFull()) {
        curIt = Q11.mEndChild1;
    }
    else {
        curIt = Q11.mEndChild2;
    }
    auto& P16 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(P16.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P16.mChildren.size(), 2);
    EXPECT_EQ(P16.mPertinentChildrenCount, 2);
    EXPECT_EQ(P16.mPertinentLeavesCount, 2);
    list<node_list_it_t> tmpList;
    tmpList.push_back(L33.backIt());
    tmpList.push_back(L34.backIt());
    EXPECT_EQ(checkUnorderedList(P16.mFullChildren,tmpList),true);
    EXPECT_EQ(P16.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(P16.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(P16.mCDPartialChild, pqTree.nil());
    auto cIt = P16.mChildren.begin();
    EXPECT_EQ(*cIt, L33.backIt());
    EXPECT_EQ(L33.mPertinentType, BLTreeNode::Full);
    cIt++;
    EXPECT_EQ(*cIt, L34.backIt());
    EXPECT_EQ(L34.mPertinentType, BLTreeNode::Full);

    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L35.backIt());
    EXPECT_EQ(L35.mPertinentType, BLTreeNode::Full);  
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L36.backIt());
    EXPECT_EQ(L36.mPertinentType, BLTreeNode::Full);  
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L37.backIt());
    EXPECT_LT(L37.mGeneration, pqTree.mGeneration);  
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L38.backIt());
    EXPECT_LT(L38.mGeneration, pqTree.mGeneration);  

    EXPECT_EQ(Q11.mParent, Q10.backIt());
    EXPECT_EQ(L39.mParent, Q10.backIt());
    EXPECT_EQ(Q10.mEndChild1, L39.backIt());
    EXPECT_EQ(Q10.mEndChild2, Q11.backIt());
    EXPECT_EQ(areImmediateSiblings(L39,Q11), true);

    EXPECT_EQ(L33.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L33.backIt(), P16.backIt()), true);
    EXPECT_EQ(L34.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L34.backIt(), P16.backIt()), true);
    EXPECT_EQ(P16.mFullChildren.size(), 2);
    EXPECT_EQ(P16.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(P16.backIt(), Q11.backIt()), true);
    EXPECT_EQ(L35.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L35.backIt(), Q11.backIt()), true);
    EXPECT_EQ(L36.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L36.backIt(), Q11.backIt()), true);
    EXPECT_EQ(L37.mFullChildrenSet, false);
    EXPECT_EQ(L38.mFullChildrenSet, false);
    EXPECT_EQ(Q11.mFullChildren.size(), 3);
    EXPECT_EQ(Q11.mFullChildrenSet, false);
    EXPECT_EQ(L39.mFullChildrenSet, false);
    EXPECT_EQ(Q10.mFullChildren.size(), 0);
}


/**  @brief tests BLPlanarizer::templateP5()
 */
TEST_F(BLPlanarityTesterTests, Test19){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P5 positive #1
    //
    //         Q12_____                           Q12_______________
    //          |      |                           |                |
    //      ___P17___ L50      =>           _______Q13___________  L50
    //     /  / | \  \                    /   /   /    \   \    \
    //  L39 L40 |*L41*L42               P18*L43 *L44  L45  L46  P17
    //       __Q13___                   / \                     / \
    //      /  /  \  \               *L41*L42                 L39 L40
    //   *L43*L44 L45 L46

    pqTree.initializeForOneIteration();
    auto& L39 = createL(pqTree, G);
    auto& L40 = createL(pqTree, G);
    auto& L41 = createL(pqTree, G);
    L41.mGeneration = pqTree.mGeneration;
    L41.mPertinentType = BLTreeNode::Full;
    L41.mPertinentLeavesCount = 1;

    auto& L42 = createL(pqTree, G);
    L42.mGeneration = pqTree.mGeneration;
    L42.mPertinentType = BLTreeNode::Full;
    L42.mPertinentLeavesCount = 1;

    auto& L43 = createL(pqTree, G);
    L43.mGeneration = pqTree.mGeneration;
    L43.mPertinentType = BLTreeNode::Full;
    L43.mPertinentLeavesCount = 1;

    auto& L44 = createL(pqTree, G);
    L44.mGeneration = pqTree.mGeneration;
    L44.mPertinentType = BLTreeNode::Full;
    L44.mPertinentLeavesCount = 1;

    auto& L45 = createL(pqTree, G);
    auto& L46 = createL(pqTree, G);
    list<node_list_it_t> Q13children;
    list<bool> Q13orientations;
    Q13children.push_back(L43.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L44.backIt());
    Q13orientations.push_back(false);
    Q13children.push_back(L45.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L46.backIt());
    Q13orientations.push_back(false);
    auto& Q13 = createQext(pqTree, Q13children, Q13orientations);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = BLTreeNode::SinglyPartial;
    Q13.mPertinentChildrenCount = 2;
    Q13.mPertinentLeavesCount = 2;

    createFullLink(L43.backIt(), Q13.backIt());
    createFullLink(L44.backIt(), Q13.backIt());

    list<node_list_it_t> P17children;
    P17children.push_back(L39.backIt());
    P17children.push_back(L40.backIt());
    P17children.push_back(Q13.backIt());
    P17children.push_back(L41.backIt());
    P17children.push_back(L42.backIt());
    auto& P17 = createP(pqTree, P17children);
    P17.mPertinentChildrenCount = 3;
    P17.mPertinentLeavesCount = 4;

    P17.mSinglyPartialChild1 = Q13.backIt();

    createFullLink(L41.backIt(), P17.backIt());
    createFullLink(L42.backIt(), P17.backIt());

    auto& L50 = createL(pqTree, G);

    list<node_list_it_t> Q12children;
    Q12children.push_back(P17.backIt());
    Q12children.push_back(L50.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP5(P17, earlyOut), true);
    EXPECT_EQ(earlyOut, false);
   
    EXPECT_EQ(Q13.mParent, Q12.backIt());

    node_list_it_t singlyIt;
    node_list_it_t emptyIt;
    if (pqTree.toNodeRef(Q12.mEndChild1).isSinglyPartial()) {
        singlyIt = Q12.mEndChild1;
        emptyIt  = Q12.mEndChild2;
    }
    else {
        singlyIt = Q12.mEndChild2;
        emptyIt  = Q12.mEndChild1;
    }

    EXPECT_EQ(singlyIt, Q13.backIt());
    EXPECT_EQ(emptyIt, L50.backIt());
    EXPECT_EQ(L50.mParent, Q12.backIt());

    EXPECT_EQ(Q12.mSinglyPartialChild1, Q13.backIt());

    EXPECT_EQ(Q13.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q13.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q13.mPertinentLeavesCount,   4);


    EXPECT_EQ(Q13.mSinglyPartialChild1,    pqTree.nil());
    EXPECT_EQ(Q13.mSinglyPartialChild2,    pqTree.nil());
    EXPECT_EQ(Q13.mCDPartialChild,         pqTree.nil());

    auto& P18 = pqTree.toNodeRef(*(Q13.mFullChildren.rbegin()));

    list<node_list_it_t> tmpList;
    tmpList.push_back(P18.backIt());
    tmpList.push_back(L43.backIt());
    tmpList.push_back(L44.backIt());
    EXPECT_EQ(checkUnorderedList(Q13.mFullChildren,tmpList),true);


    node_list_it_t prevIt = pqTree.nil();
    node_list_it_t curIt;
    EXPECT_EQ(Q13.mEndChild1 == P18.backIt() ||
              Q13.mEndChild2 == P18.backIt() , true);

    if (Q13.mEndChild1 == P18.backIt()) {
        curIt = Q13.mEndChild1;
    }
    else {
        curIt = Q13.mEndChild2;
    }

    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L43.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L44.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L45.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L46.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(P17.backIt(), curIt);
    EXPECT_EQ(P17.backIt(), P17.backIt());
    EXPECT_EQ(Q13.mEndChild1 == P17.backIt() ||
              Q13.mEndChild2 == P17.backIt() , true);
    EXPECT_EQ(P18.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P18.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P18.mPertinentChildrenCount, 2);
    EXPECT_EQ(P18.mPertinentLeavesCount, 2);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L41.backIt());
    tmpList2.push_back(L42.backIt());
    EXPECT_EQ(checkUnorderedList(P18.mFullChildren,tmpList2),true);

    auto cIt = P18.mChildren.begin();
    EXPECT_EQ(*cIt, L41.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L42.backIt());

    EXPECT_EQ(P17.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P17.mPertinentType, BLTreeNode::Empty);
    EXPECT_EQ(P17.mPertinentChildrenCount, 0);
    EXPECT_EQ(P17.mPertinentLeavesCount, 0);
    EXPECT_EQ(P17.mFullChildren.size(), 0);
    cIt = P17.mChildren.begin();
    EXPECT_EQ(*cIt, L39.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L40.backIt());

    EXPECT_EQ(L41.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L41.backIt(), P18.backIt()), true);
    EXPECT_EQ(L42.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L42.backIt(), P18.backIt()), true);
    EXPECT_EQ(P18.mFullChildren.size(), 2);
    EXPECT_EQ(P18.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(P18.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L43.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L43.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L44.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L44.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L45.mFullChildrenSet, false);
    EXPECT_EQ(L46.mFullChildrenSet, false);
    EXPECT_EQ(L39.mFullChildrenSet, false);
    EXPECT_EQ(L40.mFullChildrenSet, false);
    EXPECT_EQ(P17.mFullChildrenSet, false);
    EXPECT_EQ(Q13.mFullChildren.size(), 3);
    EXPECT_EQ(Q13.mFullChildrenSet, false);
    EXPECT_EQ(L50.mFullChildrenSet, false);
    EXPECT_EQ(Q12.mFullChildren.size(), 0);
}


/**  @brief tests BLPlanarizer::templateP5()
 */
TEST_F(BLPlanarityTesterTests, Test20){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P5 positive #2
    //
    //         Q12_____                           Q12_______________
    //          |      |                           |                |
    //      ___P17___ L50      =>           _______Q13___________  L50
    //     /    | \  \                    /   /   /    \   \    \
    //  L39     |*L41*L42               P18*L43 *L44  L45  L46  L39
    //       __Q13___                   / \
    //      /  /  \  \               *L41*L42
    //   *L43*L44 L45 L46

    pqTree.initializeForOneIteration();
    auto& L39 = createL(pqTree, G);
    auto& L41 = createL(pqTree, G);
    L41.mGeneration = pqTree.mGeneration;
    L41.mPertinentType = BLTreeNode::Full;
    L41.mPertinentLeavesCount = 1;

    auto& L42 = createL(pqTree, G);
    L42.mGeneration = pqTree.mGeneration;
    L42.mPertinentType = BLTreeNode::Full;
    L42.mPertinentLeavesCount = 1;

    auto& L43 = createL(pqTree, G);
    L43.mGeneration = pqTree.mGeneration;
    L43.mPertinentType = BLTreeNode::Full;
    L43.mPertinentLeavesCount = 1;

    auto& L44 = createL(pqTree, G);
    L44.mGeneration = pqTree.mGeneration;
    L44.mPertinentType = BLTreeNode::Full;
    L44.mPertinentLeavesCount = 1;

    auto& L45 = createL(pqTree, G);
    auto& L46 = createL(pqTree, G);
    list<node_list_it_t> Q13children;
    list<bool> Q13orientations;
    Q13children.push_back(L43.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L44.backIt());
    Q13orientations.push_back(false);
    Q13children.push_back(L45.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L46.backIt());
    Q13orientations.push_back(false);
    auto& Q13 = createQext(pqTree, Q13children, Q13orientations);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = BLTreeNode::SinglyPartial;
    Q13.mPertinentChildrenCount = 2;
    Q13.mPertinentLeavesCount = 2;

    createFullLink(L43.backIt(), Q13.backIt());
    createFullLink(L44.backIt(), Q13.backIt());

    list<node_list_it_t> P17children;
    P17children.push_back(L39.backIt());
    P17children.push_back(Q13.backIt());
    P17children.push_back(L41.backIt());
    P17children.push_back(L42.backIt());
    auto& P17 = createP(pqTree, P17children);
    P17.mPertinentChildrenCount = 3;
    P17.mPertinentLeavesCount = 4;

    P17.mSinglyPartialChild1 = Q13.backIt();

    createFullLink(L41.backIt(), P17.backIt());
    createFullLink(L42.backIt(), P17.backIt());

    auto& L50 = createL(pqTree, G);

    list<node_list_it_t> Q12children;
    Q12children.push_back(P17.backIt());
    Q12children.push_back(L50.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP5(P17, earlyOut), true);

    EXPECT_EQ(earlyOut, false);
   
    EXPECT_EQ(Q13.mParent, Q12.backIt());

    node_list_it_t singlyIt;
    node_list_it_t emptyIt;
    if (pqTree.toNodeRef(Q12.mEndChild1).isSinglyPartial()) {
        singlyIt = Q12.mEndChild1;
        emptyIt  = Q12.mEndChild2;
    }
    else {
        singlyIt = Q12.mEndChild2;
        emptyIt  = Q12.mEndChild1;
    }

    EXPECT_EQ(singlyIt, Q13.backIt());
    EXPECT_EQ(emptyIt, L50.backIt());
    EXPECT_EQ(L50.mParent, Q12.backIt());

    EXPECT_EQ(Q12.mSinglyPartialChild1, Q13.backIt());

    EXPECT_EQ(Q13.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q13.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q13.mPertinentLeavesCount,   4);


    EXPECT_EQ(Q13.mSinglyPartialChild1,    pqTree.nil());
    EXPECT_EQ(Q13.mSinglyPartialChild2,    pqTree.nil());
    EXPECT_EQ(Q13.mCDPartialChild,         pqTree.nil());

    auto& P18 = pqTree.toNodeRef(*(Q13.mFullChildren.rbegin()));

    list<node_list_it_t> tmpList;
    tmpList.push_back(P18.backIt());
    tmpList.push_back(L43.backIt());
    tmpList.push_back(L44.backIt());
    EXPECT_EQ(checkUnorderedList(Q13.mFullChildren,tmpList),true);

    node_list_it_t prevIt = pqTree.nil();
    node_list_it_t curIt;
    EXPECT_EQ(Q13.mEndChild1 == P18.backIt() ||
              Q13.mEndChild2 == P18.backIt() , true);

    if (Q13.mEndChild1 == P18.backIt()) {
        curIt = Q13.mEndChild1;
    }
    else {
        curIt = Q13.mEndChild2;
    }

    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L43.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L44.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L45.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L46.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L39.backIt());
    EXPECT_EQ(Q13.mEndChild1 == L39.backIt() ||
              Q13.mEndChild2 == L39.backIt() , true);
    EXPECT_EQ(P18.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P18.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P18.mPertinentChildrenCount, 2);
    EXPECT_EQ(P18.mPertinentLeavesCount, 2);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L41.backIt());
    tmpList2.push_back(L42.backIt());
    EXPECT_EQ(checkUnorderedList(P18.mFullChildren,tmpList2),true);

    auto cIt = P18.mChildren.begin();
    EXPECT_EQ(*cIt, L41.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L42.backIt());

    EXPECT_EQ(L41.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L41.backIt(), P18.backIt()), true);
    EXPECT_EQ(L42.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L42.backIt(), P18.backIt()), true);
    EXPECT_EQ(P18.mFullChildren.size(), 2);
    EXPECT_EQ(P18.mFullChildrenSet, true);
    EXPECT_EQ(L43.mFullChildrenSet, true);
    EXPECT_EQ(L44.mFullChildrenSet, true);
    EXPECT_EQ(L45.mFullChildrenSet, false);
    EXPECT_EQ(L46.mFullChildrenSet, false);
    EXPECT_EQ(L39.mFullChildrenSet, false);
    EXPECT_EQ(Q13.mFullChildren.size(), 3);
    EXPECT_EQ(Q13.mFullChildrenSet, false);
    EXPECT_EQ(L50.mFullChildrenSet, false);
    EXPECT_EQ(Q12.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateP5()
 */
TEST_F(BLPlanarityTesterTests, Test21){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P5 positive #3
    //
    //         Q12_____                           Q12_______________
    //          |      |                           |                |
    //         P17___ L50      =>           _______Q13_____        L50
    //          | \  \                    /   /   /    \   \
    //          |*L41*L42               P18*L43 *L44  L45  L46
    //       __Q13___                   / \
    //      /  /  \  \               *L41*L42
    //   *L43*L44 L45 L46

    pqTree.initializeForOneIteration();
    auto& L41 = createL(pqTree, G);
    L41.mGeneration = pqTree.mGeneration;
    L41.mPertinentType = BLTreeNode::Full;
    L41.mPertinentLeavesCount = 1;

    auto& L42 = createL(pqTree, G);
    L42.mGeneration = pqTree.mGeneration;
    L42.mPertinentType = BLTreeNode::Full;
    L42.mPertinentLeavesCount = 1;

    auto& L43 = createL(pqTree, G);
    L43.mGeneration = pqTree.mGeneration;
    L43.mPertinentType = BLTreeNode::Full;
    L43.mPertinentLeavesCount = 1;

    auto& L44 = createL(pqTree, G);
    L44.mGeneration = pqTree.mGeneration;
    L44.mPertinentType = BLTreeNode::Full;
    L44.mPertinentLeavesCount = 1;

    auto& L45 = createL(pqTree, G);
    auto& L46 = createL(pqTree, G);
    list<node_list_it_t> Q13children;
    list<bool> Q13orientations;
    Q13children.push_back(L43.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L44.backIt());
    Q13orientations.push_back(false);
    Q13children.push_back(L45.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L46.backIt());
    Q13orientations.push_back(false);
    auto& Q13 = createQext(pqTree, Q13children, Q13orientations);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = BLTreeNode::SinglyPartial;
    Q13.mPertinentChildrenCount = 2;
    Q13.mPertinentLeavesCount = 2;

    createFullLink(L43.backIt(), Q13.backIt());
    createFullLink(L44.backIt(), Q13.backIt());

    list<node_list_it_t> P17children;
    P17children.push_back(Q13.backIt());
    P17children.push_back(L41.backIt());
    P17children.push_back(L42.backIt());
    auto& P17 = createP(pqTree, P17children);
    P17.mPertinentChildrenCount = 3;
    P17.mPertinentLeavesCount = 4;

    P17.mSinglyPartialChild1 = Q13.backIt();

    createFullLink(L41.backIt(), P17.backIt());
    createFullLink(L42.backIt(), P17.backIt());

    auto& L50 = createL(pqTree, G);

    list<node_list_it_t> Q12children;
    Q12children.push_back(P17.backIt());
    Q12children.push_back(L50.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP5(P17, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q13.mParent, Q12.backIt());

    node_list_it_t singlyIt;
    node_list_it_t emptyIt;
    if (pqTree.toNodeRef(Q12.mEndChild1).isSinglyPartial()) {
        singlyIt = Q12.mEndChild1;
        emptyIt  = Q12.mEndChild2;
    }
    else {
        singlyIt = Q12.mEndChild2;
        emptyIt  = Q12.mEndChild1;
    }

    EXPECT_EQ(singlyIt, Q13.backIt());
    EXPECT_EQ(emptyIt, L50.backIt());
    EXPECT_EQ(L50.mParent, Q12.backIt());

    EXPECT_EQ(Q12.mSinglyPartialChild1, Q13.backIt());

    EXPECT_EQ(Q13.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q13.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q13.mPertinentLeavesCount,   4);

    EXPECT_EQ(Q13.mSinglyPartialChild1,    pqTree.nil());
    EXPECT_EQ(Q13.mSinglyPartialChild2,    pqTree.nil());
    EXPECT_EQ(Q13.mCDPartialChild,         pqTree.nil());

    auto& P18 = pqTree.toNodeRef(*(Q13.mFullChildren.rbegin()));
    list<node_list_it_t> tmpList;
    tmpList.push_back(P18.backIt());
    tmpList.push_back(L43.backIt());
    tmpList.push_back(L44.backIt());
    EXPECT_EQ(checkUnorderedList(Q13.mFullChildren,tmpList),true);

    node_list_it_t prevIt = pqTree.nil();
    node_list_it_t curIt;
    EXPECT_EQ(Q13.mEndChild1 == P18.backIt() ||
              Q13.mEndChild2 == P18.backIt() , true);

    if (Q13.mEndChild1 == P18.backIt()) {
        curIt = Q13.mEndChild1;
    }
    else {
        curIt = Q13.mEndChild2;
    }

    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L43.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L44.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L45.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L46.backIt());
    EXPECT_EQ(Q13.mEndChild1 == L46.backIt() ||
              Q13.mEndChild2 == L46.backIt() , true);
    EXPECT_EQ(P18.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P18.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P18.mPertinentChildrenCount, 2);
    EXPECT_EQ(P18.mPertinentLeavesCount, 2);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L41.backIt());
    tmpList2.push_back(L42.backIt());
    EXPECT_EQ(checkUnorderedList(P18.mFullChildren,tmpList2),true);
    auto cIt = P18.mChildren.begin();
    EXPECT_EQ(*cIt, L41.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L42.backIt());

    EXPECT_EQ(L41.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L41.backIt(), P18.backIt()), true);
    EXPECT_EQ(L42.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L42.backIt(), P18.backIt()), true);
    EXPECT_EQ(P18.mFullChildren.size(), 2);
    EXPECT_EQ(P18.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(P18.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L43.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L43.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L44.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L44.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L45.mFullChildrenSet, false);
    EXPECT_EQ(L46.mFullChildrenSet, false);
    EXPECT_EQ(Q13.mFullChildren.size(), 3);
    EXPECT_EQ(L50.mFullChildrenSet, false);
    EXPECT_EQ(Q12.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateP5()
 */
TEST_F(BLPlanarityTesterTests, Test22){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P5 positive #4
    //
    //         Q12_____                           Q12_______________
    //          |      |                           |                |
    //         P17    L50      =>           _______Q13______       L50
    //          | \                        /   /   /    \   \
    //          |*L41                  *L41*L43 *L44  L45  L46
    //       __Q13___
    //      /  /  \  \
    //   *L43*L44 L45 L46

    pqTree.initializeForOneIteration();
    auto& L41 = createL(pqTree, G);
    L41.mGeneration = pqTree.mGeneration;
    L41.mPertinentType = BLTreeNode::Full;
    L41.mPertinentLeavesCount = 1;

    auto& L43 = createL(pqTree, G);
    L43.mGeneration = pqTree.mGeneration;
    L43.mPertinentType = BLTreeNode::Full;
    L43.mPertinentLeavesCount = 1;

    auto& L44 = createL(pqTree, G);
    L44.mGeneration = pqTree.mGeneration;
    L44.mPertinentType = BLTreeNode::Full;
    L44.mPertinentLeavesCount = 1;

    auto& L45 = createL(pqTree, G);
    auto& L46 = createL(pqTree, G);
    list<node_list_it_t> Q13children;
    list<bool> Q13orientations;
    Q13children.push_back(L43.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L44.backIt());
    Q13orientations.push_back(false);
    Q13children.push_back(L45.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L46.backIt());
    Q13orientations.push_back(false);
    auto& Q13 = createQext(pqTree, Q13children, Q13orientations);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = BLTreeNode::SinglyPartial;
    Q13.mPertinentChildrenCount = 2;
    Q13.mPertinentLeavesCount = 2;

    createFullLink(L43.backIt(), Q13.backIt());
    createFullLink(L44.backIt(), Q13.backIt());

    list<node_list_it_t> P17children;
    P17children.push_back(Q13.backIt());
    P17children.push_back(L41.backIt());
    auto& P17 = createP(pqTree, P17children);
    P17.mPertinentChildrenCount = 2;
    P17.mPertinentLeavesCount = 3;

    P17.mSinglyPartialChild1 = Q13.backIt();
    createFullLink(L41.backIt(), P17.backIt());

    auto& L50 = createL(pqTree, G);

    list<node_list_it_t> Q12children;
    Q12children.push_back(P17.backIt());
    Q12children.push_back(L50.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP5(P17, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q13.mParent, Q12.backIt());

    node_list_it_t singlyIt;
    node_list_it_t emptyIt;
    if (pqTree.toNodeRef(Q12.mEndChild1).isSinglyPartial()) {
        singlyIt = Q12.mEndChild1;
        emptyIt  = Q12.mEndChild2;
    }
    else {
        singlyIt = Q12.mEndChild2;
        emptyIt  = Q12.mEndChild1;
    }

    EXPECT_EQ(singlyIt, Q13.backIt());
    EXPECT_EQ(emptyIt, L50.backIt());
    EXPECT_EQ(L50.mParent, Q12.backIt());

    EXPECT_EQ(Q12.mSinglyPartialChild1, Q13.backIt());

    EXPECT_EQ(Q13.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q13.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q13.mPertinentLeavesCount,   3);


    list<node_list_it_t> tmpList;
    tmpList.push_back(L41.backIt());
    tmpList.push_back(L43.backIt());
    tmpList.push_back(L44.backIt());
    EXPECT_EQ(checkUnorderedList(Q13.mFullChildren,tmpList),true);

    EXPECT_EQ(Q13.mSinglyPartialChild1,    pqTree.nil());
    EXPECT_EQ(Q13.mSinglyPartialChild2,    pqTree.nil());
    EXPECT_EQ(Q13.mCDPartialChild,         pqTree.nil());

    node_list_it_t prevIt = pqTree.nil();
    node_list_it_t curIt;
    EXPECT_EQ(Q13.mEndChild1 == L41.backIt() ||
              Q13.mEndChild2 == L41.backIt() , true);

    if (Q13.mEndChild1 == L41.backIt()) {
        curIt = Q13.mEndChild1;
    }
    else {
        curIt = Q13.mEndChild2;
    }

    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L43.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L44.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L45.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L46.backIt());
    EXPECT_EQ(Q13.mEndChild1 == L46.backIt() ||
              Q13.mEndChild2 == L46.backIt() , true);

    EXPECT_EQ(L41.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L41.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L43.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L43.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L44.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L44.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L45.mFullChildrenSet, false);
    EXPECT_EQ(L46.mFullChildrenSet, false);
    EXPECT_EQ(Q13.mFullChildren.size(), 3);
    EXPECT_EQ(L50.mFullChildrenSet, false);
    EXPECT_EQ(Q12.mFullChildren.size(), 0);


}



/**  @brief tests BLPlanarizer::templateP5()
 */
TEST_F(BLPlanarityTesterTests, Test23){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P5 positive #5
    //
    //         Q12_____                           Q12_______________
    //          |      |                           |                |
    //         P17    L50      =>           _______Q13______       L50
    //          | \                        /   /   /    \   \
    //          | L41                  *L43*L44  L45  L46  L41
    //       __Q13___
    //      /  /  \  \
    //   *L43*L44 L45 L46

    pqTree.initializeForOneIteration();

    auto& L43 = createL(pqTree, G);
    L43.mGeneration = pqTree.mGeneration;
    L43.mPertinentType = BLTreeNode::Full;
    L43.mPertinentLeavesCount = 1;

    auto& L44 = createL(pqTree, G);
    L44.mGeneration = pqTree.mGeneration;
    L44.mPertinentType = BLTreeNode::Full;
    L44.mPertinentLeavesCount = 1;

    auto& L45 = createL(pqTree, G);

    auto& L46 = createL(pqTree, G);

    list<node_list_it_t> Q13children;
    list<bool> Q13orientations;
    Q13children.push_back(L43.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L44.backIt());
    Q13orientations.push_back(false);
    Q13children.push_back(L45.backIt());
    Q13orientations.push_back(true);
    Q13children.push_back(L46.backIt());
    Q13orientations.push_back(false);
    auto& Q13 = createQext(pqTree, Q13children, Q13orientations);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = BLTreeNode::SinglyPartial;
    Q13.mPertinentChildrenCount = 2;
    Q13.mPertinentLeavesCount = 2;

    createFullLink(L43.backIt(), Q13.backIt());
    createFullLink(L44.backIt(), Q13.backIt());

    auto& L41 = createL(pqTree, G);

    list<node_list_it_t> P17children;
    P17children.push_back(Q13.backIt());
    P17children.push_back(L41.backIt());
    auto& P17 = createP(pqTree, P17children);
    P17.mPertinentChildrenCount = 2;
    P17.mPertinentLeavesCount = 3;

    P17.mSinglyPartialChild1 = Q13.backIt();

    auto& L50 = createL(pqTree, G);

    list<node_list_it_t> Q12children;
    Q12children.push_back(P17.backIt());
    Q12children.push_back(L50.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP5(P17, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    EXPECT_EQ(Q13.mParent, Q12.backIt());

    node_list_it_t singlyIt;
    node_list_it_t emptyIt;
    if (pqTree.toNodeRef(Q12.mEndChild1).isSinglyPartial()) {
        singlyIt = Q12.mEndChild1;
        emptyIt  = Q12.mEndChild2;
    }
    else {
        singlyIt = Q12.mEndChild2;
        emptyIt  = Q12.mEndChild1;
    }

    EXPECT_EQ(singlyIt, Q13.backIt());
    EXPECT_EQ(emptyIt, L50.backIt());
    EXPECT_EQ(L50.mParent, Q12.backIt());
    EXPECT_EQ(Q13.mParent, Q12.backIt());

    EXPECT_EQ(Q12.mSinglyPartialChild1, Q13.backIt());

    EXPECT_EQ(Q13.mPertinentType, BLTreeNode::SinglyPartial);
    EXPECT_EQ(Q13.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q13.mPertinentLeavesCount,   2);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L43.backIt());
    tmpList.push_back(L44.backIt());
    EXPECT_EQ(checkUnorderedList(Q13.mFullChildren,tmpList),true);

    EXPECT_EQ(Q13.mSinglyPartialChild1,    pqTree.nil());
    EXPECT_EQ(Q13.mSinglyPartialChild2,    pqTree.nil());
    EXPECT_EQ(Q13.mCDPartialChild,         pqTree.nil());

    node_list_it_t prevIt = pqTree.nil();
    node_list_it_t curIt;
    EXPECT_EQ(Q13.mEndChild1 == L41.backIt() ||
              Q13.mEndChild2 == L41.backIt() , true);

    if (Q13.mEndChild1 == L43.backIt()) {
        curIt = Q13.mEndChild1;
    }
    else {
        curIt = Q13.mEndChild2;
    }

    EXPECT_EQ(curIt, L43.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L44.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L45.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L46.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L41.backIt());
    EXPECT_EQ(Q13.mEndChild1 == L41.backIt() ||
              Q13.mEndChild2 == L41.backIt() , true);

    EXPECT_EQ(L43.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L43.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L44.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L44.backIt(), Q13.backIt()), true);
    EXPECT_EQ(L45.mFullChildrenSet, false);
    EXPECT_EQ(L46.mFullChildrenSet, false);
    EXPECT_EQ(L41.mFullChildrenSet, false);
    EXPECT_EQ(Q13.mFullChildren.size(), 2);
    EXPECT_EQ(L50.mFullChildrenSet, false);
    EXPECT_EQ(Q12.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateP6()
 */
TEST_F(BLPlanarityTesterTests, Test24){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);


    //   P6 positive #1
    //
    //         Q22                                Q22
    //          |                                  |
    //        _P26______________       =>         P26
    //       /  | \   \   \     \               /  |  \
    //     L70  |*L71 L72*L73    |            L70 Q23 L72
    //       __Q23___       ____Q24___       ______|________________________
    //      /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //    L74 L75*L76*L77 L78 L79*L80*L81  L74 L75*L76*L77 P27*L81*L80 L79 L78
    //                                                     / \
    //                                                  *L71*L73

    pqTree.initializeForOneIteration();

    auto& L74 = createL(pqTree, G);
    auto& L75 = createL(pqTree, G);
    auto& L76 = createL(pqTree, G);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = BLTreeNode::Full;
    L76.mPertinentLeavesCount = 1;
    auto& L77 = createL(pqTree, G);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = BLTreeNode::Full;
    L77.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = BLTreeNode::SinglyPartial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mPertinentLeavesCount = 2;

    createFullLink(L76.backIt(), Q23.backIt());
    createFullLink(L77.backIt(), Q23.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q23.mOrientInNorm.push_back(track_01.backIt());
    Q23.mOrientInReversed.push_back(track_02.backIt());
    Q23.mOrientOutNorm.push_back(track_03.backIt());
    Q23.mOrientOutReversed.push_back(track_04.backIt());

    auto& L78 = createL(pqTree, G);
    auto& L79 = createL(pqTree, G);
    auto& L80 = createL(pqTree, G);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = BLTreeNode::Full;
    L80.mPertinentLeavesCount = 1;
    auto& L81 = createL(pqTree, G);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = BLTreeNode::Full;
    L81.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = BLTreeNode::SinglyPartial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mPertinentLeavesCount = 2;

    createFullLink(L80.backIt(), Q24.backIt());
    createFullLink(L81.backIt(), Q24.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q24.mOrientInNorm.push_back(track_05.backIt());
    Q24.mOrientInReversed.push_back(track_06.backIt());
    Q24.mOrientOutNorm.push_back(track_07.backIt());
    Q24.mOrientOutReversed.push_back(track_08.backIt());

    auto& L70 = createL(pqTree, G);
    auto& L71 = createL(pqTree, G);
    L71.mGeneration = pqTree.mGeneration;
    L71.mPertinentType = BLTreeNode::Full;
    L71.mPertinentLeavesCount = 1;

    auto& L72 = createL(pqTree, G);

    auto& L73 = createL(pqTree, G);
    L73.mGeneration = pqTree.mGeneration;
    L73.mPertinentType = BLTreeNode::Full;
    L73.mPertinentLeavesCount = 1;

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(L71.backIt());
    P26children.push_back(L72.backIt());
    P26children.push_back(L73.backIt());
    P26children.push_back(Q24.backIt());
    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentType = BLTreeNode::SinglyPartial;
    P26.mPertinentChildrenCount = 4;
    P26.mPertinentLeavesCount = 6;

    createFullLink(L71.backIt(), P26.backIt());
    createFullLink(L73.backIt(), P26.backIt());

    P26.mSinglyPartialChild1 = Q23.backIt();
    P26.mSinglyPartialChild2 = Q24.backIt();

    list<node_list_it_t> Q22children;
    Q22children.push_back(P26.backIt());
    auto& Q22 = createQ(pqTree, Q22children);

    node_list_it_t root;
    EXPECT_EQ(pqTree.templateP6(P26, root), true);

    // Check P26
    EXPECT_EQ(root, Q23.backIt());
    EXPECT_EQ(P26.mChildren.size(), 3);
    auto cIt = P26.mChildren.begin();
    EXPECT_EQ(L70.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(Q23.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L72.backIt(), *cIt);

    // Check Q23
    EXPECT_EQ(Q23.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q23.mPertinentType, BLTreeNode::DoublyPartial);
    EXPECT_EQ(Q23.mPertinentChildrenCount, 5);
    EXPECT_EQ(Q23.mPertinentLeavesCount,   6);


    EXPECT_EQ(Q23.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q23.mCDPartialChild, pqTree.nil());

    auto prevIt = pqTree.nil();
    auto curIt = Q23.mEndChild1;
    EXPECT_EQ(L74.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L75.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L76.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L77.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& P27 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L81.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L80.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L79.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L78.backIt(), curIt);

    EXPECT_EQ(L74.isEmpty(), true);
    EXPECT_EQ(L75.isEmpty(), true);
    EXPECT_EQ(L76.isFull(),  true);
    EXPECT_EQ(L77.isFull(),  true);
    EXPECT_EQ(L81.isFull(),  true);
    EXPECT_EQ(L80.isFull(),  true);
    EXPECT_EQ(L79.isEmpty(), true);
    EXPECT_EQ(L78.isEmpty(), true);

    // Check P27
    EXPECT_EQ(P27.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P27.mPertinentType, BLTreeNode::Full);
    EXPECT_EQ(P27.mPertinentLeavesCount, 2);
    EXPECT_EQ(P27.mPertinentChildrenCount, 2);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L71.backIt());
    tmpList2.push_back(L73.backIt());
    EXPECT_EQ(checkUnorderedList(P27.mFullChildren,tmpList2),true);

    EXPECT_EQ(P27.mChildren.size(), 2);
    cIt = P27.mChildren.begin();
    EXPECT_EQ(L71.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L73.backIt(), *cIt);

    EXPECT_EQ(L71.isFull(), true);
    EXPECT_EQ(L73.isFull(), true);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L76.backIt());
    tmpList.push_back(L77.backIt());
    tmpList.push_back(P27.backIt());
    tmpList.push_back(L81.backIt());
    tmpList.push_back(L80.backIt());
    EXPECT_EQ(checkUnorderedList(Q23.mFullChildren,tmpList),true);


    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());
    
    EXPECT_EQ(checkOrientation(Q23, 
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L71.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L71.backIt(), P27.backIt()), true);
    EXPECT_EQ(L73.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L73.backIt(), P27.backIt()), true);
    EXPECT_EQ(P27.mFullChildren.size(), 2);
    EXPECT_EQ(P27.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(P27.backIt(), Q23.backIt()), true);

    EXPECT_EQ(L74.mFullChildrenSet, false);
    EXPECT_EQ(L75.mFullChildrenSet, false);
    EXPECT_EQ(L76.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L76.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L77.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L77.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L81.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L81.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L80.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L80.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L79.mFullChildrenSet, false);
    EXPECT_EQ(L78.mFullChildrenSet, false);
    EXPECT_EQ(Q23.mFullChildren.size(), 5);
    EXPECT_EQ(L70.mFullChildrenSet, false);
    EXPECT_EQ(L72.mFullChildrenSet, false);
    EXPECT_EQ(P26.mFullChildren.size(), 0);
    EXPECT_EQ(P26.mFullChildrenSet, false);
    EXPECT_EQ(Q22.mFullChildren.size(), 0);
}


/**  @brief tests BLPlanarizer::templateP6()
 */
TEST_F(BLPlanarityTesterTests, Test25){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);


    //   P6 positive #2
    //
    //         Q25                                Q25
    //          |                                  |
    //        _P28______________       =>         P28
    //       /  | \   \         \               /  |  \
    //     L82  |*L83 L84        |            L82 Q26 L84
    //       __Q26___       ____Q27___       ______|________________________
    //      /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //   *L85*L86 L87 L88 L89 L90*L91*L92  L88 L87*L86*L85*L83*L92*L91 L90 L89

    pqTree.initializeForOneIteration();

    auto& L85 = createL(pqTree, G);
    L85.mGeneration = pqTree.mGeneration;
    L85.mPertinentType = BLTreeNode::Full;
    L85.mPertinentLeavesCount = 1;
    auto& L86 = createL(pqTree, G);
    L86.mGeneration = pqTree.mGeneration;
    L86.mPertinentType = BLTreeNode::Full;
    L86.mPertinentLeavesCount = 1;
    auto& L87 = createL(pqTree, G);
    auto& L88 = createL(pqTree, G);

    list<node_list_it_t> Q26children;
    Q26children.push_back(L85.backIt());
    Q26children.push_back(L86.backIt());
    Q26children.push_back(L87.backIt());
    Q26children.push_back(L88.backIt());
    auto& Q26 = createQ(pqTree, Q26children);
    Q26.mGeneration = pqTree.mGeneration;
    Q26.mPertinentType = BLTreeNode::SinglyPartial;
    Q26.mPertinentChildrenCount = 2;
    Q26.mPertinentLeavesCount = 2;

    createFullLink(L85.backIt(), Q26.backIt());
    createFullLink(L86.backIt(), Q26.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q26.mOrientInNorm.push_back(track_01.backIt());
    Q26.mOrientInReversed.push_back(track_02.backIt());
    Q26.mOrientOutNorm.push_back(track_03.backIt());
    Q26.mOrientOutReversed.push_back(track_04.backIt());

    auto& L89 = createL(pqTree, G);
    auto& L90 = createL(pqTree, G);
    auto& L91 = createL(pqTree, G);
    L91.mGeneration = pqTree.mGeneration;
    L91.mPertinentType = BLTreeNode::Full;
    L91.mPertinentLeavesCount = 1;
    auto& L92 = createL(pqTree, G);
    L92.mGeneration = pqTree.mGeneration;
    L92.mPertinentType = BLTreeNode::Full;
    L92.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q27children;
    Q27children.push_back(L89.backIt());
    Q27children.push_back(L90.backIt());
    Q27children.push_back(L91.backIt());
    Q27children.push_back(L92.backIt());
    auto& Q27 = createQ(pqTree, Q27children);
    Q27.mGeneration = pqTree.mGeneration;
    Q27.mPertinentType = BLTreeNode::SinglyPartial;
    Q27.mPertinentChildrenCount = 2;
    Q27.mPertinentLeavesCount = 2;

    createFullLink(L91.backIt(), Q27.backIt());
    createFullLink(L92.backIt(), Q27.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q27.mOrientInNorm.push_back(track_05.backIt());
    Q27.mOrientInReversed.push_back(track_06.backIt());
    Q27.mOrientOutNorm.push_back(track_07.backIt());
    Q27.mOrientOutReversed.push_back(track_08.backIt());

    auto& L82 = createL(pqTree, G);
    auto& L83 = createL(pqTree, G);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = BLTreeNode::Full;
    L83.mPertinentLeavesCount = 1;
    auto& L84 = createL(pqTree, G);

    list<node_list_it_t> P28children;
    P28children.push_back(L82.backIt());
    P28children.push_back(Q26.backIt());
    P28children.push_back(L83.backIt());
    P28children.push_back(L84.backIt());
    P28children.push_back(Q27.backIt());

    auto& P28 = createP(pqTree, P28children);
    P28.mGeneration = pqTree.mGeneration;
    P28.mPertinentChildrenCount = 3;
    P28.mPertinentLeavesCount = 5;
    P28.mSinglyPartialChild1 = Q26.backIt();
    P28.mSinglyPartialChild2 = Q27.backIt();

    createFullLink(L83.backIt(), P28.backIt());

    list<node_list_it_t> Q25children;
    Q25children.push_back(P28.backIt());
    auto& Q25 = createQ(pqTree, Q25children);

    node_list_it_t root;
    EXPECT_EQ(pqTree.templateP6(P28, root), true);

    EXPECT_EQ(root, Q26.backIt());

    // Check P28
    EXPECT_EQ(P28.mChildren.size(), 3);
    auto cIt = P28.mChildren.begin();
    EXPECT_EQ(L82.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(Q26.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L84.backIt(), *cIt);

    // Check Q26
    EXPECT_EQ(Q26.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q26.mPertinentType, BLTreeNode::DoublyPartial);
    EXPECT_EQ(Q26.mPertinentChildrenCount, 5);
    EXPECT_EQ(Q26.mPertinentLeavesCount,   5);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L86.backIt());
    tmpList.push_back(L85.backIt());
    tmpList.push_back(L83.backIt());
    tmpList.push_back(L92.backIt());
    tmpList.push_back(L91.backIt());
    EXPECT_EQ(checkUnorderedList(Q26.mFullChildren,tmpList),true);

    EXPECT_EQ(Q26.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q26.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q26.mCDPartialChild, pqTree.nil());

    auto prevIt = pqTree.nil();
    auto curIt = (Q26.mEndChild1==L88.backIt())?Q26.mEndChild1:Q26.mEndChild2;
    EXPECT_EQ(L88.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L87.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L86.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L85.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L83.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L92.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L91.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L90.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L89.backIt(), curIt);

    EXPECT_EQ(L88.isEmpty(), true);
    EXPECT_EQ(L87.isEmpty(), true);
    EXPECT_EQ(L86.isFull(),  true);
    EXPECT_EQ(L85.isFull(),  true);
    EXPECT_EQ(L83.isFull(),  true);
    EXPECT_EQ(L92.isFull(),  true);
    EXPECT_EQ(L91.isFull(),  true);
    EXPECT_EQ(L90.isEmpty(), true);
    EXPECT_EQ(L89.isEmpty(), true);

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

    EXPECT_EQ(checkOrientation(Q26,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L88.mFullChildrenSet, false);
    EXPECT_EQ(L87.mFullChildrenSet, false);
    EXPECT_EQ(L86.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L86.backIt(), Q26.backIt()), true);
    EXPECT_EQ(L85.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L85.backIt(), Q26.backIt()), true);
    EXPECT_EQ(L83.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L83.backIt(), Q26.backIt()), true);
    EXPECT_EQ(L92.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L92.backIt(), Q26.backIt()), true);
    EXPECT_EQ(L91.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L91.backIt(), Q26.backIt()), true);
    EXPECT_EQ(L90.mFullChildrenSet, false);
    EXPECT_EQ(L89.mFullChildrenSet, false);
    EXPECT_EQ(Q26.mFullChildren.size(), 5);
    EXPECT_EQ(Q26.mFullChildrenSet, false);
    EXPECT_EQ(L82.mFullChildrenSet, false);
    EXPECT_EQ(L84.mFullChildrenSet, false);
    EXPECT_EQ(P28.mFullChildren.size(), 0);
    EXPECT_EQ(P28.mFullChildrenSet, false);
    EXPECT_EQ(Q25.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateP6()
 */
TEST_F(BLPlanarityTesterTests, Test26){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P6 positive #3
    //
    //         Q28                                 Q28
    //          |                                   |
    //        _P29______________       =>          P29
    //       /  |     \         \                /  |  \
    //     L93  |    L94        |              L93 Q29 L94
    //       __Q29___       ____Q30___        ______|_______________________
    //      /  /  \  \     /   /   \  \      |   |   |   |   |    |    |    |
    //    L95 L96*L97*L98*L99*L100 L101L102 L95 L96*L97*L98*L99*L100 L101 L102

    pqTree.initializeForOneIteration();

    auto& L95 = createL(pqTree, G);
    auto& L96 = createL(pqTree, G);

    auto& L97 = createL(pqTree, G);
    L97.mGeneration = pqTree.mGeneration;
    L97.mPertinentType = BLTreeNode::Full;
    L97.mPertinentLeavesCount = 1;

    auto& L98 = createL(pqTree, G);
    L98.mGeneration = pqTree.mGeneration;
    L98.mPertinentType = BLTreeNode::Full;
    L98.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q29children;
    Q29children.push_back(L95.backIt());
    Q29children.push_back(L96.backIt());
    Q29children.push_back(L97.backIt());
    Q29children.push_back(L98.backIt());
    auto& Q29 = createQ(pqTree, Q29children);
    Q29.mGeneration = pqTree.mGeneration;
    Q29.mPertinentType = BLTreeNode::SinglyPartial;
    Q29.mPertinentChildrenCount = 2;
    Q29.mPertinentLeavesCount = 2;


    createFullLink(L97.backIt(), Q29.backIt());
    createFullLink(L98.backIt(), Q29.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q29.mOrientInNorm.push_back(track_01.backIt());
    Q29.mOrientInReversed.push_back(track_02.backIt());
    Q29.mOrientOutNorm.push_back(track_03.backIt());
    Q29.mOrientOutReversed.push_back(track_04.backIt());

    auto& L99 = createL(pqTree, G);
    L99.mGeneration = pqTree.mGeneration;
    L99.mPertinentType = BLTreeNode::Full;
    L99.mPertinentLeavesCount = 1;

    auto& L100 = createL(pqTree, G);
    L100.mGeneration = pqTree.mGeneration;
    L100.mPertinentType = BLTreeNode::Full;
    L100.mPertinentLeavesCount = 1;

    auto& L101 = createL(pqTree, G);
    auto& L102 = createL(pqTree, G);

    list<node_list_it_t> Q30children;
    Q30children.push_back(L99.backIt());
    Q30children.push_back(L100.backIt());
    Q30children.push_back(L101.backIt());
    Q30children.push_back(L102.backIt());
    auto& Q30 = createQ(pqTree, Q30children);
    Q30.mGeneration = pqTree.mGeneration;
    Q30.mPertinentType = BLTreeNode::SinglyPartial;
    Q30.mPertinentChildrenCount = 2;
    Q30.mPertinentLeavesCount = 2;

    createFullLink(L99.backIt(), Q30.backIt());
    createFullLink(L100.backIt(), Q30.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q30.mOrientInNorm.push_back(track_05.backIt());
    Q30.mOrientInReversed.push_back(track_06.backIt());
    Q30.mOrientOutNorm.push_back(track_07.backIt());
    Q30.mOrientOutReversed.push_back(track_08.backIt());


    auto& L93 = createL(pqTree, G);
    auto& L94 = createL(pqTree, G);

    list<node_list_it_t> P29children;
    P29children.push_back(L93.backIt());
    P29children.push_back(Q29.backIt());
    P29children.push_back(L94.backIt());
    P29children.push_back(Q30.backIt());

    auto& P29 = createP(pqTree, P29children);
    P29.mGeneration = pqTree.mGeneration;
    P29.mPertinentChildrenCount = 2;
    P29.mPertinentLeavesCount = 4;
    P29.mSinglyPartialChild1 = Q29.backIt();
    P29.mSinglyPartialChild2 = Q30.backIt();

    list<node_list_it_t> Q28children;
    Q28children.push_back(P29.backIt());
    auto& Q28 = createQ(pqTree, Q28children);

    node_list_it_t root;
    EXPECT_EQ(pqTree.templateP6(P29, root), true);
    EXPECT_EQ(root, Q29.backIt());

    // Check P29
    EXPECT_EQ(P29.mChildren.size(), 3);
    auto cIt = P29.mChildren.begin();
    EXPECT_EQ(L93.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(Q29.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L94.backIt(), *cIt);

    // Check Q29
    EXPECT_EQ(Q29.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q29.mPertinentType, BLTreeNode::DoublyPartial);
    EXPECT_EQ(Q29.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q29.mPertinentLeavesCount,   4);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L97.backIt());
    tmpList.push_back(L98.backIt());
    tmpList.push_back(L99.backIt());
    tmpList.push_back(L100.backIt());
    EXPECT_EQ(checkUnorderedList(Q29.mFullChildren,tmpList),true);
    EXPECT_EQ(Q29.mFullChildren.size(),      4);

    EXPECT_EQ(Q29.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q29.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q29.mCDPartialChild, pqTree.nil());

    auto prevIt = pqTree.nil();
    auto curIt = (Q29.mEndChild1==L95.backIt())?Q29.mEndChild1:Q29.mEndChild2;

    EXPECT_EQ(L95.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L96.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L97.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L98.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L99.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L100.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L101.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L102.backIt(), curIt);

    EXPECT_EQ(L95.isEmpty(), true);
    EXPECT_EQ(L96.isEmpty(), true);
    EXPECT_EQ(L97.isFull(),  true);
    EXPECT_EQ(L98.isFull(),  true);
    EXPECT_EQ(L99.isFull(),  true);
    EXPECT_EQ(L100.isFull(),  true);
    EXPECT_EQ(L101.isEmpty(),  true);
    EXPECT_EQ(L102.isEmpty(), true);

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

    EXPECT_EQ(checkOrientation(Q29,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L95.mFullChildrenSet, false);
    EXPECT_EQ(L96.mFullChildrenSet, false);
    EXPECT_EQ(L97.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L97.backIt(), Q29.backIt()), true);
    EXPECT_EQ(L98.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L98.backIt(), Q29.backIt()), true);
    EXPECT_EQ(L99.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L99.backIt(), Q29.backIt()), true);
    EXPECT_EQ(L100.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L100.backIt(), Q29.backIt()), true);
    EXPECT_EQ(L101.mFullChildrenSet, false);
    EXPECT_EQ(L102.mFullChildrenSet, false);
    EXPECT_EQ(Q29.mFullChildren.size(), 4);
    EXPECT_EQ(Q29.mFullChildrenSet, false);
    EXPECT_EQ(L93.mFullChildrenSet, false);
    EXPECT_EQ(L94.mFullChildrenSet, false);
    EXPECT_EQ(P29.mFullChildren.size(), 0);
    EXPECT_EQ(P29.mFullChildrenSet, false);
    EXPECT_EQ(Q28.mFullChildren.size(), 0);
}


/**  @brief tests BLPlanarizer::templateP6()
 */
TEST_F(BLPlanarityTesterTests, Test27){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);


    //   P6 positive #4
    //
    //   ____Q31___                           ____Q31_____
    //  |     |    |                         |     |      |
    // L113   |   L114                     L113    |     L114
    //       P30_________________       =>        Q32
    //        | \       \        \         ________|________________________
    //        |*L103   *L104     |        |   |   |    |   |   |    |   |   |
    //     __Q32_____       ____Q33____ L109L110*L111*L112P31*L105*L106L107L108
    //    /  /   \   \     /   /   \   \                  / \
    //*L105*L106L107L108L109L110*L111*L112           *L103  *L104

    pqTree.initializeForOneIteration();

    auto& L105 = createL(pqTree, G);
    L105.mGeneration = pqTree.mGeneration;
    L105.mPertinentType = BLTreeNode::Full;
    L105.mPertinentLeavesCount = 1;
    auto& L106 = createL(pqTree, G);
    L106.mGeneration = pqTree.mGeneration;
    L106.mPertinentType = BLTreeNode::Full;
    L106.mPertinentLeavesCount = 1;
    auto& L107 = createL(pqTree, G);
    auto& L108 = createL(pqTree, G);

    list<node_list_it_t> Q32children;
    Q32children.push_back(L105.backIt());
    Q32children.push_back(L106.backIt());
    Q32children.push_back(L107.backIt());
    Q32children.push_back(L108.backIt());
    auto& Q32 = createQ(pqTree, Q32children);
    Q32.mGeneration = pqTree.mGeneration;
    Q32.mPertinentType = BLTreeNode::SinglyPartial;
    Q32.mPertinentChildrenCount = 2;
    Q32.mPertinentLeavesCount = 2;

    createFullLink(L105.backIt(), Q32.backIt());
    createFullLink(L106.backIt(), Q32.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q32.mOrientInNorm.push_back(track_01.backIt());
    Q32.mOrientInReversed.push_back(track_02.backIt());
    Q32.mOrientOutNorm.push_back(track_03.backIt());
    Q32.mOrientOutReversed.push_back(track_04.backIt());

    auto& L109 = createL(pqTree, G);
    auto& L110 = createL(pqTree, G);
    auto& L111 = createL(pqTree, G);
    L111.mGeneration = pqTree.mGeneration;
    L111.mPertinentType = BLTreeNode::Full;
    L111.mPertinentLeavesCount = 1;
    auto& L112 = createL(pqTree, G);
    L112.mGeneration = pqTree.mGeneration;
    L112.mPertinentType = BLTreeNode::Full;
    L112.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q33children;
    Q33children.push_back(L109.backIt());
    Q33children.push_back(L110.backIt());
    Q33children.push_back(L111.backIt());
    Q33children.push_back(L112.backIt());
    auto& Q33 = createQ(pqTree, Q33children);
    Q33.mGeneration = pqTree.mGeneration;
    Q33.mPertinentType = BLTreeNode::SinglyPartial;
    Q33.mPertinentChildrenCount = 2;
    Q33.mPertinentLeavesCount = 2;

    createFullLink(L111.backIt(), Q33.backIt());
    createFullLink(L112.backIt(), Q33.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q33.mOrientInNorm.push_back(track_05.backIt());
    Q33.mOrientInReversed.push_back(track_06.backIt());
    Q33.mOrientOutNorm.push_back(track_07.backIt());
    Q33.mOrientOutReversed.push_back(track_08.backIt());

    auto& L103 = createL(pqTree, G);
    L103.mGeneration = pqTree.mGeneration;
    L103.mPertinentType = BLTreeNode::Full;
    L103.mPertinentLeavesCount = 1;

    auto& L104 = createL(pqTree, G);
    L104.mGeneration = pqTree.mGeneration;
    L104.mPertinentType = BLTreeNode::Full;
    L104.mPertinentLeavesCount = 1;

    list<node_list_it_t> P30children;
    P30children.push_back(Q32.backIt());
    P30children.push_back(L103.backIt());
    P30children.push_back(L104.backIt());
    P30children.push_back(Q33.backIt());

    auto& P30 = createP(pqTree, P30children);
    P30.mGeneration = pqTree.mGeneration;
    P30.mPertinentChildrenCount = 4;
    P30.mPertinentLeavesCount = 6;
    P30.mSinglyPartialChild1 = Q32.backIt();
    P30.mSinglyPartialChild2 = Q33.backIt();

    createFullLink(L103.backIt(), P30.backIt());
    createFullLink(L104.backIt(), P30.backIt());

    auto& L113 = createL(pqTree, G);
    auto& L114 = createL(pqTree, G);

    list<node_list_it_t> Q31children;
    Q31children.push_back(L113.backIt());
    Q31children.push_back(P30.backIt());
    Q31children.push_back(L114.backIt());

    auto& Q31 = createQ(pqTree, Q31children);

    node_list_it_t root;

    EXPECT_EQ(pqTree.templateP6(P30, root), true);

    EXPECT_EQ(root, Q32.backIt());

    // Check Q32 (new root)
    EXPECT_EQ(areImmediateSiblings(L113,Q32), true);
    EXPECT_EQ(areImmediateSiblings(L114,Q32), true);
    EXPECT_EQ(Q31.mEndChild1, L113.backIt());
    EXPECT_EQ(Q31.mEndChild2, L114.backIt());

    // Check Q32
    EXPECT_EQ(Q32.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q32.mPertinentType, BLTreeNode::DoublyPartial);
    EXPECT_EQ(Q32.mPertinentChildrenCount, 5);
    EXPECT_EQ(Q32.mPertinentLeavesCount,   6);


    EXPECT_EQ(Q32.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q32.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q32.mCDPartialChild, pqTree.nil());

    auto prevIt = pqTree.nil();
    auto curIt = (Q32.mEndChild1==L109.backIt())?Q32.mEndChild1:Q32.mEndChild2;

    EXPECT_EQ(L109.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L110.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L111.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L112.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& P31 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L105.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L106.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L107.backIt(), curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(L108.backIt(), curIt);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L111.backIt());
    tmpList.push_back(L112.backIt());
    tmpList.push_back(P31.backIt());
    tmpList.push_back(L105.backIt());
    tmpList.push_back(L106.backIt());
    EXPECT_EQ(checkUnorderedList(Q32.mFullChildren,tmpList),true);


    EXPECT_EQ(L109.isEmpty(), true);
    EXPECT_EQ(L110.isEmpty(), true);
    EXPECT_EQ(L111.isFull(),  true);
    EXPECT_EQ(L112.isFull(),  true);
    EXPECT_EQ(P31.isFull(),  true);
    EXPECT_EQ(L105.isFull(),  true);
    EXPECT_EQ(L106.isFull(),  true);
    EXPECT_EQ(L107.isEmpty(),  true);
    EXPECT_EQ(L108.isEmpty(), true);

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

    EXPECT_EQ(checkOrientation(Q32,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L103.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L103.backIt(), P31.backIt()), true);
    EXPECT_EQ(L104.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L104.backIt(), P31.backIt()), true);
    EXPECT_EQ(P31.mFullChildren.size(), 2);

    EXPECT_EQ(L109.mFullChildrenSet, false);
    EXPECT_EQ(L110.mFullChildrenSet, false);
    EXPECT_EQ(L111.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L111.backIt(), Q32.backIt()), true);
    EXPECT_EQ(L112.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L112.backIt(), Q32.backIt()), true);
    EXPECT_EQ(P31.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(P31.backIt(), Q32.backIt()), true);
    EXPECT_EQ(L105.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L105.backIt(), Q32.backIt()), true);
    EXPECT_EQ(L106.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L106.backIt(), Q32.backIt()), true);
    EXPECT_EQ(L107.mFullChildrenSet, false);
    EXPECT_EQ(L108.mFullChildrenSet, false);
    EXPECT_EQ(Q32.mFullChildren.size(), 5);
    EXPECT_EQ(Q32.mFullChildrenSet, false);
    EXPECT_EQ(L113.mFullChildrenSet, false);
    EXPECT_EQ(L114.mFullChildrenSet, false);
    EXPECT_EQ(Q31.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarizer::templateP7()
 */
TEST_F(BLPlanarityTesterTests, Test28){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P7 positive #1
    //
    //     __Q22___                          ___Q22___
    //    |   |    |                        |    |    |
    //  *L82  |  *L83                     *L82   |  *L83
    //      _P26______________       =>         P_N1____
    //     /  | \   \   \     \                  |  \   \
    //   L70  |*L71 L72*L73    |                Q23*L71*L73
    //     __Q23___       ____Q24___       ______|________________________
    //    /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //  L74 L75*L76*L77 L78 L79*L80*L81 *L77*L76 L75 L74 P26 L78 L79*L80*L81
    //                                                   / \
    //                                                 L70 L72

    pqTree.initializeForOneIteration();

    auto& L74 = createL(pqTree, G);
    auto& L75 = createL(pqTree, G);
    auto& L76 = createL(pqTree, G);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = BLTreeNode::Full;
    L76.mPertinentLeavesCount = 1;
    auto& L77 = createL(pqTree, G);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = BLTreeNode::Full;
    L77.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = BLTreeNode::SinglyPartial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mPertinentLeavesCount = 2;

    createFullLink(L76.backIt(), Q23.backIt());
    createFullLink(L77.backIt(), Q23.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q23.mOrientInNorm.push_back(track_01.backIt());
    Q23.mOrientInReversed.push_back(track_02.backIt());
    Q23.mOrientOutNorm.push_back(track_03.backIt());
    Q23.mOrientOutReversed.push_back(track_04.backIt());

    auto& L78 = createL(pqTree, G);
    auto& L79 = createL(pqTree, G);
    auto& L80 = createL(pqTree, G);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = BLTreeNode::Full;
    L80.mPertinentLeavesCount = 1;
    auto& L81 = createL(pqTree, G);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = BLTreeNode::Full;
    L81.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = BLTreeNode::SinglyPartial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mPertinentLeavesCount = 2;

    createFullLink(L80.backIt(), Q24.backIt());
    createFullLink(L81.backIt(), Q24.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q24.mOrientInNorm.push_back(track_05.backIt());
    Q24.mOrientInReversed.push_back(track_06.backIt());
    Q24.mOrientOutNorm.push_back(track_07.backIt());
    Q24.mOrientOutReversed.push_back(track_08.backIt());


    auto& L70 = createL(pqTree, G);
    auto& L71 = createL(pqTree, G);
    L71.mGeneration = pqTree.mGeneration;
    L71.mPertinentType = BLTreeNode::Full;
    L71.mPertinentLeavesCount = 1;
    auto& L72 = createL(pqTree, G);
    auto& L73 = createL(pqTree, G);
    L73.mGeneration = pqTree.mGeneration;
    L73.mPertinentType = BLTreeNode::Full;
    L73.mPertinentLeavesCount = 1;

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
    P26.mPertinentLeavesCount = 6;
    P26.mSinglyPartialChild1 = Q23.backIt();
    P26.mSinglyPartialChild2 = Q24.backIt();

    createFullLink(L71.backIt(), P26.backIt());
    createFullLink(L73.backIt(), P26.backIt());

    auto& L82 = createL(pqTree, G);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = BLTreeNode::Full;
    L82.mPertinentLeavesCount = 1;

    auto& L83 = createL(pqTree, G);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = BLTreeNode::Full;
    L83.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(P26.backIt());
    Q22children.push_back(L83.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    P26.mParent = Q22.backIt();

    bool earlyOut;

    EXPECT_EQ(pqTree.templateP7(P26, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    // Check Q22
    auto& P_N1 = pqTree.toNodeRef(Q22.mCDPartialChild);

    auto prevIt = pqTree.nil();
    auto curIt  = Q22.mEndChild1;
    EXPECT_EQ(curIt, L82.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, P_N1.backIt());  
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L83.backIt());  
    
    // Check P_N1
    EXPECT_EQ(P_N1.mNodeType,            BLTreeNode::PType);
    EXPECT_EQ(P_N1.mPertinentType,       BLTreeNode::CDPartial);
    EXPECT_EQ(P_N1.mCDPartialChild,      Q23.backIt());

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L71.backIt());
    tmpList2.push_back(L73.backIt());
    EXPECT_EQ(checkUnorderedList(P_N1.mFullChildren,tmpList2),true);

    EXPECT_EQ(P_N1.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(P_N1.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(P_N1.mPertinentChildrenCount, 3);
    EXPECT_EQ(P_N1.mPertinentLeavesCount, 6);
    EXPECT_EQ(P_N1.mChildren.size(),3);

    tmpList2.push_back(Q23.backIt());
    EXPECT_EQ(checkUnorderedList(P_N1.mChildren,tmpList2),true);

    EXPECT_EQ(L71.isFull(), true);
    EXPECT_EQ(L73.isFull(), true);

    prevIt = pqTree.nil();
    curIt  = (Q23.mEndChild1==L77.backIt())?Q23.mEndChild1:Q23.mEndChild2;
    EXPECT_EQ(curIt, L77.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L76.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L75.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L74.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, P26.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L78.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L79.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L80.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L81.backIt());

    EXPECT_EQ(L77.isFull(), true);
    EXPECT_EQ(L76.isFull(), true);
    EXPECT_EQ(L75.isEmpty(), true);
    EXPECT_EQ(L74.isEmpty(), true);
    EXPECT_EQ(P26.isEmpty(), true);
    EXPECT_EQ(L78.isEmpty(), true);
    EXPECT_EQ(L79.isEmpty(), true);
    EXPECT_EQ(L80.isFull(), true);
    EXPECT_EQ(L81.isFull(), true);

    list<node_list_it_t> tmpList3;
    tmpList3.push_back(L77.backIt());
    tmpList3.push_back(L76.backIt());
    tmpList3.push_back(L80.backIt());
    tmpList3.push_back(L81.backIt());
    EXPECT_EQ(checkUnorderedList(Q23.mFullChildren,tmpList3),true);

    EXPECT_EQ(P26.mChildren.size(), 2);
    auto cIt = P26.mChildren.begin();
    EXPECT_EQ(*cIt, L70.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L72.backIt());

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    EXPECT_EQ(checkOrientation(Q23,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


    EXPECT_EQ(L70.mFullChildrenSet, false);
    EXPECT_EQ(L72.mFullChildrenSet, false);
    EXPECT_EQ(P26.mFullChildren.size(), 0);
    EXPECT_EQ(P26.mFullChildrenSet, false);
    EXPECT_EQ(L77.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L77.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L76.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L76.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L75.mFullChildrenSet, false);
    EXPECT_EQ(L74.mFullChildrenSet, false);
    EXPECT_EQ(L78.mFullChildrenSet, false);
    EXPECT_EQ(L79.mFullChildrenSet, false);
    EXPECT_EQ(L80.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L80.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L81.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L80.backIt(), Q23.backIt()), true);
    EXPECT_EQ(Q23.mFullChildren.size(), 4);
    EXPECT_EQ(L71.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L71.backIt(), P_N1.backIt()), true);
    EXPECT_EQ(L73.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L73.backIt(), P_N1.backIt()), true);
    EXPECT_EQ(P_N1.mFullChildren.size(), 2);
    EXPECT_EQ(P_N1.mFullChildrenSet, false);

}



/**  @brief tests BLPlanarizer::templateP7()
 */
TEST_F(BLPlanarityTesterTests, Test28_2){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P7 positive #1
    //
    //     __Q22___                          ___Q22___
    //    |   |    |                        |    |    |
    //  *L82  |  *L83                     *L82   |  *L83
    //      _P26______________       =>          |
    //     /  |     \         \                  |
    //   L70  |    L72        |                Q23
    //     __Q23___       ____Q24___       ______|________________________
    //    /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //  L74 L75*L76*L77 L78 L79*L80*L81 *L77*L76 L75 L74 P26 L78 L79*L80*L81
    //                                                   / \
    //                                                 L70 L72

    pqTree.initializeForOneIteration();

    auto& L74 = createL(pqTree, G);
    auto& L75 = createL(pqTree, G);
    auto& L76 = createL(pqTree, G);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = BLTreeNode::Full;
    L76.mPertinentLeavesCount = 1;
    auto& L77 = createL(pqTree, G);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = BLTreeNode::Full;
    L77.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = BLTreeNode::SinglyPartial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mPertinentLeavesCount = 2;

    createFullLink(L76.backIt(), Q23.backIt());
    createFullLink(L77.backIt(), Q23.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q23.mOrientInNorm.push_back(track_01.backIt());
    Q23.mOrientInReversed.push_back(track_02.backIt());
    Q23.mOrientOutNorm.push_back(track_03.backIt());
    Q23.mOrientOutReversed.push_back(track_04.backIt());

    auto& L78 = createL(pqTree, G);
    auto& L79 = createL(pqTree, G);
    auto& L80 = createL(pqTree, G);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = BLTreeNode::Full;
    L80.mPertinentLeavesCount = 1;
    auto& L81 = createL(pqTree, G);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = BLTreeNode::Full;
    L81.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = BLTreeNode::SinglyPartial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mPertinentLeavesCount = 2;

    createFullLink(L80.backIt(), Q24.backIt());
    createFullLink(L81.backIt(), Q24.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q24.mOrientInNorm.push_back(track_05.backIt());
    Q24.mOrientInReversed.push_back(track_06.backIt());
    Q24.mOrientOutNorm.push_back(track_07.backIt());
    Q24.mOrientOutReversed.push_back(track_08.backIt());


    auto& L70 = createL(pqTree, G);
    auto& L72 = createL(pqTree, G);

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(L72.backIt());
    P26children.push_back(Q24.backIt());

    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 4;
    P26.mPertinentLeavesCount = 6;
    P26.mSinglyPartialChild1 = Q23.backIt();
    P26.mSinglyPartialChild2 = Q24.backIt();

    auto& L82 = createL(pqTree, G);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = BLTreeNode::Full;
    L82.mPertinentLeavesCount = 1;

    auto& L83 = createL(pqTree, G);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = BLTreeNode::Full;
    L83.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(P26.backIt());
    Q22children.push_back(L83.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    P26.mParent = Q22.backIt();

    bool earlyOut;

    EXPECT_EQ(pqTree.templateP7(P26, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    // Check Q22
    EXPECT_EQ(Q22.mCDPartialChild, Q23.backIt());

    auto prevIt = pqTree.nil();
    auto curIt  = Q22.mEndChild1;
    EXPECT_EQ(curIt, L82.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, Q23.backIt());  
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L83.backIt());  
    
    // Check Q23
    EXPECT_EQ(Q23.mNodeType,            BLTreeNode::QType);
    EXPECT_EQ(Q23.mPertinentType,       BLTreeNode::CDPartial);
    EXPECT_EQ(Q23.mCDPartialChild,      pqTree.nil());

    EXPECT_EQ(Q23.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(Q23.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q23.mPertinentLeavesCount, 4);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L77.backIt());
    tmpList.push_back(L76.backIt());
    tmpList.push_back(L80.backIt());
    tmpList.push_back(L81.backIt());
    EXPECT_EQ(checkUnorderedList(Q23.mFullChildren,tmpList),true);

    prevIt = pqTree.nil();
    curIt  = (Q23.mEndChild1==L77.backIt())?Q23.mEndChild1:Q23.mEndChild2;
    EXPECT_EQ(curIt, L77.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L76.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L75.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L74.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, P26.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L78.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L79.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L80.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L81.backIt());

    EXPECT_EQ(L77.isFull(), true);
    EXPECT_EQ(L76.isFull(), true);
    EXPECT_EQ(L75.isEmpty(), true);
    EXPECT_EQ(L74.isEmpty(), true);
    EXPECT_EQ(P26.isEmpty(), true);
    EXPECT_EQ(L78.isEmpty(), true);
    EXPECT_EQ(L79.isEmpty(), true);
    EXPECT_EQ(L80.isFull(), true);
    EXPECT_EQ(L81.isFull(), true);

    EXPECT_EQ(P26.mChildren.size(), 2);
    auto cIt = P26.mChildren.begin();
    EXPECT_EQ(*cIt, L70.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L72.backIt());

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    EXPECT_EQ(checkOrientation(Q23,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L77.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L77.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L76.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L76.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L75.mFullChildrenSet, false);
    EXPECT_EQ(L74.mFullChildrenSet, false);
    EXPECT_EQ(L70.mFullChildrenSet, false);
    EXPECT_EQ(L72.mFullChildrenSet, false);
    EXPECT_EQ(P26.mFullChildren.size(), 0);
    EXPECT_EQ(P26.mFullChildrenSet, false);
    EXPECT_EQ(L78.mFullChildrenSet, false);
    EXPECT_EQ(L79.mFullChildrenSet, false);
    EXPECT_EQ(L80.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L80.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L81.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L81.backIt(), Q23.backIt()), true);
    EXPECT_EQ(Q23.mFullChildren.size(), 4);


}


/**  @brief tests BLPlanarizer::templateP7()
 */
TEST_F(BLPlanarityTesterTests, Test29){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P7 positive #2
    //
    //     __Q22___                          ___Q22___
    //    |   |    |                        |    |    |
    //  *L82  |  *L83                     *L82   |  *L83
    //      _P26______________       =>         P26_____
    //     /  | \   \         \                  |  \   \
    //   L70  |*L71*L73       |                Q23*L71*L73
    //     __Q23___       ____Q24___       ______|________________________
    //    /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    //  L74 L75*L76*L77*L78*L79 L80 L81 *L77*L76 L75 L74 L70 L81 L80*L79*L78
    //
    //

    pqTree.initializeForOneIteration();

    auto& L74 = createL(pqTree, G);
    auto& L75 = createL(pqTree, G);
    auto& L76 = createL(pqTree, G);
    L76.mGeneration = pqTree.mGeneration;
    L76.mPertinentType = BLTreeNode::Full;
    L76.mPertinentLeavesCount = 1;
    auto& L77 = createL(pqTree, G);
    L77.mGeneration = pqTree.mGeneration;
    L77.mPertinentType = BLTreeNode::Full;
    L77.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = BLTreeNode::SinglyPartial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mPertinentLeavesCount = 2;

    createFullLink(L76.backIt(), Q23.backIt());
    createFullLink(L77.backIt(), Q23.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q23.mOrientInNorm.push_back(track_01.backIt());
    Q23.mOrientInReversed.push_back(track_02.backIt());
    Q23.mOrientOutNorm.push_back(track_03.backIt());
    Q23.mOrientOutReversed.push_back(track_04.backIt());

    auto& L78 = createL(pqTree, G);
    L78.mGeneration = pqTree.mGeneration;
    L78.mPertinentType = BLTreeNode::Full;
    L78.mPertinentLeavesCount = 1;
    auto& L79 = createL(pqTree, G);
    L79.mGeneration = pqTree.mGeneration;
    L79.mPertinentType = BLTreeNode::Full;
    L79.mPertinentLeavesCount = 1;
    auto& L80 = createL(pqTree, G);
    auto& L81 = createL(pqTree, G);

    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = BLTreeNode::SinglyPartial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mPertinentLeavesCount = 2;

    createFullLink(L78.backIt(), Q24.backIt());
    createFullLink(L79.backIt(), Q24.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q24.mOrientInNorm.push_back(track_05.backIt());
    Q24.mOrientInReversed.push_back(track_06.backIt());
    Q24.mOrientOutNorm.push_back(track_07.backIt());
    Q24.mOrientOutReversed.push_back(track_08.backIt());

    auto& L70 = createL(pqTree, G);
    auto& L71 = createL(pqTree, G);
    L71.mGeneration = pqTree.mGeneration;
    L71.mPertinentType = BLTreeNode::Full;
    L71.mPertinentLeavesCount = 1;
    auto& L73 = createL(pqTree, G);
    L73.mGeneration = pqTree.mGeneration;
    L73.mPertinentType = BLTreeNode::Full;
    L73.mPertinentLeavesCount = 1;

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(L71.backIt());
    P26children.push_back(L73.backIt());
    P26children.push_back(Q24.backIt());

    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 4;
    P26.mPertinentLeavesCount = 6;
    P26.mSinglyPartialChild1 = Q23.backIt();
    P26.mSinglyPartialChild2 = Q24.backIt();

    createFullLink(L71.backIt(), P26.backIt());
    createFullLink(L73.backIt(), P26.backIt());

    auto& L82 = createL(pqTree, G);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = BLTreeNode::Full;
    L82.mPertinentLeavesCount = 1;

    auto& L83 = createL(pqTree, G);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = BLTreeNode::Full;
    L83.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(P26.backIt());
    Q22children.push_back(L83.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    P26.mParent = Q22.backIt();

    bool earlyOut;

    EXPECT_EQ(pqTree.templateP7(P26, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    // Check Q22
    EXPECT_EQ(Q22.mCDPartialChild, P26.backIt());

    // Check P26
    EXPECT_EQ(P26.mNodeType,            BLTreeNode::PType);
    EXPECT_EQ(P26.mPertinentType,       BLTreeNode::CDPartial);
    EXPECT_EQ(P26.mCDPartialChild,      Q23.backIt());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L71.backIt());
    tmpList.push_back(L73.backIt());
    EXPECT_EQ(checkUnorderedList(P26.mFullChildren,tmpList),true);

    EXPECT_EQ(P26.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(P26.mSinglyPartialChild2, pqTree.nil());
    EXPECT_EQ(P26.mPertinentChildrenCount, 3);
    EXPECT_EQ(P26.mPertinentLeavesCount, 6);
    EXPECT_EQ(P26.mChildren.size(),3);
    auto cIt = P26.mChildren.begin();
    EXPECT_EQ(*cIt, Q23.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L71.backIt());
    EXPECT_EQ(L71.isFull(), true);
    cIt++;
    EXPECT_EQ(*cIt, L73.backIt());
    EXPECT_EQ(L73.isFull(), true);

    auto prevIt = pqTree.nil();
    auto curIt  = (Q23.mEndChild1==L77.backIt())?Q23.mEndChild1:Q23.mEndChild2;
    EXPECT_EQ(curIt, L77.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L76.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L75.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L74.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L70.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L81.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L80.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L79.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L78.backIt());

    EXPECT_EQ(L77.isFull(), true);
    EXPECT_EQ(L76.isFull(), true);
    EXPECT_EQ(L75.isEmpty(), true);
    EXPECT_EQ(L74.isEmpty(), true);
    EXPECT_EQ(L70.isEmpty(), true);
    EXPECT_EQ(L81.isEmpty(), true);
    EXPECT_EQ(L80.isEmpty(), true);
    EXPECT_EQ(L79.isFull(), true);
    EXPECT_EQ(L78.isFull(), true);

    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L77.backIt());
    tmpList2.push_back(L76.backIt());
    tmpList2.push_back(L79.backIt());
    tmpList2.push_back(L78.backIt());
    EXPECT_EQ(checkUnorderedList(Q23.mFullChildren,tmpList2),true);

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

    EXPECT_EQ(checkOrientation(Q23,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);


     EXPECT_EQ(L77.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L77.backIt(), Q23.backIt()), true);
     EXPECT_EQ(L76.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L76.backIt(), Q23.backIt()), true);
     EXPECT_EQ(L75.mFullChildrenSet, false);
     EXPECT_EQ(L74.mFullChildrenSet, false);
     EXPECT_EQ(L70.mFullChildrenSet, false);
     EXPECT_EQ(L81.mFullChildrenSet, false);
     EXPECT_EQ(L82.mFullChildrenSet, false);
     EXPECT_EQ(L79.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L79.backIt(), Q23.backIt()), true);
     EXPECT_EQ(L78.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L78.backIt(), Q23.backIt()), true);
     EXPECT_EQ(Q23.mFullChildren.size(), 4);
     EXPECT_EQ(Q23.mFullChildrenSet, false);
     EXPECT_EQ(L71.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L71.backIt(), P26.backIt()), true);
     EXPECT_EQ(L73.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L73.backIt(), P26.backIt()), true);
     EXPECT_EQ(P26.mFullChildren.size(), 2);


}


/**  @brief tests BLPlanarizer::templateP7()
 */
TEST_F(BLPlanarityTesterTests, Test30){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P7 positive #3
    //
    //     __Q22___                          ___Q22___
    //    |   |    |                        |    |    |
    //  *L82  |  *L83                     *L82   |  *L83
    //      _P26______________       =>         Q23
    //     /  |               \                  | 
    //   L70  |                |                 |
    //     __Q23___       ____Q24___       ______|________________________
    //    /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |   |
    // *L74*L75 L76 L77*L78*L79 L80 L81 *L74*L75 L76 L77 L70 L81 L80*L79*L78
    //
    //

    pqTree.initializeForOneIteration();

    auto& L74 = createL(pqTree, G);
    L74.mGeneration = pqTree.mGeneration;
    L74.mPertinentType = BLTreeNode::Full;
    L74.mPertinentLeavesCount = 1;
    auto& L75 = createL(pqTree, G);
    L75.mGeneration = pqTree.mGeneration;
    L75.mPertinentType = BLTreeNode::Full;
    L75.mPertinentLeavesCount = 1;
    auto& L76 = createL(pqTree, G);
    auto& L77 = createL(pqTree, G);

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = BLTreeNode::SinglyPartial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mPertinentLeavesCount = 2;

    createFullLink(L74.backIt(), Q23.backIt());
    createFullLink(L75.backIt(), Q23.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q23.mOrientInNorm.push_back(track_01.backIt());
    Q23.mOrientInReversed.push_back(track_02.backIt());
    Q23.mOrientOutNorm.push_back(track_03.backIt());
    Q23.mOrientOutReversed.push_back(track_04.backIt());

    auto& L78 = createL(pqTree, G);
    L78.mGeneration = pqTree.mGeneration;
    L78.mPertinentType = BLTreeNode::Full;
    L78.mPertinentLeavesCount = 1;
    auto& L79 = createL(pqTree, G);
    L79.mGeneration = pqTree.mGeneration;
    L79.mPertinentType = BLTreeNode::Full;
    L79.mPertinentLeavesCount = 1;
    auto& L80 = createL(pqTree, G);
    auto& L81 = createL(pqTree, G);

    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = BLTreeNode::SinglyPartial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mPertinentLeavesCount = 2;

    createFullLink(L78.backIt(), Q24.backIt());
    createFullLink(L79.backIt(), Q24.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q24.mOrientInNorm.push_back(track_05.backIt());
    Q24.mOrientInReversed.push_back(track_06.backIt());
    Q24.mOrientOutNorm.push_back(track_07.backIt());
    Q24.mOrientOutReversed.push_back(track_08.backIt());

    auto& L70 = createL(pqTree, G);

    list<node_list_it_t> P26children;
    P26children.push_back(L70.backIt());
    P26children.push_back(Q23.backIt());
    P26children.push_back(Q24.backIt());

    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 2;
    P26.mPertinentLeavesCount = 4;
    P26.mSinglyPartialChild1 = Q23.backIt();
    P26.mSinglyPartialChild2 = Q24.backIt();


    auto& L82 = createL(pqTree, G);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = BLTreeNode::Full;
    L82.mPertinentLeavesCount = 1;

    auto& L83 = createL(pqTree, G);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = BLTreeNode::Full;
    L83.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(P26.backIt());
    Q22children.push_back(L83.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    P26.mParent = Q22.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP7(P26, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    // Check Q22
    EXPECT_EQ(Q22.mCDPartialChild, Q23.backIt());
    EXPECT_EQ(Q22.mEndChild1, L82.backIt());
    EXPECT_EQ(Q22.mEndChild2, L83.backIt());
    EXPECT_EQ(areImmediateSiblings(L82, Q23), true);
    EXPECT_EQ(areImmediateSiblings(L83, Q23), true);

    // Check Q23
    EXPECT_EQ(Q23.mParent,              Q22.backIt());
    EXPECT_EQ(Q23.mNodeType,            BLTreeNode::QType);
    EXPECT_EQ(Q23.mPertinentType,       BLTreeNode::CDPartial);
    EXPECT_EQ(Q23.mCDPartialChild,      pqTree.nil());
    EXPECT_EQ(Q23.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L74.backIt());
    tmpList.push_back(L75.backIt());
    tmpList.push_back(L79.backIt());
    tmpList.push_back(L78.backIt());
    EXPECT_EQ(checkUnorderedList(Q23.mFullChildren,tmpList),true);

    EXPECT_EQ(Q23.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q23.mPertinentLeavesCount, 4);
    EXPECT_EQ(Q23.mChildren.size(),0);

    auto prevIt = pqTree.nil();
    auto curIt  = (Q23.mEndChild1==L74.backIt())?Q23.mEndChild1:Q23.mEndChild2;
    EXPECT_EQ(curIt, L74.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L75.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L76.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L77.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L70.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L81.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L80.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L79.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L78.backIt());

    EXPECT_EQ(L74.isFull(), true);
    EXPECT_EQ(L75.isFull(), true);
    EXPECT_EQ(L76.isEmpty(), true);
    EXPECT_EQ(L77.isEmpty(), true);
    EXPECT_EQ(L70.isEmpty(), true);
    EXPECT_EQ(L81.isEmpty(), true);
    EXPECT_EQ(L80.isEmpty(), true);
    EXPECT_EQ(L79.isFull(), true);
    EXPECT_EQ(L78.isFull(), true);

    list<node_list_it_t> expectedOrientInNorm;
    list<node_list_it_t> expectedOrientInReversed;
    list<node_list_it_t> expectedOrientOutNorm;
    list<node_list_it_t> expectedOrientOutReversed;

    expectedOrientInNorm.push_back(track_01.backIt());
    expectedOrientInNorm.push_back(track_06.backIt());
    expectedOrientInReversed.push_back(track_02.backIt());
    expectedOrientInReversed.push_back(track_05.backIt());
    expectedOrientOutNorm.push_back(track_03.backIt());
    expectedOrientOutNorm.push_back(track_08.backIt());
    expectedOrientOutReversed.push_back(track_04.backIt());
    expectedOrientOutReversed.push_back(track_07.backIt());

    EXPECT_EQ(checkOrientation(Q23,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

     EXPECT_EQ(L74.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L74.backIt(), Q23.backIt()), true);
     EXPECT_EQ(L75.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L75.backIt(), Q23.backIt()), true);
     EXPECT_EQ(L76.mFullChildrenSet, false);
     EXPECT_EQ(L77.mFullChildrenSet, false);
     EXPECT_EQ(L70.mFullChildrenSet, false);
     EXPECT_EQ(L81.mFullChildrenSet, false);
     EXPECT_EQ(L80.mFullChildrenSet, false);
     EXPECT_EQ(L79.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L79.backIt(), Q23.backIt()), true);
     EXPECT_EQ(L78.mFullChildrenSet, true);
     EXPECT_EQ(isInFullChildren(L78.backIt(), Q23.backIt()), true);
     EXPECT_EQ(Q23.mFullChildren.size(), 4);
     EXPECT_EQ(Q23.mFullChildrenSet, false);

}


/**  @brief tests BLPlanarizer::templateP7()
 */
TEST_F(BLPlanarityTesterTests, Test31){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P7 positive #4
    //
    //     __Q22___                          ___Q22___
    //    |   |    |                        |    |    |
    //  *L82  |  *L83                     *L82   |  *L83
    //       P26______________       =>         Q23
    //        |               \                  | 
    //        |                |                 |
    //     __Q23___       ____Q24___       ______|____________________
    //    /  /  \  \     /   /   \  \     |   |   |   |   |   |   |   |
    // *L74*L75 L76 L77 L78 L79*L80*L81 *L74*L75 L76 L77 L78 L79*L80*L81
    //
    //

    pqTree.initializeForOneIteration();

    auto& L74 = createL(pqTree, G);
    L74.mGeneration = pqTree.mGeneration;
    L74.mPertinentType = BLTreeNode::Full;
    L74.mPertinentLeavesCount = 1;
    auto& L75 = createL(pqTree, G);
    L75.mGeneration = pqTree.mGeneration;
    L75.mPertinentType = BLTreeNode::Full;
    L75.mPertinentLeavesCount = 1;
    auto& L76 = createL(pqTree, G);
    auto& L77 = createL(pqTree, G);

    list<node_list_it_t> Q23children;
    Q23children.push_back(L74.backIt());
    Q23children.push_back(L75.backIt());
    Q23children.push_back(L76.backIt());
    Q23children.push_back(L77.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = BLTreeNode::SinglyPartial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mPertinentLeavesCount = 2;

    createFullLink(L74.backIt(), Q23.backIt());
    createFullLink(L75.backIt(), Q23.backIt());

    auto &track_01 = createBLGraphNode(G);
    auto &track_02 = createBLGraphNode(G);
    auto &track_03 = createBLGraphNode(G);
    auto &track_04 = createBLGraphNode(G);
    Q23.mOrientInNorm.push_back(track_01.backIt());
    Q23.mOrientInReversed.push_back(track_02.backIt());
    Q23.mOrientOutNorm.push_back(track_03.backIt());
    Q23.mOrientOutReversed.push_back(track_04.backIt());

    auto& L78 = createL(pqTree, G);
    auto& L79 = createL(pqTree, G);
    auto& L80 = createL(pqTree, G);
    L80.mGeneration = pqTree.mGeneration;
    L80.mPertinentType = BLTreeNode::Full;
    L80.mPertinentLeavesCount = 1;
    auto& L81 = createL(pqTree, G);
    L81.mGeneration = pqTree.mGeneration;
    L81.mPertinentType = BLTreeNode::Full;
    L81.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q24children;
    Q24children.push_back(L78.backIt());
    Q24children.push_back(L79.backIt());
    Q24children.push_back(L80.backIt());
    Q24children.push_back(L81.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = BLTreeNode::SinglyPartial;
    Q24.mPertinentChildrenCount = 2;
    Q24.mPertinentLeavesCount = 2;

    createFullLink(L80.backIt(), Q24.backIt());
    createFullLink(L81.backIt(), Q24.backIt());

    auto &track_05 = createBLGraphNode(G);
    auto &track_06 = createBLGraphNode(G);
    auto &track_07 = createBLGraphNode(G);
    auto &track_08 = createBLGraphNode(G);
    Q24.mOrientInNorm.push_back(track_05.backIt());
    Q24.mOrientInReversed.push_back(track_06.backIt());
    Q24.mOrientOutNorm.push_back(track_07.backIt());
    Q24.mOrientOutReversed.push_back(track_08.backIt());

    list<node_list_it_t> P26children;
    P26children.push_back(Q23.backIt());
    P26children.push_back(Q24.backIt());

    auto& P26 = createP(pqTree, P26children);
    P26.mGeneration = pqTree.mGeneration;
    P26.mPertinentChildrenCount = 2;
    P26.mPertinentLeavesCount = 4;
    P26.mSinglyPartialChild1 = Q23.backIt();
    P26.mSinglyPartialChild2 = Q24.backIt();

    auto& L82 = createL(pqTree, G);
    L82.mGeneration = pqTree.mGeneration;
    L82.mPertinentType = BLTreeNode::Full;
    L82.mPertinentLeavesCount = 1;

    auto& L83 = createL(pqTree, G);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = BLTreeNode::Full;
    L83.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q22children;
    Q22children.push_back(L82.backIt());
    Q22children.push_back(P26.backIt());
    Q22children.push_back(L83.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    P26.mParent = Q22.backIt();

    bool earlyOut;
    EXPECT_EQ(pqTree.templateP7(P26, earlyOut), true);
    EXPECT_EQ(earlyOut, false);

    // Check Q22
    EXPECT_EQ(Q22.mCDPartialChild, Q23.backIt());
    EXPECT_EQ(Q22.mEndChild1, L82.backIt());
    EXPECT_EQ(Q22.mEndChild2, L83.backIt());
    EXPECT_EQ(areImmediateSiblings(L82, Q23), true);
    EXPECT_EQ(areImmediateSiblings(L83, Q23), true);

    // Check Q23
    EXPECT_EQ(Q23.mParent,              Q22.backIt());
    EXPECT_EQ(Q23.mNodeType,            BLTreeNode::QType);
    EXPECT_EQ(Q23.mPertinentType,       BLTreeNode::CDPartial);
    EXPECT_EQ(Q23.mCDPartialChild,      pqTree.nil());
    EXPECT_EQ(Q23.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mSinglyPartialChild2, pqTree.nil());

    list<node_list_it_t> tmpList;
    tmpList.push_back(L74.backIt());
    tmpList.push_back(L75.backIt());
    tmpList.push_back(L80.backIt());
    tmpList.push_back(L81.backIt());
    EXPECT_EQ(checkUnorderedList(Q23.mFullChildren,tmpList),true);


    EXPECT_EQ(Q23.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q23.mPertinentLeavesCount, 4);
    EXPECT_EQ(Q23.mChildren.size(),0);

    auto prevIt = pqTree.nil();
    auto curIt  = (Q23.mEndChild1==L74.backIt())?Q23.mEndChild1:Q23.mEndChild2;
    EXPECT_EQ(curIt, L74.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L75.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L76.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L77.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L78.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L79.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L80.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L81.backIt());

    EXPECT_EQ(L74.isFull(), true);
    EXPECT_EQ(L75.isFull(), true);
    EXPECT_EQ(L76.isEmpty(), true);
    EXPECT_EQ(L77.isEmpty(), true);
    EXPECT_EQ(L78.isEmpty(), true);
    EXPECT_EQ(L79.isEmpty(), true);
    EXPECT_EQ(L80.isFull(), true);
    EXPECT_EQ(L81.isFull(), true);


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

    EXPECT_EQ(checkOrientation(Q23,
        expectedOrientInNorm, expectedOrientInReversed,
        expectedOrientOutNorm,  expectedOrientOutReversed), true);

    EXPECT_EQ(L74.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L74.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L75.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L75.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L76.mFullChildrenSet, false);
    EXPECT_EQ(L77.mFullChildrenSet, false);
    EXPECT_EQ(L78.mFullChildrenSet, false);
    EXPECT_EQ(L79.mFullChildrenSet, false);
    EXPECT_EQ(L80.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L80.backIt(), Q23.backIt()), true);
    EXPECT_EQ(L81.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L81.backIt(), Q23.backIt()), true);
    EXPECT_EQ(Q23.mFullChildren.size(), 4);
    EXPECT_EQ(Q23.mFullChildrenSet, false);




}


/**  @brief tests BLPlanarizer::templateP8()
 */
TEST_F(BLPlanarityTesterTests, Test32){

    BLTree pqTree;
    BLGraph G;

    pqTree.trackQFlippings();

    EXPECT_EQ(pqTree.isTrackingQFlippings(),true);

    //   P8 positive #1
    //
    //    ___Q07___
    //   |    |    |
    // *L10  P06 *L11
    //      / | \
    //   *L08 |*L09
    //     __Q05___
    //    /  /  \  \
    // *L01 L02 L03*L04
    //
    //

    pqTree.initializeForOneIteration();

    auto& L01 = createL(pqTree, G);
    L01.mGeneration = pqTree.mGeneration;
    L01.mPertinentType = BLTreeNode::Full;
    L01.mPertinentLeavesCount = 1;
    auto& L02 = createL(pqTree, G);
    auto& L03 = createL(pqTree, G);
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;
    L04.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q05children;
    Q05children.push_back(L01.backIt());
    Q05children.push_back(L02.backIt());
    Q05children.push_back(L03.backIt());
    Q05children.push_back(L04.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = BLTreeNode::CDPartial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mPertinentLeavesCount = 2;

    createFullLink(L01.backIt(), Q05.backIt());
    createFullLink(L04.backIt(), Q05.backIt());

    auto& L08 = createL(pqTree, G);
    L08.mGeneration = pqTree.mGeneration;
    L08.mPertinentType = BLTreeNode::Full;
    L08.mPertinentLeavesCount = 1;
    auto& L09 = createL(pqTree, G);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = BLTreeNode::Full;
    L09.mPertinentLeavesCount = 1;

    list<node_list_it_t> P06children;
    P06children.push_back(L08.backIt());
    P06children.push_back(Q05.backIt());
    P06children.push_back(L09.backIt());
    auto& P06 = createP(pqTree, P06children);
    P06.mGeneration = pqTree.mGeneration;
    P06.mPertinentChildrenCount = 3;
    P06.mPertinentLeavesCount = 4;

    createFullLink(L08.backIt(), P06.backIt());
    createFullLink(L09.backIt(), P06.backIt());
    P06.mCDPartialChild = Q05.backIt();

    auto& L10 = createL(pqTree, G);
    L10.mGeneration = pqTree.mGeneration;
    L10.mPertinentType = BLTreeNode::Full;
    L10.mPertinentLeavesCount = 1;
    auto& L11 = createL(pqTree, G);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = BLTreeNode::Full;
    L11.mPertinentLeavesCount = 1;

    list<node_list_it_t> Q07children;
    Q07children.push_back(L10.backIt());
    Q07children.push_back(P06.backIt());
    Q07children.push_back(L11.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;

    P06.mParent = Q07.backIt();

    bool earlyOut;

    EXPECT_EQ(pqTree.templateP8(P06, BLTree::NOT_FINAL_REDUCTION, earlyOut),
                                                                        true);
    EXPECT_EQ(earlyOut, false);

    // Check Q07
    EXPECT_EQ(Q07.mCDPartialChild, P06.backIt());

    // Check P06
    EXPECT_EQ(P06.mPertinentType, BLTreeNode::CDPartial);
    EXPECT_EQ(P06.mCDPartialChild, Q05.backIt());

    EXPECT_EQ(L01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L01.backIt(), Q05.backIt()), true);
    EXPECT_EQ(L02.mFullChildrenSet, false);
    EXPECT_EQ(L03.mFullChildrenSet, false);
    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q05.backIt()), true);
    EXPECT_EQ(Q05.mFullChildren.size(), 2);
    EXPECT_EQ(L08.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L08.backIt(), P06.backIt()), true);
    EXPECT_EQ(L09.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L09.backIt(), P06.backIt()), true);
    EXPECT_EQ(P06.mFullChildren.size(), 2);

}


