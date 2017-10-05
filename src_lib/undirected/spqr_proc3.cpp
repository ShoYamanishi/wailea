#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/spqr_proc3.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file undirected/spqr_proc3.cpp
 *
 * @brief 
 *     
 * @remarks
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

void SPQRStrategyProc3::prepareNodesAndEdges(Graph &g)
noexcept
{
    auto nitPairs = g.nodes();
    for (auto nit = nitPairs.first; nit != nitPairs.second; nit++) {

        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*nit));
        n.reorderIncidence(move(n.mOrderedAdjacencyType2));
        n.mOrderedAdjacencyType2.clear();

    }

    auto eitPairs = g.edges();
    for (auto eit = eitPairs.first; eit != eitPairs.second; eit++) {

        SPQR_DFS_Edge& e = dynamic_cast<SPQR_DFS_Edge&>(*(*eit));
        e.mStartPath = false;
        e.mIsVirtual = false;
    }

    mOutgoingPath = false;

    createNodeMap(g);

}


void SPQRStrategyProc3::createNodeMap(Graph &g)
{
    size_t numNodes = g.numNodes();
    mNMap.reserve(numNodes+1);
    SPQRNodeMapElem blankElem;
    for (size_t i = 0; i < numNodes + 1; i++) {
        mNMap.push_back(blankElem);
    }

    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {
        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*nit));
        mNMap[n.mNum].mIt = n.backIt();
        mNMap[n.mNum].mRemoved = false;
    }
}


void SPQRStrategyProc3::leaveNode(DNode& v)
{
    if (v.mNum == 1) {
        // Process the remaining edges in EStack.
        mSPQRTree.makeFinalTreeNodeAndComponent(mG,mEStack);
        removeSubgraph(mEStack);
    }
}

void SPQRStrategyProc3::processFrond(DNode& v,DEdge& vw)
{

    DNode& w = dynamic_cast<DNode&>(vw.adjacentNode(v));
    if (!mOutgoingPath) {

       mTStack.updateForFrond(v.mNum, w.mNum);

    }
    mOutgoingPath = false;
    mEStack.push_back(vw.backIt());
}

void SPQRStrategyProc3::beforeVisitingChild(DNode& v,DEdge& vw)
                                                          
{
    DNode& w = dynamic_cast<DNode&>(vw.adjacentNode(v));

    if (!mOutgoingPath) {

        mOutgoingPath = true;
        vw.mStartPath = true;
        mTStack.updateForTreeArc(w.mNum + w.mNd - 1, w.mLowPt1, v.mNum);

    }
    else{

        vw.mStartPath = false;

    }
}


void SPQRStrategyProc3::afterVisitingChild(DNode& v,DEdge& vw)
                                                          
{

    mEStack.push_back(vw.backIt());

    bool startPath = vw.mStartPath;

    preadvance(v,vw);

    mHasNewHighPt = false;

    DEdge& vwNew = dynamic_cast<DEdge&>(checkForType2s(v,vw));

    checkForType1(v,vwNew);

    if (startPath) {

        mTStack.popUntilAndIncludingEOS();

    }

    if (v.mHighPoints.size()>0) {

        DNode& h = dynamic_cast<DNode&>(*(*(*(v.mHighPoints.begin()))));

        mTStack.popUntilHigherH(v.mNum, h.mNum);
    }

    mOutgoingPath = false;
}


