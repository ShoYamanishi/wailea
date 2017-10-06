
/**  @brief tests JTSTreeNode::calculateWHAC()
 */
TEST_F(JTSPlanarizerTests, Test6) {

    JTSTree& pqTree = getPQTree();
    //JTSPlanarizer planarizer;

    // Node type Pertinent type
    // L         old generation
    // L         full
    // P         old generation
    // P         partial
    // P         full
    // Q         old generation
    // Q         partial
    // Q         full

    initializeForOneIteration(pqTree);

    // Case 1: Node L
    auto& L01 = createL(pqTree);

    calculateWHAC(L01);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

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
    calculateWHAC(P01);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 1);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);
    EXPECT_EQ(P01.mChildMax1WminusH, pqTree.nil());
    EXPECT_EQ(P01.mChildMax2WminusH, pqTree.nil());
    EXPECT_EQ(P01.mChildMaxWminusA,  L04.backIt());
    EXPECT_EQ(P01.mTypeAwithSingleA, true);

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
    calculateWHAC(P02);
    EXPECT_EQ(P02.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P02.mW, 2);
    EXPECT_EQ(P02.mH, 0);
    EXPECT_EQ(P02.mA, 0);
    EXPECT_EQ(P02.mCD, 0);
    EXPECT_EQ(P02.mChildMax1WminusH, pqTree.nil());
    EXPECT_EQ(P02.mChildMax2WminusH, pqTree.nil());
    EXPECT_EQ(P02.mTypeAwithSingleA, false);

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
    calculateWHAC(P03);
    EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P03.mW, 3);
    EXPECT_EQ(P03.mH, 0);
    EXPECT_EQ(P03.mA, 0);
    EXPECT_EQ(P03.mCD, 0);
    EXPECT_EQ(P03.mChildMax1WminusH, pqTree.nil());
    EXPECT_EQ(P03.mChildMax2WminusH, pqTree.nil());
    EXPECT_EQ(P03.mTypeAwithSingleA, false);


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
    Q01.mCD= 1;

    list<node_list_it_t> P04children;
    P04children.push_back(L11.backIt());
    P04children.push_back(L12.backIt());
    P04children.push_back(Q01.backIt());
    auto& P04 = createP(pqTree, P04children);
    calculateWHAC(P04);
    EXPECT_EQ(P04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P04.mW, 10);
    EXPECT_EQ(P04.mH, 10);
    EXPECT_EQ(P04.mA, 5);
    EXPECT_EQ(P04.mCD, 10);
    EXPECT_EQ(P04.mChildMax1WminusH, Q01.backIt());
    EXPECT_EQ(P04.mChildMax2WminusH, pqTree.nil());
    EXPECT_EQ(P04.mTypeAwithSingleA, true);
    EXPECT_EQ(P04.mChildMaxWminusA,  Q01.backIt());


    // Case 6: Node P Empty Empty Partial(W>H)

    auto& L13 = createL(pqTree);
    auto& L14 = createL(pqTree);
    auto& Q02 = createQ(pqTree,emptyChildren);
    Q02.mGeneration = pqTree.mGeneration;
    Q02.mPertinentType = JTSTreeNode::Partial;
    Q02.mW = 20;
    Q02.mH = 7;
    Q02.mA = 5;
    Q02.mCD= 3;

    list<node_list_it_t> P05children;
    P05children.push_back(L13.backIt());
    P05children.push_back(L14.backIt());
    P05children.push_back(Q02.backIt());
    auto& P05 = createP(pqTree, P05children);
    calculateWHAC(P05);
    EXPECT_EQ(P05.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P05.mW, 20);
    EXPECT_EQ(P05.mH, 7);
    EXPECT_EQ(P05.mA, 5);
    EXPECT_EQ(P05.mCD, 7);
    EXPECT_EQ(P05.mChildMax1WminusH, Q02.backIt());
    EXPECT_EQ(P05.mChildMax2WminusH, pqTree.nil());
    EXPECT_EQ(P05.mTypeAwithSingleA, true);
    EXPECT_EQ(P05.mChildMaxWminusA,  Q02.backIt());

    // Case 7: Node P Empty Partial(W=H) Partial(W=H)

    auto& L15 = createL(pqTree);
    auto& Q03 = createQ(pqTree,emptyChildren);
    Q03.mGeneration = pqTree.mGeneration;
    Q03.mPertinentType = JTSTreeNode::Partial;
    Q03.mW = 11;
    Q03.mH = 11;
    Q03.mA = 6;
    Q03.mCD = 8;
    auto& Q04 = createQ(pqTree,emptyChildren);
    Q04.mGeneration = pqTree.mGeneration;
    Q04.mPertinentType = JTSTreeNode::Partial;
    Q04.mW = 17;
    Q04.mH = 17;
    Q04.mA = 3;
    Q03.mCD = 7;
    list<node_list_it_t> P06children;
    P06children.push_back(L15.backIt());
    P06children.push_back(Q03.backIt());
    P06children.push_back(Q04.backIt());
    auto& P06 = createP(pqTree, P06children);
    calculateWHAC(P06);
    EXPECT_EQ(P06.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P06.mW, 28);
    EXPECT_EQ(P06.mH, 28);
    EXPECT_EQ(P06.mA, 14);
    EXPECT_EQ(P06.mCD, 28);
    EXPECT_EQ(P06.mChildMax1WminusH, Q04.backIt());
    EXPECT_EQ(P06.mChildMax2WminusH, Q03.backIt());
    EXPECT_EQ(P06.mTypeAwithSingleA, true);
    EXPECT_EQ(P06.mChildMaxWminusA,  Q04.backIt());


    // Case 8: Node P Empty Partial(W>H) Partial(W>H)

    auto& L16 = createL(pqTree);
    auto& Q05 = createQ(pqTree,emptyChildren);
    Q05.mGeneration = pqTree.mGeneration;
    Q05.mPertinentType = JTSTreeNode::Partial;
    Q05.mW = 30;
    Q05.mH = 11;
    Q05.mA = 6;
    Q05.mCD = 3;
    auto& Q06 = createQ(pqTree,emptyChildren);
    Q06.mGeneration = pqTree.mGeneration;
    Q06.mPertinentType = JTSTreeNode::Partial;
    Q06.mW = 40;
    Q06.mH = 17;
    Q06.mA = 3;
    Q06.mCD = 4;

    list<node_list_it_t> P07children;
    P07children.push_back(L16.backIt());
    P07children.push_back(Q05.backIt());
    P07children.push_back(Q06.backIt());
    auto& P07 = createP(pqTree, P07children);
    calculateWHAC(P07);
    EXPECT_EQ(P07.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P07.mW, 70);
    EXPECT_EQ(P07.mH, 47);
    EXPECT_EQ(P07.mA, 28);
    EXPECT_EQ(P07.mCD, 28);
    EXPECT_EQ(P07.mChildMax1WminusH, Q06.backIt());
    EXPECT_EQ(P07.mChildMax2WminusH, Q05.backIt());
    EXPECT_EQ(P07.mTypeAwithSingleA, false);


    // Case 9: Node P Empty Partial(W>H) Partial(W>H)

    auto& L17 = createL(pqTree);
    auto& Q07 = createQ(pqTree,emptyChildren);
    Q07.mGeneration = pqTree.mGeneration;
    Q07.mPertinentType = JTSTreeNode::Partial;
    Q07.mW = 30;
    Q07.mH = 14;
    Q07.mA = 6;
    Q07.mCD = 3;
    auto& Q08 = createQ(pqTree,emptyChildren);
    Q08.mGeneration = pqTree.mGeneration;
    Q08.mPertinentType = JTSTreeNode::Partial;
    Q08.mW = 40;
    Q08.mH = 20;
    Q08.mA = 3;
    Q08.mCD = 6;

    list<node_list_it_t> P08children;
    P08children.push_back(L17.backIt());
    P08children.push_back(Q07.backIt());
    P08children.push_back(Q08.backIt());
    auto& P08 = createP(pqTree, P08children);
    calculateWHAC(P08);
    EXPECT_EQ(P08.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P08.mW, 70);
    EXPECT_EQ(P08.mH, 50);
    EXPECT_EQ(P08.mA, 33);
    EXPECT_EQ(P08.mCD, 34);
    EXPECT_EQ(P08.mChildMax1WminusH, Q08.backIt());
    EXPECT_EQ(P08.mChildMax2WminusH, Q07.backIt());
    EXPECT_EQ(P08.mTypeAwithSingleA, true);
    EXPECT_EQ(P08.mChildMaxWminusA,  Q08.backIt());


    // Case 9: Node P Full Partial(W>H) Partial(W>H)

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
    Q10.mCD = 0;

    auto& Q11 = createQ(pqTree,emptyChildren);
    Q11.mGeneration = pqTree.mGeneration;
    Q11.mPertinentType = JTSTreeNode::Partial;
    Q11.mW = 40;
    Q11.mH = 20;
    Q11.mA = 3;
    Q11.mCD = 4;

    list<node_list_it_t> P09children;
    P09children.push_back(Q09.backIt());
    P09children.push_back(Q10.backIt());
    P09children.push_back(Q11.backIt());
    auto& P09 = createP(pqTree, P09children);
    calculateWHAC(P09);
    EXPECT_EQ(P09.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P09.mW, 72);
    EXPECT_EQ(P09.mH, 50);
    EXPECT_EQ(P09.mA, 34);
    EXPECT_EQ(P09.mCD, 34);
    EXPECT_EQ(P09.mChildMax1WminusH, Q11.backIt());
    EXPECT_EQ(P09.mChildMax2WminusH, Q10.backIt());
    EXPECT_EQ(P09.mTypeAwithSingleA, false);


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
    Q15.mCD = 28;

    auto& Q16 = createQ(pqTree,emptyChildren);
    Q16.mGeneration = pqTree.mGeneration;
    Q16.mPertinentType = JTSTreeNode::Partial;
    Q16.mW = 40;
    Q16.mH = 40;
    Q16.mA = 3;
    Q16.mCD = 5;

    list<node_list_it_t> P10children;
    P10children.push_back(Q12.backIt());
    P10children.push_back(Q13.backIt());
    P10children.push_back(Q14.backIt());
    P10children.push_back(Q15.backIt());
    P10children.push_back(Q16.backIt());

    auto& P10 = createP(pqTree, P10children);
    calculateWHAC(P10);
    EXPECT_EQ(P10.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P10.mW, 86);
    EXPECT_EQ(P10.mH, 70);
    EXPECT_EQ(P10.mA, 49);
    EXPECT_EQ(P10.mCD, 70);
    EXPECT_EQ(P10.mChildMax1WminusH, Q16.backIt());
    EXPECT_EQ(P10.mChildMax2WminusH, Q15.backIt());
    EXPECT_EQ(P10.mTypeAwithSingleA, true);
    EXPECT_EQ(P10.mChildMaxWminusA,  Q16.backIt());

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
    Q21.mCD = 18;

    list<node_list_it_t> P11children;
    P11children.push_back(Q17.backIt());
    P11children.push_back(Q18.backIt());
    P11children.push_back(Q19.backIt());
    P11children.push_back(Q20.backIt());
    P11children.push_back(Q21.backIt());

    auto& P11 = createP(pqTree, P11children);
    calculateWHAC(P11);
    EXPECT_EQ(P11.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P11.mW, 116);
    EXPECT_EQ(P11.mH, 70);
    EXPECT_EQ(P11.mA, 70);
    EXPECT_EQ(P11.mCD, 70);
    EXPECT_EQ(P11.mChildMax1WminusH, Q21.backIt());
    EXPECT_EQ(P11.mChildMax2WminusH, Q20.backIt());
    EXPECT_EQ(P11.mTypeAwithSingleA, false);
    EXPECT_EQ(P11.mChildMaxWminusA,  Q21.backIt());

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
    Q25.mCD = 2;

    auto& Q26 = createQ(pqTree,emptyChildren);
    Q26.mGeneration = pqTree.mGeneration;
    Q26.mPertinentType = JTSTreeNode::Partial;
    Q26.mW = 30;
    Q26.mH = 6;
    Q26.mA = 6;
    Q26.mCD = 2;

    list<node_list_it_t> P12children;
    P12children.push_back(Q22.backIt());
    P12children.push_back(Q23.backIt());
    P12children.push_back(Q24.backIt());
    P12children.push_back(Q25.backIt());
    P12children.push_back(Q26.backIt());

    auto& P12 = createP(pqTree, P12children);
    calculateWHAC(P12);
    EXPECT_EQ(P12.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P12.mW, 116);
    EXPECT_EQ(P12.mH, 33);
    EXPECT_EQ(P12.mA, 9);
    EXPECT_EQ(P12.mCD, 9);
    EXPECT_EQ(P12.mChildMax1WminusH, Q25.backIt());
    EXPECT_EQ(P12.mChildMax2WminusH, Q26.backIt());
    EXPECT_EQ(P12.mTypeAwithSingleA, false);
    EXPECT_EQ(P12.mChildMaxWminusA,  Q25.backIt());

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
    Q31.mCD = 2;

    list<node_list_it_t> Q33children;
    Q33children.push_back(Q27.backIt());
    Q33children.push_back(Q28.backIt());
    Q33children.push_back(Q29.backIt());
    Q33children.push_back(Q30.backIt());
    Q33children.push_back(Q31.backIt());

    auto& Q33 = createQ(pqTree, Q33children);
    calculateWHAC(Q33);
    EXPECT_EQ(Q33.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q33.mW, 10);
    EXPECT_EQ(Q33.mH, 6);
    EXPECT_EQ(Q33.mA, 4);
    EXPECT_EQ(Q33.mCD, 6);
    EXPECT_EQ(Q33.mChildBoundaryForQH, Q31.backIt());
    EXPECT_EQ(Q33.mQHFullOnListHead, false);
    EXPECT_EQ(Q33.mChildBoundaryOnHeadForQA, Q31.backIt());
    EXPECT_EQ(Q33.mChildBoundaryOnTailForQA, Q31.backIt());
    EXPECT_EQ(Q33.mTypeAwithSingleA, true);
    EXPECT_EQ(Q33.mChildMaxWminusA,  Q31.backIt());

    // Case 14: Q Partial Empty Empty

    auto& Q34 = createQ(pqTree,emptyChildren);
    Q34.mGeneration = pqTree.mGeneration;
    Q34.mPertinentType = JTSTreeNode::Partial;
    Q34.mW = 10;
    Q34.mH = 6;
    Q34.mA = 4;
    Q34.mCD = 5;

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
    EXPECT_EQ(Q39.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q39.mW, 10);
    EXPECT_EQ(Q39.mH, 6);
    EXPECT_EQ(Q39.mA, 4);
    EXPECT_EQ(Q39.mCD, 6);
    EXPECT_EQ(Q39.mChildBoundaryForQH, Q34.backIt());
    EXPECT_EQ(Q39.mQHFullOnListHead, true);
    EXPECT_EQ(Q39.mChildBoundaryOnHeadForQA, Q34.backIt());
    EXPECT_EQ(Q39.mChildBoundaryOnTailForQA, Q34.backIt());
    EXPECT_EQ(Q39.mTypeAwithSingleA, true);
    EXPECT_EQ(Q39.mChildMaxWminusA,  Q34.backIt());

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
    calculateWHAC(Q45);
    EXPECT_EQ(Q45.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q45.mW, 35);
    EXPECT_EQ(Q45.mH, 31);
    EXPECT_EQ(Q45.mA, 18);
    EXPECT_EQ(Q45.mCD, 28);
    EXPECT_EQ(Q45.mChildBoundaryForQH, Q40.backIt());
    EXPECT_EQ(Q45.mQHFullOnListHead, true);
    EXPECT_EQ(Q45.mTypeAwithSingleA, true);
    EXPECT_EQ(Q45.mChildMaxWminusA,  Q42.backIt());

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
    Q48.mCD = 7;

    auto& Q49 = createQ(pqTree,emptyChildren);
    Q49.mGeneration = pqTree.mGeneration;
    Q49.mPertinentType = JTSTreeNode::Partial;
    Q49.mW = 17;
    Q49.mH = 3;
    Q49.mA = 3;
    Q49.mCD = 5;

    auto& Q50 = createQ(pqTree,emptyChildren);
    Q50.mGeneration = pqTree.mGeneration;
    Q50.mPertinentType = JTSTreeNode::Partial;
    Q50.mW = 16;
    Q50.mH = 8;
    Q50.mA = 5;
    Q50.mCD = 7;

    auto& Q51 = createQ(pqTree,emptyChildren);
    Q51.mGeneration = pqTree.mGeneration;
    Q51.mPertinentType = JTSTreeNode::Partial;
    Q51.mW = 22;
    Q51.mH = 3;
    Q51.mA = 2;
    Q51.mCD = 7;

    list<node_list_it_t> Q52children;
    Q52children.push_back(Q46.backIt());
    Q52children.push_back(Q47.backIt());
    Q52children.push_back(Q48.backIt());
    Q52children.push_back(Q49.backIt());
    Q52children.push_back(Q50.backIt());
    Q52children.push_back(Q51.backIt());

    auto& Q52 = createQ(pqTree, Q52children);
    calculateWHAC(Q52);
    EXPECT_EQ(Q52.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q52.mW, 120);
    EXPECT_EQ(Q52.mH, 97);
    EXPECT_EQ(Q52.mChildBoundaryForQH, Q46.backIt());
    EXPECT_EQ(Q52.mQHFullOnListHead, true);
    EXPECT_EQ(Q52.mA, 90);
    EXPECT_EQ(Q52.mTypeAwithSingleA, false);
    EXPECT_EQ(Q52.mCD, 78);
    EXPECT_EQ(Q52.mChildBoundaryOnHeadForQA, Q46.backIt());
    EXPECT_EQ(Q52.mChildBoundaryOnTailForQA, Q47.backIt());

    // Case 17: Q Partial Partial Partial Partial Partial Partial #2
    auto& Q53 = createQ(pqTree,emptyChildren);
    Q53.mGeneration = pqTree.mGeneration;
    Q53.mPertinentType = JTSTreeNode::Partial;
    Q53.mW = 22;
    Q53.mH = 3;
    Q53.mA = 2;
    Q53.mCD = 5;

    auto& Q54 = createQ(pqTree,emptyChildren);
    Q54.mGeneration = pqTree.mGeneration;
    Q54.mPertinentType = JTSTreeNode::Partial;
    Q54.mW = 16;
    Q54.mH = 8;
    Q54.mA = 5;
    Q54.mCD = 7;

    auto& Q55 = createQ(pqTree,emptyChildren);
    Q55.mGeneration = pqTree.mGeneration;
    Q55.mPertinentType = JTSTreeNode::Partial;
    Q55.mW = 17;
    Q55.mH = 3;
    Q55.mA = 3;
    Q55.mCD = 4;

    auto& Q56 = createQ(pqTree,emptyChildren);
    Q56.mGeneration = pqTree.mGeneration;
    Q56.mPertinentType = JTSTreeNode::Partial;
    Q56.mW = 15;
    Q56.mH = 8;
    Q56.mA = 4;
    Q56.mCD = 7;

    auto& Q57 = createQ(pqTree,emptyChildren);
    Q57.mGeneration = pqTree.mGeneration;
    Q57.mPertinentType = JTSTreeNode::Partial;
    Q57.mW = 20;
    Q57.mH = 13;
    Q57.mA = 12;
    Q57.mCD = 11;

    auto& Q58 = createQ(pqTree,emptyChildren);
    Q58.mGeneration = pqTree.mGeneration;
    Q58.mPertinentType = JTSTreeNode::Partial;
    Q58.mW = 30;
    Q58.mH = 7;
    Q58.mA = 4;
    Q58.mCD = 5;

    list<node_list_it_t> Q59children;
    Q59children.push_back(Q53.backIt());
    Q59children.push_back(Q54.backIt());
    Q59children.push_back(Q55.backIt());
    Q59children.push_back(Q56.backIt());
    Q59children.push_back(Q57.backIt());
    Q59children.push_back(Q58.backIt());

    auto& Q59 = createQ(pqTree, Q59children);
    calculateWHAC(Q59);
    EXPECT_EQ(Q59.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q59.mW, 120);
    EXPECT_EQ(Q59.mH, 97);
    EXPECT_EQ(Q59.mChildBoundaryForQH, Q58.backIt());
    EXPECT_EQ(Q59.mQHFullOnListHead, false);
    EXPECT_EQ(Q59.mA, 90);
    EXPECT_EQ(Q59.mTypeAwithSingleA, false);
    EXPECT_EQ(Q59.mChildBoundaryOnHeadForQA, Q57.backIt());
    EXPECT_EQ(Q59.mChildBoundaryOnTailForQA, Q58.backIt());
    EXPECT_EQ(Q59.mCD, 78);

    // Case 18: Q Partial Partial Partial Partial Partial Partial #3

    auto& Q60 = createQ(pqTree,emptyChildren);
    Q60.mGeneration = pqTree.mGeneration;
    Q60.mPertinentType = JTSTreeNode::Partial;
    Q60.mW = 17;
    Q60.mH = 11;
    Q60.mA = 3;
    Q60.mCD = 10;

    auto& Q61 = createQ(pqTree,emptyChildren);
    Q61.mGeneration = pqTree.mGeneration;
    Q61.mPertinentType = JTSTreeNode::Partial;
    Q61.mW = 30;
    Q61.mH = 7;
    Q61.mA = 4;
    Q61.mCD = 8;

    auto& Q62 = createQ(pqTree,emptyChildren);
    Q62.mGeneration = pqTree.mGeneration;
    Q62.mPertinentType = JTSTreeNode::Partial;
    Q62.mW = 20;
    Q62.mH = 13;
    Q62.mA = 12;
    Q62.mCD = 11;

    auto& Q63 = createQ(pqTree,emptyChildren);
    Q63.mGeneration = pqTree.mGeneration;
    Q63.mPertinentType = JTSTreeNode::Partial;
    Q63.mW = 15;
    Q63.mH = 8;
    Q63.mA = 4;
    Q63.mCD = 7;

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
    Q65.mCD = 5;

    list<node_list_it_t> Q66children;
    Q66children.push_back(Q60.backIt());
    Q66children.push_back(Q61.backIt());
    Q66children.push_back(Q62.backIt());
    Q66children.push_back(Q63.backIt());
    Q66children.push_back(Q64.backIt());
    Q66children.push_back(Q65.backIt());

    auto& Q66 = createQ(pqTree, Q66children);
    calculateWHAC(Q66);
    EXPECT_EQ(Q66.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q66.mW, 120);
    EXPECT_EQ(Q66.mH, 101);
    EXPECT_EQ(Q66.mChildBoundaryForQH, Q65.backIt());
    EXPECT_EQ(Q66.mQHFullOnListHead, false);
    EXPECT_EQ(Q66.mA, 90);
    EXPECT_EQ(Q66.mTypeAwithSingleA, false);
    EXPECT_EQ(Q66.mChildBoundaryOnHeadForQA, Q61.backIt());
    EXPECT_EQ(Q66.mChildBoundaryOnTailForQA, Q62.backIt());
    EXPECT_EQ(Q66.mCD, 95);

    // Case 19: Q Partial Full Full Partial Partial Full Partial Full Full Full
    //            Partial Partial

    auto& Q67 = createQ(pqTree,emptyChildren);
    Q67.mGeneration = pqTree.mGeneration;
    Q67.mPertinentType = JTSTreeNode::Partial;
    Q67.mW = 17;
    Q67.mH = 11;
    Q67.mA = 3;
    Q67.mCD = 8;

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
    Q70.mCD = 5;

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
    Q78.mCD = 3;

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
    calculateWHAC(Q79);
    EXPECT_EQ(Q79.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q79.mW, 204);
    EXPECT_EQ(Q79.mH, 185);
    EXPECT_EQ(Q79.mChildBoundaryForQH, Q78.backIt());
    EXPECT_EQ(Q79.mQHFullOnListHead, false);
    EXPECT_EQ(Q79.mA, 145);
    EXPECT_EQ(Q79.mTypeAwithSingleA, false);
    EXPECT_EQ(Q79.mChildBoundaryOnHeadForQA, Q67.backIt());
    EXPECT_EQ(Q79.mChildBoundaryOnTailForQA, Q70.backIt());
    EXPECT_EQ(Q79.mCD, 179);

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
    calculateWHAC(Q85);
    EXPECT_EQ(Q85.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q85.mW, 62);
    EXPECT_EQ(Q85.mH, 49);
    EXPECT_EQ(Q85.mChildBoundaryForQH, Q84.backIt());
    EXPECT_EQ(Q85.mQHFullOnListHead, false);
    EXPECT_EQ(Q85.mA, 28);
    EXPECT_EQ(Q85.mTypeAwithSingleA, false);
    EXPECT_EQ(Q85.mChildBoundaryOnHeadForQA, Q80.backIt());
    EXPECT_EQ(Q85.mChildBoundaryOnTailForQA, Q82.backIt());
    EXPECT_EQ(Q85.mCD, 37);

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
    calculateWHAC(Q89);
    EXPECT_EQ(Q89.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q89.mW, 6);
    EXPECT_EQ(Q89.mH, 6);
    EXPECT_EQ(Q89.mA, 0);
    EXPECT_EQ(Q89.mTypeAwithSingleA, true);
    EXPECT_EQ(Q89.mChildMaxWminusA,     Q87.backIt());
    EXPECT_EQ(Q89.mCD, 6);

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
    calculateWHAC(Q93);
    EXPECT_EQ(Q93.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q93.mW, 16);
    EXPECT_EQ(Q93.mH, 0);
    EXPECT_EQ(Q93.mChildBoundaryForQH, Q91.backIt());
    EXPECT_EQ(Q93.mQHFullOnListHead, false);
    EXPECT_EQ(Q93.mA, 0);
    EXPECT_EQ(Q93.mTypeAwithSingleA, false);
    EXPECT_EQ(Q93.mChildBoundaryOnHeadForQA, Q91.backIt());
    EXPECT_EQ(Q93.mChildBoundaryOnTailForQA, Q92.backIt());
    EXPECT_EQ(Q93.mCD, 0);

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
    calculateWHAC(Q97);
    EXPECT_EQ(Q97.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q97.mW, 26);
    EXPECT_EQ(Q97.mH, 5);
    EXPECT_EQ(Q97.mChildBoundaryForQH, Q94.backIt());
    EXPECT_EQ(Q97.mQHFullOnListHead, false);
    EXPECT_EQ(Q97.mA, 5);
    EXPECT_EQ(Q97.mTypeAwithSingleA, false);
    EXPECT_EQ(Q97.mChildBoundaryOnHeadForQA, Q94.backIt());
    EXPECT_EQ(Q97.mChildBoundaryOnTailForQA, Q96.backIt());
    EXPECT_EQ(Q97.mCD, 4);

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
    Q100.mCD = 10;

    list<node_list_it_t> Q101children;
    Q101children.push_back(Q98.backIt());
    Q101children.push_back(Q99.backIt());
    Q101children.push_back(Q100.backIt());

    auto& Q101 = createQ(pqTree, Q101children);
    calculateWHAC(Q101);
    EXPECT_EQ(Q101.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q101.mW, 26);
    EXPECT_EQ(Q101.mH, 5);
    EXPECT_EQ(Q101.mChildBoundaryForQH, Q100.backIt());
    EXPECT_EQ(Q101.mQHFullOnListHead, true);
    EXPECT_EQ(Q101.mA, 5);
    EXPECT_EQ(Q101.mTypeAwithSingleA, false);
    EXPECT_EQ(Q101.mChildBoundaryOnHeadForQA, Q98.backIt());
    EXPECT_EQ(Q101.mChildBoundaryOnTailForQA, Q100.backIt());
    EXPECT_EQ(Q101.mCD, 10);

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
    Q103.mCD = 6;

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
    calculateWHAC(Q105);
    EXPECT_EQ(Q105.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q105.mW, 25);
    EXPECT_EQ(Q105.mH, 11);
    EXPECT_EQ(Q105.mChildBoundaryForQH, Q103.backIt());
    EXPECT_EQ(Q105.mQHFullOnListHead, false);
    EXPECT_EQ(Q105.mA, 11);
    EXPECT_EQ(Q105.mTypeAwithSingleA, false);
    EXPECT_EQ(Q105.mChildBoundaryOnHeadForQA, Q103.backIt());
    EXPECT_EQ(Q105.mChildBoundaryOnTailForQA, Q104.backIt());
    EXPECT_EQ(Q105.mCD, 6);

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
    calculateWHAC(Q109);
    EXPECT_EQ(Q109.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(Q109.mW, 21);
    EXPECT_EQ(Q109.mH, 0);
    EXPECT_EQ(Q109.mA, 0);
    EXPECT_EQ(Q109.mCD, 0);

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
    calculateWHAC(Q113);
    EXPECT_EQ(Q113.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q113.mW, 20);
    EXPECT_EQ(Q113.mH, 8);
    EXPECT_EQ(Q113.mChildBoundaryForQH, Q112.backIt());
    EXPECT_EQ(Q113.mQHFullOnListHead, false);
    EXPECT_EQ(Q113.mA, 6);
    EXPECT_EQ(Q113.mTypeAwithSingleA, true);
    EXPECT_EQ(Q113.mChildMaxWminusA,  Q112.backIt());
    EXPECT_EQ(Q113.mCD, 8);

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
    calculateWHAC(Q117);
    EXPECT_EQ(Q117.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q117.mW, 20);
    EXPECT_EQ(Q117.mH, 8);
    EXPECT_EQ(Q117.mChildBoundaryForQH, Q114.backIt());
    EXPECT_EQ(Q117.mQHFullOnListHead, true);
    EXPECT_EQ(Q117.mA, 6);
    EXPECT_EQ(Q117.mTypeAwithSingleA, true);
    EXPECT_EQ(Q117.mChildMaxWminusA,  Q114.backIt());
    EXPECT_EQ(Q117.mCD, 8);

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
    Q120.mCD = 7;

    list<node_list_it_t> Q121children;
    Q121children.push_back(Q118.backIt());
    Q121children.push_back(Q119.backIt());
    Q121children.push_back(Q120.backIt());

    auto& Q121 = createQ(pqTree, Q121children);
    calculateWHAC(Q121);
    EXPECT_EQ(Q121.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q121.mW, 45);
    EXPECT_EQ(Q121.mH, 28);
    EXPECT_EQ(Q121.mChildBoundaryForQH, Q120.backIt());
    EXPECT_EQ(Q121.mQHFullOnListHead, false);
    EXPECT_EQ(Q121.mA, 8);
    EXPECT_EQ(Q121.mTypeAwithSingleA, false);
    EXPECT_EQ(Q121.mChildBoundaryOnHeadForQA, Q119.backIt());
    EXPECT_EQ(Q121.mChildBoundaryOnTailForQA, Q120.backIt());
    EXPECT_EQ(Q121.mCD, 28);

    // Case 30: Q Partial Full Empty
    auto& Q122 = createQ(pqTree,emptyChildren);
    Q122.mGeneration = pqTree.mGeneration;
    Q122.mPertinentType = JTSTreeNode::Partial;
    Q122.mW = 25;
    Q122.mH = 8;
    Q122.mA = 6;
    Q122.mCD = 7;

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
    calculateWHAC(Q125);
    EXPECT_EQ(Q125.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q125.mW, 45);
    EXPECT_EQ(Q125.mH, 28);
    EXPECT_EQ(Q125.mChildBoundaryForQH, Q122.backIt());
    EXPECT_EQ(Q125.mQHFullOnListHead, true);
    EXPECT_EQ(Q125.mA, 8);
    EXPECT_EQ(Q125.mTypeAwithSingleA, false);
    EXPECT_EQ(Q125.mChildBoundaryOnHeadForQA, Q122.backIt());
    EXPECT_EQ(Q125.mChildBoundaryOnTailForQA, Q123.backIt());
    EXPECT_EQ(Q125.mCD, 28);


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
    calculateWHAC(Q129);
    EXPECT_EQ(Q129.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q129.mW, 35);
    EXPECT_EQ(Q129.mH, 10);
    EXPECT_EQ(Q129.mChildBoundaryForQH, Q126.backIt());
    EXPECT_EQ(Q129.mQHFullOnListHead, true);
    EXPECT_EQ(Q129.mA, 10);
    EXPECT_EQ(Q129.mTypeAwithSingleA, true);
    EXPECT_EQ(Q129.mChildMaxWminusA,  Q126.backIt());
    EXPECT_EQ(Q129.mCD, 0);

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
    Q132.mCD = 4;

    list<node_list_it_t> Q133children;
    Q133children.push_back(Q130.backIt());
    Q133children.push_back(Q131.backIt());
    Q133children.push_back(Q132.backIt());

    auto& Q133 = createQ(pqTree, Q133children);
    calculateWHAC(Q133);
    EXPECT_EQ(Q133.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q133.mW, 42);
    EXPECT_EQ(Q133.mH, 24);
    EXPECT_EQ(Q133.mChildBoundaryForQH, Q130.backIt());
    EXPECT_EQ(Q133.mQHFullOnListHead, true);
    EXPECT_EQ(Q133.mA, 21);
    EXPECT_EQ(Q133.mTypeAwithSingleA, true);
    EXPECT_EQ(Q133.mChildMaxWminusA,  Q130.backIt());
    EXPECT_EQ(Q133.mCD, 12);


    // Case 33: P Partial Empty Partial Full Full Empty
    auto& Q134 = createQ(pqTree,emptyChildren);
    Q134.mGeneration = pqTree.mGeneration;
    Q134.mPertinentType = JTSTreeNode::Partial;
    Q134.mW = 25;
    Q134.mH = 7;
    Q134.mA = 4;
    Q134.mCD = 3;

    auto& Q135 = createQ(pqTree,emptyChildren);

    auto& Q136 = createQ(pqTree,emptyChildren);
    Q136.mGeneration = pqTree.mGeneration;
    Q136.mPertinentType = JTSTreeNode::Partial;
    Q136.mW = 17;
    Q136.mH = 5;
    Q136.mA = 5;
    Q136.mCD = 4;

    auto& Q137 = createQ(pqTree,emptyChildren);
    Q137.mGeneration = pqTree.mGeneration;
    Q137.mPertinentType = JTSTreeNode::Full;
    Q137.mW = 20;
    Q137.mH = 0;
    Q137.mA = 0;
    Q137.mCD = 0;

    auto& Q138 = createQ(pqTree,emptyChildren);
    Q138.mGeneration = pqTree.mGeneration;
    Q138.mPertinentType = JTSTreeNode::Full;
    Q138.mW = 20;
    Q138.mH = 0;
    Q138.mA = 0;
    Q138.mCD = 0;


    auto& Q139 = createQ(pqTree,emptyChildren);

    list<node_list_it_t> P140children;
    P140children.push_back(Q134.backIt());
    P140children.push_back(Q135.backIt());
    P140children.push_back(Q136.backIt());
    P140children.push_back(Q137.backIt());
    P140children.push_back(Q138.backIt());
    P140children.push_back(Q139.backIt());

    auto& P140 = createP(pqTree, P140children);
    calculateWHAC(P140);
    EXPECT_EQ(P140.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P140.mW, 82);
    EXPECT_EQ(P140.mH, 24);
    EXPECT_EQ(P140.mA, 12);
    EXPECT_EQ(P140.mTypeAwithSingleA, false);
    EXPECT_EQ(P140.mCD, 12);

    // Case 34: P Full Partial Full Full Full
    auto& Q141 = createQ(pqTree,emptyChildren);
    Q141.mGeneration = pqTree.mGeneration;
    Q141.mPertinentType = JTSTreeNode::Full;
    Q141.mW = 20;
    Q141.mH = 0;
    Q141.mA = 0;
    Q141.mCD = 0;

    auto& Q142 = createQ(pqTree,emptyChildren);
    Q142.mGeneration = pqTree.mGeneration;
    Q142.mPertinentType = JTSTreeNode::Partial;
    Q142.mW = 25;
    Q142.mH = 7;
    Q142.mA = 4;
    Q142.mCD = 10;

    auto& Q143 = createQ(pqTree,emptyChildren);
    Q143.mGeneration = pqTree.mGeneration;
    Q143.mPertinentType = JTSTreeNode::Full;
    Q143.mW = 30;
    Q143.mH = 0;
    Q143.mA = 0;
    Q143.mCD = 0;

    auto& Q144 = createQ(pqTree,emptyChildren);
    Q144.mGeneration = pqTree.mGeneration;
    Q144.mPertinentType = JTSTreeNode::Full;
    Q144.mW = 30;
    Q144.mH = 0;
    Q144.mA = 0;
    Q144.mCD = 0;

    list<node_list_it_t> P145children;
    P145children.push_back(Q141.backIt());
    P145children.push_back(Q142.backIt());
    P145children.push_back(Q143.backIt());
    P145children.push_back(Q144.backIt());

    auto& P145 = createP(pqTree, P145children);
    calculateWHAC(P145);
    EXPECT_EQ(P145.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P145.mW, 105);
    EXPECT_EQ(P145.mH, 7);
    EXPECT_EQ(P145.mA, 7);
    EXPECT_EQ(P145.mTypeAwithSingleA, false);
    EXPECT_EQ(P145.mCD, 10);


    // Case 35: P Full Empty Full Full Full
    auto& Q146 = createQ(pqTree,emptyChildren);
    Q146.mGeneration = pqTree.mGeneration;
    Q146.mPertinentType = JTSTreeNode::Full;
    Q146.mW = 20;
    Q146.mH = 0;
    Q146.mA = 0;
    Q146.mCD = 0;

    auto& Q147 = createQ(pqTree,emptyChildren);

    auto& Q148 = createQ(pqTree,emptyChildren);
    Q148.mGeneration = pqTree.mGeneration;
    Q148.mPertinentType = JTSTreeNode::Full;
    Q148.mW = 30;
    Q148.mH = 0;
    Q148.mA = 0;
    Q148.mCD = 0;

    auto& Q149 = createQ(pqTree,emptyChildren);
    Q149.mGeneration = pqTree.mGeneration;
    Q149.mPertinentType = JTSTreeNode::Full;
    Q149.mW = 30;
    Q149.mH = 0;
    Q149.mA = 0;
    Q149.mCD = 0;

    list<node_list_it_t> P150children;
    P150children.push_back(Q146.backIt());
    P150children.push_back(Q147.backIt());
    P150children.push_back(Q148.backIt());
    P150children.push_back(Q149.backIt());

    auto& P150 = createP(pqTree, P150children);
    calculateWHAC(P150);
    EXPECT_EQ(P150.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P150.mW, 80);
    EXPECT_EQ(P150.mH, 0);
    EXPECT_EQ(P150.mA, 0);
    EXPECT_EQ(P150.mTypeAwithSingleA, false);
    EXPECT_EQ(P150.mCD, 0);

}


