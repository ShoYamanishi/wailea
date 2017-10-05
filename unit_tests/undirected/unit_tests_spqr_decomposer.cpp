#include "gtest/gtest.h"
#include "undirected/spqr_decomposer.hpp"
#include "undirected/spqr_tree.hpp"

namespace Wailea {

namespace Undirected { 

using DExplorer = DFSExplorer <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DStrategy = DFSStrategy <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DGraph    = DFSGraph    <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DNode     = DFSNode     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;
using DEdge     = DFSEdge     <Graph,SPQR_DFS_Node,SPQR_DFS_Edge>;

class SPQRDecomposerTests : public ::testing::Test {

  protected:  

    SPQRDecomposerTests(){;};
    virtual ~SPQRDecomposerTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    node_list_it_t copyInputGraph(SPQRDecomposer& decomp,
                            Graph& src, Graph& dst, node_list_it_t startNode){
        return decomp.copyInputGraph(src,dst,startNode);
    }

    void sortAdjacency(SPQRDecomposer& decomp, Graph& g){
        decomp.sortAdjacency(g);
    }

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
                if ( (expected[i] == e.IGBackwardLink()) &&
                    (e.IGBackwardLinkRef().IGForwardLink()==it)           ) {
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


static bool testTreeNode(
    size_t        numEdges,
    SPQRTreeNode& treeNode, 
    vector<edge_list_it_t>& componentEdges,
    vector<node_list_it_t>& componentNodes,
    vector<edge_list_it_t>& virtualEdges,
    enum SPQRTreeNode::type typeExpected){

    if (treeNode.type() == typeExpected) {
        return testTreeNode(numEdges, treeNode, 
                                componentEdges, componentNodes, virtualEdges);
    }
    else{
        return false;
    }
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


class testTreeElem {
  public:
    size_t                 mNodeNumber;
    size_t                 mNumComponentEdges;
    vector<edge_list_it_t> mRealEdges;
    vector<node_list_it_t> mNodes;
    vector<size_t>         mNeighbors;
    enum SPQRTreeNode::type 
                           mType;
    node_list_it_t         mTreeNodeFound;
    vector<edge_list_it_t> mVirtualEdges;
    vector<bool>           mNeighborsChecked;
    bool                   mChecked;

};


class nodeStatus {
  public:
    node_list_it_t mIt;
    size_t         mStatus;
};


class edgeStatus {
  public:
    edge_list_it_t mIt;
    size_t         mStatus;
    edge_list_it_t mVe1;
    edge_list_it_t mVe2;
};


testTreeElem& getElemByIndex(vector<testTreeElem>& elems,size_t i)
{
    for (auto& e: elems){
        if (e.mNodeNumber == i) {
            return e;
        }
    }
    return elems[0];
}


bool IsThereTreeEdgeByNodes(vector<edgeStatus>& treeEdges,
                                           testTreeElem& e1, testTreeElem& e2)
{
    for (auto& e: treeEdges) {
        auto te = e.mIt;
        if( ( (*te)->incidentNode1().backIt()==e1.mTreeNodeFound &&
              (*te)->incidentNode2().backIt()==e2.mTreeNodeFound   ) ||
            ( (*te)->incidentNode1().backIt()==e2.mTreeNodeFound &&
              (*te)->incidentNode2().backIt()==e1.mTreeNodeFound   )   ){
            return true;
        }
    }
    return false;
}


edgeStatus& treeEdgeByNodes(vector<edgeStatus>& treeEdges,
                                           testTreeElem& e1, testTreeElem& e2)
{
    for (auto& e: treeEdges) {
        auto te = e.mIt;
        if( ( (*te)->incidentNode1().backIt()==e1.mTreeNodeFound &&
              (*te)->incidentNode2().backIt()==e2.mTreeNodeFound   ) ||
            ( (*te)->incidentNode1().backIt()==e2.mTreeNodeFound &&
              (*te)->incidentNode2().backIt()==e1.mTreeNodeFound   )   ){
            return e;
        }
    }
    return treeEdges[0];
}



bool testTree(SPQRTree& tree, vector<testTreeElem>& elems)
{
    vector<nodeStatus> treeNodes;
    vector<edgeStatus> treeEdges;
    
    for (auto tnit = tree.nodes().first; tnit != tree.nodes().second; tnit++) {
        nodeStatus ns;
        ns.mIt = tnit;
        ns.mStatus = 0;
        treeNodes.push_back(ns);
    }

    for (auto teit = tree.edges().first; teit != tree.edges().second; teit++) {
        edgeStatus es;
        es.mIt = teit;
        es.mStatus = 0;
        treeEdges.push_back(es);
    }

    for (auto& elem : elems) {
        elem.mChecked = false;
        elem.mVirtualEdges.clear();
        elem.mNeighborsChecked.clear();
        for(size_t i = 0; i < elem.mNeighbors.size(); i++){
            elem.mNeighborsChecked.push_back(false);
        }
        for (auto& ns : treeNodes) {
            auto& tn = dynamic_cast<SPQRTreeNode&>(*(*ns.mIt));
            bool result = testTreeNode(elem.mNumComponentEdges,
                                       tn,
                                       elem.mRealEdges,
                                       elem.mNodes,
                                       elem.mVirtualEdges,
                                       elem.mType);

            if (ns.mStatus > 0 && result) {
                return false;
            }
            else if (result) {
                elem.mTreeNodeFound = ns.mIt;
                ns.mStatus++;
            }
        }
    }


    for (auto& ns : treeNodes) {
        if (ns.mStatus != 1) {
            return false;
        }
    }

    for (auto& elem : elems) {
        for(size_t i = 0; i < elem.mNeighbors.size(); i++) {
            if (elem.mNeighborsChecked[i]==false) {
                testTreeElem& peer = 
                                 getElemByIndex(elems, elem.mNeighbors[i]);
                if(IsThereTreeEdgeByNodes(treeEdges,elem,peer)){
                    edgeStatus& treeEdge =treeEdgeByNodes(
                                                         treeEdges,elem,peer);

                    auto& te = dynamic_cast<SPQRTreeEdge&>(*(*treeEdge.mIt));
                    auto& tn1 = dynamic_cast<SPQRTreeNode&>(
                                                    *(*(elem.mTreeNodeFound)));
                    auto& tn2 = dynamic_cast<SPQRTreeNode&>(
                                                    *(*(peer.mTreeNodeFound)));
                    if(testTreeEdge(te, tn1,                tn2, 
                                        elem.mVirtualEdges, peer.mVirtualEdges,
                                       treeEdge.mVe1,      treeEdge.mVe2    )){
                    
                        for(size_t j = 0; j < peer.mNeighbors.size(); j++){
                            if (peer.mNeighbors[j]==elem.mNodeNumber) {
                                peer.mNeighborsChecked[j]=true;
                                break;
                            }
                        }
                        elem.mNeighborsChecked[i]=true;
                        treeEdge.mStatus++;
                    }
                    else{
                        return false;

                    }
                }
                else{
                    return false;
                }
            }
        }
    }

    for (auto& elem : elems) {
        for(size_t i = 0; i < elem.mNeighbors.size(); i++) {
            if (elem.mNeighborsChecked[i]==false) {
                return false;
            }
        }
    }

    for (auto& es : treeEdges) {
        if (es.mStatus != 1) {
            return false;
        }
    }

    return true;
}


/**  @brief tests copyInputGraph.
 */
TEST_F(SPQRDecomposerTests, Test1) {

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

    Graph g_2;

    SPQRDecomposer decomposer;

    auto nd_start_it = copyInputGraph(decomposer, g_1, g_2, n_01.backIt());

    EXPECT_EQ(g_2.numNodes(), 13);
    EXPECT_EQ(g_2.numEdges(), 23);

    auto nit = g_2.nodes().first;
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_01.IGBackwardLink(), n_01.backIt());
    nit++;
    auto& nc_02 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_02.IGBackwardLink(), n_02.backIt());
    nit++;
    auto& nc_03 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_03.IGBackwardLink(), n_03.backIt());
    nit++;
    auto& nc_04 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_04.IGBackwardLink(), n_04.backIt());
    nit++;
    auto& nc_05 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_05.IGBackwardLink(), n_05.backIt());
    nit++;
    auto& nc_06 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_06.IGBackwardLink(), n_06.backIt());
    nit++;
    auto& nc_07 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_07.IGBackwardLink(), n_07.backIt());
    nit++;
    auto& nc_08 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_08.IGBackwardLink(), n_08.backIt());
    nit++;
    auto& nc_09 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_09.IGBackwardLink(), n_09.backIt());
    nit++;
    auto& nc_10 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_10.IGBackwardLink(), n_10.backIt());
    nit++;
    auto& nc_11 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_11.IGBackwardLink(), n_11.backIt());
    nit++;
    auto& nc_12 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_12.IGBackwardLink(), n_12.backIt());
    nit++;
    auto& nc_13 = dynamic_cast<DNode&>(*(*nit));
    EXPECT_EQ(nc_13.IGBackwardLink(), n_13.backIt());

    auto eit = g_2.edges().first;
    auto& ec_01_02 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_01_02.IGBackwardLink(), e_01_02.backIt());
    eit++;
    auto& ec_02_03 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_02_03.IGBackwardLink(), e_02_03.backIt());
    eit++;
    auto& ec_03_13 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_03_13.IGBackwardLink(), e_03_13.backIt());
    eit++;
    auto& ec_13_01 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_13_01.IGBackwardLink(), e_13_01.backIt());
    eit++;
    auto& ec_13_02 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_13_02.IGBackwardLink(), e_13_02.backIt());
    eit++;
    auto& ec_03_04 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_03_04.IGBackwardLink(), e_03_04.backIt());
    eit++;
    auto& ec_04_05 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_04_05.IGBackwardLink(), e_04_05.backIt());
    eit++;
    auto& ec_05_06 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_05_06.IGBackwardLink(), e_05_06.backIt());
    eit++;
    auto& ec_06_07 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_06_07.IGBackwardLink(), e_06_07.backIt());
    eit++;
    auto& ec_07_04 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_07_04.IGBackwardLink(), e_07_04.backIt());
    eit++;
    auto& ec_07_05 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_07_05.IGBackwardLink(), e_07_05.backIt());
    eit++;
    auto& ec_06_04 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_06_04.IGBackwardLink(), e_06_04.backIt());
    eit++;
    auto& ec_05_08 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_05_08.IGBackwardLink(), e_05_08.backIt());
    eit++;
    auto& ec_08_01 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_08_01.IGBackwardLink(), e_08_01.backIt());
    eit++;
    auto& ec_08_09 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_08_09.IGBackwardLink(), e_08_09.backIt());
    eit++;
    auto& ec_09_10 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_09_10.IGBackwardLink(), e_09_10.backIt());
    eit++;
    auto& ec_10_12 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_10_12.IGBackwardLink(), e_10_12.backIt());
    eit++;
    auto& ec_12_01 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_12_01.IGBackwardLink(), e_12_01.backIt());
    eit++;
    auto& ec_12_08 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_12_08.IGBackwardLink(), e_12_08.backIt());
    eit++;
    auto& ec_12_09 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_12_09.IGBackwardLink(), e_12_09.backIt());
    eit++;
    auto& ec_10_11 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_10_11.IGBackwardLink(), e_10_11.backIt());
    eit++;
    auto& ec_11_08 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_11_08.IGBackwardLink(), e_11_08.backIt());
    eit++;
    auto& ec_11_09 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_11_09.IGBackwardLink(), e_11_09.backIt());

    EXPECT_EQ(nd_start_it, nc_01.backIt());
}


