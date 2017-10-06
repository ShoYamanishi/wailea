#ifndef _WAILEA_UNDIRECTED_SPQR_PROC3_HPP_
#define _WAILEA_UNDIRECTED_SPQR_PROC3_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"
#include "undirected/spqr_dfs_elements.hpp"
#include "undirected/spqr_tstack.hpp"
#include "undirected/spqr_tree.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqr_proc3.hpp
 *
 * @brief 
 *     
 */
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @class  SPQRNodeMapElem
 *  @brief  holds the mapping from node number to the corresponding 
 *          SPQR_DFS_Node.
 *          It also keeps track of wheather the node has been removed or not
 *          during the DFS exploration.
 */
class SPQRNodeMapElem {
  public:
      node_list_it_t mIt;
      bool           mRemoved;
};

/** @class SPQRStrategyProc3
 *
 *  @brief
 *
 *  @remarks this class assumes the new incidence order is stored in 
 *           mOrderedAdjacencyType2 in each node.
 */ 
class SPQRStrategyProc3 :
                     public DFSStrategy<Graph, SPQR_DFS_Node,SPQR_DFS_Edge> {

  public:

    /// Constructor requires an empty BCtree
    SPQRStrategyProc3(Graph& g, SPQRTree& t)
                     noexcept : mOutgoingPath(false), mG(g), mSPQRTree(t){;}
                   
    virtual ~SPQRStrategyProc3(){;}

    // The following 3 functions are the hook points to DFSExplorer
    void leaveNode(DNode& n) override;             
    void processFrond(DNode& n, DEdge& c) override;
    void beforeVisitingChild(DNode& n, DEdge& c) override;
    void afterVisitingChild(DNode& n, DEdge& c) override;

    /** @brief sets the adjacency list of each node to type 2, and
     *         reset start path of each edge.
     */
    void prepareNodesAndEdges(Graph& g) noexcept;


  private:

    void printEStack();

    void preadvance(DNode& n, DEdge& c);
    Edge& checkForType2s(DNode& n, DEdge& c);
    void checkForType1(DNode& n, DEdge& c);


    /** @brief  pops elements from TStack while b.parent = a.
     *
     *  @param  vNum (in): node number of the current node v.
     *
     *  @return true if TStack has been updated.
     */
    bool popInvalidTStackElements(size_t vNum);

    edge_list_it_t processUpwardTriangle(
                         DNode& v, DEdge& vw, DNode& w, DEdge& wx, DNode& x);

    edge_list_it_t processSeparationComponentByHAB(DNode& n, DEdge& c);

    /** @brief removes the specified node from the high points list of the
     *         relevant nodes.
     */
    void removeHighPoints(DNode& n);


    /** @brief add edge to the graph beween n1 and n2. The position in the
     *         incidence list is determined by the reference edge refEdge.
     *         if refEdge is incident to n1 or n2, then the new edge will be
     *         inserted right before refEdge. Otherwise, the new edge will be
     *         inserted at the beginning of the incidence list.
     *
     *  @param g  (in): the graph to which a new edge is created and added.
     *
     *  @param n1 (in): the node to which a new edge is inserted.
     *
     *  @param n2 (in): the other node to which a new edge is inserted
     *
     *  @param refEdge (in): the reference edge.
     *
     *  @param t (in): type of the edge, frond or tree arc.
     *
     *  @return the edge created and added.
     */
    edge_list_it_t createAndAddDEdgeNextTo(
        Graph& g,
        Node&  n1,
        Node&  n2,
        Edge&  refEdge,
        SPQR_DFS_Edge::type t);


    /** @brief remove the edges that have been copied to a separation 
     *         component. It also removes the nodes that have become 
     *         isolated. The high point lists of the other nodes are updated
     *         to reflect the removal.
     *
     *  @param edges (in): edges to be removed.
     */
    void removeSubgraph(vector<edge_list_it_t>& edges);


    /** @brief create a map from mNum to the iterator to its node.
     *
     *  @param g (in): The input graph that has SPQR_DFS_Nodes with mNum set.
     */
    void createNodeMap(Graph &g);


    /** @brief stays true as long as the DFS exploration goes along
     *         the direction of edges. Used to detected the first edge
     *         of a new path.
     */
    bool                    mOutgoingPath;

    /** @brief the input graph explored.
     */
    Graph&                  mG;

    /** @brief SPQR tree, the output of this class.
     */
    SPQRTree&               mSPQRTree;

    /** @brief mapping from the node number to the node iterator.
     */
    vector<SPQRNodeMapElem> mNMap;

    /** @brief ESTACK.
     */
    vector<edge_list_it_t>  mEStack;

    /** @brief TSTACK.
     */
    TStack                  mTStack;


    /** @brief indicates if a new virtual frond (v[_/>w) is to be inserted.
     *  In this case, v must be inserted to w's high points at a correct
     *  location in the list.
     */
    bool                    mHasNewHighPt;

    /** @brief the new high point that replaces mHighPtReplaced.
     *  Used to insert a virtual frond dynamically in removeSubgraph().
     */
    node_list_it_t          mNewHighPtIt;

    /** @brief the owner of the high points above.
     *  Used to insert a virtual frond dynamically in removeSubgraph().
     */
    node_list_it_t          mHighPtOf;


#ifdef UNIT_TESTS
  friend class BCTreeTests;
  friend class SPQRProc3Tests;
#endif
        
};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQR_PROC3_HPP_*/
