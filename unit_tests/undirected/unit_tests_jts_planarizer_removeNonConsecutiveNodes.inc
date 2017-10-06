

/**  @brief tests JTSPlanarizer::removeNonconsecutiveNodes() #1
 */
TEST_F(JTSPlanarizerTests, Test11) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;
    JTSGraph g_2;

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
    /*auto &E01 = */createE(g_2, L01);
    auto& L02 = createL(pqTree);
    /*auto &E02 = */createE(g_2, L02);
    auto& L03 = createL(pqTree);
    /*auto &E03 = */createE(g_2, L03);
    auto& L04 = createL(pqTree);
    /*auto &E04 = */createE(g_2, L04);
    auto& L05 = createL(pqTree);
    /*auto &E05 = */createE(g_2, L05);
    auto& L06 = createL(pqTree);
    /*auto &E06 = */createE(g_2, L06);
    auto& L07 = createL(pqTree);
    /*auto &E07 = */createE(g_2, L07);
    auto& L08 = createL(pqTree);
    /*auto &E08 = */createE(g_2, L08);
    auto& L09 = createL(pqTree);
    /*auto &E09 = */createE(g_2, L09);
    auto& L10 = createL(pqTree);
    /*auto &E10 = */createE(g_2, L10);
    auto& L11 = createL(pqTree);
    /*auto &E11 = */createE(g_2, L11);
    auto& L12 = createL(pqTree);
    /*auto &E12 = */createE(g_2, L12);
    auto& L13 = createL(pqTree);
    /*auto &E13 = */createE(g_2, L13);

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);
    /*auto &E14 = */createE(g_2, L14);
    auto& L15 = createL(pqTree);
    /*auto &E15 = */createE(g_2, L15);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    /*auto &E16 = */createE(g_2, L16);
    auto& L17 = createL(pqTree);
    /*auto &E17 = */createE(g_2, L17);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    /*auto &E18 = */createE(g_2, L18);
    auto& L19 = createL(pqTree);
    /*auto &E19 = */createE(g_2, L19);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);
    /*auto &E20 = */createE(g_2, L20);
    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);
    /*auto &E21 = */createE(g_2, L21);
    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);
    /*auto &E22 = */createE(g_2, L22);
    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    /*auto &E23 = */createE(g_2, L23);
    auto& L24 = createL(pqTree);
    /*auto &E24 = */createE(g_2, L24);
    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);
    /*auto &E25 = */createE(g_2, L25);
    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    /*auto& P04 = */createP(pqTree, P04children);

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
    removeNonconsecutiveNodes(planarizer, pqTree, pertinentLeaves, root);
    EXPECT_EQ(pqTree.numNodes(), 34);




}



/**  @brief tests JTSPlanarizer::removeNonconsecutiveNodes() #2
 */
TEST_F(JTSPlanarizerTests, Test12) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;
    JTSGraph g_2;

    auto& L01 = createL(pqTree);
    /*auto &E01 = */createE(g_2, L01);
    auto& L02 = createL(pqTree);
    /*auto &E02 = */createE(g_2, L02);
    auto& L03 = createL(pqTree);
    /*auto &E03 = */createE(g_2, L03);
    auto& L04 = createL(pqTree);
    /*auto &E04 = */createE(g_2, L04);
    auto& L05 = createL(pqTree);
    /*auto &E05 = */createE(g_2, L05);
    auto& L06 = createL(pqTree);
    /*auto &E06 = */createE(g_2, L06);
    auto& L07 = createL(pqTree);
    /*auto &E07 = */createE(g_2, L07);
    auto& L08 = createL(pqTree);
    /*auto &E08 = */createE(g_2, L08);
    auto& L09 = createL(pqTree);
    /*auto &E09 = */createE(g_2, L09);
    auto& L10 = createL(pqTree);
    /*auto &E10 = */createE(g_2, L10);
    auto& L11 = createL(pqTree);
    /*auto &E11 = */createE(g_2, L11);
    auto& L12 = createL(pqTree);
    /*auto &E12 = */createE(g_2, L12);
    auto& L13 = createL(pqTree);
    /*auto &E13 = */createE(g_2, L13);
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);
    /*auto &E14 = */createE(g_2, L14);
    auto& L15 = createL(pqTree);
    /*auto &E15 = */createE(g_2, L15);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    /*auto &E16 = */createE(g_2, L16);
    auto& L17 = createL(pqTree);
    /*auto &E17 = */createE(g_2, L17);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    /*auto &E18 = */createE(g_2, L18);
    auto& L19 = createL(pqTree);
    /*auto &E19 = */createE(g_2, L19);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);
    /*auto &E20 = */createE(g_2, L20);
    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);
    /*auto &E21 = */createE(g_2, L21);
    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);
    /*auto &E22 = */createE(g_2, L22);
    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    /*auto &E23 = */createE(g_2, L23);
    auto& L24 = createL(pqTree);
    /*auto &E24 = */createE(g_2, L24);
    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);
    /*auto &E25 = */createE(g_2, L25);
    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    /*auto& P04 = */createP(pqTree, P04children);

    vector<node_list_it_t> pertinentLeaves;


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
    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    removeNonconsecutiveNodes(planarizer, pqTree, pertinentLeaves, root);
    EXPECT_EQ(pqTree.numNodes(), 34);

}


