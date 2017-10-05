


/**  @brief tests JTSTree::templateL1()
 */
TEST_F(JTSPlanarizerTests, Test18) {


    JTSTree& pqTree = getPQTree();

    //    L1 positive
    //      P01
    //      |
    //     *L01
    initializeForOneIteration(pqTree);
    auto& L01 = createL(pqTree);
    L01.mGeneration = pqTree.mGeneration;
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    auto& P01 = createP(pqTree, P01children);
    P01.mPertinentChildrenCount = 1;
    EXPECT_EQ(templateL1(pqTree, L01), true);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P01.mFullChildrenCount, 1);

    //    P1 positive
    //      Q01           Q01
    //      |             |
    //      P02     =>   *P02
    //     /|\           /|\
    //   *L*L*L        *L*L*L
    initializeForOneIteration(pqTree);
    auto& L02 = createL(pqTree);
    L02.mGeneration = pqTree.mGeneration;
    L02.mPertinentType = JTSTreeNode::Full;
    auto& L03 = createL(pqTree);
    L03.mGeneration = pqTree.mGeneration;
    L03.mPertinentType = JTSTreeNode::Full;
    auto& L04 = createL(pqTree);
    L04.mGeneration = pqTree.mGeneration;
    L04.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> P02children;
    P02children.push_back(L02.backIt());
    P02children.push_back(L03.backIt());
    P02children.push_back(L04.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration = pqTree.mGeneration;
    P02.mPertinentChildrenCount = 3;
    P02.mFullChildrenCount = 3;
    list<node_list_it_t> Q01children;
    Q01children.push_back(P02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP1(pqTree, P02), true);
    EXPECT_EQ(P02.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(Q01.mFullChildrenCount, 1);

    //    P2 positive
    //      Q02           Q02
    //      |             |
    //      P03     =>   *P03
    //     /|\           / \
    // L05*L06*L07     L05*P04
    //                     / \
    //                  *L06 *L07
    //
    initializeForOneIteration(pqTree);
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    L06.mGeneration = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    L07.mGeneration = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> P03children;
    P03children.push_back(L05.backIt());
    P03children.push_back(L06.backIt());
    P03children.push_back(L07.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    P03.mPertinentChildrenCount = 2;
    P03.mFullChildrenCount = 2;
    list<node_list_it_t> Q02children;
    Q02children.push_back(P03.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentChildrenCount = 1;
    node_list_it_t root;
    EXPECT_EQ(templateP2(pqTree, P03, root), true);
    EXPECT_NE(root, P03.backIt());
    EXPECT_NE(root, Q02.backIt());
    auto& P04 = pqTree.toNodeRef(root);
    EXPECT_EQ(P04.mChildren.size(), 2);
    EXPECT_EQ(P04.mPertinentChildrenCount, 2);
    EXPECT_EQ(P04.mFullChildrenCount, 2);
    EXPECT_EQ(P04.mPertinentType, JTSTreeNode::Full);
    auto& P04_C01 = pqTree.toNodeRef(*P04.mChildren.begin());
    auto& P04_C02 = pqTree.toNodeRef(*P04.mChildren.rbegin());
    EXPECT_EQ(&P04_C01, &L06);
    EXPECT_EQ(&P04_C02, &L07);
    EXPECT_EQ(L06.mParent, P04.backIt());
    EXPECT_EQ(L07.mParent, P04.backIt());
    auto cIt = P04.mChildren.begin();
    EXPECT_EQ(L06.mChildIt, cIt++);
    EXPECT_EQ(L07.mChildIt, cIt++);

    //   P3 positive #1
    //
    //      Q03             Q03
    //       |               |
    //      P05       =>   Q(P05)
    //     /| |\          /     \
    //L08*L09 L10*L11   P06    *P07
    //                  / \     / \
    //                L08 L10 *L09  *L11
    initializeForOneIteration(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    L09.mGeneration = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> P05children;
    P05children.push_back(L08.backIt());
    P05children.push_back(L09.backIt());
    P05children.push_back(L10.backIt());
    P05children.push_back(L11.backIt());
    auto& P05 = createP(pqTree, P05children);
    P05.mGeneration = pqTree.mGeneration;
    P05.mPertinentChildrenCount = 2;
    P05.mFullChildrenCount = 2;
    list<node_list_it_t> Q03children;
    Q03children.push_back(P05.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentChildrenCount = 1;
    EXPECT_EQ(templateP3(pqTree, P05), true);
    EXPECT_EQ(P05.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P05.mChildren.size(), 2);
    EXPECT_EQ(P05.mPertinentChildrenCount, 1);
    EXPECT_EQ(P05.mFullChildrenCount, 1);
    EXPECT_EQ(P05.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mPartialChild1, P05.backIt());
    cIt = P05.mChildren.begin();
    auto& P05_C01 = pqTree.toNodeRef(*cIt);
    cIt++;
    auto& P05_C02 = pqTree.toNodeRef(*cIt);
    node_list_it_t P06It;
    node_list_it_t P07It;
    if (P05_C01.mPertinentType == JTSTreeNode::Full) {    
        P06It = P05_C02.backIt();
        P07It = P05_C01.backIt();
    }    
    else {
        P06It = P05_C01.backIt();
        P07It = P05_C02.backIt();
    }    
    auto& P06 = pqTree.toNodeRef(P06It);
    auto& P07 = pqTree.toNodeRef(P07It);
    EXPECT_EQ(P06.mPertinentType, JTSTreeNode::Empty);
    EXPECT_EQ(P06.mChildren.size(), 2);
    EXPECT_EQ(P06.mPertinentChildrenCount, 0);
    EXPECT_EQ(P06.mFullChildrenCount, 0);
    cIt = P06.mChildren.begin();
    auto& P06_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P06_C01.mChildIt, cIt);
    EXPECT_EQ(P06_C01.mParent, P06.backIt());
    cIt++;
    auto& P06_C02 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P06_C02.mChildIt, cIt);
    EXPECT_EQ(P06_C02.mParent, P06.backIt());
    EXPECT_EQ(&P06_C01, &L08);
    EXPECT_EQ(&P06_C02, &L10);

    EXPECT_EQ(P07.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P07.mChildren.size(), 2);
    EXPECT_EQ(P07.mPertinentChildrenCount, 2);
    EXPECT_EQ(P07.mFullChildrenCount, 2);

    cIt = P07.mChildren.begin();
    auto& P07_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P07_C01.mChildIt, cIt);
    EXPECT_EQ(P07_C01.mParent, P07.backIt());
    cIt++;
    auto& P07_C02 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P07_C02.mChildIt, cIt);
    EXPECT_EQ(P07_C02.mParent, P07.backIt());
    EXPECT_EQ(&P07_C01, &L09);
    EXPECT_EQ(&P07_C02, &L11);
    EXPECT_EQ(L09.mParent, P07.backIt());
    EXPECT_EQ(L11.mParent, P07.backIt());

    //   P3 positive #2
    //
    //      Q04             Q04
    //       |               |
    //      P08       =>   Q(P08)
    //     /| \             / \
    // L12*L13*L14       L12 *P09
    //                        / \
    //                     *L13 *L14
    initializeForOneIteration(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    L13.mGeneration = pqTree.mGeneration;
    L13.mPertinentType = JTSTreeNode::Full;
    auto& L14 = createL(pqTree);
    L14.mGeneration = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> P08children;
    P08children.push_back(L12.backIt());
    P08children.push_back(L13.backIt());
    P08children.push_back(L14.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    P08.mPertinentChildrenCount = 2;
    P08.mFullChildrenCount = 2;
    list<node_list_it_t> Q04children;
    Q04children.push_back(P08.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 1;
    EXPECT_EQ(templateP3(pqTree, P08), true);
    EXPECT_EQ(P08.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P08.mChildren.size(), 2);
    EXPECT_EQ(P08.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P08.mPertinentChildrenCount, 1);
    EXPECT_EQ(P08.mFullChildrenCount, 1);
    EXPECT_EQ(Q04.mPartialChild1, P08.backIt());
    cIt = P08.mChildren.begin();
    auto& P08_C01 = pqTree.toNodeRef(*cIt);
    cIt++;
    auto& P08_C02 = pqTree.toNodeRef(*cIt);
    node_list_it_t L12It;
    node_list_it_t P09It;
    if (P08_C01.mPertinentType == JTSTreeNode::Full) {    
        L12It = P08_C02.backIt();
        P09It = P08_C01.backIt();
    }    
    else {
        L12It = P08_C01.backIt();
        P09It = P08_C02.backIt();
    }    
    EXPECT_EQ(L12It, L12.backIt());
    auto& P09 = pqTree.toNodeRef(P09It);
    EXPECT_EQ(L12.isPertinent(), false);
    EXPECT_EQ(P09.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P09.mChildren.size(), 2);
    EXPECT_EQ(P09.mPertinentChildrenCount, 2);
    EXPECT_EQ(P09.mFullChildrenCount, 2);

    cIt = P09.mChildren.begin();
    auto& P09_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P09_C01.mChildIt, cIt);
    EXPECT_EQ(P09_C01.mParent, P09.backIt());
    cIt++;
    auto& P09_C02 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P09_C02.mChildIt, cIt);
    EXPECT_EQ(P09_C02.mParent, P09.backIt());
    EXPECT_EQ(&P09_C01, &L13);
    EXPECT_EQ(&P09_C02, &L14);
    
    //   P3 positive #3
    //
    //      Q05             Q05
    //       |               |
    //      P10     =>     Q(P10)
    //     /| \             / \
    // L15 L16 *L17      *L17  P11
    //                        / \
    //                      L15 L16
    initializeForOneIteration(pqTree);
    auto& L15 = createL(pqTree);
    auto& L16 = createL(pqTree);
    auto& L17 = createL(pqTree);
    L17.mGeneration = pqTree.mGeneration;
    L17.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> P10children;
    P10children.push_back(L15.backIt());
    P10children.push_back(L16.backIt());
    P10children.push_back(L17.backIt());
    auto& P10 = createP(pqTree, P10children);
    P10.mGeneration = pqTree.mGeneration;
    P10.mPertinentChildrenCount = 1;
    P10.mFullChildrenCount = 1;
    list<node_list_it_t> Q05children;
    Q05children.push_back(P10.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentChildrenCount = 1;
    EXPECT_EQ(templateP3(pqTree, P10), true);
    EXPECT_EQ(P10.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P10.mChildren.size(), 2);
    EXPECT_EQ(P10.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P10.mPertinentChildrenCount, 1);
    EXPECT_EQ(P10.mFullChildrenCount, 1);
    EXPECT_EQ(Q05.mPartialChild1, P10.backIt());
    cIt = P10.mChildren.begin();
    auto& P10_C01 = pqTree.toNodeRef(*cIt);
    cIt++;
    auto& P10_C02 = pqTree.toNodeRef(*cIt);
    node_list_it_t L17It;
    node_list_it_t P11It;
    if (P08_C01.mPertinentType == JTSTreeNode::Full) {    
        L17It = P10_C01.backIt();
        P11It = P10_C02.backIt();
    }    
    else {
        L17It = P10_C02.backIt();
        P11It = P10_C01.backIt();
    }    
    EXPECT_EQ(L17It, L17.backIt());
    auto& P11 = pqTree.toNodeRef(P11It);
    EXPECT_EQ(L17.isFull(), true);
    EXPECT_EQ(P11.isEmpty(), true);
    EXPECT_EQ(P11.mChildren.size(), 2);
    EXPECT_EQ(P11.mPertinentChildrenCount, 0);
    EXPECT_EQ(P11.mFullChildrenCount, 0);

    cIt = P11.mChildren.begin();
    auto& P11_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P11_C01.mChildIt, cIt);
    EXPECT_EQ(P11_C01.mParent, P11.backIt());
    cIt++;
    auto& P11_C02 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P11_C02.mChildIt, cIt);
    EXPECT_EQ(P11_C02.mParent, P11.backIt());
    EXPECT_EQ(&P11_C01, &L15);
    EXPECT_EQ(&P11_C02, &L16);

    
    //   P4 positive #1
    //
    //         Q06                      Q06
    //          |                        |
    //         P12            =>        P12
    //     /  /  \  \  \             /  /  \
    //  L18 L19   \*L20*L21       L18 L19   \
    //            Q07                       Q07
    //         /  /  \  \              /  /  |  \  \
    //     *L22*L23 L24 L25         P13*L22*L23 L24 L25
    //                              /\
    //                           *L20*L21
    initializeForOneIteration(pqTree);
    auto& L18 = createL(pqTree);
    auto& L19 = createL(pqTree);
    auto& L20 = createL(pqTree);
    L20.mGeneration = pqTree.mGeneration;
    L20.mPertinentType = JTSTreeNode::Full;
    auto& L21 = createL(pqTree);
    L21.mGeneration = pqTree.mGeneration;
    L21.mPertinentType = JTSTreeNode::Full;
    auto& L22 = createL(pqTree);
    L22.mGeneration = pqTree.mGeneration;
    L22.mPertinentType = JTSTreeNode::Full;
    auto& L23 = createL(pqTree);
    L23.mGeneration = pqTree.mGeneration;
    L23.mPertinentType = JTSTreeNode::Full;
    auto& L24 = createL(pqTree);
    auto& L25 = createL(pqTree);
    list<node_list_it_t> Q07children;
    Q07children.push_back(L22.backIt());
    Q07children.push_back(L23.backIt());
    Q07children.push_back(L24.backIt());
    Q07children.push_back(L25.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = JTSTreeNode::Partial;
    Q07.mPertinentChildrenCount = 2;
    Q07.mFullChildrenCount = 2;
    list<node_list_it_t> P12children;
    P12children.push_back(L18.backIt());
    P12children.push_back(L19.backIt());
    P12children.push_back(Q07.backIt());
    P12children.push_back(L20.backIt());
    P12children.push_back(L21.backIt());
    auto& P12 = createP(pqTree, P12children);
    P12.mGeneration = pqTree.mGeneration;
    P12.mPertinentChildrenCount = 3;
    P12.mFullChildrenCount = 2;
    P12.mPartialChild1 = Q07.backIt();

    EXPECT_EQ(templateP4(pqTree, P12, root), true);
    EXPECT_EQ(root, Q07.backIt());
    EXPECT_EQ(P12.mChildren.size(),3);
    cIt = P12.mChildren.begin();
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
    EXPECT_EQ(Q07.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q07.mFullChildrenCount, 3);
    EXPECT_EQ(Q07.mChildren.size(),5);
    cIt = Q07.mChildren.begin();
    auto& P13 = pqTree.toNodeRef(*cIt);
    cIt++;
    EXPECT_EQ(*cIt, L22.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L23.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L24.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L25.backIt());

    EXPECT_EQ(P13.mChildren.size(),2);
    EXPECT_EQ(P13.mPertinentChildrenCount,2);
    EXPECT_EQ(P13.mFullChildrenCount,2);
    EXPECT_EQ(P13.mNodeType,JTSTreeNode::PType);
    EXPECT_EQ(P13.mPertinentType,JTSTreeNode::Full);

    cIt = P13.mChildren.begin();
    EXPECT_EQ(*cIt, L20.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L21.backIt());

    
    //   P4 positive #2
    //
    //         Q08                      Q08
    //          |                        |
    //         P14            =>        P14
    //     /  /  \  \                /  /  \
    //  L26 L27   \*L28            L26 L27  \
    //            Q09                       Q09
    //         /  /  \  \              /  /  |  \  \
    //     *L29*L30 L31 L32        *L28*L29*L30 L31 L32
    initializeForOneIteration(pqTree);
    auto& L26 = createL(pqTree);
    auto& L27 = createL(pqTree);
    auto& L28 = createL(pqTree);
    L28.mGeneration = pqTree.mGeneration;
    L28.mPertinentType = JTSTreeNode::Full;
    auto& L29 = createL(pqTree);
    L29.mGeneration = pqTree.mGeneration;
    L29.mPertinentType = JTSTreeNode::Full;
    auto& L30 = createL(pqTree);
    L30.mGeneration = pqTree.mGeneration;
    L30.mPertinentType = JTSTreeNode::Full;
    auto& L31 = createL(pqTree);
    auto& L32 = createL(pqTree);
    list<node_list_it_t> Q09children;
    Q09children.push_back(L29.backIt());
    Q09children.push_back(L30.backIt());
    Q09children.push_back(L31.backIt());
    Q09children.push_back(L32.backIt());
    auto& Q09 = createQ(pqTree, Q09children);
    Q09.mGeneration = pqTree.mGeneration;
    Q09.mPertinentType = JTSTreeNode::Partial;
    Q09.mPertinentChildrenCount = 2;
    Q09.mFullChildrenCount = 2;
    list<node_list_it_t> P14children;
    P14children.push_back(L26.backIt());
    P14children.push_back(L27.backIt());
    P14children.push_back(Q09.backIt());
    P14children.push_back(L28.backIt());
    auto& P14 = createP(pqTree, P14children);
    P14.mGeneration = pqTree.mGeneration;
    P14.mPertinentChildrenCount = 2;
    P14.mFullChildrenCount = 2;
    P14.mPartialChild1 = Q09.backIt();

    EXPECT_EQ(templateP4(pqTree, P14, root), true);
    EXPECT_EQ(root, Q09.backIt());

    cIt = P14.mChildren.begin();
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
    EXPECT_EQ(Q09.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q09.mFullChildrenCount, 3);
    EXPECT_EQ(Q09.mChildren.size(),5);
    cIt = Q09.mChildren.begin();
    EXPECT_EQ(*cIt, L28.backIt());
    EXPECT_EQ(L28.mPertinentType,JTSTreeNode::Full);
    cIt++;
    EXPECT_EQ(*cIt, L29.backIt());
    EXPECT_EQ(L29.mPertinentType,JTSTreeNode::Full);
    cIt++;
    EXPECT_EQ(*cIt, L30.backIt());
    EXPECT_EQ(L30.mPertinentType,JTSTreeNode::Full);
    cIt++;
    EXPECT_EQ(*cIt, L31.backIt());
    cIt++;
    EXPECT_EQ(*cIt, L32.backIt());


    //   P4 positive #3
    //
    //         Q10                          Q10
    //          |                            |
    //         P15            =>            P15(Q)
    //     /  /  \                     /  /  |  \  \
    // *L33*L34   \                 P16*L35*L36 L37 L38
    //            Q11               / \
    //         /  /  \  \        *L33 *L34
    //     *L35*L36 L37 L38
    initializeForOneIteration(pqTree);
    auto& L35 = createL(pqTree);
    L35.mGeneration = pqTree.mGeneration;
    L35.mPertinentType = JTSTreeNode::Full;
    auto& L36 = createL(pqTree);
    L36.mGeneration = pqTree.mGeneration;
    L36.mPertinentType = JTSTreeNode::Full;
    auto& L37 = createL(pqTree);
    auto& L38 = createL(pqTree);

    list<node_list_it_t> Q11children;
    Q11children.push_back(L35.backIt());
    Q11children.push_back(L36.backIt());
    Q11children.push_back(L37.backIt());
    Q11children.push_back(L38.backIt());
    auto& Q11 = createQ(pqTree, Q11children);
    Q11.mGeneration = pqTree.mGeneration;
    Q11.mPertinentType = JTSTreeNode::Partial;
    Q11.mPertinentChildrenCount = 2;
    Q11.mFullChildrenCount = 2;

    auto& L33 = createL(pqTree);
    L33.mGeneration = pqTree.mGeneration;
    L33.mPertinentType = JTSTreeNode::Full;
    auto& L34 = createL(pqTree);
    L34.mGeneration = pqTree.mGeneration;
    L34.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> P15children;
    P15children.push_back(L33.backIt());
    P15children.push_back(L34.backIt());
    P15children.push_back(Q11.backIt());
    auto& P15 = createP(pqTree, P15children);
    P15.mGeneration = pqTree.mGeneration;
    P15.mPertinentChildrenCount = 3;
    P15.mFullChildrenCount = 2;
    P15.mPartialChild1 = Q11.backIt();

    list<node_list_it_t> Q10children;
    Q10children.push_back(P15.backIt());
    auto& Q10 = createQ(pqTree, Q10children);
    Q10.mGeneration = pqTree.mGeneration;
    Q10.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP4(pqTree, P15, root), true);
    EXPECT_EQ(root, P15.backIt());
    EXPECT_EQ(P15.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P15.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P15.mChildren.size(), 5);
    cIt = Q10.mChildren.begin();
    EXPECT_EQ(P15.backIt(), *cIt);
    EXPECT_EQ(P15.mChildIt, cIt);
    EXPECT_EQ(P15.mParent, Q10.backIt());
    cIt = P15.mChildren.begin();
    auto& P15_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P15_C01.mChildIt, cIt);
    EXPECT_EQ(P15_C01.mParent, P15.backIt());
    cIt++;
    EXPECT_EQ(L35.backIt(), *cIt);
    EXPECT_EQ(L35.mChildIt, cIt);
    EXPECT_EQ(L35.mParent, P15.backIt());
    cIt++;
    EXPECT_EQ(L36.backIt(), *cIt);
    EXPECT_EQ(L36.mChildIt, cIt);
    EXPECT_EQ(L36.mParent, P15.backIt());
    cIt++;
    EXPECT_EQ(L37.backIt(), *cIt);
    EXPECT_EQ(L37.mChildIt, cIt);
    EXPECT_EQ(L37.mParent, P15.backIt());
    cIt++;
    EXPECT_EQ(L38.backIt(), *cIt);
    EXPECT_EQ(L38.mChildIt, cIt);
    EXPECT_EQ(L38.mParent, P15.backIt());

    EXPECT_EQ(P15_C01.mChildren.size(), 2);
    EXPECT_EQ(P15_C01.mNodeType, JTSTreeNode::PType);
    EXPECT_EQ(P15_C01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P15_C01.mPertinentChildrenCount, 2);
    EXPECT_EQ(P15_C01.mFullChildrenCount, 2);
    cIt = P15_C01.mChildren.begin();
    EXPECT_EQ(L33.backIt(), *cIt);
    EXPECT_EQ(L33.mChildIt, cIt);
    EXPECT_EQ(L33.mParent, P15_C01.backIt());
    cIt++;
    EXPECT_EQ(L34.backIt(), *cIt);
    EXPECT_EQ(L34.mChildIt, cIt);
    EXPECT_EQ(L34.mParent, P15_C01.backIt());


    //   P5 positive #1
    //
    //         Q12                                Q12
    //          |                                  |
    //      ___P17___         =>           _______P17(Q)________
    //     /  / | \  \                    /   /   /    \   \    \
    //  L39 L40 |*L41*L42               P18*L43 *L44  L45  L46  P19
    //       __Q13___                   / \                     / \
    //      /  /  \  \               *L41*L42                 L39 L40
    //   *L43*L44 L45 L46
    initializeForOneIteration(pqTree);
    auto& L39 = createL(pqTree);
    auto& L40 = createL(pqTree);
    auto& L41 = createL(pqTree);
    L41.mGeneration = pqTree.mGeneration;
    L41.mPertinentType = JTSTreeNode::Full;
    auto& L42 = createL(pqTree);
    L42.mGeneration = pqTree.mGeneration;
    L42.mPertinentType = JTSTreeNode::Full;
    auto& L43 = createL(pqTree);
    L43.mGeneration = pqTree.mGeneration;
    L43.mPertinentType = JTSTreeNode::Full;
    auto& L44 = createL(pqTree);
    L44.mGeneration = pqTree.mGeneration;
    L44.mPertinentType = JTSTreeNode::Full;
    auto& L45 = createL(pqTree);
    auto& L46 = createL(pqTree);
    list<node_list_it_t> Q13children;
    Q13children.push_back(L43.backIt());
    Q13children.push_back(L44.backIt());
    Q13children.push_back(L45.backIt());
    Q13children.push_back(L46.backIt());
    auto& Q13 = createQ(pqTree, Q13children);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = JTSTreeNode::Partial;
    Q13.mPertinentChildrenCount = 2;
    Q13.mFullChildrenCount = 2;

    list<node_list_it_t> P17children;
    P17children.push_back(L39.backIt());
    P17children.push_back(L40.backIt());
    P17children.push_back(Q13.backIt());
    P17children.push_back(L41.backIt());
    P17children.push_back(L42.backIt());
    auto& P17 = createP(pqTree, P17children);
    P17.mPertinentChildrenCount = 3;
    P17.mFullChildrenCount = 2;
    P17.mPartialChild1 = Q13.backIt();

    list<node_list_it_t> Q12children;
    Q12children.push_back(P17.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP5(pqTree, P17), true);

    EXPECT_EQ(P17.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P17.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P17.mChildren.size(), 6);
    cIt = P17.mChildren.begin();
    auto& P17_C01 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P17_C01.mChildIt, cIt);
    EXPECT_EQ(P17_C01.mParent, P17.backIt());
    cIt++;
    EXPECT_EQ(L43.backIt(), *cIt);
    EXPECT_EQ(L43.mChildIt, cIt);
    EXPECT_EQ(L43.mParent, P17.backIt());
    cIt++;
    EXPECT_EQ(L44.backIt(), *cIt);
    EXPECT_EQ(L44.mChildIt, cIt);
    EXPECT_EQ(L44.mParent, P17.backIt());
    cIt++;
    EXPECT_EQ(L45.backIt(), *cIt);
    EXPECT_EQ(L45.mChildIt, cIt);
    EXPECT_EQ(L45.mParent, P17.backIt());
    cIt++;
    EXPECT_EQ(L46.backIt(), *cIt);
    EXPECT_EQ(L46.mChildIt, cIt);
    EXPECT_EQ(L46.mParent, P17.backIt());
    cIt++;    
    auto& P17_C06 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P17_C06.mChildIt, cIt);
    EXPECT_EQ(P17_C06.mParent, P17.backIt());

    EXPECT_EQ(P17_C01.mChildren.size(), 2);
    EXPECT_EQ(P17_C01.mPertinentChildrenCount, 2);
    EXPECT_EQ(P17_C01.mFullChildrenCount, 2);
    EXPECT_EQ(P17_C01.mPertinentType, JTSTreeNode::Full);
    cIt = P17_C01.mChildren.begin();
    EXPECT_EQ(L41.backIt(), *cIt);
    EXPECT_EQ(L41.mChildIt, cIt);
    EXPECT_EQ(L41.mParent, P17_C01.backIt());
    cIt++;
    EXPECT_EQ(L42.backIt(), *cIt);
    EXPECT_EQ(L42.mChildIt, cIt);
    EXPECT_EQ(L42.mParent, P17_C01.backIt());


    EXPECT_EQ(P17_C06.mChildren.size(), 2);
    EXPECT_EQ(P17_C06.mPertinentChildrenCount, 0);
    EXPECT_EQ(P17_C06.mFullChildrenCount, 0);
    EXPECT_EQ(P17_C06.mPertinentType, JTSTreeNode::Empty);

    cIt = P17_C06.mChildren.begin();
    EXPECT_EQ(L39.backIt(), *cIt);
    EXPECT_EQ(L39.mChildIt, cIt);
    EXPECT_EQ(L39.mParent, P17_C06.backIt());
    cIt++;
    EXPECT_EQ(L40.backIt(), *cIt);
    EXPECT_EQ(L40.mChildIt, cIt);
    EXPECT_EQ(L40.mParent, P17_C06.backIt());


    //   P5 positive #2
    //
    //         Q15                                Q15
    //          |                                  |
    //      ___P20___         =>           _______P20(Q)________
    //     /    | \  \                    /   /   /    \   \    \
    //  L47     |*L48*L49               L47 L50  L51 *L52 *L53  P21
    //       __Q14___                                           / \
    //      /  /  \  \                                       *L48*L49
    //    L50 L51*L52*L53
    initializeForOneIteration(pqTree);
    auto& L47 = createL(pqTree);
    auto& L48 = createL(pqTree);
    L48.mGeneration = pqTree.mGeneration;
    L48.mPertinentType = JTSTreeNode::Full;
    auto& L49 = createL(pqTree);
    L49.mGeneration = pqTree.mGeneration;
    L49.mPertinentType = JTSTreeNode::Full;
    auto& L50 = createL(pqTree);
    auto& L51 = createL(pqTree);
    auto& L52 = createL(pqTree);
    L52.mGeneration = pqTree.mGeneration;
    L52.mPertinentType = JTSTreeNode::Full;
    auto& L53 = createL(pqTree);
    L53.mGeneration = pqTree.mGeneration;
    L53.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q14children;
    Q14children.push_back(L50.backIt());
    Q14children.push_back(L51.backIt());
    Q14children.push_back(L52.backIt());
    Q14children.push_back(L53.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = JTSTreeNode::Partial;
    Q14.mPertinentChildrenCount = 2;
    Q14.mFullChildrenCount = 2;

    list<node_list_it_t> P20children;
    P20children.push_back(L47.backIt());
    P20children.push_back(Q14.backIt());
    P20children.push_back(L48.backIt());
    P20children.push_back(L49.backIt());
    auto& P20 = createP(pqTree, P20children);
    P20.mPertinentChildrenCount = 3;
    P20.mFullChildrenCount = 2;
    P20.mPartialChild1 = Q14.backIt();

    list<node_list_it_t> Q15children;
    Q15children.push_back(P20.backIt());
    auto& Q15 = createQ(pqTree, Q15children);
    Q15.mGeneration = pqTree.mGeneration;
    Q15.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP5(pqTree, P20), true);
    EXPECT_EQ(P20.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P20.mChildren.size(), 6);
    EXPECT_EQ(P20.mChildIt, Q15.mChildren.begin());
    EXPECT_EQ(P20.mParent, Q15.backIt());
    EXPECT_EQ(P20.mPertinentChildrenCount, 3);
    EXPECT_EQ(P20.mFullChildrenCount, 3);
    EXPECT_EQ(P20.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P20.mPartialChild2, pqTree.nil());
    cIt = P20.mChildren.begin();
    EXPECT_EQ(L47.backIt(), *cIt);
    EXPECT_EQ(L47.mParent, P20.backIt());
    EXPECT_EQ(L47.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L50.backIt(), *cIt);
    EXPECT_EQ(L50.mParent, P20.backIt());
    EXPECT_EQ(L50.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L51.backIt(), *cIt);
    EXPECT_EQ(L51.mParent, P20.backIt());
    EXPECT_EQ(L51.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L52.backIt(), *cIt);
    EXPECT_EQ(L52.mParent, P20.backIt());
    EXPECT_EQ(L52.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L53.backIt(), *cIt);
    EXPECT_EQ(L53.mParent, P20.backIt());
    EXPECT_EQ(L53.mChildIt, cIt);
    cIt++;
    auto& P20_C06 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P20_C06.backIt(), *cIt);
    EXPECT_EQ(P20_C06.mParent, P20.backIt());
    EXPECT_EQ(P20_C06.mChildIt, cIt);
    EXPECT_EQ(P20_C06.mChildren.size(), 2);
    EXPECT_EQ(P20_C06.mPertinentChildrenCount, 2);
    EXPECT_EQ(P20_C06.mFullChildrenCount, 2);
    EXPECT_EQ(P20_C06.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P20_C06.mPartialChild2, pqTree.nil());
    cIt = P20_C06.mChildren.begin();
    EXPECT_EQ(L48.backIt(), *cIt);
    EXPECT_EQ(L48.mParent, P20_C06.backIt());
    EXPECT_EQ(L48.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L49.backIt(), *cIt);
    EXPECT_EQ(L49.mParent, P20_C06.backIt());
    EXPECT_EQ(L49.mChildIt, cIt);


    //   P5 positive #3
    //
    //         Q16                                Q16
    //          |                                  |
    //         P22___         =>               ___P22(Q)________
    //          | \  \                        /   /    \   \    \
    //          |*L54*L55                   L56  L57 *L58 *L59  P23
    //       __Q17___                                           / \
    //      /  /  \  \                                       *L54*L55
    //    L56 L57*L58*L59

    initializeForOneIteration(pqTree);
    auto& L54 = createL(pqTree);
    L54.mGeneration = pqTree.mGeneration;
    L54.mPertinentType = JTSTreeNode::Full;
    auto& L55 = createL(pqTree);
    L55.mGeneration = pqTree.mGeneration;
    L55.mPertinentType = JTSTreeNode::Full;
    auto& L56 = createL(pqTree);
    auto& L57 = createL(pqTree);
    auto& L58 = createL(pqTree);
    L58.mGeneration = pqTree.mGeneration;
    L58.mPertinentType = JTSTreeNode::Full;
    auto& L59 = createL(pqTree);
    L59.mGeneration = pqTree.mGeneration;
    L59.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> Q17children;
    Q17children.push_back(L56.backIt());
    Q17children.push_back(L57.backIt());
    Q17children.push_back(L58.backIt());
    Q17children.push_back(L59.backIt());
    auto& Q17 = createQ(pqTree, Q17children);
    Q17.mGeneration = pqTree.mGeneration;
    Q17.mPertinentType = JTSTreeNode::Partial;
    Q17.mPertinentChildrenCount = 2;
    Q17.mFullChildrenCount = 2;

    list<node_list_it_t> P22children;
    P22children.push_back(Q17.backIt());
    P22children.push_back(L54.backIt());
    P22children.push_back(L55.backIt());
    auto& P22 = createP(pqTree, P22children);
    P22.mPertinentChildrenCount = 3;
    P22.mFullChildrenCount = 2;
    P22.mPartialChild1 = Q17.backIt();

    list<node_list_it_t> Q16children;
    Q16children.push_back(P22.backIt());
    auto& Q16 = createQ(pqTree, Q16children);
    Q16.mGeneration = pqTree.mGeneration;
    Q16.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP5(pqTree, P22), true);
    EXPECT_EQ(P22.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P22.mChildren.size(), 5);
    EXPECT_EQ(P22.mChildIt, Q16.mChildren.begin());
    EXPECT_EQ(P22.mParent, Q16.backIt());
    EXPECT_EQ(P22.mPertinentChildrenCount, 3);
    EXPECT_EQ(P22.mFullChildrenCount, 3);
    EXPECT_EQ(P22.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P22.mPartialChild2, pqTree.nil());
    cIt = P22.mChildren.begin();
    EXPECT_EQ(L56.backIt(), *cIt);
    EXPECT_EQ(L56.mParent, P22.backIt());
    EXPECT_EQ(L56.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L57.backIt(), *cIt);
    EXPECT_EQ(L57.mParent, P22.backIt());
    EXPECT_EQ(L57.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L58.backIt(), *cIt);
    EXPECT_EQ(L58.mParent, P22.backIt());
    EXPECT_EQ(L58.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L59.backIt(), *cIt);
    EXPECT_EQ(L59.mParent, P22.backIt());
    EXPECT_EQ(L59.mChildIt, cIt);
    cIt++;
    auto& P22_C05 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P22_C05.backIt(), *cIt);
    EXPECT_EQ(P22_C05.mParent, P22.backIt());
    EXPECT_EQ(P22_C05.mChildIt, cIt);
    EXPECT_EQ(P22_C05.mChildren.size(), 2);
    EXPECT_EQ(P22_C05.mPertinentChildrenCount, 2);
    EXPECT_EQ(P22_C05.mFullChildrenCount, 2);
    EXPECT_EQ(P22_C05.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P22_C05.mPartialChild2, pqTree.nil());
    cIt = P22_C05.mChildren.begin();
    EXPECT_EQ(L54.backIt(), *cIt);
    EXPECT_EQ(L54.mParent, P22_C05.backIt());
    EXPECT_EQ(L54.mChildIt, cIt);
    cIt++;
    EXPECT_EQ(L55.backIt(), *cIt);
    EXPECT_EQ(L55.mParent, P22_C05.backIt());
    EXPECT_EQ(L55.mChildIt, cIt);


    //   P5 positive #4
    //
    //         Q18                                Q18
    //          |                                  |
    //         P24            =>               ___P24(Q)________
    //          | \                           /   /    \   \    \
    //          |*L60                       L61  L62 *L63 *L64 *L60
    //       __Q19___
    //      /  /  \  \
    //    L61 L62*L63*L64

    initializeForOneIteration(pqTree);
    auto& L60 = createL(pqTree);
    L60.mGeneration = pqTree.mGeneration;
    L60.mPertinentType = JTSTreeNode::Full;
    auto& L61 = createL(pqTree);
    auto& L62 = createL(pqTree);
    auto& L63 = createL(pqTree);
    L63.mGeneration = pqTree.mGeneration;
    L63.mPertinentType = JTSTreeNode::Full;
    auto& L64 = createL(pqTree);
    L64.mGeneration = pqTree.mGeneration;
    L64.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> Q19children;
    Q19children.push_back(L61.backIt());
    Q19children.push_back(L62.backIt());
    Q19children.push_back(L63.backIt());
    Q19children.push_back(L64.backIt());
    auto& Q19 = createQ(pqTree, Q19children);
    Q19.mGeneration = pqTree.mGeneration;
    Q19.mPertinentType = JTSTreeNode::Partial;
    Q19.mPertinentChildrenCount = 2;
    Q19.mFullChildrenCount = 2;

    list<node_list_it_t> P24children;
    P24children.push_back(Q19.backIt());
    P24children.push_back(L60.backIt());
    auto& P24 = createP(pqTree, P24children);
    P24.mGeneration = pqTree.mGeneration;
    P24.mPertinentType = JTSTreeNode::Partial;
    P24.mPertinentChildrenCount = 2;
    P24.mFullChildrenCount = 1;
    P24.mPartialChild1 = Q19.backIt();

    EXPECT_EQ(templateP5(pqTree, P24), true);
    EXPECT_EQ(P24.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P24.mChildren.size(), 5);
    EXPECT_EQ(P24.mPertinentChildrenCount, 3);
    EXPECT_EQ(P24.mFullChildrenCount, 3);
    EXPECT_EQ(P24.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P24.mPartialChild2, pqTree.nil());

    cIt = P24.mChildren.begin();
    EXPECT_EQ(L61.backIt(), *cIt);
    EXPECT_EQ(L61.mChildIt, cIt);
    EXPECT_EQ(L61.mParent,  P24.backIt());
    cIt++;
    EXPECT_EQ(L62.backIt(), *cIt);
    EXPECT_EQ(L62.mChildIt, cIt);
    EXPECT_EQ(L62.mParent,  P24.backIt());
    cIt++;
    EXPECT_EQ(L63.backIt(), *cIt);
    EXPECT_EQ(L63.mChildIt, cIt);
    EXPECT_EQ(L63.mParent,  P24.backIt());
    cIt++;
    EXPECT_EQ(L64.backIt(), *cIt);
    EXPECT_EQ(L64.mChildIt, cIt);
    EXPECT_EQ(L64.mParent,  P24.backIt());
    cIt++;
    EXPECT_EQ(L60.backIt(), *cIt);
    EXPECT_EQ(L60.mChildIt, cIt);
    EXPECT_EQ(L60.mParent,  P24.backIt());


    //   P5 positive #5
    //
    //         Q20                                Q20
    //          |                                  |
    //         P25            =>               ___P25(Q)________
    //          | \                           /   /    \   \    \
    //          | L65                       L65  L66  L67 *L68 *L69
    //       __Q21___
    //      /  /  \  \
    //    L66 L67*L68*L69

    initializeForOneIteration(pqTree);
    auto& L65 = createL(pqTree);
    auto& L66 = createL(pqTree);
    auto& L67 = createL(pqTree);
    auto& L68 = createL(pqTree);
    L68.mGeneration = pqTree.mGeneration;
    L68.mPertinentType = JTSTreeNode::Full;
    auto& L69 = createL(pqTree);
    L69.mGeneration = pqTree.mGeneration;
    L69.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> Q21children;
    Q21children.push_back(L66.backIt());
    Q21children.push_back(L67.backIt());
    Q21children.push_back(L68.backIt());
    Q21children.push_back(L69.backIt());
    auto& Q21 = createQ(pqTree, Q21children);
    Q21.mGeneration = pqTree.mGeneration;
    Q21.mPertinentType = JTSTreeNode::Partial;
    Q21.mPertinentChildrenCount = 2;
    Q21.mFullChildrenCount = 2;

    list<node_list_it_t> P25children;
    P25children.push_back(Q21.backIt());
    P25children.push_back(L65.backIt());
    auto& P25 = createP(pqTree, P25children);
    P25.mGeneration = pqTree.mGeneration;
    P25.mPertinentType = JTSTreeNode::Partial;
    P25.mPertinentChildrenCount = 1;
    P25.mFullChildrenCount = 0;
    P25.mPartialChild1 = Q21.backIt();

    EXPECT_EQ(templateP5(pqTree, P25), true);
    EXPECT_EQ(P25.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P25.mChildren.size(), 5);
    EXPECT_EQ(P25.mPertinentChildrenCount, 2);
    EXPECT_EQ(P25.mFullChildrenCount, 2);
    EXPECT_EQ(P25.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P25.mPartialChild2, pqTree.nil());

    cIt = P25.mChildren.begin();
    EXPECT_EQ(L65.backIt(), *cIt);
    EXPECT_EQ(L65.mChildIt, cIt);
    EXPECT_EQ(L65.mParent,  P25.backIt());
    cIt++;
    EXPECT_EQ(L66.backIt(), *cIt);
    EXPECT_EQ(L66.mChildIt, cIt);
    EXPECT_EQ(L66.mParent,  P25.backIt());
    cIt++;
    EXPECT_EQ(L67.backIt(), *cIt);
    EXPECT_EQ(L67.mChildIt, cIt);
    EXPECT_EQ(L67.mParent,  P25.backIt());
    cIt++;
    EXPECT_EQ(L68.backIt(), *cIt);
    EXPECT_EQ(L68.mChildIt, cIt);
    EXPECT_EQ(L68.mParent,  P25.backIt());
    cIt++;
    EXPECT_EQ(L69.backIt(), *cIt);
    EXPECT_EQ(L69.mChildIt, cIt);
    EXPECT_EQ(L69.mParent,  P25.backIt());


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

    list<node_list_it_t> Q22children;
    Q22children.push_back(P26.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP6(pqTree, P26, root), true);

    EXPECT_EQ(P26.mChildren.size(), 3);
    cIt = P26.mChildren.begin();    
    EXPECT_EQ(L70.backIt(), *cIt);
    EXPECT_EQ(L70.mChildIt, cIt);
    EXPECT_EQ(L70.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(Q23.backIt(), *cIt);
    EXPECT_EQ(Q23.mChildIt, cIt);
    EXPECT_EQ(Q23.mParent, P26.backIt());
    cIt++;
    EXPECT_EQ(L72.backIt(), *cIt);
    EXPECT_EQ(L72.mChildIt, cIt);
    EXPECT_EQ(L72.mParent, P26.backIt());

    EXPECT_EQ(Q23.backIt(), root);
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
    auto& Q23_C05 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(Q23_C05.mChildIt, cIt);
    EXPECT_EQ(Q23_C05.mParent, Q23.backIt());

    cIt++;
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

    EXPECT_EQ(Q23.mPertinentChildrenCount, 5);
    EXPECT_EQ(Q23.mFullChildrenCount, 5);
    EXPECT_EQ(Q23.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23.mPartialChild2, pqTree.nil());

    cIt = Q23_C05.mChildren.begin();
    EXPECT_EQ(L71.backIt(), *cIt);
    EXPECT_EQ(L71.mChildIt, cIt);
    EXPECT_EQ(L71.mParent, Q23_C05.backIt());
    cIt++;
    EXPECT_EQ(L73.backIt(), *cIt);
    EXPECT_EQ(L73.mChildIt, cIt);
    EXPECT_EQ(L73.mParent, Q23_C05.backIt());
    EXPECT_EQ(Q23_C05.mFullChildrenCount, 2);
    EXPECT_EQ(Q23_C05.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q23_C05.mPartialChild2, pqTree.nil());
    EXPECT_EQ(Q23_C05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(Q23_C05.mNodeType, JTSTreeNode::PType);

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
    initializeForOneIteration(pqTree);
    auto& L82 = createL(pqTree);
    auto& L83 = createL(pqTree);
    L83.mGeneration = pqTree.mGeneration;
    L83.mPertinentType = JTSTreeNode::Full;
    auto& L84 = createL(pqTree);
    auto& L85 = createL(pqTree);
    L85.mGeneration = pqTree.mGeneration;
    L85.mPertinentType = JTSTreeNode::Full;
    auto& L86 = createL(pqTree);
    L86.mGeneration = pqTree.mGeneration;
    L86.mPertinentType = JTSTreeNode::Full;
    auto& L87 = createL(pqTree);
    auto& L88 = createL(pqTree);
    auto& L89 = createL(pqTree);
    auto& L90 = createL(pqTree);
    auto& L91 = createL(pqTree);
    L91.mGeneration = pqTree.mGeneration;
    L91.mPertinentType = JTSTreeNode::Full;
    auto& L92 = createL(pqTree);
    L92.mGeneration = pqTree.mGeneration;
    L92.mPertinentType = JTSTreeNode::Full;
    list<node_list_it_t> Q26children;
    Q26children.push_back(L85.backIt());
    Q26children.push_back(L86.backIt());
    Q26children.push_back(L87.backIt());
    Q26children.push_back(L88.backIt());
    auto& Q26 = createQ(pqTree, Q26children);
    Q26.mGeneration = pqTree.mGeneration;
    Q26.mPertinentType = JTSTreeNode::Partial;
    Q26.mPertinentChildrenCount = 2;
    Q26.mFullChildrenCount = 2;
    list<node_list_it_t> Q27children;
    Q27children.push_back(L89.backIt());
    Q27children.push_back(L90.backIt());
    Q27children.push_back(L91.backIt());
    Q27children.push_back(L92.backIt());
    auto& Q27 = createQ(pqTree, Q27children);
    Q27.mGeneration = pqTree.mGeneration;
    Q27.mPertinentType = JTSTreeNode::Partial;
    Q27.mPertinentChildrenCount = 2;
    Q27.mFullChildrenCount = 2;

    list<node_list_it_t> P28children;
    P28children.push_back(L82.backIt());
    P28children.push_back(Q26.backIt());
    P28children.push_back(L83.backIt());
    P28children.push_back(L84.backIt());
    P28children.push_back(Q27.backIt());
    auto& P28 = createP(pqTree, P28children);
    P28.mGeneration = pqTree.mGeneration;
    P28.mPertinentChildrenCount = 3;
    P28.mFullChildrenCount = 1;
    P28.mPartialChild1 = Q26.backIt();
    P28.mPartialChild2 = Q27.backIt();

    list<node_list_it_t> Q25children;
    Q25children.push_back(P28.backIt());
    auto& Q25 = createQ(pqTree, Q25children);
    Q25.mGeneration = pqTree.mGeneration;
    Q25.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP6(pqTree, P28, root), true);

    EXPECT_EQ(P28.mChildren.size(), 3);
    cIt = P28.mChildren.begin();    
    EXPECT_EQ(L82.backIt(), *cIt);
    EXPECT_EQ(L82.mChildIt, cIt);
    EXPECT_EQ(L82.mParent, P28.backIt());
    cIt++;
    EXPECT_EQ(Q26.backIt(), *cIt);
    EXPECT_EQ(Q26.mChildIt, cIt);
    EXPECT_EQ(Q26.mParent, P28.backIt());
    cIt++;
    EXPECT_EQ(L84.backIt(), *cIt);
    EXPECT_EQ(L84.mChildIt, cIt);
    EXPECT_EQ(L84.mParent, P28.backIt());

    EXPECT_EQ(Q26.backIt(), root);
    EXPECT_EQ(Q26.mChildren.size(), 9);
    cIt = Q26.mChildren.begin();
    EXPECT_EQ(L89.backIt(), *cIt);
    EXPECT_EQ(L89.mChildIt, cIt);
    EXPECT_EQ(L89.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L90.backIt(), *cIt);
    EXPECT_EQ(L90.mChildIt, cIt);
    EXPECT_EQ(L90.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L91.backIt(), *cIt);
    EXPECT_EQ(L91.mChildIt, cIt);
    EXPECT_EQ(L91.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L92.backIt(), *cIt);
    EXPECT_EQ(L92.mChildIt, cIt);
    EXPECT_EQ(L92.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L83.backIt(), *cIt);
    EXPECT_EQ(L83.mChildIt, cIt);
    EXPECT_EQ(L83.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L85.backIt(), *cIt);
    EXPECT_EQ(L85.mChildIt, cIt);
    EXPECT_EQ(L85.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L86.backIt(), *cIt);
    EXPECT_EQ(L86.mChildIt, cIt);
    EXPECT_EQ(L86.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L87.backIt(), *cIt);
    EXPECT_EQ(L87.mChildIt, cIt);
    EXPECT_EQ(L87.mParent, Q26.backIt());
    cIt++;
    EXPECT_EQ(L88.backIt(), *cIt);
    EXPECT_EQ(L88.mChildIt, cIt);
    EXPECT_EQ(L88.mParent, Q26.backIt());

    EXPECT_EQ(Q26.mPertinentChildrenCount, 5);
    EXPECT_EQ(Q26.mFullChildrenCount, 5);
    EXPECT_EQ(Q26.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q26.mPartialChild2, pqTree.nil());

    //   P6 positive #3
    //
    //         Q28                                Q28
    //          |                                  |
    //        _P29______________       =>         P29
    //       /  |     \         \               /  |  \
    //     L93  |    L94        |             L93 Q29 L94
    //       __Q29___       ____Q30___       ______|________________________
    //      /  /  \  \     /   /   \  \      |   |   |   |   |    |    |    |
    //    L95 L96*L97*L98*L99*L100 L101L102 L95 L96*L97*L98*L99*L100 L101 L102
    initializeForOneIteration(pqTree);
    auto& L93 = createL(pqTree);
    auto& L94 = createL(pqTree);
    auto& L95 = createL(pqTree);
    auto& L96 = createL(pqTree);
    auto& L97 = createL(pqTree);
    L97.mGeneration = pqTree.mGeneration;
    L97.mPertinentType = JTSTreeNode::Full;
    auto& L98 = createL(pqTree);
    L98.mGeneration = pqTree.mGeneration;
    L98.mPertinentType = JTSTreeNode::Full;
    auto& L99 = createL(pqTree);
    L99.mGeneration = pqTree.mGeneration;
    L99.mPertinentType = JTSTreeNode::Full;
    auto& L100 = createL(pqTree);
    L100.mGeneration = pqTree.mGeneration;
    L100.mPertinentType = JTSTreeNode::Full;
    auto& L101 = createL(pqTree);
    auto& L102 = createL(pqTree);

    list<node_list_it_t> Q29children;
    Q29children.push_back(L95.backIt());
    Q29children.push_back(L96.backIt());
    Q29children.push_back(L97.backIt());
    Q29children.push_back(L98.backIt());
    auto& Q29 = createQ(pqTree, Q29children);
    Q29.mGeneration = pqTree.mGeneration;
    Q29.mPertinentType = JTSTreeNode::Partial;
    Q29.mPertinentChildrenCount = 2;
    Q29.mFullChildrenCount = 2;
    list<node_list_it_t> Q30children;
    Q30children.push_back(L99.backIt());
    Q30children.push_back(L100.backIt());
    Q30children.push_back(L101.backIt());
    Q30children.push_back(L102.backIt());
    auto& Q30 = createQ(pqTree, Q30children);
    Q30.mGeneration = pqTree.mGeneration;
    Q30.mPertinentType = JTSTreeNode::Partial;
    Q30.mPertinentChildrenCount = 2;
    Q30.mFullChildrenCount = 2;

    list<node_list_it_t> P29children;
    P29children.push_back(L93.backIt());
    P29children.push_back(Q29.backIt());
    P29children.push_back(L94.backIt());
    P29children.push_back(Q30.backIt());
    auto& P29 = createP(pqTree, P29children);
    P29.mGeneration = pqTree.mGeneration;
    P29.mPertinentChildrenCount = 2;
    P29.mFullChildrenCount = 0;
    P29.mPartialChild1 = Q29.backIt();
    P29.mPartialChild2 = Q30.backIt();

    list<node_list_it_t> Q28children;
    Q28children.push_back(P29.backIt());
    auto& Q28 = createQ(pqTree, Q28children);
    Q28.mGeneration = pqTree.mGeneration;
    Q28.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP6(pqTree, P29, root), true);

    EXPECT_EQ(P29.mChildren.size(), 3);
    cIt = P29.mChildren.begin();    
    EXPECT_EQ(L93.backIt(), *cIt);
    EXPECT_EQ(L93.mChildIt, cIt);
    EXPECT_EQ(L93.mParent, P29.backIt());
    cIt++;
    EXPECT_EQ(Q29.backIt(), *cIt);
    EXPECT_EQ(Q29.mChildIt, cIt);
    EXPECT_EQ(Q29.mParent, P29.backIt());
    cIt++;
    EXPECT_EQ(L94.backIt(), *cIt);
    EXPECT_EQ(L94.mChildIt, cIt);
    EXPECT_EQ(L94.mParent, P29.backIt());

    EXPECT_EQ(Q29.backIt(), root);
    EXPECT_EQ(Q29.mChildren.size(), 8);
    cIt = Q29.mChildren.begin();
    EXPECT_EQ(L95.backIt(), *cIt);
    EXPECT_EQ(L95.mChildIt, cIt);
    EXPECT_EQ(L95.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L96.backIt(), *cIt);
    EXPECT_EQ(L96.mChildIt, cIt);
    EXPECT_EQ(L96.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L97.backIt(), *cIt);
    EXPECT_EQ(L97.mChildIt, cIt);
    EXPECT_EQ(L97.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L98.backIt(), *cIt);
    EXPECT_EQ(L98.mChildIt, cIt);
    EXPECT_EQ(L98.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L99.backIt(), *cIt);
    EXPECT_EQ(L99.mChildIt, cIt);
    EXPECT_EQ(L99.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L100.backIt(), *cIt);
    EXPECT_EQ(L100.mChildIt, cIt);
    EXPECT_EQ(L100.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L101.backIt(), *cIt);
    EXPECT_EQ(L101.mChildIt, cIt);
    EXPECT_EQ(L101.mParent, Q29.backIt());
    cIt++;
    EXPECT_EQ(L102.backIt(), *cIt);
    EXPECT_EQ(L102.mChildIt, cIt);
    EXPECT_EQ(L102.mParent, Q29.backIt());

    EXPECT_EQ(Q29.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q29.mFullChildrenCount, 4);
    EXPECT_EQ(Q29.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q29.mPartialChild2, pqTree.nil());


    //   P6 positive #4
    //
    //       Q31                                  Q31
    //        |                                    |
    //       P30_________________       =>        P30(Q)
    //        | \       \        \         ________|________________________
    //        |*L103   *L104     |        |   |   |    |   |   |    |   |   |
    //     __Q32_____       ____Q33____ L109L110*L111*L112P31*L105*L106L107L108
    //    /  /   \   \     /   /   \   \                  / \
    //*L105*L106L107L108L109L110*L111*L112           *L103  *L104
    initializeForOneIteration(pqTree);
    auto& L103 = createL(pqTree);
    L103.mGeneration = pqTree.mGeneration;
    L103.mPertinentType = JTSTreeNode::Full;
    auto& L104 = createL(pqTree);
    L104.mGeneration = pqTree.mGeneration;
    L104.mPertinentType = JTSTreeNode::Full;
    auto& L105 = createL(pqTree);
    L105.mGeneration = pqTree.mGeneration;
    L105.mPertinentType = JTSTreeNode::Full;
    auto& L106 = createL(pqTree);
    L106.mGeneration = pqTree.mGeneration;
    L106.mPertinentType = JTSTreeNode::Full;
    auto& L107 = createL(pqTree);
    auto& L108 = createL(pqTree);
    auto& L109 = createL(pqTree);
    auto& L110 = createL(pqTree);
    auto& L111 = createL(pqTree);
    L111.mGeneration = pqTree.mGeneration;
    L111.mPertinentType = JTSTreeNode::Full;
    auto& L112 = createL(pqTree);
    L112.mGeneration = pqTree.mGeneration;
    L112.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q32children;
    Q32children.push_back(L105.backIt());
    Q32children.push_back(L106.backIt());
    Q32children.push_back(L107.backIt());
    Q32children.push_back(L108.backIt());
    auto& Q32 = createQ(pqTree, Q32children);
    Q32.mGeneration = pqTree.mGeneration;
    Q32.mPertinentType = JTSTreeNode::Partial;
    Q32.mPertinentChildrenCount = 2;
    Q32.mFullChildrenCount = 2;
    list<node_list_it_t> Q33children;
    Q33children.push_back(L109.backIt());
    Q33children.push_back(L110.backIt());
    Q33children.push_back(L111.backIt());
    Q33children.push_back(L112.backIt());
    auto& Q33 = createQ(pqTree, Q33children);
    Q33.mGeneration = pqTree.mGeneration;
    Q33.mPertinentType = JTSTreeNode::Partial;
    Q33.mPertinentChildrenCount = 2;
    Q33.mFullChildrenCount = 2;

    list<node_list_it_t> P30children;
    P30children.push_back(Q32.backIt());
    P30children.push_back(L103.backIt());
    P30children.push_back(L104.backIt());
    P30children.push_back(Q33.backIt());
    auto& P30 = createP(pqTree, P30children);
    P30.mGeneration = pqTree.mGeneration;
    P30.mPertinentChildrenCount = 4;
    P30.mFullChildrenCount = 2;
    P30.mPartialChild1 = Q32.backIt();
    P30.mPartialChild2 = Q33.backIt();

    list<node_list_it_t> Q31children;
    Q31children.push_back(P30.backIt());
    auto& Q31 = createQ(pqTree, Q31children);
    Q31.mGeneration = pqTree.mGeneration;
    Q31.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateP6(pqTree, P30, root), true);

    //   P6 positive #4
    //
    //       Q31                                  Q31
    //        |                                    |
    //       P30_________________       =>        P30(Q)
    //        | \       \        \         ________|________________________
    //        |*L103   *L104     |        |   |   |    |   |   |    |   |   |
    //     __Q32_____       ____Q33____ L109L110*L111*L112P31*L105*L106L107L108
    //    /  /   \   \     /   /   \   \                  / \
    //*L105*L106L107L108L109L110*L111*L112           *L103  *L104

    cIt = Q31.mChildren.begin();    
    EXPECT_EQ(P30.backIt(), *cIt);
    EXPECT_EQ(P30.mChildIt, cIt);
    EXPECT_EQ(P30.mParent, Q31.backIt());

    EXPECT_EQ(P30.mChildren.size(), 9);
    cIt = P30.mChildren.begin();    
    EXPECT_EQ(L109.backIt(), *cIt);
    EXPECT_EQ(L109.mChildIt, cIt);
    EXPECT_EQ(L109.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L110.backIt(), *cIt);
    EXPECT_EQ(L110.mChildIt, cIt);
    EXPECT_EQ(L110.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L111.backIt(), *cIt);
    EXPECT_EQ(L111.mChildIt, cIt);
    EXPECT_EQ(L111.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L112.backIt(), *cIt);
    EXPECT_EQ(L112.mChildIt, cIt);
    EXPECT_EQ(L112.mParent, P30.backIt());
    cIt++;
    auto& P30_C05 = pqTree.toNodeRef(*cIt);
    EXPECT_EQ(P30_C05.mChildIt, cIt);
    EXPECT_EQ(P30_C05.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L105.backIt(), *cIt);
    EXPECT_EQ(L105.mChildIt, cIt);
    EXPECT_EQ(L105.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L106.backIt(), *cIt);
    EXPECT_EQ(L106.mChildIt, cIt);
    EXPECT_EQ(L106.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L107.backIt(), *cIt);
    EXPECT_EQ(L107.mChildIt, cIt);
    EXPECT_EQ(L107.mParent, P30.backIt());
    cIt++;
    EXPECT_EQ(L108.backIt(), *cIt);
    EXPECT_EQ(L108.mChildIt, cIt);
    EXPECT_EQ(L108.mParent, P30.backIt());

    EXPECT_EQ(P30.mNodeType, JTSTreeNode::QType);
    EXPECT_EQ(P30.mPertinentChildrenCount, 5);
    EXPECT_EQ(P30.mFullChildrenCount, 5);
    EXPECT_EQ(P30.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P30.mPartialChild2, pqTree.nil());

    cIt = P30_C05.mChildren.begin();
    EXPECT_EQ(L103.backIt(), *cIt);
    EXPECT_EQ(L103.mChildIt, cIt);
    EXPECT_EQ(L103.mParent, P30_C05.backIt());
    cIt++;
    EXPECT_EQ(L104.backIt(), *cIt);
    EXPECT_EQ(L104.mChildIt, cIt);
    EXPECT_EQ(L104.mParent, P30_C05.backIt());

    EXPECT_EQ(P30_C05.mFullChildrenCount, 2);
    EXPECT_EQ(P30_C05.mPartialChild1, pqTree.nil());
    EXPECT_EQ(P30_C05.mPartialChild2, pqTree.nil());
    EXPECT_EQ(P30_C05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P30_C05.mNodeType, JTSTreeNode::PType);

}

