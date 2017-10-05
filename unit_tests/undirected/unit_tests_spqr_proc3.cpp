#include "gtest/gtest.h"
#include "undirected/spqr_decomposer.hpp"

namespace Wailea {

namespace Undirected { 

using DExplorer = DFSExplorer <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DStrategy = DFSStrategy <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DGraph    = DFSGraph    <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DNode     = DFSNode     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DEdge     = DFSEdge     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;

class SPQRProc3Tests : public ::testing::Test {

  protected:  

    SPQRProc3Tests(){;};
    virtual ~SPQRProc3Tests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    node_list_it_t copyInputGraph(SPQRDecomposer& decomp,
                            Graph& src, Graph& dst, node_list_it_t startNode){
        return decomp.copyInputGraph(src,dst,startNode);
    }

    void sortAdjacency(SPQRDecomposer& decomp, Graph& g){
        decomp.sortAdjacency(g);
    }

    bool mOutgoingPath(SPQRStrategyProc3& s){
        return s.mOutgoingPath;
    };

    void setStartNode(DExplorer& e, DNode& n) { e.setStartNode(n);}
    bool oneStep(DExplorer& e) { return e.oneStep();}

    vector<SPQRNodeMapElem>& nmap(SPQRStrategyProc3&s) { return s.mNMap; }

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


/**  @brief tests on sortAdjacency.
 *   
 *   input:
 *   (1->2)
 *   (2->3)
 *   (3->4)
 *   (3->13)
 *   (13[_/>1)
 *   (13[_/>2)
 *   (4->5)
 *   (5->6)
 *   (6->7)
 *   (6[_/>4)
 *   (7->8)
 *   (7[_/>4)
 *   (7[_/>5)
 *   (5->8)
 *   (8->9)
 *   (8[_/>1)
 *   (9->10)
 *   (10->11)
 *   (11[_/>8)
 *   (11[_/>9)
 *   (10->12)
 *   (12[_/>1)
 *   (12[_/>8)
 *   (12[_/>9)
 *
 *   expected results
 *   
 *     tree node 01
 *       8->9
 *       9->10
 *       10->12
 *       12->8  (virtual pair 1)
 *       12->9
 *       10->11
 *       11->9
 *       11->8
 *
 *     tree node 02
 *       12->8  (virtual pair 1)
 *       12->8
 *       12->8  (virtual pair 2)
 *
 *     tree node 03
 *       8->1   (virtual pair 3)
 *       8->12  (virtual pair 2)
 *       12->1
 *
 *     tree node 04
 *       8->1   (virtual pair 3)
 *       8->1   (virtual pair 4)
 *       8->1
 *
 *     tree node 05
 *       5->1   (virtual pair 6/7)
 *       5->8
 *       8->1   (virtual pair 4)
 *
 *     tree node 06
 *       4->5   (virtual pair 5)
 *       5->6
 *       6->7
 *       6->4
 *       7->4
 *       7->5
 *
 *     tree node 07
 *       4->5   (virtual pair 5)
 *       4->5   (virtual pair 6/7)
 *       4->5
 *
 *     tree node 08
 *       1->4   (virtual pair 8)
 *       4->5   (virtual pair 6/7)
 *       5->1   (virtual pair 6/7)
 *
 *     tree node 09
 *       1->3   (virtual pair 9)
 *       3->4
 *       4->1   (virtual pair 8)
 *
 *     tree node 10
 *       1->2
 *       2->3
 *       3->13
 *       13->1
 *       13->2
 *       3->1   (virtual pair 9)
 *
 *     tree edge 1-2 virtual pair 1
 *     tree edge 2-3 virtual pair 2
 *     tree edge 3-4 virtual pair 3
 *     tree edge 4-5 virtual pair 4
 *     tree edge 6-7 virtual pair 5
 *     tree edge 5-8 virtual pair 6/7
 *     tree edge 7-8 virtual pair 6/7
 *     tree edge 8-9 virtual pair 8
 *     tree edge 9-10 virtual pair 9
 *
 */
TEST_F(SPQRProc3Tests, Test1) {

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

    SPQRTree     spqrTree(g_2);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    EXPECT_EQ(mOutgoingPath(strategy3), false);

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

    EXPECT_EQ(nc_01.degree(), 4);
    auto lit = nc_01.incidentEdges().first;
    auto& nc_01_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_01, &ec_01_02);
    auto& nc_01_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_02, &ec_13_01);
    auto& nc_01_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_03, &ec_08_01);
    auto& nc_01_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_04, &ec_12_01);

    EXPECT_EQ(nc_02.degree(), 3);
    lit = nc_02.incidentEdges().first;
    auto& nc_02_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_01, &ec_01_02);
    auto& nc_02_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_02, &ec_02_03);
    auto& nc_02_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_03, &ec_13_02);

    EXPECT_EQ(nc_03.degree(), 3);
    lit = nc_03.incidentEdges().first;
    auto& nc_03_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_01, &ec_03_13);
    auto& nc_03_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_02, &ec_02_03);
    auto& nc_03_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_03, &ec_03_04);

    EXPECT_EQ(nc_13.degree(), 3);
    lit = nc_13.incidentEdges().first;
    auto& nc_13_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_01, &ec_03_13);
    auto& nc_13_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_02, &ec_13_01);
    auto& nc_13_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_03, &ec_13_02);

    EXPECT_EQ(nc_04.degree(), 4);
    lit = nc_04.incidentEdges().first;
    auto& nc_04_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_01, &ec_03_04);
    auto& nc_04_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_02, &ec_04_05);
    auto& nc_04_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_03, &ec_07_04);
    auto& nc_04_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_04, &ec_06_04);

    EXPECT_EQ(nc_05.degree(), 4);
    lit = nc_05.incidentEdges().first;
    auto& nc_05_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_01, &ec_04_05);
    auto& nc_05_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_02, &ec_05_08);
    auto& nc_05_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_03, &ec_05_06);
    auto& nc_05_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_04, &ec_07_05);

    EXPECT_EQ(nc_06.degree(), 3);
    lit = nc_06.incidentEdges().first;
    auto& nc_06_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_01, &ec_06_07);
    auto& nc_06_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_02, &ec_06_04);
    auto& nc_06_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_03, &ec_05_06);

    EXPECT_EQ(nc_07.degree(), 3);
    lit = nc_07.incidentEdges().first;
    auto& nc_07_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_01, &ec_06_07);
    auto& nc_07_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_02, &ec_07_04);
    auto& nc_07_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_03, &ec_07_05);

    EXPECT_EQ(nc_08.degree(), 5);
    lit = nc_08.incidentEdges().first;
    auto& nc_08_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_01, &ec_08_01);
    auto& nc_08_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_02, &ec_05_08);
    auto& nc_08_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_03, &ec_08_09);
    auto& nc_08_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_04, &ec_12_08);
    auto& nc_08_05 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_05, &ec_11_08);

    EXPECT_EQ(nc_09.degree(), 4);
    lit = nc_09.incidentEdges().first;
    auto& nc_09_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_01, &ec_09_10);
    auto& nc_09_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_02, &ec_08_09);
    auto& nc_09_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_03, &ec_12_09);
    auto& nc_09_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_04, &ec_11_09);

    EXPECT_EQ(nc_10.degree(), 3);
    lit = nc_10.incidentEdges().first;
    auto& nc_10_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_01, &ec_09_10);
    auto& nc_10_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_02, &ec_10_12);
    auto& nc_10_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_03, &ec_10_11);

    EXPECT_EQ(nc_11.degree(), 3);
    lit = nc_11.incidentEdges().first;
    auto& nc_11_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_01, &ec_10_11);
    auto& nc_11_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_02, &ec_11_08);
    auto& nc_11_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_03, &ec_11_09);

    EXPECT_EQ(nc_12.degree(), 4);
    lit = nc_12.incidentEdges().first;
    auto& nc_12_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_01, &ec_10_12);
    auto& nc_12_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_02, &ec_12_01);
    auto& nc_12_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_03, &ec_12_08);
    auto& nc_12_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_04, &ec_12_09);

    vector<SPQRNodeMapElem>& nMap = nmap(strategy3);

    EXPECT_EQ(nMap.size(),14);
    EXPECT_EQ(nMap[1].mIt, nc_01.backIt());
    EXPECT_EQ(nMap[1].mRemoved, false);
    EXPECT_EQ(nMap[2].mIt, nc_02.backIt());
    EXPECT_EQ(nMap[2].mRemoved, false);
    EXPECT_EQ(nMap[3].mIt, nc_03.backIt());
    EXPECT_EQ(nMap[3].mRemoved, false);
    EXPECT_EQ(nMap[4].mIt, nc_04.backIt());
    EXPECT_EQ(nMap[4].mRemoved, false);
    EXPECT_EQ(nMap[5].mIt, nc_05.backIt());
    EXPECT_EQ(nMap[5].mRemoved, false);
    EXPECT_EQ(nMap[6].mIt, nc_06.backIt());
    EXPECT_EQ(nMap[6].mRemoved, false);
    EXPECT_EQ(nMap[7].mIt, nc_07.backIt());
    EXPECT_EQ(nMap[7].mRemoved, false);
    EXPECT_EQ(nMap[8].mIt, nc_08.backIt());
    EXPECT_EQ(nMap[8].mRemoved, false);
    EXPECT_EQ(nMap[9].mIt, nc_09.backIt());
    EXPECT_EQ(nMap[9].mRemoved, false);
    EXPECT_EQ(nMap[10].mIt, nc_10.backIt());
    EXPECT_EQ(nMap[10].mRemoved, false);
    EXPECT_EQ(nMap[11].mIt, nc_11.backIt());
    EXPECT_EQ(nMap[11].mRemoved, false);
    EXPECT_EQ(nMap[12].mIt, nc_12.backIt());
    EXPECT_EQ(nMap[12].mRemoved, false);
    EXPECT_EQ(nMap[13].mIt, nc_13.backIt());
    EXPECT_EQ(nMap[13].mRemoved, false);

    mExplorer3.reset(g_2);

    setStartNode(mExplorer3, nc_01);

    // enter nc_01
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_01, ec_01_02)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_02
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_01_02
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_02, ec_02_03)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_03
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_03, ec_03_13)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_13
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_03_13
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_13, ec_13_01)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_13, ec_13_02)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave ne_13
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_03, ec_03_13)
    // skip ec_02_03
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_03, ec_03_04)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_04
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_03_04
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_04, ec_04_05)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_05
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_04_05
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_05, ec_05_08)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_08
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_08, ec_08_01)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_05_08
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_08, ec_08_09)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_09
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_09, ec_09_10)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_10
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_09_10
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_10, ec_10_12)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_12
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_10_12
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_12, ec_12_01)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_12, ec_12_08)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_12, ec_12_09)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave ne_12
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_10, ec_10_12)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_10, ec_10_11)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_11
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_10_11
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_11, ec_11_08)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_11, ec_11_09)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave nc_11
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_10, ec_10_11)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave nc_10
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_09, ec_09_10)
    // skip ec_08_09
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave nc_09
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,0);
    EXPECT_EQ(spqrTree.numEdges() ,0);

    // afterVisitingChild(nc_08, ec_08_09)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,4);
    EXPECT_EQ(spqrTree.numEdges() ,3);

    auto tnit = spqrTree.nodes().first;
    auto teit = spqrTree.edges().first;


    // Node 01
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

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
    tnit++;
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_12_08.backIt());
    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_12.backIt());
    tc_02_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(testTreeNode(3, tn_02,tc_02_edges_tested,
                                     tc_02_nodes_tested,tc_02_vedges),true);

    // Edge 01 (01-02)
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_01_ve1;
    edge_list_it_t te_01_ve2;
    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                            te_01_ve1, te_01_ve2),true);

    // Node 03
    tnit++;
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
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
    tnit++;
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    vector<edge_list_it_t> tc_04_edges_tested;
    tc_04_edges_tested.push_back(e_08_01.backIt());
    vector<node_list_it_t> tc_04_nodes_tested;
    tc_04_nodes_tested.push_back(n_01.backIt());
    tc_04_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_04_vedges;
    EXPECT_EQ(testTreeNode(3, tn_04,tc_04_edges_tested,
                                    tc_04_nodes_tested,tc_04_vedges),true);

    // Edge 02 (02-03)
    teit++;
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_02_ve1;
    edge_list_it_t te_02_ve2;
    EXPECT_EQ(testTreeEdge(te_02, tn_02, tn_03, tc_02_vedges, tc_03_vedges,
                            te_02_ve1, te_02_ve2),true);

    // Edge 03 (03-04)
    teit++;
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_03_ve1;
    edge_list_it_t te_03_ve2;
    EXPECT_EQ(testTreeEdge(te_03, tn_03, tn_04, tc_03_vedges, tc_04_vedges,
                            te_03_ve1, te_03_ve2),true);

    // leave nc_08
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,4);
    EXPECT_EQ(spqrTree.numEdges() ,3);

    // afterVisitingChild(nc_05, ec_05_08)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,5);
    EXPECT_EQ(spqrTree.numEdges() ,4);

    // Node 05
    tnit++;
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    vector<edge_list_it_t> tc_05_edges_tested;
    tc_05_edges_tested.push_back(e_05_08.backIt());
    vector<node_list_it_t> tc_05_nodes_tested;
    tc_05_nodes_tested.push_back(n_01.backIt());
    tc_05_nodes_tested.push_back(n_05.backIt());
    tc_05_nodes_tested.push_back(n_08.backIt());
    vector<edge_list_it_t> tc_05_vedges;
    EXPECT_EQ(testTreeNode(3, tn_05,tc_05_edges_tested,
                                    tc_05_nodes_tested,tc_05_vedges),true);
    // Edge 04 (04-05)
    teit++;
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_04_ve1;
    edge_list_it_t te_04_ve2;
    EXPECT_EQ(testTreeEdge(te_04, tn_04, tn_05, tc_04_vedges, tc_05_vedges,
                            te_04_ve1, te_04_ve2),true);


    // beforeVisitingChild(nc_05, ec_05_06)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_06
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // beforeVisitingChild(nc_06, ec_06_07)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // enter nc_07
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_06_07
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_07, ec_07_04)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_07, ec_07_05)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave nc_07
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_06, ec_06_07)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // procesFrond(nc_06, ec_06_04)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // skip ec_05_06
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave nc_06
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,5);
    EXPECT_EQ(spqrTree.numEdges() ,4);

    // afterVisitingChild(nc_05, ec_05_06)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,7);
    EXPECT_EQ(spqrTree.numEdges() ,5);

    // Node 06
    tnit++;
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
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
    tnit++;
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    vector<edge_list_it_t> tc_07_edges_tested;
    tc_07_edges_tested.push_back(e_04_05.backIt());
    vector<node_list_it_t> tc_07_nodes_tested;
    tc_07_nodes_tested.push_back(n_04.backIt());
    tc_07_nodes_tested.push_back(n_05.backIt());
    vector<edge_list_it_t> tc_07_vedges;
    EXPECT_EQ(testTreeNode(3, tn_07,tc_07_edges_tested,
                                    tc_07_nodes_tested,tc_07_vedges),true);

    // Edge 05 (06-07)
    teit++;
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_05_ve1;
    edge_list_it_t te_05_ve2;
    EXPECT_EQ(testTreeEdge(te_05, tn_06, tn_07, tc_06_vedges, tc_07_vedges,
                            te_05_ve1, te_05_ve2),true);


    // leave nc_05
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,7);
    EXPECT_EQ(spqrTree.numEdges() ,5);

    // afterVisitingChild(nc_04, ec_04_05)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,8);
    EXPECT_EQ(spqrTree.numEdges() ,7);

    // Node 08
    tnit++;
    auto& tn_08 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    vector<edge_list_it_t> tc_08_edges_tested;
    vector<node_list_it_t> tc_08_nodes_tested;
    tc_08_nodes_tested.push_back(n_01.backIt());
    tc_08_nodes_tested.push_back(n_04.backIt());
    tc_08_nodes_tested.push_back(n_05.backIt());
    vector<edge_list_it_t> tc_08_vedges;
    EXPECT_EQ(testTreeNode(3, tn_08,tc_08_edges_tested,
                                    tc_08_nodes_tested,tc_08_vedges),true);

    // Edge 06 and 07 (05-08) or (07-08)
    teit++;
    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_06_ve1;
    edge_list_it_t te_06_ve2;
    teit++;
    auto& te_07 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_07_ve1;
    edge_list_it_t te_07_ve2;


    if(testTreeEdge(te_06, tn_05, tn_08, tc_05_vedges, tc_08_vedges,
                                          te_06_ve1, te_06_ve2)==true){
        EXPECT_EQ(testTreeEdge(te_07, tn_07, tn_08, tc_07_vedges, tc_08_vedges,
                                                   te_07_ve1, te_07_ve2),true);


    }
    else {
        EXPECT_EQ(testTreeEdge(te_06, tn_07, tn_08, tc_07_vedges, tc_08_vedges,
                                          te_06_ve1, te_06_ve2),true);
        EXPECT_EQ(testTreeEdge(te_07, tn_05, tn_08, tc_05_vedges, tc_08_vedges,
                                                   te_07_ve1, te_07_ve2),true);
    }

    // leave nc_04
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,8);
    EXPECT_EQ(spqrTree.numEdges() ,7);

    // afterVisitingChild(nc_03, ec_03_04)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    EXPECT_EQ(spqrTree.numNodes() ,9);
    EXPECT_EQ(spqrTree.numEdges() ,8);

    // Node 09
    tnit++;
    auto& tn_09 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    vector<edge_list_it_t> tc_09_edges_tested;
    tc_09_edges_tested.push_back(e_03_04.backIt());
    vector<node_list_it_t> tc_09_nodes_tested;
    tc_09_nodes_tested.push_back(n_01.backIt());
    tc_09_nodes_tested.push_back(n_03.backIt());
    tc_09_nodes_tested.push_back(n_04.backIt());
    vector<edge_list_it_t> tc_09_vedges;
    EXPECT_EQ(testTreeNode(3, tn_09,tc_09_edges_tested,
                                    tc_09_nodes_tested,tc_09_vedges),true);

    // Edge 08 (08-09)
    teit++;
    auto& te_08 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_08_ve1;
    edge_list_it_t te_08_ve2;
    EXPECT_EQ(testTreeEdge(te_08, tn_08, tn_09, tc_08_vedges, tc_09_vedges,
                            te_08_ve1, te_08_ve2),true);

    // leave nc_03
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_02, ec_02_03)
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // leave nc_02
    EXPECT_EQ(oneStep(mExplorer3),true); 

    // afterVisitingChild(nc_01, ec_01_02)
    EXPECT_EQ(oneStep(mExplorer3),true); 


    EXPECT_EQ(spqrTree.numNodes() ,9);
    EXPECT_EQ(spqrTree.numEdges() ,8);

    // leave nc_01
    EXPECT_EQ(oneStep(mExplorer3),false); 

    EXPECT_EQ(g_2.numNodes(), 0);
    EXPECT_EQ(g_2.numEdges(), 0);

    EXPECT_EQ(spqrTree.numNodes(), 10);
    EXPECT_EQ(spqrTree.numEdges(), 9);

    // Node 10
    tnit++;
    auto& tn_10 = dynamic_cast<SPQRTreeNode&>(*(*tnit));
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

    // Edge 09 (09-10)
    teit++;
    auto& te_09 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_09_ve1;
    edge_list_it_t te_09_ve2;
    EXPECT_EQ(testTreeEdge(te_09, tn_09, tn_10, tc_09_vedges, tc_10_vedges,
                            te_09_ve1, te_09_ve2),true);

}


