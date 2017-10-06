

/**  @brief tests JTSTreeNode::determineWHABCTypeForChildren()
 */
TEST_F(JTSPlanarizerTests, Test8) {

    JTSTree& pqTree = getPQTree();

    initializeForOneIteration(pqTree);

    // Case 1: Node L
    auto& L01 = createL(pqTree);
    L01.mGeneration = pqTree.mGeneration;
    calculateWHAC(L01);
    determineRootNodeType(L01);
    determineWHABCTypeForChildren(L01);
    //EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    //EXPECT_EQ(L01.mW, 1);
    //EXPECT_EQ(L01.mH, 0);
    //EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);

    // Case 2: Node P Empty Empty Full
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    L04.mGeneration = pqTree.mGeneration;
    calculateWHAC(L04);

    list<node_list_it_t> P01children;
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    P01children.push_back(L04.backIt());
    auto& P01 = createP(pqTree, P01children);
    L04.mGeneration = pqTree.mGeneration;
    calculateWHAC(P01);
    determineRootNodeType(P01);
    determineWHABCTypeForChildren(P01);
    //EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P01.mW, 1);
    //EXPECT_EQ(P01.mH, 0);
    //EXPECT_EQ(P01.mA, 0);
    //EXPECT_EQ(P01.mChildMax1WminusH, pqTree.nil());
    //EXPECT_EQ(P01.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P01.mChildMaxWminusA,  L04.backIt());
    //EXPECT_EQ(P01.mTypeAwithSingleA, true);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);

    // Case 3: Node P Full Empty Full
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    L05.mGeneration = pqTree.mGeneration;
    calculateWHAC(L05);
    L07.mGeneration = pqTree.mGeneration;
    calculateWHAC(L07);

    list<node_list_it_t> P02children;
    P02children.push_back(L05.backIt());
    P02children.push_back(L06.backIt());
    P02children.push_back(L07.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration = pqTree.mGeneration;
    calculateWHAC(P02);
    determineRootNodeType(P02);
    determineWHABCTypeForChildren(P02);
    //EXPECT_EQ(P02.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P02.mW, 2);
    //EXPECT_EQ(P02.mH, 0);
    //EXPECT_EQ(P02.mA, 0);
    //EXPECT_EQ(P02.mChildMax1WminusH, pqTree.nil());
    //EXPECT_EQ(P02.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P02.mTypeAwithSingleA, false);
    EXPECT_EQ(P02.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L07.mWHABType, JTSTreeNode::B);


    // Case 4: Node P Full Full Full
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    L08.mGeneration = pqTree.mGeneration;
    calculateWHAC(L08);
    L09.mGeneration = pqTree.mGeneration;
    calculateWHAC(L09);
    L10.mGeneration = pqTree.mGeneration;
    calculateWHAC(L10);

    list<node_list_it_t> P03children;
    P03children.push_back(L08.backIt());
    P03children.push_back(L09.backIt());
    P03children.push_back(L10.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    calculateWHAC(P03);
    determineRootNodeType(P03);
    determineWHABCTypeForChildren(P03);
    //EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Full);
    //EXPECT_EQ(P03.mW, 3);
    //EXPECT_EQ(P03.mH, 0);
    //EXPECT_EQ(P03.mA, 0);
    //EXPECT_EQ(P03.mChildMax1WminusH, pqTree.nil());
    //EXPECT_EQ(P03.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P03.mTypeAwithSingleA, false);
    EXPECT_EQ(P03.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L08.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L09.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L10.mWHABType, JTSTreeNode::B);


    // Case 5: Node P Empty Empty Partial(W=H)
    // 
    // NOTE1: If a partial node has W=H, it means it can't be of type H
    //        (making it of type H will make the node empty.)
    // NOTE2: There are cases where mChildMax1WminusH and/or mChildMax1WminusH 
    //        is empty if there are not enough partial children.
    //      
    list<node_list_it_t> emptyChildren;

    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& Q01 = createQ(pqTree,emptyChildren);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = JTSTreeNode::Partial;
    Q01.mW = 10;
    Q01.mH = 10;
    Q01.mA = 5;
    Q01.mCD = 3;

    list<node_list_it_t> P04children;
    P04children.push_back(L11.backIt());
    P04children.push_back(L12.backIt());
    P04children.push_back(Q01.backIt());
    auto& P04 = createP(pqTree, P04children);
    P04.mGeneration = pqTree.mGeneration;
    calculateWHAC(P04);
    determineRootNodeType(P04);
    determineWHABCTypeForChildren(P04);
    //EXPECT_EQ(P04.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P04.mW, 10);
    //EXPECT_EQ(P04.mH, 10);
    //EXPECT_EQ(P04.mA, 5);
    //EXPECT_EQ(P04.mChildMax1WminusH, Q01.backIt());
    //EXPECT_EQ(P04.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P04.mTypeAwithSingleA, true);
    //EXPECT_EQ(P04.mChildMaxWminusA,  Q01.backIt());
    EXPECT_EQ(P04.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::A);


    // Case 6: Node P Empty Empty Partial(W>H)

    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);
    auto& Q02 = createQ(pqTree,emptyChildren);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentType = JTSTreeNode::Partial;
    Q02.mW = 20;
    Q02.mH = 7;
    Q02.mA = 5;
    Q02.mCD = 3;

    list<node_list_it_t> P05children;
    P05children.push_back(L13.backIt());
    P05children.push_back(L14.backIt());
    P05children.push_back(Q02.backIt());
    auto& P05 = createP(pqTree, P05children);
    P05.mGeneration = pqTree.mGeneration;
    calculateWHAC(P05);
    determineRootNodeType(P05);
    determineWHABCTypeForChildren(P05);
    //EXPECT_EQ(P05.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P05.mW, 20);
    //EXPECT_EQ(P05.mH, 7);
    //EXPECT_EQ(P05.mA, 5);
    //EXPECT_EQ(P05.mChildMax1WminusH, Q02.backIt());
    //EXPECT_EQ(P05.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P05.mTypeAwithSingleA, true);
    //EXPECT_EQ(P05.mChildMaxWminusA,  Q02.backIt());
    EXPECT_EQ(P05.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q02.mWHABType, JTSTreeNode::A);

    // Case 7: Node P Empty Partial(W=H) Partial(W=H)

    auto& L15 = createL(pqTree);
    auto& Q03 = createQ(pqTree,emptyChildren);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mW = 11;
    Q03.mH = 11;
    Q03.mA = 6;
    Q03.mCD = 10;
    auto& Q04 = createQ(pqTree,emptyChildren);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mW = 17;
    Q04.mH = 17;
    Q04.mA = 3;
    Q04.mCD = 15;

    list<node_list_it_t> P06children;
    P06children.push_back(L15.backIt());
    P06children.push_back(Q03.backIt());
    P06children.push_back(Q04.backIt());
    auto& P06 = createP(pqTree, P06children);
    P06.mGeneration = pqTree.mGeneration;
    calculateWHAC(P06);
    determineRootNodeType(P06);
    determineWHABCTypeForChildren(P06);
    //EXPECT_EQ(P06.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P06.mW, 28);
    //EXPECT_EQ(P06.mH, 28);
    //EXPECT_EQ(P06.mA, 14);
    //EXPECT_EQ(P06.mChildMax1WminusH, Q04.backIt());
    //EXPECT_EQ(P06.mChildMax2WminusH, Q03.backIt());
    //EXPECT_EQ(P06.mTypeAwithSingleA, true);
    //EXPECT_EQ(P06.mChildMaxWminusA,  Q04.backIt());
    EXPECT_EQ(P06.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q04.mWHABType, JTSTreeNode::A);

    // Case 8: Node P Empty Partial(W>H) Partial(W>H)

    auto& L16 = createL(pqTree);
    auto& Q05 = createQ(pqTree,emptyChildren);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = JTSTreeNode::Partial;
    Q05.mW = 30;
    Q05.mH = 11;
    Q05.mA = 6;
    Q05.mCD = 10;
    auto& Q06 = createQ(pqTree,emptyChildren);
    Q06.mGeneration = pqTree.mGeneration;
    Q06.mPertinentType = JTSTreeNode::Partial;
    Q06.mW = 40;
    Q06.mH = 17;
    Q06.mA = 3;
    Q06.mCD = 12;

    list<node_list_it_t> P07children;
    P07children.push_back(L16.backIt());
    P07children.push_back(Q05.backIt());
    P07children.push_back(Q06.backIt());
    auto& P07 = createP(pqTree, P07children);
    P07.mGeneration = pqTree.mGeneration;
    calculateWHAC(P07);
    determineRootNodeType(P07);
    determineWHABCTypeForChildren(P07);
    //EXPECT_EQ(P07.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P07.mW, 70);
    //EXPECT_EQ(P07.mH, 47);
    //EXPECT_EQ(P07.mA, 28);
    //EXPECT_EQ(P07.mChildMax1WminusH, Q06.backIt());
    //EXPECT_EQ(P07.mChildMax2WminusH, Q05.backIt());
    //EXPECT_EQ(P07.mTypeAwithSingleA, false);
    EXPECT_EQ(P07.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q05.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q06.mWHABType, JTSTreeNode::H);

    // Case 9.1: Node P Empty Partial(W>H) Partial(W>H)

    auto& L17 = createL(pqTree);
    auto& Q07 = createQ(pqTree,emptyChildren);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = JTSTreeNode::Partial;
    Q07.mW = 30;
    Q07.mH = 14;
    Q07.mA = 6;
    Q07.mCD = 10;
    auto& Q08 = createQ(pqTree,emptyChildren);
    Q08.mGeneration = pqTree.mGeneration;
    Q08.mPertinentType = JTSTreeNode::Partial;
    Q08.mW = 40;
    Q08.mH = 20;
    Q08.mA = 3;
    Q08.mCD = 15;

    list<node_list_it_t> P08children;
    P08children.push_back(L17.backIt());
    P08children.push_back(Q07.backIt());
    P08children.push_back(Q08.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    calculateWHAC(P08);
    determineRootNodeType(P08);
    determineWHABCTypeForChildren(P08);
    //EXPECT_EQ(P08.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P08.mW, 70);
    //EXPECT_EQ(P08.mH, 50);
    //EXPECT_EQ(P08.mA, 33);
    //EXPECT_EQ(P08.mChildMax1WminusH, Q08.backIt());
    //EXPECT_EQ(P08.mChildMax2WminusH, Q07.backIt());
    //EXPECT_EQ(P08.mTypeAwithSingleA, true);
    //EXPECT_EQ(P08.mChildMaxWminusA,  Q08.backIt());
    EXPECT_EQ(P08.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q07.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q08.mWHABType, JTSTreeNode::A);


    // Case 9.2: Node P Full Partial(W>H) Partial(W>H)

    auto& Q09 = createQ(pqTree,emptyChildren);
    Q09.mGeneration = pqTree.mGeneration;
    Q09.mPertinentType = JTSTreeNode::Full;
    Q09.mW = 2;
    Q09.mH = 0;
    Q09.mA = 0;
    Q09.mCD = 0;

    auto& Q10 = createQ(pqTree,emptyChildren);
    Q10.mGeneration = pqTree.mGeneration;
    Q10.mPertinentType = JTSTreeNode::Partial;
    Q10.mW = 30;
    Q10.mH = 14;
    Q10.mA = 6;
    Q10.mCD = 10;

    auto& Q11 = createQ(pqTree,emptyChildren);
    Q11.mGeneration = pqTree.mGeneration;
    Q11.mPertinentType = JTSTreeNode::Partial;
    Q11.mW = 40;
    Q11.mH = 20;
    Q11.mA = 3;
    Q11.mCD = 14;

    list<node_list_it_t> P09children;
    P09children.push_back(Q09.backIt());
    P09children.push_back(Q10.backIt());
    P09children.push_back(Q11.backIt());
    auto& P09 = createP(pqTree, P09children);
    P09.mGeneration = pqTree.mGeneration;
    calculateWHAC(P09);
    determineRootNodeType(P09);
    determineWHABCTypeForChildren(P09);
    //EXPECT_EQ(P09.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P09.mW, 72);
    //EXPECT_EQ(P09.mH, 50);
    //EXPECT_EQ(P09.mA, 34);
    //EXPECT_EQ(P09.mChildMax1WminusH, Q11.backIt());
    //EXPECT_EQ(P09.mChildMax2WminusH, Q10.backIt());
    //EXPECT_EQ(P09.mTypeAwithSingleA, false);
    EXPECT_EQ(P09.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q09.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q10.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q11.mWHABType, JTSTreeNode::H);

    // Case 10: Node P Full Full Full Partial(W=H) Partial(W=H)

    auto& Q12 = createQ(pqTree,emptyChildren);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentType = JTSTreeNode::Full;
    Q12.mW = 5;
    Q12.mH = 0;
    Q12.mA = 0;
    Q12.mCD = 0;

    auto& Q13 = createQ(pqTree,emptyChildren);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = JTSTreeNode::Full;
    Q13.mW = 4;
    Q13.mH = 0;
    Q13.mA = 0;
    Q13.mCD = 0;

    auto& Q14 = createQ(pqTree,emptyChildren);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = JTSTreeNode::Full;
    Q14.mW = 7;
    Q14.mH = 0;
    Q14.mA = 0;
    Q14.mCD = 0;

    auto& Q15 = createQ(pqTree,emptyChildren);
    Q15.mGeneration = pqTree.mGeneration;
    Q15.mPertinentType = JTSTreeNode::Partial;
    Q15.mW = 30;
    Q15.mH = 30;
    Q15.mA = 6;
    Q15.mCD = 20;

    auto& Q16 = createQ(pqTree,emptyChildren);
    Q16.mGeneration = pqTree.mGeneration;
    Q16.mPertinentType = JTSTreeNode::Partial;
    Q16.mW = 40;
    Q16.mH = 40;
    Q16.mA = 3;
    Q16.mCD = 20;

    list<node_list_it_t> P10children;
    P10children.push_back(Q12.backIt());
    P10children.push_back(Q13.backIt());
    P10children.push_back(Q14.backIt());
    P10children.push_back(Q15.backIt());
    P10children.push_back(Q16.backIt());

    auto& P10 = createP(pqTree, P10children);
    P10.mGeneration = pqTree.mGeneration;
    calculateWHAC(P10);
    determineRootNodeType(P10);
    determineWHABCTypeForChildren(P10);
    //EXPECT_EQ(P10.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P10.mW, 86);
    //EXPECT_EQ(P10.mH, 70);
    //EXPECT_EQ(P10.mA, 49);
    //EXPECT_EQ(P10.mChildMax1WminusH, Q16.backIt());
    //EXPECT_EQ(P10.mChildMax2WminusH, Q15.backIt());
    //EXPECT_EQ(P10.mTypeAwithSingleA, true);
    //EXPECT_EQ(P10.mChildMaxWminusA,  Q16.backIt());
    EXPECT_EQ(P10.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q12.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q13.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q14.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q15.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q16.mWHABType, JTSTreeNode::A);


    // Case 11: Node P Full Full Full Partial(W=H) Partial(W=H) #2

    auto& Q17 = createQ(pqTree,emptyChildren);
    Q17.mGeneration = pqTree.mGeneration;
    Q17.mPertinentType = JTSTreeNode::Full;
    Q17.mW = 15;
    Q17.mH = 0;
    Q17.mA = 0;
    Q17.mCD = 0;

    auto& Q18 = createQ(pqTree,emptyChildren);
    Q18.mGeneration = pqTree.mGeneration;
    Q18.mPertinentType = JTSTreeNode::Full;
    Q18.mW = 14;
    Q18.mH = 0;
    Q18.mA = 0;
    Q18.mCD = 0;

    auto& Q19 = createQ(pqTree,emptyChildren);
    Q19.mGeneration = pqTree.mGeneration;
    Q19.mPertinentType = JTSTreeNode::Full;
    Q19.mW = 17;
    Q19.mH = 0;
    Q19.mA = 0;
    Q19.mCD = 0;

    auto& Q20 = createQ(pqTree,emptyChildren);
    Q20.mGeneration = pqTree.mGeneration;
    Q20.mPertinentType = JTSTreeNode::Partial;
    Q20.mW = 30;
    Q20.mH = 30;
    Q20.mA = 6;
    Q20.mCD = 20;

    auto& Q21 = createQ(pqTree,emptyChildren);
    Q21.mGeneration = pqTree.mGeneration;
    Q21.mPertinentType = JTSTreeNode::Partial;
    Q21.mW = 40;
    Q21.mH = 40;
    Q21.mA = 3;
    Q21.mCD = 20;

    list<node_list_it_t> P11children;
    P11children.push_back(Q17.backIt());
    P11children.push_back(Q18.backIt());
    P11children.push_back(Q19.backIt());
    P11children.push_back(Q20.backIt());
    P11children.push_back(Q21.backIt());

    auto& P11 = createP(pqTree, P11children);
    P11.mGeneration = pqTree.mGeneration;
    calculateWHAC(P11);
    determineRootNodeType(P11);
    determineWHABCTypeForChildren(P11);
    //EXPECT_EQ(P11.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P11.mW, 116);
    //EXPECT_EQ(P11.mH, 70);
    //EXPECT_EQ(P11.mA, 70);
    //EXPECT_EQ(P11.mChildMax1WminusH, Q21.backIt());
    //EXPECT_EQ(P11.mChildMax2WminusH, Q20.backIt());
    //EXPECT_EQ(P11.mTypeAwithSingleA, false);
    //EXPECT_EQ(P11.mChildMaxWminusA,  Q21.backIt());
    EXPECT_EQ(P11.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q17.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q18.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q19.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q20.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q21.mWHABType, JTSTreeNode::H);


    // Case 12: Node P Full Full Full Partial(W>H) Partial(W>H)

    auto& Q22 = createQ(pqTree,emptyChildren);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentType = JTSTreeNode::Full;
    Q22.mW = 15;
    Q22.mH = 0;
    Q22.mA = 0;
    Q22.mCD = 0;

    auto& Q23 = createQ(pqTree,emptyChildren);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Full;
    Q23.mW = 14;
    Q23.mH = 0;
    Q23.mA = 0;
    Q23.mCD = 0;

    auto& Q24 = createQ(pqTree,emptyChildren);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = JTSTreeNode::Full;
    Q24.mW = 17;
    Q24.mH = 0;
    Q24.mA = 0;
    Q24.mCD = 0;

    auto& Q25 = createQ(pqTree,emptyChildren);
    Q25.mGeneration = pqTree.mGeneration;
    Q25.mPertinentType = JTSTreeNode::Partial;
    Q25.mW = 40;
    Q25.mH = 3;
    Q25.mA = 3;
    Q25.mCD = 20;

    auto& Q26 = createQ(pqTree,emptyChildren);
    Q26.mGeneration = pqTree.mGeneration;
    Q26.mPertinentType = JTSTreeNode::Partial;
    Q26.mW = 30;
    Q26.mH = 6;
    Q26.mA = 6;
    Q26.mCD = 20;

    list<node_list_it_t> P12children;
    P12children.push_back(Q22.backIt());
    P12children.push_back(Q23.backIt());
    P12children.push_back(Q24.backIt());
    P12children.push_back(Q25.backIt());
    P12children.push_back(Q26.backIt());

    auto& P12 = createP(pqTree, P12children);
    P12.mGeneration = pqTree.mGeneration;
    calculateWHAC(P12);
    determineRootNodeType(P12);
    determineWHABCTypeForChildren(P12);
    //EXPECT_EQ(P12.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P12.mW, 116);
    //EXPECT_EQ(P12.mH, 33);
    //EXPECT_EQ(P12.mA, 9);
    //EXPECT_EQ(P12.mChildMax1WminusH, Q25.backIt());
    //EXPECT_EQ(P12.mChildMax2WminusH, Q26.backIt());
    //EXPECT_EQ(P12.mTypeAwithSingleA, false);
    //EXPECT_EQ(P12.mChildMaxWminusA,  Q25.backIt());
    EXPECT_EQ(P12.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q22.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q23.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q24.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q25.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q26.mWHABType, JTSTreeNode::H);


    // Case 13: Q Empty Empty Partial

    auto& Q27 = createQ(pqTree,emptyChildren);
    auto& Q28 = createQ(pqTree,emptyChildren);
    auto& Q29 = createQ(pqTree,emptyChildren);
    auto& Q30 = createQ(pqTree,emptyChildren);

    auto& Q31 = createQ(pqTree,emptyChildren);
    Q31.mGeneration = pqTree.mGeneration;
    Q31.mPertinentType = JTSTreeNode::Partial;
    Q31.mW = 10;
    Q31.mH = 6;
    Q31.mA = 4;
    Q31.mCD = 5;

    list<node_list_it_t> Q33children;
    Q33children.push_back(Q27.backIt());
    Q33children.push_back(Q28.backIt());
    Q33children.push_back(Q29.backIt());
    Q33children.push_back(Q30.backIt());
    Q33children.push_back(Q31.backIt());

    auto& Q33 = createQ(pqTree, Q33children);
    Q33.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q33);
    determineRootNodeType(Q33);
    determineWHABCTypeForChildren(Q33);
    //EXPECT_EQ(Q33.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q33.mW, 10);
    //EXPECT_EQ(Q33.mH, 6);
    //EXPECT_EQ(Q33.mA, 4);
    //EXPECT_EQ(Q33.mChildBoundaryForQH, Q31.backIt());
    //EXPECT_EQ(Q33.mQHFullOnListHead, false);
    //EXPECT_EQ(Q33.mChildBoundaryOnHeadForQA, Q31.backIt());
    //EXPECT_EQ(Q33.mChildBoundaryOnTailForQA, Q31.backIt());
    //EXPECT_EQ(Q33.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q33.mChildMaxWminusA,  Q31.backIt());
    EXPECT_EQ(Q33.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q31.mWHABType, JTSTreeNode::A);

    // Case 14: Q Partial Empty Empty
    auto& Q34 = createQ(pqTree,emptyChildren);
    Q34.mGeneration = pqTree.mGeneration;
    Q34.mPertinentType = JTSTreeNode::Partial;
    Q34.mW = 10;
    Q34.mH = 6;
    Q34.mA = 4;
    Q34.mCD = 4;

    auto& Q35 = createQ(pqTree,emptyChildren);
    auto& Q36 = createQ(pqTree,emptyChildren);
    auto& Q37 = createQ(pqTree,emptyChildren);
    auto& Q38 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q39children;
    Q39children.push_back(Q34.backIt());
    Q39children.push_back(Q35.backIt());
    Q39children.push_back(Q36.backIt());
    Q39children.push_back(Q37.backIt());
    Q39children.push_back(Q38.backIt());

    auto& Q39 = createQ(pqTree, Q39children);
    calculateWHAC(Q39);
    determineRootNodeType(Q39);
    determineWHABCTypeForChildren(Q39);

    //EXPECT_EQ(Q39.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q39.mW, 10);
    //EXPECT_EQ(Q39.mH, 6);
    //EXPECT_EQ(Q39.mA, 4);
    //EXPECT_EQ(Q39.mChildBoundaryForQH, Q34.backIt());
    //EXPECT_EQ(Q39.mQHFullOnListHead, true);
    //EXPECT_EQ(Q39.mChildBoundaryOnHeadForQA, Q34.backIt());
    //EXPECT_EQ(Q39.mChildBoundaryOnTailForQA, Q34.backIt());
    //EXPECT_EQ(Q39.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q39.mChildMaxWminusA,  Q34.backIt());
    EXPECT_EQ(Q39.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q34.mWHABType, JTSTreeNode::A);

    // Case 15: Q Partial Empty Full Empty Parial

    auto& Q40 = createQ(pqTree,emptyChildren);
    Q40.mGeneration = pqTree.mGeneration;
    Q40.mPertinentType = JTSTreeNode::Partial;
    Q40.mW = 10;
    Q40.mH = 6;
    Q40.mA = 4;
    Q40.mCD = 5;

    auto& Q41 = createQ(pqTree,emptyChildren);
    auto& Q42 = createQ(pqTree,emptyChildren);
    Q42.mGeneration = pqTree.mGeneration;
    Q42.mPertinentType = JTSTreeNode::Full;
    Q42.mW = 17;
    Q42.mH = 0;
    Q42.mA = 0;
    Q42.mCD = 0;

    auto& Q43 = createQ(pqTree,emptyChildren);
    auto& Q44 = createQ(pqTree,emptyChildren);
    Q44.mGeneration = pqTree.mGeneration;
    Q44.mPertinentType = JTSTreeNode::Partial;
    Q44.mW = 8;
    Q44.mH = 5;
    Q44.mA = 3;
    Q44.mCD = 4;

    list<node_list_it_t> Q45children;
    Q45children.push_back(Q40.backIt());
    Q45children.push_back(Q41.backIt());
    Q45children.push_back(Q42.backIt());
    Q45children.push_back(Q43.backIt());
    Q45children.push_back(Q44.backIt());

    auto& Q45 = createQ(pqTree, Q45children);
    Q45.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q45);
    determineRootNodeType(Q45);
    determineWHABCTypeForChildren(Q45);
    //EXPECT_EQ(Q45.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q45.mW, 35);
    //EXPECT_EQ(Q45.mH, 31);
    //EXPECT_EQ(Q45.mA, 18);
    //EXPECT_EQ(Q45.mChildBoundaryForQH, Q40.backIt());
    //EXPECT_EQ(Q45.mQHFullOnListHead, true);
    //EXPECT_EQ(Q45.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q45.mChildMaxWminusA,  Q42.backIt());
    EXPECT_EQ(Q45.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q40.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q42.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q44.mWHABType, JTSTreeNode::W);


    // Case 16: Q Partial Partial Partial Partial Partial Partial
    auto& Q46 = createQ(pqTree,emptyChildren);
    Q46.mGeneration = pqTree.mGeneration;
    Q46.mPertinentType = JTSTreeNode::Partial;
    Q46.mW = 30;
    Q46.mH = 7;
    Q46.mA = 4;
    Q46.mCD = 5;

    auto& Q47 = createQ(pqTree,emptyChildren);
    Q47.mGeneration = pqTree.mGeneration;
    Q47.mPertinentType = JTSTreeNode::Partial;
    Q47.mW = 20;
    Q47.mH = 13;
    Q47.mA = 12;
    Q47.mCD = 12;

    auto& Q48 = createQ(pqTree,emptyChildren);
    Q48.mGeneration = pqTree.mGeneration;
    Q48.mPertinentType = JTSTreeNode::Partial;
    Q48.mW = 15;
    Q48.mH = 8;
    Q48.mA = 4;
    Q48.mCD = 5;

    auto& Q49 = createQ(pqTree,emptyChildren);
    Q49.mGeneration = pqTree.mGeneration;
    Q49.mPertinentType = JTSTreeNode::Partial;
    Q49.mW = 17;
    Q49.mH = 3;
    Q49.mA = 4;
    Q49.mCD = 3;

    auto& Q50 = createQ(pqTree,emptyChildren);
    Q50.mGeneration = pqTree.mGeneration;
    Q50.mPertinentType = JTSTreeNode::Partial;
    Q50.mW = 16;
    Q50.mH = 8;
    Q50.mA = 5;
    Q50.mCD = 6;

    auto& Q51 = createQ(pqTree,emptyChildren);
    Q51.mGeneration = pqTree.mGeneration;
    Q51.mPertinentType = JTSTreeNode::Partial;
    Q51.mW = 22;
    Q51.mH = 3;
    Q51.mA = 2;
    Q51.mCD = 2;

    list<node_list_it_t> Q52children;
    Q52children.push_back(Q46.backIt());
    Q52children.push_back(Q47.backIt());
    Q52children.push_back(Q48.backIt());
    Q52children.push_back(Q49.backIt());
    Q52children.push_back(Q50.backIt());
    Q52children.push_back(Q51.backIt());

    auto& Q52 = createQ(pqTree, Q52children);
    Q52.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q52);
    determineRootNodeType(Q52);
    determineWHABCTypeForChildren(Q52);
    //EXPECT_EQ(Q52.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q52.mW, 120);
    //EXPECT_EQ(Q52.mH, 97);
    //EXPECT_EQ(Q52.mChildBoundaryForQH, Q46.backIt());
    //EXPECT_EQ(Q52.mQHFullOnListHead, true);
    //EXPECT_EQ(Q52.mA, 90);
    //EXPECT_EQ(Q52.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q52.mChildBoundaryOnHeadForQA, Q46.backIt());
    //EXPECT_EQ(Q52.mChildBoundaryOnTailForQA, Q47.backIt());
    EXPECT_EQ(Q52.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q46.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q47.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q48.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q49.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q50.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q51.mWHABType, JTSTreeNode::H);

    // Case 17: Q Partial Partial Partial Partial Partial Partial #2
    auto& Q53 = createQ(pqTree,emptyChildren);
    Q53.mGeneration = pqTree.mGeneration;
    Q53.mPertinentType = JTSTreeNode::Partial;
    Q53.mW = 22;
    Q53.mH = 3;
    Q53.mA = 2;
    Q53.mCD = 2;

    auto& Q54 = createQ(pqTree,emptyChildren);
    Q54.mGeneration = pqTree.mGeneration;
    Q54.mPertinentType = JTSTreeNode::Partial;
    Q54.mW = 16;
    Q54.mH = 8;
    Q54.mA = 5;
    Q54.mCD = 6;

    auto& Q55 = createQ(pqTree,emptyChildren);
    Q55.mGeneration = pqTree.mGeneration;
    Q55.mPertinentType = JTSTreeNode::Partial;
    Q55.mW = 17;
    Q55.mH = 3;
    Q55.mA = 3;
    Q55.mCD = 3;

    auto& Q56 = createQ(pqTree,emptyChildren);
    Q56.mGeneration = pqTree.mGeneration;
    Q56.mPertinentType = JTSTreeNode::Partial;
    Q56.mW = 15;
    Q56.mH = 8;
    Q56.mA = 4;
    Q56.mCD = 4;

    auto& Q57 = createQ(pqTree,emptyChildren);
    Q57.mGeneration = pqTree.mGeneration;
    Q57.mPertinentType = JTSTreeNode::Partial;
    Q57.mW = 20;
    Q57.mH = 13;
    Q57.mA = 12;
    Q57.mCD = 12;

    auto& Q58 = createQ(pqTree,emptyChildren);
    Q58.mGeneration = pqTree.mGeneration;
    Q58.mPertinentType = JTSTreeNode::Partial;
    Q58.mW = 30;
    Q58.mH = 7;
    Q58.mA = 4;
    Q58.mCD = 4;

    list<node_list_it_t> Q59children;
    Q59children.push_back(Q53.backIt());
    Q59children.push_back(Q54.backIt());
    Q59children.push_back(Q55.backIt());
    Q59children.push_back(Q56.backIt());
    Q59children.push_back(Q57.backIt());
    Q59children.push_back(Q58.backIt());

    auto& Q59 = createQ(pqTree, Q59children);
    Q59.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q59);
    determineRootNodeType(Q59);
    determineWHABCTypeForChildren(Q59);
    //EXPECT_EQ(Q59.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q59.mW, 120);
    //EXPECT_EQ(Q59.mH, 97);
    //EXPECT_EQ(Q59.mChildBoundaryForQH, Q58.backIt());
    //EXPECT_EQ(Q59.mQHFullOnListHead, false);
    //EXPECT_EQ(Q59.mA, 90);
    //EXPECT_EQ(Q59.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q59.mChildBoundaryOnHeadForQA, Q57.backIt());
    //EXPECT_EQ(Q59.mChildBoundaryOnTailForQA, Q58.backIt());
    EXPECT_EQ(Q59.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q53.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q54.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q55.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q56.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q57.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q58.mWHABType, JTSTreeNode::H);

    // Case 18: Q Partial Partial Partial Partial Partial Partial #3

    auto& Q60 = createQ(pqTree,emptyChildren);
    Q60.mGeneration = pqTree.mGeneration;
    Q60.mPertinentType = JTSTreeNode::Partial;
    Q60.mW = 17;
    Q60.mH = 11;
    Q60.mA = 3;
    Q60.mCD = 8;

    auto& Q61 = createQ(pqTree,emptyChildren);
    Q61.mGeneration = pqTree.mGeneration;
    Q61.mPertinentType = JTSTreeNode::Partial;
    Q61.mW = 30;
    Q61.mH = 7;
    Q61.mA = 4;
    Q61.mCD = 5;

    auto& Q62 = createQ(pqTree,emptyChildren);
    Q62.mGeneration = pqTree.mGeneration;
    Q62.mPertinentType = JTSTreeNode::Partial;
    Q62.mW = 20;
    Q62.mH = 13;
    Q62.mA = 12;
    Q62.mCD = 12;

    auto& Q63 = createQ(pqTree,emptyChildren);
    Q63.mGeneration = pqTree.mGeneration;
    Q63.mPertinentType = JTSTreeNode::Partial;
    Q63.mW = 15;
    Q63.mH = 8;
    Q63.mA = 4;
    Q63.mCD = 5;

    auto& Q64 = createQ(pqTree,emptyChildren);
    Q64.mGeneration = pqTree.mGeneration;
    Q64.mPertinentType = JTSTreeNode::Partial;
    Q64.mW = 16;
    Q64.mH = 8;
    Q64.mA = 5;
    Q64.mCD = 7;

    auto& Q65 = createQ(pqTree,emptyChildren);
    Q65.mGeneration = pqTree.mGeneration;
    Q65.mPertinentType = JTSTreeNode::Partial;
    Q65.mW = 22;
    Q65.mH = 3;
    Q65.mA = 2;
    Q65.mCD = 2;

    list<node_list_it_t> Q66children;
    Q66children.push_back(Q60.backIt());
    Q66children.push_back(Q61.backIt());
    Q66children.push_back(Q62.backIt());
    Q66children.push_back(Q63.backIt());
    Q66children.push_back(Q64.backIt());
    Q66children.push_back(Q65.backIt());

    auto& Q66 = createQ(pqTree, Q66children);
    Q66.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q66);
    determineRootNodeType(Q66);
    determineWHABCTypeForChildren(Q66);
    //EXPECT_EQ(Q66.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q66.mW, 120);
    //EXPECT_EQ(Q66.mH, 101);
    //EXPECT_EQ(Q66.mChildBoundaryForQH, Q65.backIt());
    //EXPECT_EQ(Q66.mQHFullOnListHead, false);
    //EXPECT_EQ(Q66.mA, 90);
    //EXPECT_EQ(Q66.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q66.mChildBoundaryOnHeadForQA, Q61.backIt());
    //EXPECT_EQ(Q66.mChildBoundaryOnTailForQA, Q62.backIt());
    EXPECT_EQ(Q66.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q60.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q61.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q62.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q63.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q64.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q65.mWHABType, JTSTreeNode::W);

    // Case 19: Q Partial Full Full Partial Partial Full Partial Full Full Full
    //            Partial Partial

    auto& Q67 = createQ(pqTree,emptyChildren);
    Q67.mGeneration = pqTree.mGeneration;
    Q67.mPertinentType = JTSTreeNode::Partial;
    Q67.mW = 17;
    Q67.mH = 11;
    Q67.mA = 3;
    Q67.mA = 2;

    auto& Q68 = createQ(pqTree,emptyChildren);
    Q68.mGeneration = pqTree.mGeneration;
    Q68.mPertinentType = JTSTreeNode::Full;
    Q68.mW = 17;
    Q68.mH = 0;
    Q68.mA = 0;
    Q68.mCD = 0;

    auto& Q69 = createQ(pqTree,emptyChildren);
    Q69.mGeneration = pqTree.mGeneration;
    Q69.mPertinentType = JTSTreeNode::Full;
    Q69.mW = 13;
    Q69.mH = 0;
    Q69.mA = 0;
    Q69.mCD = 0;

    auto& Q70 = createQ(pqTree,emptyChildren);
    Q70.mGeneration = pqTree.mGeneration;
    Q70.mPertinentType = JTSTreeNode::Partial;
    Q70.mW = 30;
    Q70.mH = 7;
    Q70.mA = 4;
    Q70.mCD = 4;

    auto& Q71 = createQ(pqTree,emptyChildren);
    Q71.mGeneration = pqTree.mGeneration;
    Q71.mPertinentType = JTSTreeNode::Partial;
    Q71.mW = 20;
    Q71.mH = 13;
    Q71.mA = 12;
    Q71.mCD = 8;

    auto& Q72 = createQ(pqTree,emptyChildren);
    Q72.mGeneration = pqTree.mGeneration;
    Q72.mPertinentType = JTSTreeNode::Full;
    Q72.mW = 22;
    Q72.mH = 0;
    Q72.mA = 0;
    Q72.mCD = 0;

    auto& Q73 = createQ(pqTree,emptyChildren);
    Q73.mGeneration = pqTree.mGeneration;
    Q73.mPertinentType = JTSTreeNode::Partial;
    Q73.mW = 15;
    Q73.mH = 8;
    Q73.mA = 4;
    Q73.mCD = 5;

    auto& Q74 = createQ(pqTree,emptyChildren);
    Q74.mGeneration = pqTree.mGeneration;
    Q74.mPertinentType = JTSTreeNode::Full;
    Q74.mW = 2;
    Q74.mH = 0;
    Q74.mA = 0;
    Q74.mCD = 0;

    auto& Q75 = createQ(pqTree,emptyChildren);
    Q75.mGeneration = pqTree.mGeneration;
    Q75.mPertinentType = JTSTreeNode::Full;
    Q75.mW = 6;
    Q75.mH = 0;
    Q75.mA = 0;
    Q75.mCD = 0;

    auto& Q76 = createQ(pqTree,emptyChildren);
    Q76.mGeneration = pqTree.mGeneration;
    Q76.mPertinentType = JTSTreeNode::Full;
    Q76.mW = 24;
    Q76.mH = 0;
    Q76.mA = 0;
    Q76.mCD = 0;

    auto& Q77 = createQ(pqTree,emptyChildren);
    Q77.mGeneration = pqTree.mGeneration;
    Q77.mPertinentType = JTSTreeNode::Partial;
    Q77.mW = 16;
    Q77.mH = 8;
    Q77.mA = 5;
    Q77.mCD = 6;

    auto& Q78 = createQ(pqTree,emptyChildren);
    Q78.mGeneration = pqTree.mGeneration;
    Q78.mPertinentType = JTSTreeNode::Partial;
    Q78.mW = 22;
    Q78.mH = 3;
    Q78.mA = 2;
    Q78.mCD = 2;

    list<node_list_it_t> Q79children;
    Q79children.push_back(Q67.backIt());
    Q79children.push_back(Q68.backIt());
    Q79children.push_back(Q69.backIt());
    Q79children.push_back(Q70.backIt());
    Q79children.push_back(Q71.backIt());
    Q79children.push_back(Q72.backIt());
    Q79children.push_back(Q73.backIt());
    Q79children.push_back(Q74.backIt());
    Q79children.push_back(Q75.backIt());
    Q79children.push_back(Q76.backIt());
    Q79children.push_back(Q77.backIt());
    Q79children.push_back(Q78.backIt());

    auto& Q79 = createQ(pqTree, Q79children);
    Q79.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q79);
    determineRootNodeType(Q79);
    determineWHABCTypeForChildren(Q79);
    //EXPECT_EQ(Q79.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q79.mW, 204);
    //EXPECT_EQ(Q79.mH, 185);
    //EXPECT_EQ(Q79.mChildBoundaryForQH, Q78.backIt());
    //EXPECT_EQ(Q79.mQHFullOnListHead, false);
    //EXPECT_EQ(Q79.mA, 145);
    //EXPECT_EQ(Q79.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q79.mChildBoundaryOnHeadForQA, Q67.backIt());
    //EXPECT_EQ(Q79.mChildBoundaryOnTailForQA, Q70.backIt());
    EXPECT_EQ(Q79.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q67.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q68.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q69.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q70.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q71.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q72.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q73.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q74.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q75.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q76.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q77.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q78.mWHABType, JTSTreeNode::W);

    // Case 20: Q Partial Full Full Empty Parial

    auto& Q80 = createQ(pqTree,emptyChildren);
    Q80.mGeneration = pqTree.mGeneration;
    Q80.mPertinentType = JTSTreeNode::Partial;
    Q80.mW = 17;
    Q80.mH = 5;
    Q80.mA = 3;
    Q80.mCD = 4;

    auto& Q81 = createQ(pqTree,emptyChildren);
    Q81.mGeneration = pqTree.mGeneration;
    Q81.mPertinentType = JTSTreeNode::Full;
    Q81.mW = 6;
    Q81.mH = 0;
    Q81.mA = 0;
    Q81.mCD = 0;

    auto& Q82 = createQ(pqTree,emptyChildren);
    Q82.mGeneration = pqTree.mGeneration;
    Q82.mPertinentType = JTSTreeNode::Full;
    Q82.mW = 16;
    Q82.mH = 0;
    Q82.mA = 0;
    Q82.mCD = 0;

    auto& Q83 = createQ(pqTree,emptyChildren);

    auto& Q84 = createQ(pqTree,emptyChildren);
    Q84.mGeneration = pqTree.mGeneration;
    Q84.mPertinentType = JTSTreeNode::Partial;
    Q84.mW = 23;
    Q84.mH = 10;
    Q84.mA = 8;
    Q84.mCD = 9;

    list<node_list_it_t> Q85children;
    Q85children.push_back(Q80.backIt());
    Q85children.push_back(Q81.backIt());
    Q85children.push_back(Q82.backIt());
    Q85children.push_back(Q83.backIt());
    Q85children.push_back(Q84.backIt());

    auto& Q85 = createQ(pqTree, Q85children);
    Q85.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q85);
    determineRootNodeType(Q85);
    determineWHABCTypeForChildren(Q85);

    //EXPECT_EQ(Q85.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q85.mW, 62);
    //EXPECT_EQ(Q85.mH, 49);
    //EXPECT_EQ(Q85.mChildBoundaryForQH, Q84.backIt());
    //EXPECT_EQ(Q85.mQHFullOnListHead, false);
    //EXPECT_EQ(Q85.mA, 28);
    //EXPECT_EQ(Q85.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q85.mChildBoundaryOnHeadForQA, Q80.backIt());
    //EXPECT_EQ(Q85.mChildBoundaryOnTailForQA, Q82.backIt());
    EXPECT_EQ(Q85.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q80.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q81.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q82.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q84.mWHABType, JTSTreeNode::W);


    // Case 21: Q Empty Full Empty

    auto& Q86 = createQ(pqTree,emptyChildren);

    auto& Q87 = createQ(pqTree,emptyChildren);
    Q87.mGeneration = pqTree.mGeneration;
    Q87.mPertinentType = JTSTreeNode::Full;
    Q87.mW = 6;
    Q87.mH = 0;
    Q87.mA = 0;
    Q87.mCD = 0;

    auto& Q88 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q89children;
    Q89children.push_back(Q86.backIt());
    Q89children.push_back(Q87.backIt());
    Q89children.push_back(Q88.backIt());

    auto& Q89 = createQ(pqTree, Q89children);
    Q89.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q89);
    determineRootNodeType(Q89);
    determineWHABCTypeForChildren(Q89);
    //EXPECT_EQ(Q89.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q89.mW, 6);
    //EXPECT_EQ(Q89.mH, 6);
    //EXPECT_EQ(Q89.mA, 0);
    //EXPECT_EQ(Q89.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q89.mChildMaxWminusA,     Q87.backIt());
    EXPECT_EQ(Q89.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q87.mWHABType, JTSTreeNode::B);

    // Case 22: Q Empty Full Full
    auto& Q90 = createQ(pqTree,emptyChildren);

    auto& Q91 = createQ(pqTree,emptyChildren);
    Q91.mGeneration = pqTree.mGeneration;
    Q91.mPertinentType = JTSTreeNode::Full;
    Q91.mW = 6;
    Q91.mH = 0;
    Q91.mA = 0;
    Q91.mCD = 0;


    auto& Q92 = createQ(pqTree,emptyChildren);
    Q92.mGeneration = pqTree.mGeneration;
    Q92.mPertinentType = JTSTreeNode::Full;
    Q92.mW = 10;
    Q92.mH = 0;
    Q92.mA = 0;
    Q92.mCD = 0;

    list<node_list_it_t> Q93children;
    Q93children.push_back(Q90.backIt());
    Q93children.push_back(Q91.backIt());
    Q93children.push_back(Q92.backIt());

    auto& Q93 = createQ(pqTree, Q93children);
    Q93.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q93);
    determineRootNodeType(Q93);
    determineWHABCTypeForChildren(Q93);
    //EXPECT_EQ(Q93.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q93.mW, 16);
    //EXPECT_EQ(Q93.mH, 0);
    //EXPECT_EQ(Q93.mChildBoundaryForQH, Q91.backIt());
    //EXPECT_EQ(Q93.mQHFullOnListHead, false);
    //EXPECT_EQ(Q93.mA, 0);
    //EXPECT_EQ(Q93.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q93.mChildBoundaryOnHeadForQA, Q91.backIt());
    //EXPECT_EQ(Q93.mChildBoundaryOnTailForQA, Q92.backIt());
    EXPECT_EQ(Q93.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q91.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q92.mWHABType, JTSTreeNode::B);


    // Case 23: Q Partial Full Full
    auto& Q94 = createQ(pqTree,emptyChildren);
    Q94.mGeneration = pqTree.mGeneration;
    Q94.mPertinentType = JTSTreeNode::Partial;
    Q94.mW = 10;
    Q94.mH = 5;
    Q94.mA = 4;
    Q94.mCD = 4;

    auto& Q95 = createQ(pqTree,emptyChildren);
    Q95.mGeneration = pqTree.mGeneration;
    Q95.mPertinentType = JTSTreeNode::Full;
    Q95.mW = 6;
    Q95.mH = 0;
    Q95.mA = 0;
    Q95.mCD = 0;

    auto& Q96 = createQ(pqTree,emptyChildren);
    Q96.mGeneration = pqTree.mGeneration;
    Q96.mPertinentType = JTSTreeNode::Full;
    Q96.mW = 10;
    Q96.mH = 0;
    Q96.mA = 0;
    Q96.mCD = 0;

    list<node_list_it_t> Q97children;
    Q97children.push_back(Q94.backIt());
    Q97children.push_back(Q95.backIt());
    Q97children.push_back(Q96.backIt());

    auto& Q97 = createQ(pqTree, Q97children);
    Q97.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q97);
    determineRootNodeType(Q97);
    determineWHABCTypeForChildren(Q97);
    //EXPECT_EQ(Q97.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q97.mW, 26);
    //EXPECT_EQ(Q97.mH, 5);
    //EXPECT_EQ(Q97.mChildBoundaryForQH, Q94.backIt());
    //EXPECT_EQ(Q97.mQHFullOnListHead, false);
    //EXPECT_EQ(Q97.mA, 5);
    //EXPECT_EQ(Q97.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q97.mChildBoundaryOnHeadForQA, Q94.backIt());
    //EXPECT_EQ(Q97.mChildBoundaryOnTailForQA, Q96.backIt());
    EXPECT_EQ(Q97.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q94.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q95.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q96.mWHABType, JTSTreeNode::B);


    // Case 24: Q Full Full Partial
    auto& Q98 = createQ(pqTree,emptyChildren);
    Q98.mGeneration = pqTree.mGeneration;
    Q98.mPertinentType = JTSTreeNode::Full;
    Q98.mW = 6;
    Q98.mH = 0;
    Q98.mA = 0;
    Q98.mCD = 0;

    auto& Q99 = createQ(pqTree,emptyChildren);
    Q99.mGeneration = pqTree.mGeneration;
    Q99.mPertinentType = JTSTreeNode::Full;
    Q99.mW = 10;
    Q99.mH = 0;
    Q99.mA = 0;
    Q99.mCD = 0;

    auto& Q100 = createQ(pqTree,emptyChildren);
    Q100.mGeneration = pqTree.mGeneration;
    Q100.mPertinentType = JTSTreeNode::Partial;
    Q100.mW = 10;
    Q100.mH = 5;
    Q100.mA = 4;
    Q100.mCD = 2;

    list<node_list_it_t> Q101children;
    Q101children.push_back(Q98.backIt());
    Q101children.push_back(Q99.backIt());
    Q101children.push_back(Q100.backIt());

    auto& Q101 = createQ(pqTree, Q101children);
    Q101.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q101);
    determineRootNodeType(Q101);
    determineWHABCTypeForChildren(Q101);

    //EXPECT_EQ(Q101.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q101.mW, 26);
    //EXPECT_EQ(Q101.mH, 5);
    //EXPECT_EQ(Q101.mChildBoundaryForQH, Q100.backIt());
    //EXPECT_EQ(Q101.mQHFullOnListHead, true);
    //EXPECT_EQ(Q101.mA, 5);
    //EXPECT_EQ(Q101.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q101.mChildBoundaryOnHeadForQA, Q98.backIt());
    //EXPECT_EQ(Q101.mChildBoundaryOnTailForQA, Q100.backIt());
    EXPECT_EQ(Q101.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q98.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q99.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q100.mWHABType, JTSTreeNode::CD);

    // Case 25: Q Full Partial Full
    auto& Q102 = createQ(pqTree,emptyChildren);
    Q102.mGeneration = pqTree.mGeneration;
    Q102.mPertinentType = JTSTreeNode::Full;
    Q102.mW = 6;
    Q102.mH = 0;
    Q102.mA = 0;
    Q102.mCD = 0;

    auto& Q103 = createQ(pqTree,emptyChildren);
    Q103.mGeneration = pqTree.mGeneration;
    Q103.mPertinentType = JTSTreeNode::Partial;
    Q103.mW = 9;
    Q103.mH = 5;
    Q103.mA = 4;
    Q103.mCD = 4;

    auto& Q104 = createQ(pqTree,emptyChildren);
    Q104.mGeneration = pqTree.mGeneration;
    Q104.mPertinentType = JTSTreeNode::Full;
    Q104.mW = 10;
    Q104.mH = 0;
    Q104.mA = 0;
    Q104.mCD = 0;

    list<node_list_it_t> Q105children;
    Q105children.push_back(Q102.backIt());
    Q105children.push_back(Q103.backIt());
    Q105children.push_back(Q104.backIt());

    auto& Q105 = createQ(pqTree, Q105children);
    Q105.mPertinentType = JTSTreeNode::Partial;
    calculateWHAC(Q105);
    determineRootNodeType(Q105);
    determineWHABCTypeForChildren(Q105);

    //EXPECT_EQ(Q105.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q105.mW, 25);
    //EXPECT_EQ(Q105.mH, 11);
    //EXPECT_EQ(Q105.mChildBoundaryForQH, Q103.backIt());
    //EXPECT_EQ(Q105.mQHFullOnListHead, false);
    //EXPECT_EQ(Q105.mA, 11);
    //EXPECT_EQ(Q105.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q105.mChildBoundaryOnHeadForQA, Q103.backIt());
    //EXPECT_EQ(Q105.mChildBoundaryOnTailForQA, Q104.backIt());
    EXPECT_EQ(Q105.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q102.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q103.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q104.mWHABType, JTSTreeNode::B);

    // Case 26: Q Full Full Full
    auto& Q106 = createQ(pqTree,emptyChildren);
    Q106.mGeneration = pqTree.mGeneration;
    Q106.mPertinentType = JTSTreeNode::Full;
    Q106.mW = 6;
    Q106.mH = 0;
    Q106.mA = 0;
    Q106.mCD = 0;

    auto& Q107 = createQ(pqTree,emptyChildren);
    Q107.mGeneration = pqTree.mGeneration;
    Q107.mPertinentType = JTSTreeNode::Full;
    Q107.mW = 7;
    Q107.mH = 0;
    Q107.mA = 0;
    Q107.mCD = 0;

    auto& Q108 = createQ(pqTree,emptyChildren);
    Q108.mGeneration = pqTree.mGeneration;
    Q108.mPertinentType = JTSTreeNode::Full;
    Q108.mW = 8;
    Q108.mH = 0;
    Q108.mA = 0;
    Q108.mCD = 0;

    list<node_list_it_t> Q109children;
    Q109children.push_back(Q106.backIt());
    Q109children.push_back(Q107.backIt());
    Q109children.push_back(Q108.backIt());

    auto& Q109 = createQ(pqTree, Q109children);
    Q109.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q109);
    determineRootNodeType(Q109);
    determineWHABCTypeForChildren(Q109);

    //EXPECT_EQ(Q109.mPertinentType, JTSTreeNode::Full);
    //EXPECT_EQ(Q109.mW, 21);
    //EXPECT_EQ(Q109.mH, 0);
    //EXPECT_EQ(Q109.mA, 0);
    EXPECT_EQ(Q109.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q106.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q107.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q108.mWHABType, JTSTreeNode::B);


    // Case 27: Q Empty Empty Partial
    auto& Q110 = createQ(pqTree,emptyChildren);

    auto& Q111 = createQ(pqTree,emptyChildren);

    auto& Q112 = createQ(pqTree,emptyChildren);
    Q112.mGeneration = pqTree.mGeneration;
    Q112.mPertinentType = JTSTreeNode::Partial;
    Q112.mW = 20;
    Q112.mH = 8;
    Q112.mA = 6;
    Q112.mCD = 7;

    list<node_list_it_t> Q113children;
    Q113children.push_back(Q110.backIt());
    Q113children.push_back(Q111.backIt());
    Q113children.push_back(Q112.backIt());

    auto& Q113 = createQ(pqTree, Q113children);
    Q113.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q113);
    determineRootNodeType(Q113);
    determineWHABCTypeForChildren(Q113);

    //EXPECT_EQ(Q113.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q113.mW, 20);
    //EXPECT_EQ(Q113.mH, 8);
    //EXPECT_EQ(Q113.mChildBoundaryForQH, Q112.backIt());
    //EXPECT_EQ(Q113.mQHFullOnListHead, false);
    //EXPECT_EQ(Q113.mA, 6);
    //EXPECT_EQ(Q113.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q113.mChildMaxWminusA,  Q112.backIt());
    EXPECT_EQ(Q113.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q112.mWHABType, JTSTreeNode::A);

    // Case 28: Q Partial Empty Empty
    auto& Q114 = createQ(pqTree,emptyChildren);
    Q114.mGeneration = pqTree.mGeneration;
    Q114.mPertinentType = JTSTreeNode::Partial;
    Q114.mW = 20;
    Q114.mH = 8;
    Q114.mA = 6;
    Q114.mCD = 7;

    auto& Q115 = createQ(pqTree,emptyChildren);

    auto& Q116 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q117children;
    Q117children.push_back(Q114.backIt());
    Q117children.push_back(Q115.backIt());
    Q117children.push_back(Q116.backIt());

    auto& Q117 = createQ(pqTree, Q117children);
    Q117.mPertinentType = JTSTreeNode::Partial;
    calculateWHAC(Q117);
    determineRootNodeType(Q117);
    determineWHABCTypeForChildren(Q117);

    //EXPECT_EQ(Q117.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q117.mW, 20);
    //EXPECT_EQ(Q117.mH, 8);
    //EXPECT_EQ(Q117.mChildBoundaryForQH, Q114.backIt());
    //EXPECT_EQ(Q117.mQHFullOnListHead, true);
    //EXPECT_EQ(Q117.mA, 6);
    //EXPECT_EQ(Q117.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q117.mChildMaxWminusA,  Q114.backIt());
    EXPECT_EQ(Q117.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q114.mWHABType, JTSTreeNode::A);


    // Case 29: Q Empty Full Partial
    auto& Q118 = createQ(pqTree,emptyChildren);

    auto& Q119 = createQ(pqTree,emptyChildren);
    Q119.mGeneration = pqTree.mGeneration;
    Q119.mPertinentType = JTSTreeNode::Full;
    Q119.mW = 20;
    Q119.mH = 0;
    Q119.mA = 0;
    Q119.mCD = 0;

    auto& Q120 = createQ(pqTree,emptyChildren);
    Q120.mGeneration = pqTree.mGeneration;
    Q120.mPertinentType = JTSTreeNode::Partial;
    Q120.mW = 25;
    Q120.mH = 8;
    Q120.mA = 6;
    Q120.mCD = 5;

    list<node_list_it_t> Q121children;
    Q121children.push_back(Q118.backIt());
    Q121children.push_back(Q119.backIt());
    Q121children.push_back(Q120.backIt());

    auto& Q121 = createQ(pqTree, Q121children);
    Q121.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q121);
    determineRootNodeType(Q121);
    determineWHABCTypeForChildren(Q121);

    //EXPECT_EQ(Q121.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q121.mW, 45);
    //EXPECT_EQ(Q121.mH, 28);
    //EXPECT_EQ(Q121.mChildBoundaryForQH, Q120.backIt());
    //EXPECT_EQ(Q121.mQHFullOnListHead, false);
    //EXPECT_EQ(Q121.mA, 8);
    //EXPECT_EQ(Q121.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q121.mChildBoundaryOnHeadForQA, Q119.backIt());
    //EXPECT_EQ(Q121.mChildBoundaryOnTailForQA, Q120.backIt());
    EXPECT_EQ(Q121.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q119.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q120.mWHABType, JTSTreeNode::H);

    // Case 30: Q Partial Full Empty
    auto& Q122 = createQ(pqTree,emptyChildren);
    Q122.mGeneration = pqTree.mGeneration;
    Q122.mPertinentType = JTSTreeNode::Partial;
    Q122.mW = 25;
    Q122.mH = 8;
    Q122.mA = 6;
    Q122.mCD = 5;

    auto& Q123 = createQ(pqTree,emptyChildren);
    Q123.mGeneration = pqTree.mGeneration;
    Q123.mPertinentType = JTSTreeNode::Full;
    Q123.mW = 20;
    Q123.mH = 0;
    Q123.mA = 0;
    Q123.mCD = 0;

    auto& Q124 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q125children;
    Q125children.push_back(Q122.backIt());
    Q125children.push_back(Q123.backIt());
    Q125children.push_back(Q124.backIt());

    auto& Q125 = createQ(pqTree, Q125children);
    Q125.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q125);
    determineRootNodeType(Q125);
    determineWHABCTypeForChildren(Q125);

    //EXPECT_EQ(Q125.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q125.mW, 45);
    //EXPECT_EQ(Q125.mH, 28);
    //EXPECT_EQ(Q125.mChildBoundaryForQH, Q122.backIt());
    //EXPECT_EQ(Q125.mQHFullOnListHead, true);
    //EXPECT_EQ(Q125.mA, 8);
    //EXPECT_EQ(Q125.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q125.mChildBoundaryOnHeadForQA, Q122.backIt());
    //EXPECT_EQ(Q125.mChildBoundaryOnTailForQA, Q123.backIt());
    EXPECT_EQ(Q125.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q122.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q123.mWHABType, JTSTreeNode::B);

    // Case 31: Q Full  Empty Full
    auto& Q126 = createQ(pqTree,emptyChildren);
    Q126.mGeneration = pqTree.mGeneration;
    Q126.mPertinentType = JTSTreeNode::Full;
    Q126.mW = 25;
    Q126.mH = 0;
    Q126.mA = 0;
    Q126.mCD = 0;

    auto& Q127 = createQ(pqTree,emptyChildren);

    auto& Q128 = createQ(pqTree,emptyChildren);
    Q128.mGeneration = pqTree.mGeneration;
    Q128.mPertinentType = JTSTreeNode::Full;
    Q128.mW = 10;
    Q128.mH = 0;
    Q128.mA = 0;
    Q128.mCD = 0;

    list<node_list_it_t> Q129children;
    Q129children.push_back(Q126.backIt());
    Q129children.push_back(Q127.backIt());
    Q129children.push_back(Q128.backIt());

    auto& Q129 = createQ(pqTree, Q129children);
    Q129.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q129);
    determineRootNodeType(Q129);
    determineWHABCTypeForChildren(Q129);

    //EXPECT_EQ(Q129.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q129.mW, 35);
    //EXPECT_EQ(Q129.mH, 10);
    //EXPECT_EQ(Q129.mChildBoundaryForQH, Q126.backIt());
    //EXPECT_EQ(Q129.mQHFullOnListHead, true);
    //EXPECT_EQ(Q129.mA, 10);
    //EXPECT_EQ(Q129.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q129.mChildMaxWminusA,  Q126.backIt());
    EXPECT_EQ(Q129.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q126.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q128.mWHABType, JTSTreeNode::B);

    // Case 32: Q Partial  Empty Partial
    auto& Q130 = createQ(pqTree,emptyChildren);
    Q130.mGeneration = pqTree.mGeneration;
    Q130.mPertinentType = JTSTreeNode::Partial;
    Q130.mW = 25;
    Q130.mH = 7;
    Q130.mA = 4;
    Q130.mCD = 3;

    auto& Q131 = createQ(pqTree,emptyChildren);

    auto& Q132 = createQ(pqTree,emptyChildren);
    Q132.mGeneration = pqTree.mGeneration;
    Q132.mPertinentType = JTSTreeNode::Partial;
    Q132.mW = 17;
    Q132.mH = 5;
    Q132.mA = 5;
    Q132.mCD = 5;

    list<node_list_it_t> Q133children;
    Q133children.push_back(Q130.backIt());
    Q133children.push_back(Q131.backIt());
    Q133children.push_back(Q132.backIt());

    auto& Q133 = createQ(pqTree, Q133children);
    Q133.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q133);
    determineRootNodeType(Q133);
    determineWHABCTypeForChildren(Q133);

    //EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q133.mW, 42);
    //EXPECT_EQ(Q133.mH, 24);
    //EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    //EXPECT_EQ(Q133.mQHFullOnListHead, true);
    //EXPECT_EQ(Q133.mA, 21);
    //EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(Q133.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(Q130.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q132.mWHABType, JTSTreeNode::H);


    // Case 33: Q Full Full CDPartial Full Full
    auto& P135 = createP(pqTree,emptyChildren);
    P135.mGeneration = pqTree.mGeneration;
    P135.mPertinentType = JTSTreeNode::Full;
    P135.mW = 20;
    P135.mH = 0;
    P135.mA = 0;
    P135.mCD = 0;

    auto& P136 = createP(pqTree,emptyChildren);
    P136.mGeneration = pqTree.mGeneration;
    P136.mPertinentType = JTSTreeNode::Full;
    P136.mW = 20;
    P136.mH = 0;
    P136.mA = 0;
    P136.mCD = 0;

    auto& Q137 = createQ(pqTree,emptyChildren);
    Q137.mGeneration = pqTree.mGeneration;
    Q137.mPertinentType = JTSTreeNode::Partial;
    Q137.mW  = 17;
    Q137.mH  = 17;
    Q137.mA  = 17;
    Q137.mCD = 2;

    auto& P138 = createP(pqTree,emptyChildren);
    P138.mGeneration = pqTree.mGeneration;
    P138.mPertinentType = JTSTreeNode::Full;
    P138.mW = 20;
    P138.mH = 0;
    P138.mA = 0;
    P138.mCD = 0;

    auto& P139 = createP(pqTree,emptyChildren);
    P139.mGeneration = pqTree.mGeneration;
    P139.mPertinentType = JTSTreeNode::Full;
    P139.mW = 20;
    P139.mH = 0;
    P139.mA = 0;
    P139.mCD = 0;

    list<node_list_it_t> Q140children;
    Q140children.push_back(P135.backIt());
    Q140children.push_back(P136.backIt());
    Q140children.push_back(Q137.backIt());
    Q140children.push_back(P138.backIt());
    Q140children.push_back(P139.backIt());

    auto& Q140 = createQ(pqTree, Q140children);
    Q140.mGeneration = pqTree.mGeneration;
    Q140.mPertinentType = JTSTreeNode::Partial;

    calculateWHAC(Q140);

    determineRootNodeType(Q140);
    determineWHABCTypeForChildren(Q140);
    //EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q133.mW, 42);
    //EXPECT_EQ(Q133.mH, 24);
    //EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    //EXPECT_EQ(Q133.mQHFullOnListHead, true);
    //EXPECT_EQ(Q133.mA, 21);
    //EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(P135.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P136.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q137.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(P138.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P139.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q140.mWHABType, JTSTreeNode::CD);


    // Case 34: Q Full Full Partial Full Full
    auto& P141 = createP(pqTree,emptyChildren);
    P141.mGeneration = pqTree.mGeneration;
    P141.mPertinentType = JTSTreeNode::Full;
    P141.mW = 20;
    P141.mH = 0;
    P141.mA = 0;
    P141.mCD = 0;

    auto& P142 = createP(pqTree,emptyChildren);
    P142.mGeneration = pqTree.mGeneration;
    P142.mPertinentType = JTSTreeNode::Full;
    P142.mW = 20;
    P142.mH = 0;
    P142.mA = 0;
    P142.mCD = 0;

    auto& Q143 = createQ(pqTree,emptyChildren);
    Q143.mGeneration = pqTree.mGeneration;
    Q143.mPertinentType = JTSTreeNode::Partial;
    Q143.mW  = 17;
    Q143.mH  = 17;
    Q143.mA  = 17;
    Q143.mCD = 19;

    auto& P144 = createP(pqTree,emptyChildren);
    P144.mGeneration = pqTree.mGeneration;
    P144.mPertinentType = JTSTreeNode::Full;
    P144.mW = 20;
    P144.mH = 0;
    P144.mA = 0;
    P144.mCD = 0;

    auto& P145 = createP(pqTree,emptyChildren);
    P145.mGeneration = pqTree.mGeneration;
    P145.mPertinentType = JTSTreeNode::Full;
    P145.mW = 20;
    P145.mH = 0;
    P145.mA = 0;
    P145.mCD = 0;

    list<node_list_it_t> Q146children;
    Q146children.push_back(P141.backIt());
    Q146children.push_back(P142.backIt());
    Q146children.push_back(Q143.backIt());
    Q146children.push_back(P144.backIt());
    Q146children.push_back(P145.backIt());

    auto& Q146 = createQ(pqTree, Q146children);
    Q146.mGeneration = pqTree.mGeneration;
    Q146.mPertinentType = JTSTreeNode::Partial;

    calculateWHAC(Q146);

    determineRootNodeType(Q146);
    determineWHABCTypeForChildren(Q146);
    //EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q133.mW, 42);
    //EXPECT_EQ(Q133.mH, 24);
    //EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    //EXPECT_EQ(Q133.mQHFullOnListHead, true);
    //EXPECT_EQ(Q133.mA, 21);
    //EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(Q146.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(P141.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P142.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q143.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(P144.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P145.mWHABType, JTSTreeNode::B);


    // Case 35: Q Full Full Empty Full Full
    auto& P147 = createP(pqTree,emptyChildren);
    P147.mGeneration = pqTree.mGeneration;
    P147.mPertinentType = JTSTreeNode::Full;
    P147.mW = 20;
    P147.mH = 0;
    P147.mA = 0;
    P147.mCD = 0;

    auto& P148 = createP(pqTree,emptyChildren);
    P148.mGeneration = pqTree.mGeneration;
    P148.mPertinentType = JTSTreeNode::Full;
    P148.mW = 20;
    P148.mH = 0;
    P148.mA = 0;
    P148.mCD = 0;

    auto& Q149 = createQ(pqTree,emptyChildren);

    auto& P150 = createP(pqTree,emptyChildren);
    P150.mGeneration = pqTree.mGeneration;
    P150.mPertinentType = JTSTreeNode::Full;
    P150.mW = 20;
    P150.mH = 0;
    P150.mA = 0;
    P150.mCD = 0;

    auto& P151 = createP(pqTree,emptyChildren);
    P151.mGeneration = pqTree.mGeneration;
    P151.mPertinentType = JTSTreeNode::Full;
    P151.mW = 20;
    P151.mH = 0;
    P151.mA = 0;
    P151.mCD = 0;

    list<node_list_it_t> Q152children;
    Q152children.push_back(P147.backIt());
    Q152children.push_back(P148.backIt());
    Q152children.push_back(Q149.backIt());
    Q152children.push_back(P150.backIt());
    Q152children.push_back(P151.backIt());

    auto& Q152 = createQ(pqTree, Q152children);
    Q152.mGeneration = pqTree.mGeneration;
    Q152.mPertinentType = JTSTreeNode::Partial;

    calculateWHAC(Q152);

    determineRootNodeType(Q152);
    determineWHABCTypeForChildren(Q152);
    //EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q133.mW, 42);
    //EXPECT_EQ(Q133.mH, 24);
    //EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    //EXPECT_EQ(Q133.mQHFullOnListHead, true);
    //EXPECT_EQ(Q133.mA, 21);
    //EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(Q152.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(P147.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P148.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P150.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P151.mWHABType, JTSTreeNode::B);


    // Case 36: P Full Full Empty Full Full
    auto& P153 = createP(pqTree,emptyChildren);
    P153.mGeneration = pqTree.mGeneration;
    P153.mPertinentType = JTSTreeNode::Full;
    P153.mW = 20;
    P153.mH = 0;
    P153.mA = 0;
    P153.mCD = 0;

    auto& P154 = createP(pqTree,emptyChildren);
    P154.mGeneration = pqTree.mGeneration;
    P154.mPertinentType = JTSTreeNode::Full;
    P154.mW = 20;
    P154.mH = 0;
    P154.mA = 0;
    P154.mCD = 0;

    auto& Q155 = createQ(pqTree,emptyChildren);
    Q155.mGeneration = pqTree.mGeneration;
    Q155.mPertinentType = JTSTreeNode::Partial;
    Q155.mW  = 30;
    Q155.mH  = 17;
    Q155.mA  = 17;
    Q155.mCD = 10;

    auto& P156 = createP(pqTree,emptyChildren);
    P156.mGeneration = pqTree.mGeneration;
    P156.mPertinentType = JTSTreeNode::Full;
    P156.mW = 20;
    P156.mH = 0;
    P156.mA = 0;
    P156.mCD = 0;

    auto& P157 = createP(pqTree,emptyChildren);
    P157.mGeneration = pqTree.mGeneration;
    P157.mPertinentType = JTSTreeNode::Full;
    P157.mW = 20;
    P157.mH = 0;
    P157.mA = 0;
    P157.mCD = 0;

    list<node_list_it_t> P158children;
    P158children.push_back(P153.backIt());
    P158children.push_back(P154.backIt());
    P158children.push_back(Q155.backIt());
    P158children.push_back(P156.backIt());
    P158children.push_back(P157.backIt());

    auto& P158 = createP(pqTree, P158children);
    P158.mGeneration = pqTree.mGeneration;
    P158.mPertinentType = JTSTreeNode::Partial;

    calculateWHAC(P158);

    determineRootNodeType(P158);
    determineWHABCTypeForChildren(P158);
    //EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q133.mW, 42);
    //EXPECT_EQ(Q133.mH, 24);
    //EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    //EXPECT_EQ(Q133.mQHFullOnListHead, true);
    //EXPECT_EQ(Q133.mA, 21);
    //EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(P158.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(P153.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P154.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q155.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(P156.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P157.mWHABType, JTSTreeNode::B);

}


