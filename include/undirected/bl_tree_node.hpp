#ifndef _WAILEA_UNDIRECTED_BL_TREE_NODE_HPP_
#define _WAILEA_UNDIRECTED_BL_TREE_NODE_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/bl_tree_node.hpp
 *
 * @brief an implementation of O(|N|+|E|) PQ-tree used by
 *        BLPlanarityTester.
 *        In this implementation, a tree node, whether it is of type P or Q,
 *        has all the children in std::list, and
 *        all the children point to the correct parent all the time wheather
 *        they are pertinent or not.
 */
namespace Wailea {

namespace Undirected {

using namespace std;

class BLTree;

/**
 * @class BLTreeNode
 *
 * @brief represents a PQ-tree node.
 */
class BLTreeNode : public Node {

#ifdef UNIT_TESTS
    static int nodeNumMaster;
#endif

  public:

    /** @brief PQ-tree node type.
     */
    enum nodeType {TypeUnknown, PType, QType, LType, VirtualRootType};

    inline BLTreeNode(BLTree& tree) noexcept;
    inline virtual ~BLTreeNode();

    /** @brief makes this node 'pertinent' by bringing mGeneration
     *         up-to-date. It also resets all the fields for pertinent
     *         type, wha values, wha type, and template application.
     *         Used in BLPlanarityTester::bubbleUp()
     */
    inline void resetForBubbleUp() noexcept;

    inline bool hasBeenQueuedForBubbleUp() const noexcept;

    inline bool isOnQueue() const noexcept;

    inline bool isPertinent() const noexcept;

    /** @brief returns true if this node has been unblocked in bubble-up
     */
    inline bool isUnblocked() const noexcept;

    /** @brief returns true if this node has been blocked in bubble-up
     */
    inline bool isBlocked() const noexcept;

    /** @brief blocks this node.
     */
    inline void block();

    /** @brief unblock this node. It also increment the number of
     *         pertinent children of the parent.
     *
     *  @param parentIt (in): pointer to the parent.
     */
    inline void unblock(node_list_it_t parentIt);

    /** @brief unblock this node. It also increment the number of
     *         pertinent children of the parent.
     *         The parent has already been set.
     */
    inline void unblock();

    /** @brief PQ-tree node type.
     */
    inline enum nodeType nodeType() const noexcept {return mNodeType; }


    /** @brief returns true if this node is of pertinent type full, partial,
     *         or empty. Valid after the node is processed in
     *         BLPlanarityTester::applyTemplates().
     */
    inline bool isFull() const noexcept;
    inline bool isSinglyPartial() const noexcept;
    inline bool isCDPartial() const noexcept;
    inline bool isEmpty() const noexcept;


    /** @brief discards old fullChildren link if there is one.
     */
    inline void discardOldFullLink();

    /** @brief release the full children links to the children
     */
    inline void clearFullChildren();

    /** @brief discards old fullChildren link if there is one.
     */
    inline void createFullLink(BLTreeNode& P);

    /** @brief  sets the full child pointer to mFullChild of this node.
     *          It gets repeatedly overwritten during a call to 
     *          BLPlanarityTester::applyTemplates().
     */
    inline void setFullInParent();

    /** @brief  sets a singly partial child pointer to this node.
     *
     *  @return true:  Set successful
     *          false: There are already two partial children set.
     *                 The tree can't be reduced.
     */
    inline bool setSinglyPartialInParent();

    /** @brief  sets a complementarily doubly partial child pointer to this
     *          node.
     *         
     *
     *  @return true:  Set successful
     *          false: There is already a partial child set.
     *                 The tree can't be reduced.
     */
    inline bool setCDPartialInParent();

    inline void sortFullAndEmptyChildrenOnPNode(
        list<node_list_it_t>&  fullChildren,
        list<node_list_it_t>&  emptyChildren
    );

    inline void addTwoInitialChildrenToQType(
                                       node_list_it_t fIt, node_list_it_t eIt);

    inline void unlinkFromPTypeParent();
    inline void linkToPTypeParent(BLTreeNode& P);

    inline void unlinkFromQTypeParent(node_list_it_t pIt);


    inline void linkToQTypeParentToEnd(BLTreeNode& P, BLTree::addType type);

    inline bool isEndChild1Full();

    inline void resetToPAttachment() noexcept;

    inline void transferGraphNodeOrientationsFrom(
                        BLTreeNode& P, BLTree::scanDirectionType type);

