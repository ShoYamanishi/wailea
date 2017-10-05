#include "undirected/bl_planarity_tester.hpp"
#include "undirected/bl_tree.hpp"
#include "undirected/bl_tree_node.hpp"
#include "undirected/bl_inline.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/bl_tree.cpp
 *
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/** @brief makes a pertinent full or empty P node.
 *
 *  @param children (in): list of full or empty children.
 *
 *  @return pertinent full or empty P node created.
 */
node_list_it_t BLTree::makePNode(list<node_list_it_t>& children)
{
    auto  np  = make_unique<BLTreeNode>(*this);
    auto  nIt = addNode(std::move(np)).backIt();
    auto& N   = toNodeRef(nIt);

    N.mNodeType      = BLTreeNode::PType;
    N.mGeneration    = mGeneration;

    for (auto cIt : children) {
        toNodeRef(cIt).linkToPTypeParent(N);
    }

    if (N.mPertinentChildrenCount == N.mChildrenCount){

        N.mPertinentType = BLTreeNode::Full;
    }
    else if (N.mPertinentChildrenCount == 0) {

        N.mPertinentType = BLTreeNode::Empty;
    }

    return nIt;
}


node_list_it_t BLTree::makeQNode()
{
    auto  np  = make_unique<BLTreeNode>(*this);
    auto  nIt = addNode(std::move(np)).backIt();
    auto& N   = toNodeRef(nIt);

    N.mNodeType      = BLTreeNode::QType;
    N.mGeneration    = mGeneration;
    N.mPertinentType = BLTreeNode::Empty;

    return nIt;
}


node_list_it_t BLTree::bringUpOnlyChildOfPNodeAndRemove(BLTreeNode& X)
{
    auto& C = toNodeRef(*(X.mChildren.begin()));

    if (isNil(X.mSibling1)&&isNil(X.mSibling2)) {

        // Parent is a P node or X has no parent.

        if (!isNil(X.mParent)) {

            /*
             *  Parent is a P node.
             *
             *         _P_______
             *        / | \     \
             *       N1 X N2 ... Nm
             *          |
             *          C
             */

            auto& P = toNodeRef(X.mParent);

            // Insert C before X in P's mChildren and then remove X.
            C.mChildIt = P.mChildren.insert(X.mChildIt,C.backIt());
            P.mChildrenCount++;
            C.mParent = P.backIt();

            P.mChildren.erase(X.mChildIt);
            P.mChildrenCount--;
        }
        else {

            // X is the tree root.
            C.mParent = nil();
        }

    }
    else if ( (!isNil(X.mSibling1)&& isNil(X.mSibling2))||
              ( isNil(X.mSibling1)&&!isNil(X.mSibling2))   ){
        /*
         * Parent is a Q node and this is an end child.
         *
         *       ___Q_______
         *      |  |        |
         *      X  XSib ... Nm
         *      |
         *      C
         */
        auto& XSib = toNodeRef(isNil(X.mSibling1)?X.mSibling2:X.mSibling1);

        if (XSib.mSibling1 == X.backIt()) {
            XSib.mSibling1 = C.backIt();
        }
        else {
            XSib.mSibling2 = C.backIt();
        }
        C.mSibling1 = X.mSibling1;
        C.mSibling2 = X.mSibling2;

        auto& P = toNodeRef(X.mParent);
        if (P.mEndChild1 == X.backIt()) {
            P.mEndChild1 = C.backIt();
        }
        else {
            P.mEndChild2 = C.backIt();
        }

        C.mParent = X.mParent;

    }
    else {

        /*
         * Parent unknown. The parent is Q type or a virtual root.
         *
         *       ______________Q_______________
         *      |                              |
         *      N1 ... XSib1 - X - XSib2 ... - Nm
         *                     |
         *                     C
         */
        auto& XSib1 = toNodeRef(X.mSibling1);
        auto& XSib2 = toNodeRef(X.mSibling2);
        bool XSib1LinkOn1 = XSib1.mSibling1==X.backIt()?true:false;
        bool XSib2LinkOn1 = XSib2.mSibling1==X.backIt()?true:false;
        unlinkSiblings(XSib1, X);
        unlinkSiblings(XSib2, X);

        /* We don't use linkSiblings() as the preservation of orientation 
         * is important to track Q flippings.
         */
        C.mSibling1 = XSib1.backIt();
        C.mSibling2 = XSib2.backIt();

        if (XSib1LinkOn1) {
            XSib1.mSibling1 = C.backIt();
        }
        else {
            XSib1.mSibling2 = C.backIt();
        }

        if (XSib2LinkOn1) {
            XSib2.mSibling1 = C.backIt();
        }
        else {
            XSib2.mSibling2 = C.backIt();
        }

        C.mParent = X.mParent;
    }

    if (mTrackQFlippings) {

        // Transfer the orientation information from X to C.
        C.mOrientInNorm.splice(C.mOrientInNorm.end(), X.mOrientInNorm);
        C.mOrientInReversed.splice(
                                C.mOrientInReversed.end(),X.mOrientInReversed);
        C.mOrientOutNorm.splice(C.mOrientOutNorm.end(), X.mOrientOutNorm);
        C.mOrientOutReversed.splice(
                             C.mOrientOutReversed.end(), X.mOrientOutReversed);

        C.mAssumedOrientInNorm.splice(
                       C.mAssumedOrientInNorm.end(), X.mAssumedOrientInNorm);
        C.mAssumedOrientInReversed.splice(
               C.mAssumedOrientInReversed.end(), X.mAssumedOrientInReversed);
        C.mAssumedOrientOutNorm.splice(
                       C.mAssumedOrientOutNorm.end(), X.mAssumedOrientOutNorm);
        C.mAssumedOrientOutReversed.splice(
               C.mAssumedOrientOutReversed.end(), X.mAssumedOrientOutReversed);

    }
    X.discardOldFullLink();
    X.clearFullChildren();
    removeNode(X);

    return C.backIt();
}



void BLTree::fanOutLeavesFromAttachment(
    node_list_it_t        aIt,
    list<edge_list_it_t>& edgeList
) {
    BLTreeNode& A = toNodeRef(aIt);
    if (edgeList.size()==1) {
        auto& E = dynamic_cast<BLGraphEdge&>(*(*(*edgeList.begin())));

        A.mNodeType  = BLTreeNode::LType;
        A.mGraphEdge = E.backIt();
        E.mPQLeaf    = A.backIt();

    }
    else {
        for(auto eIt : edgeList) {

            auto& E = dynamic_cast<BLGraphEdge&>(*(*eIt));

            auto  np  = make_unique<BLTreeNode>(*this);
            auto  nIt = addNode(std::move(np)).backIt();
            auto& N   = toNodeRef(nIt);

            N.mNodeType  = BLTreeNode::LType;
            N.mGraphEdge = eIt;
            E.mPQLeaf    = nIt;
            N.mChildIt   = A.mChildren.insert(A.mChildren.end(),N.backIt());
            A.mChildrenCount++;
            N.mParent    = A.backIt();
        }
    }
}


void BLTree::fanOutLeavesFromQAttachment(
    node_list_it_t        aIt,
    list<edge_list_it_t>& edgeList,
    node_list_it_t        nIt
) {

    BLTreeNode& A = toNodeRef(aIt);
    if (edgeList.size()==1) {
        auto& E = dynamic_cast<BLGraphEdge&>(*(*(*edgeList.begin())));

        A.mNodeType  = BLTreeNode::LType;
        A.mGraphEdge = E.backIt();
        E.mPQLeaf    = A.backIt();

    }
    else {

        A.mNodeType = BLTreeNode::QType;

        size_t index = 0;
        node_list_it_t prevIt = nil();
        for(auto eIt : edgeList) {

            auto& E = dynamic_cast<BLGraphEdge&>(*(*eIt));

            auto  np  = make_unique<BLTreeNode>(*this);
            auto  nIt = addNode(std::move(np)).backIt();
            auto& N   = toNodeRef(nIt);

            N.mNodeType  = BLTreeNode::LType;
            N.mGraphEdge = eIt;
            E.mPQLeaf    = nIt;

            if (index ==0) {
                A.mEndChild1 = N.backIt();
                N.mParent    = A.backIt();
            }
            else if (index == edgeList.size() -1) {
                A.mEndChild2 = N.backIt();
                N.mParent    = A.backIt();
                linkSiblings(toNodeRef(prevIt), N);
            }
            else {
                linkSiblings(toNodeRef(prevIt), N);
            }

            prevIt = N.backIt();
            index++;
        }

        // The orientation of the edges fanned-out will be tracked
        // in this Q-node.
        A.mOrientInNorm.push_back(nIt);

    }
}


/**
 *   Assumption: Number of children is 0.
 *   Conditions:
 *       Type: L
 *
 *   Pertinent type : Full
 */
