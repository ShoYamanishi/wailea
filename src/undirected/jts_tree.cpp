#include "undirected/jts_planarizer.hpp"
#include "undirected/jts_tree.hpp"
#include "undirected/jts_tree_node.hpp"
#include "undirected/jts_inline.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/jts_tree.cpp
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

node_list_it_t JTSTree::makeInitialAttachmentP()
{
    auto  np    = make_unique<JTSTreeNode>(*this);
    auto  nIt   = addNode(std::move(np)).backIt();
    auto& N     = toNodeRef(nIt);
    N.mNodeType = JTSTreeNode::PType;

    return nIt;
}


node_list_it_t JTSTree::findPertinentRoot(node_list_it_t leaf)
{
    auto aIt = leaf;
    list<node_list_it_t> pathToLeaf; // Path from a pertinent
                                     // ancestor to the leaf.
    while(!isNil(aIt)){
        pathToLeaf.push_front(aIt);
        auto& A = toNodeRef(aIt);
        if (isNil(A.mParent)) {
            break;
        }
        else {
            auto& P = toNodeRef(A.mParent);
            if (!P.isPertinent()) {
                break;
            }
            aIt = A.mParent;
        }
    }

    if (pathToLeaf.size() <= 1){
       return aIt;
    }

    for (auto pIt : pathToLeaf) {
        // The pertinent root is the first one from the ancestor
        // that has multiple pertinent children.
        auto& N = toNodeRef(pIt);
        if (N.mNodeType == JTSTreeNode::LType ||
                                              N.mPertinentChildrenCount > 1) {
            return pIt;
        }
    }

    return nil();
}


// This is called in JTSPlanarizer::removePertinentTree(), 
// templateP4(), templateP5(), templateP6().
void JTSTree::transferContentsOfOnlyChildAndRemove(JTSTreeNode& X)
{

    auto& C = toNodeRef(*(X.mChildren.begin()));

    X.mNodeType               = C.mNodeType;
    X.mChildren.clear();
    for (auto gcIt : C.mChildren) {
        auto& GC = toNodeRef(gcIt);
        GC.linkToParent(X);
    }
    X.mPertinentType          = C.mPertinentType;
    X.mPertinentChildrenCount = C.mPertinentChildrenCount;
    X.mGraphEdge              = C.mGraphEdge;
    X.mFullChildrenCount      = C.mFullChildrenCount;
    X.mPartialChild1          = C.mPartialChild1;
    X.mPartialChild2          = C.mPartialChild2;
    X.mCDPartialChild         = C.mCDPartialChild;
    if (X.mNodeType == JTSTreeNode::LType) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*(X.mGraphEdge)));
        E.mPQLeaf = X.backIt();
    }
    removeNode(C);

}


void JTSTree::removePertinentNodeIfNecessary(JTSTreeNode& X)
{
    if (X.mNodeType == JTSTreeNode::LType) {
        // Pertinent leaf.
        if (X.mWHABType == JTSTreeNode::W) {
            X.markGraphEdgeRemoved();
            X.unlinkFromParent();
            removeNode(X);
        }
        return;        
    }
    else if (X.mChildren.size() == 0) {
        // P or Q node has no children.
        // Remove.
        X.unlinkFromParent();
        removeNode(X);
        return;

    }
    else if (X.mChildren.size() == 1) {
        // P or Q node has one child.
        // Bring the child up to this node.
        transferContentsOfOnlyChildAndRemove(X);
    }

    if (X.mPertinentChildrenCount == 0) {
        // There is no pertinent children. Make this empty.
        X.decrementPertinentChildrenCountOfParent();
        X.mPertinentType = JTSTreeNode::Empty;
    }
    // Full child never becomes a Partial child.
}


/**
 *   Assumption: Number of children is 0.
 *   Conditions:
 *       Type: L
 *   Pertinent type : Full B
 */
