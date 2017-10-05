#ifndef _WAILEA_UNDIRECTED_JTS_PLANARIZER_HPP_
#define _WAILEA_UNDIRECTED_JTS_PLANARIZER_HPP_
#include "undirected/base.hpp"
#include "undirected/planarizable_graph.hpp"
/**
 * @file undirected/jts_planarizer.hpp
 *
 * @brief the header file the users of JTSPlanarizer inluce.
 *        It finds a planar connected spnanning subgraph Gp for a given 
 *        {s,t}-orientable graph G. The removed edges are returned 
 *        as in the complementary graph to Gp.
 *        It needs an {s,t}-ordering (bipolar orientation) of G.
 *        It can be found by STNumbering class defined in st_numbering.hpp
 *
 * @remark
 *  For a given st-ordering, it finds a planar spanning subgraph Gp of G
 *  using [JTS89]'s first step called PLANARIZE. This runs in O(|N|^2).
 *
 *  The planar graph Gp obtained by PLANARIZE in [JTS89] may not be
 *  biconnected and there can be some nodes that have no outgoing edges
 *  with respect to the orientation induced by the st-ordering. Such nodes
 *  can be considered as sink nodes.
 *  The resultant graph is a spanning, connected upward planar subgraph
 *  with the single source node s, and possibly multiple sink nodes
 *  including t.
 *
 *  If Gp has multiple sink nodes in terms of the given st-ordering, but
 *  it is bi-connected, we can find another st-ordering for Gp with the same
 *  s and t.
 *
 * @remark on the implementation
 *   dependency: combinatorial/undirected/base.{hpp,cpp}
 *
 *   file organization:
 *     * combinatorial/undirected/jts_planarizer.hpp
 *                                - The users of JTSPlanarizer are to 
 *                                  include this file.
 *
 *     * combinatorial/undirected/jts_planarizer.cpp
 *         JTSPlanarizer          - top level implementation class
 *
 *         CopiedGraph[Node,Edge] - used to return the planar subgraph
 *                                    and removed edges.
 *
 *         JTSGraph[Node,Edge]    - used internally in JTSPlanarizer
 *                                    as a copy of the input graph.
 *
 *     * combinatorial/undirected/jts_tree.{hpp,cpp}
 *         JTSTree                - implements PQ-tree operations.
 *
 *     * combinatorial/undirected/jts_tree_node.{hpp,cpp}
 *         JTSTreeNode            - implements PQ-tree node data structure
 *                                  and operations.
 *     * combinatorial/undirected/jts_inline.hpp
 *                                - inline function definitions for JTSTree
 *                                  and JTSTreeNode.
 *
 *
 * @usage
 *
 *   JTSPlanarizer pl;
 *   Graph         G;  // The input {s,t}-orientable graph for which
 *                     // a planar subgraph is found.
 *   list<node_list_it_t> stOrder
 *                     // {s,t}-orientation of the nodes of G.
 *
 *   Graph planarSubgraph; // The planar subgraph is stored here.
 *   Graph removedEdges;   // The removed edges are stored here.
 *
 *   pl.findUpwardPlanarSingleSourceSubgraph
 *                                 (G, stOrder, planarSubgraph, removedEdges);
 * 
 *
 * @references
 *  [BL76] Testing for the consecutive ones property, interval graphs, and
 *       graph planarity using PQ-tree algorithms
 *       Kellogg S. Booth & George S. Lueker
 *       Journal of Computer and System Sciences archive Volume 13 Issue 3,
 *       December, 1976 Pages 335-379 Academic Press, Inc. Orlando, FL, USA
 *
 *  [JTS89] O(n2) Algorithms for Graph Planarization
 *       R. Jayakumar, K. Thulasiraman, M.N.S. Swamy,
 *       IEEE Transactions on Computer-aided Design, Vol 8. No. 3, March 1989
 */

namespace Wailea {

namespace Undirected {

using namespace std;


/**
 * @class JTSGraph
 *
 * @brief the input graph is augumented to this class internally
 *        to be processed by PQ-tree.
 */
class JTSGraph : public Graph {
  public:
    /** @brief pointer to the original target graph.
     */
    Graph*  mOriginal;
};


/**
 * @class JTSGraphEdge
 *
 * @brief the input graph edges are augumented to this class internally
 *        to be processed by JTSPlanarizer.
 */
class JTSGraphEdge : public Edge {

