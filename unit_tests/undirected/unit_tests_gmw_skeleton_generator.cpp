#include "gtest/gtest.h"
#include "undirected/gmw_skeleton_generator.hpp"

#include "undirected/tree_path_finder.hpp"
#include "undirected/tree_splitter.hpp"

namespace Wailea {

namespace Undirected {

class GMWSkeletonTests : public ::testing::Test {

  protected:

    GMWSkeletonTests(){;};
    virtual ~GMWSkeletonTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};
    void setComponentEdgeType(
           SPQRComponentEdge& e, enum SPQRComponentEdge::type t){e.mType = t;};

    void setSPQRTreeEdgeParams(
        SPQRComponentEdge& te,
        edge_list_it_t e1,
        edge_list_it_t e2,
        node_list_it_t n1
    ) {
        te.mTreeEdge            = e1;
        te.mVirtualPairEdge     = e2;
        te.mVirtualPairTreeNode = n1;
    }

    void setType(GMWSkeleton& skel, GMWSkeleton::type t){ skel.mType = t;}

    void setTreeNodeType(SPQRTreeNode& tn, enum SPQRTreeNode::type t) 
                                                               {tn.mType = t;}

    void setBlockNodes(GMWSkeleton& skel, vector<node_list_it_t>& ns){
        for (auto it : ns) { skel.mBlockNodes.push_back(it); }
    }

    void setBlockNit11(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit11 = nit; 
    }

    void setBlockNit12(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit12 = nit;
    }

    void setBlockNit21(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit21 = nit;
    }

    void setBlockNit22(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit22 = nit;
    }

    void generateSkeleton(GMWSkeleton& skel){
        skel.generateSkeleton();
    }
    
    bool checkSkelEdges(
        Graph& g,
        vector<edge_list_it_t>&edges,
        size_t numVirtualEdges,
        vector<edge_list_it_t>&virtualEdges 
    ) {

        if (g.numEdges() != edges.size() + numVirtualEdges){

            return false;
        }
        vector<int> counters;
        for (int i = 0; i < g.numEdges(); i++) {counters.push_back(0);}
        int index = 0;
        for (auto eit = g.edges().first; eit != g.edges().second; eit++) {

            auto& SE = dynamic_cast<GMWSkeletonEdge&>(*(*eit));

            for (auto e : edges) {
                if (SE.IGBackwardLink() == e && SE.mVirtual == false) {

                    counters[index] = counters[index] + 1;
                }
            }
            index++;
        }

        size_t ve = 0;
        index = 0;

        for (auto eit = g.edges().first; eit != g.edges().second; eit++) {

            auto& SE = dynamic_cast<GMWSkeletonEdge&>(*(*eit));

            if (counters[index] != 1) {

                if (counters[index] == 0&& SE.mVirtual == true) {

                    virtualEdges.push_back(eit);
                    ve++;
                }
                else {

                    return false;
                }
            }
            index++;
        }

        if (ve != numVirtualEdges) {

            return false;
        }

        return true;
    }    

};



/**  @brief Tests GMWSkeleton::generateSkeleton()
 */
TEST_F(GMWSkeletonTests, Test1) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeleton skel;
    // END1_NODE_END2_NODE

    setType(skel, GMWSkeleton::END1_NODE_END2_NODE);
    vector<node_list_it_t> blockNodes;

    blockNodes.push_back(bn_01.backIt());
    blockNodes.push_back(bn_02.backIt());    
    blockNodes.push_back(bn_03.backIt());
    blockNodes.push_back(bn_04.backIt());
    blockNodes.push_back(bn_05.backIt());
    blockNodes.push_back(bn_06.backIt());
    blockNodes.push_back(bn_07.backIt());
    blockNodes.push_back(bn_08.backIt());

    setBlockNodes(skel, blockNodes);
    setBlockNit11(skel, bn_01.backIt());
    setBlockNit21(skel, bn_02.backIt());

    generateSkeleton(skel);

    Graph& sk = skel.graph();

    EXPECT_EQ(sk.numNodes(), 8);
    auto sknit = sk.nodes().first;
    auto& sn_01 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_01.IGBackwardLink(), bn_01.backIt());
    sknit++;
    auto& sn_02 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_02.IGBackwardLink(), bn_02.backIt());
    sknit++;
    auto& sn_03 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_03.IGBackwardLink(), bn_03.backIt());
    sknit++;
    auto& sn_04 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_04.IGBackwardLink(), bn_04.backIt());
    sknit++;
    auto& sn_05 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_05.IGBackwardLink(), bn_05.backIt());
    sknit++;
    auto& sn_06 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_06.IGBackwardLink(), bn_06.backIt());
    sknit++;
    auto& sn_07 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_07.IGBackwardLink(), bn_07.backIt());
    sknit++;
    auto& sn_08 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_08.IGBackwardLink(), bn_08.backIt());

    EXPECT_EQ(sk.numEdges(), 11);

    vector<edge_list_it_t> skelEdgesTested;
    vector<edge_list_it_t> virtualEdges;
    skelEdgesTested.push_back(be_01_02.backIt());
    skelEdgesTested.push_back(be_01_03.backIt());
    skelEdgesTested.push_back(be_01_07.backIt());
    skelEdgesTested.push_back(be_02_04.backIt());
    skelEdgesTested.push_back(be_02_08.backIt());
    skelEdgesTested.push_back(be_03_04.backIt());
    skelEdgesTested.push_back(be_03_05.backIt());
    skelEdgesTested.push_back(be_04_06.backIt());
    skelEdgesTested.push_back(be_05_06.backIt());
    skelEdgesTested.push_back(be_05_07.backIt());
    skelEdgesTested.push_back(be_06_08.backIt());

    EXPECT_EQ(checkSkelEdges(sk, skelEdgesTested, 0, virtualEdges), true);

    EXPECT_EQ(skel.mSkelNit1, sn_01.backIt());
    EXPECT_EQ(skel.mSkelNit2, sn_02.backIt());
    EXPECT_EQ(skel.mSkelEit1, sk.edges().second);
    EXPECT_EQ(skel.mSkelEit2, sk.edges().second);

}


/**  @brief Tests GMWSkeleton::generateSkeleton()
 */
TEST_F(GMWSkeletonTests, Test2) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeleton skel;
    // END1_NODE_END2_NODE

    setType(skel, GMWSkeleton::END1_NODE_END2_EDGE);
    vector<node_list_it_t> blockNodes;

    blockNodes.push_back(bn_01.backIt());
    blockNodes.push_back(bn_02.backIt());    
    blockNodes.push_back(bn_03.backIt());
    blockNodes.push_back(bn_04.backIt());
    blockNodes.push_back(bn_05.backIt());
    blockNodes.push_back(bn_06.backIt());
    blockNodes.push_back(bn_07.backIt());
    blockNodes.push_back(bn_08.backIt());

    setBlockNodes(skel, blockNodes);
    setBlockNit11(skel, bn_01.backIt());
    setBlockNit21(skel, bn_07.backIt());
    setBlockNit22(skel, bn_08.backIt());

    generateSkeleton(skel);

    Graph& sk = skel.graph();

    EXPECT_EQ(sk.numNodes(), 8);
    auto sknit = sk.nodes().first;
    auto& sn_01 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_01.IGBackwardLink(), bn_01.backIt());
    sknit++;
    auto& sn_02 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_02.IGBackwardLink(), bn_02.backIt());
    sknit++;
    auto& sn_03 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_03.IGBackwardLink(), bn_03.backIt());
    sknit++;
    auto& sn_04 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_04.IGBackwardLink(), bn_04.backIt());
    sknit++;
    auto& sn_05 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_05.IGBackwardLink(), bn_05.backIt());
    sknit++;
    auto& sn_06 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_06.IGBackwardLink(), bn_06.backIt());
    sknit++;
    auto& sn_07 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_07.IGBackwardLink(), bn_07.backIt());
    sknit++;
    auto& sn_08 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_08.IGBackwardLink(), bn_08.backIt());

    EXPECT_EQ(sk.numEdges(), 12);

    vector<edge_list_it_t> skelEdgesTested;
    vector<edge_list_it_t> virtualEdges;
    skelEdgesTested.push_back(be_01_02.backIt());
    skelEdgesTested.push_back(be_01_03.backIt());
    skelEdgesTested.push_back(be_01_07.backIt());
    skelEdgesTested.push_back(be_02_04.backIt());
    skelEdgesTested.push_back(be_02_08.backIt());
    skelEdgesTested.push_back(be_03_04.backIt());
    skelEdgesTested.push_back(be_03_05.backIt());
    skelEdgesTested.push_back(be_04_06.backIt());
    skelEdgesTested.push_back(be_05_06.backIt());
    skelEdgesTested.push_back(be_05_07.backIt());
    skelEdgesTested.push_back(be_06_08.backIt());

    EXPECT_EQ(checkSkelEdges(sk, skelEdgesTested, 1, virtualEdges), true);

    EXPECT_EQ(skel.mSkelNit1, sn_01.backIt());
    EXPECT_EQ(skel.mSkelNit2, sk.nodes().second);
    EXPECT_EQ(skel.mSkelEit1, sk.edges().second);
    EXPECT_EQ(skel.mSkelEit2, *(virtualEdges.begin()));

    auto& VE2 = dynamic_cast<GMWSkeletonEdge&>(*(*skel.mSkelEit2));
    EXPECT_EQ((&(VE2.incidentNode1()) == &sn_07 && 
               &(VE2.incidentNode2()) == &sn_08)||
              (&(VE2.incidentNode1()) == &sn_08 && 
               &(VE2.incidentNode2()) == &sn_07)  , true);

}


/**  @brief Tests GMWSkeleton::generateSkeleton()
 */
TEST_F(GMWSkeletonTests, Test3) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeleton skel;

    setType(skel, GMWSkeleton::END1_EDGE_END2_NODE);
    vector<node_list_it_t> blockNodes;

    blockNodes.push_back(bn_10.backIt());
    blockNodes.push_back(bn_12.backIt());    
    blockNodes.push_back(bn_17.backIt());
    blockNodes.push_back(bn_18.backIt());
    blockNodes.push_back(bn_19.backIt());
    blockNodes.push_back(bn_20.backIt());
    blockNodes.push_back(bn_21.backIt());
    blockNodes.push_back(bn_22.backIt());

    setBlockNodes(skel, blockNodes);
    setBlockNit11(skel, bn_10.backIt());
    setBlockNit12(skel, bn_12.backIt());
    setBlockNit21(skel, bn_19.backIt());

    generateSkeleton(skel);

    Graph& sk = skel.graph();

    EXPECT_EQ(sk.numNodes(), 8);
    auto sknit = sk.nodes().first;
    auto& sn_01 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_01.IGBackwardLink(), bn_10.backIt());
    sknit++;
    auto& sn_02 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_02.IGBackwardLink(), bn_12.backIt());
    sknit++;
    auto& sn_03 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_03.IGBackwardLink(), bn_17.backIt());
    sknit++;
    auto& sn_04 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_04.IGBackwardLink(), bn_18.backIt());
    sknit++;
    auto& sn_05 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_05.IGBackwardLink(), bn_19.backIt());
    sknit++;
    auto& sn_06 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_06.IGBackwardLink(), bn_20.backIt());
    sknit++;
    auto& sn_07 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_07.IGBackwardLink(), bn_21.backIt());
    sknit++;
    auto& sn_08 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_08.IGBackwardLink(), bn_22.backIt());

    EXPECT_EQ(sk.numEdges(), 14);

    vector<edge_list_it_t> skelEdgesTested;
    vector<edge_list_it_t> virtualEdges;
    skelEdgesTested.push_back(be_10_17.backIt());
    skelEdgesTested.push_back(be_12_17.backIt());
    skelEdgesTested.push_back(be_10_18.backIt());
    skelEdgesTested.push_back(be_12_18.backIt());
    skelEdgesTested.push_back(be_17_18.backIt());
    skelEdgesTested.push_back(be_12_21.backIt());
    skelEdgesTested.push_back(be_12_19.backIt());
    skelEdgesTested.push_back(be_18_20.backIt());
    skelEdgesTested.push_back(be_18_22.backIt());
    skelEdgesTested.push_back(be_19_20.backIt());
    skelEdgesTested.push_back(be_19_21.backIt());
    skelEdgesTested.push_back(be_20_22.backIt());
    skelEdgesTested.push_back(be_21_22.backIt());

    EXPECT_EQ(checkSkelEdges(sk, skelEdgesTested, 1, virtualEdges), true);

    EXPECT_EQ(skel.mSkelNit1, sk.nodes().second);
    EXPECT_EQ(skel.mSkelNit2, sn_05.backIt());

    EXPECT_EQ(skel.mSkelEit1, *(virtualEdges.begin()));
    EXPECT_EQ(skel.mSkelEit2, sk.edges().second);

    auto& VE1 = dynamic_cast<GMWSkeletonEdge&>(*(*skel.mSkelEit1));
    EXPECT_EQ((&(VE1.incidentNode1()) == &sn_01 && 
               &(VE1.incidentNode2()) == &sn_02)||
              (&(VE1.incidentNode1()) == &sn_02 && 
               &(VE1.incidentNode2()) == &sn_01)  , true);
}


