#ifndef _WAILEA_UNDIRECTED_SPQR_PROC4_HPP_
#define _WAILEA_UNDIRECTED_SPQR_PROC4_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"
#include "undirected/spqr_tree.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqr_proc4.hpp
 *
 * @brief merging P nodes and S nodes in an SPQR-tree using DFS.
 *     
 */
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @class SPQR_PROC4_Node
 */
class SPQR_PROC4_Node : public Node {

  public:
    enum type {Unknown, RType, PType, SType};

    inline SPQR_PROC4_Node():mType(Unknown){;}
    inline virtual ~SPQR_PROC4_Node(){;}

    /** @brief type of the original node.
     */
    enum type mType;    
};


/** @class SPQR_PROC4_Edge
 */
class SPQR_PROC4_Edge : public Edge {

  public:
    inline SPQR_PROC4_Edge(){;}
    inline virtual ~SPQR_PROC4_Edge(){;}
};


/** @class SPQRStrategyProc4
 *
 *  @brief the last DFS in which the P nodes and S ndoes are merged in the
 *         SPQR tree.
 */ 
class SPQRStrategyProc4:
                 public DFSStrategy<Graph, SPQR_PROC4_Node, SPQR_PROC4_Edge> {
  public:
    SPQRStrategyProc4(SPQRTree& t):mSPQRTree(t){;}

    inline virtual ~SPQRStrategyProc4(){;}


    void beforeVisitingChild(DNode& v, DEdge& vw) noexcept override
    {
        DNode& w = dynamic_cast<DNode&>(vw.adjacentNode(v));
        if ( v.mType == SPQR_PROC4_Node::SType && 
             w.mType == SPQR_PROC4_Node::SType    ) {
            // Merge v and w.
            mSPQRTree.mergeTwoNodesS(vw.IGBackwardLink(), w.IGBackwardLink());
            w.setIGBackwardLink(v.IGBackwardLink());
        }
        else if ( v.mType == SPQR_PROC4_Node::PType && 
                  w.mType == SPQR_PROC4_Node::PType    ) {
            // Merge v and w.
            mSPQRTree.mergeTwoNodesP(vw.IGBackwardLink(), w.IGBackwardLink());
            w.setIGBackwardLink(v.IGBackwardLink());
        }
    }


  private:
    /** @brief SPQR tree, the output of this class.
     */
    SPQRTree&               mSPQRTree;

#ifdef UNIT_TESTS
  friend class SPQRProc4Tests;
#endif
        
};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEAL_UNDIRECTED_SPQR_PROC4_HPP_*/
