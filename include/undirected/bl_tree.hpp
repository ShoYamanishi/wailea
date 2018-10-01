#ifndef _WAILEA_UNDIRECTED_BL_TREE_HPP_
#define _WAILEA_UNDIRECTED_BL_TREE_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/bl_tree.hpp
 *
 * @brief an implementation of O(|N| + |E|) PQ-tree used by BLPlanarityTester.
 *       
 */
namespace Wailea {

namespace Undirected {

using namespace std;

/**
 * @class BLTree
 *
 * @brief represents a PQ-tree.
 */
class BLTree : public Graph {

  public:

    enum reductionType     {
                            FINAL_REDUCTION,
                            NOT_FINAL_REDUCTION,
                            NORMAL_REDUCTION,
                            COMPLEMENTARY_REDUCTION
                           };

    enum addType           {FULL_END, EMPTY_END};

    enum scanDirectionType {NORMAL_DIRECTION,REVERSED_DIRECTION};

    BLTree();

    virtual ~BLTree();

    void trackQFlippings();

    bool isTrackingQFlippings();

    node_list_it_t nil();

    BLTreeNode& toNodeRef(node_list_it_t nIt);

    bool isNil(node_list_it_t nIt);

    node_list_it_t makePAttachment();

    void initializeForOneIteration();

    void advanceSib(node_list_it_t& prev, node_list_it_t& cur);

    void unlinkFromPTypeParent(list<node_list_it_t>& children);

    /** @brief creates a new P node and put children under.
     *
     *  @param children (in): the children nodes to be put under the new
     *                        P node. They must be either all full or all
     *                        empty.
     */
    node_list_it_t makePNode(list<node_list_it_t>& children);

    node_list_it_t makeQNode();

    node_list_it_t bringUpOnlyChildOfPNodeAndRemove(BLTreeNode& X);
                                          
    void linkSiblings(BLTreeNode& N1, BLTreeNode& N2);

    void unlinkSiblings(BLTreeNode& N1, BLTreeNode& N2);

    void removeVirtualRoot(node_list_it_t vIt);

    void fanOutLeavesFromAttachment(
                           node_list_it_t aIt, list<edge_list_it_t>& edgeList);
                    
    void fanOutLeavesFromQAttachment(
       node_list_it_t aIt, list<edge_list_it_t>& edgeList, node_list_it_t nIt);

    void concatenateOneSinglyPartialToTheOther(
                         BLTreeNode& Main, BLTreeNode& Absorbed, addType type);

    void flattenSinglyPartialChildToQNode(
                        BLTreeNode& P, node_list_it_t cIt, reductionType type);

    bool templateL1(BLTreeNode& X, reductionType reduction);
    bool templateP1(BLTreeNode& X, reductionType reduction);
    bool templateP2(BLTreeNode& X, node_list_it_t& pertinentRoot);
    bool templateP3(BLTreeNode& X, bool& earlyOut);
    bool templateP3Old(BLTreeNode& X, bool& earlyOut);
    bool templateP4(BLTreeNode& X, node_list_it_t& pertinentRoot);
    bool templateP5(BLTreeNode& X, bool& earlyOut);
    bool templateP5Old(BLTreeNode& X, bool& earlyOut);
    bool templateP6(BLTreeNode& X, node_list_it_t& pertinentRoot);
    bool templateP7(BLTreeNode& X, bool& earlyOut);
    bool templateP7ReuseX(BLTreeNode& X, bool& earlyOut);
    bool templateP7Old(BLTreeNode& X, bool& earlyOut);
    bool templateP8(BLTreeNode& X, reductionType reduction, bool& earlyOut);

    bool templateQ1(BLTreeNode& X, reductionType reduction);
    bool templateQ2(BLTreeNode& X, reductionType reduction, bool& earlyOut);
    bool templateQ3(BLTreeNode& X);
    bool templateQ4(BLTreeNode& X, reductionType reduction, bool& earlyOut);
    bool templateQ5(BLTreeNode& X, reductionType reduction, bool& earlyOut);

    bool checkSequenceQ1(BLTreeNode& X);
    bool checkSequenceQ2(BLTreeNode& X);
    bool checkSequenceQ3(BLTreeNode& X);
    bool checkSequenceQ4(BLTreeNode& X);

    void collectGraphEdgesP4(BLTreeNode&X, list<node_list_it_t>&fullChildren);
    void collectGraphEdgesP5(BLTreeNode&X, list<node_list_it_t>&fullChildren);
    void collectGraphEdgesP6Step1(
                             BLTreeNode&X, list<node_list_it_t>&fullChildren);
    void collectGraphEdgesP7Step1(
                             BLTreeNode&X, list<node_list_it_t>&fullChildren);

    void collectGraphEdgesQ2( BLTreeNode& X, node_list_it_t partialIt );
    void collectGraphEdgesQ3( BLTreeNode& X );
    void collectGraphEdgesQ3OnRealRoot   ( BLTreeNode& X );
    void collectGraphEdgesQ3OnVirtualRoot( BLTreeNode& X );
    void collectGraphEdgesQ4( BLTreeNode& X );
    void collectGraphEdgesQ5( BLTreeNode& X );

    node_list_it_t CDPartialRoot();

    void setCollectingEdges();
    bool isCollectingEdges();

#ifdef UNIT_TESTS
    void printEdgeList(list<edge_list_it_t>& edgeList);
    void printTree(ostream& os, node_list_it_t pr);
#endif

#ifdef UNIT_TESTS
  public:
#else
  private:
#endif

    /** @brief indicates the current iteration of PQ-tree reduction.
     *         This is incremented after every bubbleup and reduction.
     *         If BLTreeNode's mGeneration agrees with it,
     *         it is a pertinent node.
     */
    unsigned long long   mGeneration;


    /** @brief set if PQ-tree tracks the orientation of the Q-nodes.
     *         It is used to find an embedding.
     */
    bool                 mTrackQFlippings;


    /** @brief points to the lowest (first) complementarily doubly partial
     *         node found during the template application.
     *         This will be the new tree root.
     */
    node_list_it_t       mCDPartialRoot;


    /** @brief flag to indicate if it is collecting the edges ordering.
     *         Used for finding an embedding.
     */
    bool                 mCollectingEdges;

    /** @brief indicates the immediately following sibling of the top 
     *         pertinent node chain if the pertinent root is virtual.
     *         Used to store the orientation of the collected edges.
     */
    node_list_it_t       mVirtualNextSib;

    /** @brief indicates the assumed orientation of the immediately 
     *         following sibling of the top 
     *         pertinent node chain if the pertinent root is virtual.
     *         Used to store the orientation of the collected edges.
     */
    bool                 mVirtualNextSibAssumeReversed;

friend class BLPlanarityTester;
friend class BLTreeNode;

#ifdef UNIT_TESTS
  friend class BLPlanatiryTesterTests;
#endif

};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_BL_TREE_HPP_*/
