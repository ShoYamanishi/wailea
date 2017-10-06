
/**  @brief tests JTSPlanarizer::bubbleUp()
 */
TEST_F(JTSPlanarizerTests, Test5) {

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

    EXPECT_EQ(root, L01.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);

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
    //*L01*L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P01.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);

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
    //*L01*L02*L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P01.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);

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
    //*L01 L02 L03 *L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L04.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, Q03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03 *L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L16.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, Q03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);

    //                         P04
    //                       /  |  \
    //                    *L24 Q05*L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L25.backIt());
    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P04.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


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
    //*L01*L02*L03 *L04*L05*L06 *L07*L08*L09 *L10*L11*L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L10.backIt());
    pertinentLeaves.push_back(L11.backIt());
    pertinentLeaves.push_back(L12.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L10.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L11.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    //*L13 P01*L14 *L15 Q01*L16 *L17 P02*L18 *L19 Q02*L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    //*L01*L02*L03 *L04*L05*L06 *L07*L08*L09 *L10*L11*L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L01.backIt());
    pertinentLeaves.push_back(L02.backIt());
    pertinentLeaves.push_back(L03.backIt());
    pertinentLeaves.push_back(L04.backIt());
    pertinentLeaves.push_back(L05.backIt());
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L07.backIt());
    pertinentLeaves.push_back(L08.backIt());
    pertinentLeaves.push_back(L09.backIt());
    pertinentLeaves.push_back(L10.backIt());
    pertinentLeaves.push_back(L11.backIt());
    pertinentLeaves.push_back(L12.backIt());
    pertinentLeaves.push_back(L13.backIt());
    pertinentLeaves.push_back(L14.backIt());
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L18.backIt());
    pertinentLeaves.push_back(L19.backIt());
    pertinentLeaves.push_back(L20.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P03.backIt());
    EXPECT_EQ(L01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L07.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L08.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L09.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L10.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L11.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L12.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L18.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L19.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                     L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14 *L15 Q01 L16 *L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L15.backIt());
    pertinentLeaves.push_back(L17.backIt());
    pertinentLeaves.push_back(L21.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P03.backIt());
    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05 L25
    //                       /  |  \
    //                     *L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01*L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05*L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L06.backIt());
    pertinentLeaves.push_back(L16.backIt());
    pertinentLeaves.push_back(L21.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L24.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P04.backIt());

    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05 L25
    //                       /  |  \
    //                      L22 | L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________  L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01 L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L24.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, L24.backIt());
    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L22.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05 L25
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
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P04.backIt());
    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);


    //                         P04
    //                       /  |  \
    //                    *L24 Q05 L25
    //                       /  |  \
    //                     *L22 |*L23
    //             ____________P03__________
    //            /             |           \
    //   ________Q03_________ *L21 ________Q04_________
    //  /   /   /    \   \   \    /   /   /    \   \   \
    // L13 P01*L14  L15 Q01 L16  L17 P02 L18  L19 Q02 L20
    //   /  |  \     /   |   \    /   |   \    /   |   \
    // L01 L02 L03  L04 L05 L06  L07 L08 L09  L10 L11 L12
    pertinentLeaves.clear();
    initializeForOneIteration(pqTree);
    pertinentLeaves.push_back(L24.backIt());
    pertinentLeaves.push_back(L22.backIt());
    pertinentLeaves.push_back(L23.backIt());
    pertinentLeaves.push_back(L21.backIt());
    pertinentLeaves.push_back(L14.backIt());

    root = bubbleUp(planarizer, pqTree, pertinentLeaves);
    EXPECT_EQ(root, P04.backIt());
    EXPECT_LT(L01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L02.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L05.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L06.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L07.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L08.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L09.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L10.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L11.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L12.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L13.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L14.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L15.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L16.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L17.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L18.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L19.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L20.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L21.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L22.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L23.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(L24.mGeneration, pqTree.mGeneration);
    EXPECT_LT(L25.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(P02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P03.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(P04.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q01.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q02.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q03.mGeneration, pqTree.mGeneration);
    EXPECT_LT(Q04.mGeneration, pqTree.mGeneration);
    EXPECT_EQ(Q05.mGeneration, pqTree.mGeneration);

}