bool BLTree::templateL1(BLTreeNode& X, reductionType reduction)
{
    if (X.mNodeType!=BLTreeNode::LType) {
        return false;
    }
//cerr << "templateL1:[" << X.mNodeNum << "]\n";
    X.mPertinentType = BLTreeNode::Full;

    if (reduction == NOT_FINAL_REDUCTION) {
        X.setFullInParent();
    }

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Full          : ALL
 *       Empty         : 0
 *       SinglyPartial : 0
 *       CDPartial     : 0
 *       Empty         : 0
 *
 *   Pertinent type : Full
 */
bool BLTree::templateP1(BLTreeNode& X, reductionType reduction)
{
    if ( !(X.mNodeType == BLTreeNode::PType &&
           X.mFullChildren.size() == X.mChildrenCount &&
           isNil(X.mSinglyPartialChild1) &&
           isNil(X.mSinglyPartialChild2) &&
           isNil(X.mCDPartialChild)                 ) ) {
        return false;
    }
//cerr << "templateP1:[" << X.mNodeNum << "]\n";
    X.mPertinentType = BLTreeNode::Full;

    if (reduction == NOT_FINAL_REDUCTION) {
        X.setFullInParent();
    }

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Full          : greater than 1
 *       Empty         : greater than 0
 *       SinglyPartial : 0
 *       CDPartial     : 0
 *
 *   Pertinent type : N/A (the root is Full)
 *   This is a final reduction.
 */
bool BLTree::templateP2(BLTreeNode& X, node_list_it_t& pertinentRoot)
{

    if ( !( X.mNodeType == BLTreeNode::PType          &&
            X.mFullChildren.size() >= 2                 &&
            X.mFullChildren.size() < X.mChildrenCount &&
            isNil(X.mSinglyPartialChild1)             &&
            isNil(X.mSinglyPartialChild2)             && 
            isNil(X.mCDPartialChild)                     ) ) {

        return false;

    }
//cerr << "templateP2:[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(fullChildren);

    pertinentRoot = makePNode(fullChildren);

    toNodeRef(pertinentRoot).linkToPTypeParent(X);

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Full          : greater than 0
 *       Empty         : greater than 0
 *       SinglyPartial : 0
 *       CDPartial     : 0
 *
 *   Pertinent type : Singly Partial
 *
 *   This is not a final reduction
 */
bool BLTree::templateP3Old(BLTreeNode& X, bool& earlyOut)
{

    if ( !( X.mNodeType==BLTreeNode::PType            &&
            X.mFullChildren.size() < X.mChildrenCount &&
            X.mFullChildren.size() >= 1                 &&
            isNil(X.mSinglyPartialChild1)             &&
            isNil(X.mSinglyPartialChild2)             &&
            isNil(X.mCDPartialChild)                     ) ) {
        return false;
    }
//cerr << "templateP3:[" << X.mNodeNum << "]\n";

    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(fullChildren);
    unlinkFromPTypeParent(emptyChildren);

    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {

        fullIt = makePNode(fullChildren);
    }
    else {

        fullIt = *(fullChildren.begin());
    }

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {

        emptyIt = makePNode(emptyChildren);
    }
    else {

        emptyIt = *(emptyChildren.begin());
    }

    X.mNodeType      = BLTreeNode::QType;

    X.addTwoInitialChildrenToQType(fullIt, emptyIt);

    earlyOut = false;

    if (!X.setSinglyPartialInParent()){

        earlyOut = true;

    }

    return true;
}



bool BLTree::templateP3(BLTreeNode& X, bool& earlyOut)
{

    if ( !( X.mNodeType==BLTreeNode::PType            &&
            X.mFullChildren.size() < X.mChildrenCount &&
            X.mFullChildren.size() >= 1               &&
            isNil(X.mSinglyPartialChild1)             &&
            isNil(X.mSinglyPartialChild2)             &&
            isNil(X.mCDPartialChild)                     ) ) {
        return false;
    }
//cerr << "templateP3:[" << X.mNodeNum << "]\n";

    list<node_list_it_t> fullChildren(X.mFullChildren);

    unlinkFromPTypeParent(fullChildren);

    X.mPertinentType = BLTreeNode::Empty;
    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {

        fullIt = makePNode(fullChildren);
    }
    else {

        fullIt = *(fullChildren.begin());
    }

    // We know there is at least one (empty) children left.
    if (X.mChildrenCount > 1) {

        auto  SPIt = makeQNode();
        auto& SP   = toNodeRef(SPIt);
        auto& P    = toNodeRef(X.mParent);

        if (P.mNodeType == BLTreeNode::PType) {

            P.mChildren.erase(X.mChildIt);
            P.mChildrenCount--;
            X.mParent = nil();

            SP.mChildIt = P.mChildren.insert(P.mChildren.end(), SPIt);
            P.mChildrenCount++;
            SP.mParent  = P.backIt();

        }
        else {
            // P.mNodeType == BLTreeNode::QType ||
            // P.mNodeType == BLTreeNode::VRType

            if ( (!isNil(X.mSibling1)&& isNil(X.mSibling2))||
                 ( isNil(X.mSibling1)&&!isNil(X.mSibling2))   ) {

                auto& XSib = toNodeRef(isNil(X.mSibling1)?
                                                    X.mSibling2:X.mSibling1);
                if (XSib.mSibling1 == X.backIt()) {
                    XSib.mSibling1 = SP.backIt();
                }
                else {
                    XSib.mSibling2 = SP.backIt();
                }
                SP.mSibling1 = X.mSibling1;
                SP.mSibling2 = X.mSibling2;

                if (P.mEndChild1 == X.backIt()) {
                    P.mEndChild1 = SP.backIt();
                }
                else {
                    P.mEndChild2 = SP.backIt();
                }
                SP.mParent = X.mParent;

                X.mParent = nil();
                X.mSibling1 = nil();
                X.mSibling2 = nil();

            }
            else if (!isNil(X.mSibling1)&&!isNil(X.mSibling2)) {

                auto& XSib1 = toNodeRef(X.mSibling1);
                auto& XSib2 = toNodeRef(X.mSibling2);
                bool XSib1LinkOn1 = XSib1.mSibling1==X.backIt()?true:false;
                bool XSib2LinkOn1 = XSib2.mSibling1==X.backIt()?true:false;
                unlinkSiblings(XSib1, X);
                unlinkSiblings(XSib2, X);

                /* We don't use linkSiblings() as the preservation of
                 * orientation is important to track Q flippings.
                 */
                SP.mSibling1 = XSib1.backIt();
                SP.mSibling2 = XSib2.backIt();

                if (XSib1LinkOn1) {
                    XSib1.mSibling1 = SP.backIt();
                }
                else {
                    XSib1.mSibling2 = SP.backIt();
                }

                if (XSib2LinkOn1) {
                    XSib2.mSibling1 = SP.backIt();
                }
                else {
                    XSib2.mSibling2 = SP.backIt();
                }
                SP.mParent = X.mParent;

                X.mParent = nil();
                X.mSibling1 = nil();
                X.mSibling2 = nil();

            }
            else {

                SP.mSibling1 = nil();
                SP.mSibling2 = nil();
                SP.mParent   = X.mParent;

                P.mEndChild1 = SP.backIt();
                P.mEndChild2 = SP.backIt();

                X.mParent = nil();
                X.mSibling1 = nil();
                X.mSibling2 = nil();
            }
        }

        SP.addTwoInitialChildrenToQType(fullIt, X.backIt());
        earlyOut = false;
        if (!SP.setSinglyPartialInParent()){
            earlyOut = true;
        }

        if (mTrackQFlippings) {

            // Transfer the orientation information from X to C.
            SP.mOrientInNorm.splice(SP.mOrientInNorm.end(), X.mOrientInNorm);
            SP.mOrientInReversed.splice(
                               SP.mOrientInReversed.end(),X.mOrientInReversed);
            SP.mOrientOutNorm.splice(
                                    SP.mOrientOutNorm.end(), X.mOrientOutNorm);
            SP.mOrientOutReversed.splice(
                            SP.mOrientOutReversed.end(), X.mOrientOutReversed);
            SP.mAssumedOrientInNorm.splice(
                        SP.mAssumedOrientInNorm.end(), X.mAssumedOrientInNorm);
            SP.mAssumedOrientInReversed.splice(
                SP.mAssumedOrientInReversed.end(), X.mAssumedOrientInReversed);
            SP.mAssumedOrientOutNorm.splice(
                      SP.mAssumedOrientOutNorm.end(), X.mAssumedOrientOutNorm);
            SP.mAssumedOrientOutReversed.splice(
              SP.mAssumedOrientOutReversed.end(), X.mAssumedOrientOutReversed);
        }
    }
    else {

        auto& E = toNodeRef(*(X.mChildren.begin()));
        E.unlinkFromPTypeParent();
        X.mNodeType      = BLTreeNode::QType;
        X.addTwoInitialChildrenToQType(fullIt, E.backIt());
        earlyOut = false;

        if (!X.setSinglyPartialInParent()){

            earlyOut = true;

        }
    }

    return true;
}





/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type:           P
 *       SinglyPartial : 1
 *       CDPartial :     0
 *
 *   Pertinent type    : N/A (The root is SinglyPartial.)
 *
 *   This is a final reduction.
 */
bool BLTree::templateP4(BLTreeNode& X, node_list_it_t& pertinentRoot)
{

    if ( !( X.mNodeType==BLTreeNode::PType &&
            isNil(X.mCDPartialChild) &&
            ( ( isNil(X.mSinglyPartialChild1) && 
               !isNil(X.mSinglyPartialChild2)   )||
              (!isNil(X.mSinglyPartialChild1) &&
                isNil(X.mSinglyPartialChild2)   )   ) ) ) {
        return false;
    }
//cerr << "templateP4:[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;// Not used.

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(fullChildren);

    node_list_it_t partialIt;
    if (isNil(X.mSinglyPartialChild1)) {
        partialIt = X.mSinglyPartialChild2;
        X.mSinglyPartialChild2 = nil();
    }
    else {
        partialIt = X.mSinglyPartialChild1;
        X.mSinglyPartialChild1 = nil();
    }
    auto& Partial = toNodeRef(partialIt);

    node_list_it_t fullIt;

    if (fullChildren.size() > 1) {

        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToQTypeParentToEnd(Partial, FULL_END);

    }
    else if (fullChildren.size() == 1) {

        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToQTypeParentToEnd(Partial, FULL_END);

    }

    if (X.mChildrenCount == 1) {

        bringUpOnlyChildOfPNodeAndRemove(X);

    }

    pertinentRoot = partialIt;

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type:            P
 *       SinglyPartial  : 1
 *       CDPartial      : 0
 *
 *   Pertinent type : Singly Partial
 *
 *   This is not a final reduction
 */
bool BLTree::templateP5Old(BLTreeNode& X, bool& earlyOut)
{
    if ( !( X.mNodeType==BLTreeNode::PType &&
            isNil(X.mCDPartialChild) &&
            ( ( isNil(X.mSinglyPartialChild1) && 
               !isNil(X.mSinglyPartialChild2)   )||
              (!isNil(X.mSinglyPartialChild1) &&
                isNil(X.mSinglyPartialChild2)   )   ) ) ) {

        return false;
    }
//cerr << "templateP5:[" << X.mNodeNum << "]\n";

    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(fullChildren);
    unlinkFromPTypeParent(emptyChildren);

    node_list_it_t partialIt;
    if (isNil(X.mSinglyPartialChild1)) {
        partialIt = X.mSinglyPartialChild2;
    }
    else {
        partialIt = X.mSinglyPartialChild1;
    }

    auto& Partial = toNodeRef(partialIt);
    
    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {
        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToQTypeParentToEnd(Partial, FULL_END);
    }
    else if (fullChildren.size() == 1) {
        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToQTypeParentToEnd(Partial, FULL_END);
    }

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {
        emptyIt = makePNode(emptyChildren);
        toNodeRef(emptyIt).linkToQTypeParentToEnd(Partial, EMPTY_END);
    }
    else if (emptyChildren.size() == 1) {
        emptyIt = *(emptyChildren.begin());
        toNodeRef(emptyIt).linkToQTypeParentToEnd(Partial, EMPTY_END);
    }

    bringUpOnlyChildOfPNodeAndRemove(X);
    // From this point on X is invalid as it has been removed.

    earlyOut = false;
    if (!Partial.setSinglyPartialInParent()) {
        earlyOut = true;;
    }

    return true;
}


bool BLTree::templateP5(BLTreeNode& X, bool& earlyOut)
{
    if ( !( X.mNodeType==BLTreeNode::PType &&
            isNil(X.mCDPartialChild) &&
            ( ( isNil(X.mSinglyPartialChild1) &&
               !isNil(X.mSinglyPartialChild2)   )||
              (!isNil(X.mSinglyPartialChild1) &&
                isNil(X.mSinglyPartialChild2)   )   ) ) ) {

        return false;
    }
//cerr << "templateP5:[" << X.mNodeNum << "]\n";

    list<node_list_it_t> fullChildren(X.mFullChildren);
    size_t numFullChildren = X.mFullChildren.size();
    unlinkFromPTypeParent(fullChildren);
    node_list_it_t partialIt;
    if (isNil(X.mSinglyPartialChild1)) {
        partialIt = X.mSinglyPartialChild2;

    }
    else {
        partialIt = X.mSinglyPartialChild1;
    }

    auto& SP = toNodeRef(partialIt);

    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {
        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToQTypeParentToEnd(SP, FULL_END);

    }
    else if (fullChildren.size() == 1) {
        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToQTypeParentToEnd(SP, FULL_END);
    }

    // We know X contains SP and zero or more empty children
    if (X.mChildrenCount == 1) {
        // Zero empty child under X.

        bringUpOnlyChildOfPNodeAndRemove(X);
        // From this point on X is invalid as it has been removed.

        earlyOut = false;
        if (!SP.setSinglyPartialInParent()) {
            earlyOut = true;;
        }
    }
    else if (X.mChildrenCount == 2) {
        // One empty child under X.
        auto emptyIt = (*(X.mChildren.begin()))!=partialIt?
                                *(X.mChildren.begin()):*(X.mChildren.rbegin());

        auto& E = toNodeRef(emptyIt);

        E.unlinkFromPTypeParent();

        E.linkToQTypeParentToEnd(SP, EMPTY_END);

        bringUpOnlyChildOfPNodeAndRemove(X);
        // From this point on X is invalid as it has been removed.

        earlyOut = false;

        if (!SP.setSinglyPartialInParent()) {

            earlyOut = true;;
        }

    }
    else {
        X.mPertinentLeavesCount += numFullChildren;
        SP.unlinkFromPTypeParent();
        X.mPertinentType = BLTreeNode::Empty;

        // More than one empty child under X. Reuse X.
        auto& P = toNodeRef(X.mParent);

        if (P.mNodeType == BLTreeNode::PType) {
            P.mChildren.erase(X.mChildIt);
            P.mChildrenCount--;
            X.mParent = nil();

            SP.mChildIt = P.mChildren.insert(P.mChildren.end(), partialIt);
            P.mChildrenCount++;
            SP.mParent  = P.backIt();

        }
        else {
            // P.mNodeType == BLTreeNode::QType ||
            // P.mNodeType == BLTreeNode::VRType

            if ( (!isNil(X.mSibling1)&& isNil(X.mSibling2))||
                 ( isNil(X.mSibling1)&&!isNil(X.mSibling2))  ){

                auto& XSib = toNodeRef(isNil(X.mSibling1)?
                                                    X.mSibling2:X.mSibling1);
                if (XSib.mSibling1 == X.backIt()) {
                    XSib.mSibling1 = SP.backIt();
                }
                else {
                    XSib.mSibling2 = SP.backIt();
                }
                SP.mSibling1 = X.mSibling1;
                SP.mSibling2 = X.mSibling2;

                if (P.mEndChild1 == X.backIt()) {
                    P.mEndChild1 = SP.backIt();
                }
                else {
                    P.mEndChild2 = SP.backIt();
                }
                SP.mParent = X.mParent;

                X.mParent = nil();
                X.mSibling1 = nil();
                X.mSibling2 = nil();
            }
            else if (!isNil(X.mSibling1)&&!isNil(X.mSibling2)) {

                auto& XSib1 = toNodeRef(X.mSibling1);
                auto& XSib2 = toNodeRef(X.mSibling2);
                bool XSib1LinkOn1 = XSib1.mSibling1==X.backIt()?true:false;
                bool XSib2LinkOn1 = XSib2.mSibling1==X.backIt()?true:false;
                unlinkSiblings(XSib1, X);
                unlinkSiblings(XSib2, X);

                /* We don't use linkSiblings() as the preservation of
                 * orientation is important to track Q flippings.
                 */
                SP.mSibling1 = XSib1.backIt();
                SP.mSibling2 = XSib2.backIt();

                if (XSib1LinkOn1) {
                    XSib1.mSibling1 = SP.backIt();
                }
                else {
                    XSib1.mSibling2 = SP.backIt();
                }

                if (XSib2LinkOn1) {
                    XSib2.mSibling1 = SP.backIt();
                }
                else {
                    XSib2.mSibling2 = SP.backIt();
                }
                SP.mParent = X.mParent;

                X.mParent = nil();
                X.mSibling1 = nil();
                X.mSibling2 = nil();

            }
            else {

                SP.mSibling1 = nil();
                SP.mSibling2 = nil();
                SP.mParent   = X.mParent;

                P.mEndChild1 = SP.backIt();
                P.mEndChild2 = SP.backIt();

                X.mParent = nil();
                X.mSibling1 = nil();
                X.mSibling2 = nil();
            }
        }

        X.linkToQTypeParentToEnd(SP, EMPTY_END);

        earlyOut = false;
        if (!SP.setSinglyPartialInParent()){
            earlyOut = true;
        }

        if (mTrackQFlippings) {

            // Transfer the orientation information from X to C.
            SP.mOrientInNorm.splice(SP.mOrientInNorm.end(), X.mOrientInNorm);
            SP.mOrientInReversed.splice(
                               SP.mOrientInReversed.end(),X.mOrientInReversed);
            SP.mOrientOutNorm.splice(
                                    SP.mOrientOutNorm.end(), X.mOrientOutNorm);
            SP.mOrientOutReversed.splice(
                            SP.mOrientOutReversed.end(), X.mOrientOutReversed);
            SP.mAssumedOrientInNorm.splice(
                        SP.mAssumedOrientInNorm.end(), X.mAssumedOrientInNorm);
            SP.mAssumedOrientInReversed.splice(
                SP.mAssumedOrientInReversed.end(), X.mAssumedOrientInReversed);
            SP.mAssumedOrientOutNorm.splice(
                      SP.mAssumedOrientOutNorm.end(), X.mAssumedOrientOutNorm);
            SP.mAssumedOrientOutReversed.splice(
              SP.mAssumedOrientOutReversed.end(), X.mAssumedOrientOutReversed);

        }
    }

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type:           P
 *       SinglyPartial : 2
 *       CDPartial     : 0
 *
 *   Pertinent type    : Doubly Partial
 *
 *   This is a final reduction.
 */
bool BLTree::templateP6(BLTreeNode& X, node_list_it_t& pertinentRoot)
{
    if ( !( X.mNodeType==BLTreeNode::PType &&
            isNil(X.mCDPartialChild) &&
            !isNil(X.mSinglyPartialChild1) &&
            !isNil(X.mSinglyPartialChild2)       ) ) {

        return false;
    }
//cerr << "templateP6:[" << X.mNodeNum << "]\n";

    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(fullChildren);

    auto  partialItBase     = X.mSinglyPartialChild1;
    auto& PartialBase       = toNodeRef(partialItBase);
    auto  partialItAbsorbed = X.mSinglyPartialChild2;
    auto& PartialAbsorbed   = toNodeRef(partialItAbsorbed);

    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {

        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToQTypeParentToEnd(PartialBase, FULL_END);

    }
    else if (fullChildren.size() == 1) {

        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToQTypeParentToEnd(PartialBase, FULL_END);

    }

    PartialAbsorbed.unlinkFromPTypeParent();

    concatenateOneSinglyPartialToTheOther(
                                       PartialBase, PartialAbsorbed, FULL_END);

    if (X.mChildrenCount == 1) {

        bringUpOnlyChildOfPNodeAndRemove(X);

    }

    PartialBase.mPertinentType = BLTreeNode::DoublyPartial;

    pertinentRoot = partialItBase;

    return true;
}


void BLTree::concatenateOneSinglyPartialToTheOther(
    BLTreeNode& Main,
    BLTreeNode& Absorbed,
    addType     type
) {

    node_list_it_t mainFullEndIt;
    node_list_it_t mainEmptyEndIt;
    node_list_it_t absorbedFullEndIt;
    node_list_it_t absorbedEmptyEndIt;

    auto &MainEnd1     = toNodeRef(Main.mEndChild1);
    auto &AbsorbedEnd1 = toNodeRef(Absorbed.mEndChild1);

    if (MainEnd1.isFull()) {

        mainFullEndIt  = Main.mEndChild1;
        mainEmptyEndIt = Main.mEndChild2;

    }
    else {

        mainFullEndIt  = Main.mEndChild2;
        mainEmptyEndIt = Main.mEndChild1;

    }

    if (AbsorbedEnd1.isFull()) {

        absorbedFullEndIt  = Absorbed.mEndChild1;
        absorbedEmptyEndIt = Absorbed.mEndChild2;

    }
    else {

        absorbedFullEndIt  = Absorbed.mEndChild2;
        absorbedEmptyEndIt = Absorbed.mEndChild1;

    }

    // Now all the full and empty ends are found.
    auto& mainFullEnd      = toNodeRef(mainFullEndIt);
    auto& mainEmptyEnd     = toNodeRef(mainEmptyEndIt);
    auto& absorbedFullEnd  = toNodeRef(absorbedFullEndIt);
    auto& absorbedEmptyEnd = toNodeRef(absorbedEmptyEndIt);


    // Update the parent pointer of full children under absorbed node.

    node_list_it_t prevIt = nil();
    node_list_it_t curIt  = absorbedFullEndIt;

    while (!isNil(curIt)) {

        auto& GC = toNodeRef(curIt);
        if (GC.isEmpty()) {
            break;
        }
        GC.mParent  = Main.backIt();
        advanceSib(prevIt, curIt);

    }

    if (type==FULL_END) {

        // Concatenate full ends. The resultant main node
        // has empty nodes at both ends.

        linkSiblings(mainFullEnd, absorbedFullEnd);

        if (MainEnd1.isFull()) {

            Main.mEndChild1 = absorbedEmptyEndIt;

        }
        else {

            Main.mEndChild2 = absorbedEmptyEndIt;

        }

        absorbedEmptyEnd.mParent = Main.backIt();
    }

    else {

        // Concatenate empty ends. The resultant main node
        // has full nodes at both ends.

        linkSiblings(mainEmptyEnd, absorbedEmptyEnd);

        if (MainEnd1.isFull()) {

            Main.mEndChild2 = absorbedFullEndIt;

        }
        else {

            Main.mEndChild1 = absorbedFullEndIt;

        }

        absorbedFullEnd.mParent = Main.backIt();

    }

    Main.mPertinentChildrenCountProcessed
                                  += Absorbed.mPertinentChildrenCountProcessed;
    Main.mPertinentChildrenCount  += Absorbed.mPertinentChildrenCount;
    Main.mPertinentLeavesCount    += Absorbed.mPertinentLeavesCount;

    list<node_list_it_t> absorbedFullChildrenSaved(Absorbed.mFullChildren);
    for (auto fit : absorbedFullChildrenSaved) {

        auto& F = toNodeRef(fit);

        F.discardOldFullLink();

        F.createFullLink(Main);

    }

    if (mTrackQFlippings) {

        scanDirectionType direction;

        if( ( MainEnd1.isFull() &&  AbsorbedEnd1.isFull() )||
            (!MainEnd1.isFull() && !AbsorbedEnd1.isFull() )  ) {

            direction = REVERSED_DIRECTION; 
        }
        else {

            direction = NORMAL_DIRECTION; 
        }

        Main.transferGraphNodeOrientationsFrom(Absorbed, direction);

    }

    Absorbed.discardOldFullLink();

    Absorbed.clearFullChildren();

    removeNode(Absorbed);

}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type:           P
 *       SinglyPartial : 2
 *       CDPartial     : 0
 *
 *   New node type : Q
 *
 *   Pertinent type : Complementarily Doubly Partial
 *
 *   This is not a final reduction.
 */
bool BLTree::templateP7Old(BLTreeNode& X, bool& earlyOut)
{
    if ( !( X.mNodeType==BLTreeNode::PType &&
            isNil(X.mCDPartialChild) &&
            !isNil(X.mSinglyPartialChild1) &&
            !isNil(X.mSinglyPartialChild2)       ) ) {

        return false;
    }
//cerr << "templateP7:[" << X.mNodeNum << "]\n";


    list<node_list_it_t> fullChildren;  // Not used.
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(emptyChildren);

    auto  partialItBase     = X.mSinglyPartialChild1;
    auto& PartialBase       = toNodeRef(partialItBase);
    auto  partialItAbsorbed = X.mSinglyPartialChild2;
    auto& PartialAbsorbed   = toNodeRef(partialItAbsorbed);

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {

        emptyIt = makePNode(emptyChildren);
        toNodeRef(emptyIt).linkToQTypeParentToEnd(PartialBase, EMPTY_END);

    }
    else if (emptyChildren.size() == 1) {

        emptyIt = *(emptyChildren.begin());
        toNodeRef(emptyIt).linkToQTypeParentToEnd(PartialBase, EMPTY_END);

    }

    size_t savedPartialAbsorbedPertinentLeavesCount
                                       = PartialAbsorbed.mPertinentLeavesCount;

    PartialAbsorbed.unlinkFromPTypeParent();
    
    concatenateOneSinglyPartialToTheOther(
                                      PartialBase, PartialAbsorbed, EMPTY_END);


    X.mPertinentLeavesCount += savedPartialAbsorbedPertinentLeavesCount;
    X.mSinglyPartialChild1 = nil();
    X.mSinglyPartialChild2 = nil();

    PartialBase.mPertinentType = BLTreeNode::CDPartial;

    if (!isNil(mCDPartialRoot)) {

        earlyOut = true;

    }
    else {

        earlyOut = false;
        mCDPartialRoot = partialItBase;

    }

    if (X.mChildrenCount == 1) {
        auto& newX = toNodeRef(bringUpOnlyChildOfPNodeAndRemove(X));
        newX.setCDPartialInParent();
    }
    else {

        PartialBase.setCDPartialInParent();

        X.setCDPartialInParent();
        X.mPertinentType = BLTreeNode::CDPartial;

    }

    return true;
}


bool BLTree::templateP7(BLTreeNode& X, bool& earlyOut)
{
    if ( !( X.mNodeType==BLTreeNode::PType &&
            isNil(X.mCDPartialChild) &&
            !isNil(X.mSinglyPartialChild1) &&
            !isNil(X.mSinglyPartialChild2)       ) ) {

        return false;
    }

//cerr << "templateP7:[" << X.mNodeNum << "]\n";

    size_t numFull   = X.mFullChildren.size();
    size_t numEmpty  = X.mChildrenCount - (2 + numFull);

    if (numEmpty > 1) {
        return templateP7ReuseX(X, earlyOut);
    }

    list<node_list_it_t> fullChildren;  // Not used.                            
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromPTypeParent(emptyChildren);

    auto  partialItBase     = X.mSinglyPartialChild1;
    auto& PartialBase       = toNodeRef(partialItBase);
    auto  partialItAbsorbed = X.mSinglyPartialChild2;
    auto& PartialAbsorbed   = toNodeRef(partialItAbsorbed);

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {
        // Must not come here.                                                  
        emptyIt = makePNode(emptyChildren);
        toNodeRef(emptyIt).linkToQTypeParentToEnd(PartialBase, EMPTY_END);

    }
    else if (emptyChildren.size() == 1) {

        emptyIt = *(emptyChildren.begin());
        toNodeRef(emptyIt).linkToQTypeParentToEnd(PartialBase, EMPTY_END);

    }

    size_t savedPartialAbsorbedPertinentLeavesCount
                                      = PartialAbsorbed.mPertinentLeavesCount;

    PartialAbsorbed.unlinkFromPTypeParent();

    concatenateOneSinglyPartialToTheOther(
                                      PartialBase, PartialAbsorbed, EMPTY_END);


    X.mPertinentLeavesCount += savedPartialAbsorbedPertinentLeavesCount;
    X.mSinglyPartialChild1 = nil();
    X.mSinglyPartialChild2 = nil();

    PartialBase.mPertinentType = BLTreeNode::CDPartial;

    if (!isNil(mCDPartialRoot)) {

        earlyOut = true;

    }
    else {

        earlyOut = false;
        mCDPartialRoot = partialItBase;

    }

    if (X.mChildrenCount == 1) {
        auto& newX = toNodeRef(bringUpOnlyChildOfPNodeAndRemove(X));
        newX.setCDPartialInParent();
    }
    else {

        PartialBase.setCDPartialInParent();

        X.setCDPartialInParent();
        X.mPertinentType = BLTreeNode::CDPartial;

    }

    return true;
}



bool BLTree::templateP7ReuseX(BLTreeNode& X, bool& earlyOut)
{
//cerr << "templateP7ReuseX:[" << X.mNodeNum << "]\n";

    // There are more than 1 empty child.
    list<node_list_it_t> fullChildren(X.mFullChildren);

    unlinkFromPTypeParent(fullChildren);

    auto  partialItBase     = X.mSinglyPartialChild1;
    auto& PartialBase       = toNodeRef(partialItBase);
    auto  partialItAbsorbed = X.mSinglyPartialChild2;
    auto& PartialAbsorbed   = toNodeRef(partialItAbsorbed);

    PartialBase.unlinkFromPTypeParent();
    PartialAbsorbed.unlinkFromPTypeParent();

    X.mPertinentType = BLTreeNode::Empty;

    auto& P = toNodeRef(X.mParent);
    auto& CD = toNodeRef(makePNode(fullChildren));

    if (P.mNodeType == BLTreeNode::PType) {

        P.mChildren.erase(X.mChildIt);
        P.mChildrenCount--;
        X.mParent = nil();
        CD.mChildIt = P.mChildren.insert(P.mChildren.end(), CD.backIt());
        P.mChildrenCount++;
        CD.mParent  = P.backIt();

    }
    else {

        // P.mNodeType == BLTreeNode::QType ||
        // P.mNodeType == BLTreeNode::VRType

        if ( (!isNil(X.mSibling1)&& isNil(X.mSibling2))||
             ( isNil(X.mSibling1)&&!isNil(X.mSibling2))  ){

            auto& XSib = toNodeRef(isNil(X.mSibling1)?
                                                    X.mSibling2:X.mSibling1);
            if (XSib.mSibling1 == X.backIt()) {
                XSib.mSibling1 = CD.backIt();
            }
            else {
                XSib.mSibling2 = CD.backIt();
            }
            CD.mSibling1 = X.mSibling1;
            CD.mSibling2 = X.mSibling2;

            if (P.mEndChild1 == X.backIt()) {
                P.mEndChild1 = CD.backIt();
            }
            else {
                P.mEndChild2 = CD.backIt();
            }
            CD.mParent = X.mParent;

            X.mParent = nil();
            X.mSibling1 = nil();
            X.mSibling2 = nil();
        }
        else if (!isNil(X.mSibling1)&&!isNil(X.mSibling2)) {

            auto& XSib1 = toNodeRef(X.mSibling1);
            auto& XSib2 = toNodeRef(X.mSibling2);
            bool XSib1LinkOn1 = XSib1.mSibling1==X.backIt()?true:false;
            bool XSib2LinkOn1 = XSib2.mSibling1==X.backIt()?true:false;
            unlinkSiblings(XSib1, X);
            unlinkSiblings(XSib2, X);

            /* We don't use linkSiblings() as the preservation of
             * orientation is important to track Q flippings.
             */
            CD.mSibling1 = XSib1.backIt();
            CD.mSibling2 = XSib2.backIt();

            if (XSib1LinkOn1) {
                XSib1.mSibling1 = CD.backIt();
            }
            else {
                XSib1.mSibling2 = CD.backIt();
            }

            if (XSib2LinkOn1) {
                XSib2.mSibling1 = CD.backIt();
            }
            else {
                XSib2.mSibling2 = CD.backIt();
            }

            CD.mParent = X.mParent;

            X.mParent = nil();
            X.mSibling1 = nil();
            X.mSibling2 = nil();

        }
        else {

            CD.mSibling1 = nil();
            CD.mSibling2 = nil();
            CD.mParent   = X.mParent;

            P.mEndChild1 = CD.backIt();
            P.mEndChild2 = CD.backIt();

            X.mParent = nil();
            X.mSibling1 = nil();
            X.mSibling2 = nil();
        }
    }

    X.linkToQTypeParentToEnd(PartialBase, EMPTY_END);
    concatenateOneSinglyPartialToTheOther(
                                      PartialBase, PartialAbsorbed, EMPTY_END);
    PartialBase.mPertinentType = BLTreeNode::CDPartial;

    PartialBase.linkToPTypeParent(CD);

    if (!isNil(mCDPartialRoot)) {
        earlyOut = true;
    }
    else {
        earlyOut = false;
        mCDPartialRoot = partialItBase;
    }


    if (CD.mChildrenCount == 1) {
        auto& newCD = toNodeRef(bringUpOnlyChildOfPNodeAndRemove(CD));
        newCD.setCDPartialInParent();
    }
    else {

        PartialBase.setCDPartialInParent();

        CD.setCDPartialInParent();

        CD.mPertinentType = BLTreeNode::CDPartial;

    }

    return true;
}



/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type:           P
 *       Empty         : 0
 *       SinglyPartial : 0
 *       CDPartial     : 1
 *
 *   Pertinent type : Conplementarily Doubly Partial
 *
 */
bool BLTree::templateP8(BLTreeNode& X, reductionType reduction, bool& earlyOut)
{
    if ( !( X.mNodeType==BLTreeNode::PType &&
            !isNil(X.mCDPartialChild) &&
            isNil(X.mSinglyPartialChild1) &&
            isNil(X.mSinglyPartialChild2) &&
            (X.mPertinentChildrenCount == X.mChildrenCount) ) ) {
        return false;
    }

//cerr << "templateP8:[" << X.mNodeNum << "]\n";

    X.mPertinentType = BLTreeNode::CDPartial;

    earlyOut = false;

    if (reduction == NOT_FINAL_REDUCTION) {

        if (!X.setCDPartialInParent()) {

            earlyOut = true;;
        }

    }

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type          : Q
 *       Full          : ALL
 *       SinglyPartial : 0
 *       Empty         : 0
 *   Pertinent type : Full B
 */
bool BLTree::templateQ1(BLTreeNode& X, reductionType reduction)
{
    if (!(X.mNodeType == BLTreeNode::QType && checkSequenceQ1(X))) {

        return false;

    }
//cerr << "templateQ1:[" << X.mNodeNum << "]\n";

    X.mPertinentType = BLTreeNode::Full;

    if (reduction == NOT_FINAL_REDUCTION) {

        X.setFullInParent();

    }

    if (mTrackQFlippings) {

        // Sweep all the children (full), collect the node orientation
        // information, and transfer them to X.
        transferGraphNodeOrientationsQ1(X);

    }
    return true;
}


bool BLTree::checkSequenceQ1(BLTreeNode& X)
{
    /*
     *     Class of sequences it accepts in RE: ^F+$
     *
     *    Finite-state deterministic automaton transition after merging
     *    equivalent states.
     *
     *        ---
     *       | 1 |
     *    ---+---|
     *   | E | X |
     *   | P | X |
     *   | F | 1 |
     *    -------
     *
     *   Initial state: 1
     *   Acceptable final states: 1
     *   Forbidden state: X
     *
     */
    node_list_it_t prevIt = nil();
    node_list_it_t curIt  = X.mEndChild1;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if      (C.isFull())          { ; }
        else if (C.isSinglyPartial()) { return false; }
        else if (C.isCDPartial())     { return false; }
        else if (C.isEmpty())         { return false; }
        else                          { return false; }

        advanceSib(prevIt, curIt);
    }

    return true;
}


void BLTree::transferGraphNodeOrientationsQ1(BLTreeNode& X)
{
    // Scan from End1 to End2 (natural orientation), and transfer
    // the saved orientations to X (Q-node).

    auto prevIt = nil();
    auto curIt  = X.mEndChild1;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        // Assumes the natural orientation is in Sib1->C->->Sib2.
        if (C.mSibling1 == prevIt) {

            // Forward direction
            X.solveAssumedGraphNodeOrientationsFrom(C, NORMAL_DIRECTION);
        }
        else {

            // Reverse direction
            X.solveAssumedGraphNodeOrientationsFrom(C, REVERSED_DIRECTION);
        }

        advanceSib(prevIt, curIt);

    }
}



/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: Q
 *       Sequence: ^(F+SPE*|F+E+|SPE+|E+SPF*|E+F+|SPF+)$
 *       (F: Full, SP: Singly Partial, E: Empty/Nonpertinent)
 *
 *   Pertinent type : Singly Partial
 */
bool BLTree::templateQ2(BLTreeNode& X, reductionType reduction, bool& earlyOut)
{


    if ( !( X.mNodeType==BLTreeNode::QType && checkSequenceQ2(X) ) ) {

        return false;

    }

//cerr << "templateQ2:[" << X.mNodeNum << "]\n";


    node_list_it_t partialIt = nil();

    if (!isNil(X.mSinglyPartialChild1)) {

        partialIt = X.mSinglyPartialChild1;
    }
    else if (!isNil(X.mSinglyPartialChild2)) {

        partialIt = X.mSinglyPartialChild2;
    }

    if (mTrackQFlippings) {

        // Scan the full children, collect the node orientation information
        // and transfer them to X.
        transferGraphNodeOrientationsQ2(X, partialIt);

    }

    if (!isNil(partialIt)) {

        flattenSinglyPartialChildToQNode(X, partialIt, NORMAL_REDUCTION);

    }

    earlyOut = false;
    X.mPertinentType = BLTreeNode::SinglyPartial;

    if (reduction == NOT_FINAL_REDUCTION) {

        if (!X.setSinglyPartialInParent()) {

            earlyOut = true;
        }
    }    

    return true;
}


bool BLTree::checkSequenceQ2(BLTreeNode& X)
{

    if (!isNil(X.mCDPartialChild)) {

        return false;
    }

    auto& End1 = toNodeRef(X.mEndChild1);
    auto& End2 = toNodeRef(X.mEndChild2);

    if (!End1.isFull()&&!End2.isFull()) {

        // Neither side is full.
        // One side must be singly partial.
        if (End1.isSinglyPartial()||End2.isSinglyPartial()) {

            if (X.mPertinentChildrenCount==1) {

                return true;
            }
            else {

                return false;
            }
        }
        else {

            return false;
        }
    }

    node_list_it_t curIt  = End1.isFull()?X.mEndChild1:X.mEndChild2;
    node_list_it_t prevIt = nil();

    size_t pertinentChildrenFound = 0;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isFull()) {

            pertinentChildrenFound++;
        }
        else if (C.isSinglyPartial()) {

            pertinentChildrenFound++;
            break;
        }
        else if (C.isEmpty()) {

            break;
        }
        else if (C.isCDPartial()) {

            return false;
        }

        advanceSib(prevIt, curIt);

    }

    return X.mPertinentChildrenCount == pertinentChildrenFound;

}


void BLTree::transferGraphNodeOrientationsQ2(
                                        BLTreeNode& X, node_list_it_t pIt)
{
    /**
     * Transfer the node orientations of full children to X.
     * The natural direction of children is from end1 to end2.
     * The natural direction of each child is in sib1->C->sib2
     */
    auto& XEnd1 = toNodeRef(X.mEndChild1);
    auto& XEnd2 = toNodeRef(X.mEndChild2);

    node_list_it_t curIt;

    // We can't use isFull() to check the full side,
    // as X may have a singly partial node.
    bool XEnd1IsFull;
    if ( XEnd1.isFull() || (XEnd1.isSinglyPartial() && XEnd2.isEmpty()) ) {

        curIt = X.mEndChild1;
        XEnd1IsFull = true;

    }
    else {

        curIt = X.mEndChild2;
        XEnd1IsFull = false;

    }

    // Scan from the full end.
    auto prevIt = nil();
    
    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isEmpty()) {
            break;
        }

        BLTree::scanDirectionType direction;

        if( ( C.mSibling1 == prevIt &&  XEnd1IsFull )||
            ( C.mSibling1 != prevIt && !XEnd1IsFull )  ) {
            direction = BLTree::NORMAL_DIRECTION;
        }
        else {
            direction = BLTree::REVERSED_DIRECTION;
            // Forward direction
        }

        X.solveAssumedGraphNodeOrientationsFrom(C, direction);

        advanceSib(prevIt, curIt);
    }


    if (!isNil(pIt)) {

        // Transfer the node orientation of the partial Q to X.
        auto& P     = toNodeRef(pIt);
        auto& PEnd1 = toNodeRef(P.mEndChild1);

        BLTree::scanDirectionType direction;

        if ( ( XEnd1IsFull &&  PEnd1.isFull()) ||
             (!XEnd1IsFull && !PEnd1.isFull())   ) {
            direction = BLTree::NORMAL_DIRECTION;
        }
        else {
            direction = BLTree::REVERSED_DIRECTION;
        }

        X.transferGraphNodeOrientationsFrom(P, direction);

    }
}


