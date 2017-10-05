#include "undirected/vis_rep_finder.hpp"
#include "undirected/st_numbering.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/vis_rep_finder.cpp
 *
 * @brief
 */

namespace Wailea {

namespace Undirected {


void VisRepFinder::find()
{
    if (mEBCTree.numNodes()==0){
        // k0 (empty graph)
        return;
    }

    auto& bcTree  = mEBCTree.bcTree();
    auto& LG      = dynamic_cast<VRRectLabelGraph&>(bcTree.original());
    if (LG.numNodes()==1 && LG.numEdges()==0) {
        // k1    
        auto& N    = dynamic_cast<VRRectLabelNode&>(*(*(LG.nodes().first)));
        N.mY       =  N.height()/2.0;
        N.mXleft   =  N.width()/2.0;
        N.mXright  =  N.width()/2.0;
        LG.mWidth  = N.width();
        LG.mHeight = N.height();

        return;
    }
    
    prepare();

    performPass1BottomUp();

    performPass2TopDown();

    findVRCoordinates();

    cleanUp();
}


/** @brief augument the nodes in EmbeddedBCTree::ExploreationNode,
 *        EmbeddedNode, EmbeddedEdge, EmbeddedFace, and DualEdge for
 *        each Block in BCTree in EmbeddedBCTree with auxiliary data
 *        structures as follows.
 *
 *        EmbeddedBCTree::ExplorationNode  => ETNodeAux
 *        EmbeddedNode                     => EGNodeAux
 *        EmbeddedEdge                     => EGEdgeAux
 *        EmbeddedFace                     => DGFaceAux
 *        DualEdge                         => DGEdgeAux
 */
void VisRepFinder::prepare()
{

    auto& bcTree     = mEBCTree.bcTree();
    auto& expTree    = mEBCTree.explorationTree();
    auto& labelGraph = dynamic_cast<VRRectLabelGraph&>(bcTree.original());

    mHGap = labelGraph.horizontalGap();
    mVGap = labelGraph.verticalGap();

    auto nPair = expTree.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& expNode     = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                                      *(*nit));
        auto& embTreeNode = dynamic_cast<EmbeddedBCTreeNode&>(
                                                  expNode.IGBackwardLinkRef());

        // Create an aux data for EmbeddedBCTree.
        embTreeNode.pushUtility(mAux.size());
        mAux.emplace_back();
        auto& aux = mAux[embTreeNode.utility()];

        auto enPair = embTreeNode.embeddedGraph().nodes();
        for (auto enit = enPair.first; enit != enPair.second; enit++) {

            // Create an aux data for EmbeddedNode.
            auto& EmbNode = dynamic_cast<EmbeddedNode&>(*(*enit));
            EmbNode.pushUtility(aux.mEGNodeAux.size());
            aux.mEGNodeAux.emplace_back();

        }

        auto eePair = embTreeNode.embeddedGraph().edges();
        for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {

            // Create an aux data for EmbeddedEdge.
            auto& embEdge = dynamic_cast<EmbeddedEdge&>(*(*eeit));
            embEdge.pushUtility(aux.mEGEdgeAux.size());
            aux.mEGEdgeAux.emplace_back();

        }

        auto dfPair = embTreeNode.dualGraph().nodes();
        for (auto dfit = dfPair.first; dfit != dfPair.second; dfit++) {

            // Create an aux data for EmbeddedFace.
            auto& face = dynamic_cast<EmbeddedFace&>(*(*dfit));
            face.pushUtility(aux.mDGFaceAux.size());
            aux.mDGFaceAux.emplace_back();

        }

        auto dePair = embTreeNode.dualGraph().edges();
        for (auto deit = dePair.first; deit != dePair.second; deit++) {

            // Create an aux data for DualEdge.
            auto& dualEdge = dynamic_cast<DualEdge&>(*(*deit));
            dualEdge.pushUtility(aux.mDGEdgeAux.size());
            aux.mDGEdgeAux.emplace_back();

        }
    }

}


void VisRepFinder::cleanUp()
{
    auto& expTree    = mEBCTree.explorationTree();

    auto nPair = expTree.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& expNode     = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                                      *(*nit));
        auto& embTreeNode = dynamic_cast<EmbeddedBCTreeNode&>(
                                                  expNode.IGBackwardLinkRef());
        embTreeNode.popUtility();

        auto enPair = embTreeNode.embeddedGraph().nodes();
        for (auto enit = enPair.first; enit != enPair.second; enit++) {
            auto& EmbNode = dynamic_cast<EmbeddedNode&>(*(*enit));
            EmbNode.popUtility();
        }

        auto eePair = embTreeNode.embeddedGraph().edges();
        for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {
            auto& embEdge = dynamic_cast<EmbeddedEdge&>(*(*eeit));
            embEdge.popUtility();
        }

        auto dfPair = embTreeNode.dualGraph().nodes();
        for (auto dfit = dfPair.first; dfit != dfPair.second; dfit++) {
            auto& face = dynamic_cast<EmbeddedFace&>(*(*dfit));
            face.popUtility();
        }

        auto dePair = embTreeNode.dualGraph().edges();
        for (auto deit = dePair.first; deit != dePair.second; deit++) {
            auto& dualEdge = dynamic_cast<DualEdge&>(*(*deit));
            dualEdge.popUtility();
        }
    }

}



/** @brief explore EmbeddedBCTree::ExplorationTree from the bottom and 
 *         generate visibility representation in the local coordinates
 *         of each Block.
 */
void VisRepFinder::performPass1BottomUp()
{

#ifdef UNIT_TESTS_XXX
    cerr << "performPass1BottomUp() BEGIN\n";
#endif

    auto& ExpTree = mEBCTree.explorationTree();

//    cerr << "ExpTree(" << ExpTree.numNodes() << "," << ExpTree.numEdges() << ")\n";

    list<node_list_it_t> Q;
    size_t               Qsize = 0;

#ifdef UNIT_TESTS_XXX
    cerr << "Initial Queue BEGIN\n";
#endif

    for (auto nit = ExpTree.nodes().first; 
                                       nit != ExpTree.nodes().second; nit++) {

        auto& expNode = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                                      *(*nit));

#ifdef UNIT_TESTS_XXX
        debugPrint(expNode, 4);
#endif

        expNode.resetNumChildrenProcessed();
        if (expNode.degreeOut()==0){
#ifdef UNIT_TESTS_XXX
        cerr << "Pushing to Queue\n";
#endif
            Q.push_back(nit);
            Qsize++;
        }

    }

#ifdef UNIT_TESTS_XXX
    cerr << "Initial Queue END: size " << Qsize << "," << Q.size() << "\n";
#endif


#ifdef UNIT_TESTS_XXX
    size_t iteration = 0;
#endif
    while (Qsize > 0) {

#ifdef UNIT_TESTS_XXX
        cerr << "Iteration: " << iteration++ << "\n";
#endif

        auto& expNode = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                          *(*(*(Q.begin()))));
        Q.pop_front();
        Qsize--;    
        
        processOneTreeNodePass1(expNode);

#ifdef UNIT_TESTS_XXX
        cerr << "    AFTER processOneTreeNodePass1()\n";
        debugPrint(expNode, 4);
#endif
        if (expNode.degreeIn()>0) {
            auto& expEdge = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(
                                     *(*(*(expNode.incidentEdgesIn().first))));
            auto& expParent = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                expEdge.adjacentNode(expNode));
            expParent.incrementNumChildrenProcessed();

            // If all the children have been processed, put the parent in Q.
            if (expParent.numChildrenProcessed()==expParent.degreeOut()) {
                Q.push_back(expParent.backIt());
                Qsize++;
            }
        }
    }

#ifdef UNIT_TESTS_XXX
    cerr << "performPass1BottomUp() END\n";
#endif

}

/** @brief assuming the embedding of the block is in the natural orientation
 *         (upward from t to s, rightward from t* to s*), find a visibility
 *         representation of the block, considering the space required for
 *         the labels and the child blocks (absorbees).
 */
void VisRepFinder::processOneTreeNodePass1(
    EmbeddedBCTree::ExplorationNode& expNode
) {

    auto& embTreeNode = dynamic_cast<EmbeddedBCTreeNode&>(
                                                  expNode.IGBackwardLinkRef());

    node_list_it_t bottomNodeIt;
    node_list_it_t rightFaceIt;
    edge_list_it_t stIt;
    edge_list_it_t stDualIt;
    
    findBottomNodeAndLeftMostFace(
                       embTreeNode, bottomNodeIt, rightFaceIt, stIt, stDualIt);

#ifdef UNIT_TESTS_XXX
    debugPrintFindBottomNodeAndLeftMostFace(
                       embTreeNode, bottomNodeIt, rightFaceIt, stIt, stDualIt, 4);
#endif

    orientEGandDG(embTreeNode, bottomNodeIt, rightFaceIt, stIt, stDualIt);

#ifdef UNIT_TESTS_XXX
    debugPrintOrientEGandDG(embTreeNode, 4);
#endif
    assignDefaultCosts(embTreeNode);

    handleNodeLabels(embTreeNode, stDualIt);

#ifdef UNIT_TESTS_XXX
    debugPrintHandleNodeLabels(embTreeNode, 4);
#endif

    handleEdgeLabels(embTreeNode);

#ifdef UNIT_TESTS_XXX
    debugPrintHandleEdgeLabels(embTreeNode, 4);
#endif

    handleChildBlocks(expNode);

#ifdef UNIT_TESTS_XXX
    debugPrintHandleChildBlocks(embTreeNode, 4);
#endif

    calculateCosts(embTreeNode, stDualIt);

#ifdef UNIT_TESTS_XXX
    debugPrintCalculateCosts(embTreeNode, 4);
#endif

    scheduleEG(embTreeNode, bottomNodeIt);

#ifdef UNIT_TESTS_XXX
    debugPrintScheduleEG(embTreeNode, 4);
#endif

    scheduleDG(embTreeNode, rightFaceIt, stDualIt);

#ifdef UNIT_TESTS_XXX
    debugPrintScheduleDG(embTreeNode, 4);
#endif

    assignCoordinates(embTreeNode, stIt, stDualIt, bottomNodeIt);

#ifdef UNIT_TESTS_XXX
    debugPrintAssignCoordinates(embTreeNode, 4);
#endif

}


/** @brief find the bottom and the other face of the outer face from
 *         the top node and the outer face.
 *
 *     top (s)    ==================
 *                /|       /|     |
 *            {s,t}|        |     |/
 *                 | right  |    ===
 *    (outer face) |  face ===    | outer face
 *                 |        |     |/
 *           (s*)--|--t*   /|    ===   s*
 *        {s*,t*}  |        .     |
 *                 |/       |/    |/
 *     bottom (t) ==================
 *
 */
void VisRepFinder::findBottomNodeAndLeftMostFace(
    EmbeddedBCTreeNode& embTreeNode,
    node_list_it_t&     bottomIt,
    node_list_it_t&     rightIt,
    edge_list_it_t&     stIt,
    edge_list_it_t&     stDualIt
){

    auto& topNode   = embTreeNode.topNode();
    auto& outerFace = embTreeNode.outerFace();

    auto iPair = topNode.incidentEdges();

    for (auto iit = iPair.first ; iit != iPair.second ; iit++) {

        auto& embEdge = dynamic_cast<EmbeddedEdge&>(*(*(*iit)));

        if (embEdge.mHalfEdge1.mDstNode==topNode.backIt()) {
            if (embEdge.mHalfEdge1.mEmbeddedFace==outerFace.backIt()) {

                bottomIt = embEdge.mHalfEdge1.mSrcNode;
                rightIt  = embEdge.mHalfEdge2.mEmbeddedFace;
                stIt     = *iit;
                stDualIt = embEdge.mDualEdge;
                return;
            }
        }
        else if (embEdge.mHalfEdge2.mDstNode==topNode.backIt()) {
            if (embEdge.mHalfEdge2.mEmbeddedFace==outerFace.backIt()) {

                bottomIt = embEdge.mHalfEdge2.mSrcNode;
                rightIt  = embEdge.mHalfEdge1.mEmbeddedFace;
                stIt     = *iit;
                stDualIt = embEdge.mDualEdge;
                return;
            }
        }
    }
}


/** @brief find the rank from bottom node (s) to the top (t) by using
 *         st-numbering and orient the edges upward.
 *         Also, orient the dual edges in the direction from t* to s*,
 *         except for {s*,t*} which is oriented s*->t*.
 */
void VisRepFinder::orientEGandDG(

    EmbeddedBCTreeNode& embTreeNode,
    node_list_it_t      bottomIt,
    node_list_it_t      rightIt,
    edge_list_it_t      stIt,
    edge_list_it_t      stDualIt

) {
    auto& aux = mAux[embTreeNode.utility()];

    // Make ST-ordering of EmbeddedNodes.
    // From T (lower) to S(higher)
    vector<node_list_it_t> tsOrdering;

    if (embTreeNode.embeddedGraph().numNodes() >= 2) {
        auto& S   = embTreeNode.topNode();
        auto& T   = dynamic_cast<EmbeddedNode&>(*(*bottomIt));

        // Generate ST-numbering
        STNumbering st;

        if (embTreeNode.embeddedGraph().numNodes()>2) {
            tsOrdering = st.getBipolarOrientation(
                                            embTreeNode.embeddedGraph(), T, S);
        }
        else {
            tsOrdering.push_back(T.backIt());
            tsOrdering.push_back(S.backIt());
        }
    }
    else if (embTreeNode.embeddedGraph().numNodes() == 1) {
        tsOrdering.push_back(embTreeNode.topNode().backIt());
    }

    long index = 0;
    for (auto nit: tsOrdering) {
        auto& embNode = dynamic_cast<EmbeddedNode&>(*(*nit));
        aux.mEGNodeAux[embNode.utility()].mSTNum = index++;
    }
    // Orient EmbeddedEdges and DualEdges based on ST-numbering.
    auto ePair = embTreeNode.embeddedGraph().edges();
    for (auto eit = ePair.first ; eit != ePair.second; eit++) {

        auto& embEdge  = dynamic_cast<EmbeddedEdge&>(*(*eit));
        auto& embNode1 = dynamic_cast<EmbeddedNode&>(embEdge.incidentNode1());
        auto& embNode2 = dynamic_cast<EmbeddedNode&>(embEdge.incidentNode2());

        if (aux.mEGNodeAux[embNode1.utility()].mSTNum >
                                  aux.mEGNodeAux[embNode2.utility()].mSTNum ) {
            // Node1 -> Node2  (Node 1 < Node 2)
            aux.mEGEdgeAux[embEdge.utility()].mOriented1to2 = true;
            aux.mEGNodeAux[embNode1.utility()].mEdgesOut.push_back(eit);
            aux.mEGNodeAux[embNode2.utility()].mEdgesIn.push_back(eit);

        }
        else {
            // Node2 -> Node1 (Node 2 < Node 1)
            aux.mEGEdgeAux[embEdge.utility()].mOriented1to2 = false;
            aux.mEGNodeAux[embNode1.utility()].mEdgesIn.push_back(eit);
            aux.mEGNodeAux[embNode2.utility()].mEdgesOut.push_back(eit);

        }

        auto& dualEdge  = dynamic_cast<DualEdge&>(*(*(embEdge.mDualEdge)));
        auto dit = dualEdge.backIt();
        if (dit==stDualIt) {
            aux.mDGEdgeAux[dualEdge.utility()].mCountMeOut = true;
        }

        auto& face1  = dynamic_cast<EmbeddedFace&>(dualEdge.incidentNode1());
        auto& face2  = dynamic_cast<EmbeddedFace&>(dualEdge.incidentNode2());
        bool oriented1to2 = aux.mEGEdgeAux[embEdge.utility()].mOriented1to2;

        //     Node 1    ===               Node 2    ===
        //       |       /|src               |       /|src
        //       |        |                  |        |
        //       |     he2|he1-> right   or  |     he2|he1-> right
        //       |        |      face        |        |      face
        //      \|/       |/                \|/       |/
        //     Node 2    ===               Node 1    ===
        node_list_it_t rightFit;
        if ( ( (oriented1to2) &&
               (embEdge.mHalfEdge1.mSrcNode == embNode1.backIt()) ) ||
             ( (!oriented1to2) &&
               (embEdge.mHalfEdge1.mSrcNode == embNode2.backIt()) )    ) {
            rightFit = embEdge.mHalfEdge1.mEmbeddedFace;
        }
        else {
            rightFit = embEdge.mHalfEdge2.mEmbeddedFace;
        }

        if (face2.backIt() == rightFit) {
            // Face1 -> Face 2
            aux.mDGEdgeAux[dualEdge.utility()].mOriented1to2 = true;
            if (dit!=stDualIt) {
                aux.mDGFaceAux[face1.utility()].mEdgesOut.push_back(dit);
                aux.mDGFaceAux[face2.utility()].mEdgesIn.push_back(dit);
            }
        }
        else {
            // Face2 -> Face 1
            aux.mDGEdgeAux[dualEdge.utility()].mOriented1to2 = false;
            if (dit!=stDualIt) {
                aux.mDGFaceAux[face1.utility()].mEdgesIn.push_back(dit);
                aux.mDGFaceAux[face2.utility()].mEdgesOut.push_back(dit);
            }
        }
    }

    // Setting the left and the right faces of each node.
    auto& topNode    = embTreeNode.topNode();
    auto& bottomNode = dynamic_cast<EmbeddedNode&>(*(*bottomIt));
    auto& outerFace  = embTreeNode.outerFace();

    aux.mEGNodeAux[topNode.utility()   ].mLeftFace  = outerFace.backIt();
    aux.mEGNodeAux[topNode.utility()   ].mRightFace = outerFace.backIt();
    aux.mEGNodeAux[bottomNode.utility()].mLeftFace  = outerFace.backIt();
    aux.mEGNodeAux[bottomNode.utility()].mRightFace = outerFace.backIt();

    auto fPair = embTreeNode.dualGraph().nodes();
    for (auto fit = fPair.first ; fit != fPair.second; fit++) {

        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        if (F.mCycleEdges.size()>1) {
            auto cit1 = F.mCycleEdges.begin();
            auto bit1 = F.mCycleHalfEdgesOn1.begin();
            auto cit2 = cit1;
            auto bit2 = bit1;
            cit2++;
            bit2++;
            for(; cit1 != F.mCycleEdges.end(); cit1++,bit1++) {
                auto& EE1 = dynamic_cast<EmbeddedEdge&>(*(*(*cit1)));
                auto& EE2 = dynamic_cast<EmbeddedEdge&>(*(*(*cit2)));
                node_list_it_t nit_prev, nit_main, nit_next;

                if (*bit1){
                    nit_prev = EE1.mHalfEdge1.mSrcNode;
                    nit_main = EE1.mHalfEdge1.mDstNode;
                }
                else {
                    nit_prev = EE1.mHalfEdge2.mSrcNode;
                    nit_main = EE1.mHalfEdge2.mDstNode;
                }
                if (*bit2){
                    nit_next = EE2.mHalfEdge1.mDstNode;
                }
                else {
                    nit_next = EE2.mHalfEdge2.mDstNode;
                }                

                auto& Nprev = dynamic_cast<EmbeddedNode&>(*(*nit_prev));
                auto& Nmain = dynamic_cast<EmbeddedNode&>(*(*nit_main));
                auto& Nnext = dynamic_cast<EmbeddedNode&>(*(*nit_next));
                auto& AuxPrev = aux.mEGNodeAux[Nprev.utility()];
                auto& AuxMain = aux.mEGNodeAux[Nmain.utility()];
                auto& AuxNext = aux.mEGNodeAux[Nnext.utility()];
                if (AuxPrev.mSTNum > AuxMain.mSTNum &&
                                     AuxMain.mSTNum > AuxNext.mSTNum    ) {
                    AuxMain.mRightFace = fit;
                }
                else if (AuxPrev.mSTNum < AuxMain.mSTNum &&
                                     AuxMain.mSTNum < AuxNext.mSTNum    ) {
                    AuxMain.mLeftFace = fit;
                }

                cit2++;
                bit2++;
                if (cit2 == F.mCycleEdges.end()) {
                    cit2 = F.mCycleEdges.begin();
                    bit2 = F.mCycleHalfEdgesOn1.begin();
                }
            }
        }        
    }
}


