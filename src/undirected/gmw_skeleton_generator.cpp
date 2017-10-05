#include "undirected/gmw_skeleton_generator.hpp"
#include "undirected/spqr_decomposer.hpp"
#include "undirected/tree_path_finder.hpp"
#include "undirected/tree_splitter.hpp"

/**
 * @file undirected/gmw_skeleton_generator.cpp
 *
 * @brief
 */
namespace Wailea {

namespace Undirected {

using namespace std;

void GMWSkeleton::generateSkeleton()
{

    auto& aNode = *(*(*(mBlockNodes.begin())));
    auto& B = dynamic_cast<Block&>(aNode.graph());
    vector<edge_list_it_t> blockEdges = B.nodeInducedEdges(mBlockNodes);

    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    for (auto nit : mBlockNodes) {

        auto np = make_unique<GMWSkeletonNode>();
        np->pushIGBackwardLink(nit);
        nodePairs.push_back(make_pair(nit,std::move(np)));

    }

    for (auto eit : blockEdges) {

        bool skipping = false;

        auto& BE = dynamic_cast<BlockEdge&>(*(*eit));
        auto  nit1 = BE.incidentNode1().backIt();
        auto  nit2 = BE.incidentNode2().backIt();
     
        if (mType == END1_EDGE_END2_NODE || mType == END1_EDGE_END2_EDGE) {

            if ( (nit1 == mBlockNit11 && nit2 == mBlockNit12)|| 
                 (nit1 == mBlockNit12 && nit2 == mBlockNit11)  ) { 

                skipping = true;
            }
        }

        if (mType == END1_NODE_END2_EDGE || mType == END1_EDGE_END2_EDGE) {

            if ( (nit1 == mBlockNit21 && nit2 == mBlockNit22)|| 
                 (nit1 == mBlockNit22 && nit2 == mBlockNit21)  ) { 

                skipping = true;
            }
        }

        if (!skipping) {

            auto ep = make_unique<GMWSkeletonEdge>();
            ep->pushIGBackwardLink(eit);
            ep->mVirtual  = false;
            edgePairs.push_back(make_pair(eit,std::move(ep)));

        }

    }

    B.copySubgraph(nodePairs, edgePairs, *mSkel);
    node_list_it_t skelNit11;
    node_list_it_t skelNit12;
    node_list_it_t skelNit21;
    node_list_it_t skelNit22;

    for (auto nit=mSkel->nodes().first; nit!=mSkel->nodes().second; nit++) {

        auto &SN = dynamic_cast<GMWSkeletonNode&>(*(*nit));

        if (SN.IGBackwardLink() == mBlockNit11) {

            skelNit11 = nit;
        }

        if (mType == END1_EDGE_END2_NODE || mType == END1_EDGE_END2_EDGE) {

            if (SN.IGBackwardLink() == mBlockNit12) {

                skelNit12 = nit;
            }
        }

        if (SN.IGBackwardLink() == mBlockNit21) {

            skelNit21 = nit;
        }

        if (mType == END1_NODE_END2_EDGE || mType == END1_EDGE_END2_EDGE) {

            if (SN.IGBackwardLink() == mBlockNit22) {

                skelNit22 = nit;
            }
        }

    }

    // Make virtual edges.
    if (mType == END1_EDGE_END2_NODE ||  mType == END1_EDGE_END2_EDGE) {

        auto& N1 = *(*skelNit11);
        auto& N2 = *(*skelNit12);

        auto ep = make_unique<GMWSkeletonEdge>();
        ep->pushIGBackwardLink(B.edges().second);
        ep->mVirtual  = true;

        auto& E = mSkel->addEdge(std::move(ep), N1, N2);

        mSkelNit1 = mSkel->nodes().second;
        mSkelEit1 = E.backIt();

    }
    else {

        mSkelNit1 = skelNit11;
        mSkelEit1 = mSkel->edges().second;
    }

    if (mType == END1_NODE_END2_EDGE || mType == END1_EDGE_END2_EDGE) {

        auto& N1 = *(*skelNit21);

        auto& N2 = *(*skelNit22);


        auto ep = make_unique<GMWSkeletonEdge>();

        ep->pushIGBackwardLink(B.edges().second);

        ep->mVirtual  = true;

        auto& E = mSkel->addEdge(std::move(ep), N1, N2);

        mSkelNit2 = mSkel->nodes().second;

        mSkelEit2 = E.backIt();

    }
    else {

        mSkelNit2 = skelNit21;
        mSkelEit2 = mSkel->edges().second;
    }

}


void GMWSkeletonGenerator::processBlock(
    Block&         B,
    node_list_it_t bnit1,
    node_list_it_t bnit2
) {

    // Decompose the given biconnected component to the SPQR-tree.
    SPQRDecomposer decomposer;
    SPQRTree       spqrTree = decomposer.decompose(B, *(*bnit1));

    // Find the tree nodes for N1 and N2.
    node_list_it_t tnit1;
    node_list_it_t tnit2;
    findTreeNodesFromBlockNodes(spqrTree, bnit1, bnit2, tnit1, tnit2);

    auto& TN1 = dynamic_cast<SPQRTreeNode&>(*(*tnit1));
    auto& TN2 = dynamic_cast<SPQRTreeNode&>(*(*tnit2));

    // Find a path from N1 to N2 on the tree.
    TreePathFinder finder;

    list<node_list_it_t> spqrPathNodes;
    list<edge_list_it_t> spqrPathEdges;
    finder.findPath(spqrTree, TN1, TN2, spqrPathNodes, spqrPathEdges);

    // Find the minimal path from N1 to N2 on the tree.
    findMinimalTreePath(spqrPathNodes, spqrPathEdges, bnit1, bnit2);

    // Find the component nodes for each representative node along the path.

    if (spqrPathNodes.size() == 1) {

        // Check if {bnit1, bnit2} is a virtual edge.
        if(!isVirtualEdge(*(spqrPathNodes.begin()), bnit1, bnit2)){
            // One single skeleton that consists of the entire block.        
            GMWSkeleton skel;
            skel.mType = GMWSkeleton::END1_NODE_END2_NODE;
            skel.mBlockNit11 = bnit1;
            skel.mBlockNit21 = bnit2;

            for (auto bnit=B.nodes().first; bnit!=B.nodes().second; bnit++) {
                skel.mBlockNodes.push_back(bnit);
            }
            skel.generateSkeleton();
            mSkeletons.push_back(std::move(skel));
        }
    }
    else {

        // Prepare the tree edges for expansion.
        TreeSplitter splitter(spqrTree);
        splitter.prepareTree(spqrPathEdges);    

        size_t index = 0;
        list<edge_list_it_t>::iterator eItIt = spqrPathEdges.begin();
        for (auto nit : spqrPathNodes) {

            auto& TN = dynamic_cast<SPQRTreeNode&>(*(*nit));
            
            if (TN.type() == SPQRTreeNode::RType) {

                if (index == 0) {

                    // First component on the path.
                    GMWSkeleton skel;
                    skel.mType  = GMWSkeleton::END1_NODE_END2_EDGE;
                    skel.mBlockNit11 = bnit1;
                    auto& TE2  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));
                    edge_list_it_t ve2It;
                    if (TE2.incidentNode1().backIt()== nit) {
                        ve2It = TE2.virtualEdge1();
                    }
                    else {
                        ve2It = TE2.virtualEdge2();
                    }
                    auto& VE2 = dynamic_cast<Edge&>(*(*ve2It));
                    auto& CN2_1 = dynamic_cast<SPQRComponentNode&>(
                                                         VE2.incidentNode1());
                    auto& CN2_2 = dynamic_cast<SPQRComponentNode&>(
                                                         VE2.incidentNode2());
                    skel.mBlockNit21 = CN2_1.IGBackwardLink();
                    skel.mBlockNit22 = CN2_2.IGBackwardLink();

                    list<node_list_it_t> treeNodes 
                                                 = splitter.findComponent(nit);
                    skel.mBlockNodes = fromTreeNodesToSkeletonNodes(treeNodes);
                    skel.generateSkeleton();
                    mSkeletons.push_back(std::move(skel));

                }
                else if (index == (spqrPathNodes.size() - 1) ) {

                    // Last component on the path.
                    GMWSkeleton skel;
                    skel.mType  = GMWSkeleton::END1_EDGE_END2_NODE;

                    auto& TE1  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));
                    edge_list_it_t ve1It;
                    if (TE1.incidentNode1().backIt()== nit) {
                        ve1It = TE1.virtualEdge1();
                    }
                    else {
                        ve1It = TE1.virtualEdge2();
                    }
                    auto& VE1 = dynamic_cast<Edge&>(*(*ve1It));
                    auto& CN1_1 = dynamic_cast<SPQRComponentNode&>(
                                                         VE1.incidentNode1());
                    auto& CN1_2 = dynamic_cast<SPQRComponentNode&>(
                                                         VE1.incidentNode2());
                    skel.mBlockNit11 = CN1_1.IGBackwardLink();
                    skel.mBlockNit12 = CN1_2.IGBackwardLink();
                    skel.mBlockNit21 = bnit2;

                    list<node_list_it_t> treeNodes
                                                 = splitter.findComponent(nit);
                    skel.mBlockNodes = fromTreeNodesToSkeletonNodes(treeNodes);
                    skel.generateSkeleton();
                    mSkeletons.push_back(std::move(skel));

                }
                else {

                    // Intermediate component on the path.
                    GMWSkeleton skel;
                    skel.mType  = GMWSkeleton::END1_EDGE_END2_EDGE;

                    auto& TE1  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));
                    edge_list_it_t ve1It;
                    if (TE1.incidentNode1().backIt()== nit) {
                        ve1It = TE1.virtualEdge1();
                    }
                    else {
                        ve1It = TE1.virtualEdge2();
                    }
                    auto& VE1 = dynamic_cast<Edge&>(*(*ve1It));
                    auto& CN1_1 = dynamic_cast<SPQRComponentNode&>(
                                                         VE1.incidentNode1());
                    auto& CN1_2 = dynamic_cast<SPQRComponentNode&>(
                                                         VE1.incidentNode2());
                    skel.mBlockNit11 = CN1_1.IGBackwardLink();
                    skel.mBlockNit12 = CN1_2.IGBackwardLink();
    