void BLTree::flattenSinglyPartialChildToQNode(
    BLTreeNode&    P,
    node_list_it_t cIt,
    reductionType  type
){

    auto& C = toNodeRef(cIt);

    // Partially Unlink C from P.
    if(P.mSinglyPartialChild1 == cIt) {

        P.mSinglyPartialChild1 = nil();

    }
    else if(P.mSinglyPartialChild2 == cIt) {

        P.mSinglyPartialChild2 = nil();

    }

    P.mPertinentChildrenCount--;
    P.mPertinentLeavesCount -= C.mPertinentLeavesCount;

    // relink parent of full children of C to P.
    node_list_it_t prevIt = nil();
    node_list_it_t curIt  = (C.isEndChild1Full())?C.mEndChild1:C.mEndChild2;
    while (!isNil(curIt)) {

        auto& GC = toNodeRef(curIt);
        if (GC.isEmpty()) {
            break;
        }

        GC.mParent  = P.backIt();
        P.mPertinentChildrenCount++;
        P.mPertinentLeavesCount += GC.mPertinentLeavesCount;
        GC.discardOldFullLink();
        GC.createFullLink(P);

        advanceSib(prevIt, curIt);
    }

    // Link siblings and the end child of P.
    auto& CEnd1 = toNodeRef(C.mEndChild1);
    auto& CEnd2 = toNodeRef(C.mEndChild2);

    if (P.mEndChild1 == cIt || P.mEndChild2 == cIt) {

        /* C is at an end of P.
         *
         *             P
         *       ______|_______
         *      |              |
         *      C - Csib ...   N
         */
        auto& CSib = toNodeRef(isNil(C.mSibling1)?C.mSibling2:C.mSibling1);
        bool CSibLinkOn1 = CSib.mSibling1==C.backIt()?true:false;
        unlinkSiblings(CSib,C);

        bool CSibFull = (type==NORMAL_REDUCTION)?
                        (CSib.isFull()||CSib.isSinglyPartial()) : 
                        CSib.isFull();

        if ((CEnd1.isFull() && CSibFull) || (!CEnd1.isFull() && !CSibFull) ) {

            /**
             *              P                               P
             *         _____|______                  _______|_____
             *        |            |                |             |
             *        C - CSib(F)  N                C - CSib(E)   N
             * CEnd1__|_CEnd2                CEnd1__|_CEnd2       
             *   |         |                   |        |
             *   F         E                   E        F
             */

            if (CSibLinkOn1) {
                CSib.mSibling1 = CEnd1.backIt();
            }
            else {
                CSib.mSibling2 = CEnd1.backIt();
            }

            if (isNil(CEnd1.mSibling1)) {
                CEnd1.mSibling1 = CSib.backIt();
            }
            else {
                CEnd1.mSibling2 = CSib.backIt();
            }

            if (P.mEndChild1 == cIt) {
                P.mEndChild1 = C.mEndChild2;
            }
            else {
                P.mEndChild2 = C.mEndChild2;
            }

            CEnd2.mParent = P.backIt();

        }
        else {

            /**
             *              P                               P
             *         _____|______                  _______|_____
             *        |            |                |             |
             *        C - CSib(F)  N                C - CSib(E)   N
             * CEnd1__|_CEnd2                CEnd1__|_CEnd2       
             *   |         |                   |        |
             *   E         F                   F        E
             */
            if (CSibLinkOn1) {
                CSib.mSibling1 = CEnd2.backIt();
            }
            else {
                CSib.mSibling2 = CEnd2.backIt();
            }

            if (isNil(CEnd2.mSibling1)) {
                CEnd2.mSibling1 = CSib.backIt();
            }
            else {
                CEnd2.mSibling2 = CSib.backIt();
            }

            if (P.mEndChild1 == cIt) {

                P.mEndChild1 = C.mEndChild1;
            }
            else {

                P.mEndChild2 = C.mEndChild1;
            }
            CEnd1.mParent = P.backIt();

        }

    }
    else {

        /**
         *           P
         *      _____|_____________
         *     |         |         |
         *     N CSib1 - C - CSib2 N
         *        CEnd1__|__CEnd2
         *         |          |
         *         E          F
         */
        auto& CSib1 = toNodeRef(C.mSibling1);
        bool CSib1Full = (type==NORMAL_REDUCTION)?
                         (CSib1.isFull()||CSib1.isSinglyPartial()):
                         CSib1.isFull();
        bool CSib1LinkOn1 = CSib1.mSibling1==C.backIt()?true:false;

        unlinkSiblings(CSib1,C);

        auto& CSib2 = toNodeRef(C.mSibling2);
        bool CSib2LinkOn1 = CSib2.mSibling1==C.backIt()?true:false;

        unlinkSiblings(CSib2,C);

        if ((CEnd1.isFull()&&CSib1Full)||(!CEnd1.isFull()&&!CSib1Full) ) {

            if (CSib1LinkOn1) {
                CSib1.mSibling1 = CEnd1.backIt();
            }
            else {
                CSib1.mSibling2 = CEnd1.backIt();
            }

            if (isNil(CEnd1.mSibling1)) {
                CEnd1.mSibling1 = CSib1.backIt();
            }
            else {
                CEnd1.mSibling2 = CSib1.backIt();
            }

            if (CSib2LinkOn1) {
                CSib2.mSibling1 = CEnd2.backIt();
            }
            else {
                CSib2.mSibling2 = CEnd2.backIt();
            }

            if (isNil(CEnd2.mSibling1)) {
                CEnd2.mSibling1 = CSib2.backIt();
            }
            else {
                CEnd2.mSibling2 = CSib2.backIt();
            }

        }
        else {

            if (CSib1LinkOn1) {
                CSib1.mSibling1 = CEnd2.backIt();
            }
            else {
                CSib1.mSibling2 = CEnd2.backIt();
            }

            if (isNil(CEnd2.mSibling1)) {
                CEnd2.mSibling1 = CSib1.backIt();
            }
            else {
                CEnd2.mSibling2 = CSib1.backIt();
            }

            if (CSib2LinkOn1) {
                CSib2.mSibling1 = CEnd1.backIt();
            }
            else {
                CSib2.mSibling2 = CEnd1.backIt();
            }

            if (isNil(CEnd1.mSibling1)) {
                CEnd1.mSibling1 = CSib2.backIt();
            }
            else {
                CEnd1.mSibling2 = CSib2.backIt();
            }

        }
    }

    C.discardOldFullLink();
    C.clearFullChildren();
    removeNode(C);

}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: Q
 *       Sequence: ^(E+F+E+|E+F+SPE*|E*SPF*SPE*|E*SPF+E+)$
 *       (F: Full, SP: Singly Partial, E: Empty/Nonpertinent)
 *
 *   Pertinent type : Doubly Partial
 *
 *   This is a final reduction.
 */