/**  @brief tests JTSTree::templateQ1&Q2()
 */
TEST_F(JTSPlanarizerTests, Test19) {


    JTSTree& pqTree = getPQTree();

    //     Q1 positive
    //
    //      Q01
    //       |
    //      Q02
    //    /  |  \
    // *L01*L02*L03

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

    list<node_list_it_t> Q02children;
    Q02children.push_back(L01.backIt());
    Q02children.push_back(L02.backIt());
    Q02children.push_back(L03.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    Q02.mPertinentChildrenCount = 3;
    Q02.mFullChildrenCount = 3;

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ1(pqTree, Q02), true);
    EXPECT_EQ(Q02.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(Q01.mFullChildrenCount, 1);


    //     Q2 positive #1
    //
    //                Q03                                   Q03
    //                 |                                     |
    //    ____________Q04___________           _____________Q04______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    // *L08*L09   ____Q05____  L10 L11      *L08 *L09 *L07 *L06 L05 L04 L10 L11
    //           |   |   |   |   
    //          L04 L05*L06*L07
    initializeForOneIteration(pqTree);
    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    L06.mGeneration    = pqTree.mGeneration;
    L06.mPertinentType = JTSTreeNode::Full;
    auto& L07 = createL(pqTree);
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L04.backIt());
    Q05children.push_back(L05.backIt());
    Q05children.push_back(L06.backIt());
    Q05children.push_back(L07.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = JTSTreeNode::Partial;
    Q05.mPertinentChildrenCount = 2;
    Q05.mFullChildrenCount = 2;

    list<node_list_it_t> Q04children;
    Q04children.push_back(L08.backIt());
    Q04children.push_back(L09.backIt());
    Q04children.push_back(Q05.backIt());
    Q04children.push_back(L10.backIt());
    Q04children.push_back(L11.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentChildrenCount = 3;
    Q04.mFullChildrenCount = 2;
    Q04.mPartialChild1 = Q05.backIt();

    list<node_list_it_t> Q03children;
    Q03children.push_back(Q04.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q04), true);
    EXPECT_EQ(Q04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mFullChildrenCount, 0);
    EXPECT_EQ(Q03.mPartialChild1, Q04.backIt());

    EXPECT_EQ(Q04.mChildren.size(), 8);
    EXPECT_EQ(Q04.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q04.mFullChildrenCount, 4);
    EXPECT_EQ(Q04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q04.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q04.mPartialChild2, pqTree.nil());
    auto cIt = Q04.mChildren.begin();
    EXPECT_EQ(L08.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L09.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L07.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L06.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L05.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L04.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L10.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L11.backIt(), *cIt);


    //     Q2 positive #2
    //
    //                Q06                                   Q06
    //                 |                                     |
    //    ____________Q07___________           _____________Q07______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    //  L16 L17   ____Q08____ *L18*L19       L16  L17  L12  L13*L14*L15*L18*L19
    //           |   |   |   |   
    //          L12 L13*L14*L15
    initializeForOneIteration(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);
    L14.mGeneration    = pqTree.mGeneration;
    L14.mPertinentType = JTSTreeNode::Full;
    auto& L15 = createL(pqTree);
    L15.mGeneration    = pqTree.mGeneration;
    L15.mPertinentType = JTSTreeNode::Full;
    auto& L16 = createL(pqTree);
    auto& L17 = createL(pqTree);
    auto& L18 = createL(pqTree);
    L18.mGeneration    = pqTree.mGeneration;
    L18.mPertinentType = JTSTreeNode::Full;
    auto& L19 = createL(pqTree);
    L19.mGeneration    = pqTree.mGeneration;
    L19.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q08children;
    Q08children.push_back(L12.backIt());
    Q08children.push_back(L13.backIt());
    Q08children.push_back(L14.backIt());
    Q08children.push_back(L15.backIt());
    auto& Q08 = createQ(pqTree, Q08children);
    Q08.mGeneration = pqTree.mGeneration;
    Q08.mPertinentType = JTSTreeNode::Partial;
    Q08.mPertinentChildrenCount = 2;
    Q08.mFullChildrenCount = 2;

    list<node_list_it_t> Q07children;
    Q07children.push_back(L16.backIt());
    Q07children.push_back(L17.backIt());
    Q07children.push_back(Q08.backIt());
    Q07children.push_back(L18.backIt());
    Q07children.push_back(L19.backIt());
    auto& Q07 = createQ(pqTree, Q07children);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentChildrenCount = 3;
    Q07.mFullChildrenCount = 2;
    Q07.mPartialChild1 = Q08.backIt();

    list<node_list_it_t> Q06children;
    Q06children.push_back(Q07.backIt());
    auto& Q06 = createQ(pqTree, Q06children);
    Q06.mGeneration = pqTree.mGeneration;
    Q06.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q07), true);
    EXPECT_EQ(Q07.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q06.mFullChildrenCount, 0);
    EXPECT_EQ(Q06.mPartialChild1, Q07.backIt());

    EXPECT_EQ(Q07.mChildren.size(), 8);
    EXPECT_EQ(Q07.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q07.mFullChildrenCount, 4);
    EXPECT_EQ(Q07.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q07.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q07.mPartialChild2, pqTree.nil());
    cIt = Q07.mChildren.begin();
    EXPECT_EQ(L16.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L17.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L12.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L13.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L14.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L15.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L18.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L19.backIt(), *cIt);


    //     Q2 positive #3
    //
    //                Q09                                   Q09
    //                 |                                     |
    //    ____________Q10___________           _____________Q10______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    //  L24 L25   ____Q11____ *L26*L27       L24  L25  L23 L22 *L21*L20*L26*L27
    //           |   |   |   |   
    //         *L20*L21 L22 L23
    initializeForOneIteration(pqTree);
    auto& L20 = createL(pqTree);
    L20.mGeneration    = pqTree.mGeneration;
    L20.mPertinentType = JTSTreeNode::Full;
    auto& L21 = createL(pqTree);
    L21.mGeneration    = pqTree.mGeneration;
    L21.mPertinentType = JTSTreeNode::Full;
    auto& L22 = createL(pqTree);
    auto& L23 = createL(pqTree);
    auto& L24 = createL(pqTree);
    auto& L25 = createL(pqTree);
    auto& L26 = createL(pqTree);
    L26.mGeneration    = pqTree.mGeneration;
    L26.mPertinentType = JTSTreeNode::Full;
    auto& L27 = createL(pqTree);
    L27.mGeneration    = pqTree.mGeneration;
    L27.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q11children;
    Q11children.push_back(L20.backIt());
    Q11children.push_back(L21.backIt());
    Q11children.push_back(L22.backIt());
    Q11children.push_back(L23.backIt());
    auto& Q11 = createQ(pqTree, Q11children);
    Q11.mGeneration = pqTree.mGeneration;
    Q11.mPertinentType = JTSTreeNode::Partial;
    Q11.mPertinentChildrenCount = 2;
    Q11.mFullChildrenCount = 2;

    list<node_list_it_t> Q10children;
    Q10children.push_back(L24.backIt());
    Q10children.push_back(L25.backIt());
    Q10children.push_back(Q11.backIt());
    Q10children.push_back(L26.backIt());
    Q10children.push_back(L27.backIt());
    auto& Q10 = createQ(pqTree, Q10children);
    Q10.mGeneration = pqTree.mGeneration;
    Q10.mPertinentChildrenCount = 3;
    Q10.mFullChildrenCount = 2;
    Q10.mPartialChild1 = Q11.backIt();

    list<node_list_it_t> Q09children;
    Q09children.push_back(Q10.backIt());
    auto& Q09 = createQ(pqTree, Q09children);
    Q09.mGeneration = pqTree.mGeneration;
    Q09.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q10), true);
    EXPECT_EQ(Q10.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q09.mFullChildrenCount, 0);
    EXPECT_EQ(Q09.mPartialChild1, Q10.backIt());

    EXPECT_EQ(Q10.mChildren.size(), 8);
    EXPECT_EQ(Q10.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q10.mFullChildrenCount, 4);
    EXPECT_EQ(Q10.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q10.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q10.mPartialChild2, pqTree.nil());
    cIt = Q10.mChildren.begin();
    EXPECT_EQ(L24.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L25.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L23.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L22.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L21.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L20.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L26.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L27.backIt(), *cIt);


    //     Q2 positive #4
    //
    //                Q12                                   Q12
    //                 |                                     |
    //    ____________Q13___________           _____________Q13______________
    //   |   |         |        |   |   =>    |    |    |    |   |   |   |   |
    // *L32*L33   ____Q14____  L34 L35      *L32 *L33 *L28 *L29 L30 L31 L34 L35
    //           |   |   |   |   
    //         *L28*L29 L30 L31
    initializeForOneIteration(pqTree);
    auto& L28 = createL(pqTree);
    L28.mGeneration    = pqTree.mGeneration;
    L28.mPertinentType = JTSTreeNode::Full;
    auto& L29 = createL(pqTree);
    L29.mGeneration    = pqTree.mGeneration;
    L29.mPertinentType = JTSTreeNode::Full;
    auto& L30 = createL(pqTree);
    auto& L31 = createL(pqTree);
    auto& L32 = createL(pqTree);
    L32.mGeneration    = pqTree.mGeneration;
    L32.mPertinentType = JTSTreeNode::Full;
    auto& L33 = createL(pqTree);
    L33.mGeneration    = pqTree.mGeneration;
    L33.mPertinentType = JTSTreeNode::Full;
    auto& L34 = createL(pqTree);
    auto& L35 = createL(pqTree);

    list<node_list_it_t> Q14children;
    Q14children.push_back(L28.backIt());
    Q14children.push_back(L29.backIt());
    Q14children.push_back(L30.backIt());
    Q14children.push_back(L31.backIt());
    auto& Q14 = createQ(pqTree, Q14children);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = JTSTreeNode::Partial;
    Q14.mPertinentChildrenCount = 2;
    Q14.mFullChildrenCount = 2;

    list<node_list_it_t> Q13children;
    Q13children.push_back(L32.backIt());
    Q13children.push_back(L33.backIt());
    Q13children.push_back(Q14.backIt());
    Q13children.push_back(L34.backIt());
    Q13children.push_back(L35.backIt());
    auto& Q13 = createQ(pqTree, Q13children);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentChildrenCount = 3;
    Q13.mFullChildrenCount = 2;
    Q13.mPartialChild1 = Q14.backIt();

    list<node_list_it_t> Q12children;
    Q12children.push_back(Q13.backIt());
    auto& Q12 = createQ(pqTree, Q12children);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q13), true);
    EXPECT_EQ(Q13.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q12.mFullChildrenCount, 0);
    EXPECT_EQ(Q12.mPartialChild1, Q13.backIt());

    EXPECT_EQ(Q13.mChildren.size(), 8);
    EXPECT_EQ(Q13.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q13.mFullChildrenCount, 4);
    EXPECT_EQ(Q13.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q13.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q13.mPartialChild2, pqTree.nil());
    cIt = Q13.mChildren.begin();
    EXPECT_EQ(L32.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L33.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L28.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L29.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L30.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L31.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L34.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L35.backIt(), *cIt);


    //     Q2 positive #5
    //
    //                Q15                                   Q15
    //                 |                                     |
    //                Q16___________           _____________Q16______
    //                 |        |   |   =>    |    |    |    |   |   |
    //            ____Q17____  L40 L41      *L36 *L37  L38  L39 L40 L41
    //           |   |   |   |   
    //         *L36*L37 L38 L39
    initializeForOneIteration(pqTree);
    auto& L36 = createL(pqTree);
    L36.mGeneration    = pqTree.mGeneration;
    L36.mPertinentType = JTSTreeNode::Full;
    auto& L37 = createL(pqTree);
    L37.mGeneration    = pqTree.mGeneration;
    L37.mPertinentType = JTSTreeNode::Full;
    auto& L38 = createL(pqTree);
    auto& L39 = createL(pqTree);
    auto& L40 = createL(pqTree);
    auto& L41 = createL(pqTree);

    list<node_list_it_t> Q17children;
    Q17children.push_back(L36.backIt());
    Q17children.push_back(L37.backIt());
    Q17children.push_back(L38.backIt());
    Q17children.push_back(L39.backIt());
    auto& Q17 = createQ(pqTree, Q17children);
    Q17.mGeneration = pqTree.mGeneration;
    Q17.mPertinentType = JTSTreeNode::Partial;
    Q17.mPertinentChildrenCount = 2;
    Q17.mFullChildrenCount = 2;

    list<node_list_it_t> Q16children;
    Q16children.push_back(Q17.backIt());
    Q16children.push_back(L40.backIt());
    Q16children.push_back(L41.backIt());
    auto& Q16 = createQ(pqTree, Q16children);
    Q16.mGeneration = pqTree.mGeneration;
    Q16.mPertinentChildrenCount = 1;
    Q16.mFullChildrenCount = 0;
    Q16.mPartialChild1 = Q17.backIt();

    list<node_list_it_t> Q15children;
    Q15children.push_back(Q16.backIt());
    auto& Q15 = createQ(pqTree, Q15children);
    Q15.mGeneration = pqTree.mGeneration;
    Q15.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q16), true);
    EXPECT_EQ(Q16.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q15.mFullChildrenCount, 0);
    EXPECT_EQ(Q15.mPartialChild1, Q16.backIt());

    EXPECT_EQ(Q16.mChildren.size(), 6);
    EXPECT_EQ(Q16.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q16.mFullChildrenCount, 2);
    EXPECT_EQ(Q16.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q16.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q16.mPartialChild2, pqTree.nil());
    cIt = Q16.mChildren.begin();
    EXPECT_EQ(L36.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L37.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L38.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L39.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L40.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L41.backIt(), *cIt);


    //     Q2 positive #6
    //
    //                Q18                                   Q18
    //                 |                                     |
    //     ___________Q19                      _____________Q19______
    //    |   |        |                =>    |    |    |    |   |   |
    //   L46 L47  ____Q20____                L46  L47  L45  L44*L43*L42
    //           |   |   |   |   
    //         *L42*L43 L44 L45
    initializeForOneIteration(pqTree);
    auto& L42 = createL(pqTree);
    L42.mGeneration    = pqTree.mGeneration;
    L42.mPertinentType = JTSTreeNode::Full;
    auto& L43 = createL(pqTree);
    L43.mGeneration    = pqTree.mGeneration;
    L43.mPertinentType = JTSTreeNode::Full;
    auto& L44 = createL(pqTree);
    auto& L45 = createL(pqTree);
    auto& L46 = createL(pqTree);
    auto& L47 = createL(pqTree);

    list<node_list_it_t> Q20children;
    Q20children.push_back(L42.backIt());
    Q20children.push_back(L43.backIt());
    Q20children.push_back(L44.backIt());
    Q20children.push_back(L45.backIt());
    auto& Q20 = createQ(pqTree, Q20children);
    Q20.mGeneration = pqTree.mGeneration;
    Q20.mPertinentType = JTSTreeNode::Partial;
    Q20.mPertinentChildrenCount = 2;
    Q20.mFullChildrenCount = 2;

    list<node_list_it_t> Q19children;
    Q19children.push_back(L46.backIt());
    Q19children.push_back(L47.backIt());
    Q19children.push_back(Q20.backIt());
    auto& Q19 = createQ(pqTree, Q19children);
    Q19.mGeneration = pqTree.mGeneration;
    Q19.mPertinentChildrenCount = 1;
    Q19.mFullChildrenCount = 0;
    Q19.mPartialChild1 = Q20.backIt();

    list<node_list_it_t> Q18children;
    Q18children.push_back(Q19.backIt());
    auto& Q18 = createQ(pqTree, Q18children);
    Q18.mGeneration = pqTree.mGeneration;
    Q18.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q19), true);
    EXPECT_EQ(Q19.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q18.mFullChildrenCount, 0);
    EXPECT_EQ(Q18.mPartialChild1, Q19.backIt());

    EXPECT_EQ(Q19.mChildren.size(), 6);
    EXPECT_EQ(Q19.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q19.mFullChildrenCount, 2);
    EXPECT_EQ(Q19.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q19.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q19.mPartialChild2, pqTree.nil());
    cIt = Q19.mChildren.begin();
    EXPECT_EQ(L46.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L47.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L45.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L44.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L43.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L42.backIt(), *cIt);


    //     Q2 positive #7
    //
    //                Q21                                   Q21
    //                 |                                     |
    //                Q22___________           _____________Q22______
    //                 |        |   |   =>    |    |    |    |   |   |
    //            ____Q23____  L52 L53      *L51 *L50  L49  L48 L52 L53
    //           |   |   |   |   
    //          L48 L49*L50*L51
    initializeForOneIteration(pqTree);
    auto& L48 = createL(pqTree);
    auto& L49 = createL(pqTree);
    auto& L50 = createL(pqTree);
    L50.mGeneration    = pqTree.mGeneration;
    L50.mPertinentType = JTSTreeNode::Full;
    auto& L51 = createL(pqTree);
    L51.mGeneration    = pqTree.mGeneration;
    L51.mPertinentType = JTSTreeNode::Full;
    auto& L52 = createL(pqTree);
    auto& L53 = createL(pqTree);

    list<node_list_it_t> Q23children;
    Q23children.push_back(L48.backIt());
    Q23children.push_back(L49.backIt());
    Q23children.push_back(L50.backIt());
    Q23children.push_back(L51.backIt());
    auto& Q23 = createQ(pqTree, Q23children);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Partial;
    Q23.mPertinentChildrenCount = 2;
    Q23.mFullChildrenCount = 2;

    list<node_list_it_t> Q22children;
    Q22children.push_back(Q23.backIt());
    Q22children.push_back(L52.backIt());
    Q22children.push_back(L53.backIt());
    auto& Q22 = createQ(pqTree, Q22children);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentChildrenCount = 1;
    Q22.mFullChildrenCount = 0;
    Q22.mPartialChild1 = Q23.backIt();

    list<node_list_it_t> Q21children;
    Q21children.push_back(Q22.backIt());
    auto& Q21 = createQ(pqTree, Q21children);
    Q21.mGeneration = pqTree.mGeneration;
    Q21.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q22), true);
    EXPECT_EQ(Q22.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q21.mFullChildrenCount, 0);
    EXPECT_EQ(Q21.mPartialChild1, Q22.backIt());

    EXPECT_EQ(Q22.mChildren.size(), 6);
    EXPECT_EQ(Q22.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q22.mFullChildrenCount, 2);
    EXPECT_EQ(Q22.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q22.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q22.mPartialChild2, pqTree.nil());
    cIt = Q22.mChildren.begin();
    EXPECT_EQ(L51.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L50.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L49.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L48.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L52.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L53.backIt(), *cIt);


    //     Q2 positive #8
    //
    //                Q24                                   Q24
    //                 |                                     |
    //     ___________Q25                      _____________Q25______
    //    |   |        |                =>    |    |    |    |   |   |
    //   L58 L59  ____Q26____                L58  L59  L54  L55*L56*L57
    //           |   |   |   |   
    //          L54 L55*L56*L57
    initializeForOneIteration(pqTree);
    auto& L54 = createL(pqTree);
    auto& L55 = createL(pqTree);
    auto& L56 = createL(pqTree);
    L56.mGeneration    = pqTree.mGeneration;
    L56.mPertinentType = JTSTreeNode::Full;
    auto& L57 = createL(pqTree);
    L57.mGeneration    = pqTree.mGeneration;
    L57.mPertinentType = JTSTreeNode::Full;
    auto& L58 = createL(pqTree);
    auto& L59 = createL(pqTree);

    list<node_list_it_t> Q26children;
    Q26children.push_back(L54.backIt());
    Q26children.push_back(L55.backIt());
    Q26children.push_back(L56.backIt());
    Q26children.push_back(L57.backIt());
    auto& Q26 = createQ(pqTree, Q26children);
    Q26.mGeneration = pqTree.mGeneration;
    Q26.mPertinentType = JTSTreeNode::Partial;
    Q26.mPertinentChildrenCount = 2;
    Q26.mFullChildrenCount = 2;

    list<node_list_it_t> Q25children;
    Q25children.push_back(L58.backIt());
    Q25children.push_back(L59.backIt());
    Q25children.push_back(Q26.backIt());
    auto& Q25 = createQ(pqTree, Q25children);
    Q25.mGeneration = pqTree.mGeneration;
    Q25.mPertinentChildrenCount = 1;
    Q25.mFullChildrenCount = 0;
    Q25.mPartialChild1 = Q26.backIt();

    list<node_list_it_t> Q24children;
    Q24children.push_back(Q25.backIt());
    auto& Q24 = createQ(pqTree, Q24children);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q25), true);
    EXPECT_EQ(Q25.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q24.mFullChildrenCount, 0);
    EXPECT_EQ(Q24.mPartialChild1, Q25.backIt());

    EXPECT_EQ(Q25.mChildren.size(), 6);
    EXPECT_EQ(Q25.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q25.mFullChildrenCount, 2);
    EXPECT_EQ(Q25.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q25.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q25.mPartialChild2, pqTree.nil());
    cIt = Q25.mChildren.begin();
    EXPECT_EQ(L58.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L59.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L54.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L55.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L56.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L57.backIt(), *cIt);


    //     Q2 positive #9
    //
    //                Q27                                   Q27
    //                 |                                     |
    //                Q28___________           _____________Q28_______
    //                 |        |   |   =>    |    |    |    |    |   |
    //            ____Q29____ *L64*L65       L63  L62 *L61 *L60 *L64 *L65
    //           |   |   |   |   
    //         *L60*L61 L62 L63
    initializeForOneIteration(pqTree);
    auto& L60 = createL(pqTree);
    L60.mGeneration    = pqTree.mGeneration;
    L60.mPertinentType = JTSTreeNode::Full;
    auto& L61 = createL(pqTree);
    L61.mGeneration    = pqTree.mGeneration;
    L61.mPertinentType = JTSTreeNode::Full;
    auto& L62 = createL(pqTree);
    auto& L63 = createL(pqTree);
    auto& L64 = createL(pqTree);
    L64.mGeneration    = pqTree.mGeneration;
    L64.mPertinentType = JTSTreeNode::Full;
    auto& L65 = createL(pqTree);
    L65.mGeneration    = pqTree.mGeneration;
    L65.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q29children;
    Q29children.push_back(L60.backIt());
    Q29children.push_back(L61.backIt());
    Q29children.push_back(L62.backIt());
    Q29children.push_back(L63.backIt());
    auto& Q29 = createQ(pqTree, Q29children);
    Q29.mGeneration = pqTree.mGeneration;
    Q29.mPertinentType = JTSTreeNode::Partial;
    Q29.mPertinentChildrenCount = 2;
    Q29.mFullChildrenCount = 2;

    list<node_list_it_t> Q28children;
    Q28children.push_back(Q29.backIt());
    Q28children.push_back(L64.backIt());
    Q28children.push_back(L65.backIt());
    auto& Q28 = createQ(pqTree, Q28children);
    Q28.mGeneration = pqTree.mGeneration;
    Q28.mPertinentChildrenCount = 3;
    Q28.mFullChildrenCount = 2;
    Q28.mPartialChild1 = Q29.backIt();

    list<node_list_it_t> Q27children;
    Q27children.push_back(Q28.backIt());
    auto& Q27 = createQ(pqTree, Q27children);
    Q27.mGeneration = pqTree.mGeneration;
    Q27.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q28), true);
    EXPECT_EQ(Q28.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q27.mFullChildrenCount, 0);
    EXPECT_EQ(Q27.mPartialChild1, Q28.backIt());

    EXPECT_EQ(Q28.mChildren.size(), 6);
    EXPECT_EQ(Q28.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q28.mFullChildrenCount, 4);
    EXPECT_EQ(Q28.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q28.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q28.mPartialChild2, pqTree.nil());
    cIt = Q28.mChildren.begin();
    EXPECT_EQ(L63.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L62.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L61.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L60.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L64.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L65.backIt(), *cIt);


    //     Q2 positive #10
    //
    //                Q30                           Q30
    //                 |                             |
    //     ___________Q31              _____________Q31_______
    //    |    |       |        =>    |    |    |    |    |   |
    //  *L70 *L71 ____Q32____       *L70 *L71 *L66 *L67  L68  L69
    //           |   |   |   |   
    //         *L66*L67 L68 L69
    initializeForOneIteration(pqTree);
    auto& L66 = createL(pqTree);
    L66.mGeneration    = pqTree.mGeneration;
    L66.mPertinentType = JTSTreeNode::Full;
    auto& L67 = createL(pqTree);
    L67.mGeneration    = pqTree.mGeneration;
    L67.mPertinentType = JTSTreeNode::Full;
    auto& L68 = createL(pqTree);
    auto& L69 = createL(pqTree);
    auto& L70 = createL(pqTree);
    L70.mGeneration    = pqTree.mGeneration;
    L70.mPertinentType = JTSTreeNode::Full;
    auto& L71 = createL(pqTree);
    L71.mGeneration    = pqTree.mGeneration;
    L71.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q32children;
    Q32children.push_back(L66.backIt());
    Q32children.push_back(L67.backIt());
    Q32children.push_back(L68.backIt());
    Q32children.push_back(L69.backIt());
    auto& Q32 = createQ(pqTree, Q32children);
    Q32.mGeneration = pqTree.mGeneration;
    Q32.mPertinentType = JTSTreeNode::Partial;
    Q32.mPertinentChildrenCount = 2;
    Q32.mFullChildrenCount = 2;

    list<node_list_it_t> Q31children;
    Q31children.push_back(L70.backIt());
    Q31children.push_back(L71.backIt());
    Q31children.push_back(Q32.backIt());
    auto& Q31 = createQ(pqTree, Q31children);
    Q31.mGeneration = pqTree.mGeneration;
    Q31.mPertinentChildrenCount = 3;
    Q31.mFullChildrenCount = 2;
    Q31.mPartialChild1 = Q32.backIt();

    list<node_list_it_t> Q30children;
    Q30children.push_back(Q31.backIt());
    auto& Q30 = createQ(pqTree, Q30children);
    Q30.mGeneration = pqTree.mGeneration;
    Q30.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q31), true);
    EXPECT_EQ(Q31.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q30.mFullChildrenCount, 0);
    EXPECT_EQ(Q30.mPartialChild1, Q31.backIt());

    EXPECT_EQ(Q31.mChildren.size(), 6);
    EXPECT_EQ(Q31.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q31.mFullChildrenCount, 4);
    EXPECT_EQ(Q31.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q31.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q31.mPartialChild2, pqTree.nil());
    cIt = Q31.mChildren.begin();
    EXPECT_EQ(L70.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L71.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L66.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L67.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L68.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L69.backIt(), *cIt);

    //     Q2 positive #11
    //
    //                Q33                                   Q33
    //                 |                                     |
    //                Q34___________           _____________Q34________
    //                 |        |   |   =>    |    |    |    |    |    |
    //            ____Q35____ *L76*L77       L72  L73 *L74 *L75 *L76 *L77
    //           |   |   |   |   
    //          L72 L73*L74*L75
    initializeForOneIteration(pqTree);
    auto& L72 = createL(pqTree);
    auto& L73 = createL(pqTree);
    auto& L74 = createL(pqTree);
    L74.mGeneration    = pqTree.mGeneration;
    L74.mPertinentType = JTSTreeNode::Full;
    auto& L75 = createL(pqTree);
    L75.mGeneration    = pqTree.mGeneration;
    L75.mPertinentType = JTSTreeNode::Full;
    auto& L76 = createL(pqTree);
    L76.mGeneration    = pqTree.mGeneration;
    L76.mPertinentType = JTSTreeNode::Full;
    auto& L77 = createL(pqTree);
    L77.mGeneration    = pqTree.mGeneration;
    L77.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q35children;
    Q35children.push_back(L72.backIt());
    Q35children.push_back(L73.backIt());
    Q35children.push_back(L74.backIt());
    Q35children.push_back(L75.backIt());
    auto& Q35 = createQ(pqTree, Q35children);
    Q35.mGeneration = pqTree.mGeneration;
    Q35.mPertinentType = JTSTreeNode::Partial;
    Q35.mPertinentChildrenCount = 2;
    Q35.mFullChildrenCount = 2;

    list<node_list_it_t> Q34children;
    Q34children.push_back(Q35.backIt());
    Q34children.push_back(L76.backIt());
    Q34children.push_back(L77.backIt());
    auto& Q34 = createQ(pqTree, Q34children);
    Q34.mGeneration = pqTree.mGeneration;
    Q34.mPertinentChildrenCount = 3;
    Q34.mFullChildrenCount = 2;
    Q34.mPartialChild1 = Q35.backIt();

    list<node_list_it_t> Q33children;
    Q33children.push_back(Q34.backIt());
    auto& Q33 = createQ(pqTree, Q33children);
    Q33.mGeneration = pqTree.mGeneration;
    Q33.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q34), true);
    EXPECT_EQ(Q34.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q33.mFullChildrenCount, 0);
    EXPECT_EQ(Q33.mPartialChild1, Q34.backIt());

    EXPECT_EQ(Q34.mChildren.size(), 6);
    EXPECT_EQ(Q34.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q34.mFullChildrenCount, 4);
    EXPECT_EQ(Q34.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q34.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q34.mPartialChild2, pqTree.nil());
    cIt = Q34.mChildren.begin();
    EXPECT_EQ(L72.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L73.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L74.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L75.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L76.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L77.backIt(), *cIt);


    //     Q2 positive #12
    //
    //                Q36                                   Q36
    //                 |                                     |
    //                Q37___________           _____________Q37_______
    //                 |        |   |   =>    |    |    |    |    |   |
    //            ____Q38____ *L82*L83       L78  L79 *L80 *L81 *L82 *L83
    //           |   |   |   |   
    //          L78 L79*L80*L81
    initializeForOneIteration(pqTree);
    auto& L78 = createL(pqTree);
    auto& L79 = createL(pqTree);
    auto& L80 = createL(pqTree);
    L80.mGeneration    = pqTree.mGeneration;
    L80.mPertinentType = JTSTreeNode::Full;
    auto& L81 = createL(pqTree);
    L81.mGeneration    = pqTree.mGeneration;
    L81.mPertinentType = JTSTreeNode::Full;
    auto& L82 = createL(pqTree);
    L82.mGeneration    = pqTree.mGeneration;
    L82.mPertinentType = JTSTreeNode::Full;
    auto& L83 = createL(pqTree);
    L83.mGeneration    = pqTree.mGeneration;
    L83.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q38children;
    Q38children.push_back(L78.backIt());
    Q38children.push_back(L79.backIt());
    Q38children.push_back(L80.backIt());
    Q38children.push_back(L81.backIt());
    auto& Q38 = createQ(pqTree, Q38children);
    Q38.mGeneration = pqTree.mGeneration;
    Q38.mPertinentType = JTSTreeNode::Partial;
    Q38.mPertinentChildrenCount = 2;
    Q38.mFullChildrenCount = 2;

    list<node_list_it_t> Q37children;
    Q37children.push_back(Q38.backIt());
    Q37children.push_back(L82.backIt());
    Q37children.push_back(L83.backIt());
    auto& Q37 = createQ(pqTree, Q37children);
    Q37.mGeneration = pqTree.mGeneration;
    Q37.mPertinentChildrenCount = 3;
    Q37.mFullChildrenCount = 2;
    Q37.mPartialChild1 = Q38.backIt();

    list<node_list_it_t> Q36children;
    Q36children.push_back(Q37.backIt());
    auto& Q36 = createQ(pqTree, Q36children);
    Q36.mGeneration = pqTree.mGeneration;
    Q36.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q37), true);
    EXPECT_EQ(Q37.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q36.mFullChildrenCount, 0);
    EXPECT_EQ(Q36.mPartialChild1, Q37.backIt());

    EXPECT_EQ(Q37.mChildren.size(), 6);
    EXPECT_EQ(Q37.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q37.mFullChildrenCount, 4);
    EXPECT_EQ(Q37.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q37.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q37.mPartialChild2, pqTree.nil());
    cIt = Q37.mChildren.begin();
    EXPECT_EQ(L78.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L79.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L80.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L81.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L82.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L83.backIt(), *cIt);

    //     Q2 positive #13
    //
    //            Q39                      Q39
    //             |                        |
    //       _____Q40____              ____Q40_____
    //      |    |   |   |  =>        |    |   |   |
    //    *L84 *L85 L86 L87         *L84 *L85 L86 L87
    initializeForOneIteration(pqTree);
    auto& L84 = createL(pqTree);
    L84.mGeneration    = pqTree.mGeneration;
    L84.mPertinentType = JTSTreeNode::Full;
    auto& L85 = createL(pqTree);
    L85.mGeneration    = pqTree.mGeneration;
    L85.mPertinentType = JTSTreeNode::Full;
    auto& L86 = createL(pqTree);
    auto& L87 = createL(pqTree);

    list<node_list_it_t> Q40children;
    Q40children.push_back(L84.backIt());
    Q40children.push_back(L85.backIt());
    Q40children.push_back(L86.backIt());
    Q40children.push_back(L87.backIt());
    auto& Q40 = createQ(pqTree, Q40children);
    Q40.mGeneration = pqTree.mGeneration;
    Q40.mPertinentChildrenCount = 2;
    Q40.mFullChildrenCount = 2;

    list<node_list_it_t> Q39children;
    Q39children.push_back(Q40.backIt());
    auto& Q39 = createQ(pqTree, Q39children);
    Q39.mGeneration = pqTree.mGeneration;
    Q39.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q40), true);
    EXPECT_EQ(Q40.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q39.mFullChildrenCount, 0);
    EXPECT_EQ(Q39.mPartialChild1, Q40.backIt());

    EXPECT_EQ(Q40.mChildren.size(), 4);
    EXPECT_EQ(Q40.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q40.mFullChildrenCount, 2);
    EXPECT_EQ(Q40.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q40.mPartialChild2, pqTree.nil());
    cIt = Q40.mChildren.begin();
    EXPECT_EQ(L84.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L85.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L86.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L87.backIt(), *cIt);


    //     Q2 positive #14
    //
    //            Q41                      Q41
    //             |                        |
    //       _____Q42____              ____Q42_____
    //      |    |   |   |  =>        |    |   |   |
    //     L88  L89*L90*L91          L88  L89*L90*L91
    initializeForOneIteration(pqTree);
    auto& L88 = createL(pqTree);
    auto& L89 = createL(pqTree);
    auto& L90 = createL(pqTree);
    L90.mGeneration    = pqTree.mGeneration;
    L90.mPertinentType = JTSTreeNode::Full;
    auto& L91 = createL(pqTree);
    L91.mGeneration    = pqTree.mGeneration;
    L91.mPertinentType = JTSTreeNode::Full;

    list<node_list_it_t> Q42children;
    Q42children.push_back(L88.backIt());
    Q42children.push_back(L89.backIt());
    Q42children.push_back(L90.backIt());
    Q42children.push_back(L91.backIt());
    auto& Q42 = createQ(pqTree, Q42children);
    Q42.mGeneration = pqTree.mGeneration;
    Q42.mPertinentChildrenCount = 2;
    Q42.mFullChildrenCount = 2;

    list<node_list_it_t> Q41children;
    Q41children.push_back(Q42.backIt());
    auto& Q41 = createQ(pqTree, Q41children);
    Q41.mGeneration = pqTree.mGeneration;
    Q41.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ2(pqTree, Q42), true);
    EXPECT_EQ(Q42.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q41.mFullChildrenCount, 0);
    EXPECT_EQ(Q41.mPartialChild1, Q42.backIt());

    EXPECT_EQ(Q42.mChildren.size(), 4);
    EXPECT_EQ(Q42.mPertinentChildrenCount, 2);
    EXPECT_EQ(Q42.mFullChildrenCount, 2);
    EXPECT_EQ(Q42.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q42.mPartialChild2, pqTree.nil());
    cIt = Q42.mChildren.begin();
    EXPECT_EQ(L88.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L89.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L90.backIt(), *cIt);
    cIt++;
    EXPECT_EQ(L91.backIt(), *cIt);

}