/**  @brief Tests GMWSkeleton::generateSkeleton()
 */
TEST_F(GMWSkeletonTests, Test4) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeleton skel;

    setType(skel, GMWSkeleton::END1_EDGE_END2_EDGE);
    vector<node_list_it_t> blockNodes;

    blockNodes.push_back(bn_07.backIt());
    blockNodes.push_back(bn_08.backIt());    
    blockNodes.push_back(bn_09.backIt());
    blockNodes.push_back(bn_10.backIt());
    blockNodes.push_back(bn_13.backIt());
    blockNodes.push_back(bn_14.backIt());
    blockNodes.push_back(bn_15.backIt());
    blockNodes.push_back(bn_16.backIt());

    setBlockNodes(skel, blockNodes);
    setBlockNit11(skel, bn_07.backIt());
    setBlockNit12(skel, bn_08.backIt());
    setBlockNit21(skel, bn_09.backIt());
    setBlockNit22(skel, bn_10.backIt());

    generateSkeleton(skel);

    Graph& sk = skel.graph();

    EXPECT_EQ(sk.numNodes(), 8);
    auto sknit = sk.nodes().first;
    auto& sn_01 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_01.IGBackwardLink(), bn_07.backIt());
    sknit++;
    auto& sn_02 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_02.IGBackwardLink(), bn_08.backIt());
    sknit++;
    auto& sn_03 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_03.IGBackwardLink(), bn_09.backIt());
    sknit++;
    auto& sn_04 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_04.IGBackwardLink(), bn_10.backIt());
    sknit++;
    auto& sn_05 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_05.IGBackwardLink(), bn_13.backIt());
    sknit++;
    auto& sn_06 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_06.IGBackwardLink(), bn_14.backIt());
    sknit++;
    auto& sn_07 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_07.IGBackwardLink(), bn_15.backIt());
    sknit++;
    auto& sn_08 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_08.IGBackwardLink(), bn_16.backIt());

    EXPECT_EQ(sk.numEdges(), 11);

    vector<edge_list_it_t> skelEdgesTested;
    vector<edge_list_it_t> virtualEdges;
    skelEdgesTested.push_back(be_07_09.backIt());
    skelEdgesTested.push_back(be_08_15.backIt());
    skelEdgesTested.push_back(be_08_13.backIt());
    skelEdgesTested.push_back(be_13_15.backIt());
    skelEdgesTested.push_back(be_13_14.backIt());
    skelEdgesTested.push_back(be_14_10.backIt());
    skelEdgesTested.push_back(be_10_16.backIt());
    skelEdgesTested.push_back(be_14_16.backIt());
    skelEdgesTested.push_back(be_15_16.backIt());

    EXPECT_EQ(checkSkelEdges(sk, skelEdgesTested, 2, virtualEdges), true);

    EXPECT_EQ(skel.mSkelNit1, sk.nodes().second);
    EXPECT_EQ(skel.mSkelNit2, sk.nodes().second);

    EXPECT_EQ(  (skel.mSkelEit1==*(virtualEdges.begin()) && 
                skel.mSkelEit2==*(virtualEdges.rbegin())  )||
               (skel.mSkelEit1==*(virtualEdges.begin()) && 
                skel.mSkelEit2==*(virtualEdges.rbegin())  ), true);

    auto& VE1 = dynamic_cast<GMWSkeletonEdge&>(*(*skel.mSkelEit1));
    EXPECT_EQ((&(VE1.incidentNode1()) == &sn_01 && 
               &(VE1.incidentNode2()) == &sn_02)||
              (&(VE1.incidentNode1()) == &sn_02 && 
               &(VE1.incidentNode2()) == &sn_01)  , true);

    auto& VE2 = dynamic_cast<GMWSkeletonEdge&>(*(*skel.mSkelEit2));
    EXPECT_EQ((&(VE2.incidentNode1()) == &sn_03 && 
               &(VE2.incidentNode2()) == &sn_04)||
              (&(VE2.incidentNode1()) == &sn_04 && 
               &(VE2.incidentNode2()) == &sn_03)  , true);

}


class GMWSkeletonGeneratorTests : public ::testing::Test {

  protected:

    GMWSkeletonGeneratorTests(){;};
    virtual ~GMWSkeletonGeneratorTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    void setComponentEdgeType(
           SPQRComponentEdge& e, enum SPQRComponentEdge::type t){e.mType = t;};

    void setSPQRTreeEdgeParams(
        SPQRComponentEdge& te,
        edge_list_it_t e1,
        edge_list_it_t e2,
        node_list_it_t n1
    ) {
        te.mTreeEdge            = e1;
        te.mVirtualPairEdge     = e2;
        te.mVirtualPairTreeNode = n1;

    }

    void findTreeNodesFromBlockNodes(
        GMWSkeletonGenerator& skel,
        SPQRTree&       T,
        node_list_it_t  bnit1,
        node_list_it_t  bnit2,
        node_list_it_t& tnit1,
        node_list_it_t& tnit2
    ){ skel.findTreeNodesFromBlockNodes(T, bnit1, bnit2, tnit1, tnit2); }


    void findMinimalTreePath(
        GMWSkeletonGenerator& skel,
        list<node_list_it_t>& treePathNodes,
        list<edge_list_it_t>& treePathEdges,
        node_list_it_t        bnit1,
        node_list_it_t        bnit2
    ){ skel.findMinimalTreePath(treePathNodes, treePathEdges, bnit1, bnit2); }

    vector<node_list_it_t> fromTreeNodesToSkeletonNodes(
        GMWSkeletonGenerator& skel,
        list<node_list_it_t> treeNodes
    ){ return skel.fromTreeNodesToSkeletonNodes(treeNodes); }

    bool checkNodes(
             vector<node_list_it_t>& nodes1, vector<node_list_it_t>& nodes2) {

        if (nodes1.size() != nodes2.size()){
            return false;
        }

        vector<int> counters;
        for (int i = 0; i < nodes1.size(); i++) {counters.push_back(0);}
        int index = 0;
        for (auto n1 : nodes1) {

            for (auto n2 : nodes2) {

                if (n1 == n2) {
                    counters[index] = counters[index] + 1;
                }
            }
            index++;
        }

        for (auto c1 : counters) {

            if (c1 != 1) {
                return false;
            }
        }
        return true;
    }    

    bool isVirtualEdge(
        GMWSkeletonGenerator& skelGen, 
        node_list_it_t tnit,
        node_list_it_t bnit1,
        node_list_it_t bnit2
    ) {
        return skelGen.isVirtualEdge(tnit, bnit1, bnit2);
    }

    void generateSkeleton(GMWSkeleton& skel){
        skel.generateSkeleton();
    }

    void setType(GMWSkeleton& skel, GMWSkeleton::type t){ skel.mType = t;}

    void setBlockNodes(GMWSkeleton& skel, vector<node_list_it_t>& ns){
        for (auto it : ns) { skel.mBlockNodes.push_back(it); }
    }

    void setBlockNit11(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit11 = nit; 
    }

    void setBlockNit12(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit12 = nit;
    }

    void setBlockNit21(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit21 = nit;
    }

    void setBlockNit22(GMWSkeleton& skel, node_list_it_t nit){
        skel.mBlockNit22 = nit;
    }

    void setTreeNodeType(SPQRTreeNode& tn, enum SPQRTreeNode::type t)
                                                              {tn.mType = t;}
    
    bool checkSkelEdges(
        Graph& g,
        vector<edge_list_it_t>&edges,
        size_t numVirtualEdges,
        vector<edge_list_it_t>&virtualEdges 
    ) {

        if (g.numEdges() != edges.size() + numVirtualEdges){

            return false;
        }
        vector<int> counters;
        for (int i = 0; i < g.numEdges(); i++) {counters.push_back(0);}
        int index = 0;
        for (auto eit = g.edges().first; eit != g.edges().second; eit++) {

            auto& SE = dynamic_cast<GMWSkeletonEdge&>(*(*eit));

            for (auto e : edges) {
                if (SE.IGBackwardLink() == e && SE.mVirtual == false) {

                    counters[index] = counters[index] + 1;
                }
            }
            index++;
        }

        size_t ve = 0;
        index = 0;

        for (auto eit = g.edges().first; eit != g.edges().second; eit++) {

            auto& SE = dynamic_cast<GMWSkeletonEdge&>(*(*eit));

            if (counters[index] != 1) {

                if (counters[index] == 0&& SE.mVirtual == true) {

                    virtualEdges.push_back(eit);
                    ve++;
                }
                else {

                    return false;
                }
            }
            index++;
        }

        if (ve != numVirtualEdges) {

            return false;
        }

        return true;
    }    


    bool checkSkelNodes(
        Graph& skel,
        vector<node_list_it_t>& blockNodes,
        vector<node_list_it_t>& skelNodes
    ) {
        if (skel.numNodes() != blockNodes.size()){
            return false;
        }
        skelNodes.clear();
        vector<size_t> counters;
        for (int i = 0; i < blockNodes.size(); i++) {
            skelNodes.push_back(*blockNodes.end());
            counters.push_back(0);
        }
        int index = 0;
        for (auto n2 : blockNodes) {
            for (auto n1=skel.nodes().first; n1!=skel.nodes().second; n1++) {
                auto& SK = dynamic_cast<GMWSkeletonNode&>(*(*n1));
                if (SK.IGBackwardLink() == n2) {
                    counters[index] = counters[index] + 1;
                    skelNodes[index] = n1;
                }
            }
            index++;
        }
        for (auto c1 : counters) {
            if (c1 != 1) {
                return false;
            }
        }
        return true;
    }    

};



/**  @brief Tests GMWSkeletonGenerator::findTreeNodesFromBlockNodes().
 */
