#ifndef _WAILEA_UNDIRECTED_JTS_TREE_NODE_HPP_
#define _WAILEA_UNDIRECTED_JTS_TREE_NODE_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/jts_tree_node.hpp
 *
 * @brief an implementation of O(|N|^2) PQ-tree used by
 *        JTSPlanarizer.
 *        In this implementation, a tree node, whether it is of type P or Q,
 *        has all the children in std::list, and
 *        all the children point to the correct parent all the time wheather
 *        they are pertinent or not.
 */
namespace Wailea {

namespace Undirected {

using namespace std;

class JTSTree;

/**
 * @class JTSTreeNode
 *
 * @brief represents a PQ-tree node.
 */
class JTSTreeNode : public Node {

  public:

#ifdef UNIT_TESTS
    static int nodeNumMaster;
#endif

    using wha_val_t = signed long;

    inline JTSTreeNode(JTSTree& tree);
    inline virtual ~JTSTreeNode();



  /**
   * ======== Utilities across all the phrases of JTSPlanarizer
   */


    /** @brief returns true if this node is pertinent.
     *         Valid after JTSPlanarizer::bubbleUp() is done.
     */
    inline bool isPertinent();

    /** @brief returns true if this node is of pertinent type full, partial,
     *         or empty. Valid after calculateWHAC() is called for 
     *         this node in JTSPlanarizer::findWHA().
     */
    inline bool isFull();
    inline bool isPartial();
    inline bool isCDPartial();
    inline bool isEmpty();

    node_list_it_t getOnePertinentChild();

    void unlinkFromParent();
    void linkToParent(JTSTreeNode& P, list<node_list_it_t>::iterator pos);
    void linkToParent(JTSTreeNode& P);
    void linkToParentFullEnd(JTSTreeNode& P);
    void linkToParentEmptyEnd(JTSTreeNode& P);


  /**
   * ======== JTSPlanarizer::bubbleUp() specific. =========
   */

    /** @brief makes this node 'pertinent' by bringing mGeneration
     *         up-to-date. It also resets all the fields for pertinent
     *         type, wha values, wha type, and template application.
     *         Used in JTSPlanarizer::bubbleUp()
     */
    inline void resetForBubbleUp();

    inline void incrementPertinentChildrenCountOfParent();



  /**
   * ======== JTSPlanarizer::findWHA() specific. =========
   */

    /** @brief returns true if all the children of this node are of pertinent
     *         type full. Valid after calculateWHAC() is called for 
     *         this node in JTSPlanarizer::findWHA().
     */
    bool areAllChildrenFull();

    /** @brief returns true if all but except for one children are full.
     *         
     *  @param  eIt  (Out): Iterator to the non-fullchild into mChildren list.
     */
    bool areAllChildrenExceptForOneFull(list<node_list_it_t>::iterator &eIt);


    /** @brief calculates W,H,A, and CD values and sets the pertinent type 
     *         of this node
     *         It is done based on its node type, and its childrens' pertinent
     *         types.
     *
     *    W: Number of pertinent leaves removed to make this node nonpertinent.
     *
     *    H: Minimum number of pertinent leaves removed to make this node a
     *       Q node with one-sided partially consecutive pertinent children.
     *       This value is 0 if this is a full node, as it can be considered
     *       one-sided partially consecutive Q node.
     *
     *    A: Minimum number of pertinent leaves removed to make this node a
     *       Q node with consecutive pertinent children in the middle.
     *       This value is 0 if this is a full node, as it can be considered
     *       partially consecutive Q node in the middle.
     *       Also A value never exceeds H as one-sided partially consecutive
     *       Q node.
     *
     *    B is a kind of H, and H is a kind of  A
     *
     *    C: Minimum number of pertinent leaves removed to make this node
     *       CDPartial. If it is impossible to become CDPartial, same as W.
     */
    void calculateWHAC();