                    eItIt++;
                    auto& TE2  = dynamic_cast<SPQRTreeEdge&>(*(*(*eItIt)));
                    edge_list_it_t ve2It;
                    if (TE2.incidentNode1().backIt()== nit) {
                        ve2It = TE2.virtualEdge1();
                    }
                    else {
                        ve2It = TE2.virtualEdge2();
                    }
                    auto& VE2 = dynamic_cast<Edge&>(*(*ve2It));
                    auto& CN2_1 = dynamic_cast<SPQRComponentNode&>(
                                                         VE2.incidentNode1());
                    auto& CN2_2 = dynamic_cast<SPQRComponentNode&>(
                                                         VE2.incidentNode2());
                    skel.mBlockNit21 = CN2_1.IGBackwardLink();
                    skel.mBlockNit22 = CN2_2.IGBackwardLink();

                    list<node_list_it_t> treeNodes
                                                 = splitter.findComponent(nit);
                    skel.mBlockNodes = fromTreeNodesToSkeletonNodes(treeNodes);
                    skel.generateSkeleton();
                    mSkeletons.push_back(std::move(skel));

                }
            }
            else { // Not Rtype
                if ( (index != 0) && (index != (spqrPathNodes.size() - 1)) ) {
                    eItIt++;
                }
            }

            index++;
        }
        splitter.cleanUpTree();
    }
}


