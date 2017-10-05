#ifndef _WAILEA_UNDIRECTED_PLANAR_INSERTION_PATH_FINDER_HPP_
#define _WAILEA_UNDIRECTED_PLANAR_INSERTION_PATH_FINDER_HPP_

#include "undirected/base.hpp"
#include "undirected/planar_dual_graph_maker.hpp"

/**
 * @file undirected/planar_insertion_path_finder.hpp
 *
 * @brief it takes as input a simple biconnected planar graph and its
 *        two features in node or edge, and then finds a suitable route
 *        between them. The route is given in an ordered list of edges
 *        through which an edge can be inserted.
 *
 * @remark
 *
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/** @class PlanarInsertionPathFinder
 *
 *  @brief it takes a planar graph and two end features of node or face,
 *         find a planar embedding and its dual graph, then find the 
 *         shortest path between two faces incident to the features.
 *         The shortest path is given in the ordered list of edges in 
 *         the planar graph.
 *
 *  @remark it uses Node::utility internally.
 */
class PlanarInsertionPathFinder {

  public:
    /** @brief constructor that takes the input graph
     *
     *  @param G (in): simple biconnected planar graph.
     */
    PlanarInsertionPathFinder(Graph& G):mG(G){;}

    ~PlanarInsertionPathFinder(){;}

    /** @brief main function it finds the insertion path between two features.
     *         A feature can be either a node or an edge.
     *         
     *  @param  nIt1 (in): a node on side 1. Only nIt1 or eIt1 can be active.
     *                     it should be G.nodes().second if not active
     *
     *  @param  eIt1 (in): an edge on side 1.
     *                     it should be G.edges().second if not active
     *
     *  @param  nIt2 (in): a node on side 2. Only nIt2 or eIt2 can be active.
     *                     it should be G.nodes().second if not active
     *
     *  @param  eIt2 (in): an edge on side 2.
     *                     it should be G.edges().second if not active
     *
     *  @return list of edges in G across which a new edge that goes through
     *          the two features can be inserted.
     *
     *  @remark it uses Node::utility internally.
     *
     *  @remark on implementaion
     *
     *   1. Find an st-ordering of G between two arbitrary nodes.
     *   2. Find a planar embedding using PQ-tree and the st-ordering.
     *   3. Find a dual graph for the planar embedding.
     *   4  Find the min-dist path on the dual graph between two features
     *      using Dijkstra's algorithm.
     */
    list<edge_list_it_t> findInsertionPath(
        node_list_it_t nIt1, 
        edge_list_it_t eIt1,
        node_list_it_t nIt2, 
        edge_list_it_t eIt2
    );

  private:

    /** @brief performs the followign on G.
     *   1. Find an st-ordering of G between two arbitrary nodes.
     *   2. Find a planar embedding using PQ-tree and the st-ordering.
     *   3. Find a dual graph for the planar embedding.
     */
    void findDualGraph();

    /** @brief finds the two incident faces of the given edge.
     * 
     *  @param eit (in); edge in Gemb.
     *
     *  @return list of faces in Gdual.
     */
    vector<node_list_it_t> incidentFaces(edge_list_it_t eit);

    /** @brief finds all the incident faces of the given node.
     * 
     *  @param eit (in); node in Gemb.
     *
     *  @return list of faces in Gdual.
     */
    vector<node_list_it_t> incidentFaces(node_list_it_t nit);

    /** @brief find a min-dist path between two sets of faces.
     *
     *  @param faces1 (in): set of faces on side 1.
     *
     *  @param faces2 (in): set of faces on side 2.
     *
     *  @return  list of edges on the min-dist on G.
     *
     *  @remark it uses Node::utility internally.
     */
    list<edge_list_it_t> performDijkstra(
                 vector<node_list_it_t> faces1, vector<node_list_it_t> faces2);

    Graph&        mG;
    EmbeddedGraph mGemb;
    DualGraph     mGdual;


#ifdef UNIT_TESTS
  friend class PlanarInsertionPathFinderTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_PLANAR_INSERTION_PATH_FINDER_HPP_*/
