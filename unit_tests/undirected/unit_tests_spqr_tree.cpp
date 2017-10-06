#include "gtest/gtest.h"
#include "undirected/spqr_decomposer.hpp"

namespace Wailea {

namespace Undirected { 

using DExplorer = DFSExplorer <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DStrategy = DFSStrategy <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DGraph    = DFSGraph    <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DNode     = DFSNode     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DEdge     = DFSEdge     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;

class SPQRTreeTests : public ::testing::Test {

  protected:  

    SPQRTreeTests(){;};
    virtual ~SPQRTreeTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    node_list_it_t copyInputGraph(SPQRDecomposer& decomp,
                            Graph& src, Graph& dst, node_list_it_t startNode){
        return decomp.copyInputGraph(src,dst,startNode);
    }

    void sortAdjacency(SPQRDecomposer& decomp, Graph& g){
        decomp.sortAdjacency(g);
    }


    void makeNewNodesEdgesAndComponents(
        SPQRTree& t,
        Graph&                  G,
        vector<edge_list_it_t>& edgesInGToBeCopied,
        node_list_it_t          nodeAInG,
        node_list_it_t          nodeBInG,
        bool                    hasEdgeABInG,
        edge_list_it_t          edgeABInGIt,
        bool                    hasVirtualEdgeABInComp3,
        edge_list_it_t          virtualEdgeABInComp3It,
        node_list_it_t          treeNodeComp3,
        node_list_it_t&         treeNodeToBeLinkedToG,
        edge_list_it_t&         virtualEdgeToBeLinkedToG
    ){
        t.makeNewNodesEdgesAndComponents(
            G,
            edgesInGToBeCopied,
            nodeAInG,
            nodeBInG,
            hasEdgeABInG,
            edgeABInGIt,
            hasVirtualEdgeABInComp3,
            virtualEdgeABInComp3It,
            treeNodeComp3,
            treeNodeToBeLinkedToG,
            virtualEdgeToBeLinkedToG);
    }

    void makeFinalTreeNodeAndComponent(
        SPQRTree&               t,
        Graph&                  G,
        vector<edge_list_it_t>& edgesInGToBeCopied
    ){
        t.makeFinalTreeNodeAndComponent(G,edgesInGToBeCopied);
    }

    void setStartNode(DExplorer& e, DNode& n) { e.setStartNode(n);}
    bool oneStep(DExplorer& e) { return e.oneStep();}

};


static bool test_tc_edges(
    SPQRComponent&          c, 
    vector<edge_list_it_t>& expected, 
    vector<edge_list_it_t>& vedges
){
    vector<bool>flags;
    for (size_t i = 0; i < expected.size(); i++){
        flags.push_back(false);
    }
    for(auto it = c.edges().first; it != c.edges().second; it++){
        auto& e = dynamic_cast<SPQRComponentEdge&>(*(*it));
        if(e.type()==SPQRComponentEdge::VirtualType){
            vedges.push_back(it);
        }
        else{
            bool flag = false;
            for (size_t i = 0; i < expected.size(); i++) {
                if(expected[i] == e.IGBackwardLink()){
                    if(flags[i]==false){
                        flags[i]=true;
                        flag = true;
                        break;
                    }
                    else{
                        return false;
                    }
                }
            }
            if(flag==false){
                return false;            
            }
        }
    }
    for (auto b : flags) {
        if (b==false){
            return false;
        }
    }
    return true;
}


static bool test_tc_nodes(
    SPQRComponent&          c, 
    vector<node_list_it_t>& expected
){
    vector<bool>flags;
    for (size_t i = 0; i < expected.size(); i++){
        flags.push_back(false);
    }
    for(auto it = c.nodes().first; it != c.nodes().second; it++){
        auto& n = dynamic_cast<SPQRComponentNode&>(*(*it));
        bool flag = false;
        for (size_t i = 0; i < expected.size(); i++) {
            if(expected[i] == n.IGBackwardLink()){
                if(flags[i]==false){
                    flags[i]=true;
                    flag = true;
                    break;
                }
                else{
                    return false;
                }
            }
        }
        if(flag==false){
            return false;            
        }
    }
    for (auto b : flags) {
        if (b==false){
            return false;
        }
    }
    return true;
}


static bool testTreeNode(
    size_t        numEdges,
    SPQRTreeNode& treeNode, 
    vector<edge_list_it_t>& componentEdges,
    vector<node_list_it_t>& componentNodes,
    vector<edge_list_it_t>& virtualEdges ){

    auto& component = treeNode.component();

    if (component.numEdges() != numEdges) {
        return false;
    }

    if (component.numNodes() != componentNodes.size()) {
        return false;
    }

    if (test_tc_edges(component, componentEdges, virtualEdges)==false){
        return false;
    }

    if (test_tc_nodes(component, componentNodes)==false){
        return false;
    }
    return true;
}


static bool testTreeEdge(
    SPQRTreeEdge& te,
    SPQRTreeNode& n1,
    SPQRTreeNode& n2,
    vector<edge_list_it_t>& vedges1,
    vector<edge_list_it_t>& vedges2,
    edge_list_it_t& ve1,
    edge_list_it_t& ve2
){
    auto& te_n1 = dynamic_cast<SPQRTreeNode&>(te.incidentNode1());
    auto& te_n2 = dynamic_cast<SPQRTreeNode&>(te.incidentNode2());
    if (!( (&n1 == &te_n1 && &n2 == &te_n2)||
           (&n1 == &te_n2 && &n2 == &te_n1)  ) ){
        return false;
    }

    bool found = false;

    auto& te_ve1 = dynamic_cast<SPQRComponentEdge&>(*(*(
                        (&n1 == &te_n1)?te.virtualEdge1():te.virtualEdge2())));
    auto& te_ve2 = dynamic_cast<SPQRComponentEdge&>(*(*(
                        (&n1 == &te_n1)?te.virtualEdge2():te.virtualEdge1())));

    found = false;
    for (auto eit: vedges1 ) {
        if ( eit == te_ve1.backIt() ){
            found = true;
            ve1 = eit;
            break;
        }
    }
    if (found==false){
        return false;
    }
    found = false;
    for (auto eit: vedges2 ) {
        if ( eit == te_ve2.backIt() ){
            found = true;
            ve2 = eit;
            break;
        }
    }
    if (found==false){
        return false;
    }

    if((&(te_ve1.pairVirtualEdge()) == &te_ve2) &&
       (&(te_ve2.pairVirtualEdge()) == &te_ve1) &&
       (&(te_ve1.pairTreeNode()) == &n2)        &&
       (&(te_ve2.pairTreeNode()) == &n1)           ){
        return true;
    }
    return false;
}


static bool testTreeEdge(
    SPQRTreeEdge& te,
    SPQRTreeNode& n1,
    SPQRTreeNode& n2,
    vector<edge_list_it_t>& vedges1,
    vector<edge_list_it_t>& vedges2,
    bool               has_unlinked_ve_on_1,
    bool               has_unlinked_ve_on_2,
    edge_list_it_t& unlinked_ve,    
    edge_list_it_t& ve1,
    edge_list_it_t& ve2
){
    auto& te_n1 = dynamic_cast<SPQRTreeNode&>(te.incidentNode1());
    auto& te_n2 = dynamic_cast<SPQRTreeNode&>(te.incidentNode2());
    if (!( (&n1 == &te_n1 && &n2 == &te_n2)||
           (&n1 == &te_n2 && &n2 == &te_n1)  ) ){
        return false;
    }

    bool found = false;

    auto& te_ve1 = dynamic_cast<SPQRComponentEdge&>(*(*(
                        (&n1 == &te_n1)?te.virtualEdge1():te.virtualEdge2())));
    auto& te_ve2 = dynamic_cast<SPQRComponentEdge&>(*(*(
                        (&n1 == &te_n1)?te.virtualEdge2():te.virtualEdge1())));

    found = false;
    for (auto eit: vedges1 ) {
        if ( eit == te_ve1.backIt() ){
            found = true;
            ve1 = eit;
            break;
        }
    }
    if (found==false){
        return false;
    }
    found = false;
    for (auto eit: vedges2 ) {
        if ( eit == te_ve2.backIt() ){
            found = true;
            ve2 = eit;
            break;
        }
    }
    if (found==false){
        return false;
    }

    if(has_unlinked_ve_on_1) {
        found = false;
        for (auto eit: vedges1 ) {
            if ( eit == unlinked_ve&& eit != ve1 ){
                found = true;
                break;
            }
        }
        if (found==false){
            return false;
        }
    }

    if(has_unlinked_ve_on_2) {
        found = false;
        for (auto eit: vedges2 ) {
            if ( eit == unlinked_ve && eit != ve2 ){
                found = true;
                break;
            }
        }
        if (found==false){
            return false;
        }
    }

    if((&(te_ve1.pairVirtualEdge()) == &te_ve2) &&
       (&(te_ve2.pairVirtualEdge()) == &te_ve1) &&
       (&(te_ve1.pairTreeNode()) == &n2)        &&
       (&(te_ve2.pairTreeNode()) == &n1)           ){
        return true;
    }
    return false;
}


/**  @brief tests on a realistic graph.
 */
TEST_F(SPQRTreeTests, Test1) {

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
    auto& n_11 = g_1.addNode(make_unique<Node>());
    auto& n_12 = g_1.addNode(make_unique<Node>());
    auto& n_13 = g_1.addNode(make_unique<Node>());

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_13 = g_1.addEdge(make_unique<Edge>(),n_03,n_13);
    auto& e_13_01 = g_1.addEdge(make_unique<Edge>(),n_13,n_01);
    auto& e_13_02 = g_1.addEdge(make_unique<Edge>(),n_13,n_02);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    auto& e_07_04 = g_1.addEdge(make_unique<Edge>(),n_07,n_04);
    auto& e_07_05 = g_1.addEdge(make_unique<Edge>(),n_07,n_05);
    auto& e_06_04 = g_1.addEdge(make_unique<Edge>(),n_06,n_04);
    auto& e_05_08 = g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    auto& e_08_01 = g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    auto& e_08_09 = g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    auto& e_10_12 = g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    auto& e_12_01 = g_1.addEdge(make_unique<Edge>(),n_12,n_01);
    auto& e_12_08 = g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    auto& e_12_09 = g_1.addEdge(make_unique<Edge>(),n_12,n_09);
    auto& e_10_11 = g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    auto& e_11_08 = g_1.addEdge(make_unique<Edge>(),n_11,n_08);
    auto& e_11_09 = g_1.addEdge(make_unique<Edge>(),n_11,n_09);

    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                      (nc_01));

