#include "undirected/embedded_bctree.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/bl_planarity_tester.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

namespace Wailea {

namespace Undirected {

/**
 * @file undirected/embedded_bctree.cpp
 */
EmbeddedBCTree::EmbeddedBCTree(BCTree& tree):mTree(&tree)
{

    vector<pair<node_list_it_t, node_ptr_t> > nodePairs;
    vector<pair<edge_list_it_t, edge_ptr_t> > edgePairs;
    auto nitPair = tree.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto& N = dynamic_cast<BCTreeNode&>(*(*nit));
        auto np = make_unique<EmbeddedBCTreeNode>(N);
        nodePairs.push_back(make_pair(nit,std::move(np)));
    }

    auto eitPair = tree.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto& E = dynamic_cast<BCTreeEdge&>(*(*eit));
        auto ep = make_unique<EmbeddedBCTreeEdge>(E);
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }

    tree.copySubgraph(nodePairs, edgePairs, *this);

    nitPair = nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto& EBCN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        auto&  BCN = dynamic_cast<BCTreeNode&>(EBCN.IGBackwardLinkRef());
        BCN.pushIGForwardLink(nit);
    }

    eitPair = edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto& EBCE = dynamic_cast<EmbeddedBCTreeEdge&>(*(*eit));
        auto&  BCE = dynamic_cast<BCTreeEdge&>(EBCE.IGBackwardLinkRef());
        BCE.pushIGForwardLink(eit);
    }
};


void EmbeddedBCTree::makeDefaultEmbedding()
{
    // temporary mapping from BlockNode to EmbeddedNode
    vector<node_list_it_t> indexedNewNodes;

    // make an embedding and a dual graph for each block.        
    for (auto nit = nodes().first; nit != nodes().second; nit++) {

        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (ETN.type() == BCTreeNode::BlockType) {

            auto& TN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
            auto& B   = TN.block();

            if (B.numNodes() > 2) {

                auto beit = B.edges().first;
                auto& BE = *(*beit);
                auto& N1 = BE.incidentNode1();
                auto& N2 = BE.incidentNode2();

                vector<node_list_it_t> stNumbering;

                STNumbering stMaker;
                stNumbering = stMaker.getBipolarOrientation(B, N1, N2);

                BLPlanarityTester blMaker;
                blMaker.findEmbedding(B, stNumbering);

            }

            PlanarDualGraphMaker dualMaker;
            dualMaker.makeDualGraph(B, ETN.embeddedGraph(), ETN.dualGraph());

            // Link Block and EmbeddedGraph
            auto enPair = ETN.embeddedGraph().nodes();
            for (auto enit = enPair.first; enit != enPair.second; enit++) {
                                      
                auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
                auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());

                BN.pushUtility(indexedNewNodes.size());
                indexedNewNodes.push_back(enit);
            }
        }
    }


    // make a default unification face group for each cut vertex.
    for (auto nit = nodes().first; nit != nodes().second; nit++) {

        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (ETN.type() == BCTreeNode::CutVertexType) {                
            UnificationGroup UG;
            auto ePair = ETN.incidentEdges();
            for (auto eit = ePair.first; eit != ePair.second; eit++) { 
                auto& ETE      = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(*eit)));
                auto& TE       = dynamic_cast<BCTreeEdge&>(
                                                      ETE.IGBackwardLinkRef());
                auto& BN       = TE.cutVertexInBlock();
                auto& ETNblock = dynamic_cast<EmbeddedBCTreeNode&>(
                                                        ETE.adjacentNode(ETN));
                auto& CV = dynamic_cast<EmbeddedNode&>(
                                           *(*(indexedNewNodes[BN.utility()])));
                edge_list_it_t eCWIt, eCCWIt;
                auto fit = findBiggestFace(CV, eCWIt, eCCWIt);
                UnificationFace uf(
                           ETNblock.backIt(), fit, CV.backIt(), eCWIt, eCCWIt);
                UG.push(std::move(uf));
            }
            ETN.pushBackUnificationGroup(std::move(UG));
        }
    }

    // Pop the utility values used above.
    for (auto nit = nodes().first; nit != nodes().second; nit++) {

        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (ETN.type() == BCTreeNode::CutVertexType) {                
            UnificationGroup UG;
            auto ePair = ETN.incidentEdges();
            for (auto eit = ePair.first; eit != ePair.second; eit++) { 
                auto& ETE      = dynamic_cast<EmbeddedBCTreeEdge&>(*(*(*eit)));
                auto& TE       = dynamic_cast<BCTreeEdge&>(
                                                      ETE.IGBackwardLinkRef());
                auto& BN       = TE.cutVertexInBlock();
                BN.popUtility();
            }
        }
    }
}


