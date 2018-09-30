#ifndef _WAILEA_UNDIRECTED_BL_INLINE_HPP_
#define _WAILEA_UNDIRECTED_BL_INLINE_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/bl_inline.hpp
 *
 * @brief inline function definitions of bl_tree.hpp, bl_tree_node.hpp.
 */
namespace Wailea {

namespace Undirected {

using namespace std;


inline BLTreeNode::BLTreeNode(BLTree& t) noexcept :mTree(t)
{

    mNodeType                      = TypeUnknown;
    mEndChild1                     = t.nil();
    mEndChild2                     = t.nil();
    mSibling1                      = t.nil();
    mSibling2                      = t.nil();
    mParent                        = t.nil();
    mGeneration                    = 0;
    mBlocked                       = false;
    mUnblocked                     = false;
    mPertinentChildrenCount        = 0;
    mPertinentChildrenCountProcessed = 0;
    mPertinentLeavesCount          = 0;
    mPertinentType                 = PertinentUnknown;
    mSinglyPartialChild1           = t.nil();
    mSinglyPartialChild2           = t.nil();
    mCDPartialChild                = t.nil();

    mFullChildrenSet               = false;

    mChildrenCount                 = 0;

#ifdef UNIT_TESTS
    mNodeNum                       = ++nodeNumMaster;
#endif
}


inline BLTreeNode::~BLTreeNode(){;}


/** @brief resets this node for a bubbleup
 */
inline void BLTreeNode::resetForBubbleUp() noexcept
{
    mGeneration                      = mTree.mGeneration;
    mBlocked                         = false;
    mUnblocked                       = false;
    mPertinentType                   = PertinentUnknown;
    mPertinentChildrenCount          = 0;
    mPertinentChildrenCountProcessed = 0;
    mPertinentLeavesCount            = 0;
    mSinglyPartialChild1             = mTree.nil();
    mSinglyPartialChild2             = mTree.nil();
    mCDPartialChild                  = mTree.nil();

    clearFullChildren();
    discardOldFullLink();
}


inline bool BLTreeNode::hasBeenQueuedForBubbleUp() const noexcept {
    return mGeneration == mTree.mGeneration;
}


inline bool BLTreeNode::isPertinent() const noexcept {
    return mGeneration == mTree.mGeneration;
}


/** @brief returns true if this node has been unblocked in bubble-up
 */
inline bool BLTreeNode::isUnblocked() const noexcept {
    return (mGeneration == mTree.mGeneration) && mUnblocked;
}


/** @brief returns true if this node has been blocked in bubble-up
 */
inline bool BLTreeNode::isBlocked() const noexcept {
    return (mGeneration == mTree.mGeneration) && mBlocked;
}


/** @brief returns true if this node is on queue in bubble-up
 */
inline bool BLTreeNode::isOnQueue() const noexcept {
    return (mGeneration == mTree.mGeneration) && !mBlocked && !mUnblocked;
}



/** @brief unblock this node. It also increment the number of 
 *         pertinent children of the parent.
 *         The parent node must have been reset and queued.
 *
 *  @param parentIt  (in): externally supplied pointer to the parent.
 */
inline void BLTreeNode::unblock(node_list_it_t parentIt) {

    mBlocked   = false;
    mUnblocked = true;
    mParent    = parentIt;
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(parentIt);
        P.mPertinentChildrenCount++;
    }

}


/** @brief unblock this node if it already knows its parent.
 *         It also increment the number of 
 *         pertinent children of the parent.
 *         The parent node must have been reset and queued.
 */
inline void BLTreeNode::unblock() {
    mBlocked   = false;
    mUnblocked = true;
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        P.mPertinentChildrenCount++;
    }
}

/** @brief blocks this node.
 */
inline void BLTreeNode::block() {
    mBlocked   = true;
    mUnblocked = false;
}



inline bool BLTreeNode::isFull() const noexcept {
    return mGeneration == mTree.mGeneration &&
           mPertinentType == Full;
}


inline bool BLTreeNode::isSinglyPartial() const noexcept {
    return mGeneration == mTree.mGeneration &&
           mPertinentType == SinglyPartial;
}


inline bool BLTreeNode::isCDPartial() const noexcept {
    return mGeneration == mTree.mGeneration &&
           mPertinentType == CDPartial;
}


inline bool BLTreeNode::isEmpty() const noexcept {
    return mGeneration < mTree.mGeneration ||
           mPertinentType == Empty;
}


/** @brief discards old fullChildren link if there is one.
 */
