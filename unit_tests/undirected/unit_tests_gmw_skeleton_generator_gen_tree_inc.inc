    // Create underlying block.
    Graph G01;
    auto& n_01 = G01.addNode(make_unique<NumNode>(1));
    auto& n_02 = G01.addNode(make_unique<NumNode>(2));
    auto& n_03 = G01.addNode(make_unique<NumNode>(3));
    auto& n_04 = G01.addNode(make_unique<NumNode>(4));
    auto& n_05 = G01.addNode(make_unique<NumNode>(5));
    auto& n_06 = G01.addNode(make_unique<NumNode>(6));
    auto& n_07 = G01.addNode(make_unique<NumNode>(7));
    auto& n_08 = G01.addNode(make_unique<NumNode>(8));
    auto& n_09 = G01.addNode(make_unique<NumNode>(9));
    auto& n_10 = G01.addNode(make_unique<NumNode>(10));
    auto& n_11 = G01.addNode(make_unique<NumNode>(11));
    auto& n_12 = G01.addNode(make_unique<NumNode>(12));
    auto& n_13 = G01.addNode(make_unique<NumNode>(13));
    auto& n_14 = G01.addNode(make_unique<NumNode>(14));
    auto& n_15 = G01.addNode(make_unique<NumNode>(15));
    auto& n_16 = G01.addNode(make_unique<NumNode>(16));
    auto& n_17 = G01.addNode(make_unique<NumNode>(17));
    auto& n_18 = G01.addNode(make_unique<NumNode>(18));
    auto& n_19 = G01.addNode(make_unique<NumNode>(19));
    auto& n_20 = G01.addNode(make_unique<NumNode>(20));
    auto& n_21 = G01.addNode(make_unique<NumNode>(21));
    auto& n_22 = G01.addNode(make_unique<NumNode>(22));

    auto& e_01_02 = G01.addEdge(make_unique<Edge>(), n_01, n_02);
    auto& e_01_03 = G01.addEdge(make_unique<Edge>(), n_01, n_03);
    auto& e_01_07 = G01.addEdge(make_unique<Edge>(), n_01, n_07);
    auto& e_02_04 = G01.addEdge(make_unique<Edge>(), n_02, n_04);
    auto& e_02_08 = G01.addEdge(make_unique<Edge>(), n_02, n_08);
    auto& e_03_04 = G01.addEdge(make_unique<Edge>(), n_03, n_04);
    auto& e_03_05 = G01.addEdge(make_unique<Edge>(), n_03, n_05);
    auto& e_04_06 = G01.addEdge(make_unique<Edge>(), n_04, n_06);
    auto& e_05_06 = G01.addEdge(make_unique<Edge>(), n_05, n_06);
    auto& e_05_07 = G01.addEdge(make_unique<Edge>(), n_05, n_07);
    auto& e_06_08 = G01.addEdge(make_unique<Edge>(), n_06, n_08);
    auto& e_07_09 = G01.addEdge(make_unique<Edge>(), n_07, n_09);
    auto& e_08_15 = G01.addEdge(make_unique<Edge>(), n_08, n_15);
    auto& e_08_13 = G01.addEdge(make_unique<Edge>(), n_08, n_13);
    auto& e_13_14 = G01.addEdge(make_unique<Edge>(), n_13, n_14);
    auto& e_13_15 = G01.addEdge(make_unique<Edge>(), n_13, n_15);
    auto& e_15_16 = G01.addEdge(make_unique<Edge>(), n_15, n_16);
    auto& e_14_10 = G01.addEdge(make_unique<Edge>(), n_14, n_10);
    auto& e_14_16 = G01.addEdge(make_unique<Edge>(), n_14, n_16);
    auto& e_10_16 = G01.addEdge(make_unique<Edge>(), n_10, n_16);
    auto& e_09_10 = G01.addEdge(make_unique<Edge>(), n_09, n_10);
    auto& e_09_11 = G01.addEdge(make_unique<Edge>(), n_09, n_11);
    auto& e_10_11 = G01.addEdge(make_unique<Edge>(), n_10, n_11);
    auto& e_09_12 = G01.addEdge(make_unique<Edge>(), n_09, n_12);
    auto& e_11_12 = G01.addEdge(make_unique<Edge>(), n_11, n_12);
    auto& e_10_12 = G01.addEdge(make_unique<Edge>(), n_10, n_12);
    auto& e_10_17 = G01.addEdge(make_unique<Edge>(), n_10, n_17);
    auto& e_10_18 = G01.addEdge(make_unique<Edge>(), n_10, n_18);
    auto& e_12_17 = G01.addEdge(make_unique<Edge>(), n_12, n_17);
    auto& e_17_18 = G01.addEdge(make_unique<Edge>(), n_17, n_18);
    auto& e_12_18 = G01.addEdge(make_unique<Edge>(), n_12, n_18);
    auto& e_12_21 = G01.addEdge(make_unique<Edge>(), n_12, n_21);
    auto& e_12_19 = G01.addEdge(make_unique<Edge>(), n_12, n_19);
    auto& e_19_20 = G01.addEdge(make_unique<Edge>(), n_19, n_20);
    auto& e_19_21 = G01.addEdge(make_unique<Edge>(), n_19, n_21);
    auto& e_18_20 = G01.addEdge(make_unique<Edge>(), n_18, n_20);
    auto& e_18_22 = G01.addEdge(make_unique<Edge>(), n_18, n_22);
    auto& e_20_22 = G01.addEdge(make_unique<Edge>(), n_20, n_22);
    auto& e_21_22 = G01.addEdge(make_unique<Edge>(), n_21, n_22);


    Block B01;
    auto& bn_01 = B01.addNode(make_unique<BlockNode>(
                                   n_01.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_02 = B01.addNode(make_unique<BlockNode>(
                                   n_02.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_03 = B01.addNode(make_unique<BlockNode>(
                                   n_03.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_04 = B01.addNode(make_unique<BlockNode>(
                                   n_04.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_05 = B01.addNode(make_unique<BlockNode>(
                                   n_05.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_06 = B01.addNode(make_unique<BlockNode>(
                                   n_06.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_07 = B01.addNode(make_unique<BlockNode>(
                                   n_07.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_08 = B01.addNode(make_unique<BlockNode>(
                                   n_08.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_09 = B01.addNode(make_unique<BlockNode>(
                                   n_09.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_10 = B01.addNode(make_unique<BlockNode>(
                                   n_10.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_11 = B01.addNode(make_unique<BlockNode>(
                                   n_11.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_12 = B01.addNode(make_unique<BlockNode>(
                                   n_12.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_13 = B01.addNode(make_unique<BlockNode>(
                                   n_13.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_14 = B01.addNode(make_unique<BlockNode>(
                                   n_14.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_15 = B01.addNode(make_unique<BlockNode>(
                                   n_15.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_16 = B01.addNode(make_unique<BlockNode>(
                                   n_16.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_17 = B01.addNode(make_unique<BlockNode>(
                                   n_17.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_18 = B01.addNode(make_unique<BlockNode>(
                                   n_18.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_19 = B01.addNode(make_unique<BlockNode>(
                                   n_19.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_20 = B01.addNode(make_unique<BlockNode>(
                                   n_20.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_21 = B01.addNode(make_unique<BlockNode>(
                                   n_21.backIt(),BlockNode::OrdinaryNodeType));
    auto& bn_22 = B01.addNode(make_unique<BlockNode>(
                                   n_22.backIt(),BlockNode::OrdinaryNodeType));

    auto& be_01_02 = B01.addEdge(make_unique<BlockEdge>(
                                              e_01_02.backIt()), bn_01, bn_02);
    auto& be_01_03 = B01.addEdge(make_unique<BlockEdge>(
                                              e_01_03.backIt()), bn_01, bn_03);
    auto& be_01_07 = B01.addEdge(make_unique<BlockEdge>(
                                              e_01_07.backIt()), bn_01, bn_07);
    auto& be_02_04 = B01.addEdge(make_unique<BlockEdge>(
                                              e_02_04.backIt()), bn_02, bn_04);
    auto& be_02_08 = B01.addEdge(make_unique<BlockEdge>(
                                              e_02_08.backIt()), bn_02, bn_08);
    auto& be_03_04 = B01.addEdge(make_unique<BlockEdge>(
                                              e_03_04.backIt()), bn_03, bn_04);
    auto& be_03_05 = B01.addEdge(make_unique<BlockEdge>(
                                              e_03_05.backIt()), bn_03, bn_05);
    auto& be_04_06 = B01.addEdge(make_unique<BlockEdge>(
                                              e_04_06.backIt()), bn_04, bn_06);
    auto& be_05_06 = B01.addEdge(make_unique<BlockEdge>(
                                              e_05_06.backIt()), bn_05, bn_06);
    auto& be_05_07 = B01.addEdge(make_unique<BlockEdge>(
                                              e_05_07.backIt()), bn_05, bn_07);
    auto& be_06_08 = B01.addEdge(make_unique<BlockEdge>(
                                              e_06_08.backIt()), bn_06, bn_08);
    auto& be_07_09 = B01.addEdge(make_unique<BlockEdge>(
                                              e_07_09.backIt()), bn_07, bn_09);
    auto& be_08_15 = B01.addEdge(make_unique<BlockEdge>(
                                              e_08_15.backIt()), bn_08, bn_15);
    auto& be_08_13 = B01.addEdge(make_unique<BlockEdge>(
                                              e_08_13.backIt()), bn_08, bn_13);
    auto& be_13_14 = B01.addEdge(make_unique<BlockEdge>(
                                              e_13_14.backIt()), bn_13, bn_14);
    auto& be_13_15 = B01.addEdge(make_unique<BlockEdge>(
                                              e_13_15.backIt()), bn_13, bn_15);
    auto& be_15_16 = B01.addEdge(make_unique<BlockEdge>(
                                              e_15_16.backIt()), bn_15, bn_16);
    auto& be_14_10 = B01.addEdge(make_unique<BlockEdge>(
                                              e_14_10.backIt()), bn_14, bn_10);
    auto& be_14_16 = B01.addEdge(make_unique<BlockEdge>(
                                              e_14_16.backIt()), bn_14, bn_16);
    auto& be_10_16 = B01.addEdge(make_unique<BlockEdge>(
                                              e_10_16.backIt()), bn_10, bn_16);
    auto& be_09_10 = B01.addEdge(make_unique<BlockEdge>(
                                              e_09_10.backIt()), bn_09, bn_10);
    auto& be_09_11 = B01.addEdge(make_unique<BlockEdge>(
                                              e_09_11.backIt()), bn_09, bn_11);
    auto& be_10_11 = B01.addEdge(make_unique<BlockEdge>(
                                              e_10_11.backIt()), bn_10, bn_11);
    auto& be_09_12 = B01.addEdge(make_unique<BlockEdge>(
                                              e_09_12.backIt()), bn_09, bn_12);
    auto& be_11_12 = B01.addEdge(make_unique<BlockEdge>(
                                              e_11_12.backIt()), bn_11, bn_12);
    auto& be_10_12 = B01.addEdge(make_unique<BlockEdge>(
                                              e_10_12.backIt()), bn_10, bn_12);
    auto& be_10_17 = B01.addEdge(make_unique<BlockEdge>(
                                              e_10_17.backIt()), bn_10, bn_17);
    auto& be_10_18 = B01.addEdge(make_unique<BlockEdge>(
                                              e_10_18.backIt()), bn_10, bn_18);
    auto& be_12_17 = B01.addEdge(make_unique<BlockEdge>(
                                              e_12_17.backIt()), bn_12, bn_17);
    auto& be_17_18 = B01.addEdge(make_unique<BlockEdge>(
                                              e_17_18.backIt()), bn_17, bn_18);
    auto& be_12_18 = B01.addEdge(make_unique<BlockEdge>(
                                              e_12_18.backIt()), bn_12, bn_18);
    auto& be_12_21 = B01.addEdge(make_unique<BlockEdge>(
                                              e_12_21.backIt()), bn_12, bn_21);
    auto& be_12_19 = B01.addEdge(make_unique<BlockEdge>(
                                              e_12_19.backIt()), bn_12, bn_19);
    auto& be_19_20 = B01.addEdge(make_unique<BlockEdge>(
                                              e_19_20.backIt()), bn_19, bn_20);
    auto& be_19_21 = B01.addEdge(make_unique<BlockEdge>(
                                              e_19_21.backIt()), bn_19, bn_21);
    auto& be_18_20 = B01.addEdge(make_unique<BlockEdge>(
                                              e_18_20.backIt()), bn_18, bn_20);
    auto& be_18_22 = B01.addEdge(make_unique<BlockEdge>(
                                              e_18_22.backIt()), bn_18, bn_22);
    auto& be_20_22 = B01.addEdge(make_unique<BlockEdge>(
                                              e_20_22.backIt()), bn_20, bn_22);
    auto& be_21_22 = B01.addEdge(make_unique<BlockEdge>(
                                              e_21_22.backIt()), bn_21, bn_22);


    SPQRTree spqrTree(B01);

    // Create SPQR Tree nodes.

    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_01, SPQRTreeNode::RType);
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_02, SPQRTreeNode::SType);
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_03, SPQRTreeNode::RType);
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_04, SPQRTreeNode::PType);
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_05, SPQRTreeNode::RType);
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_06, SPQRTreeNode::PType);
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_07, SPQRTreeNode::RType);
    auto& tn_08 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_08, SPQRTreeNode::PType);
    auto& tn_09 = dynamic_cast<SPQRTreeNode&>(
                               spqrTree.addNode(make_unique<SPQRTreeNode>()));
    setTreeNodeType(tn_09, SPQRTreeNode::RType);

    auto& c_01 = tn_01.component();
    auto& c_02 = tn_02.component();
    auto& c_03 = tn_03.component();
    auto& c_04 = tn_04.component();
    auto& c_05 = tn_05.component();
    auto& c_06 = tn_06.component();
    auto& c_07 = tn_07.component();
    auto& c_08 = tn_08.component();
    auto& c_09 = tn_09.component();


    // Component 01
    auto& cn_01_01 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_01.backIt()));
    auto& cn_01_02 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_02.backIt()));
    auto& cn_01_03 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_03.backIt()));
    auto& cn_01_04 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_04.backIt()));
    auto& cn_01_05 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_05.backIt()));
    auto& cn_01_06 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_06.backIt()));
    auto& cn_01_07 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_07.backIt()));
    auto& cn_01_08 = c_01.addNode(
                               make_unique<SPQRComponentNode>(bn_08.backIt()));

    auto& ce_01_01_02 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_01_02.backIt()), cn_01_01, cn_01_02));
    setComponentEdgeType(ce_01_01_02, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_01_03 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_01_03.backIt()), cn_01_01, cn_01_03));
    setComponentEdgeType(ce_01_01_03, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_01_07 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_01_07.backIt()), cn_01_01, cn_01_07));
    setComponentEdgeType(ce_01_01_07, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_02_04 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_02_04.backIt()), cn_01_02, cn_01_04));
    setComponentEdgeType(ce_01_02_04, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_03_04 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_03_04.backIt()), cn_01_03, cn_01_04));
    setComponentEdgeType(ce_01_03_04, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_02_08 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_02_08.backIt()), cn_01_02, cn_01_08));
    setComponentEdgeType(ce_01_02_08, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_04_06 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_04_06.backIt()), cn_01_04, cn_01_06));
    setComponentEdgeType(ce_01_04_06, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_03_05 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_03_05.backIt()), cn_01_03, cn_01_05));
    setComponentEdgeType(ce_01_03_05, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_05_06 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_05_06.backIt()), cn_01_05, cn_01_06));
    setComponentEdgeType(ce_01_05_06, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_05_07 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_05_07.backIt()), cn_01_05, cn_01_07));
    setComponentEdgeType(ce_01_05_07, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_06_08 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
       make_unique<SPQRComponentEdge>(be_06_08.backIt()), cn_01_06, cn_01_08));
    setComponentEdgeType(ce_01_06_08, SPQRComponentEdge::OrdinaryType);

    auto& ce_01_07_08 = dynamic_cast<SPQRComponentEdge&>(c_01.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_01_07, cn_01_08));
    setComponentEdgeType(ce_01_07_08, SPQRComponentEdge::VirtualType);


    // Component 02
    auto& cn_02_07 = c_02.addNode(
                               make_unique<SPQRComponentNode>(bn_07.backIt()));
    auto& cn_02_08 = c_02.addNode(
                               make_unique<SPQRComponentNode>(bn_08.backIt()));
    auto& cn_02_09 = c_02.addNode(
                               make_unique<SPQRComponentNode>(bn_09.backIt()));
    auto& cn_02_10 = c_02.addNode(
                               make_unique<SPQRComponentNode>(bn_10.backIt()));

    auto& ce_02_07_08 = dynamic_cast<SPQRComponentEdge&>(c_02.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_02_07, cn_02_08));
    setComponentEdgeType(ce_02_07_08, SPQRComponentEdge::VirtualType);

    auto& ce_02_07_09 = dynamic_cast<SPQRComponentEdge&>(c_02.addEdge(
       make_unique<SPQRComponentEdge>(be_07_09.backIt()), cn_02_07, cn_02_09));
    setComponentEdgeType(ce_02_07_09, SPQRComponentEdge::OrdinaryType);

    auto& ce_02_08_10 = dynamic_cast<SPQRComponentEdge&>(c_02.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_02_08, cn_02_10));
    setComponentEdgeType(ce_02_08_10, SPQRComponentEdge::VirtualType);

    auto& ce_02_09_10 = dynamic_cast<SPQRComponentEdge&>(c_02.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_02_09, cn_02_10));
    setComponentEdgeType(ce_02_09_10, SPQRComponentEdge::VirtualType);


    // Component 03
    auto& cn_03_08 = c_03.addNode(
                               make_unique<SPQRComponentNode>(bn_08.backIt()));
    auto& cn_03_10 = c_03.addNode(
                               make_unique<SPQRComponentNode>(bn_10.backIt()));
    auto& cn_03_13 = c_03.addNode(
                               make_unique<SPQRComponentNode>(bn_13.backIt()));
    auto& cn_03_14 = c_03.addNode(
                               make_unique<SPQRComponentNode>(bn_14.backIt()));
    auto& cn_03_15 = c_03.addNode(
                               make_unique<SPQRComponentNode>(bn_15.backIt()));
    auto& cn_03_16 = c_03.addNode(
                               make_unique<SPQRComponentNode>(bn_16.backIt()));

    auto& ce_03_08_15 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_08_15.backIt()), cn_03_08, cn_03_15));
    setComponentEdgeType(ce_03_08_15, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_08_10 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_03_08, cn_03_10));
    setComponentEdgeType(ce_03_08_10, SPQRComponentEdge::VirtualType);

    auto& ce_03_08_13 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_08_13.backIt()), cn_03_08, cn_03_13));
    setComponentEdgeType(ce_03_08_13, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_13_15 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_13_15.backIt()), cn_03_13, cn_03_15));
    setComponentEdgeType(ce_03_13_15, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_13_14 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_13_14.backIt()), cn_03_13, cn_03_14));
    setComponentEdgeType(ce_03_13_14, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_15_16 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_15_16.backIt()), cn_03_15, cn_03_16));
    setComponentEdgeType(ce_03_15_16, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_14_10 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_14_10.backIt()), cn_03_14, cn_03_10));
    setComponentEdgeType(ce_03_14_10, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_14_16 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_14_16.backIt()), cn_03_14, cn_03_16));
    setComponentEdgeType(ce_03_14_16, SPQRComponentEdge::OrdinaryType);

    auto& ce_03_10_16 = dynamic_cast<SPQRComponentEdge&>(c_03.addEdge(
       make_unique<SPQRComponentEdge>(be_10_16.backIt()), cn_03_10, cn_03_16));
    setComponentEdgeType(ce_03_10_16, SPQRComponentEdge::OrdinaryType);


    // Component 04
    auto& cn_04_09 = c_04.addNode(
                               make_unique<SPQRComponentNode>(bn_09.backIt()));
    auto& cn_04_10 = c_04.addNode(
                               make_unique<SPQRComponentNode>(bn_10.backIt()));

    auto& ce_04_09_10_v1 = dynamic_cast<SPQRComponentEdge&>(c_04.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_04_09, cn_04_10));
    setComponentEdgeType(ce_04_09_10_v1, SPQRComponentEdge::VirtualType);

    auto& ce_04_09_10_v2 = dynamic_cast<SPQRComponentEdge&>(c_04.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_04_09, cn_04_10));
    setComponentEdgeType(ce_04_09_10_v2, SPQRComponentEdge::VirtualType);

    auto& ce_04_09_10 = dynamic_cast<SPQRComponentEdge&>(c_04.addEdge(
      make_unique<SPQRComponentEdge>(be_09_10.backIt()), cn_04_09, cn_04_10));
    setComponentEdgeType(ce_04_09_10, SPQRComponentEdge::OrdinaryType);


    // Component 05
    auto& cn_05_09 = c_05.addNode(
                               make_unique<SPQRComponentNode>(bn_09.backIt()));
    auto& cn_05_10 = c_05.addNode(
                               make_unique<SPQRComponentNode>(bn_10.backIt()));
    auto& cn_05_11 = c_05.addNode(
                               make_unique<SPQRComponentNode>(bn_11.backIt()));
    auto& cn_05_12 = c_05.addNode(
                               make_unique<SPQRComponentNode>(bn_12.backIt()));

    auto& ce_05_09_10 = dynamic_cast<SPQRComponentEdge&>(c_05.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_05_09, cn_05_10));
    setComponentEdgeType(ce_05_09_10, SPQRComponentEdge::VirtualType);

    auto& ce_05_09_11 = dynamic_cast<SPQRComponentEdge&>(c_05.addEdge(
      make_unique<SPQRComponentEdge>(be_09_11.backIt()), cn_05_09, cn_05_11));
    setComponentEdgeType(ce_05_09_11, SPQRComponentEdge::OrdinaryType);

    auto& ce_05_09_12 = dynamic_cast<SPQRComponentEdge&>(c_05.addEdge(
      make_unique<SPQRComponentEdge>(be_09_12.backIt()), cn_05_09, cn_05_12));
    setComponentEdgeType(ce_05_09_12, SPQRComponentEdge::OrdinaryType);

    auto& ce_05_11_12 = dynamic_cast<SPQRComponentEdge&>(c_05.addEdge(
      make_unique<SPQRComponentEdge>(be_11_12.backIt()), cn_05_11, cn_05_12));
    setComponentEdgeType(ce_05_11_12, SPQRComponentEdge::OrdinaryType);

    auto& ce_05_10_11 = dynamic_cast<SPQRComponentEdge&>(c_05.addEdge(
      make_unique<SPQRComponentEdge>(be_10_11.backIt()), cn_05_10, cn_05_11));
    setComponentEdgeType(ce_05_10_11, SPQRComponentEdge::OrdinaryType);

    auto& ce_05_10_12 = dynamic_cast<SPQRComponentEdge&>(c_05.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_05_10, cn_05_12));
    setComponentEdgeType(ce_05_10_12, SPQRComponentEdge::VirtualType);


    // Component 06
    auto& cn_06_10 = c_06.addNode(
                               make_unique<SPQRComponentNode>(bn_10.backIt()));
    auto& cn_06_12 = c_06.addNode(
                               make_unique<SPQRComponentNode>(bn_12.backIt()));

    auto& ce_06_10_12_v1 = dynamic_cast<SPQRComponentEdge&>(c_06.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_06_10, cn_06_12));
    setComponentEdgeType(ce_06_10_12_v1, SPQRComponentEdge::VirtualType);

    auto& ce_06_10_12_v2 = dynamic_cast<SPQRComponentEdge&>(c_06.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_06_10, cn_06_12));
    setComponentEdgeType(ce_06_10_12_v2, SPQRComponentEdge::VirtualType);

    auto& ce_06_10_12 = dynamic_cast<SPQRComponentEdge&>(c_06.addEdge(
       make_unique<SPQRComponentEdge>(be_10_12.backIt()), cn_06_10, cn_06_12));
    setComponentEdgeType(ce_06_10_12, SPQRComponentEdge::OrdinaryType);

    // Component 07
    auto& cn_07_10 = c_07.addNode(
                               make_unique<SPQRComponentNode>(bn_10.backIt()));
    auto& cn_07_12 = c_07.addNode(
                               make_unique<SPQRComponentNode>(bn_12.backIt()));
    auto& cn_07_17 = c_07.addNode(
                               make_unique<SPQRComponentNode>(bn_17.backIt()));
    auto& cn_07_18 = c_07.addNode(
                               make_unique<SPQRComponentNode>(bn_18.backIt()));

    auto& ce_07_10_12 = dynamic_cast<SPQRComponentEdge&>(c_07.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_07_10, cn_07_12));
    setComponentEdgeType(ce_07_10_12, SPQRComponentEdge::VirtualType);

    auto& ce_07_10_17 = dynamic_cast<SPQRComponentEdge&>(c_07.addEdge(
      make_unique<SPQRComponentEdge>(be_10_17.backIt()), cn_07_10, cn_07_17));
    setComponentEdgeType(ce_07_10_17, SPQRComponentEdge::OrdinaryType);

    auto& ce_07_10_18 = dynamic_cast<SPQRComponentEdge&>(c_07.addEdge(
      make_unique<SPQRComponentEdge>(be_10_18.backIt()), cn_07_10, cn_07_18));
    setComponentEdgeType(ce_07_10_18, SPQRComponentEdge::OrdinaryType);

    auto& ce_07_17_18 = dynamic_cast<SPQRComponentEdge&>(c_07.addEdge(
      make_unique<SPQRComponentEdge>(be_17_18.backIt()), cn_07_17, cn_07_18));
    setComponentEdgeType(ce_07_17_18, SPQRComponentEdge::OrdinaryType);

    auto& ce_07_12_18 = dynamic_cast<SPQRComponentEdge&>(c_07.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_07_12, cn_07_18));
    setComponentEdgeType(ce_07_12_18, SPQRComponentEdge::VirtualType);

    auto& ce_07_12_17 = dynamic_cast<SPQRComponentEdge&>(c_07.addEdge(
      make_unique<SPQRComponentEdge>(be_12_17.backIt()), cn_07_12, cn_07_17));
    setComponentEdgeType(ce_07_12_17, SPQRComponentEdge::OrdinaryType);

    // Component 08
    auto& cn_08_12 = c_08.addNode(
                               make_unique<SPQRComponentNode>(bn_12.backIt()));
    auto& cn_08_18 = c_08.addNode(
                               make_unique<SPQRComponentNode>(bn_18.backIt()));

    auto& ce_08_12_18_v1 = dynamic_cast<SPQRComponentEdge&>(c_08.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_08_12, cn_08_18));
    setComponentEdgeType(ce_08_12_18_v1, SPQRComponentEdge::VirtualType);

    auto& ce_08_12_18_v2 = dynamic_cast<SPQRComponentEdge&>(c_08.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_08_12, cn_08_18));
    setComponentEdgeType(ce_08_12_18_v2, SPQRComponentEdge::VirtualType);

    auto& ce_08_12_18 = dynamic_cast<SPQRComponentEdge&>(c_08.addEdge(
      make_unique<SPQRComponentEdge>(be_12_18.backIt()), cn_08_12, cn_08_18));
    setComponentEdgeType(ce_08_12_18, SPQRComponentEdge::OrdinaryType);


    // Component 09
    auto& cn_09_12 = c_09.addNode(
                               make_unique<SPQRComponentNode>(bn_12.backIt()));
    auto& cn_09_18 = c_09.addNode(
                               make_unique<SPQRComponentNode>(bn_18.backIt()));
    auto& cn_09_19 = c_09.addNode(
                               make_unique<SPQRComponentNode>(bn_19.backIt()));
    auto& cn_09_20 = c_09.addNode(
                               make_unique<SPQRComponentNode>(bn_20.backIt()));
    auto& cn_09_21 = c_09.addNode(
                               make_unique<SPQRComponentNode>(bn_21.backIt()));
    auto& cn_09_22 = c_09.addNode(
                               make_unique<SPQRComponentNode>(bn_22.backIt()));

    auto& ce_09_12_18 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(B01.edges().second), cn_09_12, cn_09_18));
    setComponentEdgeType(ce_09_12_18, SPQRComponentEdge::VirtualType);

    auto& ce_09_12_21 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_12_21.backIt()), cn_09_12, cn_09_21));
    setComponentEdgeType(ce_09_12_21, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_12_19 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_12_19.backIt()), cn_09_12, cn_09_19));
    setComponentEdgeType(ce_09_12_19, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_19_20 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_19_20.backIt()), cn_09_19, cn_09_20));
    setComponentEdgeType(ce_09_19_20, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_19_21 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_19_21.backIt()), cn_09_19, cn_09_21));
    setComponentEdgeType(ce_09_19_21, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_21_22 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_21_22.backIt()), cn_09_21, cn_09_22));
    setComponentEdgeType(ce_09_21_22, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_18_20 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_18_20.backIt()), cn_09_18, cn_09_20));
    setComponentEdgeType(ce_09_18_20, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_18_22 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_18_22.backIt()), cn_09_18, cn_09_22));
    setComponentEdgeType(ce_09_18_22, SPQRComponentEdge::OrdinaryType);

    auto& ce_09_20_22 = dynamic_cast<SPQRComponentEdge&>(c_09.addEdge(
      make_unique<SPQRComponentEdge>(be_20_22.backIt()), cn_09_20, cn_09_22));
    setComponentEdgeType(ce_09_20_22, SPQRComponentEdge::OrdinaryType);


    // Create Tree edges.

    auto& te_01_02 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                    ce_01_07_08.backIt(), ce_02_07_08.backIt()), tn_01, tn_02);
    setSPQRTreeEdgeParams(
         ce_01_07_08, te_01_02.backIt(), ce_02_07_08.backIt(), tn_02.backIt());
    setSPQRTreeEdgeParams(
         ce_02_07_08, te_01_02.backIt(), ce_01_07_08.backIt(), tn_01.backIt());

    auto& te_02_03 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                    ce_02_08_10.backIt(), ce_03_08_10.backIt()), tn_02, tn_03);

    setSPQRTreeEdgeParams(
         ce_02_08_10, te_02_03.backIt(), ce_03_08_10.backIt(), tn_03.backIt());
    setSPQRTreeEdgeParams(
         ce_03_08_10, te_02_03.backIt(), ce_02_08_10.backIt(), tn_02.backIt());

    auto& te_02_04 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                 ce_02_09_10.backIt(), ce_04_09_10_v1.backIt()), tn_02, tn_04);

    setSPQRTreeEdgeParams(
      ce_02_09_10, te_02_04.backIt(), ce_04_09_10_v1.backIt(), tn_04.backIt());
    setSPQRTreeEdgeParams(
      ce_04_09_10_v1, te_02_04.backIt(), ce_02_09_10.backIt(), tn_02.backIt());

    auto& te_04_05 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                 ce_04_09_10_v2.backIt(), ce_05_09_10.backIt()), tn_04, tn_05);

    setSPQRTreeEdgeParams(
      ce_04_09_10_v2, te_04_05.backIt(), ce_05_09_10.backIt(), tn_05.backIt());
    setSPQRTreeEdgeParams(
      ce_05_09_10, te_04_05.backIt(), ce_04_09_10_v2.backIt(), tn_04.backIt());

    auto& te_05_06 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                 ce_05_10_12.backIt(), ce_06_10_12_v1.backIt()), tn_05, tn_06);

    setSPQRTreeEdgeParams(
      ce_05_10_12, te_05_06.backIt(), ce_06_10_12_v1.backIt(), tn_06.backIt());
    setSPQRTreeEdgeParams(
      ce_06_10_12_v1, te_05_06.backIt(), ce_05_10_12.backIt(), tn_05.backIt());

    auto& te_06_07 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                 ce_06_10_12_v2.backIt(), ce_07_10_12.backIt()), tn_06, tn_07);

    setSPQRTreeEdgeParams(
      ce_06_10_12_v2, te_06_07.backIt(), ce_07_10_12.backIt(), tn_07.backIt());
    setSPQRTreeEdgeParams(
      ce_07_10_12, te_06_07.backIt(), ce_06_10_12_v2.backIt(), tn_06.backIt());

    auto& te_07_08 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                 ce_07_12_18.backIt(), ce_08_12_18_v1.backIt()), tn_07, tn_08);

    setSPQRTreeEdgeParams(
      ce_07_12_18, te_07_08.backIt(), ce_08_12_18_v1.backIt(), tn_08.backIt());
    setSPQRTreeEdgeParams(
      ce_08_12_18_v1, te_07_08.backIt(), ce_07_12_18.backIt(), tn_07.backIt());

    auto& te_08_09 = spqrTree.addEdge(make_unique<SPQRTreeEdge>(
                 ce_08_12_18_v2.backIt(), ce_09_12_18.backIt()), tn_08, tn_09);

    setSPQRTreeEdgeParams(
      ce_08_12_18_v2, te_08_09.backIt(), ce_09_12_18.backIt(), tn_09.backIt());
    setSPQRTreeEdgeParams(
      ce_09_12_18, te_08_09.backIt(), ce_08_12_18_v2.backIt(), tn_08.backIt());