void SPQRStrategyProc3::checkForType1(DNode& v, DEdge& vw)
{
    if (v.mNum == 1) {
        return;
    }

    DNode& w           = dynamic_cast<DNode&>(vw.adjacentNode(v));
    Edge&  vParentEdge = *(*(v.mParentEdge));
    DNode& vParent     = dynamic_cast<DNode&>(vParentEdge.adjacentNode(v));

    if( !( w.mLowPt2 >= v.mNum && w.mLowPt1 < v.mNum   && 
           (vParent.mNum != 1 || v.isThereUnexplored())   ) ) {
        return ;
    }
    DNode& b = v;
    DNode& bParent = vParent;
    DNode& a = dynamic_cast<DNode&>(*(*(mNMap[w.mLowPt1].mIt)));

    // Those two become valid only if mHasNewHighPt becomes true below.
    mHighPtOf    = a.backIt();
    mNewHighPtIt = b.backIt();

    vector<edge_list_it_t> edgesToSepComponent;

    while (mEStack.size()>0) {

        DEdge& xy = dynamic_cast<DEdge&>(*(*(*(mEStack.rbegin()))));
        DNode& x  = dynamic_cast<DNode&>(xy.incidentNode1());
        DNode& y  = dynamic_cast<DNode&>(xy.incidentNode2());

        if (!( ( w.mNum <= x.mNum && x.mNum < w.mNum + w.mNd )||
               ( w.mNum <= y.mNum && y.mNum < w.mNum + w.mNd )  ) ) {

            break;
        }

        edgesToSepComponent.push_back(xy.backIt());

        mEStack.pop_back();

    }

    if (bParent.mNum == a.mNum) {

        DEdge& edgeAB = dynamic_cast<DEdge&>(vParentEdge);

        if (edgeAB.mIsVirtual) {

            auto pairEdgeABIt   = edgeAB.mVirtualPairEdge;
            auto pairTreeNodeIt = edgeAB.mVirtualPairTreeNode;

            node_list_it_t newTreeNodeIt;
            edge_list_it_t virtualEdgeInTreeNodeIt;

            mSPQRTree.makeNewNodesEdgesAndComponents(mG,
                                                 edgesToSepComponent,
                                                 a.backIt(),
                                                 b.backIt(),
                                                 false,
                                                 mG.edges().second,
                                                 true,
                                                 pairEdgeABIt,
                                                 pairTreeNodeIt,
                                                 newTreeNodeIt,
                                                 virtualEdgeInTreeNodeIt );

            edgeAB.mVirtualPairEdge     = virtualEdgeInTreeNodeIt;
            edgeAB.mVirtualPairTreeNode = newTreeNodeIt;

            removeSubgraph(edgesToSepComponent);

        }
        else {

            // Make the existing (a,b) in G virtual. Make a new real edge 
            // and pass it to the separating pair.
            edge_ptr_t newRealEdgeABPtr = make_unique<DEdge>();
            DEdge& newRealEdgeAB = dynamic_cast<DEdge&>(
                                   mG.addEdge(move(newRealEdgeABPtr), a, b));

            newRealEdgeAB.mType = SPQR_DFS_Edge::TreeArcType;
            newRealEdgeAB.mIsVirtual = false;
            newRealEdgeAB.pushIGBackwardLink(edgeAB.IGBackwardLink());

            node_list_it_t newTreeNodeIt;
            edge_list_it_t virtualEdgeInTreeNodeIt;

            mSPQRTree.makeNewNodesEdgesAndComponents(mG,
                                                 edgesToSepComponent,
                                                 a.backIt(),
                                                 b.backIt(),
                                                 true,
                                                 newRealEdgeAB.backIt(),
                                                 false,
                                                 mSPQRTree.edges().second,
                                                 mSPQRTree.nodes().second,
                                                 newTreeNodeIt,
                                                 virtualEdgeInTreeNodeIt );


            edgeAB.mIsVirtual           = true;
            edgeAB.mVirtualPairEdge     = virtualEdgeInTreeNodeIt;
            edgeAB.mVirtualPairTreeNode = newTreeNodeIt;

            edgesToSepComponent.push_back(newRealEdgeAB.backIt());

            removeSubgraph(edgesToSepComponent);

        }

    }
    else {

        edge_list_it_t edgeABIt = mG.edges().second;

        if (mEStack.size() > 0) {
            DEdge& xy =  dynamic_cast<DEdge&>(*(*(*(mEStack.rbegin()))));

            DNode& x = dynamic_cast<DNode&>(xy.incidentNode1());

            DNode& y = dynamic_cast<DNode&>(xy.incidentNode2());

            if ( (x.mNum == a.mNum && y.mNum == b.mNum) ||
                 (y.mNum == a.mNum && x.mNum == b.mNum)   ) {

                edgeABIt = xy.backIt();
                mEStack.pop_back();
            }
        }

        node_list_it_t newTreeNodeIt;
        edge_list_it_t virtualEdgeInTreeNodeIt;

        mSPQRTree.makeNewNodesEdgesAndComponents(mG,
                                                 edgesToSepComponent,
                                                 a.backIt(),
                                                 b.backIt(),
                                                 (mG.edges().second!=edgeABIt),
                                                 edgeABIt,
                                                 false,
                                                 mSPQRTree.edges().second,
                                                 mSPQRTree.nodes().second,
                                                 newTreeNodeIt,
                                                 virtualEdgeInTreeNodeIt );

        DEdge& newEdgeBA = dynamic_cast<DEdge&>(*(*createAndAddDEdgeNextTo(
                                                 mG,
                                                 b,
                                                 a,
                                                 vw,
                                                 SPQR_DFS_Edge::FrondType
                                               )));

        newEdgeBA.mIsVirtual           = true;
        newEdgeBA.mVirtualPairEdge     = virtualEdgeInTreeNodeIt;
        newEdgeBA.mVirtualPairTreeNode = newTreeNodeIt;

        mEStack.push_back(newEdgeBA.backIt());

        if (edgeABIt != mG.edges().second) {
            edgesToSepComponent.push_back(edgeABIt);
        }
        else {
            // New frond is inserted. Need to update the high pt.
            mHasNewHighPt = true;
        }
        removeSubgraph(edgesToSepComponent);

    }
}