/**  @brief tests on sortAdjacency.
 */
TEST_F(SPQRDecomposerTests, Test2) {

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
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit));

    SPQRStrategyProc1 strategy1;
    DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>mExplorer1(g_2, strategy1);
                                                 
    mExplorer1.reset(g_2);
    mExplorer1.run(dynamic_cast<DFSNode<Graph, SPQR_DFS_Node, SPQR_DFS_Edge>&>
                                                                     (nc_01));
    sortAdjacency(decomposer, g_2);

    auto eit = g_2.edges().first;
    auto& ec_01_02 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_01_02.mPhi, 5);
    eit++;
    auto& ec_02_03 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_02_03.mPhi, 5);
    eit++;
    auto& ec_03_13 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_03_13.mPhi, 3);
    eit++;
    auto& ec_13_01 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_13_01.mPhi, 4);
    eit++;
    auto& ec_13_02 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_13_02.mPhi, 7);
    eit++;
    auto& ec_03_04 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_03_04.mPhi, 5);
    eit++;
    auto& ec_04_05 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_04_05.mPhi, 5);
    eit++;
    auto& ec_05_06 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_05_06.mPhi, 17);
    eit++;
    auto& ec_06_07 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_06_07.mPhi, 15);
    eit++;
    auto& ec_07_04 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_07_04.mPhi, 16);
    eit++;
    auto& ec_07_05 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_07_05.mPhi, 19);
    eit++;
    auto& ec_06_04 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_06_04.mPhi, 16);
    eit++;
    auto& ec_05_08 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_05_08.mPhi, 5);
    eit++;
    auto& ec_08_01 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_08_01.mPhi, 4);
    eit++;
    auto& ec_08_09 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_08_09.mPhi, 5);
    eit++;
    auto& ec_09_10 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_09_10.mPhi, 3);
    eit++;
    auto& ec_10_12 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_10_12.mPhi, 3);
    eit++;
    auto& ec_12_01 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_12_01.mPhi, 4);
    eit++;
    auto& ec_12_08 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_12_08.mPhi, 28);
    eit++;
    auto& ec_12_09 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_12_09.mPhi, 31);
    eit++;
    auto& ec_10_11 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_10_11.mPhi, 27);
    eit++;
    auto& ec_11_08 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_11_08.mPhi, 28);
    eit++;
    auto& ec_11_09 = dynamic_cast<DEdge&>(*(*eit));
    EXPECT_EQ(ec_11_09.mPhi, 31);

    nit++;
    auto& nc_02 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_03 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_04 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_05 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_06 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_07 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_08 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_09 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_10 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_11 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_12 = dynamic_cast<DNode&>(*(*nit));
    nit++;
    auto& nc_13 = dynamic_cast<DNode&>(*(*nit));

    EXPECT_EQ(nc_01.mOrderedAdjacencyType1.size(), 4);
    auto lit = nc_01.mOrderedAdjacencyType1.begin();
    auto& nc_01_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_1, &ec_01_02);
    auto& nc_01_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_2, &ec_13_01);
    auto& nc_01_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_3, &ec_08_01);
    auto& nc_01_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_4, &ec_12_01);

    EXPECT_EQ(nc_02.mOrderedAdjacencyType1.size(), 3);
    lit = nc_02.mOrderedAdjacencyType1.begin();
    auto& nc_02_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_1, &ec_01_02);
    auto& nc_02_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_2, &ec_02_03);
    auto& nc_02_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_3, &ec_13_02);

    EXPECT_EQ(nc_03.mOrderedAdjacencyType1.size(), 3);
    lit = nc_03.mOrderedAdjacencyType1.begin();
    auto& nc_03_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_1, &ec_03_13);
    auto& nc_03_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_2, &ec_02_03);
    auto& nc_03_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_3, &ec_03_04);

    EXPECT_EQ(nc_13.mOrderedAdjacencyType1.size(), 3);
    lit = nc_13.mOrderedAdjacencyType1.begin();
    auto& nc_13_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_1, &ec_03_13);
    auto& nc_13_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_2, &ec_13_01);
    auto& nc_13_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_3, &ec_13_02);

    EXPECT_EQ(nc_04.mOrderedAdjacencyType1.size(), 4);
    lit = nc_04.mOrderedAdjacencyType1.begin();
    auto& nc_04_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_1, &ec_03_04);
    auto& nc_04_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_2, &ec_04_05);
    auto& nc_04_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_3, &ec_07_04);
    auto& nc_04_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_4, &ec_06_04);

    EXPECT_EQ(nc_05.mOrderedAdjacencyType1.size(), 4);
    lit = nc_05.mOrderedAdjacencyType1.begin();
    auto& nc_05_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_1, &ec_04_05);
    auto& nc_05_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_2, &ec_05_08);
    auto& nc_05_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_3, &ec_05_06);
    auto& nc_05_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_4, &ec_07_05);

    EXPECT_EQ(nc_06.mOrderedAdjacencyType1.size(), 3);
    lit = nc_06.mOrderedAdjacencyType1.begin();
    auto& nc_06_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_1, &ec_06_07);
    auto& nc_06_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_2, &ec_05_06);
    auto& nc_06_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_3, &ec_06_04);

    EXPECT_EQ(nc_07.mOrderedAdjacencyType1.size(), 3);
    lit = nc_07.mOrderedAdjacencyType1.begin();
    auto& nc_07_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_1, &ec_06_07);
    auto& nc_07_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_2, &ec_07_04);
    auto& nc_07_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_3, &ec_07_05);

    EXPECT_EQ(nc_08.mOrderedAdjacencyType1.size(), 5);
    lit = nc_08.mOrderedAdjacencyType1.begin();
    auto& nc_08_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_1, &ec_05_08);
    auto& nc_08_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_2, &ec_08_09);
    auto& nc_08_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_3, &ec_08_01);
    auto& nc_08_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_4, &ec_12_08);
    auto& nc_08_5 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_08_5, &ec_11_08);

    EXPECT_EQ(nc_09.mOrderedAdjacencyType1.size(), 4);
    lit = nc_09.mOrderedAdjacencyType1.begin();
    auto& nc_09_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_1, &ec_09_10);
    auto& nc_09_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_2, &ec_08_09);
    auto& nc_09_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_3, &ec_12_09);
    auto& nc_09_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_4, &ec_11_09);

    EXPECT_EQ(nc_10.mOrderedAdjacencyType1.size(), 3);
    lit = nc_10.mOrderedAdjacencyType1.begin();
    auto& nc_10_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_1, &ec_09_10);
    auto& nc_10_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_2, &ec_10_12);
    auto& nc_10_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_3, &ec_10_11);

    EXPECT_EQ(nc_11.mOrderedAdjacencyType1.size(), 3);
    lit = nc_11.mOrderedAdjacencyType1.begin();
    auto& nc_11_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_1, &ec_10_11);
    auto& nc_11_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_2, &ec_11_08);
    auto& nc_11_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_3, &ec_11_09);

    EXPECT_EQ(nc_12.mOrderedAdjacencyType1.size(), 4);
    lit = nc_12.mOrderedAdjacencyType1.begin();
    auto& nc_12_1 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_1, &ec_10_12);
    auto& nc_12_2 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_2, &ec_12_01);
    auto& nc_12_3 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_3, &ec_12_08);
    auto& nc_12_4 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_4, &ec_12_09);

    EXPECT_EQ(nc_01.mOrderedAdjacencyType2.size(), 4);
    lit = nc_01.mOrderedAdjacencyType2.begin();
    auto& nc_01_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_01, &ec_01_02);
    auto& nc_01_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_02, &ec_13_01);
    auto& nc_01_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_03, &ec_08_01);
    auto& nc_01_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_01_04, &ec_12_01);

    EXPECT_EQ(nc_02.mOrderedAdjacencyType2.size(), 3);
    lit = nc_02.mOrderedAdjacencyType2.begin();
    auto& nc_02_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_01, &ec_01_02);
    auto& nc_02_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_02, &ec_02_03);
    auto& nc_02_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_02_03, &ec_13_02);

    EXPECT_EQ(nc_03.mOrderedAdjacencyType2.size(), 3);
    lit = nc_03.mOrderedAdjacencyType2.begin();
    auto& nc_03_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_01, &ec_03_13);
    auto& nc_03_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_02, &ec_02_03);
    auto& nc_03_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_03_03, &ec_03_04);

    EXPECT_EQ(nc_13.mOrderedAdjacencyType2.size(), 3);
    lit = nc_13.mOrderedAdjacencyType2.begin();
    auto& nc_13_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_01, &ec_03_13);
    auto& nc_13_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_02, &ec_13_01);
    auto& nc_13_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_13_03, &ec_13_02);

    EXPECT_EQ(nc_04.mOrderedAdjacencyType2.size(), 4);
    lit = nc_04.mOrderedAdjacencyType2.begin();
    auto& nc_04_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_01, &ec_03_04);
    auto& nc_04_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_02, &ec_04_05);
    auto& nc_04_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_03, &ec_07_04);
    auto& nc_04_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_04_04, &ec_06_04);

    EXPECT_EQ(nc_05.mOrderedAdjacencyType2.size(), 4);
    lit = nc_05.mOrderedAdjacencyType2.begin();
    auto& nc_05_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_01, &ec_04_05);
    auto& nc_05_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_02, &ec_05_08);
    auto& nc_05_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_03, &ec_05_06);
    auto& nc_05_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_05_04, &ec_07_05);

    EXPECT_EQ(nc_06.mOrderedAdjacencyType2.size(), 3);
    lit = nc_06.mOrderedAdjacencyType2.begin();
    auto& nc_06_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_01, &ec_06_07);
    auto& nc_06_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_02, &ec_06_04);
    auto& nc_06_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_06_03, &ec_05_06);

    EXPECT_EQ(nc_07.mOrderedAdjacencyType2.size(), 3);
    lit = nc_07.mOrderedAdjacencyType2.begin();
    auto& nc_07_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_01, &ec_06_07);
    auto& nc_07_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_02, &ec_07_04);
    auto& nc_07_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_07_03, &ec_07_05);

    EXPECT_EQ(nc_08.mOrderedAdjacencyType2.size(), 5);
    lit = nc_08.mOrderedAdjacencyType2.begin();
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

    EXPECT_EQ(nc_09.mOrderedAdjacencyType2.size(), 4);
    lit = nc_09.mOrderedAdjacencyType2.begin();
    auto& nc_09_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_01, &ec_09_10);
    auto& nc_09_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_02, &ec_08_09);
    auto& nc_09_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_03, &ec_12_09);
    auto& nc_09_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_09_04, &ec_11_09);

    EXPECT_EQ(nc_10.mOrderedAdjacencyType2.size(), 3);
    lit = nc_10.mOrderedAdjacencyType2.begin();
    auto& nc_10_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_01, &ec_09_10);
    auto& nc_10_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_02, &ec_10_12);
    auto& nc_10_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_10_03, &ec_10_11);

    EXPECT_EQ(nc_11.mOrderedAdjacencyType2.size(), 3);
    lit = nc_11.mOrderedAdjacencyType2.begin();
    auto& nc_11_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_01, &ec_10_11);
    auto& nc_11_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_02, &ec_11_08);
    auto& nc_11_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_11_03, &ec_11_09);

    EXPECT_EQ(nc_12.mOrderedAdjacencyType2.size(), 4);
    lit = nc_12.mOrderedAdjacencyType2.begin();
    auto& nc_12_01 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_01, &ec_10_12);
    auto& nc_12_02 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_02, &ec_12_01);
    auto& nc_12_03 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_03, &ec_12_08);
    auto& nc_12_04 = dynamic_cast<DEdge&>(*(*(*lit++)));
    EXPECT_EQ(&nc_12_04, &ec_12_09);

}


