#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/bctree.hpp"
#include "undirected/spqr_dfs_elements.hpp"
#include "undirected/spqr_proc4.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/tree_path_finder.hpp"

#include "../src/undirected/dfs.cpp"

namespace Wailea {

namespace Undirected {

/** @brief list of classes with actual template parameter types.
 *         this is needed to let the compiler know what actual
 *         classes will be instantiated.
 *         We could avoid this file by moving all the implementation of 
 *         dfs.cpp to dfs.hpp, but this may cause a code bloat depending
 *         on the compiler.
 */

template class DFSExplorer<Graph,Node,Edge>;
template class DFSStrategy<Graph,Node,Edge>;
template class DFSGraph<Graph,Node,Edge>;
template class DFSNode<Graph,Node,Edge>;
template class DFSEdge<Graph,Node,Edge>;


// Used by undirected/bctree.hpp
template class DFSExplorer<Graph, 
                           BiconnectedDecomposer::BCDFSNode,
                           BiconnectedDecomposer::BCDFSEdge  >;

template class DFSStrategy<Graph,
                           BiconnectedDecomposer::BCDFSNode,
                           BiconnectedDecomposer::BCDFSEdge  >;

template class DFSGraph   <Graph,
                           BiconnectedDecomposer::BCDFSNode,
                           BiconnectedDecomposer::BCDFSEdge  >;

template class DFSNode    <Graph,
                           BiconnectedDecomposer::BCDFSNode,
                           BiconnectedDecomposer::BCDFSEdge  >;

template class DFSEdge    <Graph,
                           BiconnectedDecomposer::BCDFSNode,
                           BiconnectedDecomposer::BCDFSEdge  >;


// Used by undirected/spqr_decomposer.hpp
template class DFSExplorer<Graph, 
                           SPQR_DFS_Node,
                           SPQR_DFS_Edge  >;

template class DFSStrategy<Graph,
                           SPQR_DFS_Node,
                           SPQR_DFS_Edge  >;

template class DFSGraph   <Graph,
                           SPQR_DFS_Node,
                           SPQR_DFS_Edge  >;

template class DFSNode    <Graph,
                           SPQR_DFS_Node,
                           SPQR_DFS_Edge  >;

template class DFSEdge    <Graph,
                           SPQR_DFS_Node,
                           SPQR_DFS_Edge  >;

// Used by undirected/spqr_proc4.hpp
template class DFSExplorer<Graph, 
                           SPQR_PROC4_Node,
                           SPQR_PROC4_Edge  >;

template class DFSStrategy<Graph,
                           SPQR_PROC4_Node,
                           SPQR_PROC4_Edge  >;

template class DFSGraph   <Graph,
                           SPQR_PROC4_Node,
                           SPQR_PROC4_Edge  >;

template class DFSNode    <Graph,
                           SPQR_PROC4_Node,
                           SPQR_PROC4_Edge  >;

template class DFSEdge    <Graph,
                           SPQR_PROC4_Node,
                           SPQR_PROC4_Edge  >;


// Used by undirected/st_numbering.hpp
template class DFSExplorer<Graph, 
                           ST_DFS_Node,
                           ST_DFS_Edge  >;

template class DFSStrategy<Graph,
                           ST_DFS_Node,
                           ST_DFS_Edge  >;

template class DFSGraph   <Graph,
                           ST_DFS_Node,
                           ST_DFS_Edge  >;

template class DFSNode    <Graph,
                           ST_DFS_Node,
                           ST_DFS_Edge  >;

template class DFSEdge    <Graph,
                           ST_DFS_Node,
                           ST_DFS_Edge  >;

// Used by undirected/tree_path_finder.hpp
template class DFSExplorer<Graph, 
                           TPF_DFS_Node,
                           TPF_DFS_Edge  >;

template class DFSStrategy<Graph,
                           TPF_DFS_Node,
                           TPF_DFS_Edge  >;

template class DFSGraph   <Graph,
                           TPF_DFS_Node,
                           TPF_DFS_Edge  >;

template class DFSNode    <Graph,
                           TPF_DFS_Node,
                           TPF_DFS_Edge  >;

template class DFSEdge    <Graph,
                           TPF_DFS_Node,
                           TPF_DFS_Edge  >;

}// namespace Undirected

}// namespace Wailea