/**  @brief tests JTSTree::templateQ3() positive #1 EEPFFPEE
 */
TEST_F(JTSPlanarizerTests, Test20) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
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
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L07 *L08 *L09 *L10  L11  L12  L13  L14
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
    auto& L07 = createL(pqTree);
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}



/**  @brief tests JTSTree::templateQ3() positive #1 EEPFFPEE
 */
TEST_F(JTSPlanarizerTests, Test21) {


    JTSTree& pqTree = getPQTree();

    //     Q3
    //
    //                                   Q01
    //                                    |
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
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03 *L07 *L08 *L12 *L11  L10  L09  L13  L14
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
    auto& L07 = createL(pqTree);
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}



/**  @brief tests JTSTree::templateQ3() positive #1 EEPFFPEE
 */
TEST_F(JTSPlanarizerTests, Test22) {


    JTSTree& pqTree = getPQTree();

    //     Q3
    //
    //                                   Q01
    //                                    |
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
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L07 *L08 *L09 *L10  L11  L12  L13  L14
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
    auto& L07 = createL(pqTree);
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 EEPFFPEE
 */
TEST_F(JTSPlanarizerTests, Test23) {


    JTSTree& pqTree = getPQTree();

    //     Q3
    //
    //                                   Q01
    //                                    |
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
    //                                    |
    //       ____________________________Q02_________________________________
    //      |    |    |    |    |    |    |    |    |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06 *L07 *L08 *L12 *L11  L10  L09  L13  L14
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
    auto& L07 = createL(pqTree);
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 14);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}