        /** @brief calculates W, H, A values of a partial P node.
         *
         *   W: Sum of W of all the pertinent children.
         *   H: Min{Sum of H and Ws : Pick one for H and the rest for W |
         *                                                    partial children}
         *   AdoubleH = Min{Sum of Hs and Ws :
         *                  Pick two for H and the rest for W|
         *                                                    partial children}
         *   AsingleA = Min(Sum of A and Ws : 
         *                  Pick one for A and the rest for W |
         *                                                  pertinent children}
         *
         *   A = Min{AdoubleH, AsingleA} Tie is broken in favor of AsingleA.
         *
         *              / C(Ne): If all except one nodes are Full. Ne indicates
         *   CsingleA = |       the non-full child.
         *              \ Infinity: Otherwise
         *
         *   C = Min{AdoubleH, CsingleA}
         */
        void calculateWHACforP();

        /** @brief calculates W, H, A values of a partial P node.
         *
         *   W: Sum of W of all the pertinent children.
         *   H: Min{Sum of Ws and H :
         *          From one side of the siblings chain, find maximal
         *          consecutive full children and then one partial child if
         *          there is any.
         *          H for the partial child if there is any, and W for the rest
         *          of the children to the other end. }
         *   * H is equal to W if neither end sibling is pertinent.
         *
         *   AsingleA = Min{ Pick one for A and the rest for W | 
         *                                                 pertinent children }
         *   AdoubleH = Min{ Sum of Ws and Pick a maximal consecutive sequence
         *                   of children such that the ends are either partial
         *                   or full, and full children in the middle.
         *                   H for the end children and W for the chilfren that
         *                   are not in the sequence }
         *                 
         *
         *   A = Min{ AdoubleH, AsingleA } Tie is broken in vafor of AsingleA.
         *
         *             / Infinity : If both ends are empty.
         *   CdoubleH =|
         *             \ Sum of W of the pertinent children form Nx to Ny where
         *               Nx is the first child after consecutive full
         *               children and possibly one partial child from end1, and
         *               Ny is the first child after consecutive full
         *               children and possibly one partial child from end2.
         *
         *              / Infinity : If there are more than one non-full child.
         *   CsingleCD =| 0        : If all the children are full
         *              \ C(Nx)    : If there is one non-full child. Nx is the
         *                           non-full child.
         */
        void calculateWHACforQ();

            wha_val_t sumWOfPertinentChildren();
            wha_val_t sumWOfPartialChildren();
            wha_val_t maxWminusAOfPertinentChildren();
            void twoMaxesWminusHOfPartialChildren(
                wha_val_t& max1WminusH,
                wha_val_t& max2WminusH
            );
            wha_val_t calculateHforQ();
            wha_val_t maxSumWminusHOfChildren();
            wha_val_t calculateCDforQ();

  /**
   * ======== JTSPlanarizer::determineWHABNodeTypes() specific. =========
   */


    /** @brief determine the WHABC type of this node if this is a root node.
     *         If it is a full node, then it and all the descendants are of
     *         type B.
     *         If it is not a tree root and  it is a partial node, 
     *         then its type is either H, or A
     *         depending on the cost values mH and mA. Tie is broken in vafor
     *         of A as A is less restrictive than H.
     *         If it is the tree root and it is a partial node, 
     *         then its type is H, A, or CD depending on the cost values
     *         mH, mA, and mCD.
     */
    void determineRootNodeType();