/** @brief finds a biggest face incident to the given EmbeddedNode
 *  
 *  @param TN      (in): EmbeddedNode.
 *
 *  @param eCWIt  (out): EmbeddedEdge incident to TN and the face.
 *                       CW-side.
 *  @param eCCWIt (out): EmbeddedEdge incident to TN and the face.
 *                       CCW-side.
 *
 *  @return the biggest face found.
 */
node_list_it_t EmbeddedBCTree::findBiggestFace(
    EmbeddedNode&   TN,
    edge_list_it_t& eCWIt,
    edge_list_it_t& eCCWIt
) {
    auto ePair  = TN.incidentEdges();
    long maxDeg = 0;
    node_list_it_t maxIt;

    for (auto eit = ePair.first; eit != ePair.second; eit++) {

        auto& EE = dynamic_cast<EmbeddedEdge&>(*(*(*eit)));
        auto& DE = dynamic_cast<DualEdge&>(*(*(EE.mDualEdge)));

        auto& F1 = dynamic_cast<EmbeddedFace&>(DE.incidentNode1());
        auto& F2 = dynamic_cast<EmbeddedFace&>(DE.incidentNode2());

        if (maxDeg < F1.degree()) {
            maxDeg = F1.degree();
            maxIt  = F1.backIt();

            if (EE.mHalfEdge1.mEmbeddedFace == F1.backIt()) {
                if (EE.mHalfEdge1.mSrcNode == TN.backIt()) {
                    //    TN ========================
                    //         /|               /|
                    //          |        he1.prev|
                    //        EE|                |
                    //          | he1            |
                    //  DE - - -|- - -  F1 - - - + - - -
                    //          |                |
                    //          |                |
                    //          |/               |/
                    //         CW               CCW

                    eCCWIt = EE.mHalfEdge1.mPrevEdge;
                    eCWIt  = *eit;

                }
                else {// EE.mHalfEdge1.mDstNode == TN.backIt()
                    //    TN ========================
                    //         /|               /|
                    //          |he1.next     he1|
                    //          |                |EE
                    //          |                |
                    //     - - -|- - -  F1 - - - + - - - DE
                    //          |                |
                    //          |                |
                    //          |/               |/
                    //         CW               CCW

                    eCCWIt = *eit;
                    eCWIt  = EE.mHalfEdge1.mNextEdge;
                }
            }
            else {
                if (EE.mHalfEdge2.mSrcNode == TN.backIt()) {
                    eCCWIt = EE.mHalfEdge2.mPrevEdge;
                    eCWIt  = *eit;
                }
                else {
                    eCCWIt = *eit;
                    eCWIt  = EE.mHalfEdge2.mNextEdge;
                }
            }
        }

        if (maxDeg < F2.degree()) {
            maxDeg = F2.degree();
            maxIt  = F2.backIt();
            if (EE.mHalfEdge1.mEmbeddedFace == F2.backIt()) {
                if (EE.mHalfEdge1.mSrcNode == TN.backIt()) {
                    eCCWIt = EE.mHalfEdge1.mPrevEdge;
                    eCWIt  = *eit;
                }
                else {
                    eCCWIt = *eit;
                    eCWIt  = EE.mHalfEdge1.mNextEdge;
                }
            }
            else {
                if (EE.mHalfEdge2.mSrcNode == TN.backIt()) {
                    eCCWIt = EE.mHalfEdge2.mPrevEdge;
                    eCWIt  = *eit;
                }
                else {
                    eCCWIt = *eit;
                    eCWIt  = EE.mHalfEdge2.mNextEdge;
                }
            }
        }
    }

    return maxIt;
}