/**  @brief tests JTSTree::templateQ3() positive EEPFPEE
 */
TEST_F(JTSPlanarizerTests, Test24) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                |
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |    
    //             *L03 *L04  L05  L06*L09 *L10  L11  L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                 Q01
    //                                  |
    //       __________________________Q02___________________________
    //      |    |    |    |    |    |      |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L037  *L09 *L10  L11  L12  L13  L14
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
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}



/**  @brief tests JTSTree::templateQ3() positive EEPFPEE
 */
TEST_F(JTSPlanarizerTests, Test25) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                |
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |    
    //             *L03 *L04  L05  L06 L09  L10 *L11 *L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                 Q01
    //                                  |
    //       __________________________Q02___________________________
    //      |    |    |    |    |    |      |    |    |    |    |    |
    //     L01  L02  L06  L05 *L04 *L03   *L12 *L11  L10  L09  L13  L14
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
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive EEPFPEE
 */
TEST_F(JTSPlanarizerTests, Test26) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                |
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |    
    //              L03  L04 *L05 *L06*L09 *L10  L11  L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                 Q01
    //                                  |
    //       __________________________Q02___________________________
    //      |    |    |    |    |    |      |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06  *L09 *L10  L11  L12  L13  L14
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
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}



/**  @brief tests JTSTree::templateQ3() positive EEPFPEE
 */