TEST_F(GMWSkeletonGeneratorTests, Test1) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skel;
    node_list_it_t tnit1;
    node_list_it_t tnit2;

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_01.backIt(), bn_02.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1, tn_01.backIt());
    EXPECT_EQ(tnit2, tn_01.backIt());

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_03.backIt(), bn_04.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1, tn_01.backIt());
    EXPECT_EQ(tnit2, tn_01.backIt());

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_05.backIt(), bn_06.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1, tn_01.backIt());
    EXPECT_EQ(tnit2, tn_01.backIt());

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_07.backIt(), bn_08.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_01.backIt()||tnit1==tn_02.backIt(), true);
    EXPECT_EQ(tnit2==tn_01.backIt()||tnit2==tn_02.backIt()||
                                                  tnit2==tn_03.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_09.backIt(), bn_10.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_02.backIt()||tnit1==tn_04.backIt()||
                                                  tnit1==tn_05.backIt(), true);
    EXPECT_EQ(tnit2==tn_02.backIt()||tnit2==tn_03.backIt()||
              tnit2==tn_04.backIt()||tnit2==tn_05.backIt()||
              tnit2==tn_06.backIt()||tnit2==tn_07.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_11.backIt(), bn_12.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_05.backIt(), true);
    EXPECT_EQ(tnit2==tn_05.backIt()||tnit2==tn_06.backIt()||
              tnit2==tn_07.backIt()||tnit2==tn_08.backIt()||
              tnit2==tn_09.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_13.backIt(), bn_14.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_03.backIt(), true);
    EXPECT_EQ(tnit2==tn_03.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_15.backIt(), bn_16.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_03.backIt(), true);
    EXPECT_EQ(tnit2==tn_03.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_17.backIt(), bn_18.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_07.backIt(), true);
    EXPECT_EQ(tnit2==tn_07.backIt()||tnit2==tn_08.backIt()||
                                                  tnit2==tn_09.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_19.backIt(), bn_20.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_09.backIt(), true);
    EXPECT_EQ(tnit2==tn_09.backIt(), true);

    findTreeNodesFromBlockNodes(
                 skel, spqrTree, bn_21.backIt(), bn_22.backIt(), tnit1, tnit2);
    EXPECT_EQ(tnit1==tn_09.backIt(), true);
    EXPECT_EQ(tnit2==tn_09.backIt(), true);

}


/**  @brief Tests GMWSkeletonGenerator::findMinimalTreePath().
 */
TEST_F(GMWSkeletonGeneratorTests, Test2) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skel;

    list<node_list_it_t> nodes_01;
    list<edge_list_it_t> edges_01;

    nodes_01.push_back(tn_01.backIt());
    nodes_01.push_back(tn_02.backIt());
    nodes_01.push_back(tn_03.backIt());
    edges_01.push_back(te_01_02.backIt());
    edges_01.push_back(te_02_03.backIt());

    findMinimalTreePath(
                   skel, nodes_01, edges_01, bn_07.backIt(), bn_08.backIt());
    EXPECT_EQ(nodes_01.size(), 1);
    EXPECT_EQ(edges_01.size(), 0);
    EXPECT_EQ(*(nodes_01.begin())==tn_01.backIt()||
              *(nodes_01.begin())==tn_02.backIt(), true);

    list<node_list_it_t> nodes_02;
    list<edge_list_it_t> edges_02;

    nodes_02.push_back(tn_01.backIt());
    nodes_02.push_back(tn_02.backIt());
    nodes_02.push_back(tn_04.backIt());
    nodes_02.push_back(tn_05.backIt());
    nodes_02.push_back(tn_06.backIt());
    nodes_02.push_back(tn_07.backIt());
    edges_02.push_back(te_01_02.backIt());
    edges_02.push_back(te_02_04.backIt());
    edges_02.push_back(te_04_05.backIt());
    edges_02.push_back(te_05_06.backIt());
    edges_02.push_back(te_06_07.backIt());

    findMinimalTreePath(
                   skel, nodes_02, edges_02, bn_08.backIt(), bn_12.backIt());
    EXPECT_EQ(nodes_02.size(), 3);
    EXPECT_EQ(edges_02.size(), 2);

    auto neit = nodes_02.begin();
    EXPECT_EQ(*neit, tn_02.backIt());
    neit++;
    EXPECT_EQ(*neit, tn_04.backIt());
    neit++;
    EXPECT_EQ(*neit, tn_05.backIt());

    auto eeit = edges_02.begin();
    EXPECT_EQ(*eeit, te_02_04.backIt());
    eeit++;
    EXPECT_EQ(*eeit, te_04_05.backIt());

    list<node_list_it_t> nodes_03;
    list<edge_list_it_t> edges_03;

    nodes_03.push_back(tn_03.backIt());
    nodes_03.push_back(tn_02.backIt());
    nodes_03.push_back(tn_04.backIt());
    nodes_03.push_back(tn_05.backIt());
    nodes_03.push_back(tn_06.backIt());
    nodes_03.push_back(tn_07.backIt());
    nodes_03.push_back(tn_08.backIt());
    nodes_03.push_back(tn_09.backIt());
    edges_03.push_back(te_02_03.backIt());
    edges_03.push_back(te_02_04.backIt());
    edges_03.push_back(te_04_05.backIt());
    edges_03.push_back(te_05_06.backIt());
    edges_03.push_back(te_06_07.backIt());
    edges_03.push_back(te_07_08.backIt());
    edges_03.push_back(te_08_09.backIt());

    findMinimalTreePath(
                   skel, nodes_03, edges_03, bn_13.backIt(), bn_22.backIt());
    EXPECT_EQ(nodes_03.size(), 8);
    EXPECT_EQ(edges_03.size(), 7);

}



/**  @brief Tests GMWSkeletonGenerator::fromTreeNodesToSkeletonNodes().
 */
TEST_F(GMWSkeletonGeneratorTests, Test3) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skel;

    list<node_list_it_t> treeNodes_01;
    treeNodes_01.push_back(tn_04.backIt());
    vector<node_list_it_t> blockNodes_01 = 
                              fromTreeNodesToSkeletonNodes(skel, treeNodes_01);
    vector<node_list_it_t> blockNodesExpected_01;
    blockNodesExpected_01.push_back(bn_09.backIt());
    blockNodesExpected_01.push_back(bn_10.backIt());
    EXPECT_EQ(checkNodes(blockNodes_01, blockNodesExpected_01), true);

    list<node_list_it_t> treeNodes_02;
    treeNodes_02.push_back(tn_01.backIt());
    treeNodes_02.push_back(tn_02.backIt());
    treeNodes_02.push_back(tn_03.backIt());

    vector<node_list_it_t> blockNodes_02 = 
                              fromTreeNodesToSkeletonNodes(skel, treeNodes_02);
    vector<node_list_it_t> blockNodesExpected_02;
    blockNodesExpected_02.push_back(bn_01.backIt());
    blockNodesExpected_02.push_back(bn_02.backIt());
    blockNodesExpected_02.push_back(bn_03.backIt());
    blockNodesExpected_02.push_back(bn_04.backIt());
    blockNodesExpected_02.push_back(bn_05.backIt());
    blockNodesExpected_02.push_back(bn_06.backIt());
    blockNodesExpected_02.push_back(bn_07.backIt());
    blockNodesExpected_02.push_back(bn_08.backIt());
    blockNodesExpected_02.push_back(bn_09.backIt());
    blockNodesExpected_02.push_back(bn_10.backIt());
    blockNodesExpected_02.push_back(bn_13.backIt());
    blockNodesExpected_02.push_back(bn_14.backIt());
    blockNodesExpected_02.push_back(bn_15.backIt());
    blockNodesExpected_02.push_back(bn_16.backIt());
    EXPECT_EQ(checkNodes(blockNodes_02, blockNodesExpected_02), true);

    list<node_list_it_t> treeNodes_03;
    treeNodes_03.push_back(tn_05.backIt());
    treeNodes_03.push_back(tn_06.backIt());
    treeNodes_03.push_back(tn_07.backIt());

    vector<node_list_it_t> blockNodes_03 = 
                              fromTreeNodesToSkeletonNodes(skel, treeNodes_03);
    vector<node_list_it_t> blockNodesExpected_03;
    blockNodesExpected_03.push_back(bn_09.backIt());
    blockNodesExpected_03.push_back(bn_10.backIt());
    blockNodesExpected_03.push_back(bn_11.backIt());
    blockNodesExpected_03.push_back(bn_12.backIt());
    blockNodesExpected_03.push_back(bn_17.backIt());
    blockNodesExpected_03.push_back(bn_18.backIt());

    EXPECT_EQ(checkNodes(blockNodes_03, blockNodesExpected_03), true);

}


/**  @brief Tests GMWSkeletonGenerator::processBlock() in steps.
 */
TEST_F(GMWSkeletonGeneratorTests, Test4) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    // Find the tree nodes for N1 and N2.
    bool res1 = 
       isVirtualEdge(skelGen, tn_01.backIt(), bn_07.backIt(), bn_08.backIt());
    EXPECT_EQ(res1, true);

    bool res2 = 
       isVirtualEdge(skelGen, tn_01.backIt(), bn_07.backIt(), bn_01.backIt());
    EXPECT_EQ(res2, false);

    bool res3 = 
       isVirtualEdge(skelGen, tn_02.backIt(), bn_07.backIt(), bn_08.backIt());
    EXPECT_EQ(res3, true);

    bool res4 = 
       isVirtualEdge(skelGen, tn_03.backIt(), bn_13.backIt(), bn_14.backIt());
    EXPECT_EQ(res4, false);

}

/**  @brief Tests GMWSkeletonGenerator::processBlock() in steps.
 *                same block, virtual edge
 */
TEST_F(GMWSkeletonGeneratorTests, Test5) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    // Find the tree nodes for N1 and N2.
    node_list_it_t bnit1 = bn_07.backIt();
    node_list_it_t bnit2 = bn_08.backIt();

    node_list_it_t tnit1;
    node_list_it_t tnit2;

    findTreeNodesFromBlockNodes(skelGen, spqrTree, bnit1, bnit2, tnit1, tnit2);

    auto& TN1 = dynamic_cast<SPQRTreeNode&>(*(*tnit1));
    auto& TN2 = dynamic_cast<SPQRTreeNode&>(*(*tnit2));

    // Find a path from N1 to N2 on the tree.
    TreePathFinder finder;

    list<node_list_it_t> spqrPathNodes;
    list<edge_list_it_t> spqrPathEdges;
    finder.findPath(spqrTree, TN1, TN2, spqrPathNodes, spqrPathEdges);

    // Find the minimal path from N1 to N2 on the tree.
    findMinimalTreePath(skelGen, spqrPathNodes, spqrPathEdges, bnit1, bnit2);

    // Find the component nodes for each representative node along the path.
    EXPECT_EQ(spqrPathNodes.size(),1);
    EXPECT_EQ(
         isVirtualEdge(skelGen, *(spqrPathNodes.begin()), bnit1, bnit2), true);
    
}


/**  @brief Tests GMWSkeletonGenerator::processBlock() in steps.
 *                same block, not virtual edge
 */