void VisRepFinder::assignDefaultCosts(EmbeddedBCTreeNode& embTreeNode)
{
    auto& aux   = mAux[embTreeNode.utility()];
    aux.reset();

    auto nPair = embTreeNode.embeddedGraph().nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& embNode  = dynamic_cast<EmbeddedNode&>(*(*nit));
        auto& Naux     = aux.mEGNodeAux[embNode.utility()];
        Naux.reset();
    }

    auto eePair = embTreeNode.embeddedGraph().edges();
    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {
        auto& embEdge = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& EEaux   = aux.mEGEdgeAux[embEdge.utility()];
        EEaux.reset();
    }

    auto fPair = embTreeNode.dualGraph().nodes();
    for (auto fit = fPair.first; fit != fPair.second; fit++) {
        auto& face = dynamic_cast<EmbeddedFace&>(*(*fit));
        auto& Faux = aux.mDGFaceAux[face.utility()];
        Faux.reset();

    }

    auto dePair = embTreeNode.dualGraph().edges();
    for (auto deit = dePair.first; deit != dePair.second; deit++) {
        auto& dualEdge = dynamic_cast<DualEdge&>(*(*deit));
        auto& DEaux    = aux.mDGEdgeAux[dualEdge.utility()];
        DEaux.reset();
    }
}


/** @brief dispatcher function based on the unification type.
 */
void VisRepFinder::handleChildBlocks(EmbeddedBCTree::ExplorationNode& expNode)
{
    for (auto& sortedUG : expNode.sortedUnificationGroups()) {

        switch (findAbsorbingType(sortedUG.absorber())) {

          case INNER_FACE_TOP_CV:
            pass1ChildBlocksInnerFaceTopCV(sortedUG);
            break;
          case INNER_FACE_BOTTOM_CV:
            pass1ChildBlocksInnerFaceBottomCV(sortedUG);
            break;
          case INNER_FACE_LEFT_CV:
            pass1ChildBlocksInnerFaceLeftCV(sortedUG);
            break;
          case INNER_FACE_RIGHT_CV:
            pass1ChildBlocksInnerFaceRightCV(sortedUG);
            break;
          case OUTER_FACE_TOP_CV:
            pass1ChildBlocksOuterFaceTopCV(sortedUG);
            break;
          case OUTER_FACE_BOTTOM_CV:
            pass1ChildBlocksOuterFaceBottomCV(sortedUG);
            break;
          case OUTER_FACE_LEFT_CV:
            pass1ChildBlocksOuterFaceLeftCV(sortedUG);
            break;
          case OUTER_FACE_RIGHT_CV:
            pass1ChildBlocksOuterFaceRightCV(sortedUG);
            break;
          case K2_TOP_CV:
//cerr << "K2_TOP_CV\n";
            pass1ChildBlocksK2TopCV(sortedUG);
            break;
          case K2_BOTTOM_CV:
//cerr << "K2_BOTTOM_CV\n";
          default:
            pass1ChildBlocksK2BottomCV(sortedUG);
            break;
        }
    }
}

/** 
 */
void VisRepFinder::pass1ChildBlocksInnerFaceTopCV(
SortedUnificationGroup& UG
) {
    // CW-CCW classification is according to the block, not to the UG.
    // CW is to the left and CCW is to the right of absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());

    coord_t sumWidths, maxHeight;
    findSumWidthsAndMaxHeight(UG, 0, UG.absorbeesSize(), sumWidths, maxHeight);

    aux_eeCW.mChildrenHeightUpperRight = maxHeight - aux_cv.mLabelHeight;
    aux_deCW.mChildrenWidthUpperRight  = sumWidths/2.0;
    aux_eeCCW.mChildrenHeightUpperLeft = maxHeight - aux_cv.mLabelHeight;
    aux_deCCW.mChildrenWidthUpperLeft  = sumWidths/2.0;

}


void VisRepFinder::pass1ChildBlocksInnerFaceBottomCV(
    SortedUnificationGroup& UG
) {
    // CW-CCW classification is according to the block, not to the UG.
    // CW is to the left and CCW is to the right of absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());

    coord_t sumWidths, maxHeight;
    findSumWidthsAndMaxHeight(UG, 0, UG.absorbeesSize(), sumWidths, maxHeight);

    aux_eeCW.mChildrenHeightLowerLeft   = maxHeight - aux_cv.mLabelHeight;
    aux_deCW.mChildrenWidthLowerLeft    = sumWidths/2.0;
    aux_eeCCW.mChildrenHeightLowerRight = maxHeight - aux_cv.mLabelHeight;
    aux_deCCW.mChildrenWidthLowerRight  = sumWidths/2.0;
}


void VisRepFinder::pass1ChildBlocksInnerFaceLeftCV(
    SortedUnificationGroup& UG
) {

    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());

    UG.mIndexSecondStart = findBestSplitPointInChildren(UG);

    coord_t sumWidthsEarly, maxHeightEarly;
    findSumWidthsAndMaxHeight(
                 UG, 0, UG.mIndexSecondStart, sumWidthsEarly, maxHeightEarly);

    coord_t sumWidthsLate, maxHeightLate;
    findSumWidthsAndMaxHeight( UG, UG.mIndexSecondStart, UG.absorbeesSize(),
                                                sumWidthsLate, maxHeightLate);

    if (UG.absorber().roleOfECWReversed() ) {
        aux_eeCW.mChildrenHeightUpperRight =
              (maxHeightEarly==0.0)?0.0:(maxHeightEarly - aux_cv.mLabelHeight);

        aux_deCW.mChildrenWidthUpperRight = sumWidthsEarly;

        aux_eeCCW.mChildrenHeightLowerRight = 
                (maxHeightLate==0.0)?0.0:(maxHeightLate - aux_cv.mLabelHeight);

        aux_deCCW.mChildrenWidthLowerRight = sumWidthsLate;
    }
    else {
        aux_eeCW.mChildrenHeightUpperRight =
                (maxHeightLate==0.0)?0.0:(maxHeightLate - aux_cv.mLabelHeight);

        aux_deCW.mChildrenWidthUpperRight = sumWidthsLate;

        aux_eeCCW.mChildrenHeightLowerRight =
              (maxHeightEarly==0.0)?0.0:(maxHeightEarly - aux_cv.mLabelHeight);
                                            
        aux_deCCW.mChildrenWidthLowerRight = sumWidthsEarly;
    }
}


void VisRepFinder::pass1ChildBlocksInnerFaceRightCV(
    SortedUnificationGroup& UG
) {
    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());

    UG.mIndexSecondStart = findBestSplitPointInChildren(UG);

    coord_t sumWidthsEarly, maxHeightEarly;
    findSumWidthsAndMaxHeight(
                  UG, 0, UG.mIndexSecondStart, sumWidthsEarly, maxHeightEarly);

    coord_t sumWidthsLate, maxHeightLate;
    findSumWidthsAndMaxHeight(UG, UG.mIndexSecondStart, UG.absorbeesSize(),
                                                 sumWidthsLate, maxHeightLate);

    if ( UG.absorber().roleOfECWReversed()) {
        aux_eeCW.mChildrenHeightLowerLeft  = maxHeightEarly - 
                                              aux_cv.mLabelHeight;

        aux_deCW.mChildrenWidthLowerLeft   = sumWidthsEarly + mHGap/2.0;

        aux_eeCCW.mChildrenHeightUpperLeft = maxHeightLate - 
                                              aux_cv.mLabelHeight;

        aux_deCCW.mChildrenWidthUpperLeft  = sumWidthsLate + mHGap/2.0;
    }
    else {
        aux_eeCW.mChildrenHeightLowerLeft  = maxHeightLate -
                                              aux_cv.mLabelHeight;

        aux_deCW.mChildrenWidthLowerLeft   = sumWidthsLate + mHGap/2.0;

        aux_eeCCW.mChildrenHeightUpperLeft = maxHeightEarly -
                                              aux_cv.mLabelHeight;

        aux_deCCW.mChildrenWidthUpperLeft  = sumWidthsEarly + mHGap/2.0;
    }
}


void VisRepFinder::pass1ChildBlocksOuterFaceTopCV(SortedUnificationGroup& UG) {

    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& embTN  = UG.absorber().treeNode();
    auto& aux    = mAux[embTN.utility()];

    coord_t sumWidths, maxHeight;
    findSumWidthsAndMaxHeight(UG, 0, UG.absorbeesSize(), sumWidths, maxHeight);

    aux.mExtraHeightRight = maxHeight;
    aux.mExtraWidthRight  = sumWidths;
}


void VisRepFinder::pass1ChildBlocksOuterFaceBottomCV(
    SortedUnificationGroup& UG
) {

    auto& embTN  = UG.absorber().treeNode();
    auto& aux    = mAux[embTN.utility()];

    coord_t sumWidths, maxHeight;
    findSumWidthsAndMaxHeight(UG, 0, UG.absorbeesSize(), sumWidths, maxHeight);

    aux.mExtraHeightBottom = maxHeight;
    aux.mExtraWidthBottom  = sumWidths;
}


void VisRepFinder::pass1ChildBlocksOuterFaceLeftCV(SortedUnificationGroup& UG)
{
    pass1ChildBlocksInnerFaceLeftCV(UG);
}


void VisRepFinder::pass1ChildBlocksOuterFaceRightCV(SortedUnificationGroup& UG)
{
    // Must not be called.
    ;
}


void VisRepFinder::pass1ChildBlocksK2TopCV(SortedUnificationGroup& UG) {
    pass1ChildBlocksOuterFaceTopCV(UG);
}


void VisRepFinder::pass1ChildBlocksK2BottomCV(SortedUnificationGroup& UG) {
    pass1ChildBlocksOuterFaceBottomCV(UG);
}


EmbeddedEdge& VisRepFinder::blockEdgeCW(UnificationFace& UF) {

    node_list_it_t fIt  = UF.faceInDGIt();
    node_list_it_t cvIt = UF.cutVertexInEGIt();
    EmbeddedEdge&  eCW  = UF.edgeCWInEG();

    if ( ( (eCW.mHalfEdge1.mEmbeddedFace == fIt ) &&
           (eCW.mHalfEdge1.mSrcNode      == cvIt)    ) ||
         ( (eCW.mHalfEdge2.mEmbeddedFace == fIt ) &&
           (eCW.mHalfEdge2.mSrcNode      == cvIt)    )    ) {
        return eCW;
    }
    else {
        return UF.edgeCCWInEG();
    }
}


EmbeddedEdge& VisRepFinder::blockEdgeCCW(UnificationFace& UF) {

    node_list_it_t fIt  = UF.faceInDGIt();
    node_list_it_t cvIt = UF.cutVertexInEGIt();
    EmbeddedEdge&  eCW  = UF.edgeCWInEG();

    if ( ( (eCW.mHalfEdge1.mEmbeddedFace == fIt ) &&
           (eCW.mHalfEdge1.mSrcNode      == cvIt)    ) ||
         ( (eCW.mHalfEdge2.mEmbeddedFace == fIt ) &&
           (eCW.mHalfEdge2.mSrcNode      == cvIt)    )    ) {
        return UF.edgeCCWInEG();
    }
    else {
        return eCW;
    }
}


enum VisRepFinder::absorbingType VisRepFinder::findAbsorbingType(
    UnificationFace& UF
) {

    auto& edgeCCW = blockEdgeCCW(UF);
    auto& edgeCW  = blockEdgeCW (UF);

    auto& NodeCV  = UF.cutVertexInEG();
    auto& NodeCCW = dynamic_cast<EmbeddedNode&>(edgeCCW.adjacentNode(NodeCV));
    auto& NodeCW  = dynamic_cast<EmbeddedNode&>(edgeCW.adjacentNode(NodeCV));

    auto& embTN   = UF.treeNode();
    auto& aux     = mAux[embTN.utility()];
    auto  rankCV  = aux.mEGNodeAux[NodeCV.utility()].mSTNum;
    auto  rankCCW = aux.mEGNodeAux[NodeCCW.utility()].mSTNum;
    auto  rankCW  = aux.mEGNodeAux[NodeCW.utility()].mSTNum;

    if (embTN.embeddedGraph().numNodes()==2) {
        if (&embTN.topNode()== &UF.cutVertexInEG()) {
            return K2_TOP_CV;
        }
        else {
            return K2_BOTTOM_CV;
        }
    }
    else if (&UF.faceInDG() == &embTN.outerFace()) {
        if (rankCV > rankCCW && rankCV > rankCW) {
            return OUTER_FACE_TOP_CV;
        }
        else if  (rankCW < rankCV  && rankCV < rankCCW) {
            return OUTER_FACE_LEFT_CV;
        }
        else if  (rankCCW < rankCV  && rankCV < rankCW) {
            return OUTER_FACE_RIGHT_CV;
        }
        else {
            return OUTER_FACE_BOTTOM_CV;
        }
    }
    else {
        if (rankCV > rankCCW && rankCV > rankCW) {
            return INNER_FACE_TOP_CV;
        }
        else if  (rankCW < rankCV  && rankCV < rankCCW) {
            return INNER_FACE_LEFT_CV;
        }
        else if  (rankCCW < rankCV  && rankCV < rankCW) {
            return INNER_FACE_RIGHT_CV;
        }
        else {
            return INNER_FACE_BOTTOM_CV;
        }
    }
}


EGNodeAux& VisRepFinder::findAuxsCV(UnificationFace& UF)
{
    auto& embTN = UF.treeNode();
    auto& aux   = mAux[embTN.utility()];
    auto& cv    = UF.cutVertexInEG();
    return aux.mEGNodeAux[cv.utility()];
}


EGEdgeAux& VisRepFinder::findAuxsEECW(UnificationFace& UF)
{
    auto& embTN = UF.treeNode();
    auto& aux   = mAux[embTN.utility()];
    auto& eeCW  = blockEdgeCW(UF);
    return  aux.mEGEdgeAux[eeCW.utility()];
}


EGEdgeAux& VisRepFinder::findAuxsEECCW(UnificationFace& UF)
{
    auto& embTN = UF.treeNode();
    auto& aux   = mAux[embTN.utility()];
    auto& eeCCW = blockEdgeCCW(UF);
    return  aux.mEGEdgeAux[eeCCW.utility()];
}


DGEdgeAux& VisRepFinder::findAuxsDECW(UnificationFace& UF)
{
    auto& embTN = UF.treeNode();
    auto& aux   = mAux[embTN.utility()];
    auto& eeCW  = blockEdgeCW(UF);
    auto& deCW  = dynamic_cast<DualEdge&>(*(*(eeCW.mDualEdge)));
    return aux.mDGEdgeAux[deCW.utility()];
}


