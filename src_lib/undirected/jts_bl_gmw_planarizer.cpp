#include "undirected/jts_bl_gmw_planarizer.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/bctree.hpp"
#include "undirected/jts_planarizer.hpp"
#include "undirected/gmw_edge_inserter.hpp"
#include "undirected/bl_planarity_tester.hpp"

/**
 * @file undirected/jts_bl_gmw_planarizer.cpp
 *
 * @brief
 */
namespace Wailea {

namespace Undirected {

using namespace std;

/**
 *              Diagram for IGBackward/ForwardLinks 
 *              ===================================
 *
 *
 *                                           BCTree
 *                                       +-------------+
 *    Planarizable        Original       |             |
 *       Graph             Graph         |    TN1      |
 *   +-----------+   +---------------+   |  /|\ |\     |
 *   |           |   |               |   |   |  B \    |
 *   |  PlN1 -B---------> N1 -F--------------   |  |   |
 *   |   /|\     |   |    |  <------------------   |   |  PlanarizableGraph
 *   |    |      |   |    F/|\|      |   |         |   |     for Block       
 *   |    |      |   |    | | F      |   |   Block |   |   +-----------+
 *   |    |      |   |    | | |      |   | +-------|-+ |   |           |
 *   |     ---------------  | |      |   | |      /  | |   |           |
 *   |           |   |      |  -------------->BN1/-F----------->BPlN1  |
 *   |           |   |      |        |   | |   | /|\ | |   |     |     |
 *   |           |   |      |        |   | |   |  |  | |   |     |     |
 *   |           |   |       ----------------B-    ------------B-      |
 *   |           |   |               |   | |         | |   |           |
 *   |           |   |               |   | |         | |   |           |
 *   |  PlN2 -B---------> N2 -F-------------->BN2 -F----------->BPlN2  |
 *   |   /|\     |   |    | /|\      |   | |   | /|\ | |   |     |     |
 *   |    |      |   |    |  |       |   | |   |  |  | |   |     |     |    
 *   |     -------------F-    ---------------B-    ------------B-      |
 *   |           |   |               |   | |         | |   |           |
 *   |           |   |               |   | |         | |   |           |
 *   |  PlN3 -B------------------------------------------------>BPlN3  |  
 *   |   /|\     |   |               |   | |         | |   |     |     |
 *   |    |      |   |               |   | |         | |   |     F     |
 *   |     ------------------------------------------------------      |
 *   |           |   |               |   | |         | |   |           |
 *   |           |   |               |   | |         | |   |           |
 *   |  PlE1_1 -B---------> E1 -F------------->BE1 -F----------->BPlE1_1|
 *   |   /|\     |   |   | /|\/| \   |   | |  |/|\/|\| |   |     |     |
 *   |    |      |   |   |  |  |     |   | |  | |  | | |   |     |     |
 *   |     ------------F-   |   ------------B-  |   -----------B-      |
 *   |           |   |      |        |   | |    |    | |   |           |
 *   |           |   |      |        |   | +---------+ |   |           |
 *   |           |   |      |        |   |      |      |   |           |
 *   |  PlE1_2-B------------         |   |       ------------B-BPlE1_2 |
 *   |   /|\     |   |               |   |             |   |     |     |
 *   |    |      |   |               |   |             |   |     |     |
 *   |     ------------(temporary link for mChains)------------F-      |
 *   |           |   |               |   |             |   |           |
 *   +-----------+   +---------------+   +-------------+   +-----------+
 *
 *   Cur vertex:    N1 N1 TN1 BN1 BPlN1 PlN1
 *   Ordinary node: N2 N2 TN2 BN2 BPlN2 PlN2
 *   Virtual node:  BPlN3 PlN3
 *   Edge: (head of the split edge chain): BPlE1_1, PlE1_1, BE1, E1
 *   Edge: (head of the split edge chain): BPlE1_2, PlE1_2 (, BE1, E1)
 *
 */
void JTS_BL_GMW_Planarizer::planarizeConnectedGraph(
    Graph&             G,
    PlanarizableGraph& planarizedG
) {
    if (planarizedG.numNodes() > 0) {
        throw std::invalid_argument(Constants::kExceptionNotEmpty);
    }

    // Decompose the graph into BC-tree.
    BiconnectedDecomposer decomposer;
    BCTree bcTree = decomposer.decompose(G);

    // Planarize all the blocks
    vector<PlanarizableGraph> planarizedBlocks;

    for (auto tnit = bcTree.nodes().first; 
                                       tnit != bcTree.nodes().second; tnit++) {
        auto& TN = dynamic_cast<BCTreeNode&>(*(*tnit));

        if (TN.type() == BCTreeNode::BlockType) {
            PlanarizableGraph planarizedBlock;
            planarizeBiconnectedGraph(TN.block(), planarizedBlock);
            planarizedBlocks.push_back(std::move(planarizedBlock));
        }
    }
    // Fill planarizedG with non-virtual nodes.
    for (auto gnit = G.nodes().first; gnit != G.nodes().second; gnit++) {

        auto& GN   = *(*gnit);
        auto  plnp = make_unique<PlanarizableNode>();
        auto& PLN  = dynamic_cast<PlanarizableNode&>(
                                         planarizedG.addNode(std::move(plnp)));

        PLN.pushIGBackwardLink(gnit);
        PLN.setVirtual(false);
        GN.pushIGForwardLink(PLN.backIt());
    }
    for (auto& planarizedBlock : planarizedBlocks) {

        // Fill planarizedG with virtual nodes.
        for (auto snit = planarizedBlock.nodes().first; 
                              snit != planarizedBlock.nodes().second; snit++) {

            auto& SN = dynamic_cast<PlanarizableNode&>(*(*snit));

            if (SN.isVirtual()) {
                auto  plnp = make_unique<PlanarizableNode>();
                auto& PLN  = dynamic_cast<PlanarizableNode&>(
                                         planarizedG.addNode(std::move(plnp)));
                PLN.pushIGBackwardLink(snit);
                PLN.setVirtual(true);
                SN.pushIGForwardLink(PLN.backIt());
            }

        }

        // Block edge pass 1. Create edges.
        for (auto seit = planarizedBlock.edges().first; 
                              seit != planarizedBlock.edges().second; seit++) {

            auto& SE  = dynamic_cast<PlanarizableEdge&>(*(*seit));
            auto& SN1 = dynamic_cast<PlanarizableNode&>(SE.incidentNode1());
            auto& SN2 = dynamic_cast<PlanarizableNode&>(SE.incidentNode2());

            auto& PLN1 = findNodeInPlanarizedG(SN1);
            auto& PLN2 = findNodeInPlanarizedG(SN2);

            auto  plep = make_unique<PlanarizableEdge>();
            auto& PLE  = dynamic_cast<PlanarizableEdge&>(
                             planarizedG.addEdge(std::move(plep), PLN1, PLN2));

            auto& BE = dynamic_cast<BlockEdge&>(SE.IGBackwardLinkRef());
            auto& GE = dynamic_cast<Edge&>(BE.IGBackwardLinkRef());

            GE.pushIGForwardLink(PLE.backIt());
            PLE.pushIGBackwardLink(GE.backIt());

            // Temporary link from SE to PLE for PlanarizableEdge::mChainNx.
            SE.pushIGBackwardLink(PLE.backIt());
        }

        // Block edge pass 2. Patch up the chains.
        for (auto seit = planarizedBlock.edges().first; 
                              seit != planarizedBlock.edges().second; seit++) {

            auto& SE  = dynamic_cast<PlanarizableEdge&>(*(*seit));

            auto& PLE = dynamic_cast<PlanarizableEdge&>(
                                                       SE.IGBackwardLinkRef());

            if (!SE.isTerminalN1()){

                if (SE.chainN1() != planarizedBlock.edges().second) {
                    auto& SChainN1 = dynamic_cast<PlanarizableEdge&>(
                                                           *(*(SE.chainN1())));
                    auto& PLChainN1 = dynamic_cast<PlanarizableEdge&>(
                                                 SChainN1.IGBackwardLinkRef());
                    PLE.setChainN1(PLChainN1.backIt());
                }

            }

            if (!SE.isTerminalN2()){

                if (SE.chainN2() != planarizedBlock.edges().second) {

                    auto& SChainN2 = dynamic_cast<PlanarizableEdge&>(
                                                           *(*(SE.chainN2())));

                    auto& PLChainN2 = dynamic_cast<PlanarizableEdge&>(
                                                 SChainN2.IGBackwardLinkRef());

                    PLE.setChainN2(PLChainN2.backIt());
                } 
            }

        }

        // Remove temporary link from SE to PLE.
        planarizedBlock.popIGBackwardLinkFromEdges();

    }

    // Cleanup the inter-graph links in planarizedG
    for(auto plnit = planarizedG.nodes().first;
                                plnit != planarizedG.nodes().second; plnit++) {
        auto& PLN = dynamic_cast<PlanarizableNode&>(*(*plnit));
        if(PLN.isVirtual()){
            PLN.setIGBackwardLink(planarizedG.nodes().second);
        }
    }

    // Cleanup the inter-graph links in G
    for(auto gnit = G.nodes().first; gnit != G.nodes().second; gnit++) {

        auto& GN = dynamic_cast<Node&>(*(*gnit));

        auto lit = GN.popIGForwardLink();

        GN.setIGForwardLink(lit);

        GN.popUtility();

    }

    for(auto geit = G.edges().first; geit != G.edges().second; geit++) {
                                
        auto& GE = dynamic_cast<Edge&>(*(*geit));
        auto lit = GE.popIGForwardLink();
        GE.setIGForwardLink(lit);
    }

}


// From an edge (PlanarizableEdge) in the planarized Block to the corresponding
// edge (PlanarizableEdge) in planarized graph.
PlanarizableEdge& JTS_BL_GMW_Planarizer::findEdgeInPlanarizedG(
    PlanarizableEdge& E
) {
    auto& BE = dynamic_cast<BlockEdge&>(E.IGBackwardLinkRef());
    auto& GE = dynamic_cast<Edge&>(BE.IGBackwardLinkRef());
    return dynamic_cast<PlanarizableEdge&>(GE.IGForwardLinkRef());
}


// From a node (PlanarizableNode) in the planarized Block to the corresponding
// node (PlanarizableNode) in planarized graph.
PlanarizableNode& JTS_BL_GMW_Planarizer::findNodeInPlanarizedG(
    PlanarizableNode& N
) {
    if (N.isVirtual()) {
        return dynamic_cast<PlanarizableNode&>(N.IGForwardLinkRef());
    }
    else {
        // BN is a BlockNode in the Block
        auto& BN = dynamic_cast<BlockNode&>(N.IGBackwardLinkRef());
        auto& GN = dynamic_cast<Node&>(BN.IGBackwardLinkRef());
        return dynamic_cast<PlanarizableNode&>(GN.IGForwardLinkRef());
    }
}


void JTS_BL_GMW_Planarizer::planarizeBiconnectedGraph(
    Graph&             G,
    PlanarizableGraph& planarizedG
) {
    PlanarizableGraph&     planarSubgraph = planarizedG;
    vector<edge_list_it_t> removedEdges;

    if (planarSubgraph.numNodes() > 0) {

        throw std::invalid_argument(Constants::kExceptionNotEmpty);
    }
    findPlanarSubgraph      (G, planarSubgraph, removedEdges);
    maximalizePlanarSubgraph(G, planarSubgraph, removedEdges);
    reinsertRemovedEdges    (G, planarSubgraph, removedEdges);
}


void JTS_BL_GMW_Planarizer::findPlanarSubgraph(
    Graph&                  G,
    PlanarizableGraph&      planarSubgraph,
    vector<edge_list_it_t>& removedEdges
) {

    JTSPlanarizer jts;
    STNumbering   st;

    vector<node_list_it_t> stOrder;
    auto bnit = G.nodes().first;
    if (G.numNodes() >=2) {
        auto& S = dynamic_cast<Node&>(*(*(bnit)));
        bnit++;
        auto& T = dynamic_cast<Node&>(*(*(bnit)));
        stOrder = st.getBipolarOrientation(G, S, T);
    }
    else if (G.numNodes() == 2) {
        stOrder.push_back(bnit);
        bnit++;
        stOrder.push_back(bnit);
    }
    else if (G.numNodes() == 1) {
        stOrder.push_back(bnit);
    }

    jts.findUpwardPlanarSingleSourceSubgraph(
                                 G, stOrder, planarSubgraph, removedEdges);

}


void JTS_BL_GMW_Planarizer::maximalizePlanarSubgraph(
    Graph&                  G,
    PlanarizableGraph&      planarSubgraph,
    vector<edge_list_it_t>& removedEdges
) {

    if (G.numNodes() <= 4) {
        // Up to K4. No need for further attempt to insert edge.
        return;
    }

    // Check if the planarSubgraph is still biconnected.
    BiconnectedDecomposer bd;
    BCTree tree = bd.decompose(planarSubgraph);

    if (tree.blockNodes().size() != 1 || tree.cutVertexNodes().size() != 0) {
        // planarSubgraph is not biconnected. Giving up.

        return;
    }

    // Recreate st-ordering on the biconnected planar-subgraph.
    auto bnit = G.nodes().first;
    auto& S = dynamic_cast<Node&>(*(*(bnit))).IGForwardLinkRef();
    bnit++;
    auto& T = dynamic_cast<Node&>(*(*(bnit))).IGForwardLinkRef();
    STNumbering st;
    vector<node_list_it_t> stOrder = st.getBipolarOrientation(
                                                         planarSubgraph, S, T);

    // For each removed edge, try inserting.
    vector<edge_list_it_t> removedEdgesUpdated;

    for (auto eit : removedEdges) {

        auto& Eorg  = dynamic_cast<Edge&>(*(*eit));
        auto& N1org = Eorg.incidentNode1();
        auto& N2org = Eorg.incidentNode2();
        auto& N1new = dynamic_cast<PlanarizableNode&>(
                                                     N1org.IGForwardLinkRef());
        auto& N2new = dynamic_cast<PlanarizableNode&>(
                                                     N2org.IGForwardLinkRef());
        auto  ep    = make_unique<PlanarizableEdge>();
        auto& Etry  = dynamic_cast<PlanarizableEdge&>(
                               planarSubgraph.addEdge(move(ep), N1new, N2new));

        BLPlanarityTester bl;

        bool isPlanar = bl.isPlanar(planarSubgraph, stOrder);

        if (isPlanar) {

            // Insersion successful.
            // Make Etry a proper edge in planarSubgraph.
            Etry.pushIGBackwardLink(eit);
            Eorg.setIGForwardLink(Etry.backIt());
            Eorg.setUtility(JTSPlanarizer::kForwardTypeInPlanarSubgraph);
        }
        else {

            // Insersion unsucessful. Remove the tried edge from planarSubgraph
            planarSubgraph.removeEdge(Etry);
            removedEdgesUpdated.push_back(eit);
        }

    }

    removedEdges = std::move(removedEdgesUpdated);

}


void JTS_BL_GMW_Planarizer::reinsertRemovedEdges(
    Graph&                  G,
    PlanarizableGraph&      planarSubgraph,
    vector<edge_list_it_t>& removedEdges
) {

    for (auto eit2 = planarSubgraph.edges().first; eit2 != planarSubgraph.edges().second; eit2++) {
        auto& E  = dynamic_cast<PlanarizableEdge&>(*(*eit2));
        E.mChainN1 = planarSubgraph.edges().second;
        E.mChainN2 = planarSubgraph.edges().second;
    }

    for (auto eit : removedEdges) {
        auto& E  = dynamic_cast<Edge&>(*(*eit));
        auto& N1 = dynamic_cast<PlanarizableNode&>(
                                         E.incidentNode1().IGForwardLinkRef());
        auto& N2 = dynamic_cast<PlanarizableNode&>(
                                         E.incidentNode2().IGForwardLinkRef());
        GMWEdgeInserter inserter;
        inserter.findInsertionPath(planarSubgraph, N1, N2);
        list<edge_list_it_t> iPath = inserter.getPath();
        vector<edge_ptr_t>   removedEdges =
                                planarSubgraph.insertEdge(iPath, N1, N2, eit);
        removedEdges.clear();
    }
}

}// namespace Undirected

}// namespace Wailea