    inline void solveAssumedGraphNodeOrientationsFrom(
                        BLTreeNode& P, BLTree::scanDirectionType type);

    inline void assumeGraphNodeOrientationsFrom(
                        BLTreeNode& P, BLTree::scanDirectionType type);

#ifdef UNIT_TESTS
  public:
#else
  private:
#endif

    /** @brief reference to the PQ-tree (JTSTree) that owns this node.
     */
    BLTree&                       mTree;

    enum nodeType                 mNodeType;


    /** @brief The list of children for P-node
     */
    list<node_list_it_t>           mChildren;

    /** @brief number of children in mChildren.
     *         This is needed as mChildren.size() may not be
     *         O(1) operation.
     */
    size_t                         mChildrenCount;

    /** @brief points to the position in mChildren of the parent.
     *         This is used when this node is removed from the tree.
     */
    list<node_list_it_t>::iterator mChildIt;


    /** @brief poitns to both end children if this is a Q-node.
     */
    node_list_it_t                 mEndChild1;
    node_list_it_t                 mEndChild2;


    /** @brief points to the siblings if this is a child of a Q-node.
     */
    node_list_it_t                 mSibling1;
    node_list_it_t                 mSibling2;


    /** @brief Parent. Valid only if the parent is a P node, or
     *                 if this is an end node of the parent, or
     *                 if this becomes unblocked during bubble-up.
     */
    node_list_it_t                 mParent;

    /** @brief corresdponding graph edge if this is an L-node.
     */
    edge_list_it_t                 mGraphEdge;


    /** @brief indicates if this is a pertinent node.
     *         the node is pertinent
     *         if this agrees with JTSTree's mGeneration.
     */
    unsigned long long             mGeneration;


    /** @brief indicates if this pertinent node is examined but blocked.
     */
    bool                           mBlocked;


    /** @brief indicates if this pertinent node is unblocked and found
     *         its parent or not.
     */
    bool                           mUnblocked;

    /** @brief number of pertinent children. Valid only if mGeneration
     *         is up-to-date. After JTSPlanarizer::bubbleUp(), this value
     *         stays always correct across the call to JTSPlanarizer::
     *
     */
    size_t                         mPertinentChildrenCount;


    /** @brief number of pertinent children processed or unprocessed.
     */
    size_t                         mPertinentChildrenCountProcessed;


    /** @brief number of pertinent leaves under this node.
     *         This is used to determine if the node is the pertinent root
     *         or not in BLPlanarityTester::applyTemplates().
     *         The lifetime of the value is within a call of BLPlanarityTester
     *         ::applyTemplates()
     *         The value is originated from the pertinent roots up to the
     *         pertinent root. At each node, the value is accumulated from 
     *         its children when they are taken off the queue, and the value
     *         is passed on to its parent before any template is applied.
     *         This means the templates don't have to worry about this
     *         variable.
     */
    size_t                         mPertinentLeavesCount;


    /** @brief indicates if the node is full, singly partial, 
     *         complementarily doubly partial, or empty.
     *         This is set and used in BLTree::applyTemplates()
     */
    enum pertinentType {
        PertinentUnknown,
        Full,
        SinglyPartial,
        DoublyPartial,
        CDPartial,
        Empty
    };
    pertinentType                  mPertinentType;

    /** @brief contains up to two singly partial children.
     *         Valid only if mGeneration is up-to-date.
     */
    node_list_it_t                 mSinglyPartialChild1;
    node_list_it_t                 mSinglyPartialChild2;


    /** @brief points to a complementarily doubly partial child.
     *         Valid only if mGeneration is up-to-date.
     */
    node_list_it_t                 mCDPartialChild;


    /** @brief contains full children
     *         It is used as the starting point to find the consecutive
     *         pertinent nodes of Q node is either end child is non-pertinent.
     */
   list<node_list_it_t>            mFullChildren;

   /** @brief the following 3 are required for the child side to maintain the 
    *         integrity of the data structure.
    */

   /** @brief node in which this node is listed in mFullChilden.
    */
   node_list_it_t                  mFullChildrenParent;

   /** @brief iterator into mFullChildren in the node
    *         specified by mFullChildrenParent.
    */
   list<node_list_it_t>::iterator  mFullChildrenIt;

    /** @brief set if the value of mFullChildrenIt is valid.
     */
   bool                            mFullChildrenSet;