inline void BLTreeNode::discardOldFullLink() {

    if(mFullChildrenSet) {


        auto& oldP = mTree.toNodeRef(mFullChildrenParent);
//cerr << "Discarding " << mNodeNum << " from " << oldP.mNodeNum << "\n";
        oldP.mFullChildren.erase(mFullChildrenIt);
        mFullChildrenSet = false;
    }
}

/** @brief release the full children links to the children
 */
inline void BLTreeNode::clearFullChildren() {
    for (auto fit : mFullChildren) {
        auto& C = mTree.toNodeRef(fit);
        if (!C.mFullChildrenSet) {
            ;// Must not reach.
//cerr << "SOMETHING IS WRONG WITH " << C.mNodeNum << " from " << mNodeNum << "\n";
        }
        else {
//cerr << "Unlinking " << C.mNodeNum << " from " << mNodeNum << "\n";
            C.mFullChildrenSet = false;
        }
    }
    mFullChildren.clear();
}

/** @brief discards old fullChildren link if there is one.
 */
inline void BLTreeNode::createFullLink(BLTreeNode& P) {

//cerr << "Linling " << mNodeNum << " to " << P.mNodeNum << "\n";

    // Create new link.
    mFullChildrenIt = P.mFullChildren.insert(P.mFullChildren.end(),backIt());
    mFullChildrenParent = P.backIt();
    mFullChildrenSet = true;

}




/** @brief increments the full children count in the parent and sets
 *         the full child pointer to this node.
 */
inline void BLTreeNode::setFullInParent() {

    discardOldFullLink();
    auto& P = mTree.toNodeRef(mParent);
    createFullLink(P);

}

inline bool BLTreeNode::setSinglyPartialInParent() {

    discardOldFullLink();

    auto& P = mTree.toNodeRef(mParent);

    if (mTree.isNil(P.mSinglyPartialChild1)) {

        P.mSinglyPartialChild1 = backIt();
        return true;

    }
    else if (mTree.isNil(P.mSinglyPartialChild2)) {

        P.mSinglyPartialChild2 = backIt();
        return true;

    }
    else {

        // There are more than 2 singly partial children under the parent.
        // It can't be reduced.
        return false;

    }

    return true;
}


inline bool BLTreeNode::setCDPartialInParent() {

    discardOldFullLink();

    auto& P = mTree.toNodeRef(mParent);

    if (mTree.isNil(P.mCDPartialChild)) {

        P.mCDPartialChild = backIt();
        return true;

    }
    else {

        return false;

    }
}


/** @brief makes the node to a singly partial Q type 
 *         with one full and one empty children.
 *
 *  @param fIt  (in): pointer to full child
 *
 *  @param eIt  (in): pointer to empty child
 */
inline void BLTreeNode::addTwoInitialChildrenToQType(
    node_list_it_t fIt,
    node_list_it_t eIt
) {


    auto& F = mTree.toNodeRef(fIt);
    auto& E = mTree.toNodeRef(eIt);

    discardOldFullLink();
    F.discardOldFullLink();
    E.discardOldFullLink();

    mChildren.clear();
    mChildrenCount          = 0;
    mEndChild1              = fIt;
    mEndChild2              = eIt;

    F.createFullLink(dynamic_cast<BLTreeNode&>(*this));

    mPertinentChildrenCount = 1;
    mPertinentLeavesCount   = F.mPertinentLeavesCount;
    mPertinentType          = SinglyPartial;

    F.mSibling1             = mTree.nil();
    F.mSibling2             = eIt;
    E.mSibling1             = fIt;
    E.mSibling2             = mTree.nil();
    F.mParent               = backIt();
    E.mParent               = backIt();

}


inline void BLTreeNode::unlinkFromPTypeParent()
{
    discardOldFullLink();

    if (!mTree.isNil(mParent)) {

        auto& P = mTree.toNodeRef(mParent);

        if (P.mNodeType == PType) {

            P.mChildren.erase(mChildIt);
            P.mChildrenCount--;
            mParent = mTree.nil();

            if (isFull()||isSinglyPartial()||isCDPartial()) {

                P.mPertinentChildrenCount--;
                P.mPertinentLeavesCount -= mPertinentLeavesCount;

            }

            if (P.mSinglyPartialChild1==backIt()) {
                P.mSinglyPartialChild1 = mTree.nil();
            }
            else if (P.mSinglyPartialChild2==backIt()) {
                P.mSinglyPartialChild2 = mTree.nil();
            }
            if (P.mCDPartialChild == backIt()) {
                P.mCDPartialChild = mTree.nil();
            }

            P.mPertinentType = PertinentUnknown;

        }
    }
}