TEST_F(JTSPlanarizerTests, Test27) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                |
    //       ________________________Q02________________________
    //      |    |           |                 |         |      |
    //     L01  L02   ____ _Q03_____     _____Q04______  L13   L14
    //               |    |    |    |   |    |    |    |    
    //              L03  L04 *L05 *L06 L09  L10 *L11 *L12
    // 
    //                                _||_
    //                                \  /
    //                                 \/ 
    //
    //                                 Q01
    //                                  |
    //       __________________________Q02___________________________
    //      |    |    |    |    |    |      |    |    |    |    |    |
    //     L01  L02  L03  L04 *L05 *L06  *L12 *L11  L10   L09  L13  L14
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
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    L11.mGeneration    = pqTree.mGeneration;
    L11.mPertinentType = JTSTreeNode::Full;
    auto& L12 = createL(pqTree);
    L12.mGeneration    = pqTree.mGeneration;
    L12.mPertinentType = JTSTreeNode::Full;
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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
    Q02.mPertinentChildrenCount = 2;
    Q02.mFullChildrenCount = 0;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}



/**  @brief tests JTSTree::templateQ3() positive #1 PFFPEE
 */
TEST_F(JTSPlanarizerTests, Test28) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________________
    //                       |         |    |          |         |      |
    //                ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
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
    //               L06  L05 *L04 *L03 *L07 *L08 *L09 *L10  L11  L12  L13  L14
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
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFPEE
 */
