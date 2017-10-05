#include "gtest/gtest.h"
#include "undirected/base.hpp"
#include "undirected/bctree.hpp"

namespace Wailea {

namespace Undirected { 


class BCTreeTests : public ::testing::Test {

  protected:  

    std::unique_ptr<Graph> generate_graph(){
        return std::make_unique<Graph>();
    }
 
    std::unique_ptr<Node> generate_node(){
        return std::make_unique<NumNode>(0);
    }

    std::unique_ptr<Edge> generate_edge(){
        return std::make_unique<Edge>();
    }

    BCTreeTests(){;};
    virtual ~BCTreeTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief DFS for an empty graph.
 */
TEST_F(BCTreeTests, EmptyGraph) {

    std::unique_ptr<Graph> gp1 = generate_graph();
    Graph&  g1 = *gp1;

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1);

    EXPECT_EQ(&(tree.original()),&g1);
    auto cv = tree.cutVertexNodes();
    EXPECT_EQ(cv.size(),0);
    auto blocks = tree.blockNodes();
    EXPECT_EQ(blocks.size(),0);
}


/**  @brief DFS for a single node.
 */
TEST_F(BCTreeTests, SingleNode) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node();
    auto& n1  = g1.addNode(std::move(np1));

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1);

    EXPECT_EQ(&(tree.original()),&g1);

    EXPECT_EQ(tree.numNodes(),1);
    auto cv = tree.cutVertexNodes();
    EXPECT_EQ(cv.size(),0);
    auto blocks = tree.blockNodes();
    EXPECT_EQ(blocks.size(),1);
    BCTreeNode& tn = dynamic_cast<BCTreeNode&>(*(*(blocks[0])));
    EXPECT_EQ(tn.degree(),0);
    EXPECT_EQ(tn.type(),BCTreeNode::BlockType);
    Block& b1 = tn.block();
    vector<node_list_it_t> b1_cvs = b1.cutVertices();
    EXPECT_EQ(b1_cvs.size(),0);
    BCTreeNode& b1_tn = b1.BCTreeNode();
    EXPECT_EQ(&b1_tn, &tn);
    EXPECT_EQ(b1.numNodes(), 1);
    EXPECT_EQ(b1.numEdges(), 0);
    BlockNode& bn1 = dynamic_cast<BlockNode&>(*(*(b1.nodes().first)));
    EXPECT_EQ(bn1.degree(),0);
    EXPECT_EQ(bn1.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(bn1.IGBackwardLinkRef()),&n1);
    EXPECT_EQ(n1.IGForwardLink(), bn1.backIt());
    EXPECT_EQ(n1.utility(), ConstantsBCTree::kForwardTypeBlockNode);

}


/**  @brief DFS for a single edge with two nodes.
 */