/**  @brief tests JTSTreeNode::determineWHABCTypeForChildren() #2
 *                The top node is of type H.
 */
TEST_F(JTSPlanarizerTests, Test9) {

    JTSTree& pqTree = getPQTree();

    initializeForOneIteration(pqTree);

    // Case 1: Node L
    auto& L01 = createL(pqTree);
    L01.mGeneration = pqTree.mGeneration;
    calculateWHAC(L01);
    determineRootNodeType(L01);
    determineWHABCTypeForChildren(L01);
    //EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    //EXPECT_EQ(L01.mW, 1);
    //EXPECT_EQ(L01.mH, 0);
    //EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);

    // Case 2: Node P Empty Empty Full
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);
    auto& L04 = createL(pqTree);
    L04.mGeneration = pqTree.mGeneration;
    calculateWHAC(L04);

    list<node_list_it_t> P01children;
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    P01children.push_back(L04.backIt());
    auto& P01 = createP(pqTree, P01children);
    L04.mGeneration = pqTree.mGeneration;
    calculateWHAC(P01);
    P01.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P01);
    //EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P01.mW, 1);
    //EXPECT_EQ(P01.mH, 0);
    //EXPECT_EQ(P01.mA, 0);
    //EXPECT_EQ(P01.mChildMax1WminusH, pqTree.nil());
    //EXPECT_EQ(P01.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P01.mChildMaxWminusA,  L04.backIt());
    //EXPECT_EQ(P01.mTypeAwithSingleA, true);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);

    // Case 3: Node P Full Empty Full
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);
    auto& L07 = createL(pqTree);
    L05.mGeneration = pqTree.mGeneration;
    calculateWHAC(L05);
    L07.mGeneration = pqTree.mGeneration;
    calculateWHAC(L07);

    list<node_list_it_t> P02children;
    P02children.push_back(L05.backIt());
    P02children.push_back(L06.backIt());
    P02children.push_back(L07.backIt());
    auto& P02 = createP(pqTree, P02children);
    P02.mGeneration = pqTree.mGeneration;
    calculateWHAC(P02);
    P02.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P02);
    //EXPECT_EQ(P02.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P02.mW, 2);
    //EXPECT_EQ(P02.mH, 0);
    //EXPECT_EQ(P02.mA, 0);
    //EXPECT_EQ(P02.mChildMax1WminusH, pqTree.nil());
    //EXPECT_EQ(P02.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P02.mTypeAwithSingleA, false);
    EXPECT_EQ(P02.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L07.mWHABType, JTSTreeNode::B);


    // Case 4: Node P Full Full Full
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);
    auto& L10 = createL(pqTree);
    L08.mGeneration = pqTree.mGeneration;
    calculateWHAC(L08);
    L09.mGeneration = pqTree.mGeneration;
    calculateWHAC(L09);
    L10.mGeneration = pqTree.mGeneration;
    calculateWHAC(L10);

    list<node_list_it_t> P03children;
    P03children.push_back(L08.backIt());
    P03children.push_back(L09.backIt());
    P03children.push_back(L10.backIt());
    auto& P03 = createP(pqTree, P03children);
    P03.mGeneration = pqTree.mGeneration;
    calculateWHAC(P03);
    P03.mWHABType = JTSTreeNode::B;
    determineWHABCTypeForChildren(P03);
    //EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Full);
    //EXPECT_EQ(P03.mW, 3);
    //EXPECT_EQ(P03.mH, 0);
    //EXPECT_EQ(P03.mA, 0);
    //EXPECT_EQ(P03.mChildMax1WminusH, pqTree.nil());
    //EXPECT_EQ(P03.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P03.mTypeAwithSingleA, false);
    EXPECT_EQ(P03.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L08.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L09.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L10.mWHABType, JTSTreeNode::B);


    // Case 5: Node P Empty Empty Partial(W=H)
    // 
    // NOTE1: If a partial node has W=H, it means it can't be of type H
    //        (making it of type H will make the node empty.)
    // NOTE2: There are cases where mChildMax1WminusH and/or mChildMax1WminusH 
    //        is empty if there are not enough partial children.
    //      
    list<node_list_it_t> emptyChildren;

    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);
    auto& Q01 = createQ(pqTree,emptyChildren);
    Q01.mGeneration = pqTree.mGeneration;
    Q01.mPertinentType = JTSTreeNode::Partial;
    Q01.mW = 10;
    Q01.mH = 10;
    Q01.mA = 5;
    Q01.mCD = 5;

    list<node_list_it_t> P04children;
    P04children.push_back(L11.backIt());
    P04children.push_back(L12.backIt());
    P04children.push_back(Q01.backIt());
    auto& P04 = createP(pqTree, P04children);
    P04.mGeneration = pqTree.mGeneration;
    calculateWHAC(P04);
    P04.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P04);
    //EXPECT_EQ(P04.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P04.mW, 10);
    //EXPECT_EQ(P04.mH, 10);
    //EXPECT_EQ(P04.mA, 5);
    //EXPECT_EQ(P04.mChildMax1WminusH, Q01.backIt());
    //EXPECT_EQ(P04.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P04.mTypeAwithSingleA, true);
    //EXPECT_EQ(P04.mChildMaxWminusA,  Q01.backIt());
    EXPECT_EQ(P04.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::H);

    // Case 6: Node P Empty Empty Partial(W>H)

    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);
    auto& Q02 = createQ(pqTree,emptyChildren);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentType = JTSTreeNode::Partial;
    Q02.mW = 20;
    Q02.mH = 7;
    Q02.mA = 5;
    Q02.mCD = 5;

    list<node_list_it_t> P05children;
    P05children.push_back(L13.backIt());
    P05children.push_back(L14.backIt());
    P05children.push_back(Q02.backIt());
    auto& P05 = createP(pqTree, P05children);
    P05.mGeneration = pqTree.mGeneration;
    calculateWHAC(P05);
    P05.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P05);
    //EXPECT_EQ(P05.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P05.mW, 20);
    //EXPECT_EQ(P05.mH, 7);
    //EXPECT_EQ(P05.mA, 5);
    //EXPECT_EQ(P05.mChildMax1WminusH, Q02.backIt());
    //EXPECT_EQ(P05.mChildMax2WminusH, pqTree.nil());
    //EXPECT_EQ(P05.mTypeAwithSingleA, true);
    //EXPECT_EQ(P05.mChildMaxWminusA,  Q02.backIt());
    EXPECT_EQ(P05.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q02.mWHABType, JTSTreeNode::H);

    // Case 7: Node P Empty Partial(W=H) Partial(W=H)

    auto& L15 = createL(pqTree);
    auto& Q03 = createQ(pqTree,emptyChildren);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mW = 11;
    Q03.mH = 11;
    Q03.mA = 6;
    Q03.mCD = 6;
    auto& Q04 = createQ(pqTree,emptyChildren);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mW = 17;
    Q04.mH = 17;
    Q04.mA = 3;
    Q04.mCD = 3;

    list<node_list_it_t> P06children;
    P06children.push_back(L15.backIt());
    P06children.push_back(Q03.backIt());
    P06children.push_back(Q04.backIt());
    auto& P06 = createP(pqTree, P06children);
    P06.mGeneration = pqTree.mGeneration;
    calculateWHAC(P06);
    P06.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P06);
    //EXPECT_EQ(P06.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P06.mW, 28);
    //EXPECT_EQ(P06.mH, 28);
    //EXPECT_EQ(P06.mA, 14);
    //EXPECT_EQ(P06.mChildMax1WminusH, Q04.backIt());
    //EXPECT_EQ(P06.mChildMax2WminusH, Q03.backIt());
    //EXPECT_EQ(P06.mTypeAwithSingleA, true);
    //EXPECT_EQ(P06.mChildMaxWminusA,  Q04.backIt());
    EXPECT_EQ(P06.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q04.mWHABType, JTSTreeNode::H);

    // Case 8: Node P Empty Partial(W>H) Partial(W>H)

    auto& L16 = createL(pqTree);
    auto& Q05 = createQ(pqTree,emptyChildren);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = JTSTreeNode::Partial;
    Q05.mW = 30;
    Q05.mH = 11;
    Q05.mA = 6;
    Q05.mCD = 6;

    auto& Q06 = createQ(pqTree,emptyChildren);
    Q06.mGeneration = pqTree.mGeneration;
    Q06.mPertinentType = JTSTreeNode::Partial;
    Q06.mW = 40;
    Q06.mH = 17;
    Q06.mA = 3;
    Q06.mCD = 14;

    list<node_list_it_t> P07children;
    P07children.push_back(L16.backIt());
    P07children.push_back(Q05.backIt());
    P07children.push_back(Q06.backIt());
    auto& P07 = createP(pqTree, P07children);
    P07.mGeneration = pqTree.mGeneration;
    calculateWHAC(P07);
    P07.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P07);
    //EXPECT_EQ(P07.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P07.mW, 70);
    //EXPECT_EQ(P07.mH, 47);
    //EXPECT_EQ(P07.mA, 28);
    //EXPECT_EQ(P07.mChildMax1WminusH, Q06.backIt());
    //EXPECT_EQ(P07.mChildMax2WminusH, Q05.backIt());
    //EXPECT_EQ(P07.mTypeAwithSingleA, false);
    EXPECT_EQ(P07.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q05.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q06.mWHABType, JTSTreeNode::H);

    // Case 9.1: Node P Empty Partial(W>H) Partial(W>H)

    auto& L17 = createL(pqTree);
    auto& Q07 = createQ(pqTree,emptyChildren);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = JTSTreeNode::Partial;
    Q07.mW = 30;
    Q07.mH = 14;
    Q07.mA = 6;
    Q07.mCD = 10;

    auto& Q08 = createQ(pqTree,emptyChildren);
    Q08.mGeneration = pqTree.mGeneration;
    Q08.mPertinentType = JTSTreeNode::Partial;
    Q08.mW = 40;
    Q08.mH = 20;
    Q08.mA = 3;
    Q08.mCD = 10;

    list<node_list_it_t> P08children;
    P08children.push_back(L17.backIt());
    P08children.push_back(Q07.backIt());
    P08children.push_back(Q08.backIt());
    auto& P08 = createP(pqTree, P08children);
    P08.mGeneration = pqTree.mGeneration;
    calculateWHAC(P08);
    P08.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P08);
    //EXPECT_EQ(P08.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P08.mW, 70);
    //EXPECT_EQ(P08.mH, 50);
    //EXPECT_EQ(P08.mA, 33);
    //EXPECT_EQ(P08.mChildMax1WminusH, Q08.backIt());
    //EXPECT_EQ(P08.mChildMax2WminusH, Q07.backIt());
    //EXPECT_EQ(P08.mTypeAwithSingleA, true);
    //EXPECT_EQ(P08.mChildMaxWminusA,  Q08.backIt());
    EXPECT_EQ(P08.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q07.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q08.mWHABType, JTSTreeNode::H);

    // Case 9.2: Node P Full Partial(W>H) Partial(W>H)

    auto& Q09 = createQ(pqTree,emptyChildren);
    Q09.mGeneration = pqTree.mGeneration;
    Q09.mPertinentType = JTSTreeNode::Full;
    Q09.mW = 2;
    Q09.mH = 0;
    Q09.mA = 0;
    Q09.mCD = 0;

    auto& Q10 = createQ(pqTree,emptyChildren);
    Q10.mGeneration = pqTree.mGeneration;
    Q10.mPertinentType = JTSTreeNode::Partial;
    Q10.mW = 30;
    Q10.mH = 14;
    Q10.mA = 6;
    Q10.mCD = 12;
    auto& Q11 = createQ(pqTree,emptyChildren);
    Q11.mGeneration = pqTree.mGeneration;
    Q11.mPertinentType = JTSTreeNode::Partial;
    Q11.mW = 40;
    Q11.mH = 20;
    Q11.mA = 3;
    Q11.mCD = 10;

    list<node_list_it_t> P09children;
    P09children.push_back(Q09.backIt());
    P09children.push_back(Q10.backIt());
    P09children.push_back(Q11.backIt());
    auto& P09 = createP(pqTree, P09children);
    P09.mGeneration = pqTree.mGeneration;
    calculateWHAC(P09);
    P09.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P09);
    //EXPECT_EQ(P09.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P09.mW, 72);
    //EXPECT_EQ(P09.mH, 50);
    //EXPECT_EQ(P09.mA, 34);
    //EXPECT_EQ(P09.mChildMax1WminusH, Q11.backIt());
    //EXPECT_EQ(P09.mChildMax2WminusH, Q10.backIt());
    //EXPECT_EQ(P09.mTypeAwithSingleA, false);
    EXPECT_EQ(P09.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q09.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q10.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q11.mWHABType, JTSTreeNode::H);

    // Case 10: Node P Full Full Full Partial(W=H) Partial(W=H)

    auto& Q12 = createQ(pqTree,emptyChildren);
    Q12.mGeneration = pqTree.mGeneration;
    Q12.mPertinentType = JTSTreeNode::Full;
    Q12.mW = 5;
    Q12.mH = 0;
    Q12.mA = 0;
    Q12.mCD = 0;

    auto& Q13 = createQ(pqTree,emptyChildren);
    Q13.mGeneration = pqTree.mGeneration;
    Q13.mPertinentType = JTSTreeNode::Full;
    Q13.mW = 4;
    Q13.mH = 0;
    Q13.mA = 0;
    Q13.mCD = 0;

    auto& Q14 = createQ(pqTree,emptyChildren);
    Q14.mGeneration = pqTree.mGeneration;
    Q14.mPertinentType = JTSTreeNode::Full;
    Q14.mW = 7;
    Q14.mH = 0;
    Q14.mA = 0;
    Q14.mCD = 0;

    auto& Q15 = createQ(pqTree,emptyChildren);
    Q15.mGeneration = pqTree.mGeneration;
    Q15.mPertinentType = JTSTreeNode::Partial;
    Q15.mW = 30;
    Q15.mH = 30;
    Q15.mA = 6;
    Q15.mCD = 6;

    auto& Q16 = createQ(pqTree,emptyChildren);
    Q16.mGeneration = pqTree.mGeneration;
    Q16.mPertinentType = JTSTreeNode::Partial;
    Q16.mW = 40;
    Q16.mH = 40;
    Q16.mA = 3;
    Q16.mCD = 3;

    list<node_list_it_t> P10children;
    P10children.push_back(Q12.backIt());
    P10children.push_back(Q13.backIt());
    P10children.push_back(Q14.backIt());
    P10children.push_back(Q15.backIt());
    P10children.push_back(Q16.backIt());

    auto& P10 = createP(pqTree, P10children);
    P10.mGeneration = pqTree.mGeneration;
    calculateWHAC(P10);
    P10.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P10);
    //EXPECT_EQ(P10.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P10.mW, 86);
    //EXPECT_EQ(P10.mH, 70);
    //EXPECT_EQ(P10.mA, 49);
    //EXPECT_EQ(P10.mChildMax1WminusH, Q16.backIt());
    //EXPECT_EQ(P10.mChildMax2WminusH, Q15.backIt());
    //EXPECT_EQ(P10.mTypeAwithSingleA, true);
    //EXPECT_EQ(P10.mChildMaxWminusA,  Q16.backIt());
    EXPECT_EQ(P10.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q12.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q13.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q14.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q15.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q16.mWHABType, JTSTreeNode::H);


    // Case 11: Node P Full Full Full Partial(W=H) Partial(W=H) #2

    auto& Q17 = createQ(pqTree,emptyChildren);
    Q17.mGeneration = pqTree.mGeneration;
    Q17.mPertinentType = JTSTreeNode::Full;
    Q17.mW = 15;
    Q17.mH = 0;
    Q17.mA = 0;
    Q17.mCD = 0;

    auto& Q18 = createQ(pqTree,emptyChildren);
    Q18.mGeneration = pqTree.mGeneration;
    Q18.mPertinentType = JTSTreeNode::Full;
    Q18.mW = 14;
    Q18.mH = 0;
    Q18.mA = 0;
    Q18.mCD = 0;

    auto& Q19 = createQ(pqTree,emptyChildren);
    Q19.mGeneration = pqTree.mGeneration;
    Q19.mPertinentType = JTSTreeNode::Full;
    Q19.mW = 17;
    Q19.mH = 0;
    Q19.mA = 0;
    Q19.mCD = 0;

    auto& Q20 = createQ(pqTree,emptyChildren);
    Q20.mGeneration = pqTree.mGeneration;
    Q20.mPertinentType = JTSTreeNode::Partial;
    Q20.mW = 30;
    Q20.mH = 30;
    Q20.mA = 6;
    Q20.mCD = 6;

    auto& Q21 = createQ(pqTree,emptyChildren);
    Q21.mGeneration = pqTree.mGeneration;
    Q21.mPertinentType = JTSTreeNode::Partial;
    Q21.mW = 40;
    Q21.mH = 40;
    Q21.mA = 3;
    Q21.mCD = 3;

    list<node_list_it_t> P11children;
    P11children.push_back(Q17.backIt());
    P11children.push_back(Q18.backIt());
    P11children.push_back(Q19.backIt());
    P11children.push_back(Q20.backIt());
    P11children.push_back(Q21.backIt());

    auto& P11 = createP(pqTree, P11children);
    P11.mGeneration = pqTree.mGeneration;
    calculateWHAC(P11);
    P11.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P11);
    //EXPECT_EQ(P11.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P11.mW, 116);
    //EXPECT_EQ(P11.mH, 70);
    //EXPECT_EQ(P11.mA, 70);
    //EXPECT_EQ(P11.mChildMax1WminusH, Q21.backIt());
    //EXPECT_EQ(P11.mChildMax2WminusH, Q20.backIt());
    //EXPECT_EQ(P11.mTypeAwithSingleA, false);
    //EXPECT_EQ(P11.mChildMaxWminusA,  Q21.backIt());
    EXPECT_EQ(P11.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q17.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q18.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q19.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q20.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q21.mWHABType, JTSTreeNode::H);

    // Case 12: Node P Full Full Full Partial(W>H) Partial(W>H)

    auto& Q22 = createQ(pqTree,emptyChildren);
    Q22.mGeneration = pqTree.mGeneration;
    Q22.mPertinentType = JTSTreeNode::Full;
    Q22.mW = 15;
    Q22.mH = 0;
    Q22.mA = 0;
    Q22.mCD = 0;

    auto& Q23 = createQ(pqTree,emptyChildren);
    Q23.mGeneration = pqTree.mGeneration;
    Q23.mPertinentType = JTSTreeNode::Full;
    Q23.mW = 14;
    Q23.mH = 0;
    Q23.mA = 0;
    Q23.mCD = 0;

    auto& Q24 = createQ(pqTree,emptyChildren);
    Q24.mGeneration = pqTree.mGeneration;
    Q24.mPertinentType = JTSTreeNode::Full;
    Q24.mW = 17;
    Q24.mH = 0;
    Q24.mA = 0;
    Q24.mCD = 0;

    auto& Q25 = createQ(pqTree,emptyChildren);
    Q25.mGeneration = pqTree.mGeneration;
    Q25.mPertinentType = JTSTreeNode::Partial;
    Q25.mW = 40;
    Q25.mH = 3;
    Q25.mA = 3;
    Q25.mCD = 3;

    auto& Q26 = createQ(pqTree,emptyChildren);
    Q26.mGeneration = pqTree.mGeneration;
    Q26.mPertinentType = JTSTreeNode::Partial;
    Q26.mW = 30;
    Q26.mH = 6;
    Q26.mA = 6;
    Q26.mCD = 6;

    list<node_list_it_t> P12children;
    P12children.push_back(Q22.backIt());
    P12children.push_back(Q23.backIt());
    P12children.push_back(Q24.backIt());
    P12children.push_back(Q25.backIt());
    P12children.push_back(Q26.backIt());

    auto& P12 = createP(pqTree, P12children);
    P12.mGeneration = pqTree.mGeneration;
    calculateWHAC(P12);
    P12.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(P12);
    //EXPECT_EQ(P12.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(P12.mW, 116);
    //EXPECT_EQ(P12.mH, 33);
    //EXPECT_EQ(P12.mA, 9);
    //EXPECT_EQ(P12.mChildMax1WminusH, Q25.backIt());
    //EXPECT_EQ(P12.mChildMax2WminusH, Q26.backIt());
    //EXPECT_EQ(P12.mTypeAwithSingleA, false);
    //EXPECT_EQ(P12.mChildMaxWminusA,  Q25.backIt());
    EXPECT_EQ(P12.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q22.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q23.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q24.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q25.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q26.mWHABType, JTSTreeNode::W);


    // Case 13: Q Empty Empty Partial

    auto& Q27 = createQ(pqTree,emptyChildren);
    auto& Q28 = createQ(pqTree,emptyChildren);
    auto& Q29 = createQ(pqTree,emptyChildren);
    auto& Q30 = createQ(pqTree,emptyChildren);

    auto& Q31 = createQ(pqTree,emptyChildren);
    Q31.mGeneration = pqTree.mGeneration;
    Q31.mPertinentType = JTSTreeNode::Partial;
    Q31.mW = 10;
    Q31.mH = 6;
    Q31.mA = 4;
    Q31.mCD = 4;

    list<node_list_it_t> Q33children;
    Q33children.push_back(Q27.backIt());
    Q33children.push_back(Q28.backIt());
    Q33children.push_back(Q29.backIt());
    Q33children.push_back(Q30.backIt());
    Q33children.push_back(Q31.backIt());

    auto& Q33 = createQ(pqTree, Q33children);
    Q33.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q33);
    Q33.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q33);
    //EXPECT_EQ(Q33.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q33.mW, 10);
    //EXPECT_EQ(Q33.mH, 6);
    //EXPECT_EQ(Q33.mA, 4);
    //EXPECT_EQ(Q33.mChildBoundaryForQH, Q31.backIt());
    //EXPECT_EQ(Q33.mQHFullOnListHead, false);
    //EXPECT_EQ(Q33.mChildBoundaryOnHeadForQA, Q31.backIt());
    //EXPECT_EQ(Q33.mChildBoundaryOnTailForQA, Q31.backIt());
    //EXPECT_EQ(Q33.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q33.mChildMaxWminusA,  Q31.backIt());
    EXPECT_EQ(Q33.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q31.mWHABType, JTSTreeNode::H);

    // Case 14: Q Partial Empty Empty
    auto& Q34 = createQ(pqTree,emptyChildren);
    Q34.mGeneration = pqTree.mGeneration;
    Q34.mPertinentType = JTSTreeNode::Partial;
    Q34.mW = 10;
    Q34.mH = 6;
    Q34.mA = 4;
    Q34.mCD = 4;

    auto& Q35 = createQ(pqTree,emptyChildren);
    auto& Q36 = createQ(pqTree,emptyChildren);
    auto& Q37 = createQ(pqTree,emptyChildren);
    auto& Q38 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q39children;
    Q39children.push_back(Q34.backIt());
    Q39children.push_back(Q35.backIt());
    Q39children.push_back(Q36.backIt());
    Q39children.push_back(Q37.backIt());
    Q39children.push_back(Q38.backIt());

    auto& Q39 = createQ(pqTree, Q39children);
    calculateWHAC(Q39);
    Q39.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q39);

    //EXPECT_EQ(Q39.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q39.mW, 10);
    //EXPECT_EQ(Q39.mH, 6);
    //EXPECT_EQ(Q39.mA, 4);
    //EXPECT_EQ(Q39.mChildBoundaryForQH, Q34.backIt());
    //EXPECT_EQ(Q39.mQHFullOnListHead, true);
    //EXPECT_EQ(Q39.mChildBoundaryOnHeadForQA, Q34.backIt());
    //EXPECT_EQ(Q39.mChildBoundaryOnTailForQA, Q34.backIt());
    //EXPECT_EQ(Q39.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q39.mChildMaxWminusA,  Q34.backIt());
    EXPECT_EQ(Q39.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q34.mWHABType, JTSTreeNode::H);

    // Case 15: Q Partial Empty Full Empty Parial

    auto& Q40 = createQ(pqTree,emptyChildren);
    Q40.mGeneration = pqTree.mGeneration;
    Q40.mPertinentType = JTSTreeNode::Partial;
    Q40.mW = 10;
    Q40.mH = 6;
    Q40.mA = 4;
    Q40.mCD = 4;

    auto& Q41 = createQ(pqTree,emptyChildren);
    auto& Q42 = createQ(pqTree,emptyChildren);
    Q42.mGeneration = pqTree.mGeneration;
    Q42.mPertinentType = JTSTreeNode::Full;
    Q42.mW = 17;
    Q42.mH = 0;
    Q42.mA = 0;
    Q42.mCD = 0;

    auto& Q43 = createQ(pqTree,emptyChildren);
    auto& Q44 = createQ(pqTree,emptyChildren);
    Q44.mGeneration = pqTree.mGeneration;
    Q44.mPertinentType = JTSTreeNode::Partial;
    Q44.mW = 8;
    Q44.mH = 5;
    Q44.mA = 3;
    Q44.mCD = 3;

    list<node_list_it_t> Q45children;
    Q45children.push_back(Q40.backIt());
    Q45children.push_back(Q41.backIt());
    Q45children.push_back(Q42.backIt());
    Q45children.push_back(Q43.backIt());
    Q45children.push_back(Q44.backIt());

    auto& Q45 = createQ(pqTree, Q45children);
    Q45.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q45);
    Q45.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q45);
    //EXPECT_EQ(Q45.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q45.mW, 35);
    //EXPECT_EQ(Q45.mH, 31);
    //EXPECT_EQ(Q45.mA, 18);
    //EXPECT_EQ(Q45.mChildBoundaryForQH, Q40.backIt());
    //EXPECT_EQ(Q45.mQHFullOnListHead, true);
    //EXPECT_EQ(Q45.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q45.mChildMaxWminusA,  Q42.backIt());
    EXPECT_EQ(Q45.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q40.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q42.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q44.mWHABType, JTSTreeNode::W);


    // Case 16: Q Partial Partial Partial Partial Partial Partial
    auto& Q46 = createQ(pqTree,emptyChildren);
    Q46.mGeneration = pqTree.mGeneration;
    Q46.mPertinentType = JTSTreeNode::Partial;
    Q46.mW = 30;
    Q46.mH = 7;
    Q46.mA = 4;
    Q46.mCD = 4;

    auto& Q47 = createQ(pqTree,emptyChildren);
    Q47.mGeneration = pqTree.mGeneration;
    Q47.mPertinentType = JTSTreeNode::Partial;
    Q47.mW = 20;
    Q47.mH = 13;
    Q47.mA = 12;
    Q47.mCD = 12;

    auto& Q48 = createQ(pqTree,emptyChildren);
    Q48.mGeneration = pqTree.mGeneration;
    Q48.mPertinentType = JTSTreeNode::Partial;
    Q48.mW = 15;
    Q48.mH = 8;
    Q48.mA = 4;
    Q48.mCD = 4;

    auto& Q49 = createQ(pqTree,emptyChildren);
    Q49.mGeneration = pqTree.mGeneration;
    Q49.mPertinentType = JTSTreeNode::Partial;
    Q49.mW = 17;
    Q49.mH = 3;
    Q49.mA = 3;
    Q49.mCD = 3;

    auto& Q50 = createQ(pqTree,emptyChildren);
    Q50.mGeneration = pqTree.mGeneration;
    Q50.mPertinentType = JTSTreeNode::Partial;
    Q50.mW = 16;
    Q50.mH = 8;
    Q50.mA = 5;
    Q50.mCD = 5;

    auto& Q51 = createQ(pqTree,emptyChildren);
    Q51.mGeneration = pqTree.mGeneration;
    Q51.mPertinentType = JTSTreeNode::Partial;
    Q51.mW = 22;
    Q51.mH = 3;
    Q51.mA = 2;
    Q51.mCD = 2;

    list<node_list_it_t> Q52children;
    Q52children.push_back(Q46.backIt());
    Q52children.push_back(Q47.backIt());
    Q52children.push_back(Q48.backIt());
    Q52children.push_back(Q49.backIt());
    Q52children.push_back(Q50.backIt());
    Q52children.push_back(Q51.backIt());

    auto& Q52 = createQ(pqTree, Q52children);
    Q52.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q52);
    Q52.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q52);
    //EXPECT_EQ(Q52.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q52.mW, 120);
    //EXPECT_EQ(Q52.mH, 97);
    //EXPECT_EQ(Q52.mChildBoundaryForQH, Q46.backIt());
    //EXPECT_EQ(Q52.mQHFullOnListHead, true);
    //EXPECT_EQ(Q52.mA, 90);
    //EXPECT_EQ(Q52.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q52.mChildBoundaryOnHeadForQA, Q46.backIt());
    //EXPECT_EQ(Q52.mChildBoundaryOnTailForQA, Q47.backIt());
    EXPECT_EQ(Q52.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q46.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q47.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q48.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q49.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q50.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q51.mWHABType, JTSTreeNode::W);

    // Case 17: Q Partial Partial Partial Partial Partial Partial #2
    auto& Q53 = createQ(pqTree,emptyChildren);
    Q53.mGeneration = pqTree.mGeneration;
    Q53.mPertinentType = JTSTreeNode::Partial;
    Q53.mW = 22;
    Q53.mH = 3;
    Q53.mA = 2;
    Q53.mCD = 2;

    auto& Q54 = createQ(pqTree,emptyChildren);
    Q54.mGeneration = pqTree.mGeneration;
    Q54.mPertinentType = JTSTreeNode::Partial;
    Q54.mW = 16;
    Q54.mH = 8;
    Q54.mA = 5;
    Q54.mCD = 5;

    auto& Q55 = createQ(pqTree,emptyChildren);
    Q55.mGeneration = pqTree.mGeneration;
    Q55.mPertinentType = JTSTreeNode::Partial;
    Q55.mW = 17;
    Q55.mH = 3;
    Q55.mA = 3;
    Q55.mCD = 3;


    auto& Q56 = createQ(pqTree,emptyChildren);
    Q56.mGeneration = pqTree.mGeneration;
    Q56.mPertinentType = JTSTreeNode::Partial;
    Q56.mW = 15;
    Q56.mH = 8;
    Q56.mA = 4;
    Q56.mCD = 4;

    auto& Q57 = createQ(pqTree,emptyChildren);
    Q57.mGeneration = pqTree.mGeneration;
    Q57.mPertinentType = JTSTreeNode::Partial;
    Q57.mW = 20;
    Q57.mH = 13;
    Q57.mA = 12;
    Q57.mCD = 12;

    auto& Q58 = createQ(pqTree,emptyChildren);
    Q58.mGeneration = pqTree.mGeneration;
    Q58.mPertinentType = JTSTreeNode::Partial;
    Q58.mW = 30;
    Q58.mH = 7;
    Q58.mA = 4;
    Q58.mCD = 4;

    list<node_list_it_t> Q59children;
    Q59children.push_back(Q53.backIt());
    Q59children.push_back(Q54.backIt());
    Q59children.push_back(Q55.backIt());
    Q59children.push_back(Q56.backIt());
    Q59children.push_back(Q57.backIt());
    Q59children.push_back(Q58.backIt());

    auto& Q59 = createQ(pqTree, Q59children);
    Q59.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q59);
    Q59.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q59);
    //EXPECT_EQ(Q59.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q59.mW, 120);
    //EXPECT_EQ(Q59.mH, 97);
    //EXPECT_EQ(Q59.mChildBoundaryForQH, Q58.backIt());
    //EXPECT_EQ(Q59.mQHFullOnListHead, false);
    //EXPECT_EQ(Q59.mA, 90);
    //EXPECT_EQ(Q59.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q59.mChildBoundaryOnHeadForQA, Q57.backIt());
    //EXPECT_EQ(Q59.mChildBoundaryOnTailForQA, Q58.backIt());
    EXPECT_EQ(Q59.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q53.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q54.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q55.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q56.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q57.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q58.mWHABType, JTSTreeNode::H);

    // Case 18: Q Partial Partial Partial Partial Partial Partial #3

    auto& Q60 = createQ(pqTree,emptyChildren);
    Q60.mGeneration = pqTree.mGeneration;
    Q60.mPertinentType = JTSTreeNode::Partial;
    Q60.mW = 17;
    Q60.mH = 11;
    Q60.mA = 3;
    Q60.mCD = 3;

    auto& Q61 = createQ(pqTree,emptyChildren);
    Q61.mGeneration = pqTree.mGeneration;
    Q61.mPertinentType = JTSTreeNode::Partial;
    Q61.mW = 30;
    Q61.mH = 7;
    Q61.mA = 4;
    Q61.mCD = 4;

    auto& Q62 = createQ(pqTree,emptyChildren);
    Q62.mGeneration = pqTree.mGeneration;
    Q62.mPertinentType = JTSTreeNode::Partial;
    Q62.mW = 20;
    Q62.mH = 13;
    Q62.mA = 12;
    Q62.mCD = 12;

    auto& Q63 = createQ(pqTree,emptyChildren);
    Q63.mGeneration = pqTree.mGeneration;
    Q63.mPertinentType = JTSTreeNode::Partial;
    Q63.mW = 15;
    Q63.mH = 8;
    Q63.mA = 4;
    Q63.mCD = 4;

    auto& Q64 = createQ(pqTree,emptyChildren);
    Q64.mGeneration = pqTree.mGeneration;
    Q64.mPertinentType = JTSTreeNode::Partial;
    Q64.mW = 16;
    Q64.mH = 8;
    Q64.mA = 5;
    Q64.mCD = 5;

    auto& Q65 = createQ(pqTree,emptyChildren);
    Q65.mGeneration = pqTree.mGeneration;
    Q65.mPertinentType = JTSTreeNode::Partial;
    Q65.mW = 22;
    Q65.mH = 3;
    Q65.mA = 2;
    Q65.mCD = 2;

    list<node_list_it_t> Q66children;
    Q66children.push_back(Q60.backIt());
    Q66children.push_back(Q61.backIt());
    Q66children.push_back(Q62.backIt());
    Q66children.push_back(Q63.backIt());
    Q66children.push_back(Q64.backIt());
    Q66children.push_back(Q65.backIt());

    auto& Q66 = createQ(pqTree, Q66children);
    Q66.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q66);
    Q66.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q66);
    //EXPECT_EQ(Q66.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q66.mW, 120);
    //EXPECT_EQ(Q66.mH, 101);
    //EXPECT_EQ(Q66.mChildBoundaryForQH, Q65.backIt());
    //EXPECT_EQ(Q66.mQHFullOnListHead, false);
    //EXPECT_EQ(Q66.mA, 90);
    //EXPECT_EQ(Q66.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q66.mChildBoundaryOnHeadForQA, Q61.backIt());
    //EXPECT_EQ(Q66.mChildBoundaryOnTailForQA, Q62.backIt());
    EXPECT_EQ(Q66.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q60.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q61.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q62.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q63.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q64.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q65.mWHABType, JTSTreeNode::H);

    // Case 19: Q Partial Full Full Partial Partial Full Partial Full Full Full
    //            Partial Partial

    auto& Q67 = createQ(pqTree,emptyChildren);
    Q67.mGeneration = pqTree.mGeneration;
    Q67.mPertinentType = JTSTreeNode::Partial;
    Q67.mW = 17;
    Q67.mH = 11;
    Q67.mA = 3;
    Q67.mCD = 3;

    auto& Q68 = createQ(pqTree,emptyChildren);
    Q68.mGeneration = pqTree.mGeneration;
    Q68.mPertinentType = JTSTreeNode::Full;
    Q68.mW = 17;
    Q68.mH = 0;
    Q68.mA = 0;
    Q68.mCD = 0;

    auto& Q69 = createQ(pqTree,emptyChildren);
    Q69.mGeneration = pqTree.mGeneration;
    Q69.mPertinentType = JTSTreeNode::Full;
    Q69.mW = 13;
    Q69.mH = 0;
    Q69.mA = 0;
    Q69.mCD = 0;

    auto& Q70 = createQ(pqTree,emptyChildren);
    Q70.mGeneration = pqTree.mGeneration;
    Q70.mPertinentType = JTSTreeNode::Partial;
    Q70.mW = 30;
    Q70.mH = 7;
    Q70.mA = 4;
    Q70.mCD = 4;

    auto& Q71 = createQ(pqTree,emptyChildren);
    Q71.mGeneration = pqTree.mGeneration;
    Q71.mPertinentType = JTSTreeNode::Partial;
    Q71.mW = 20;
    Q71.mH = 13;
    Q71.mA = 12;
    Q71.mCD = 12;

    auto& Q72 = createQ(pqTree,emptyChildren);
    Q72.mGeneration = pqTree.mGeneration;
    Q72.mPertinentType = JTSTreeNode::Full;
    Q72.mW = 22;
    Q72.mH = 0;
    Q72.mA = 0;
    Q72.mCD = 0;

    auto& Q73 = createQ(pqTree,emptyChildren);
    Q73.mGeneration = pqTree.mGeneration;
    Q73.mPertinentType = JTSTreeNode::Partial;
    Q73.mW = 15;
    Q73.mH = 8;
    Q73.mA = 4;
    Q73.mCD = 4;

    auto& Q74 = createQ(pqTree,emptyChildren);
    Q74.mGeneration = pqTree.mGeneration;
    Q74.mPertinentType = JTSTreeNode::Full;
    Q74.mW = 2;
    Q74.mH = 0;
    Q74.mA = 0;
    Q74.mCD = 0;

    auto& Q75 = createQ(pqTree,emptyChildren);
    Q75.mGeneration = pqTree.mGeneration;
    Q75.mPertinentType = JTSTreeNode::Full;
    Q75.mW = 6;
    Q75.mH = 0;
    Q75.mA = 0;
    Q75.mCD = 0;

    auto& Q76 = createQ(pqTree,emptyChildren);
    Q76.mGeneration = pqTree.mGeneration;
    Q76.mPertinentType = JTSTreeNode::Full;
    Q76.mW = 24;
    Q76.mH = 0;
    Q76.mA = 0;
    Q76.mCD = 0;

    auto& Q77 = createQ(pqTree,emptyChildren);
    Q77.mGeneration = pqTree.mGeneration;
    Q77.mPertinentType = JTSTreeNode::Partial;
    Q77.mW = 16;
    Q77.mH = 8;
    Q77.mA = 5;
    Q77.mCD = 5;

    auto& Q78 = createQ(pqTree,emptyChildren);
    Q78.mGeneration = pqTree.mGeneration;
    Q78.mPertinentType = JTSTreeNode::Partial;
    Q78.mW = 22;
    Q78.mH = 3;
    Q78.mA = 2;
    Q78.mCD = 2;

    list<node_list_it_t> Q79children;
    Q79children.push_back(Q67.backIt());
    Q79children.push_back(Q68.backIt());
    Q79children.push_back(Q69.backIt());
    Q79children.push_back(Q70.backIt());
    Q79children.push_back(Q71.backIt());
    Q79children.push_back(Q72.backIt());
    Q79children.push_back(Q73.backIt());
    Q79children.push_back(Q74.backIt());
    Q79children.push_back(Q75.backIt());
    Q79children.push_back(Q76.backIt());
    Q79children.push_back(Q77.backIt());
    Q79children.push_back(Q78.backIt());

    auto& Q79 = createQ(pqTree, Q79children);
    Q79.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q79);
    Q79.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q79);
    //EXPECT_EQ(Q79.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q79.mW, 204);
    //EXPECT_EQ(Q79.mH, 185);
    //EXPECT_EQ(Q79.mChildBoundaryForQH, Q78.backIt());
    //EXPECT_EQ(Q79.mQHFullOnListHead, false);
    //EXPECT_EQ(Q79.mA, 145);
    //EXPECT_EQ(Q79.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q79.mChildBoundaryOnHeadForQA, Q67.backIt());
    //EXPECT_EQ(Q79.mChildBoundaryOnTailForQA, Q70.backIt());
    EXPECT_EQ(Q79.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q67.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q68.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q69.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q70.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q71.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q72.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q73.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q74.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q75.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q76.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q77.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q78.mWHABType, JTSTreeNode::H);

    // Case 20: Q Partial Full Full Empty Parial

    auto& Q80 = createQ(pqTree,emptyChildren);
    Q80.mGeneration = pqTree.mGeneration;
    Q80.mPertinentType = JTSTreeNode::Partial;
    Q80.mW = 17;
    Q80.mH = 5;
    Q80.mA = 3;
    Q80.mCD = 3;

    auto& Q81 = createQ(pqTree,emptyChildren);
    Q81.mGeneration = pqTree.mGeneration;
    Q81.mPertinentType = JTSTreeNode::Full;
    Q81.mW = 6;
    Q81.mH = 0;
    Q81.mA = 0;
    Q81.mCD = 0;

    auto& Q82 = createQ(pqTree,emptyChildren);
    Q82.mGeneration = pqTree.mGeneration;
    Q82.mPertinentType = JTSTreeNode::Full;
    Q82.mW = 16;
    Q82.mH = 0;
    Q82.mA = 0;
    Q82.mCD = 0;

    auto& Q83 = createQ(pqTree,emptyChildren);

    auto& Q84 = createQ(pqTree,emptyChildren);
    Q84.mGeneration = pqTree.mGeneration;
    Q84.mPertinentType = JTSTreeNode::Partial;
    Q84.mW = 23;
    Q84.mH = 10;
    Q84.mA = 8;
    Q84.mCD = 8;

    list<node_list_it_t> Q85children;
    Q85children.push_back(Q80.backIt());
    Q85children.push_back(Q81.backIt());
    Q85children.push_back(Q82.backIt());
    Q85children.push_back(Q83.backIt());
    Q85children.push_back(Q84.backIt());

    auto& Q85 = createQ(pqTree, Q85children);
    Q85.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q85);
    Q85.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q85);

    //EXPECT_EQ(Q85.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q85.mW, 62);
    //EXPECT_EQ(Q85.mH, 49);
    //EXPECT_EQ(Q85.mChildBoundaryForQH, Q84.backIt());
    //EXPECT_EQ(Q85.mQHFullOnListHead, false);
    //EXPECT_EQ(Q85.mA, 28);
    //EXPECT_EQ(Q85.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q85.mChildBoundaryOnHeadForQA, Q80.backIt());
    //EXPECT_EQ(Q85.mChildBoundaryOnTailForQA, Q82.backIt());
    EXPECT_EQ(Q85.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q80.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q81.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q82.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q84.mWHABType, JTSTreeNode::H);


    // Case 21: Q Empty Full Empty

    auto& Q86 = createQ(pqTree,emptyChildren);

    auto& Q87 = createQ(pqTree,emptyChildren);
    Q87.mGeneration = pqTree.mGeneration;
    Q87.mPertinentType = JTSTreeNode::Full;
    Q87.mW = 6;
    Q87.mH = 0;
    Q87.mA = 0;
    Q87.mCD = 0;

    auto& Q88 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q89children;
    Q89children.push_back(Q86.backIt());
    Q89children.push_back(Q87.backIt());
    Q89children.push_back(Q88.backIt());

    auto& Q89 = createQ(pqTree, Q89children);
    Q89.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q89);
    Q89.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q89);
    //EXPECT_EQ(Q89.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q89.mW, 6);
    //EXPECT_EQ(Q89.mH, 6);
    //EXPECT_EQ(Q89.mA, 0);
    //EXPECT_EQ(Q89.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q89.mChildMaxWminusA,     Q87.backIt());
    EXPECT_EQ(Q89.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q87.mWHABType, JTSTreeNode::W);

    // Case 22: Q Empty Full Full
    auto& Q90 = createQ(pqTree,emptyChildren);

    auto& Q91 = createQ(pqTree,emptyChildren);
    Q91.mGeneration = pqTree.mGeneration;
    Q91.mPertinentType = JTSTreeNode::Full;
    Q91.mW = 6;
    Q91.mH = 0;
    Q91.mA = 0;
    Q91.mCD = 0;


    auto& Q92 = createQ(pqTree,emptyChildren);
    Q92.mGeneration = pqTree.mGeneration;
    Q92.mPertinentType = JTSTreeNode::Full;
    Q92.mW = 10;
    Q92.mH = 0;
    Q92.mA = 0;
    Q92.mCD = 0;

    list<node_list_it_t> Q93children;
    Q93children.push_back(Q90.backIt());
    Q93children.push_back(Q91.backIt());
    Q93children.push_back(Q92.backIt());

    auto& Q93 = createQ(pqTree, Q93children);
    Q93.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q93);
    Q93.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q93);
    //EXPECT_EQ(Q93.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q93.mW, 16);
    //EXPECT_EQ(Q93.mH, 0);
    //EXPECT_EQ(Q93.mChildBoundaryForQH, Q91.backIt());
    //EXPECT_EQ(Q93.mQHFullOnListHead, false);
    //EXPECT_EQ(Q93.mA, 0);
    //EXPECT_EQ(Q93.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q93.mChildBoundaryOnHeadForQA, Q91.backIt());
    //EXPECT_EQ(Q93.mChildBoundaryOnTailForQA, Q92.backIt());
    EXPECT_EQ(Q93.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q91.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q92.mWHABType, JTSTreeNode::B);


    // Case 23: Q Partial Full Full
    auto& Q94 = createQ(pqTree,emptyChildren);
    Q94.mGeneration = pqTree.mGeneration;
    Q94.mPertinentType = JTSTreeNode::Partial;
    Q94.mW = 10;
    Q94.mH = 5;
    Q94.mA = 4;
    Q94.mCD = 4;

    auto& Q95 = createQ(pqTree,emptyChildren);
    Q95.mGeneration = pqTree.mGeneration;
    Q95.mPertinentType = JTSTreeNode::Full;
    Q95.mW = 6;
    Q95.mH = 0;
    Q95.mA = 0;
    Q95.mCD = 0;

    auto& Q96 = createQ(pqTree,emptyChildren);
    Q96.mGeneration = pqTree.mGeneration;
    Q96.mPertinentType = JTSTreeNode::Full;
    Q96.mW = 10;
    Q96.mH = 0;
    Q96.mA = 0;
    Q96.mCD = 0;

    list<node_list_it_t> Q97children;
    Q97children.push_back(Q94.backIt());
    Q97children.push_back(Q95.backIt());
    Q97children.push_back(Q96.backIt());

    auto& Q97 = createQ(pqTree, Q97children);
    Q97.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q97);
    Q97.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q97);
    //EXPECT_EQ(Q97.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q97.mW, 26);
    //EXPECT_EQ(Q97.mH, 5);
    //EXPECT_EQ(Q97.mChildBoundaryForQH, Q94.backIt());
    //EXPECT_EQ(Q97.mQHFullOnListHead, false);
    //EXPECT_EQ(Q97.mA, 5);
    //EXPECT_EQ(Q97.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q97.mChildBoundaryOnHeadForQA, Q94.backIt());
    //EXPECT_EQ(Q97.mChildBoundaryOnTailForQA, Q96.backIt());
    EXPECT_EQ(Q97.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q94.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q95.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q96.mWHABType, JTSTreeNode::B);


    // Case 24: Q Full Full Partial
    auto& Q98 = createQ(pqTree,emptyChildren);
    Q98.mGeneration = pqTree.mGeneration;
    Q98.mPertinentType = JTSTreeNode::Full;
    Q98.mW = 6;
    Q98.mH = 0;
    Q98.mA = 0;
    Q98.mCD = 0;

    auto& Q99 = createQ(pqTree,emptyChildren);
    Q99.mGeneration = pqTree.mGeneration;
    Q99.mPertinentType = JTSTreeNode::Full;
    Q99.mW = 10;
    Q99.mH = 0;
    Q99.mA = 0;
    Q99.mCD = 0;

    auto& Q100 = createQ(pqTree,emptyChildren);
    Q100.mGeneration = pqTree.mGeneration;
    Q100.mPertinentType = JTSTreeNode::Partial;
    Q100.mW = 10;
    Q100.mH = 5;
    Q100.mA = 4;

    list<node_list_it_t> Q101children;
    Q101children.push_back(Q98.backIt());
    Q101children.push_back(Q99.backIt());
    Q101children.push_back(Q100.backIt());

    auto& Q101 = createQ(pqTree, Q101children);
    Q101.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q101);
    Q101.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q101);

    //EXPECT_EQ(Q101.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q101.mW, 26);
    //EXPECT_EQ(Q101.mH, 5);
    //EXPECT_EQ(Q101.mChildBoundaryForQH, Q100.backIt());
    //EXPECT_EQ(Q101.mQHFullOnListHead, true);
    //EXPECT_EQ(Q101.mA, 5);
    //EXPECT_EQ(Q101.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q101.mChildBoundaryOnHeadForQA, Q98.backIt());
    //EXPECT_EQ(Q101.mChildBoundaryOnTailForQA, Q100.backIt());
    EXPECT_EQ(Q101.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q98.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q99.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q100.mWHABType, JTSTreeNode::H);

    // Case 25: Q Full Partial Full
    auto& Q102 = createQ(pqTree,emptyChildren);
    Q102.mGeneration = pqTree.mGeneration;
    Q102.mPertinentType = JTSTreeNode::Full;
    Q102.mW = 6;
    Q102.mH = 0;
    Q102.mA = 0;
    Q102.mCD = 0;

    auto& Q103 = createQ(pqTree,emptyChildren);
    Q103.mGeneration = pqTree.mGeneration;
    Q103.mPertinentType = JTSTreeNode::Partial;
    Q103.mW = 9;
    Q103.mH = 5;
    Q103.mA = 4;
    Q103.mCD = 4;

    auto& Q104 = createQ(pqTree,emptyChildren);
    Q104.mGeneration = pqTree.mGeneration;
    Q104.mPertinentType = JTSTreeNode::Full;
    Q104.mW = 10;
    Q104.mH = 0;
    Q104.mA = 0;
    Q104.mCD = 0;

    list<node_list_it_t> Q105children;
    Q105children.push_back(Q102.backIt());
    Q105children.push_back(Q103.backIt());
    Q105children.push_back(Q104.backIt());

    auto& Q105 = createQ(pqTree, Q105children);
    Q105.mPertinentType = JTSTreeNode::Partial;
    calculateWHAC(Q105);
    Q105.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q105);

    //EXPECT_EQ(Q105.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q105.mW, 25);
    //EXPECT_EQ(Q105.mH, 11);
    //EXPECT_EQ(Q105.mChildBoundaryForQH, Q103.backIt());
    //EXPECT_EQ(Q105.mQHFullOnListHead, false);
    //EXPECT_EQ(Q105.mA, 11);
    //EXPECT_EQ(Q105.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q105.mChildBoundaryOnHeadForQA, Q103.backIt());
    //EXPECT_EQ(Q105.mChildBoundaryOnTailForQA, Q104.backIt());
    EXPECT_EQ(Q105.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q102.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q103.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q104.mWHABType, JTSTreeNode::B);

    // Case 26: Q Full Full Full
    auto& Q106 = createQ(pqTree,emptyChildren);
    Q106.mGeneration = pqTree.mGeneration;
    Q106.mPertinentType = JTSTreeNode::Full;
    Q106.mW = 6;
    Q106.mH = 0;
    Q106.mA = 0;
    Q106.mCD = 0;

    auto& Q107 = createQ(pqTree,emptyChildren);
    Q107.mGeneration = pqTree.mGeneration;
    Q107.mPertinentType = JTSTreeNode::Full;
    Q107.mW = 7;
    Q107.mH = 0;
    Q107.mA = 0;
    Q107.mCD = 0;

    auto& Q108 = createQ(pqTree,emptyChildren);
    Q108.mGeneration = pqTree.mGeneration;
    Q108.mPertinentType = JTSTreeNode::Full;
    Q108.mW = 8;
    Q108.mH = 0;
    Q108.mA = 0;
    Q108.mCD = 0;

    list<node_list_it_t> Q109children;
    Q109children.push_back(Q106.backIt());
    Q109children.push_back(Q107.backIt());
    Q109children.push_back(Q108.backIt());

    auto& Q109 = createQ(pqTree, Q109children);
    Q109.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q109);
    Q109.mWHABType = JTSTreeNode::B;
    determineWHABCTypeForChildren(Q109);

    //EXPECT_EQ(Q109.mPertinentType, JTSTreeNode::Full);
    //EXPECT_EQ(Q109.mW, 21);
    //EXPECT_EQ(Q109.mH, 0);
    //EXPECT_EQ(Q109.mA, 0);
    EXPECT_EQ(Q109.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q106.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q107.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q108.mWHABType, JTSTreeNode::B);


    // Case 27: Q Empty Empty Partial
    auto& Q110 = createQ(pqTree,emptyChildren);

    auto& Q111 = createQ(pqTree,emptyChildren);

    auto& Q112 = createQ(pqTree,emptyChildren);
    Q112.mGeneration = pqTree.mGeneration;
    Q112.mPertinentType = JTSTreeNode::Partial;
    Q112.mW = 20;
    Q112.mH = 8;
    Q112.mA = 6;
    Q112.mCD = 6;

    list<node_list_it_t> Q113children;
    Q113children.push_back(Q110.backIt());
    Q113children.push_back(Q111.backIt());
    Q113children.push_back(Q112.backIt());

    auto& Q113 = createQ(pqTree, Q113children);
    Q113.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q113);
    Q113.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q113);

    //EXPECT_EQ(Q113.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q113.mW, 20);
    //EXPECT_EQ(Q113.mH, 8);
    //EXPECT_EQ(Q113.mChildBoundaryForQH, Q112.backIt());
    //EXPECT_EQ(Q113.mQHFullOnListHead, false);
    //EXPECT_EQ(Q113.mA, 6);
    //EXPECT_EQ(Q113.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q113.mChildMaxWminusA,  Q112.backIt());
    EXPECT_EQ(Q113.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q112.mWHABType, JTSTreeNode::H);

    // Case 28: Q Partial Empty Empty
    auto& Q114 = createQ(pqTree,emptyChildren);
    Q114.mGeneration = pqTree.mGeneration;
    Q114.mPertinentType = JTSTreeNode::Partial;
    Q114.mW = 20;
    Q114.mH = 8;
    Q114.mA = 6;
    Q114.mCD = 6;

    auto& Q115 = createQ(pqTree,emptyChildren);

    auto& Q116 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q117children;
    Q117children.push_back(Q114.backIt());
    Q117children.push_back(Q115.backIt());
    Q117children.push_back(Q116.backIt());

    auto& Q117 = createQ(pqTree, Q117children);
    Q117.mPertinentType = JTSTreeNode::Partial;
    calculateWHAC(Q117);
    Q117.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q117);

    //EXPECT_EQ(Q117.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q117.mW, 20);
    //EXPECT_EQ(Q117.mH, 8);
    //EXPECT_EQ(Q117.mChildBoundaryForQH, Q114.backIt());
    //EXPECT_EQ(Q117.mQHFullOnListHead, true);
    //EXPECT_EQ(Q117.mA, 6);
    //EXPECT_EQ(Q117.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q117.mChildMaxWminusA,  Q114.backIt());
    EXPECT_EQ(Q117.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q114.mWHABType, JTSTreeNode::H);


    // Case 29: Q Empty Full Partial
    auto& Q118 = createQ(pqTree,emptyChildren);

    auto& Q119 = createQ(pqTree,emptyChildren);
    Q119.mGeneration = pqTree.mGeneration;
    Q119.mPertinentType = JTSTreeNode::Full;
    Q119.mW = 20;
    Q119.mH = 0;
    Q119.mA = 0;
    Q119.mCD = 0;

    auto& Q120 = createQ(pqTree,emptyChildren);
    Q120.mGeneration = pqTree.mGeneration;
    Q120.mPertinentType = JTSTreeNode::Partial;
    Q120.mW = 25;
    Q120.mH = 8;
    Q120.mA = 6;
    Q120.mCD = 6;

    list<node_list_it_t> Q121children;
    Q121children.push_back(Q118.backIt());
    Q121children.push_back(Q119.backIt());
    Q121children.push_back(Q120.backIt());

    auto& Q121 = createQ(pqTree, Q121children);
    Q121.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q121);
    Q121.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q121);

    //EXPECT_EQ(Q121.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q121.mW, 45);
    //EXPECT_EQ(Q121.mH, 28);
    //EXPECT_EQ(Q121.mChildBoundaryForQH, Q120.backIt());
    //EXPECT_EQ(Q121.mQHFullOnListHead, false);
    //EXPECT_EQ(Q121.mA, 8);
    //EXPECT_EQ(Q121.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q121.mChildBoundaryOnHeadForQA, Q119.backIt());
    //EXPECT_EQ(Q121.mChildBoundaryOnTailForQA, Q120.backIt());
    EXPECT_EQ(Q121.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q119.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q120.mWHABType, JTSTreeNode::H);

    // Case 30: Q Partial Full Empty
    auto& Q122 = createQ(pqTree,emptyChildren);
    Q122.mGeneration = pqTree.mGeneration;
    Q122.mPertinentType = JTSTreeNode::Partial;
    Q122.mW = 25;
    Q122.mH = 8;
    Q122.mA = 6;
    Q122.mCD = 6;

    auto& Q123 = createQ(pqTree,emptyChildren);
    Q123.mGeneration = pqTree.mGeneration;
    Q123.mPertinentType = JTSTreeNode::Full;
    Q123.mW = 20;
    Q123.mH = 0;
    Q123.mCD = 0;

    auto& Q124 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> Q125children;
    Q125children.push_back(Q122.backIt());
    Q125children.push_back(Q123.backIt());
    Q125children.push_back(Q124.backIt());

    auto& Q125 = createQ(pqTree, Q125children);
    Q125.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q125);
    Q125.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q125);

    //EXPECT_EQ(Q125.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q125.mW, 45);
    //EXPECT_EQ(Q125.mH, 28);
    //EXPECT_EQ(Q125.mChildBoundaryForQH, Q122.backIt());
    //EXPECT_EQ(Q125.mQHFullOnListHead, true);
    //EXPECT_EQ(Q125.mA, 8);
    //EXPECT_EQ(Q125.mTypeAwithSingleA, false);
    //EXPECT_EQ(Q125.mChildBoundaryOnHeadForQA, Q122.backIt());
    //EXPECT_EQ(Q125.mChildBoundaryOnTailForQA, Q123.backIt());
    EXPECT_EQ(Q125.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q122.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q123.mWHABType, JTSTreeNode::W);

    // Case 31: Q Full  Empty Full
    auto& Q126 = createQ(pqTree,emptyChildren);
    Q126.mGeneration = pqTree.mGeneration;
    Q126.mPertinentType = JTSTreeNode::Full;
    Q126.mW = 25;
    Q126.mH = 0;
    Q126.mA = 0;
    Q126.mCD = 0;

    auto& Q127 = createQ(pqTree,emptyChildren);

    auto& Q128 = createQ(pqTree,emptyChildren);
    Q128.mGeneration = pqTree.mGeneration;
    Q128.mPertinentType = JTSTreeNode::Full;
    Q128.mW = 10;
    Q128.mH = 0;
    Q128.mA = 0;
    Q128.mCD = 0;

    list<node_list_it_t> Q129children;
    Q129children.push_back(Q126.backIt());
    Q129children.push_back(Q127.backIt());
    Q129children.push_back(Q128.backIt());

    auto& Q129 = createQ(pqTree, Q129children);
    Q129.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q129);
    Q129.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q129);

    //EXPECT_EQ(Q129.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q129.mW, 35);
    //EXPECT_EQ(Q129.mH, 10);
    //EXPECT_EQ(Q129.mChildBoundaryForQH, Q126.backIt());
    //EXPECT_EQ(Q129.mQHFullOnListHead, true);
    //EXPECT_EQ(Q129.mA, 10);
    //EXPECT_EQ(Q129.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q129.mChildMaxWminusA,  Q126.backIt());
    EXPECT_EQ(Q129.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q126.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q128.mWHABType, JTSTreeNode::W);

    // Case 32: Q Partial  Empty Partial
    auto& Q130 = createQ(pqTree,emptyChildren);
    Q130.mGeneration = pqTree.mGeneration;
    Q130.mPertinentType = JTSTreeNode::Partial;
    Q130.mW = 25;
    Q130.mH = 7;
    Q130.mA = 4;
    Q130.mCD = 4;

    auto& Q131 = createQ(pqTree,emptyChildren);

    auto& Q132 = createQ(pqTree,emptyChildren);
    Q132.mGeneration = pqTree.mGeneration;
    Q132.mPertinentType = JTSTreeNode::Partial;
    Q132.mW = 17;
    Q132.mH = 5;
    Q132.mA = 5;
    Q132.mCD = 5;

    list<node_list_it_t> Q133children;
    Q133children.push_back(Q130.backIt());
    Q133children.push_back(Q131.backIt());
    Q133children.push_back(Q132.backIt());

    auto& Q133 = createQ(pqTree, Q133children);
    Q133.mGeneration = pqTree.mGeneration;
    calculateWHAC(Q133);
    Q133.mWHABType = JTSTreeNode::H;
    determineWHABCTypeForChildren(Q133);

    //EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    //EXPECT_EQ(Q133.mW, 42);
    //EXPECT_EQ(Q133.mH, 24);
    //EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    //EXPECT_EQ(Q133.mQHFullOnListHead, true);
    //EXPECT_EQ(Q133.mA, 21);
    //EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    //EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(Q133.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q130.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(Q132.mWHABType, JTSTreeNode::W);

}