TEST_F(GMWSkeletonGeneratorTests, Test6) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    // Find the tree nodes for N1 and N2.
    node_list_it_t bnit1 = bn_01.backIt();
    node_list_it_t bnit2 = bn_08.backIt();

    node_list_it_t tnit1;
    node_list_it_t tnit2;

    findTreeNodesFromBlockNodes(skelGen, spqrTree, bnit1, bnit2, tnit1, tnit2);

    auto& TN1 = dynamic_cast<SPQRTreeNode&>(*(*tnit1));
    auto& TN2 = dynamic_cast<SPQRTreeNode&>(*(*tnit2));

    // Find a path from N1 to N2 on the tree.
    TreePathFinder finder;

    list<node_list_it_t> spqrPathNodes;
    list<edge_list_it_t> spqrPathEdges;
    finder.findPath(spqrTree, TN1, TN2, spqrPathNodes, spqrPathEdges);

    // Find the minimal path from N1 to N2 on the tree.
    findMinimalTreePath(skelGen, spqrPathNodes, spqrPathEdges, bnit1, bnit2);

    // Find the component nodes for each representative node along the path.
    EXPECT_EQ(spqrPathNodes.size(),1);
    EXPECT_EQ(*(spqrPathNodes.begin()),tn_01.backIt());
    EXPECT_EQ(
        isVirtualEdge(skelGen, *(spqrPathNodes.begin()), bnit1, bnit2), false);

    GMWSkeleton skel;
    setType(skel, GMWSkeleton::END1_NODE_END2_NODE);
    setBlockNit11(skel, bnit1);
    setBlockNit21(skel, bnit2);

    vector<node_list_it_t> blockNodes;
    for (auto bnit=B01.nodes().first; bnit!=B01.nodes().second; bnit++) {
        blockNodes.push_back(bnit);
    }
    setBlockNodes(skel, blockNodes);
    generateSkeleton(skel);

     // Check the skeleton
    Graph& sk = skel.graph();

    EXPECT_EQ(sk.numNodes(), 22);
    auto sknit = sk.nodes().first;
    auto& sn_01 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_01.IGBackwardLink(), bn_01.backIt());
    sknit++;
    auto& sn_02 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_02.IGBackwardLink(), bn_02.backIt());
    sknit++;
    auto& sn_03 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_03.IGBackwardLink(), bn_03.backIt());
    sknit++;
    auto& sn_04 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_04.IGBackwardLink(), bn_04.backIt());
    sknit++;
    auto& sn_05 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_05.IGBackwardLink(), bn_05.backIt());
    sknit++;
    auto& sn_06 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_06.IGBackwardLink(), bn_06.backIt());
    sknit++;
    auto& sn_07 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_07.IGBackwardLink(), bn_07.backIt());
    sknit++;
    auto& sn_08 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_08.IGBackwardLink(), bn_08.backIt());
    sknit++;
    auto& sn_09 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_09.IGBackwardLink(), bn_09.backIt());
    sknit++;
    auto& sn_10 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_10.IGBackwardLink(), bn_10.backIt());
    sknit++;
    auto& sn_11 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_11.IGBackwardLink(), bn_11.backIt());
    sknit++;
    auto& sn_12 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_12.IGBackwardLink(), bn_12.backIt());
    sknit++;
    auto& sn_13 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_13.IGBackwardLink(), bn_13.backIt());
    sknit++;
    auto& sn_14 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_14.IGBackwardLink(), bn_14.backIt());
    sknit++;
    auto& sn_15 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_15.IGBackwardLink(), bn_15.backIt());
    sknit++;
    auto& sn_16 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_16.IGBackwardLink(), bn_16.backIt());
    sknit++;
    auto& sn_17 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_17.IGBackwardLink(), bn_17.backIt());
    sknit++;
    auto& sn_18 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_18.IGBackwardLink(), bn_18.backIt());
    sknit++;
    auto& sn_19 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_19.IGBackwardLink(), bn_19.backIt());
    sknit++;
    auto& sn_20 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_20.IGBackwardLink(), bn_20.backIt());
    sknit++;
    auto& sn_21 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_21.IGBackwardLink(), bn_21.backIt());
    sknit++;
    auto& sn_22 = dynamic_cast<GMWSkeletonNode&>(*(*sknit));
    EXPECT_EQ(sn_22.IGBackwardLink(), bn_22.backIt());

    EXPECT_EQ(sk.numEdges(), 39);

    vector<edge_list_it_t> skelEdgesTested;
    vector<edge_list_it_t> virtualEdges;
    skelEdgesTested.push_back(be_01_02.backIt());
    skelEdgesTested.push_back(be_01_03.backIt());
    skelEdgesTested.push_back(be_01_07.backIt());
    skelEdgesTested.push_back(be_02_04.backIt());
    skelEdgesTested.push_back(be_02_08.backIt());
    skelEdgesTested.push_back(be_03_04.backIt());
    skelEdgesTested.push_back(be_03_05.backIt());
    skelEdgesTested.push_back(be_04_06.backIt());
    skelEdgesTested.push_back(be_05_06.backIt());
    skelEdgesTested.push_back(be_05_07.backIt());
    skelEdgesTested.push_back(be_06_08.backIt());
    skelEdgesTested.push_back(be_07_09.backIt());
    skelEdgesTested.push_back(be_08_15.backIt());
    skelEdgesTested.push_back(be_08_13.backIt());
    skelEdgesTested.push_back(be_13_14.backIt());
    skelEdgesTested.push_back(be_13_15.backIt());
    skelEdgesTested.push_back(be_15_16.backIt());
    skelEdgesTested.push_back(be_14_10.backIt());
    skelEdgesTested.push_back(be_14_16.backIt());
    skelEdgesTested.push_back(be_10_16.backIt());
    skelEdgesTested.push_back(be_09_10.backIt());
    skelEdgesTested.push_back(be_09_11.backIt());
    skelEdgesTested.push_back(be_10_11.backIt());
    skelEdgesTested.push_back(be_09_12.backIt());
    skelEdgesTested.push_back(be_11_12.backIt());
    skelEdgesTested.push_back(be_10_12.backIt());
    skelEdgesTested.push_back(be_10_17.backIt());
    skelEdgesTested.push_back(be_10_18.backIt());
    skelEdgesTested.push_back(be_12_17.backIt());
    skelEdgesTested.push_back(be_17_18.backIt());
    skelEdgesTested.push_back(be_12_18.backIt());
    skelEdgesTested.push_back(be_12_21.backIt());
    skelEdgesTested.push_back(be_12_19.backIt());
    skelEdgesTested.push_back(be_19_20.backIt());
    skelEdgesTested.push_back(be_19_21.backIt());
    skelEdgesTested.push_back(be_18_20.backIt());
    skelEdgesTested.push_back(be_18_22.backIt());
    skelEdgesTested.push_back(be_20_22.backIt());
    skelEdgesTested.push_back(be_21_22.backIt());

    EXPECT_EQ(checkSkelEdges(sk, skelEdgesTested, 0, virtualEdges), true);

    EXPECT_EQ(skel.mSkelNit1, sn_01.backIt());
    EXPECT_EQ(skel.mSkelNit2, sn_08.backIt());
    EXPECT_EQ(skel.mSkelEit1, sk.edges().second);
    EXPECT_EQ(skel.mSkelEit2, sk.edges().second);
    
}


/**  @brief Tests GMWSkeletonGenerator::processBlock() in steps.
 *                realistic.
 */
