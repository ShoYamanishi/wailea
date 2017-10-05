#include "gtest/gtest.h"
#include "undirected/base.hpp"
#include "undirected/embedded_bctree.hpp"

namespace Wailea {

namespace Undirected { 


class EmbeddedBCTreeTests : public ::testing::Test {

  protected:  

    std::unique_ptr<Graph> generate_graph(){
        return std::make_unique<Graph>();
    }
 
    std::unique_ptr<Node> generate_node(long i){
        return std::make_unique<NumNode>(i);
    }

    std::unique_ptr<Edge> generate_edge(){
        return std::make_unique<Edge>();
    }

    EmbeddedBCTreeTests(){;};
    virtual ~EmbeddedBCTreeTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief
 */
TEST_F(EmbeddedBCTreeTests, EmptyGraph) {

    std::unique_ptr<Graph> gp1 = generate_graph();
    Graph&  g1 = *gp1;

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 0);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();

    ebcT.findGeometricEmbedding();

    EXPECT_THROW(ebcT.root(), std::invalid_argument);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),0);
    EXPECT_EQ(expT.numEdges(),0);
    EXPECT_THROW(expT.root(), std::invalid_argument);

}


/**  @brief
 */
TEST_F(EmbeddedBCTreeTests, SingleNodeGraph) {

    std::unique_ptr<Graph> gp1 = generate_graph();
    Graph&  g1 = *gp1;

    auto  np1 = generate_node(1);
    auto& n1  = g1.addNode(std::move(np1));

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 1);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();
    auto ebcNit = ebcT.nodes().first;
    auto& ebcN1 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN1 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    EXPECT_EQ(ebcN1.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN1.IGBackwardLinkRef(), &bcN1);

    EmbeddedGraph& ebcN1eg = ebcN1.embeddedGraph();
    DualGraph&     ebcN1dg = ebcN1.dualGraph();
    EXPECT_THROW(ebcN1.numUnificationGroups(),  std::invalid_argument);
    EXPECT_THROW(ebcN1.unificationGroup(0), std::invalid_argument);

    EXPECT_EQ(ebcN1eg.numNodes(), 1);
    EXPECT_EQ(ebcN1eg.numEdges(), 0);
    EXPECT_EQ(ebcN1dg.numNodes(), 1);
    EXPECT_EQ(ebcN1dg.numEdges(), 0);

    auto  ebcN1egNit = ebcN1eg.nodes().first;
    auto& ebcN1egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN1egNit)));

    auto  ebcN1dgFit = ebcN1dg.nodes().first;
    auto& ebcN1dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN1dgFit)));

    auto& ebcN1egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN1egN1.IGBackwardLink())));
    auto& ebcN1egN1GN = dynamic_cast<NumNode&>(ebcN1egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN1egN1GN.backIt(), n1.backIt());
    ebcT.findGeometricEmbedding();
    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN1);

    EXPECT_EQ(&ebcN1.topNode(),   &ebcN1egN1);
    EXPECT_EQ(&ebcN1.outerFace(), &ebcN1dgF1);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),1);
    EXPECT_EQ(expT.numEdges(),0);
    auto& expTR = expT.root();
    EXPECT_EQ(&(expTR.IGBackwardLinkRef()), &ebcN1);
    EXPECT_EQ(expTR.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>& expTRUGs = expTR.sortedUnificationGroups();
    EXPECT_EQ(expTRUGs.size(), 0);
}


/**  @brief
 */
TEST_F(EmbeddedBCTreeTests, SingleEdgeGraph) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node(1);
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node(2);
    auto& n2  = g1.addNode(std::move(np2));
    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 1);
    EXPECT_EQ(ebcT.numEdges(), 0);

    ebcT.makeDefaultEmbedding();
    auto ebcNit = ebcT.nodes().first;
    auto& ebcN1 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN1 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    EXPECT_EQ(ebcN1.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN1.IGBackwardLinkRef(), &bcN1);

    EmbeddedGraph& ebcN1eg = ebcN1.embeddedGraph();
    DualGraph&     ebcN1dg = ebcN1.dualGraph();
    EXPECT_THROW(ebcN1.numUnificationGroups(),  std::invalid_argument);
    EXPECT_THROW(ebcN1.unificationGroup(0), std::invalid_argument);

    EXPECT_EQ(ebcN1eg.numNodes(), 2);
    EXPECT_EQ(ebcN1eg.numEdges(), 1);
    EXPECT_EQ(ebcN1dg.numNodes(), 1);
    EXPECT_EQ(ebcN1dg.numEdges(), 1);

    auto  ebcN1egNit = ebcN1eg.nodes().first;
    auto& ebcN1egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN1egNit)));
    auto& ebcN1egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN1egN1.IGBackwardLink())));
    auto& ebcN1egN1GN = dynamic_cast<NumNode&>(ebcN1egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN1egN1GN.backIt(), n1.backIt());
    ebcN1egNit++;
    auto& ebcN1egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN1egNit)));
    auto& ebcN1egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN1egN2.IGBackwardLink())));
    auto& ebcN1egN2GN = dynamic_cast<NumNode&>(ebcN1egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN1egN2GN.backIt(), n2.backIt());

    auto  ebcN1egEit = ebcN1eg.edges().first;
    auto& ebcN1egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN1egEit)));
    auto& ebcN1egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN1egE1.IGBackwardLink())));
    auto& ebcN1egE1GE = dynamic_cast<Edge&>(ebcN1egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN1egE1GE.backIt(), e1.backIt());

    auto  ebcN1dgFit = ebcN1dg.nodes().first;
    auto& ebcN1dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN1dgFit)));

    auto  ebcN1dgEit = ebcN1dg.edges().first;
    auto& ebcN1dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN1dgEit)));
    EXPECT_EQ(ebcN1dgE1.incidentNode1().backIt(), ebcN1dgF1.backIt());
    EXPECT_EQ(ebcN1dgE1.incidentNode2().backIt(), ebcN1dgF1.backIt());

    ebcT.findGeometricEmbedding();

    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN1);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),1);
    EXPECT_EQ(expT.numEdges(),0);
    auto& expTR = expT.root();
    EXPECT_EQ(&(expTR.IGBackwardLinkRef()), &ebcN1);
    EXPECT_EQ(expTR.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>& expTRUGs = expTR.sortedUnificationGroups();
    EXPECT_EQ(expTRUGs.size(), 0);

}


/**  @brief Two serial edges
 */
TEST_F(EmbeddedBCTreeTests, TwoSerialEdgesGraph) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node(1);
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node(2);
    auto& n2  = g1.addNode(std::move(np2));
    auto  np3 = generate_node(3);
    auto& n3  = g1.addNode(std::move(np3));
    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);
    auto  ep2 = generate_edge();
    auto& e2  = g1.addEdge(std::move(ep2),n2,n3);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 3);
    EXPECT_EQ(ebcT.numEdges(), 2);

    ebcT.makeDefaultEmbedding();
    auto ebcNit = ebcT.nodes().first;
    auto& ebcN1 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN2 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN3 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN1 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN2 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN3 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    auto ebcEit = ebcT.edges().first;
    auto& ebcE1 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE2 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));

    auto bcEit = bcT.edges().first;
    auto& bcE1 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE2 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));

    EXPECT_EQ(ebcN1.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN1.IGBackwardLinkRef(), &bcN1);
    EXPECT_THROW(ebcN1.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN1.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN1.numUnificationGroups(), 1);
    auto& ebcN1UG1 = ebcN1.unificationGroup(0);

    EXPECT_EQ(ebcN2.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN2.IGBackwardLinkRef(), &bcN2);
    auto& ebcN2eg = ebcN2.embeddedGraph();
    auto& ebcN2dg = ebcN2.dualGraph();
    EXPECT_EQ(ebcN2eg.numNodes(), 2);
    EXPECT_EQ(ebcN2eg.numEdges(), 1);
    EXPECT_EQ(ebcN2dg.numNodes(), 1);
    EXPECT_EQ(ebcN2dg.numEdges(), 1);

    auto  ebcN2egNit = ebcN2eg.nodes().first;
    auto& ebcN2egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN2egNit)));
    auto& ebcN2egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN2egN1.IGBackwardLink())));
    auto& ebcN2egN1GN = dynamic_cast<NumNode&>(ebcN2egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN2egN1GN.backIt(), n2.backIt());
    ebcN2egNit++;
    auto& ebcN2egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN2egNit)));
    auto& ebcN2egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN2egN2.IGBackwardLink())));
    auto& ebcN2egN2GN = dynamic_cast<NumNode&>(ebcN2egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN2egN2GN.backIt(), n3.backIt());

    auto  ebcN2egEit = ebcN2eg.edges().first;
    auto& ebcN2egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN2egEit)));
    auto& ebcN2egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN2egE1.IGBackwardLink())));
    auto& ebcN2egE1GE = dynamic_cast<Edge&>(ebcN2egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN2egE1GE.backIt(), e2.backIt());

    auto  ebcN2dgFit = ebcN2dg.nodes().first;
    auto& ebcN2dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN2dgFit)));

    //auto  ebcN2dgEit = ebcN2dg.edges().first;
    //auto& ebcN2dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN2dgEit)));

    EXPECT_EQ(ebcN3.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN3.IGBackwardLinkRef(), &bcN3);
    auto& ebcN3eg = ebcN3.embeddedGraph();
    auto& ebcN3dg = ebcN3.dualGraph();
    EXPECT_EQ(ebcN3eg.numNodes(), 2);
    EXPECT_EQ(ebcN3eg.numEdges(), 1);
    EXPECT_EQ(ebcN3dg.numNodes(), 1);
    EXPECT_EQ(ebcN3dg.numEdges(), 1);

    auto  ebcN3egNit = ebcN3eg.nodes().first;
    auto& ebcN3egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN3egNit)));
    auto& ebcN3egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN3egN1.IGBackwardLink())));
    auto& ebcN3egN1GN = dynamic_cast<NumNode&>(ebcN3egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN3egN1GN.backIt(), n1.backIt());
    ebcN3egNit++;
    auto& ebcN3egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN3egNit)));
    auto& ebcN3egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN3egN2.IGBackwardLink())));
    auto& ebcN3egN2GN = dynamic_cast<NumNode&>(ebcN3egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN3egN2GN.backIt(), n2.backIt());

    auto  ebcN3egEit = ebcN3eg.edges().first;
    auto& ebcN3egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN3egEit)));
    auto& ebcN3egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN3egE1.IGBackwardLink())));
    auto& ebcN3egE1GE = dynamic_cast<Edge&>(ebcN3egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN3egE1GE.backIt(), e1.backIt());

    auto  ebcN3dgFit = ebcN3dg.nodes().first;
    auto& ebcN3dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN3dgFit)));

    //auto  ebcN3dgEit = ebcN3dg.edges().first;
    //auto& ebcN3dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN3dgEit)));

    EXPECT_EQ(&ebcE1.IGBackwardLinkRef(), &bcE1);
    EXPECT_EQ(&ebcE1.incidentNodeBlockType(), &ebcN2);

    EXPECT_EQ(&ebcE2.IGBackwardLinkRef(), &bcE2);
    EXPECT_EQ(&ebcE2.incidentNodeBlockType(), &ebcN3);

    EXPECT_EQ(ebcN1UG1.size(), 2);
    UnificationFace& uf1 = ebcN1UG1.at(0);
    UnificationFace& uf2 = ebcN1UG1.at(1);

    EXPECT_EQ(&uf1.treeNode(), &ebcN2);
    EXPECT_EQ(&uf1.faceInDG(), &ebcN2dgF1);
    EXPECT_EQ(uf1.faceInDGIt(), ebcN2dgF1.backIt());

    EXPECT_EQ(&uf1.cutVertexInEG(), &ebcN2egN1);
    EXPECT_EQ(uf1.cutVertexInEGIt(), ebcN2egN1.backIt());

    EXPECT_EQ(&uf1.edgeCCWInEG(), &ebcN2egE1);
    EXPECT_EQ(uf1.edgeCCWInEGIt(), ebcN2egE1.backIt());

    EXPECT_EQ(&uf1.edgeCWInEG(), &ebcN2egE1);
    EXPECT_EQ(uf1.edgeCWInEGIt(), ebcN2egE1.backIt());

    EXPECT_EQ(uf1.roleOfECWReversed(), false);

    EXPECT_EQ(&uf2.treeNode(), &ebcN3);
    EXPECT_EQ(&uf2.faceInDG(), &ebcN3dgF1);
    EXPECT_EQ(uf2.faceInDGIt(), ebcN3dgF1.backIt());

    EXPECT_EQ(&uf2.cutVertexInEG(), &ebcN3egN2);
    EXPECT_EQ(uf2.cutVertexInEGIt(), ebcN3egN2.backIt());

    EXPECT_EQ(&uf2.edgeCCWInEG(), &ebcN3egE1);
    EXPECT_EQ(uf2.edgeCCWInEGIt(), ebcN3egE1.backIt());

    EXPECT_EQ(&uf2.edgeCWInEG(), &ebcN3egE1);
    EXPECT_EQ(uf2.edgeCWInEGIt(), ebcN3egE1.backIt());

    EXPECT_EQ(uf2.roleOfECWReversed(), false);

    ebcT.findGeometricEmbedding();

    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN2);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();
    EXPECT_EQ(expT.numNodes(),2);
    EXPECT_EQ(expT.numEdges(),1);

    auto& expTR = expT.root();

    auto expNit = expT.nodes().first;
    auto& expN1 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN2 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));

    EXPECT_EQ(&expTR, &expN1);

    EXPECT_EQ(&expN1.IGBackwardLinkRef(), &ebcN2);
    EXPECT_EQ(expN1.numChildrenProcessed(), 0);

    vector<SortedUnificationGroup>& expN1UGs = expN1.sortedUnificationGroups();

    EXPECT_EQ(expN1UGs.size(), 1);
    EXPECT_EQ(expN1UGs[0].absorbeesSize(), 1);

    EXPECT_EQ(expN1UGs[0].absorber(),     uf1);
    EXPECT_EQ(expN1UGs[0].absorbeeAt(0),  uf2);


    EXPECT_EQ(&expN2.IGBackwardLinkRef(), &ebcN3);
    EXPECT_EQ(expN2.numChildrenProcessed(), 0);

    vector<SortedUnificationGroup>& expN2UGs = expN2.sortedUnificationGroups();
    EXPECT_EQ(expN2UGs.size(), 0);


    auto expEit = expT.edges().first;
    auto& expE1 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(&expE1.incidentNodeSrc(), &expN1);
    EXPECT_EQ(&expE1.incidentNodeDst(), &expN2);

    EXPECT_EQ(&ebcN2.outerFace(), &ebcN2dgF1);
    EXPECT_EQ(&ebcN2.topNode(),   &ebcN2egN2);

    EXPECT_EQ(&ebcN3.outerFace(), &ebcN3dgF1);
    EXPECT_EQ(&ebcN3.topNode(),   &ebcN3egN2);

}


/**  @brief single cut vertex, multiple k2s.
 */
