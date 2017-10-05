#include "undirected/base.hpp"
#include "undirected/tree_path_finder.hpp"
#include "undirected/gmw_bc_path.hpp"

/**
 * @file undirected/gmw_bc_path.cpp
 *
 * @brief
 *
 * @remark
 *
 */
namespace Wailea {

namespace Undirected {

using namespace std;


void GMWBCPath::processConnectedGraph(node_list_it_t n1It, node_list_it_t n2It)
{
    mPathType = BC_PATH_TYPE_UNKNOWN;

    BiconnectedDecomposer decomposer; 
    mBCTree = decomposer.decompose(mG);

    mTNIt1 = mBCTree.nodes().second;
    mTNIt2 = mBCTree.nodes().second;

    // First check for cut vertices.
    vector<node_list_it_t> cutVertices = mBCTree.cutVertexNodes();
    for (auto nit : cutVertices) {
        auto& TN = dynamic_cast<BCTreeNode&>(*(*nit));
        if (TN.IGBackwardLink()==n1It) {
            mTNIt1 = TN.backIt();
        }
        else if (TN.IGBackwardLink()==n2It) {
            mTNIt2 = TN.backIt();
        }
    }

    if ( mTNIt1 != mBCTree.nodes().second &&
         mTNIt2 != mBCTree.nodes().second    ) {
        mPathType = BC_PATH_BOTH_CUT_VERTICES;
        return;
    }

    // Check for block nodes.
    vector<node_list_it_t> blockVertices = mBCTree.blockNodes();
    for (auto nit : blockVertices) {
        auto& TN = dynamic_cast<BCTreeNode&>(*(*nit));
        auto& Block = TN.block();

        for (auto bit = Block.nodes().first; 
                                      bit !=Block.nodes().second; bit++ ) {
            auto& BN = dynamic_cast<BlockNode&>(*(*bit));
            if (mTNIt1 == mBCTree.nodes().second &&
                                              BN.IGBackwardLink() == n1It) {
                mTNIt1  = TN.backIt();
                mBNIt11 = BN.backIt();
            }    

            if (mTNIt2 == mBCTree.nodes().second &&
                                              BN.IGBackwardLink() == n2It) {
                mTNIt2  = TN.backIt();
                mBNIt22 = BN.backIt();
            }
        }
    }

    if (mTNIt1 == mTNIt2) {
        // Both nodes belong to the same block.
        mPathType = BC_PATH_BOTH_IN_THE_SAME_BLOCK;
        return;

    }
    else {

        auto& TN1 = dynamic_cast<BCTreeNode&>(*(*mTNIt1));
        auto& TN2 = dynamic_cast<BCTreeNode&>(*(*mTNIt2));

        TreePathFinder finder;
        list<edge_list_it_t> bcPathEdges;
        list<node_list_it_t> bcPathNodes;
        finder.findPath(mBCTree, TN1, TN2, bcPathNodes, bcPathEdges);

        if (TN1.type() == BCTreeNode::BlockType) {

            auto& E  = dynamic_cast<BCTreeEdge&>(*(*(*(bcPathEdges.begin()))));
            auto& BN = E.cutVertexInBlock();
            mBNIt12  = BN.backIt();



        }

        if (TN2.type() == BCTreeNode::BlockType) {

            auto& E  =dynamic_cast<BCTreeEdge&>(*(*(*(bcPathEdges.rbegin()))));
            auto& BN = E.cutVertexInBlock();
            mBNIt21  = BN.backIt();

        }
        
        if (TN1.type() == BCTreeNode::BlockType &&
            TN2.type() == BCTreeNode::BlockType     ){
            mPathType = BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK;
        }
        else if (TN1.type() == BCTreeNode::CutVertexType &&
                 TN2.type() == BCTreeNode::BlockType        ){
            mPathType = BC_PATH_N1_IS_CUT_VERTEX_N2_IS_BLOCK;
        }
        else {
            mPathType = BC_PATH_N1_IS_BLOCK_N2_IS_CUT_VERTEX;
        }

    }
}

GMWBCPath::pathType GMWBCPath::getPathType()
{
    return mPathType;
}


Block& GMWBCPath::getSingleBlock(
    node_list_it_t& n1It,
    node_list_it_t& n2It
) {

    n1It = mBNIt11;
    n2It = mBNIt22;
    auto& TN = dynamic_cast<BCTreeNode&>(*(*mTNIt1));
    return TN.block();

}


Block& GMWBCPath::getBlock1(
    node_list_it_t& n1It,
    node_list_it_t& cvIt
) {

    n1It = mBNIt11;
    cvIt = mBNIt12;
    auto& TN = dynamic_cast<BCTreeNode&>(*(*mTNIt1));
    return TN.block();

}


Block& GMWBCPath::getBlock2(
    node_list_it_t& n1It,
    node_list_it_t& cvIt
) {

    n1It = mBNIt21;
    cvIt = mBNIt22;
    auto& TN = dynamic_cast<BCTreeNode&>(*(*mTNIt2));
    return TN.block();

}


}// namespace Undirected

}// namespace Wailea