/**  @brief tests JTSPlanarizer::findWHA()
 */
TEST_F(JTSPlanarizerTests, Test7) {

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

    EXPECT_EQ(root, L01.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

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

    EXPECT_EQ(root, P01.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L02.mW, 1);
    EXPECT_EQ(L02.mH, 0);
    EXPECT_EQ(L02.mA, 0);
    EXPECT_EQ(L02.mCD, 0);


    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);


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

    EXPECT_EQ(root, P01.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L02.mW, 1);
    EXPECT_EQ(L02.mH, 0);
    EXPECT_EQ(L02.mA, 0);
    EXPECT_EQ(L02.mCD, 0);


    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);


    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P01.mW, 3);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

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

    EXPECT_EQ(root, Q03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);

    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L14.mW, 1);
    EXPECT_EQ(L14.mH, 0);
    EXPECT_EQ(L14.mA, 0);
    EXPECT_EQ(L14.mCD, 0);

    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L15.mW, 1);
    EXPECT_EQ(L15.mH, 0);
    EXPECT_EQ(L15.mA, 0);
    EXPECT_EQ(L15.mCD, 0);

    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mW, 4);
    EXPECT_EQ(Q03.mH, 4);
    EXPECT_EQ(Q03.mA, 0);
    EXPECT_EQ(Q03.mCD, 4);

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

    EXPECT_EQ(root, Q03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);

    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L04.mW, 1);
    EXPECT_EQ(L04.mH, 0);
    EXPECT_EQ(L04.mA, 0);
    EXPECT_EQ(L04.mCD, 0);

    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L05.mW, 1);
    EXPECT_EQ(L05.mH, 0);
    EXPECT_EQ(L05.mA, 0);
    EXPECT_EQ(L05.mCD, 0);

    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L14.mW, 1);
    EXPECT_EQ(L14.mH, 0);
    EXPECT_EQ(L14.mA, 0);
    EXPECT_EQ(L14.mCD, 0);

    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L15.mW, 1);
    EXPECT_EQ(L15.mH, 0);
    EXPECT_EQ(L15.mA, 0);
    EXPECT_EQ(L15.mCD, 0);

    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L16.mW, 1);
    EXPECT_EQ(L16.mH, 0);
    EXPECT_EQ(L16.mA, 0);
    EXPECT_EQ(L16.mCD, 0);

    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mW, 7);
    EXPECT_EQ(Q03.mH, 4);
    EXPECT_EQ(Q03.mA, 1);
    EXPECT_EQ(Q03.mCD, 4);

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

    EXPECT_EQ(root, P03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);

    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L04.mW, 1);
    EXPECT_EQ(L04.mH, 0);
    EXPECT_EQ(L04.mA, 0);
    EXPECT_EQ(L04.mCD, 0);

    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L05.mW, 1);
    EXPECT_EQ(L05.mH, 0);
    EXPECT_EQ(L05.mA, 0);
    EXPECT_EQ(L05.mCD, 0);

    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L14.mW, 1);
    EXPECT_EQ(L14.mH, 0);
    EXPECT_EQ(L14.mA, 0);
    EXPECT_EQ(L14.mCD, 0);

    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L15.mW, 1);
    EXPECT_EQ(L15.mH, 0);
    EXPECT_EQ(L15.mA, 0);
    EXPECT_EQ(L15.mCD, 0);

    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L16.mW, 1);
    EXPECT_EQ(L16.mH, 0);
    EXPECT_EQ(L16.mA, 0);
    EXPECT_EQ(L16.mCD, 0);

    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mW, 7);
    EXPECT_EQ(Q03.mH, 4);
    EXPECT_EQ(Q03.mA, 1);
    EXPECT_EQ(Q03.mCD, 4);

    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L21.mW, 1);
    EXPECT_EQ(L21.mH, 0);
    EXPECT_EQ(L21.mA, 0);
    EXPECT_EQ(L21.mCD, 0);

    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P03.mW, 8);
    EXPECT_EQ(P03.mH, 4);
    EXPECT_EQ(P03.mA, 2);
    EXPECT_EQ(P03.mCD, 4);

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

    EXPECT_EQ(root, P03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);

    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L04.mW, 1);
    EXPECT_EQ(L04.mH, 0);
    EXPECT_EQ(L04.mA, 0);
    EXPECT_EQ(L04.mCD, 0);

    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L05.mW, 1);
    EXPECT_EQ(L05.mH, 0);
    EXPECT_EQ(L05.mA, 0);
    EXPECT_EQ(L05.mCD, 0);

    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L14.mW, 1);
    EXPECT_EQ(L14.mH, 0);
    EXPECT_EQ(L14.mA, 0);
    EXPECT_EQ(L14.mCD, 0);

    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L15.mW, 1);
    EXPECT_EQ(L15.mH, 0);
    EXPECT_EQ(L15.mA, 0);
    EXPECT_EQ(L15.mCD, 0);

    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L16.mW, 1);
    EXPECT_EQ(L16.mH, 0);
    EXPECT_EQ(L16.mA, 0);
    EXPECT_EQ(L16.mCD, 0);

    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L17.mW, 1);
    EXPECT_EQ(L17.mH, 0);
    EXPECT_EQ(L17.mA, 0);
    EXPECT_EQ(L17.mCD, 0);

    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mW, 7);
    EXPECT_EQ(Q03.mH, 4);
    EXPECT_EQ(Q03.mA, 1);
    EXPECT_EQ(Q03.mCD, 4);

    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q04.mW, 1);
    EXPECT_EQ(Q04.mH, 0);
    EXPECT_EQ(Q04.mA, 0);
    EXPECT_EQ(Q04.mCD, 0);

    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L21.mW, 1);
    EXPECT_EQ(L21.mH, 0);
    EXPECT_EQ(L21.mA, 0);
    EXPECT_EQ(L21.mCD, 0);

    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P03.mW, 9);
    EXPECT_EQ(P03.mH, 5);
    EXPECT_EQ(P03.mA, 3);
    EXPECT_EQ(P03.mCD, 4);

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

    EXPECT_EQ(root, Q05.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);

    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L04.mW, 1);
    EXPECT_EQ(L04.mH, 0);
    EXPECT_EQ(L04.mA, 0);
    EXPECT_EQ(L04.mCD, 0);

    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L05.mW, 1);
    EXPECT_EQ(L05.mH, 0);
    EXPECT_EQ(L05.mA, 0);
    EXPECT_EQ(L05.mCD, 0);

    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L07.mW, 1);
    EXPECT_EQ(L07.mH, 0);
    EXPECT_EQ(L07.mA, 0);
    EXPECT_EQ(L07.mCD, 0);

    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L08.mW, 1);
    EXPECT_EQ(L08.mH, 0);
    EXPECT_EQ(L08.mA, 0);
    EXPECT_EQ(L08.mCD, 0);

    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L09.mW, 1);
    EXPECT_EQ(L09.mH, 0);
    EXPECT_EQ(L09.mA, 0);
    EXPECT_EQ(L09.mCD, 0);

    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P02.mW, 3);
    EXPECT_EQ(P02.mH, 0);
    EXPECT_EQ(P02.mA, 0);
    EXPECT_EQ(P02.mCD, 0);

    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L14.mW, 1);
    EXPECT_EQ(L14.mH, 0);
    EXPECT_EQ(L14.mA, 0);
    EXPECT_EQ(L14.mCD, 0);

    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L15.mW, 1);
    EXPECT_EQ(L15.mH, 0);
    EXPECT_EQ(L15.mA, 0);
    EXPECT_EQ(L15.mCD, 0);

    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L16.mW, 1);
    EXPECT_EQ(L16.mH, 0);
    EXPECT_EQ(L16.mA, 0);
    EXPECT_EQ(L16.mCD, 0);

    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L17.mW, 1);
    EXPECT_EQ(L17.mH, 0);
    EXPECT_EQ(L17.mA, 0);
    EXPECT_EQ(L17.mCD, 0);

    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mW, 7);
    EXPECT_EQ(Q03.mH, 4);
    EXPECT_EQ(Q03.mA, 1);
    EXPECT_EQ(Q03.mCD, 4);

    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q04.mW, 4);
    EXPECT_EQ(Q04.mH, 0);
    EXPECT_EQ(Q04.mA, 0);
    EXPECT_EQ(Q04.mCD, 0);

    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L21.mW, 1);
    EXPECT_EQ(L21.mH, 0);
    EXPECT_EQ(L21.mA, 0);
    EXPECT_EQ(L21.mCD, 0);

    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P03.mW, 12);
    EXPECT_EQ(P03.mH, 7);
    EXPECT_EQ(P03.mA, 4);
    EXPECT_EQ(P03.mCD, 4);

    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L22.mW, 1);
    EXPECT_EQ(L22.mH, 0);
    EXPECT_EQ(L22.mA, 0);
    EXPECT_EQ(L22.mCD, 0);

    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q05.mW, 13);
    EXPECT_EQ(Q05.mH, 7);
    EXPECT_EQ(Q05.mA, 5);
    EXPECT_EQ(Q05.mCD, 7);

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

    EXPECT_EQ(root, P04.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L01.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L01.mW, 1);
    EXPECT_EQ(L01.mH, 0);
    EXPECT_EQ(L01.mA, 0);
    EXPECT_EQ(L01.mCD, 0);

    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L03.mW, 1);
    EXPECT_EQ(L03.mH, 0);
    EXPECT_EQ(L03.mA, 0);
    EXPECT_EQ(L03.mCD, 0);

    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L04.mW, 1);
    EXPECT_EQ(L04.mH, 0);
    EXPECT_EQ(L04.mA, 0);
    EXPECT_EQ(L04.mCD, 0);

    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L05.mW, 1);
    EXPECT_EQ(L05.mH, 0);
    EXPECT_EQ(L05.mA, 0);
    EXPECT_EQ(L05.mCD, 0);

    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L07.mW, 1);
    EXPECT_EQ(L07.mH, 0);
    EXPECT_EQ(L07.mA, 0);
    EXPECT_EQ(L07.mCD, 0);

    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L08.mW, 1);
    EXPECT_EQ(L08.mH, 0);
    EXPECT_EQ(L08.mA, 0);
    EXPECT_EQ(L08.mCD, 0);

    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L09.mW, 1);
    EXPECT_EQ(L09.mH, 0);
    EXPECT_EQ(L09.mA, 0);
    EXPECT_EQ(L09.mCD, 0);

    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P01.mW, 2);
    EXPECT_EQ(P01.mH, 0);
    EXPECT_EQ(P01.mA, 0);
    EXPECT_EQ(P01.mCD, 0);

    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(P02.mW, 3);
    EXPECT_EQ(P02.mH, 0);
    EXPECT_EQ(P02.mA, 0);
    EXPECT_EQ(P02.mCD, 0);

    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L14.mW, 1);
    EXPECT_EQ(L14.mH, 0);
    EXPECT_EQ(L14.mA, 0);
    EXPECT_EQ(L14.mCD, 0);

    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L15.mW, 1);
    EXPECT_EQ(L15.mH, 0);
    EXPECT_EQ(L15.mA, 0);
    EXPECT_EQ(L15.mCD, 0);

    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L16.mW, 1);
    EXPECT_EQ(L16.mH, 0);
    EXPECT_EQ(L16.mA, 0);
    EXPECT_EQ(L16.mCD, 0);

    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L17.mW, 1);
    EXPECT_EQ(L17.mH, 0);
    EXPECT_EQ(L17.mA, 0);
    EXPECT_EQ(L17.mCD, 0);

    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q03.mW, 7);
    EXPECT_EQ(Q03.mH, 4);
    EXPECT_EQ(Q03.mA, 1);
    EXPECT_EQ(Q03.mCD, 4);

    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q04.mW, 4);
    EXPECT_EQ(Q04.mH, 0);
    EXPECT_EQ(Q04.mA, 0);
    EXPECT_EQ(Q04.mCD, 0);

    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L21.mW, 1);
    EXPECT_EQ(L21.mH, 0);
    EXPECT_EQ(L21.mA, 0);
    EXPECT_EQ(L21.mCD, 0);

    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P03.mW, 12);
    EXPECT_EQ(P03.mH, 7);
    EXPECT_EQ(P03.mA, 4);
    EXPECT_EQ(P03.mCD, 4);

    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q05.mW, 12);
    EXPECT_EQ(Q05.mH, 12);
    EXPECT_EQ(Q05.mA, 4);
    EXPECT_EQ(Q05.mCD, 12);

    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L24.mW, 1);
    EXPECT_EQ(L24.mH, 0);
    EXPECT_EQ(L24.mA, 0);
    EXPECT_EQ(L24.mCD, 0);

    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P04.mW, 13);
    EXPECT_EQ(P04.mH, 12);
    EXPECT_EQ(P04.mA, 5);
    EXPECT_EQ(P04.mCD, 12);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05*L25
    //                       /  |  \
    //                     *L22 |*L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
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

    EXPECT_EQ(root, P04.backIt());

    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L22.mW, 1);
    EXPECT_EQ(L22.mH, 0);
    EXPECT_EQ(L22.mA, 0);
    EXPECT_EQ(L22.mCD, 0);

    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L23.mW, 1);
    EXPECT_EQ(L23.mH, 0);
    EXPECT_EQ(L23.mA, 0);
    EXPECT_EQ(L23.mCD, 0);

    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(Q05.mW, 2);
    EXPECT_EQ(Q05.mH, 1);
    EXPECT_EQ(Q05.mA, 1);
    EXPECT_EQ(Q05.mCD, 0);

    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L24.mW, 1);
    EXPECT_EQ(L24.mH, 0);
    EXPECT_EQ(L24.mA, 0);
    EXPECT_EQ(L24.mCD, 0);

    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mPertinentType, JTSTreeNode::Full);
    EXPECT_EQ(L25.mW, 1);
    EXPECT_EQ(L25.mH, 0);
    EXPECT_EQ(L25.mA, 0);
    EXPECT_EQ(L25.mCD, 0);

    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mPertinentType, JTSTreeNode::Partial);
    EXPECT_EQ(P04.mW, 4);
    EXPECT_EQ(P04.mH, 1);
    EXPECT_EQ(P04.mA, 1);
    EXPECT_EQ(P04.mCD, 0);

}

