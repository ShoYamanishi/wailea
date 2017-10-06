
/**  @brief tests BLPlanarityTester::isPlanar() in steps for plana4 graph.
 */
TEST_F(BLPlanarityTesterTests, Test108) {

    BLTree  pqTree;
    BLGraph g_1;
    BLPlanarityTester tester;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03,n_05);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());

    BLGraph                graphCopy;
    vector<node_list_it_t> stOrderCopy;

    copyInputGraph(tester, g_1, stOrder, graphCopy, stOrderCopy);

    node_list_it_t attachmentNode = pqTree.makePAttachment();

    size_t index = 1;
    auto nItIt = stOrderCopy.begin();

    // Iteration 1
    auto nit = *nItIt;
    auto& n1 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n1.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 3, 1, 0, 0), true);

    // Iteration 2

    nit = *nItIt;
    auto& n2 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
                    fromGraphEdgesToPQLeaves(tester, n2.mIncomingEdges);

    node_list_it_t pertinentRoot;
    node_list_it_t virtualRoot =pqTree.nil();

    bool result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
         
    EXPECT_EQ(result, true);

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);

    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n2.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 5, 2, 0, 0), true);

    // Iteration 3

    nit = *nItIt;
    auto& n3 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n3.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
          
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(
                        tester, pqTree, pertinentLeaves, pertinentRoot);

    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n3.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 5, 3, 1, 0), true);

    // Iteration 4

    nit = *nItIt;
    auto& n4 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves =
                    fromGraphEdgesToPQLeaves(tester, n4.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);

    EXPECT_EQ(virtualRoot, pqTree.nil());


    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);

    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n4.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 4, 1, 1, 0), true);

    // Iteration 5

    nit = *nItIt;
    auto& n5 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n5.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    EXPECT_EQ(checkTreeNodes(pqTree, 0, 1, 0, 0), true);

}


/**  @brief tests BLPlanarityTester::isPlanar() in steps for planar graph.
 */
TEST_F(BLPlanarityTesterTests, Test109) {

    BLTree  pqTree;
    BLGraph g_1;
    BLPlanarityTester tester;

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
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());
    stOrder.push_back(n_09.backIt());
    stOrder.push_back(n_10.backIt());

    BLGraph                graphCopy;
    vector<node_list_it_t> stOrderCopy;

    copyInputGraph(tester, g_1, stOrder, graphCopy, stOrderCopy);

    node_list_it_t attachmentNode = pqTree.makePAttachment();

    size_t index = 0;
    auto nItIt = stOrderCopy.begin();

    // Iteration 1
    auto nit = *nItIt;
    auto& n1 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n1.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 5, 1, 0, 0), true);

    // Iteration 2
    nit = *nItIt;
    auto& n2 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
                    fromGraphEdgesToPQLeaves(tester, n2.mIncomingEdges);

    node_list_it_t pertinentRoot;
    node_list_it_t virtualRoot =pqTree.nil();

    bool result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
       
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n2.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 6, 2, 0, 0), true);

    // Iteration 3
    nit = *nItIt;
    auto& n3 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n3.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n3.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 6, 2, 0, 0), true);

    // Iteration 4
    nit = *nItIt;
    auto& n4 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n4.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
         
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n4.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 6, 2, 1, 0), true);

    // Iteration 5
    nit = *nItIt;
    auto& n5 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n5.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n5.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 8, 3, 1, 0), true);


    // Iteration 6
    nit = *nItIt;
    auto& n6 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n6.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n6.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 8, 3, 1, 0), true);

    // Iteration 7
    nit = *nItIt;
    auto& n7 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n7.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);

    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n7.mOutgoingEdges);

    EXPECT_EQ(checkTreeNodes(pqTree, 8, 3, 1, 0), true);

    index++;
    nItIt++;

    // Iteration 8
    nit = *nItIt;
    auto& n8 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n8.mIncomingEdges);
                   
    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n8.mOutgoingEdges);

    EXPECT_EQ(checkTreeNodes(pqTree, 6, 2, 1, 0), true);

    index++;
    nItIt++;

    // Iteration 9
    nit = *nItIt;
    auto& n9 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n9.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
         
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n9.mOutgoingEdges);

    EXPECT_EQ(checkTreeNodes(pqTree, 4, 1, 1, 0), true);

    index++;
    nItIt++;


    // Iteration 10
    nit = *nItIt;
    auto& n10 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n10.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
          
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    EXPECT_EQ(index, stOrderCopy.size() - 1);
    EXPECT_EQ(checkTreeNodes(pqTree, 0, 1, 0, 0), true);

}


