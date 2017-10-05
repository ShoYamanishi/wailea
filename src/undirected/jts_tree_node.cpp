#include "undirected/jts_planarizer.hpp"
#include "undirected/jts_tree.hpp"
#include "undirected/jts_tree_node.hpp"
#include "undirected/jts_inline.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/jts_tree_node.cpp
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

#ifdef UNIT_TESTS
int JTSTreeNode::nodeNumMaster = 0;
#endif


bool JTSTreeNode::areAllChildrenFull() {

    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (!C.isFull()) {
            return false;
        }
    }
    return true;

}


bool JTSTreeNode::areAllChildrenExceptForOneFull(
    list<node_list_it_t>::iterator &eIt
) {
    size_t fullCount = 0;
    for (auto cIt = mChildren.begin(); cIt != mChildren.end(); cIt++) {
        auto& C = mTree.toNodeRef(*cIt);
        if (!C.isFull()) {
            eIt = cIt;
        }
        else {
            fullCount++;
        }
    }
    return (fullCount + 1 == mChildren.size());
}


void JTSTreeNode::unlinkFromParent()
{
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        P.mChildren.erase(mChildIt);
        mParent = mTree.nil();
        if (isPertinent()) {
            P.mPertinentChildrenCount--;
            if (isFull() && P.mFullChildrenCount > 0) {
                P.mFullChildrenCount--;
            }
            else if (isPartial()) {
                if(P.mPartialChild1==backIt()){
                    P.mPartialChild1= mTree.nil();
                }
                if(P.mPartialChild2==backIt()){
                    P.mPartialChild2= mTree.nil();
                }
            }
        }
    }
}


void JTSTreeNode::linkToParent(JTSTreeNode& P,
                                          list<node_list_it_t>::iterator pos)
{
    mChildIt = P.mChildren.insert(pos,backIt());
    mParent  = P.backIt();
    if (isFull()) {
        P.mPertinentChildrenCount++;
        P.mFullChildrenCount++;
    }
    if (isPartial()) {
        P.mPertinentChildrenCount++;
        if (mTree.isNil(P.mPartialChild1)) {
            P.mPartialChild1 = backIt();
        }
        else if (mTree.isNil(P.mPartialChild2)) {
            P.mPartialChild2 = backIt();
        }
    }
}


void JTSTreeNode::linkToParent(JTSTreeNode& P)
{
    linkToParent(P, P.mChildren.end());
}


void JTSTreeNode::linkToParentFullEnd(JTSTreeNode& P)
{
    auto& H = mTree.toNodeRef(*(P.mChildren.begin()));
    if (H.isFull()) {
        linkToParent(P, P.mChildren.begin());
    }
    else {
        linkToParent(P, P.mChildren.end());
    }
}


void JTSTreeNode::linkToParentEmptyEnd(JTSTreeNode& P)
{
    auto& H = mTree.toNodeRef(*(P.mChildren.begin()));
    if (H.isFull()) {
        linkToParent(P, P.mChildren.end());
    }
    else {
        linkToParent(P, P.mChildren.begin());
    }
}


node_list_it_t JTSTreeNode::getOnePertinentChild()
{
    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (C.isPertinent()) {
            return cIt;
        }
    }
    return mTree.nil();
}


void JTSTreeNode::calculateWHAC()
{
//cerr << "calculateWHAC()\n";
    if (mNodeType == LType) {

        mW             = 1;
        mH             = 0;
        mA             = 0;
        mCD            = 0;
        mPertinentType = Full;

    }
    else if (areAllChildrenFull()) {
//cerr << "calculateWHAC() 1\n";
        mW             = sumWOfPertinentChildren();
        mH             = 0;
        mA             = 0;
        mCD            = 0;
        mPertinentType = Full;

    }
    else {
//cerr << "calculateWHAC() 2\n";
        if (mNodeType == PType) {
//cerr << "calculateWHAC() 3\n";
            calculateWHACforP();

        }
        else if (mNodeType == QType) {

            calculateWHACforQ();

        }
        mPertinentType = Partial;

    }
}


