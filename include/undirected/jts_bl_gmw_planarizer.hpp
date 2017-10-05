#ifndef _WAILEA_UNDIRECTED_JTS_BL_GMW_PLANARIZER_HPP_
#define _WAILEA_UNDIRECTED_JTS_BL_GMW_PLANARIZER_HPP_

#include "undirected/base.hpp"
#include "undirected/planarizable_graph.hpp"

/**
 * @file undirected/jts_bl_gmw_planarizer.hpp
 *
 * @brief planarize a biconnected graph with the following algorithms
 *        1. JTS algorithm to find a good spanning planar subgraph.
 *        2. BL algorighm to find a maximal spanning planar subgraph
 *        3. GMW edge insertion algorithm to insert removed edges back in.
 */

namespace Wailea {

namespace Undirected {

using namespace std;

class JTS_BL_GMW_Planarizer {

  public:

    /** @brief finds a planarized graph of the given connected graph.
     *
     *  @param  G              (in): the input graph. Must be biconnected.
     *
     *  @param  planarizedG   (out): planarized copy of G.
     */
    void planarizeConnectedGraph(
        Graph&             G,
        PlanarizableGraph& planarizedG
    );


    /** @brief finds a planarized graph of the given biconnected graph.
     *
     *  @param  G              (in): the input graph. Must be biconnected.
     *
     *  @param  planarizedG   (out): planarized copy of G.
     */
    void planarizeBiconnectedGraph(
        Graph&             G,
        PlanarizableGraph& planarizedG
    );

  private:

    PlanarizableNode& findNodeInPlanarizedG(PlanarizableNode& N);

    PlanarizableEdge& findEdgeInPlanarizedG(PlanarizableEdge& E);

    void findPlanarSubgraph(
        Graph&                  G,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    );

    void maximalizePlanarSubgraph(
        Graph&                  G,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    );

    void reinsertRemovedEdges(
        Graph&                  G,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    );

#ifdef UNIT_TESTS
friend class JTS_BL_GMW_PlanarizerTests;
#endif
};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEAL_UNDIRECTED_JTS_BL_GMW_PLANARIZER_HPP_*/
