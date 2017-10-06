#ifndef _WAILEA_UNDIRECTED_SPQR_DFS_ELEMENTS_HPP_
#define _WAILEA_UNDIRECTED_SPQR_DFS_ELEMENTS_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

namespace Wailea {

namespace Undirected { 

using namespace std;

class SPQR_DFS_Edge;

/** @class SPQR_DFS_Node
 *
 *  @brief a graph node to be explored by BCStrategy.
 *         This class is augumented to DFSNode<SPQR_DFS_Node,SPQR_DFS_Edge>
 *         for DFSExplorer<Grah,SPQR_DFS_Node,SPQR_DFS_Edge>.
 */
class SPQR_DFS_Node : public Node {

  public:
    inline SPQR_DFS_Node():mNum(0),mLowPt1(0),mLowPt2(0),mNd(0){;}

    inline virtual ~SPQR_DFS_Node(){;}

    inline SPQR_DFS_Edge& adjacentEdge(SPQR_DFS_Edge& e);

    /** @brief node number that has the following characteristics.
     *         v < w <=  v-*->w
     *         v < w <=  v-*->u & v-*->w & v is visted ealier than w.
     */
    size_t               mNum;

    /** @brief the lowest possible node reachable from this node along
     *         the orientation of the edges in the palm-tree.
     */
    size_t               mLowPt1;

    /** @brief the second lowest possible node reachable from this node along
     *         the orientation of the edges in the palm-tree.
     */
    size_t               mLowPt2;

    /** @brief parent edge of this node in the palm-tree.
     */ 
    edge_list_it_t       mParentEdge;

    /** @brief number of descendants including itself in the palm-tree.
     */
    size_t               mNd;

    /** @brief the adjacency order for DFS Proc 2.
     */
    list<edge_list_it_t> mOrderedAdjacencyType1;
    list<edge_list_it_t> mOrderedAdjacencyType1Back;

    /** @brief the adjacency order for DFS Proc 3.
     */
    list<edge_list_it_t> mOrderedAdjacencyType2;
    list<edge_list_it_t> mOrderedAdjacencyType2Back;

    /** @brief the list of adjacent nodes of the indicent fronds
     *         in the reverse orientation.
     */
    list<node_list_it_t> mHighPoints;

    /** @brief the list of node numbers of nodes to which this node is listed
     *         in their mHighPoints. This is used to dynamically remove
     *         the nodes and update mHighPoints.
     */
    vector<size_t> mHighPointReferences;
                        

    /** @brief the list of iterators at which this node is listed in
     *         their mHighPoints. This is used to dynamically remove
     *         the nodes and update mHighPoints.
     */
    vector<list<node_list_it_t>::iterator>
                         mHighPointIterators;
};


/** @class SPQR_DFS_Edge
 *
 *  @brief a graph node to be explored by BCStrategy.
 *         This class is augumented to DFSNode<SPQR_DFS_Node,SPQR_DFS_Edge>
 *         for DFSExplorer<Grah,SPQR_DFS_Node,SPQR_DFS_Edge>.
 */
class SPQR_DFS_Edge : public Edge {

  public:
    enum type {
        Unknown,
        TreeArcType,
        FrondType,
    };

    inline SPQR_DFS_Edge():
                  mType(Unknown),mPhi(0),mStartPath(false),mIsVirtual(false){;}
    inline virtual ~SPQR_DFS_Edge(){;}

    enum type mType;

    size_t    mPhi;

    bool      mStartPath;

    /** @brief indicates if this is a virtual edge of not.
     */
    bool      mIsVirtual;

    /** @brief the pair edge if this is virtual.
    */
    edge_list_it_t mVirtualPairEdge;

    /** @brief the graph to which the pair edge belongs to if this is virtual
     */
    node_list_it_t mVirtualPairTreeNode;

};


inline SPQR_DFS_Edge& SPQR_DFS_Node::adjacentEdge(SPQR_DFS_Edge& e){

    if (this->degree() == 0) {
        throw std::invalid_argument(Constants::kExceptionEdgeNotInGraph);
    }
    auto itPair = this->incidentEdges();
    for(auto it = itPair.first ; it != itPair.second; it++) {
        SPQR_DFS_Edge& a = dynamic_cast<SPQR_DFS_Edge&>(*(*(*it)));
        if (&a !=  &e) {
            return a;
        }
    }
    return e;
}


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQR_DFS_ELEMENTS_HPP_*/