void EmbeddedBCTree::findGeometricEmbedding(
    node_list_it_t rootBlock,
    node_list_it_t rootOuterFace,
    node_list_it_t rootTopNode
){
    mRoot = rootBlock;

    if (rootBlock==nodes().second){
        return;
    }

    vector<node_list_it_t> ExpNodes;

    // Construct ExplorationNodes and put them under ExpNodes.
    // so that they are addressable from EmbeddedBCTreeNodes.
    for (auto nit = nodes().first; nit != nodes().second; nit++) {

        auto& TN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));

        if (TN.type() == BCTreeNode::BlockType){
            auto  expNp = make_unique<ExplorationNode>(TN);
            auto& expN  = dynamic_cast<ExplorationNode&>(
                                           mExpTree.addNode(std::move(expNp)));
            TN.pushUtility(ExpNodes.size());
            ExpNodes.push_back(expN.backIt());

        }
    }
    for (auto nit = nodes().first; nit != nodes().second; nit++) {
        auto& TN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (TN.type() == BCTreeNode::CutVertexType) {

            // Make back reference to unification face.

            for (size_t ugi = 0; ugi < TN.numUnificationGroups(); ugi++) {
                auto& UG = TN.unificationGroup(ugi);
                for (size_t ufi = 0; ufi < UG.size(); ufi++) {
                    auto& UF      = UG.at(ufi);
                    auto& TNblock = UF.treeNode();
                    UnificationFaceRef FR(TN, ugi, ufi);
                    UF.setBackIt(TNblock.mUnexploredFaces.insert(
                               TNblock.mUnexploredFaces.end(), std::move(FR)));
                }
            }

        }
    }

    // BFS queue
    list<node_list_it_t> Q;
    size_t Qsize = 0; // Separate size variable needed as Q.size()
                      // takes O(|Q|) in some implmentations.

    Q.push_back(rootBlock);
    Qsize++;

    auto& ETNR = dynamic_cast<EmbeddedBCTreeNode&>(*(*rootBlock));
    ETNR.setOrientation(rootOuterFace, rootTopNode);
    mExpTree.setRoot(ExpNodes[ETNR.utility()]);

    //       ETN(block)                     EXPN1
    //       +--------------------+          +-+
    //       |                    |..........| |
    //       |                    |          +-+
    //       |mUnexploredEdges    |           |
    //       | [          *    ]  |           |
    //       +-----------/--------+           |
    //        |         /                     |
    //        |        /                      |
    //        |       /                       |
    //     ETE|      |                        |
    //        |      |                        |
    //        |      |                        |
    //       +-------+------+                 |
    //       |       |  UGs |                 |
    //       |    +--+-----+|                 |
    //       |    |  |face ||                 |
    //       |    | \|/    ||                 |
    //   ETCV|  UG|==*==*==||mUnificationFaces|
    //       |    |     |  ||                 |
    //       |    |backIt  ||                 |
    //       |    |     |  ||                 |
    //       |    +-----+--+|                 |
    //       |          |   |                 |
    //       +----------|---+                 |
    //        |         |                     |
    //        |        /                      |
    //        |       /                       |
    //        |      /                        |
    //        |     /erase()                 \|/
    //       +-+ <--mUnexploredEdges         +-+
    //       | |.............................| |
    //       +-+                             +-+
    //     ChildTN(block)                   EXPN2
    while (Qsize > 0) {
        auto nit = *(Q.begin());
        Q.pop_front();
        Qsize--;

        auto& ETN    = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));

        auto& EXPN1  = dynamic_cast<ExplorationNode&>(
                                                *(*(ExpNodes[ETN.utility()])));

        auto& UFrefs = ETN.unexploredFaces();

        for (auto refIt = UFrefs.begin(); refIt != UFrefs.end();refIt++) {
            auto& UFref = *refIt;

            auto& UG = UFref.group();

            SortedUnificationGroup SUG(UG, UFref.faceIndex());

            for (size_t ufi = 0; ufi < UG.size(); ufi++) {
                if (ufi != UFref.faceIndex()) {
                    auto&UF = UG.at(ufi);

                    UF.markExplored();

                    UF.setOrientation();

                    Q.push_back(UF.treeNode().backIt());

                    Qsize++;

                    auto& EXPN2 = dynamic_cast<ExplorationNode&>(
                                       *(*(ExpNodes[UF.treeNode().utility()])));

                    auto  expEp = make_unique<ExplorationEdge>();

                    mExpTree.addEdge(std::move(expEp), EXPN1, EXPN2);

                }

            }

            EXPN1.mSortedUnificationGroups.push_back(std::move(SUG));

        }

    }

    // Pop utility value from the nodes
    for (auto nit = nodes().first; nit != nodes().second; nit++) {
        auto& TN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (TN.type() == BCTreeNode::BlockType){
            TN.popUtility();
        }
    }

    pushDefaultIGForwardLinkToNodes();
    auto nitPair = mExpTree.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto& expN  = dynamic_cast<ExplorationNode&>(*(*nit));
        auto& EBCTN = dynamic_cast<EmbeddedBCTreeNode&>(
                                                     expN.IGBackwardLinkRef());
        EBCTN.setIGForwardLink(nit);
    }

}

    
void EmbeddedBCTree::findGeometricEmbedding(
    node_list_it_t rootBlock, 
    node_list_it_t rootOuterFace
) {
    mRoot = rootBlock;

    if (rootBlock==nodes().second){
        return;
    }

    auto& N = dynamic_cast<EmbeddedBCTreeNode&>(*(*rootBlock));
    if ( N.dualGraph().numEdges() == 0) {
        findGeometricEmbedding(
                 rootBlock, rootOuterFace, N.embeddedGraph().nodes().first);
    }
    else {
        auto& F    = dynamic_cast<EmbeddedFace&>(*(*rootOuterFace));
        /* pick the first dual edge */
        auto& DE   = dynamic_cast<DualEdge&>(*(*(*(F.incidentEdges().first))));
        auto& EE   = dynamic_cast<EmbeddedEdge&>(*(*(DE.mEmbeddedEdge)));
        auto& EN1  = dynamic_cast<EmbeddedNode&>(EE.incidentNode1());
        auto& BN1 = dynamic_cast<BlockNode&>(EN1.IGBackwardLinkRef());
        if (BN1.type()==BlockNode::OrdinaryNodeType) {
            /* pick node 1 as the top node */
            findGeometricEmbedding(
                     rootBlock, rootOuterFace, EE.incidentNode1().backIt());
        }
        else {
            findGeometricEmbedding(
                     rootBlock, rootOuterFace, EE.incidentNode2().backIt());
        }
    }
}