Edge& SPQRStrategyProc3::checkForType2s(DNode& v, DEdge& vwOriginal)
{

    if (v.mNum == 1) {
        return vwOriginal;
    }

    // current edge changes over the iterations of the while loop.
    edge_list_it_t currentEdgeIt = vwOriginal.backIt();

    while ( true  ) {

        DEdge& vw          = dynamic_cast<DEdge&>(*(*currentEdgeIt));
        DNode& w           = dynamic_cast<DNode&>( vw.adjacentNode(v) );
        DEdge& wx          = dynamic_cast<DEdge&>( w.adjacentEdge(vw) );
        DNode& x           = dynamic_cast<DNode&>( wx.adjacentNode(w) );
        if(!(mTStack.a() == v.mNum||(w.degree() == 2 && w.mNum < x.mNum))) {
            break;
        }

        if (popInvalidTStackElements(v.mNum)) {
            ;// TStack updated. Winding back to the beginning of while.
        }
        else if ( w.degree() == 2 && w.mNum < x.mNum ) {
            currentEdgeIt = processUpwardTriangle(v, vw, w, wx, x);
        }
        else {// top.a == v.mNum
            currentEdgeIt = processSeparationComponentByHAB(v, vw);
        }
    }
    return dynamic_cast<DEdge&>(*(*currentEdgeIt));
}



bool SPQRStrategyProc3::popInvalidTStackElements(size_t vNum)
{
    bool isTStackUpdated = false;
    while (!mTStack.isEmpty()) {
        if (mTStack.a() == vNum) {

            if (mNMap[mTStack.b()].mRemoved) {
                mTStack.pop();
                isTStackUpdated = true;
            }
            else {
                DNode& b = dynamic_cast<DNode&>(*(*(mNMap[mTStack.b()].mIt)));
                Edge&  bParentEdge = *(*b.mParentEdge);
                DNode& bParent = dynamic_cast<DNode&>(
                                                 bParentEdge.adjacentNode(b));
                if (bParent.mNum == mTStack.a()) {
                    mTStack.pop();
                    isTStackUpdated = true;
                }
                else {
                    break;
                }
            }
        }
        else {
            break;
        }
    }
    return isTStackUpdated;
}