  public:

    /** @brief indicates if this edge is removed or not.
     */
    bool           mRemoved;

    /** @brief pointer to the corresponding leaf in the PQ-tree.
     */
    node_list_it_t mPQLeaf;

};


/**
 * @class JTSGraphNode
 *
 * @brief the input graph nodes are augumented to this class internally
 *        to be processed by JTSPlanarizer.
 */
class JTSGraphNode : public Node {

  public:

    /** @brief st-number of this node.
     */
    size_t               mStNumber;


    /** @brief incoming incident edge set w.r.t. the st-ordering.
     */
    list<edge_list_it_t> mIncomingEdges;


    /** @brief outgoing incident edge set w.r.t. the st-ordering.
     */
    list<edge_list_it_t> mOutgoingEdges;

};

class JTSTree;
class JTSTreeNode;

/** @class JTSPlanarizer
 *
 *  @brief top level class to find a spanning connected planar subgraph Gp
 *         of a given G and an st-ordering.
 *
 */
class JTSPlanarizer {

  public:

    static const utility_t   kForwardTypeInPlanarSubgraph;
    static const utility_t   kForwardTypeRemoved;

    /** @brief find a spanning connected planar subgraph of a given
     *         biconnected graph.
     *
     *  @param G              (in):  biconnected graph to be planarized
     *
     *  @param stOrder        (in):  (s,t)-ordering of the graph g.
     *
     *  @param planarSubgraph (out): planar subgraph.
     *
     *  @param removedEdges   (out): removed edges
     *                               Complement of planarSubgraph in G.
     *     
     *  @remark on the inter-graph links
     *          If an edge is in planar subgraph, then
     *            - Edges::IGForwardLink() in G points to the corresponding
     *              node in planarSubgraph.
     *            - Edges::utility() in G has kForwardTypeInPlanarSubgraph.
     *          If an edge is removed, then
     *            - Edges::IGForwardLink() in G points to planarSubgraph's
     *              edges().second.
     *            - Edges::utility() in G has kForwardTypeRemoved.
     *
     *          Nodes::IGBackwardLink() in planarSubgraph and removedEdges
     *          point to the corresponding Node in G.
     *          Edges::IGBackwardLink() in planarSubgraph and removedEdges
     *          point to the corresponding Edge in G.
     *
     *  @throws bad_alloc()
     *          if there is a memory shortage during a call.
     *
     *  @remark on exception safety
     *          If an exception is thrown, all the data structures internally
     *          allocated during a call will be freed.
     *
     *  @remark on undefined behavior
     *          If G is not (s,t)-orientable, or if stOrder is not a correct
     *          (s,t)-ordering the behavior is undefined.
     *
     *  @remark on complexity
     *          The required memory and running time will be still in the 
     *          order of O(|N|^2).
     */
    void findUpwardPlanarSingleSourceSubgraph(
        Graph&                  G,
        vector<node_list_it_t>& stOrder,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges   );

  private:


    /** @brief make an internal copy of the input graph. The copy is in
     *         It also makes a bipolar (st) orientation of the copied nodes.
     *
     *  @param   srcGraph      (in):  input graph G
     *
     *  @param   stOrderSrc    (in):  bipolar orientation for G
     *
     *  @param   dstGraph      (out): copy of G
     *
     *  @param   stOrderDst    (out): bipolar orientation for the nodes in
     *                                dstGraph
     *
     *  @param   pqTree        (in):  JTSTree instance.
     *
     */
    void copyInputGraph(
        Graph&                  srcGraph,
        vector<node_list_it_t>& stOrderSrc,
        JTSGraph&               dstGraph,
        vector<node_list_it_t>& stOrderDst,
        JTSTree&                pqTree
    );


    /** @brief   find a set of PQ-nodes of L type that correspond to
     *           the given set of graph edges.
     *
     *  @param   pqTree          (in): PQ-tree (JTSTree)
     *
     *  @param   edgeList        (in): graph edges
     *
     *  @return  corresponding PQ-tree nodes of L type.
     *
     */
    vector<node_list_it_t> fromGraphEdgesToPQLeaves(
        JTSTree&              pqTree,
        list<edge_list_it_t>& edgeList
    );