    sortAdjacency(decomposer, g_2);

    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);

    auto eit = g_2.edges().first;
    auto& ec_01_02 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_02_03 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_03_13 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_13_01 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_13_02 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_03_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_04_05 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_05_06 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_06_07 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_07_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_07_05 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_06_04 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_05_08 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_08_01 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_08_09 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_09_10 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_10_12 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_12_01 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_12_08 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_12_09 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_10_11 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_11_08 = dynamic_cast<DEdge&>(*(*eit++));
    auto& ec_11_09 = dynamic_cast<DEdge&>(*(*eit++));

    auto& nc_02 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_03 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_04 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_05 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_06 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_07 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_08 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_09 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_10 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_11 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_12 = dynamic_cast<DNode&>(*(*nit++));
    auto& nc_13 = dynamic_cast<DNode&>(*(*nit++));

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    SPQRTree     spqrTree(g_1);

    // Component 1 & 2
    // 1: 12->9, 10->12, 11->8, 11->9, 10->11, 9->10, 8->9, ve1(8,12)
    // 2: 12->8, ve1(8,12), ve2(8,12)

    vector<edge_list_it_t> sepEdges1;

    sepEdges1.push_back(ec_12_09.backIt());
    sepEdges1.push_back(ec_10_12.backIt());
    sepEdges1.push_back(ec_11_08.backIt());
    sepEdges1.push_back(ec_11_09.backIt());
    sepEdges1.push_back(ec_10_11.backIt());
    sepEdges1.push_back(ec_09_10.backIt());
    sepEdges1.push_back(ec_08_09.backIt());

    node_list_it_t tn_returned_01;
    edge_list_it_t ve_returned_01;

    makeNewNodesEdgesAndComponents(spqrTree, g_2,
            sepEdges1, nc_08.backIt(),nc_12.backIt(), 
            true,  ec_12_08.backIt(),
            false, g_2.edges().second,  g_2.nodes().second,
            tn_returned_01, ve_returned_01  );

    EXPECT_EQ(spqrTree.numNodes() ,2);
    EXPECT_EQ(spqrTree.numEdges() ,1);

    auto tnit = spqrTree.nodes().first;
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_01 = tn_01.component();
    tnit++;
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_02 = tn_02.component();
    auto teit = spqrTree.edges().first;
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    EXPECT_EQ(tc_01.numEdges(), 8);
    EXPECT_EQ(tc_01.numNodes(), 5);

    vector<edge_list_it_t> tc_01_edges_tested;
    tc_01_edges_tested.push_back(e_12_09.backIt());
    tc_01_edges_tested.push_back(e_10_12.backIt());
    tc_01_edges_tested.push_back(e_11_08.backIt());
    tc_01_edges_tested.push_back(e_11_09.backIt());
    tc_01_edges_tested.push_back(e_10_11.backIt());
    tc_01_edges_tested.push_back(e_09_10.backIt());
    tc_01_edges_tested.push_back(e_08_09.backIt());

    vector<node_list_it_t> tc_01_nodes_tested;
    tc_01_nodes_tested.push_back(n_12.backIt());
    tc_01_nodes_tested.push_back(n_08.backIt());
    tc_01_nodes_tested.push_back(n_09.backIt());
    tc_01_nodes_tested.push_back(n_10.backIt());
    tc_01_nodes_tested.push_back(n_11.backIt());

    vector<edge_list_it_t> tc_01_vedges;

    EXPECT_EQ(test_tc_edges(tc_01, tc_01_edges_tested, tc_01_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_01, tc_01_nodes_tested), true);
    EXPECT_EQ(tc_01_vedges.size(),1);