/**  @brief tests BLPlanarityTester::isPlanar() in steps for nonplanar graph.
 */
TEST_F(BLPlanarityTesterTests, Test110) {

    BLTree  pqTree;
    BLGraph g_1;
    BLPlanarityTester tester;

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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());
    stOrder.push_back(n_09.backIt());
    stOrder.push_back(n_10.backIt());

    BLGraph                graphCopy;
    vector<node_list_it_t> stOrderCopy;

    copyInputGraph(tester, g_1, stOrder, graphCopy, stOrderCopy);

    node_list_it_t attachmentNode = pqTree.makePAttachment();

    size_t index = 0;
    auto nItIt = stOrderCopy.begin();

    // Iteration 1
    auto nit = *nItIt;
    auto& n1 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n1.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 5, 1, 0, 0), true);

    // Iteration 2
    nit = *nItIt;
    auto& n2 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    vector<node_list_it_t> pertinentLeaves =
                    fromGraphEdgesToPQLeaves(tester, n2.mIncomingEdges);

    node_list_it_t pertinentRoot;
    node_list_it_t virtualRoot =pqTree.nil();

    bool result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n2.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 8, 2, 0, 0), true);

    // Iteration 3
    nit = *nItIt;
    auto& n3 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n3.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n3.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 9, 3, 0, 0), true);

    // Iteration 4
    nit = *nItIt;
    auto& n4 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n4.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
        
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n4.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 9, 3, 1, 0), true);

    // Iteration 5
    nit = *nItIt;
    auto& n5 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n5.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
         
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, true);

    attachmentNode = removePertinentTree(tester, pqTree, pertinentRoot);

    pqTree.fanOutLeavesFromAttachment(attachmentNode, n5.mOutgoingEdges);

    index++;
    nItIt++;

    EXPECT_EQ(checkTreeNodes(pqTree, 11, 4, 1, 0), true);

    // Iteration 6
    nit = *nItIt;
    auto& n6 = dynamic_cast<BLGraphNode&>(*(*nit));

    pqTree.initializeForOneIteration();

    pertinentLeaves = fromGraphEdgesToPQLeaves(tester, n6.mIncomingEdges);

    virtualRoot =pqTree.nil();

    result = bubbleUp(tester, pqTree, pertinentLeaves, virtualRoot);
         
    EXPECT_EQ(result, true);
    EXPECT_EQ(virtualRoot, pqTree.nil());

    result = applyTemplates(tester, pqTree, pertinentLeaves, pertinentRoot);
                        
    EXPECT_EQ(result, false);

}

/**  @brief tests BLPlanarityTester::isPlanar()  for nonplanar graph.
 */
TEST_F(BLPlanarityTesterTests, Test111) {

    BLGraph g_1;
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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());
    stOrder.push_back(n_09.backIt());
    stOrder.push_back(n_10.backIt());

    BLPlanarityTester tester;
    bool result = tester.isPlanar(g_1, stOrder);
    EXPECT_EQ(result, false);

}


/**  @brief tests BLPlanarityTester::isPlanar()  for K8
 */
TEST_F(BLPlanarityTesterTests, Test112) {

    BLGraph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());
    auto& n_05 = g_1.addNode(make_unique<Node>());
    auto& n_06 = g_1.addNode(make_unique<Node>());
    auto& n_07 = g_1.addNode(make_unique<Node>());
    auto& n_08 = g_1.addNode(make_unique<Node>());

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

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());
    stOrder.push_back(n_05.backIt());
    stOrder.push_back(n_06.backIt());
    stOrder.push_back(n_07.backIt());
    stOrder.push_back(n_08.backIt());

    BLPlanarityTester tester;
    bool result = tester.isPlanar(g_1, stOrder);
    EXPECT_EQ(result, false);

}


/**  @brief tests BLPlanarityTester::isPlanar()  for K4
 */
