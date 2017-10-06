#include "undirected/gmw_edge_inserter.hpp"
#include "undirected/gmw_bc_path.hpp"
#include "undirected/gmw_skeleton_generator.hpp"
#include "undirected/planar_insertion_path_finder.hpp"

/**
 * @file undirected/gmw_edge_inserter.cpp
 *
 * @brief
 *
 * @remark
 */
namespace Wailea {

namespace Undirected {

using namespace std;


void GMWEdgeInserter::findInsertionPath(
    Graph&       G,
    Node&        N1,
    Node&        N2
) {

    mInsertionPath.clear();

    if (N1.backIt() == N2.backIt()) {
        return;
    }

    list<edge_list_it_t> edgesList;

    GMWBCPath bcPath(G);

    bcPath.processConnectedGraph(N1.backIt(), N2.backIt());

    GMWBCPath::pathType bcType = bcPath.getPathType();

    node_list_it_t bn1It;
    node_list_it_t bn2It;

    if (bcType==GMWBCPath::BC_PATH_BOTH_CUT_VERTICES) {

        // Both N1 and N2 are cut vertices. {N1,N2} can be added
        // to G without breaking planarity.
        ;
    }
    else if (bcType==GMWBCPath::BC_PATH_BOTH_IN_THE_SAME_BLOCK) {

        auto& block = bcPath.getSingleBlock(bn1It, bn2It);
        processBlock(block, bn1It, bn2It);

    }
    else {

        if (bcType==GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK||
            bcType==GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_CUT_VERTEX) {

            auto& block = bcPath.getBlock1(bn1It, bn2It);
            processBlock(block, bn1It, bn2It);

        }

        if (bcType==GMWBCPath::BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK||
            bcType==GMWBCPath::BC_PATH_N1_IS_CUT_VERTEX_N2_IS_BLOCK) {

            auto& block = bcPath.getBlock2(bn1It, bn2It);
            processBlock(block, bn1It, bn2It);

        }
    }
}


list<edge_list_it_t> GMWEdgeInserter::getPath() {
    list<edge_list_it_t> rvoPath;
    for (auto eit : mInsertionPath) {
        rvoPath.push_back(eit);
    }
    return rvoPath;
}


void GMWEdgeInserter::processBlock(
    Block&         B,
    node_list_it_t bn1It,
    node_list_it_t bn2It
) {

    if (B.numNodes() < 3) {
        // No need to process if block consists of a single edge.
        return;
    }

    GMWSkeletonGenerator generator;
    generator.processBlock(B, bn1It, bn2It);

    for (size_t i = 0 ; i < generator.numSkeletons(); i++) {

        GMWSkeleton& BlockSkel = generator.skeleton(i);
        PlanarInsertionPathFinder finder(BlockSkel.graph());

        list<edge_list_it_t> skelEdges = finder.findInsertionPath(
            BlockSkel.mSkelNit1,
            BlockSkel.mSkelEit1,
            BlockSkel.mSkelNit2,
            BlockSkel.mSkelEit2
        );

        for (auto seit : skelEdges) {

            auto& SE = dynamic_cast<GMWSkeletonEdge&>(*(*seit)); 
            if (!SE.mVirtual) {
                auto& BE = dynamic_cast<BlockEdge&>(SE.IGBackwardLinkRef());
                mInsertionPath.push_back(BE.IGBackwardLink());
            }
        }
    }

}

}// namespace Undirected

}// namespace Wailea