    EXPECT_EQ(tc_02.numEdges(), 3);
    EXPECT_EQ(tc_02.numNodes(), 2);

    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_12_08.backIt());

    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_12.backIt());
    tc_02_nodes_tested.push_back(n_08.backIt());

    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(test_tc_edges(tc_02, tc_02_edges_tested, tc_02_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_02, tc_02_nodes_tested), true);
    EXPECT_EQ(tc_02_vedges.size(),2);

    edge_list_it_t te_ve_01;
    edge_list_it_t te_ve_02;

    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                        false, true, ve_returned_01, te_ve_01, te_ve_02),true);

    EXPECT_EQ(tn_returned_01, tn_02.backIt());
                                                   
    ec_12_08.mIsVirtual = true;
    ec_12_08.mVirtualPairEdge = ve_returned_01; 
    ec_12_08.mVirtualPairTreeNode = tn_returned_01; 

    g_2.removeEdge(ec_12_09);
    g_2.removeEdge(ec_10_12);
    g_2.removeEdge(ec_11_08);
    g_2.removeEdge(ec_11_09);
    g_2.removeEdge(ec_10_11);
    g_2.removeEdge(ec_09_10);
    g_2.removeEdge(ec_08_09);
    g_2.removeNode(nc_09);
    g_2.removeNode(nc_10);
    g_2.removeNode(nc_11);

    // Component 3 & 4
    // 3: 7->4, 7->5, 6->7, 6->4, 5->6, ve(4->5)
    // 4: 4->5, ve(4->5), ve(4->5)

    vector<edge_list_it_t> sepEdges2;

    sepEdges2.push_back(ec_07_04.backIt());
    sepEdges2.push_back(ec_07_05.backIt());
    sepEdges2.push_back(ec_06_07.backIt());
    sepEdges2.push_back(ec_06_04.backIt());
    sepEdges2.push_back(ec_05_06.backIt());

    node_list_it_t tn_returned_02;
    edge_list_it_t ve_returned_02;

    makeNewNodesEdgesAndComponents(spqrTree, g_2,
            sepEdges2, nc_04.backIt(),nc_05.backIt(), 
            true,  ec_04_05.backIt(),
            false, g_2.edges().second,  g_2.nodes().second,
            tn_returned_02, ve_returned_02  );

    EXPECT_EQ(spqrTree.numNodes() ,4);
    EXPECT_EQ(spqrTree.numEdges() ,2);

    tnit++;
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_03 = tn_03.component();
    tnit++;
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_04 = tn_04.component();
    teit++;
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit));

    EXPECT_EQ(tc_03.numEdges(), 6);
    EXPECT_EQ(tc_03.numNodes(), 4);

    vector<edge_list_it_t> tc_03_edges_tested;
    tc_03_edges_tested.push_back(e_07_04.backIt());
    tc_03_edges_tested.push_back(e_07_05.backIt());
    tc_03_edges_tested.push_back(e_06_07.backIt());
    tc_03_edges_tested.push_back(e_06_04.backIt());
    tc_03_edges_tested.push_back(e_05_06.backIt());

    vector<node_list_it_t> tc_03_nodes_tested;
    tc_03_nodes_tested.push_back(n_04.backIt());
    tc_03_nodes_tested.push_back(n_05.backIt());
    tc_03_nodes_tested.push_back(n_06.backIt());
    tc_03_nodes_tested.push_back(n_07.backIt());

    vector<edge_list_it_t> tc_03_vedges;
    EXPECT_EQ(test_tc_edges(tc_03, tc_03_edges_tested, tc_03_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_03, tc_03_nodes_tested), true);
    EXPECT_EQ(tc_03_vedges.size(),1);

    EXPECT_EQ(tc_04.numEdges(), 3);
    EXPECT_EQ(tc_04.numNodes(), 2);

    vector<edge_list_it_t> tc_04_edges_tested;
    tc_04_edges_tested.push_back(e_04_05.backIt());

    vector<node_list_it_t> tc_04_nodes_tested;
    tc_04_nodes_tested.push_back(n_04.backIt());
    tc_04_nodes_tested.push_back(n_05.backIt());

    vector<edge_list_it_t> tc_04_vedges;
    EXPECT_EQ(test_tc_edges(tc_04, tc_04_edges_tested, tc_04_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_04, tc_04_nodes_tested), true);
    EXPECT_EQ(tc_04_vedges.size(),2);

    edge_list_it_t te_ve_03;
    edge_list_it_t te_ve_04;

    EXPECT_EQ(testTreeEdge(te_02, tn_03, tn_04, tc_03_vedges, tc_04_vedges,
                        false, true, ve_returned_02, te_ve_03, te_ve_04),true);

    EXPECT_EQ(tn_returned_02, tn_04.backIt());

    g_2.removeEdge(ec_07_04);
    g_2.removeEdge(ec_07_05);
    g_2.removeEdge(ec_06_07);
    g_2.removeEdge(ec_06_04);
    g_2.removeEdge(ec_05_06);
    g_2.removeEdge(ec_04_05);
    g_2.removeNode(nc_06);
    g_2.removeNode(nc_07);

    edge_ptr_t ec_04_05_2p = make_unique<DEdge>();
    DEdge& ec_04_05_2 = dynamic_cast<DEdge&>(
                               g_2.addEdge(move(ec_04_05_2p), nc_04, nc_05));
    ec_04_05_2.mIsVirtual = true;
    ec_04_05_2.mVirtualPairEdge = ve_returned_02;
    ec_04_05_2.mVirtualPairTreeNode = tn_returned_02; 

    // Component 5
    // 5: 4->5, 5->8, ve(4->8)

    vector<edge_list_it_t> sepEdges3;

    sepEdges3.push_back(ec_04_05_2.backIt());
    sepEdges3.push_back(ec_05_08.backIt());

    node_list_it_t tn_returned_03;
    edge_list_it_t ve_returned_03;

    makeNewNodesEdgesAndComponents(spqrTree, g_2,
            sepEdges3, nc_04.backIt(),nc_08.backIt(), 
            false,  g_2.edges().second,
            false, g_2.edges().second,  g_2.nodes().second,
            tn_returned_03, ve_returned_03  );

    EXPECT_EQ(spqrTree.numNodes() ,5);
    EXPECT_EQ(spqrTree.numEdges() ,3);

    tnit++;
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_05 = tn_05.component();
    teit++;
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit));

    EXPECT_EQ(tc_05.numEdges(), 3);
    EXPECT_EQ(tc_05.numNodes(), 3);

    vector<edge_list_it_t> tc_05_edges_tested;
    vector<node_list_it_t> tc_05_nodes_tested;

    tc_05_edges_tested.push_back(e_05_08.backIt());
    tc_05_nodes_tested.push_back(n_04.backIt());
    tc_05_nodes_tested.push_back(n_05.backIt());
    tc_05_nodes_tested.push_back(n_08.backIt());

    vector<edge_list_it_t> tc_05_vedges;
    EXPECT_EQ(test_tc_edges(tc_05, tc_05_edges_tested, tc_05_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_05, tc_05_nodes_tested), true);
    EXPECT_EQ(tc_05_vedges.size(),2);

    edge_list_it_t te_ve_05;
    edge_list_it_t te_ve_06;

    EXPECT_EQ(testTreeEdge(te_03, tn_05, tn_04, tc_05_vedges, tc_04_vedges,
                        true, false, ve_returned_03, te_ve_05, te_ve_04),true);

    EXPECT_EQ(tn_returned_03, tn_05.backIt());

    g_2.removeEdge(ec_04_05_2);
    g_2.removeEdge(ec_05_08);
    g_2.removeNode(nc_05);

    edge_ptr_t ec_04_08_p = make_unique<DEdge>();
    DEdge& ec_04_08 = dynamic_cast<DEdge&>(
                               g_2.addEdge(move(ec_04_08_p), nc_04, nc_08));
    ec_04_08.mIsVirtual = true;
    ec_04_08.mVirtualPairEdge = ve_returned_03; 
    ec_04_08.mVirtualPairTreeNode = tn_returned_03; 

    // Component 6
    // 6: 3->4, 4->8, ve(3->8)

    vector<edge_list_it_t> sepEdges4;

    sepEdges4.push_back(ec_03_04.backIt());
    sepEdges4.push_back(ec_04_08.backIt());

    node_list_it_t tn_returned_04;
    edge_list_it_t ve_returned_04;

    makeNewNodesEdgesAndComponents(spqrTree, g_2,
            sepEdges4, nc_03.backIt(),nc_04.backIt(), 
            false,  g_2.edges().second,
            false,  g_2.edges().second,  g_2.nodes().second,
            tn_returned_04, ve_returned_04  );

    EXPECT_EQ(spqrTree.numNodes() ,6);
    EXPECT_EQ(spqrTree.numEdges() ,4);

    tnit++;
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_06 = tn_06.component();
    teit++;
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit));

    EXPECT_EQ(tc_06.numEdges(), 3);
    EXPECT_EQ(tc_06.numNodes(), 3);

    vector<edge_list_it_t> tc_06_edges_tested;
    vector<node_list_it_t> tc_06_nodes_tested;

    tc_06_edges_tested.push_back(e_03_04.backIt());
    tc_06_nodes_tested.push_back(n_03.backIt());
    tc_06_nodes_tested.push_back(n_04.backIt());
    tc_06_nodes_tested.push_back(n_08.backIt());

    vector<edge_list_it_t> tc_06_vedges;
    EXPECT_EQ(test_tc_edges(tc_06, tc_06_edges_tested, tc_06_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_06, tc_06_nodes_tested), true);
    EXPECT_EQ(tc_06_vedges.size(),2);

    EXPECT_EQ(tn_returned_04, tn_06.backIt());

    edge_list_it_t te_ve_07;
    edge_list_it_t te_ve_08;

    EXPECT_EQ(testTreeEdge(te_04, tn_06, tn_05, tc_06_vedges, tc_05_vedges,
                       true, false, ve_returned_04, te_ve_06, te_ve_05),true);

    g_2.removeEdge(ec_03_04);
    g_2.removeEdge(ec_04_08);
    g_2.removeNode(nc_04);

    edge_ptr_t ec_03_08_p = make_unique<DEdge>();
    DEdge& ec_03_08 = dynamic_cast<DEdge&>(
                               g_2.addEdge(move(ec_03_08_p), nc_03, nc_08));

    ec_03_08.mIsVirtual = true;
    ec_03_08.mVirtualPairEdge = ve_returned_04; 
    ec_03_08.mVirtualPairTreeNode = tn_returned_04; 

    // Component 7 & 8
    // 7: ve(12->8), 12->1, ve(1->8)
    // 8: 1->8 ve(1->8) ve(1->8)

    vector<edge_list_it_t> sepEdges5;
    sepEdges5.push_back(ec_12_01.backIt());
    sepEdges5.push_back(ec_12_08.backIt());

    node_list_it_t tn_returned_05;
    edge_list_it_t ve_returned_05;

    makeNewNodesEdgesAndComponents(spqrTree, g_2,
            sepEdges5, nc_01.backIt(),nc_08.backIt(), 
            true,   ec_08_01.backIt(),
            false,  g_2.edges().second,  g_2.nodes().second,
            tn_returned_05, ve_returned_05  );

    EXPECT_EQ(spqrTree.numNodes() ,8);
    EXPECT_EQ(spqrTree.numEdges() ,6);

    tnit++;
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_07 = tn_07.component();
    tnit++;
    auto& tn_08 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_08 = tn_08.component();

    teit++;
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    teit++;
    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*teit));

    EXPECT_EQ(tc_07.numEdges(), 3);
    EXPECT_EQ(tc_07.numNodes(), 3);

    vector<edge_list_it_t> tc_07_edges_tested;
    tc_07_edges_tested.push_back(e_12_01.backIt());

    vector<node_list_it_t> tc_07_nodes_tested;
    tc_07_nodes_tested.push_back(n_01.backIt());
    tc_07_nodes_tested.push_back(n_08.backIt());
    tc_07_nodes_tested.push_back(n_12.backIt());

    vector<edge_list_it_t> tc_07_vedges;
    EXPECT_EQ(test_tc_edges(tc_07, tc_07_edges_tested, tc_07_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_07, tc_07_nodes_tested), true);
    EXPECT_EQ(tc_07_vedges.size(),2);

    EXPECT_EQ(tc_08.numEdges(), 3);
    EXPECT_EQ(tc_08.numNodes(), 2);

    vector<edge_list_it_t> tc_08_edges_tested;
    tc_08_edges_tested.push_back(e_08_01.backIt());

    vector<node_list_it_t> tc_08_nodes_tested;
    tc_08_nodes_tested.push_back(n_01.backIt());
    tc_08_nodes_tested.push_back(n_08.backIt());

    vector<edge_list_it_t> tc_08_vedges;
    EXPECT_EQ(test_tc_edges(tc_08, tc_08_edges_tested, tc_08_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_08, tc_08_nodes_tested), true);
    EXPECT_EQ(tc_08_vedges.size(),2);

    edge_list_it_t te_ve_09;
    edge_list_it_t te_ve_10;

    EXPECT_EQ(testTreeEdge(te_05, tn_02, tn_07, tc_02_vedges, tc_07_vedges,
                       false, false, ve_returned_04, te_ve_09, te_ve_10),true);

    edge_list_it_t te_ve_11;
    edge_list_it_t te_ve_12;

    EXPECT_EQ(testTreeEdge(te_06, tn_07, tn_08, tc_07_vedges, tc_08_vedges,
                       false, true, ve_returned_05, te_ve_11, te_ve_12),true);

    EXPECT_EQ(tn_returned_05, tn_08.backIt());

    g_2.removeEdge(ec_12_08);
    g_2.removeEdge(ec_12_01);
    g_2.removeEdge(ec_08_01);
    g_2.removeNode(nc_12);

    edge_ptr_t ec_01_08_p = make_unique<DEdge>();
    DEdge& ec_01_08 = dynamic_cast<DEdge&>(
                               g_2.addEdge(move(ec_01_08_p), nc_01, nc_08));
    ec_01_08.mIsVirtual = true;
    ec_01_08.mVirtualPairEdge = ve_returned_05; 
    ec_01_08.mVirtualPairTreeNode = tn_returned_05; 

    // Component 9
    // 9: ve(1->3), ve(3->8), ve(1->8)

    vector<edge_list_it_t> sepEdges6;
    sepEdges6.push_back(ec_01_08.backIt());
    sepEdges6.push_back(ec_03_08.backIt());

    node_list_it_t tn_returned_06;
    edge_list_it_t ve_returned_06;

    makeNewNodesEdgesAndComponents(spqrTree, g_2,
            sepEdges6, nc_01.backIt(),nc_03.backIt(), 
            false,   g_2.edges().second,
            false,  g_2.edges().second,  g_2.nodes().second,
            tn_returned_06, ve_returned_06  );

    EXPECT_EQ(spqrTree.numNodes() ,9);
    EXPECT_EQ(spqrTree.numEdges() ,8);

    tnit++;
    auto& tn_09 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_09 = tn_09.component();

    teit++;
    auto& te_07 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    teit++;
    auto& te_08 = dynamic_cast<SPQRTreeEdge&>(*(*teit));

    EXPECT_EQ(tc_09.numEdges(), 3);
    EXPECT_EQ(tc_09.numNodes(), 3);

    vector<edge_list_it_t> tc_09_edges_tested;

    vector<node_list_it_t> tc_09_nodes_tested;
    tc_09_nodes_tested.push_back(n_01.backIt());
    tc_09_nodes_tested.push_back(n_03.backIt());
    tc_09_nodes_tested.push_back(n_08.backIt());

    vector<edge_list_it_t> tc_09_vedges;
    EXPECT_EQ(test_tc_edges(tc_09, tc_09_edges_tested, tc_09_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_09, tc_09_nodes_tested), true);
    EXPECT_EQ(tc_09_vedges.size(),3);

    EXPECT_EQ(tc_09.numEdges(), 3);
    EXPECT_EQ(tc_09.numNodes(), 3);

    edge_list_it_t te_ve_13;
    edge_list_it_t te_ve_14;

    EXPECT_EQ( testTreeEdge(te_07, tn_08, tn_09, tc_08_vedges, tc_09_vedges,
                       false, true, ve_returned_06, te_ve_13, te_ve_14) ||
               testTreeEdge(te_08, tn_08, tn_09, tc_08_vedges, tc_09_vedges,
                       false, true, ve_returned_06, te_ve_13, te_ve_14),true);

    edge_list_it_t te_ve_15;
    edge_list_it_t te_ve_16;

    EXPECT_EQ( testTreeEdge(te_07, tn_06, tn_09, tc_06_vedges, tc_09_vedges,
                       false, true, ve_returned_06, te_ve_15, te_ve_16) ||
               testTreeEdge(te_08, tn_06, tn_09, tc_06_vedges, tc_09_vedges,
                       false, true, ve_returned_06, te_ve_15, te_ve_16),true);
    EXPECT_EQ(tn_returned_06,tn_09.backIt());


    g_2.removeEdge(ec_01_08);
    g_2.removeEdge(ec_03_08);
    g_2.removeNode(nc_08);

    edge_ptr_t ec_01_03_p = make_unique<DEdge>();
    DEdge& ec_01_03 = dynamic_cast<DEdge&>(
                               g_2.addEdge(move(ec_01_03_p), nc_01, nc_03));
    ec_01_03.mIsVirtual = true;
    ec_01_03.mVirtualPairEdge = ve_returned_06; 
    ec_01_03.mVirtualPairTreeNode = tn_returned_06; 

    // Component 10
    // 10: ve(1->3),
    EXPECT_EQ(g_2.numNodes(), 4);
    EXPECT_EQ(g_2.numEdges(), 6);

    vector<edge_list_it_t> sepEdges7;
    sepEdges7.push_back(ec_01_03.backIt());
    sepEdges7.push_back(ec_01_02.backIt());
    sepEdges7.push_back(ec_02_03.backIt());
    sepEdges7.push_back(ec_03_13.backIt());
    sepEdges7.push_back(ec_13_01.backIt());
    sepEdges7.push_back(ec_13_02.backIt());

    makeFinalTreeNodeAndComponent(spqrTree,g_2, sepEdges7);

    EXPECT_EQ(spqrTree.numNodes() ,10);
    EXPECT_EQ(spqrTree.numEdges() , 9);

    tnit++;
    auto& tn_10 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& tc_10 = tn_10.component();

    teit++;
    auto& te_09 = dynamic_cast<SPQRTreeEdge&>(*(*teit));

    EXPECT_EQ(tc_10.numNodes(), 4);
    EXPECT_EQ(tc_10.numEdges(), 6);

    vector<edge_list_it_t> tc_10_edges_tested;
    tc_10_edges_tested.push_back(e_01_02.backIt());
    tc_10_edges_tested.push_back(e_02_03.backIt());
    tc_10_edges_tested.push_back(e_03_13.backIt());
    tc_10_edges_tested.push_back(e_13_01.backIt());
    tc_10_edges_tested.push_back(e_13_02.backIt());

    vector<node_list_it_t> tc_10_nodes_tested;
    tc_10_nodes_tested.push_back(n_01.backIt());
    tc_10_nodes_tested.push_back(n_02.backIt());
    tc_10_nodes_tested.push_back(n_03.backIt());
    tc_10_nodes_tested.push_back(n_13.backIt());

    vector<edge_list_it_t> tc_10_vedges;
    EXPECT_EQ(test_tc_edges(tc_10, tc_10_edges_tested, tc_10_vedges), true);
    EXPECT_EQ(test_tc_nodes(tc_10, tc_10_nodes_tested), true);
    EXPECT_EQ(tc_10_vedges.size(),1);

    edge_list_it_t te_ve_17;
    edge_list_it_t te_ve_18;

    EXPECT_EQ(testTreeEdge(te_09, tn_10, tn_09, tc_10_vedges, tc_09_vedges,
                  false, false, ve_returned_06, te_ve_17, te_ve_18),true);

    g_2.removeEdge(ec_01_02);
    g_2.removeEdge(ec_02_03);
    g_2.removeEdge(ec_03_13);
    g_2.removeEdge(ec_13_01);
    g_2.removeEdge(ec_13_02);
    g_2.removeEdge(ec_01_03);
    g_2.removeNode(nc_01);
    g_2.removeNode(nc_02);
    g_2.removeNode(nc_03);
    g_2.removeNode(nc_13);

}


