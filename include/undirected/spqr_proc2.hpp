#ifndef _WAILEA_UNDIRECTED_SPQR_PROC2_HPP_
#define _WAILEA_UNDIRECTED_SPQR_PROC2_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"
#include "undirected/spqr_dfs_elements.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqr_proc2.hpp
 *
 * @brief triconnected decomposition procedure 2.
 *     
 * @remarks  this corresponds to "Step 3" in  Hopcroft, John; Tarjan, Robert
 *           (1973), "Dividing a graph into  triconnected components', 
 *           SIAM Journal on Computing 2 (3): 135-158, doi:10.1137/0202012
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

/** @class SPQRStrategyProc2
 *
 *  @brief   the second DFS in which the node number,  lowpt1 and lowpt2 are
 *           recalculated and high points are found.
 *
 *  @remarks this class assumes the new incidence order is stored in 
 *           mOrderedAdjacencyType1 in each node.
 */ 
class SPQRStrategyProc2 :
                     public DFSStrategy<Graph, SPQR_DFS_Node,SPQR_DFS_Edge> {

  public:
    SPQRStrategyProc2():mV(0){;}
    inline virtual ~SPQRStrategyProc2(){;}

    // The following 3 functions are the hook points to DFSExplorer
    void enterNode(DNode& n) noexcept override;
    void processFrond(DNode& n, DEdge& c) override;
    void afterVisitingChild(DNode& n, DEdge& c) noexcept override;

     /** @brief sets the adjacency list of each node to type 1, and
      *         resets mHighPoints, mHighPointReferences, and
      *         mHighPointIterators.
      */
     void prepareNodesForDFS(Graph& g) noexcept;

  private:

    /**  @brief the last number assigned to the node.
    */
    size_t  mV;

#ifdef UNIT_TESTS
  friend class SPQRProc2Tests;
#endif
        
};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQR_PROC2_HPP_*/