/**  @brief tests JTSPlanarizer::determineWHABCNodeTypes()
 */
TEST_F(JTSPlanarizerTests, Test10) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;

    // Create a simulated PQ-tree.
    // 
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

    auto& L01 = createL(pqTree);
    auto& L02 = createL(pqTree);
    auto& L03 = createL(pqTree);

    auto& L04 = createL(pqTree);
    auto& L05 = createL(pqTree);
    auto& L06 = createL(pqTree);

    auto& L07 = createL(pqTree);
    auto& L08 = createL(pqTree);
    auto& L09 = createL(pqTree);

    auto& L10 = createL(pqTree);
    auto& L11 = createL(pqTree);
    auto& L12 = createL(pqTree);

    auto& L13 = createL(pqTree);
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);

    auto& L15 = createL(pqTree);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    auto& L17 = createL(pqTree);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    auto& L19 = createL(pqTree);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);

    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);

    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);

    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    auto& L24 = createL(pqTree);

    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);

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
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);

    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);

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
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L02.mWHABType, JTSTreeNode::B);

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
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L02.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L03.mWHABType, JTSTreeNode::B);

    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02*L03  L04 L05 L06  L07 L08 L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L02.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L03.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L15.mWHABType, JTSTreeNode::B);

    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15 Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02*L03 *L04*L05 L06  L07 L08 L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L03.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L15.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L16.mWHABType, JTSTreeNode::W);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15 Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02*L03 *L04*L05 L06  L07 L08 L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L21.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L21.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L02.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L14.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L15.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L16.mWHABType, JTSTreeNode::W);

    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15 Q01*L16  *L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02*L03 *L04*L05 L06  L07 L08 L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L21.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q04.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L17.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L21.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L02.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L14.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L15.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L16.mWHABType, JTSTreeNode::W);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                     *L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15 Q01*L16  *L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02*L03 *L04*L05 L06 *L07*L08*L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L21.backIt());
    pertinentLeaves.push_back(L22.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(Q05.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L22.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(P03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(Q04.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L21.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L03.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L14.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L15.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L16.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L17.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P02.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L07.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L08.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L09.mWHABType, JTSTreeNode::B);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   _______*Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13*P01*L14 *L15 Q01*L16  *L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02*L03 *L04*L05 L06 *L07*L08*L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L21.backIt());
    pertinentLeaves.push_back(L24.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P04.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L24.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q05.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(P03.mWHABType, JTSTreeNode::A);
    EXPECT_EQ(L01.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L03.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q04.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L21.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q03.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(P01.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L14.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L15.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(Q01.mWHABType, JTSTreeNode::H);
    EXPECT_EQ(L04.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L05.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L16.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L17.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(P02.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L07.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L08.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L09.mWHABType, JTSTreeNode::B);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05*L25
    //                       /  |  \
    //                     *L22 |*L23
    //             ____________P03__________
    //            /             |           \
    //   _______ Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16   L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P04.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(L22.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L23.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L24.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L25.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q05.mWHABType, JTSTreeNode::CD);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05*L25
    //                       /  |  \
    //                     *L22 |*L23
    //             ____________P03__________
    //            /             |           \
    //   _______ Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01*L14  L15 Q01 L16   L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    EXPECT_EQ(P04.mWHABType, JTSTreeNode::CD);
    EXPECT_EQ(L14.mWHABType, JTSTreeNode::W);
    EXPECT_EQ(L22.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L23.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L24.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(L25.mWHABType, JTSTreeNode::B);
    EXPECT_EQ(Q05.mWHABType, JTSTreeNode::CD);

}