    /** @brief determine the WHAB type of the children based on the
     *         node type and WHAB type of this node.
     *         If WHAB type of this node is B, then all the descendants are
     *         of type B.
     *         If WHAB type of this node is W, then all the descendants are
     *         of type W.
     *         If WHAB type of this node is H or B, then the following rules
     *         described in [JTS89] are applied.
     *
     *         * Node P and H
     *             Full children                     => B
     *             Partial cihldren mChildMax1minusH => H
     *             Other partial children            => W
     *
     *         * Node P and A and mTypeAwithSingleA == true
     *             mChildMaxWmiusA is Full           => B
     *             mChildMaxWmiusA is Partial        => A
     *             Other pertinent children          => W
     *
     *         * Node P and A and mTypeAwithSingleA == false
     *             Full children                     => B
     *             mChildMax1WminusH is Full         => B
     *             mChildMax1WminusH is Partial      => H
     *             mChildMax2WminusH is Full         => B
     *             mChildMax2WminusH is Partial      => H
     *             Other partial children            => W
     *
     *         * Node P and CD and 
     *           all the children except for one are full.
     *             Full children                     => B
     *             The partial child                 => CD
     *
     *          * Node P and CD and otherwise
     *             Full children                     => B
     *             mChildMax1WminusH is Full         => B
     *             mChildMax1WminusH is Partial      => H
     *             mChildMax2WminusH is Full         => B
     *             mChildMax2WminusH is Partial      => H
     *             Other partial children            => W
     *
     *         * Node Q and H and mQHFullOnListHead == true
     *             From the list head until and
     *             excluding mChildBoundaryForQH     => B
     *             mChildBoundaryForQH is Full       => B
     *             mChildBoundaryForQH is Partial    => H
     *             After mChildBoundaryForQH
     *             till the list tail                => W
     *
     *         * Node Q and H and mQHFullOnListHead == false
     *             From the list head until and
     *             excluding mChildBoundaryForQH     => W
     *             mChildBoundaryForQH is Full       => B
     *             mChildBoundaryForQH is Partial    => H
     *             After mChildBoundaryForQH
     *             till the list tail                => B
     *
     *         * Node Q and A and mTypeAwithSingleA == true
     *             mChildMaxWmiusA is Full           => B
     *             mChildMaxWmiusA is Partial        => A
     *             Other pertinent children          => W
     *
     *         * Node Q and A and mTypeAwithSingleA == false
     *             From the list head until and
     *             excluding
     *             mChildBoundaryOnHeadForQA         => W
     *             mChildBoundaryOnHeadForQA is Full => B
     *             mChildBoundaryOnHeadForQA is
     *             Partial                           => H
     *             After mChildBoundaryOnHeadForQA
     *             until and excluing
     *             mChildBoundaryOnTailForQA         => B
     *             mChildBoundaryOnTailForQA is Full => B
     *             mChildBoundaryOnTailForQA is
     *             Partial                           => H
     *             After mChildBoundaryOnHeadForQA
     *             until the list tail               => W
     *
     *         * Node Q and CD and 
     *           all the children except for one are full.
     *             Full children                     => B
     *             The partial child                 => CD
     *          
     *         * Node Q and CD and 
     *           not all the children except for one are full.
     *             From the list head until and
     *             excluding the first encountered empty child
     *             Full children                     => B
     *             Partial children                  => H
     *
     *             From the list tail until and
     *             excluding the first encountered empty child
     *             Full children                     => B
     *             Partial children                  => H
     *          
     *             Middle children in the list       => W
     */
    void determineWHABCTypeForChildren();
        void setBForChildren();
        void setWForChildren();
        void determineWHABCTypeForChildrenOfPTypeH();
        void determineWHABCTypeForChildrenOfPTypeA();
        void determineWHABCTypeForChildrenOfPTypeCD();
        void determineWHABCTypeForChildrenOfQTypeH();
        void determineWHABCTypeForChildrenOfQTypeA();
        void determineWHABCTypeForChildrenOfQTypeCD();

  /**
   * ======== JTSPlanarizer::removeNonconsecutiveNodes() specific. =========
   */
    inline void markGraphEdgeRemoved();
    inline void decrementPertinentChildrenCountOfParent();

  /**
   * ======== JTSPlanarizer::findNewPertinentLeaves() specific. =========
   */
    inline bool isGraphEdgeRemoved();

  /**
   * ======== JTSPlanarizer::applyTemplates() specific. =========
   */
    inline void setFullInParent();
    inline void setPartialInParent();
    inline void setCDPartialInParent();
    void sortFullAndEmptyChildrenOnPNode(
        list<node_list_it_t>&  fullChildren,
        list<node_list_it_t>&  emptyChildren
    );
    inline bool isHeadChildFull();

#ifdef UNIT_TESTS
  public:
#else
  private:
#endif

  /**
   * ======== Valid across all the phrases of JTSPlanarizer
   */

    /** @brief reference to the PQ-tree (JTSTree) that owns this node.
     */
    JTSTree&                     mTree;

    /** @brief PQ-tree node type.
     */
    enum nodeType {TypeUnknown, PType, QType, LType};
    nodeType                       mNodeType;


    /** @brief The list of children
     */
    list<node_list_it_t>           mChildren;