TEST_F(BLPlanarityTesterTests, Test113) {

    BLGraph g_1;

    auto& n_01 = g_1.addNode(make_unique<Node>());
    auto& n_02 = g_1.addNode(make_unique<Node>());
    auto& n_03 = g_1.addNode(make_unique<Node>());
    auto& n_04 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);

    vector<node_list_it_t> stOrder;
    stOrder.push_back(n_01.backIt());
    stOrder.push_back(n_02.backIt());
    stOrder.push_back(n_03.backIt());
    stOrder.push_back(n_04.backIt());

    BLPlanarityTester tester;
    bool result = tester.isPlanar(g_1, stOrder);
    EXPECT_EQ(result, true);

}


/**  @brief tests BLPlanarityTester::isPlanar()  for random planar graph
 *          with all the combinations of {s,t}.
 */
TEST_F(BLPlanarityTesterTests, Test114) {

    BLGraph g_1;


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
            BLPlanarityTester tester;
            size_t index = 0;
//            bool planar = tester.isPlanarOldAlg(g_1, st_list, index);
            bool planar = tester.isPlanar(g_1, st_list);
            if (!planar) {
                cerr << "Failed at iteration: " << index << "\n";
            }
//            else {
//                cerr << "Succeeded.\n";
//            }
            EXPECT_EQ(planar, true);
        }
    }
}


/**  @brief tests BLPlanarityTester::isPlanar()  for dodecahedron
 *          with all the combinations of {s,t}.
 */
TEST_F(BLPlanarityTesterTests, Test115) {

    BLGraph g_1;

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
            BLPlanarityTester tester;
            size_t index = 0;
//            bool planar = tester.isPlanarOldAlg(g_1, st_list, index);
            bool planar = tester.isPlanar(g_1, st_list);

            if (!planar) {
                cerr << "Failed at iteration: " << index << "\n";
            }
//            else {
//                cerr << "Succeeded.\n";
//            }
            EXPECT_EQ(planar, true);

        }
    }
}


/**  @brief tests BLPlanarityTester::isPlanar()  for icosahedron
 *          with all the combinations of {s,t}.
 */
TEST_F(BLPlanarityTesterTests, TestIsPlanarEx01) {

    BLGraph g_1;

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

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_04_03 = */g_1.addEdge(make_unique<Edge>(),n_04,n_03);
    /*auto& e_03_06 = */g_1.addEdge(make_unique<Edge>(),n_03,n_06);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03,n_05);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_02_06 = */g_1.addEdge(make_unique<Edge>(),n_02,n_06);
    /*auto& e_04_07 = */g_1.addEdge(make_unique<Edge>(),n_04,n_07);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(),n_05,n_07);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(),n_06,n_08);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    /*auto& e_07_09 = */g_1.addEdge(make_unique<Edge>(),n_07,n_09);
    /*auto& e_08_10 = */g_1.addEdge(make_unique<Edge>(),n_08,n_10);
    /*auto& e_07_11 = */g_1.addEdge(make_unique<Edge>(),n_07,n_11);
    /*auto& e_08_11 = */g_1.addEdge(make_unique<Edge>(),n_08,n_11);
    /*auto& e_09_11 = */g_1.addEdge(make_unique<Edge>(),n_09,n_11);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    /*auto& e_04_09 = */g_1.addEdge(make_unique<Edge>(),n_04,n_09);
    /*auto& e_06_10 = */g_1.addEdge(make_unique<Edge>(),n_06,n_10);
    /*auto& e_01_09 = */g_1.addEdge(make_unique<Edge>(),n_01,n_09);
    /*auto& e_02_10 = */g_1.addEdge(make_unique<Edge>(),n_02,n_10);
    /*auto& e_01_12 = */g_1.addEdge(make_unique<Edge>(),n_01,n_12);
    /*auto& e_02_12 = */g_1.addEdge(make_unique<Edge>(),n_02,n_12);
    /*auto& e_09_12 = */g_1.addEdge(make_unique<Edge>(),n_09,n_12);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    /*auto& e_11_12 = */g_1.addEdge(make_unique<Edge>(),n_11,n_12);

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
            BLPlanarityTester tester;
            size_t index = 0;