void EmbeddedBCTree::findGeometricEmbedding(
    node_list_it_t rootBlock
) {

    mRoot = rootBlock;

    /* pick the largest face as the outer face of the block */
    if (rootBlock==nodes().second){
        return;
    }

    auto& N = dynamic_cast<EmbeddedBCTreeNode&>(*(*rootBlock));

    if ( N.dualGraph().numNodes() == 0) {

        auto  expNp = make_unique<ExplorationNode>(N);
        auto& expN  = dynamic_cast<ExplorationNode&>(
                                           mExpTree.addNode(std::move(expNp)));
        mExpTree.setRoot(expN.backIt());
        return;
    }
    // Pick the face with the most non-cut-vertex incident nodes.
    auto nPair = N.dualGraph().nodes();
   
    node_list_it_t maxIt;
    size_t         maxNum = 0;
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*nit));

        size_t numOrdinaryNodes=0;
        auto cycleBit = F.mCycleHalfEdgesOn1.begin();
        for (auto cycleFit = F.mCycleEdges.begin();
                     cycleFit != F.mCycleEdges.end(); cycleFit++, cycleBit++) {
            auto& EE = dynamic_cast<EmbeddedEdge&>(*(*(*cycleFit)));
            auto enit = (*cycleBit)?
                                EE.mHalfEdge1.mSrcNode:EE.mHalfEdge2.mSrcNode;
            auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
            auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
            if (BN.type()==BlockNode::OrdinaryNodeType) {
                numOrdinaryNodes++;
            }
        }
        if (maxNum <= numOrdinaryNodes) {
            maxNum = numOrdinaryNodes;
            maxIt = nit;
        }
    }
    findGeometricEmbedding(rootBlock, maxIt);
}


void EmbeddedBCTree::findGeometricEmbedding()
{

    if (numNodes()==0) {
        mRoot = nodes().second;
        return;
    }

    /** pick the largest block as the root block
     *
     *  Cost = 3|V| + |E|   
     */
    node_list_it_t maxIt  = nodes().second;
    size_t         maxNum = 0;

    auto nPair = nodes();

    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& N = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));

        if (N.type() == BCTreeNode::BlockType) {

            auto& BN = dynamic_cast<BCTreeNode&>(N.IGBackwardLinkRef());
            auto& B  = BN.block();

            size_t cost = B.numNodes()*3 + B.numEdges();

            if (maxNum < cost) {

                maxNum = cost;
                maxIt = nit;
            }

        }

    }        
    findGeometricEmbedding(maxIt);
}

}// namespace Undirected

}// namespace Wailea