DGEdgeAux& VisRepFinder::findAuxsDECCW(UnificationFace& UF)
{
    auto& embTN = UF.treeNode();
    auto& aux   = mAux[embTN.utility()];
    auto& eeCCW = blockEdgeCCW(UF);
    auto& deCCW = dynamic_cast<DualEdge&>(*(*(eeCCW.mDualEdge)));
    return aux.mDGEdgeAux[deCCW.utility()];
}


DGFaceAux& VisRepFinder::findAuxsFace(UnificationFace& UF)
{
    auto& embTN = UF.treeNode();
    auto& aux   = mAux[embTN.utility()];
    auto& face  = UF.faceInDG();
    return aux.mDGFaceAux[face.utility()];
}


void VisRepFinder::findSumWidthsAndMaxHeight(
    SortedUnificationGroup& UG,
    long                    startIndex,
    long                    endIndex,
    coord_t&                sumWidths,
    coord_t&                maxHeight
) {

    sumWidths  = 0.0;
    maxHeight  = 0.0;

    bool first = true;
    for (size_t i = startIndex; i < endIndex; i++) {

        auto& UF    = UG.absorbeeAt(i);
        auto& embTN = UF.treeNode();
        auto& aux   = mAux[embTN.utility()];

        if (first)
            first = false;
        else
            sumWidths += mHGap;

        sumWidths += aux.mWidth;
        maxHeight = std::max(maxHeight, aux.mHeight);

    }
}


/** @brief returns the best splitting point in the start index of the
 *         second row.
 */
size_t VisRepFinder::findBestSplitPointInChildren(SortedUnificationGroup& UG)
{
//cerr << "findBestSplitPointInChildren() BEGIN\n";
    if (UG.absorbeesSize()==1) {
        return 1;
    }
    coord_t minCost  = 0.0;
    size_t  minIndex = 0;
    auto&   embTN    = UG.absorber().treeNode();
    auto&   aux      = mAux[embTN.utility()];
    auto&   aux_cv   = aux.mEGNodeAux[UG.absorber().cutVertexInEG().utility()];

    for (size_t i = 0; i <= UG.absorbeesSize(); i++){

        coord_t sumWidth1, maxHeight1;
        findSumWidthsAndMaxHeight(UG, 0, i, sumWidth1, maxHeight1);

        coord_t sumWidth2, maxHeight2;
        findSumWidthsAndMaxHeight(
                             UG, i, UG.absorbeesSize(), sumWidth2, maxHeight2);

        coord_t width  = std::max( sumWidth1, sumWidth2 );
        coord_t height = maxHeight1 + maxHeight2 - aux_cv.mLabelHeight;

        coord_t aspectRatio = (width > height)?width/height:height/width;
        coord_t cost        = kSplitPointAlpha * aspectRatio * aspectRatio +
                              (1.0 - kSplitPointAlpha) * width * height;
        //cerr << "cost: " << i << "," << cost << "\n";
        if (i==0) {
            minCost = cost;
            minIndex = i;
        }
        else {
            if (minCost > cost) {
                minCost = cost;
                minIndex = i;
            }
        }

    }
//cerr << "findBestSplitPointInChildren() END: " << minIndex << "\n";
    return minIndex;
}


void VisRepFinder::handleNodeLabels(
    EmbeddedBCTreeNode& embTreeNode,
    edge_list_it_t      stDualIt
) {
    auto& aux = mAux[embTreeNode.utility()];

    auto enPair = embTreeNode.embeddedGraph().nodes();
    for (auto enit = enPair.first; enit != enPair.second; enit++) {

        auto& embNode   = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& blockNode = dynamic_cast<BlockNode&>(
                                                  embNode.IGBackwardLinkRef());
        auto& labelNode = dynamic_cast<VRRectLabelNode&>(
                                                blockNode.IGBackwardLinkRef());
        auto& embNodeAux= aux.mEGNodeAux[embNode.utility()];

        embNodeAux.mLabelHeight = labelNode.height();

        // NOTE we can't count on the ordering in mEdgesIn and Out
        // as these are not aligned with the planar embedding.

        vector<edge_list_it_t> upperEdges;
        vector<edge_list_it_t> lowerEdges;
        findUpperAndLowerEdges(embNode, aux, upperEdges, lowerEdges);
        coord_t leftGap, rightGap;

        findLeftAndRightGaps(
            leftGap,
            rightGap,
            upperEdges,
            lowerEdges,
            embTreeNode.outerFace().backIt(),
            stDualIt
        );

        if (upperEdges.size() > 0) {

            coord_t upperWidth = (labelNode.width()+leftGap+rightGap)/
                                                            upperEdges.size();
            for (auto eit : upperEdges) {
                auto& ee = dynamic_cast<EmbeddedEdge&>(*(*eit));
                auto& de = dynamic_cast<DualEdge&>(*(*(ee.mDualEdge)));
                auto& aux_de = aux.mDGEdgeAux[de.utility()];
                aux_de.mWidthLabelShareLower = upperWidth;
            }
        }

        if (lowerEdges.size() > 0) {

            coord_t lowerWidth = (labelNode.width()+leftGap+rightGap)/
                                                            lowerEdges.size();
            for (auto eit : lowerEdges) {
                auto& ee = dynamic_cast<EmbeddedEdge&>(*(*eit));
                auto& de = dynamic_cast<DualEdge&>(*(*(ee.mDualEdge)));
                auto& aux_de = aux.mDGEdgeAux[de.utility()];
                aux_de.mWidthLabelShareUpper = lowerWidth;
            }
        }
    }
}


void VisRepFinder::findLeftAndRightGaps(
    coord_t&                leftGap,
    coord_t&                rightGap,
    vector<edge_list_it_t>& upperEdges,
    vector<edge_list_it_t>& lowerEdges,
    node_list_it_t          outerFaceIt,
    edge_list_it_t          stDualIt
) {

    bool facingRightBoundary = false;
    bool facingLeftBoundary  = false;

    for (auto eit: upperEdges) {
        auto& EE = dynamic_cast<EmbeddedEdge&>(*(*eit));
        auto& DE = dynamic_cast<DualEdge&>(*(*(EE.mDualEdge)));
        if ( (DE.incidentNode1().backIt()==outerFaceIt)||
             (DE.incidentNode2().backIt()==outerFaceIt)   ) {
            facingRightBoundary = true;
        }
        if (EE.mDualEdge == stDualIt) {
            facingLeftBoundary = true;
        }
    }

    for (auto eit: lowerEdges) {
        auto& EE = dynamic_cast<EmbeddedEdge&>(*(*eit));
        auto& DE = dynamic_cast<DualEdge&>(*(*(EE.mDualEdge)));
        if ( (DE.incidentNode1().backIt()==outerFaceIt)||
             (DE.incidentNode2().backIt()==outerFaceIt)   ) {
            facingRightBoundary = true;
        }
        if (EE.mDualEdge == stDualIt) {
            facingLeftBoundary = true;
        }
    }

    if (facingRightBoundary) {
        rightGap = 0.0;
    }
    else {
        rightGap = mHGap/2.0;
    }

    if (facingLeftBoundary) {
        leftGap = 0.0;
    }
    else {
        leftGap = mHGap/2.0;
    }

}


void VisRepFinder::findUpperAndLowerEdges(
    EmbeddedNode&           embNode,
    ETNodeAux&              aux,
    vector<edge_list_it_t>& upperEdges,
    vector<edge_list_it_t>& lowerEdges
) {

    auto embNodeAux    = aux.mEGNodeAux[embNode.utility()];

    auto iPair = embNode.incidentEdges();
    for (auto iit = iPair.first; iit != iPair.second; iit++) {
        auto& embEdge = dynamic_cast<EmbeddedEdge&>(*(*(*iit)));
        auto& adjNode = embEdge.adjacentNode(embNode);
        auto adjNodeAux = aux.mEGNodeAux[adjNode.utility()];
        if (embNodeAux.mSTNum < adjNodeAux.mSTNum) {
            upperEdges.push_back(*iit);
        }
        else {
            lowerEdges.push_back(*iit);
        }
    }
}


void VisRepFinder::handleEdgeLabels(EmbeddedBCTreeNode& embTN)
{
    auto& aux  = mAux[embTN.utility()];

    auto eePair = embTN.embeddedGraph().edges();

    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {

        auto& embEdge   = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& blockEdge = dynamic_cast<BlockEdge&>(
                                                  embEdge.IGBackwardLinkRef());
        auto& labelEdge = dynamic_cast<VRRectLabelEdge&>(
                                                blockEdge.IGBackwardLinkRef());
        auto& dualEdge  = dynamic_cast<DualEdge&>(*(*(embEdge.mDualEdge)));

        auto& aux_ee    = aux.mEGEdgeAux[embEdge.utility()];
        auto& aux_de    = aux.mDGEdgeAux[dualEdge.utility()];

        auto& N1        = dynamic_cast<EmbeddedNode&>(embEdge.incidentNode1());
        auto& N2        = dynamic_cast<EmbeddedNode&>(embEdge.incidentNode2());
        auto& aux_n1    = aux.mEGNodeAux[N1.utility()];
        auto& aux_n2    = aux.mEGNodeAux[N2.utility()];

        bool cwIsLeft = aux_n1.mSTNum  >  aux_n2.mSTNum;

        assignEdgeWidths(labelEdge, cwIsLeft, aux_de);

        aux_ee.mLabelHeightUpper  = cwIsLeft?labelEdge.heightNode1():
                                             labelEdge.heightNode2();
        aux_ee.mLabelHeightMiddle = labelEdge.heightMiddle();
        aux_ee.mLabelHeightLower  = cwIsLeft?labelEdge.heightNode2():
                                             labelEdge.heightNode1();

        aux_ee.mLabelHeight       = getEdgeLabelHeight(
                                        aux_ee.mLabelHeightUpper,
                                        aux_ee.mLabelHeightLower,
                                        aux_ee.mLabelHeightMiddle 
                                    );
    }
}


void VisRepFinder::assignEdgeWidths(
    VRRectLabelEdge& labelEdge,    
    bool             cwIsLeft,
    DGEdgeAux&       aux_de                        
) {
    assignEdgeWidth(
                cwIsLeft?labelEdge.typeNode1():
                         labelEdge.typeNode2(),
                cwIsLeft?labelEdge.widthNode1():
                         labelEdge.widthNode2(),
                cwIsLeft,
                aux_de.mLabelWidthUpperLeft,
                aux_de.mLabelWidthUpperRight  );

    assignEdgeWidth(
                labelEdge.typeMiddle(), 
                labelEdge.widthMiddle(),
                cwIsLeft,
                aux_de.mLabelWidthMiddleLeft,
                aux_de.mLabelWidthMiddleRight  );

    assignEdgeWidth(
                cwIsLeft?labelEdge.typeNode2():
                         labelEdge.typeNode1(),
                cwIsLeft?labelEdge.widthNode2():
                         labelEdge.widthNode1(),
                cwIsLeft,
                aux_de.mLabelWidthLowerLeft,
                aux_de.mLabelWidthLowerRight  );


    aux_de.mLabelWidthRight = max(
                                    aux_de.mLabelWidthUpperRight,
                                    aux_de.mLabelWidthMiddleRight,
                                    aux_de.mLabelWidthLowerRight
                              );

    aux_de.mLabelWidthLeft  = max( 
                                    aux_de.mLabelWidthUpperLeft,
                                    aux_de.mLabelWidthMiddleLeft,
                                    aux_de.mLabelWidthLowerLeft
                              );
//cerr << "\n\n";
//cerr << "labelEdge.widthMiddle(): " << labelEdge.widthMiddle() << "\n";
//cerr << "aux_de.mLabelWidthUpperLeft: " << aux_de.mLabelWidthUpperLeft << "\n";
//cerr << "aux_de.mLabelWidthMiddleLeft: " << aux_de.mLabelWidthMiddleLeft << "\n";
//cerr << "aux_de.mLabelWidthLowerLeft: " << aux_de.mLabelWidthLowerLeft << "\n";
//cerr << "aux_de.mLabelWidthLeft: " << aux_de.mLabelWidthLeft << "\n";
}


void VisRepFinder::assignEdgeWidth(
    VRRectLabelEdge::posType type,
    coord_t                  width,
    bool                     cwIsLeft,
    coord_t&                 widthLeft,
    coord_t&                 widthRight
) {
    switch (type) {

      case VRRectLabelEdge::POS_CENTER:

        if (cwIsLeft){
            widthLeft  = width/2.0;
            widthRight = width/2.0;
        }
        else {
            widthLeft  = width/2.0;
            widthRight = width/2.0;
        }
        break;

      case VRRectLabelEdge::POS_COUNTER_CLOCKWISE:
        if (cwIsLeft){
            widthLeft  = 0.0;
            widthRight = width;
        }
        else {
            widthLeft  = width;
            widthRight = 0.0;
        }
        break;

      case VRRectLabelEdge::POS_CLOCKWISE:
      default:
        if (cwIsLeft){
            widthLeft  = width;
            widthRight = 0.0;
        }
        else {
            widthLeft  = 0.0;
            widthRight = width;
        }
        break;

    }
}


coord_t VisRepFinder::getEdgeLabelHeight(
                                  coord_t upper, coord_t middle, coord_t lower)
{
    if ( upper > 0.0 ) {
        if ( middle > 0.0 ) {
            if ( lower > 0.0 ) {
                return upper + mVGap + middle + mVGap + lower;
            }
            else {
                return upper + mVGap + middle + mVGap;
            }
        }
        else {
            if ( lower > 0.0 ) {
                return upper + mVGap + lower;
            }
            else {
                return upper + mVGap;
            }
        }
    }
    else {
        if ( middle > 0.0 ) {
            if ( lower > 0.0 ) {
                return mVGap + middle + mVGap + lower;
            }
            else {
                return mVGap + middle + mVGap;
            }
        }
        else {
            if ( lower > 0.0 ) {
                return mVGap + lower;
            }
            else {
                return mVGap;
            }
        }
    }
}


coord_t VisRepFinder::max(coord_t v1, coord_t v2, coord_t v3)
{
    return std::max(std::max(v1, v2), v3);
}


void VisRepFinder::calculateCosts(
    EmbeddedBCTreeNode& embTreeNode,
    edge_list_it_t      stDualIt
) {
    auto& aux   = mAux[embTreeNode.utility()];
    auto  outerFaceIt = embTreeNode.outerFace().backIt();

    auto enPair = embTreeNode.embeddedGraph().nodes();
    for (auto enit = enPair.first; enit != enPair.second; enit++) {

        auto& en     = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& aux_en = aux.mEGNodeAux[en.utility()];

        // The cost is the node label height.
        aux_en.mCost = aux_en.mLabelHeight;
    }

    auto eePair = embTreeNode.embeddedGraph().edges();
    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {

        auto& ee     = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& aux_ee = aux.mEGEdgeAux[ee.utility()];

        aux_ee.mCost = max(
                            aux_ee.mLabelHeight,

                            aux_ee.mChildrenHeightUpperRight + mVGap + 
                                aux_ee.mChildrenHeightLowerRight,
                              
                            aux_ee.mChildrenHeightUpperLeft + mVGap +
                                aux_ee.mChildrenHeightLowerLeft
                          );
    }

    auto dePair = embTreeNode.dualGraph().edges();
    for (auto deit = dePair.first; deit != dePair.second; deit++) {

        auto& de     = dynamic_cast<DualEdge&>(*(*deit));
        auto& aux_de = aux.mDGEdgeAux[de.utility()];
        
        auto childrenWidthLeft  = std::max(aux_de.mChildrenWidthUpperLeft,
                                           aux_de.mChildrenWidthLowerLeft);
        auto childrenWidthRight = std::max(aux_de.mChildrenWidthUpperRight,
                                          aux_de.mChildrenWidthLowerRight);
 
        coord_t leftMarginGap  = (deit == stDualIt)?0.0:mHGap/2.0;
        coord_t rightMarginGap = 
        (
          ( aux_de.mOriented1to2 && de.incidentNode2().backIt()==outerFaceIt)||
          (!aux_de.mOriented1to2 && de.incidentNode1().backIt()==outerFaceIt)
        )?
        0.0:mHGap/2.0;
//cerr << "childrenWidthLeft: " << childrenWidthLeft << "\n";
//cerr << "mLabelWidthLeft: " << aux_de.mLabelWidthLeft << "\n";
        if (childrenWidthLeft > 0.0) {
            aux_de.mLeftInset = aux_de.mLabelWidthLeft + mHGap +
                                childrenWidthLeft + leftMarginGap;
//            cerr << "mLeftInset: " << aux_de.mLeftInset << "\n";
        }
        else {
            aux_de.mLeftInset = aux_de.mLabelWidthLeft + leftMarginGap;
        }

        if (childrenWidthRight > 0.0) {
            aux_de.mRightInset = aux_de.mLabelWidthRight + mHGap +
                                childrenWidthRight + rightMarginGap;
        }
        else {
            aux_de.mRightInset = aux_de.mLabelWidthRight + rightMarginGap;
        }

        aux_de.mCost = max(
                           aux_de.mLeftInset + aux_de.mRightInset,
                           aux_de.mWidthLabelShareUpper,
                           aux_de.mWidthLabelShareLower
                       );
    }
}