bool BLTree::templateQ3(BLTreeNode& X)
{
    if ( !( ( X.mNodeType==BLTreeNode::QType          ||
              X.mNodeType==BLTreeNode::VirtualRootType  ) &&
            checkSequenceQ3(X) )) {

        return false;

    }
//cerr << "templateQ3:[" << X.mNodeNum << "]\n";

    if (mTrackQFlippings) {

        transferGraphNodeOrientationsQ3(X);

    }

    if (!isNil(X.mSinglyPartialChild1)) {

        flattenSinglyPartialChildToQNode(
                                  X, X.mSinglyPartialChild1, NORMAL_REDUCTION);

    }

    if (!isNil(X.mSinglyPartialChild2)) {

        flattenSinglyPartialChildToQNode(
                                  X, X.mSinglyPartialChild2, NORMAL_REDUCTION);

    }

    X.mPertinentType = BLTreeNode::DoublyPartial;

    return true;
}


bool BLTree::checkSequenceQ3(BLTreeNode& X)
{

    if (!isNil(X.mCDPartialChild)) {

        return false;

    }

    if (X.mFullChildren.size() == 0) {

        // There are no full children.
        // There must be two consecutive singly partial children.
        if (!isNil(X.mSinglyPartialChild1) && 
            !isNil(X.mSinglyPartialChild2) && 
            X.mPertinentChildrenCount==2 ) {

            auto& Part1 = toNodeRef(X.mSinglyPartialChild1);
            if ( (Part1.mSibling1 == X.mSinglyPartialChild2)||
                 (Part1.mSibling2 == X.mSinglyPartialChild2)   ){

                return true;
            }
            else {

                return false;
            }
        }
        else {        

            return false;
        }
    }

    // There is at least one full child.
    size_t pertinentChildrenFound = 0;

    node_list_it_t curIt  = *(X.mFullChildren.rbegin());
    node_list_it_t prevIt = toNodeRef(curIt).mSibling1;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isFull()) {

            pertinentChildrenFound++;
        }
        else if (C.isSinglyPartial()) {

            pertinentChildrenFound++;
            break;
        }
        else if (C.isEmpty()) {

                break;
        }

        advanceSib(prevIt, curIt);

    }

    prevIt = *(X.mFullChildren.rbegin());
    curIt  = toNodeRef(prevIt).mSibling1;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isFull()) {
            pertinentChildrenFound++;
        }
        else if (C.isSinglyPartial()) {

            pertinentChildrenFound++;
            break;
        }
        else if (C.isEmpty()) {

            break;
        }

        advanceSib(prevIt, curIt);

    }

    return X.mPertinentChildrenCount == pertinentChildrenFound;

}