TEST_F(EmbeddedBCTreeTests, FourEdgesSingleCutVertex) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;
    auto  np1 = generate_node(1);
    auto& n1  = g1.addNode(std::move(np1));
    auto  np2 = generate_node(2);
    auto& n2  = g1.addNode(std::move(np2));
    auto  np3 = generate_node(3);
    auto& n3  = g1.addNode(std::move(np3));
    auto  np4 = generate_node(4);
    auto& n4  = g1.addNode(std::move(np4));
    auto  np5 = generate_node(5);
    auto& n5  = g1.addNode(std::move(np5));

    auto  ep1 = generate_edge();
    auto& e1  = g1.addEdge(std::move(ep1),n1,n2);
    auto  ep2 = generate_edge();
    auto& e2  = g1.addEdge(std::move(ep2),n1,n3);
    auto  ep3 = generate_edge();
    auto& e3  = g1.addEdge(std::move(ep3),n1,n4);
    auto  ep4 = generate_edge();
    auto& e4  = g1.addEdge(std::move(ep4),n1,n5);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 5);
    EXPECT_EQ(ebcT.numEdges(), 4);

    ebcT.makeDefaultEmbedding();
    auto ebcNit = ebcT.nodes().first;
    auto& ebcN1 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN2 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN3 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN4 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN5 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN1 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN2 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN3 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN4 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN5 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    auto ebcEit = ebcT.edges().first;
    /*auto& ebcE1 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));*/
    ebcEit++;
    /*auto& ebcE2 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));*/
    ebcEit++;
    /*auto& ebcE3= dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));*/
    ebcEit++;
    /*auto& ebcE4= dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));*/

    auto bcEit = bcT.edges().first;
    /*auto& bcE1 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));*/
    bcEit++;
    /*auto& bcE2 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));*/
    bcEit++;
    /*auto& bcE3 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));*/
    bcEit++;
    /*auto& bcE4 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));*/

    EXPECT_EQ(ebcN1.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN1.IGBackwardLinkRef(), &bcN1);
    EXPECT_THROW(ebcN1.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN1.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN1.numUnificationGroups(), 1);
    auto& ebcN1UG1 = ebcN1.unificationGroup(0);

    EXPECT_EQ(ebcN2.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN2.IGBackwardLinkRef(), &bcN2);
    auto& ebcN2eg = ebcN2.embeddedGraph();
    auto& ebcN2dg = ebcN2.dualGraph();
    EXPECT_EQ(ebcN2eg.numNodes(), 2);
    EXPECT_EQ(ebcN2eg.numEdges(), 1);
    EXPECT_EQ(ebcN2dg.numNodes(), 1);
    EXPECT_EQ(ebcN2dg.numEdges(), 1);

    auto  ebcN2egNit = ebcN2eg.nodes().first;
    auto& ebcN2egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN2egNit)));
    auto& ebcN2egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN2egN1.IGBackwardLink())));
    auto& ebcN2egN1GN = dynamic_cast<NumNode&>(ebcN2egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN2egN1GN.backIt(), n1.backIt());
    ebcN2egNit++;
    auto& ebcN2egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN2egNit)));
    auto& ebcN2egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN2egN2.IGBackwardLink())));
    auto& ebcN2egN2GN = dynamic_cast<NumNode&>(ebcN2egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN2egN2GN.backIt(), n2.backIt());

    auto  ebcN2egEit = ebcN2eg.edges().first;
    auto& ebcN2egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN2egEit)));
    auto& ebcN2egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN2egE1.IGBackwardLink())));
    auto& ebcN2egE1GE = dynamic_cast<Edge&>(ebcN2egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN2egE1GE.backIt(), e1.backIt());

    auto  ebcN2dgFit = ebcN2dg.nodes().first;
    auto& ebcN2dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN2dgFit)));

    //auto  ebcN2dgEit = ebcN2dg.edges().first;
    //auto& ebcN2dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN2dgEit)));

    EXPECT_EQ(ebcN3.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN3.IGBackwardLinkRef(), &bcN3);
    auto& ebcN3eg = ebcN3.embeddedGraph();
    auto& ebcN3dg = ebcN3.dualGraph();
    EXPECT_EQ(ebcN3eg.numNodes(), 2);
    EXPECT_EQ(ebcN3eg.numEdges(), 1);
    EXPECT_EQ(ebcN3dg.numNodes(), 1);
    EXPECT_EQ(ebcN3dg.numEdges(), 1);

    auto  ebcN3egNit = ebcN3eg.nodes().first;
    auto& ebcN3egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN3egNit)));
    auto& ebcN3egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN3egN1.IGBackwardLink())));
    auto& ebcN3egN1GN = dynamic_cast<NumNode&>(ebcN3egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN3egN1GN.backIt(), n1.backIt());
    ebcN3egNit++;
    auto& ebcN3egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN3egNit)));
    auto& ebcN3egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN3egN2.IGBackwardLink())));
    auto& ebcN3egN2GN = dynamic_cast<NumNode&>(ebcN3egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN3egN2GN.backIt(), n3.backIt());

    auto  ebcN3egEit = ebcN3eg.edges().first;
    auto& ebcN3egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN3egEit)));
    auto& ebcN3egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN3egE1.IGBackwardLink())));
    auto& ebcN3egE1GE = dynamic_cast<Edge&>(ebcN3egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN3egE1GE.backIt(), e2.backIt());

    auto  ebcN3dgFit = ebcN3dg.nodes().first;
    auto& ebcN3dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN3dgFit)));

    //auto  ebcN3dgEit = ebcN3dg.edges().first;
    //auto& ebcN3dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN3dgEit)));

    EXPECT_EQ(ebcN4.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN4.IGBackwardLinkRef(), &bcN4);
    auto& ebcN4eg = ebcN4.embeddedGraph();
    auto& ebcN4dg = ebcN4.dualGraph();
    EXPECT_EQ(ebcN4eg.numNodes(), 2);
    EXPECT_EQ(ebcN4eg.numEdges(), 1);
    EXPECT_EQ(ebcN4dg.numNodes(), 1);
    EXPECT_EQ(ebcN4dg.numEdges(), 1);

    auto  ebcN4egNit = ebcN4eg.nodes().first;
    auto& ebcN4egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN4egNit)));
    auto& ebcN4egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN4egN1.IGBackwardLink())));
    auto& ebcN4egN1GN = dynamic_cast<NumNode&>(ebcN4egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN4egN1GN.backIt(), n1.backIt());
    ebcN4egNit++;
    auto& ebcN4egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN4egNit)));
    auto& ebcN4egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN4egN2.IGBackwardLink())));
    auto& ebcN4egN2GN = dynamic_cast<NumNode&>(ebcN4egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN4egN2GN.backIt(), n4.backIt());

    auto  ebcN4egEit = ebcN4eg.edges().first;
    auto& ebcN4egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN4egEit)));
    auto& ebcN4egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN4egE1.IGBackwardLink())));
    auto& ebcN4egE1GE = dynamic_cast<Edge&>(ebcN4egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN4egE1GE.backIt(), e3.backIt());

    auto  ebcN4dgFit = ebcN4dg.nodes().first;
    auto& ebcN4dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN4dgFit)));

    //auto  ebcN4dgEit = ebcN4dg.edges().first;
    //auto& ebcN4dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN4dgEit)));

    EXPECT_EQ(ebcN5.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN5.IGBackwardLinkRef(), &bcN5);
    auto& ebcN5eg = ebcN5.embeddedGraph();
    auto& ebcN5dg = ebcN5.dualGraph();
    EXPECT_EQ(ebcN5eg.numNodes(), 2);
    EXPECT_EQ(ebcN5eg.numEdges(), 1);
    EXPECT_EQ(ebcN5dg.numNodes(), 1);
    EXPECT_EQ(ebcN5dg.numEdges(), 1);

    auto  ebcN5egNit = ebcN5eg.nodes().first;
    auto& ebcN5egN1 = dynamic_cast<EmbeddedNode&>(*(*(ebcN5egNit)));
    auto& ebcN5egN1BN = dynamic_cast<BlockNode&>(*(*(ebcN5egN1.IGBackwardLink())));
    auto& ebcN5egN1GN = dynamic_cast<NumNode&>(ebcN5egN1BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN5egN1GN.backIt(), n1.backIt());
    ebcN5egNit++;
    auto& ebcN5egN2 = dynamic_cast<EmbeddedNode&>(*(*(ebcN5egNit)));
    auto& ebcN5egN2BN = dynamic_cast<BlockNode&>(*(*(ebcN5egN2.IGBackwardLink())));
    auto& ebcN5egN2GN = dynamic_cast<NumNode&>(ebcN5egN2BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN5egN2GN.backIt(), n5.backIt());

    auto  ebcN5egEit = ebcN5eg.edges().first;
    auto& ebcN5egE1 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN5egEit)));
    auto& ebcN5egE1BE = dynamic_cast<BlockEdge&>(*(*(ebcN5egE1.IGBackwardLink())));
    auto& ebcN5egE1GE = dynamic_cast<Edge&>(ebcN5egE1BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN5egE1GE.backIt(), e4.backIt());

    auto  ebcN5dgFit = ebcN5dg.nodes().first;
    auto& ebcN5dgF1 = dynamic_cast<EmbeddedFace&>(*(*(ebcN5dgFit)));

    //auto  ebcN5dgEit = ebcN5dg.edges().first;
    //auto& ebcN5dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN5dgEit)));

    EXPECT_EQ(ebcN1UG1.size(), 4);
    UnificationFace& uf1 = ebcN1UG1.at(0);
    UnificationFace& uf2 = ebcN1UG1.at(1);
    UnificationFace& uf3 = ebcN1UG1.at(2);
    UnificationFace& uf4 = ebcN1UG1.at(3);

    EXPECT_EQ(&uf1.treeNode(), &ebcN2);
    EXPECT_EQ(&uf1.faceInDG(), &ebcN2dgF1);
    EXPECT_EQ(uf1.faceInDGIt(), ebcN2dgF1.backIt());

    EXPECT_EQ(&uf1.cutVertexInEG(), &ebcN2egN1);
    EXPECT_EQ(uf1.cutVertexInEGIt(), ebcN2egN1.backIt());

    EXPECT_EQ(&uf1.edgeCCWInEG(), &ebcN2egE1);
    EXPECT_EQ(uf1.edgeCCWInEGIt(), ebcN2egE1.backIt());

    EXPECT_EQ(&uf1.edgeCWInEG(), &ebcN2egE1);
    EXPECT_EQ(uf1.edgeCWInEGIt(), ebcN2egE1.backIt());

    EXPECT_EQ(uf1.roleOfECWReversed(), false);

    EXPECT_EQ(&uf2.treeNode(), &ebcN3);
    EXPECT_EQ(&uf2.faceInDG(), &ebcN3dgF1);
    EXPECT_EQ(uf2.faceInDGIt(), ebcN3dgF1.backIt());

    EXPECT_EQ(&uf2.cutVertexInEG(), &ebcN3egN1);
    EXPECT_EQ(uf2.cutVertexInEGIt(), ebcN3egN1.backIt());

    EXPECT_EQ(&uf2.edgeCCWInEG(), &ebcN3egE1);
    EXPECT_EQ(uf2.edgeCCWInEGIt(), ebcN3egE1.backIt());

    EXPECT_EQ(&uf2.edgeCWInEG(), &ebcN3egE1);
    EXPECT_EQ(uf2.edgeCWInEGIt(), ebcN3egE1.backIt());

    EXPECT_EQ(uf2.roleOfECWReversed(), false);

    EXPECT_EQ(&uf3.treeNode(), &ebcN4);
    EXPECT_EQ(&uf3.faceInDG(), &ebcN4dgF1);
    EXPECT_EQ(uf3.faceInDGIt(), ebcN4dgF1.backIt());

    EXPECT_EQ(&uf3.cutVertexInEG(), &ebcN4egN1);
    EXPECT_EQ(uf3.cutVertexInEGIt(), ebcN4egN1.backIt());

    EXPECT_EQ(&uf3.edgeCCWInEG(), &ebcN4egE1);
    EXPECT_EQ(uf3.edgeCCWInEGIt(), ebcN4egE1.backIt());

    EXPECT_EQ(&uf3.edgeCWInEG(), &ebcN4egE1);
    EXPECT_EQ(uf3.edgeCWInEGIt(), ebcN4egE1.backIt());

    EXPECT_EQ(uf3.roleOfECWReversed(), false);

    EXPECT_EQ(&uf4.treeNode(), &ebcN5);
    EXPECT_EQ(&uf4.faceInDG(), &ebcN5dgF1);
    EXPECT_EQ(uf4.faceInDGIt(), ebcN5dgF1.backIt());

    EXPECT_EQ(&uf4.cutVertexInEG(), &ebcN5egN1);
    EXPECT_EQ(uf4.cutVertexInEGIt(), ebcN5egN1.backIt());

    EXPECT_EQ(&uf4.edgeCCWInEG(), &ebcN5egE1);
    EXPECT_EQ(uf4.edgeCCWInEGIt(), ebcN5egE1.backIt());

    EXPECT_EQ(&uf4.edgeCWInEG(), &ebcN5egE1);
    EXPECT_EQ(uf4.edgeCWInEGIt(), ebcN5egE1.backIt());

    EXPECT_EQ(uf4.roleOfECWReversed(), false);


    ebcT.findGeometricEmbedding();

    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN2);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();

    auto& expTR = expT.root();

    EXPECT_EQ(&ebcTR, &ebcN2);

    EXPECT_EQ(expT.numNodes(),4);
    EXPECT_EQ(expT.numEdges(),3);

    auto expNit = expT.nodes().first;
    auto& expN1 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN2 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN3 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN4 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));

    EXPECT_EQ(&expTR, &expN1);

    auto expEit = expT.edges().first;
    /*auto& expE1 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));*/
    expEit++;
    /*auto& expE2 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));*/
    expEit++;
    /*auto& expE3 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));*/

    EXPECT_EQ(&expN1.IGBackwardLinkRef(), &ebcN2);
    EXPECT_EQ(expN1.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>& expN1UGs = expN1.sortedUnificationGroups();
    EXPECT_EQ(expN1UGs.size(), 1);
    EXPECT_EQ(expN1UGs[0].absorbeesSize(), 3);

    EXPECT_EQ(expN1UGs[0].absorber(),     uf1);
    EXPECT_EQ(expN1UGs[0].absorbeeAt(0),  uf2);
    EXPECT_EQ(expN1UGs[0].absorbeeAt(1),  uf3);
    EXPECT_EQ(expN1UGs[0].absorbeeAt(2),  uf4);

    EXPECT_EQ(&expN2.IGBackwardLinkRef(), &ebcN3);
    EXPECT_EQ(expN2.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>& expN2UGs = expN2.sortedUnificationGroups();
    EXPECT_EQ(expN2UGs.size(), 0);

    EXPECT_EQ(&expN3.IGBackwardLinkRef(), &ebcN4);
    EXPECT_EQ(expN3.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>& expN3UGs = expN3.sortedUnificationGroups();
    EXPECT_EQ(expN3UGs.size(), 0);

    EXPECT_EQ(&expN4.IGBackwardLinkRef(), &ebcN5);
    EXPECT_EQ(expN4.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>& expN4UGs = expN4.sortedUnificationGroups();
    EXPECT_EQ(expN4UGs.size(), 0);

    EXPECT_EQ(&ebcN2.outerFace(), &ebcN2dgF1);
    EXPECT_EQ(&ebcN2.topNode(),   &ebcN2egN2);

    EXPECT_EQ(&ebcN3.outerFace(), &ebcN3dgF1);
    EXPECT_EQ(&ebcN3.topNode(),   &ebcN3egN1);

    EXPECT_EQ(&ebcN4.outerFace(), &ebcN4dgF1);
    EXPECT_EQ(&ebcN4.topNode(),   &ebcN4egN1);

    EXPECT_EQ(&ebcN5.outerFace(), &ebcN5dgF1);
    EXPECT_EQ(&ebcN5.topNode(),   &ebcN5egN1);

}



/**  @brief
 */
TEST_F(EmbeddedBCTreeTests, Tree1) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;

    auto  np01 = generate_node(1);
    auto& n01  = g1.addNode(std::move(np01));
    auto  np02 = generate_node(2);
    auto& n02  = g1.addNode(std::move(np02));
    auto  np03 = generate_node(3);
    auto& n03  = g1.addNode(std::move(np03));
    auto  np04 = generate_node(4);
    auto& n04  = g1.addNode(std::move(np04));
    auto  np05 = generate_node(5);
    auto& n05  = g1.addNode(std::move(np05));
    auto  np06 = generate_node(6);
    auto& n06  = g1.addNode(std::move(np06));
    auto  np07 = generate_node(7);
    auto& n07  = g1.addNode(std::move(np07));
    auto  np08 = generate_node(8);
    auto& n08  = g1.addNode(std::move(np08));
    auto  np09 = generate_node(9);
    auto& n09  = g1.addNode(std::move(np09));
    auto  np10 = generate_node(10);
    auto& n10  = g1.addNode(std::move(np10));
    auto  np11 = generate_node(11);
    auto& n11  = g1.addNode(std::move(np11));
    auto  np12 = generate_node(12);
    auto& n12  = g1.addNode(std::move(np12));

    auto  ep01_02 = generate_edge();
    auto& e01_02  = g1.addEdge(std::move(ep01_02),n01,n02);
    auto  ep01_03 = generate_edge();
    auto& e01_03  = g1.addEdge(std::move(ep01_03),n01,n03);
    auto  ep01_04 = generate_edge();
    auto& e01_04  = g1.addEdge(std::move(ep01_04),n01,n04);
    auto  ep02_05 = generate_edge();
    auto& e02_05  = g1.addEdge(std::move(ep02_05),n02,n05);
    auto  ep02_06 = generate_edge();
    auto& e02_06  = g1.addEdge(std::move(ep02_06),n02,n06);
    auto  ep02_07 = generate_edge();
    auto& e02_07  = g1.addEdge(std::move(ep02_07),n02,n07);
    auto  ep03_08 = generate_edge();
    auto& e03_08  = g1.addEdge(std::move(ep03_08),n03,n08);
    auto  ep03_09 = generate_edge();
    auto& e03_09  = g1.addEdge(std::move(ep03_09),n03,n09);
    auto  ep03_10 = generate_edge();
    auto& e03_10  = g1.addEdge(std::move(ep03_10),n03,n10);
    auto  ep09_11 = generate_edge();
    auto& e09_11  = g1.addEdge(std::move(ep09_11),n09,n11);
    auto  ep09_12 = generate_edge();
    auto& e09_12  = g1.addEdge(std::move(ep09_12),n09,n12);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 15);
    EXPECT_EQ(ebcT.numEdges(), 14);

    ebcT.makeDefaultEmbedding();

    auto ebcNit = ebcT.nodes().first;
    auto& ebcN01 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN02 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN03 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN04 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN05 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN06 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN07 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN08 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN09 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN10 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN11 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN12 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN13 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN14 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN15 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN01 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN02 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN03 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN04 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN05 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN06 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN07 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN08 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN09 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN10 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN11 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN12 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN13 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN14 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN15 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    auto ebcEit = ebcT.edges().first;
    auto& ebcE01 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE02 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE03 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE04 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE05 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE06 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE07 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE08 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE09 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE10 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE11 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE12 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE13 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE14 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));

    auto bcEit = bcT.edges().first;
    auto& bcE01 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE02 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE03 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE04 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE05 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE06 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE07 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE08 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE09 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE10 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE11 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE12 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE13 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE14 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));



    EXPECT_EQ(ebcN01.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN01.IGBackwardLinkRef(), &bcN01);
    EXPECT_EQ(&bcN01.IGBackwardLinkRef(), &n02);
    EXPECT_THROW(ebcN01.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN01.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN01.numUnificationGroups(), 1);
    auto& ebcN01UG1 = ebcN01.unificationGroup(0);



    EXPECT_EQ(ebcN05.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN05.IGBackwardLinkRef(), &bcN05);
    EXPECT_EQ(&bcN05.IGBackwardLinkRef(), &n01);
    EXPECT_THROW(ebcN05.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN05.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN05.numUnificationGroups(), 1);
    auto& ebcN05UG1 = ebcN05.unificationGroup(0);



    EXPECT_EQ(ebcN07.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN07.IGBackwardLinkRef(), &bcN07);
    EXPECT_EQ(&bcN07.IGBackwardLinkRef(), &n03);
    EXPECT_THROW(ebcN07.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN07.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN07.numUnificationGroups(), 1);
    auto& ebcN07UG1 = ebcN07.unificationGroup(0);



    EXPECT_EQ(ebcN09.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN09.IGBackwardLinkRef(), &bcN09);
    EXPECT_EQ(&bcN09.IGBackwardLinkRef(), &n09);
    EXPECT_THROW(ebcN09.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN09.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN09.numUnificationGroups(), 1);
    auto& ebcN09UG1 = ebcN09.unificationGroup(0);



    EXPECT_EQ(ebcN02.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN02.IGBackwardLinkRef(), &bcN02);
    auto& ebcN02eg = ebcN02.embeddedGraph();
    auto& ebcN02dg = ebcN02.dualGraph();
    EXPECT_EQ(ebcN02eg.numNodes(), 2);
    EXPECT_EQ(ebcN02eg.numEdges(), 1);
    EXPECT_EQ(ebcN02dg.numNodes(), 1);
    EXPECT_EQ(ebcN02dg.numEdges(), 1);

    auto  ebcN02egNit = ebcN02eg.nodes().first;
    auto& ebcN02egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN01.IGBackwardLink())));
    auto& ebcN02egN01GN = dynamic_cast<NumNode&>(ebcN02egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN01GN.backIt(), n02.backIt());
    ebcN02egNit++;
    auto& ebcN02egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN02.IGBackwardLink())));
    auto& ebcN02egN02GN = dynamic_cast<NumNode&>(ebcN02egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN02GN.backIt(), n05.backIt());

    auto  ebcN02egEit = ebcN02eg.edges().first;
    auto& ebcN02egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE01.IGBackwardLink())));
    auto& ebcN02egE01GE = dynamic_cast<Edge&>(ebcN02egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE01GE.backIt(), e02_05.backIt());

    auto  ebcN02dgFit = ebcN02dg.nodes().first;
    auto& ebcN02dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN02dgFit)));

    //auto  ebcN02dgEit = ebcN02dg.edges().first;
    //auto& ebcN02dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN02dgEit)));



    EXPECT_EQ(ebcN03.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN03.IGBackwardLinkRef(), &bcN03);
    auto& ebcN03eg = ebcN03.embeddedGraph();
    auto& ebcN03dg = ebcN03.dualGraph();
    EXPECT_EQ(ebcN03eg.numNodes(), 2);
    EXPECT_EQ(ebcN03eg.numEdges(), 1);
    EXPECT_EQ(ebcN03dg.numNodes(), 1);
    EXPECT_EQ(ebcN03dg.numEdges(), 1);

    auto  ebcN03egNit = ebcN03eg.nodes().first;
    auto& ebcN03egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN01.IGBackwardLink())));
    auto& ebcN03egN01GN = dynamic_cast<NumNode&>(ebcN03egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN01GN.backIt(), n02.backIt());
    ebcN03egNit++;
    auto& ebcN03egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN02.IGBackwardLink())));
    auto& ebcN03egN02GN = dynamic_cast<NumNode&>(ebcN03egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN02GN.backIt(), n06.backIt());

    auto  ebcN03egEit = ebcN03eg.edges().first;
    auto& ebcN03egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE01.IGBackwardLink())));
    auto& ebcN03egE01GE = dynamic_cast<Edge&>(ebcN03egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE01GE.backIt(), e02_06.backIt());

    auto  ebcN03dgFit = ebcN03dg.nodes().first;
    auto& ebcN03dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN03dgFit)));

    //auto  ebcN03dgEit = ebcN03dg.edges().first;
    //auto& ebcN03dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN03dgEit)));



    EXPECT_EQ(ebcN04.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN04.IGBackwardLinkRef(), &bcN04);
    auto& ebcN04eg = ebcN04.embeddedGraph();
    auto& ebcN04dg = ebcN04.dualGraph();
    EXPECT_EQ(ebcN04eg.numNodes(), 2);
    EXPECT_EQ(ebcN04eg.numEdges(), 1);
    EXPECT_EQ(ebcN04dg.numNodes(), 1);
    EXPECT_EQ(ebcN04dg.numEdges(), 1);

    auto  ebcN04egNit = ebcN04eg.nodes().first;
    auto& ebcN04egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN01.IGBackwardLink())));
    auto& ebcN04egN01GN = dynamic_cast<NumNode&>(ebcN04egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN01GN.backIt(), n02.backIt());
    ebcN04egNit++;
    auto& ebcN04egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN02.IGBackwardLink())));
    auto& ebcN04egN02GN = dynamic_cast<NumNode&>(ebcN04egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN02GN.backIt(), n07.backIt());

    auto  ebcN04egEit = ebcN04eg.edges().first;
    auto& ebcN04egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE01.IGBackwardLink())));
    auto& ebcN04egE01GE = dynamic_cast<Edge&>(ebcN04egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE01GE.backIt(), e02_07.backIt());

    auto  ebcN04dgFit = ebcN04dg.nodes().first;
    auto& ebcN04dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN04dgFit)));

    //auto  ebcN04dgEit = ebcN04dg.edges().first;
    //auto& ebcN04dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN04dgEit)));



    EXPECT_EQ(ebcN06.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN06.IGBackwardLinkRef(), &bcN06);
    auto& ebcN06eg = ebcN06.embeddedGraph();
    auto& ebcN06dg = ebcN06.dualGraph();
    EXPECT_EQ(ebcN06eg.numNodes(), 2);
    EXPECT_EQ(ebcN06eg.numEdges(), 1);
    EXPECT_EQ(ebcN06dg.numNodes(), 1);
    EXPECT_EQ(ebcN06dg.numEdges(), 1);

    auto  ebcN06egNit = ebcN06eg.nodes().first;
    auto& ebcN06egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN01.IGBackwardLink())));
    auto& ebcN06egN01GN = dynamic_cast<NumNode&>(ebcN06egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN01GN.backIt(), n01.backIt());
    ebcN06egNit++;
    auto& ebcN06egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN02.IGBackwardLink())));
    auto& ebcN06egN02GN = dynamic_cast<NumNode&>(ebcN06egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN02GN.backIt(), n02.backIt());

    auto  ebcN06egEit = ebcN06eg.edges().first;
    auto& ebcN06egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN06egEit)));
    auto& ebcN06egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN06egE01.IGBackwardLink())));
    auto& ebcN06egE01GE = dynamic_cast<Edge&>(ebcN06egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egE01GE.backIt(), e01_02.backIt());

    auto  ebcN06dgFit = ebcN06dg.nodes().first;
    auto& ebcN06dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN06dgFit)));

    //auto  ebcN06dgEit = ebcN06dg.edges().first;
    //auto& ebcN06dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN06dgEit)));



    EXPECT_EQ(ebcN08.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN08.IGBackwardLinkRef(), &bcN08);
    auto& ebcN08eg = ebcN08.embeddedGraph();
    auto& ebcN08dg = ebcN08.dualGraph();
    EXPECT_EQ(ebcN08eg.numNodes(), 2);
    EXPECT_EQ(ebcN08eg.numEdges(), 1);
    EXPECT_EQ(ebcN08dg.numNodes(), 1);
    EXPECT_EQ(ebcN08dg.numEdges(), 1);

    auto  ebcN08egNit = ebcN08eg.nodes().first;
    auto& ebcN08egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN08egNit)));
    auto& ebcN08egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN08egN01.IGBackwardLink())));
    auto& ebcN08egN01GN = dynamic_cast<NumNode&>(ebcN08egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egN01GN.backIt(), n03.backIt());
    ebcN08egNit++;
    auto& ebcN08egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN08egNit)));
    auto& ebcN08egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN08egN02.IGBackwardLink())));
    auto& ebcN08egN02GN = dynamic_cast<NumNode&>(ebcN08egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egN02GN.backIt(), n08.backIt());

    auto  ebcN08egEit = ebcN08eg.edges().first;
    auto& ebcN08egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN08egEit)));
    auto& ebcN08egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN08egE01.IGBackwardLink())));
    auto& ebcN08egE01GE = dynamic_cast<Edge&>(ebcN08egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egE01GE.backIt(), e03_08.backIt());

    auto  ebcN08dgFit = ebcN08dg.nodes().first;
    auto& ebcN08dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN08dgFit)));

    //auto  ebcN08dgEit = ebcN08dg.edges().first;
    //auto& ebcN08dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN08dgEit)));



    EXPECT_EQ(ebcN10.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN10.IGBackwardLinkRef(), &bcN10);
    auto& ebcN10eg = ebcN10.embeddedGraph();
    auto& ebcN10dg = ebcN10.dualGraph();
    EXPECT_EQ(ebcN10eg.numNodes(), 2);
    EXPECT_EQ(ebcN10eg.numEdges(), 1);
    EXPECT_EQ(ebcN10dg.numNodes(), 1);
    EXPECT_EQ(ebcN10dg.numEdges(), 1);

    auto  ebcN10egNit = ebcN10eg.nodes().first;
    auto& ebcN10egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN10egNit)));
    auto& ebcN10egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN10egN01.IGBackwardLink())));
    auto& ebcN10egN01GN = dynamic_cast<NumNode&>(ebcN10egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN10egN01GN.backIt(), n09.backIt());
    ebcN10egNit++;
    auto& ebcN10egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN10egNit)));
    auto& ebcN10egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN10egN02.IGBackwardLink())));
    auto& ebcN10egN02GN = dynamic_cast<NumNode&>(ebcN10egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN10egN02GN.backIt(), n11.backIt());

    auto  ebcN10egEit = ebcN10eg.edges().first;
    auto& ebcN10egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN10egEit)));
    auto& ebcN10egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN10egE01.IGBackwardLink())));
    auto& ebcN10egE01GE = dynamic_cast<Edge&>(ebcN10egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN10egE01GE.backIt(), e09_11.backIt());

    auto  ebcN10dgFit = ebcN10dg.nodes().first;
    auto& ebcN10dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN10dgFit)));

    //auto  ebcN10dgEit = ebcN10dg.edges().first;
    //auto& ebcN10dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN10dgEit)));



    EXPECT_EQ(ebcN11.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN11.IGBackwardLinkRef(), &bcN11);
    auto& ebcN11eg = ebcN11.embeddedGraph();
    auto& ebcN11dg = ebcN11.dualGraph();
    EXPECT_EQ(ebcN11eg.numNodes(), 2);
    EXPECT_EQ(ebcN11eg.numEdges(), 1);
    EXPECT_EQ(ebcN11dg.numNodes(), 1);
    EXPECT_EQ(ebcN11dg.numEdges(), 1);

    auto  ebcN11egNit = ebcN11eg.nodes().first;
    auto& ebcN11egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN01.IGBackwardLink())));
    auto& ebcN11egN01GN = dynamic_cast<NumNode&>(ebcN11egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN01GN.backIt(), n09.backIt());
    ebcN11egNit++;
    auto& ebcN11egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN02.IGBackwardLink())));
    auto& ebcN11egN02GN = dynamic_cast<NumNode&>(ebcN11egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN02GN.backIt(), n12.backIt());

    auto  ebcN11egEit = ebcN11eg.edges().first;
    auto& ebcN11egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE01.IGBackwardLink())));
    auto& ebcN11egE01GE = dynamic_cast<Edge&>(ebcN11egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE01GE.backIt(), e09_12.backIt());

    auto  ebcN11dgFit = ebcN11dg.nodes().first;
    auto& ebcN11dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN11dgFit)));

    //auto  ebcN11dgEit = ebcN11dg.edges().first;
    //auto& ebcN11dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN11dgEit)));



    EXPECT_EQ(ebcN12.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN12.IGBackwardLinkRef(), &bcN12);
    auto& ebcN12eg = ebcN12.embeddedGraph();
    auto& ebcN12dg = ebcN12.dualGraph();
    EXPECT_EQ(ebcN12eg.numNodes(), 2);
    EXPECT_EQ(ebcN12eg.numEdges(), 1);
    EXPECT_EQ(ebcN12dg.numNodes(), 1);
    EXPECT_EQ(ebcN12dg.numEdges(), 1);

    auto  ebcN12egNit = ebcN12eg.nodes().first;
    auto& ebcN12egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN12egNit)));
    auto& ebcN12egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN12egN01.IGBackwardLink())));
    auto& ebcN12egN01GN = dynamic_cast<NumNode&>(ebcN12egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN12egN01GN.backIt(), n03.backIt());
    ebcN12egNit++;
    auto& ebcN12egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN12egNit)));
    auto& ebcN12egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN12egN02.IGBackwardLink())));
    auto& ebcN12egN02GN = dynamic_cast<NumNode&>(ebcN12egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN12egN02GN.backIt(), n09.backIt());

    auto  ebcN12egEit = ebcN12eg.edges().first;
    auto& ebcN12egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN12egEit)));
    auto& ebcN12egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN12egE01.IGBackwardLink())));
    auto& ebcN12egE01GE = dynamic_cast<Edge&>(ebcN12egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN12egE01GE.backIt(), e03_09.backIt());

    auto  ebcN12dgFit = ebcN12dg.nodes().first;
    auto& ebcN12dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN12dgFit)));

    //auto  ebcN12dgEit = ebcN12dg.edges().first;
    //auto& ebcN12dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN12dgEit)));



    EXPECT_EQ(ebcN13.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN13.IGBackwardLinkRef(), &bcN13);
    auto& ebcN13eg = ebcN13.embeddedGraph();
    auto& ebcN13dg = ebcN13.dualGraph();
    EXPECT_EQ(ebcN13eg.numNodes(), 2);
    EXPECT_EQ(ebcN13eg.numEdges(), 1);
    EXPECT_EQ(ebcN13dg.numNodes(), 1);
    EXPECT_EQ(ebcN13dg.numEdges(), 1);

    auto  ebcN13egNit = ebcN13eg.nodes().first;
    auto& ebcN13egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN13egNit)));
    auto& ebcN13egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN13egN01.IGBackwardLink())));
    auto& ebcN13egN01GN = dynamic_cast<NumNode&>(ebcN13egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN13egN01GN.backIt(), n03.backIt());
    ebcN13egNit++;
    auto& ebcN13egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN13egNit)));
    auto& ebcN13egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN13egN02.IGBackwardLink())));
    auto& ebcN13egN02GN = dynamic_cast<NumNode&>(ebcN13egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN13egN02GN.backIt(), n10.backIt());

    auto  ebcN13egEit = ebcN13eg.edges().first;
    auto& ebcN13egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN13egEit)));
    auto& ebcN13egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN13egE01.IGBackwardLink())));
    auto& ebcN13egE01GE = dynamic_cast<Edge&>(ebcN13egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN13egE01GE.backIt(), e03_10.backIt());

    auto  ebcN13dgFit = ebcN13dg.nodes().first;
    auto& ebcN13dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN13dgFit)));

    //auto  ebcN13dgEit = ebcN13dg.edges().first;
    //auto& ebcN13dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN13dgEit)));



    EXPECT_EQ(ebcN14.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN14.IGBackwardLinkRef(), &bcN14);
    auto& ebcN14eg = ebcN14.embeddedGraph();
    auto& ebcN14dg = ebcN14.dualGraph();
    EXPECT_EQ(ebcN14eg.numNodes(), 2);
    EXPECT_EQ(ebcN14eg.numEdges(), 1);
    EXPECT_EQ(ebcN14dg.numNodes(), 1);
    EXPECT_EQ(ebcN14dg.numEdges(), 1);

    auto  ebcN14egNit = ebcN14eg.nodes().first;
    auto& ebcN14egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN14egNit)));
    auto& ebcN14egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN14egN01.IGBackwardLink())));
    auto& ebcN14egN01GN = dynamic_cast<NumNode&>(ebcN14egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN14egN01GN.backIt(), n01.backIt());
    ebcN14egNit++;
    auto& ebcN14egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN14egNit)));
    auto& ebcN14egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN14egN02.IGBackwardLink())));
    auto& ebcN14egN02GN = dynamic_cast<NumNode&>(ebcN14egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN14egN02GN.backIt(), n03.backIt());

    auto  ebcN14egEit = ebcN14eg.edges().first;
    auto& ebcN14egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN14egEit)));
    auto& ebcN14egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN14egE01.IGBackwardLink())));
    auto& ebcN14egE01GE = dynamic_cast<Edge&>(ebcN14egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN14egE01GE.backIt(), e01_03.backIt());

    auto  ebcN14dgFit = ebcN14dg.nodes().first;
    auto& ebcN14dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN14dgFit)));

    //auto  ebcN14dgEit = ebcN14dg.edges().first;
    //auto& ebcN14dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN14dgEit)));

    EXPECT_EQ(ebcN15.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN15.IGBackwardLinkRef(), &bcN15);
    auto& ebcN15eg = ebcN15.embeddedGraph();
    auto& ebcN15dg = ebcN15.dualGraph();
    EXPECT_EQ(ebcN15eg.numNodes(), 2);
    EXPECT_EQ(ebcN15eg.numEdges(), 1);
    EXPECT_EQ(ebcN15dg.numNodes(), 1);
    EXPECT_EQ(ebcN15dg.numEdges(), 1);

    auto  ebcN15egNit = ebcN15eg.nodes().first;
    auto& ebcN15egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN15egNit)));
    auto& ebcN15egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN15egN01.IGBackwardLink())));
    auto& ebcN15egN01GN = dynamic_cast<NumNode&>(ebcN15egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN15egN01GN.backIt(), n01.backIt());
    ebcN15egNit++;
    auto& ebcN15egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN15egNit)));
    auto& ebcN15egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN15egN02.IGBackwardLink())));
    auto& ebcN15egN02GN = dynamic_cast<NumNode&>(ebcN15egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN15egN02GN.backIt(), n04.backIt());

    auto  ebcN15egEit = ebcN15eg.edges().first;
    auto& ebcN15egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN15egEit)));
    auto& ebcN15egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN15egE01.IGBackwardLink())));
    auto& ebcN15egE01GE = dynamic_cast<Edge&>(ebcN15egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN15egE01GE.backIt(), e01_04.backIt());

    auto  ebcN15dgFit = ebcN15dg.nodes().first;
    auto& ebcN15dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN15dgFit)));

    //auto  ebcN15dgEit = ebcN15dg.edges().first;
    //auto& ebcN15dgE1 = dynamic_cast<DualEdge&>(*(*(ebcN15dgEit)));

    EXPECT_EQ(&ebcE01.IGBackwardLinkRef(), &bcE01);
    EXPECT_EQ(&ebcE01.incidentNodeBlockType(), &ebcN02);

    EXPECT_EQ(&ebcE02.IGBackwardLinkRef(), &bcE02);
    EXPECT_EQ(&ebcE02.incidentNodeBlockType(), &ebcN03);

    EXPECT_EQ(&ebcE03.IGBackwardLinkRef(), &bcE03);
    EXPECT_EQ(&ebcE03.incidentNodeBlockType(), &ebcN04);

    EXPECT_EQ(&ebcE04.IGBackwardLinkRef(), &bcE04);
    EXPECT_EQ(&ebcE04.incidentNodeBlockType(), &ebcN06);

    EXPECT_EQ(&ebcE05.IGBackwardLinkRef(), &bcE05);
    EXPECT_EQ(&ebcE05.incidentNodeBlockType(), &ebcN06);

    EXPECT_EQ(&ebcE06.IGBackwardLinkRef(), &bcE06);
    EXPECT_EQ(&ebcE06.incidentNodeBlockType(), &ebcN08);

    EXPECT_EQ(&ebcE07.IGBackwardLinkRef(), &bcE07);
    EXPECT_EQ(&ebcE07.incidentNodeBlockType(), &ebcN10);

    EXPECT_EQ(&ebcE08.IGBackwardLinkRef(), &bcE08);
    EXPECT_EQ(&ebcE08.incidentNodeBlockType(), &ebcN11);

    EXPECT_EQ(&ebcE09.IGBackwardLinkRef(), &bcE09);
    EXPECT_EQ(&ebcE09.incidentNodeBlockType(), &ebcN12);

    EXPECT_EQ(&ebcE10.IGBackwardLinkRef(), &bcE10);
    EXPECT_EQ(&ebcE10.incidentNodeBlockType(), &ebcN12);

    EXPECT_EQ(&ebcE11.IGBackwardLinkRef(), &bcE11);
    EXPECT_EQ(&ebcE11.incidentNodeBlockType(), &ebcN13);

    EXPECT_EQ(&ebcE12.IGBackwardLinkRef(), &bcE12);
    EXPECT_EQ(&ebcE12.incidentNodeBlockType(), &ebcN14);

    EXPECT_EQ(&ebcE13.IGBackwardLinkRef(), &bcE13);
    EXPECT_EQ(&ebcE13.incidentNodeBlockType(), &ebcN14);

    EXPECT_EQ(&ebcE14.IGBackwardLinkRef(), &bcE14);
    EXPECT_EQ(&ebcE14.incidentNodeBlockType(), &ebcN15);

    EXPECT_EQ(ebcN01UG1.size(), 4);
    UnificationFace& uf1 = ebcN01UG1.at(0);
    UnificationFace& uf2 = ebcN01UG1.at(1);
    UnificationFace& uf3 = ebcN01UG1.at(2);
    UnificationFace& uf4 = ebcN01UG1.at(3);

    EXPECT_EQ(&uf1.treeNode(), &ebcN02);
    EXPECT_EQ(&uf1.faceInDG(), &ebcN02dgF01);
    EXPECT_EQ(uf1.faceInDGIt(), ebcN02dgF01.backIt());
    EXPECT_EQ(&uf1.cutVertexInEG(), &ebcN02egN01);
    EXPECT_EQ(uf1.cutVertexInEGIt(), ebcN02egN01.backIt());
    EXPECT_EQ(&uf1.edgeCCWInEG(), &ebcN02egE01);
    EXPECT_EQ(uf1.edgeCCWInEGIt(), ebcN02egE01.backIt());
    EXPECT_EQ(&uf1.edgeCWInEG(), &ebcN02egE01);
    EXPECT_EQ(uf1.edgeCWInEGIt(), ebcN02egE01.backIt());
    EXPECT_EQ(uf1.roleOfECWReversed(), false);

    EXPECT_EQ(&uf2.treeNode(), &ebcN03);
    EXPECT_EQ(&uf2.faceInDG(), &ebcN03dgF01);
    EXPECT_EQ(uf2.faceInDGIt(), ebcN03dgF01.backIt());
    EXPECT_EQ(&uf2.cutVertexInEG(), &ebcN03egN01);
    EXPECT_EQ(uf2.cutVertexInEGIt(), ebcN03egN01.backIt());
    EXPECT_EQ(&uf2.edgeCCWInEG(), &ebcN03egE01);
    EXPECT_EQ(uf2.edgeCCWInEGIt(), ebcN03egE01.backIt());
    EXPECT_EQ(&uf2.edgeCWInEG(), &ebcN03egE01);
    EXPECT_EQ(uf2.edgeCWInEGIt(), ebcN03egE01.backIt());
    EXPECT_EQ(uf2.roleOfECWReversed(), false);

    EXPECT_EQ(&uf3.treeNode(), &ebcN04);
    EXPECT_EQ(&uf3.faceInDG(), &ebcN04dgF01);
    EXPECT_EQ(uf3.faceInDGIt(), ebcN04dgF01.backIt());
    EXPECT_EQ(&uf3.cutVertexInEG(), &ebcN04egN01);
    EXPECT_EQ(uf3.cutVertexInEGIt(), ebcN04egN01.backIt());
    EXPECT_EQ(&uf3.edgeCCWInEG(), &ebcN04egE01);
    EXPECT_EQ(uf3.edgeCCWInEGIt(), ebcN04egE01.backIt());
    EXPECT_EQ(&uf3.edgeCWInEG(), &ebcN04egE01);
    EXPECT_EQ(uf3.edgeCWInEGIt(), ebcN04egE01.backIt());
    EXPECT_EQ(uf3.roleOfECWReversed(), false);

    EXPECT_EQ(&uf4.treeNode(), &ebcN06);
    EXPECT_EQ(&uf4.faceInDG(), &ebcN06dgF01);
    EXPECT_EQ(uf4.faceInDGIt(), ebcN06dgF01.backIt());
    EXPECT_EQ(&uf4.cutVertexInEG(), &ebcN06egN02);
    EXPECT_EQ(uf4.cutVertexInEGIt(), ebcN06egN02.backIt());
    EXPECT_EQ(&uf4.edgeCCWInEG(), &ebcN06egE01);
    EXPECT_EQ(uf4.edgeCCWInEGIt(), ebcN06egE01.backIt());
    EXPECT_EQ(&uf4.edgeCWInEG(), &ebcN06egE01);
    EXPECT_EQ(uf4.edgeCWInEGIt(), ebcN06egE01.backIt());
    EXPECT_EQ(uf4.roleOfECWReversed(), false);

    EXPECT_EQ(ebcN05UG1.size(), 3);
    UnificationFace& uf5 = ebcN05UG1.at(0);
    UnificationFace& uf6 = ebcN05UG1.at(1);
    UnificationFace& uf7 = ebcN05UG1.at(2);

    EXPECT_EQ(&uf5.treeNode(), &ebcN06);
    EXPECT_EQ(&uf5.faceInDG(), &ebcN06dgF01);
    EXPECT_EQ(uf5.faceInDGIt(), ebcN06dgF01.backIt());
    EXPECT_EQ(&uf5.cutVertexInEG(), &ebcN06egN01);
    EXPECT_EQ(uf5.cutVertexInEGIt(), ebcN06egN01.backIt());
    EXPECT_EQ(&uf5.edgeCCWInEG(), &ebcN06egE01);
    EXPECT_EQ(uf5.edgeCCWInEGIt(), ebcN06egE01.backIt());
    EXPECT_EQ(&uf5.edgeCWInEG(), &ebcN06egE01);
    EXPECT_EQ(uf5.edgeCWInEGIt(), ebcN06egE01.backIt());
    EXPECT_EQ(uf5.roleOfECWReversed(), false);

    EXPECT_EQ(&uf6.treeNode(), &ebcN14);
    EXPECT_EQ(&uf6.faceInDG(), &ebcN14dgF01);
    EXPECT_EQ(uf6.faceInDGIt(), ebcN14dgF01.backIt());
    EXPECT_EQ(&uf6.cutVertexInEG(), &ebcN14egN01);
    EXPECT_EQ(uf6.cutVertexInEGIt(), ebcN14egN01.backIt());
    EXPECT_EQ(&uf6.edgeCCWInEG(), &ebcN14egE01);
    EXPECT_EQ(uf6.edgeCCWInEGIt(), ebcN14egE01.backIt());
    EXPECT_EQ(&uf6.edgeCWInEG(), &ebcN14egE01);
    EXPECT_EQ(uf6.edgeCWInEGIt(), ebcN14egE01.backIt());
    EXPECT_EQ(uf6.roleOfECWReversed(), false);

    EXPECT_EQ(&uf7.treeNode(), &ebcN15);
    EXPECT_EQ(&uf7.faceInDG(), &ebcN15dgF01);
    EXPECT_EQ(uf7.faceInDGIt(), ebcN15dgF01.backIt());
    EXPECT_EQ(&uf7.cutVertexInEG(), &ebcN15egN01);
    EXPECT_EQ(uf7.cutVertexInEGIt(), ebcN15egN01.backIt());
    EXPECT_EQ(&uf7.edgeCCWInEG(), &ebcN15egE01);
    EXPECT_EQ(uf7.edgeCCWInEGIt(), ebcN15egE01.backIt());
    EXPECT_EQ(&uf7.edgeCWInEG(), &ebcN15egE01);
    EXPECT_EQ(uf7.edgeCWInEGIt(), ebcN15egE01.backIt());
    EXPECT_EQ(uf7.roleOfECWReversed(), false);

    EXPECT_EQ(ebcN07UG1.size(), 4);
    UnificationFace& uf8  = ebcN07UG1.at(0);
    UnificationFace& uf9  = ebcN07UG1.at(1);
    UnificationFace& uf10 = ebcN07UG1.at(2);
    UnificationFace& uf11 = ebcN07UG1.at(3);

    EXPECT_EQ(&uf8.treeNode(), &ebcN08);
    EXPECT_EQ(&uf8.faceInDG(), &ebcN08dgF01);
    EXPECT_EQ(uf8.faceInDGIt(), ebcN08dgF01.backIt());
    EXPECT_EQ(&uf8.cutVertexInEG(), &ebcN08egN01);
    EXPECT_EQ(uf8.cutVertexInEGIt(), ebcN08egN01.backIt());
    EXPECT_EQ(&uf8.edgeCCWInEG(), &ebcN08egE01);
    EXPECT_EQ(uf8.edgeCCWInEGIt(), ebcN08egE01.backIt());
    EXPECT_EQ(&uf8.edgeCWInEG(), &ebcN08egE01);
    EXPECT_EQ(uf8.edgeCWInEGIt(), ebcN08egE01.backIt());
    EXPECT_EQ(uf8.roleOfECWReversed(), false);

    EXPECT_EQ(&uf9.treeNode(), &ebcN12);
    EXPECT_EQ(&uf9.faceInDG(), &ebcN12dgF01);
    EXPECT_EQ(uf9.faceInDGIt(), ebcN12dgF01.backIt());
    EXPECT_EQ(&uf9.cutVertexInEG(), &ebcN12egN01);
    EXPECT_EQ(uf9.cutVertexInEGIt(), ebcN12egN01.backIt());
    EXPECT_EQ(&uf9.edgeCCWInEG(), &ebcN12egE01);
    EXPECT_EQ(uf9.edgeCCWInEGIt(), ebcN12egE01.backIt());
    EXPECT_EQ(&uf9.edgeCWInEG(), &ebcN12egE01);
    EXPECT_EQ(uf9.edgeCWInEGIt(), ebcN12egE01.backIt());
    EXPECT_EQ(uf9.roleOfECWReversed(), false);

    EXPECT_EQ(&uf10.treeNode(), &ebcN13);
    EXPECT_EQ(&uf10.faceInDG(), &ebcN13dgF01);
    EXPECT_EQ(uf10.faceInDGIt(), ebcN13dgF01.backIt());
    EXPECT_EQ(&uf10.cutVertexInEG(), &ebcN13egN01);
    EXPECT_EQ(uf10.cutVertexInEGIt(), ebcN13egN01.backIt());
    EXPECT_EQ(&uf10.edgeCCWInEG(), &ebcN13egE01);
    EXPECT_EQ(uf10.edgeCCWInEGIt(), ebcN13egE01.backIt());
    EXPECT_EQ(&uf10.edgeCWInEG(), &ebcN13egE01);
    EXPECT_EQ(uf10.edgeCWInEGIt(), ebcN13egE01.backIt());
    EXPECT_EQ(uf10.roleOfECWReversed(), false);

    EXPECT_EQ(&uf11.treeNode(), &ebcN14);
    EXPECT_EQ(&uf11.faceInDG(), &ebcN14dgF01);
    EXPECT_EQ(uf11.faceInDGIt(), ebcN14dgF01.backIt());
    EXPECT_EQ(&uf11.cutVertexInEG(), &ebcN14egN02);
    EXPECT_EQ(uf11.cutVertexInEGIt(), ebcN14egN02.backIt());
    EXPECT_EQ(&uf11.edgeCCWInEG(), &ebcN14egE01);
    EXPECT_EQ(uf11.edgeCCWInEGIt(), ebcN14egE01.backIt());
    EXPECT_EQ(&uf11.edgeCWInEG(), &ebcN14egE01);
    EXPECT_EQ(uf11.edgeCWInEGIt(), ebcN14egE01.backIt());
    EXPECT_EQ(uf11.roleOfECWReversed(), false);

    EXPECT_EQ(ebcN09UG1.size(), 3);
    UnificationFace& uf12  = ebcN09UG1.at(0);
    UnificationFace& uf13  = ebcN09UG1.at(1);
    UnificationFace& uf14  = ebcN09UG1.at(2);

    EXPECT_EQ(&uf12.treeNode(), &ebcN10);
    EXPECT_EQ(&uf12.faceInDG(), &ebcN10dgF01);
    EXPECT_EQ(uf12.faceInDGIt(), ebcN10dgF01.backIt());
    EXPECT_EQ(&uf12.cutVertexInEG(), &ebcN10egN01);
    EXPECT_EQ(uf12.cutVertexInEGIt(), ebcN10egN01.backIt());
    EXPECT_EQ(&uf12.edgeCCWInEG(), &ebcN10egE01);
    EXPECT_EQ(uf12.edgeCCWInEGIt(), ebcN10egE01.backIt());
    EXPECT_EQ(&uf12.edgeCWInEG(), &ebcN10egE01);
    EXPECT_EQ(uf12.edgeCWInEGIt(), ebcN10egE01.backIt());
    EXPECT_EQ(uf12.roleOfECWReversed(), false);

    EXPECT_EQ(&uf13.treeNode(), &ebcN11);
    EXPECT_EQ(&uf13.faceInDG(), &ebcN11dgF01);
    EXPECT_EQ(uf13.faceInDGIt(), ebcN11dgF01.backIt());
    EXPECT_EQ(&uf13.cutVertexInEG(), &ebcN11egN01);
    EXPECT_EQ(uf13.cutVertexInEGIt(), ebcN11egN01.backIt());
    EXPECT_EQ(&uf13.edgeCCWInEG(), &ebcN11egE01);
    EXPECT_EQ(uf13.edgeCCWInEGIt(), ebcN11egE01.backIt());
    EXPECT_EQ(&uf13.edgeCWInEG(), &ebcN11egE01);
    EXPECT_EQ(uf13.edgeCWInEGIt(), ebcN11egE01.backIt());
    EXPECT_EQ(uf13.roleOfECWReversed(), false);

    EXPECT_EQ(&uf14.treeNode(), &ebcN12);
    EXPECT_EQ(&uf14.faceInDG(), &ebcN12dgF01);
    EXPECT_EQ(uf14.faceInDGIt(), ebcN12dgF01.backIt());
    EXPECT_EQ(&uf14.cutVertexInEG(), &ebcN12egN02);
    EXPECT_EQ(uf14.cutVertexInEGIt(), ebcN12egN02.backIt());
    EXPECT_EQ(&uf14.edgeCCWInEG(), &ebcN12egE01);
    EXPECT_EQ(uf14.edgeCCWInEGIt(), ebcN12egE01.backIt());
    EXPECT_EQ(&uf14.edgeCWInEG(), &ebcN12egE01);
    EXPECT_EQ(uf14.edgeCWInEGIt(), ebcN12egE01.backIt());
    EXPECT_EQ(uf14.roleOfECWReversed(), false);

    ebcT.findGeometricEmbedding();

    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN02);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();

    auto& expTR = expT.root();

    EXPECT_EQ(expT.numNodes(),11);
    EXPECT_EQ(expT.numEdges(),10);

    auto expNit = expT.nodes().first;
    auto& expN01 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN02 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN03 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN04 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN05 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN06 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN07 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN08 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN09 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN10 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN11 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));

    EXPECT_EQ(&expTR, &expN01);

    EXPECT_EQ(&expN01.IGBackwardLinkRef(), &ebcN02);
    EXPECT_EQ(expN01.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN01UGs=expN01.sortedUnificationGroups();
    EXPECT_EQ(expN01UGs.size(), 1);
    EXPECT_EQ(expN01UGs[0].absorbeesSize(), 3);
    EXPECT_EQ(expN01UGs[0].absorber(),     uf1);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(0),  uf2);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(1),  uf3);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(2),  uf4);

    EXPECT_EQ(&expN02.IGBackwardLinkRef(), &ebcN03);
    EXPECT_EQ(expN02.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN02UGs=expN02.sortedUnificationGroups();
    EXPECT_EQ(expN02UGs.size(), 0);

    EXPECT_EQ(&expN03.IGBackwardLinkRef(), &ebcN04);
    EXPECT_EQ(expN03.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN03UGs=expN03.sortedUnificationGroups();
    EXPECT_EQ(expN03UGs.size(), 0);

    EXPECT_EQ(&expN04.IGBackwardLinkRef(), &ebcN06);
    EXPECT_EQ(expN04.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN04UGs=expN04.sortedUnificationGroups();
    EXPECT_EQ(expN04UGs.size(), 1);
    EXPECT_EQ(expN04UGs[0].absorbeesSize(), 2);
    EXPECT_EQ(expN04UGs[0].absorber(),     uf5);
    EXPECT_EQ(expN04UGs[0].absorbeeAt(0),  uf6);
    EXPECT_EQ(expN04UGs[0].absorbeeAt(1),  uf7);

    EXPECT_EQ(&expN05.IGBackwardLinkRef(), &ebcN08);
    EXPECT_EQ(expN05.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN05UGs=expN05.sortedUnificationGroups();
    EXPECT_EQ(expN05UGs.size(), 0);

    EXPECT_EQ(&expN06.IGBackwardLinkRef(), &ebcN10);
    EXPECT_EQ(expN06.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN06UGs=expN06.sortedUnificationGroups();
    EXPECT_EQ(expN06UGs.size(), 0);

    EXPECT_EQ(&expN07.IGBackwardLinkRef(), &ebcN11);
    EXPECT_EQ(expN07.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN07UGs=expN07.sortedUnificationGroups();
    EXPECT_EQ(expN07UGs.size(), 0);

    EXPECT_EQ(&expN08.IGBackwardLinkRef(), &ebcN12);
    EXPECT_EQ(expN08.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN08UGs=expN08.sortedUnificationGroups();
    EXPECT_EQ(expN08UGs.size(), 1);
    EXPECT_EQ(expN08UGs[0].absorbeesSize(), 2);
    EXPECT_EQ(expN08UGs[0].absorber(),     uf14);
    EXPECT_EQ(expN08UGs[0].absorbeeAt(0),  uf12);
    EXPECT_EQ(expN08UGs[0].absorbeeAt(1),  uf13);

    EXPECT_EQ(&expN09.IGBackwardLinkRef(), &ebcN13);
    EXPECT_EQ(expN09.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN09UGs=expN09.sortedUnificationGroups();
    EXPECT_EQ(expN09UGs.size(), 0);

    EXPECT_EQ(&expN10.IGBackwardLinkRef(), &ebcN14);
    EXPECT_EQ(expN10.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN10UGs=expN10.sortedUnificationGroups();
    EXPECT_EQ(expN10UGs.size(), 1);
    EXPECT_EQ(expN10UGs[0].absorbeesSize(), 3);
    EXPECT_EQ(expN10UGs[0].absorber(),     uf11);
    EXPECT_EQ(expN10UGs[0].absorbeeAt(0),  uf8);
    EXPECT_EQ(expN10UGs[0].absorbeeAt(1),  uf9);
    EXPECT_EQ(expN10UGs[0].absorbeeAt(2),  uf10);

    EXPECT_EQ(&expN11.IGBackwardLinkRef(), &ebcN15);
    EXPECT_EQ(expN11.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN11UGs=expN11.sortedUnificationGroups();
    EXPECT_EQ(expN11UGs.size(), 0);

    auto expEit = expT.edges().first;
    auto& expE01 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE01.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE01.incidentNodeDst().backIt(), expN02.backIt());
    expEit++;

    auto& expE02 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE02.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE02.incidentNodeDst().backIt(), expN03.backIt());

    expEit++;
    auto& expE03 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE03.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE03.incidentNodeDst().backIt(), expN04.backIt());

    expEit++;
    auto& expE04 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE04.incidentNodeSrc().backIt(), expN04.backIt());
    EXPECT_EQ(expE04.incidentNodeDst().backIt(), expN10.backIt());

    expEit++;
    auto& expE05 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE05.incidentNodeSrc().backIt(), expN04.backIt());
    EXPECT_EQ(expE05.incidentNodeDst().backIt(), expN11.backIt());

    expEit++;
    auto& expE06 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE06.incidentNodeSrc().backIt(), expN10.backIt());
    EXPECT_EQ(expE06.incidentNodeDst().backIt(), expN05.backIt());

    expEit++;
    auto& expE07 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE07.incidentNodeSrc().backIt(), expN10.backIt());
    EXPECT_EQ(expE07.incidentNodeDst().backIt(), expN08.backIt());

    expEit++;
    auto& expE08 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE08.incidentNodeSrc().backIt(), expN10.backIt());
    EXPECT_EQ(expE08.incidentNodeDst().backIt(), expN09.backIt());

    expEit++;
    auto& expE09 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE09.incidentNodeSrc().backIt(), expN08.backIt());
    EXPECT_EQ(expE09.incidentNodeDst().backIt(), expN06.backIt());

    expEit++;
    auto& expE10 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE10.incidentNodeSrc().backIt(), expN08.backIt());
    EXPECT_EQ(expE10.incidentNodeDst().backIt(), expN07.backIt());

    EXPECT_EQ(&ebcN02.outerFace(), &ebcN02dgF01);
    EXPECT_EQ(&ebcN02.topNode(),   &ebcN02egN02);//n05

    EXPECT_EQ(&ebcN03.outerFace(), &ebcN03dgF01);
    EXPECT_EQ(&ebcN03.topNode(),   &ebcN03egN01);//n02

    EXPECT_EQ(&ebcN04.outerFace(), &ebcN04dgF01);
    EXPECT_EQ(&ebcN04.topNode(),   &ebcN04egN01);//n02

    EXPECT_EQ(&ebcN06.outerFace(), &ebcN06dgF01);
    EXPECT_EQ(&ebcN06.topNode(),   &ebcN06egN02);//n02

    EXPECT_EQ(&ebcN10.outerFace(), &ebcN10dgF01);
    EXPECT_EQ(&ebcN10.topNode(),   &ebcN10egN01);//n09

    EXPECT_EQ(&ebcN11.outerFace(), &ebcN11dgF01);
    EXPECT_EQ(&ebcN11.topNode(),   &ebcN11egN01);//n09

    EXPECT_EQ(&ebcN12.outerFace(), &ebcN12dgF01);
    EXPECT_EQ(&ebcN12.topNode(),   &ebcN12egN01);//n03

    EXPECT_EQ(&ebcN13.outerFace(), &ebcN13dgF01);
    EXPECT_EQ(&ebcN13.topNode(),   &ebcN13egN01);//n03

    EXPECT_EQ(&ebcN14.outerFace(), &ebcN14dgF01);
    EXPECT_EQ(&ebcN14.topNode(),   &ebcN14egN01);//n01

    EXPECT_EQ(&ebcN15.outerFace(), &ebcN15dgF01);
    EXPECT_EQ(&ebcN15.topNode(),   &ebcN15egN01);//n01

}