TEST_F(BCTreeTests, SingleEdge) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node();
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node();
    auto& n2  = g1.addNode(std::move(np2));
    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1);

    EXPECT_EQ(tree.numNodes(),1);
    EXPECT_EQ(tree.numEdges(),0);
    Graph& g2 = tree.original();
    EXPECT_EQ(&g1, &g2);
    BCTreeNode& bctn1 = dynamic_cast<BCTreeNode&>(*(*(tree.nodes().first)));
    EXPECT_EQ(bctn1.type(),BCTreeNode::BlockType);
    Block& block1 = bctn1.block();
    vector<node_list_it_t> b1_cvs = block1.cutVertices();
    EXPECT_EQ(b1_cvs.size(),0);
    BCTreeNode& b1_tn = block1.BCTreeNode();
    EXPECT_EQ(&b1_tn, &bctn1);
    EXPECT_EQ(block1.numNodes(),2);
    EXPECT_EQ(block1.numEdges(),1);
    node_list_it_t bnit = block1.nodes().first;
    edge_list_it_t beit = block1.edges().first;
    BlockNode& bn1 = dynamic_cast<BlockNode&>(*(*(bnit)));
    bnit++;
    BlockNode& bn2 = dynamic_cast<BlockNode&>(*(*(bnit)));
    BlockEdge& be1 = dynamic_cast<BlockEdge&>(*(*(beit)));

    EXPECT_EQ(bn1.degree(),1);
    auto bn1inc = bn1.incidentEdges().first;
    auto& bn1_1 = dynamic_cast<BlockEdge&>(*(*(*bn1inc)));
    EXPECT_EQ(&bn1_1, &be1);
    EXPECT_EQ(bn1.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(bn1.IGBackwardLinkRef()),&n1);

    EXPECT_EQ(bn2.degree(),1);
    auto bn2inc = bn2.incidentEdges().first;
    auto& bn2_1 = dynamic_cast<BlockEdge&>(*(*(*bn2inc)));
    EXPECT_EQ(&bn2_1, &be1);
    EXPECT_EQ(bn2.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(bn2.IGBackwardLinkRef()),&n2);

    EXPECT_EQ(&(be1.IGBackwardLinkRef()),&e1);
    EXPECT_EQ(&(be1.incidentNode1()),&bn1);
    EXPECT_EQ(&(be1.incidentNode2()),&bn2);
    EXPECT_EQ(n1.IGForwardLink(), bn1.backIt());
    EXPECT_EQ(n1.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    EXPECT_EQ(n2.IGForwardLink(), bn2.backIt());
    EXPECT_EQ(n2.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    EXPECT_EQ(e1.IGForwardLink(), be1.backIt());

}


/**  @brief DFS for a path.
 */
TEST_F(BCTreeTests, PathAtTheEnd) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node();
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node();
    auto& n2  = g1.addNode(std::move(np2));
    auto  np3 = generate_node();
    auto& n3  = g1.addNode(std::move(np3));
    auto  np4 = generate_node();
    auto& n4  = g1.addNode(std::move(np4));
    auto  np5 = generate_node();
    auto& n5  = g1.addNode(std::move(np5));
    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);
    auto  ep2 = generate_edge();
    auto& e2  = g1.addEdge(std::move(ep2),n2,n3);
    auto  ep3 = generate_edge();
    auto& e3  = g1.addEdge(std::move(ep3),n3,n4);
    auto  ep4 = generate_edge();
    auto& e4  = g1.addEdge(std::move(ep4),n4,n5);

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1);
    EXPECT_EQ(tree.numNodes(),7);
    EXPECT_EQ(tree.numEdges(),6);

    auto treeNodeIt = tree.nodes().first;
    BCTreeNode& tn1 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn2 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn3 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn4 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn5 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn6 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn7 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;

    auto treeEdgeIt = tree.edges().first;
    BCTreeEdge& te1 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te2 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te3 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te4 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te5 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te6 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));

    EXPECT_EQ(tn1.degree(),2);//cv n4
    EXPECT_EQ(tn1.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn1.IGBackwardLinkRef()),&n4);
    EXPECT_EQ(n4.IGForwardLink(), tn1.backIt());
    EXPECT_EQ(n4.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn2.degree(),1);//b  e4
    EXPECT_EQ(tn2.type(), BCTreeNode::BlockType);
    Block& block2 = tn2.block();

    vector<node_list_it_t> b2_cvs = block2.cutVertices();
    EXPECT_EQ(b2_cvs.size(),1);
    BlockNode& b2_n1 = dynamic_cast<BlockNode&>(*(*(b2_cvs[0]))); 
    BCTreeNode& b2_tn = block2.BCTreeNode();
    EXPECT_EQ(&b2_tn, &tn2);
    EXPECT_EQ(block2.numNodes(),2);
    EXPECT_EQ(block2.numEdges(),1);
    auto block2_nit =  block2.nodes().first;
    BlockNode& block2_bn1 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block2_bn1.IGBackwardLinkRef()),&n4);

    EXPECT_EQ(&(block2_bn1.treeEdge()),&te1);
    EXPECT_EQ(&b2_n1, &block2_bn1);
    block2_nit++;
    BlockNode& block2_bn2 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn2.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block2_bn2.IGBackwardLinkRef()),&n5);
    EXPECT_EQ(n5.IGForwardLink(), block2_bn2.backIt());
    EXPECT_EQ(n5.utility(), ConstantsBCTree::kForwardTypeBlockNode);

    auto block2_eit =  block2.edges().first;
    BlockEdge& block2_be1 = dynamic_cast<BlockEdge&>(*(*block2_eit));
    EXPECT_EQ(&(block2_be1.IGBackwardLinkRef()),&e4);
    EXPECT_EQ(e4.IGForwardLink(), block2_be1.backIt());
    EXPECT_EQ(&(block2_be1.incidentNode1()),&block2_bn1);
    EXPECT_EQ(&(block2_be1.incidentNode2()),&block2_bn2);

    EXPECT_EQ(tn3.degree(),2);//cv n3
    EXPECT_EQ(tn3.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn3.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(n3.IGForwardLink(), tn3.backIt());
    EXPECT_EQ(n3.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn4.degree(),2);//b  e3
    EXPECT_EQ(tn4.type(), BCTreeNode::BlockType);
    Block& block4 = tn4.block();
    vector<node_list_it_t> b4_cvs = block4.cutVertices();
    EXPECT_EQ(b4_cvs.size(),2);
    BlockNode& b4_n1 = dynamic_cast<BlockNode&>(*(*(b4_cvs[0]))); 
    BlockNode& b4_n2 = dynamic_cast<BlockNode&>(*(*(b4_cvs[1]))); 
    BCTreeNode& b4_tn = block4.BCTreeNode();
    EXPECT_EQ(&b4_tn, &tn4);

    EXPECT_EQ(block4.numNodes(),2);
    EXPECT_EQ(block4.numEdges(),1);
    auto block4_nit =  block4.nodes().first;
    BlockNode& block4_bn1 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block4_bn1.IGBackwardLinkRef()),&n3);

    EXPECT_EQ(&(block4_bn1.treeEdge()),&te2);
    EXPECT_EQ(&b4_n1, &block4_bn1);
    block4_nit++;
    BlockNode& block4_bn2 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block4_bn2.IGBackwardLinkRef()),&n4);

    EXPECT_EQ(&(block4_bn2.treeEdge()),&te3);
    EXPECT_EQ(&b4_n2, &block4_bn2);
    auto block4_eit =  block4.edges().first;
    BlockEdge& block4_be1 = dynamic_cast<BlockEdge&>(*(*block4_eit));
    EXPECT_EQ(&(block4_be1.IGBackwardLinkRef()),&e3);
    EXPECT_EQ(e3.IGForwardLink(), block4_be1.backIt());
    EXPECT_EQ(&(block4_be1.incidentNode1()),&block4_bn1);
    EXPECT_EQ(&(block4_be1.incidentNode2()),&block4_bn2);

    EXPECT_EQ(tn5.degree(),2);//cv n2
    EXPECT_EQ(tn5.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn5.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(n2.IGForwardLink(), tn5.backIt());
    EXPECT_EQ(n2.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn6.degree(),2);//b  e2
    EXPECT_EQ(tn6.type(), BCTreeNode::BlockType);
    Block& block6 = tn6.block();
    vector<node_list_it_t> b6_cvs = block6.cutVertices();
    EXPECT_EQ(b6_cvs.size(),2);
    BlockNode& b6_n1 = dynamic_cast<BlockNode&>(*(*(b6_cvs[0]))); 
    BlockNode& b6_n2 = dynamic_cast<BlockNode&>(*(*(b6_cvs[1]))); 
    BCTreeNode& b6_tn = block6.BCTreeNode();
    EXPECT_EQ(&b6_tn, &tn6);
    EXPECT_EQ(block6.numNodes(),2);
    EXPECT_EQ(block6.numEdges(),1);
    auto block6_nit =  block6.nodes().first;
    BlockNode& block6_bn1 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block6_bn1.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(&(block6_bn1.treeEdge()),&te4);
    EXPECT_EQ(&b6_n1, &block6_bn1);
    block6_nit++;
    BlockNode& block6_bn2 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block6_bn2.IGBackwardLinkRef()),&n3);

    EXPECT_EQ(&(block6_bn2.treeEdge()),&te5);
    EXPECT_EQ(&b6_n2, &block6_bn2);
    auto block6_eit =  block6.edges().first;
    BlockEdge& block6_be1 = dynamic_cast<BlockEdge&>(*(*block6_eit));
    EXPECT_EQ(&(block6_be1.IGBackwardLinkRef()),&e2);
    EXPECT_EQ(e2.IGForwardLink(), block6_be1.backIt());
    EXPECT_EQ(&(block6_be1.incidentNode1()),&block6_bn1);
    EXPECT_EQ(&(block6_be1.incidentNode2()),&block6_bn2);

    EXPECT_EQ(tn7.degree(),1);//b  e1
    EXPECT_EQ(tn7.type(), BCTreeNode::BlockType);
    Block& block7 = tn7.block();
    vector<node_list_it_t> b7_cvs = block7.cutVertices();
    EXPECT_EQ(b7_cvs.size(),1);
    BlockNode& b7_n1 = dynamic_cast<BlockNode&>(*(*(b7_cvs[0]))); 
    BCTreeNode& b7_tn = block7.BCTreeNode();
    EXPECT_EQ(&b7_tn, &tn7);

    EXPECT_EQ(block7.numNodes(),2);
    EXPECT_EQ(block7.numEdges(),1);
    auto block7_nit =  block7.nodes().first;
    BlockNode& block7_bn1 = dynamic_cast<BlockNode&>(*(*block7_nit));
    EXPECT_EQ(block7_bn1.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block7_bn1.IGBackwardLinkRef()),&n1);
    EXPECT_EQ(n1.IGForwardLink(), block7_bn1.backIt());
    EXPECT_EQ(n1.utility(), ConstantsBCTree::kForwardTypeBlockNode);

    block7_nit++;
    BlockNode& block7_bn2 = dynamic_cast<BlockNode&>(*(*block7_nit));
    EXPECT_EQ(block7_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block7_bn2.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(&(block7_bn2.treeEdge()),&te6);
    EXPECT_EQ(&b7_n1, &block7_bn2);
    auto block7_eit =  block7.edges().first;
    BlockEdge& block7_be1 = dynamic_cast<BlockEdge&>(*(*block7_eit));
    EXPECT_EQ(&(block7_be1.IGBackwardLinkRef()),&e1);
    EXPECT_EQ(e1.IGForwardLink(), block7_be1.backIt());
    EXPECT_EQ(&(block7_be1.incidentNode1()),&block7_bn1);
    EXPECT_EQ(&(block7_be1.incidentNode2()),&block7_bn2);
     
    EXPECT_EQ(&(te1.blockNode()),&tn2);
    EXPECT_EQ(&(te1.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te1.cutVertexInBlock()),&block2_bn1);
    EXPECT_EQ(te1.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te1.incidentNode1()),&tn1);
    EXPECT_EQ(&(te1.incidentNode2()),&tn2);

    EXPECT_EQ(&(te2.blockNode()),&tn4);
    EXPECT_EQ(&(te2.cutVertexNode()),&tn3);
    EXPECT_EQ(&(te2.cutVertexInBlock()),&block4_bn1);
    EXPECT_EQ(te2.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te2.incidentNode1()),&tn3);
    EXPECT_EQ(&(te2.incidentNode2()),&tn4);

    EXPECT_EQ(&(te3.blockNode()),&tn4);
    EXPECT_EQ(&(te3.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te3.cutVertexInBlock()),&block4_bn2);
    EXPECT_EQ(te3.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te3.incidentNode1()),&tn1);
    EXPECT_EQ(&(te3.incidentNode2()),&tn4);

    EXPECT_EQ(&(te4.blockNode()),&tn6);
    EXPECT_EQ(&(te4.cutVertexNode()),&tn5);
    EXPECT_EQ(&(te4.cutVertexInBlock()),&block6_bn1);
    EXPECT_EQ(te4.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te4.incidentNode1()),&tn5);
    EXPECT_EQ(&(te4.incidentNode2()),&tn6);

    EXPECT_EQ(&(te5.blockNode()),&tn6);
    EXPECT_EQ(&(te5.cutVertexNode()),&tn3);
    EXPECT_EQ(&(te5.cutVertexInBlock()),&block6_bn2);
    EXPECT_EQ(te5.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te5.incidentNode1()),&tn3);
    EXPECT_EQ(&(te5.incidentNode2()),&tn6);

    EXPECT_EQ(&(te6.blockNode()),&tn7);
    EXPECT_EQ(&(te6.cutVertexNode()),&tn5);
    EXPECT_EQ(&(te6.cutVertexInBlock()),&block7_bn2);
    EXPECT_EQ(te6.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te6.incidentNode1()),&tn5);
    EXPECT_EQ(&(te6.incidentNode2()),&tn7);

    // e1----n2----e2----n3----e3----n4----e4
    // tn7   tn5   tn6   tn3   tn4   tn1   tn2
    //    te6   te4   te5   te2   te3   te1



}