void JTSTreeNode::calculateWHACforP()
{
    wha_val_t sumWPertinent = sumWOfPertinentChildren();
    wha_val_t sumWPartial   = sumWOfPartialChildren();
    wha_val_t maxWminusA    = maxWminusAOfPertinentChildren();
    wha_val_t max1WminusH;
    wha_val_t max2WminusH;
    twoMaxesWminusHOfPartialChildren(max1WminusH, max2WminusH);

    mW = sumWPertinent;

    mH = sumWPartial -  max1WminusH;

    if ( mPertinentChildrenCount == 1 ||
         (sumWPertinent - maxWminusA) <=
         (sumWPartial - (max1WminusH + max2WminusH)) ) {
        mTypeAwithSingleA = true;
        mA                = sumWPertinent - maxWminusA;
    }
    else {

        mTypeAwithSingleA = false;
        mA                = sumWPartial - (max1WminusH + max2WminusH);

    }

    list<node_list_it_t>::iterator notFull;
    if (areAllChildrenExceptForOneFull(notFull)) {
        auto &C = mTree.toNodeRef(*notFull);
        mCD = C.mCD;
    }
    else {
        mCD = sumWPartial - (max1WminusH + max2WminusH);
    }

}


JTSTreeNode::wha_val_t JTSTreeNode::sumWOfPertinentChildren()
{
    wha_val_t totalW = 0;
    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (C.isPertinent()) {

            totalW += C.mW;
        }

    }
    return totalW;
}


JTSTreeNode::wha_val_t JTSTreeNode::sumWOfPartialChildren()
{
    wha_val_t totalW = 0;
    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (C.isPartial()) {

            totalW += C.mW;
        }

    }
    return totalW;
}


JTSTreeNode::wha_val_t JTSTreeNode::maxWminusAOfPertinentChildren()
{
    wha_val_t maxWminusA = 0;
    mChildMaxWminusA = mTree.nil();
    for (auto cIt : mChildren) {
        auto &C = mTree.toNodeRef(cIt);
        if (C.isPertinent()) {
            if (maxWminusA < C.mW - C.mA) {
                maxWminusA = C.mW - C.mA;
                mChildMaxWminusA = cIt;
            }
        }

    }
    return maxWminusA;
}


void JTSTreeNode::twoMaxesWminusHOfPartialChildren(
    wha_val_t& max1WminusH,
    wha_val_t& max2WminusH
) {
    max1WminusH = 0;
    max2WminusH = 0;
    mChildMax1WminusH = mTree.nil();
    mChildMax2WminusH = mTree.nil();
    for (auto cIt : mChildren) {
        auto &C = mTree.toNodeRef(cIt);
        if (C.isPartial()) {
            if (max1WminusH <= C.mW - C.mH) {
                max2WminusH       = max1WminusH;
                mChildMax2WminusH = mChildMax1WminusH;

                max1WminusH       = C.mW - C.mH;
                mChildMax1WminusH = cIt;
            }
            else if (max2WminusH == 0) {
                max2WminusH = C.mW - C.mH;
                mChildMax2WminusH = cIt;
            }
        }
    }
}


void JTSTreeNode::calculateWHACforQ()
{
    wha_val_t sumWPertinent  = sumWOfPertinentChildren();
    wha_val_t maxWminusA     = maxWminusAOfPertinentChildren();
    wha_val_t maxSumWminusHs = maxSumWminusHOfChildren();

    mW = sumWPertinent;
    mH = calculateHforQ();

    if (maxWminusA >= maxSumWminusHs) {

        mTypeAwithSingleA = true;
        mA                = sumWPertinent - maxWminusA;

    }
    else {

        mTypeAwithSingleA = false;
        mA                = sumWPertinent - maxSumWminusHs;

    }

    mCD = calculateCDforQ();

}