/**  @brief tests on graph with K4 and Ps.
 *
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
TEST_F(SPQRProc3Tests, Test2) {

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

    SPQRTree     spqrTree(g_2);

    SPQRStrategyProc3 strategy3(g_2, spqrTree);
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer3(g_2,strategy3);

    strategy3.prepareNodesAndEdges(g_2);

    mExplorer3.reset(g_2);
    mExplorer3.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    EXPECT_EQ(spqrTree.numNodes(), 7);
    EXPECT_EQ(spqrTree.numEdges(), 6);

    auto tnit = spqrTree.nodes().first;
    auto teit = spqrTree.edges().first;

    // Node 01
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

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

    // Node 02
    tnit++;
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

    vector<edge_list_it_t> tc_02_edges_tested;
    tc_02_edges_tested.push_back(e_01_02.backIt());
    vector<node_list_it_t> tc_02_nodes_tested;
    tc_02_nodes_tested.push_back(n_01.backIt());
    tc_02_nodes_tested.push_back(n_02.backIt());
    vector<edge_list_it_t> tc_02_vedges;
    EXPECT_EQ(testTreeNode(3, tn_02,tc_02_edges_tested,
                                     tc_02_nodes_tested,tc_02_vedges),true);

    // Edge 01 (01-02)
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_01_ve1;
    edge_list_it_t te_01_ve2;
    EXPECT_EQ(testTreeEdge(te_01, tn_01, tn_02, tc_01_vedges, tc_02_vedges,
                            te_01_ve1, te_01_ve2),true);

    // Node 03
    tnit++;
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

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

    // Node 04
    tnit++;
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

    vector<edge_list_it_t> tc_04_edges_tested;
    vector<node_list_it_t> tc_04_nodes_tested;
    tc_04_nodes_tested.push_back(n_01.backIt());
    tc_04_nodes_tested.push_back(n_02.backIt());
    vector<edge_list_it_t> tc_04_vedges;
    EXPECT_EQ(testTreeNode(3, tn_04,tc_04_edges_tested,
                                     tc_04_nodes_tested,tc_04_vedges),true);

    // Edge 02 (03-04)
    teit++;
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_02_ve1;
    edge_list_it_t te_02_ve2;
    EXPECT_EQ(testTreeEdge(te_02, tn_03, tn_04, tc_03_vedges, tc_04_vedges,
                            te_02_ve1, te_02_ve2),true);

    // Edge 03 (02-04)
    teit++;
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_03_ve1;
    edge_list_it_t te_03_ve2;
    EXPECT_EQ(testTreeEdge(te_03, tn_02, tn_04, tc_02_vedges, tc_04_vedges,
                            te_03_ve1, te_03_ve2),true);

    // Node 05
    tnit++;
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

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
    tnit++;
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

    vector<edge_list_it_t> tc_06_edges_tested;
    vector<node_list_it_t> tc_06_nodes_tested;
    tc_06_nodes_tested.push_back(n_01.backIt());
    tc_06_nodes_tested.push_back(n_02.backIt());
    vector<edge_list_it_t> tc_06_vedges;
    EXPECT_EQ(testTreeNode(3, tn_06,tc_06_edges_tested,
                                     tc_06_nodes_tested,tc_06_vedges),true);

    // Edge 04 (05-06)
    teit++;
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_04_ve1;
    edge_list_it_t te_04_ve2;
    EXPECT_EQ(testTreeEdge(te_04, tn_05, tn_06, tc_05_vedges, tc_06_vedges,
                            te_04_ve1, te_04_ve2),true);

    // Edge 05 (04-06)
    teit++;
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_05_ve1;
    edge_list_it_t te_05_ve2;
    EXPECT_EQ(testTreeEdge(te_05, tn_04, tn_06, tc_04_vedges, tc_06_vedges,
                            te_05_ve1, te_05_ve2),true);

    // Node 07
    tnit++;
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit));

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
    // Edge 06 (06-07)
    teit++;
    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*teit));
    edge_list_it_t te_06_ve1;
    edge_list_it_t te_06_ve2;
    EXPECT_EQ(testTreeEdge(te_06, tn_06, tn_07, tc_06_vedges, tc_07_vedges,
                            te_06_ve1, te_06_ve2),true);

}

} // namespace Undirected

} // namespace Wailea