void BLTree::transferGraphNodeOrientationsQ3(BLTreeNode& X)
{
    if (X.mNodeType == BLTreeNode::QType) {

        // All the children are pertinent. Scanning from end1 to end2.
        auto prevIt = nil();
        auto curIt  = X.mEndChild1;

        while (!isNil(curIt)) {

            auto& C = toNodeRef(curIt);

            
            if (C.mSibling1 == prevIt) {
                // C's natural orientation is aligned with X's.
                X.solveAssumedGraphNodeOrientationsFrom(C, NORMAL_DIRECTION);
            }
            else {
                X.solveAssumedGraphNodeOrientationsFrom(C, REVERSED_DIRECTION);
            }

            if( (X.mSinglyPartialChild1 == curIt) || 
                (X.mSinglyPartialChild2 == curIt)   ) {

                auto& CEnd1 = toNodeRef(C.mEndChild1);
                
                if (isNil(prevIt)) {
                    if (CEnd1.isFull()) {
                        X.transferGraphNodeOrientationsFrom(
                                                        C, REVERSED_DIRECTION);
                    }
                    else {
                        X.transferGraphNodeOrientationsFrom(
                                                          C, NORMAL_DIRECTION);
                    }
                }
                else {
                    auto& Prev = toNodeRef(prevIt);
                    if ( Prev.isFull() || Prev.isSinglyPartial() ) {
                        if (CEnd1.isFull()) {
                            X.transferGraphNodeOrientationsFrom(
                                                          C, NORMAL_DIRECTION);
                        }
                        else {
                            X.transferGraphNodeOrientationsFrom(
                                                        C, REVERSED_DIRECTION);
                        }
                    }
                    else {
                        if (CEnd1.isFull()) {
                            X.transferGraphNodeOrientationsFrom(
                                                        C, REVERSED_DIRECTION);
                        }
                        else {
                            X.transferGraphNodeOrientationsFrom(
                                                          C, NORMAL_DIRECTION);
                        }
                    }
                }
            }

            advanceSib(prevIt, curIt);

        }
    }
   
    else { //(X.mNodeType == BLTreeNode::VirtualRootType)

        // We don't know the true parent.
        // First we find the boundary node.
        node_list_it_t prevIt;
        node_list_it_t curIt;

        if (X.mFullChildren.size() == 0) {

            // There must be two singly partial children, which 
            // are an immediate sibling to each other.
            curIt = X.mSinglyPartialChild1;

            auto& PC1 = toNodeRef(X.mSinglyPartialChild1);

            if (PC1.mSibling1==X.mSinglyPartialChild2) {

                prevIt = PC1.mSibling2;

            }
            else {

                prevIt = PC1.mSibling1;

            }
        }

        else {

            // There is a full children somewhere in X.
            // Pick X.mSibling1 arbitrarily for the direction of searching.
            prevIt = toNodeRef(*(X.mFullChildren.rbegin())).mSibling2;
            curIt  = *(X.mFullChildren.rbegin());

            while (!isNil(curIt)) {

                auto& C = toNodeRef(curIt);      

                if (C.isEmpty()) {
                    break;
                }

                advanceSib(prevIt, curIt);
            }

            std::swap(curIt, prevIt);
            // Now curIt points to the end of pertinent nodes, and 
            // prevIt is the boundary non-pertinent node.
        }

        /*
         * Now curIt is at the boundary pertinent node.
         * Scanning from one end to the other 
         * assuming the direction in the following while loop is in 
         * the parent's end1->end2.
         */
        while (!isNil(curIt)) {

            auto& C = toNodeRef(curIt);

            if (C.isEmpty()) {
                break;
            }

            if (C.mSibling1 == prevIt) {
                // C's natural orientation is aligned with X's.
                X.solveAssumedGraphNodeOrientationsFrom(C, NORMAL_DIRECTION);
            }
            else {
                X.solveAssumedGraphNodeOrientationsFrom(C, REVERSED_DIRECTION);
            }

            if( (X.mSinglyPartialChild1==curIt) ||
                (X.mSinglyPartialChild2==curIt)    ) {

                auto& CEnd1 = toNodeRef(C.mEndChild1);
                
                if (isNil(prevIt)) {

                    if (CEnd1.isFull()) {
                        X.transferGraphNodeOrientationsFrom(
                                                        C, REVERSED_DIRECTION);
                    }
                    else {
                        X.transferGraphNodeOrientationsFrom(
                                                        C, NORMAL_DIRECTION);
                    }

                }
                else {

                    auto& Prev = toNodeRef(prevIt);
                    if ( Prev.isFull() || Prev.isSinglyPartial() ) {
                        if (CEnd1.isFull()) {
                            X.transferGraphNodeOrientationsFrom(
                                                        C, NORMAL_DIRECTION);
                        }
                        else {
                            X.transferGraphNodeOrientationsFrom(
                                                        C, REVERSED_DIRECTION);
                        }
                    }
                    else {
                        if (CEnd1.isFull()) {
                            X.transferGraphNodeOrientationsFrom(
                                                        C, REVERSED_DIRECTION);
                        }
                        else {
                            X.transferGraphNodeOrientationsFrom(
                                                        C, NORMAL_DIRECTION);
                        }
                    }

                }
            }

            advanceSib(prevIt, curIt);
        }


        // Now the the node orientaitons are accumulated to the virtual
        // root. Transfer them to the next sibling in the direction above, 
        // assuming the next sibling's direction is in sib1->next->sib2.

        auto& Next = toNodeRef(curIt);

        if (Next.mSibling1 == prevIt) {

            // Next's natural orientation is aligend with the assumed
            // orientation.
            Next.assumeGraphNodeOrientationsFrom(X, NORMAL_DIRECTION);
        }
        else {

            Next.assumeGraphNodeOrientationsFrom(X, REVERSED_DIRECTION);
        }
    }
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: Q
 *       Sequence: ^(F+E+F+|F+E+SPF*|F*SPE*SPF*|F*SPE+F+)$
 *       (F: Full, SP: Singly Partial, E: Empty/Nonpertinent)
 *
 *   Pertinent type : Complementarily Doubly Partial
 *
 */
bool BLTree::templateQ4(BLTreeNode& X, reductionType reduction, bool& earlyOut)
{
    if ( !(X.mNodeType==BLTreeNode::QType && checkSequenceQ4(X)) ) {
           
        return false;

    }
//cerr << "templateQ4:[" << X.mNodeNum << "]\n";

    if (mTrackQFlippings) {

        transferGraphNodeOrientationsQ4(X);

    }

    if (!isNil(X.mSinglyPartialChild1)) {

        flattenSinglyPartialChildToQNode(
                           X, X.mSinglyPartialChild1, COMPLEMENTARY_REDUCTION);
    }

    if (!isNil(X.mSinglyPartialChild2)) {

        flattenSinglyPartialChildToQNode(
                           X, X.mSinglyPartialChild2, COMPLEMENTARY_REDUCTION);
    }

    X.mPertinentType = BLTreeNode::CDPartial;

    earlyOut = false;
    if (!isNil(mCDPartialRoot)) {

        earlyOut = true;
        return true;

    }

    mCDPartialRoot = X.backIt();

    if (reduction==NOT_FINAL_REDUCTION) {

        X.setCDPartialInParent();

    }

    return true;
}


bool BLTree::checkSequenceQ4(BLTreeNode& X)
{
    if (!isNil(X.mCDPartialChild)) {
        return false;
    }

    // Scan from EndChild1 inwards.
    size_t pertinentChildrenFound = 0;
    node_list_it_t curIt     = X.mEndChild1;
    node_list_it_t prevIt    = nil();
    node_list_it_t partIt    = nil();
    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isFull()) {
            pertinentChildrenFound++;
        }
        else if (C.isSinglyPartial()) {
            partIt = curIt;
            pertinentChildrenFound++;
            break;
        }
        else if (C.isEmpty()) {
            break;
        }

        advanceSib(prevIt, curIt);

    }

    // Scan from EndChild2 inwards.
    curIt  = X.mEndChild2;
    prevIt = nil();

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isFull()) {
            pertinentChildrenFound++;
        }
        else if (C.isSinglyPartial()) {
            if (partIt != curIt){
                pertinentChildrenFound++;
            }
            break;
        }
        else if (C.isEmpty()) {
            break;
        }

        advanceSib(prevIt, curIt);

    }

    return X.mPertinentChildrenCount == pertinentChildrenFound;

}