TEST_F(JTSPlanarizerTests, Test29) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02____________________________
    //                       |         |    |          |         |      |
    //                ____ _Q03_____ *L07 *L08   _____Q04______  L13   L14
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
    //               L03  L04 *L05 *L06 *L07 *L08 *L09 *L10  L11  L12  L13  L14
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
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
    auto& L09 = createL(pqTree);
    L09.mGeneration    = pqTree.mGeneration;
    L09.mPertinentType = JTSTreeNode::Full;
    auto& L10 = createL(pqTree);
    L10.mGeneration    = pqTree.mGeneration;
    L10.mPertinentType = JTSTreeNode::Full;
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);

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

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 12);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFP
 */
TEST_F(JTSPlanarizerTests, Test30) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___________
    //                       |         |    |          |
    //                ____ _Q03_____ *L07 *L08   _____Q04______
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
    //               L03  L04 *L05 *L06 *L07 *L08 *L09 *L10  L11  L12
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
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
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
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 10);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFP
 */
TEST_F(JTSPlanarizerTests, Test31) {


    JTSTree& pqTree = getPQTree();

    //
    //                                   Q01
    //                                    |
    //                        ___________Q02___________
    //                       |         |    |          |
    //                ____ _Q03_____ *L07 *L08   _____Q04______
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
    //               L03  L04 *L05 *L06 *L07 *L08 *L12 *L11  L10  L09
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
    L07.mGeneration    = pqTree.mGeneration;
    L07.mPertinentType = JTSTreeNode::Full;
    auto& L08 = createL(pqTree);
    L08.mGeneration    = pqTree.mGeneration;
    L08.mPertinentType = JTSTreeNode::Full;
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
    Q02.mPertinentChildrenCount = 4;
    Q02.mFullChildrenCount = 2;
    Q02.mPartialChild1 = Q03.backIt();
    Q02.mPartialChild2 = Q04.backIt();

    list<node_list_it_t> Q01children;
    Q01children.push_back(Q02.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentChildrenCount = 1;

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 10);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 6);
    EXPECT_EQ(Q02.mFullChildrenCount, 6);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFP
 */