bool JTSTree::templateL1(JTSTreeNode& X)
{
    if (X.mNodeType!=JTSTreeNode::LType) {
        return false;
    }
//cerr << "templateL1[" << X.mNodeNum << "]\n";
    X.setFullInParent();
    X.mPertinentType = JTSTreeNode::Full;
    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Full     : ALL
 *       Partial  : 0
 *       Empty    : 0
 *   Pertinent type : Full B
 */
bool JTSTree::templateP1(JTSTreeNode& X)
{
    if ( !(X.mNodeType == JTSTreeNode::PType &&
           X.mFullChildrenCount == X.mChildren.size() &&
           isNil(X.mPartialChild1) &&
           isNil(X.mPartialChild2)                     ) ) {
        return false;
    }
//cerr << "templateP1[" << X.mNodeNum << "]\n";
    X.setFullInParent();
    X.mPertinentType = JTSTreeNode::Full;
    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Full     : greater than 1
 *       Partial  : greater than 0
 *       Empty    : 0
 *   Pertinent type : N/A (the root is Full B)
 *   This is a final reduction.
 */
bool JTSTree::templateP2(JTSTreeNode& X, node_list_it_t& pertinentRoot)
{
    if ( !( X.mNodeType == JTSTreeNode::PType &&
            X.mFullChildrenCount >= 2         &&
            isNil(X.mPartialChild1)           &&
            isNil(X.mPartialChild2)              ) ) {
        return false;
    }
//cerr << "templateP2[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    node_list_it_t nIt;
    if (fullChildren.size() > 1) {

        unlinkFromParent(X, fullChildren);
        nIt = makePNode(fullChildren);
        toNodeRef(nIt).linkToParent(X);
    }

    pertinentRoot  = nIt;

    return true;
}


void JTSTree::unlinkFromParent(JTSTreeNode& P, list<node_list_it_t>& children)
{
    for(auto cIt : children) {
        auto& C = toNodeRef(cIt);
        C.unlinkFromParent();
    }
}


node_list_it_t JTSTree::makePNode(list<node_list_it_t>& children)
{
    auto  np  = make_unique<JTSTreeNode>(*this);
    auto  nIt = addNode(std::move(np)).backIt();
    auto& N   = toNodeRef(nIt);

    N.mNodeType      = JTSTreeNode::PType;
    N.mGeneration    = mGeneration;

    for (auto cIt : children) {
        auto& C = toNodeRef(cIt);
        C.linkToParent(N);
    }

    if (N.mPertinentChildrenCount == N.mChildren.size()){

        N.mPertinentType = JTSTreeNode::Full;
    }
    else if (N.mPertinentChildrenCount == 0) {

        N.mPertinentType = JTSTreeNode::Empty;
    }
    else {

        N.mPertinentType = JTSTreeNode::Partial;
    }

    return nIt;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Full     : greater than 0
 *       Partial  : greater than 0
 *       Empty    : 0
 *   Pertinent type : Partial H
 */
bool JTSTree::templateP3(JTSTreeNode& X)
{
    if ( !( X.mNodeType==JTSTreeNode::PType &&
            X.mFullChildrenCount < X.mChildren.size() &&
            X.mFullChildrenCount >= 1 &&
            isNil(X.mPartialChild1) &&
            isNil(X.mPartialChild2) ) ) {
        return false;
    }
//cerr << "templateP3[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);
    unlinkFromParent(X, fullChildren);
    unlinkFromParent(X, emptyChildren);
    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {
        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToParent(X);
    }
    else {
        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToParent(X);
    }

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {
        emptyIt = makePNode(emptyChildren);
        toNodeRef(emptyIt).linkToParent(X);

    }
    else {
        emptyIt = *(emptyChildren.begin());
        toNodeRef(emptyIt).linkToParent(X);
    }
    X.mNodeType      = JTSTreeNode::QType;
    X.mPertinentType = JTSTreeNode::Partial;
    X.setPartialInParent();
    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Partial   : 1
 *   Pertinent type : N/A (The root is Partial H.)
 *   This is a final reduction.
 */
bool JTSTree::templateP4(JTSTreeNode& X, node_list_it_t& pertinentRoot)
{
    if ( !( X.mNodeType==JTSTreeNode::PType &&
            ( (isNil(X.mPartialChild1) && !isNil(X.mPartialChild2))||
              (!isNil(X.mPartialChild1) && isNil(X.mPartialChild2))   ) ) ) {
        return false;
    }
//cerr << "templateP4[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;// Not used.
    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromParent(X, fullChildren);

    node_list_it_t partialIt;
    if (isNil(X.mPartialChild1)) {
        partialIt = X.mPartialChild2;
    }
    else {
        partialIt = X.mPartialChild1;
    }
    auto& Partial = toNodeRef(partialIt);

    node_list_it_t fullIt;

    if (fullChildren.size() > 1) {
        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToParentFullEnd(Partial);
    }
    else if (fullChildren.size() == 1) {
        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToParentFullEnd(Partial);
    }

    if (X.mChildren.size() == 1) {

        transferContentsOfOnlyChildAndRemove(X);
        pertinentRoot = X.backIt();

    }
    else { 

        pertinentRoot = partialIt;

    }
    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Partial  : 1
 *   Pertinent type : Partial H
 */
bool JTSTree::templateP5(JTSTreeNode& X)
{
    if ( !( X.mNodeType==JTSTreeNode::PType &&
            ( (isNil(X.mPartialChild1) && !isNil(X.mPartialChild2))||
              (!isNil(X.mPartialChild1) && isNil(X.mPartialChild2))   ) ) ) {
        return false;
    }
//cerr << "templateP5[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromParent(X, fullChildren);
    unlinkFromParent(X, emptyChildren);

    node_list_it_t partialIt;
    if (isNil(X.mPartialChild1)) {
        partialIt = X.mPartialChild2;
    }
    else {
        partialIt = X.mPartialChild1;
    }
    auto& Partial = toNodeRef(partialIt);
    
    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {
        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToParentFullEnd(Partial);
    }
    else if (fullChildren.size() == 1) {
        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToParentFullEnd(Partial);
    }

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {
        emptyIt = makePNode(emptyChildren);
        toNodeRef(emptyIt).linkToParentEmptyEnd(Partial);
    }
    else if (emptyChildren.size() == 1) {
        emptyIt = *(emptyChildren.begin());
        toNodeRef(emptyIt).linkToParentEmptyEnd(Partial);
    }

    // Copy the contents of the partial child to X.
    transferContentsOfOnlyChildAndRemove(X);

    X.mPertinentType = JTSTreeNode::Partial;
    X.setPartialInParent();

    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *       Empty    : 2
 *   Pertinent type : Partial A
 *   This is a final reduction.
 */
bool JTSTree::templateP6(JTSTreeNode& X, node_list_it_t& pertinentRoot)
{
    if ( !( X.mNodeType==JTSTreeNode::PType &&
            !isNil(X.mPartialChild1) &&
            !isNil(X.mPartialChild2)       ) ) {
        return false;
    }
//cerr << "templateP6[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromParent(X, fullChildren);

    auto  partialItBase     = X.mPartialChild1;
    auto& PartialBase       = toNodeRef(partialItBase);
    auto  partialItAbsorbed = X.mPartialChild2;
    auto& PartialAbsorbed   = toNodeRef(partialItAbsorbed);



    node_list_it_t fullIt;
    if (fullChildren.size() > 1) {
        fullIt = makePNode(fullChildren);
        toNodeRef(fullIt).linkToParentFullEnd(PartialBase);
    }
    else if (fullChildren.size() == 1) {
        fullIt = *(fullChildren.begin());
        toNodeRef(fullIt).linkToParentFullEnd(PartialBase);
    }

    PartialAbsorbed.unlinkFromParent();

    concatenateOnePartialToTheOtherP6(PartialBase, PartialAbsorbed);

    removeNode(PartialAbsorbed);

    if (X.mChildren.size() == 1) {

        transferContentsOfOnlyChildAndRemove(X);
        pertinentRoot = X.backIt();
    }
    else {
        pertinentRoot = partialItBase;
    }
    return true;
}


void JTSTree::concatenateOnePartialToTheOtherP6(
    JTSTreeNode& M,
    JTSTreeNode& A
) {
    if (M.isHeadChildFull()) {
        if (A.isHeadChildFull()) {
            for (auto cItIt = A.mChildren.begin();
                                cItIt != A.mChildren.end();) {
                auto& C = toNodeRef(*cItIt);
                cItIt++;
                C.unlinkFromParent();
                C.linkToParent(M, M.mChildren.begin());
            }
        }
        else {
            // Can't use rbegin (reverse iterator) as the erase() called 
            // in unlinkFromParent corrupts the integrity of reverse iterators.
            if (A.mChildren.size() > 0) {
                auto cItIt = A.mChildren.end();
                cItIt--;
                for (auto remain = A.mChildren.size(); remain > 0; remain--) {
                    auto& C = toNodeRef(*cItIt);
                    cItIt--;
                    C.unlinkFromParent();
                    C.linkToParent(M, M.mChildren.begin());
                }
            }
        }
    }
    else {
        if (A.isHeadChildFull()) {
            for (auto cItIt = A.mChildren.begin();
                                 cItIt != A.mChildren.end();) {
                auto& C = toNodeRef(*cItIt);
                cItIt++;
                C.unlinkFromParent();
                C.linkToParent(M, M.mChildren.end());
            }
        }
        else {
            // Can't use rbegin (reverse iterator) as the erase() called 
            // in unlinkFromParent corrupts the integrity of reverse iterators.
            if (A.mChildren.size() > 0) {
                auto cItIt = A.mChildren.end();
                cItIt--;
                for (auto remain = A.mChildren.size(); remain > 0; remain--) {
                    auto& C = toNodeRef(*cItIt);
                    cItIt--;
                    C.unlinkFromParent();
                    C.linkToParent(M, M.mChildren.end());
                }
            }
        }
    }
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: P
 *   Pertinent type : Partial CD
 *   This is not a final reduction.
 */
bool JTSTree::templateP7(JTSTreeNode& X)
{
    if ( !( X.mNodeType==JTSTreeNode::PType &&
            !isNil(X.mPartialChild1) &&
            !isNil(X.mPartialChild2)       ) ) {
        return false;
    }
//cerr << "templateP7[" << X.mNodeNum << "]\n";
    list<node_list_it_t> fullChildren;
    list<node_list_it_t> emptyChildren;

    X.sortFullAndEmptyChildrenOnPNode(fullChildren, emptyChildren);

    unlinkFromParent(X, emptyChildren);

    auto  partialItBase     = X.mPartialChild1;
    auto& PartialBase       = toNodeRef(partialItBase);
    auto  partialItAbsorbed = X.mPartialChild2;
    auto& PartialAbsorbed   = toNodeRef(partialItAbsorbed);

    node_list_it_t emptyIt;
    if (emptyChildren.size() > 1) {
        emptyIt = makePNode(emptyChildren);
        toNodeRef(emptyIt).linkToParentEmptyEnd(PartialBase);
    }
    else if (emptyChildren.size() == 1) {
        emptyIt = *(emptyChildren.begin());
        toNodeRef(emptyIt).linkToParentEmptyEnd(PartialBase);
    }

    PartialAbsorbed.unlinkFromParent();

    concatenateOnePartialToTheOtherP7(PartialBase, PartialAbsorbed);

    removeNode(PartialAbsorbed);

    PartialBase.mPertinentType = JTSTreeNode::CDPartial;
    PartialBase.setCDPartialInParent();

    if (X.mPartialChild1 == partialItBase) {
        X.mPartialChild1 = nil();
    }
    else {
        X.mPartialChild2 = nil();
    }

    if (X.mChildren.size() == 1) {

        transferContentsOfOnlyChildAndRemove(X);

    }

    X.mPertinentType = JTSTreeNode::CDPartial;
    X.setCDPartialInParent();

    return true;
}


void JTSTree::concatenateOnePartialToTheOtherP7(
    JTSTreeNode& M,
    JTSTreeNode& A
) {
    if (!M.isHeadChildFull()) {
        if (!A.isHeadChildFull()) {
            for (auto cItIt = A.mChildren.begin();
                                cItIt != A.mChildren.end();) {
                auto& C = toNodeRef(*cItIt);
                cItIt++;
                C.unlinkFromParent();
                C.linkToParent(M, M.mChildren.begin());
            }
        }
        else {
            // Can't use rbegin (reverse iterator) as the erase() called 
            // in unlinkFromParent corrupts the integrity of reverse iterators.
            if (A.mChildren.size() > 0) {
                auto cItIt = A.mChildren.end();
                cItIt--;
                for (auto remain = A.mChildren.size(); remain > 0; remain--) {
                    auto& C = toNodeRef(*cItIt);
                    cItIt--;
                    C.unlinkFromParent();
                    C.linkToParent(M, M.mChildren.begin());
                }
            }
        }
    }
    else {
        if (!A.isHeadChildFull()) {
            for (auto cItIt = A.mChildren.begin();
                                 cItIt != A.mChildren.end();) {
                auto& C = toNodeRef(*cItIt);
                cItIt++;
                C.unlinkFromParent();
                C.linkToParent(M, M.mChildren.end());
            }
        }
        else {
            // Can't use rbegin (reverse iterator) as the erase() called 
            // in unlinkFromParent corrupts the integrity of reverse iterators.
            if (A.mChildren.size() > 0) {
                auto cItIt = A.mChildren.end();
                cItIt--;
                for (auto remain = A.mChildren.size(); remain > 0; remain--) {
                    auto& C = toNodeRef(*cItIt);
                    cItIt--;
                    C.unlinkFromParent();
                    C.linkToParent(M, M.mChildren.end());
                }
            }
        }
    }
}


/**
 *   Condition:
 *       Type: P
 *   Pertinent type : CD
 */
bool JTSTree::templateP8(JTSTreeNode& X)
{

    list<node_list_it_t>::iterator eIt;
    if (X.mNodeType == JTSTreeNode::PType     &&
        X.areAllChildrenExceptForOneFull(eIt)    ){
        auto& C = toNodeRef(*eIt);
        if (C.isCDPartial()) {
//cerr << "templateP8[" << X.mNodeNum << "]\n";
            X.mPertinentType = JTSTreeNode::CDPartial;
            X.setCDPartialInParent();
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


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: Q
 *       Full     : ALL
 *       Partial  : 0
 *       Empty    : 0
 *   Pertinent type : Full B
 */
bool JTSTree::templateQ1(JTSTreeNode& X)
{
    if ( !(X.mNodeType == JTSTreeNode::QType &&
           X.mFullChildrenCount == X.mChildren.size() &&
           isNil(X.mPartialChild1) &&
           isNil(X.mPartialChild2)                     ) ) {
        return false;
    }
//cerr << "templateQ1[" << X.mNodeNum << "]\n";
    X.mPertinentType = JTSTreeNode::Full;
    X.setFullInParent();
    return true;
}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: Q
 *       Sequence: ^(F*P?E*)|(E*P?F*)$
 *   Pertinent type : Full H
 */
bool JTSTree::templateQ2(JTSTreeNode& X)
{
    if ( !( X.mNodeType==JTSTreeNode::QType &&
            checkSequenceQ2(X) )) {
        return false;
    }
//cerr << "templateQ2[" << X.mNodeNum << "]\n";
    node_list_it_t partialIt = nil();
    if (!isNil(X.mPartialChild1)) {

        partialIt = X.mPartialChild1;
    }
    else if (!isNil(X.mPartialChild2)) {

        partialIt = X.mPartialChild2;
    }

    if (!isNil(partialIt)) {

        flattenPartialChildToMiddleOfPartialNode(X, partialIt, true);

    }

    X.mPertinentType = JTSTreeNode::Partial;

    X.setPartialInParent();

    return true;
}


bool JTSTree::checkSequenceQ2(JTSTreeNode& X)
{
    /*
     *     Class of sequences it accepts in RE: ^(E*P?F*)|(F*P?E*)$
     *     More rigorously defined including the mininmum length contraints:
     *       ^(E+P|E+PF+|E+F+|PF+|F+P|F+PE+|F+E+|PE+)$
     *
     *    Finite-state deterministic automaton transition after merging
     *    equivalent states.
     *
     *        -----------------------
     *       | 1 | 2 | 3 | 4 | 5 | 6 |
     *    ---+---+---+---+---+---+---|
     *   | E | 2 | 2 | 6 | 6 | X | 6 |
     *   | P | 3 | 5 | X | 6 | X | X |
     *   | F | 4 | 5 | 5 | 4 | 5 | X |
     *    ---------------------------
     *
     *   Initial state: 1
     *   Acceptable final states: 5,6
     *   Forbidden state: X
     *
     */
    int state = 1;
    enum np {E,P,F} lookAhead;
    for (auto cIt : X.mChildren) {
        auto& C = toNodeRef(cIt);
        if      (C.isFull())    {lookAhead = F;}
        else if (C.isPartial()) {lookAhead = P;}
        else if (C.isEmpty())   {lookAhead = E;}
        else { return false; }

        switch (state) {

          case 1:
            if      (lookAhead==E)   { state = 2; }
            else if (lookAhead==P)   { state = 3; }
            else  /*(lookAhead==F)*/ { state = 4; }
            break;

          case 2:
            if      (lookAhead==E)   { state = 2; }
            else if (lookAhead==P)   { state = 5; }
           else  /*(lookAhead==F)*/ { state = 5; }
            break;

          case 3:
            if      (lookAhead==E)   { state = 6; }
            else if (lookAhead==P)   { return false; }
            else  /*(lookAhead==F)*/ { state = 5; }
            break;

          case 4:
            if      (lookAhead==E)   { state =  6; }
            else if (lookAhead==P)   { state =  6; }
            else  /*(lookAhead==F)*/ { state =  4; }
            break;

          case 5:
            if      (lookAhead==E)   { return false; }
            else if (lookAhead==P)   { return false; }
            else  /*(lookAhead==F)*/ { state = 5; }
            break;

          case 6:
            if      (lookAhead==E)   { state = 6; }
            else if (lookAhead==P)   { return false; }
            else  /*(lookAhead==F)*/ { return false; }
            break;
        }
    }
    if ( state ==  5 || state ==  6 || state ==  7 || state ==  8 ) {
        return true;
    }
    else {
        return false;
    }
}


void JTSTree::flattenPartialChildToMiddleOfPartialNode(
    JTSTreeNode&   P,
    node_list_it_t cIt,
    bool fullInTheMiddle
){
    auto& C = toNodeRef(cIt);

    auto mSibFrontItIt = C.mChildIt;
    auto mSibBackItIt  = C.mChildIt;

    node_list_it_t mSibFrontIt;
    node_list_it_t mSibBackIt;
    if(mSibFrontItIt==P.mChildren.begin()){
        mSibFrontIt = nil();
    }
    else {
        mSibFrontItIt--;
        mSibFrontIt = *mSibFrontItIt;
    }
    mSibBackItIt++;
    if(mSibBackItIt==P.mChildren.end()){
        mSibBackIt = nil();
    }
    else {
        mSibBackIt = *mSibBackItIt;
    }
    auto& Chead = toNodeRef(*(C.mChildren.begin()));
    auto& Ctail = toNodeRef(*(C.mChildren.rbegin()));
    bool reversed = false;
    if(!isNil(mSibFrontIt)) {
        // The partial node is not at the beginning.
        auto& mSibFront = toNodeRef(mSibFrontIt);
        if (fullInTheMiddle) {
            if ( (mSibFront.isFull()    && Ctail.isFull() ) ||
                 (mSibFront.isEmpty()   && Ctail.isEmpty()) || 
                 (mSibFront.isPartial() && Ctail.isFull())     ) {
                // Add grand children in the reversed order
                reversed = true;
            }
        }
        else {
            if ( (mSibFront.isFull()    && Ctail.isFull() ) ||
                 (mSibFront.isEmpty()   && Ctail.isEmpty()) || 
                 (mSibFront.isPartial() && Chead.isFull())     ) {
                // Add grand children in the reversed order
                reversed = true;
            }
        }
    }
    else {
        // The partial node is at the beginning.
        auto& mSibBack = toNodeRef(mSibBackIt);
        if (fullInTheMiddle) {
            if ( (mSibBack.isFull()    && Chead.isFull() ) ||
                 (mSibBack.isEmpty()   && Chead.isEmpty()) ||
                 (mSibBack.isPartial() && Chead.isFull())     ) {
                // Add grand children in the reversed order
                reversed = true;
            }
        }
        else {
            if ( (mSibBack.isFull()    && Chead.isFull() ) ||
                 (mSibBack.isEmpty()   && Chead.isEmpty()) ||
                 (mSibBack.isPartial() && Ctail.isFull())     ) {
                // Add grand children in the reversed order
                reversed = true;
            }
        }
    }
    auto insertBefore = C.mChildIt;
    insertBefore++;
    C.unlinkFromParent();
    if (reversed==false) {
        // Can't use "for (auto gcIt : C.mChildren)" syntax as gcItIt must be
        // updated in the middle of the loop.
        for (auto gcItIt = C.mChildren.begin(); gcItIt != C.mChildren.end();) {

            auto& GC = toNodeRef(*gcItIt);

            gcItIt++;

            GC.unlinkFromParent();

            GC.linkToParent(P, insertBefore);

        }

    }
    else {
        // Can't use rbegin (reverse iterator) as the erase() called 
        // in unlinkFromParent corrupts the integrity of reverse iterators.
        auto gcItIt = C.mChildren.end();
        gcItIt--;

        for (auto remain = C.mChildren.size(); remain > 0 ; remain--) {

            auto& GC = toNodeRef(*gcItIt);

            gcItIt--;

            GC.unlinkFromParent();

            GC.linkToParent(P, insertBefore);

        }
    }
    removeNode(C);

}


/**
 *   Assumption: Number of children is greater than 1
 *   Condition:
 *       Type: Q
 *       Sequence: ^E*P?F*P?E*$
 *
 *   Pertinent type : Partial A
 *   This is a final reduction.
 */
bool JTSTree::templateQ3(JTSTreeNode& X)
{
    if ( !( X.mNodeType==JTSTreeNode::QType &&
            checkSequenceQ3(X) )) {
        return false;
    }
//cerr << "templateQ3[" << X.mNodeNum << "]\n";
    if (!isNil(X.mPartialChild1)) {

        flattenPartialChildToMiddleOfPartialNode(X, X.mPartialChild1, true);

    }

    if (!isNil(X.mPartialChild2)) {

        flattenPartialChildToMiddleOfPartialNode(X, X.mPartialChild2, true);

    }
    return true;
}


bool JTSTree::checkSequenceQ3(JTSTreeNode& X)
{
    /*
     *     Class of sequences it accepts in RE: ^E*P?F*P?E*$
     *     More rigorously defined including the mininmum length contraints:
     *       ^P|E+P|PF+|E+PF+|E+F+|F+|PP|E+PP|PF+P|E+PF+P|E+F+P|F+P|
     *        PE+|E+PE+|PF+E+|E+PF+E+|E+F+E+|F+E+|
     *        PPE+|E+PPE+|PF+PE+|E+PF+PE+|E+F+PE+|F+PE+$
     *
     *    Finite-state deterministic automaton transition after
     *    merging equivalent states
     *
     *        ------------
     *       | 1 | 2 | 3 |
     *    ---+---+---+---|
     *   | E | 1 | 3 | 3 |
     *   | P | 2 | 3 | X |
     *   | F | 2 | 2 | X |
     *    ---------------
     *
     *   Initial state: 1
     *   Acceptable final states: 2,3
     *   Forbidden state: X
     *
     */

    int state = 1;
    enum np {E,P,F} lookAhead;
    for (auto cIt : X.mChildren) {
        auto& C = toNodeRef(cIt);
        if      (C.isFull())    { lookAhead = F; }
        else if (C.isPartial()) { lookAhead = P; }
        else if (C.isEmpty())   { lookAhead = E; }
        else { return false; }

        switch (state) {

          case 1:
            if      (lookAhead==E)   { state = 1; }
            else if (lookAhead==P)   { state = 2; }
            else  /*(lookAhead==F)*/ { state = 2; }
            break;

          case 2:
            if      (lookAhead==E)   { state = 3; }
            else if (lookAhead==P)   { state = 3; }
            else  /*(lookAhead==F)*/ { state = 2; }
            break;

          case 3:
          default:
            if      (lookAhead==E)   { state = 3; }
            else if (lookAhead==P)   { return false; }
            else  /*(lookAhead==F)*/ { return false; }
            break;

        }
    }

    if ( state == 2 || state == 3 ) {
        return true;
    }
    else {
        return false;
    }
}


bool JTSTree::checkSequenceQ4(JTSTreeNode& X)
{
    /*
     *     Class of sequences it accepts in RE: ^F*P?E*P?F*$
     *     More rigorously defined including the mininmum length contraints:
     *
     *    Finite-state deterministic automaton transition after
     *    merging equivalent states
     *
     *        ---------------
     *       | 1 | 2 | 3 | 4 |
     *    ---+---+---+---+---
     *   | F | 2 | 2 | 4 | 4 |
     *   | P | 3 | 3 | 4 | X |
     *   | E | X | 3 | 3 | X |
     *    -------------------
     *
     *   Initial state: 1
     *   Acceptable final states: 4
     *   Forbidden state: X
     */

    int state = 1;
    enum np {E,P,F} lookAhead;
    for (auto cIt : X.mChildren) {
        auto& C = toNodeRef(cIt);
        if      (C.isFull())    { lookAhead = F; }
        else if (C.isPartial()) { lookAhead = P; }
        else if (C.isEmpty())   { lookAhead = E; }
        else { return false; }

        switch (state) {

          case 1:
            if      (lookAhead==E)   { return false; }
            else if (lookAhead==P)   { state = 3; }
            else  /*(lookAhead==F)*/ { state = 2; }
            break;

          case 2:
            if      (lookAhead==E)   { state = 3; }
            else if (lookAhead==P)   { state = 3; }
            else  /*(lookAhead==F)*/ { state = 2; }
            break;

          case 3:
            if      (lookAhead==E)   { state = 3; }
            else if (lookAhead==P)   { state = 4; }
            else  /*(lookAhead==F)*/ { state = 4; }
            break;

          case 4:
          default:
            if      (lookAhead==E)   { return false; }
            else if (lookAhead==P)   { return false; }
            else  /*(lookAhead==F)*/ { state = 4; }
            break;


        }
    }

    if (state == 4) {
        return true;
    }
    else {
        return false;
    }
}



/**
 *   Condition:
 *       Type: Q
 *       Sequence: ^F*P?E*P?F*$
 *
 *   Pertinent type : Partial CD
 */
bool JTSTree::templateQ4(JTSTreeNode& X)
{
    if ( !( X.mNodeType==JTSTreeNode::QType &&
            checkSequenceQ4(X) )) {
        return false;
    }
//cerr << "templateQ4[" << X.mNodeNum << "]\n";

    if (!isNil(X.mPartialChild1)) {

        flattenPartialChildToMiddleOfPartialNode(X, X.mPartialChild1, false);

    }

    if (!isNil(X.mPartialChild2)) {

        flattenPartialChildToMiddleOfPartialNode(X, X.mPartialChild2, false);

    }

    X.mPertinentType = JTSTreeNode::CDPartial;
    X.setCDPartialInParent();

    return true;
}


/**
 *   Condition:
 *       Type: Q
 *   Pertinent type : CD
 */
bool JTSTree::templateQ5(JTSTreeNode& X)
{

    list<node_list_it_t>::iterator eIt;
    if (X.mNodeType==JTSTreeNode::QType       &&
        X.areAllChildrenExceptForOneFull(eIt)    ){
        auto& C = toNodeRef(*eIt);
        if (C.isCDPartial()) {
//cerr << "templateQ5[" << X.mNodeNum << "]\n";
            X.mPertinentType = JTSTreeNode::CDPartial;
            X.setCDPartialInParent();
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


void JTSTree::resetNodeToPAttachment(JTSTreeNode& X)
{
    X.mNodeType = JTSTreeNode::PType;
}


void JTSTree::removeNodeAndDescendants(JTSTreeNode& X)
{
    X.unlinkFromParent();

    // Queue all the children and remove.
    list<node_list_it_t> Q;
    Q.push_back(X.backIt());
    while (Q.size() > 0) {
        auto& C  = JTSTree::toNodeRef(*(Q.begin()));
        Q.pop_front();
        Q.insert(Q.end(),C.mChildren.begin(), C.mChildren.end());
        removeNode(C);
    }
}


node_list_it_t JTSTree::makePAttachmentBefore(
    JTSTreeNode&                   P,
    list<node_list_it_t>::iterator insertBefore
) {
    auto  np  = make_unique<JTSTreeNode>(*this);
    auto  nIt = addNode(std::move(np)).backIt();
    auto& N   = toNodeRef(nIt);
    N.linkToParent(P, insertBefore);
    N.mNodeType = JTSTreeNode::PType;
    return N.backIt();
}


void JTSTree::fanOutLeavesFromAttachment(
    JTSTreeNode&          A,
    list<edge_list_it_t>& edgeList
) {
    if (edgeList.size()==1) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*(*edgeList.begin())));

        A.mNodeType  = JTSTreeNode::LType;
        A.mGraphEdge = E.backIt();
        E.mPQLeaf    = A.backIt();
        E.mRemoved   = false;

    }
    else {
        for(auto eIt : edgeList) {

            auto& E = dynamic_cast<JTSGraphEdge&>(*(*eIt));

            auto  np  = make_unique<JTSTreeNode>(*this);
            auto  nIt = addNode(std::move(np)).backIt();
            auto& N   = toNodeRef(nIt);

            N.mNodeType  = JTSTreeNode::LType;
            N.mGraphEdge = eIt;
            E.mPQLeaf    = nIt;
            E.mRemoved   = false;

            N.linkToParent(A);

        }
    }
}


#ifdef UNIT_TESTS
void JTSTree::printTree(ostream& os) {
    cerr << "======= PQ-tree BEGIN======\n";
    for (auto nit = nodes().first; nit != nodes().second; nit++) {
        auto& N = toNodeRef(nit);
        N.printNode(*this, os);
        os << "\n";
    }
    cerr << "======= PQ-tree END  ======\n";
}
#endif


}// namespace Undirected

}// namespace Wailea