JTSTreeNode::wha_val_t JTSTreeNode::calculateCDforQ()
{
//cerr << "calculateCDforQ()\n";
    node_list_it_t boundarySide1 = mTree.nil();

    for (auto cItIt = mChildren.begin(); cItIt != mChildren.end(); cItIt++) {
        auto& C = mTree.toNodeRef(*cItIt);
        if (C.isFull()) {
            ;
        }
        else if (C.isPartial()) {
            boundarySide1 = *cItIt;
            break;
        }
        else {// isEmpty()
            boundarySide1 = *cItIt;
            break;
        }
    }
    auto  boundarySide2rIt = mChildren.rend();

    for (auto rItIt = mChildren.rbegin(); rItIt != mChildren.rend(); rItIt++) {
        auto& C = mTree.toNodeRef(*rItIt);
        if (C.isFull()) {
            ;
        }
        else if (C.isPartial()) {
            if (*rItIt == boundarySide1) {
                return C.mCD;
            }
            else {// isEmpty()
                boundarySide2rIt = rItIt;
                break;
            }
        }
        else {
            if (*rItIt == boundarySide1) {
                return 0;
            }
            boundarySide2rIt = rItIt;
            break;
        }
    }

    wha_val_t sumCD = 0;

    for (auto rItIt = boundarySide2rIt; rItIt != mChildren.rend(); rItIt++) {
        auto &C = mTree.toNodeRef(*rItIt);
        if (rItIt == boundarySide2rIt) {
            if (C.isPartial()) {
                sumCD += C.mH;
            }
        }
        else if (*rItIt == boundarySide1) {
            if (C.isPartial()) {
                sumCD += C.mH;
            }
            return sumCD;
        }
        else {
            if (C.isPertinent()) {
                sumCD += C.mW;
            }
        }
    }

    return sumCD;

}


JTSTreeNode::wha_val_t JTSTreeNode::calculateHforQ()
{
    wha_val_t      Hforward = 0;
    node_list_it_t boundaryForward = mTree.nil();
    bool           inFull = true;
    for (auto cIt : mChildren) {

        auto& C = mTree.toNodeRef(cIt);
        if (inFull) {
            if (C.isFull()) {
                boundaryForward = cIt;
            }
            if (C.isPartial()) {
                Hforward = C.mH;
                boundaryForward = cIt;
                inFull = false;
            }
            if (C.isEmpty()) {
                inFull = false;
            }
        }
        else {
            if (C.isPertinent()) {
                Hforward += C.mW;
            }
        }

    }

    wha_val_t      Hbackward        = 0;
    node_list_it_t boundaryBackward = mTree.nil();
    inFull = true;
    for (auto cItIt = mChildren.rbegin();
                                      cItIt != mChildren.rend() ; cItIt++) {
        auto& C = mTree.toNodeRef(*cItIt);
        if (inFull) {
            if (C.isFull()) {
                boundaryBackward = *cItIt;
            }
            if (C.isPartial()) {
                Hbackward = C.mH;
                boundaryBackward = *cItIt;
                inFull = false;
            }
            if (C.isEmpty()){
                inFull = false;
            }
        }
        else {
            if (C.isPertinent()) {
                Hbackward += C.mW;
            }
        }
    }

    if (Hforward < Hbackward) {
        mChildBoundaryForQH = boundaryForward;
        mQHFullOnListHead = true;
        return Hforward;
    }
    else {
        mChildBoundaryForQH = boundaryBackward;
        mQHFullOnListHead = false;
        return Hbackward;
    }
}


JTSTreeNode::wha_val_t JTSTreeNode::maxSumWminusHOfChildren()
{
    wha_val_t       maxSumWminusH = 0;
    wha_val_t       sumWminusH    = 0;
    bool            counting      = false;
    node_list_it_t  mayBeBoundary1;
    node_list_it_t  prevIt        = mTree.nil();

    for (auto cIt : mChildren) {

        auto& C = mTree.toNodeRef(cIt);

        if (counting) {

            if (C.isFull()) {
                // cIt is in the middle of the consecutive sequence.
                sumWminusH += C.mW;

            }
            else if (C.isPartial()) {
                // cIt is the end of the consecutive pertinent sequence.
                sumWminusH += (C.mW - C.mH);
                if (maxSumWminusH < sumWminusH) {

                    mChildBoundaryOnHeadForQA = mayBeBoundary1;
                    mChildBoundaryOnTailForQA = cIt;
                    maxSumWminusH             = sumWminusH;

                }
                mayBeBoundary1 = cIt;
                // cIt is also the beginning of a new consecutive pertinent
                // sequence.
                sumWminusH     = (C.mW - C.mH);

            }
            else if (C.isEmpty()) {
                // prevIt is the end of the consecutive pertinent sequence.
                if (maxSumWminusH < sumWminusH) {

                    mChildBoundaryOnHeadForQA = mayBeBoundary1;
                    mChildBoundaryOnTailForQA = prevIt;
                    maxSumWminusH             = sumWminusH;

                }
                counting = false;
            }

        }
        else {

            if ( C.isFull() || C.isPartial()) {
                // cIt is the beginning of a new consecutive pertinent
                // sequence.
                mayBeBoundary1 = cIt;
                sumWminusH     = (C.mW - C.mH);
                counting       = true;

            }
        }
        prevIt = cIt;
    }

    if (counting) {

        if (maxSumWminusH < sumWminusH) {

            mChildBoundaryOnHeadForQA = mayBeBoundary1;
            mChildBoundaryOnTailForQA = prevIt;
            maxSumWminusH             = sumWminusH;

        }
    }

    return maxSumWminusH;
}