/**  @brief tests JTSPlanarizer::removeNonconsecutiveNodes() #3
 */
TEST_F(JTSPlanarizerTests, Test13) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;
    JTSGraph g_2;

    auto& L01 = createL(pqTree);
    /*auto &E01 = */createE(g_2, L01);
    auto& L02 = createL(pqTree);
    /*auto &E02 = */createE(g_2, L02);
    auto& L03 = createL(pqTree);
    /*auto &E03 = */createE(g_2, L03);
    auto& L04 = createL(pqTree);
    /*auto &E04 = */createE(g_2, L04);
    auto& L05 = createL(pqTree);
    /*auto &E05 = */createE(g_2, L05);
    auto& L06 = createL(pqTree);
    /*auto &E06 = */createE(g_2, L06);
    auto& L07 = createL(pqTree);
    /*auto &E07 = */createE(g_2, L07);
    auto& L08 = createL(pqTree);
    /*auto &E08 = */createE(g_2, L08);
    auto& L09 = createL(pqTree);
    /*auto &E09 = */createE(g_2, L09);
    auto& L10 = createL(pqTree);
    /*auto &E10 = */createE(g_2, L10);
    auto& L11 = createL(pqTree);
    /*auto &E11 = */createE(g_2, L11);
    auto& L12 = createL(pqTree);
    /*auto &E12 = */createE(g_2, L12);
    auto& L13 = createL(pqTree);
    /*auto &E13 = */createE(g_2, L13);
    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);
    /*auto &E14 = */createE(g_2, L14);
    auto& L15 = createL(pqTree);
    /*auto &E15 = */createE(g_2, L15);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    /*auto &E16 = */createE(g_2, L16);
    auto& L17 = createL(pqTree);
    /*auto &E17 = */createE(g_2, L17);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    /*auto &E18 = */createE(g_2, L18);
    auto& L19 = createL(pqTree);
    /*auto &E19 = */createE(g_2, L19);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);
    /*auto &E20 = */createE(g_2, L20);
    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);
    /*auto &E21 = */createE(g_2, L21);
    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);
    /*auto &E22 = */createE(g_2, L22);
    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    /*auto &E23 = */createE(g_2, L23);
    auto& L24 = createL(pqTree);
    /*auto &E24 = */createE(g_2, L24);
    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);
    /*auto &E25 = */createE(g_2, L25);
    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    /*auto& P04 = */createP(pqTree, P04children);

    vector<node_list_it_t> pertinentLeaves;


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

    auto &E16 = createE(g_2, L16);

    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    removeNonconsecutiveNodes(planarizer, pqTree, pertinentLeaves, root);
    EXPECT_EQ(pqTree.numNodes(), 33);
    EXPECT_EQ(E16.mRemoved, true);
    EXPECT_EQ(Q03.mChildren.size(), 5);
    EXPECT_EQ(Q03.mPertinentChildrenCount, 4);

}


/**  @brief tests JTSPlanarizer::removeNonconsecutiveNodes() #4
 */