/** @brief separate this node from its Q parent.
 *
 *  @param extPIt  (in): externally supplied pointer to the parent.
 *                       used if it does not know its parent by itself.
 */
inline void BLTreeNode::unlinkFromQTypeParent(node_list_it_t extPIt)
{

    discardOldFullLink();

    // This holds the internally found parent.
    node_list_it_t pIt = mTree.nil();

    if (mTree.isNil(mSibling1) && !mTree.isNil(mSibling2)) {

        // This node is either End1 or 2. Knows its parent.

        auto &P    = mTree.toNodeRef(mParent);
        auto &Sib2 = mTree.toNodeRef(mSibling2);
        if (P.mEndChild1 == backIt()) {
            P.mEndChild1 = mSibling2;
        }
        else {
            P.mEndChild2 = mSibling2;
        }
        mTree.unlinkSiblings(*this, Sib2);
        mSibling2    = mTree.nil();
        Sib2.mParent = mParent;
        pIt          = mParent;
        mParent      = mTree.nil();

    }
    else if (!mTree.isNil(mSibling1) && mTree.isNil(mSibling2)) {

        // This node is either End1 or 2. Knows its parent.

        auto &P    = mTree.toNodeRef(mParent);
        auto &Sib1 = mTree.toNodeRef(mSibling1);
        if (P.mEndChild1 == backIt()) {
            P.mEndChild1 = mSibling1;
        }
        else {
            P.mEndChild2 = mSibling1;
        }
        mTree.unlinkSiblings(*this, Sib1);
        mSibling1    = mTree.nil();
        Sib1.mParent = mParent;
        pIt          = mParent;
        mParent      = mTree.nil();

    }
    else if (!mTree.isNil(mSibling1) && !mTree.isNil(mSibling2)) {

        // This is a middle node. It does not know its parent.
        auto &Sib1 = mTree.toNodeRef(mSibling1);
        auto &Sib2 = mTree.toNodeRef(mSibling2);
        mTree.unlinkSiblings(*this, Sib1);
        mTree.unlinkSiblings(*this, Sib2);
        mTree.linkSiblings(Sib1, Sib2);
        mParent   = mTree.nil();

    }   

    if (mTree.isNil(pIt) && mTree.isNil(extPIt)) {
        return;
    }

    if (mTree.isNil(pIt)) {
        // If this node does not know its parent, use the externally
        // supplied pointer.
        pIt = extPIt;
    }

    auto &P = mTree.toNodeRef(pIt);
    if (P.isPertinent()) {
        if (isFull()||isSinglyPartial()||isCDPartial()) {
        
            P.mPertinentChildrenCount--;
            P.mPertinentLeavesCount -= mPertinentLeavesCount;

        }

        else if (P.mSinglyPartialChild1==backIt()) {
            P.mSinglyPartialChild1 = mTree.nil();
        }
        else if (P.mSinglyPartialChild2==backIt()) {
            P.mSinglyPartialChild2 = mTree.nil();
        }
        if (P.mCDPartialChild == backIt()) {
            P.mCDPartialChild = mTree.nil();
        }
    }
}

 
/** @brief link this node under a P-type parent.
 *         It assumes this node is either of Full, CDPartial, or Empty type.
 */
inline void BLTreeNode::linkToPTypeParent(BLTreeNode& P)
{

    discardOldFullLink();

    if (P.mNodeType == PType) {

        mChildIt = P.mChildren.insert(P.mChildren.end(), backIt());
        P.mChildrenCount++;

        mParent  = P.backIt();

        if (isFull()) {

            P.mPertinentChildrenCount++;
            P.mPertinentLeavesCount += mPertinentLeavesCount;

            // Create new link.
            createFullLink(P);
        }

        if (isCDPartial()) {

            P.mPertinentChildrenCount++;
            P.mPertinentLeavesCount += mPertinentLeavesCount;

        }

    }
}


inline void BLTreeNode::sortFullAndEmptyChildrenOnPNode(
    list<node_list_it_t>&  fullChildren,
    list<node_list_it_t>&  emptyChildren
) {

    for (auto cIt : mChildren) {

        auto& C = mTree.toNodeRef(cIt);

        if (C.isFull()) {

            fullChildren.push_back(cIt);

        }
        else if (C.isEmpty()) {

            emptyChildren.push_back(cIt);

        }
    }
}