/**  @brief tests on determineTypes().
 */
TEST_F(SPQRTreeTests, Test2) {


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
    auto& n_11 = g_1.addNode(make_unique<Node>());
    auto& n_12 = g_1.addNode(make_unique<Node>());
    auto& n_13 = g_1.addNode(make_unique<Node>());

    /*auto& e_01_02 = */g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    /*auto& e_02_03 = */g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    /*auto& e_03_13 = */g_1.addEdge(make_unique<Edge>(),n_03,n_13);
    /*auto& e_13_01 = */g_1.addEdge(make_unique<Edge>(),n_13,n_01);
    /*auto& e_13_02 = */g_1.addEdge(make_unique<Edge>(),n_13,n_02);
    /*auto& e_03_04 = */g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    /*auto& e_04_05 = */g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    /*auto& e_05_06 = */g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    /*auto& e_06_07 = */g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    /*auto& e_07_04 = */g_1.addEdge(make_unique<Edge>(),n_07,n_04);
    /*auto& e_07_05 = */g_1.addEdge(make_unique<Edge>(),n_07,n_05);
    /*auto& e_06_04 = */g_1.addEdge(make_unique<Edge>(),n_06,n_04);
    /*auto& e_05_08 = */g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    /*auto& e_08_01 = */g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    /*auto& e_08_09 = */g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    /*auto& e_09_10 = */g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    /*auto& e_10_12 = */g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    /*auto& e_12_01 = */g_1.addEdge(make_unique<Edge>(),n_12,n_01);
    /*auto& e_12_08 = */g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    /*auto& e_12_09 = */g_1.addEdge(make_unique<Edge>(),n_12,n_09);
    /*auto& e_10_11 = */g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    /*auto& e_11_08 = */g_1.addEdge(make_unique<Edge>(),n_11,n_08);
    /*auto& e_11_09 = */g_1.addEdge(make_unique<Edge>(),n_11,n_09);

    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    auto np = make_unique<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
    edge_list_it_t it;
    np->mOrderedAdjacencyType1.clear();
    np->mOrderedAdjacencyType1Back.clear();
    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    sortAdjacency(decomposer, g_2);
    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    SPQRTree     spqrTree(g_2);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    mExplorer3.reset(g_2);
    mExplorer3.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    spqrTree.determineTypes();

    auto tnit = spqrTree.nodes().first;

    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_08 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_09 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_10 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));

    EXPECT_EQ(tn_01.type(), SPQRTreeNode::RType);
    EXPECT_EQ(tn_02.type(), SPQRTreeNode::PType);
    EXPECT_EQ(tn_03.type(), SPQRTreeNode::SType);
    EXPECT_EQ(tn_04.type(), SPQRTreeNode::PType);
    EXPECT_EQ(tn_05.type(), SPQRTreeNode::SType);
    EXPECT_EQ(tn_06.type(), SPQRTreeNode::RType);
    EXPECT_EQ(tn_07.type(), SPQRTreeNode::PType);
    EXPECT_EQ(tn_08.type(), SPQRTreeNode::SType);
    EXPECT_EQ(tn_09.type(), SPQRTreeNode::SType);
    EXPECT_EQ(tn_10.type(), SPQRTreeNode::RType);

}


/**  @brief tests on mergeTwoNodesP() #1
 *   input
 *      (1->2)
 *      (2->3)
 *      (3->4)
 *      (3->1)
 *      (4->1)
 *      (4->2)
 *      (2->5)
 *      (5->6)
 *      (5->1)
 *      (6->1)
 *      (6->2)
 *      (2->7)
 *      (7->8)
 *      (7->1)
 *      (8->1)
 *      (8->2)
 *      (2->9)
 *      (9->10)
 *      (9->1)
 *      (10->1)
 *      (10->2)
 *
 *   tree node 01
 *      (1->2) virtual pair 1
 *      (2->3)
 *      (3->4)
 *      (4->1)
 *      (4->2)
 *      (3->1)
 *
 *   tree node 02
 *      (1->2) virtual pair 1
 *      (1->2)
 *      (1->2) virtual pair 2
 *
 *   tree node 03
 *      (1->2) virtual pair 3
 *      (2->5)
 *      (5->6)
 *      (6->1)
 *      (6->2)
 *      (5->1)
 *
 *   tree node 04
 *      (1->2) virtual pair 3
 *      (1->2) virtual pair 2
 *      (1->2) virtual pair 4
 *
 *   tree node 05
 *      (1->2) virtual pair 5
 *      (2->7)
 *      (7->8)
 *      (8->1)
 *      (8->2)
 *      (7->1)
 *
 *   tree node 04
 *      (1->2) virtual pair 4
 *      (1->2) virtual pair 5
 *      (1->2) virtual pair 6
 *
 *   tree node 05
 *      (1->2) virtual pair 6
 *      (2->9)
 *      (9->10)
 *      (10->1)
 *      (10->2)
 *      (9->1)
 */