void GMWSkeletonGenerator::findTreeNodesFromBlockNodes(
    SPQRTree&       T,
    node_list_it_t  bnit1,
    node_list_it_t  bnit2,
    node_list_it_t& tnit1,
    node_list_it_t& tnit2
) {

    tnit1 = T.nodes().second;
    tnit2 = T.nodes().second;

    for (auto tnit = T.nodes().first; tnit != T.nodes().second; tnit++) {

        auto& TN = dynamic_cast<SPQRTreeNode&>(*(*(tnit)));

        auto& C = TN.component();

        for (auto cnit = C.nodes().first; cnit != C.nodes().second; cnit++ ) {

            auto& CN = dynamic_cast<SPQRComponentNode&>(*(*cnit));

            if (CN.IGBackwardLink() == bnit1) {
                tnit1 = tnit;
            }

            if (CN.IGBackwardLink() == bnit2) {
                tnit2 = tnit;
            }
        }
    }

}


void GMWSkeletonGenerator::findMinimalTreePath(
    list<node_list_it_t>& treePathNodes,
    list<edge_list_it_t>& treePathEdges,
    node_list_it_t        bnit1,
    node_list_it_t        bnit2
) {
    size_t numN1Found = 0;
    size_t numN2Found = 0;

    for (auto tnit : treePathNodes) {

        auto& TN = dynamic_cast<SPQRTreeNode&>(*(*(tnit)));

        auto& C = TN.component();

        bool  N1Found = false;
        bool  N2Found = false;

        for (auto cnit = C.nodes().first; cnit != C.nodes().second; cnit++ ) {

            auto& CN = dynamic_cast<SPQRComponentNode&>(*(*cnit));

            if (CN.IGBackwardLink() == bnit1) {

                numN1Found++;
                N1Found = true;

            }

            if (CN.IGBackwardLink() == bnit2) {

                numN2Found++;
                N2Found = true;

            }
        }

        if (N1Found && N2Found) {

            // BN1 and BN2 on the same component.
            treePathNodes.clear();
            treePathEdges.clear();
            treePathNodes.push_back(tnit);
            return ;

        }
    }

    if (numN1Found > 0) { 
        numN1Found--;
    }

    if (numN2Found > 0) { 
        numN2Found--;
    }

    for (size_t i = 0; i < numN1Found; i++) {
        treePathEdges.pop_front();
        treePathNodes.pop_front();
    }

    for (size_t i = 0; i < numN2Found; i++) {
        treePathEdges.pop_back();
        treePathNodes.pop_back();
    }

}