/**  @brief 4 k3s
 */
TEST_F(EmbeddedBCTreeTests, FourK3s) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;

    auto  np01 = generate_node(1);
    auto& n01  = g1.addNode(std::move(np01));
    auto  np02 = generate_node(2);
    auto& n02  = g1.addNode(std::move(np02));
    auto  np03 = generate_node(3);
    auto& n03  = g1.addNode(std::move(np03));
    auto  np04 = generate_node(4);
    auto& n04  = g1.addNode(std::move(np04));
    auto  np05 = generate_node(5);
    auto& n05  = g1.addNode(std::move(np05));
    auto  np06 = generate_node(6);
    auto& n06  = g1.addNode(std::move(np06));
    auto  np07 = generate_node(7);
    auto& n07  = g1.addNode(std::move(np07));
    auto  np08 = generate_node(8);
    auto& n08  = g1.addNode(std::move(np08));
    auto  np09 = generate_node(9);
    auto& n09  = g1.addNode(std::move(np09));

    auto  ep01_02 = generate_edge();
    auto& e01_02  = g1.addEdge(std::move(ep01_02),n01,n02);
    auto  ep02_09 = generate_edge();
    auto& e02_09  = g1.addEdge(std::move(ep02_09),n02,n09);
    auto  ep09_01 = generate_edge();
    auto& e09_01  = g1.addEdge(std::move(ep09_01),n09,n01);
    auto  ep01_04 = generate_edge();
    auto& e01_04  = g1.addEdge(std::move(ep01_04),n01,n04);
    auto  ep04_03 = generate_edge();
    auto& e04_03  = g1.addEdge(std::move(ep04_03),n04,n03);
    auto  ep03_01 = generate_edge();
    auto& e03_01  = g1.addEdge(std::move(ep03_01),n03,n01);
    auto  ep01_06 = generate_edge();
    auto& e01_06  = g1.addEdge(std::move(ep01_06),n01,n06);
    auto  ep06_05 = generate_edge();
    auto& e06_05  = g1.addEdge(std::move(ep06_05),n06,n05);
    auto  ep05_01 = generate_edge();
    auto& e05_01  = g1.addEdge(std::move(ep05_01),n05,n01);
    auto  ep01_08 = generate_edge();
    auto& e01_08  = g1.addEdge(std::move(ep01_08),n01,n08);
    auto  ep08_07 = generate_edge();
    auto& e08_07  = g1.addEdge(std::move(ep08_07),n08,n07);
    auto  ep07_01 = generate_edge();
    auto& e07_01  = g1.addEdge(std::move(ep07_01),n07,n01);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 5);
    EXPECT_EQ(ebcT.numEdges(), 4);

    ebcT.makeDefaultEmbedding();

    auto ebcNit = ebcT.nodes().first;
    auto& ebcN01 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN02 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN03 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN04 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN05 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN01 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN02 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN03 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN04 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN05 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    auto ebcEit = ebcT.edges().first;
    auto& ebcE01 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE02 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE03 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE04 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));

    auto bcEit = bcT.edges().first;
    auto& bcE01 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE02 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE03 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE04 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));

    EXPECT_EQ(ebcN01.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN01.IGBackwardLinkRef(), &bcN01);
    EXPECT_EQ(&bcN01.IGBackwardLinkRef(), &n01);
    EXPECT_THROW(ebcN01.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN01.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN01.numUnificationGroups(), 1);
    auto& ebcN01UG1 = ebcN01.unificationGroup(0);


    EXPECT_EQ(ebcN02.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN02.IGBackwardLinkRef(), &bcN02);
    auto& ebcN02eg = ebcN02.embeddedGraph();
    auto& ebcN02dg = ebcN02.dualGraph();
    EXPECT_EQ(ebcN02eg.numNodes(), 3);
    EXPECT_EQ(ebcN02eg.numEdges(), 3);
    EXPECT_EQ(ebcN02dg.numNodes(), 2);
    EXPECT_EQ(ebcN02dg.numEdges(), 3);

    auto  ebcN02egNit = ebcN02eg.nodes().first;
    auto& ebcN02egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN01.IGBackwardLink())));
    auto& ebcN02egN01GN = dynamic_cast<NumNode&>(ebcN02egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN01GN.backIt(), n09.backIt());
    ebcN02egNit++;
    auto& ebcN02egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN02.IGBackwardLink())));
    auto& ebcN02egN02GN = dynamic_cast<NumNode&>(ebcN02egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN02GN.backIt(), n01.backIt());
    ebcN02egNit++;
    auto& ebcN02egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN03.IGBackwardLink())));
    auto& ebcN02egN03GN = dynamic_cast<NumNode&>(ebcN02egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN03GN.backIt(), n02.backIt());

    auto  ebcN02egEit = ebcN02eg.edges().first;
    auto& ebcN02egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE01.IGBackwardLink())));
    auto& ebcN02egE01GE = dynamic_cast<Edge&>(ebcN02egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE01GE.backIt(), e09_01.backIt());
    ebcN02egEit++;
    auto& ebcN02egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE02.IGBackwardLink())));
    auto& ebcN02egE02GE = dynamic_cast<Edge&>(ebcN02egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE02GE.backIt(), e02_09.backIt());
    ebcN02egEit++;
    auto& ebcN02egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE03.IGBackwardLink())));
    auto& ebcN02egE03GE = dynamic_cast<Edge&>(ebcN02egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE03GE.backIt(), e01_02.backIt());

    auto  ebcN02dgFit = ebcN02dg.nodes().first;
    auto& ebcN02dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN02dgFit)));
    ebcN02dgFit++;
    auto& ebcN02dgF02 = dynamic_cast<EmbeddedFace&>(*(*(ebcN02dgFit)));

    //auto  ebcN02dgEit = ebcN02dg.edges().first;
    //auto& ebcN02dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN02dgEit)));
    //ebcN02dgEit++;
    //auto& ebcN02dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN02dgEit)));


    EXPECT_EQ(ebcN03.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN03.IGBackwardLinkRef(), &bcN03);
    auto& ebcN03eg = ebcN03.embeddedGraph();
    auto& ebcN03dg = ebcN03.dualGraph();
    EXPECT_EQ(ebcN03eg.numNodes(), 3);
    EXPECT_EQ(ebcN03eg.numEdges(), 3);
    EXPECT_EQ(ebcN03dg.numNodes(), 2);
    EXPECT_EQ(ebcN03dg.numEdges(), 3);

    auto  ebcN03egNit = ebcN03eg.nodes().first;
    auto& ebcN03egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN01.IGBackwardLink())));
    auto& ebcN03egN01GN = dynamic_cast<NumNode&>(ebcN03egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN01GN.backIt(), n03.backIt());
    ebcN03egNit++;
    auto& ebcN03egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN02.IGBackwardLink())));
    auto& ebcN03egN02GN = dynamic_cast<NumNode&>(ebcN03egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN02GN.backIt(), n01.backIt());
    ebcN03egNit++;
    auto& ebcN03egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN03.IGBackwardLink())));
    auto& ebcN03egN03GN = dynamic_cast<NumNode&>(ebcN03egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN03GN.backIt(), n04.backIt());

    auto  ebcN03egEit = ebcN03eg.edges().first;
    auto& ebcN03egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE01.IGBackwardLink())));
    auto& ebcN03egE01GE = dynamic_cast<Edge&>(ebcN03egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE01GE.backIt(), e03_01.backIt());
    ebcN03egEit++;
    auto& ebcN03egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE02.IGBackwardLink())));
    auto& ebcN03egE02GE = dynamic_cast<Edge&>(ebcN03egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE02GE.backIt(), e04_03.backIt());
    ebcN03egEit++;
    auto& ebcN03egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE03.IGBackwardLink())));
    auto& ebcN03egE03GE = dynamic_cast<Edge&>(ebcN03egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE03GE.backIt(), e01_04.backIt());

    auto  ebcN03dgFit = ebcN03dg.nodes().first;
    auto& ebcN03dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN03dgFit)));
    ebcN03dgFit++;
    auto& ebcN03dgF02 = dynamic_cast<EmbeddedFace&>(*(*(ebcN03dgFit)));

    //auto  ebcN03dgEit = ebcN03dg.edges().first;
    //auto& ebcN03dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN03dgEit)));
    //ebcN03dgEit++;
    //auto& ebcN03dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN03dgEit)));


    EXPECT_EQ(ebcN04.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN04.IGBackwardLinkRef(), &bcN04);
    auto& ebcN04eg = ebcN04.embeddedGraph();
    auto& ebcN04dg = ebcN04.dualGraph();
    EXPECT_EQ(ebcN04eg.numNodes(), 3);
    EXPECT_EQ(ebcN04eg.numEdges(), 3);
    EXPECT_EQ(ebcN04dg.numNodes(), 2);
    EXPECT_EQ(ebcN04dg.numEdges(), 3);

    auto  ebcN04egNit = ebcN04eg.nodes().first;
    auto& ebcN04egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN01.IGBackwardLink())));
    auto& ebcN04egN01GN = dynamic_cast<NumNode&>(ebcN04egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN01GN.backIt(), n05.backIt());
    ebcN04egNit++;
    auto& ebcN04egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN02.IGBackwardLink())));
    auto& ebcN04egN02GN = dynamic_cast<NumNode&>(ebcN04egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN02GN.backIt(), n01.backIt());
    ebcN04egNit++;
    auto& ebcN04egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN03.IGBackwardLink())));
    auto& ebcN04egN03GN = dynamic_cast<NumNode&>(ebcN04egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN03GN.backIt(), n06.backIt());

    auto  ebcN04egEit = ebcN04eg.edges().first;
    auto& ebcN04egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE01.IGBackwardLink())));
    auto& ebcN04egE01GE = dynamic_cast<Edge&>(ebcN04egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE01GE.backIt(), e05_01.backIt());
    ebcN04egEit++;
    auto& ebcN04egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE02.IGBackwardLink())));
    auto& ebcN04egE02GE = dynamic_cast<Edge&>(ebcN04egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE02GE.backIt(), e06_05.backIt());
    ebcN04egEit++;
    auto& ebcN04egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE03.IGBackwardLink())));
    auto& ebcN04egE03GE = dynamic_cast<Edge&>(ebcN04egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE03GE.backIt(), e01_06.backIt());

    auto  ebcN04dgFit = ebcN04dg.nodes().first;
    auto& ebcN04dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN04dgFit)));
    ebcN04dgFit++;
    auto& ebcN04dgF02 = dynamic_cast<EmbeddedFace&>(*(*(ebcN04dgFit)));

    //auto  ebcN04dgEit = ebcN04dg.edges().first;
    //auto& ebcN04dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN04dgEit)));
    //ebcN04dgEit++;
    //auto& ebcN04dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN04dgEit)));

    EXPECT_EQ(ebcN05.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN05.IGBackwardLinkRef(), &bcN05);
    auto& ebcN05eg = ebcN05.embeddedGraph();
    auto& ebcN05dg = ebcN05.dualGraph();
    EXPECT_EQ(ebcN05eg.numNodes(), 3);
    EXPECT_EQ(ebcN05eg.numEdges(), 3);
    EXPECT_EQ(ebcN05dg.numNodes(), 2);
    EXPECT_EQ(ebcN05dg.numEdges(), 3);

    auto  ebcN05egNit = ebcN05eg.nodes().first;
    auto& ebcN05egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN05egNit)));
    auto& ebcN05egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN05egN01.IGBackwardLink())));
    auto& ebcN05egN01GN = dynamic_cast<NumNode&>(ebcN05egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egN01GN.backIt(), n07.backIt());
    ebcN05egNit++;
    auto& ebcN05egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN05egNit)));
    auto& ebcN05egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN05egN02.IGBackwardLink())));
    auto& ebcN05egN02GN = dynamic_cast<NumNode&>(ebcN05egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egN02GN.backIt(), n01.backIt());
    ebcN05egNit++;
    auto& ebcN05egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN05egNit)));
    auto& ebcN05egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN05egN03.IGBackwardLink())));
    auto& ebcN05egN03GN = dynamic_cast<NumNode&>(ebcN05egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egN03GN.backIt(), n08.backIt());

    auto  ebcN05egEit = ebcN05eg.edges().first;
    auto& ebcN05egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN05egEit)));
    auto& ebcN05egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN05egE01.IGBackwardLink())));
    auto& ebcN05egE01GE = dynamic_cast<Edge&>(ebcN05egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egE01GE.backIt(), e07_01.backIt());
    ebcN05egEit++;
    auto& ebcN05egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN05egEit)));
    auto& ebcN05egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN05egE02.IGBackwardLink())));
    auto& ebcN05egE02GE = dynamic_cast<Edge&>(ebcN05egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egE02GE.backIt(), e08_07.backIt());
    ebcN05egEit++;
    auto& ebcN05egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN05egEit)));
    auto& ebcN05egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN05egE03.IGBackwardLink())));
    auto& ebcN05egE03GE = dynamic_cast<Edge&>(ebcN05egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egE03GE.backIt(), e01_08.backIt());

    auto  ebcN05dgFit = ebcN05dg.nodes().first;
    auto& ebcN05dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN05dgFit)));
    ebcN05dgFit++;
    auto& ebcN05dgF02 = dynamic_cast<EmbeddedFace&>(*(*(ebcN05dgFit)));

    //auto  ebcN05dgEit = ebcN05dg.edges().first;
    //auto& ebcN05dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN05dgEit)));
    //ebcN05dgEit++;
    //auto& ebcN05dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN05dgEit)));

    EXPECT_EQ(&ebcE01.IGBackwardLinkRef(), &bcE01);
    EXPECT_EQ(&ebcE01.incidentNodeBlockType(), &ebcN02);

    EXPECT_EQ(&ebcE02.IGBackwardLinkRef(), &bcE02);
    EXPECT_EQ(&ebcE02.incidentNodeBlockType(), &ebcN03);

    EXPECT_EQ(&ebcE03.IGBackwardLinkRef(), &bcE03);
    EXPECT_EQ(&ebcE03.incidentNodeBlockType(), &ebcN04);

    EXPECT_EQ(&ebcE04.IGBackwardLinkRef(), &bcE04);
    EXPECT_EQ(&ebcE04.incidentNodeBlockType(), &ebcN05);

    EXPECT_EQ(ebcN01UG1.size(), 4);
    UnificationFace& uf1 = ebcN01UG1.at(0);
    UnificationFace& uf2 = ebcN01UG1.at(1);
    UnificationFace& uf3 = ebcN01UG1.at(2);
    UnificationFace& uf4 = ebcN01UG1.at(3);

    EXPECT_EQ(&uf1.treeNode(),       &ebcN02);
    EXPECT_EQ(&uf1.faceInDG(),       &ebcN02dgF02);
    EXPECT_EQ( uf1.faceInDGIt(),      ebcN02dgF02.backIt());
    EXPECT_EQ(&uf1.cutVertexInEG(),  &ebcN02egN02);
    EXPECT_EQ( uf1.cutVertexInEGIt(), ebcN02egN02.backIt());
    EXPECT_EQ(&uf1.edgeCCWInEG(),    &ebcN02egE01);
    EXPECT_EQ( uf1.edgeCCWInEGIt(),   ebcN02egE01.backIt());
    EXPECT_EQ(&uf1.edgeCWInEG()    , &ebcN02egE03);
    EXPECT_EQ( uf1.edgeCWInEGIt(),    ebcN02egE03.backIt());
    EXPECT_EQ( uf1.roleOfECWReversed(), false);

    EXPECT_EQ(&uf2.treeNode(),       &ebcN03);
    EXPECT_EQ(&uf2.faceInDG(),       &ebcN03dgF02);
    EXPECT_EQ( uf2.faceInDGIt(),      ebcN03dgF02.backIt());
    EXPECT_EQ(&uf2.cutVertexInEG(),  &ebcN03egN02);
    EXPECT_EQ( uf2.cutVertexInEGIt(), ebcN03egN02.backIt());
    EXPECT_EQ(&uf2.edgeCCWInEG(),    &ebcN03egE01);
    EXPECT_EQ( uf2.edgeCCWInEGIt(),   ebcN03egE01.backIt());
    EXPECT_EQ(&uf2.edgeCWInEG()    , &ebcN03egE03);
    EXPECT_EQ( uf2.edgeCWInEGIt(),    ebcN03egE03.backIt());
    EXPECT_EQ( uf2.roleOfECWReversed(), false);

    EXPECT_EQ(&uf3.treeNode(),       &ebcN04);
    EXPECT_EQ(&uf3.faceInDG(),       &ebcN04dgF02);
    EXPECT_EQ( uf3.faceInDGIt(),      ebcN04dgF02.backIt());
    EXPECT_EQ(&uf3.cutVertexInEG(),  &ebcN04egN02);
    EXPECT_EQ( uf3.cutVertexInEGIt(), ebcN04egN02.backIt());
    EXPECT_EQ(&uf3.edgeCCWInEG(),    &ebcN04egE01);
    EXPECT_EQ( uf3.edgeCCWInEGIt(),   ebcN04egE01.backIt());
    EXPECT_EQ(&uf3.edgeCWInEG()    , &ebcN04egE03);
    EXPECT_EQ( uf3.edgeCWInEGIt(),    ebcN04egE03.backIt());
    EXPECT_EQ( uf3.roleOfECWReversed(), false);

    EXPECT_EQ(&uf4.treeNode(),       &ebcN05);
    EXPECT_EQ(&uf4.faceInDG(),       &ebcN05dgF02);
    EXPECT_EQ( uf4.faceInDGIt(),      ebcN05dgF02.backIt());
    EXPECT_EQ(&uf4.cutVertexInEG(),  &ebcN05egN02);
    EXPECT_EQ( uf4.cutVertexInEGIt(), ebcN05egN02.backIt());
    EXPECT_EQ(&uf4.edgeCCWInEG(),    &ebcN05egE01);
    EXPECT_EQ( uf4.edgeCCWInEGIt(),   ebcN05egE01.backIt());
    EXPECT_EQ(&uf4.edgeCWInEG()    , &ebcN05egE03);
    EXPECT_EQ( uf4.edgeCWInEGIt(),    ebcN05egE03.backIt());
    EXPECT_EQ( uf4.roleOfECWReversed(), false);

    ebcT.findGeometricEmbedding();

    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN02);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();

    auto& expTR = expT.root();

    EXPECT_EQ(expT.numNodes(),4);
    EXPECT_EQ(expT.numEdges(),3);

    auto expNit = expT.nodes().first;
    auto& expN01 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN02 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN03 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN04 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));

    EXPECT_EQ(&expTR, &expN01);

    EXPECT_EQ(&expN01.IGBackwardLinkRef(), &ebcN02);
    EXPECT_EQ(expN01.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN01UGs=expN01.sortedUnificationGroups();
    EXPECT_EQ(expN01UGs.size(), 1);
    EXPECT_EQ(expN01UGs[0].absorbeesSize(), 3);
    EXPECT_EQ(expN01UGs[0].absorber(),     uf1);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(0),  uf2);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(1),  uf3);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(2),  uf4);

    EXPECT_EQ(&expN02.IGBackwardLinkRef(), &ebcN03);
    EXPECT_EQ(expN02.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN02UGs=expN02.sortedUnificationGroups();
    EXPECT_EQ(expN02UGs.size(), 0);

    EXPECT_EQ(&expN03.IGBackwardLinkRef(), &ebcN04);
    EXPECT_EQ(expN03.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN03UGs=expN03.sortedUnificationGroups();
    EXPECT_EQ(expN03UGs.size(), 0);

    EXPECT_EQ(&expN04.IGBackwardLinkRef(), &ebcN05);
    EXPECT_EQ(expN04.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN04UGs=expN04.sortedUnificationGroups();
    EXPECT_EQ(expN04UGs.size(), 0);

    auto expEit = expT.edges().first;
    auto& expE01 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE01.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE01.incidentNodeDst().backIt(), expN02.backIt());
    expEit++;

    auto& expE02 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE02.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE02.incidentNodeDst().backIt(), expN03.backIt());

    expEit++;
    auto& expE03 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE03.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE03.incidentNodeDst().backIt(), expN04.backIt());

    EXPECT_EQ(&ebcN02.outerFace(), &ebcN02dgF02);
    EXPECT_EQ(&ebcN02.topNode(),   &ebcN02egN01);//n09

    EXPECT_EQ(&ebcN03.outerFace(), &ebcN03dgF02);
    EXPECT_EQ(&ebcN03.topNode(),   &ebcN03egN02);//n01

    EXPECT_EQ(&ebcN04.outerFace(), &ebcN04dgF02);
    EXPECT_EQ(&ebcN04.topNode(),   &ebcN04egN02);//n01

    EXPECT_EQ(&ebcN05.outerFace(), &ebcN05dgF02);
    EXPECT_EQ(&ebcN05.topNode(),   &ebcN05egN02);//n01

}