TEST_F(SPQRTreeTests, Test3) {

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

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_03_01 = g_1.addEdge(make_unique<Edge>(),n_03,n_01);
    auto& e_04_01 = g_1.addEdge(make_unique<Edge>(),n_04,n_01);
    auto& e_04_02 = g_1.addEdge(make_unique<Edge>(),n_04,n_02);
    auto& e_02_05 = g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_05_01 = g_1.addEdge(make_unique<Edge>(),n_05,n_01);
    auto& e_06_01 = g_1.addEdge(make_unique<Edge>(),n_06,n_01);
    auto& e_06_02 = g_1.addEdge(make_unique<Edge>(),n_06,n_02);
    auto& e_02_07 = g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    auto& e_07_08 = g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    auto& e_07_01 = g_1.addEdge(make_unique<Edge>(),n_07,n_01);
    auto& e_08_01 = g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    auto& e_08_02 = g_1.addEdge(make_unique<Edge>(),n_08,n_02);
    auto& e_02_09 = g_1.addEdge(make_unique<Edge>(),n_02,n_09);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    auto& e_09_01 = g_1.addEdge(make_unique<Edge>(),n_09,n_01);
    auto& e_10_01 = g_1.addEdge(make_unique<Edge>(),n_10,n_01);
    auto& e_10_02 = g_1.addEdge(make_unique<Edge>(),n_10,n_02);

    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    auto np = make_unique<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
    edge_list_it_t it;
    np->mOrderedAdjacencyType1.clear();
    np->mOrderedAdjacencyType1Back.clear();
    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    sortAdjacency(decomposer, g_2);
    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    SPQRTree     spqrTree(g_1);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    mExplorer3.reset(g_2);
    mExplorer3.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));

    EXPECT_EQ(spqrTree.numNodes(), 7);
    EXPECT_EQ(spqrTree.numEdges(), 6);

    spqrTree.determineTypes();

    auto tnit = spqrTree.nodes().first;
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));

    auto teit = spqrTree.edges().first;
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));

    // Merge tn_02 and tn_04
    spqrTree.mergeTwoNodesP(te_03.backIt(), tn_04.backIt());
    EXPECT_EQ(spqrTree.numNodes(), 6);
    EXPECT_EQ(spqrTree.numEdges(), 5);

    // Node 01
    vector<edge_list_it_t> tc_01_edges_tested;
    tc_01_edges_tested.push_back(e_02_03.backIt());
    tc_01_edges_tested.push_back(e_03_04.backIt());
    tc_01_edges_tested.push_back(e_03_01.backIt());
    tc_01_edges_tested.push_back(e_04_01.backIt());
    tc_01_edges_tested.push_back(e_04_02.backIt());

    vector<node_list_it_t> tc_01_nodes_tested;
    tc_01_nodes_tested.push_back(n_01.backIt());
    tc_01_nodes_tested.push_back(n_02.backIt());
    tc_01_nodes_tested.push_back(n_03.backIt());
    tc_01_nodes_tested.push_back(n_04.backIt());
    vector<edge_list_it_t> tc_01_vedges;
    EXPECT_EQ(testTreeNode(6, tn_01,tc_01_edges_tested,
                                    tc_01_nodes_tested,tc_01_vedges),true);

    // Node 02 merged
    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_01_02.backIt());
    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_01.backIt());
    tc_02_nodes_tested.push_back(n_02.backIt());
    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(testTreeNode(4, tn_02,tc_02_edges_tested,
                                     tc_02_nodes_tested,tc_02_vedges),true);

    // Node 03
    vector<edge_list_it_t> tc_03_edges_tested;
    tc_03_edges_tested.push_back(e_02_05.backIt());
    tc_03_edges_tested.push_back(e_05_06.backIt());
    tc_03_edges_tested.push_back(e_06_01.backIt());
    tc_03_edges_tested.push_back(e_06_02.backIt());
    tc_03_edges_tested.push_back(e_05_01.backIt());

    vector<node_list_it_t> tc_03_nodes_tested;
    tc_03_nodes_tested.push_back(n_01.backIt());
    tc_03_nodes_tested.push_back(n_02.backIt());
    tc_03_nodes_tested.push_back(n_05.backIt());
    tc_03_nodes_tested.push_back(n_06.backIt());
    vector<edge_list_it_t> tc_03_vedges;
    EXPECT_EQ(testTreeNode(6, tn_03,tc_03_edges_tested,
                                    tc_03_nodes_tested,tc_03_vedges),true);

    // Node 04 removed.

    // Node 05
    vector<edge_list_it_t> tc_05_edges_tested;
    tc_05_edges_tested.push_back(e_02_07.backIt());
    tc_05_edges_tested.push_back(e_07_08.backIt());
    tc_05_edges_tested.push_back(e_07_01.backIt());
    tc_05_edges_tested.push_back(e_08_02.backIt());
    tc_05_edges_tested.push_back(e_08_01.backIt());

    vector<node_list_it_t> tc_05_nodes_tested;
    tc_05_nodes_tested.push_back(n_01.backIt());
    tc_05_nodes_tested.push_back(n_02.backIt());
    tc_05_nodes_tested.push_back(n_07.backIt());
    tc_05_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges;
    EXPECT_EQ(testTreeNode(6, tn_05,tc_05_edges_tested,
                                    tc_05_nodes_tested,tc_05_vedges),true);

    // Node 06
    vector<edge_list_it_t> tc_06_edges_tested;
    vector<node_list_it_t> tc_06_nodes_tested;
    tc_06_nodes_tested.push_back(n_01.backIt());
    tc_06_nodes_tested.push_back(n_02.backIt());
    vector<edge_list_it_t> tc_06_vedges;
    EXPECT_EQ(testTreeNode(3, tn_06,tc_06_edges_tested,
                                     tc_06_nodes_tested,tc_06_vedges),true);


    // Node 07
    vector<edge_list_it_t> tc_07_edges_tested;
    tc_07_edges_tested.push_back(e_02_09.backIt());
    tc_07_edges_tested.push_back(e_09_10.backIt());
    tc_07_edges_tested.push_back(e_09_01.backIt());
    tc_07_edges_tested.push_back(e_10_02.backIt());
    tc_07_edges_tested.push_back(e_10_01.backIt());

    vector<node_list_it_t> tc_07_nodes_tested;
    tc_07_nodes_tested.push_back(n_01.backIt());
    tc_07_nodes_tested.push_back(n_02.backIt());
    tc_07_nodes_tested.push_back(n_09.backIt());
    tc_07_nodes_tested.push_back(n_10.backIt());
    vector<edge_list_it_t> tc_07_vedges;
    EXPECT_EQ(testTreeNode(6, tn_07,tc_07_edges_tested,
                                    tc_07_nodes_tested,tc_07_vedges),true);

    // Edge 01 (01-02)
    edge_list_it_t te_01_ve1;
    edge_list_it_t te_01_ve2;
    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                            te_01_ve1, te_01_ve2),true);

    // Edge 02 (03-02)
    edge_list_it_t te_02_ve1;
    edge_list_it_t te_02_ve2;
    EXPECT_EQ(testTreeEdge(te_02, tn_03, tn_02, tc_03_vedges, tc_02_vedges,
                            te_02_ve1, te_02_ve2),true);

    // Edge 03 removed.

    // Edge 04 (05-06)
    edge_list_it_t te_04_ve1;
    edge_list_it_t te_04_ve2;
    EXPECT_EQ(testTreeEdge(te_04, tn_05, tn_06, tc_05_vedges, tc_06_vedges,
                            te_04_ve1, te_04_ve2),true);

    // Edge 05 (02-06)
    edge_list_it_t te_05_ve1;
    edge_list_it_t te_05_ve2;
    EXPECT_EQ(testTreeEdge(te_05, tn_02, tn_06, tc_02_vedges, tc_06_vedges,
                            te_05_ve1, te_05_ve2),true);

    // Edge 06 (06-07)
    edge_list_it_t te_06_ve1;
    edge_list_it_t te_06_ve2;
    EXPECT_EQ(testTreeEdge(te_06, tn_06, tn_07, tc_06_vedges, tc_07_vedges,
                            te_06_ve1, te_06_ve2),true);


}


/**  @brief tests on mergeTwoNodesP() #2
 *   input
 *      (1->2)
 *      (2->3)
 *      (3->4)
 *      (3->1)
 *      (4->1)
 *      (4->2)
 *      (2->5)
 *      (5->6)
 *      (5->1)
 *      (6->1)
 *      (6->2)
 *      (2->7)
 *      (7->8)
 *      (7->1)
 *      (8->1)
 *      (8->2)
 *      (2->9)
 *      (9->10)
 *      (9->1)
 *      (10->1)
 *      (10->2)
 *
 *   tree node 01
 *      (1->2) virtual pair 1
 *      (2->3)
 *      (3->4)
 *      (4->1)
 *      (4->2)
 *      (3->1)
 *
 *   tree node 02
 *      (1->2) virtual pair 1
 *      (1->2)
 *      (1->2) virtual pair 2
 *
 *   tree node 03
 *      (1->2) virtual pair 3
 *      (2->5)
 *      (5->6)
 *      (6->1)
 *      (6->2)
 *      (5->1)
 *
 *   tree node 04
 *      (1->2) virtual pair 3
 *      (1->2) virtual pair 2
 *      (1->2) virtual pair 4
 *
 *   tree node 05
 *      (1->2) virtual pair 5
 *      (2->7)
 *      (7->8)
 *      (8->1)
 *      (8->2)
 *      (7->1)
 *
 *   tree node 04
 *      (1->2) virtual pair 4
 *      (1->2) virtual pair 5
 *      (1->2) virtual pair 6
 *
 *   tree node 05
 *      (1->2) virtual pair 6
 *      (2->9)
 *      (9->10)
 *      (10->1)
 *      (10->2)
 *      (9->1)
 */