edge_list_it_t SPQRStrategyProc3::processUpwardTriangle(
                           DNode& v, DEdge& vw, DNode& w, DEdge& wx, DNode& x)
{

    mEStack.pop_back();// vw
    mEStack.pop_back();// wx

    vector <edge_list_it_t> edgesToSepComponent;
    edgesToSepComponent.push_back(vw.backIt());
    edgesToSepComponent.push_back(wx.backIt());

    edge_list_it_t edgeXVIt = mG.edges().second;
    if (mEStack.size()>0) {

        DEdge& e = dynamic_cast<DEdge&>(*(*(*(mEStack.rbegin()))));
        if (((&(e.incidentNode1()) == &v)&&(&(e.incidentNode2()) == &x))||
            ((&(e.incidentNode1()) == &x)&&(&(e.incidentNode2()) == &v)) ){
            edgeXVIt = e.backIt();
            mEStack.pop_back();
        }
    }

    node_list_it_t newTreeNodeIt;
    edge_list_it_t virtualEdgeInTreeNodeIt;
    mSPQRTree.makeNewNodesEdgesAndComponents(mG,
                                             edgesToSepComponent,
                                             v.backIt(),
                                             x.backIt(),
                                             (edgeXVIt!=mG.edges().second),
                                             edgeXVIt,
                                             false,
                                             mSPQRTree.edges().second,
                                             mSPQRTree.nodes().second,
                                             newTreeNodeIt,
                                             virtualEdgeInTreeNodeIt );
                                     
    DEdge& newEdgeXV = dynamic_cast<DEdge&>(*(*createAndAddDEdgeNextTo(
                                                 mG,
                                                 v,
                                                 x,
                                                 vw,
                                                 SPQR_DFS_Edge::TreeArcType
                                             )));

    newEdgeXV.mIsVirtual           = true;
    newEdgeXV.mVirtualPairEdge     = virtualEdgeInTreeNodeIt;
    newEdgeXV.mVirtualPairTreeNode = newTreeNodeIt;
    x.mParentEdge                  = newEdgeXV.backIt();
    mEStack.push_back(newEdgeXV.backIt());

    if (edgeXVIt != mG.edges().second) {
        edgesToSepComponent.push_back(edgeXVIt);
    }
    removeSubgraph(edgesToSepComponent);

    return newEdgeXV.backIt();
}


edge_list_it_t SPQRStrategyProc3::processSeparationComponentByHAB(
                                                          DNode& v, DEdge& vw)
{

    vector <edge_list_it_t> edgesToSepComponent;

    edge_list_it_t edgeABIt = mG.edges().second;

    DNode& a = v;
    DNode& b = dynamic_cast<DNode&>(*(*(mNMap[mTStack.b()].mIt)));

    while (mEStack.size()>0) {
        DEdge& e = dynamic_cast<DEdge&>(*(*(*(mEStack.rbegin()))));
        DNode& n1 = dynamic_cast<DNode&>(e.incidentNode1());
        DNode& n2 = dynamic_cast<DNode&>(e.incidentNode2());

        if (!( a.mNum <= n1.mNum && n1.mNum <= mTStack.h() &&
               a.mNum <= n2.mNum && n2.mNum <= mTStack.h()   )) {
            break;
        }

        if( (n1.mNum == a.mNum && n2.mNum == b.mNum) ||
            (n1.mNum == b.mNum && n2.mNum == a.mNum)   ) {
            edgeABIt = e.backIt();
        }
        else {
            edgesToSepComponent.push_back(e.backIt());
        }
        mEStack.pop_back();
    }

    node_list_it_t newTreeNodeIt;
    edge_list_it_t virtualEdgeInTreeNodeIt;
    mSPQRTree.makeNewNodesEdgesAndComponents(mG,
                                             edgesToSepComponent,
                                             a.backIt(),
                                             b.backIt(),
                                             (edgeABIt!=mG.edges().second),
                                             edgeABIt,
                                             false,
                                             mSPQRTree.edges().second,
                                             mSPQRTree.nodes().second,
                                             newTreeNodeIt,
                                             virtualEdgeInTreeNodeIt );
                                     
    DEdge& newEdgeAB = dynamic_cast<DEdge&>(*(*createAndAddDEdgeNextTo(
                                                 mG,
                                                 a,
                                                 b,
                                                 vw,
                                                 SPQR_DFS_Edge::TreeArcType
                                             )));

    newEdgeAB.mIsVirtual           = true;
    newEdgeAB.mVirtualPairEdge     = virtualEdgeInTreeNodeIt;
    newEdgeAB.mVirtualPairTreeNode = newTreeNodeIt;
    b.mParentEdge                  = newEdgeAB.backIt();
    mEStack.push_back(newEdgeAB.backIt());

    if (edgeABIt != mG.edges().second) {
        edgesToSepComponent.push_back(edgeABIt);
    }
    removeSubgraph(edgesToSepComponent);

    return newEdgeAB.backIt();
}


void SPQRStrategyProc3::printEStack(){
    cerr << "ESTACK[";
    for (auto eit : mEStack) {
        DEdge& e = dynamic_cast<DEdge&>(*(*(eit)));
        DNode& n1 = dynamic_cast<DNode&>(e.incidentNode1());
        DNode& n2 = dynamic_cast<DNode&>(e.incidentNode2());
        cerr << "(" << n1.mNum << "," << n2.mNum << ") ";
    }
    cerr << "]\n";
}


