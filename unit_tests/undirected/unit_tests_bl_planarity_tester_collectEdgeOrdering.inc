
/**  @brief tests BLPlanarityTester::applyTemplates()
 */
TEST_F(BLPlanarityTesterTests, Test83) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

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

    pqTree.initializeForOneIteration();
    auto& L1_5 = createL(pqTree, G);
    auto& L2_4 = createL(pqTree, G);
    L2_4.mGeneration    = pqTree.mGeneration;
    auto& L2_5 = createL(pqTree, G);
    auto& L3_4 = createL(pqTree, G);
    L3_4.mGeneration    = pqTree.mGeneration;
    auto& L3_5 = createL(pqTree, G);

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

    vector<node_list_it_t> pertinentLeaves;
    pertinentLeaves.push_back(L2_4.backIt());
    pertinentLeaves.push_back(L3_4.backIt());

    auto pertinentRoot = Q01.backIt();

    EXPECT_EQ(
        applyTemplates(
               tester, pqTree, pertinentLeaves, pertinentRoot), true);
    EXPECT_EQ(pertinentRoot, Q01.backIt());
    EXPECT_EQ(Q01.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q01.mPertinentChildrenCount, 2);

    auto prevIt  = pqTree.nil();
    auto curIt   = Q01.mEndChild1;
    auto otherIt = Q01.mEndChild2;
    auto& Q01_C01 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q01_C02 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q01_C03 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q01_C04 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(curIt, otherIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, pqTree.nil());

    EXPECT_EQ(&Q01_C01, &L2_5);
    EXPECT_EQ(L2_5.isEmpty(), true);
    EXPECT_EQ(&Q01_C02, &L2_4);
    EXPECT_EQ(L2_4.isFull(), true);
    EXPECT_EQ(&Q01_C03, &L3_4);
    EXPECT_EQ(L3_4.isFull(), true);
    EXPECT_EQ(&Q01_C04, &L3_5);
    EXPECT_EQ(L3_5.isEmpty(), true);


    EXPECT_EQ(L2_4.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L2_4.backIt(), Q01.backIt()), true);
    EXPECT_EQ(L3_4.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L3_4.backIt(), Q01.backIt()), true);
    EXPECT_EQ(Q01.mFullChildren.size(), 2);
    EXPECT_EQ(P01.mFullChildren.size(), 0);

}


/**  @brief tests BLPlanarityTester::applyTemplates()
 */
TEST_F(BLPlanarityTesterTests, Test84) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    //
    //              _P01_
    //             /     \
    //        ___Q01___ *L1_5
    //       |    |    |
    //    *L2_5*L4_5*L3_5
    //
    //             _||_
    //             \  /
    //              \/
    //
    //              _P01_
    //             /     \
    //        ___Q01___ *L1_5
    //       |    |    |
    //    *L2_5*L4_5*L3_5
    //

    pqTree.initializeForOneIteration();
    auto& L2_5 = createL(pqTree, G);
    L2_5.mGeneration    = pqTree.mGeneration;
    auto& L4_5 = createL(pqTree, G);
    L4_5.mGeneration    = pqTree.mGeneration;
    auto& L3_5 = createL(pqTree, G);
    L3_5.mGeneration    = pqTree.mGeneration;
    auto& L1_5 = createL(pqTree, G);
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

    L4_5.mParent = Q01.backIt();

    auto pertinentRoot = P01.backIt();
    EXPECT_EQ(
        applyTemplates(
                 tester, pqTree, pertinentLeaves, pertinentRoot), true);
    EXPECT_EQ(pertinentRoot, P01.backIt());
    EXPECT_EQ(P01.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P01.mPertinentChildrenCount, 2);
    EXPECT_EQ(P01.mChildren.size(), 2);
    EXPECT_EQ(P01.isFull(),true);

    list<node_list_it_t> tmpList;
    tmpList.push_back(L1_5.backIt());
    tmpList.push_back(Q01.backIt());
    EXPECT_EQ(checkUnorderedList(P01.mFullChildren,tmpList),true);

    auto cIt = P01.mChildren.begin();
    EXPECT_EQ(Q01.backIt(), *cIt);
    EXPECT_EQ(Q01.mChildIt, cIt);
    EXPECT_EQ(Q01.mParent, P01.backIt());
    EXPECT_EQ(Q01.mPertinentChildrenCount, 3);


    list<node_list_it_t> tmpList2;
    tmpList2.push_back(L2_5.backIt());
    tmpList2.push_back(L4_5.backIt());
    tmpList2.push_back(L3_5.backIt());
    EXPECT_EQ(checkUnorderedList(Q01.mFullChildren,tmpList2),true);

    EXPECT_EQ(Q01.isFull(), true);
    cIt++;
    EXPECT_EQ(L1_5.backIt(), *cIt);
    EXPECT_EQ(L1_5.mChildIt, cIt);
    EXPECT_EQ(L1_5.mParent, P01.backIt());
    EXPECT_EQ(L1_5.isFull(), true);

    auto prevIt  = pqTree.nil();
    auto curIt   = Q01.mEndChild1;
    auto otherIt = Q01.mEndChild2;

    auto& Q01_C01 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q01_C02 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto& Q01_C03 = pqTree.toNodeRef(curIt);
    EXPECT_EQ(curIt, otherIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, pqTree.nil());

    EXPECT_EQ(&Q01_C01, &L2_5);
    EXPECT_EQ(L2_5.isFull(), true);
    EXPECT_EQ(&Q01_C02, &L4_5);
    EXPECT_EQ(L4_5.isFull(), true);
    EXPECT_EQ(&Q01_C03, &L3_5);
    EXPECT_EQ(L3_5.isFull(), true);


    EXPECT_EQ(L2_5.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L2_5.backIt(), Q01.backIt()), true);
    EXPECT_EQ(L4_5.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L4_5.backIt(), Q01.backIt()), true);
    EXPECT_EQ(L3_5.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L3_5.backIt(), Q01.backIt()), true);
    EXPECT_EQ(Q01.mFullChildren.size(), 3);
    EXPECT_EQ(Q01.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(Q01.backIt(), P01.backIt()), true);
    EXPECT_EQ(L1_5.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L1_5.backIt(), P01.backIt()), true);
    EXPECT_EQ(P01.mFullChildren.size(), 2);

}