TEST_F(JTSPlanarizerTests, Test14) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;
    JTSGraph g_2;

    auto& L01 = createL(pqTree);
    /*auto &E01 = */createE(g_2, L01);
    auto& L02 = createL(pqTree);
    /*auto &E02 = */createE(g_2, L02);
    auto& L03 = createL(pqTree);
    /*auto &E03 = */createE(g_2, L03);
    auto& L04 = createL(pqTree);
    /*auto &E04 = */createE(g_2, L04);
    auto& L05 = createL(pqTree);
    /*auto &E05 = */createE(g_2, L05);
    auto& L06 = createL(pqTree);
    /*auto &E06 = */createE(g_2, L06);
    auto& L07 = createL(pqTree);
    /*auto &E07 = */createE(g_2, L07);
    auto& L08 = createL(pqTree);
    /*auto &E08 = */createE(g_2, L08);
    auto& L09 = createL(pqTree);
    /*auto &E09 = */createE(g_2, L09);
    auto& L10 = createL(pqTree);
    /*auto &E10 = */createE(g_2, L10);
    auto& L11 = createL(pqTree);
    /*auto &E11 = */createE(g_2, L11);
    auto& L12 = createL(pqTree);
    /*auto &E12 = */createE(g_2, L12);
    auto& L13 = createL(pqTree);
    /*auto &E13 = */createE(g_2, L13);


    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);
    /*auto &E14 = */createE(g_2, L14);
    auto& L15 = createL(pqTree);
    /*auto &E15 = */createE(g_2, L15);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    auto &E16 = createE(g_2, L16);
    auto& L17 = createL(pqTree);
    auto &E17 = createE(g_2, L17);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    /*auto &E18 = */createE(g_2, L18);
    auto& L19 = createL(pqTree);
    /*auto &E19 = */createE(g_2, L19);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);
    /*auto &E20 = */createE(g_2, L20);
    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);
    auto &E21 = createE(g_2, L21);
    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);
    /*auto &E22 = */createE(g_2, L22);
    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    /*auto &E23 = */createE(g_2, L23);
    auto& L24 = createL(pqTree);
    /*auto &E24 = */createE(g_2, L24);
    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);
    /*auto &E25 = */createE(g_2, L25);
    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    /*auto& P04 = */createP(pqTree, P04children);

    vector<node_list_it_t> pertinentLeaves;


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
    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    removeNonconsecutiveNodes(planarizer, pqTree, pertinentLeaves, root);

    EXPECT_EQ(pqTree.numNodes(), 31);

    EXPECT_EQ(E16.mRemoved, true);
    EXPECT_EQ(E17.mRemoved, true);
    EXPECT_EQ(E21.mRemoved, true);
    EXPECT_EQ(Q03.mChildren.size(), 5);
    EXPECT_EQ(Q03.mPertinentChildrenCount, 4);
    EXPECT_EQ(Q04.mChildren.size(), 5);
    EXPECT_EQ(Q04.mPertinentChildrenCount, 0);
    EXPECT_EQ(P03.mChildren.size(), 2);
    EXPECT_EQ(P03.mPertinentChildrenCount, 1);

}


/**  @brief tests JTSPlanarizer::removeNonconsecutiveNodes() #5
 */
TEST_F(JTSPlanarizerTests, Test15) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;
    JTSGraph g_2;

    auto& L01 = createL(pqTree);
    auto &E01 = createE(g_2, L01);
    auto& L02 = createL(pqTree);
    auto &E02 = createE(g_2, L02);
    auto& L03 = createL(pqTree);
    auto &E03 = createE(g_2, L03);
    auto& L04 = createL(pqTree);
    /*auto &E04 = */createE(g_2, L04);
    auto& L05 = createL(pqTree);
    /*auto &E05 = */createE(g_2, L05);
    auto& L06 = createL(pqTree);
    /*auto &E06 = */createE(g_2, L06);
    auto& L07 = createL(pqTree);
    /*auto &E07 = */createE(g_2, L07);
    auto& L08 = createL(pqTree);
    /*auto &E08 = */createE(g_2, L08);
    auto& L09 = createL(pqTree);
    /*auto &E09 = */createE(g_2, L09);
    auto& L10 = createL(pqTree);
    /*auto &E10 = */createE(g_2, L10);
    auto& L11 = createL(pqTree);
    /*auto &E11 = */createE(g_2, L11);
    auto& L12 = createL(pqTree);
    /*auto &E12 = */createE(g_2, L12);
    auto& L13 = createL(pqTree);
    /*auto &E13 = */createE(g_2, L13);

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);
    auto &E14 = createE(g_2, L14);
    auto& L15 = createL(pqTree);
    auto &E15 = createE(g_2, L15);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    /*auto &E16 = */createE(g_2, L16);
    auto& L17 = createL(pqTree);
    /*auto &E17 = */createE(g_2, L17);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    /*auto &E18 = */createE(g_2, L18);
    auto& L19 = createL(pqTree);
    /*auto &E19 = */createE(g_2, L19);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);
    /*auto &E20 = */createE(g_2, L20);
    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);
    /*auto &E21 = */createE(g_2, L21);
    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);
    auto &E22 = createE(g_2, L22);
    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    /*auto &E23 = */createE(g_2, L23);
    auto& L24 = createL(pqTree);
    /*auto &E24 = */createE(g_2, L24);
    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);
    /*auto &E25 = */createE(g_2, L25);
    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    /*auto& P04 = */createP(pqTree, P04children);

    vector<node_list_it_t> pertinentLeaves;


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
    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    removeNonconsecutiveNodes(planarizer, pqTree, pertinentLeaves, root);

    EXPECT_EQ(pqTree.numNodes(), 28);
    EXPECT_EQ(E01.mRemoved, true);
    EXPECT_EQ(E03.mRemoved, true);
    EXPECT_EQ(E14.mRemoved, true);
    EXPECT_EQ(E15.mRemoved, true);
    EXPECT_EQ(E22.mRemoved, true);

    EXPECT_EQ(P01.mNodeType, JTSTreeNode::LType);
    EXPECT_EQ(P01.mChildren.size(), 0);
    EXPECT_EQ(P01.mPertinentChildrenCount, 0);
    EXPECT_EQ(P01.mGraphEdge, E02.backIt());
    EXPECT_EQ(Q03.mChildren.size(), 4);
    EXPECT_EQ(Q03.mPertinentChildrenCount, 2);
    EXPECT_EQ(P03.mChildren.size(), 3);
    EXPECT_EQ(P03.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q05.mChildren.size(), 2);
    EXPECT_EQ(Q05.mPertinentChildrenCount, 1);
    EXPECT_EQ(root, P03.backIt());

}