    /** @brief points to the position in mChildren of the parent.
     *         This is used when this node is removed from the tree.
     */
    list<node_list_it_t>::iterator mChildIt;


    /** @brief Parent. 
     */
    node_list_it_t                 mParent;


    /** @brief corresdponding graph edge if this is an L-node.
     */
    edge_list_it_t                 mGraphEdge;


  /**
   * ======== Utility used by JTSTree::queueParentIfNecessaryCountingUp()
   *          and JTSTree::queueParentIfNecessaryCountingDown(). ===========
   */

    /** @brief number of pertinent children processed or unprocessed.
     */
    size_t                         mPertinentChildrenCountProcessed;



  /**
   * ======== Valid after JTSPlanarizer::bubbleUp().
   */

    /** @brief indicates if this is a pertinent node.
     *         the node is pertinent
     *         if this agrees with JTSTree's mGeneration.
     */
    unsigned long long             mGeneration;


    /** @brief number of pertinent children. Valid only if mGeneration
     *         is up-to-date. After JTSPlanarizer::bubbleUp(), this value
     *         stays always correct across the call to JTSPlanarizer::
     *
     */
    size_t                         mPertinentChildrenCount;


  /**
   * ======== Valid after JTSPlanarizer::findWHA()
   *                              until determineWHABNodeTypes() ===========
   */

    /** @brief indicates if the node is full, partial, or empty.
     *         At each iteration, this is first set in JTSPlanarizer::findWHA()
     *         The value may become temporarily invalid during a call to
     *         JSTPlanarizer::removeNonconsecutiveNodes(), when a child of the
     *         node descrements mPertinentChildrenCount, or when a child is
     *         removed, but it will be corrected when the node is processed
     *         in JTSTree::removePertinentNodeIfNecessary().
     */
    enum pertinentType {PertinentUnknown, Full, Partial, CDPartial, Empty};
    pertinentType                  mPertinentType;


    /** @brief  W, H, A, and CD values.
     */
    wha_val_t                      mW;
    wha_val_t                      mH;
    wha_val_t                      mA;
    wha_val_t                      mCD;


    /** @brief the node that has max{W - H}
     */
    node_list_it_t                 mChildMax1WminusH;


    /** @brief the node that has 2nd max {W - H}
     */
    node_list_it_t                 mChildMax2WminusH;


    /** @brief  the node that has max{W - A}
     */
    node_list_it_t                 mChildMaxWminusA;


    /** @brief if this is a Q node, the boundary full or partial node for
     *         type H
     */
    node_list_it_t                 mChildBoundaryForQH;


    /** @brief if this is a Q node, true if the end child for type H is on
     *         the head side of the list.
     */
    bool                           mQHFullOnListHead;


    /** @brief the node for type A closer to head of the list.
     */
    node_list_it_t                 mChildBoundaryOnHeadForQA;


    /** @brief the node for type A closer to tail of the list.
     */
    node_list_it_t                 mChildBoundaryOnTailForQA;


    /** @brief true  if TypeA will be made by a single A.
     *         false if two Hs and possibly some full nodes in between.
     */
    bool                           mTypeAwithSingleA;


  /**
   * ======== Valid after determineWHABNodeTypes() 
   *                             until removeNonconsecutiveNodes() ===========
   */


    /** @brief determined WHAB type
     */
    enum WHABType {WHABUnknown, W, H, A, B, CD};
    WHABType                       mWHABType;


  /**
   * ======== Valid during  applyTemplates() ==========
   */


    /** @brief contains up to two partial children.
     *         Valid only if mGeneration is up-to-date.
     */
    node_list_it_t                 mPartialChild1;
    node_list_it_t                 mPartialChild2;

    /** @brief contains the complementarily partial child
     */
    node_list_it_t                 mCDPartialChild;

    /** @brief number of fullchildren found.
     *         Valid only if mGeneration is up-to-date.
     */
    size_t                         mFullChildrenCount;


  friend class JTSTree;
  friend class JTSPlanarizer;

#ifdef UNIT_TESTS
    void printNode(JTSTree& tree, ostream& os);
  friend class JTSPlanarizerTests;
  int mNodeNum;
#endif


};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_JTS_TREE_NODE_HPP_*/