void JTSTreeNode::determineRootNodeType()
{
    if(isFull()){
        mWHABType = B;
    }
    else if (mTree.isNil(mParent)) {

        if(mCD < mH && mCD < mA) {
             mWHABType = CD;
        }
        else if(mH < mA){
            mWHABType = H;
        }
        else {
            mWHABType = A;
        }
    }
    else {
        if(mH < mA){
            mWHABType = H;
        }
        else {
            mWHABType = A;
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildren()
{
    if (mNodeType == PType) {
        if (mWHABType == B) {
            setBForChildren();
        }
        else if (mWHABType == W) {
            setWForChildren();
        }
        else if (mWHABType == H) {
            determineWHABCTypeForChildrenOfPTypeH();
        }
        else if (mWHABType == A) {
            determineWHABCTypeForChildrenOfPTypeA();
        }
        else if (mWHABType == CD) {
            determineWHABCTypeForChildrenOfPTypeCD();
        }
    }
    else if (mNodeType == QType) {
        if (mWHABType == B) {
            setBForChildren();
        }
        else if (mWHABType == W) {
            setWForChildren();
        }
        else if (mWHABType == H) {
            determineWHABCTypeForChildrenOfQTypeH();
        }
        else if (mWHABType == A) {
            determineWHABCTypeForChildrenOfQTypeA();
        }
        else if (mWHABType == CD) {
            determineWHABCTypeForChildrenOfQTypeCD();
        }
    }
}


void JTSTreeNode::setWForChildren()
{
    // All the pertinent children => W 
    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (C.isPertinent()) {
            C.mWHABType = W;
        }
    }
}


void JTSTreeNode::setBForChildren()
{
    // All the children => W 
    // All the children are supposed to be Full.
    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (C.isFull()) {
            C.mWHABType = B;
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildrenOfPTypeH()
{
    // Full children          => B
    // mChildMax1WminusH      => H
    // Other partial children => W
    for (auto cIt : mChildren) {
        auto& C = mTree.toNodeRef(cIt);
        if (C.isFull()) {
            C.mWHABType = B;
        }
        if (C.isPartial()) {
            if (cIt == mChildMax1WminusH) {
                if (C.isFull()) {
                    C.mWHABType = B;
                }
                else {
                    C.mWHABType = H;
                }
            }
            else {
                C.mWHABType = W;
            }
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildrenOfPTypeA()
{
    if (mTypeAwithSingleA) {
        // mChildMaxWminusA          => A
        // Other perrtinent children => W.
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPertinent()) {
                if (cIt == mChildMaxWminusA) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = A;
                    }
                }
                else {
                    C.mWHABType = W;
                }
            }
        }
    }
    else {
        // Full children          => B
        // mChildMax1WminusH      => H
        // mChildMax2WminusH      => H
        // Other partial children => W
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPartial()) {
                if (cIt == mChildMax1WminusH) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = H;
                    }
                }
                else if (cIt == mChildMax2WminusH) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = H;
                    }
                }
                else {
                    C.mWHABType = W;
                }
            }
            else if (C.isFull()) {
                 C.mWHABType = B;
           }
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildrenOfPTypeCD()
{
    list<node_list_it_t>::iterator eIt;
    if (areAllChildrenExceptForOneFull(eIt)) {
        // full children          => B
        // the only partial child => CD
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPertinent()) {
                if (cIt == *eIt) {
                    C.mWHABType = CD;
                }
                else {
                    C.mWHABType = B;
                }
            }
        }
    }
    else {
        // Full children          => B
        // mChildMax1WminusH      => H
        // mChildMax2WminusH      => H
        // Other partial children => W
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPartial()) {
                if (cIt == mChildMax1WminusH) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = H;
                    }
                }
                else if (cIt == mChildMax2WminusH) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = H;
                    }
                }
                else {
                    C.mWHABType = W;
                }
            }
            else if (C.isFull()) {
                 C.mWHABType = B;
           }
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildrenOfQTypeH()
{
    if (mTree.isNil(mChildBoundaryForQH)) {
        // W = H.
        // All the pertinent children => W.
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPertinent()) {
                C.mWHABType = W;
            }
        }
    }
    if (mQHFullOnListHead) {
        // from list head before mChildMax1WminusH => B
        // mChildMax1WminusH                       => H
        // after mChildMaxWminusH                  => W
        bool inPertinent = true;
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (inPertinent) {
                if (cIt == mChildBoundaryForQH) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = H;
                    }
                    inPertinent = false;
                }
                else {
                    C.mWHABType = B;
                }
            }
            else {
                if (C.isPertinent()) {
                    C.mWHABType = W;
                }
            }
        }
    }
    else {
        // from list head before mChildMax1WminusH => W
        // mChildMax1WminusH                       => H
        // after mChildMaxWminusH                  => B
        bool inPertinent = false;
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (!inPertinent) {
                if (C.isPertinent()) {
                    if (cIt == mChildBoundaryForQH) {
                        if (C.isFull()) {
                            C.mWHABType = B;
                        }
                        else {
                            C.mWHABType = H;
                        }
                        inPertinent = true;
                    }
                    else {
                        C.mWHABType = W;
                    }
                }
             }
             else {
                 if (C.isPertinent()) {
                    C.mWHABType = B;
                 }
             }
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildrenOfQTypeA()
{
    if (mTypeAwithSingleA) {

        // mChildMaxWmiusA          => A
        // Other pertinent children => W
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPertinent()) {
                if (cIt == mChildMaxWminusA) {
                    if (C.isFull()) {
                        C.mWHABType = B;
                    }
                    else {
                        C.mWHABType = A;
                    }
                }
                else {
                    C.mWHABType = W;
                }
            }
        }
    }
    else {
        // from head before mChildBoundaryOnHeadForQA => W
        // mChildBoundaryOnHeadForQA                  => H
        // in between                                 => B
        // mChildBoundaryOnTailForQA                  => H
        // after mChildBoundaryOnTailForQA until tail => W
        bool inPertinent = false;
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (!inPertinent) {
                if (C.isPertinent()) {
                    if (cIt == mChildBoundaryOnHeadForQA) {
                        if (C.isFull()) {
                            C.mWHABType = B;
                        }
                        else {
                            C.mWHABType = H;
                        }
                        if (mChildBoundaryOnHeadForQA
                                              != mChildBoundaryOnTailForQA) {
                            inPertinent = true;
                        }
                    }
                    else {
                        C.mWHABType = W;
                    }
                }
            }
            else {
                if (C.isPertinent()) {
                    if (cIt == mChildBoundaryOnTailForQA) {
                        if (C.isFull()) {
                            C.mWHABType = B;
                        }
                        else {
                            C.mWHABType = H;
                        }
                        inPertinent = false;
                    }
                    else {
                        C.mWHABType = B;
                    }
                }
            }
        }
    }
}


