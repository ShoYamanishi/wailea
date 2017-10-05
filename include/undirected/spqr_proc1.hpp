#ifndef _WAILEA_UNDIRECTED_SPQR_PROC1_HPP_
#define _WAILEA_UNDIRECTED_SPQR_PROC1_HPP_

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
 * @file undirected/spqr_proc1.hpp
 *
 * @brief triconnected decomposition procedure 1.
 *     
 * @remarks  this corresponds to "Step 1" in  Hopcroft, John; Tarjan, Robert
 *           (1973), "Dividing a graph into  triconnected components', 
 *           SIAM Journal on Computing 2 (3): 135-158, doi:10.1137/0202012
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

/** @class SPQRStrategyProc1
 *
 *  @brief the first DFS in which the palm-tree is formed and 
 *         the first version of lowpt1 and lowpt2
 *         are calculated based on arbitrary incidence order in each node.
 */ 
class SPQRStrategyProc1 :
                     public DFSStrategy<Graph, SPQR_DFS_Node,SPQR_DFS_Edge> {

  public:
    SPQRStrategyProc1():mI(0){;}
    inline virtual ~SPQRStrategyProc1(){;}
    // The following 4 functions are the hook points to DFSExplorer
    void enterNode(DNode& n) noexcept override;
    void processFrond(DNode& n, DEdge& c) noexcept override;
    void beforeVisitingChild(DNode& n, DEdge& c) noexcept override;
    void afterVisitingChild(DNode& n, DEdge& c) noexcept override;

  private:

    /**  @brief the last number assigned to the node.
    */
    size_t  mI;

#ifdef UNIT_TESTS
  friend class SPQRProc1Tests;
#endif
        
};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQR_PROC1_HPP_*/