void VisRepFinder::scheduleEG(
    EmbeddedBCTreeNode& embTreeNode, 
    node_list_it_t      bottomIt
) {
    auto& aux   = mAux[embTreeNode.utility()];

    auto nPair = embTreeNode.embeddedGraph().nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& embNode    = dynamic_cast<EmbeddedNode&>(*(*nit));
        auto& embNodeAux = aux.mEGNodeAux[embNode.utility()];
        embNodeAux.mEarlyStart = 0;
        embNodeAux.mNumOutgoingProcessed = 0;

    }

    list<node_list_it_t> Q;
    size_t Qsize  = 0;

    Q.push_back(bottomIt);
    Qsize++;

    while (Qsize >0) {
        auto& embNode = dynamic_cast<EmbeddedNode&>(*(*(*(Q.begin()))));
        auto& embNodeAux = aux.mEGNodeAux[embNode.utility()];

        Q.pop_front();
        Qsize--;

        for (auto eit : embNodeAux.mEdgesIn) {
            auto& embEdge    = dynamic_cast<EmbeddedEdge&>(*(*eit));
            auto& embEdgeAux = aux.mEGEdgeAux[embEdge.utility()];

            auto& adjNode    = dynamic_cast<EmbeddedNode&>(
                                                embEdge.adjacentNode(embNode));
            auto& adjNodeAux = aux.mEGNodeAux[adjNode.utility()];

            auto adjNodeStart = embNodeAux.mEarlyStart +
                                         (embNodeAux.mCost + embEdgeAux.mCost);

            if ( adjNodeStart > adjNodeAux.mEarlyStart) {
                adjNodeAux.mEarlyStart = adjNodeStart;
            }

            adjNodeAux.mNumOutgoingProcessed++;

            if (adjNodeAux.mNumOutgoingProcessed ==
                                                adjNodeAux.mEdgesOut.size()) {
                Q.push_back(adjNode.backIt());
                Qsize++;
            }
        }
    }
}


void VisRepFinder::scheduleDG(
    EmbeddedBCTreeNode& embTreeNode,
    node_list_it_t      rightIt,
    edge_list_it_t      stDualIt
) {

    auto& aux   = mAux[embTreeNode.utility()];

    auto fPair = embTreeNode.dualGraph().nodes();
    for (auto fit = fPair.first; fit != fPair.second; fit++) {

        auto& f     = dynamic_cast<EmbeddedFace&>(*(*fit));
        auto& aux_f = aux.mDGFaceAux[f.utility()];

        if (fit == rightIt) {
            auto& st     = dynamic_cast<DualEdge&>(*(*stDualIt));
            auto& aux_st = aux.mDGEdgeAux[st.utility()];
            aux_f.mEarlyStart = aux_st.mCost;
        }
        else {
            aux_f.mEarlyStart = 0;
        }
        aux_f.mNumIncomingProcessed = 0;

    }

    list<node_list_it_t> Q;
    size_t Qsize = 0;

    Q.push_back(rightIt);
    Qsize++;

    while (Qsize >0) {

        auto& f     = dynamic_cast<EmbeddedFace&>(*(*(*(Q.begin()))));
        auto& aux_f = aux.mDGFaceAux[f.utility()];
        Q.pop_front();
        Qsize--;

        for (auto eit : aux_f.mEdgesOut) {

            auto& de     = dynamic_cast<DualEdge&>(*(*eit));
            auto& aux_de = aux.mDGEdgeAux[de.utility()];
            if (!aux_de.mCountMeOut) {

                auto& adjF        = dynamic_cast<EmbeddedFace&>(
                                                           de.adjacentNode(f));

                auto& aux_adjF    = aux.mDGFaceAux[adjF.utility()];

                auto adjFaceStart = 
                                aux_f.mEarlyStart + aux_f.mCost + aux_de.mCost;

                if ( adjFaceStart > aux_adjF.mEarlyStart) {
                    aux_adjF.mEarlyStart = adjFaceStart;
                }

                aux_adjF.mNumIncomingProcessed++;
                if (aux_adjF.mNumIncomingProcessed==aux_adjF.mEdgesIn.size()) {
                                                  
                    Q.push_back(adjF.backIt());
                    Qsize++;

                }
            }
        }
    }
}


void VisRepFinder::assignCoordinates(
    EmbeddedBCTreeNode& embTreeNode,
    edge_list_it_t      stIt,
    edge_list_it_t      stDualIt,
    node_list_it_t      bottomIt
) {
    auto& aux = mAux[embTreeNode.utility()];

    auto nPair = embTreeNode.embeddedGraph().nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& en     = dynamic_cast<EmbeddedNode&>(*(*nit));
        auto& aux_en = aux.mEGNodeAux[en.utility()];
        aux_en.mY    = aux_en.mEarlyStart;

    }

    auto fPair = embTreeNode.dualGraph().nodes();
    for (auto fit = fPair.first; fit != fPair.second; fit++) {

        auto& f     = dynamic_cast<EmbeddedFace&>(*(*fit));
        auto& aux_f = aux.mDGFaceAux[f.utility()];
        aux_f.mX    = aux_f.mEarlyStart;

    }

    auto eePair = embTreeNode.embeddedGraph().edges();
    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {

        auto& ee     = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& aux_ee = aux.mEGEdgeAux[ee.utility()];
        auto& de     = dynamic_cast<DualEdge&>(*(*(ee.mDualEdge)));
        auto& aux_de = aux.mDGEdgeAux[de.utility()];
        auto& en1    = dynamic_cast<EmbeddedNode&>(ee.incidentNode1());
        auto& en2    = dynamic_cast<EmbeddedNode&>(ee.incidentNode2());
                                                      
        node_list_it_t fLIt;
        node_list_it_t fRIt;

        if( ( (aux_ee.mOriented1to2) &&
              (ee.mHalfEdge1.mSrcNode == en1.backIt()) ) ||
            ( (!aux_ee.mOriented1to2) &&
              (ee.mHalfEdge1.mSrcNode == en2.backIt()) )   ) {

            fRIt = ee.mHalfEdge1.mEmbeddedFace;
            fLIt = ee.mHalfEdge2.mEmbeddedFace;
        }
        else {
            fRIt = ee.mHalfEdge2.mEmbeddedFace;
            fLIt = ee.mHalfEdge1.mEmbeddedFace;
        }

        auto& fR     = dynamic_cast<EmbeddedFace&>(*(*fRIt));
        auto& aux_fR = aux.mDGFaceAux[fR.utility()];
        auto& fL     = dynamic_cast<EmbeddedFace&>(*(*fLIt));
        auto& aux_fL = aux.mDGFaceAux[fL.utility()];

        auto Xfr = aux_fR.mEarlyStart - aux_de.mRightInset;
        
        auto Xfl = ((eeit==stIt)?0.0:aux_fL.mEarlyStart) +
                                            aux_fL.mCost + aux_de.mLeftInset;

        aux_ee.mX = (Xfr + Xfl) / 2.0;

    }

    // Calculate the height of the area for this block, and normalize the
    // coordinates.
    auto& oF     = embTreeNode.outerFace();
    auto& aux_oF = aux.mDGFaceAux[oF.utility()];
    auto& tN     = embTreeNode.topNode();
    auto& aux_tN = aux.mEGNodeAux[tN.utility()];
    auto& bN     = dynamic_cast<EmbeddedNode&>(*(*bottomIt));
    auto& aux_bN = aux.mEGNodeAux[bN.utility()];

    aux.mBlockWidth  = aux_oF.mEarlyStart + aux_oF.mCost;
    aux.mBlockHeight = aux_tN.mY + aux_tN.mCost;

    normalizeBlockCoordinates(embTreeNode, aux_bN.mLabelHeight);

}


void VisRepFinder::normalizeBlockCoordinates(
    EmbeddedBCTreeNode& embTreeNode,
    coord_t             bottomLabelHeight
) {
//cerr << "normalizeBlockCoordinates() BEGIN\n";
    auto& aux = mAux[embTreeNode.utility()];

    if (aux.mExtraWidthRight   > 0.0 &&
        aux.mExtraHeightRight  > 0.0    ) {

        if (aux.mExtraWidthBottom  > 0.0 &&
            aux.mExtraHeightBottom > 0.0   ) {
//cerr << "normalizeBlockCoordinates() 1\n";
            normalizeBlockCoordinatesBothExtras(
                embTreeNode,
                bottomLabelHeight
            );

        }
        else {
//cerr << "normalizeBlockCoordinates() 2\n";
            normalizeBlockCoordinatesExtraRight(
                embTreeNode
            );

        }

    }
    else {

        if (aux.mExtraWidthBottom  > 0.0 &&
            aux.mExtraHeightBottom > 0.0   ) {
//cerr << "normalizeBlockCoordinates() 3\n";
            normalizeBlockCoordinatesExtraBottom(
                embTreeNode,
                bottomLabelHeight
            );
        }
        else {
//cerr << "normalizeBlockCoordinates() 4\n";
            normalizeBlockCoordinatesNoExtra(
                embTreeNode
            );

        }

    }

    auto nPair = embTreeNode.embeddedGraph().nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& embNode    = dynamic_cast<EmbeddedNode&>(*(*nit));
        auto& embNodeAux = aux.mEGNodeAux[embNode.utility()];
        embNodeAux.mY += (aux.mYOffsetBlock);

    }

    auto eePair = embTreeNode.embeddedGraph().edges();
    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {

        auto& embEdge    = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& embEdgeAux = aux.mEGEdgeAux[embEdge.utility()];
        embEdgeAux.mX += (aux.mXOffsetBlock);

    }

    auto fPair = embTreeNode.dualGraph().nodes();
    for (auto fit = fPair.first; fit != fPair.second; fit++) {

        auto& f     = dynamic_cast<EmbeddedFace&>(*(*fit));
        auto& aux_f = aux.mDGFaceAux[f.utility()];
        aux_f.mX    += (aux.mXOffsetBlock);

    }
//cerr << "normalizeBlockCoordinates() END\n";
}


void VisRepFinder::normalizeBlockCoordinatesNoExtra(
    EmbeddedBCTreeNode& embTreeNode
) {

    auto& aux = mAux[embTreeNode.utility()];

    aux.mXOffsetBlock  = 0.0;
    aux.mXOffsetBottom = 0.0;
    aux.mXOffsetRight  = 0.0;
    aux.mWidth         = aux.mBlockWidth;

    aux.mYOffsetBlock  = 0.0;
    aux.mYOffsetRight  = 0.0;
    aux.mYOffsetBottom = 0.0;
    aux.mHeight        = aux.mBlockHeight;

}


void VisRepFinder::normalizeBlockCoordinatesExtraRight(
    EmbeddedBCTreeNode& embTreeNode
) {
//cerr << "normalizeBlockCoordinatesExtraRight() BEGIN\n";
    auto& aux = mAux[embTreeNode.utility()];

    aux.mXOffsetBlock  = 0.0;
    aux.mXOffsetBottom = 0.0;
    aux.mXOffsetRight  = aux.mBlockWidth + mHGap;
    aux.mWidth         = aux.mXOffsetRight + aux.mExtraWidthRight;

    coord_t totalHeight = std::max(aux.mBlockHeight, aux.mExtraHeightRight);

    aux.mYOffsetBlock  = totalHeight - aux.mBlockHeight;
    aux.mYOffsetRight  = totalHeight - aux.mExtraHeightRight;
    aux.mYOffsetBottom = 0.0;
    aux.mHeight        = totalHeight;

//cerr << "Xoffset: " << Xoffset << "\n";
//cerr << "aux.mXOffsetBottom: " << aux.mXOffsetBottom << "\n";
//cerr << "aux.mXOffsetRight: " << aux.mXOffsetRight << "\n";
//cerr << "aux.mWidth: " << aux.mWidth << "\n";

//cerr << "Yoffset: " << Yoffset << "\n";
//cerr << "aux.mYOffsetBottom: " << aux.mYOffsetBottom << "\n";
//cerr << "aux.mYOffsetRight: " << aux.mYOffsetRight << "\n";
//cerr << "aux.mHeight: " << aux.mHeight << "\n";

//cerr << "normalizeBlockCoordinatesExtraRight() END\n";
}


void VisRepFinder::normalizeBlockCoordinatesExtraBottom(
    EmbeddedBCTreeNode& embTreeNode,
    coord_t             bottomLabelHeight
) {
//cerr << "normalizeBlockCoordinatesExtraBottom() BEGIN\n";

    auto& aux = mAux[embTreeNode.utility()];

    coord_t totalWidth = std::max(aux.mBlockWidth, aux.mExtraWidthBottom);

    aux.mXOffsetBlock  = (totalWidth - aux.mBlockWidth)/2.0;
    aux.mXOffsetBottom = (totalWidth - aux.mExtraWidthBottom)/2.0;
    aux.mXOffsetRight  = 0.0;
    aux.mWidth         = totalWidth;

    aux.mYOffsetBlock  = aux.mExtraHeightBottom - bottomLabelHeight;
    aux.mYOffsetRight  = 0.0;
    aux.mYOffsetBottom = 0.0;
    aux.mHeight        = aux.mYOffsetBlock + aux.mBlockHeight;


//cerr << "Xoffset: " << Xoffset << "\n";
//cerr << "aux.mXOffsetBottom: " << aux.mXOffsetBottom << "\n";
//cerr << "aux.mXOffsetRight: " << aux.mXOffsetRight << "\n";
//cerr << "aux.mWidth: " << aux.mWidth << "\n";

//cerr << "Yoffset: " << Yoffset << "\n";
//cerr << "aux.mYOffsetBottom: " << aux.mYOffsetBottom << "\n";
//cerr << "aux.mYOffsetRight: " << aux.mYOffsetRight << "\n";
//cerr << "aux.mHeight: " << aux.mHeight << "\n";

//cerr << "normalizeBlockCoordinatesExtraBottom() END\n";

}


void VisRepFinder::normalizeBlockCoordinatesBothExtras(
    EmbeddedBCTreeNode& embTreeNode,
    coord_t             bottomLabelHeight
) {

    auto& aux = mAux[embTreeNode.utility()];

    // Stack the block pane on top of the bottom pane.
    coord_t totalWidthLeft = std::max(aux.mBlockWidth, aux.mExtraWidthBottom);
    aux.mXOffsetBlock      = (totalWidthLeft - aux.mBlockWidth)/2.0;
    aux.mXOffsetBottom     = (totalWidthLeft - aux.mExtraWidthBottom)/2.0;

    // Append the right pane to the right.

    // Check if the right pane fits immediately to the right of the block pane.
    if ( (aux.mBlockHeight - bottomLabelHeight - mVGap) > 
                                                       aux.mExtraHeightRight) {
        // Enough height to the left of the right pane.
        aux.mXOffsetRight = aux.mXOffsetBlock + aux.mBlockWidth + mHGap;
    }
    else {

        aux.mXOffsetRight = totalWidthLeft + mHGap;
    }

    aux.mWidth = std::max( aux.mXOffsetRight  + aux.mExtraWidthRight,
                           aux.mXOffsetBottom + aux.mExtraWidthBottom );


    aux.mHeight = std::max(
        aux.mExtraHeightBottom - bottomLabelHeight + aux.mBlockHeight,
        aux.mExtraHeightRight
    );                           

    aux.mYOffsetBlock  = aux.mHeight - aux.mBlockHeight;
    aux.mYOffsetBottom = aux.mYOffsetBlock + aux.mExtraHeightBottom - bottomLabelHeight;
    aux.mYOffsetRight  = aux.mHeight - aux.mExtraHeightRight;
    
}

            
void VisRepFinder::performPass2TopDown()
{
//    cerr << "\n\nperformPass2TopDown() BEGIN\n";
    auto& expTree = mEBCTree.explorationTree();
    auto& expRoot   = expTree.root();
    auto& embRoot   = dynamic_cast<EmbeddedBCTreeNode&>(
                                                  expRoot.IGBackwardLinkRef());
    auto& aux       = mAux[embRoot.utility()];

    aux.mAnchorX1            = 0.0;
    aux.mAnchorY1            = 0.0;
    aux.mAnchorX2            = aux.mWidth;
    aux.mAnchorY2            = aux.mHeight;
    aux.mFlippedVertically   = false;
    aux.mFlippedHorizontally = false;

    list<node_list_it_t> Q;
    size_t               Qsize = 0;


    Q.push_back(expRoot.backIt());
    Qsize++;

    while (Qsize > 0) {

        auto& expNode = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                          *(*(*(Q.begin()))));
        Q.pop_front();
        Qsize--;

        processOneTreeNodePass2(expNode);
#ifdef UNIT_TESTS
//       cerr << "AFTER processOneTreeNodePass2(expNode);\n";
       if (mDebugPrint) {
           cerr << "CHILD VR BEGIN\n";
           debugPrint(expNode, 4);
           cerr << "CHILD VR END\n";
       }
#endif
        auto ePair = expNode.incidentEdgesOut();
        for (auto eit = ePair.first; eit != ePair.second; eit++) {
            auto& expEdge = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(
                                                                   *(*(*eit)));
            auto& adjNode = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                expEdge.adjacentNode(expNode));
            Q.push_back(adjNode.backIt());
            Qsize++;
        }
    }
//    cerr << "performPass2TopDown() END\n";

}


