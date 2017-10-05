#ifndef _WAILEA_UNDIRECTED_GM_EDGE_INSERTER_HPP_
#define _WAILEA_UNDIRECTED_GM_EDGE_INSERTER_HPP_

#include "undirected/base.hpp"
#include "undirected/bctree.hpp"

/**
 * @file undirected/gmw_edge_inserter.hpp
 *
 * @brief
 *
 * @remark
 *
 *
 * =============================
 *   Overview of the algorithm
 * =============================
 *
 * Let the input connected planar graph G.
 *
 * Let s and d be the nodes to which a new edge {s,d} is added in G.
 *
 * Let L an ordered list of graph edges. L is initially empty.
 * GMWEdgeInserter finds a suitable route in a planar embedding of G
 * along which {s,d} can be added with as few edge crossings as possible
 * using the algorithm proposed  in [GMW05].
 * The edges in L in the order specifies the route as follows.
 *
 *      e1  e2 ... em
 *       |  |      |
 *  s----+--+--...-+---d
 *       |  |      |
 *
 * Overview of the algorithm
 *   First, G is decomposed into bionnected components in a BC-tree, and then
 *   find the block (biconnected component) to which s and d belong.
 *   Second, decomponse the block into triconnecte components in an SPQR-tree,
 *   then arranged the triconnected components in the order along the path 
 *   from s to t. Each triconnecte component has entering and leaving features
 *   (node or edge).
 *   Third, for each triconnected component, find a planar embedding, and
 *   find a route in its dual graph with minimum number of crossings.
 *   The ordered list of crossing edges are returned as the route.
 *
 * Steps.
 *
 * 1. Find a BC-tree (biconnected decomposition tree) of G
 *
 * Find the path on the BC-tree (B1,B2,...Bm) from s to d.
 *
 *       B1-----a1----B2-----a2  ... am-1---Bm
 *    --------     --------              --------
 *   |s     a1|   |a1    a2| ...     ...|am-1   d|
 *    --------     --------              --------
 *
 * Here ai denotes (copy of) cut vertices on which the path enters or leaves
 * the blocks.
 * Note s can be cut vertex. So can d.
 * If both s and d are cut vertices, the edge can be inserted between them
 * without breaking planarity of G.
 *
 * If s or d is a cut vertex, then it can be reachable from any cut vertex or
 * any block without breaking planarity, and hence there is nothing to do for 
 * the node.
 * 
 * Here we assume s is not a cut vertex.
 *
 * Let s belong to block B1, and let a1 be the cut-vertex on the path to d.
 * Block B1 is further decomposed into a SPQR-tree.
 *
 * Find the path on the SPQR-tree from s to a1.
 * Decompose the tree into indivisual connected components along the path
 * by removing all the tree edges along the path.
 *
 *       C1---------C2------...----Ck
 *    --------   --------       --------
 *   |s     e1|=|e1    e2| ... |ek-1  a1|
 *    --------   --------       --------
 *
 * From C1 to Ck, process each tree node as follows.
 * Expand Ci's skeleton into S2
 * Take C2 for example.
 *                _||_
 *                \  /
 *      S2         \/
 *       __________________________
 *      /   ___  __  __      ___   \
 *     |   /   \/  \/  \    /   \   |
 *     |  e1 f1| f2| f3|...|fn  e2  |
 *     |   \___/\__/\__/    \___/   |
 *      \__________________________/
 *         fx
 *
 * NOTE1: Node Ci's skeleton consist of components of multiple tree nodes
 * reachable from Ci without passing the edges on the path on the SPQR-tree.
 * NOTE2: If Ci is P or S, skip it, as the graph edge {s,d} can be embedded 
 * in one face of it without breaking planarity within Ci.
 *
 * Find a planar embedding of S2 and its dual graph D2.
 *
 * Let f1 a face incident to e1, and f2 to e2.
 * (In case of C1 and Ck, let f1 be incident to s, a1, am-1, or d.)
 * Find the shortest path from f1 to fn on D2.      
 *
 * Make the path minimal in terms of the incidence to e1 and e2.
 * For example, both f1 and f2 can be incident to e1. In this case discard
 * f1 and its incident dual edge form the path to make f2 the end face.
 *
 *          f1---*---*---*---*--fn
 *
 * Gather the ordered list of dual edges (e'1,e'2,...e'n) on the path,
 * and append the corresponding graph edges  to L maintaining the order.
 * If a skeleton has virtual edges, then they do not exist in th original 
 * graph. Those edges are not include in L.
 * 
 * Process C1, C2, ... Ck in the same way.
 * If node d is not a cut-vertex, process the corresponding block Bm 
 * in the same way.
 *
 * ===================
 *   Data structures
 * ===================
 *
 * GMWEdgeInserter [To be reviewed and unit tested]
 *                    - Top level class to implement the functionality.
 *    It uses the following:
 *      GMWBCPath in gmw_bc_path.hpp
 *      GMWSkeletonGenerator in gmw_skeleton_generator.hpp
 *      PlanarInsertionPathFinder in planar_insertion_path_finder.hpp
 *
 * GMWBCPath [DONE]   - Find the BC-path between s and d
 *                      and returns the corresponding blocks in
 *                      Block [BlockNode, BlockEdge]
 *    It uses the following:
 *      BiconnectedDecomposer in bctree.hpp
 *      TreePathFinder in tree_path_finder.hpp
 *      
 * GMWSkeletonGenerator [To be reviewed and unit-tested]
 *                    - Takes a block with two terminal nodes, and generates
 *                      the corresponding expanded skeletons between them
 *                      in a list of GMWSkeleton objects
 *
 *    It uses the following:
 *      SPQRDecomposer in spqr_decomposer.hpp
 *      TreePathFinder in tree_path_finder.hpp
 *      TreeSplitter   in tree_splitter.hpp
 *      BlockNode::utility to find the nodes in the skeleton without
 *          redundancy.
 *                             
 * TreePathFinder [DONE]
 *                    - Find a path between two nodes in the tree.
 *                    - DFS* to implement DFS exploration.
 *
 * TreeSplitter   [DONE]
 *                    - Takes a set of cut edges, splits the tree into 
 *                      components and finds the corresponding component nodes
 *                      for each component.
 *    It uses the following:
 *      Node::utility as a flag to indicate if the node has been discovered 
 *          or not.
 *      Edge::utility to indicate if it is a cut or not.
 *
 * PlanarInsertionPathFinder [DONE]
 *                    - Takes a block (biconnected graph) with two designated 
 *                      vertices s and d, and find an ordered set of edges
 *                      along which {s,d} can be inserted.
 *                      It uses STNumbering to find an st-ordering between 
 *                      s and d.
 *    It uses the following:
 *      BLPlanarityTester in bl_planarity_tester.hpp
 *      PlanarDualGraphMaker in planar_dual_graph_maker.hpp
 *      EmbeddedFace::utility to keep track of the current minimum distance 
 *          when performing Dijkstra's Dynamic Programming.
 *
 * @references
 *  [GMW05] Inserting an Edge into a Planar Graph
 *          Carsten Gutwenger, Petra Mutzel, Rene Weiskircher
 *           Algorithmica 41(4):289-308, April 2005
 *
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/** @class GMEdgeInserter
 *
 *  @brief performs Gutwenger, Mutzel, Weiskircher's edge insertion.
 *         It takes as its input a simple connected planar graph G and
 *         two designated nodes N1 and N2, and then finds a route
 *         through which an edge can be inserted. The route is given in 
 *         an ordered list of edges.
 */
class GMWEdgeInserter {

  public:
    /** @brief top level function. It takes a simple connected planar graph 
     *         G and its two nodes N1 and N2, and generates a list of 
     *         edges along which {N1,N2} can be added.
     *
     *  @param G  (in): Graph 
     *
     *  @param N1 (in): Node 1
     *
     *  @param N2 (in): Node 2
     *
     */
    void findInsertionPath(Graph& G, Node&  N1, Node&  N2);

    /** @brief returns the path as in the ordered set of edges in the original
     *         input graph
     */
    list<edge_list_it_t> getPath();

  private:

    /** @brief finds the partial insertion path for the given block between
     *         two designated nodes.
     *
     *  @param B     (in): block
     *
     *  @param bn1It (in): terminal node 1
     *
     *  @param bn2It (in): terminal node 2
     */
    void processBlock(Block& B, node_list_it_t bn1It, node_list_it_t bn2It);


    /** @brief holds the insersion path as in an ordered list of edges of the
     *         original graph
     */
    list<edge_list_it_t> mInsertionPath;

#ifdef UNIT_TESTS
  friend class GMWEdgeInserterTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_GM_EDGE_INSERTER_HPP_*/