TEST_F(JTSPlanarizerTests, Test32) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                 |
    //                        ________Q02_______
    //                       |                  |
    //                ____ _Q03_____      _____Q04______
    //               |    |    |    |    |    |    |    |    
    //              L03  L04 *L05 *L06  L09  L10 *L11 *L12
    // 
    //                               _||_
    //                               \  /
    //                                \/ 
    //
    //                                 Q01
    //                                  |
    //                 ________________Q02_______________
    //                |    |    |    |    |    |    |    |
    //               L03  L04 *L05 *L06 *L12 *L11  L10  L09
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

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFP
 */
TEST_F(JTSPlanarizerTests, Test33) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                 |
    //                        ________Q02_______
    //                       |                  |
    //                ____ _Q03_____      _____Q04______
    //               |    |    |    |    |    |    |    |    
    //             *L03 *L04  L05  L06  L09  L10 *L11 *L12
    // 
    //                               _||_
    //                               \  /
    //                                \/ 
    //
    //                                 Q01
    //                                  |
    //                 ________________Q02_______________
    //                |    |    |    |    |    |    |    |
    //               L06  L05 *L04 *L03 *L12 *L11  L10  L09
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

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFP
 */
TEST_F(JTSPlanarizerTests, Test34) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                 |
    //                        ________Q02_______
    //                       |                  |
    //                ____ _Q03_____      _____Q04______
    //               |    |    |    |    |    |    |    |    
    //              L03  L04 *L05 *L06 *L09 *L10  L11  L12
    // 
    //                               _||_
    //                               \  /
    //                                \/ 
    //
    //                                 Q01
    //                                  |
    //                 ________________Q02_______________
    //                |    |    |    |    |    |    |    |
    //               L03  L04 *L05 *L06 *L09 *L10  L11  L12
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

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}


/**  @brief tests JTSTree::templateQ3() positive #1 PFFP
 */
TEST_F(JTSPlanarizerTests, Test35) {


    JTSTree& pqTree = getPQTree();

    //
    //                                Q01
    //                                 |
    //                        ________Q02_______
    //                       |                  |
    //                ____ _Q03_____      _____Q04______
    //               |    |    |    |    |    |    |    |    
    //             *L03 *L04  L05  L06 *L09 *L10  L11  L12
    // 
    //                               _||_
    //                               \  /
    //                                \/ 
    //
    //                                 Q01
    //                                  |
    //                 ________________Q02_______________
    //                |    |    |    |    |    |    |    |
    //               L06  L05 *L04 *L03 *L09 *L10  L11  L12
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

    EXPECT_EQ(templateQ3(pqTree, Q02), true);
    EXPECT_EQ(Q02.mChildren.size(), 8);
    EXPECT_EQ(Q02.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q02.mFullChildrenCount, 4);
    EXPECT_EQ(Q02.mPartialChild1, pqTree.nil());
    EXPECT_EQ(Q02.mPartialChild2, pqTree.nil());
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

}