/**  @brief DFS for a path starting in the middle
 */
TEST_F(BCTreeTests, PathInTheMiddle) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node();
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node();
    auto& n2  = g1.addNode(std::move(np2));
    auto  np3 = generate_node();
    auto& n3  = g1.addNode(std::move(np3));
    auto  np4 = generate_node();
    auto& n4  = g1.addNode(std::move(np4));
    auto  np5 = generate_node();
    auto& n5  = g1.addNode(std::move(np5));
    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);
    auto  ep2 = generate_edge();
    auto& e2  = g1.addEdge(std::move(ep2),n2,n3);
    auto  ep3 = generate_edge();
    auto& e3  = g1.addEdge(std::move(ep3),n3,n4);
    auto  ep4 = generate_edge();
    auto& e4  = g1.addEdge(std::move(ep4),n4,n5);

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1,n3);
    EXPECT_EQ(tree.numNodes(),7);
    EXPECT_EQ(tree.numEdges(),6);

    auto treeNodeIt = tree.nodes().first;
    BCTreeNode& tn1 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn2 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn3 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn4 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn5 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn6 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn7 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;

    auto treeEdgeIt = tree.edges().first;
    BCTreeEdge& te1 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te2 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te3 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te4 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te5 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te6 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));

    EXPECT_EQ(tn1.degree(),2);//cv n4
    EXPECT_EQ(tn1.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn1.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(n2.IGForwardLink(), tn1.backIt());
    EXPECT_EQ(n2.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn2.degree(),1);
    EXPECT_EQ(tn2.type(), BCTreeNode::BlockType);
    Block& block2 = tn2.block();
    vector<node_list_it_t> b2_cvs = block2.cutVertices();
    EXPECT_EQ(b2_cvs.size(),1);
    BlockNode& b2_n1 = dynamic_cast<BlockNode&>(*(*(b2_cvs[0]))); 
    BCTreeNode& b2_tn = block2.BCTreeNode();
    EXPECT_EQ(&b2_tn, &tn2);
    EXPECT_EQ(block2.numNodes(),2);
    EXPECT_EQ(block2.numEdges(),1);
    auto block2_nit =  block2.nodes().first;
    BlockNode& block2_bn1 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn1.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block2_bn1.IGBackwardLinkRef()),&n1);
    EXPECT_EQ(n1.IGForwardLink(), block2_bn1.backIt());
    EXPECT_EQ(n1.utility(), ConstantsBCTree::kForwardTypeBlockNode);

    block2_nit++;
    BlockNode& block2_bn2 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block2_bn2.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(&(block2_bn2.treeEdge()),&te1);
    EXPECT_EQ(&b2_n1,&block2_bn2);
  
    auto block2_eit =  block2.edges().first;
    BlockEdge& block2_be1 = dynamic_cast<BlockEdge&>(*(*block2_eit));
    EXPECT_EQ(&(block2_be1.IGBackwardLinkRef()),&e1);
    EXPECT_EQ(e1.IGForwardLink(), block2_be1.backIt());
    EXPECT_EQ(&(block2_be1.incidentNode1()),&block2_bn1);
    EXPECT_EQ(&(block2_be1.incidentNode2()),&block2_bn2);

    EXPECT_EQ(tn3.degree(),2);
    EXPECT_EQ(tn3.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn3.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(n3.IGForwardLink(), tn3.backIt());
    EXPECT_EQ(n3.utility(), ConstantsBCTree::kForwardTypeCutVertex);


    EXPECT_EQ(tn4.degree(),2);
    EXPECT_EQ(tn4.type(), BCTreeNode::BlockType);
    Block& block4 = tn4.block();
    vector<node_list_it_t> b4_cvs = block4.cutVertices();
    EXPECT_EQ(b4_cvs.size(),2);
    BlockNode& b4_n1 = dynamic_cast<BlockNode&>(*(*(b4_cvs[0]))); 
    BlockNode& b4_n2 = dynamic_cast<BlockNode&>(*(*(b4_cvs[1]))); 
    BCTreeNode& b4_tn = block4.BCTreeNode();
    EXPECT_EQ(&b4_tn, &tn4);
    EXPECT_EQ(block4.numNodes(),2);
    EXPECT_EQ(block4.numEdges(),1);
    auto block4_nit =  block4.nodes().first;
    BlockNode& block4_bn1 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block4_bn1.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(&(block4_bn1.treeEdge()),&te2);
    EXPECT_EQ(&b4_n1,&block4_bn1);
    block4_nit++;
    BlockNode& block4_bn2 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block4_bn2.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(&(block4_bn2.treeEdge()),&te3);
    EXPECT_EQ(&b4_n2,&block4_bn2);
    auto block4_eit =  block4.edges().first;
    BlockEdge& block4_be1 = dynamic_cast<BlockEdge&>(*(*block4_eit));
    EXPECT_EQ(&(block4_be1.IGBackwardLinkRef()),&e2);
    EXPECT_EQ(e2.IGForwardLink(), block4_be1.backIt());
    EXPECT_EQ(&(block4_be1.incidentNode1()),&block4_bn1);
    EXPECT_EQ(&(block4_be1.incidentNode2()),&block4_bn2);

    EXPECT_EQ(tn5.degree(),2);
    EXPECT_EQ(tn5.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn5.IGBackwardLinkRef()),&n4);
    EXPECT_EQ(n4.IGForwardLink(), tn5.backIt());
    EXPECT_EQ(n4.utility(), ConstantsBCTree::kForwardTypeCutVertex);


    EXPECT_EQ(tn6.degree(),1);
    EXPECT_EQ(tn6.type(), BCTreeNode::BlockType);
    Block& block6 = tn6.block();
    vector<node_list_it_t> b6_cvs = block6.cutVertices();
    EXPECT_EQ(b6_cvs.size(),1);
    BlockNode& b6_n1 = dynamic_cast<BlockNode&>(*(*(b6_cvs[0]))); 
    BCTreeNode& b6_tn = block6.BCTreeNode();
    EXPECT_EQ(&b6_tn, &tn6);
    EXPECT_EQ(block6.numNodes(),2);
    EXPECT_EQ(block6.numEdges(),1);
    auto block6_nit =  block6.nodes().first;
    BlockNode& block6_bn1 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block6_bn1.IGBackwardLinkRef()),&n4);
    EXPECT_EQ(&(block6_bn1.treeEdge()),&te4);
    EXPECT_EQ(&b6_n1,&block6_bn1);
    block6_nit++;
    BlockNode& block6_bn2 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn2.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block6_bn2.IGBackwardLinkRef()),&n5);
    EXPECT_EQ(n5.IGForwardLink(), block6_bn2.backIt());
    EXPECT_EQ(n5.utility(), ConstantsBCTree::kForwardTypeBlockNode);

    auto block6_eit =  block6.edges().first;
    BlockEdge& block6_be1 = dynamic_cast<BlockEdge&>(*(*block6_eit));
    EXPECT_EQ(&(block6_be1.IGBackwardLinkRef()),&e4);
    EXPECT_EQ(e4.IGForwardLink(), block6_be1.backIt());
    EXPECT_EQ(&(block6_be1.incidentNode1()),&block6_bn1);
    EXPECT_EQ(&(block6_be1.incidentNode2()),&block6_bn2);

    EXPECT_EQ(tn7.degree(),2);
    EXPECT_EQ(tn7.type(), BCTreeNode::BlockType);
    Block& block7 = tn7.block();
    vector<node_list_it_t> b7_cvs = block7.cutVertices();
    EXPECT_EQ(b7_cvs.size(),2);
    BlockNode& b7_n1 = dynamic_cast<BlockNode&>(*(*(b7_cvs[0]))); 
    BlockNode& b7_n2 = dynamic_cast<BlockNode&>(*(*(b7_cvs[1]))); 
    BCTreeNode& b7_tn = block7.BCTreeNode();
    EXPECT_EQ(&b7_tn, &tn7);
    EXPECT_EQ(block7.numNodes(),2);
    EXPECT_EQ(block7.numEdges(),1);
    auto block7_nit =  block7.nodes().first;
    BlockNode& block7_bn1 = dynamic_cast<BlockNode&>(*(*block7_nit));
    EXPECT_EQ(block7_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block7_bn1.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(&(block7_bn1.treeEdge()),&te5);
    EXPECT_EQ(&b7_n1,&block7_bn1);
    block7_nit++;
    BlockNode& block7_bn2 = dynamic_cast<BlockNode&>(*(*block7_nit));
    EXPECT_EQ(block7_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block7_bn2.IGBackwardLinkRef()),&n4);
    EXPECT_EQ(&(block7_bn2.treeEdge()),&te6);
    EXPECT_EQ(&b7_n2,&block7_bn2);
    auto block7_eit =  block7.edges().first;
    BlockEdge& block7_be1 = dynamic_cast<BlockEdge&>(*(*block7_eit));
    EXPECT_EQ(&(block7_be1.IGBackwardLinkRef()),&e3);
    EXPECT_EQ(e3.IGForwardLink(), block7_be1.backIt());
    EXPECT_EQ(&(block7_be1.incidentNode1()),&block7_bn1);
    EXPECT_EQ(&(block7_be1.incidentNode2()),&block7_bn2);

    EXPECT_EQ(&(te1.blockNode()),&tn2);
    EXPECT_EQ(&(te1.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te1.cutVertexInBlock()),&block2_bn2);
    EXPECT_EQ(te1.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te1.incidentNode1()),&tn1);
    EXPECT_EQ(&(te1.incidentNode2()),&tn2);

    EXPECT_EQ(&(te2.blockNode()),&tn4);
    EXPECT_EQ(&(te2.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te2.cutVertexInBlock()),&block4_bn1);
    EXPECT_EQ(te2.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te2.incidentNode1()),&tn1);
    EXPECT_EQ(&(te2.incidentNode2()),&tn4);

    EXPECT_EQ(&(te3.blockNode()),&tn4);
    EXPECT_EQ(&(te3.cutVertexNode()),&tn3);
    EXPECT_EQ(&(te3.cutVertexInBlock()),&block4_bn2);
    EXPECT_EQ(te3.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te3.incidentNode1()),&tn3);
    EXPECT_EQ(&(te3.incidentNode2()),&tn4);

    EXPECT_EQ(&(te4.blockNode()),&tn6);
    EXPECT_EQ(&(te4.cutVertexNode()),&tn5);
    EXPECT_EQ(&(te4.cutVertexInBlock()),&block6_bn1);
    EXPECT_EQ(te4.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te4.incidentNode1()),&tn5);
    EXPECT_EQ(&(te4.incidentNode2()),&tn6);

    EXPECT_EQ(&(te5.blockNode()),&tn7);
    EXPECT_EQ(&(te5.cutVertexNode()),&tn3);
    EXPECT_EQ(&(te5.cutVertexInBlock()),&block7_bn1);
    EXPECT_EQ(te5.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te5.incidentNode1()),&tn3);
    EXPECT_EQ(&(te5.incidentNode2()),&tn7);

    EXPECT_EQ(&(te6.blockNode()),&tn7);
    EXPECT_EQ(&(te6.cutVertexNode()),&tn5);
    EXPECT_EQ(&(te6.cutVertexInBlock()),&block7_bn2);
    EXPECT_EQ(te6.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te6.incidentNode1()),&tn5);
    EXPECT_EQ(&(te6.incidentNode2()),&tn7);

    // n1-----(e1)-----n2-----(e2)-----n3-----(e3)-----n4-----(e4)-----n5
    //                               Start
    //         tn2     tn1     tn4     tn3     tn7     tn5     tn6
    //          b2              b4              b7              b6
    //             te1     te2     te3    te5      te6     te4
}