/**  @brief tests BLPlanarityTester::applyTemplates()
 */
TEST_F(BLPlanarityTesterTests, Test85) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;


    //
    //          P01
    //     ______|____
    //    /           \
    // *L02          Q03
    //          ______|____________________________
    //         |           |    |                  |
    //       *L04         P05  L06                P07
    //                  ___|___               _____|______
    //                 /       \             /     |      \
    //                L08     P09           P10    L11    L12
    //                       __|___      ____|_____
    //                      /      \    /    |     \
    //                    L13     Q14 *L15   L16   L17
    //                     ________|______
    //                    |               |
    //                   P18             L19
    //                 ___|___
    //                /       \
    //              L20      L21
    //

    pqTree.initializeForOneIteration();


    auto& L20 = createL(pqTree, G);
    auto& L21 = createL(pqTree, G);

    list<node_list_it_t> P18children;
    P18children.push_back(L20.backIt());
    P18children.push_back(L21.backIt());
    auto& P18 = createP(pqTree, P18children);

    auto& L19 = createL(pqTree, G);

    list<node_list_it_t> Q14children;
    Q14children.push_back(P18.backIt());
    Q14children.push_back(L19.backIt());
    auto& Q14 = createQ(pqTree, Q14children);

    auto& L13 = createL(pqTree, G);

    list<node_list_it_t> P09children;
    P09children.push_back(L13.backIt());
    P09children.push_back(Q14.backIt());
    auto& P09 = createP(pqTree, P09children);

    auto& L08 = createL(pqTree, G);

    list<node_list_it_t> P05children;
    P05children.push_back(L08.backIt());
    P05children.push_back(P09.backIt());
    auto& P05 = createP(pqTree, P05children);

    auto& L15 = createL(pqTree, G);
    L15.mGeneration           = pqTree.mGeneration;

    auto& L16 = createL(pqTree, G);
    auto& L17 = createL(pqTree, G);

    list<node_list_it_t> P10children;
    P10children.push_back(L15.backIt());
    P10children.push_back(L16.backIt());
    P10children.push_back(L17.backIt());
    auto& P10 = createP(pqTree, P10children);
    P10.mGeneration = pqTree.mGeneration;
    P10.mPertinentChildrenCount = 1;

    auto& L11 = createL(pqTree, G);
    auto& L12 = createL(pqTree, G);

    list<node_list_it_t> P07children;
    P07children.push_back(P10.backIt());
    P07children.push_back(L11.backIt());
    P07children.push_back(L12.backIt());
    auto& P07 = createP(pqTree, P07children);
    P07.mGeneration = pqTree.mGeneration;
    P07.mPertinentChildrenCount = 1;

    auto& L04 = createL(pqTree, G);
    L04.mGeneration    = pqTree.mGeneration;

    auto& L06 = createL(pqTree, G);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L04.backIt());
    Q03children.push_back(P05.backIt());
    Q03children.push_back(L06.backIt());
    Q03children.push_back(P07.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentChildrenCount = 2;

    auto& L02 = createL(pqTree, G);
    L02.mGeneration    = pqTree.mGeneration;

    list<node_list_it_t> P01children;
    P01children.push_back(L02.backIt());
    P01children.push_back(Q03.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration = pqTree.mGeneration;
    P01.mPertinentChildrenCount = 2;


    //
    //          P01
    //     ______|____
    //    /           \
    // *L02          Q03
    //          ______|__________________________________
    //         |           |    |         |       |      |
    //       *L04         P05  L06        P       P    *L15
    //                  ___|___         /   \   /   \
    //                 /       \      L11  L12 L16 L17
    //                L08     P09
    //                       __|___
    //                      /      \
    //                    L13     Q14 
    //                     ________|______
    //                    |               |
    //                   P18             L19
    //                 ___|___
    //                /       \
    //              L20      L21
    //

    vector<node_list_it_t> pertinentLeaves;
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L15.backIt());

    auto pertinentRoot = P01.backIt();
    EXPECT_EQ(
        applyTemplates(
                 tester, pqTree, pertinentLeaves, pertinentRoot), true);
    EXPECT_EQ(pertinentRoot, P01.backIt());
    EXPECT_EQ(pqTree.CDPartialRoot(), Q03.backIt());

    EXPECT_EQ(P01.mNodeType, BLTreeNode::PType);
    EXPECT_EQ(P01.mPertinentType, BLTreeNode::CDPartial);
    EXPECT_EQ(P01.mPertinentChildrenCount, 2);
    EXPECT_EQ(P01.mPertinentLeavesCount, 3);
    EXPECT_EQ(P01.mFullChildren.size(), 1);
    EXPECT_EQ(*(P01.mFullChildren.begin()), L02.backIt());
    EXPECT_EQ(P01.mCDPartialChild, Q03.backIt());
    EXPECT_EQ(P01.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(P01.mSinglyPartialChild2, pqTree.nil());

    EXPECT_EQ(L02.isFull(),true);
    EXPECT_EQ(L02.mPertinentLeavesCount,1);

    EXPECT_EQ(Q03.mNodeType, BLTreeNode::QType);
    EXPECT_EQ(Q03.mPertinentType, BLTreeNode::CDPartial);
    EXPECT_EQ(Q03.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q03.mPertinentLeavesCount, 2);
    EXPECT_EQ(Q03.mFullChildren.size(), 2);
    EXPECT_EQ((*(Q03.mFullChildren.begin())==L04.backIt()&&
               *(Q03.mFullChildren.rbegin())==L15.backIt()  )||
              (*(Q03.mFullChildren.rbegin())==L04.backIt()&&
               *(Q03.mFullChildren.begin())==L15.backIt()  ) ,true);

    EXPECT_EQ(Q03.mCDPartialChild, pqTree.nil());
    EXPECT_EQ(Q03.mSinglyPartialChild1, pqTree.nil());
    EXPECT_EQ(Q03.mSinglyPartialChild2, pqTree.nil());

    auto prevIt = pqTree.nil();
    auto curIt  = Q03.mEndChild1;
    EXPECT_EQ(curIt, L04.backIt());
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, P05.backIt());    
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L06.backIt());    
    pqTree.advanceSib(prevIt, curIt);
    auto &Pnew01 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    auto &Pnew02 = pqTree.toNodeRef(curIt);
    pqTree.advanceSib(prevIt, curIt);
    EXPECT_EQ(curIt, L15.backIt());

    auto cIt = Pnew01.mChildren.begin();
    EXPECT_EQ(*cIt, L11.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L12.backIt());

    cIt = Pnew02.mChildren.begin();
    EXPECT_EQ(*cIt, L16.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L17.backIt());


    EXPECT_EQ(L04.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L04.backIt(), Q03.backIt()), true);
    EXPECT_EQ(L15.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L15.backIt(), Q03.backIt()), true);
    EXPECT_EQ(Q03.mFullChildren.size(), 2);
    EXPECT_EQ(Q03.mFullChildrenSet, false);
    EXPECT_EQ(L02.mFullChildrenSet, true);
    EXPECT_EQ(isInFullChildren(L02.backIt(), P01.backIt()), true);
    EXPECT_EQ(P01.mFullChildren.size(), 1);

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): L
 */