void BLTree::transferGraphNodeOrientationsQ4(BLTreeNode& X)
{

    // Scan from the end1.
    auto prevIt = nil();
    auto curIt  = X.mEndChild1;    
    auto partIt = nil();
    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isEmpty()) {

            break;
        }

        if (C.mSibling1 == prevIt) {

            X.solveAssumedGraphNodeOrientationsFrom(C, NORMAL_DIRECTION);
        }
        else {

            X.solveAssumedGraphNodeOrientationsFrom(C, REVERSED_DIRECTION);
        }

        if ( X.mSinglyPartialChild1==curIt || X.mSinglyPartialChild2==curIt ) {

            // Transfer the node orientation of the partial Q to X.
            auto& P     = toNodeRef(curIt);
            auto& PEnd1 = toNodeRef(P.mEndChild1);
            if (PEnd1.isFull()) {
                X.transferGraphNodeOrientationsFrom(P, NORMAL_DIRECTION);
            }
            else {
                X.transferGraphNodeOrientationsFrom(P, REVERSED_DIRECTION);
            }
            partIt = curIt;
            break;
        }        
        advanceSib(prevIt, curIt);
    }

    // Scan from the end2.
    prevIt = nil();
    curIt = X.mEndChild2;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isEmpty()) {

            break;
        }

        if (C.mSibling1 == prevIt) {

            X.solveAssumedGraphNodeOrientationsFrom(C, REVERSED_DIRECTION);
        }
        else {

            X.solveAssumedGraphNodeOrientationsFrom(C, NORMAL_DIRECTION);
        }

        if ( X.mSinglyPartialChild1==curIt || X.mSinglyPartialChild2==curIt ) {

            if (curIt != partIt) {            

                // Transfer the node orientation of the partial Q to X.
                auto& P     = toNodeRef(curIt);
                auto& PEnd1 = toNodeRef(P.mEndChild1);
                if (PEnd1.isFull()) {
                    X.transferGraphNodeOrientationsFrom(P, REVERSED_DIRECTION);
                }
                else {
                    X.transferGraphNodeOrientationsFrom(P, NORMAL_DIRECTION);
                }
            }
            break;

        }        
        advanceSib(prevIt, curIt);
    }
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type:           Q
 *       Empty         : 0
 *       SinglyPartial : 0
 *       CDPartial     : 1
 *   Pertinent type : Complementarily Doubly Partial
 *   This is a final reduction.
 */