    /** @brief Usage of
     *            mOrientInNorm,
     *            mOrientInReversed,
     *            mOrientOutNorm,
     *            mOrientOutReversed,
     *            mAssumedOrientOutNorm, and
     *            mAssumedOrientOutReversed.
     *
     *         findEmbedding() finds an embedding as in the cyclic ordered
     *         incident edges per node. Such cyclic ordered edges are found
     *         using two passes of PQ-tree reductions.
     *
     *         Before any PQ-tree reductions, each (unordered) incident edges
     *         list Ne(n) of node n is split into two lists: Nei(n) and Neo(n).
     *         Nei(n) contains the edges whose adjacent node has lower
     *         st-number. They are also denoted by 'incoming edges' of n.
     *         Neo(n) contains the edges whose adjacent node has higher
     *         st-number. They are also denoted by 'outgoing edges' of n.
     *         In the 1st pass, the nodes are scanned in the ordering
     *         from s to to, and at each iteration for node n, an ordering 
     *         Ordi(n) of Nei(n) is found during one PQ-reduction for n.
     *         Ordi(n) will be used in the final embedding found, but there is
     *         an ambiguity: It can take either of two orientations:
     *         For example, if Nei(1) has {2,3,4,5}, and Ordi(n) is found to 
     *         be (5->2->4->3), then the final embedding can take either
     *         (5->2->4->3) or (3->4->2->5). This ambiguity is solved at the
     *         2nd pass in the reversed s,t-order from t to s.
     *  
     *         In the 2nd pass, at the beginning of each iteration, the
     *         leaves that correspond to Ordi(n) are fanned-out from a
     *         node. This latter node is of Q-type since, the order of the 
     *         edges have been determined as in Ordi(n) with the ambiguity of
     *         the orientation. This Q-node may be flipped during the following
     *         PQ-reductions throught the application of the templates.
     *         If the Q-node is flipped, the corresponding Ordi(n) must be 
     *         flipped. Such an orientation information is kept in Q-node's
     *             mOrientInNorm and 
     *             mOrientInReversed;
     *         When Ordi(n) is fanned-out from the attachment Q-node, the 
     *         edges from the beginning of Ordi(n) to the end are fanned out
     *         from mEndChild1 to mEndChild2. The corresponding graph node n
     *         is put into mOrientInNorm.
     *         We here call it 'natural orientation of Q-nodes is
     *         from end1 to end2' for later convenience.
     *        
     *         Later the Q-node may be absorbed to another Q-node, and it may
     *         be flipped w,r,t to the absorbing Q-node. The following are
     *         the occasions where a Q-node is absorbed to another.
     * 
     *         P6: One singly partial Q-node is concatenated to the other.
     *         P7: One singly partial Q-node is concatenated to the other.
     *         Q2: One singly partial Q-node is absorbed to its parent Q-node.
     *         Q3: One or two singly partial Q-node is absorbed to its parent.
     *         Q4: One or two singly partial Q-node is absorbed to its parent.
     *         
     *         In those cases, the orientation information kept in the absorbed
     *         Q-node is transferred to the absorbing Q-node.
     *         If it is absorbed without flipping, then the transfer is :
     *
     *         mOrientInNorm => mOrientInNorm
     *         mOrientInReversed => mOrientInReversed
     *
     *         If it is with a flipping, then the transfer is crossed like:
     *
     *         mOrientInNorm => mOrientInReversed
     *         mOrientInReversed => mOrientInNorm
     *
     *         At the end of the 2nd pass, there will be only one Q-node 
     *         remaining in the PQ-tree, and the final orientation information
     *         of Ordi(n) will be retrieved from those two lists of the Q-node.
     *
     *         The orientation of Ordo(n), which is an ordered list of Neo(n)
     *         (and Ordo(n) itself) is found in the same way.
     *
     *         At each iteration of the 2nd pass for graph node n, the
     *         pertinent leaves that correspond to the edges in Neo(n) are
     *         consecutively arranged before reduction. If the pertinent root
     *         is of L-type or P-type, an arbitrary order and orientation can
     *         be taken for Ordo(n). If the pertinent root is of Q-type, then
     *         the orientation of Ordo(n) must be associated to the root 
     *         Q-node. The order of Ordo(n) is assumed to be natural (end1->
     *         end2), and the node n is put into mOrientOutNorm.
     *         Later, when the Q-node is absorbed to another, 
     *         mOrientOutNorm & mOrientOutReversed
     *         are transferred to the ones in the absorbing Q-node in the same
     *         way as in Ordi(n).
     *
     *         There is one complication to keep the complexity in O(|n|).
     *         If a virtual root is introduced during a bubble-up, and the
     *         final template is Q3, then we don't know the true pertinent
     *         root which is a Q-node. Suqsequently we don't know the natural
     *         direction of the sibling chain. If this is the case, we assume
     *         the direction in which the consecutive nodes are collected to be
     *         the natural direction, and pass that assumption information of
     *         Ordo(n) onto the immediately following sibling.
     *         For example, 
     *                               Q01 (unknown)
     *           End1_________________|_________________End2
     *            |    |    |    |    |    |    |    |    |
     *           L01  L02 *L03 *L04 *L05 *Q06  L07  L08  L09
     *        
     *         In this case, L03, L04, L05, and Q06 are the pertinent nodes.
     *         Since we don't know the true parent (Q01) is unknown and hence
     *         we don't know which direction is natural, we assume one 
     *         direction to be natural.
     *         In this example (Q06->L05->L04->L03) is assumed to be natural
     *       
     *                               Q01 (unknown)
     *           End1_________________|_________________End2
     *            |                                      |
     *           L01 L02(*L03<-*L04<-*L05<-*Q06)L07 L08 L09
     *
     *         This orientation information about Ordo(n) is passed on to L02.
     *         The graph n is put into either of 
     *         mAssumedOrientOutNorm & mNodesBOPassedBySiblings in L02.
     *
     *         L02 takes either of the following two arrangements:
     * 
     *           Sib1 <- L02 -> Sib2(L03)  or  Sib2 <- L02 -> Sib1(L03).
     *
     *         Here, we introduce another 'natural' order for the sibling
     *         links. We assume Sib1->This->Sib2 to be the natural orientation,
     *         i.e., Sib1->This->Sib2 is aligned with the parents end1->end2
     *         direction.
     *         With this in mind, if L02's Sib1 is L03, the last node of the
     *         pertinent node chain, then we assume the orientation of the
     *         pertinent node chain is aligned with L02's natural orientation,
     *         and we put n to mAssumedOrientOutNorm.
     *         On the other hand, if L02's Sib2 is L03, then we put n to 
     *         mNodesBOPassedBySiblings.
     *
     *         Later, when L02's true parent (Q01) becomes known, the nodes
     *         in mAssumedOrientOutNorm & mNodesBOPassedBySiblings are 
     *         transferred to the parent's
     *         mOrientOutNorm & mOrientOutReversed.
     *         depending on whether the natural orientation of L03 agrees with
     *         Q01's or not.
     *         The following are the occasions in which the node's true parent
     *         is found and hence the assumptions stored in
     *         mAssumedOrientOutNorm & mNodesBOPassedBySiblings are solved
     *         and transferred to Q-node's
     *         mOrientOutNorm & mOrientOutReversed.
     *
     *         Q1: Scan all the full children from end1 to end2.
     *         Q2: Scan either from end1 or end2 depending on which side
     *             is full.
     *         Q3: If a virtual root is not introduced, then Q3 have either
     *             full or singly partial nodes at both ends.
     *             Scan from end1 to end2.
     *         Q4: Scan from end1 to the end pertinent child, and then scan
     *             from end2 to the end of pertinent child.
     *         Q6: Scan from end1 to end2.
     *
     *         If, however, L02 gets removed in Q3 with a virtual root, then
     *         the assumptions in 
     *         mAssumedOrientOutNorm & mNodesBOPassedBySiblings must be 
     *         transferred. In this case, in the same way as L03, the contens
     *         of the lists are transferred to the 1st non-pertinent node
     *         immediately following the end of the pertinent node in the 
     *         pertinent chain.
     */
    list<node_list_it_t> mOrientInNorm;
    list<node_list_it_t> mOrientInReversed;
    list<node_list_it_t> mOrientOutNorm;
    list<node_list_it_t> mOrientOutReversed;
    list<node_list_it_t> mAssumedOrientInNorm;
    list<node_list_it_t> mAssumedOrientInReversed;
    list<node_list_it_t> mAssumedOrientOutNorm;
    list<node_list_it_t> mAssumedOrientOutReversed;


  friend class BLTree;
  friend class BLPlanarityTester;
  friend class PStackElem;

#ifdef UNIT_TESTS
  friend class BLPlanarizerTests;
  int mNodeNum;
#endif


};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_BL_TREE_NODE_HPP_*/