/**  @brief DFS for a path starting in the middle
 */
TEST_F(BCTreeTests, SingleBlockK4) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node();
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node();
    auto& n2  = g1.addNode(std::move(np2));
    auto  np3 = generate_node();
    auto& n3  = g1.addNode(std::move(np3));
    auto  np4 = generate_node();
    auto& n4  = g1.addNode(std::move(np4));

    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);
    auto  ep2 = generate_edge();
    auto& e2  = g1.addEdge(std::move(ep2),n1,n3);
    auto  ep3 = generate_edge();
    auto& e3  = g1.addEdge(std::move(ep3),n1,n4);
    auto  ep4 = generate_edge();
    auto& e4  = g1.addEdge(std::move(ep4),n2,n3);
    auto  ep5 = generate_edge();
    auto& e5  = g1.addEdge(std::move(ep5),n2,n4);
    auto  ep6 = generate_edge();
    auto& e6  = g1.addEdge(std::move(ep6),n3,n4);

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1,n1);
    EXPECT_EQ(tree.numNodes(),1);
    EXPECT_EQ(tree.numEdges(),0);

    BCTreeNode& tn1 = dynamic_cast<BCTreeNode&>(*(*tree.nodes().first));
    EXPECT_EQ(tn1.type(),BCTreeNode::BlockType);
    Block& block1 = tn1.block();
    vector<node_list_it_t> b1_cvs = block1.cutVertices();
    EXPECT_EQ(b1_cvs.size(),0);
    BCTreeNode& b1_tn = block1.BCTreeNode();
    EXPECT_EQ(&b1_tn, &tn1);

    EXPECT_EQ(block1.numNodes(),4);
    EXPECT_EQ(block1.numEdges(),6);

    //Order of exploration: e1->e4->e2->e6->e3->e5
    auto edgeIt = block1.edges().first;
    BlockEdge& block1_e1 = dynamic_cast<BlockEdge&>(*(*edgeIt));
    EXPECT_EQ(&(block1_e1.IGBackwardLinkRef()), &e5);
    EXPECT_EQ(e5.IGForwardLink(), block1_e1.backIt());
    edgeIt++;
    BlockEdge& block1_e2 = dynamic_cast<BlockEdge&>(*(*edgeIt));
    EXPECT_EQ(&(block1_e2.IGBackwardLinkRef()), &e3);
    EXPECT_EQ(e3.IGForwardLink(), block1_e2.backIt());
    edgeIt++;
    BlockEdge& block1_e3 = dynamic_cast<BlockEdge&>(*(*edgeIt));
    EXPECT_EQ(&(block1_e3.IGBackwardLinkRef()), &e6);
    EXPECT_EQ(e6.IGForwardLink(), block1_e3.backIt());
    edgeIt++;
    BlockEdge& block1_e4 = dynamic_cast<BlockEdge&>(*(*edgeIt));
    EXPECT_EQ(&(block1_e4.IGBackwardLinkRef()), &e2);
    EXPECT_EQ(e2.IGForwardLink(), block1_e4.backIt());
    edgeIt++;
    BlockEdge& block1_e5 = dynamic_cast<BlockEdge&>(*(*edgeIt));
    EXPECT_EQ(&(block1_e5.IGBackwardLinkRef()), &e4);
    EXPECT_EQ(e4.IGForwardLink(), block1_e5.backIt());
    edgeIt++;
    BlockEdge& block1_e6 = dynamic_cast<BlockEdge&>(*(*edgeIt));
    EXPECT_EQ(&(block1_e6.IGBackwardLinkRef()), &e1);
    EXPECT_EQ(e1.IGForwardLink(), block1_e6.backIt());

    // Order of induced nodes. n2->n4->n1->n3
    auto nodeIt = block1.nodes().first;
    BlockNode& block1_n1 = dynamic_cast<BlockNode&>(*(*nodeIt));
    EXPECT_EQ(&(block1_n1.IGBackwardLinkRef()), &n2);
    EXPECT_EQ(block1_n1.type(), BlockNode::OrdinaryNodeType);
    EXPECT_EQ(n2.IGForwardLink(), block1_n1.backIt());
    EXPECT_EQ(n2.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    nodeIt++;
    BlockNode& block1_n2 = dynamic_cast<BlockNode&>(*(*nodeIt));
    EXPECT_EQ(&(block1_n2.IGBackwardLinkRef()), &n4);
    EXPECT_EQ(block1_n2.type(), BlockNode::OrdinaryNodeType);
    EXPECT_EQ(n4.IGForwardLink(), block1_n2.backIt());
    EXPECT_EQ(n4.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    nodeIt++;
    BlockNode& block1_n3 = dynamic_cast<BlockNode&>(*(*nodeIt));
    EXPECT_EQ(&(block1_n3.IGBackwardLinkRef()), &n1);
    EXPECT_EQ(block1_n3.type(), BlockNode::OrdinaryNodeType);
    EXPECT_EQ(n1.IGForwardLink(), block1_n3.backIt());
    EXPECT_EQ(n1.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    nodeIt++;
    BlockNode& block1_n4 = dynamic_cast<BlockNode&>(*(*nodeIt));
    EXPECT_EQ(&(block1_n4.IGBackwardLinkRef()), &n3);
    EXPECT_EQ(block1_n4.type(), BlockNode::OrdinaryNodeType);
    EXPECT_EQ(n3.IGForwardLink(), block1_n4.backIt());
    EXPECT_EQ(n3.utility(), ConstantsBCTree::kForwardTypeBlockNode);


}


/**  @brief DFS for a mid-sized graph from an ordinary node.
 */
TEST_F(BCTreeTests, RootBlock) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node();
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node();
    auto& n2  = g1.addNode(std::move(np2));
    auto  np3 = generate_node();
    auto& n3  = g1.addNode(std::move(np3));
    auto  np4 = generate_node();
    auto& n4  = g1.addNode(std::move(np4));
    auto  np5 = generate_node();
    auto& n5  = g1.addNode(std::move(np5));
    auto  np6 = generate_node();
    auto& n6  = g1.addNode(std::move(np6));
    auto  np7 = generate_node();
    auto& n7  = g1.addNode(std::move(np7));
    auto  np8 = generate_node();
    auto& n8  = g1.addNode(std::move(np8));
    auto  np9 = generate_node();
    auto& n9  = g1.addNode(std::move(np9));

    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);
    auto  ep2 = generate_edge();
    auto& e2  = g1.addEdge(std::move(ep2),n1,n3);
    auto  ep3 = generate_edge();
    auto& e3  = g1.addEdge(std::move(ep3),n2,n3);
    auto  ep4 = generate_edge();
    auto& e4  = g1.addEdge(std::move(ep4),n3,n4);
    auto  ep5 = generate_edge();
    auto& e5  = g1.addEdge(std::move(ep5),n3,n5);
    auto  ep6 = generate_edge();
    auto& e6  = g1.addEdge(std::move(ep6),n4,n5);
    auto  ep7 = generate_edge();
    auto& e7  = g1.addEdge(std::move(ep7),n3,n6);
    auto  ep8 = generate_edge();
    auto& e8  = g1.addEdge(std::move(ep8),n3,n7);
    auto  ep9 = generate_edge();
    auto& e9  = g1.addEdge(std::move(ep9),n6,n7);
    auto  ep10 = generate_edge();
    auto& e10  = g1.addEdge(std::move(ep10),n6,n8);
    auto  ep11 = generate_edge();
    auto& e11  = g1.addEdge(std::move(ep11),n6,n9);
    auto  ep12 = generate_edge();
    auto& e12  = g1.addEdge(std::move(ep12),n8,n9);

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1,n1);
    EXPECT_EQ(tree.numNodes(),6);
    EXPECT_EQ(tree.numEdges(),5);

    auto treeNodeIt = tree.nodes().first;
    BCTreeNode& tn1 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn2 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn3 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn4 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn5 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn6 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));

    auto treeEdgeIt = tree.edges().first;
    BCTreeEdge& te1 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te2 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te3 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te4 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    BCTreeEdge& te5 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));

    EXPECT_EQ(tn1.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn1.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(n3.IGForwardLink(), tn1.backIt());
    EXPECT_EQ(n3.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn2.type(), BCTreeNode::BlockType);
    Block& block2 = tn2.block();
    vector<node_list_it_t> b2_cvs = block2.cutVertices();
    EXPECT_EQ(b2_cvs.size(),1);
    BlockNode& b2_n1 = dynamic_cast<BlockNode&>(*(*(b2_cvs[0]))); 
    BCTreeNode& b2_tn = block2.BCTreeNode();
    EXPECT_EQ(&b2_tn, &tn2);
    EXPECT_EQ(block2.numNodes(),3);
    EXPECT_EQ(block2.numEdges(),3);
    auto block2_nit =  block2.nodes().first;
    BlockNode& block2_bn1 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block2_bn1.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(&(block2_bn1.treeEdge()),&te1);
    EXPECT_EQ(&b2_n1,&block2_bn1);
    block2_nit++;
    BlockNode& block2_bn2 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn2.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block2_bn2.IGBackwardLinkRef()),&n5);
    EXPECT_EQ(n5.IGForwardLink(), block2_bn2.backIt());
    EXPECT_EQ(n5.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    block2_nit++;
    BlockNode& block2_bn3 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn3.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block2_bn3.IGBackwardLinkRef()),&n4);
    EXPECT_EQ(n4.IGForwardLink(), block2_bn3.backIt());
    EXPECT_EQ(n4.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    auto block2_eit =  block2.edges().first;
    BlockEdge& block2_be1 = dynamic_cast<BlockEdge&>(*(*block2_eit));
    EXPECT_EQ(&(block2_be1.IGBackwardLinkRef()),&e5);
    EXPECT_EQ(e5.IGForwardLink(), block2_be1.backIt());
    EXPECT_EQ(&(block2_be1.incidentNode1()),&block2_bn1);
    EXPECT_EQ(&(block2_be1.incidentNode2()),&block2_bn2);
    block2_eit++;
    BlockEdge& block2_be2 = dynamic_cast<BlockEdge&>(*(*block2_eit));
    EXPECT_EQ(&(block2_be2.IGBackwardLinkRef()),&e6);
    EXPECT_EQ(e6.IGForwardLink(), block2_be2.backIt());
    EXPECT_EQ(&(block2_be2.incidentNode1()),&block2_bn3);
    EXPECT_EQ(&(block2_be2.incidentNode2()),&block2_bn2);
    block2_eit++;
    BlockEdge& block2_be3 = dynamic_cast<BlockEdge&>(*(*block2_eit));
    EXPECT_EQ(&(block2_be3.IGBackwardLinkRef()),&e4);
    EXPECT_EQ(e4.IGForwardLink(), block2_be3.backIt());
    EXPECT_EQ(&(block2_be3.incidentNode1()),&block2_bn1);
    EXPECT_EQ(&(block2_be3.incidentNode2()),&block2_bn3);

    EXPECT_EQ(tn3.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn3.IGBackwardLinkRef()),&n6);
    EXPECT_EQ(n6.IGForwardLink(), tn3.backIt());
    EXPECT_EQ(n6.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn4.type(), BCTreeNode::BlockType);
    Block& block4 = tn4.block();
    vector<node_list_it_t> b4_cvs = block4.cutVertices();
    EXPECT_EQ(b4_cvs.size(),1);
    BlockNode& b4_n1 = dynamic_cast<BlockNode&>(*(*(b4_cvs[0]))); 
    BCTreeNode& b4_tn = block4.BCTreeNode();
    EXPECT_EQ(&b4_tn, &tn4);
    EXPECT_EQ(block4.numNodes(),3);
    EXPECT_EQ(block4.numEdges(),3);
    auto block4_nit =  block4.nodes().first;
    BlockNode& block4_bn1 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block4_bn1.IGBackwardLinkRef()),&n6);
    EXPECT_EQ(&(block4_bn1.treeEdge()),&te2);
    EXPECT_EQ(&b4_n1,&block4_bn1);
    block4_nit++;
    BlockNode& block4_bn2 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn2.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block4_bn2.IGBackwardLinkRef()),&n9);
    EXPECT_EQ(n9.IGForwardLink(), block4_bn2.backIt());
    EXPECT_EQ(n9.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    block4_nit++;
    BlockNode& block4_bn3 = dynamic_cast<BlockNode&>(*(*block4_nit));
    EXPECT_EQ(block4_bn3.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block4_bn3.IGBackwardLinkRef()),&n8);
    EXPECT_EQ(n8.IGForwardLink(), block4_bn3.backIt());
    EXPECT_EQ(n8.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    auto block4_eit =  block4.edges().first;
    BlockEdge& block4_be1 = dynamic_cast<BlockEdge&>(*(*block4_eit));
    EXPECT_EQ(&(block4_be1.IGBackwardLinkRef()),&e11);
    EXPECT_EQ(e11.IGForwardLink(), block4_be1.backIt());
    EXPECT_EQ(&(block4_be1.incidentNode1()),&block4_bn1);
    EXPECT_EQ(&(block4_be1.incidentNode2()),&block4_bn2);
    block4_eit++;
    BlockEdge& block4_be2 = dynamic_cast<BlockEdge&>(*(*block4_eit));
    EXPECT_EQ(&(block4_be2.IGBackwardLinkRef()),&e12);
    EXPECT_EQ(e12.IGForwardLink(), block4_be2.backIt());
    EXPECT_EQ(&(block4_be2.incidentNode1()),&block4_bn3);
    EXPECT_EQ(&(block4_be2.incidentNode2()),&block4_bn2);
    block4_eit++;
    BlockEdge& block4_be3 = dynamic_cast<BlockEdge&>(*(*block4_eit));
    EXPECT_EQ(&(block4_be3.IGBackwardLinkRef()),&e10);
    EXPECT_EQ(e10.IGForwardLink(), block4_be3.backIt());
    EXPECT_EQ(&(block4_be3.incidentNode1()),&block4_bn1);
    EXPECT_EQ(&(block4_be3.incidentNode2()),&block4_bn3);

    EXPECT_EQ(tn5.type(), BCTreeNode::BlockType);
    Block& block5 = tn5.block();
    vector<node_list_it_t> b5_cvs = block5.cutVertices();
    EXPECT_EQ(b5_cvs.size(),2);
    BlockNode& b5_n1 = dynamic_cast<BlockNode&>(*(*(b5_cvs[0]))); 
    BlockNode& b5_n2 = dynamic_cast<BlockNode&>(*(*(b5_cvs[1]))); 
    BCTreeNode& b5_tn = block5.BCTreeNode();
    EXPECT_EQ(&b5_tn, &tn5);
    EXPECT_EQ(block5.numNodes(),3);
    EXPECT_EQ(block5.numEdges(),3);
    auto block5_nit =  block5.nodes().first;
    BlockNode& block5_bn1 = dynamic_cast<BlockNode&>(*(*block5_nit));
    EXPECT_EQ(block5_bn1.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block5_bn1.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(&(block5_bn1.treeEdge()),&te3);
    EXPECT_EQ(&b5_n1,&block5_bn1);
    block5_nit++;
    BlockNode& block5_bn2 = dynamic_cast<BlockNode&>(*(*block5_nit));
    EXPECT_EQ(block5_bn2.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block5_bn2.IGBackwardLinkRef()),&n7);
    EXPECT_EQ(n7.IGForwardLink(), block5_bn2.backIt());
    EXPECT_EQ(n7.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    block5_nit++;
    BlockNode& block5_bn3 = dynamic_cast<BlockNode&>(*(*block5_nit));
    EXPECT_EQ(block5_bn3.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block5_bn3.IGBackwardLinkRef()),&n6);
    EXPECT_EQ(&(block5_bn3.treeEdge()),&te4);
    EXPECT_EQ(&b5_n2,&block5_bn3);
    auto block5_eit =  block5.edges().first;
    BlockEdge& block5_be1 = dynamic_cast<BlockEdge&>(*(*block5_eit));
    EXPECT_EQ(&(block5_be1.IGBackwardLinkRef()),&e8);
    EXPECT_EQ(e8.IGForwardLink(), block5_be1.backIt());
    EXPECT_EQ(&(block5_be1.incidentNode1()),&block5_bn1);
    EXPECT_EQ(&(block5_be1.incidentNode2()),&block5_bn2);
    block5_eit++;
    BlockEdge& block5_be2 = dynamic_cast<BlockEdge&>(*(*block5_eit));
    EXPECT_EQ(&(block5_be2.IGBackwardLinkRef()),&e9);
    EXPECT_EQ(e9.IGForwardLink(), block5_be2.backIt());
    EXPECT_EQ(&(block5_be2.incidentNode1()),&block5_bn3);
    EXPECT_EQ(&(block5_be2.incidentNode2()),&block5_bn2);
    block5_eit++;
    BlockEdge& block5_be3 = dynamic_cast<BlockEdge&>(*(*block5_eit));
    EXPECT_EQ(&(block5_be3.IGBackwardLinkRef()),&e7);
    EXPECT_EQ(e7.IGForwardLink(), block5_be3.backIt());
    EXPECT_EQ(&(block5_be3.incidentNode1()),&block5_bn1);
    EXPECT_EQ(&(block5_be3.incidentNode2()),&block5_bn3);

    EXPECT_EQ(tn6.type(), BCTreeNode::BlockType);
    Block& block6 = tn6.block();
    vector<node_list_it_t> b6_cvs = block6.cutVertices();
    EXPECT_EQ(b6_cvs.size(),1);
    BlockNode& b6_n1 = dynamic_cast<BlockNode&>(*(*(b6_cvs[0]))); 
    BCTreeNode& b6_tn = block6.BCTreeNode();
    EXPECT_EQ(&b6_tn, &tn6);
    EXPECT_EQ(block6.numNodes(),3);
    EXPECT_EQ(block6.numEdges(),3);
    auto block6_nit =  block6.nodes().first;
    BlockNode& block6_bn1 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn1.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block6_bn1.IGBackwardLinkRef()),&n1);
    EXPECT_EQ(n1.IGForwardLink(), block6_bn1.backIt());
    EXPECT_EQ(n1.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    block6_nit++;
    BlockNode& block6_bn2 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block6_bn2.IGBackwardLinkRef()),&n3);
    EXPECT_EQ(&(block6_bn2.treeEdge()),&te5);
    EXPECT_EQ(&b6_n1,&block6_bn2);
    block6_nit++;
    BlockNode& block6_bn3 = dynamic_cast<BlockNode&>(*(*block6_nit));
    EXPECT_EQ(block6_bn3.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block6_bn3.IGBackwardLinkRef()),&n2);
    EXPECT_EQ(n2.IGForwardLink(), block6_bn3.backIt());
    EXPECT_EQ(n2.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    auto block6_eit =  block6.edges().first;
    BlockEdge& block6_be1 = dynamic_cast<BlockEdge&>(*(*block6_eit));
    EXPECT_EQ(&(block6_be1.IGBackwardLinkRef()),&e2);
    EXPECT_EQ(e2.IGForwardLink(), block6_be1.backIt());
    EXPECT_EQ(&(block6_be1.incidentNode1()),&block6_bn1);
    EXPECT_EQ(&(block6_be1.incidentNode2()),&block6_bn2);
    block6_eit++;
    BlockEdge& block6_be2 = dynamic_cast<BlockEdge&>(*(*block6_eit));
    EXPECT_EQ(&(block6_be2.IGBackwardLinkRef()),&e3);
    EXPECT_EQ(e3.IGForwardLink(), block6_be2.backIt());
    EXPECT_EQ(&(block6_be2.incidentNode1()),&block6_bn3);
    EXPECT_EQ(&(block6_be2.incidentNode2()),&block6_bn2);
    block6_eit++;
    BlockEdge& block6_be3 = dynamic_cast<BlockEdge&>(*(*block6_eit));
    EXPECT_EQ(&(block6_be3.IGBackwardLinkRef()),&e1);
    EXPECT_EQ(e1.IGForwardLink(), block6_be3.backIt());
    EXPECT_EQ(&(block6_be3.incidentNode1()),&block6_bn1);
    EXPECT_EQ(&(block6_be3.incidentNode2()),&block6_bn3);

    EXPECT_EQ(&(te1.blockNode()),&tn2);
    EXPECT_EQ(&(te1.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te1.cutVertexInBlock()),&block2_bn1);
    EXPECT_EQ(te1.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te1.incidentNode1()),&tn1);
    EXPECT_EQ(&(te1.incidentNode2()),&tn2);

    EXPECT_EQ(&(te2.blockNode()),&tn4);
    EXPECT_EQ(&(te2.cutVertexNode()),&tn3);
    EXPECT_EQ(&(te2.cutVertexInBlock()),&block4_bn1);
    EXPECT_EQ(te2.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te2.incidentNode1()),&tn3);
    EXPECT_EQ(&(te2.incidentNode2()),&tn4);

    EXPECT_EQ(&(te3.blockNode()),&tn5);
    EXPECT_EQ(&(te3.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te3.cutVertexInBlock()),&block5_bn1);
    EXPECT_EQ(te3.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te3.incidentNode1()),&tn1);
    EXPECT_EQ(&(te3.incidentNode2()),&tn5);

    EXPECT_EQ(&(te4.blockNode()),&tn5);
    EXPECT_EQ(&(te4.cutVertexNode()),&tn3);
    EXPECT_EQ(&(te4.cutVertexInBlock()),&block5_bn3);
    EXPECT_EQ(te4.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te4.incidentNode1()),&tn3);
    EXPECT_EQ(&(te4.incidentNode2()),&tn5);

    EXPECT_EQ(&(te5.blockNode()),&tn6);
    EXPECT_EQ(&(te5.cutVertexNode()),&tn1);
    EXPECT_EQ(&(te5.cutVertexInBlock()),&block6_bn2);
    EXPECT_EQ(te5.isCutVertexOnNode1(),true);
    EXPECT_EQ(&(te5.incidentNode1()),&tn1);
    EXPECT_EQ(&(te5.incidentNode2()),&tn6);

}