void VisRepFinder::processOneTreeNodePass2(
    EmbeddedBCTree::ExplorationNode& expNode
) {
//    cerr << "processOneTreeNodePass2() BEGIN\n";

    translate(expNode);

#ifdef UNIT_TESTS_XXX
    debugPrintTranslate(
            dynamic_cast<EmbeddedBCTreeNode&>(expNode.IGBackwardLinkRef()), 4);
#endif

    // Set AnchorX, AnchorY, mRorate180, and mFlipped of children
    // assuming the absorber's embedding is in the normal orientation.
    for (auto& sortedUG : expNode.sortedUnificationGroups()) {

        switch (findAbsorbingType(sortedUG.absorber())) {

          case INNER_FACE_TOP_CV:
            pass2ChildBlocksInnerFaceTopCV(sortedUG);
            break;

          case INNER_FACE_BOTTOM_CV:
            pass2ChildBlocksInnerFaceBottomCV(sortedUG);
            break;

          case INNER_FACE_LEFT_CV:
            pass2ChildBlocksInnerFaceLeftCV(sortedUG);
            break;

          case INNER_FACE_RIGHT_CV:
            pass2ChildBlocksInnerFaceRightCV(sortedUG);
            break;

          case OUTER_FACE_TOP_CV:
            pass2ChildBlocksOuterFaceTopCV(sortedUG);
            break;

          case OUTER_FACE_BOTTOM_CV:
            pass2ChildBlocksOuterFaceBottomCV(sortedUG);
            break;

          case OUTER_FACE_LEFT_CV:
            pass2ChildBlocksOuterFaceLeftCV(sortedUG);
            break;

          case OUTER_FACE_RIGHT_CV:
            pass2ChildBlocksOuterFaceRightCV(sortedUG);
            break;

          case K2_TOP_CV:
            pass2ChildBlocksK2TopCV(sortedUG);
            break;

          case K2_BOTTOM_CV:
          default:
//cerr << "PASS2: K2_BOTTOM_CV\n";
            pass2ChildBlocksK2BottomCV(sortedUG);
            break;

        }
    }


    correctOrientations(expNode);

//    cerr << "processOneTreeNodePass2() END\n";
}


void VisRepFinder::pass2ChildBlocksInnerFaceTopCV(SortedUnificationGroup& UG)
{
//cerr << "pass2ChildBlocksInnerFaceTopCV()\n";
    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());
    auto XL  = aux_eeCW.mX  + aux_deCW.mLabelWidthRight;
    auto XR  = aux_eeCCW.mX - aux_deCCW.mLabelWidthLeft;
    auto cvY = aux_cv.mY    + aux_cv.mLabelHeight;

    assignXYToAbsorbees(
        UG,
        0,
        UG.absorbeesSize(),
        UG.absorber().roleOfECWReversed()?
            VisRepFinder::LtoR:
            VisRepFinder::RtoL,
        VisRepFinder::TOP,
        VisRepFinder::FullGapLeftFullGapRight,
        XL,
        XR,
        cvY
    );

}


void VisRepFinder::pass2ChildBlocksInnerFaceBottomCV(
    SortedUnificationGroup& UG
) {
//cerr << "pass2ChildBlocksInnerFaceBottomCV() BEGIN\n";
    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());

    auto XL  = aux_eeCCW.mX + aux_deCCW.mLabelWidthRight;
    auto XR  = aux_eeCW.mX  - aux_deCW.mLabelWidthLeft;
    auto cvY = aux_cv.mY;

    assignXYToAbsorbees(
        UG,
        0,
        UG.absorbeesSize(),
        UG.absorber().roleOfECWReversed()?
            VisRepFinder::RtoL:
            VisRepFinder::LtoR,
        VisRepFinder::BOTTOM,
        VisRepFinder::FullGapLeftFullGapRight,
        XL,
        XR,
        cvY
    );
//cerr << "pass2ChildBlocksInnerFaceBottomCV() END\n";
}


void VisRepFinder::pass2ChildBlocksInnerFaceLeftCV(SortedUnificationGroup& UG)
{
//cerr << "pass2ChildBlocksInnerFaceLeftCV()\n";
    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());
    auto& aux_face  = findAuxsFace(UG.absorber());

    if (UG.absorber().roleOfECWReversed()) {

        auto XL1  = aux_eeCW.mX + aux_deCW.mLabelWidthRight;
        auto XR1  = aux_face.mX;
        auto cvY1 = aux_cv.mY + aux_cv.mLabelHeight;

        assignXYToAbsorbees(
            UG,
            0,
            UG.mIndexSecondStart,
            VisRepFinder::LtoR,
            VisRepFinder::TOP,
            VisRepFinder::FullGapLeftHalfGapRight,
            XL1,
            XR1,
            cvY1
        );

        auto XL2  = aux_eeCCW.mX + aux_deCCW.mLabelWidthRight;
        auto XR2  = aux_face.mX;
        auto cvY2 = aux_cv.mY;

        assignXYToAbsorbees(
            UG,
            UG.mIndexSecondStart,
            UG.absorbeesSize(),
            VisRepFinder::RtoL,
            VisRepFinder::BOTTOM,
            VisRepFinder::FullGapLeftHalfGapRight,
            XL2,
            XR2,
            cvY2
        );
    }
    else {

        auto XL1  = aux_eeCCW.mX + aux_deCCW.mLabelWidthRight;
        auto XR1  = aux_face.mX;
        auto cvY1 = aux_cv.mY;

        assignXYToAbsorbees(
            UG,
            0,
            UG.mIndexSecondStart,
            VisRepFinder::LtoR,
            VisRepFinder::BOTTOM,
            VisRepFinder::FullGapLeftHalfGapRight,
            XL1,
            XR1,
            cvY1
        );

        auto XL2  = aux_eeCW.mX + aux_deCW.mLabelWidthRight;
        auto XR2  = aux_face.mX;
        auto cvY2 = aux_cv.mY + aux_cv.mLabelHeight;

        assignXYToAbsorbees(
            UG,
            UG.mIndexSecondStart,
            UG.absorbeesSize(),
            VisRepFinder::RtoL,
            VisRepFinder::TOP,
            VisRepFinder::FullGapLeftHalfGapRight,
            XL2,
            XR2,
            cvY2
        );
    }
}


void VisRepFinder::pass2ChildBlocksInnerFaceRightCV(SortedUnificationGroup& UG)
{

    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());
    auto& aux_face  = findAuxsFace(UG.absorber());

    if (UG.absorber().roleOfECWReversed()) {

        auto XL1  = aux_face.mX;
        auto XR1  = aux_eeCW.mX - aux_deCW.mLabelWidthLeft;
        auto cvY1 = aux_cv.mY;

        assignXYToAbsorbees(
            UG,
            0,
            UG.mIndexSecondStart,
            VisRepFinder::RtoL,
            VisRepFinder::BOTTOM,
            VisRepFinder::HalfGapLeftFullGapRight,
            XL1,
            XR1,
            cvY1
        );

        auto XL2  = aux_face.mX;
        auto XR2  = aux_eeCCW.mX - aux_deCCW.mLabelWidthLeft;
        auto cvY2 = aux_cv.mY + aux_cv.mLabelHeight;

        assignXYToAbsorbees(
            UG,
            UG.mIndexSecondStart,
            UG.absorbeesSize(),
            VisRepFinder::LtoR,
            VisRepFinder::TOP,
            VisRepFinder::HalfGapLeftFullGapRight,
            XL2,
            XR2,
            cvY2
        );
    }
    else {

        auto XL1  = aux_face.mX;
        auto XR1  = aux_eeCCW.mX - aux_deCCW.mLabelWidthLeft;
        auto cvY1 = aux_cv.mY + aux_cv.mLabelHeight;

        assignXYToAbsorbees(
            UG,
            0,
            UG.mIndexSecondStart,
            VisRepFinder::RtoL,
            VisRepFinder::TOP,
            VisRepFinder::HalfGapLeftFullGapRight,
            XL1,
            XR1,
            cvY1
        );

        auto XL2  = aux_face.mX;
        auto XR2  = aux_eeCW.mX - aux_deCW.mLabelWidthLeft;
        auto cvY2 = aux_cv.mY;

        assignXYToAbsorbees(
            UG,
            UG.mIndexSecondStart,
            UG.absorbeesSize(),
            VisRepFinder::LtoR,
            VisRepFinder::BOTTOM,
            VisRepFinder::HalfGapLeftFullGapRight,
            XL2,
            XR2,
            cvY2
        );

    }
}


void VisRepFinder::pass2ChildBlocksOuterFaceTopCV(SortedUnificationGroup& UG)
{
//cerr << "pass2ChildBlocksOuterFaceTopCV() BEGIN\n";
    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& embTN  = UG.absorber().treeNode();
    auto& aux    = mAux[embTN.utility()];

//    auto& aux_cv    = findAuxsCV(UG.absorber());
//    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
//    auto& aux_deCW  = findAuxsDECW(UG.absorber());
//    auto XL  = aux_eeCW.mX + aux_deCW.mLabelWidthRight;
//    auto XR  = aux_eeCW.mX + aux_deCW.mRightInset;
//    auto cvY = aux_cv.mY + aux_cv.mLabelHeight;

    auto XL  = aux.mXOffsetRight;
    auto XR  = XL + aux.mExtraWidthRight ;
    auto cvY = aux.mYOffsetRight + aux.mExtraHeightRight;

//cerr << "XL: " << XL << "\n";
//cerr << "XR: " << XR << "\n";
//cerr << "cvY: " << cvY << "\n";

    assignXYToAbsorbees(
        UG,
        0,
        UG.absorbeesSize(),
        UG.absorber().roleOfECWReversed()?
            VisRepFinder::LtoR:
            VisRepFinder::RtoL,
        VisRepFinder::TOP,
        VisRepFinder::NoGapLeftNoGapRight,
        XL,
        XR,
        cvY
    );
//cerr << "pass2ChildBlocksOuterFaceTopCV() END\n";
}


void VisRepFinder::pass2ChildBlocksOuterFaceBottomCV(
    SortedUnificationGroup& UG
) {
//cerr << "pass2ChildBlocksOuterFaceBottomCV()\n";
    auto& embTN  = UG.absorber().treeNode();
    auto& aux    = mAux[embTN.utility()];

//    auto& cv     = UG.absorber().cutVertexInEG();
//    auto& aux_cv = aux.mEGNodeAux[cv.utility()];

//    auto XL  = aux.mAnchorX1;
//    auto XR  = aux.mAnchorX2;
//    auto cvY = aux_cv.mY + aux_cv.mLabelHeight;


    auto XL  = aux.mXOffsetBottom;
    auto XR  = XL + aux.mExtraWidthBottom;
    auto cvY = aux.mYOffsetBottom + aux.mExtraHeightBottom;

//cerr << "UG.absorber().roleOfECWReversed()?" << UG.absorber().roleOfECWReversed() << "\n";
//cerr << "UG.absorbeesSize(): " << UG.absorbeesSize() << "\n";
//cerr << "XL: " << XL << "\n";
//cerr << "XR: " << XR << "\n";
//cerr << "cvY: " << cvY << "\n";
    assignXYToAbsorbees(
        UG,
        0,
        UG.absorbeesSize(),
        UG.absorber().roleOfECWReversed()?
            VisRepFinder::LtoR:
            VisRepFinder::RtoL,
        VisRepFinder::TOP,
        VisRepFinder::NoGapLeftNoGapRight,
        XL,
        XR,
        cvY
    );
}


void VisRepFinder::pass2ChildBlocksOuterFaceLeftCV(SortedUnificationGroup& UG)
{
    // CW-CCW classification is according to the block, not to the UG.
    // CCW is to the upper and CW is to the lower in the absorber face.
    auto& aux_cv    = findAuxsCV(UG.absorber());
    auto& aux_eeCW  = findAuxsEECW(UG.absorber());
    auto& aux_eeCCW = findAuxsEECCW(UG.absorber());
    auto& aux_deCW  = findAuxsDECW(UG.absorber());
    auto& aux_deCCW = findAuxsDECCW(UG.absorber());
    auto& aux_face  = findAuxsFace(UG.absorber());

    if (UG.absorber().roleOfECWReversed()) {

        auto XL1  = aux_eeCW.mX + aux_deCW.mLabelWidthRight;
        auto XR1  = aux_face.mX;
        auto cvY1 = aux_cv.mY + aux_cv.mLabelHeight;

        assignXYToAbsorbees(
            UG,
            0,
            UG.mIndexSecondStart,
            VisRepFinder::LtoR,
            VisRepFinder::TOP,
            VisRepFinder::FullGapLeftNoGapRight,
            XL1,
            XR1,
            cvY1
        );

        auto XL2  = aux_eeCCW.mX + aux_deCCW.mLabelWidthRight;
        auto XR2  = aux_face.mX;
        auto cvY2 = aux_cv.mY;

        assignXYToAbsorbees(
            UG,
            UG.mIndexSecondStart,
            UG.absorbeesSize(),
            VisRepFinder::RtoL,
            VisRepFinder::BOTTOM,
            VisRepFinder::FullGapLeftNoGapRight,
            XL2,
            XR2,
            cvY2
        );
    }
    else {

        auto XL1  = aux_eeCCW.mX + aux_deCCW.mLabelWidthRight;
        auto XR1  = aux_face.mX;
        auto cvY1 = aux_cv.mY;

        assignXYToAbsorbees(
            UG,
            0,
            UG.mIndexSecondStart,
            VisRepFinder::LtoR,
            VisRepFinder::BOTTOM,
            VisRepFinder::FullGapLeftNoGapRight,
            XL1,
            XR1,
            cvY1
        );

        auto XL2  = aux_eeCW.mX + aux_deCW.mLabelWidthRight;
        auto XR2  = aux_face.mX;
        auto cvY2 = aux_cv.mY + aux_cv.mLabelHeight;

        assignXYToAbsorbees(
            UG,
            UG.mIndexSecondStart,
            UG.absorbeesSize(),
            VisRepFinder::RtoL,
            VisRepFinder::TOP,
            VisRepFinder::FullGapLeftNoGapRight,
            XL2,
            XR2,
            cvY2
        );
    }

}


void VisRepFinder::pass2ChildBlocksOuterFaceRightCV(SortedUnificationGroup& UG)
{
    // Must not be called.
    ;
}


void VisRepFinder::pass2ChildBlocksK2TopCV(SortedUnificationGroup& UG)
{
    pass2ChildBlocksOuterFaceTopCV(UG);
}


void VisRepFinder::pass2ChildBlocksK2BottomCV(SortedUnificationGroup& UG)
{
    pass2ChildBlocksOuterFaceBottomCV(UG);
}


void VisRepFinder::assignXYToAbsorbees(
    SortedUnificationGroup&  UG,
    long                     startIndex,
    long                     endIndex,
    enum assignDirectionType dirType,
    enum topNodeLocationType topNodeLocation,
    enum terminalType        terminal,
    coord_t                  posL,
    coord_t                  posR,
    coord_t                  cvY
) {
//cerr << "assignXYToAbsorbees() BEGIN\n";

    if (startIndex == endIndex) {
      return;
    }

    coord_t sumWidths = findSumWidthsWithoutGaps(UG, startIndex, endIndex);

    coord_t numGaps = (coord_t)(endIndex - startIndex -1);

    switch(terminal) {
      case FullGapLeftFullGapRight:
        numGaps+= 2.0;
        break;
      case FullGapLeftHalfGapRight:
      case HalfGapLeftFullGapRight:
        numGaps+= 1.5;
        break;
      case FullGapLeftNoGapRight:
        numGaps+= 1.0;
        break;
      case NoGapLeftNoGapRight:
      default:
        break;
    }

//cerr << "numGaps: " << numGaps << "\n";

    auto scaling = (posR - posL) / (sumWidths + numGaps * mHGap);
//cerr << "scaling : " << scaling << "\n"; 
    auto scaledGap = mHGap * scaling;
//cerr << "scaledGap : " << scaledGap << "\n"; 
    if(dirType == LtoR){
//cerr << "assignXYToAbsorbees() LtoR\n";
        auto startX = posL;

        switch(terminal) {
          case FullGapLeftFullGapRight:
          case FullGapLeftHalfGapRight:
          case FullGapLeftNoGapRight:
            startX += (scaledGap * 1.0);
            break;
          case HalfGapLeftFullGapRight:
            startX += (scaledGap * 0.5);
            break;
          case NoGapLeftNoGapRight:
          default:
            break;
        }

        for (size_t i = startIndex; i < endIndex; i++) {

            auto& UF    = UG.absorbeeAt(i);
            auto& embTN = UF.treeNode();
            auto& aux   = mAux[embTN.utility()];

            if (topNodeLocation==TOP) {
                aux.mAnchorY1 = cvY - aux.mHeight;
                aux.mAnchorY2 = cvY;
                aux.mFlippedVertically = false;
//cerr << "assignXYToAbsorbees() TOP\n";
            }
            else {
                aux.mAnchorY1 = cvY;
                aux.mAnchorY2 = cvY + aux.mHeight;
                aux.mFlippedVertically = true;
//cerr << "assignXYToAbsorbees() BOTTOM: << " << aux.mAnchorY1 << "," << aux.mAnchorY2 << "\n";
            }

            if (UF.roleOfECWReversed()) {
                aux.mFlippedHorizontally = false; 
//cerr << "assignXYToAbsorbees() Reversed CKP1\n";
            }
            else {
                aux.mFlippedHorizontally = true; 
//cerr << "assignXYToAbsorbees() NOT Reversed CKP2\n";
            }

//cerr << "assignXYToAbsorbees(): Scaling Width<< " << scaling << "," << aux.mWidth << "\n";
            aux.mAnchorX1 = startX + aux.mWidth * 0.5 * (scaling - 1.0);
            aux.mAnchorX2 = aux.mAnchorX1 + aux.mWidth;
//cerr << "assignXYToAbsorbees() X1 X2: << " << aux.mAnchorX1 << "," << aux.mAnchorX2 << "\n";

            startX += (aux.mWidth * scaling);
            startX += scaledGap;
        }
    }
    else { // (dirType == RtoL)
//cerr << "assignXYToAbsorbees() RtoL\n";
        auto startX = posR;

        switch(terminal) {
          case FullGapLeftFullGapRight:
          case HalfGapLeftFullGapRight:
            startX -= (scaledGap * 1.0);
            break;
          case FullGapLeftHalfGapRight:
            startX -= (scaledGap * 0.5);
            break;
          case FullGapLeftNoGapRight:
          default:
            break;
        }

        for (size_t i = startIndex; i < endIndex; i++) {

            auto& UF    = UG.absorbeeAt(i);
            auto& embTN = UF.treeNode();
            auto& aux   = mAux[embTN.utility()];

            if (topNodeLocation==TOP) {
                aux.mAnchorY1 = cvY - aux.mHeight;
                aux.mAnchorY2 = cvY;
                aux.mFlippedVertically = false;
            }
            else {
                aux.mAnchorY1 = cvY;
                aux.mAnchorY2 = cvY + aux.mHeight;
                aux.mFlippedVertically = true;
            }

            if (UF.roleOfECWReversed()) {
                aux.mFlippedHorizontally = true; 
//cerr << "assignXYToAbsorbees() Reversed CKP3\n";
            }
            else {
                aux.mFlippedHorizontally = false; 
//cerr << "assignXYToAbsorbees() Not Reversed CKP4\n";
            }

            aux.mAnchorX2 = startX - aux.mWidth * 0.5 * (scaling - 1.0);
            aux.mAnchorX1 = aux.mAnchorX2 - aux.mWidth;

            startX -= (aux.mWidth * scaling);
            startX -= scaledGap;
        }
    }
//cerr << "assignXYToAbsorbees() END\n";
}