bool BLTree::templateQ5(BLTreeNode& X, reductionType reduction, bool& earlyOut)
{
    if ( !( X.mNodeType==BLTreeNode::QType &&
            !isNil(X.mCDPartialChild) &&
            isNil(X.mSinglyPartialChild1) &&
            isNil(X.mSinglyPartialChild2)   ) ) {
        return false;
    }
//cerr << "templateQ5:[" << X.mNodeNum << "]\n";

    node_list_it_t prevIt = nil();
    node_list_it_t curIt  = X.mEndChild1;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        if (C.isEmpty()) {
            return false;;
        }

        advanceSib(prevIt, curIt);
    }

    if (mTrackQFlippings) {

        transferGraphNodeOrientationsQ5(X);

    }

    X.mPertinentType = BLTreeNode::CDPartial;

    earlyOut = false;
    if (reduction==NOT_FINAL_REDUCTION) {

        if (!X.setCDPartialInParent()) {

            earlyOut = true;;

        }

    }
    return true;
}


void BLTree::transferGraphNodeOrientationsQ5(BLTreeNode& X)
{
    // Scan from End1 to End2 (natural orientation), and transfer
    // the saved orientations to X (Q-node).

    auto prevIt = nil();
    auto curIt  = X.mEndChild1;

    while (!isNil(curIt)) {

        auto& C = toNodeRef(curIt);

        // Assumes the natural orientation is in Sib1->C->->Sib2.
        if (C.mSibling1 == prevIt) {

            // Forward direction
            X.solveAssumedGraphNodeOrientationsFrom(C, NORMAL_DIRECTION);
        }
        else {

            // Reverse direction
            X.solveAssumedGraphNodeOrientationsFrom(C, REVERSED_DIRECTION);
        }

        advanceSib(prevIt, curIt);

    }
}