TEST_F(SPQRTreeTests, Test4) {

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

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_03_01 = g_1.addEdge(make_unique<Edge>(),n_03,n_01);
    auto& e_04_01 = g_1.addEdge(make_unique<Edge>(),n_04,n_01);
    auto& e_04_02 = g_1.addEdge(make_unique<Edge>(),n_04,n_02);
    auto& e_02_05 = g_1.addEdge(make_unique<Edge>(),n_02,n_05);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_05_01 = g_1.addEdge(make_unique<Edge>(),n_05,n_01);
    auto& e_06_01 = g_1.addEdge(make_unique<Edge>(),n_06,n_01);
    auto& e_06_02 = g_1.addEdge(make_unique<Edge>(),n_06,n_02);
    auto& e_02_07 = g_1.addEdge(make_unique<Edge>(),n_02,n_07);
    auto& e_07_08 = g_1.addEdge(make_unique<Edge>(),n_07,n_08);
    auto& e_07_01 = g_1.addEdge(make_unique<Edge>(),n_07,n_01);
    auto& e_08_01 = g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    auto& e_08_02 = g_1.addEdge(make_unique<Edge>(),n_08,n_02);
    auto& e_02_09 = g_1.addEdge(make_unique<Edge>(),n_02,n_09);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    auto& e_09_01 = g_1.addEdge(make_unique<Edge>(),n_09,n_01);
    auto& e_10_01 = g_1.addEdge(make_unique<Edge>(),n_10,n_01);
    auto& e_10_02 = g_1.addEdge(make_unique<Edge>(),n_10,n_02);

    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    auto np = make_unique<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
    edge_list_it_t it;
    np->mOrderedAdjacencyType1.clear();
    np->mOrderedAdjacencyType1Back.clear();
    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    sortAdjacency(decomposer, g_2);
    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    SPQRTree     spqrTree(g_1);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    mExplorer3.reset(g_2);
    mExplorer3.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));

    EXPECT_EQ(spqrTree.numNodes(), 7);
    EXPECT_EQ(spqrTree.numEdges(), 6);

    spqrTree.determineTypes();

    auto tnit = spqrTree.nodes().first;
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));

    auto teit = spqrTree.edges().first;
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));

    // Merge tn_02 and tn_04
    spqrTree.mergeTwoNodesP(te_03.backIt(), tn_04.backIt());

    // Merge tn_02 and tn_04
    spqrTree.mergeTwoNodesP(te_05.backIt(), tn_06.backIt());

    EXPECT_EQ(spqrTree.numNodes(), 5);
    EXPECT_EQ(spqrTree.numEdges(), 4);

    // Node 01
    vector<edge_list_it_t> tc_01_edges_tested;
    tc_01_edges_tested.push_back(e_02_03.backIt());
    tc_01_edges_tested.push_back(e_03_04.backIt());
    tc_01_edges_tested.push_back(e_03_01.backIt());
    tc_01_edges_tested.push_back(e_04_01.backIt());
    tc_01_edges_tested.push_back(e_04_02.backIt());

    vector<node_list_it_t> tc_01_nodes_tested;
    tc_01_nodes_tested.push_back(n_01.backIt());
    tc_01_nodes_tested.push_back(n_02.backIt());
    tc_01_nodes_tested.push_back(n_03.backIt());
    tc_01_nodes_tested.push_back(n_04.backIt());
    vector<edge_list_it_t> tc_01_vedges;
    EXPECT_EQ(testTreeNode(6, tn_01,tc_01_edges_tested,
                                    tc_01_nodes_tested,tc_01_vedges),true);

    // Node 02 merged
    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_01_02.backIt());
    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_01.backIt());
    tc_02_nodes_tested.push_back(n_02.backIt());
    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(testTreeNode(5, tn_02,tc_02_edges_tested,
                                     tc_02_nodes_tested,tc_02_vedges),true);

    // Node 03
    vector<edge_list_it_t> tc_03_edges_tested;
    tc_03_edges_tested.push_back(e_02_05.backIt());
    tc_03_edges_tested.push_back(e_05_06.backIt());
    tc_03_edges_tested.push_back(e_06_01.backIt());
    tc_03_edges_tested.push_back(e_06_02.backIt());
    tc_03_edges_tested.push_back(e_05_01.backIt());

    vector<node_list_it_t> tc_03_nodes_tested;
    tc_03_nodes_tested.push_back(n_01.backIt());
    tc_03_nodes_tested.push_back(n_02.backIt());
    tc_03_nodes_tested.push_back(n_05.backIt());
    tc_03_nodes_tested.push_back(n_06.backIt());
    vector<edge_list_it_t> tc_03_vedges;
    EXPECT_EQ(testTreeNode(6, tn_03,tc_03_edges_tested,
                                    tc_03_nodes_tested,tc_03_vedges),true);

    // Node 04 removed.

    // Node 05
    vector<edge_list_it_t> tc_05_edges_tested;
    tc_05_edges_tested.push_back(e_02_07.backIt());
    tc_05_edges_tested.push_back(e_07_08.backIt());
    tc_05_edges_tested.push_back(e_07_01.backIt());
    tc_05_edges_tested.push_back(e_08_02.backIt());
    tc_05_edges_tested.push_back(e_08_01.backIt());

    vector<node_list_it_t> tc_05_nodes_tested;
    tc_05_nodes_tested.push_back(n_01.backIt());
    tc_05_nodes_tested.push_back(n_02.backIt());
    tc_05_nodes_tested.push_back(n_07.backIt());
    tc_05_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges;
    EXPECT_EQ(testTreeNode(6, tn_05,tc_05_edges_tested,
                                    tc_05_nodes_tested,tc_05_vedges),true);

    // Node 07
    vector<edge_list_it_t> tc_07_edges_tested;
    tc_07_edges_tested.push_back(e_02_09.backIt());
    tc_07_edges_tested.push_back(e_09_10.backIt());
    tc_07_edges_tested.push_back(e_09_01.backIt());
    tc_07_edges_tested.push_back(e_10_02.backIt());
    tc_07_edges_tested.push_back(e_10_01.backIt());

    vector<node_list_it_t> tc_07_nodes_tested;
    tc_07_nodes_tested.push_back(n_01.backIt());
    tc_07_nodes_tested.push_back(n_02.backIt());
    tc_07_nodes_tested.push_back(n_09.backIt());
    tc_07_nodes_tested.push_back(n_10.backIt());
    vector<edge_list_it_t> tc_07_vedges;
    EXPECT_EQ(testTreeNode(6, tn_07,tc_07_edges_tested,
                                    tc_07_nodes_tested,tc_07_vedges),true);

    // Edge 01 (01-02)
    edge_list_it_t te_01_ve1;
    edge_list_it_t te_01_ve2;
    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                            te_01_ve1, te_01_ve2),true);

    // Edge 02 (03-02)
    edge_list_it_t te_02_ve1;
    edge_list_it_t te_02_ve2;
    EXPECT_EQ(testTreeEdge(te_02, tn_03, tn_02, tc_03_vedges, tc_02_vedges,
                            te_02_ve1, te_02_ve2),true);

    // Edge 03 removed.

    // Edge 04 (05-02)
    edge_list_it_t te_04_ve1;
    edge_list_it_t te_04_ve2;
    EXPECT_EQ(testTreeEdge(te_04, tn_05, tn_02, tc_05_vedges, tc_02_vedges,
                            te_04_ve1, te_04_ve2),true);

    // Edge 05 removed

    // Edge 06 (02-07)
    edge_list_it_t te_06_ve1;
    edge_list_it_t te_06_ve2;
    EXPECT_EQ(testTreeEdge(te_06, tn_02, tn_07, tc_02_vedges, tc_07_vedges,
                            te_06_ve1, te_06_ve2),true);


}


/**  @brief tests on mergeTwoNodesP() #1
 */