inline void BLTreeNode::linkToQTypeParentToEnd(
    BLTreeNode&     P,
    BLTree::addType aType
) {

    discardOldFullLink();

    /* This operation assumes there is no partial children under P,
     * and this node is either Full or Empty.
     */

    auto& E1 = mTree.toNodeRef(P.mEndChild1);
    auto& E2 = mTree.toNodeRef(P.mEndChild2);

    mParent = P.backIt();
    mSibling1 = mTree.nil();
    if ( ( aType==BLTree::FULL_END  &&  E1.isFull() ) ||
         ( aType==BLTree::EMPTY_END && !E1.isFull() )   ){

        mSibling2 = E1.backIt();

        if (mTree.isNil(E1.mSibling1)) {
            E1.mSibling1 = backIt();
        }
        else {

            E1.mSibling2 = backIt();
        }
        P.mEndChild1 = backIt();
    }
    else {

        mSibling2 = E2.backIt();
        if (mTree.isNil(E2.mSibling1)) {

            E2.mSibling1 = backIt();
        }
        else {

            E2.mSibling2 = backIt();
        }
        P.mEndChild2 = backIt();
    }

    if (isFull()) {
        P.mPertinentChildrenCount++;
        P.mPertinentLeavesCount += mPertinentLeavesCount;

        // Create new link.
        createFullLink(P);
    }
}


inline bool BLTreeNode::isEndChild1Full()
{
    if (mNodeType == QType) {
        auto& E1 = mTree.toNodeRef(mEndChild1);
        if (E1.isFull()) {
            return true;
        }
    }
    return false;
}


inline void BLTreeNode::resetToPAttachment() noexcept
{
    discardOldFullLink();
    clearFullChildren();

    mNodeType  = PType;
    mEndChild1 = mTree.nil();
    mEndChild2 = mTree.nil();
    mChildren.clear();
    mChildrenCount = 0;
}


inline void BLTreeNode::transferGraphNodeOrientationsFrom(
                            BLTreeNode& P, BLTree::scanDirectionType direction)
{
    if (direction == BLTree::REVERSED_DIRECTION) {

        mOrientInNorm.splice(
                      mOrientInNorm.end(),        P.mOrientInReversed);

        mOrientInReversed.splice(
                      mOrientInReversed.end(),    P.mOrientInNorm);

        mOrientOutNorm.splice(
                      mOrientOutNorm.end(),       P.mOrientOutReversed);

        mOrientOutReversed.splice(
                      mOrientOutReversed.end(),   P.mOrientOutNorm);

    }
    else {

        mOrientInNorm.splice(
                      mOrientInNorm.end(),        P.mOrientInNorm);

        mOrientInReversed.splice(
                      mOrientInReversed.end(),    P.mOrientInReversed);

        mOrientOutNorm.splice(
                      mOrientOutNorm.end(),       P.mOrientOutNorm);

        mOrientOutReversed.splice(
                      mOrientOutReversed.end(),   P.mOrientOutReversed);
    }

    P.mOrientInReversed.clear();
    P.mOrientInNorm.clear();
    P.mOrientOutReversed.clear();
    P.mOrientOutNorm.clear();

}


inline void BLTreeNode::assumeGraphNodeOrientationsFrom(
                            BLTreeNode& C, BLTree::scanDirectionType direction)
{
    if (direction == BLTree::REVERSED_DIRECTION) {

        mAssumedOrientOutNorm.splice(
                 mAssumedOrientOutNorm.end(),     C.mOrientOutReversed);

        mAssumedOrientOutReversed.splice(
                 mAssumedOrientOutReversed.end(), C.mOrientOutNorm);

        mAssumedOrientInNorm.splice(
                 mAssumedOrientInNorm.end(),      C.mOrientInReversed);

        mAssumedOrientInReversed.splice(
                 mAssumedOrientInReversed.end(),  C.mOrientInNorm);

    }
    else {

        mAssumedOrientOutNorm.splice(
                 mAssumedOrientOutNorm.end(),     C.mOrientOutNorm);

        mAssumedOrientOutReversed.splice(
                 mAssumedOrientOutReversed.end(), C.mOrientOutReversed);

        mAssumedOrientInNorm.splice(
                 mAssumedOrientInNorm.end(),      C.mOrientInNorm);
           
        mAssumedOrientInReversed.splice(
                 mAssumedOrientInReversed.end(),  C.mOrientInReversed);
    }

    C.mOrientOutNorm.clear();
    C.mOrientOutReversed.clear();
    C.mOrientInNorm.clear();
    C.mOrientInReversed.clear();

}