/**  @brief DFS for a mid-sized graph from an ordinary node.
 */
TEST_F(BCTreeTests, Realistic1) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;

    // Nodes: 1 6 2 7 11 12 3 8 13 16 18 9 17 22 19 15 23 25 14 20 24 
    auto  np1 = std::make_unique<NumNode>(1);
    auto& n1  = g1.addNode(std::move(np1));
    auto  np6 = std::make_unique<NumNode>(6);
    auto& n6  = g1.addNode(std::move(np6));
    auto  np2 = std::make_unique<NumNode>(2);
    auto& n2  = g1.addNode(std::move(np2));
    auto  np7 = std::make_unique<NumNode>(7);
    auto& n7  = g1.addNode(std::move(np7));
    auto  np11 = std::make_unique<NumNode>(11);
    auto& n11  = g1.addNode(std::move(np11));
    auto  np12 = std::make_unique<NumNode>(12);
    auto& n12  = g1.addNode(std::move(np12));
    auto  np3 = std::make_unique<NumNode>(3);
    auto& n3  = g1.addNode(std::move(np3));
    auto  np8 = std::make_unique<NumNode>(8);
    auto& n8  = g1.addNode(std::move(np8));
    auto  np13 = std::make_unique<NumNode>(13);
    auto& n13  = g1.addNode(std::move(np13));
    auto  np16 = std::make_unique<NumNode>(16);
    auto& n16  = g1.addNode(std::move(np16));
    auto  np18 = std::make_unique<NumNode>(18);
    auto& n18  = g1.addNode(std::move(np18));
    auto  np9 = std::make_unique<NumNode>(9);
    auto& n9  = g1.addNode(std::move(np9));
    auto  np17 = std::make_unique<NumNode>(17);
    auto& n17  = g1.addNode(std::move(np17));
    auto  np22 = std::make_unique<NumNode>(22);
    auto& n22  = g1.addNode(std::move(np22));
    auto  np19 = std::make_unique<NumNode>(19);
    auto& n19  = g1.addNode(std::move(np19));
    auto  np15 = std::make_unique<NumNode>(15);
    auto& n15  = g1.addNode(std::move(np15));
    auto  np23 = std::make_unique<NumNode>(23);
    auto& n23  = g1.addNode(std::move(np23));
    auto  np25 = std::make_unique<NumNode>(25);
    auto& n25  = g1.addNode(std::move(np25));
    auto  np14 = std::make_unique<NumNode>(14);
    auto& n14  = g1.addNode(std::move(np14));
    auto  np20 = std::make_unique<NumNode>(20);
    auto& n20  = g1.addNode(std::move(np20));
    auto  np24 = std::make_unique<NumNode>(24);
    auto& n24  = g1.addNode(std::move(np24));

    // Edges: {1,6} {1,2} {1,7} {6,11} {6,7} {6,12} {2,7} {2,3} {2,8} {7,8} {7,13} {11,16} {12,18} {3,8} {8,13} {8,9} {16,17} {16,22} {18,19} {9,15} {17,22} {17,23} {22,23} {19,25} {14,15} {15,20} {23,24} {20,25} {24,25} 
    auto  ep1_6 = generate_edge();
    /*auto& e1_6  = */g1.addEdge(std::move(ep1_6),n1,n6);

    auto  ep1_2 = generate_edge();
    /*auto& e1_2  = */g1.addEdge(std::move(ep1_2),n1,n2);

    auto  ep1_7 = generate_edge();
    /*auto& e1_7  = */g1.addEdge(std::move(ep1_7),n1,n7);

    auto  ep6_11 = generate_edge();
    /*auto& e6_11  = */g1.addEdge(std::move(ep6_11),n6,n11);

    auto  ep6_7 = generate_edge();
    /*auto& e6_7  = */g1.addEdge(std::move(ep6_7),n6,n7);

    auto  ep6_12 = generate_edge();
    /*auto& e6_12  = */g1.addEdge(std::move(ep6_12),n6,n12);

    auto  ep2_7 = generate_edge();
    /*auto& e2_7  = */g1.addEdge(std::move(ep2_7),n2,n7);

    auto  ep2_3 = generate_edge();
    /*auto& e2_3  = */g1.addEdge(std::move(ep2_3),n2,n3);

    auto  ep2_8 = generate_edge();
    /*auto& e2_8  = */g1.addEdge(std::move(ep2_8),n2,n8);

    auto  ep7_8 = generate_edge();
    /*auto& e7_8  = */g1.addEdge(std::move(ep7_8),n7,n8);

    auto  ep7_13 = generate_edge();
    /*auto& e7_13  = */g1.addEdge(std::move(ep7_13),n7,n13);

    auto  ep11_16 = generate_edge();
    /*auto& e11_16 = */g1.addEdge(std::move(ep11_16),n11,n16);

    auto  ep12_18 = generate_edge();
    /*auto& e12_18 = */g1.addEdge(std::move(ep12_18),n12,n18);

    auto  ep3_8 = generate_edge();
    /*auto& e3_8 = */g1.addEdge(std::move(ep3_8),n3,n8);

    auto  ep8_13 = generate_edge();
    /*auto& e8_13 = */g1.addEdge(std::move(ep8_13),n8,n13);

    auto  ep8_9 = generate_edge();
    /*auto& e8_9 = */g1.addEdge(std::move(ep8_9),n8,n9);

    auto  ep16_17 = generate_edge();
    /*auto& e16_17 = */g1.addEdge(std::move(ep16_17),n16,n17);

    auto  ep16_22 = generate_edge();
    /*auto& e16_22 = */g1.addEdge(std::move(ep16_22),n16,n22);

    auto  ep18_19 = generate_edge();
    /*auto& e18_19 = */g1.addEdge(std::move(ep18_19),n18,n19);

    auto  ep9_15 = generate_edge();
    /*auto& e9_15 = */g1.addEdge(std::move(ep9_15),n9,n15);

    auto  ep17_22 = generate_edge();
    /*auto& e17_22 = */g1.addEdge(std::move(ep17_22),n17,n22);

    auto  ep17_23 = generate_edge();
    /*auto& e17_23 = */g1.addEdge(std::move(ep17_23),n17,n23);

    auto  ep22_23 = generate_edge();
    /*auto& e22_23 = */g1.addEdge(std::move(ep22_23),n22,n23);

    auto  ep19_25 = generate_edge();
    /*auto& e19_25 = */g1.addEdge(std::move(ep19_25),n19,n25);

    auto  ep14_15 = generate_edge();
    /*auto& e14_15 = */g1.addEdge(std::move(ep14_15),n14,n15);

    auto  ep15_20 = generate_edge();
    /*auto& e15_20 = */g1.addEdge(std::move(ep15_20),n15,n20);

    auto  ep23_24 = generate_edge();
    /*auto& e23_24 = */g1.addEdge(std::move(ep23_24),n23,n24);

    auto  ep20_25 = generate_edge();
    /*auto& e20_25 = */g1.addEdge(std::move(ep20_25),n20,n25);

    auto  ep24_25 = generate_edge();
    /*auto& e24_25 = */g1.addEdge(std::move(ep24_25),n24,n25);
    // Edges: {1,6} {1,2} {1,7} {6,11} {6,7} {6,12} {2,7} {2,3} {2,8} {7,8} {7,13} {11,16} {12,18} {3,8} {8,13} {8,9} {16,17} {16,22} {18,19} {9,15} {17,22} {17,23} {22,23} {19,25} {14,15} {15,20} {23,24} {20,25} {24,25} 

    BiconnectedDecomposer decomp;
    BCTree tree = decomp.decompose(g1,n1);
    EXPECT_EQ(tree.numNodes(),3);
    EXPECT_EQ(tree.numEdges(),2);

    auto treeNodeIt = tree.nodes().first;
    BCTreeNode& tn1 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn2 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;
    BCTreeNode& tn3 = dynamic_cast<BCTreeNode&>(*(*(treeNodeIt)));
    treeNodeIt++;

    auto treeEdgeIt = tree.edges().first;
    BCTreeEdge& te1 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));
    treeEdgeIt++;
    /*BCTreeEdge& te2 = dynamic_cast<BCTreeEdge&>(*(*(treeEdgeIt)));*/
    treeEdgeIt++;

    EXPECT_EQ(tn1.type(), BCTreeNode::CutVertexType);
    EXPECT_EQ(&(tn1.IGBackwardLinkRef()),&n15);
    EXPECT_EQ(n15.IGForwardLink(), tn1.backIt());
    EXPECT_EQ(n15.utility(), ConstantsBCTree::kForwardTypeCutVertex);

    EXPECT_EQ(tn2.type(), BCTreeNode::BlockType);
    Block& block2 = tn2.block();
    vector<node_list_it_t> b2_cvs = block2.cutVertices();
    EXPECT_EQ(b2_cvs.size(),1);
    BlockNode& b2_n1 = dynamic_cast<BlockNode&>(*(*(b2_cvs[0]))); 
    BCTreeNode& b2_tn = block2.BCTreeNode();
    EXPECT_EQ(&b2_tn, &tn2);
    EXPECT_EQ(block2.numNodes(),2);
    EXPECT_EQ(block2.numEdges(),1);
    auto block2_nit =  block2.nodes().first;
    BlockNode& block2_bn1 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn1.type(),BlockNode::OrdinaryNodeType);
    EXPECT_EQ(&(block2_bn1.IGBackwardLinkRef()),&n14);
    EXPECT_EQ(n14.IGForwardLink(), block2_bn1.backIt());
    EXPECT_EQ(n14.utility(), ConstantsBCTree::kForwardTypeBlockNode);
    block2_nit++;
    BlockNode& block2_bn2 = dynamic_cast<BlockNode&>(*(*block2_nit));
    EXPECT_EQ(block2_bn2.type(),BlockNode::CutVertexType);
    EXPECT_EQ(&(block2_bn2.IGBackwardLinkRef()),&n15);
    EXPECT_EQ(&(block2_bn2.treeEdge()),&te1);
    EXPECT_EQ(&b2_n1,&block2_bn2);
    block2_nit++;

    EXPECT_EQ(tn3.type(), BCTreeNode::BlockType);
    Block& block3 = tn3.block();
    vector<node_list_it_t> b3_cvs = block3.cutVertices();
    EXPECT_EQ(b3_cvs.size(),1);
    /*BlockNode& b3_n1 = dynamic_cast<BlockNode&>(*(*(b2_cvs[0])));*/
    BCTreeNode& b3_tn = block3.BCTreeNode();
    EXPECT_EQ(&b3_tn, &tn3);
    EXPECT_EQ(block3.numNodes(),20);
    EXPECT_EQ(block3.numEdges(),28);


}


} // namespace Undirected

} // namespace Wailea