TEST_F(SPQRTreeTests, Test5) {

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
    auto& n_11 = g_1.addNode(make_unique<Node>());
    auto& n_12 = g_1.addNode(make_unique<Node>());
    auto& n_13 = g_1.addNode(make_unique<Node>());

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_13 = g_1.addEdge(make_unique<Edge>(),n_03,n_13);
    auto& e_13_01 = g_1.addEdge(make_unique<Edge>(),n_13,n_01);
    auto& e_13_02 = g_1.addEdge(make_unique<Edge>(),n_13,n_02);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    auto& e_07_04 = g_1.addEdge(make_unique<Edge>(),n_07,n_04);
    auto& e_07_05 = g_1.addEdge(make_unique<Edge>(),n_07,n_05);
    auto& e_06_04 = g_1.addEdge(make_unique<Edge>(),n_06,n_04);
    auto& e_05_08 = g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    auto& e_08_01 = g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    auto& e_08_09 = g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    auto& e_10_12 = g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    auto& e_12_01 = g_1.addEdge(make_unique<Edge>(),n_12,n_01);
    auto& e_12_08 = g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    auto& e_12_09 = g_1.addEdge(make_unique<Edge>(),n_12,n_09);
    auto& e_10_11 = g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    auto& e_11_08 = g_1.addEdge(make_unique<Edge>(),n_11,n_08);
    auto& e_11_09 = g_1.addEdge(make_unique<Edge>(),n_11,n_09);


    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    auto np = make_unique<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
    edge_list_it_t it;
    np->mOrderedAdjacencyType1.clear();
    np->mOrderedAdjacencyType1Back.clear();
    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    sortAdjacency(decomposer, g_2);
    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    SPQRTree     spqrTree(g_1);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    mExplorer3.reset(g_2);
    mExplorer3.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));

    EXPECT_EQ(spqrTree.numNodes(), 10);
    EXPECT_EQ(spqrTree.numEdges(),  9);

    spqrTree.determineTypes();

    auto tnit = spqrTree.nodes().first;
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_08 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_09 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_10 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));

    auto teit = spqrTree.edges().first;
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_06_tmp = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    auto  te_06_it = teit++;
    auto  te_07_it = teit++;
    auto& te_08 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_09 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));


    // Node 05 tmp
    vector<edge_list_it_t> tc_05_edges_tested_tmp;
    tc_05_edges_tested_tmp.push_back(e_05_08.backIt());
    vector<node_list_it_t> tc_05_nodes_tested_tmp;
    tc_05_nodes_tested_tmp.push_back(n_01.backIt());
    tc_05_nodes_tested_tmp.push_back(n_05.backIt());
    tc_05_nodes_tested_tmp.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges_tmp;
    EXPECT_EQ(testTreeNode(3, tn_05,tc_05_edges_tested_tmp,
                             tc_05_nodes_tested_tmp,tc_05_vedges_tmp),true);
    // Node 08 tmp
    vector<edge_list_it_t> tc_08_edges_tested_tmp;
    vector<node_list_it_t> tc_08_nodes_tested_tmp;
    tc_08_nodes_tested_tmp.push_back(n_01.backIt());
    tc_08_nodes_tested_tmp.push_back(n_04.backIt());
    tc_08_nodes_tested_tmp.push_back(n_05.backIt());
    vector<edge_list_it_t> tc_08_vedges_tmp;
    EXPECT_EQ(testTreeNode(3, tn_08,tc_08_edges_tested_tmp,
                               tc_08_nodes_tested_tmp,tc_08_vedges_tmp),true);

    edge_list_it_t te_06_ve1_tmp;
    edge_list_it_t te_06_ve2_tmp;
    if(testTreeEdge(te_06_tmp, tn_05, tn_08,
                    tc_05_vedges_tmp, tc_08_vedges_tmp,
                    te_06_ve1_tmp, te_06_ve2_tmp)==true){
        te_06_it++;
        te_07_it--;
    }

    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*te_06_it));
    auto& te_07 = dynamic_cast<SPQRTreeEdge&>(*(*te_07_it));

    // Merge tn_05 and tn_08 (te_07)
    spqrTree.mergeTwoNodesS(te_07.backIt(), tn_08.backIt());

    EXPECT_EQ(spqrTree.numNodes(),9);
    EXPECT_EQ(spqrTree.numEdges(),8);

    // Node 01
    vector<edge_list_it_t> tc_01_edges_tested;
    tc_01_edges_tested.push_back(e_12_09.backIt());
    tc_01_edges_tested.push_back(e_10_12.backIt());
    tc_01_edges_tested.push_back(e_11_08.backIt());
    tc_01_edges_tested.push_back(e_11_09.backIt());
    tc_01_edges_tested.push_back(e_10_11.backIt());
    tc_01_edges_tested.push_back(e_09_10.backIt());
    tc_01_edges_tested.push_back(e_08_09.backIt());
    vector<node_list_it_t> tc_01_nodes_tested;
    tc_01_nodes_tested.push_back(n_12.backIt());
    tc_01_nodes_tested.push_back(n_08.backIt());
    tc_01_nodes_tested.push_back(n_09.backIt());
    tc_01_nodes_tested.push_back(n_10.backIt());
    tc_01_nodes_tested.push_back(n_11.backIt());
    vector<edge_list_it_t> tc_01_vedges;
    EXPECT_EQ(testTreeNode(8, tn_01,tc_01_edges_tested,
                                     tc_01_nodes_tested,tc_01_vedges),true);

    // Node 02
    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_12_08.backIt());
    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_12.backIt());
    tc_02_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(testTreeNode(3, tn_02,tc_02_edges_tested,
                                     tc_02_nodes_tested,tc_02_vedges),true);

    // Node 03
    vector<edge_list_it_t> tc_03_edges_tested;
    tc_03_edges_tested.push_back(e_12_01.backIt());
    vector<node_list_it_t> tc_03_nodes_tested;
    tc_03_nodes_tested.push_back(n_01.backIt());
    tc_03_nodes_tested.push_back(n_08.backIt());
    tc_03_nodes_tested.push_back(n_12.backIt());
    vector<edge_list_it_t> tc_03_vedges;
    EXPECT_EQ(testTreeNode(3, tn_03,tc_03_edges_tested,
                                     tc_03_nodes_tested,tc_03_vedges),true);

    // Node 04
    vector<edge_list_it_t> tc_04_edges_tested;
    tc_04_edges_tested.push_back(e_08_01.backIt());
    vector<node_list_it_t> tc_04_nodes_tested;
    tc_04_nodes_tested.push_back(n_01.backIt());
    tc_04_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_04_vedges;
    EXPECT_EQ(testTreeNode(3, tn_04,tc_04_edges_tested,
                                    tc_04_nodes_tested,tc_04_vedges),true);

    // Node 05
    vector<edge_list_it_t> tc_05_edges_tested;
    tc_05_edges_tested.push_back(e_05_08.backIt());
    vector<node_list_it_t> tc_05_nodes_tested;
    tc_05_nodes_tested.push_back(n_01.backIt());
    tc_05_nodes_tested.push_back(n_04.backIt());
    tc_05_nodes_tested.push_back(n_05.backIt());
    tc_05_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges;
    EXPECT_EQ(testTreeNode(4, tn_05,tc_05_edges_tested,
                             tc_05_nodes_tested,tc_05_vedges),true);

    // Node 06
    vector<edge_list_it_t> tc_06_edges_tested;
    tc_06_edges_tested.push_back(e_05_06.backIt());
    tc_06_edges_tested.push_back(e_06_07.backIt());
    tc_06_edges_tested.push_back(e_06_04.backIt());
    tc_06_edges_tested.push_back(e_07_04.backIt());
    tc_06_edges_tested.push_back(e_07_05.backIt());
    vector<node_list_it_t> tc_06_nodes_tested;
    tc_06_nodes_tested.push_back(n_04.backIt());
    tc_06_nodes_tested.push_back(n_05.backIt());
    tc_06_nodes_tested.push_back(n_06.backIt());
    tc_06_nodes_tested.push_back(n_07.backIt());
    vector<edge_list_it_t> tc_06_vedges;
    EXPECT_EQ(testTreeNode(6, tn_06,tc_06_edges_tested,
                                    tc_06_nodes_tested,tc_06_vedges),true);

    // Node 07
    vector<edge_list_it_t> tc_07_edges_tested;
    tc_07_edges_tested.push_back(e_04_05.backIt());
    vector<node_list_it_t> tc_07_nodes_tested;
    tc_07_nodes_tested.push_back(n_04.backIt());
    tc_07_nodes_tested.push_back(n_05.backIt());
    vector<edge_list_it_t> tc_07_vedges;
    EXPECT_EQ(testTreeNode(3, tn_07,tc_07_edges_tested,
                              tc_07_nodes_tested,tc_07_vedges),true);

    // Node 08 removed.

    // Node 09
    vector<edge_list_it_t> tc_09_edges_tested;
    tc_09_edges_tested.push_back(e_03_04.backIt());
    vector<node_list_it_t> tc_09_nodes_tested;
    tc_09_nodes_tested.push_back(n_01.backIt());
    tc_09_nodes_tested.push_back(n_03.backIt());
    tc_09_nodes_tested.push_back(n_04.backIt());
    vector<edge_list_it_t> tc_09_vedges;
    EXPECT_EQ(testTreeNode(3, tn_09,tc_09_edges_tested,
                                    tc_09_nodes_tested,tc_09_vedges),true);

    // Node 10
    vector<edge_list_it_t> tc_10_edges_tested;
    tc_10_edges_tested.push_back(e_01_02.backIt());
    tc_10_edges_tested.push_back(e_02_03.backIt());
    tc_10_edges_tested.push_back(e_03_13.backIt());
    tc_10_edges_tested.push_back(e_13_01.backIt());
    tc_10_edges_tested.push_back(e_13_02.backIt());
    vector<node_list_it_t> tc_10_nodes_tested;
    tc_10_nodes_tested.push_back(n_01.backIt());
    tc_10_nodes_tested.push_back(n_02.backIt());
    tc_10_nodes_tested.push_back(n_03.backIt());
    tc_10_nodes_tested.push_back(n_13.backIt());
    vector<edge_list_it_t> tc_10_vedges;
    EXPECT_EQ(testTreeNode(6, tn_10,tc_10_edges_tested,
                                    tc_10_nodes_tested,tc_10_vedges),true);

    // Edge 01 (01-02)
    edge_list_it_t te_01_ve1;
    edge_list_it_t te_01_ve2;
    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                            te_01_ve1, te_01_ve2),true);

    // Edge 02 (02-03)
    edge_list_it_t te_02_ve1;
    edge_list_it_t te_02_ve2;
    EXPECT_EQ(testTreeEdge(te_02, tn_02, tn_03, tc_02_vedges, tc_03_vedges,
                            te_02_ve1, te_02_ve2),true);

    // Edge 03 (03-04)
    edge_list_it_t te_03_ve1;
    edge_list_it_t te_03_ve2;
    EXPECT_EQ(testTreeEdge(te_03, tn_03, tn_04, tc_03_vedges, tc_04_vedges,
                            te_03_ve1, te_03_ve2),true);

    // Edge 04 (04-05)
    edge_list_it_t te_04_ve1;
    edge_list_it_t te_04_ve2;
    EXPECT_EQ(testTreeEdge(te_04, tn_04, tn_05, tc_04_vedges, tc_05_vedges,
                            te_04_ve1, te_04_ve2),true);

    // Edge 05 (06-07)
    edge_list_it_t te_05_ve1;
    edge_list_it_t te_05_ve2;
    EXPECT_EQ(testTreeEdge(te_05, tn_06, tn_07, tc_06_vedges, tc_07_vedges,
                            te_05_ve1, te_05_ve2),true);

    // Edge 06 (05-07)
    edge_list_it_t te_06_ve1;
    edge_list_it_t te_06_ve2;
    EXPECT_EQ(testTreeEdge(te_06, tn_05, tn_07, tc_05_vedges, tc_07_vedges,
                                                  te_06_ve1, te_06_ve2),true);

    // Edge 07 (05-08) removed

    // Edge 08 (05-09)
    edge_list_it_t te_08_ve1;
    edge_list_it_t te_08_ve2;
    EXPECT_EQ(testTreeEdge(te_08, tn_05, tn_09, tc_05_vedges, tc_09_vedges,
                            te_08_ve1, te_08_ve2),true);

    // Edge 09 (09-10)
    edge_list_it_t te_09_ve1;
    edge_list_it_t te_09_ve2;
    EXPECT_EQ(testTreeEdge(te_09, tn_09, tn_10, tc_09_vedges, tc_10_vedges,
                            te_09_ve1, te_09_ve2),true);

}


/**  @brief tests on mergeTwoNodesP() #2
 */