TEST_F(BLPlanarityTesterTests, Test86) {

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
    auto& GN01 = createBLGraphNode(G);
    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);
    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 1);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L02.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): P
 */
TEST_F(BLPlanarityTesterTests, Test87) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;


    //      *P01
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

    auto pertinentRoot = P01.backIt();
    auto& GN01 = createBLGraphNode(G);
    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);
    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 3);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L01.backIt());
    eIt++;
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L02.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L03.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): P
 */
TEST_F(BLPlanarityTesterTests, Test88) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;


    //      *P01
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

    auto pertinentRoot = P01.backIt();
    auto& GN01 = createBLGraphNode(G);
    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 9);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L02.backIt());
    eIt++;
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L05.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L06.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L11.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L12.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L13.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L09.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L10.backIt());
    eIt++;
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L04.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): Q
 */
TEST_F(BLPlanarityTesterTests, Test89) {

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

    auto pertinentRoot = Q01.backIt();
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 13);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L02.backIt());
    eIt++;
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L05.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L06.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L11.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L12.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L13.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L09.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L10.backIt());
    eIt++;
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L04.backIt());
    eIt++;
    auto& E10 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E10.mPQLeaf, L15.backIt());
    eIt++;
    auto& E11 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E11.mPQLeaf, L16.backIt());
    eIt++;
    auto& E12 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E12.mPQLeaf, L17.backIt());
    eIt++;
    auto& E13 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E13.mPQLeaf, L18.backIt());

    EXPECT_EQ(Q01.mOrientInNorm.size(), 1);
    auto nIt = Q01.mOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): Q
 */