TEST_F(GMWSkeletonGeneratorTests, Test7) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    // Find the tree nodes for N1 and N2.
    node_list_it_t bnit1 = bn_03.backIt();
    node_list_it_t bnit2 = bn_19.backIt();

    node_list_it_t tnit1;
    node_list_it_t tnit2;

    findTreeNodesFromBlockNodes(skelGen, spqrTree, bnit1, bnit2, tnit1, tnit2);
    EXPECT_EQ(tnit1, tn_01.backIt());
    EXPECT_EQ(tnit2, tn_09.backIt());

    auto& TN1 = dynamic_cast<SPQRTreeNode&>(*(*tnit1));
    auto& TN2 = dynamic_cast<SPQRTreeNode&>(*(*tnit2));

    // Find a path from N1 to N2 on the tree.
    TreePathFinder finder;

    list<node_list_it_t> spqrPathNodes;
    list<edge_list_it_t> spqrPathEdges;
    finder.findPath(spqrTree, TN1, TN2, spqrPathNodes, spqrPathEdges);

    // Find the minimal path from N1 to N2 on the tree.
    findMinimalTreePath(skelGen, spqrPathNodes, spqrPathEdges, bnit1, bnit2);

    // Find the component nodes for each representative node along the path.
    EXPECT_EQ(spqrPathNodes.size(),8);
    EXPECT_EQ(spqrPathEdges.size(),7);


    // Prepare the tree edges for expansion.
    TreeSplitter splitter(spqrTree);
    splitter.prepareTree(spqrPathEdges);

    size_t index = 0;
    list<edge_list_it_t>::iterator eItIt = spqrPathEdges.begin();
    list<node_list_it_t>::iterator nItIt = spqrPathNodes.begin();

    // Tree Component 0

    auto& TN_0 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_0.type(),SPQRTreeNode::RType);

    GMWSkeleton skel_0;
    setType(skel_0,GMWSkeleton::END1_NODE_END2_EDGE);
    setBlockNit11(skel_0,bnit1);

    auto& TE2_0  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));
    edge_list_it_t ve2It;
    if (TE2_0.incidentNode1().backIt()== *nItIt) {
        ve2It = TE2_0.virtualEdge1();
    }
   else {
        ve2It = TE2_0.virtualEdge2();
    }
    EXPECT_EQ(ve2It,ce_01_07_08.backIt());
    auto& VE2_0 = dynamic_cast<Edge&>(*(*ve2It));
    auto& CN1_0 = dynamic_cast<SPQRComponentNode&>(VE2_0.incidentNode1());
    auto& CN2_0 = dynamic_cast<SPQRComponentNode&>(VE2_0.incidentNode2());
    setBlockNit21(skel_0, CN1_0.IGBackwardLink());
    setBlockNit22(skel_0, CN2_0.IGBackwardLink());

    list<node_list_it_t> treeNodes_0 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_0.size(), 1);
    vector<node_list_it_t> blockNodes_0 =
                           fromTreeNodesToSkeletonNodes(skelGen, treeNodes_0);

    vector<node_list_it_t> blockNodesExpected_0;
    blockNodesExpected_0.push_back(bn_01.backIt());
    blockNodesExpected_0.push_back(bn_02.backIt());
    blockNodesExpected_0.push_back(bn_03.backIt());
    blockNodesExpected_0.push_back(bn_04.backIt());
    blockNodesExpected_0.push_back(bn_05.backIt());
    blockNodesExpected_0.push_back(bn_06.backIt());
    blockNodesExpected_0.push_back(bn_07.backIt());
    blockNodesExpected_0.push_back(bn_08.backIt());
    EXPECT_EQ(checkNodes(blockNodes_0, blockNodesExpected_0), true);
    setBlockNodes(skel_0, blockNodes_0);

    generateSkeleton(skel_0);

    EXPECT_EQ(skel_0.graph().numNodes(), 8);
    EXPECT_EQ(skel_0.graph().numEdges(), 12);

    vector<node_list_it_t> skelNodes_0;

    EXPECT_EQ(
        checkSkelNodes(skel_0.graph(),blockNodesExpected_0,skelNodes_0),true);

    vector<edge_list_it_t> skelEdgesTested_0;
    vector<edge_list_it_t> virtualEdges_0;
    skelEdgesTested_0.push_back(be_01_02.backIt());
    skelEdgesTested_0.push_back(be_01_03.backIt());
    skelEdgesTested_0.push_back(be_01_07.backIt());
    skelEdgesTested_0.push_back(be_02_04.backIt());
    skelEdgesTested_0.push_back(be_02_08.backIt());
    skelEdgesTested_0.push_back(be_03_04.backIt());
    skelEdgesTested_0.push_back(be_03_05.backIt());
    skelEdgesTested_0.push_back(be_04_06.backIt());
    skelEdgesTested_0.push_back(be_05_06.backIt());
    skelEdgesTested_0.push_back(be_05_07.backIt());
    skelEdgesTested_0.push_back(be_06_08.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_0.graph(), skelEdgesTested_0, 1, virtualEdges_0), true);

    EXPECT_EQ(skel_0.mSkelNit1, skelNodes_0[2]);
    EXPECT_EQ(skel_0.mSkelNit2, skel_0.graph().nodes().second);

    EXPECT_EQ(skel_0.mSkelEit1, skel_0.graph().edges().second);
    EXPECT_EQ(skel_0.mSkelEit2, *(virtualEdges_0.begin()));

    auto& SVE2_0 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_0.mSkelEit2));
    EXPECT_EQ((SVE2_0.incidentNode1().backIt() == skelNodes_0[6] && 
               SVE2_0.incidentNode2().backIt() == skelNodes_0[7]    )||
              (SVE2_0.incidentNode1().backIt() == skelNodes_0[7] && 
               SVE2_0.incidentNode2().backIt() == skelNodes_0[6]    )  , true);

    index++;
    nItIt++;

    // Tree Component 1

    auto& TN_1 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_1.type(),SPQRTreeNode::SType);

    // Intermediate component on the path.
    GMWSkeleton skel_1;
    setType(skel_1, GMWSkeleton::END1_EDGE_END2_EDGE);

    auto& TE1_1  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    edge_list_it_t ve1It;
    if (TE1_1.incidentNode1().backIt()== *nItIt) {
        ve1It = TE1_1.virtualEdge1();
    }
    else {
        ve1It = TE1_1.virtualEdge2();
    }
    EXPECT_EQ(ve1It,ce_02_07_08.backIt());

    auto& VE1_1 = dynamic_cast<Edge&>(*(*ve1It));
    auto& CN1_1_1 = dynamic_cast<SPQRComponentNode&>(VE1_1.incidentNode1());
    auto& CN1_2_1 = dynamic_cast<SPQRComponentNode&>(VE1_1.incidentNode2());
                                                         
    setBlockNit11(skel_1, CN1_1_1.IGBackwardLink());
    setBlockNit12(skel_1, CN1_2_1.IGBackwardLink());

    eItIt++;
    auto& TE2_1  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    if (TE2_1.incidentNode1().backIt()== *nItIt) {
        ve2It = TE2_1.virtualEdge1();
    }
    else {
        ve2It = TE2_1.virtualEdge2();
    }
    EXPECT_EQ(ve2It,ce_02_09_10.backIt());

    auto& VE2_1 = dynamic_cast<Edge&>(*(*ve2It));
    auto& CN2_1_1 = dynamic_cast<SPQRComponentNode&>(VE2_1.incidentNode1());
    auto& CN2_2_1 = dynamic_cast<SPQRComponentNode&>(VE2_1.incidentNode2());
                                                         
    setBlockNit21(skel_1, CN2_1_1.IGBackwardLink());
    setBlockNit22(skel_1, CN2_2_1.IGBackwardLink());

    list<node_list_it_t> treeNodes_1 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_1.size(), 2);
    vector<node_list_it_t> blockNodes_1
                          = fromTreeNodesToSkeletonNodes(skelGen, treeNodes_1);

    vector<node_list_it_t> blockNodesExpected_1;
    blockNodesExpected_1.push_back(bn_07.backIt());
    blockNodesExpected_1.push_back(bn_08.backIt());
    blockNodesExpected_1.push_back(bn_09.backIt());
    blockNodesExpected_1.push_back(bn_10.backIt());
    blockNodesExpected_1.push_back(bn_13.backIt());
    blockNodesExpected_1.push_back(bn_14.backIt());
    blockNodesExpected_1.push_back(bn_15.backIt());
    blockNodesExpected_1.push_back(bn_16.backIt());
    EXPECT_EQ(checkNodes(blockNodes_1, blockNodesExpected_1), true);

    setBlockNodes(skel_1, blockNodes_1);
    generateSkeleton(skel_1);

    EXPECT_EQ(skel_1.graph().numNodes(), 8);
    EXPECT_EQ(skel_1.graph().numEdges(), 11);

    vector<node_list_it_t> skelNodes_1;
    EXPECT_EQ(
        checkSkelNodes(skel_1.graph(),blockNodesExpected_1,skelNodes_1),true);

    vector<edge_list_it_t> skelEdgesTested_1;
    vector<edge_list_it_t> virtualEdges_1;
    skelEdgesTested_1.push_back(be_07_09.backIt());
    skelEdgesTested_1.push_back(be_08_15.backIt());
    skelEdgesTested_1.push_back(be_08_13.backIt());
    skelEdgesTested_1.push_back(be_13_15.backIt());
    skelEdgesTested_1.push_back(be_13_14.backIt());
    skelEdgesTested_1.push_back(be_15_16.backIt());
    skelEdgesTested_1.push_back(be_14_10.backIt());
    skelEdgesTested_1.push_back(be_14_16.backIt());
    skelEdgesTested_1.push_back(be_10_16.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_1.graph(), skelEdgesTested_1, 2, virtualEdges_1), true);

    EXPECT_EQ(skel_1.mSkelNit1, skel_1.graph().nodes().second);
    EXPECT_EQ(skel_1.mSkelNit2, skel_1.graph().nodes().second);

    EXPECT_EQ(skel_1.mSkelEit1, *(virtualEdges_1.begin()));
    EXPECT_EQ(skel_1.mSkelEit2, *(virtualEdges_1.rbegin()));

    auto& SVE1_1 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_1.mSkelEit1));
    EXPECT_EQ((SVE1_1.incidentNode1().backIt() == skelNodes_1[0] && 
               SVE1_1.incidentNode2().backIt() == skelNodes_1[1]    )||
              (SVE1_1.incidentNode1().backIt() == skelNodes_1[1] && 
               SVE1_1.incidentNode2().backIt() == skelNodes_1[0]    )  , true);

    auto& SVE2_1 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_1.mSkelEit2));
    EXPECT_EQ((SVE2_1.incidentNode1().backIt() == skelNodes_1[2] && 
               SVE2_1.incidentNode2().backIt() == skelNodes_1[3]    )||
              (SVE2_1.incidentNode1().backIt() == skelNodes_1[3] && 
               SVE2_1.incidentNode2().backIt() == skelNodes_1[2]    )  , true);



    index++;
    nItIt++;

    // Tree Component 2

    auto& TN_2 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_2.type(),SPQRTreeNode::PType);
    eItIt++;

    index++;
    nItIt++;

    // Tree Component 3

    auto& TN_3 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_3.type(),SPQRTreeNode::RType);

    // Intermediate component on the path.
    GMWSkeleton skel_3;
    setType(skel_3, GMWSkeleton::END1_EDGE_END2_EDGE);

    auto& TE1_3  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    if (TE1_3.incidentNode1().backIt()== *nItIt) {
        ve1It = TE1_3.virtualEdge1();
    }
    else {
        ve1It = TE1_3.virtualEdge2();
    }
    EXPECT_EQ(ve1It,ce_05_09_10.backIt());

    auto& VE1_3 = dynamic_cast<Edge&>(*(*ve1It));
    auto& CN1_1_3 = dynamic_cast<SPQRComponentNode&>(VE1_3.incidentNode1());
    auto& CN1_2_3 = dynamic_cast<SPQRComponentNode&>(VE1_3.incidentNode2());
                                                         
    setBlockNit11(skel_3, CN1_1_3.IGBackwardLink());
    setBlockNit12(skel_3, CN1_2_3.IGBackwardLink());

    eItIt++;
    auto& TE2_3  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    if (TE2_3.incidentNode1().backIt()== *nItIt) {
        ve2It = TE2_3.virtualEdge1();
    }
    else {
        ve2It = TE2_3.virtualEdge2();
    }
    EXPECT_EQ(ve2It,ce_05_10_12.backIt());

    auto& VE2_3 = dynamic_cast<Edge&>(*(*ve2It));
    auto& CN2_1_3 = dynamic_cast<SPQRComponentNode&>(VE2_3.incidentNode1());
    auto& CN2_2_3 = dynamic_cast<SPQRComponentNode&>(VE2_3.incidentNode2());
                                                         
    setBlockNit21(skel_3, CN2_1_3.IGBackwardLink());
    setBlockNit22(skel_3, CN2_2_3.IGBackwardLink());

    list<node_list_it_t> treeNodes_3 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_3.size(), 1);
    vector<node_list_it_t> blockNodes_3
                          = fromTreeNodesToSkeletonNodes(skelGen, treeNodes_3);

    vector<node_list_it_t> blockNodesExpected_3;
    blockNodesExpected_3.push_back(bn_09.backIt());
    blockNodesExpected_3.push_back(bn_10.backIt());
    blockNodesExpected_3.push_back(bn_11.backIt());
    blockNodesExpected_3.push_back(bn_12.backIt());
    EXPECT_EQ(checkNodes(blockNodes_3, blockNodesExpected_3), true);

    setBlockNodes(skel_3, blockNodes_3);
    generateSkeleton(skel_3);

    EXPECT_EQ(skel_3.graph().numNodes(), 4);
    EXPECT_EQ(skel_3.graph().numEdges(), 6);

    vector<node_list_it_t> skelNodes_3;
    EXPECT_EQ(
        checkSkelNodes(skel_3.graph(),blockNodesExpected_3,skelNodes_3),true);

    vector<edge_list_it_t> skelEdgesTested_3;
    vector<edge_list_it_t> virtualEdges_3;
    skelEdgesTested_3.push_back(be_09_11.backIt());
    skelEdgesTested_3.push_back(be_10_11.backIt());
    skelEdgesTested_3.push_back(be_09_12.backIt());
    skelEdgesTested_3.push_back(be_11_12.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_3.graph(), skelEdgesTested_3, 2, virtualEdges_3), true);

    EXPECT_EQ(skel_3.mSkelNit1, skel_3.graph().nodes().second);
    EXPECT_EQ(skel_3.mSkelNit2, skel_3.graph().nodes().second);

    EXPECT_EQ(skel_3.mSkelEit1, *(virtualEdges_3.begin()));
    EXPECT_EQ(skel_3.mSkelEit2, *(virtualEdges_3.rbegin()));

    auto& SVE1_3 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_3.mSkelEit1));
    EXPECT_EQ((SVE1_3.incidentNode1().backIt() == skelNodes_3[0] && 
               SVE1_3.incidentNode2().backIt() == skelNodes_3[1]    )||
              (SVE1_3.incidentNode1().backIt() == skelNodes_3[1] && 
               SVE1_3.incidentNode2().backIt() == skelNodes_3[0]    )  , true);

    auto& SVE2_3 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_3.mSkelEit2));
    EXPECT_EQ((SVE2_3.incidentNode1().backIt() == skelNodes_3[1] && 
               SVE2_3.incidentNode2().backIt() == skelNodes_3[3]    )||
              (SVE2_3.incidentNode1().backIt() == skelNodes_3[3] && 
               SVE2_3.incidentNode2().backIt() == skelNodes_3[1]    )  , true);



    index++;
    nItIt++;

    // Tree Component 4

    auto& TN_4 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_4.type(),SPQRTreeNode::PType);
    eItIt++;

    index++;
    nItIt++;


    // Tree Component 5

    auto& TN_5 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_5.type(),SPQRTreeNode::RType);

    // Intermediate component on the path.
    GMWSkeleton skel_5;
    setType(skel_5, GMWSkeleton::END1_EDGE_END2_EDGE);

    auto& TE1_5  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    if (TE1_5.incidentNode1().backIt()== *nItIt) {
        ve1It = TE1_5.virtualEdge1();
    }
    else {
        ve1It = TE1_5.virtualEdge2();
    }
    EXPECT_EQ(ve1It,ce_07_10_12.backIt());

    auto& VE1_5 = dynamic_cast<Edge&>(*(*ve1It));
    auto& CN1_1_5 = dynamic_cast<SPQRComponentNode&>(VE1_5.incidentNode1());
    auto& CN1_2_5 = dynamic_cast<SPQRComponentNode&>(VE1_5.incidentNode2());
                                                         
    setBlockNit11(skel_5, CN1_1_5.IGBackwardLink());
    setBlockNit12(skel_5, CN1_2_5.IGBackwardLink());

    eItIt++;
    auto& TE2_5  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    if (TE2_5.incidentNode1().backIt()== *nItIt) {
        ve2It = TE2_5.virtualEdge1();
    }
    else {
        ve2It = TE2_5.virtualEdge2();
    }
    EXPECT_EQ(ve2It,ce_07_12_18.backIt());

    auto& VE2_5 = dynamic_cast<Edge&>(*(*ve2It));
    auto& CN2_1_5 = dynamic_cast<SPQRComponentNode&>(VE2_5.incidentNode1());
    auto& CN2_2_5 = dynamic_cast<SPQRComponentNode&>(VE2_5.incidentNode2());
                                                         
    setBlockNit21(skel_5, CN2_1_5.IGBackwardLink());
    setBlockNit22(skel_5, CN2_2_5.IGBackwardLink());

    list<node_list_it_t> treeNodes_5 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_5.size(), 1);
    vector<node_list_it_t> blockNodes_5
                          = fromTreeNodesToSkeletonNodes(skelGen, treeNodes_5);

    vector<node_list_it_t> blockNodesExpected_5;
    blockNodesExpected_5.push_back(bn_10.backIt());
    blockNodesExpected_5.push_back(bn_12.backIt());
    blockNodesExpected_5.push_back(bn_17.backIt());
    blockNodesExpected_5.push_back(bn_18.backIt());
    EXPECT_EQ(checkNodes(blockNodes_5, blockNodesExpected_5), true);

    setBlockNodes(skel_5, blockNodes_5);
    generateSkeleton(skel_5);

    EXPECT_EQ(skel_5.graph().numNodes(), 4);
    EXPECT_EQ(skel_5.graph().numEdges(), 6);

    vector<node_list_it_t> skelNodes_5;
    EXPECT_EQ(
        checkSkelNodes(skel_5.graph(),blockNodesExpected_5,skelNodes_5),true);

    vector<edge_list_it_t> skelEdgesTested_5;
    vector<edge_list_it_t> virtualEdges_5;
    skelEdgesTested_5.push_back(be_10_17.backIt());
    skelEdgesTested_5.push_back(be_10_18.backIt());
    skelEdgesTested_5.push_back(be_12_17.backIt());
    skelEdgesTested_5.push_back(be_17_18.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_5.graph(), skelEdgesTested_5, 2, virtualEdges_5), true);

    EXPECT_EQ(skel_5.mSkelNit1, skel_5.graph().nodes().second);
    EXPECT_EQ(skel_5.mSkelNit2, skel_5.graph().nodes().second);

    EXPECT_EQ(skel_5.mSkelEit1, *(virtualEdges_5.begin()));
    EXPECT_EQ(skel_5.mSkelEit2, *(virtualEdges_5.rbegin()));

    auto& SVE1_5 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_5.mSkelEit1));
    EXPECT_EQ((SVE1_5.incidentNode1().backIt() == skelNodes_5[0] && 
               SVE1_5.incidentNode2().backIt() == skelNodes_5[1]    )||
              (SVE1_5.incidentNode1().backIt() == skelNodes_5[1] && 
               SVE1_5.incidentNode2().backIt() == skelNodes_5[0]    )  , true);

    auto& SVE2_5 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_5.mSkelEit2));
    EXPECT_EQ((SVE2_5.incidentNode1().backIt() == skelNodes_5[1] && 
               SVE2_5.incidentNode2().backIt() == skelNodes_5[3]    )||
              (SVE2_5.incidentNode1().backIt() == skelNodes_5[3] && 
               SVE2_5.incidentNode2().backIt() == skelNodes_5[1]    )  , true);



    index++;
    nItIt++;

    // Tree Component 6

    auto& TN_6 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_6.type(),SPQRTreeNode::PType);
    eItIt++;

    index++;
    nItIt++;

    // Tree Component 7

    auto& TN_7 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_7.type(),SPQRTreeNode::RType);


    GMWSkeleton skel_7;
    setType(skel_7, GMWSkeleton::END1_EDGE_END2_NODE);

    auto& TE1_7  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    if (TE1_7.incidentNode1().backIt()== *nItIt) {
        ve1It = TE1_7.virtualEdge1();
    }
    else {
        ve1It = TE1_7.virtualEdge2();
    }
    EXPECT_EQ(ve1It,ce_09_12_18.backIt());

    auto& VE1_7 = dynamic_cast<Edge&>(*(*ve1It));
    auto& CN1_1_7 = dynamic_cast<SPQRComponentNode&>(VE1_7.incidentNode1());
    auto& CN1_2_7 = dynamic_cast<SPQRComponentNode&>(VE1_7.incidentNode2());
                                                         
    setBlockNit11(skel_7, CN1_1_7.IGBackwardLink());
    setBlockNit12(skel_7, CN1_2_7.IGBackwardLink());
    setBlockNit21(skel_7, bnit2);

    list<node_list_it_t> treeNodes_7 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_7.size(), 1);
    vector<node_list_it_t> blockNodes_7
                          = fromTreeNodesToSkeletonNodes(skelGen, treeNodes_7);

    setBlockNodes(skel_7, blockNodes_7);
    generateSkeleton(skel_7);

    vector<node_list_it_t> blockNodesExpected_7;
    blockNodesExpected_7.push_back(bn_12.backIt());
    blockNodesExpected_7.push_back(bn_18.backIt());
    blockNodesExpected_7.push_back(bn_19.backIt());
    blockNodesExpected_7.push_back(bn_20.backIt());
    blockNodesExpected_7.push_back(bn_21.backIt());
    blockNodesExpected_7.push_back(bn_22.backIt());
    EXPECT_EQ(checkNodes(blockNodes_7, blockNodesExpected_7), true);

    EXPECT_EQ(skel_7.graph().numNodes(), 6);
    EXPECT_EQ(skel_7.graph().numEdges(), 9);

    vector<node_list_it_t> skelNodes_7;
    EXPECT_EQ(
        checkSkelNodes(skel_7.graph(),blockNodesExpected_7,skelNodes_7),true);

    vector<edge_list_it_t> skelEdgesTested_7;
    vector<edge_list_it_t> virtualEdges_7;
    skelEdgesTested_7.push_back(be_12_21.backIt());
    skelEdgesTested_7.push_back(be_12_19.backIt());
    skelEdgesTested_7.push_back(be_19_21.backIt());
    skelEdgesTested_7.push_back(be_19_20.backIt());
    skelEdgesTested_7.push_back(be_18_20.backIt());
    skelEdgesTested_7.push_back(be_18_22.backIt());
    skelEdgesTested_7.push_back(be_20_22.backIt());
    skelEdgesTested_7.push_back(be_21_22.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_7.graph(), skelEdgesTested_7, 1, virtualEdges_7), true);

    EXPECT_EQ(skel_7.mSkelNit1, skel_7.graph().nodes().second);
    EXPECT_EQ(skel_7.mSkelNit2, skelNodes_7[2]);

    EXPECT_EQ(skel_7.mSkelEit1, *(virtualEdges_7.begin()));
    EXPECT_EQ(skel_7.mSkelEit2, skel_7.graph().edges().second);

    auto& SVE1_7 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_7.mSkelEit1));
    EXPECT_EQ((SVE1_7.incidentNode1().backIt() == skelNodes_7[0] && 
               SVE1_7.incidentNode2().backIt() == skelNodes_7[1]    )||
              (SVE1_7.incidentNode1().backIt() == skelNodes_7[1] && 
               SVE1_7.incidentNode2().backIt() == skelNodes_7[0]    )  , true);

    index++;
    nItIt++;

}


