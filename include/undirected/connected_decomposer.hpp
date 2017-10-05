#ifndef _WAILEA_UNDIRECTED_CONNECTED_DECOMPOSER_HPP_
#define _WAILEA_UNDIRECTED_CONNECTED_DECOMPOSER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/connected_component.hpp
 *
 * @brief connected decomposition.
 *     
 * @details
 */

namespace Wailea {

namespace Undirected { 

using namespace std;

/** @class ConnectedDecomposer
 *
 *  @brief it takes a simple graph and decompose into connected components.
 *         each component is given in the set of nodes.
 *
 */
class ConnectedDecomposer {

  public:
    /** @brief performs the decomposition.
     *
     *  @param   g (in):         input graph. It must be simple.
     *
     *  @param   nodeSets (out): lists of vertices. Each list contains the 
     *                           nodes for a connected component.
     *
     *  @remark
     *           If the input graph is not simple, the behavior is undefined.
     *           The required memory and running time will be still in the 
     *           order of O(|N|+|E|).
     *
     *  @remark  If uses Node::utility internally.
     */

    void decompose(Graph& g, vector<vector<node_list_it_t> >& nodeSets);

  private:

#ifdef UNIT_TESTS
  friend class ConnectedDecomposerTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_CONNECTED_DECOMPOSER_HPP_*/