/**  @brief tests on createNodeMap
 */
TEST_F(SPQRDecomposerTests, Test3) {

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
    auto& nc_01 = dynamic_cast<DNode&>(*(*nit));

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

}


/**  @brief tests on decompose().
 */
TEST_F(SPQRDecomposerTests, Test4) {

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

    SPQRDecomposer decomposer;
    SPQRTree spqrTree = decomposer.decompose(g_1,n_01);

    EXPECT_EQ(spqrTree.numNodes(),8);
    EXPECT_EQ(spqrTree.numEdges(),7);

    auto tnit = spqrTree.nodes().first;
    auto& tn_01 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_02 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_03 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_04 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_05 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_06 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_07 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));
    auto& tn_10 = dynamic_cast<SPQRTreeNode&>(*(*tnit++));

    auto teit = spqrTree.edges().first;
    auto& te_01 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_02 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_03 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_04 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_05 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_06 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));
    auto& te_09 = dynamic_cast<SPQRTreeEdge&>(*(*teit++));

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

    EXPECT_EQ(e_01_02.utilitySize(), 1);
    EXPECT_EQ(e_02_03.utilitySize(), 1);
    EXPECT_EQ(e_03_13.utilitySize(), 1);
    EXPECT_EQ(e_13_01.utilitySize(), 1);
    EXPECT_EQ(e_13_02.utilitySize(), 1);
    EXPECT_EQ(e_03_04.utilitySize(), 1);
    EXPECT_EQ(e_04_05.utilitySize(), 1);
    EXPECT_EQ(e_05_06.utilitySize(), 1);
    EXPECT_EQ(e_06_07.utilitySize(), 1);
    EXPECT_EQ(e_07_04.utilitySize(), 1);
    EXPECT_EQ(e_07_05.utilitySize(), 1);
    EXPECT_EQ(e_06_04.utilitySize(), 1);
    EXPECT_EQ(e_05_08.utilitySize(), 1);
    EXPECT_EQ(e_08_01.utilitySize(), 1);
    EXPECT_EQ(e_08_09.utilitySize(), 1);
    EXPECT_EQ(e_09_10.utilitySize(), 1);
    EXPECT_EQ(e_10_12.utilitySize(), 1);
    EXPECT_EQ(e_12_01.utilitySize(), 1);
    EXPECT_EQ(e_12_08.utilitySize(), 1);
    EXPECT_EQ(e_12_09.utilitySize(), 1);
    EXPECT_EQ(e_10_11.utilitySize(), 1);
    EXPECT_EQ(e_11_08.utilitySize(), 1);
    EXPECT_EQ(e_11_09.utilitySize(), 1);

    EXPECT_EQ(e_01_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_02_03.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_03_13.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_13_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_13_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_03_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_04_05.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
    EXPECT_EQ(e_05_06.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_06_07.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_07_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_07_05.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_06_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_05_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_08_01.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
    EXPECT_EQ(e_08_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_09_10.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_10_12.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_12_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_12_08.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
    EXPECT_EQ(e_12_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_10_11.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_11_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_11_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);


    EXPECT_EQ(e_01_02.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_02_03.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_03_13.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_13_01.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_13_02.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_03_04.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_04_05.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_05_06.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_06_07.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_07_04.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_07_05.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_06_04.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_05_08.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_08_01.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_08_09.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_09_10.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_10_12.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_12_01.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_12_08.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_12_09.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_10_11.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_11_08.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_11_09.IGForwardLinksSize(), 1);

    EXPECT_EQ(e_01_02.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_02_03.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_03_13.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_13_01.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_13_02.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_03_04.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_04_05.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_05_06.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_06_07.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_07_04.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_07_05.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_06_04.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_05_08.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_08_01.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_08_09.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_09_10.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_10_12.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_12_01.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_12_08.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_12_09.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_10_11.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_11_08.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_11_09.IGBackwardLinksSize(),0);

}


/**  @brief tests on decompose() with n_01 as the start.
 */
TEST_F(SPQRDecomposerTests, Test5) {

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

    SPQRDecomposer decomposer;
    SPQRTree spqrTree = decomposer.decompose(g_1,n_01);

    testTreeElem tte_01;
    tte_01.mNodeNumber = 1;
    tte_01.mNumComponentEdges = 8;
    tte_01.mRealEdges.push_back(e_12_09.backIt());
    tte_01.mRealEdges.push_back(e_10_12.backIt());
    tte_01.mRealEdges.push_back(e_11_08.backIt());
    tte_01.mRealEdges.push_back(e_11_09.backIt());
    tte_01.mRealEdges.push_back(e_10_11.backIt());
    tte_01.mRealEdges.push_back(e_09_10.backIt());
    tte_01.mRealEdges.push_back(e_08_09.backIt());
    tte_01.mNodes.push_back(n_12.backIt());
    tte_01.mNodes.push_back(n_08.backIt());
    tte_01.mNodes.push_back(n_09.backIt());
    tte_01.mNodes.push_back(n_10.backIt());
    tte_01.mNodes.push_back(n_11.backIt());
    tte_01.mNeighbors.push_back(2);
    tte_01.mType = SPQRTreeNode::RType;

    testTreeElem tte_02;
    tte_02.mNodeNumber = 2;
    tte_02.mNumComponentEdges = 3;
    tte_02.mRealEdges.push_back(e_12_08.backIt());
    tte_02.mNodes.push_back(n_12.backIt());
    tte_02.mNodes.push_back(n_08.backIt());
    tte_02.mNeighbors.push_back(1);
    tte_02.mNeighbors.push_back(3);
    tte_02.mType = SPQRTreeNode::PType;

    testTreeElem tte_03;
    tte_03.mNodeNumber = 3;
    tte_03.mNumComponentEdges = 3;
    tte_03.mRealEdges.push_back(e_12_01.backIt());
    tte_03.mNodes.push_back(n_01.backIt());
    tte_03.mNodes.push_back(n_08.backIt());
    tte_03.mNodes.push_back(n_12.backIt());
    tte_03.mNeighbors.push_back(2);
    tte_03.mNeighbors.push_back(4);
    tte_03.mType = SPQRTreeNode::SType;

    testTreeElem tte_04;
    tte_04.mNodeNumber = 4;
    tte_04.mNumComponentEdges = 3;
    tte_04.mRealEdges.push_back(e_08_01.backIt());
    tte_04.mNodes.push_back(n_01.backIt());
    tte_04.mNodes.push_back(n_08.backIt());
    tte_04.mNeighbors.push_back(3);
    tte_04.mNeighbors.push_back(5);
    tte_04.mType = SPQRTreeNode::PType;

    testTreeElem tte_05;
    tte_05.mNodeNumber = 5;
    tte_05.mNumComponentEdges = 5;
    tte_05.mRealEdges.push_back(e_05_08.backIt());
    tte_05.mRealEdges.push_back(e_03_04.backIt());
    tte_05.mNodes.push_back(n_01.backIt());
    tte_05.mNodes.push_back(n_03.backIt());
    tte_05.mNodes.push_back(n_04.backIt());
    tte_05.mNodes.push_back(n_05.backIt());
    tte_05.mNodes.push_back(n_08.backIt());
    tte_05.mNeighbors.push_back(4);
    tte_05.mNeighbors.push_back(7);
    tte_05.mNeighbors.push_back(8);
    tte_05.mType = SPQRTreeNode::SType;

    testTreeElem tte_06;
    tte_06.mNodeNumber = 6;
    tte_06.mNumComponentEdges = 6;
    tte_06.mRealEdges.push_back(e_05_06.backIt());
    tte_06.mRealEdges.push_back(e_06_07.backIt());
    tte_06.mRealEdges.push_back(e_06_04.backIt());
    tte_06.mRealEdges.push_back(e_07_04.backIt());
    tte_06.mRealEdges.push_back(e_07_05.backIt());
    tte_06.mNodes.push_back(n_04.backIt());
    tte_06.mNodes.push_back(n_05.backIt());
    tte_06.mNodes.push_back(n_06.backIt());
    tte_06.mNodes.push_back(n_07.backIt());
    tte_06.mNeighbors.push_back(7);
    tte_06.mType = SPQRTreeNode::RType;

    testTreeElem tte_07;
    tte_07.mNodeNumber = 7;
    tte_07.mNumComponentEdges = 3;
    tte_07.mRealEdges.push_back(e_04_05.backIt());
    tte_07.mNodes.push_back(n_04.backIt());
    tte_07.mNodes.push_back(n_05.backIt());
    tte_07.mNeighbors.push_back(5);
    tte_07.mNeighbors.push_back(6);
    tte_07.mType = SPQRTreeNode::PType;

    testTreeElem tte_08;
    tte_08.mNodeNumber = 8;
    tte_08.mNumComponentEdges = 6;
    tte_08.mRealEdges.push_back(e_01_02.backIt());
    tte_08.mRealEdges.push_back(e_02_03.backIt());
    tte_08.mRealEdges.push_back(e_03_13.backIt());
    tte_08.mRealEdges.push_back(e_13_01.backIt());
    tte_08.mRealEdges.push_back(e_13_02.backIt());
    tte_08.mNodes.push_back(n_01.backIt());
    tte_08.mNodes.push_back(n_02.backIt());
    tte_08.mNodes.push_back(n_03.backIt());
    tte_08.mNodes.push_back(n_13.backIt());
    tte_08.mNeighbors.push_back(5);
    tte_08.mType = SPQRTreeNode::RType;

    vector<testTreeElem> elems;
    elems.push_back(tte_01);
    elems.push_back(tte_02);
    elems.push_back(tte_03);
    elems.push_back(tte_04);
    elems.push_back(tte_05);
    elems.push_back(tte_06);
    elems.push_back(tte_07);
    elems.push_back(tte_08);

    EXPECT_EQ(testTree(spqrTree, elems),true);

    EXPECT_EQ(e_01_02.utilitySize(), 1);
    EXPECT_EQ(e_02_03.utilitySize(), 1);
    EXPECT_EQ(e_03_13.utilitySize(), 1);
    EXPECT_EQ(e_13_01.utilitySize(), 1);
    EXPECT_EQ(e_13_02.utilitySize(), 1);
    EXPECT_EQ(e_03_04.utilitySize(), 1);
    EXPECT_EQ(e_04_05.utilitySize(), 1);
    EXPECT_EQ(e_05_06.utilitySize(), 1);
    EXPECT_EQ(e_06_07.utilitySize(), 1);
    EXPECT_EQ(e_07_04.utilitySize(), 1);
    EXPECT_EQ(e_07_05.utilitySize(), 1);
    EXPECT_EQ(e_06_04.utilitySize(), 1);
    EXPECT_EQ(e_05_08.utilitySize(), 1);
    EXPECT_EQ(e_08_01.utilitySize(), 1);
    EXPECT_EQ(e_08_09.utilitySize(), 1);
    EXPECT_EQ(e_09_10.utilitySize(), 1);
    EXPECT_EQ(e_10_12.utilitySize(), 1);
    EXPECT_EQ(e_12_01.utilitySize(), 1);
    EXPECT_EQ(e_12_08.utilitySize(), 1);
    EXPECT_EQ(e_12_09.utilitySize(), 1);
    EXPECT_EQ(e_10_11.utilitySize(), 1);
    EXPECT_EQ(e_11_08.utilitySize(), 1);
    EXPECT_EQ(e_11_09.utilitySize(), 1);

    EXPECT_EQ(e_01_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_02_03.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_03_13.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_13_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_13_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_03_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_04_05.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
    EXPECT_EQ(e_05_06.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_06_07.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_07_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_07_05.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_06_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_05_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_08_01.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
    EXPECT_EQ(e_08_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_09_10.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_10_12.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_12_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_12_08.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
    EXPECT_EQ(e_12_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_10_11.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_11_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
    EXPECT_EQ(e_11_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);


    EXPECT_EQ(e_01_02.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_02_03.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_03_13.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_13_01.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_13_02.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_03_04.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_04_05.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_05_06.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_06_07.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_07_04.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_07_05.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_06_04.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_05_08.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_08_01.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_08_09.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_09_10.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_10_12.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_12_01.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_12_08.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_12_09.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_10_11.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_11_08.IGForwardLinksSize(), 1);
    EXPECT_EQ(e_11_09.IGForwardLinksSize(), 1);

    EXPECT_EQ(e_01_02.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_02_03.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_03_13.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_13_01.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_13_02.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_03_04.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_04_05.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_05_06.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_06_07.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_07_04.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_07_05.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_06_04.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_05_08.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_08_01.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_08_09.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_09_10.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_10_12.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_12_01.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_12_08.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_12_09.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_10_11.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_11_08.IGBackwardLinksSize(),0);
    EXPECT_EQ(e_11_09.IGBackwardLinksSize(),0);

}


/**  @brief tests on decompose() with multiple starting points
 */
TEST_F(SPQRDecomposerTests, Test6) {

    for (size_t i = 0 ; i < 13; i++) {
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
 
        SPQRDecomposer decomposer;
        node_list_it_t nStartIt;
        switch (i) {
          case 0:
            nStartIt = n_01.backIt();
            break;
          case 1:
            nStartIt = n_02.backIt();
            break;
          case 2:
            nStartIt = n_03.backIt();
            break;
          case 3:
            nStartIt = n_04.backIt();
            break;
          case 4:
            nStartIt = n_05.backIt();
            break;
          case 5:
            nStartIt = n_06.backIt();
            break;
          case 6:
            nStartIt = n_07.backIt();
            break;
          case 7:
            nStartIt = n_08.backIt();
            break;
          case 8:
            nStartIt = n_09.backIt();
            break;
          case 9:
            nStartIt = n_10.backIt();
            break;
          case 10:
            nStartIt = n_11.backIt();
            break;
          case 11:
            nStartIt = n_12.backIt();
            break;
          case 12:
          default:
            nStartIt = n_13.backIt();
        }

        auto& nStart = dynamic_cast<Node&>(*(*nStartIt));

        SPQRTree spqrTree = decomposer.decompose(g_1,nStart);

        testTreeElem tte_01;
        tte_01.mNodeNumber = 1;
        tte_01.mNumComponentEdges = 8;
        tte_01.mRealEdges.push_back(e_12_09.backIt());
        tte_01.mRealEdges.push_back(e_10_12.backIt());
        tte_01.mRealEdges.push_back(e_11_08.backIt());
        tte_01.mRealEdges.push_back(e_11_09.backIt());
        tte_01.mRealEdges.push_back(e_10_11.backIt());
        tte_01.mRealEdges.push_back(e_09_10.backIt());
        tte_01.mRealEdges.push_back(e_08_09.backIt());
        tte_01.mNodes.push_back(n_12.backIt());
        tte_01.mNodes.push_back(n_08.backIt());
        tte_01.mNodes.push_back(n_09.backIt());
        tte_01.mNodes.push_back(n_10.backIt());
        tte_01.mNodes.push_back(n_11.backIt());
        tte_01.mNeighbors.push_back(2);
        tte_01.mType = SPQRTreeNode::RType;

        testTreeElem tte_02;
        tte_02.mNodeNumber = 2;
        tte_02.mNumComponentEdges = 3;
        tte_02.mRealEdges.push_back(e_12_08.backIt());
        tte_02.mNodes.push_back(n_12.backIt());
        tte_02.mNodes.push_back(n_08.backIt());
        tte_02.mNeighbors.push_back(1);
        tte_02.mNeighbors.push_back(3);
        tte_02.mType = SPQRTreeNode::PType;

        testTreeElem tte_03;
        tte_03.mNodeNumber = 3;
        tte_03.mNumComponentEdges = 3;
        tte_03.mRealEdges.push_back(e_12_01.backIt());
        tte_03.mNodes.push_back(n_01.backIt());
        tte_03.mNodes.push_back(n_08.backIt());
        tte_03.mNodes.push_back(n_12.backIt());
        tte_03.mNeighbors.push_back(2);
        tte_03.mNeighbors.push_back(4);
        tte_03.mType = SPQRTreeNode::SType;

        testTreeElem tte_04;
        tte_04.mNodeNumber = 4;
        tte_04.mNumComponentEdges = 3;
        tte_04.mRealEdges.push_back(e_08_01.backIt());
        tte_04.mNodes.push_back(n_01.backIt());
        tte_04.mNodes.push_back(n_08.backIt());
        tte_04.mNeighbors.push_back(3);
        tte_04.mNeighbors.push_back(5);
        tte_04.mType = SPQRTreeNode::PType;

        testTreeElem tte_05;
        tte_05.mNodeNumber = 5;
        tte_05.mNumComponentEdges = 5;
        tte_05.mRealEdges.push_back(e_05_08.backIt());
        tte_05.mRealEdges.push_back(e_03_04.backIt());
        tte_05.mNodes.push_back(n_01.backIt());
        tte_05.mNodes.push_back(n_03.backIt());
        tte_05.mNodes.push_back(n_04.backIt());
        tte_05.mNodes.push_back(n_05.backIt());
        tte_05.mNodes.push_back(n_08.backIt());
        tte_05.mNeighbors.push_back(4);
        tte_05.mNeighbors.push_back(7);
        tte_05.mNeighbors.push_back(8);
        tte_05.mType = SPQRTreeNode::SType;

        testTreeElem tte_06;
        tte_06.mNodeNumber = 6;
        tte_06.mNumComponentEdges = 6;
        tte_06.mRealEdges.push_back(e_05_06.backIt());
        tte_06.mRealEdges.push_back(e_06_07.backIt());
        tte_06.mRealEdges.push_back(e_06_04.backIt());
        tte_06.mRealEdges.push_back(e_07_04.backIt());
        tte_06.mRealEdges.push_back(e_07_05.backIt());
        tte_06.mNodes.push_back(n_04.backIt());
        tte_06.mNodes.push_back(n_05.backIt());
        tte_06.mNodes.push_back(n_06.backIt());
        tte_06.mNodes.push_back(n_07.backIt());
        tte_06.mNeighbors.push_back(7);
        tte_06.mType = SPQRTreeNode::RType;

        testTreeElem tte_07;
        tte_07.mNodeNumber = 7;
        tte_07.mNumComponentEdges = 3;
        tte_07.mRealEdges.push_back(e_04_05.backIt());
        tte_07.mNodes.push_back(n_04.backIt());
        tte_07.mNodes.push_back(n_05.backIt());
        tte_07.mNeighbors.push_back(5);
        tte_07.mNeighbors.push_back(6);
        tte_07.mType = SPQRTreeNode::PType;

        testTreeElem tte_08;
        tte_08.mNodeNumber = 8;
        tte_08.mNumComponentEdges = 6;
        tte_08.mRealEdges.push_back(e_01_02.backIt());
        tte_08.mRealEdges.push_back(e_02_03.backIt());
        tte_08.mRealEdges.push_back(e_03_13.backIt());
        tte_08.mRealEdges.push_back(e_13_01.backIt());
        tte_08.mRealEdges.push_back(e_13_02.backIt());
        tte_08.mNodes.push_back(n_01.backIt());
        tte_08.mNodes.push_back(n_02.backIt());
        tte_08.mNodes.push_back(n_03.backIt());
        tte_08.mNodes.push_back(n_13.backIt());
        tte_08.mNeighbors.push_back(5);
        tte_08.mType = SPQRTreeNode::RType;

        vector<testTreeElem> elems;
        elems.push_back(tte_01);
        elems.push_back(tte_02);
        elems.push_back(tte_03);
        elems.push_back(tte_04);
        elems.push_back(tte_05);
        elems.push_back(tte_06);
        elems.push_back(tte_07);
        elems.push_back(tte_08);

        EXPECT_EQ(testTree(spqrTree, elems),true);

        EXPECT_EQ(e_01_02.utilitySize(), 1);
        EXPECT_EQ(e_02_03.utilitySize(), 1);
        EXPECT_EQ(e_03_13.utilitySize(), 1);
        EXPECT_EQ(e_13_01.utilitySize(), 1);
        EXPECT_EQ(e_13_02.utilitySize(), 1);
        EXPECT_EQ(e_03_04.utilitySize(), 1);
        EXPECT_EQ(e_04_05.utilitySize(), 1);
        EXPECT_EQ(e_05_06.utilitySize(), 1);
        EXPECT_EQ(e_06_07.utilitySize(), 1);
        EXPECT_EQ(e_07_04.utilitySize(), 1);
        EXPECT_EQ(e_07_05.utilitySize(), 1);
        EXPECT_EQ(e_06_04.utilitySize(), 1);
        EXPECT_EQ(e_05_08.utilitySize(), 1);
        EXPECT_EQ(e_08_01.utilitySize(), 1);
        EXPECT_EQ(e_08_09.utilitySize(), 1);
        EXPECT_EQ(e_09_10.utilitySize(), 1);
        EXPECT_EQ(e_10_12.utilitySize(), 1);
        EXPECT_EQ(e_12_01.utilitySize(), 1);
        EXPECT_EQ(e_12_08.utilitySize(), 1);
        EXPECT_EQ(e_12_09.utilitySize(), 1);
        EXPECT_EQ(e_10_11.utilitySize(), 1);
        EXPECT_EQ(e_11_08.utilitySize(), 1);
        EXPECT_EQ(e_11_09.utilitySize(), 1);

        EXPECT_EQ(e_01_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_02_03.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_03_13.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_13_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_13_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_03_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_04_05.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_05_06.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_06_07.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_07_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_07_05.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_06_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_05_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_08_01.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_08_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_09_10.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_12.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_12_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_12_08.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_12_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_11.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_11_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_11_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);


        EXPECT_EQ(e_01_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_03.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_03_13.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_13_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_13_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_03_04.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_04_05.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_05_06.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_06_07.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_07_04.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_07_05.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_06_04.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_05_08.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_08_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_08_09.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_09_10.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_12.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_12_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_12_08.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_12_09.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_11.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_11_08.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_11_09.IGForwardLinksSize(), 1);

        EXPECT_EQ(e_01_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_03.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_03_13.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_13_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_13_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_03_04.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_04_05.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_05_06.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_06_07.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_07_04.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_07_05.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_06_04.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_05_08.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_08_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_08_09.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_09_10.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_12.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_12_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_12_08.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_12_09.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_11.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_11_08.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_11_09.IGBackwardLinksSize(),0);
    }
}



/**  @brief tests on decompose() with multiple starting points
 *          graph taken from https://en.wikipedia.org/wiki/Tree_decomposition
 */
TEST_F(SPQRDecomposerTests, Test7) {

    for (size_t i = 0 ; i < 8; i++) {
        Graph g_1;
        auto& n_A = g_1.addNode(make_unique<Node>());
        auto& n_B = g_1.addNode(make_unique<Node>());
        auto& n_C = g_1.addNode(make_unique<Node>());
        auto& n_D = g_1.addNode(make_unique<Node>());
        auto& n_E = g_1.addNode(make_unique<Node>());
        auto& n_F = g_1.addNode(make_unique<Node>());
        auto& n_G = g_1.addNode(make_unique<Node>());
        auto& n_H = g_1.addNode(make_unique<Node>());

        auto& e_A_B = g_1.addEdge(make_unique<Edge>(),n_A, n_B);
        auto& e_A_C = g_1.addEdge(make_unique<Edge>(),n_A, n_C);
        auto& e_B_C = g_1.addEdge(make_unique<Edge>(),n_B, n_C);
        auto& e_B_E = g_1.addEdge(make_unique<Edge>(),n_B, n_E);
        auto& e_B_F = g_1.addEdge(make_unique<Edge>(),n_B, n_F);
        auto& e_B_G = g_1.addEdge(make_unique<Edge>(),n_B, n_G);
        auto& e_C_D = g_1.addEdge(make_unique<Edge>(),n_C, n_D);
        auto& e_C_E = g_1.addEdge(make_unique<Edge>(),n_C, n_E);
        auto& e_D_E = g_1.addEdge(make_unique<Edge>(),n_D, n_E);
        auto& e_E_G = g_1.addEdge(make_unique<Edge>(),n_E, n_G);
        auto& e_E_H = g_1.addEdge(make_unique<Edge>(),n_E, n_H);
        auto& e_F_G = g_1.addEdge(make_unique<Edge>(),n_F, n_G);
        auto& e_G_H = g_1.addEdge(make_unique<Edge>(),n_G, n_H);

        SPQRDecomposer decomposer;
        node_list_it_t nStartIt;
        switch (i) {
          case 0:
            nStartIt = n_A.backIt();
            break;
          case 1:
            nStartIt = n_B.backIt();
            break;
          case 2:
            nStartIt = n_C.backIt();
            break;
          case 3:
            nStartIt = n_D.backIt();
            break;
          case 4:
            nStartIt = n_E.backIt();
            break;
          case 5:
            nStartIt = n_F.backIt();
            break;
          case 6:
            nStartIt = n_G.backIt();
            break;
          case 7:
          default:
            nStartIt = n_H.backIt();
        }

        auto& nStart = dynamic_cast<Node&>(*(*nStartIt));

        SPQRTree spqrTree = decomposer.decompose(g_1,nStart);


        testTreeElem tte_01;
        tte_01.mNodeNumber = 1;
        tte_01.mNumComponentEdges = 3;
        tte_01.mRealEdges.push_back(e_A_B.backIt());
        tte_01.mRealEdges.push_back(e_A_C.backIt());
        tte_01.mNodes.push_back(n_A.backIt());
        tte_01.mNodes.push_back(n_B.backIt());
        tte_01.mNodes.push_back(n_C.backIt());
        tte_01.mNeighbors.push_back(2);
        tte_01.mType = SPQRTreeNode::SType;

        testTreeElem tte_02;
        tte_02.mNodeNumber = 2;
        tte_02.mNumComponentEdges = 3;
        tte_02.mRealEdges.push_back(e_B_C.backIt());
        tte_02.mNodes.push_back(n_B.backIt());
        tte_02.mNodes.push_back(n_C.backIt());
        tte_02.mNeighbors.push_back(1);
        tte_02.mNeighbors.push_back(5);
        tte_02.mType = SPQRTreeNode::PType;

        testTreeElem tte_03;
        tte_03.mNodeNumber = 3;
        tte_03.mNumComponentEdges = 3;
        tte_03.mRealEdges.push_back(e_C_D.backIt());
        tte_03.mRealEdges.push_back(e_D_E.backIt());
        tte_03.mNodes.push_back(n_C.backIt());
        tte_03.mNodes.push_back(n_D.backIt());
        tte_03.mNodes.push_back(n_E.backIt());
        tte_03.mNeighbors.push_back(4);
        tte_03.mType = SPQRTreeNode::SType;

        testTreeElem tte_04;
        tte_04.mNodeNumber = 4;
        tte_04.mNumComponentEdges = 3;
        tte_04.mRealEdges.push_back(e_C_E.backIt());
        tte_04.mNodes.push_back(n_C.backIt());
        tte_04.mNodes.push_back(n_E.backIt());
        tte_04.mNeighbors.push_back(3);
        tte_04.mNeighbors.push_back(5);
        tte_04.mType = SPQRTreeNode::PType;

        testTreeElem tte_05;
        tte_05.mNodeNumber = 5;
        tte_05.mNumComponentEdges = 3;
        tte_05.mNodes.push_back(n_B.backIt());
        tte_05.mNodes.push_back(n_C.backIt());
        tte_05.mNodes.push_back(n_E.backIt());
        tte_05.mNeighbors.push_back(2);
        tte_05.mNeighbors.push_back(4);
        tte_05.mNeighbors.push_back(6);
        tte_05.mType = SPQRTreeNode::SType;

        testTreeElem tte_06;
        tte_06.mNodeNumber = 6;
        tte_06.mNumComponentEdges = 3;
        tte_06.mRealEdges.push_back(e_B_E.backIt());
        tte_06.mNodes.push_back(n_B.backIt());
        tte_06.mNodes.push_back(n_E.backIt());
        tte_06.mNeighbors.push_back(5);
        tte_06.mNeighbors.push_back(7);
        tte_06.mType = SPQRTreeNode::PType;

        testTreeElem tte_07;
        tte_07.mNodeNumber = 7;
        tte_07.mNumComponentEdges = 3;
        tte_07.mNodes.push_back(n_B.backIt());
        tte_07.mNodes.push_back(n_E.backIt());
        tte_07.mNodes.push_back(n_G.backIt());
        tte_07.mNeighbors.push_back(6);
        tte_07.mNeighbors.push_back(8);
        tte_07.mNeighbors.push_back(10);
        tte_07.mType = SPQRTreeNode::SType;

        testTreeElem tte_08;
        tte_08.mNodeNumber = 8;
        tte_08.mNumComponentEdges = 3;
        tte_08.mRealEdges.push_back(e_B_G.backIt());
        tte_08.mNodes.push_back(n_B.backIt());
        tte_08.mNodes.push_back(n_G.backIt());
        tte_08.mNeighbors.push_back(7);
        tte_08.mNeighbors.push_back(9);
        tte_08.mType = SPQRTreeNode::PType;

        testTreeElem tte_09;
        tte_09.mNodeNumber = 9;
        tte_09.mNumComponentEdges = 3;
        tte_09.mRealEdges.push_back(e_B_F.backIt());
        tte_09.mRealEdges.push_back(e_F_G.backIt());
        tte_09.mNodes.push_back(n_B.backIt());
        tte_09.mNodes.push_back(n_F.backIt());
        tte_09.mNodes.push_back(n_G.backIt());
        tte_09.mNeighbors.push_back(8);
        tte_09.mType = SPQRTreeNode::SType;

        testTreeElem tte_10;
        tte_10.mNodeNumber = 10;
        tte_10.mNumComponentEdges = 3;
        tte_10.mRealEdges.push_back(e_E_G.backIt());
        tte_10.mNodes.push_back(n_E.backIt());
        tte_10.mNodes.push_back(n_G.backIt());
        tte_10.mNeighbors.push_back(7);
        tte_10.mNeighbors.push_back(11);
        tte_10.mType = SPQRTreeNode::PType;

        testTreeElem tte_11;
        tte_11.mNodeNumber = 11;
        tte_11.mNumComponentEdges = 3;
        tte_11.mRealEdges.push_back(e_E_H.backIt());
        tte_11.mRealEdges.push_back(e_G_H.backIt());
        tte_11.mNodes.push_back(n_E.backIt());
        tte_11.mNodes.push_back(n_G.backIt());
        tte_11.mNodes.push_back(n_H.backIt());
        tte_11.mNeighbors.push_back(10);
        tte_11.mType = SPQRTreeNode::SType;

        vector<testTreeElem> elems;
        elems.push_back(tte_01);
        elems.push_back(tte_02);
        elems.push_back(tte_03);
        elems.push_back(tte_04);
        elems.push_back(tte_05);
        elems.push_back(tte_06);
        elems.push_back(tte_07);
        elems.push_back(tte_08);
        elems.push_back(tte_09);
        elems.push_back(tte_10);
        elems.push_back(tte_11);

        EXPECT_EQ(testTree(spqrTree, elems),true);

        EXPECT_EQ(e_A_B.utilitySize(), 1);
        EXPECT_EQ(e_A_C.utilitySize(), 1);
        EXPECT_EQ(e_B_C.utilitySize(), 1);
        EXPECT_EQ(e_B_E.utilitySize(), 1);
        EXPECT_EQ(e_B_F.utilitySize(), 1);
        EXPECT_EQ(e_B_G.utilitySize(), 1);
        EXPECT_EQ(e_C_D.utilitySize(), 1);
        EXPECT_EQ(e_C_E.utilitySize(), 1);
        EXPECT_EQ(e_D_E.utilitySize(), 1);
        EXPECT_EQ(e_E_G.utilitySize(), 1);
        EXPECT_EQ(e_E_H.utilitySize(), 1);
        EXPECT_EQ(e_F_G.utilitySize(), 1);
        EXPECT_EQ(e_G_H.utilitySize(), 1);

        EXPECT_EQ(e_A_B.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_A_C.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_B_C.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_B_E.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_B_F.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_B_G.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_C_D.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_C_E.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_D_E.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_E_G.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_E_H.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_F_G.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_G_H.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);

        EXPECT_EQ(e_A_B.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_A_C.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_B_C.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_B_E.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_B_F.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_B_G.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_C_D.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_C_E.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_D_E.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_E_G.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_E_H.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_F_G.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_G_H.IGForwardLinksSize(), 1);

        EXPECT_EQ(e_A_B.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_A_C.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_B_C.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_B_E.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_B_F.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_B_G.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_C_D.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_C_E.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_D_E.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_E_G.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_E_H.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_F_G.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_G_H.IGBackwardLinksSize(),0);

    }
}


/**  @brief tests on decompose() with multiple starting points
 *          graph with consecutive 3-bonds merged into 1 P type.
 */
TEST_F(SPQRDecomposerTests, Test8) {

    for (size_t i = 0 ; i < 10; i++) {
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

        auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01, n_02);
        auto& e_02_03 = g_1.addEdge(make_unique<Edge>(),n_02, n_03);
        auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03, n_04);
        auto& e_04_01 = g_1.addEdge(make_unique<Edge>(),n_04, n_01);
        auto& e_04_02 = g_1.addEdge(make_unique<Edge>(),n_04, n_02);
        auto& e_03_01 = g_1.addEdge(make_unique<Edge>(),n_03, n_01);
        auto& e_02_05 = g_1.addEdge(make_unique<Edge>(),n_02, n_05);
        auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05, n_06);
        auto& e_06_01 = g_1.addEdge(make_unique<Edge>(),n_06, n_01);
        auto& e_06_02 = g_1.addEdge(make_unique<Edge>(),n_06, n_02);
        auto& e_05_01 = g_1.addEdge(make_unique<Edge>(),n_05, n_01);
        auto& e_02_07 = g_1.addEdge(make_unique<Edge>(),n_02, n_07);
        auto& e_07_08 = g_1.addEdge(make_unique<Edge>(),n_07, n_08);
        auto& e_08_01 = g_1.addEdge(make_unique<Edge>(),n_08, n_01);
        auto& e_08_02 = g_1.addEdge(make_unique<Edge>(),n_08, n_02);
        auto& e_07_01 = g_1.addEdge(make_unique<Edge>(),n_07, n_01);
        auto& e_02_09 = g_1.addEdge(make_unique<Edge>(),n_02, n_09);
        auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09, n_10);
        auto& e_10_01 = g_1.addEdge(make_unique<Edge>(),n_10, n_01);
        auto& e_10_02 = g_1.addEdge(make_unique<Edge>(),n_10, n_02);
        auto& e_09_01 = g_1.addEdge(make_unique<Edge>(),n_09, n_01);

        SPQRDecomposer decomposer;
        node_list_it_t nStartIt;
        switch (i) {
          case 0:
            nStartIt = n_01.backIt();
            break;
          case 1:
            nStartIt = n_02.backIt();
            break;
          case 2:
            nStartIt = n_03.backIt();
            break;
          case 3:
            nStartIt = n_04.backIt();
            break;
          case 4:
            nStartIt = n_05.backIt();
            break;
          case 5:
            nStartIt = n_06.backIt();
            break;
          case 6:
            nStartIt = n_07.backIt();
            break;
          case 7:
            nStartIt = n_08.backIt();
            break;
          case 8:
            nStartIt = n_09.backIt();
            break;
          case 9:
          default:
            nStartIt = n_10.backIt();
        }

        auto& nStart = dynamic_cast<Node&>(*(*nStartIt));

        SPQRTree spqrTree = decomposer.decompose(g_1,nStart);

        testTreeElem tte_01;
        tte_01.mNodeNumber = 1;
        tte_01.mNumComponentEdges = 6;
        tte_01.mRealEdges.push_back(e_02_03.backIt());
        tte_01.mRealEdges.push_back(e_03_04.backIt());
        tte_01.mRealEdges.push_back(e_04_01.backIt());
        tte_01.mRealEdges.push_back(e_04_02.backIt());
        tte_01.mRealEdges.push_back(e_03_01.backIt());
        tte_01.mNodes.push_back(n_01.backIt());
        tte_01.mNodes.push_back(n_02.backIt());
        tte_01.mNodes.push_back(n_03.backIt());
        tte_01.mNodes.push_back(n_04.backIt());
        tte_01.mNeighbors.push_back(5);
        tte_01.mType = SPQRTreeNode::RType;

        testTreeElem tte_02;
        tte_02.mNodeNumber = 2;
        tte_02.mNumComponentEdges = 6;
        tte_02.mRealEdges.push_back(e_02_05.backIt());
        tte_02.mRealEdges.push_back(e_05_06.backIt());
        tte_02.mRealEdges.push_back(e_06_01.backIt());
        tte_02.mRealEdges.push_back(e_06_02.backIt());
        tte_02.mRealEdges.push_back(e_05_01.backIt());
        tte_02.mNodes.push_back(n_01.backIt());
        tte_02.mNodes.push_back(n_02.backIt());
        tte_02.mNodes.push_back(n_05.backIt());
        tte_02.mNodes.push_back(n_06.backIt());
        tte_02.mNeighbors.push_back(5);
        tte_02.mType = SPQRTreeNode::RType;

        testTreeElem tte_03;
        tte_03.mNodeNumber = 3;
        tte_03.mNumComponentEdges = 6;
        tte_03.mRealEdges.push_back(e_02_07.backIt());
        tte_03.mRealEdges.push_back(e_07_08.backIt());
        tte_03.mRealEdges.push_back(e_08_01.backIt());
        tte_03.mRealEdges.push_back(e_08_02.backIt());
        tte_03.mRealEdges.push_back(e_07_01.backIt());
        tte_03.mNodes.push_back(n_01.backIt());
        tte_03.mNodes.push_back(n_02.backIt());
        tte_03.mNodes.push_back(n_07.backIt());
        tte_03.mNodes.push_back(n_08.backIt());
        tte_03.mNeighbors.push_back(5);
        tte_03.mType = SPQRTreeNode::RType;

        testTreeElem tte_04;
        tte_04.mNodeNumber = 4;
        tte_04.mNumComponentEdges = 6;
        tte_04.mRealEdges.push_back(e_02_09.backIt());
        tte_04.mRealEdges.push_back(e_09_10.backIt());
        tte_04.mRealEdges.push_back(e_10_01.backIt());
        tte_04.mRealEdges.push_back(e_10_02.backIt());
        tte_04.mRealEdges.push_back(e_09_01.backIt());
        tte_04.mNodes.push_back(n_01.backIt());
        tte_04.mNodes.push_back(n_02.backIt());
        tte_04.mNodes.push_back(n_09.backIt());
        tte_04.mNodes.push_back(n_10.backIt());
        tte_04.mNeighbors.push_back(5);
        tte_04.mType = SPQRTreeNode::RType;

        testTreeElem tte_05;
        tte_05.mNodeNumber = 5;
        tte_05.mNumComponentEdges = 5;
        tte_05.mRealEdges.push_back(e_01_02.backIt());
        tte_05.mNodes.push_back(n_01.backIt());
        tte_05.mNodes.push_back(n_02.backIt());
        tte_05.mNeighbors.push_back(1);
        tte_05.mNeighbors.push_back(2);
        tte_05.mNeighbors.push_back(3);
        tte_05.mNeighbors.push_back(4);
        tte_05.mType = SPQRTreeNode::PType;

        vector<testTreeElem> elems;
        elems.push_back(tte_01);
        elems.push_back(tte_02);
        elems.push_back(tte_03);
        elems.push_back(tte_04);
        elems.push_back(tte_05);

        EXPECT_EQ(testTree(spqrTree, elems),true);

        EXPECT_EQ(e_01_02.utilitySize(), 1);
        EXPECT_EQ(e_02_03.utilitySize(), 1);
        EXPECT_EQ(e_03_04.utilitySize(), 1);
        EXPECT_EQ(e_04_01.utilitySize(), 1);
        EXPECT_EQ(e_04_02.utilitySize(), 1);
        EXPECT_EQ(e_03_01.utilitySize(), 1);
        EXPECT_EQ(e_02_05.utilitySize(), 1);
        EXPECT_EQ(e_05_06.utilitySize(), 1);
        EXPECT_EQ(e_06_01.utilitySize(), 1);
        EXPECT_EQ(e_06_02.utilitySize(), 1);
        EXPECT_EQ(e_05_01.utilitySize(), 1);
        EXPECT_EQ(e_02_07.utilitySize(), 1);
        EXPECT_EQ(e_07_08.utilitySize(), 1);
        EXPECT_EQ(e_08_01.utilitySize(), 1);
        EXPECT_EQ(e_08_02.utilitySize(), 1);
        EXPECT_EQ(e_07_01.utilitySize(), 1);
        EXPECT_EQ(e_02_09.utilitySize(), 1);
        EXPECT_EQ(e_09_10.utilitySize(), 1);
        EXPECT_EQ(e_10_01.utilitySize(), 1);
        EXPECT_EQ(e_10_02.utilitySize(), 1);
        EXPECT_EQ(e_09_01.utilitySize(), 1);

        EXPECT_EQ(e_01_02.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_02_03.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_03_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_04_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_04_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_03_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_02_05.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_05_06.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_06_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_06_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_05_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_02_07.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_07_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_08_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_08_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_07_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_02_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_09_10.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_09_01.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);

        EXPECT_EQ(e_01_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_03.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_03_04.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_04_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_04_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_03_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_05.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_05_06.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_06_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_06_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_05_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_07.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_07_08.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_08_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_08_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_07_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_09.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_09_10.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_01.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_09_01.IGForwardLinksSize(), 1);

        EXPECT_EQ(e_01_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_03.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_03_04.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_04_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_04_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_03_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_05.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_05_06.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_06_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_06_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_05_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_07.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_07_08.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_08_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_08_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_07_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_09.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_09_10.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_01.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_09_01.IGBackwardLinksSize(),0);

    }
}


/**  @brief tests on decompose() with multiple starting points
 *          graph in https://en.wikipedia.org/wiki/SPQR_tree
 */
TEST_F(SPQRDecomposerTests, Test9) {

    for (size_t i = 0 ; i < 16; i++) {
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
        auto& n_14 = g_1.addNode(make_unique<Node>());
        auto& n_15 = g_1.addNode(make_unique<Node>());
        auto& n_16 = g_1.addNode(make_unique<Node>());

        auto& e_01_02 = g_1.addEdge(make_unique<Edge>(),n_01, n_02);
        auto& e_01_07 = g_1.addEdge(make_unique<Edge>(),n_01, n_07);
        auto& e_01_03 = g_1.addEdge(make_unique<Edge>(),n_01, n_03);
        auto& e_02_04 = g_1.addEdge(make_unique<Edge>(),n_02, n_04);
        auto& e_02_08 = g_1.addEdge(make_unique<Edge>(),n_02, n_08);
        auto& e_03_04 = g_1.addEdge(make_unique<Edge>(),n_03, n_04);
        auto& e_03_05 = g_1.addEdge(make_unique<Edge>(),n_03, n_05);
        auto& e_04_06 = g_1.addEdge(make_unique<Edge>(),n_04, n_06);
        auto& e_05_06 = g_1.addEdge(make_unique<Edge>(),n_05, n_06);
        auto& e_05_07 = g_1.addEdge(make_unique<Edge>(),n_05, n_07);
        auto& e_06_08 = g_1.addEdge(make_unique<Edge>(),n_06, n_08);
        auto& e_07_09 = g_1.addEdge(make_unique<Edge>(),n_07, n_09);
        auto& e_08_13 = g_1.addEdge(make_unique<Edge>(),n_08, n_13);
        auto& e_08_15 = g_1.addEdge(make_unique<Edge>(),n_08, n_15);
        auto& e_13_15 = g_1.addEdge(make_unique<Edge>(),n_13, n_15);
        auto& e_15_16 = g_1.addEdge(make_unique<Edge>(),n_15, n_16);
        auto& e_13_14 = g_1.addEdge(make_unique<Edge>(),n_13, n_14);
        auto& e_10_16 = g_1.addEdge(make_unique<Edge>(),n_10, n_16);
        auto& e_14_16 = g_1.addEdge(make_unique<Edge>(),n_14, n_16);
        auto& e_09_10 = g_1.addEdge(make_unique<Edge>(),n_09, n_10);
        auto& e_10_14 = g_1.addEdge(make_unique<Edge>(),n_10, n_14);
        auto& e_09_11 = g_1.addEdge(make_unique<Edge>(),n_09, n_11);
        auto& e_10_11 = g_1.addEdge(make_unique<Edge>(),n_10, n_11);
        auto& e_11_12 = g_1.addEdge(make_unique<Edge>(),n_11, n_12);
        auto& e_09_12 = g_1.addEdge(make_unique<Edge>(),n_09, n_12);
        auto& e_10_12 = g_1.addEdge(make_unique<Edge>(),n_10, n_12);

        SPQRDecomposer decomposer;
        node_list_it_t nStartIt;
        switch (i) {
          case 0:
            nStartIt = n_01.backIt();
            break;
          case 1:
            nStartIt = n_02.backIt();
            break;
          case 2:
            nStartIt = n_03.backIt();
            break;
          case 3:
            nStartIt = n_04.backIt();
            break;
          case 4:
            nStartIt = n_05.backIt();
            break;
          case 5:
            nStartIt = n_06.backIt();
            break;
          case 6:
            nStartIt = n_07.backIt();
            break;
          case 7:
            nStartIt = n_08.backIt();
            break;
          case 8:
            nStartIt = n_09.backIt();
            break;
          case 9:
            nStartIt = n_10.backIt();
            break;
          case 10:
            nStartIt = n_11.backIt();
            break;
          case 11:
            nStartIt = n_12.backIt();
            break;
          case 12:
            nStartIt = n_13.backIt();
            break;
          case 13:
            nStartIt = n_14.backIt();
            break;
          case 14:
            nStartIt = n_15.backIt();
            break;
          case 15:
          default:
            nStartIt = n_16.backIt();
        }

        auto& nStart = dynamic_cast<Node&>(*(*nStartIt));

        SPQRTree spqrTree = decomposer.decompose(g_1,nStart);

        testTreeElem tte_01;
        tte_01.mNodeNumber = 1;
        tte_01.mNumComponentEdges = 12;
        tte_01.mRealEdges.push_back(e_01_02.backIt());
        tte_01.mRealEdges.push_back(e_01_03.backIt());
        tte_01.mRealEdges.push_back(e_03_04.backIt());
        tte_01.mRealEdges.push_back(e_02_04.backIt());
        tte_01.mRealEdges.push_back(e_01_07.backIt());
        tte_01.mRealEdges.push_back(e_03_05.backIt());
        tte_01.mRealEdges.push_back(e_04_06.backIt());
        tte_01.mRealEdges.push_back(e_02_08.backIt());
        tte_01.mRealEdges.push_back(e_05_06.backIt());
        tte_01.mRealEdges.push_back(e_05_07.backIt());
        tte_01.mRealEdges.push_back(e_06_08.backIt());
        tte_01.mNodes.push_back(n_01.backIt());
        tte_01.mNodes.push_back(n_02.backIt());
        tte_01.mNodes.push_back(n_03.backIt());
        tte_01.mNodes.push_back(n_04.backIt());
        tte_01.mNodes.push_back(n_05.backIt());
        tte_01.mNodes.push_back(n_06.backIt());
        tte_01.mNodes.push_back(n_07.backIt());
        tte_01.mNodes.push_back(n_08.backIt());
        tte_01.mNeighbors.push_back(2);
        tte_01.mType = SPQRTreeNode::RType;

        testTreeElem tte_02;
        tte_02.mNodeNumber = 2;
        tte_02.mNumComponentEdges = 4;
        tte_02.mRealEdges.push_back(e_07_09.backIt());
        tte_02.mNodes.push_back(n_07.backIt());
        tte_02.mNodes.push_back(n_08.backIt());
        tte_02.mNodes.push_back(n_09.backIt());
        tte_02.mNodes.push_back(n_10.backIt());
        tte_02.mNeighbors.push_back(1);
        tte_02.mNeighbors.push_back(3);
        tte_02.mNeighbors.push_back(5);
        tte_02.mType = SPQRTreeNode::SType;

        testTreeElem tte_03;
        tte_03.mNodeNumber = 3;
        tte_03.mNumComponentEdges = 3;
        tte_03.mRealEdges.push_back(e_09_10.backIt());
        tte_03.mNodes.push_back(n_09.backIt());
        tte_03.mNodes.push_back(n_10.backIt());
        tte_03.mNeighbors.push_back(2);
        tte_03.mNeighbors.push_back(4);
        tte_03.mType = SPQRTreeNode::PType;

        testTreeElem tte_04;
        tte_04.mNodeNumber = 4;
        tte_04.mNumComponentEdges = 6;
        tte_04.mRealEdges.push_back(e_09_11.backIt());
        tte_04.mRealEdges.push_back(e_10_11.backIt());
        tte_04.mRealEdges.push_back(e_09_12.backIt());
        tte_04.mRealEdges.push_back(e_10_12.backIt());
        tte_04.mRealEdges.push_back(e_11_12.backIt());
        tte_04.mNodes.push_back(n_09.backIt());
        tte_04.mNodes.push_back(n_10.backIt());
        tte_04.mNodes.push_back(n_11.backIt());
        tte_04.mNodes.push_back(n_12.backIt());
        tte_04.mNeighbors.push_back(3);
        tte_04.mType = SPQRTreeNode::RType;

        testTreeElem tte_05;
        tte_05.mNodeNumber = 5;
        tte_05.mNumComponentEdges = 9;
        tte_05.mRealEdges.push_back(e_08_13.backIt());
        tte_05.mRealEdges.push_back(e_08_15.backIt());
        tte_05.mRealEdges.push_back(e_13_15.backIt());
        tte_05.mRealEdges.push_back(e_15_16.backIt());
        tte_05.mRealEdges.push_back(e_13_14.backIt());
        tte_05.mRealEdges.push_back(e_10_16.backIt());
        tte_05.mRealEdges.push_back(e_14_16.backIt());
        tte_05.mRealEdges.push_back(e_10_14.backIt());
        tte_05.mNodes.push_back(n_08.backIt());
        tte_05.mNodes.push_back(n_13.backIt());
        tte_05.mNodes.push_back(n_15.backIt());
        tte_05.mNodes.push_back(n_16.backIt());
        tte_05.mNodes.push_back(n_10.backIt());
        tte_05.mNodes.push_back(n_14.backIt());
        tte_05.mNeighbors.push_back(2);
        tte_05.mType = SPQRTreeNode::RType;

        vector<testTreeElem> elems;
        elems.push_back(tte_01);
        elems.push_back(tte_02);
        elems.push_back(tte_03);
        elems.push_back(tte_04);
        elems.push_back(tte_05);

        EXPECT_EQ(testTree(spqrTree, elems),true);

        EXPECT_EQ(e_01_02.utilitySize(), 1);
        EXPECT_EQ(e_01_07.utilitySize(), 1);
        EXPECT_EQ(e_01_03.utilitySize(), 1);
        EXPECT_EQ(e_02_04.utilitySize(), 1);
        EXPECT_EQ(e_02_08.utilitySize(), 1);
        EXPECT_EQ(e_03_04.utilitySize(), 1);
        EXPECT_EQ(e_03_05.utilitySize(), 1);
        EXPECT_EQ(e_04_06.utilitySize(), 1);
        EXPECT_EQ(e_05_06.utilitySize(), 1);
        EXPECT_EQ(e_05_07.utilitySize(), 1);
        EXPECT_EQ(e_06_08.utilitySize(), 1);
        EXPECT_EQ(e_07_09.utilitySize(), 1);
        EXPECT_EQ(e_08_13.utilitySize(), 1);
        EXPECT_EQ(e_08_15.utilitySize(), 1);
        EXPECT_EQ(e_13_15.utilitySize(), 1);
        EXPECT_EQ(e_15_16.utilitySize(), 1);
        EXPECT_EQ(e_13_14.utilitySize(), 1);
        EXPECT_EQ(e_10_16.utilitySize(), 1);
        EXPECT_EQ(e_14_16.utilitySize(), 1);
        EXPECT_EQ(e_09_10.utilitySize(), 1);
        EXPECT_EQ(e_10_14.utilitySize(), 1);
        EXPECT_EQ(e_09_11.utilitySize(), 1);
        EXPECT_EQ(e_10_11.utilitySize(), 1);
        EXPECT_EQ(e_11_12.utilitySize(), 1);
        EXPECT_EQ(e_09_12.utilitySize(), 1);
        EXPECT_EQ(e_10_12.utilitySize(), 1);

        EXPECT_EQ(e_01_02.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_01_07.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_01_03.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_02_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_02_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_03_04.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_03_05.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_04_06.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_05_06.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_05_07.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_06_08.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_07_09.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_08_13.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_08_15.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_13_15.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_15_16.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_13_14.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_16.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_14_16.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_09_10.utility(), SPQRTree::kForwardEdgeTypeSeparatingEdge);
        EXPECT_EQ(e_10_14.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_09_11.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_11.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_11_12.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_09_12.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);
        EXPECT_EQ(e_10_12.utility(), SPQRTree::kForwardEdgeTypeOrdinaryEdge);

        EXPECT_EQ(e_01_02.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_01_07.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_01_03.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_04.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_02_08.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_03_04.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_03_05.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_04_06.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_05_06.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_05_07.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_06_08.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_07_09.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_08_13.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_08_15.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_13_15.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_15_16.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_13_14.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_16.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_14_16.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_09_10.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_14.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_09_11.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_11.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_11_12.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_09_12.IGForwardLinksSize(), 1);
        EXPECT_EQ(e_10_12.IGForwardLinksSize(), 1);

        EXPECT_EQ(e_01_02.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_01_07.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_01_03.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_04.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_02_08.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_03_04.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_03_05.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_04_06.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_05_06.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_05_07.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_06_08.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_07_09.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_08_13.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_08_15.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_13_15.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_15_16.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_13_14.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_16.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_14_16.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_09_10.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_14.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_09_11.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_11.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_11_12.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_09_12.IGBackwardLinksSize(),0);
        EXPECT_EQ(e_10_12.IGBackwardLinksSize(),0);
    }
}

} // namespace Undirected

} // namespace Wailea