/**  @brief Tests GMWSkeletonGenerator::processBlock() in steps.
 *                realistic 2.
 */
TEST_F(GMWSkeletonGeneratorTests, Test8) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    // Find the tree nodes for N1 and N2.
    node_list_it_t bnit1 = bn_11.backIt();
    node_list_it_t bnit2 = bn_13.backIt();

    node_list_it_t tnit1;
    node_list_it_t tnit2;

    findTreeNodesFromBlockNodes(skelGen, spqrTree, bnit1, bnit2, tnit1, tnit2);
    EXPECT_EQ(tnit1, tn_05.backIt());
    EXPECT_EQ(tnit2, tn_03.backIt());

    auto& TN1 = dynamic_cast<SPQRTreeNode&>(*(*tnit1));
    auto& TN2 = dynamic_cast<SPQRTreeNode&>(*(*tnit2));

    // Find a path from N1 to N2 on the tree.
    TreePathFinder finder;

    list<node_list_it_t> spqrPathNodes;
    list<edge_list_it_t> spqrPathEdges;
    finder.findPath(spqrTree, TN1, TN2, spqrPathNodes, spqrPathEdges);

    // Find the minimal path from N1 to N2 on the tree.
    findMinimalTreePath(skelGen, spqrPathNodes, spqrPathEdges, bnit1, bnit2);

    // Find the component nodes for each representative node along the path.
    EXPECT_EQ(spqrPathNodes.size(),4);
    EXPECT_EQ(spqrPathEdges.size(),3);


    // Prepare the tree edges for expansion.
    TreeSplitter splitter(spqrTree);
    splitter.prepareTree(spqrPathEdges);

    size_t index = 0;
    list<edge_list_it_t>::iterator eItIt = spqrPathEdges.begin();
    list<node_list_it_t>::iterator nItIt = spqrPathNodes.begin();

    // Tree Component 0

    auto& TN_0 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_0.type(),SPQRTreeNode::RType);

    GMWSkeleton skel_0;
    setType(skel_0,GMWSkeleton::END1_NODE_END2_EDGE);
    setBlockNit11(skel_0,bnit1);

    auto& TE2_0  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));
    edge_list_it_t ve2It;
    if (TE2_0.incidentNode1().backIt()== *nItIt) {
        ve2It = TE2_0.virtualEdge1();
    }
   else {
        ve2It = TE2_0.virtualEdge2();
    }
    EXPECT_EQ(ve2It,ce_05_09_10.backIt());
    auto& VE2_0 = dynamic_cast<Edge&>(*(*ve2It));
    auto& CN1_0 = dynamic_cast<SPQRComponentNode&>(VE2_0.incidentNode1());
    auto& CN2_0 = dynamic_cast<SPQRComponentNode&>(VE2_0.incidentNode2());
    setBlockNit21(skel_0, CN1_0.IGBackwardLink());
    setBlockNit22(skel_0, CN2_0.IGBackwardLink());

    list<node_list_it_t> treeNodes_0 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_0.size(), 5);
    vector<node_list_it_t> blockNodes_0 =
                           fromTreeNodesToSkeletonNodes(skelGen, treeNodes_0);

    vector<node_list_it_t> blockNodesExpected_0;
    blockNodesExpected_0.push_back(bn_09.backIt());
    blockNodesExpected_0.push_back(bn_10.backIt());
    blockNodesExpected_0.push_back(bn_11.backIt());
    blockNodesExpected_0.push_back(bn_12.backIt());
    blockNodesExpected_0.push_back(bn_17.backIt());
    blockNodesExpected_0.push_back(bn_18.backIt());
    blockNodesExpected_0.push_back(bn_19.backIt());
    blockNodesExpected_0.push_back(bn_20.backIt());
    blockNodesExpected_0.push_back(bn_21.backIt());
    blockNodesExpected_0.push_back(bn_22.backIt());
    EXPECT_EQ(checkNodes(blockNodes_0, blockNodesExpected_0), true);
    setBlockNodes(skel_0, blockNodes_0);

    generateSkeleton(skel_0);

    EXPECT_EQ(skel_0.graph().numNodes(), 10);
    EXPECT_EQ(skel_0.graph().numEdges(), 19);

    vector<node_list_it_t> skelNodes_0;

    EXPECT_EQ(
     checkSkelNodes(skel_0.graph(), blockNodesExpected_0, skelNodes_0), true);

    vector<edge_list_it_t> skelEdgesTested_0;
    vector<edge_list_it_t> virtualEdges_0;
    skelEdgesTested_0.push_back(be_09_11.backIt());
    skelEdgesTested_0.push_back(be_10_11.backIt());
    skelEdgesTested_0.push_back(be_09_12.backIt());
    skelEdgesTested_0.push_back(be_10_12.backIt());
    skelEdgesTested_0.push_back(be_11_12.backIt());
    skelEdgesTested_0.push_back(be_10_18.backIt());
    skelEdgesTested_0.push_back(be_10_17.backIt());
    skelEdgesTested_0.push_back(be_12_17.backIt());
    skelEdgesTested_0.push_back(be_12_18.backIt());
    skelEdgesTested_0.push_back(be_17_18.backIt());
    skelEdgesTested_0.push_back(be_12_21.backIt());
    skelEdgesTested_0.push_back(be_12_19.backIt());
    skelEdgesTested_0.push_back(be_18_22.backIt());
    skelEdgesTested_0.push_back(be_18_20.backIt());
    skelEdgesTested_0.push_back(be_19_20.backIt());
    skelEdgesTested_0.push_back(be_19_21.backIt());
    skelEdgesTested_0.push_back(be_20_22.backIt());
    skelEdgesTested_0.push_back(be_21_22.backIt());

    EXPECT_EQ(checkSkelEdges(
                 skel_0.graph(), skelEdgesTested_0, 1, virtualEdges_0), true);

    EXPECT_EQ(skel_0.mSkelNit1, skelNodes_0[2]);
    EXPECT_EQ(skel_0.mSkelNit2, skel_0.graph().nodes().second);

    EXPECT_EQ(skel_0.mSkelEit1, skel_0.graph().edges().second);
    EXPECT_EQ(skel_0.mSkelEit2, *(virtualEdges_0.begin()));

    auto& SVE2_0 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_0.mSkelEit2));
    EXPECT_EQ((SVE2_0.incidentNode1().backIt() == skelNodes_0[0] && 
               SVE2_0.incidentNode2().backIt() == skelNodes_0[1]    )||
              (SVE2_0.incidentNode1().backIt() == skelNodes_0[1] && 
               SVE2_0.incidentNode2().backIt() == skelNodes_0[0]    )  , true);

    index++;
    nItIt++;

    // Tree Component 1

    auto& TN_1 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_1.type(),SPQRTreeNode::PType);
    eItIt++;

    index++;
    nItIt++;

    // Tree Component 2

    auto& TN_2 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_2.type(),SPQRTreeNode::SType);
    eItIt++;

    index++;
    nItIt++;

    // Tree Component 3

    auto& TN_3 = dynamic_cast<SPQRTreeNode&>(*(*(*nItIt)));

    EXPECT_EQ(TN_3.type(),SPQRTreeNode::RType);


    GMWSkeleton skel_3;
    setType(skel_3, GMWSkeleton::END1_EDGE_END2_NODE);

    auto& TE1_3  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));

    edge_list_it_t ve1It;
    if (TE1_3.incidentNode1().backIt()== *nItIt) {
        ve1It = TE1_3.virtualEdge1();
    }
    else {
        ve1It = TE1_3.virtualEdge2();
    }
    EXPECT_EQ(ve1It,ce_03_08_10.backIt());

    auto& VE1_3 = dynamic_cast<Edge&>(*(*ve1It));
    auto& CN1_1_3 = dynamic_cast<SPQRComponentNode&>(VE1_3.incidentNode1());
    auto& CN1_2_3 = dynamic_cast<SPQRComponentNode&>(VE1_3.incidentNode2());
                                                         
    setBlockNit11(skel_3, CN1_1_3.IGBackwardLink());
    setBlockNit12(skel_3, CN1_2_3.IGBackwardLink());
    setBlockNit21(skel_3, bnit2);

    list<node_list_it_t> treeNodes_3 = splitter.findComponent(*nItIt);
    EXPECT_EQ(treeNodes_3.size(), 1);
    vector<node_list_it_t> blockNodes_3
                          = fromTreeNodesToSkeletonNodes(skelGen, treeNodes_3);

    setBlockNodes(skel_3, blockNodes_3);
    generateSkeleton(skel_3);

    vector<node_list_it_t> blockNodesExpected_3;
    blockNodesExpected_3.push_back(bn_08.backIt());
    blockNodesExpected_3.push_back(bn_10.backIt());
    blockNodesExpected_3.push_back(bn_13.backIt());
    blockNodesExpected_3.push_back(bn_14.backIt());
    blockNodesExpected_3.push_back(bn_15.backIt());
    blockNodesExpected_3.push_back(bn_16.backIt());
    EXPECT_EQ(checkNodes(blockNodes_3, blockNodesExpected_3), true);

    EXPECT_EQ(skel_3.graph().numNodes(), 6);
    EXPECT_EQ(skel_3.graph().numEdges(), 9);

    vector<node_list_it_t> skelNodes_3;
    EXPECT_EQ(
        checkSkelNodes(skel_3.graph(),blockNodesExpected_3,skelNodes_3),true);

    vector<edge_list_it_t> skelEdgesTested_3;
    vector<edge_list_it_t> virtualEdges_3;
    skelEdgesTested_3.push_back(be_08_15.backIt());
    skelEdgesTested_3.push_back(be_08_13.backIt());
    skelEdgesTested_3.push_back(be_13_15.backIt());
    skelEdgesTested_3.push_back(be_13_14.backIt());
    skelEdgesTested_3.push_back(be_14_10.backIt());
    skelEdgesTested_3.push_back(be_14_16.backIt());
    skelEdgesTested_3.push_back(be_15_16.backIt());
    skelEdgesTested_3.push_back(be_10_16.backIt());

    EXPECT_EQ(checkSkelEdges(
                 skel_3.graph(), skelEdgesTested_3, 1, virtualEdges_3), true);

    EXPECT_EQ(skel_3.mSkelNit1, skel_3.graph().nodes().second);
    EXPECT_EQ(skel_3.mSkelNit2, skelNodes_3[2]);

    EXPECT_EQ(skel_3.mSkelEit1, *(virtualEdges_3.begin()));
    EXPECT_EQ(skel_3.mSkelEit2, skel_3.graph().edges().second);

    auto& SVE1_3 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_3.mSkelEit1));
    EXPECT_EQ((SVE1_3.incidentNode1().backIt() == skelNodes_3[0] && 
               SVE1_3.incidentNode2().backIt() == skelNodes_3[1]    )||
              (SVE1_3.incidentNode1().backIt() == skelNodes_3[1] && 
               SVE1_3.incidentNode2().backIt() == skelNodes_3[0]    )  , true);

    index++;
    nItIt++;

}