TEST_F(SPQRTreeTests, Test6) {

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
    auto& n_11 = g_1.addNode(make_unique<Node>());
    auto& n_12 = g_1.addNode(make_unique<Node>());
    auto& n_13 = g_1.addNode(make_unique<Node>());

    auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01,n_02);
    auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02,n_03);
    auto& e_03_13 = g_1.addEdge(make_unique<Edge>(),n_03,n_13);
    auto& e_13_01 = g_1.addEdge(make_unique<Edge>(),n_13,n_01);
    auto& e_13_02 = g_1.addEdge(make_unique<Edge>(),n_13,n_02);
    auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03,n_04);
    auto& e_04_05 = g_1.addEdge(make_unique<Edge>(),n_04,n_05);
    auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05,n_06);
    auto& e_06_07 = g_1.addEdge(make_unique<Edge>(),n_06,n_07);
    auto& e_07_04 = g_1.addEdge(make_unique<Edge>(),n_07,n_04);
    auto& e_07_05 = g_1.addEdge(make_unique<Edge>(),n_07,n_05);
    auto& e_06_04 = g_1.addEdge(make_unique<Edge>(),n_06,n_04);
    auto& e_05_08 = g_1.addEdge(make_unique<Edge>(),n_05,n_08);
    auto& e_08_01 = g_1.addEdge(make_unique<Edge>(),n_08,n_01);
    auto& e_08_09 = g_1.addEdge(make_unique<Edge>(),n_08,n_09);
    auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09,n_10);
    auto& e_10_12 = g_1.addEdge(make_unique<Edge>(),n_10,n_12);
    auto& e_12_01 = g_1.addEdge(make_unique<Edge>(),n_12,n_01);
    auto& e_12_08 = g_1.addEdge(make_unique<Edge>(),n_12,n_08);
    auto& e_12_09 = g_1.addEdge(make_unique<Edge>(),n_12,n_09);
    auto& e_10_11 = g_1.addEdge(make_unique<Edge>(),n_10,n_11);
    auto& e_11_08 = g_1.addEdge(make_unique<Edge>(),n_11,n_08);
    auto& e_11_09 = g_1.addEdge(make_unique<Edge>(),n_11,n_09);


    DFSGraph<Graph, SPQR_DFS_Node, SPQR_DFS_Edge> g_2;

    SPQRDecomposer decomposer;

    auto np = make_unique<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>>();
    edge_list_it_t it;
    np->mOrderedAdjacencyType1.clear();
    np->mOrderedAdjacencyType1Back.clear();
    /*auto nd_start_it = */copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit++));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    sortAdjacency(decomposer, g_2);
    SPQRStrategyProc2 strategy2;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
                                                 mExplorer2(g_2,strategy2);
    strategy2.prepareNodesForDFS(g_2);

    mExplorer2.reset(g_2);
    mExplorer2.run(dynamic_cast<
                     DFSNode<Graph, SPQR_DFS_Node,SPQR_DFS_Edge>&>(nc_01));

    SPQRTree     spqrTree(g_1);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    mExplorer3.reset(g_2);
    mExplorer3.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));

    EXPECT_EQ(spqrTree.numNodes(), 10);
    EXPECT_EQ(spqrTree.numEdges(),  9);

    spqrTree.determineTypes();

    auto tnit = spqrTree.nodes().first;
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_08 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_09 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_10 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));

    auto teit = spqrTree.edges().first;
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_06_tmp = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    auto  te_06_it = teit++;
    auto  te_07_it = teit++;
    auto& te_08 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_09 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));


    // Node 05 tmp
    vector<edge_list_it_t> tc_05_edges_tested_tmp;
    tc_05_edges_tested_tmp.push_back(e_05_08.backIt());
    vector<node_list_it_t> tc_05_nodes_tested_tmp;
    tc_05_nodes_tested_tmp.push_back(n_01.backIt());
    tc_05_nodes_tested_tmp.push_back(n_05.backIt());
    tc_05_nodes_tested_tmp.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges_tmp;
    EXPECT_EQ(testTreeNode(3, tn_05,tc_05_edges_tested_tmp,
                             tc_05_nodes_tested_tmp,tc_05_vedges_tmp),true);
    // Node 08 tmp
    vector<edge_list_it_t> tc_08_edges_tested_tmp;
    vector<node_list_it_t> tc_08_nodes_tested_tmp;
    tc_08_nodes_tested_tmp.push_back(n_01.backIt());
    tc_08_nodes_tested_tmp.push_back(n_04.backIt());
    tc_08_nodes_tested_tmp.push_back(n_05.backIt());
    vector<edge_list_it_t> tc_08_vedges_tmp;
    EXPECT_EQ(testTreeNode(3, tn_08,tc_08_edges_tested_tmp,
                               tc_08_nodes_tested_tmp,tc_08_vedges_tmp),true);

    edge_list_it_t te_06_ve1_tmp;
    edge_list_it_t te_06_ve2_tmp;
    if(testTreeEdge(te_06_tmp, tn_05, tn_08,
                    tc_05_vedges_tmp, tc_08_vedges_tmp,
                    te_06_ve1_tmp, te_06_ve2_tmp)==true){
        te_06_it++;
        te_07_it--;
    }

    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*te_06_it));
    auto& te_07 = dynamic_cast<SPQRTreeEdge&>(*(*te_07_it));

    // Merge tn_05 and tn_08 (te_07)
    spqrTree.mergeTwoNodesS(te_07.backIt(), tn_08.backIt());

    // Merge tn_05 and tn_09 (te_08)
    spqrTree.mergeTwoNodesS(te_08.backIt(), tn_09.backIt());

    EXPECT_EQ(spqrTree.numNodes(),8);
    EXPECT_EQ(spqrTree.numEdges(),7);

    // Node 01
    vector<edge_list_it_t> tc_01_edges_tested;
    tc_01_edges_tested.push_back(e_12_09.backIt());
    tc_01_edges_tested.push_back(e_10_12.backIt());
    tc_01_edges_tested.push_back(e_11_08.backIt());
    tc_01_edges_tested.push_back(e_11_09.backIt());
    tc_01_edges_tested.push_back(e_10_11.backIt());
    tc_01_edges_tested.push_back(e_09_10.backIt());
    tc_01_edges_tested.push_back(e_08_09.backIt());
    vector<node_list_it_t> tc_01_nodes_tested;
    tc_01_nodes_tested.push_back(n_12.backIt());
    tc_01_nodes_tested.push_back(n_08.backIt());
    tc_01_nodes_tested.push_back(n_09.backIt());
    tc_01_nodes_tested.push_back(n_10.backIt());
    tc_01_nodes_tested.push_back(n_11.backIt());
    vector<edge_list_it_t> tc_01_vedges;
    EXPECT_EQ(testTreeNode(8, tn_01,tc_01_edges_tested,
                                     tc_01_nodes_tested,tc_01_vedges),true);

    // Node 02
    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_12_08.backIt());
    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_12.backIt());
    tc_02_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(testTreeNode(3, tn_02,tc_02_edges_tested,
                                     tc_02_nodes_tested,tc_02_vedges),true);

    // Node 03
    vector<edge_list_it_t> tc_03_edges_tested;
    tc_03_edges_tested.push_back(e_12_01.backIt());
    vector<node_list_it_t> tc_03_nodes_tested;
    tc_03_nodes_tested.push_back(n_01.backIt());
    tc_03_nodes_tested.push_back(n_08.backIt());
    tc_03_nodes_tested.push_back(n_12.backIt());
    vector<edge_list_it_t> tc_03_vedges;
    EXPECT_EQ(testTreeNode(3, tn_03,tc_03_edges_tested,
                                     tc_03_nodes_tested,tc_03_vedges),true);

    // Node 04
    vector<edge_list_it_t> tc_04_edges_tested;
    tc_04_edges_tested.push_back(e_08_01.backIt());
    vector<node_list_it_t> tc_04_nodes_tested;
    tc_04_nodes_tested.push_back(n_01.backIt());
    tc_04_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_04_vedges;
    EXPECT_EQ(testTreeNode(3, tn_04,tc_04_edges_tested,
                                    tc_04_nodes_tested,tc_04_vedges),true);

    // Node 05
    vector<edge_list_it_t> tc_05_edges_tested;
    tc_05_edges_tested.push_back(e_05_08.backIt());
    tc_05_edges_tested.push_back(e_03_04.backIt());
    vector<node_list_it_t> tc_05_nodes_tested;
    tc_05_nodes_tested.push_back(n_01.backIt());
    tc_05_nodes_tested.push_back(n_03.backIt());
    tc_05_nodes_tested.push_back(n_04.backIt());
    tc_05_nodes_tested.push_back(n_05.backIt());
    tc_05_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges;
    EXPECT_EQ(testTreeNode(5, tn_05,tc_05_edges_tested,
                                   tc_05_nodes_tested,tc_05_vedges),true);

    // Node 06
    vector<edge_list_it_t> tc_06_edges_tested;
    tc_06_edges_tested.push_back(e_05_06.backIt());
    tc_06_edges_tested.push_back(e_06_07.backIt());
    tc_06_edges_tested.push_back(e_06_04.backIt());
    tc_06_edges_tested.push_back(e_07_04.backIt());
    tc_06_edges_tested.push_back(e_07_05.backIt());
    vector<node_list_it_t> tc_06_nodes_tested;
    tc_06_nodes_tested.push_back(n_04.backIt());
    tc_06_nodes_tested.push_back(n_05.backIt());
    tc_06_nodes_tested.push_back(n_06.backIt());
    tc_06_nodes_tested.push_back(n_07.backIt());
    vector<edge_list_it_t> tc_06_vedges;
    EXPECT_EQ(testTreeNode(6, tn_06,tc_06_edges_tested,
                                    tc_06_nodes_tested,tc_06_vedges),true);

    // Node 07
    vector<edge_list_it_t> tc_07_edges_tested;
    tc_07_edges_tested.push_back(e_04_05.backIt());
    vector<node_list_it_t> tc_07_nodes_tested;
    tc_07_nodes_tested.push_back(n_04.backIt());
    tc_07_nodes_tested.push_back(n_05.backIt());
    vector<edge_list_it_t> tc_07_vedges;
    EXPECT_EQ(testTreeNode(3, tn_07,tc_07_edges_tested,
                              tc_07_nodes_tested,tc_07_vedges),true);

    // Node 08 removed.

    // Node 09 removed.

    // Node 10
    vector<edge_list_it_t> tc_10_edges_tested;
    tc_10_edges_tested.push_back(e_01_02.backIt());
    tc_10_edges_tested.push_back(e_02_03.backIt());
    tc_10_edges_tested.push_back(e_03_13.backIt());
    tc_10_edges_tested.push_back(e_13_01.backIt());
    tc_10_edges_tested.push_back(e_13_02.backIt());
    vector<node_list_it_t> tc_10_nodes_tested;
    tc_10_nodes_tested.push_back(n_01.backIt());
    tc_10_nodes_tested.push_back(n_02.backIt());
    tc_10_nodes_tested.push_back(n_03.backIt());
    tc_10_nodes_tested.push_back(n_13.backIt());
    vector<edge_list_it_t> tc_10_vedges;
    EXPECT_EQ(testTreeNode(6, tn_10,tc_10_edges_tested,
                                    tc_10_nodes_tested,tc_10_vedges),true);

    // Edge 01 (01-02)
    edge_list_it_t te_01_ve1;
    edge_list_it_t te_01_ve2;
    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                            te_01_ve1, te_01_ve2),true);

    // Edge 02 (02-03)
    edge_list_it_t te_02_ve1;
    edge_list_it_t te_02_ve2;
    EXPECT_EQ(testTreeEdge(te_02, tn_02, tn_03, tc_02_vedges, tc_03_vedges,
                            te_02_ve1, te_02_ve2),true);

    // Edge 03 (03-04)
    edge_list_it_t te_03_ve1;
    edge_list_it_t te_03_ve2;
    EXPECT_EQ(testTreeEdge(te_03, tn_03, tn_04, tc_03_vedges, tc_04_vedges,
                            te_03_ve1, te_03_ve2),true);

    // Edge 04 (04-05)
    edge_list_it_t te_04_ve1;
    edge_list_it_t te_04_ve2;
    EXPECT_EQ(testTreeEdge(te_04, tn_04, tn_05, tc_04_vedges, tc_05_vedges,
                            te_04_ve1, te_04_ve2),true);

    // Edge 05 (06-07)
    edge_list_it_t te_05_ve1;
    edge_list_it_t te_05_ve2;
    EXPECT_EQ(testTreeEdge(te_05, tn_06, tn_07, tc_06_vedges, tc_07_vedges,
                            te_05_ve1, te_05_ve2),true);

    // Edge 06 (05-07)
    edge_list_it_t te_06_ve1;
    edge_list_it_t te_06_ve2;
    EXPECT_EQ(testTreeEdge(te_06, tn_05, tn_07, tc_05_vedges, tc_07_vedges,
                                                  te_06_ve1, te_06_ve2),true);

    // Edge 07 (05-08) removed

    // Edge 08 (05-09) removed

    // Edge 09 (05-10)
    edge_list_it_t te_09_ve1;
    edge_list_it_t te_09_ve2;
    EXPECT_EQ(testTreeEdge(te_09, tn_05, tn_10,
                     tc_05_vedges, tc_10_vedges, te_09_ve1, te_09_ve2), true);

}

} // namespace Undirected

} // namespace Wailea