//            bool planar = tester.isPlanarOldAlg(g_1, st_list, index);
            bool planar = tester.isPlanar(g_1, st_list);

            if (!planar) {
                cerr << "Failed at iteration: " << index << "\n";
            }
//            else {
//                cerr << "Succeeded.\n";
//            }
            EXPECT_EQ(planar, true);

        }
    }
}


/**  @brief tests BLPlanarityTester::isPlanar()  for octahedron
 *          with all the combinations of {s,t}.
 */
TEST_F(BLPlanarityTesterTests, TestIsPlanarEx02) {

    BLGraph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));
    auto& n_05 = g_1.addNode(make_unique<NumNode>(5));
    auto& n_06 = g_1.addNode(make_unique<NumNode>(6));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_05 = */g_1.addEdge(make_unique<Edge>(),n_01,n_05);
    /*auto& e_01_06 = */g_1.addEdge(make_unique<Edge>(),n_01,n_06);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_02_05 = */g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    /*auto& e_03_06 = */g_1.addEdge(make_unique<Edge>(),n_03,n_06);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_04_06 = */g_1.addEdge(make_unique<Edge>(),n_04,n_06);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);

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
            BLPlanarityTester tester;
            size_t index = 0;
//            bool planar = tester.isPlanarOldAlg(g_1, st_list, index);
            bool planar = tester.isPlanar(g_1, st_list);

            if (!planar) {
                cerr << "Failed at iteration: " << index << "\n";
            }
//            else {
//                cerr << "Succeeded.\n";
//            }
            EXPECT_EQ(planar, true);

        }
    }
}



/**  @brief tests BLPlanarityTester::isPlanar()  for cube
 *          with all the combinations of {s,t}.
 */
TEST_F(BLPlanarityTesterTests, TestIsPlanarEx03) {

    BLGraph g_1;

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
    /*auto& e_01_07 = */g_1.addEdge(make_unique<Edge>(),n_01,n_07);
    /*auto& e_03_05 = */g_1.addEdge(make_unique<Edge>(),n_03,n_05);
    /*auto& e_05_07 = */g_1.addEdge(make_unique<Edge>(),n_05,n_07);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_04_06 = */g_1.addEdge(make_unique<Edge>(),n_04,n_06);
    /*auto& e_04_02 = */g_1.addEdge(make_unique<Edge>(),n_04,n_02);
    /*auto& e_07_08 = */g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    /*auto& e_06_08 = */g_1.addEdge(make_unique<Edge>(),n_06,n_08);
    /*auto& e_02_08 = */g_1.addEdge(make_unique<Edge>(),n_02,n_08);

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
            BLPlanarityTester tester;
            size_t index = 0;
//            bool planar = tester.isPlanarOldAlg(g_1, st_list, index);
            bool planar = tester.isPlanar(g_1, st_list);

            if (!planar) {
                cerr << "Failed at iteration: " << index << "\n";
            }
//            else {
//                cerr << "Succeeded.\n";
//            }
            EXPECT_EQ(planar, true);

        }
    }
}


/**  @brief tests BLPlanarityTester::isPlanar()  for tetrahedron
 *          with all the combinations of {s,t}.
 */
TEST_F(BLPlanarityTesterTests, TestIsPlanarEx04) {

    BLGraph g_1;

    auto& n_01 = g_1.addNode(make_unique<NumNode>(1));
    auto& n_02 = g_1.addNode(make_unique<NumNode>(2));
    auto& n_03 = g_1.addNode(make_unique<NumNode>(3));
    auto& n_04 = g_1.addNode(make_unique<NumNode>(4));

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_01_03 = */g_1.addEdge(make_unique<Edge>(),n_01,n_03);
    /*auto& e_01_04 = */g_1.addEdge(make_unique<Edge>(),n_01,n_04);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_02_04 = */g_1.addEdge(make_unique<Edge>(),n_02,n_04);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);

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
            BLPlanarityTester tester;
            size_t index = 0;
//            bool planar = tester.isPlanarOldAlg(g_1, st_list, index);
            bool planar = tester.isPlanar(g_1, st_list);

            if (!planar) {
                cerr << "Failed at iteration: " << index << "\n";
            }
//            else {
//                cerr << "Succeeded.\n";
//            }
            EXPECT_EQ(planar, true);

        }
    }
}