/**  @brief Tests GMWSkeletonGenerator::processBlock()
 *                realistic.
 */
TEST_F(GMWSkeletonGeneratorTests, Test9) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    node_list_it_t bnit1 = bn_03.backIt();
    node_list_it_t bnit2 = bn_19.backIt();

    skelGen.processBlock(B01, bnit1, bnit2);

    EXPECT_EQ(skelGen.numSkeletons(), 4);

    auto& skel_0 = skelGen.skeleton(0);

    vector<node_list_it_t> blockNodesExpected_0;
    blockNodesExpected_0.push_back(bn_01.backIt());
    blockNodesExpected_0.push_back(bn_02.backIt());
    blockNodesExpected_0.push_back(bn_03.backIt());
    blockNodesExpected_0.push_back(bn_04.backIt());
    blockNodesExpected_0.push_back(bn_05.backIt());
    blockNodesExpected_0.push_back(bn_06.backIt());
    blockNodesExpected_0.push_back(bn_07.backIt());
    blockNodesExpected_0.push_back(bn_08.backIt());

    EXPECT_EQ(skel_0.graph().numNodes(), 8);
    EXPECT_EQ(skel_0.graph().numEdges(), 12);

    vector<node_list_it_t> skelNodes_0;

    EXPECT_EQ(
     checkSkelNodes(skel_0.graph(), blockNodesExpected_0, skelNodes_0), true);

    vector<edge_list_it_t> skelEdgesTested_0;
    vector<edge_list_it_t> virtualEdges_0;
    skelEdgesTested_0.push_back(be_01_02.backIt());
    skelEdgesTested_0.push_back(be_01_03.backIt());
    skelEdgesTested_0.push_back(be_01_07.backIt());
    skelEdgesTested_0.push_back(be_02_04.backIt());
    skelEdgesTested_0.push_back(be_02_08.backIt());
    skelEdgesTested_0.push_back(be_03_04.backIt());
    skelEdgesTested_0.push_back(be_03_05.backIt());
    skelEdgesTested_0.push_back(be_04_06.backIt());
    skelEdgesTested_0.push_back(be_05_06.backIt());
    skelEdgesTested_0.push_back(be_05_07.backIt());
    skelEdgesTested_0.push_back(be_06_08.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_0.graph(), skelEdgesTested_0, 1, virtualEdges_0), true);
    EXPECT_EQ(skelNodes_0.size(),8);
    EXPECT_EQ(skel_0.mSkelNit1, skelNodes_0[2]);
    EXPECT_EQ(skel_0.mSkelNit2, skel_0.graph().nodes().second);
    EXPECT_EQ(skel_0.mSkelEit1, skel_0.graph().edges().second);
    EXPECT_EQ(skel_0.mSkelEit2, *(virtualEdges_0.begin()));

    auto& SVE2_0 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_0.mSkelEit2));
    EXPECT_EQ((SVE2_0.incidentNode1().backIt() == skelNodes_0[6] && 
               SVE2_0.incidentNode2().backIt() == skelNodes_0[7]    )||
              (SVE2_0.incidentNode1().backIt() == skelNodes_0[7] && 
               SVE2_0.incidentNode2().backIt() == skelNodes_0[6]    )  , true);


    auto& skel_1 = skelGen.skeleton(1);

    vector<node_list_it_t> blockNodesExpected_1;
    blockNodesExpected_1.push_back(bn_09.backIt());
    blockNodesExpected_1.push_back(bn_10.backIt());
    blockNodesExpected_1.push_back(bn_11.backIt());
    blockNodesExpected_1.push_back(bn_12.backIt());

    EXPECT_EQ(skel_1.graph().numNodes(), 4);
    EXPECT_EQ(skel_1.graph().numEdges(), 6);

    vector<node_list_it_t> skelNodes_1;

    EXPECT_EQ(
     checkSkelNodes(skel_1.graph(), blockNodesExpected_1, skelNodes_1), true);

    vector<edge_list_it_t> skelEdgesTested_1;
    vector<edge_list_it_t> virtualEdges_1;
    skelEdgesTested_1.push_back(be_09_11.backIt());
    skelEdgesTested_1.push_back(be_09_12.backIt());
    skelEdgesTested_1.push_back(be_10_11.backIt());
    skelEdgesTested_1.push_back(be_11_12.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_1.graph(), skelEdgesTested_1, 2, virtualEdges_1), true);
    EXPECT_EQ(skelNodes_1.size(),4);
    EXPECT_EQ(skel_1.mSkelNit1, skel_1.graph().nodes().second);
    EXPECT_EQ(skel_1.mSkelNit2, skel_1.graph().nodes().second);
    EXPECT_EQ(skel_1.mSkelEit1, *(virtualEdges_1.begin()));
    EXPECT_EQ(skel_1.mSkelEit2, *(virtualEdges_1.rbegin()));

    auto& SVE1_1 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_1.mSkelEit1));
    EXPECT_EQ((SVE1_1.incidentNode1().backIt() == skelNodes_1[0] && 
               SVE1_1.incidentNode2().backIt() == skelNodes_1[1]    )||
              (SVE1_1.incidentNode1().backIt() == skelNodes_1[1] && 
               SVE1_1.incidentNode2().backIt() == skelNodes_1[0]    )  , true);

    auto& SVE2_1 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_1.mSkelEit2));
    EXPECT_EQ((SVE2_1.incidentNode1().backIt() == skelNodes_1[1] && 
               SVE2_1.incidentNode2().backIt() == skelNodes_1[3]    )||
              (SVE2_1.incidentNode1().backIt() == skelNodes_1[3] && 
               SVE2_1.incidentNode2().backIt() == skelNodes_1[1]    )  , true);

    auto& skel_2 = skelGen.skeleton(2);

    vector<node_list_it_t> blockNodesExpected_2;
    blockNodesExpected_2.push_back(bn_10.backIt());
    blockNodesExpected_2.push_back(bn_12.backIt());
    blockNodesExpected_2.push_back(bn_17.backIt());
    blockNodesExpected_2.push_back(bn_18.backIt());

    EXPECT_EQ(skel_2.graph().numNodes(), 4);
    EXPECT_EQ(skel_2.graph().numEdges(), 6);

    vector<node_list_it_t> skelNodes_2;

    EXPECT_EQ(
     checkSkelNodes(skel_2.graph(), blockNodesExpected_2, skelNodes_2), true);

    vector<edge_list_it_t> skelEdgesTested_2;
    vector<edge_list_it_t> virtualEdges_2;
    skelEdgesTested_2.push_back(be_10_17.backIt());
    skelEdgesTested_2.push_back(be_12_17.backIt());
    skelEdgesTested_2.push_back(be_10_18.backIt());
    skelEdgesTested_2.push_back(be_17_18.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_2.graph(), skelEdgesTested_2, 2, virtualEdges_2), true);
    EXPECT_EQ(skelNodes_2.size(),4);
    EXPECT_EQ(skel_2.mSkelNit1, skel_2.graph().nodes().second);
    EXPECT_EQ(skel_2.mSkelNit2, skel_2.graph().nodes().second);
    EXPECT_EQ(skel_2.mSkelEit1, *(virtualEdges_2.begin()));
    EXPECT_EQ(skel_2.mSkelEit2, *(virtualEdges_2.rbegin()));

    auto& SVE1_2 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_2.mSkelEit1));
    EXPECT_EQ((SVE1_2.incidentNode1().backIt() == skelNodes_2[0] && 
               SVE1_2.incidentNode2().backIt() == skelNodes_2[1]    )||
              (SVE1_2.incidentNode1().backIt() == skelNodes_2[1] && 
               SVE1_2.incidentNode2().backIt() == skelNodes_2[0]    )  , true);

    auto& SVE2_2 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_2.mSkelEit2));
    EXPECT_EQ((SVE2_2.incidentNode1().backIt() == skelNodes_2[1] && 
               SVE2_2.incidentNode2().backIt() == skelNodes_2[3]    )||
              (SVE2_2.incidentNode1().backIt() == skelNodes_2[3] && 
               SVE2_2.incidentNode2().backIt() == skelNodes_2[1]    )  , true);

    auto& skel_3 = skelGen.skeleton(3);

    vector<node_list_it_t> blockNodesExpected_3;
    blockNodesExpected_3.push_back(bn_12.backIt());
    blockNodesExpected_3.push_back(bn_18.backIt());
    blockNodesExpected_3.push_back(bn_19.backIt());
    blockNodesExpected_3.push_back(bn_20.backIt());
    blockNodesExpected_3.push_back(bn_21.backIt());
    blockNodesExpected_3.push_back(bn_22.backIt());

    EXPECT_EQ(skel_3.graph().numNodes(), 6);
    EXPECT_EQ(skel_3.graph().numEdges(), 9);

    vector<node_list_it_t> skelNodes_3;

    EXPECT_EQ(
     checkSkelNodes(skel_3.graph(), blockNodesExpected_3, skelNodes_3), true);

    vector<edge_list_it_t> skelEdgesTested_3;
    vector<edge_list_it_t> virtualEdges_3;
    skelEdgesTested_3.push_back(be_12_21.backIt());
    skelEdgesTested_3.push_back(be_12_19.backIt());
    skelEdgesTested_3.push_back(be_19_21.backIt());
    skelEdgesTested_3.push_back(be_19_20.backIt());
    skelEdgesTested_3.push_back(be_21_22.backIt());
    skelEdgesTested_3.push_back(be_18_20.backIt());
    skelEdgesTested_3.push_back(be_18_22.backIt());
    skelEdgesTested_3.push_back(be_20_22.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_3.graph(), skelEdgesTested_3, 1, virtualEdges_3), true);

    EXPECT_EQ(skelNodes_3.size(),6);

    EXPECT_EQ(skel_3.mSkelNit1, skel_3.graph().nodes().second);

    EXPECT_EQ(skel_3.mSkelNit2, skelNodes_3[2]);

    EXPECT_EQ(skel_3.mSkelEit1, *(virtualEdges_3.rbegin()));

    EXPECT_EQ(skel_3.mSkelEit2, skel_3.graph().edges().second);

    auto& SVE1_3 = dynamic_cast<GMWSkeletonEdge&>(*(*skel_3.mSkelEit1));

    EXPECT_EQ((SVE1_3.incidentNode1().backIt() == skelNodes_3[0] && 
               SVE1_3.incidentNode2().backIt() == skelNodes_3[1]    )||
              (SVE1_3.incidentNode1().backIt() == skelNodes_3[1] && 
               SVE1_3.incidentNode2().backIt() == skelNodes_3[0]    )  , true);


}



