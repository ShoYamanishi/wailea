#ifndef _WAILEA_UNDIRECTED_JTS_INLINE_HPP_
#define _WAILEA_UNDIRECTED_JTS_INLINE_HPP_

namespace Wailea {

namespace Undirected {

using namespace std;


inline JTSTreeNode::JTSTreeNode(JTSTree& tree):
    mTree(tree),
    mNodeType(TypeUnknown),
    mParent(tree.nil()),
    mPertinentChildrenCountProcessed(0),
    mGeneration(0),
    mPertinentChildrenCount(0),
    mPertinentType(PertinentUnknown),
    mW(0),
    mH(0),
    mA(0),
    mCD(0),
    mChildMax1WminusH(tree.nil()),
    mChildMax2WminusH(tree.nil()),
    mChildMaxWminusA(tree.nil()),
    mChildBoundaryForQH(tree.nil()),
    mQHFullOnListHead(false),
    mChildBoundaryOnHeadForQA(tree.nil()),
    mChildBoundaryOnTailForQA(tree.nil()),
    mTypeAwithSingleA(false),
    mWHABType(WHABUnknown),
    mPartialChild1(tree.nil()),
    mPartialChild2(tree.nil()),
    mCDPartialChild(tree.nil()),
    mFullChildrenCount(0){
#ifdef UNIT_TESTS
    mNodeNum                       = ++nodeNumMaster;
#endif
    ;}


inline JTSTreeNode::~JTSTreeNode(){;}


inline void JTSTreeNode::resetForBubbleUp() {

    mGeneration               = mTree.mGeneration;
    mPertinentType            = PertinentUnknown;
    mPertinentChildrenCount   = 0;
    mPertinentChildrenCountProcessed
                              = 0;
    mPartialChild1            = mTree.nil();
    mPartialChild2            = mTree.nil();
    mCDPartialChild           = mTree.nil();
    mFullChildrenCount        = 0;
    mW                        = 0;
    mH                        = 0;
    mA                        = 0;
    mCD                       = 0;
    mChildMax1WminusH         = mTree.nil();
    mChildMax2WminusH         = mTree.nil();
    mChildMaxWminusA          = mTree.nil();
    mChildBoundaryForQH       = mTree.nil();
    mQHFullOnListHead         = false;
    mChildBoundaryOnHeadForQA = mTree.nil();
    mChildBoundaryOnTailForQA = mTree.nil();
    mTypeAwithSingleA         = false;
    mWHABType                 = WHABUnknown;

}


inline bool JTSTreeNode::isPertinent() {

    if (mGeneration == mTree.mGeneration &&
        mPertinentType != Empty             ) {
        return true;
    }
    else {
        return false;
    }
}


inline bool JTSTreeNode::isFull() {

    if (mGeneration    == mTree.mGeneration &&
        mPertinentType == Full                ) {
        return true;
    }
    else {
        return false;
    }

}


inline bool JTSTreeNode::isPartial() {

    if (mGeneration    == mTree.mGeneration &&
        mPertinentType == Partial             ) {
        return true;
    }
    else {
        return false;
    }

}


inline bool JTSTreeNode::isCDPartial() {

    if (mGeneration    == mTree.mGeneration &&
        mPertinentType == CDPartial             ) {
        return true;
    }
    else {
        return false;
    }

}


inline bool JTSTreeNode::isEmpty() {

    if (mGeneration < mTree.mGeneration ||
        mPertinentType == Empty           ) {
        return true;
    }
    else {
        return false;
    }

}


inline void JTSTreeNode::markGraphEdgeRemoved()
{
    if (mNodeType == LType) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*(mGraphEdge)));
        E.mRemoved = true;
    }
}


inline bool JTSTreeNode::isGraphEdgeRemoved()
{
    if (mNodeType == LType) {
        auto& E = dynamic_cast<JTSGraphEdge&>(*(*(mGraphEdge)));
        return E.mRemoved;
    }
    return false;
}


inline void JTSTreeNode::incrementPertinentChildrenCountOfParent()
{
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        P.mPertinentChildrenCount++;
    }
}


inline void JTSTreeNode::decrementPertinentChildrenCountOfParent()
{
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        P.mPertinentChildrenCount--;
    }
}


inline void JTSTreeNode::setFullInParent()
{
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        P.mFullChildrenCount++;
    }
}


inline void JTSTreeNode::setPartialInParent()
{
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        if (mTree.isNil(P.mPartialChild1)) {
            P.mPartialChild1 = backIt();
        }
        else if (mTree.isNil(P.mPartialChild2)) {
            P.mPartialChild2 = backIt();
        }
    }
}


inline void JTSTreeNode::setCDPartialInParent()
{
    if (!mTree.isNil(mParent)) {
        auto& P = mTree.toNodeRef(mParent);
        if (mTree.isNil(P.mCDPartialChild)) {
            P.mCDPartialChild = backIt();
        }
    }
}


inline bool JTSTreeNode::isHeadChildFull()
{
    if (mChildren.size()==0) {
        return false;
    }
    auto &H = mTree.toNodeRef(*(mChildren.begin()));
    return H.isFull();
}


inline JTSTree::JTSTree():mGeneration(0){;}
inline JTSTree::~JTSTree(){;}


inline JTSTreeNode& JTSTree::toNodeRef(node_list_it_t nIt)
{
    return dynamic_cast<JTSTreeNode&>(*(*nIt));
}


inline bool JTSTree::isNil(node_list_it_t nIt)
{
    return nIt == nodes().second;
}


inline node_list_it_t JTSTree::nil()
{
    return nodes().second;
}


inline void JTSTree::initializeForOneIteration()
{
    mGeneration++;
}

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_JTS_INLINE_HPP_*/