coord_t VisRepFinder::findSumWidthsWithoutGaps(
    SortedUnificationGroup& UG,
    long                    startIndex,
    long                    endIndex
) {
    coord_t sumWidths  = 0.0;

    for (size_t i = startIndex; i < endIndex; i++) {

        auto& UF    = UG.absorbeeAt(i);
        auto& embTN = UF.treeNode();
        auto& aux   = mAux[embTN.utility()];

        sumWidths += aux.mWidth;
    }
    return sumWidths;
}


void VisRepFinder::translate(
    EmbeddedBCTree::ExplorationNode& expNode
) {
//cerr  << "translate() BEGIN\n";
    auto& embTN = 
                dynamic_cast<EmbeddedBCTreeNode&>(expNode.IGBackwardLinkRef());
    auto& aux   = mAux[embTN.utility()];

    // based on the anchor point.
    auto enPair = embTN.embeddedGraph().nodes();
    for (auto enit = enPair.first; enit != enPair.second; enit++) {
        auto& en     = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& aux_en = aux.mEGNodeAux[en.utility()];

        aux_en.mY += aux.mAnchorY1;
    }

    auto eePair = embTN.embeddedGraph().edges();
    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {
        auto& ee     = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& aux_ee = aux.mEGEdgeAux[ee.utility()];

        aux_ee.mX += aux.mAnchorX1;
    }

    auto fPair = embTN.dualGraph().nodes();
    for (auto fit = fPair.first; fit != fPair.second; fit++) {
        auto& f     = dynamic_cast<EmbeddedFace&>(*(*fit));
        auto& aux_f = aux.mDGFaceAux[f.utility()];

        aux_f.mX += aux.mAnchorX1;
    }

//    auto& sortedUGs = expNode.sortedUnificationGroups();
//
//    for (auto& UG : sortedUGs) {
//
//        for (size_t i = 0; i < UG.absorbeesSize(); i++) {
//
//            auto& UF     = UG.absorbeeAt(i);
//            auto& embTN2 = UF.treeNode();
//            auto& aux_ch = mAux[embTN2.utility()];
//
//            aux_ch.mAnchorY1 += aux.mAnchorY1;
//            aux_ch.mAnchorY2 += aux.mAnchorY1;
//            aux_ch.mAnchorX1 += aux.mAnchorX1;
//            aux_ch.mAnchorX2 += aux.mAnchorX1;
//        }
//    }

//    cerr << "BEFORE\n";
//    cerr << "aux.mXOffsetBlock: " <<  aux.mXOffsetBlock << "\n";
//    cerr << "aux.mYOffsetBlock: " <<  aux.mYOffsetBlock << "\n";
//    cerr << "aux.mXOffsetRight: " <<  aux.mXOffsetRight << "\n";
//    cerr << "aux.mYOffsetRight: " <<  aux.mYOffsetRight << "\n";
//    cerr << "aux.mXOffsetBottom: " <<  aux.mXOffsetBottom << "\n";
//    cerr << "aux.mYOffsetBottom: " <<  aux.mYOffsetBottom << "\n";


    aux.mXOffsetBlock  += aux.mAnchorX1;
    aux.mYOffsetBlock  += aux.mAnchorY1;
    aux.mXOffsetRight  += aux.mAnchorX1;
    aux.mYOffsetRight  += aux.mAnchorY1;
    aux.mXOffsetBottom += aux.mAnchorX1;
    aux.mYOffsetBottom += aux.mAnchorY1;

//    cerr << "AFTER\n";
//    cerr << "aux.mXOffsetBlock: " <<  aux.mXOffsetBlock << "\n";
//    cerr << "aux.mYOffsetBlock: " <<  aux.mYOffsetBlock << "\n";
//    cerr << "aux.mXOffsetRight: " <<  aux.mXOffsetRight << "\n";
//    cerr << "aux.mYOffsetRight: " <<  aux.mYOffsetRight << "\n";
//    cerr << "aux.mXOffsetBottom: " <<  aux.mXOffsetBottom << "\n";
//    cerr << "aux.mYOffsetBottom: " <<  aux.mYOffsetBottom << "\n";

//cerr  << "translate() END\n";
}


void VisRepFinder::correctOrientations(
    EmbeddedBCTree::ExplorationNode& expNode
) {
//    cerr << "correctOrientations() BEGIN\n";
    auto& embTN =
                dynamic_cast<EmbeddedBCTreeNode&>(expNode.IGBackwardLinkRef());

    auto& aux   = mAux[embTN.utility()];

//    cerr << "aux.mAnchorY1: " << aux.mAnchorY1 << "\n";
//    cerr << "aux.mAnchorY2: " << aux.mAnchorY2 << "\n";

    if (aux.mFlippedVertically) {
//cerr << "aux.mFlippedVertically\n";
        auto enPair = embTN.embeddedGraph().nodes();
        for (auto enit = enPair.first; enit != enPair.second; enit++) {
            auto& en     = dynamic_cast<EmbeddedNode&>(*(*enit));
            auto& aux_en = aux.mEGNodeAux[en.utility()];
//            cerr << "aux_en.mY 1: " << aux_en.mY << "\n";
            aux_en.mY = aux.mAnchorY2 - (aux_en.mY - aux.mAnchorY1);
//            cerr << "aux_en.mY 2: " << aux_en.mY << "\n";
        }

    }
    if (aux.mFlippedHorizontally) {
//cerr << "aux.mFlippedHorizontally\n";
        auto eePair = embTN.embeddedGraph().edges();
        for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {
            auto& ee     = dynamic_cast<EmbeddedEdge&>(*(*eeit));
            auto& aux_ee = aux.mEGEdgeAux[ee.utility()];

            aux_ee.mX = aux.mAnchorX2 - (aux_ee.mX - aux.mAnchorX1);

        }

        auto fPair = embTN.dualGraph().nodes();
        for (auto fit = fPair.first; fit != fPair.second; fit++) {
            auto& f     = dynamic_cast<EmbeddedFace&>(*(*fit));
            auto& aux_f = aux.mDGFaceAux[f.utility()];

            aux_f.mX = aux.mAnchorX2 - (aux_f.mX - aux.mAnchorX1);
        }
    }

    if ( ( aux.mFlippedVertically&& !aux.mFlippedHorizontally)||
         (!aux.mFlippedVertically&&  aux.mFlippedHorizontally)  ) {
//cerr << "Edge Flipped\n";
        auto eePair = embTN.embeddedGraph().edges();
        for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {
            auto& ee     = dynamic_cast<EmbeddedEdge&>(*(*eeit));
            auto& aux_ee = aux.mEGEdgeAux[ee.utility()];
            aux_ee.mFlipped = true;
        }
    }

    auto& sortedUGs = expNode.sortedUnificationGroups();

    for (auto& UG : sortedUGs) {

        for (size_t i = 0; i < UG.absorbeesSize(); i++) {

            auto& UF     = UG.absorbeeAt(i);
            auto& embTN  = UF.treeNode();
            auto& aux_ch = mAux[embTN.utility()];

            if (aux.mFlippedVertically) {
                aux_ch.mAnchorY1 = 
                            aux.mAnchorY2 - (aux_ch.mAnchorY1 - aux.mAnchorY1);
                aux_ch.mAnchorY2 = 
                            aux.mAnchorY2 - (aux_ch.mAnchorY2 - aux.mAnchorY1);
                std::swap(aux_ch.mAnchorY1, aux_ch.mAnchorY2);
                aux_ch.mFlippedVertically = !aux_ch.mFlippedVertically;
            }
            if (aux.mFlippedHorizontally) {
                aux_ch.mAnchorX1 = 
                            aux.mAnchorX2 - (aux_ch.mAnchorX1 - aux.mAnchorX1);
                aux_ch.mAnchorX2 = 
                            aux.mAnchorX2 - (aux_ch.mAnchorX2 - aux.mAnchorX1);
                std::swap(aux_ch.mAnchorX1, aux_ch.mAnchorX2);
                aux_ch.mFlippedHorizontally = !aux_ch.mFlippedHorizontally;
            }
        }
    }

    if (aux.mFlippedVertically) {
//cerr << "Flipping Vertically\n";
//    cerr << "aux.mYOffsetBlock: " <<  aux.mYOffsetBlock << "\n";
//    cerr << "aux.mYOffsetRight: " <<  aux.mYOffsetRight << "\n";
//    cerr << "aux.mYOffsetBottom: " <<  aux.mYOffsetBottom << "\n";

        aux.mYOffsetBlock =
                          aux.mAnchorY2 - (aux.mYOffsetBlock - aux.mAnchorY1) -
                          aux.mBlockHeight;
        aux.mYOffsetRight =
                          aux.mAnchorY2 - (aux.mYOffsetRight - aux.mAnchorY1) -
                          aux.mExtraHeightRight;
        aux.mYOffsetBottom = 
                        aux.mAnchorY2 - (aux.mYOffsetBottom  - aux.mAnchorY1) -
                        aux.mExtraHeightBottom;

//    cerr << "aux.mYOffsetBlock: " <<  aux.mYOffsetBlock << "\n";
//    cerr << "aux.mYOffsetRight: " <<  aux.mYOffsetRight << "\n";
//    cerr << "aux.mYOffsetBottom: " <<  aux.mYOffsetBottom << "\n";

    }

    if (aux.mFlippedHorizontally) {
//    cerr << "Flipping Horizontally\n";
//    cerr << "aux.mAnchorX1: " << aux.mAnchorX1 << "\n";
//    cerr << "aux.mAnchorX2: " << aux.mAnchorX2 << "\n";
//    cerr << "aux.mXOffsetBlock: " <<  aux.mXOffsetBlock << "\n";
//    cerr << "aux.mXOffsetRight: " <<  aux.mXOffsetRight << "\n";
//    cerr << "aux.mXOffsetBottom: " <<  aux.mXOffsetBottom << "\n";
//    cerr << "aux.mBlockWidth: " << aux.mBlockWidth << "\n";
//    cerr << "aux.mExtraWidthRight: " << aux.mExtraWidthRight  << "\n";
//    cerr << "aux.mExtraWidthBottom: " << aux.mExtraWidthBottom  << "\n";

        aux.mXOffsetBlock =
                          aux.mAnchorX2 - (aux.mXOffsetBlock - aux.mAnchorX1) -
                          aux.mBlockWidth;
        aux.mXOffsetRight =
                          aux.mAnchorX2 - (aux.mXOffsetRight - aux.mAnchorX1) -
                          aux.mExtraWidthRight;
        aux.mXOffsetBottom = 
                        aux.mAnchorX2 - (aux.mXOffsetBottom  - aux.mAnchorX1) -
                        aux.mExtraWidthBottom;
//    cerr << "aux.mXOffsetBlock: " <<  aux.mXOffsetBlock << "\n";
//    cerr << "aux.mXOffsetRight: " <<  aux.mXOffsetRight << "\n";
//    cerr << "aux.mXOffsetBottom: " <<  aux.mXOffsetBottom << "\n";

    }

//    cerr << "correctOrientations() END\n";
}


void VisRepFinder::findVRCoordinates()
{

    auto& expTree      = mEBCTree.explorationTree();
    auto& bcTree       = mEBCTree.bcTree();
    auto& labelGraph   = dynamic_cast<VRRectLabelGraph&>(bcTree.original());

    auto& expRoot      = expTree.root();
    auto& embRoot      =
                dynamic_cast<EmbeddedBCTreeNode&>(expRoot.IGBackwardLinkRef());
    auto& aux          = mAux[embRoot.utility()];

    // Assign the width and height of VR.
    labelGraph.mWidth  = aux.mWidth;
    labelGraph.mHeight = aux.mHeight;

    // Reset the flag.
    auto lgnPair = labelGraph.nodes();
    for (auto nit = lgnPair.first; nit != lgnPair.second; nit++) {
        auto& N = dynamic_cast<VRRectLabelNode&>(*(*nit));
        N.mXset = false;
    }


    // Exploring through the tree, and find Y for EmbeddedNodes and
    // X for EmbeddedEdges.
    list<node_list_it_t> Q;
    size_t               Qsize = 0;

    Q.push_back(expRoot.backIt());
    Qsize++;

    while (Qsize > 0) {

        auto& expNode = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                          *(*(*(Q.begin()))));
        Q.pop_front();
        Qsize--;

        findVRCoordinatesOfTreeNode(expNode);

        auto ePair = expNode.incidentEdgesOut();
        for (auto eit = ePair.first; eit != ePair.second; eit++) {
            auto& expEdge = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(
                                                                   *(*(*eit)));
            auto& adjNode = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                expEdge.adjacentNode(expNode));
            Q.push_back(adjNode.backIt());
            Qsize++;
        }
    }

    auto lgePair = labelGraph.edges();
    for (auto eit = lgePair.first; eit != lgePair.second; eit++) {
        auto& E  = dynamic_cast<VRRectLabelEdge&>(*(*eit));
        auto& N1 = dynamic_cast<VRRectLabelNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<VRRectLabelNode&>(E.incidentNode2());
        E.mY1 = N1.mY;
        E.mY2 = N2.mY;
    }
}


void VisRepFinder::findVRCoordinatesOfTreeNode(
    EmbeddedBCTree::ExplorationNode& expNode
) {
    auto& embTN =
                dynamic_cast<EmbeddedBCTreeNode&>(expNode.IGBackwardLinkRef());
    auto& aux       = mAux[embTN.utility()];
    auto& outerFace = embTN.outerFace();

    // Adjust mX and mY of EmbeddedNodes and EmbeddedEdges
    // based on the anchor point and the orientation.
    auto enPair = embTN.embeddedGraph().nodes();
    for (auto enit = enPair.first; enit != enPair.second; enit++) {
        auto& en     = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& aux_en = aux.mEGNodeAux[en.utility()];
        auto& bn = dynamic_cast<BlockNode&>(en.IGBackwardLinkRef());
        auto& ln = dynamic_cast<VRRectLabelNode&>(bn.IGBackwardLinkRef());

        ln.mY = aux.mFlippedVertically?
                    aux_en.mY - aux_en.mLabelHeight / 2.0:
                    aux_en.mY + aux_en.mLabelHeight / 2.0;

        auto& fleft  = dynamic_cast<EmbeddedFace&>(*(*(aux_en.mLeftFace)));
        auto& fright = dynamic_cast<EmbeddedFace&>(*(*(aux_en.mRightFace)));
        auto& aux_fl = aux.mDGFaceAux[fleft.utility()];
        auto& aux_fr = aux.mDGFaceAux[fright.utility()];


        coord_t Xleft, Xright; // Left and right are aligned with the current
                               // X-axis L->R, which is reversed if the block
                               // is flipped.

        if (aux.mFlippedHorizontally) {

            Xright = (aux_en.mLeftFace!=outerFace.backIt())?
                       (aux_fl.mX + mHGap/2.0):
                       (aux.mXOffsetBlock + aux.mBlockWidth);
            Xleft  = (aux_en.mRightFace!=outerFace.backIt())?
                       (aux_fr.mX - mHGap/2.0):(aux.mXOffsetBlock);
//cerr << "(aux_en.mLeftFace!=outerFace.backIt())? " << (aux_en.mLeftFace!=outerFace.backIt()) << "\n";
//cerr << "(aux_en.mRightFace!=outerFace.backIt())? " << (aux_en.mRightFace!=outerFace.backIt()) << "\n";
//cerr << "aux.mXOffsetBlock: "  << aux.mXOffsetBlock << "\n";
        }
        else {
            Xleft  = (aux_en.mLeftFace!=outerFace.backIt())?
                       (aux_fl.mX + mHGap/2.0):(aux.mXOffsetBlock);
            Xright = (aux_en.mRightFace!=outerFace.backIt())?
                       (aux_fr.mX - mHGap/2.0):
                       (aux.mXOffsetBlock + aux.mBlockWidth);
        }
//cerr << "aux.mFlippedHorizontally: " << aux.mFlippedHorizontally << "\n";
//cerr << "aux.mXOffsetBlock: " << aux.mXOffsetBlock << "\n";
//cerr << "Xleft: " << (aux_en.mLeftFace!=outerFace.backIt()) << " " << Xleft << "\n";
//cerr << "Xright: " << (aux_en.mRightFace!=outerFace.backIt()) << " " << Xright << "\n";

        if (ln.mXset==false) {
            ln.mXleft  = Xleft;
            ln.mXright = Xright;
            ln.mXset=true;
        }
        else {
            ln.mXleft  = std::min(ln.mXleft,  Xleft);
            ln.mXright = std::max(ln.mXright, Xright);
        }
    }

    auto eePair = embTN.embeddedGraph().edges();
    for (auto eeit = eePair.first; eeit != eePair.second; eeit++) {
        auto& ee = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& be = dynamic_cast<BlockEdge&>(ee.IGBackwardLinkRef());
        auto& le = dynamic_cast<VRRectLabelEdge&>(be.IGBackwardLinkRef());
        auto& aux_ee = aux.mEGEdgeAux[ee.utility()];

        le.mX = aux_ee.mX;
        le.mFlipped = aux_ee.mFlipped;
    }
}