void JTSTreeNode::determineWHABCTypeForChildrenOfQTypeCD()
{
    list<node_list_it_t>::iterator eIt;
    if (areAllChildrenExceptForOneFull(eIt)) {
        // full children          => B
        // the only partial child => CD
        for (auto cIt : mChildren) {
            auto& C = mTree.toNodeRef(cIt);
            if (C.isPertinent()) {
                if (cIt == *eIt) {
                    C.mWHABType = CD;
                }
                else {
                    C.mWHABType = B;
                }
            }
        }
    }
    else {

        node_list_it_t boundarySide1 = mTree.nil();

        for (auto cItIt=mChildren.begin(); cItIt!=mChildren.end(); cItIt++) {

            auto& C = mTree.toNodeRef(*cItIt);

            if (C.isFull()) {
                boundarySide1 = *cItIt;
                C.mWHABType = B;
            }
            else if (C.isPartial()) {
                boundarySide1 = *cItIt;
                C.mWHABType = H;
                break;
            }
            else {// isEmpty()
                break;
            }

        }

        bool  nonPertinentRegion = false;

        for (auto rItIt=mChildren.rbegin(); *rItIt!=boundarySide1; rItIt++) {

            auto& C = mTree.toNodeRef(*rItIt);
            if (nonPertinentRegion) {

                C.mWHABType = W;

            }
            else {
                if (C.isFull()) {
                    C.mWHABType = B;
                }
                else if (C.isPartial()) {

                    nonPertinentRegion = true;
                    C.mWHABType = H;                    

                }
                else {// isEmpty()

                    nonPertinentRegion = true;
                    C.mWHABType = W;

                }
            }
        }
    }
}