    /** @brief   performs bubble-up() of [JTS89]. Bottom up.
     *
     *  @param   pqTree          (in): PQ-tree (JTSTree)
     *
     *  @param   pertinentLeaves (in): the pertinent leaves of the PQ-tree.
     *
     *  @return  the pertinent root.
     *
     *  @remarks the purpose of this function is to find the pertinent
     *           subtree whose nodes are identified by their mGeneration.
     *           Also, the number of pertinent chldren of each pertinent node
     *           is found to mPertinentChildrenCount.
     *
     *           This function returns te pertinent root of the pertinent 
     *           subtree of the PQ-tree.
     */
    node_list_it_t bubbleUp(
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves
    );


    /** @brief   put the parent node of the given node to the queue
     *           if the conditions are met. This is called by bubble-up().
     *
     *  @param   Q         (in): FIFO queue in bubble-up()
     *
     *  @param   pqTree    (in): PT-tree used in bubble-up()
     *
     *  @param   X         (in): PQ-tree node
     *
     *  @remarks the parent of a pertinent node is put to the queue when
     *           the any of its pertinent children has been dequeued and is
     *           about to be processed.
     *           This condition is detected if the parent has an old value
     *           in its mGeneration or not. The parent is reset and put to
     *           the queueu before the child is processed.
     */
    void queueParentIfNecessaryBubbleUp(
        list<node_list_it_t>& Q,
        JTSTree&              pqTree,
        JTSTreeNode&          X
    );


    /** @brief   find W, H, A values defined in [JTS89] for the pertinent
     *           PQ-tree nodes. Bottom up.
     *
     *  @param   pqTree          (in): PQ-tree.
     *
     *  @param   pertinentLeaves (in): pertinent leaves given to bubble-up().
     *
     *  @return  pertinentRoot   (in): pertinent root for the given pertinent
     *                                 subtree of the PQ-tree.
     *
     *  @remarks from the pertinent leaves up to the pertinent root, find
     *           the mW, mH, and mA values of each node based on its node type
     *           and its childrens mW, mH, and mA values.
     *           It also sets the pertinent node type (Full or Partial) 
     *           of all the pertinent nodes.
     */
    void findWHA(
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t          pertinentRoot
    );


    /** @brief   put the parent node of the given node to the queue
     *           if all of its pertinent children are to have been processed.
     *           This is used in findWHA() and applyTemplates().
     *
     *
     *  @param   Q         (in): FIFO queue
     *
     *  @param   pqTree    (in): PQ-tree
     *
     *  @param   X         (in): PQ-tree node
     *
     *  @remarks on imeplementation
     *    It uses mPertinentChildrenCountProcessed of JTSTreeNode.
     *    It assumes mPertinentChildrenCount is correctly set for the
     *    pertinent nodes, and mPertinentChildrenCountProcessed is zero
     *    when findWHA() or applyTemplates() starts. Whenever a pertinent
     *    child has taken off from the queue, and  is about to be processed
     *    it increments  mPertinentChildrenCountProcessed.
     *    If mPertinentChildrenCountProcessed becomes equal to
     *    mPertinentChildrenCount, then the parent will be put to the queue.
     */
    void queueParentIfNecessaryCountingUp(
        list<node_list_it_t>& Q,
        JTSTree&              pqTree,
        JTSTreeNode&          X
    );


    /** @brief   determines the WHAB types of the pertinent nodes top-down
     *           from the pertinent root according to [JTS89].
     *           After the call, mWHABType of the pertinent nodes are set.
     *
     *  @param   pqTree        (in): PQ-tree (JTSTree)
     *
     *  @param   pertinentRoot (in): pertinent root for the given pertinent
     *                               subtree of the PQ-tree.
     */
    void determineWHABCNodeTypes(
        JTSTree&       pqTree,
        node_list_it_t pertinentRoot
    );