#ifdef UNIT_TESTS
void BLTree::printTree(ostream& os, node_list_it_t pr) {

    // Find the root.
    os << "==========TREE BEGIN==========\n";
//    os << "numNodes: " << numNodes() << "\n";
//    for (auto tt  = nodes().first; tt != nodes().second; tt++) {
//        auto& N = toNodeRef(tt);
//        os << "N:" << N.mNodeNum << "\n";
//    }

    auto pIt = nodes().first;
    while (!isNil(pIt)) {
        auto& P = toNodeRef(pIt);
        if (!isNil(P.mSibling1)&&!isNil(P.mSibling2)) {
            auto it1 = P.mSibling1;
            auto it2 = pIt;
            while (!isNil(it2)) {
                advanceSib(it1, it2);
            }

            auto& P2 = toNodeRef(it1);
            if (isNil(P2.mParent)) {
                break;
            }
            else {
                pIt = P2.mParent;
            }
        }
        else {
            if (isNil(P.mParent)) {
                break;
            }
            else {
                pIt = P.mParent;
            }
        }
    }

    list<node_list_it_t> nqueue;
    nqueue.push_back(pIt);
    while (nqueue.size() > 0) {
      
        auto& N = toNodeRef(*(nqueue.begin()));
        nqueue.pop_front();
        if (N.mNodeType == BLTreeNode::PType) {

            os << "P" << N.mNodeNum;
            if (N.mGeneration == mGeneration&& !(N.isEmpty())) {
                os << "*";
            }
            os << " [";
            // Print the children
            for (auto cIt : N.mChildren) {            
                auto& C = toNodeRef(cIt);
                if (C.mNodeType == BLTreeNode::LType) {
                    auto& E = dynamic_cast<BLGraphEdge&>(*(*(C.mGraphEdge)));
                    auto& EOrg = dynamic_cast<Edge&>(E.IGBackwardLinkRef());
                    auto& N1 = dynamic_cast<NumNode&>(EOrg.incidentNode1());
                    auto& N2 = dynamic_cast<NumNode&>(EOrg.incidentNode2());
                    os << "L" << C.mNodeNum;
                    if (C.mGeneration == mGeneration && !C.isEmpty()) {
                        os << "*";
                    }
                    if (pr == cIt) {
                        os << "R";
                    }
                    os << "{" << N1.num() << "," << N2.num() << "}, ";

                     if (C.mAssumedOrientInNorm.size()>0||
                         C.mAssumedOrientInReversed.size()>0||
                         C.mAssumedOrientOutNorm.size()>0||
                         C.mAssumedOrientOutReversed.size()>0) {
                        os << " AFI:[";
                        for (auto nit : C.mAssumedOrientInNorm) {
                            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                            os << NOrg.num() << ",";
                        }
                        os << "]  ABI:[";
                        for (auto nit : C.mAssumedOrientInReversed) {
                            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                            os << NOrg.num() << ",";
                        }
                        os << "]  AFO:[";
                        for (auto nit : C.mAssumedOrientOutNorm) {
                            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                            os << NOrg.num() << ",";
                        }
                        os << "]  ABO:[";
                        for (auto nit : C.mAssumedOrientOutReversed) {
                             auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                             auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                             os << NOrg.num() << ",";
                        }
                        os << "], ";
                    }

                }
                else if (C.mNodeType == BLTreeNode::PType) {
                    os << "P" << C.mNodeNum;
                    if (C.mGeneration == mGeneration && !C.isEmpty()) {
                        os << "*";
                    }
                    if (pr == cIt) {
                        os << "R";
                    }
                    os << ", ";
                    nqueue.push_back(cIt);
                }
                else if (C.mNodeType == BLTreeNode::QType) {
                    os << "Q" << C.mNodeNum;
                    if (C.mGeneration == mGeneration && !C.isEmpty()) {
                        os << "*";
                    }
                    if (pr == cIt) {
                        os << "R";
                    }
                    os << ", ";
                    nqueue.push_back(cIt);
                }
            }

        }
        else if (N.mNodeType == BLTreeNode::QType) {

            os << "Q" << N.mNodeNum;
            if (N.mGeneration == mGeneration && !N.isEmpty()) {
                os << "*";
            }
            os << " [";
            // Print the children
            auto prevIt = nil();
            auto curIt  = N.mEndChild1;
            while (!isNil(curIt)) {
                auto& C = toNodeRef(curIt);
                bool natural;
                if (C.mSibling1 == prevIt) {
                    natural = true;
                }
                else {
                    natural = false;
                }
                if (C.mNodeType == BLTreeNode::LType) {
                    auto& E = dynamic_cast<BLGraphEdge&>(*(*(C.mGraphEdge)));
                    auto& EOrg = dynamic_cast<Edge&>(E.IGBackwardLinkRef());
                    auto& N1 = dynamic_cast<NumNode&>(EOrg.incidentNode1());
                    auto& N2 = dynamic_cast<NumNode&>(EOrg.incidentNode2());
                    os << "L" << C.mNodeNum;
                    if (C.mGeneration == mGeneration && !C.isEmpty()) {
                        os << "*";
                    }
                    if (pr == curIt) {
                        os << "R";
                    }
                    if (natural) {
                       os << " -> ";
                    }
                    else {
                       os << " <- ";
                    }
                    os << "{" << N1.num() << "," << N2.num() << "}, ";

                     if (C.mAssumedOrientInNorm.size()>0||
                         C.mAssumedOrientInReversed.size()>0||
                         C.mAssumedOrientOutNorm.size()>0||
                         C.mAssumedOrientOutReversed.size()>0) {
                        os << " AFI:[";
                        for (auto nit : C.mAssumedOrientInNorm) {
                            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                            os << NOrg.num() << ",";
                        }
                        os << "]  ABI:[";
                        for (auto nit : C.mAssumedOrientInReversed) {
                            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                            os << NOrg.num() << ",";
                        }
                        os << "]  AFO:[";
                        for (auto nit : C.mAssumedOrientOutNorm) {
                            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                            os << NOrg.num() << ",";
                        }
                        os << "]  ABO:[";
                        for (auto nit : C.mAssumedOrientOutReversed) {
                             auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
                             auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
                             os << NOrg.num() << ",";
                        }
                        os << "], ";
                    }
                }
                else if (C.mNodeType == BLTreeNode::PType) {
                    os << "P" << C.mNodeNum;
                    if (C.mGeneration == mGeneration && !C.isEmpty()) {
                        os << "*";
                    }
                    if (pr == curIt) {
                        os << "R";
                    }
                    if (natural) {
                       os << " -> ";
                    }
                    else {
                       os << " <- ";
                    }
                    os << ", ";
                    nqueue.push_back(curIt);
                }
                else if (C.mNodeType == BLTreeNode::QType) {
                    os << "Q" << C.mNodeNum;
                    if (C.mGeneration == mGeneration && !C.isEmpty()) {
                        os << "*";
                    }
                    if (pr == curIt) {
                        os << "R";
                    }
                    if (natural) {
                       os << " -> ";
                    }
                    else {
                       os << " <- ";
                    }
                    os << ", ";
                    nqueue.push_back(curIt);
                }
                advanceSib(prevIt, curIt);
            }
        }
        os << "]  FI:[";
        for (auto nit : N.mOrientInNorm) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]  BI:[";
        for (auto nit : N.mOrientInReversed) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]  FO:[";
        for (auto nit : N.mOrientOutNorm) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]  BO:[";
        for (auto nit : N.mOrientOutReversed) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }

        os << "]  AFI:[";
        for (auto nit : N.mAssumedOrientInNorm) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]  ABI:[";
        for (auto nit : N.mAssumedOrientInReversed) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]  AFO:[";
        for (auto nit : N.mAssumedOrientOutNorm) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]  ABO:[";
        for (auto nit : N.mAssumedOrientOutReversed) {
            auto& NCopy = dynamic_cast<BLGraphNode&>(*(*(nit)));
            auto& NOrg  = dynamic_cast<NumNode&>(NCopy.IGBackwardLinkRef());
            os << NOrg.num() << ",";
        }
        os << "]\n";

    }
    os << "==========TREE END==========\n";
}
#endif /*UNIT_TESTS*/

}// namespace Undirected

}// namespace Wailea