void JTSTreeNode::sortFullAndEmptyChildrenOnPNode(
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


#ifdef UNIT_TESTS
void JTSTreeNode::printNode(JTSTree& tree, ostream& os) {


    if (mNodeType==TypeUnknown) {
        os << "U";
    }
    if (mNodeType==PType) {
        os << "P";
    }
    if (mNodeType==QType) {
        os << "Q";
    }
    if (mNodeType==LType) {
        os << "L";
    }
    os << this->mNodeNum ;

    if (mNodeType==LType) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*(mGraphEdge)));
        auto& OE = dynamic_cast<Edge&>(E.IGBackwardLinkRef());
        auto& N1 = dynamic_cast<NumNode&>(OE.incidentNode1());
        auto& N2 = dynamic_cast<NumNode&>(OE.incidentNode2());
        os << " {" << N1.num() << "," << N2.num() << "}";
        if (E.mPQLeaf != backIt()) {
            os << "    Graph Edge's mPQLeaf does not match!\n";
        }
    }
    else {
        os << " chi:" << mChildren.size() << "[";
        for (auto nit : mChildren) {
            auto& TN = tree.toNodeRef(nit);
            os << TN.mNodeNum << ",";
        }
    }
    os << "]";
    if (tree.isNil(mParent)){
        os << "mParent(nil) ";
    }
    else {
        auto& TN = tree.toNodeRef(mParent);
        os << "mParent(" << TN.mNodeNum << ") ";
    }

    if (tree.mGeneration > mGeneration) {
        os << "non-pert ";
    }
    else if (mPertinentType == Empty) {
        os << "pert ";
        os << "Empty ";
    }
    else {
        os << "pert ";

        if (mPertinentType == Partial) {
            os << "Partial ";
        }
        else if (mPertinentType == CDPartial) {
            os << "CDPartial ";
        }
        else if (mPertinentType == Full) {
            os << "Full ";
        }
        else if (mPertinentType == PertinentUnknown) {
            os << "Unknown ";
        }

        if (tree.isNil(mPartialChild1)){
            os << "mPartialChild1(nil) ";
        }
        else {
            auto& TN = tree.toNodeRef(mPartialChild1);
            os << "mPartialChild1(" << TN.mNodeNum << ") ";
        }

        if (tree.isNil(mPartialChild2)){
            os << "mPartialChild2(nil) ";
        }
        else {
            auto& TN = tree.toNodeRef(mPartialChild2);
            os << "mPartialChild2(" << TN.mNodeNum << ") ";
        }

        if (tree.isNil(mCDPartialChild)){
            os << "mCDPartialChild(nil) ";
        }
        else {
            auto& TN = tree.toNodeRef(mCDPartialChild);
            os << "mCDPartialChild(" << TN.mNodeNum << ") ";
        }

        os << "W:" << mW << " ";
        os << "H:" << mH << " ";
        os << "A:" << mA << " ";
        os << "CD:" << mCD << " ";
        if (mWHABType == WHABUnknown) {
            os << "[Unknown] ";
        }
        else if (mWHABType == W) {
            os << "[W] ";
        }
        else if (mWHABType == H) {
            os << "[H] ";
        }
        else if (mWHABType == A) {
            os << "[A] ";
        }
        else if (mWHABType == B) {
            os << "[B] ";
        }
        os << "Pert Cnt:" <<  mPertinentChildrenCount << " ";
        os << "Full Cnt:" <<  mFullChildrenCount << " ";

    }

    // mTree


    //node_list_it_t                 mChildBoundaryForQH;
    //bool                           mQHFullOnListHead;
    //node_list_it_t                 mChildBoundaryOnHeadForQA;
    //node_list_it_t                 mChildBoundaryOnTailForQA;
    //bool                           mTypeAwithSingleA;
    //WHABType                       mWHABType;
    //node_list_it_t                 mPartialChild1;
    //node_list_it_t                 mPartialChild2;
    //size_t                         mFullChildrenCount;

}
#endif


}// namespace Undirected

}// namespace Wailea