    /** @brief   remove the non-consecutive pertinent nodes that do not
     *           conform to the WHAB types determined for each parent node.
     *
     *  @param   pqTree          (in):     PQ-tree (JTSTree)
     *
     *  @param   pertinentLeaves (in):     pertinent leaves given to bubble-up.
     *
     *  @param   pertinentRoot   (in/out): pertinent root of the pertinent
     *                                     subtree. On return it has the
     *                                     pertinent root for the new reducible
     *                                     pertinent tree.
     *
     *  @remarks this operation may remove some of the pertinent leaves and 
     *           nodes, and then mark the graph edges of JTSGraphEdge 'removed'
     *           to find a planar subgraph.
     *           - If a pertinent L-node is marked 'W', it is removed from the
     *             tree.
     *           - If a pertinent node has no longer any child, then it is 
     *             removed from the tree.
     *           - If a pertinent node has one child, its contents are braught
     *             up to the parent and the child is removed.
     *           - If a pertinent node has no longer any pertinent child, 
     *             it is pertinent type is set to 'empty'.
     *
     *           Throughout the operation mPertinentChildrenCount of the
     *           pertinent nodes may change when their chilfren are removed,
     *           but their values stay correct.
     *           The values of mPertinentChildrenCount of pertinent nodes can
     *           be changed as this function uses
     *           queueParentIfNecessaryCoutingDown, which does not depend on
     *           mPertinentChildrenCount,  to put the parents to the queue.
     */
    void removeNonconsecutiveNodes(
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         pertinentRoot
    );


    /** @brief   put the parent node of the given node to the queue
     *           if all of its pertinent children are to have been processed.
     *           This is used in removeNonconsecutiveNodes().
     *
     *
     *  @param   Q         (in): FIFO queue.
     *
     *  @param   pqTree    (in): PT-tree (JTSTree)
     *
     *  @return  X         (in): PQ-tree node
     *
     *  @remarks on imeplementation
     *    It uses mPertinentChildrenCountProcessed of JTSTreeNode.
     *    It assumes mPertinentChildrenCount is correctly set for the
     *    pertinent nodes, and mPertinentChildrenCountProcessed is equal to
     *    mPertinentChildrenCount when removeNonconsecutiveNodes() starts.
     *    Whenever a pertinent child has taken off from the queue,
     *    and  is about to be processed it decrements
     *    mPertinentChildrenCountProcessed.
     *    If mPertinentChildrenCountProcessed becomes 0, then the parent will
     *    be put to the queue.
     */
    void queueParentIfNecessaryCountingDown(
        list<node_list_it_t>& Q,
        JTSTree&              pqTree,
        JTSTreeNode&          X
    );


    /** @brief applies the templates from the pertinent leaves up to
     *         the pertinent root.
     *         After a call to it, all the pertinent leaves are consecutively
     *         arranged.
     *
     *  @param   pqTree          (in): PQ-tree
     *
     *  @param   pertinentLeaves (in): Pertinent leaves not removed.
     *                                 
     *  @param   pertinentRoot   (in): Pertinent root
     *
     *  @return  new pertinent root after the reductions have been made.
     */
    node_list_it_t applyTemplates(
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t          pertinentRoot
    );


    /** @brief removes the pertinent subtree of the PQ-tree after the reduction
     *         have been made. After a call to this, the subtree is replaced
     *         with an empty P-node, which will be used as the attachment node
     *         for the new L nodes.
     *
     *  @param   pqTree          (in): PQ-tree
     *
     *  @param   pertinentRoot   (in): root of the pertinent subtree to be
     *                                 removed
     *
     *  @return  attachment node for the new L nodes
     */
    node_list_it_t removePertinentTree(
        JTSTree&       pqTree,
        node_list_it_t pertinentRoot
    );


    /** @brief make a subgraph of the input graph to
     *         be returned to the user.
     *
     *  @param originalGraph   (in):  original graph
     *
     *  @param internalGraph   (in):  internal copy of the input graph that has
     *                                removed edge information in mRemoved
     *                                of JTSGraphEdge objects.
     *
     *  @param planarSubgraph  (out): copy of the planar subgraph found.
     *
     *  @param removedEdges    (out): removed edges as in the complement of
     *                                planarSubgraph.
     */
    void makePlanarSubgraphAndRemovedEdges(
        Graph&                  originalGraph,
        JTSGraph&               internalGraph,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    );


    /** @brief make a copy of g, assuming g is already planar (up to k4).
     *
     *  @param originalGraph   (in):  original graph
     *
     *  @param planarSubgraph  (out): copy of the planar subgraph found.
     */
    void handlek0tok4(
        Graph&                  g,
        PlanarizableGraph&      planarSubgraph
    );

#ifdef UNIT_TESTS
  friend class JTSPlanarizerTests;
#endif

};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_JTS_PLANARIZER_HPP_*/