TEST_F(BLPlanarityTesterTests, Test90) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *Q01
    //    ____|_______________________
    //   |    |    |           |      |
    // *L02 *P03  L04       __Q14__  L18
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

    //      *Q01
    //    ____|_______________________
    //   |    |    |           |      |
    // *L02 *P03  L04       __Q14__  L18
    //      / | \          |   |   |
    //   *L05*L06*Q07     L15 L16 L17
    //            /|\
    //       *P08*L09*L10 
    //       / | \
    //   *L11*L12*L13

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

    auto pertinentRoot = Q01.backIt();
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 8);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L02.backIt());
    eIt++;
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L05.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L06.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L11.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L12.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L13.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L09.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L10.backIt());

    EXPECT_EQ(Q01.mOrientInNorm.size(), 1);
    auto nIt = Q01.mOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): Q
 */
TEST_F(BLPlanarityTesterTests, Test91) {

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
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 5);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L04.backIt());
    eIt++;
    auto& E10 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E10.mPQLeaf, L15.backIt());
    eIt++;
    auto& E11 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E11.mPQLeaf, L16.backIt());
    eIt++;
    auto& E12 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E12.mPQLeaf, L17.backIt());
    eIt++;
    auto& E13 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E13.mPQLeaf, L18.backIt());

    EXPECT_EQ(Q01.mOrientInNorm.size(), 1);
    auto nIt = Q01.mOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): VR
 */
TEST_F(BLPlanarityTesterTests, Test92) {

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
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 11);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L05.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L06.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L11.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L12.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L13.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L09.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L10.backIt());
    eIt++;
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L04.backIt());
    eIt++;
    auto& E10 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E10.mPQLeaf, L15.backIt());
    eIt++;
    auto& E11 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E11.mPQLeaf, L16.backIt());
    eIt++;
    auto& E12 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E12.mPQLeaf, L17.backIt());

    EXPECT_EQ(L18.mAssumedOrientInNorm.size(), 1);
    auto nIt = L18.mAssumedOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): VR
 */
TEST_F(BLPlanarityTesterTests, Test93) {

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

    auto& VR = createVR(pqTree);

    VR.mGeneration = pqTree.mGeneration;
    VR.mPertinentType = BLTreeNode::Full;
    VR.mPertinentChildrenCount = 2;


    createFullLink(P03.backIt(), VR.backIt());
    createFullLink(L04.backIt(), VR.backIt());

    auto pertinentRoot = VR.backIt();
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 8);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L05.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L06.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L11.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L12.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L13.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L09.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L10.backIt());
    eIt++;
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L04.backIt());

    EXPECT_EQ(Q14.mAssumedOrientInReversed.size(), 1);
    auto nIt = Q14.mAssumedOrientInReversed.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): CDPartial
 */