#ifdef UNIT_TESTS

inline static std::ostream& debugIndent(std::ostream& os, size_t ind)
{
    for (size_t i = 0; i < ind; i++) {
        os << ' ';
    }
    return os;
}


std::ostream& EGNodeAux::debugPrint(std::ostream& os, size_t indent, long nodeNum)
{

    debugIndent(os, indent);
    os << "EGNodeAux: ";
    os << "mSTNum: " << mSTNum << " ";
    os << "mNumOutgoingProcessed: " << mNumOutgoingProcessed << " ";
    os << "mLabelHeight: " << mLabelHeight << " ";
    os << "mCost: " << mCost << " ";
    os << "mEarlyStart: " << mEarlyStart << " ";
    os << "mY: " << mY << " ";
#ifdef USE_TEST_GRAPH_GENERATOR
    os << "mEdgesIn: ";

    for (auto e : mEdgesIn) {

        auto& E  = dynamic_cast<EmbeddedEdge&>(*(*e));
        auto& N1 = dynamic_cast<EmbeddedNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<EmbeddedNode&>(E.incidentNode2());
        auto& B1 = dynamic_cast<BlockNode&>(N1.IGBackwardLinkRef());
        auto& B2 = dynamic_cast<BlockNode&>(N2.IGBackwardLinkRef());
        auto& L1 = dynamic_cast<VRRectLabelNode&>(B1.IGBackwardLinkRef());
        auto& L2 = dynamic_cast<VRRectLabelNode&>(B2.IGBackwardLinkRef());

        os << "{" << ((L1.mDebugNum==nodeNum)?L2.mDebugNum:
                                              L1.mDebugNum) << ","
           <<        ((L1.mDebugNum==nodeNum)?L1.mDebugNum:
                                              L2.mDebugNum) << "} ";

    }
    os << " ";
    os << "mEdgesOut: ";
    for (auto e : mEdgesOut) {

        auto& E  = dynamic_cast<EmbeddedEdge&>(*(*e));
        auto& N1 = dynamic_cast<EmbeddedNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<EmbeddedNode&>(E.incidentNode2());
        auto& B1 = dynamic_cast<BlockNode&>(N1.IGBackwardLinkRef());
        auto& B2 = dynamic_cast<BlockNode&>(N2.IGBackwardLinkRef());
        auto& L1 = dynamic_cast<VRRectLabelNode&>(B1.IGBackwardLinkRef());
        auto& L2 = dynamic_cast<VRRectLabelNode&>(B2.IGBackwardLinkRef());
        os << "{" << ((L1.mDebugNum==nodeNum)?L1.mDebugNum:
                                              L2.mDebugNum) << ","
           <<        ((L1.mDebugNum==nodeNum)?L2.mDebugNum:
                                              L1.mDebugNum) << "} ";

    }
#endif

    return os;
}


std::ostream& EGEdgeAux::debugPrint(std::ostream& os, size_t indent)
{

    debugIndent(os, indent);
    os << "EGEdgeAux: ";
    os << (mOriented1to2?"mOriented1to2":"mOriented2to1") << " ";
    os << "mLabelHeightUpper: " << mLabelHeightUpper << " ";
    os << "mLabelHeightMiddle: " << mLabelHeightMiddle << " ";
    os << "mLabelHeightLower: " << mLabelHeightLower << " ";
    os << "mLabelHeight: " << mLabelHeight << " ";
    os << "mChildrenHeightUpperRight: " <<  mChildrenHeightUpperRight << " ";
    os << "mChildrenHeightLowerRight: " <<  mChildrenHeightLowerRight << " ";
    os << "mChildrenHeightUpperLeft: " <<  mChildrenHeightUpperLeft << " ";
    os << "mChildrenHeightLowerLeft: " <<  mChildrenHeightLowerLeft << " ";
    os << "mCost: " <<  mCost << " ";
    os << "mX: " <<  mX << " ";

    return os;
}


std::ostream& DGFaceAux::debugPrint(std::ostream& os, size_t indent, long nodeNum)
{

    debugIndent(os, indent);
    os << "DGFaceAux: ";
    os << "mNumIncomingProcessed: " << mNumIncomingProcessed << " ";
    os << "mCost: " << mCost << " ";
    os << "mEarlyStart: " << mEarlyStart << " ";
    os << "mX: " << mX << " ";
#ifdef USE_TEST_GRAPH_GENERATOR
    os << "mEdgesIn: ";
    for (auto e : mEdgesIn) {

        auto& D  = dynamic_cast<DualEdge&>(*(*e));
        auto& F1 = dynamic_cast<EmbeddedFace&>(D.incidentNode1());
        auto& F2 = dynamic_cast<EmbeddedFace&>(D.incidentNode2());
        os << "{" << ((F1.mDebugNum==nodeNum)?F2.mDebugNum:
                                              F1.mDebugNum) << ","
           <<        ((F1.mDebugNum==nodeNum)?F1.mDebugNum:
                                              F2.mDebugNum) << "} ";
    }
    os << " ";
    os << "mEdgesOut: ";
    for (auto e : mEdgesOut) {

        auto& D  = dynamic_cast<DualEdge&>(*(*e));
        auto& F1 = dynamic_cast<EmbeddedFace&>(D.incidentNode1());
        auto& F2 = dynamic_cast<EmbeddedFace&>(D.incidentNode2());
        os << "{" << ((F1.mDebugNum==nodeNum)?F1.mDebugNum:
                                              F2.mDebugNum) << ","
           <<        ((F1.mDebugNum==nodeNum)?F2.mDebugNum:
                                              F1.mDebugNum) << "} ";
    }
#endif

    return os;
}


std::ostream& DGEdgeAux::debugPrint(std::ostream& os, size_t indent)
{

    debugIndent(os, indent);
    os << "DGEdgeAux: ";
    os << (mOriented1to2?"mOriented1to2":"mOriented2to1") << " ";
    os << "mWidthLabelShareUpper: " << mWidthLabelShareUpper << " ";
    os << "mWidthLabelShareLower: " << mWidthLabelShareLower << " ";
    os << "mLabelWidthUpperRight: " << mLabelWidthUpperRight << " ";
    os << "mLabelWidthMiddleRight: " << mLabelWidthMiddleRight << " ";
    os << "mLabelWidthLowerRight: " << mLabelWidthLowerRight << " ";
    os << "mLabelWidthUpperLeft: " << mLabelWidthUpperLeft << " ";
    os << "mLabelWidthMiddleLeft: " << mLabelWidthMiddleLeft << " ";
    os << "mLabelWidthLowerLeft: " << mLabelWidthLowerLeft << " ";
    os << "mChildrenWidthUpperRight: " << mChildrenWidthUpperRight << " ";
    os << "mChildrenWidthLowerRight: " << mChildrenWidthLowerRight << " ";
    os << "mChildrenWidthUpperLeft: " << mChildrenWidthUpperLeft << " ";
    os << "mChildrenWidthLowerLeft: " << mChildrenWidthLowerLeft << " ";
    os << "mLabelWidthRight: " << mLabelWidthRight << " ";
    os << "mLabelWidthLeft: " << mLabelWidthLeft << " ";
    os << "mLeftInset: " << mLeftInset << " ";
    os << "mRightInset: " << mRightInset << " ";
    os << "mCountMeOut: " << mCountMeOut << " ";
    os << "mCost: " << mCost << " ";

    return os;
}


std::ostream& ETNodeAux::debugPrint(std::ostream& os, size_t indent)
{
    debugIndent(os, indent);
    os << "ETNodeAux: ";
    os << "mFlippedVertically: " << (mFlippedVertically?"Yes":"No") << " ";
    os << "mFlippedHorizontally: " << (mFlippedHorizontally?"Yes":"No") << " ";
    os << "mBlockHeight: " << mBlockHeight <<  " ";
    os << "mBlockWidth: " << mBlockWidth <<  " ";
    os << "mExtraHeightBottom: " << mExtraHeightBottom <<  " ";
    os << "mExtraWidthBottom: " << mExtraWidthBottom <<  " ";
    os << "mExtraHeightRight: " << mExtraHeightRight <<  " ";
    os << "mExtraWidthRight: " << mExtraWidthRight <<  " ";
    os << "mXOffsetBlock: "  << mXOffsetBlock << " ";
    os << "mYOffsetBlock: "  << mYOffsetBlock << " ";
    os << "mXOffsetRight: "  << mXOffsetRight << " ";
    os << "mYOffsetRight: "  << mYOffsetRight << " ";
    os << "mXOffsetBottom: " << mXOffsetBottom << " ";
    os << "mYOffsetBottom: " << mYOffsetBottom << " ";
    os << "mWidth: " << mWidth <<  " ";
    os << "mHeight: " << mHeight <<  " ";
    os << "mAnchorX1: " << mAnchorX1 << " ";
    os << "mAnchorX2: " << mAnchorX2 << " ";
    os << "mAnchorY1: " << mAnchorY1 << " ";
    os << "mAnchorY2: " << mAnchorY2 << " ";

    return os;
}


void VisRepFinder::debugPrint(EmbeddedBCTree::ExplorationNode& N, size_t indent)
{
    auto& EBN = dynamic_cast<EmbeddedBCTreeNode&>(N.IGBackwardLinkRef());
    auto& BN  = dynamic_cast<BCTreeNode&>(EBN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "ExplorationNode: Block: " << B.mDebugNum << " ";
    mAux[EBN.utility()].debugPrint(cerr, 0);
    cerr << "\n";
    for (auto enit = EBN.embeddedGraph().nodes().first;
         enit != EBN.embeddedGraph().nodes().second; enit++ ) {
         auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
         auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
         auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
         debugIndent(std::cerr, indent+4);
         cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
         mAux[EBN.utility()].mEGNodeAux[EN.utility()].debugPrint(
                                                        cerr, 0, LN.mDebugNum);
         cerr << "\n";
    }
    for (auto eeit = EBN.embeddedGraph().edges().first;
         eeit != EBN.embeddedGraph().edges().second; eeit++ ) {
         auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
         auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
         auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
         auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
         auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
         auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
         auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
         debugIndent(std::cerr, indent+4);
         cerr << "EmbeddedEdge: {" << LN1.mDebugNum << ","
                                                      << LN2.mDebugNum << "} ";
         mAux[EBN.utility()].mEGEdgeAux[EE.utility()].debugPrint(cerr, 0);
         cerr << "\n";
    }
    for (auto fit = EBN.dualGraph().nodes().first;
         fit != EBN.dualGraph().nodes().second; fit++ ) {
         auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
         debugIndent(std::cerr, indent+4);
         cerr << "EmbeddedFace: " << F.mDebugNum << " ";
         mAux[EBN.utility()].mDGFaceAux[F.utility()].debugPrint(
                                                         cerr, 0, F.mDebugNum);
         cerr << "\n";
    }
    for (auto deit = EBN.dualGraph().edges().first;
         deit != EBN.dualGraph().edges().second; deit++ ) {
         auto& DE  = dynamic_cast<DualEdge&>(*(*deit));
         auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
         auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());
         auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
         auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
         auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
         auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
         auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
         auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
         auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());

         debugIndent(std::cerr, indent+4);
         cerr << "DualEdge: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";
         cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," 
                                                     << LN2.mDebugNum << "} ";
         mAux[EBN.utility()].mDGEdgeAux[DE.utility()].debugPrint(cerr, 0);
         cerr << "\n";
    }
}


void VisRepFinder::debugPrintFindBottomNodeAndLeftMostFace(
    EmbeddedBCTreeNode& ETN,
    node_list_it_t&     bottomIt,
    node_list_it_t&     rightIt,
    edge_list_it_t&     stIt,
    edge_list_it_t&     stDualIt,
    size_t              indent
) {

    debugIndent(std::cerr, indent);
    cerr << "findBottomNodeAndLeftMostFace()\n";
    auto& BTN = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BTN.block();
    debugIndent(std::cerr, indent);
    cerr << "Block: " << B.mDebugNum << " ";
    auto& TopEN = ETN.topNode();
    auto& TopBN = dynamic_cast<BlockNode&>(TopEN.IGBackwardLinkRef());
    auto& TopLN = dynamic_cast<VRRectLabelNode&>(TopBN.IGBackwardLinkRef());
    cerr << "Top Node: " << TopLN.mDebugNum << " ";

    auto& BottomEN = dynamic_cast<EmbeddedNode&>(*(*bottomIt));
    auto& BottomBN = dynamic_cast<BlockNode&>(BottomEN.IGBackwardLinkRef());
    auto& BottomLN = dynamic_cast<VRRectLabelNode&>(
                                                 BottomBN.IGBackwardLinkRef());
    cerr << "Bottom Node: " << BottomLN.mDebugNum << " ";

    auto& OuterF = ETN.outerFace();
    cerr << "Outer Face: " << OuterF.mDebugNum << " ";

    auto& RightF = dynamic_cast<EmbeddedFace&>(*(*rightIt));
    cerr << "Right Face: " << RightF.mDebugNum << " ";

    auto& ST = dynamic_cast<EmbeddedEdge&>(*(*stIt));
    auto& EN1 = dynamic_cast<EmbeddedNode&>(ST.incidentNode1());
    auto& EN2 = dynamic_cast<EmbeddedNode&>(ST.incidentNode2());
    auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
    auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
    auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
    auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
    cerr << "ST: {" << LN1.mDebugNum << "," << LN2.mDebugNum << "} ";   

    auto& STDual = dynamic_cast<DualEdge&>(*(*stDualIt));
    auto& F1 = dynamic_cast<EmbeddedFace&>(STDual.incidentNode1());
    auto& F2 = dynamic_cast<EmbeddedFace&>(STDual.incidentNode2());
    cerr << "ST dual: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";       

    cerr << "\n";

}