/**  @brief multiple unification groups at a cut vertex
 */
TEST_F(EmbeddedBCTreeTests, MultipleUGs) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;

    auto  np01 = generate_node(1);
    auto& n01  = g1.addNode(std::move(np01));
    auto  np02 = generate_node(2);
    auto& n02  = g1.addNode(std::move(np02));
    auto  np03 = generate_node(3);
    auto& n03  = g1.addNode(std::move(np03));
    auto  np04 = generate_node(4);
    auto& n04  = g1.addNode(std::move(np04));
    auto  np05 = generate_node(5);
    auto& n05  = g1.addNode(std::move(np05));
    auto  np06 = generate_node(6);
    auto& n06  = g1.addNode(std::move(np06));
    auto  np07 = generate_node(7);
    auto& n07  = g1.addNode(std::move(np07));
    auto  np08 = generate_node(8);
    auto& n08  = g1.addNode(std::move(np08));
    auto  np09 = generate_node(9);
    auto& n09  = g1.addNode(std::move(np09));
    auto  np10 = generate_node(10);
    auto& n10  = g1.addNode(std::move(np10));
    auto  np11 = generate_node(11);
    auto& n11  = g1.addNode(std::move(np11));
    auto  np12 = generate_node(12);
    auto& n12  = g1.addNode(std::move(np12));
    auto  np13 = generate_node(13);
    auto& n13  = g1.addNode(std::move(np13));

    auto  ep01_02 = generate_edge();
    auto& e01_02  = g1.addEdge(std::move(ep01_02),n01,n02);
    auto  ep02_03 = generate_edge();
    auto& e02_03  = g1.addEdge(std::move(ep02_03),n02,n03);
    auto  ep03_01 = generate_edge();
    auto& e03_01  = g1.addEdge(std::move(ep03_01),n03,n01);
    auto  ep01_04 = generate_edge();
    auto& e01_04  = g1.addEdge(std::move(ep01_04),n01,n04);
    auto  ep04_05 = generate_edge();
    auto& e04_05  = g1.addEdge(std::move(ep04_05),n04,n05);
    auto  ep05_01 = generate_edge();
    auto& e05_01  = g1.addEdge(std::move(ep05_01),n05,n01);
    auto  ep01_06 = generate_edge();
    auto& e01_06  = g1.addEdge(std::move(ep01_06),n01,n06);
    auto  ep06_07 = generate_edge();
    auto& e06_07  = g1.addEdge(std::move(ep06_07),n06,n07);
    auto  ep07_01 = generate_edge();
    auto& e07_01  = g1.addEdge(std::move(ep07_01),n07,n01);
    auto  ep01_08 = generate_edge();
    auto& e01_08  = g1.addEdge(std::move(ep01_08),n01,n08);
    auto  ep08_09 = generate_edge();
    auto& e08_09  = g1.addEdge(std::move(ep08_09),n08,n09);
    auto  ep09_01 = generate_edge();
    auto& e09_01  = g1.addEdge(std::move(ep09_01),n09,n01);
    auto  ep01_10 = generate_edge();
    auto& e01_10  = g1.addEdge(std::move(ep01_10),n01,n10);
    auto  ep10_11 = generate_edge();
    auto& e10_11  = g1.addEdge(std::move(ep10_11),n10,n11);
    auto  ep11_01 = generate_edge();
    auto& e11_01  = g1.addEdge(std::move(ep11_01),n11,n01);
    auto  ep01_12 = generate_edge();
    auto& e01_12  = g1.addEdge(std::move(ep01_12),n01,n12);
    auto  ep12_13 = generate_edge();
    auto& e12_13  = g1.addEdge(std::move(ep12_13),n12,n13);
    auto  ep13_01 = generate_edge();
    auto& e13_01  = g1.addEdge(std::move(ep13_01),n13,n01);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 7);
    EXPECT_EQ(ebcT.numEdges(), 6);

    auto ebcNit = ebcT.nodes().first;
    auto& ebcN01 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN02 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN03 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN04 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN05 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN06 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN07 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN01 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN02 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN03 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN04 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN05 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN06 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN07 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    auto ebcEit = ebcT.edges().first;
    auto& ebcE01 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE02 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE03 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE04 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE05 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE06 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));

    auto bcEit = bcT.edges().first;
    auto& bcE01 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE02 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE03 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE04 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE05 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE06 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));

    // Create embeddings for the blocks.

    PlanarDualGraphMaker PDM;

    EmbeddedGraph N02eg;
    DualGraph N02dg;
    PDM.makeDualGraph(bcN02.block(), N02eg, N02dg);
    ebcN02.setEmbedding(move(N02eg), move(N02dg));

    EmbeddedGraph N03eg;
    DualGraph N03dg;
    PDM.makeDualGraph(bcN03.block(), N03eg, N03dg);
    ebcN03.setEmbedding(move(N03eg), move(N03dg));

    EmbeddedGraph N04eg;
    DualGraph N04dg;
    PDM.makeDualGraph(bcN04.block(), N04eg, N04dg);
    ebcN04.setEmbedding(move(N04eg), move(N04dg));

    EmbeddedGraph N05eg;
    DualGraph N05dg;
    PDM.makeDualGraph(bcN05.block(), N05eg, N05dg);
    ebcN05.setEmbedding(move(N05eg), move(N05dg));

    EmbeddedGraph N06eg;
    DualGraph N06dg;
    PDM.makeDualGraph(bcN06.block(), N06eg, N06dg);
    ebcN06.setEmbedding(move(N06eg), move(N06dg));

    EmbeddedGraph N07eg;
    DualGraph N07dg;
    PDM.makeDualGraph(bcN07.block(), N07eg, N07dg);
    ebcN07.setEmbedding(move(N07eg), move(N07dg));


    EXPECT_EQ(ebcN02.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN02.IGBackwardLinkRef(), &bcN02);
    auto& ebcN02eg = ebcN02.embeddedGraph();
    auto& ebcN02dg = ebcN02.dualGraph();
    EXPECT_EQ(ebcN02eg.numNodes(), 3);
    EXPECT_EQ(ebcN02eg.numEdges(), 3);
    EXPECT_EQ(ebcN02dg.numNodes(), 2);
    EXPECT_EQ(ebcN02dg.numEdges(), 3);

    auto  ebcN02egNit = ebcN02eg.nodes().first;
    auto& ebcN02egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN01.IGBackwardLink())));
    auto& ebcN02egN01GN = dynamic_cast<NumNode&>(ebcN02egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN01GN.backIt(), n03.backIt());
    ebcN02egNit++;
    auto& ebcN02egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN02.IGBackwardLink())));
    auto& ebcN02egN02GN = dynamic_cast<NumNode&>(ebcN02egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN02GN.backIt(), n01.backIt());
    ebcN02egNit++;
    auto& ebcN02egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN03.IGBackwardLink())));
    auto& ebcN02egN03GN = dynamic_cast<NumNode&>(ebcN02egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN03GN.backIt(), n02.backIt());

    auto  ebcN02egEit = ebcN02eg.edges().first;
    auto& ebcN02egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE01.IGBackwardLink())));
    auto& ebcN02egE01GE = dynamic_cast<Edge&>(ebcN02egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE01GE.backIt(), e03_01.backIt());
    ebcN02egEit++;
    auto& ebcN02egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE02.IGBackwardLink())));
    auto& ebcN02egE02GE = dynamic_cast<Edge&>(ebcN02egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE02GE.backIt(), e02_03.backIt());
    ebcN02egEit++;
    auto& ebcN02egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE03.IGBackwardLink())));
    auto& ebcN02egE03GE = dynamic_cast<Edge&>(ebcN02egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE03GE.backIt(), e01_02.backIt());

    auto  ebcN02dgFit = ebcN02dg.nodes().first;
    node_list_it_t ebcN02dgFit010203, ebcN02dgFit030201;
    if (((ebcN02egE01.mHalfEdge1.mEmbeddedFace == ebcN02dgFit) &&
         (ebcN02egE01.mHalfEdge1.mSrcNode      == ebcN02egN01.backIt()))||
        ((ebcN02egE01.mHalfEdge2.mEmbeddedFace == ebcN02dgFit) &&
         (ebcN02egE01.mHalfEdge2.mSrcNode      == ebcN02egN01.backIt()))   ) {
        ebcN02dgFit010203 = ebcN02dgFit++;
        ebcN02dgFit030201 = ebcN02dgFit;
    }
    else {
        ebcN02dgFit030201 = ebcN02dgFit++;
        ebcN02dgFit010203 = ebcN02dgFit;
    }
    auto& ebcN02dgF010203=dynamic_cast<EmbeddedFace&>(*(*(ebcN02dgFit010203)));
    /*auto& ebcN02dgF030201=dynamic_cast<EmbeddedFace&>(*(*(ebcN02dgFit030201)));*/

    //auto  ebcN02dgEit = ebcN02dg.edges().first;
    //auto& ebcN02dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN02dgEit)));
    //ebcN02dgEit++;
    //auto& ebcN02dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN02dgEit)));


    EXPECT_EQ(ebcN03.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN03.IGBackwardLinkRef(), &bcN03);
    auto& ebcN03eg = ebcN03.embeddedGraph();
    auto& ebcN03dg = ebcN03.dualGraph();
    EXPECT_EQ(ebcN03eg.numNodes(), 3);
    EXPECT_EQ(ebcN03eg.numEdges(), 3);
    EXPECT_EQ(ebcN03dg.numNodes(), 2);
    EXPECT_EQ(ebcN03dg.numEdges(), 3);

    auto  ebcN03egNit = ebcN03eg.nodes().first;
    auto& ebcN03egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN01.IGBackwardLink())));
    auto& ebcN03egN01GN = dynamic_cast<NumNode&>(ebcN03egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN01GN.backIt(), n05.backIt());
    ebcN03egNit++;
    auto& ebcN03egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN02.IGBackwardLink())));
    auto& ebcN03egN02GN = dynamic_cast<NumNode&>(ebcN03egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN02GN.backIt(), n01.backIt());
    ebcN03egNit++;
    auto& ebcN03egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN03egNit)));
    auto& ebcN03egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN03egN03.IGBackwardLink())));
    auto& ebcN03egN03GN = dynamic_cast<NumNode&>(ebcN03egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egN03GN.backIt(), n04.backIt());

    auto  ebcN03egEit = ebcN03eg.edges().first;
    auto& ebcN03egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE01.IGBackwardLink())));
    auto& ebcN03egE01GE = dynamic_cast<Edge&>(ebcN03egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE01GE.backIt(), e05_01.backIt());
    ebcN03egEit++;
    auto& ebcN03egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE02.IGBackwardLink())));
    auto& ebcN03egE02GE = dynamic_cast<Edge&>(ebcN03egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE02GE.backIt(), e04_05.backIt());
    ebcN03egEit++;
    auto& ebcN03egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN03egEit)));
    auto& ebcN03egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN03egE03.IGBackwardLink())));
    auto& ebcN03egE03GE = dynamic_cast<Edge&>(ebcN03egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN03egE03GE.backIt(), e01_04.backIt());

    auto  ebcN03dgFit = ebcN03dg.nodes().first;
    node_list_it_t ebcN03dgFit010405, ebcN03dgFit050401;
    if (((ebcN03egE01.mHalfEdge1.mEmbeddedFace == ebcN03dgFit) &&
         (ebcN03egE01.mHalfEdge1.mSrcNode == ebcN03egN01.backIt()))||
        ((ebcN03egE01.mHalfEdge2.mEmbeddedFace == ebcN03dgFit) &&
         (ebcN03egE01.mHalfEdge2.mSrcNode == ebcN03egN01.backIt()))   ) {
        ebcN03dgFit010405 = ebcN03dgFit++;
        ebcN03dgFit050401 = ebcN03dgFit;
    }
    else {
        ebcN03dgFit050401 = ebcN03dgFit++;
        ebcN03dgFit010405 = ebcN03dgFit;
    }

    auto& ebcN03dgF010405=dynamic_cast<EmbeddedFace&>(*(*(ebcN03dgFit010405)));
    auto& ebcN03dgF050401=dynamic_cast<EmbeddedFace&>(*(*(ebcN03dgFit050401)));

    //auto  ebcN03dgEit = ebcN03dg.edges().first;
    //auto& ebcN03dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN03dgEit)));
    //ebcN03dgEit++;
    //auto& ebcN03dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN03dgEit)));


    EXPECT_EQ(ebcN04.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN04.IGBackwardLinkRef(), &bcN04);
    auto& ebcN04eg = ebcN04.embeddedGraph();
    auto& ebcN04dg = ebcN04.dualGraph();
    EXPECT_EQ(ebcN04eg.numNodes(), 3);
    EXPECT_EQ(ebcN04eg.numEdges(), 3);
    EXPECT_EQ(ebcN04dg.numNodes(), 2);
    EXPECT_EQ(ebcN04dg.numEdges(), 3);

    auto  ebcN04egNit = ebcN04eg.nodes().first;
    auto& ebcN04egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN01.IGBackwardLink())));
    auto& ebcN04egN01GN = dynamic_cast<NumNode&>(ebcN04egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN01GN.backIt(), n07.backIt());
    ebcN04egNit++;
    auto& ebcN04egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN02.IGBackwardLink())));
    auto& ebcN04egN02GN = dynamic_cast<NumNode&>(ebcN04egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN02GN.backIt(), n01.backIt());
    ebcN04egNit++;
    auto& ebcN04egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN03.IGBackwardLink())));
    auto& ebcN04egN03GN = dynamic_cast<NumNode&>(ebcN04egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN03GN.backIt(), n06.backIt());

    auto  ebcN04egEit = ebcN04eg.edges().first;
    auto& ebcN04egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE01.IGBackwardLink())));
    auto& ebcN04egE01GE = dynamic_cast<Edge&>(ebcN04egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE01GE.backIt(), e07_01.backIt());
    ebcN04egEit++;
    auto& ebcN04egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE02.IGBackwardLink())));
    auto& ebcN04egE02GE = dynamic_cast<Edge&>(ebcN04egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE02GE.backIt(), e06_07.backIt());
    ebcN04egEit++;
    auto& ebcN04egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE03.IGBackwardLink())));
    auto& ebcN04egE03GE = dynamic_cast<Edge&>(ebcN04egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE03GE.backIt(), e01_06.backIt());

    auto  ebcN04dgFit = ebcN04dg.nodes().first;
    node_list_it_t ebcN04dgFit010607, ebcN04dgFit070601;
    if (((ebcN04egE01.mHalfEdge1.mEmbeddedFace == ebcN04dgFit) &&
         (ebcN04egE01.mHalfEdge1.mSrcNode == ebcN04egN01.backIt()))||
        ((ebcN04egE01.mHalfEdge2.mEmbeddedFace == ebcN04dgFit) &&
         (ebcN04egE01.mHalfEdge2.mSrcNode == ebcN04egN01.backIt()))   ) {
        ebcN04dgFit010607 = ebcN04dgFit++;
        ebcN04dgFit070601 = ebcN04dgFit;
    }
    else {
        ebcN04dgFit070601 = ebcN04dgFit++;
        ebcN04dgFit010607 = ebcN04dgFit;
    }
    auto& ebcN04dgF010607=dynamic_cast<EmbeddedFace&>(*(*(ebcN04dgFit010607)));
    /*auto& ebcN04dgF070601=dynamic_cast<EmbeddedFace&>(*(*(ebcN04dgFit070601)));*/

    //auto  ebcN04dgEit = ebcN04dg.edges().first;
    //auto& ebcN04dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN04dgEit)));
    //ebcN04dgEit++;
    //auto& ebcN04dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN04dgEit)));

    EXPECT_EQ(ebcN05.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN05.IGBackwardLinkRef(), &bcN05);
    auto& ebcN05eg = ebcN05.embeddedGraph();
    auto& ebcN05dg = ebcN05.dualGraph();
    EXPECT_EQ(ebcN05eg.numNodes(), 3);
    EXPECT_EQ(ebcN05eg.numEdges(), 3);
    EXPECT_EQ(ebcN05dg.numNodes(), 2);
    EXPECT_EQ(ebcN05dg.numEdges(), 3);

    auto  ebcN05egNit = ebcN05eg.nodes().first;
    auto& ebcN05egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN05egNit)));
    auto& ebcN05egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN05egN01.IGBackwardLink())));
    auto& ebcN05egN01GN = dynamic_cast<NumNode&>(ebcN05egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egN01GN.backIt(), n09.backIt());
    ebcN05egNit++;
    auto& ebcN05egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN05egNit)));
    auto& ebcN05egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN05egN02.IGBackwardLink())));
    auto& ebcN05egN02GN = dynamic_cast<NumNode&>(ebcN05egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egN02GN.backIt(), n01.backIt());
    ebcN05egNit++;
    auto& ebcN05egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN05egNit)));
    auto& ebcN05egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN05egN03.IGBackwardLink())));
    auto& ebcN05egN03GN = dynamic_cast<NumNode&>(ebcN05egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egN03GN.backIt(), n08.backIt());

    auto  ebcN05egEit = ebcN05eg.edges().first;
    auto& ebcN05egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN05egEit)));
    auto& ebcN05egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN05egE01.IGBackwardLink())));
    auto& ebcN05egE01GE = dynamic_cast<Edge&>(ebcN05egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egE01GE.backIt(), e09_01.backIt());
    ebcN05egEit++;
    auto& ebcN05egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN05egEit)));
    auto& ebcN05egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN05egE02.IGBackwardLink())));
    auto& ebcN05egE02GE = dynamic_cast<Edge&>(ebcN05egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egE02GE.backIt(), e08_09.backIt());
    ebcN05egEit++;
    auto& ebcN05egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN05egEit)));
    auto& ebcN05egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN05egE03.IGBackwardLink())));
    auto& ebcN05egE03GE = dynamic_cast<Edge&>(ebcN05egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN05egE03GE.backIt(), e01_08.backIt());

    auto  ebcN05dgFit = ebcN05dg.nodes().first;
    node_list_it_t ebcN05dgFit010809, ebcN05dgFit090801;
    if (((ebcN05egE01.mHalfEdge1.mEmbeddedFace == ebcN05dgFit) &&
         (ebcN05egE01.mHalfEdge1.mSrcNode == ebcN05egN01.backIt()))||
        ((ebcN05egE01.mHalfEdge2.mEmbeddedFace == ebcN05dgFit) &&
         (ebcN05egE01.mHalfEdge2.mSrcNode == ebcN05egN01.backIt()))   ) {
        ebcN05dgFit010809 = ebcN05dgFit++;
        ebcN05dgFit090801 = ebcN05dgFit;
    }
    else {
        ebcN05dgFit090801 = ebcN05dgFit++;
        ebcN05dgFit010809 = ebcN05dgFit;
    }
    auto& ebcN05dgF010809=dynamic_cast<EmbeddedFace&>(*(*(ebcN05dgFit010809)));
    auto& ebcN05dgF090801=dynamic_cast<EmbeddedFace&>(*(*(ebcN05dgFit090801)));

    //auto  ebcN05dgEit = ebcN05dg.edges().first;
    //auto& ebcN05dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN05dgEit)));
    //ebcN05dgEit++;
    //auto& ebcN05dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN05dgEit)));


    EXPECT_EQ(ebcN06.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN06.IGBackwardLinkRef(), &bcN06);
    auto& ebcN06eg = ebcN06.embeddedGraph();
    auto& ebcN06dg = ebcN06.dualGraph();
    EXPECT_EQ(ebcN06eg.numNodes(), 3);
    EXPECT_EQ(ebcN06eg.numEdges(), 3);
    EXPECT_EQ(ebcN06dg.numNodes(), 2);
    EXPECT_EQ(ebcN06dg.numEdges(), 3);

    auto  ebcN06egNit = ebcN06eg.nodes().first;
    auto& ebcN06egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN01.IGBackwardLink())));
    auto& ebcN06egN01GN = dynamic_cast<NumNode&>(ebcN06egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN01GN.backIt(), n11.backIt());
    ebcN06egNit++;
    auto& ebcN06egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN02.IGBackwardLink())));
    auto& ebcN06egN02GN = dynamic_cast<NumNode&>(ebcN06egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN02GN.backIt(), n01.backIt());
    ebcN06egNit++;
    auto& ebcN06egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN03.IGBackwardLink())));
    auto& ebcN06egN03GN = dynamic_cast<NumNode&>(ebcN06egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN03GN.backIt(), n10.backIt());

    auto  ebcN06egEit = ebcN06eg.edges().first;
    auto& ebcN06egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN06egEit)));
    auto& ebcN06egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN06egE01.IGBackwardLink())));
    auto& ebcN06egE01GE = dynamic_cast<Edge&>(ebcN06egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egE01GE.backIt(), e11_01.backIt());
    ebcN06egEit++;
    auto& ebcN06egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN06egEit)));
    auto& ebcN06egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN06egE02.IGBackwardLink())));
    auto& ebcN06egE02GE = dynamic_cast<Edge&>(ebcN06egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egE02GE.backIt(), e10_11.backIt());
    ebcN06egEit++;
    auto& ebcN06egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN06egEit)));
    auto& ebcN06egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN06egE03.IGBackwardLink())));
    auto& ebcN06egE03GE = dynamic_cast<Edge&>(ebcN06egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egE03GE.backIt(), e01_10.backIt());

    auto  ebcN06dgFit = ebcN06dg.nodes().first;
    node_list_it_t ebcN06dgFit011011, ebcN06dgFit111001;
    if (((ebcN06egE01.mHalfEdge1.mEmbeddedFace == ebcN06dgFit) &&
         (ebcN06egE01.mHalfEdge1.mSrcNode == ebcN06egN01.backIt()))||
        ((ebcN06egE01.mHalfEdge2.mEmbeddedFace == ebcN06dgFit) &&
         (ebcN06egE01.mHalfEdge2.mSrcNode == ebcN06egN01.backIt()))   ) {
        ebcN06dgFit011011 = ebcN06dgFit++;
        ebcN06dgFit111001 = ebcN06dgFit;
    }
    else {
        ebcN06dgFit111001 = ebcN06dgFit++;
        ebcN06dgFit011011 = ebcN06dgFit;
    }
    /*auto& ebcN06dgF011011=dynamic_cast<EmbeddedFace&>(*(*(ebcN06dgFit011011)));*/
    auto& ebcN06dgF111001=dynamic_cast<EmbeddedFace&>(*(*(ebcN06dgFit111001)));

    //auto  ebcN06dgEit = ebcN06dg.edges().first;
    //auto& ebcN06dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN06dgEit)));
    //ebcN06dgEit++;
    //auto& ebcN06dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN06dgEit)));


    EXPECT_EQ(ebcN07.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN07.IGBackwardLinkRef(), &bcN07);
    auto& ebcN07eg = ebcN07.embeddedGraph();
    auto& ebcN07dg = ebcN07.dualGraph();
    EXPECT_EQ(ebcN07eg.numNodes(), 3);
    EXPECT_EQ(ebcN07eg.numEdges(), 3);
    EXPECT_EQ(ebcN07dg.numNodes(), 2);
    EXPECT_EQ(ebcN07dg.numEdges(), 3);

    auto  ebcN07egNit = ebcN07eg.nodes().first;
    auto& ebcN07egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN07egNit)));
    auto& ebcN07egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN07egN01.IGBackwardLink())));
    auto& ebcN07egN01GN = dynamic_cast<NumNode&>(ebcN07egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN07egN01GN.backIt(), n13.backIt());
    ebcN07egNit++;
    auto& ebcN07egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN07egNit)));
    auto& ebcN07egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN07egN02.IGBackwardLink())));
    auto& ebcN07egN02GN = dynamic_cast<NumNode&>(ebcN07egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN07egN02GN.backIt(), n01.backIt());
    ebcN07egNit++;
    auto& ebcN07egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN07egNit)));
    auto& ebcN07egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN07egN03.IGBackwardLink())));
    auto& ebcN07egN03GN = dynamic_cast<NumNode&>(ebcN07egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN07egN03GN.backIt(), n12.backIt());

    auto  ebcN07egEit = ebcN07eg.edges().first;
    auto& ebcN07egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN07egEit)));
    auto& ebcN07egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN07egE01.IGBackwardLink())));
    auto& ebcN07egE01GE = dynamic_cast<Edge&>(ebcN07egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN07egE01GE.backIt(), e13_01.backIt());
    ebcN07egEit++;
    auto& ebcN07egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN07egEit)));
    auto& ebcN07egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN07egE02.IGBackwardLink())));
    auto& ebcN07egE02GE = dynamic_cast<Edge&>(ebcN07egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN07egE02GE.backIt(), e12_13.backIt());
    ebcN07egEit++;
    auto& ebcN07egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN07egEit)));
    auto& ebcN07egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN07egE03.IGBackwardLink())));
    auto& ebcN07egE03GE = dynamic_cast<Edge&>(ebcN07egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN07egE03GE.backIt(), e01_12.backIt());

    auto  ebcN07dgFit = ebcN07dg.nodes().first;
    node_list_it_t ebcN07dgFit011213, ebcN07dgFit131201;
    if (((ebcN07egE01.mHalfEdge1.mEmbeddedFace == ebcN07dgFit) &&
         (ebcN07egE01.mHalfEdge1.mSrcNode == ebcN07egN01.backIt()))||
        ((ebcN07egE01.mHalfEdge2.mEmbeddedFace == ebcN07dgFit) &&
         (ebcN07egE01.mHalfEdge2.mSrcNode == ebcN07egN01.backIt()))   ) {
        ebcN07dgFit011213 = ebcN07dgFit++;
        ebcN07dgFit131201 = ebcN07dgFit;
    }
    else {
        ebcN07dgFit131201 = ebcN07dgFit++;
        ebcN07dgFit011213 = ebcN07dgFit;
    }
    /*auto& ebcN07dgF011213=dynamic_cast<EmbeddedFace&>(*(*(ebcN07dgFit011213)));*/
    auto& ebcN07dgF131201=dynamic_cast<EmbeddedFace&>(*(*(ebcN07dgFit131201)));

    //auto  ebcN07dgEit = ebcN07dg.edges().first;
    //auto& ebcN07dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN07dgEit)));
    //ebcN07dgEit++;
    //auto& ebcN07dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN07dgEit)));

    EXPECT_EQ(ebcN01.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN01.IGBackwardLinkRef(), &bcN01);
    EXPECT_EQ(&bcN01.IGBackwardLinkRef(), &n01);
    EXPECT_THROW(ebcN01.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN01.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN01.numUnificationGroups(), 0);

    EXPECT_EQ(&ebcE01.IGBackwardLinkRef(), &bcE01);
    EXPECT_EQ(&ebcE01.incidentNodeBlockType(), &ebcN02);

    EXPECT_EQ(&ebcE02.IGBackwardLinkRef(), &bcE02);
    EXPECT_EQ(&ebcE02.incidentNodeBlockType(), &ebcN03);

    EXPECT_EQ(&ebcE03.IGBackwardLinkRef(), &bcE03);
    EXPECT_EQ(&ebcE03.incidentNodeBlockType(), &ebcN04);

    EXPECT_EQ(&ebcE04.IGBackwardLinkRef(), &bcE04);
    EXPECT_EQ(&ebcE04.incidentNodeBlockType(), &ebcN05);

    EXPECT_EQ(&ebcE05.IGBackwardLinkRef(), &bcE05);
    EXPECT_EQ(&ebcE05.incidentNodeBlockType(), &ebcN06);

    EXPECT_EQ(&ebcE06.IGBackwardLinkRef(), &bcE06);
    EXPECT_EQ(&ebcE06.incidentNodeBlockType(), &ebcN07);

    // Manually set an embedding.

    UnificationGroup UG01, UG02, UG03;

    UnificationFace UF_010203(
                                  ebcN02.backIt(), 
                                  ebcN02dgF010203.backIt(),
                                  ebcN02egN02.backIt(),
                                  ebcN02egE01.backIt(),
                                  ebcN02egE03.backIt()
                             );

    UnificationFace UF_010405_1(
                                  ebcN03.backIt(), 
                                  ebcN03dgF010405.backIt(),
                                  ebcN03egN02.backIt(),
                                  ebcN03egE03.backIt(),
                                  ebcN03egE01.backIt()
                             );

    UnificationFace UF_010405_2(
                                  ebcN03.backIt(), 
                                  ebcN03dgF050401.backIt(),
                                  ebcN03egN02.backIt(),
                                  ebcN03egE01.backIt(),
                                  ebcN03egE03.backIt()
                             );

    UnificationFace UF_010607(
                                  ebcN04.backIt(), 
                                  ebcN04dgF010607.backIt(),
                                  ebcN04egN02.backIt(),
                                  ebcN04egE01.backIt(),
                                  ebcN04egE03.backIt()
                             );

    UnificationFace UF_010809_1(
                                  ebcN05.backIt(), 
                                  ebcN05dgF090801.backIt(),
                                  ebcN05egN02.backIt(),
                                  ebcN05egE01.backIt(),
                                  ebcN05egE03.backIt()
                             );

    UnificationFace UF_010809_2(
                                  ebcN05.backIt(), 
                                  ebcN05dgF010809.backIt(),
                                  ebcN05egN02.backIt(),
                                  ebcN05egE03.backIt(),
                                  ebcN05egE01.backIt()
                             );


    UnificationFace UF_011011(
                                  ebcN06.backIt(), 
                                  ebcN06dgF111001.backIt(),
                                  ebcN06egN02.backIt(),
                                  ebcN06egE03.backIt(),
                                  ebcN06egE01.backIt()
                             );

    UnificationFace UF_011213(
                                  ebcN07.backIt(), 
                                  ebcN07dgF131201.backIt(),
                                  ebcN07egN02.backIt(),
                                  ebcN07egE01.backIt(),
                                  ebcN07egE03.backIt()
                             );

    EXPECT_EQ( UF_010203.roleOfECWReversed(),   true);
    EXPECT_EQ( UF_010405_1.roleOfECWReversed(), false);
    EXPECT_EQ( UF_010405_2.roleOfECWReversed(), false);
    EXPECT_EQ( UF_010607.roleOfECWReversed(),   true);
    EXPECT_EQ( UF_010809_1.roleOfECWReversed(), false);
    EXPECT_EQ( UF_010809_2.roleOfECWReversed(), false);
    EXPECT_EQ( UF_011011.roleOfECWReversed(),   true);
    EXPECT_EQ( UF_011213.roleOfECWReversed(),   false);

    UG01.push(UF_011011);
    UG01.push(UF_011213);
    UG01.push(UF_010809_1);

    UG02.push(UF_010607);
    UG02.push(UF_010809_2);
    UG02.push(UF_010405_1);

    UG03.push(UF_010405_2);
    UG03.push(UF_010203);

    ebcN01.pushBackUnificationGroup(std::move(UG01));
    ebcN01.pushBackUnificationGroup(std::move(UG02));
    ebcN01.pushBackUnificationGroup(std::move(UG03));

    ebcT.findGeometricEmbedding(ebcN02.backIt());

    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN02);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();

    auto& expTR = expT.root();

    EXPECT_EQ(expT.numNodes(),6);
    EXPECT_EQ(expT.numEdges(),5);

    auto expNit = expT.nodes().first;
    auto& expN01 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN02 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN03 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN04 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN05 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN06 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));

    EXPECT_EQ(&expTR, &expN01);

    EXPECT_EQ(&expN01.IGBackwardLinkRef(), &ebcN02);
    EXPECT_EQ(expN01.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN01UGs=expN01.sortedUnificationGroups();
    EXPECT_EQ(expN01UGs.size(), 1);
    EXPECT_EQ(expN01UGs[0].absorbeesSize(), 1);
    EXPECT_EQ(expN01UGs[0].absorber(),     UF_010203);
    EXPECT_EQ(expN01UGs[0].absorbeeAt(0),  UF_010405_2);

    EXPECT_EQ(&expN02.IGBackwardLinkRef(), &ebcN03);
    EXPECT_EQ(expN02.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN02UGs=expN02.sortedUnificationGroups();
    EXPECT_EQ(expN02UGs.size(), 1);
    EXPECT_EQ(expN02UGs[0].absorbeesSize(), 2);
    EXPECT_EQ(expN02UGs[0].absorber(),     UF_010405_1);
    EXPECT_EQ(expN02UGs[0].absorbeeAt(0),  UF_010607);
    EXPECT_EQ(expN02UGs[0].absorbeeAt(1),  UF_010809_2);

    EXPECT_EQ(&expN03.IGBackwardLinkRef(), &ebcN04);
    EXPECT_EQ(expN03.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN03UGs=expN03.sortedUnificationGroups();
    EXPECT_EQ(expN03UGs.size(), 0);

    EXPECT_EQ(&expN04.IGBackwardLinkRef(), &ebcN05);
    EXPECT_EQ(expN04.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN04UGs=expN04.sortedUnificationGroups();
    EXPECT_EQ(expN04UGs.size(), 1);
    EXPECT_EQ(expN04UGs[0].absorbeesSize(), 2);
    EXPECT_EQ(expN04UGs[0].absorber(),     UF_010809_1);
    EXPECT_EQ(expN04UGs[0].absorbeeAt(0),  UF_011011);
    EXPECT_EQ(expN04UGs[0].absorbeeAt(1),  UF_011213);

    EXPECT_EQ(&expN05.IGBackwardLinkRef(), &ebcN06);
    EXPECT_EQ(expN05.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN05UGs=expN05.sortedUnificationGroups();
    EXPECT_EQ(expN05UGs.size(), 0);

    EXPECT_EQ(&expN06.IGBackwardLinkRef(), &ebcN07);
    EXPECT_EQ(expN06.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN06UGs=expN06.sortedUnificationGroups();
    EXPECT_EQ(expN06UGs.size(), 0);

    auto expEit = expT.edges().first;
    auto& expE01 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE01.incidentNodeSrc().backIt(), expN01.backIt());
    EXPECT_EQ(expE01.incidentNodeDst().backIt(), expN02.backIt());
    expEit++;

    auto& expE02 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE02.incidentNodeSrc().backIt(), expN02.backIt());
    EXPECT_EQ(expE02.incidentNodeDst().backIt(), expN03.backIt());

    expEit++;
    auto& expE03 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE03.incidentNodeSrc().backIt(), expN02.backIt());
    EXPECT_EQ(expE03.incidentNodeDst().backIt(), expN04.backIt());

    expEit++;
    auto& expE04 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE04.incidentNodeSrc().backIt(), expN04.backIt());
    EXPECT_EQ(expE04.incidentNodeDst().backIt(), expN05.backIt());

    expEit++;
    auto& expE05 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE05.incidentNodeSrc().backIt(), expN04.backIt());
    EXPECT_EQ(expE05.incidentNodeDst().backIt(), expN06.backIt());

    EXPECT_EQ(&ebcN02.outerFace(), &ebcN02dgF010203);
    EXPECT_EQ(&ebcN02.topNode(),   &ebcN02egN01);

    EXPECT_EQ(&ebcN03.outerFace(), &ebcN03dgF050401);
    EXPECT_EQ(&ebcN03.topNode(),   &ebcN03egN02);

    EXPECT_EQ(&ebcN04.outerFace(), &ebcN04dgF010607);
    EXPECT_EQ(&ebcN04.topNode(),   &ebcN04egN02);

    EXPECT_EQ(&ebcN05.outerFace(), &ebcN05dgF010809);
    EXPECT_EQ(&ebcN05.topNode(),   &ebcN05egN02);

    EXPECT_EQ(&ebcN06.outerFace(), &ebcN06dgF111001);
    EXPECT_EQ(&ebcN06.topNode(),   &ebcN06egN02);

    EXPECT_EQ(&ebcN07.outerFace(), &ebcN07dgF131201);
    EXPECT_EQ(&ebcN07.topNode(),   &ebcN07egN02);

}