TEST_F(BLPlanarityTesterTests, Test94) {

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
    createFullLink(L09.backIt(), Q07.backIt());
    createFullLink(L10.backIt(), Q07.backIt());

    pqTree.mCDPartialRoot = Q07.backIt();

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

    auto pertinentRoot = Q01.backIt();
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 12);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L10.backIt());
    eIt++;
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L04.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L15.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L16.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L17.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L18.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L02.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L05.backIt());
    eIt++;
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L06.backIt());
    eIt++;
    auto& E10 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E10.mPQLeaf, L11.backIt());
    eIt++;
    auto& E11 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E11.mPQLeaf, L12.backIt());
    eIt++;
    auto& E12 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E12.mPQLeaf, L13.backIt());

    EXPECT_EQ(Q07.mOrientInNorm.size(), 1);
    auto nIt = Q07.mOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): CDPartial
 */
TEST_F(BLPlanarityTesterTests, Test95) {

    BLTree  pqTree;
    BLGraph G;
    BLPlanarityTester tester;

    pqTree.initializeForOneIteration();
    pqTree.trackQFlippings();

    //      *P01
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

    P03.mCDPartialChild = Q07.backIt();

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
    Q14.mPertinentChildrenCount = 3;

    createFullLink(L15.backIt(), Q14.backIt());
    createFullLink(L17.backIt(), Q14.backIt());

    pqTree.mCDPartialRoot = Q14.backIt();

    auto& L02 = createL(pqTree, G);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = BLTreeNode::Full;
    auto& L04 = createL(pqTree, G);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = BLTreeNode::Full;

    auto& L18 = createL(pqTree, G);
    L18.mGeneration = pqTree.mGeneration;
    L18.mPertinentType = BLTreeNode::Full;

    list<node_list_it_t> P01children;
    P01children.push_back(L02.backIt());
    P01children.push_back(P03.backIt());
    P01children.push_back(L04.backIt());
    P01children.push_back(Q14.backIt());
    P01children.push_back(L18.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mGeneration = pqTree.mGeneration;
    P01.mPertinentType = BLTreeNode::CDPartial;
    P01.mPertinentChildrenCount = 5;

    createFullLink(L02.backIt(), P01.backIt());
    createFullLink(P03.backIt(), P01.backIt());
    createFullLink(L04.backIt(), P01.backIt());
    createFullLink(L18.backIt(), P01.backIt());

    P01.mCDPartialChild = Q14.backIt();

    auto pertinentRoot = P01.backIt();
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 12);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L17.backIt());
    eIt++;
    auto& E02 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E02.mPQLeaf, L18.backIt());
    eIt++;
    auto& E03 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E03.mPQLeaf, L02.backIt());
    eIt++;
    auto& E04 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E04.mPQLeaf, L05.backIt());
    eIt++;
    auto& E05 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E05.mPQLeaf, L06.backIt());
    eIt++;
    auto& E06 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E06.mPQLeaf, L11.backIt());
    eIt++;
    auto& E07 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E07.mPQLeaf, L12.backIt());
    eIt++;
    auto& E08 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E08.mPQLeaf, L13.backIt());
    eIt++;
    auto& E09 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E09.mPQLeaf, L09.backIt());
    eIt++;
    auto& E10 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E10.mPQLeaf, L10.backIt());
    eIt++;
    auto& E11 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E11.mPQLeaf, L04.backIt());
    eIt++;
    auto& E12 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E12.mPQLeaf, L15.backIt());

    EXPECT_EQ(Q14.mOrientInNorm.size(), 1);
    auto nIt = Q14.mOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}


/**  @brief tests BLPlanarityTester::collectEdgeOrdering(): CDPartial
 */
TEST_F(BLPlanarityTesterTests, Test96) {

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
    auto& GN01 = createBLGraphNode(G);

    collectEdgeOrdering(
             tester, pqTree, pertinentRoot, GN01, BLPlanarityTester::INCOMING);

    EXPECT_EQ(GN01.mIncomingEdgesOrdered.size(), 2);
    auto eIt = GN01.mIncomingEdgesOrdered.begin();
    auto& E01 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E01.mPQLeaf, L18.backIt());
    eIt++;
    auto& E12 = dynamic_cast<BLGraphEdge&>(*(*(*eIt)));
    EXPECT_EQ(E12.mPQLeaf, L02.backIt());

    EXPECT_EQ(Q01.mOrientInNorm.size(), 1);
    auto nIt = Q01.mOrientInNorm.begin();
    EXPECT_EQ(*nIt, GN01.backIt());

}