void VisRepFinder::debugPrintOrientEGandDG(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "orientEGandDG() Block: " << B.mDebugNum << "\n";

    for (auto enit = ETN.embeddedGraph().nodes().first;
        enit != ETN.embeddedGraph().nodes().second; enit++ ) {
        auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mEGNodeAux[EN.utility()];
        cerr << "mSTNum: " << aux.mSTNum << " ";
#ifdef USE_TEST_GRAPH_GENERATOR
        cerr << "mEdgesIn: ";
        for (auto e : aux.mEdgesIn) {

            auto& E  = dynamic_cast<EmbeddedEdge&>(*(*e));
            auto& N1 = dynamic_cast<EmbeddedNode&>(E.incidentNode1());
            auto& N2 = dynamic_cast<EmbeddedNode&>(E.incidentNode2());
            auto& B1 = dynamic_cast<BlockNode&>(N1.IGBackwardLinkRef());
            auto& B2 = dynamic_cast<BlockNode&>(N2.IGBackwardLinkRef());
            auto& L1 = dynamic_cast<VRRectLabelNode&>(B1.IGBackwardLinkRef());
            auto& L2 = dynamic_cast<VRRectLabelNode&>(B2.IGBackwardLinkRef());

            cerr << "{" << ((L1.mDebugNum==LN.mDebugNum)?L2.mDebugNum:
                                                    L1.mDebugNum) << ","
                 <<        ((L1.mDebugNum==LN.mDebugNum)?L1.mDebugNum:
                                                    L2.mDebugNum) << "} ";
        }
        cerr << " ";
        cerr << "mEdgesOut: ";
        for (auto e : aux.mEdgesOut) {

            auto& E  = dynamic_cast<EmbeddedEdge&>(*(*e));
            auto& N1 = dynamic_cast<EmbeddedNode&>(E.incidentNode1());
            auto& N2 = dynamic_cast<EmbeddedNode&>(E.incidentNode2());
            auto& B1 = dynamic_cast<BlockNode&>(N1.IGBackwardLinkRef());
            auto& B2 = dynamic_cast<BlockNode&>(N2.IGBackwardLinkRef());
            auto& L1 = dynamic_cast<VRRectLabelNode&>(B1.IGBackwardLinkRef());
            auto& L2 = dynamic_cast<VRRectLabelNode&>(B2.IGBackwardLinkRef());
            cerr << "{" << ((L1.mDebugNum==LN.mDebugNum)?L1.mDebugNum:
                                                    L2.mDebugNum) << ","
                 <<        ((L1.mDebugNum==LN.mDebugNum)?L2.mDebugNum:
                                                    L1.mDebugNum) << "} ";

        }
#endif
        cerr << "\n";
    }

    for (auto eeit = ETN.embeddedGraph().edges().first;
         eeit != ETN.embeddedGraph().edges().second; eeit++ ) {
         auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
         auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
         auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
         auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
         auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
         auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
         auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
         debugIndent(std::cerr, indent+4);
         cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," 
                                                      << LN2.mDebugNum << "} ";
         auto& aux = mAux[ETN.utility()].mEGEdgeAux[EE.utility()];
         cerr << (aux.mOriented1to2?"mOriented1to2":"mOriented2to1") << " ";
         cerr << "\n";
    }

    for (auto fit = ETN.dualGraph().nodes().first;
        fit != ETN.dualGraph().nodes().second; fit++ ) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedFace: " << F.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mDGFaceAux[F.utility()];
#ifdef USE_TEST_GRAPH_GENERATOR
        cerr << "mEdgesIn: ";
        for (auto e : aux.mEdgesIn) {

            auto& D  = dynamic_cast<DualEdge&>(*(*e));
            auto& F1 = dynamic_cast<EmbeddedFace&>(D.incidentNode1());
            auto& F2 = dynamic_cast<EmbeddedFace&>(D.incidentNode2());
            cerr << "{" << ((F1.mDebugNum==F.mDebugNum)?F2.mDebugNum:
                                                    F1.mDebugNum) << ","
                 <<        ((F1.mDebugNum==F.mDebugNum)?F1.mDebugNum:
                                                    F2.mDebugNum) << "} ";
        }
        cerr << " ";
        cerr << "mEdgesOut: ";
        for (auto e : aux.mEdgesOut) {

            auto& D  = dynamic_cast<DualEdge&>(*(*e));
            auto& F1 = dynamic_cast<EmbeddedFace&>(D.incidentNode1());
            auto& F2 = dynamic_cast<EmbeddedFace&>(D.incidentNode2());
            cerr << "{" << ((F1.mDebugNum==F.mDebugNum)?F1.mDebugNum:
                                                    F2.mDebugNum) << ","
                 <<        ((F1.mDebugNum==F.mDebugNum)?F2.mDebugNum:
                                                    F1.mDebugNum) << "} ";
        }
#endif
        cerr << "\n";
    }

    for (auto deit = ETN.dualGraph().edges().first;
         deit != ETN.dualGraph().edges().second; deit++ ) {
         auto& DE  = dynamic_cast<DualEdge&>(*(*deit));
         auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
         auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());

         auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
         auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
         auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
         auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
         auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
         auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
         auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());

         debugIndent(std::cerr, indent+4);
         cerr << "DualEdge: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";
         cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," 
                                                      << LN2.mDebugNum << "} ";
         auto& aux = mAux[ETN.utility()].mDGEdgeAux[DE.utility()];
         cerr << (aux.mOriented1to2?"mOriented1to2":"mOriented2to1") << " ";
         cerr << "mCountMeOut: " << aux.mCountMeOut << " ";
         cerr << "\n";
    }
}


void VisRepFinder::debugPrintHandleNodeLabels(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "handleNodeLabels() Block: " << B.mDebugNum << "\n";

    for (auto enit = ETN.embeddedGraph().nodes().first;
        enit != ETN.embeddedGraph().nodes().second; enit++ ) {
        auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mEGNodeAux[EN.utility()];
        cerr << "mLabelHeight: " << aux.mLabelHeight << " ";
        cerr << "\n";
    }

    for (auto deit = ETN.dualGraph().edges().first;
         deit != ETN.dualGraph().edges().second; deit++ ) {
         auto& DE  = dynamic_cast<DualEdge&>(*(*deit));
         auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
         auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());

         auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
         auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
         auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
         auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
         auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
         auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
         auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());

         debugIndent(std::cerr, indent+4);
         cerr << "DualEdge: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";
         cerr << "EmbeddedEdge: {" << LN1.mDebugNum << ","
                                                      << LN2.mDebugNum << "} ";
         auto& aux = mAux[ETN.utility()].mDGEdgeAux[DE.utility()];
         
         cerr << "mWidthLabelShareLower: " << aux.mWidthLabelShareLower << " ";
         cerr << "mWidthLabelShareUpper: " << aux.mWidthLabelShareUpper << " ";

         cerr << "\n";
    }
}


void VisRepFinder::debugPrintHandleEdgeLabels(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "handleEdgeLabels() Block: " << B.mDebugNum << "\n";

    for (auto eeit = ETN.embeddedGraph().edges().first;
        eeit != ETN.embeddedGraph().edges().second; eeit++ ) {
        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << ","
                                                      << LN2.mDebugNum << "} ";
        auto& aux = mAux[ETN.utility()].mEGEdgeAux[EE.utility()];
        cerr << "mLabelHeightUpper: " << aux.mLabelHeightUpper << " ";
        cerr << "mLabelHeightMiddle: " << aux.mLabelHeightMiddle << " ";
        cerr << "mLabelHeightLower: " << aux.mLabelHeightLower << " ";
        cerr << "mLabelHeight: " << aux.mLabelHeight << " ";
        cerr << "\n";
    }

    for (auto deit = ETN.dualGraph().edges().first;
        deit != ETN.dualGraph().edges().second; deit++ ) {
        auto& DE  = dynamic_cast<DualEdge&>(*(*deit));
        auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
        auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());

        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "DualEdge: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," 
                                                      << LN2.mDebugNum << "} ";
        auto& aux = mAux[ETN.utility()].mDGEdgeAux[DE.utility()];
        cerr << "mLabelWidthUpperLeft: " << aux.mLabelWidthUpperLeft << " ";
        cerr << "mLabelWidthUpperRight: " << aux.mLabelWidthUpperRight << " ";
        cerr << "mLabelWidthMiddleLeft: " << aux.mLabelWidthMiddleLeft << " ";
        cerr << "mLabelWidthMiddleRight: " << aux.mLabelWidthMiddleRight 
                                                                        << " ";
        cerr << "mLabelWidthLowerLeft: " << aux.mLabelWidthLowerLeft << " ";
        cerr << "mLabelWidthLowerRight: " << aux.mLabelWidthLowerRight << " ";
        cerr << "mLabelWidthLeft: " << aux.mLabelWidthLeft << " ";
        cerr << "mLabelWidthRight: " << aux.mLabelWidthRight << " ";
        cerr << "\n";
    }
}


void VisRepFinder::debugPrintHandleChildBlocks(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "handleChildBlocks() Block: " << B.mDebugNum << " ";
    cerr << "mExtraHeightBottom: " << mAux[ETN.utility()].mExtraHeightBottom 
                                                                      <<  " ";
    cerr << "mExtraWidthBottom: " << mAux[ETN.utility()].mExtraWidthBottom
                                                                      <<  " ";
    cerr << "mExtraHeightRight: " << mAux[ETN.utility()].mExtraHeightRight
                                                                      <<  " ";
    cerr << "mExtraWidthRight: " << mAux[ETN.utility()].mExtraWidthRight
                                                                      <<  "\n";
    for (auto eeit = ETN.embeddedGraph().edges().first;
        eeit != ETN.embeddedGraph().edges().second; eeit++ ) {
        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," << LN2.mDebugNum 
                                                                       << "} ";
        auto& aux = mAux[ETN.utility()].mEGEdgeAux[EE.utility()];
        cerr << "mChildrenHeightUpperRight: " << aux.mChildrenHeightUpperRight
                                                                        << " ";
        cerr << "mChildrenHeightLowerRight: " << aux.mChildrenHeightLowerRight
                                                                        << " ";
        cerr << "mChildrenHeightUpperLeft: "  << aux.mChildrenHeightUpperLeft
                                                                        << " ";
        cerr << "mChildrenHeightLowerLeft: "  << aux.mChildrenHeightLowerLeft
                                                                        << " ";
        cerr << "\n";
    }

    for (auto deit = ETN.dualGraph().edges().first;
        deit != ETN.dualGraph().edges().second; deit++ ) {
        auto& DE  = dynamic_cast<DualEdge&>(*(*deit));
        auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
        auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());

        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "DualEdge: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," << LN2.mDebugNum 
                                                                       << "} ";
        auto& aux = mAux[ETN.utility()].mDGEdgeAux[DE.utility()];
        cerr << "mChildrenWidthUpperRight: " << aux.mChildrenWidthUpperRight
                                                                        << " ";
        cerr << "mChildrenWidthLowerRight: " << aux.mChildrenWidthLowerRight
                                                                        << " ";
        cerr << "mChildrenWidthUpperLeft: "  << aux.mChildrenWidthUpperLeft
                                                                        << " ";
        cerr << "mChildrenWidthLowerLeft: "  << aux.mChildrenWidthLowerLeft
                                                                        << " ";
        cerr << "\n";
    }
}


void VisRepFinder::debugPrintCalculateCosts(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "handleCalculateCosts() Block: " << B.mDebugNum << "\n";

    for (auto enit = ETN.embeddedGraph().nodes().first;
        enit != ETN.embeddedGraph().nodes().second; enit++ ) {
        auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mEGNodeAux[EN.utility()];
        cerr << "mCost: " << aux.mCost << " ";
        cerr << "\n";
    }

    for (auto eeit = ETN.embeddedGraph().edges().first;
        eeit != ETN.embeddedGraph().edges().second; eeit++ ) {
        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," << LN2.mDebugNum 
                                                                       << "} ";
        auto& aux = mAux[ETN.utility()].mEGEdgeAux[EE.utility()];
        cerr << "mCost: " << aux.mCost << " ";
        cerr << "\n";
    }

    for (auto fit = ETN.dualGraph().nodes().first;
        fit != ETN.dualGraph().nodes().second; fit++ ) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedFace: " << F.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mDGFaceAux[F.utility()];
        cerr << "mCost: " << aux.mCost << " ";
        cerr << "\n";
    }

    for (auto deit = ETN.dualGraph().edges().first;
        deit != ETN.dualGraph().edges().second; deit++ ) {
        auto& DE  = dynamic_cast<DualEdge&>(*(*deit));
        auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
        auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());
        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());

        debugIndent(std::cerr, indent+4);
        cerr << "DualEdge: {" << F1.mDebugNum << "," << F2.mDebugNum << "} ";
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," << LN2.mDebugNum 
                                                                       << "} ";
        auto& aux = mAux[ETN.utility()].mDGEdgeAux[DE.utility()];
        cerr << "mCost: " << aux.mCost << " ";
        cerr << "mLeftInset: " << aux.mLeftInset << " ";
        cerr << "mRightInset: " << aux.mRightInset << " ";
        cerr << "\n";
    }
}


void VisRepFinder::debugPrintScheduleEG(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "ScheduleEG() Block: " << B.mDebugNum << "\n";

    for (auto enit = ETN.embeddedGraph().nodes().first;
        enit != ETN.embeddedGraph().nodes().second; enit++ ) {
        auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mEGNodeAux[EN.utility()];
        cerr << "mEarlyStart: " << aux.mEarlyStart << " ";
        cerr << "\n";
    }
}


void VisRepFinder::debugPrintScheduleDG(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "ScheduleDG() Block: " << B.mDebugNum << "\n";

    for (auto fit = ETN.dualGraph().nodes().first;
        fit != ETN.dualGraph().nodes().second; fit++ ) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedFace: " << F.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mDGFaceAux[F.utility()];
        cerr << "mEarlyStart: " << aux.mEarlyStart << " ";
        cerr << "\n";
    }
}


void VisRepFinder::debugPrintAssignCoordinates(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "assignCoordinates() Block: " << B.mDebugNum << " ";
    cerr << "mExtraWidthBottom: " << mAux[ETN.utility()].mExtraWidthBottom 
                                                                        << " ";
    cerr << "mExtraHeightBottom: " << mAux[ETN.utility()].mExtraWidthBottom
                                                                        << " ";
    cerr << "mExtraWidthRight: " << mAux[ETN.utility()].mExtraWidthBottom 
                                                                        << " ";
    cerr << "mExtraHeightRight: " << mAux[ETN.utility()].mExtraWidthBottom
                                                                        << " ";
    cerr << "mXOffsetBlock: "  << mAux[ETN.utility()].mXOffsetBlock << " ";
    cerr << "mYOffsetBlock: "  << mAux[ETN.utility()].mYOffsetBlock << " ";
    cerr << "mXOffsetRight: "  << mAux[ETN.utility()].mXOffsetRight << " ";
    cerr << "mYOffsetRight: "  << mAux[ETN.utility()].mYOffsetRight << " ";
    cerr << "mXOffsetBottom: " << mAux[ETN.utility()].mXOffsetBottom << " ";
    cerr << "mYOffsetBottom: " << mAux[ETN.utility()].mYOffsetBottom << " ";
    cerr << "mHeight: "        << mAux[ETN.utility()].mHeight << " ";
    cerr << "mWidth: "         << mAux[ETN.utility()].mWidth << "\n";

    for (auto enit = ETN.embeddedGraph().nodes().first;
        enit != ETN.embeddedGraph().nodes().second; enit++ ) {
        auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mEGNodeAux[EN.utility()];
        cerr << "mY: " << aux.mY << " ";
        cerr << "\n";
    }

    for (auto eeit = ETN.embeddedGraph().edges().first;
        eeit != ETN.embeddedGraph().edges().second; eeit++ ) {
        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," << LN2.mDebugNum 
                                                                       << "} ";
        auto& aux = mAux[ETN.utility()].mEGEdgeAux[EE.utility()];
        cerr << "mX: " << aux.mX << " ";
        cerr << "\n";
    }

    for (auto fit = ETN.dualGraph().nodes().first;
        fit != ETN.dualGraph().nodes().second; fit++ ) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedFace: " << F.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mDGFaceAux[F.utility()];
        cerr << "mX: " << aux.mX << " ";
        cerr << "\n";
    }
}


void VisRepFinder::debugPrintTranslate(
    EmbeddedBCTreeNode& ETN,
    size_t              indent
) {
    auto& BN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
    auto& B   = BN.block();

    debugIndent(std::cerr, indent);
    cerr << "translate() Block: " << B.mDebugNum << " ";
    cerr << "mAnchorX1: " << mAux[ETN.utility()].mAnchorX1 <<  " ";
    cerr << "mAnchorY1: " << mAux[ETN.utility()].mAnchorY1 <<  " ";
    cerr << "mAnchorX2: " << mAux[ETN.utility()].mAnchorX2 <<  " ";
    cerr << "mAnchorY2: " << mAux[ETN.utility()].mAnchorY2 <<  " ";
    cerr << "mXOffsetBlock: " << mAux[ETN.utility()].mXOffsetBlock <<  " ";
    cerr << "mYOffsetBlock: " << mAux[ETN.utility()].mYOffsetBlock <<  " ";
    cerr << "mXOffsetRight: " << mAux[ETN.utility()].mXOffsetRight <<  " ";
    cerr << "mYOffsetRight: " << mAux[ETN.utility()].mYOffsetRight <<  " ";
    cerr << "mXOffsetBottom: " << mAux[ETN.utility()].mXOffsetBottom <<  " ";
    cerr << "mYOffsetBottom: " << mAux[ETN.utility()].mYOffsetBottom <<  " ";

    for (auto enit = ETN.embeddedGraph().nodes().first;
        enit != ETN.embeddedGraph().nodes().second; enit++ ) {
        auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
        auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedNode: " << LN.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mEGNodeAux[EN.utility()];
        cerr << "mY: " << aux.mY << " ";
        cerr << "\n";
    }

    for (auto eeit = ETN.embeddedGraph().edges().first;
        eeit != ETN.embeddedGraph().edges().second; eeit++ ) {
        auto& EE  = dynamic_cast<EmbeddedEdge&>(*(*eeit));
        auto& EN1 = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& EN2 = dynamic_cast<EmbeddedNode&>(EE.incidentNode2());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(EN2.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedEdge: {" << LN1.mDebugNum << "," << LN2.mDebugNum << "} ";
        auto& aux = mAux[ETN.utility()].mEGEdgeAux[EE.utility()];
        cerr << "mX: " << aux.mX << " ";
        cerr << "\n";
    }

    for (auto fit = ETN.dualGraph().nodes().first;
        fit != ETN.dualGraph().nodes().second; fit++ ) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        debugIndent(std::cerr, indent+4);
        cerr << "EmbeddedFace: " << F.mDebugNum << " ";
        auto& aux = mAux[ETN.utility()].mDGFaceAux[F.utility()];
        cerr << "mX: " << aux.mX << " ";
        cerr << "\n";
    }
}
#endif


}// namespace Undirected

}// namespace Wailea