/**  @brief Tests GMWSkeletonGenerator::processBlock()
 *                one node
 */
TEST_F(GMWSkeletonGeneratorTests, Test10) {

    #include "unit_tests/undirected/unit_tests_gmw_skeleton_generator_gen_tree_inc.inc"

    GMWSkeletonGenerator skelGen;

    node_list_it_t bnit1 = bn_13.backIt();
    node_list_it_t bnit2 = bn_14.backIt();

    skelGen.processBlock(B01, bnit1, bnit2);

    EXPECT_EQ(skelGen.numSkeletons(), 1);

    auto& skel_0 = skelGen.skeleton(0);

    vector<node_list_it_t> blockNodesExpected_0;
    blockNodesExpected_0.push_back(bn_01.backIt());
    blockNodesExpected_0.push_back(bn_02.backIt());
    blockNodesExpected_0.push_back(bn_03.backIt());
    blockNodesExpected_0.push_back(bn_04.backIt());
    blockNodesExpected_0.push_back(bn_05.backIt());
    blockNodesExpected_0.push_back(bn_06.backIt());
    blockNodesExpected_0.push_back(bn_07.backIt());
    blockNodesExpected_0.push_back(bn_08.backIt());
    blockNodesExpected_0.push_back(bn_09.backIt());
    blockNodesExpected_0.push_back(bn_10.backIt());
    blockNodesExpected_0.push_back(bn_11.backIt());
    blockNodesExpected_0.push_back(bn_12.backIt());
    blockNodesExpected_0.push_back(bn_13.backIt());
    blockNodesExpected_0.push_back(bn_14.backIt());
    blockNodesExpected_0.push_back(bn_15.backIt());
    blockNodesExpected_0.push_back(bn_16.backIt());
    blockNodesExpected_0.push_back(bn_17.backIt());
    blockNodesExpected_0.push_back(bn_18.backIt());
    blockNodesExpected_0.push_back(bn_19.backIt());
    blockNodesExpected_0.push_back(bn_20.backIt());
    blockNodesExpected_0.push_back(bn_21.backIt());
    blockNodesExpected_0.push_back(bn_22.backIt());

    EXPECT_EQ(skel_0.graph().numNodes(), 22);
    EXPECT_EQ(skel_0.graph().numEdges(), 39);

    vector<node_list_it_t> skelNodes_0;

    EXPECT_EQ(
     checkSkelNodes(skel_0.graph(), blockNodesExpected_0, skelNodes_0), true);

    vector<edge_list_it_t> skelEdgesTested_0;
    vector<edge_list_it_t> virtualEdges_0;
    skelEdgesTested_0.push_back(be_01_02.backIt());
    skelEdgesTested_0.push_back(be_01_03.backIt());
    skelEdgesTested_0.push_back(be_01_07.backIt());
    skelEdgesTested_0.push_back(be_02_04.backIt());
    skelEdgesTested_0.push_back(be_02_08.backIt());
    skelEdgesTested_0.push_back(be_03_04.backIt());
    skelEdgesTested_0.push_back(be_03_05.backIt());
    skelEdgesTested_0.push_back(be_04_06.backIt());
    skelEdgesTested_0.push_back(be_05_06.backIt());
    skelEdgesTested_0.push_back(be_05_07.backIt());
    skelEdgesTested_0.push_back(be_06_08.backIt());
    skelEdgesTested_0.push_back(be_07_09.backIt());
    skelEdgesTested_0.push_back(be_08_15.backIt());
    skelEdgesTested_0.push_back(be_08_13.backIt());
    skelEdgesTested_0.push_back(be_13_15.backIt());
    skelEdgesTested_0.push_back(be_13_14.backIt());
    skelEdgesTested_0.push_back(be_15_16.backIt());
    skelEdgesTested_0.push_back(be_14_10.backIt());
    skelEdgesTested_0.push_back(be_14_16.backIt());
    skelEdgesTested_0.push_back(be_10_16.backIt());
    skelEdgesTested_0.push_back(be_09_10.backIt());
    skelEdgesTested_0.push_back(be_09_11.backIt());
    skelEdgesTested_0.push_back(be_10_11.backIt());
    skelEdgesTested_0.push_back(be_09_12.backIt());
    skelEdgesTested_0.push_back(be_11_12.backIt());
    skelEdgesTested_0.push_back(be_10_12.backIt());
    skelEdgesTested_0.push_back(be_10_17.backIt());
    skelEdgesTested_0.push_back(be_10_18.backIt());
    skelEdgesTested_0.push_back(be_12_17.backIt());
    skelEdgesTested_0.push_back(be_17_18.backIt());
    skelEdgesTested_0.push_back(be_12_18.backIt());
    skelEdgesTested_0.push_back(be_12_21.backIt());
    skelEdgesTested_0.push_back(be_12_19.backIt());
    skelEdgesTested_0.push_back(be_19_20.backIt());
    skelEdgesTested_0.push_back(be_18_20.backIt());
    skelEdgesTested_0.push_back(be_18_22.backIt());
    skelEdgesTested_0.push_back(be_20_22.backIt());
    skelEdgesTested_0.push_back(be_19_21.backIt());
    skelEdgesTested_0.push_back(be_21_22.backIt());

    EXPECT_EQ(checkSkelEdges(
                skel_0.graph(), skelEdgesTested_0, 0, virtualEdges_0), true);
    EXPECT_EQ(skelNodes_0.size(),22);
    EXPECT_EQ(skel_0.mSkelNit1, skelNodes_0[12]);
    EXPECT_EQ(skel_0.mSkelNit2, skelNodes_0[13]);
    EXPECT_EQ(skel_0.mSkelEit1, skel_0.graph().edges().second);
    EXPECT_EQ(skel_0.mSkelEit2, skel_0.graph().edges().second);

}

} // namespace Undirected

} // namespace Wailea