/**  @brief realistic example
 */
TEST_F(EmbeddedBCTreeTests, Realistic1) {

    auto  gp1 = generate_graph();
    auto& g1  = *gp1;

    auto  np01 = generate_node(1);
    auto& n01  = g1.addNode(std::move(np01));
    auto  np02 = generate_node(2);
    auto& n02  = g1.addNode(std::move(np02));
    auto  np03 = generate_node(3);
    auto& n03  = g1.addNode(std::move(np03));
    auto  np04 = generate_node(4);
    auto& n04  = g1.addNode(std::move(np04));
    auto  np05 = generate_node(5);
    auto& n05  = g1.addNode(std::move(np05));
    auto  np06 = generate_node(6);
    auto& n06  = g1.addNode(std::move(np06));
    auto  np07 = generate_node(7);
    auto& n07  = g1.addNode(std::move(np07));
    auto  np08 = generate_node(8);
    auto& n08  = g1.addNode(std::move(np08));
    auto  np09 = generate_node(9);
    auto& n09  = g1.addNode(std::move(np09));
    auto  np10 = generate_node(10);
    auto& n10  = g1.addNode(std::move(np10));
    auto  np11 = generate_node(11);
    auto& n11  = g1.addNode(std::move(np11));
    auto  np12 = generate_node(12);
    auto& n12  = g1.addNode(std::move(np12));
    auto  np13 = generate_node(13);
    auto& n13  = g1.addNode(std::move(np13));

    auto  ep01_02 = generate_edge();
    auto& e01_02  = g1.addEdge(std::move(ep01_02),n01,n02);
    auto  ep01_03 = generate_edge();
    auto& e01_03  = g1.addEdge(std::move(ep01_03),n01,n03);
    auto  ep01_04 = generate_edge();
    auto& e01_04  = g1.addEdge(std::move(ep01_04),n01,n04);
    auto  ep01_05 = generate_edge();
    auto& e01_05  = g1.addEdge(std::move(ep01_05),n01,n05);
    auto  ep03_04 = generate_edge();
    auto& e03_04  = g1.addEdge(std::move(ep03_04),n03,n04);
    auto  ep04_07 = generate_edge();
    auto& e04_07  = g1.addEdge(std::move(ep04_07),n04,n07);
    auto  ep04_08 = generate_edge();
    auto& e04_08  = g1.addEdge(std::move(ep04_08),n04,n08);
    auto  ep04_10 = generate_edge();
    auto& e04_10  = g1.addEdge(std::move(ep04_10),n04,n10);
    auto  ep05_06 = generate_edge();
    auto& e05_06  = g1.addEdge(std::move(ep05_06),n05,n06);
    auto  ep05_07 = generate_edge();
    auto& e05_07  = g1.addEdge(std::move(ep05_07),n05,n07);
    auto  ep06_07 = generate_edge();
    auto& e06_07  = g1.addEdge(std::move(ep06_07),n06,n07);
    auto  ep08_09 = generate_edge();
    auto& e08_09  = g1.addEdge(std::move(ep08_09),n08,n09);
    auto  ep10_11 = generate_edge();
    auto& e10_11  = g1.addEdge(std::move(ep10_11),n10,n11);
    auto  ep10_12 = generate_edge();
    auto& e10_12  = g1.addEdge(std::move(ep10_12),n10,n12);
    auto  ep10_13 = generate_edge();
    auto& e10_13  = g1.addEdge(std::move(ep10_13),n10,n13);
    auto  ep11_12 = generate_edge();
    auto& e11_12  = g1.addEdge(std::move(ep11_12),n11,n12);

    BiconnectedDecomposer decomp;
    BCTree bcT = decomp.decompose(g1);

    EmbeddedBCTree ebcT(bcT);

    ebcT.makeDefaultEmbedding();

    EXPECT_EQ(&(ebcT.bcTree()),&bcT);
    EXPECT_EQ(ebcT.numNodes(), 11);
    EXPECT_EQ(ebcT.numEdges(), 10);

    auto ebcNit = ebcT.nodes().first;
    auto& ebcN01 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN02 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN03 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN04 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN05 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN06 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN07 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN08 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN09 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN10 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));
    ebcNit++;
    auto& ebcN11 = dynamic_cast<EmbeddedBCTreeNode&>(*(*(ebcNit)));

    auto bcNit = bcT.nodes().first;
    auto& bcN01 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN02 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN03 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN04 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN05 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN06 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN07 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN08 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN09 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN10 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));
    bcNit++;
    auto& bcN11 = dynamic_cast<BCTreeNode&>(*(*(bcNit)));

    auto ebcEit = ebcT.edges().first;
    auto& ebcE01 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE02 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE03 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE04 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE05 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE06 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE07 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE08 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE09 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));
    ebcEit++;
    auto& ebcE10 = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(ebcEit)));

    auto bcEit = bcT.edges().first;
    auto& bcE01 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE02 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE03 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE04 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE05 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE06 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE07 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE08 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE09 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));
    bcEit++;
    auto& bcE10 = dynamic_cast<BCTreeEdge&>(*(*(bcEit)));


    EXPECT_EQ(ebcN01.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN01.IGBackwardLinkRef(), &bcN01);
    EXPECT_EQ(&bcN01.IGBackwardLinkRef(), &n01);
    EXPECT_THROW(ebcN01.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN01.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN01.numUnificationGroups(), 1);
    auto& ebcN01UG1 = ebcN01.unificationGroup(0);


    EXPECT_EQ(ebcN02.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN02.IGBackwardLinkRef(), &bcN02);
    auto& ebcN02eg = ebcN02.embeddedGraph();
    auto& ebcN02dg = ebcN02.dualGraph();
    EXPECT_EQ(ebcN02eg.numNodes(), 2);
    EXPECT_EQ(ebcN02eg.numEdges(), 1);
    EXPECT_EQ(ebcN02dg.numNodes(), 1);
    EXPECT_EQ(ebcN02dg.numEdges(), 1);

    auto  ebcN02egNit = ebcN02eg.nodes().first;
    auto& ebcN02egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN01.IGBackwardLink())));
    auto& ebcN02egN01GN = dynamic_cast<NumNode&>(ebcN02egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN01GN.backIt(), n01.backIt());
    ebcN02egNit++;
    auto& ebcN02egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN02egNit)));
    auto& ebcN02egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN02egN02.IGBackwardLink())));
    auto& ebcN02egN02GN = dynamic_cast<NumNode&>(ebcN02egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egN02GN.backIt(), n02.backIt());

    auto  ebcN02egEit = ebcN02eg.edges().first;
    auto& ebcN02egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN02egEit)));
    auto& ebcN02egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN02egE01.IGBackwardLink())));
    auto& ebcN02egE01GE = dynamic_cast<Edge&>(ebcN02egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN02egE01GE.backIt(), e01_02.backIt());

    auto  ebcN02dgFit = ebcN02dg.nodes().first;
    auto& ebcN02dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN02dgFit)));

    //auto  ebcN02dgEit = ebcN02dg.edges().first;
    //auto& ebcN02dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN02dgEit)));


    EXPECT_EQ(ebcN03.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN03.IGBackwardLinkRef(), &bcN03);
    EXPECT_EQ(&bcN03.IGBackwardLinkRef(), &n08);
    EXPECT_THROW(ebcN03.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN03.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN03.numUnificationGroups(), 1);
    auto& ebcN03UG1 = ebcN03.unificationGroup(0);


    EXPECT_EQ(ebcN04.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN04.IGBackwardLinkRef(), &bcN04);
    auto& ebcN04eg = ebcN04.embeddedGraph();
    auto& ebcN04dg = ebcN04.dualGraph();
    EXPECT_EQ(ebcN04eg.numNodes(), 2);
    EXPECT_EQ(ebcN04eg.numEdges(), 1);
    EXPECT_EQ(ebcN04dg.numNodes(), 1);
    EXPECT_EQ(ebcN04dg.numEdges(), 1);

    auto  ebcN04egNit = ebcN04eg.nodes().first;
    auto& ebcN04egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN01.IGBackwardLink())));
    auto& ebcN04egN01GN = dynamic_cast<NumNode&>(ebcN04egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN01GN.backIt(), n08.backIt());
    ebcN04egNit++;
    auto& ebcN04egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN04egNit)));
    auto& ebcN04egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN04egN02.IGBackwardLink())));
    auto& ebcN04egN02GN = dynamic_cast<NumNode&>(ebcN04egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egN02GN.backIt(), n09.backIt());

    auto  ebcN04egEit = ebcN04eg.edges().first;
    auto& ebcN04egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN04egEit)));
    auto& ebcN04egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN04egE01.IGBackwardLink())));
    auto& ebcN04egE01GE = dynamic_cast<Edge&>(ebcN04egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN04egE01GE.backIt(), e08_09.backIt());

    auto  ebcN04dgFit = ebcN04dg.nodes().first;
    auto& ebcN04dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN04dgFit)));

    //auto  ebcN04dgEit = ebcN04dg.edges().first;
    //auto& ebcN04dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN04dgEit)));


    EXPECT_EQ(ebcN05.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN05.IGBackwardLinkRef(), &bcN05);
    EXPECT_EQ(&bcN05.IGBackwardLinkRef(), &n04);
    EXPECT_THROW(ebcN05.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN05.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN05.numUnificationGroups(), 1);
    auto& ebcN05UG1 = ebcN05.unificationGroup(0);


    EXPECT_EQ(ebcN06.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN06.IGBackwardLinkRef(), &bcN06);
    auto& ebcN06eg = ebcN06.embeddedGraph();
    auto& ebcN06dg = ebcN06.dualGraph();
    EXPECT_EQ(ebcN06eg.numNodes(), 2);
    EXPECT_EQ(ebcN06eg.numEdges(), 1);
    EXPECT_EQ(ebcN06dg.numNodes(), 1);
    EXPECT_EQ(ebcN06dg.numEdges(), 1);

    auto  ebcN06egNit = ebcN06eg.nodes().first;
    auto& ebcN06egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN01.IGBackwardLink())));
    auto& ebcN06egN01GN = dynamic_cast<NumNode&>(ebcN06egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN01GN.backIt(), n04.backIt());
    ebcN06egNit++;
    auto& ebcN06egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN06egNit)));
    auto& ebcN06egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN06egN02.IGBackwardLink())));
    auto& ebcN06egN02GN = dynamic_cast<NumNode&>(ebcN06egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egN02GN.backIt(), n08.backIt());

    auto  ebcN06egEit = ebcN06eg.edges().first;
    auto& ebcN06egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN06egEit)));
    auto& ebcN06egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN06egE01.IGBackwardLink())));
    auto& ebcN06egE01GE = dynamic_cast<Edge&>(ebcN06egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN06egE01GE.backIt(), e04_08.backIt());

    auto  ebcN06dgFit = ebcN06dg.nodes().first;
    auto& ebcN06dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN06dgFit)));

    //auto  ebcN06dgEit = ebcN06dg.edges().first;
    //auto& ebcN06dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN06dgEit)));


    EXPECT_EQ(ebcN07.type(),      BCTreeNode::CutVertexType);
    EXPECT_EQ(&ebcN07.IGBackwardLinkRef(), &bcN07);
    EXPECT_EQ(&bcN07.IGBackwardLinkRef(), &n10);
    EXPECT_THROW(ebcN07.embeddedGraph(), std::invalid_argument);
    EXPECT_THROW(ebcN07.dualGraph(), std::invalid_argument);
    EXPECT_EQ(ebcN07.numUnificationGroups(), 1);
    auto& ebcN07UG1 = ebcN07.unificationGroup(0);


    EXPECT_EQ(ebcN08.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN08.IGBackwardLinkRef(), &bcN08);
    auto& ebcN08eg = ebcN08.embeddedGraph();
    auto& ebcN08dg = ebcN08.dualGraph();
    EXPECT_EQ(ebcN08eg.numNodes(), 3);
    EXPECT_EQ(ebcN08eg.numEdges(), 3);
    EXPECT_EQ(ebcN08dg.numNodes(), 2);
    EXPECT_EQ(ebcN08dg.numEdges(), 3);

    auto  ebcN08egNit = ebcN08eg.nodes().first;
    auto& ebcN08egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN08egNit)));
    auto& ebcN08egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN08egN01.IGBackwardLink())));
    auto& ebcN08egN01GN = dynamic_cast<NumNode&>(ebcN08egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egN01GN.backIt(), n10.backIt());
    ebcN08egNit++;
    auto& ebcN08egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN08egNit)));
    auto& ebcN08egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN08egN02.IGBackwardLink())));
    auto& ebcN08egN02GN = dynamic_cast<NumNode&>(ebcN08egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egN02GN.backIt(), n12.backIt());
    ebcN08egNit++;
    auto& ebcN08egN03 = dynamic_cast<EmbeddedNode&>(*(*(ebcN08egNit)));
    auto& ebcN08egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN08egN03.IGBackwardLink())));
    auto& ebcN08egN03GN = dynamic_cast<NumNode&>(ebcN08egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egN03GN.backIt(), n11.backIt());

    auto  ebcN08egEit = ebcN08eg.edges().first;
    auto& ebcN08egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN08egEit)));
    auto& ebcN08egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN08egE01.IGBackwardLink())));
    auto& ebcN08egE01GE = dynamic_cast<Edge&>(ebcN08egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egE01GE.backIt(), e10_12.backIt());
    ebcN08egEit++;
    auto& ebcN08egE02   = dynamic_cast<EmbeddedEdge&>(*(*(ebcN08egEit)));
    auto& ebcN08egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN08egE02.IGBackwardLink())));
    auto& ebcN08egE02GE = dynamic_cast<Edge&>(ebcN08egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egE02GE.backIt(), e11_12.backIt());
    ebcN08egEit++;
    auto& ebcN08egE03   = dynamic_cast<EmbeddedEdge&>(*(*(ebcN08egEit)));
    auto& ebcN08egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN08egE03.IGBackwardLink())));
    auto& ebcN08egE03GE = dynamic_cast<Edge&>(ebcN08egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN08egE03GE.backIt(), e10_11.backIt());

    auto  ebcN08dgFit = ebcN08dg.nodes().first;
    auto& ebcN08dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN08dgFit)));
    ebcN08dgFit++;
    /*auto& ebcN08dgF02 = dynamic_cast<EmbeddedFace&>(*(*(ebcN08dgFit)));*/

    auto eit = ebcN08dgF01.mCycleEdges.begin();
    EXPECT_EQ(ebcN08egE03.backIt(), 
              dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN08egE02.backIt(), 
              dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN08egE01.backIt(), 
              dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    // ebcN08dgF01 : 10 -> 11-> 12
    // ebcN08dgF02 : 10 -> 12-> 11

    //auto  ebcN08dgEit = ebcN08dg.edges().first;
    //auto& ebcN08dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN08dgEit)));
    //ebcN08dgEit++;
    //auto& ebcN08dgE02 = dynamic_cast<DualEdge&>(*(*(ebcN08dgEit)));
    //ebcN08dgEit++;
    //auto& ebcN08dgE03 = dynamic_cast<DualEdge&>(*(*(ebcN08dgEit)));


    EXPECT_EQ(ebcN09.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN09.IGBackwardLinkRef(), &bcN09);
    auto& ebcN09eg = ebcN09.embeddedGraph();
    auto& ebcN09dg = ebcN09.dualGraph();
    EXPECT_EQ(ebcN09eg.numNodes(), 2);
    EXPECT_EQ(ebcN09eg.numEdges(), 1);
    EXPECT_EQ(ebcN09dg.numNodes(), 1);
    EXPECT_EQ(ebcN09dg.numEdges(), 1);

    auto  ebcN09egNit = ebcN09eg.nodes().first;
    auto& ebcN09egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN09egNit)));
    auto& ebcN09egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN09egN01.IGBackwardLink())));
    auto& ebcN09egN01GN = dynamic_cast<NumNode&>(ebcN09egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN09egN01GN.backIt(), n10.backIt());
    ebcN09egNit++;
    auto& ebcN09egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN09egNit)));
    auto& ebcN09egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN09egN02.IGBackwardLink())));
    auto& ebcN09egN02GN = dynamic_cast<NumNode&>(ebcN09egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN09egN02GN.backIt(), n13.backIt());

    auto  ebcN09egEit = ebcN09eg.edges().first;
    auto& ebcN09egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN09egEit)));
    auto& ebcN09egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN09egE01.IGBackwardLink())));
    auto& ebcN09egE01GE = dynamic_cast<Edge&>(ebcN09egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN09egE01GE.backIt(), e10_13.backIt());

    auto  ebcN09dgFit = ebcN09dg.nodes().first;
    auto& ebcN09dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN09dgFit)));

    //auto  ebcN09dgEit = ebcN09dg.edges().first;
    //auto& ebcN09dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN09dgEit)));


    EXPECT_EQ(ebcN10.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN10.IGBackwardLinkRef(), &bcN10);
    auto& ebcN10eg = ebcN10.embeddedGraph();
    auto& ebcN10dg = ebcN10.dualGraph();
    EXPECT_EQ(ebcN10eg.numNodes(), 2);
    EXPECT_EQ(ebcN10eg.numEdges(), 1);
    EXPECT_EQ(ebcN10dg.numNodes(), 1);
    EXPECT_EQ(ebcN10dg.numEdges(), 1);

    auto  ebcN10egNit = ebcN10eg.nodes().first;
    auto& ebcN10egN01 = dynamic_cast<EmbeddedNode&>(*(*(ebcN10egNit)));
    auto& ebcN10egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN10egN01.IGBackwardLink())));
    auto& ebcN10egN01GN = dynamic_cast<NumNode&>(ebcN10egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN10egN01GN.backIt(), n04.backIt());
    ebcN10egNit++;
    auto& ebcN10egN02 = dynamic_cast<EmbeddedNode&>(*(*(ebcN10egNit)));
    auto& ebcN10egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN10egN02.IGBackwardLink())));
    auto& ebcN10egN02GN = dynamic_cast<NumNode&>(ebcN10egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN10egN02GN.backIt(), n10.backIt());

    auto  ebcN10egEit = ebcN10eg.edges().first;
    auto& ebcN10egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN10egEit)));
    auto& ebcN10egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN10egE01.IGBackwardLink())));
    auto& ebcN10egE01GE = dynamic_cast<Edge&>(ebcN10egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN10egE01GE.backIt(), e04_10.backIt());

    auto  ebcN10dgFit = ebcN10dg.nodes().first;
    auto& ebcN10dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN10dgFit)));

    //auto  ebcN10dgEit = ebcN10dg.edges().first;
    //auto& ebcN10dgE01 = dynamic_cast<DualEdge&>(*(*(ebcN10dgEit)));


    EXPECT_EQ(ebcN11.type(),      BCTreeNode::BlockType);
    EXPECT_EQ(&ebcN11.IGBackwardLinkRef(), &bcN11);
    auto& ebcN11eg = ebcN11.embeddedGraph();
    auto& ebcN11dg = ebcN11.dualGraph();
    EXPECT_EQ(ebcN11eg.numNodes(), 6);
    EXPECT_EQ(ebcN11eg.numEdges(), 8);
    EXPECT_EQ(ebcN11dg.numNodes(), 4);
    EXPECT_EQ(ebcN11dg.numEdges(), 8);

    auto  ebcN11egNit = ebcN11eg.nodes().first;
    auto& ebcN11egN01   = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN01BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN01.IGBackwardLink())));
    auto& ebcN11egN01GN = dynamic_cast<NumNode&>(ebcN11egN01BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN01GN.backIt(), n06.backIt());
    ebcN11egNit++;
    auto& ebcN11egN02   = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN02BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN02.IGBackwardLink())));
    auto& ebcN11egN02GN = dynamic_cast<NumNode&>(ebcN11egN02BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN02GN.backIt(), n07.backIt());
    ebcN11egNit++;
    auto& ebcN11egN03   = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN03BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN03.IGBackwardLink())));
    auto& ebcN11egN03GN = dynamic_cast<NumNode&>(ebcN11egN03BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN03GN.backIt(), n05.backIt());
    ebcN11egNit++;
    auto& ebcN11egN04   = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN04BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN04.IGBackwardLink())));
    auto& ebcN11egN04GN = dynamic_cast<NumNode&>(ebcN11egN04BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN04GN.backIt(), n01.backIt());
    ebcN11egNit++;
    auto& ebcN11egN05   = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN05BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN05.IGBackwardLink())));
    auto& ebcN11egN05GN = dynamic_cast<NumNode&>(ebcN11egN05BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN05GN.backIt(), n04.backIt());
    ebcN11egNit++;
    auto& ebcN11egN06   = dynamic_cast<EmbeddedNode&>(*(*(ebcN11egNit)));
    auto& ebcN11egN06BN =dynamic_cast<BlockNode&>(*(*(ebcN11egN06.IGBackwardLink())));
    auto& ebcN11egN06GN = dynamic_cast<NumNode&>(ebcN11egN06BN.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egN06GN.backIt(), n03.backIt());


    auto  ebcN11egEit = ebcN11eg.edges().first;
    auto& ebcN11egE01 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE01BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE01.IGBackwardLink())));
    auto& ebcN11egE01GE = dynamic_cast<Edge&>(ebcN11egE01BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE01GE.backIt(), e06_07.backIt());
    ebcN11egEit++;
    auto& ebcN11egE02 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE02BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE02.IGBackwardLink())));
    auto& ebcN11egE02GE = dynamic_cast<Edge&>(ebcN11egE02BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE02GE.backIt(), e05_06.backIt());
    ebcN11egEit++;
    auto& ebcN11egE03 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE03BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE03.IGBackwardLink())));
    auto& ebcN11egE03GE = dynamic_cast<Edge&>(ebcN11egE03BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE03GE.backIt(), e01_05.backIt());
    ebcN11egEit++;
    auto& ebcN11egE04 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE04BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE04.IGBackwardLink())));
    auto& ebcN11egE04GE = dynamic_cast<Edge&>(ebcN11egE04BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE04GE.backIt(), e05_07.backIt());
    ebcN11egEit++;
    auto& ebcN11egE05 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE05BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE05.IGBackwardLink())));
    auto& ebcN11egE05GE = dynamic_cast<Edge&>(ebcN11egE05BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE05GE.backIt(), e04_07.backIt());
    ebcN11egEit++;
    auto& ebcN11egE06 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE06BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE06.IGBackwardLink())));
    auto& ebcN11egE06GE = dynamic_cast<Edge&>(ebcN11egE06BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE06GE.backIt(), e01_04.backIt());
    ebcN11egEit++;
    auto& ebcN11egE07 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE07BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE07.IGBackwardLink())));
    auto& ebcN11egE07GE = dynamic_cast<Edge&>(ebcN11egE07BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE07GE.backIt(), e03_04.backIt());
    ebcN11egEit++;
    auto& ebcN11egE08 = dynamic_cast<EmbeddedEdge&>(*(*(ebcN11egEit)));
    auto& ebcN11egE08BE =dynamic_cast<BlockEdge&>(*(*(ebcN11egE08.IGBackwardLink())));
    auto& ebcN11egE08GE = dynamic_cast<Edge&>(ebcN11egE08BE.IGBackwardLinkRef());
    EXPECT_EQ(ebcN11egE08GE.backIt(), e01_03.backIt());

    auto  ebcN11dgFit = ebcN11dg.nodes().first;
    auto& ebcN11dgF01 = dynamic_cast<EmbeddedFace&>(*(*(ebcN11dgFit)));
    ebcN11dgFit++;
    auto& ebcN11dgF02 = dynamic_cast<EmbeddedFace&>(*(*(ebcN11dgFit)));
    ebcN11dgFit++;
    auto& ebcN11dgF03 = dynamic_cast<EmbeddedFace&>(*(*(ebcN11dgFit)));
    ebcN11dgFit++;
    auto& ebcN11dgF04 = dynamic_cast<EmbeddedFace&>(*(*(ebcN11dgFit)));

    eit = ebcN11dgF01.mCycleEdges.begin();
    EXPECT_EQ(ebcN11egE01.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE04.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE02.backIt(), // 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());

    eit = ebcN11dgF02.mCycleEdges.begin();
    EXPECT_EQ(ebcN11egE02.backIt(), // 05-06
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE03.backIt(), // 01-05
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE08.backIt(), // 01-03
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE07.backIt(), // 03-04
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE05.backIt(), // 04-07
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE01.backIt(), // 06-07
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());

    eit = ebcN11dgF03.mCycleEdges.begin();
    EXPECT_EQ(ebcN11egE05.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE06.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE03.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE04.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());

    eit = ebcN11dgF04.mCycleEdges.begin();
    EXPECT_EQ(ebcN11egE06.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE07.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());
    EXPECT_EQ(ebcN11egE08.backIt(), 
                        dynamic_cast<EmbeddedEdge&>(*(**(eit++))).backIt());

    EXPECT_EQ(&ebcE01.IGBackwardLinkRef(), &bcE01);
    EXPECT_EQ(&ebcE01.incidentNodeBlockType(), &ebcN02);

    EXPECT_EQ(&ebcE02.IGBackwardLinkRef(), &bcE02);
    EXPECT_EQ(&ebcE02.incidentNodeBlockType(), &ebcN04);

    EXPECT_EQ(&ebcE03.IGBackwardLinkRef(), &bcE03);
    EXPECT_EQ(&ebcE03.incidentNodeBlockType(), &ebcN06);

    EXPECT_EQ(&ebcE04.IGBackwardLinkRef(), &bcE04);
    EXPECT_EQ(&ebcE04.incidentNodeBlockType(), &ebcN06);

    EXPECT_EQ(&ebcE05.IGBackwardLinkRef(), &bcE05);
    EXPECT_EQ(&ebcE05.incidentNodeBlockType(), &ebcN08);

    EXPECT_EQ(&ebcE06.IGBackwardLinkRef(), &bcE06);
    EXPECT_EQ(&ebcE06.incidentNodeBlockType(), &ebcN09);

    EXPECT_EQ(&ebcE07.IGBackwardLinkRef(), &bcE07);
    EXPECT_EQ(&ebcE07.incidentNodeBlockType(), &ebcN10);

    EXPECT_EQ(&ebcE08.IGBackwardLinkRef(), &bcE08);
    EXPECT_EQ(&ebcE08.incidentNodeBlockType(), &ebcN10);

    EXPECT_EQ(&ebcE09.IGBackwardLinkRef(), &bcE09);
    EXPECT_EQ(&ebcE09.incidentNodeBlockType(), &ebcN11);

    EXPECT_EQ(&ebcE10.IGBackwardLinkRef(), &bcE10);
    EXPECT_EQ(&ebcE10.incidentNodeBlockType(), &ebcN11);

    EXPECT_EQ(ebcN01UG1.size(), 2);
    UnificationFace& uf1 = ebcN01UG1.at(0);
    UnificationFace& uf2 = ebcN01UG1.at(1);

    EXPECT_EQ(&uf1.treeNode(),       &ebcN02);
    EXPECT_EQ(&uf1.faceInDG(),       &ebcN02dgF01);
    EXPECT_EQ( uf1.faceInDGIt(),      ebcN02dgF01.backIt());
    EXPECT_EQ(&uf1.cutVertexInEG(),  &ebcN02egN01);
    EXPECT_EQ( uf1.cutVertexInEGIt(), ebcN02egN01.backIt());
    EXPECT_EQ(&uf1.edgeCCWInEG(),    &ebcN02egE01);
    EXPECT_EQ( uf1.edgeCCWInEGIt(),   ebcN02egE01.backIt());
    EXPECT_EQ(&uf1.edgeCWInEG()    , &ebcN02egE01);
    EXPECT_EQ( uf1.edgeCWInEGIt(),    ebcN02egE01.backIt());
    EXPECT_EQ( uf1.roleOfECWReversed(), false);

    EXPECT_EQ(&uf2.treeNode(),       &ebcN11);
    EXPECT_EQ(&uf2.faceInDG(),       &ebcN11dgF02);
    EXPECT_EQ( uf2.faceInDGIt(),      ebcN11dgF02.backIt());
    EXPECT_EQ(&uf2.cutVertexInEG(),  &ebcN11egN04);
    EXPECT_EQ( uf2.cutVertexInEGIt(), ebcN11egN04.backIt());
    EXPECT_EQ(&uf2.edgeCCWInEG(),    &ebcN11egE03); // 1->3
    EXPECT_EQ( uf2.edgeCCWInEGIt(),   ebcN11egE03.backIt());
    EXPECT_EQ(&uf2.edgeCWInEG()    , &ebcN11egE08); // 1->5
    EXPECT_EQ( uf2.edgeCWInEGIt(),    ebcN11egE08.backIt());
    EXPECT_EQ( uf2.roleOfECWReversed(), false);

    EXPECT_EQ(ebcN05UG1.size(), 3);
    UnificationFace& uf3 = ebcN05UG1.at(0);
    UnificationFace& uf4 = ebcN05UG1.at(1);
    UnificationFace& uf5 = ebcN05UG1.at(2);

    EXPECT_EQ(&uf3.treeNode(),       &ebcN06);
    EXPECT_EQ(&uf3.faceInDG(),       &ebcN06dgF01);
    EXPECT_EQ( uf3.faceInDGIt(),      ebcN06dgF01.backIt());
    EXPECT_EQ(&uf3.cutVertexInEG(),  &ebcN06egN01);
    EXPECT_EQ( uf3.cutVertexInEGIt(), ebcN06egN01.backIt());
    EXPECT_EQ(&uf3.edgeCCWInEG(),    &ebcN06egE01);
    EXPECT_EQ( uf3.edgeCCWInEGIt(),   ebcN06egE01.backIt());
    EXPECT_EQ(&uf3.edgeCWInEG()    , &ebcN06egE01);
    EXPECT_EQ( uf3.edgeCWInEGIt(),    ebcN06egE01.backIt());
    EXPECT_EQ( uf3.roleOfECWReversed(), false);

    EXPECT_EQ(&uf4.treeNode(),       &ebcN10);
    EXPECT_EQ(&uf4.faceInDG(),       &ebcN10dgF01);
    EXPECT_EQ( uf4.faceInDGIt(),      ebcN10dgF01.backIt());
    EXPECT_EQ(&uf4.cutVertexInEG(),  &ebcN10egN01);
    EXPECT_EQ( uf4.cutVertexInEGIt(), ebcN10egN01.backIt());
    EXPECT_EQ(&uf4.edgeCCWInEG(),    &ebcN10egE01);
    EXPECT_EQ( uf4.edgeCCWInEGIt(),   ebcN10egE01.backIt());
    EXPECT_EQ(&uf4.edgeCWInEG()    , &ebcN10egE01);
    EXPECT_EQ( uf4.edgeCWInEGIt(),    ebcN10egE01.backIt());
    EXPECT_EQ( uf4.roleOfECWReversed(), false);

    EXPECT_EQ(&uf5.treeNode(),       &ebcN11);
    EXPECT_EQ(&uf5.faceInDG(),       &ebcN11dgF02);
    EXPECT_EQ( uf5.faceInDGIt(),      ebcN11dgF02.backIt());
    EXPECT_EQ(&uf5.cutVertexInEG(),  &ebcN11egN05);
    EXPECT_EQ( uf5.cutVertexInEGIt(), ebcN11egN05.backIt());
    EXPECT_EQ(&uf5.edgeCCWInEG(),    &ebcN11egE07); // 4 -> 7
    EXPECT_EQ( uf5.edgeCCWInEGIt(),   ebcN11egE07.backIt());
    EXPECT_EQ(&uf5.edgeCWInEG()    , &ebcN11egE05); // 3 -> 4
    EXPECT_EQ( uf5.edgeCWInEGIt(),    ebcN11egE05.backIt());
    EXPECT_EQ( uf5.roleOfECWReversed(), false);

    EXPECT_EQ(ebcN03UG1.size(), 2);
    UnificationFace& uf6 = ebcN03UG1.at(0);
    UnificationFace& uf7 = ebcN03UG1.at(1);

    EXPECT_EQ(&uf6.treeNode(),       &ebcN04);
    EXPECT_EQ(&uf6.faceInDG(),       &ebcN04dgF01);
    EXPECT_EQ( uf6.faceInDGIt(),      ebcN04dgF01.backIt());
    EXPECT_EQ(&uf6.cutVertexInEG(),  &ebcN04egN01);
    EXPECT_EQ( uf6.cutVertexInEGIt(), ebcN04egN01.backIt());
    EXPECT_EQ(&uf6.edgeCCWInEG(),    &ebcN04egE01);
    EXPECT_EQ( uf6.edgeCCWInEGIt(),   ebcN04egE01.backIt());
    EXPECT_EQ(&uf6.edgeCWInEG()    , &ebcN04egE01);
    EXPECT_EQ( uf6.edgeCWInEGIt(),    ebcN04egE01.backIt());
    EXPECT_EQ( uf6.roleOfECWReversed(), false);

    EXPECT_EQ(&uf7.treeNode(),       &ebcN06);
    EXPECT_EQ(&uf7.faceInDG(),       &ebcN06dgF01);
    EXPECT_EQ( uf7.faceInDGIt(),      ebcN06dgF01.backIt());
    EXPECT_EQ(&uf7.cutVertexInEG(),  &ebcN06egN02);
    EXPECT_EQ( uf7.cutVertexInEGIt(), ebcN06egN02.backIt());
    EXPECT_EQ(&uf7.edgeCCWInEG(),    &ebcN06egE01);
    EXPECT_EQ( uf7.edgeCCWInEGIt(),   ebcN06egE01.backIt());
    EXPECT_EQ(&uf7.edgeCWInEG()    , &ebcN06egE01);
    EXPECT_EQ( uf7.edgeCWInEGIt(),    ebcN06egE01.backIt());
    EXPECT_EQ( uf7.roleOfECWReversed(), false);

    EXPECT_EQ(ebcN07UG1.size(), 3);
    UnificationFace& uf8 = ebcN07UG1.at(0);
    UnificationFace& uf9 = ebcN07UG1.at(1);
    UnificationFace& uf10= ebcN07UG1.at(2);

    EXPECT_EQ(&uf8.treeNode(),       &ebcN08);
    EXPECT_EQ(&uf8.faceInDG(),       &ebcN08dgF01);
    EXPECT_EQ( uf8.faceInDGIt(),      ebcN08dgF01.backIt());
    EXPECT_EQ(&uf8.cutVertexInEG(),  &ebcN08egN01);
    EXPECT_EQ( uf8.cutVertexInEGIt(), ebcN08egN01.backIt());
    EXPECT_EQ(&uf8.edgeCCWInEG(),    &ebcN08egE01);
    EXPECT_EQ( uf8.edgeCCWInEGIt(),   ebcN08egE01.backIt());
    EXPECT_EQ(&uf8.edgeCWInEG()    , &ebcN08egE03);
    EXPECT_EQ( uf8.edgeCWInEGIt(),    ebcN08egE03.backIt());
    EXPECT_EQ( uf8.roleOfECWReversed(), false);

    EXPECT_EQ(&uf9.treeNode(),       &ebcN09);
    EXPECT_EQ(&uf9.faceInDG(),       &ebcN09dgF01);
    EXPECT_EQ( uf9.faceInDGIt(),      ebcN09dgF01.backIt());
    EXPECT_EQ(&uf9.cutVertexInEG(),  &ebcN09egN01);
    EXPECT_EQ( uf9.cutVertexInEGIt(), ebcN09egN01.backIt());
    EXPECT_EQ(&uf9.edgeCCWInEG(),    &ebcN09egE01);
    EXPECT_EQ( uf9.edgeCCWInEGIt(),   ebcN09egE01.backIt());
    EXPECT_EQ(&uf9.edgeCWInEG()    , &ebcN09egE01);
    EXPECT_EQ( uf9.edgeCWInEGIt(),    ebcN09egE01.backIt());
    EXPECT_EQ( uf9.roleOfECWReversed(), false);

    EXPECT_EQ(&uf10.treeNode(),       &ebcN10);
    EXPECT_EQ(&uf10.faceInDG(),       &ebcN10dgF01);
    EXPECT_EQ( uf10.faceInDGIt(),      ebcN10dgF01.backIt());
    EXPECT_EQ(&uf10.cutVertexInEG(),  &ebcN10egN02);
    EXPECT_EQ( uf10.cutVertexInEGIt(), ebcN10egN02.backIt());
    EXPECT_EQ(&uf10.edgeCCWInEG(),    &ebcN10egE01);
    EXPECT_EQ( uf10.edgeCCWInEGIt(),   ebcN10egE01.backIt());
    EXPECT_EQ(&uf10.edgeCWInEG()    , &ebcN10egE01);
    EXPECT_EQ( uf10.edgeCWInEGIt(),    ebcN10egE01.backIt());
    EXPECT_EQ( uf10.roleOfECWReversed(), false);
    ebcT.findGeometricEmbedding(
                  ebcN11.backIt(),ebcN11dgF03.backIt(), ebcN11egN04.backIt());
    auto& ebcTR = ebcT.root();

    EXPECT_EQ(&ebcTR, &ebcN11);

    EmbeddedBCTree::ExplorationTree& expT =  ebcT.explorationTree();

    auto& expTR = expT.root();

    EXPECT_EQ(expT.numNodes(),7);
    EXPECT_EQ(expT.numEdges(),6);

    auto expNit = expT.nodes().first;
    auto& expN01 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN02 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN03 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN04 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN05 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN06 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));
    expNit++;
    auto& expN07 = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(*(*expNit));

    EXPECT_EQ(&expTR, &expN07);

    EXPECT_EQ(&expN01.IGBackwardLinkRef(), &ebcN02);
    EXPECT_EQ(expN01.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN01UGs=expN01.sortedUnificationGroups();
    EXPECT_EQ(expN01UGs.size(), 0);

    EXPECT_EQ(&expN02.IGBackwardLinkRef(), &ebcN04);
    EXPECT_EQ(expN02.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN02UGs=expN02.sortedUnificationGroups();
    EXPECT_EQ(expN02UGs.size(), 0);

    EXPECT_EQ(&expN03.IGBackwardLinkRef(), &ebcN06);
    EXPECT_EQ(expN03.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN03UGs=expN03.sortedUnificationGroups();
    EXPECT_EQ(expN03UGs.size(), 1);

    EXPECT_EQ(expN03UGs[0].absorbeesSize(), 1);
    EXPECT_EQ(expN03UGs[0].absorber(),     uf7);
    EXPECT_EQ(expN03UGs[0].absorbeeAt(0),  uf6);

    EXPECT_EQ(&expN04.IGBackwardLinkRef(), &ebcN08);
    EXPECT_EQ(expN04.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN04UGs=expN04.sortedUnificationGroups();
    EXPECT_EQ(expN04UGs.size(), 0);

    EXPECT_EQ(&expN05.IGBackwardLinkRef(), &ebcN09);
    EXPECT_EQ(expN05.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN05UGs=expN05.sortedUnificationGroups();
    EXPECT_EQ(expN05UGs.size(), 0);

    EXPECT_EQ(&expN06.IGBackwardLinkRef(), &ebcN10);
    EXPECT_EQ(expN06.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN06UGs=expN06.sortedUnificationGroups();
    EXPECT_EQ(expN06UGs.size(), 1);

    EXPECT_EQ(expN06UGs[0].absorbeesSize(), 2);
    EXPECT_EQ(expN06UGs[0].absorber(),     uf10);
    EXPECT_EQ(expN06UGs[0].absorbeeAt(0),  uf8);
    EXPECT_EQ(expN06UGs[0].absorbeeAt(1),  uf9);

    EXPECT_EQ(&expN07.IGBackwardLinkRef(), &ebcN11);
    EXPECT_EQ(expN07.numChildrenProcessed(), 0);
    vector<SortedUnificationGroup>&expN07UGs=expN07.sortedUnificationGroups();
    EXPECT_EQ(expN07UGs.size(), 2);

    EXPECT_EQ(expN07UGs[0].absorbeesSize(), 1);
    EXPECT_EQ(expN07UGs[0].absorber(),     uf2);
    EXPECT_EQ(expN07UGs[0].absorbeeAt(0),  uf1);

    EXPECT_EQ(expN07UGs[1].absorbeesSize(), 2);
    EXPECT_EQ(expN07UGs[1].absorber(),     uf5);
    EXPECT_EQ(expN07UGs[1].absorbeeAt(0),  uf3);
    EXPECT_EQ(expN07UGs[1].absorbeeAt(1),  uf4);

    auto expEit = expT.edges().first;
    auto& expE01 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE01.incidentNodeSrc().backIt(), expN07.backIt());
    EXPECT_EQ(expE01.incidentNodeDst().backIt(), expN01.backIt());
    expEit++;
    auto& expE02 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE02.incidentNodeSrc().backIt(), expN07.backIt());
    EXPECT_EQ(expE02.incidentNodeDst().backIt(), expN03.backIt());
    expEit++;
    auto& expE03 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE03.incidentNodeSrc().backIt(), expN07.backIt());
    EXPECT_EQ(expE03.incidentNodeDst().backIt(), expN06.backIt());
    expEit++;
    auto& expE04 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE04.incidentNodeSrc().backIt(), expN03.backIt());
    EXPECT_EQ(expE04.incidentNodeDst().backIt(), expN02.backIt());
    expEit++;
    auto& expE05 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE05.incidentNodeSrc().backIt(), expN06.backIt());
    EXPECT_EQ(expE05.incidentNodeDst().backIt(), expN04.backIt());
    expEit++;
    auto& expE06 = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(*(*expEit));
    EXPECT_EQ(expE06.incidentNodeSrc().backIt(), expN06.backIt());
    EXPECT_EQ(expE06.incidentNodeDst().backIt(), expN05.backIt());
    expEit++;

    EXPECT_EQ(&ebcN02.outerFace(), &ebcN02dgF01);
    EXPECT_EQ(&ebcN02.topNode(),   &ebcN02egN01);

    EXPECT_EQ(&ebcN04.outerFace(), &ebcN04dgF01);
    EXPECT_EQ(&ebcN04.topNode(),   &ebcN04egN01);

    EXPECT_EQ(&ebcN06.outerFace(), &ebcN06dgF01);
    EXPECT_EQ(&ebcN06.topNode(),   &ebcN06egN01);

    EXPECT_EQ(&ebcN08.outerFace(), &ebcN08dgF01);
    EXPECT_EQ(&ebcN08.topNode(),   &ebcN08egN01);

    EXPECT_EQ(&ebcN09.outerFace(), &ebcN09dgF01);
    EXPECT_EQ(&ebcN09.topNode(),   &ebcN09egN01);

    EXPECT_EQ(&ebcN10.outerFace(), &ebcN10dgF01);
    EXPECT_EQ(&ebcN10.topNode(),   &ebcN10egN01);

    EXPECT_EQ(&ebcN11.outerFace(), &ebcN11dgF03);
    EXPECT_EQ(&ebcN11.topNode(),   &ebcN11egN04);

}

} // namespace Undirected

} // namespace Wailea