/**  @brief tests JTSPlanarizer::removeNonconsecutiveNodes() #6
 */
TEST_F(JTSPlanarizerTests, Test16) {

    JTSTree& pqTree = getPQTree();
    JTSPlanarizer planarizer;
    JTSGraph g_2;

    auto& L01 = createL(pqTree);
    auto &E01 = createE(g_2, L01);
    auto& L02 = createL(pqTree);
    auto &E02 = createE(g_2, L02);
    auto& L03 = createL(pqTree);
    auto &E03 = createE(g_2, L03);
    auto& L04 = createL(pqTree);
    /*auto &E04 = */createE(g_2, L04);
    auto& L05 = createL(pqTree);
    /*auto &E05 = */createE(g_2, L05);
    auto& L06 = createL(pqTree);
    /*auto &E06 = */createE(g_2, L06);
    auto& L07 = createL(pqTree);
    /*auto &E07 = */createE(g_2, L07);
    auto& L08 = createL(pqTree);
    /*auto &E08 = */createE(g_2, L08);
    auto& L09 = createL(pqTree);
    /*auto &E09 = */createE(g_2, L09);
    auto& L10 = createL(pqTree);
    /*auto &E10 = */createE(g_2, L10);
    auto& L11 = createL(pqTree);
    /*auto &E11 = */createE(g_2, L11);
    auto& L12 = createL(pqTree);
    /*auto &E12 = */createE(g_2, L12);
    auto& L13 = createL(pqTree);
    /*auto &E13 = */createE(g_2, L13);

    list<node_list_it_t> P01children;
    P01children.push_back(L01.backIt());
    P01children.push_back(L02.backIt());
    P01children.push_back(L03.backIt());
    auto& P01 = createP(pqTree, P01children);
    auto& L14 = createL(pqTree);
    auto &E14 = createE(g_2, L14);
    auto& L15 = createL(pqTree);
    auto &E15 = createE(g_2, L15);
    list<node_list_it_t> Q01children;
    Q01children.push_back(L04.backIt());
    Q01children.push_back(L05.backIt());
    Q01children.push_back(L06.backIt());
    auto& Q01 = createQ(pqTree, Q01children);
    auto& L16 = createL(pqTree);
    /*auto &E16 = */createE(g_2, L16);
    auto& L17 = createL(pqTree);
    /*auto &E17 = */createE(g_2, L17);
    list<node_list_it_t> P02children;
    P02children.push_back(L07.backIt());
    P02children.push_back(L08.backIt());
    P02children.push_back(L09.backIt());
    auto& P02 = createP(pqTree, P02children);
    auto& L18 = createL(pqTree);
    /*auto &E18 = */createE(g_2, L18);
    auto& L19 = createL(pqTree);
    /*auto &E19 = */createE(g_2, L19);
    list<node_list_it_t> Q02children;
    Q02children.push_back(L10.backIt());
    Q02children.push_back(L11.backIt());
    Q02children.push_back(L12.backIt());
    auto& Q02 = createQ(pqTree, Q02children);
    auto& L20 = createL(pqTree);
    /*auto &E20 = */createE(g_2, L20);
    list<node_list_it_t> Q03children;
    Q03children.push_back(L13.backIt());
    Q03children.push_back(P01.backIt());
    Q03children.push_back(L14.backIt());
    Q03children.push_back(L15.backIt());
    Q03children.push_back(Q01.backIt());
    Q03children.push_back(L16.backIt());
    auto& Q03 = createQ(pqTree, Q03children);
    auto& L21 = createL(pqTree);
    /*auto &E21 = */createE(g_2, L21);
    list<node_list_it_t> Q04children;
    Q04children.push_back(L17.backIt());
    Q04children.push_back(P02.backIt());
    Q04children.push_back(L18.backIt());
    Q04children.push_back(L19.backIt());
    Q04children.push_back(Q02.backIt());
    Q04children.push_back(L20.backIt());
    auto& Q04 = createQ(pqTree, Q04children);
    auto& L22 = createL(pqTree);
    /*auto &E22 = */createE(g_2, L22);
    list<node_list_it_t> P03children;
    P03children.push_back(Q03.backIt());
    P03children.push_back(L21.backIt());
    P03children.push_back(Q04.backIt());
    auto& P03 = createP(pqTree, P03children);
    auto& L23 = createL(pqTree);
    /*auto &E23 = */createE(g_2, L23);
    auto& L24 = createL(pqTree);
    auto &E24 = createE(g_2, L24);
    list<node_list_it_t> Q05children;
    Q05children.push_back(L22.backIt());
    Q05children.push_back(P03.backIt());
    Q05children.push_back(L23.backIt());
    auto& Q05 = createQ(pqTree, Q05children);
    auto& L25 = createL(pqTree);
    /*auto &E25 = */createE(g_2, L25);
    list<node_list_it_t> P04children;
    P04children.push_back(L24.backIt());
    P04children.push_back(Q05.backIt());
    P04children.push_back(L25.backIt());
    auto& P04 = createP(pqTree, P04children);

    vector<node_list_it_t> pertinentLeaves;

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
    auto root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    findWHA(planarizer, pqTree, pertinentLeaves, root);
    determineWHABCNodeTypes(planarizer, pqTree, root);
    removeNonconsecutiveNodes(planarizer, pqTree, pertinentLeaves, root);

    EXPECT_EQ(pqTree.numNodes(), 28);
    EXPECT_EQ(E01.mRemoved, true);
    EXPECT_EQ(E03.mRemoved, true);
    EXPECT_EQ(E14.mRemoved, true);
    EXPECT_EQ(E15.mRemoved, true);
    EXPECT_EQ(E24.mRemoved, true);

    EXPECT_EQ(P01.mNodeType, JTSTreeNode::LType);
    EXPECT_EQ(P01.mChildren.size(), 0);
    EXPECT_EQ(P01.mPertinentChildrenCount, 0);
    EXPECT_EQ(P01.mGraphEdge, E02.backIt());
    EXPECT_EQ(Q03.mChildren.size(), 4);
    EXPECT_EQ(Q03.mPertinentChildrenCount, 2);
    EXPECT_EQ(P03.mChildren.size(), 3);
    EXPECT_EQ(P03.mPertinentChildrenCount, 3);
    EXPECT_EQ(Q05.mChildren.size(), 3);
    EXPECT_EQ(Q05.mPertinentChildrenCount, 1);
    EXPECT_EQ(P04.mChildren.size(), 2);
    EXPECT_EQ(P04.mPertinentChildrenCount, 1);
    EXPECT_EQ(root, P03.backIt());

}


/**  @brief tests JTSTree::findNewPertinentLeaves()
 */
TEST_F(JTSPlanarizerTests, Test17) {

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
    nIt++;
    nIt++;
    nIt++;
    nIt++;
    nIt++;
    nIt++;
    nIt++;
    nIt++;
    nIt++;
    auto& nc_10 = dynamic_cast<JTSGraphNode&>(*(*nIt));

    auto ePair = g_2.edges();
    auto eIt   = ePair.first;

    eIt++;
    eIt++;
    eIt++;
    eIt++;
    auto& ec_01_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    eIt++;
    auto& ec_07_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_08_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));
    auto& ec_09_10 = dynamic_cast<JTSGraphEdge&>(*(*eIt++));

    ec_01_10.mRemoved = true;
    ec_08_10.mRemoved = true;
    ec_07_10.mRemoved = false;
    ec_09_10.mRemoved = false;


    vector<node_list_it_t> leaves =  fromGraphEdgesToPQLeaves(
                                    planarizer, pqTree, nc_10.mIncomingEdges);

    EXPECT_EQ(leaves.size(),2);

}