inline void BLTreeNode::solveAssumedGraphNodeOrientationsFrom(
                            BLTreeNode& C, BLTree::scanDirectionType direction)
{
    if (direction==BLTree::REVERSED_DIRECTION) {

        mOrientOutNorm.splice(
                 mOrientOutNorm.end(),            C.mAssumedOrientOutReversed);

        mOrientOutReversed.splice(
                 mOrientOutReversed.end(),        C.mAssumedOrientOutNorm);

        mOrientInNorm.splice(
                 mOrientInNorm.end(),             C.mAssumedOrientInReversed);

        mOrientInReversed.splice(
                 mOrientInReversed.end(),         C.mAssumedOrientInNorm);
    }
    else {

        mOrientOutNorm.splice(
                 mOrientOutNorm.end(),            C.mAssumedOrientOutNorm);

        mOrientOutReversed.splice(
                 mOrientOutReversed.end(),        C.mAssumedOrientOutReversed);

        mOrientInNorm.splice(
                 mOrientInNorm.end(),             C.mAssumedOrientInNorm);

        mOrientInReversed.splice(
                 mOrientInReversed.end(),         C.mAssumedOrientInReversed);
          
    }

    C.mAssumedOrientOutNorm.clear();
    C.mAssumedOrientOutReversed.clear();
    C.mAssumedOrientInNorm.clear();
    C.mAssumedOrientInReversed.clear();

}


inline BLTree::BLTree():
    mGeneration(0),
    mTrackQFlippings(false),
    mCollectingEdges(false){;}


inline BLTree::~BLTree(){;}


inline node_list_it_t BLTree::nil()
{
    return nodes().second;
}


inline void BLTree::trackQFlippings()
{
    mTrackQFlippings = true;
}


inline bool BLTree::isTrackingQFlippings()
{
    return mTrackQFlippings;
}


inline BLTreeNode& BLTree::toNodeRef(node_list_it_t nIt)
{
    return dynamic_cast<BLTreeNode&>(*(*nIt));
}


inline bool BLTree::isNil(node_list_it_t nIt)
{
    return nIt == nodes().second;
}


inline node_list_it_t BLTree::makePAttachment()
{
    auto  np    = make_unique<BLTreeNode>(*this);
    auto  nIt   = addNode(std::move(np)).backIt();
    auto& N     = toNodeRef(nIt);
    N.mNodeType = BLTreeNode::PType;
    return nIt;
}


/** @brief initializes variables for one reduction.
 */
inline void BLTree::initializeForOneIteration() {

    mGeneration++;
    mCDPartialRoot        = nil();

}


inline void BLTree::advanceSib(node_list_it_t& prev, node_list_it_t& cur)
{
    if(!isNil(cur)){
        auto& C = toNodeRef(cur);
        if (C.mSibling1==prev) {
            prev = cur;
            cur  = C.mSibling2;
        }
        else {
            prev = cur;
            cur  = C.mSibling1;
        }
    }
}


inline void BLTree::linkSiblings(BLTreeNode& N1, BLTreeNode& N2)
{

    if (isNil(N1.mSibling1)) {

        N1.mSibling1 = N2.backIt();
    }
    else {

        N1.mSibling2 = N2.backIt();
    }
    if (isNil(N2.mSibling1)) {

        N2.mSibling1 = N1.backIt();
    }
    else {

        N2.mSibling2 = N1.backIt();
    }
}


inline void BLTree::unlinkSiblings(BLTreeNode& N1, BLTreeNode& N2)
{
    if (N1.mSibling1 == N2.backIt()) {
        N1.mSibling1 = nil();
    }
    else if (N1.mSibling2 == N2.backIt()) {
        N1.mSibling2 = nil();
    }


    if (N2.mSibling1 == N1.backIt()) {
        N2.mSibling1 = nil();
    }
    else if (N2.mSibling2 == N1.backIt()) {
        N2.mSibling2 = nil();
    }
}


inline void BLTree::removeVirtualRoot(node_list_it_t vIt)
{
    auto& V = toNodeRef(vIt);
    V.clearFullChildren();
    removeNode(V);
}


inline void BLTree::unlinkFromPTypeParent(list<node_list_it_t>& children)
{
    for (auto cIt : children) {
        toNodeRef(cIt).unlinkFromPTypeParent();
    }
}


inline node_list_it_t BLTree::CDPartialRoot()
{
    return mCDPartialRoot;
}


inline void BLTree::setCollectingEdges()
{
    mCollectingEdges = true;
}


inline bool BLTree::isCollectingEdges()
{
    return mCollectingEdges;
}


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_BL_INLINE_HPP_*/
