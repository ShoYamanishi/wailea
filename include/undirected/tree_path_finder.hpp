#ifndef _WAILEA_UNDIRECTED_TREE_PATH_FINDER_HPP_
#define _WAILEA_UNDIRECTED_TREE_PATH_FINDER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/tree_path_finder.hpp
 *
 * @brief find the path from two given nodes on a tree.
 *     
 *  @remarks class diagram
 *
 *    
 *    TreePathFinder
 *        defines->TreePathFinderStrategy
 *        defines->TPF_DFS_Node
 *        defines->TPF_DFS_Edge
 *        owns->DFSExplorer<Graph,TPF_DFS_Node,TPF_DFS_Edge>
 *        owns->SPQRStrategy<Graph,TPF_DFS_Node,TPF_DFS_Edge>
 */
 
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @class TPF_DFS_Node
 *
 *  @brief a node edge to be explored by TreePathFinderStrategy.
 *         This class is augumented to DFSNode<TPF_DFS_Node,TPF_DFS_Edge>
 *         for DFSExplorer<Graph,TPF_DFS_Node, TPF_DFS_Edge>.
 */
class TPF_DFS_Node : public Node {

  public:
    /** @brief indicates the edge from which this node is being explored.
     */
    edge_list_it_t mBackEdge;
};


/** @class TPF_DFS_Edge
 *
 *  @brief an edge edge to be explored by TreePathFinderStrategy.
 *         This class is augumented to DFSNode<TPF_DFS_Node,TPF_DFS_Edge>
 *         for DFSExplorer<Grah,TPF_DFS_Node,TPF_DFS_Edge>.
 */
class TPF_DFS_Edge : public Edge {};


/** @class  TreePathFinder
 *  @brief  takes two designated nodes on a rootless tree and finds the path
 *          between them in an ordered list of edges.
 *          
 *  @details 
 *     
 *      Complexities:
 *        It uses heap memory in the order of O(|N|+|E|).
 *        It runs in the order of  O(|N|+|E|).
 */
class TreePathFinder {

  public:

    using DNode = DFSNode<Graph, TPF_DFS_Node, TPF_DFS_Edge>;
    using DEdge = DFSEdge<Graph, TPF_DFS_Node, TPF_DFS_Edge>;

    /** @brief find the path between two nodes of a rootless tree.
     *
     *  @param   T (in):       input rootless tree. 
     *                         It must be simple and biconnected.
     *
     *  @param   s (in):       an end node.
     *
     *  @param   t (in):       another end node.
     *
     *  @param   nodes (out):  ordered list of nodes on the path from s to t
     *
     *  @param   edges (out):  ordered list of edges on the path from s to t.
     *
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed.
     *
     *  @remark undefined behavior
     *           If the input graph is not simple, the behavior is undefined.
     *           If the input graph is connected, the behavior is undefined.
     *
     */
    void findPath(

        Graph&                T,
        Node&                 s,
        Node&                 t,
        list<node_list_it_t>& nodes,
        list<edge_list_it_t>& edges

    );

  private:

    /** @brief   creates a copy of the input graph for DFS exploration.
     *
     *  @param   Gorg  (in):  input graph
     *  @param   Sorg  (in):  input node s
     *  @param   Torg  (in):  input node t
     *
     *  @param   Gcopy (out): copied graph
     *  @param   Scopy (out): corresponding node in the copied graph
     *  @param   Tcopy (out): corresponding node in the copied graph
     */
    void copyInputGraph(

        Graph&          Gorg,
        node_list_it_t  Sorg,
        node_list_it_t  Torg,

        Graph&          Gcopy,
        node_list_it_t& Scopy,
        node_list_it_t& Tcopy

    );

#ifdef UNIT_TESTS
  friend class TreePathFinderTests;
#endif
        
};

}// namespace Undirected

}// namespace Wailea


#endif /*_WAILEA_UNDIRECTED_TREE_PATH_FINDER_HPP_*/
