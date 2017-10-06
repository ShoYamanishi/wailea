#ifndef _WAILEA_UNDIRECTED_TREE_SPLITTER_HPP_
#define _WAILEA_UNDIRECTED_TREE_SPLITTER_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/tree_splitter.hpp
 *
 * @brief It takes a set of cut edges, splits the tree into 
 *        components and finds the corresponding component nodes for each 
 *        component. 
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/** @class TreeSplitter
 *
 *  @brief it splits a tree into connected components for a given cutset.
 *
 */
class TreeSplitter {

  public:
    /** @brief constructor
     *
     *  @param tree (in): tree
     */
    TreeSplitter(Graph& tree);

    /** @brief destructor
     */
    ~TreeSplitter();

    /** @brief initialize the graph for decomposition.
     *         
     *  @param cutSet (in): cut set
     *
     *  @remark it calls Node::pushUtiltiy(), and Edge::pushUtility() in thre tree
     */
    void prepareTree(list<edge_list_it_t>& cutSet);

    /** @brief find a connected component from a node.
     *
     *  @param startIt (in): the start node to explore the graph in BFS.
     *
     *  @return list of nodes discovered for the component.
     */
    list<node_list_it_t> findComponent(node_list_it_t startIt);

    /** @brief clean up the tree after finishing calling
     *         findComponent().
     *         It pops utility from Nodes and Edges of tree.
     */
    void cleanUpTree();

  private:

    Graph& mTree;

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEAL_UNDIRECTED_TREE_SPLITTER_HPP_*/