vector<node_list_it_t> GMWSkeletonGenerator::fromTreeNodesToSkeletonNodes(
    list<node_list_it_t> treeNodes
) {

    vector<node_list_it_t> blockNodes;

    for (auto tnit : treeNodes) {

        auto& TN = dynamic_cast<SPQRTreeNode&>(*(*tnit));
        auto& C  = TN.component();

        for (auto cnit = C.nodes().first; cnit != C.nodes().second; cnit++ ) {

            auto& CN = dynamic_cast<SPQRComponentNode&>(*(*cnit));
            auto& BN = dynamic_cast<BlockNode&>(CN.IGBackwardLinkRef());
            BN.pushUtility(0);

        }
    }

    for (auto tnit : treeNodes) {

        auto& TN = dynamic_cast<SPQRTreeNode&>(*(*tnit));
        auto& C  = TN.component();

        for (auto cnit = C.nodes().first; cnit != C.nodes().second; cnit++ ) {

            auto& CN = dynamic_cast<SPQRComponentNode&>(*(*cnit));
            auto& BN = dynamic_cast<BlockNode&>(CN.IGBackwardLinkRef());
            if (BN.utility() == 0) {
                BN.setUtility(1);
                blockNodes.push_back(BN.backIt());
            }
        }
    }

    for (auto tnit : treeNodes) {

        auto& TN = dynamic_cast<SPQRTreeNode&>(*(*tnit));
        auto& C  = TN.component();

        for (auto cnit = C.nodes().first; cnit != C.nodes().second; cnit++ ) {

            auto& CN = dynamic_cast<SPQRComponentNode&>(*(*cnit));
            auto& BN = dynamic_cast<BlockNode&>(CN.IGBackwardLinkRef());
            BN.popUtility();

        }

    }

    return blockNodes;
}


size_t GMWSkeletonGenerator::numSkeletons() const {
    return mSkeletons.size();
}


GMWSkeleton& GMWSkeletonGenerator::skeleton(size_t i)
{
    return mSkeletons[i];
}


bool GMWSkeletonGenerator::isVirtualEdge(
    node_list_it_t tnit,
    node_list_it_t bnit1,
    node_list_it_t bnit2
) {
    auto& TN = dynamic_cast<SPQRTreeNode&>(*(*tnit));
    auto& C  = TN.component();

    for (auto ceit = C.edges().first; ceit != C.edges().second; ceit++ ) {

        auto& CE  = dynamic_cast<SPQRComponentEdge&>(*(*ceit));
        if (CE.type()==SPQRComponentEdge::VirtualType) {
            auto& CN1 = dynamic_cast<SPQRComponentNode&>(CE.incidentNode1());
            auto& BN1 = dynamic_cast<BlockNode&>(CN1.IGBackwardLinkRef());
            auto& CN2 = dynamic_cast<SPQRComponentNode&>(CE.incidentNode2());
            auto& BN2 = dynamic_cast<BlockNode&>(CN2.IGBackwardLinkRef());         
            if ( (BN1.backIt() == bnit1 && BN2.backIt() == bnit2)||
                 (BN1.backIt() == bnit2 && BN2.backIt() == bnit1)  ) {
                return true;
            }
        }
    }
    return false;    
}

}// namespace Undirected

}// namespace Wailea