void SPQRStrategyProc3::removeSubgraph(vector<edge_list_it_t>& edges)
{
    for (auto eIt : edges) {    
        DEdge& e = dynamic_cast<DEdge&>(*(*eIt)); 
        DNode& n1 = dynamic_cast<DNode&>(e.incidentNode1());
        DNode& n2 = dynamic_cast<DNode&>(e.incidentNode2());
        mG.removeEdge(e);
        if (n1.degree() == 0) {
            mNMap[n1.mNum].mRemoved = true;
            removeHighPoints(n1);
            mG.removeNode(n1);
        }

        if (n2.degree() == 0) {
            mNMap[n2.mNum].mRemoved = true;
            removeHighPoints(n2);
            mG.removeNode(n2);
        }
    }
}


void SPQRStrategyProc3::removeHighPoints(DNode& n)
{
    size_t j =  n.mHighPointIterators.size();
    for (size_t i = 0; i < j ; i++) {
        if (mNMap[n.mHighPointReferences[i]].mRemoved == false) {
            DNode& v = dynamic_cast<DNode&>(
                                *(*(mNMap[n.mHighPointReferences[i]].mIt)));
            if (mHasNewHighPt && v.backIt() == mHighPtOf) {

                // Update the high points for the newly inserted frond.
                auto hit1 = n.mHighPointIterators[i];
                hit1++;
                auto hit2 = v.mHighPoints.insert(hit1, mNewHighPtIt);
                DNode& newHighPt = dynamic_cast<DNode&>(*(*mNewHighPtIt));
                newHighPt.mHighPointReferences.push_back(v.mNum);
                newHighPt.mHighPointIterators.push_back(hit2);
                mHasNewHighPt = false;
            }

            v.mHighPoints.erase(n.mHighPointIterators[i]);
        }
    }
    n.mHighPointIterators.clear();
    n.mHighPointReferences.clear();
    n.mHighPoints.clear();
}


void SPQRStrategyProc3::preadvance(DNode& v, DEdge& vw)
{
    node_incidence_it_t it;
    if (&(vw.incidentNode1()) == &v) {
        it = vw.incidentBackItNode1();
    }
    else {
        it = vw.incidentBackItNode2();
    }
    it++;
    for (; it != v.incidentEdges().second; it++) {
        DEdge& e = dynamic_cast<DEdge&>(*(*(*it)));
        if (!e.isVisited()) {
            break;
        }
    }

    v.setNextDFSEdge(it);
}


edge_list_it_t SPQRStrategyProc3::createAndAddDEdgeNextTo(
    Graph& g,
    Node&  n1,
    Node&  n2,
    Edge&  refEdge,
    SPQR_DFS_Edge::type t
){

    node_incidence_it_t n1It = n1.incidentEdges().first;
    node_incidence_it_t n2It = n2.incidentEdges().first;

    if (&(refEdge.incidentNode1()) == &n1) {
        n1It = refEdge.incidentBackItNode1();
        if (n1It != n1.incidentEdges().second) {
            n1It++;
        }
    }
    else if (&(refEdge.incidentNode2()) == &n1) {
        n1It = refEdge.incidentBackItNode2();
        if (n1It != n1.incidentEdges().second) {
            n1It++;
        }
    }

    if (&(refEdge.incidentNode1()) == &n2) {
        n2It = refEdge.incidentBackItNode1();
        if (n2It != n2.incidentEdges().second) {
            n2It++;
        }
    }
    else if (&(refEdge.incidentNode2()) == &n2) {
        n2It = refEdge.incidentBackItNode2();
        if (n2It != n2.incidentEdges().second) {
            n2It++;
        }
    }

    edge_ptr_t eNewPtr = make_unique<DEdge>();
    DEdge& eNew = dynamic_cast<DEdge&>(
                                g.addEdge(move(eNewPtr), n1, n2, n1It, n2It));
    eNew.mIsVirtual = true;
    eNew.mType    = t;
    eNew.setVisited();
    return eNew.backIt();

}

}// namespace Undirected

}// namespace Wailea

