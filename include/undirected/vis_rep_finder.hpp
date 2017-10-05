#ifndef _WAILEA_UNDIRECTED_VIS_REP_FINDER_HPP_
#define _WAILEA_UNDIRECTED_VIS_REP_FINDER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/planar_dual_graph_maker.hpp"
#include "undirected/embedded_bctree.hpp"
#include "undirected/planarizable_graph.hpp"


#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


namespace Wailea {

namespace Undirected {

/**
 * @file undirected/vis_rep_finder.hpp
 *
 * @brief finds a visibility representation for a planar VRRectLabelGraph.
 *
 * @remark
 *
 * This file handles visibility representation for connected graphs.
 * not just for biconnected graphs.
 * Visibility representation can be considered the closest representation
 * to an actual drawing.
 * Visibility representation was originally developed to express a drawing of
 * a biconnectd graph on a 2 dimensioal plane with a designated
 * the outer face, the top node,  and the ordering of the incident edges around
 * each vertex fixed out of two orientations (clockwise /counter-clockwise).
 * Most of the algorithms to generate a visiblity representation take a
 * biconnected component with its combinatorial embedding as their inputs, and
 * most of the algorithms to find a planar embedding take a biconnected graph.
 * as their inputs.
 * On the other hand, any combinatorial embedding of a tree always gives a
 * planar embedding, and there is no need to find a planar embedding by an 
 * algorithm. The embedding of a tree is usually given externaly to represent
 * some semantics of the data structure that uses the tree.
 *
 * The class VisRepVinder and its accompanying classes are an attempt to expand
 * the visibility representation to connected graphs.
 * A connected graph can be decomposed into block-cut-tree, or BC-tree.
 * Each node of a BC-tree is either a cut vertex or a block.
 * As stated above, a combinatorial embedding is usually found by some 
 * planarity test algorithm for bi-connected components.
 * On the other hand, a combinatorial embedding of a tree is assumed to be 
 * given in the ordering of incident edges lists. They are considered to be
 * given externally, rather than to be found by some algorithms.
 * Here we assume the combinatorial embedding of the connected graph is 
 * represented by EmbeddedBCTree that has an internal embedding for each block,
 * and a data structure called FaceUnificationGroup for cut vertices.
 * Moreover, the outer/inner classification has been already done in
 * EmbedddBCTree. Please see embedded_bctree.hpp for details.
 *
 * =========================================================================
 * An existing standard algorithm to find a visibility representation for a 
 * biconnected component [TT86]
 * =========================================================================
 *
 * We first explain the visibility representation algorithm for biconnected
 * component. Our algorithm is based on it.
 *
 * Finding a visibility representation of a biconnected graph is usually done
 * in the following steps [TT86].
 * Input: Embedding with its dual graph, top node, and the outer face.
 * 1. Let s be the top node.
 * 2. Let t be the node incident to s and the outer face s* where s is the 
 *    destination node,  and t is the source node of the half edge of {s,t}
 *    according to the s*.
 *    Please note that around each inner face, the incident half edges cycle
 *    around it counter-clockwise as follows.
 *   
 * We denote the face opposite to the outer face across {s,t} by 'right face'
 * t*.
 * We also denote the outer face by 's*', and the right face by 't*'.
 *
 * Ex.1) biconnected graph not k2.
 *
 *  (top node) s ==================================
 *               /|               /|            /|
 *                |                |eCCW         |
 *           {s,t}|  eCW           |/            |
 *                |               === v1         |/  outer face
 *          (s*)--+---> t*        /|            ===      s*
 *     outer face |  right face    |/           /|
 *                |               === v2         .
 *                |                .             .
 *                |                .             .
 *                |                .             |
 *                |/               |/            |/
 *(bottom node)t ===================================
 *
 *
 * Ex.2) k2 (minimum biconnected component)
 *
 *    (top node) s ===
 *                 /|
 *      outer face  | outer face
 *                  |
 *             s* --+---> t* = s* (self loop)
 *                  |
 *                  |
 *                  |/
 *  (bottom node)t ===
 *
 * 3. Find (s,t)-ordering.
 *    For the reason described later, we find (t,s)-ordering with the 't'
 *    at the rank 0 upward to 's', which has the highest rank.
 *    Magically, (t,s)-ordering does not violate the constraints
 *    in the planar embedding in the following sense;
 *
 *    Lemma 1 [TT86]: Each face of the embeding consists of two directed paths
 *                  induced by the (s,t)-ordering
 *    Lemma 2 [TT86]: All outgoing (ingoing) arcs of any vertex v of the
 *                  embedding appear consecutively around v.
 * 
 * 4. Orient each edge of the embedding according to the (t,s)-ordering.
 *
 * 5. Orient each dual edge such that its corresponding graph edge
 *    cuts across upward if we place the source and the destination nodes
 *    of the dual edge left and right respectively.
 *
 *             edge
 *             /|\
 *              |
 *          ----+---> Dual edge
 *              |
 *              |
 *
 * 7. Use a scheduling algorithm (PERT critical path, DFS-based, network
 *    simplex etc.) that takes both node and edge costs to assign ranks to the 
 *    vertices. In a standard algorithm, only the edge costs are used. The 
 *    value is fixed to 1 or 2.
 *
 * 8. Remove {s*,t*} from the dual graph.
 *
 * 9. Use a scheduling algorithm that takes both node and edge cost to assign
 *    ranks to the faces on the dual graph.
 *
 * 10. For each edge e, let x(e) be a value between the ranks of two incident 
 *     face.
 *
 * 11. For each vertex v, let LeftX(v) be min{y(e)} and RightX(v) be max 
 *     among incident edges.
 *
 * Eventually, t has the least y value , s has the greatest y value among
 * the nodes. In the same way, t* has the least x value and s* has the
 * greatest x value among the faces.
 *
 * =======================================================
 * Making a visibility representation of a connected graph
 * from an EmbeddedBCTree
 * =======================================================
 * In our approach, a visibility representation is made by assembling
 * visibility representations of the blocks utilizing the algorithm
 * for biconnected components described above.
 * The algorithm takes a top node and an outer face as part of its input.
 * Hence we have to first determine the outer face and the top node for 
 * each block in the bc-tree.
 * Also, the blocks incident to the same cutvertex must be joined at the
 * cut vertex in the embedding. A set of blocks to be joined at a cut vertex
 * is defined by 'face unification group', in which each participating block 
 * nominates an incident face to be unified with others. Please note that a cut
 * vertex can have  multiple face unification groups.
 * When determining an outer face of each block, we have to take the following
 * constraint into consideration: at each face unification group, at most
 * one face can be inner face, and the rest must be outer.
 * This relationship among the blocks at each face unification block is
 * called absorbor/absorbees relationship into 'sorted face unification 
 * group'.
 * This is done in EmbeddedBCTree::findGeometricEmbedding(), which gives a
 * rooted directed tree EmbeddedBCTree::ExplorationTree.
 * EmbeddedBCTree::ExplorationTree takes ExplorationNode and ExplorationEdge
 * as its nodes and edges. The former represents one block, but the latter
 * does not represent any edge in EmbeddedBCTree but to represent a parent-
 * child relationship between two blocks.
 * each ExplorationNode has FaceUnificationGroups. The parent-child 
 * relationships represent absorber-absorbee relationship among the blocks,
 * and FaceUnificationGroups represent how the faces of the absorbee blocks
 * are unified to the absorbers'.
 *
 * Generation of visibility representation for 
 * EmbeddedBCTRee::findGeometricEmbedding() is done by VisRepFinder defined in
 * this file in two tree traversals on ExplorationTree: One in bottom-up and
 * the other top-down.
 * In the bottom-up phase, at each block node, visibility representation for
 * the block is found based on the standard visibility representaton algorithm
 * for biconnected components, but some extensions to utilize the node and 
 * edge costs.
 * The block may have some child blocks with absorbee faces into the block.
 * We accommodate those child blocks in the absorber faces at the cut vertices.
 * In order to do that, we first allocate enough space for those blocks.
 * How to allocate those spaces will be described below.
 * We also have to accommodate enough space for vertex and edge labels.
 * This will be described later.
 *
 * In the top-down phase, we assign the global coordinates to all the 
 * descendant visibility representations from the root.
 *
 * ==========================================================
 * How to accommodate absorbee visibility representations at
 * each cut vertex for a sorted unification group.
 * ==========================================================
 * Here we denote the relevant edges by cw and ccw, which 
 * mean clockwise and counter-clockwise respectively.
 * Those are relative to the face unification group, not 
 * to the ordering among the half edges of the embedding of the block.
 *
 * 1. Absorber is inner face/top node
 *
 *               Cut vertex
 *     ======+=========+=============+=====
 *      |    |         |             |   |
 *      |  +-+-+  +----+----+      +-+-+ |
 *      |  |VR1|  |         | ...  |VRy| |
 *      |  +---+  |   VR2   |      |   | |
 *      | cw  ccw |         |      +---+ |
 *      |         +---------+     cw  ccw|
 *      |        cw        ccw           |
 *     CCW                               CW
 *
 *
 * 2. Absorber is outer face/top node
 *
 *               Cut vertex
 *     ===========+=========+=============+==
 *      |    |    |         |             |
 *      |    |  +-+-+  +----+----+      +-+-+
 *      |    |  |VR1|  |         | ...  |VRy|
 *      |    |  +---+  |   VR2   |      |   |
 *      |    | cw ccw  |         |      +---+
 *      |    |         +---------+     cw  ccw
 *      |    |        cw        ccw
 *     CW   CCW
 *  
 * 3. Absorber is inner face/bottom node
 *
 *     CW                              CCW
 *      |           ccw        cw       |
 *      | ccw cw     +---------+        |
 *      |  +---+     |         |ccw  cw |
 *      |  |   |     |   VR2   | +---+  |
 *      |  |VRy| ... |         | |VR1|  |
 *      |  +-+-+     +----+----+ +-+-+  |
 *      |    |            |        |    |
 *     ======+============+========+======
 *                Cut vertex
 *
 * 4. Absorber is outer face/bottom node
 *
 *     CCW                         CW
 *      |                           |
 *      |                           |
 *      |       Cut vertex          |
 *     ===+=========+=============+===
 *        |         |             |
 *      +-+-+  +----+----+      +-+-+
 *      |VR1|  |         | ...  |VRy|
 *      +---+  |   VR2   |      |   |
 *     cw  ccw |         |      +---+
 *             +---------+     cw  ccw
 *            cw        ccw
 *
 * 5. Absorber is outer or inner face/left node
 *
 *     CW  ccw        cw
 *      |   +---------+      ccw      cw
 *      |   |         |       +-------+
 *      |   |  VRUx   | ...   | VRU1  |
 *      |   |         |       |       |
 *      |   +----+----+       +---+---+
 *      |        |                |
 *  CV =========++===============++====
 *      |       |                |
 *      |    +--+--+         +---+---+
 *      |    | VRL1|   ....  | VRLx  |
 *      |    |     |         +-------+
 *      |    +-----+        cw      ccw
 *     CCW  cw   ccw
 *     
 *
 * 6. Absorber is outer or inner face/right node
 *        ccw       cw                  CCW
 *         +---------+      ccw      cw  |
 *         |         |       +-------+   |
 *         |  VRU1   | ...   | VRUx  |   |
 *         |         |       |       |   |
 *         +---------+       +-------+   |
 *              |               |        |
 *          ===============================  CV
 *             |                 |        |
 *          +--+--+          +---+---+    |
 *          |VRL2 |   ....   | VRL1  |    |
 *          |     |          +-------+    |
 *          +-----+         cw      ccw   |
 *         cw    ccw                      CW 
 *
 *
 *
 * ==========================================================
 * NOTES on the coordinate systems and the orientations used 
 * ==========================================================
 *
 * 1. Circular orientations of the incident half edges around each face in an 
 *    embedding of a block.
 *
 *    The incident half edges of each inner face is asssumed to be in counter-
 *    clockwise as follows.
 *    
 *    v3            v2
 *     *------------*       --------->           v1             e3 
 *     |  <-------  |    /|\*--------* |          *         e4\ | /e2
 *     | | inner /|\|     | |v2    v3| |       /|\| |          \|/
 *     | |  face  | |     | |        | |face    | | |      e5---V---e1
 *     |\|/       | |     | |v1    v4| |        | | |          /|\
 *     |  ------->  |     | *--------*\|/       | |\|/      e6/ | \e8
 *     *------------*       <---------            *             e7
 *    v4            v1                           v2
 *          
 *       Inner face         Outer face           K2      counter-clockwise
 *                                                          ordering of
 *                                                        incident edges
 *
 * 2. Circular orientations of the incident edges around each cut vertex
 *    in the sorted unification group.
 *    A sorted unification group unifies the faces in the differrent block
 *    at the same cut vertex.
 *    A sorted unification group has one absorber and the rest are absorbees.
 *
 *    The ordering of the unification faces in the absorbee list is in the 
 *    counter-clockwise ordering. 
 *
 *             =======================...========================= CV
 *             |      |    f1    |             |   fN    |       |
 *             | absorbee1 absorbee1     absorbeeN   absorbeeN   |
 *             |  edge CW  edge CCW         edge CW   edge CCW   |
 *             |   ------------------------------------------>   |  
 *             |     Absorbees arranged in counter-clockwise     |
 *             |                around the CW.                   |
 *             |                                                 |
 *         absorber              absorber face                absorber
 *          edge CCW                                            edge CW
 * 
 *    Please note UnificationFace::edgeCWInEG() and edgeCCWInEG() are 
 *    the edges on the clockwise and counter-clockwise side respectively
 *    around the cut vertex in the unification group, which may not be
 *    necessarily aligned with the embedding of the block.
 *
 *    Ex1.) An absorbee block joining in the ordinary (unflipped)
            embedding at CV.
 *
 *               CV ....=============....
 *                   /| |           | |
 *                    | |           | | (outer) face
 *                    | |           | | to be unified
 *                    | |           | |
 *                    | |           | |/
 *                  edgeCWInEG()  edgeCCWInEG()         
 *                
 *
 *    Ex2.) An absorbee block joining in the reversed (flipped) 
 *          embedding at CV.
 *
 *               CV ....=============....
 *                    | |           | |\
 *                    | |           | | (outer) face
 *                    | |           | | to be unified
 *                    | |           | |
 *                   \| |           | |
 *                  edgeCWInEG()  edgeCCWInEG()         
 *
 *                    Embedding of the block is flipped.
 *
 * 3. Ordering of the nodes in the embedding of each block and the induced
 *    orientation of the edges.
 *
 *      top node    S   (+) Highest value
 *
 *                 /|\
 *                  |
 *                  | edge orientation is upward.
 *                  |
 *                  |
 *                  |
 *
 *      bottom node T   (-) Lowest value
 *
 * 4. Ordering of the faces in the dual graph of each block and the induced
 *    orientation of the dual edges.
 *
 *    The embedding of the block is normal (unflipped) in the sense that
 *    the half edges around each inner face is in counter-clockwise order.
 *
 *           N2
 *          /|\
 *           |
 *         | | |\ half edge
 *         | | |
 *       --+-+-+-> Right-ward orientation of the dual
 *         | | |   edge in the normal (unflipped) embedding.
 *   half \| | |
 *   edge    |
 *           |
 *           N1
 *   Upward orientation 
 *      of the edge
 *
 *
 * 5. Geometric coordinate system used to describe the node and edge label 
 *    placement and dimensions.
 *
 *    The labels are specified as rectangles with widths and heights.
 *    The widths and heights in real numbers.
 *
 *    The placement of the edge labels are specified in one of three positions:
 *    center, couter-clockwise, and clockwise.  This is decided as follows.
 *    If you look at N2 from N1, the right hand side is clockwise.
 *
 *                 N2
 *                /|\
 *       couneter- |
 *       clockwise | clockwise
 *                 |
 *                 N1
 *
 * 6. Local geometric coordinate system for each block to place the nodes and
 *    the edges and to allocate necessary spaces for child blocks.
 *
 *    The orientation of X and Y axes are aligned with the 
 *    normal (unflipped) embedding of the block
 *
 *           higher positive
 *                 /|\      .(+100.0, +100.0)
 *                  |
 *     (left)       |    (right)
 *     negative     |   positive
 *     X-axis ------+------>
 *         (0.0,0.0)|
 *                  |
 *                  |
 *       .(-100.0, -100.0)
 *           lower negative
 *
 *
 * 7. Global geometric coordinate system in which the resultant visibility
 *    representation is given.
 *    
 *    The orientation of X and Y axes are aligned with the 
 *    normal (unflipped) embedding of the root block.
 *    The embeddings of the descendent blocks may be rotated
 *    , vertically flipped, or horizontally flipped.
 *
 *           higher positive
 *                 /|\      .(+100.0, +100.0)/|\
 *                  |
 *     (left)       |    (right)
 *     negative     |   positive
 *     X-axis ------+------>
 *         (0.0,0.0)|
 *                  |
 *                  |
 *       .(-100.0, -100.0)
 *           lower negative
 *
 * =======================================
 * How to allocate space for child blocks
 * =======================================
 * We augument the basic algorithm above to include handling of the child 
 * visibility representations at the cut vertices in the UF groups.
 * Assume an absorber face fx for a cut vertex of the current block is 
 * incident to two half edges he1, and he2.
 * And the edges denoted by CW and CCW mean the edge clockwise side, and 
 * counter-clockwise side respectively with respect to the circular ordering
 * around the cut vertex in the unification group, which does not necessary
 * mean the circular ordering of the incident edges around the face fx.
 * In the following discussion we assume the orientation of the absorber block
 * is fixed as its top cut vertex node positioned at the top (higher y), and
 * each inner face is oriented counter-clockwise.
 * 
 * Finding an optimum arrangement in terms of the aspect ratio and/or area
 * is a combinatorial optimization problem, which probably belongs to
 * NP-complete (TSP or 3-SAT). Here we simply give up optimality, but try to
 * find a good enough arrangement in terms of the aspect ratio and the area
 *
 * Each inner face is split into two horizontal halves: Left pane and the right
 * pane. The left pane is incident to the left path of edges from the top node
 * to the bottom. The right pane is to the right path in the same way. Each
 * pane is vertically split into sub-panes at the incident nodes. Each
 * sub-pane is incident to an edge.
 *
 * Each sub-pane consists of several areas.
 *
 * NOTE: Here the inner face is oriented such that the edges of the graph are
 *   oriented upward according to the ts-numbering, and the face oriented
 *   counte-clockwise in the embedding.
 *
 * Ex. Inner face, Top node.
 *
 *                              Left Pane of F2
 *         Right Pane of F1    <----------------> Right Pane of F2
 *      <--------------------->                 <--------------->
 *                                      <---------------> W7
 *                                      <------->W8
 *                                              <-------> W9
 *                             <-----> W10
 *                         <------> W11
 *                    <-------> W12
 *                             <-----> W13
 *                    <-------> W14
 *    ...----------> W15
 *      <----------> W16
 *                 <--> HG           <--> HG            <--> HG
 *                                      <-----> W17
 *                              <---------> W18
 *        <--------> W19
 *      <---------------------> W20
 *                             <----------------> W21
 *      <---------------------------------------> W22
 *
 *     X1                     X31              X2
 *      |                     ||                |                 H23 H24 H28
 * Y4 -------------+-----------------+--+---------------+---------- + +   +
 *     N2          |          E1        |               |      E2   | |H27| H30
 * Y3 -------------|--+-------++-----+--|---------------|--+----++- + | + |  +
 *      |          |  |       ||     |  |       |       |  |    ||    | | |  |
 *                 |  |       ||     |  |               |  |    ||    | | |  |
 *      |          |  |       ++-----+  |       |       |  |    ||    | | |  |
 *                 |  |       ||     |  |               |  |    ||    | | |  |
 *      |----------+  |    +--++--+  |  |       |       |  |    ||    | | +  |
 *                    |    |  ||  |  |  |               |  |    ||    | |    |
 *    D1|=============|====|==||==|=====|=======|       |  |    ||    | | VG |
 *                    |    |  ||  |  |  +---------------+  |    ||    + | +  |
 *     F1             |    |  ||  |  |         F2          |    ||      | |  |
 *                    |    +--++--+  |  +-----+            |    ||    + | +  |
 *      | +--------+  |       ||     |  |     | |          |    ||    | | +  |
 *        |        |  +-------++     |  |     |            |    ||    | | |  |
 *      | |        |  |       ||     |  |     | |          |    ||    | | |  |
 *  Y6    |--------|--+-------++-----+--|-----|            |    ||  + | + |  +
 *      | |        |      N1  E1        |     | |          |    ||  | |   |
 *  Y7    +--------+---++----+--+-------+-+---+            |    ||  + +   +
 *      |              ||    |  |         |     |          |    ||H25 H26 H29
 *                     ||    |  |         |   D2 ==========|====||=
 *                     ||    |  |         |
 *                     E3    |  +---------+
 *
 *
 * D1:  Dual of E1
 * D2:  Dual of E2
 * HG:  Horizontal gap between two features.
 *                              HG :=VRRectLabelGraph::horizontalGap()
 * VG:  Vertical gap between two features.
 *                              HG :=VRRectLabelGraph::verticalGap()
 * X1:  center of the left face F1     X1 := DGFaceAux::mEarlyStart
 *                                           DGFaceAux::mX
 * X2:  center of the right face F2    X2 := DGFaceAux::mEarlyStart
 *                                           DGFaceAux::mX
 * Y3:  lower edge of the node N2      DGNodeAux::mEarlyStart
 *      In this case, N1 is the top node of the block.
 * Y4:  upper edge of the node N2      EGNodeAux::mEarlyStart + 
 *                                     EGNodeAux::mCost
 *      DGNodeAux::mCost := VRRectLabelNode::mHeight
 *
 * Y5:  lower edge of the node N1      EGNodeAux::mEarlyStart
 *      In this case, N1 is the top node of the block.
 *
 * Y6:  upper edge of the node N1      EGNodeAux::mEarlyStart + 
 *                                     EGNodeAux::mCost
 *      DGNodeAux::mCost := VRRectLabelNode::mHeight
 *
 * W7:  Width required to accommodate the absorbee VRs under N2
 *      between E1 and E2. The width is calculated as follows.
 *
 *     <-----------   W7   ----------->
 *     |W_vr1|HG|W_vr2|HG|...|HG|W_vrN|
 *
 *       HG:   horizontal gap. G :=VRRectLabelGraph::horizontalGap()
 *       W_vri: Width of the absorbee VRi.
 * 
 *       W7 = SUM{W_vri} + (N-1)*G
 *
 * W8:  F2's Left pane's share of W7. It's w7/2.0.
 *                           W8 := DGEdgeAux::mChildrenWidthUpperRight for D1.
 * W9:  F2's Right pane's share of W7. It's w7/2.0.
 *                           W9 := DGEdgeAux::mChildrenWidthUpperLeft for D2.
 * W10: Width of the label edge of E1 on the N2 side.
 *                          W10 :=  VRRectLabelEdge::widthNode2()
 *      It is assumed that the label is placed on F2 side. 
 *      i.e., VRRectLabelEdge::typeNode2() == POS_CLOCKWISE.
 *
 * W11: Width of the label edge of E1 in the middle of the edge.
 *      It is assumed that the label is placed on the center of E1.
 *      i.e., VRRectLabelEdge::typeNode2() == POS_CENTER.
 *                          W11 :=  VRRectLabelEdge::widthMiddle()
 *
 * W12: Width of the label edge of E1 on the N1 side.
 *                          W12 :=  VRRectLabelEdge::widthNode1()
 *      It is assumed that the label is placed on F1 side. 
 *      i.e., VRRectLabelEdge::typeNode1() == POS_COUNTER_CLOCKWISE.
 *
 * W13: Horizontal extent of the label edges of E1 on the right hand side,
 *      i.e., F2 side.
 *      W13 := DGEdgeAux::mLabelWidthRight for D1
 *          := max {DGEdgeAux::mLabelWidthUpperRight,
 *                  DGEdgeAux::mLabelWidthMiddleRight,
 *                  DGEdgeAux::mLabelWidthLowerRight  }
 *
 * W14: Horizontal extent of the label edges of D1 on the right hand side,
 *      i.e., F1 side.
 *      W14 := DGEdgeAux::mLabelWidthLeft
 *          := max {DGEdgeAux::mLabelWidthUpperLeft,
 *                  DGEdgeAux::mLabelWidthMiddleLeft,
 *                  DGEdgeAux::mLabelWidthLowerLeft  }
 *
 * W15: Width required to accommodate the absorbee VRs under N2
 *      between E1 and the edge to the left of it. It is calculated
 *      in the same way as W7.
 *
 * W16: F1's Right pane's share of W15. It's w15/2.0.
 *                           W9 := DGEdgeAux::mChildrenWidthUpperLeft for D1.
 *
 * W17: Width required to accommodate the absorbee VRs at N1 whose absorber
 *      face is F2, and incident edges are E1 and E3 and HG/2. 
 *      This is one of two halves of absorbee VRs that goes over N1 and to the
 *      right of E1. The extra HG/2 is required to keep this area and the 
 *      others on the other side of the face apart at least by HG.
 *      
 *                          W17 := DGEdgeAux::mChildrenWidthLowerRight for D1.
 *      
 * W18: Width required to accommodate the absorbee VRs at N1 whose absorber
 *      face is F2, and incident edges are E1 and E3 and HG/2 This is one of 
 *      two halves of absorbee VRs that goes under N1 and to the right of E3.
 *     
 * W19: Width required to accommodate the absorbee VRs at N1 whose absorber
 *      face is F1. This is one of two halves of the absorbee VRs that goes
 *      over N1 and to the left of E1.
 *
 * W20: Extent from the center (X1) of F1 to the right along D1 to accommodate
 *      all the features on the left hand side of D1.
 *      W20 := DGEdgeAux::mLeftInset
 *          := max{DGEdgeAux::mChildrenWidthUpperLeft, 
 *                 DGEdgeAux::mChildrenWidthLowerLeft }
 *             + HG + mLabelWidthLeft 
 *
 * W21: Extent from the center (X2) of F2 to the left along D1 to accommodate
 *      all the features on the right hand side of D1.
 *      W21 := DGEdgeAux::mRightInset
 *          := max{DGEdgeAux::mChildrenWidthUpperRight, 
 *                 DGEdgeAux::mChildrenWidthLowerRight }
 *             + HG + mLabelWidthRight
 *
 * W22: Total cost for D1 to accommodate all the features horizontally between
 *      F1 and F2.
 *      W22 := max {
 *                   W20 + W21,
 *                   DGEdgeAux::mWidthLabelShareUpper,
 *                   DGEdgeAux::mWidthLabelShareLower
 *             }
 *          := DGEdgeAux::mCost
 *
 *
 * H23: Height of the node label of N2.
 *      H23 := EGNodeAux::mCost 
 *          := VRRectLabelGraph::height()
 *
 * H24: Height required to accommodate the absorbee VRs under N2
 *      between E1 and E2. The Height is calculated as follows.
 *      
 *       H24 := max{H_vri} where H_vri is the height of the absorbee VRi.
 *           := EGEdgeAux::mChildrenHeightUpperRight
 *
 * H25: Height of the node label of N1.
 *      H25 := EGNodeAux::mHeight 
 *          := VRRectLabelGraph::height()
 *
 * H26: Height required to accommodate the absorbee VRs over N1
 *      between E1 and E3. 
 *      This is one of two halves of absorbee VRs that goes over N1 and to the
 *      right of E1. The height is calculated as follows.
 *      
 *       H26 := max{H_vri} where H_vri is the height of the absorbee VRi.
 *           := EGEdgeAux::mChildrenHeightLowerRight
 *
 * H27: Height required to accommodate the edge labels of E1.
 *       H27 := EGEdgeAux::mLabelHeight
 *       
 * H28: Height required to accommodate the absorbee VRs under N2
 *      between E1 and the one to the left of it.
 *      The Height is calculated as follows.
 *      
 *       H28 := max{H_vri} where H_vri is the height of the absorbee VRi.
 *           := EGEdgeAux::mChildrenHeightUpperLeft
 *
 * H29: Height required to accommodate the absorbee VRs over N1
 *      between E1 and E3. 
 *      This is one of two halves of absorbee VRs that goes over N1 and to the
 *      left of E1. The height is calculated as follows.
 *      
 *       H26 := max{H_vri} where H_vri is the height of the absorbee VRi.
 *           := EGEdgeAux::mChildrenHeightLoweLeft
 *
 * H30: Total cost for E1 to accommodate all the features vertically along E1.
 *      H30 := max{
 *                  EGEdgeAux::mLabelHeight,
 *
 *                  max{
 *                       EGEdgeAux::mChildrenHeightUpperRight
 *                       EGEdgeAux::mChildrenHeightUpperLeft 
 *                  } + VG + 
 *                  max{
 *                       EGEdgeAux::mChildrenHeightLowerRight
 *                       EGEdgeAux::mChildrenHeightLowerLeft 
 *                  }
 *             }
 *          := EGEdgeAux::mCost
 *
 *
 * X31: Horizontal coordiate assigned to E1. 
 *
 *        X3 := (X2 - W21) + (X1 + W20) / 2
 *           := EGEdgeAux::mX
 *      Note the value (X2 - W21) - (X1 + W20) is a slack on D1 after X1 and X2
 *      are determined.
 *
 * Ex. Outer face, bottom node.
 *
 *       
 *                    X1                                  X2
 *                    |                                   |
 *                                                                   H12
 *                    +-----------------------------------+           +    +
 *                    |  S                                |           |    |
 *            Y9  +---++-----+-------------------------------++       +    |
 *                |   ||     |    |                          ||    |       |
 *                |   ||E1   |    T*                         ..    S*      |
 *                |   ||     |    |                      .. .      |       |
 *                |   ||     |                    F1  .  .. .              |
 *        D1 ...==|===||=====|====|                |  |  || |      |       |
 *                |   ||     |                        |E2|| |  D2          |
 *                |   ||     |    |                |==|==||=|======|       |
 *                |   ||     |                        |  || |              |
 *                |   ||     |    |                |  |  || |      |       |
 *                +---++-+---+-----------------------++--++-+         + +  |
 *                    |  |T                          |    |           | |  |
 *           Y8       +--+---------------------------+----+           | +  |
 *                       |                           |                |H11 |
 *                       |                           |                |    |
 *                       +---------------------------+                +    +
 *                                                                   H10  H13
 *             W3 <-->                                 W4 <->
 *                    W5 <--------------------------->
 *                <------------------------------------------------> W6
 *
 *                <------------------------------------------------> W7
 *
 *
 * S: The top node of the absorber block with the highest rank.
 * T: The bottom node to which the absorbee blocks are unified.
 *
 * S*: The outer face of the block.
 * T*: The right face. The other face of S* across {S,T}.
 *
 * E1: Edge {S,T} that runs through the left side of the block 
 * D1: Dual Edge {S*,T*}. The lest-most dual edge.
 * E2: Edge incident to S* and T.
 * D2: Dual Edge of E2.
 *
 * X1: X-position of Edge E1. X1 := EGEdgeAux::mX 
 * X2: X-position of Edge E2. X2 := EGEdgeAux::mX 
 *
 * W3: Horizontal extent of the label edges of E1 on the left hand side.
 *      W3 := DGEdgeAux::mLabelWidthLeft
 *
 * W4: Horizontal extent of the label edges of E2 on the right hand side.
 *      W4 := DGEdgeAux::mLabelWidthRight
 *
 * W5: Width required to accommodate the absorbee VRs under T
 *     between E1 and E2 in S*. 
 *       W5 := ETNodeAux::mExtraWidthBottom
 *
 * W6: Width required to accommodate all the other features execpt for the
 *     absorbee VRs for the outer face/bottom node.
 *       W6 := DGFaceAux::mEarlyStart for S* - X1 + W3.
 *
 * W7: Width required for this block.
 *     W7 := max{ W6, W7 }
 *        := ETNodeAux::mWidth
 *
 * Y8:  lower edge of the node T      EGNodeAux::mEarlyStart
 *
 * Y9:  lower edge of the node S      EGNodeAux::mEarlyStart
 *
 * H10: Height required to accommodate the absorbee VRs under T.
 *      H10: ETNodeAux::mExtraHeightBottom + EGNodeAux::mLabelHeight
 *
 * H11: Height of the node label of T.
 *      H11 := EGNodeAux::mLabelHeight
 *          := VRRectLabelGraph::height()
 *
 * H12: Height of the node label of S.
 *      H12 := EGNodeAux::mLabelHeight
 *          := VRRectLabelGraph::height()
 *
 * H13: Height required for this block.
 *       H13 := Y9 - Y8 + H12 + H10 - H11
 *           := ETNodeAux::mHeight
 *
 * ===================================================
 * Allocating spaces for vertex label  and edge labels
 * ===================================================
 *
 *  The height of the node label is dealt with by the costs of the nodes.
 *  The width is distributed evenly among duals of the incident edges.
 *
 *   EGNodeAux::mLabelHeight for N := VRRectLabelNode::height() for N.
 *
 *           eu1   eu2   eu3   eu4   euN
 *            |     |     |     |     |
 *            |     |     |     |     |
 *            |     |     |     |     |
 *            |     |     |     |     |
 *          +---------------------------+
 *          |                           |
 *      fL  |              N            |  fR
 *          |                           |
 *          +---------------------------+
 *            |     |     |     |     |
 *            |     |     |     |     |
 *            |     |     |     |     |
 *            |     |     |     |     |
 *           el1   el2   el3   el4   elM
 *
 *   EGEdgeAux::
 *  Assume there are N edge from above, and M edges from below.
 *  The width of node label width(label) is evenly distributed among them.
 *
 *  DGEdgeAux::mWidthLabelShareUpper for upper node
 *                                               := VRRectLabelNode::width()/N
 *  DGEdgeAux::mWidthLabelShareUpper for lower node
 *                                               := VRRectLabelNode::width()/M
 *
 * There are 3 edge labels along the edge: one closer to node 1, one in the
 * middle, and one closer to node 2. Each edge is places any of three
 * hozirontal positions. in the right face, in the center of the edge, and
 * in the left face.
 *
 *      N2 ========+=======
 *         +-------+------+
 *         |       |+----+|
 *         |       || L2 ||   Closer to N1, positioned in the right face
 *         |       |+----+|
 *         |       |      |
 *         |+-----+|      |
 *         ||     ||      |
 *         || Lm  ||      |   In the middle, positioned in the left face
 *         ||     ||      |
 *         |+-----+|E1    |
 *    fL   |       |      |   fR
 *         |      +-+     |
 *         |      |L1     |   Closer to N2, positioned in the center
 *         |      | |     |
 *         |      +-+     |
 *         +-------+------+
 *      N1 ========+========
 *
 * The height of those three labels are dealt with in the cost of the edge E1.
 *   EGEdgeAux::mLabelHeight := VRRectLabelEdge::heightNode1()  + 
 *                              VRRectLabelGraph::verticalGap() + 
 *                              VRRectLabelEdge::heightMiddle() +
 *                              VRRectLabelGraph::verticalGap() + 
 *                              VRRectLabelEdge::heightNode2()
 *
 * The widths of those three labels are dealt with in the cost of the dual of
 * E1
 *
 * 1. Assuming all the three labels exist and are positioned in the right face
 *    then we have to accommodate the space for them.
 *
 * DGEdgeAux::mLabelWidthLeft := 0.0
 *
 * DGEdgeAux::mLabelWidthRight := max { VRRectLabelEdge::widthNode1(),
 *                                      VRRectLabelEdge::widthMiddle(),
 *                                      VRRectLabelEdge::heightNode2() }
 *
 * 2. Assuming all the three labels exist and are positioned in the center.
 *
 * DGEdgeAux::mLabelWidthLeft :=  max { VRRectLabelEdge::widthNode1()/2.0,
 *                                      VRRectLabelEdge::widthMiddle()/2.0,
 *                                      VRRectLabelEdge::heightNode2()/2.0 }
 *
 * DGEdgeAux::mLabelWidthRight := max { VRRectLabelEdge::widthNode1()/2.0,
 *                                      VRRectLabelEdge::widthMiddle()/2.0,
 *                                      VRRectLabelEdge::heightNode2()/2.0 }
 *
 * 3. Assuming all the three labels exist and are positioned in the left face
 *    then we have to accommodate the space for them.
 *
 * DGEdgeAux::mLabelWidthLeft :=  max { VRRectLabelEdge::widthNode1(),
 *                                      VRRectLabelEdge::widthMiddle(),
 *                                      VRRectLabelEdge::heightNode2() }
 *
 * DGEdgeAux::mLabelWidthRight := 0.0
 *
 * ====================
 * Input data structure
 * ====================
 * First, the input connected graph must be planar.
 * It must be represented by VRRectLabelGraph[Node,Edge], which provides
 * means to handle rectangular label dimensions in integer.
 *
 * The graph then must be decomposed into BCTree, and then an embedding
 * must be given in EmbeddedBCTree.
 * EmbeddedBCTree generates ExplorationTree as a result of generating
 * a geometric embedding of the connected graph.
 * EmbeddedBCTree::ExplorationTree is the input to VisRepFinder.
 *
 * The overall flow of preparation is drawn as follows.
 *
 *   VRRectLabelGraph[Node,Edge] : Original input the user must create.
 *
 *     ||     biconnected
 *    \||/    decomposition
 *
 *   BCTree
 *
 *     ||     embedding of each
 *     ||     block and the
 *     ||     face unification
 *     ||     groups at each 
 *    \||/    cut vertex
 *
 *   EmbeddedBCTree
 *
 *     ||     top node and outer face of
 *     ||     each block, absorber-absorbee
 *     ||     relationships at each face
 *    \||/    unification group.
 *
 *   EmbeddedBCTree::ExplorationTree
 *
 *  - VRRectLabel{Node,Edge} and Block{Node,Edge} in BCTreeNode.block()
 *    must be cross-linked.
 *
 *  - Block{Node,Edge} in BCTreeNode.block() and Embedded{Node,Edge} in 
 *    EmbeddedBCTreeNode.embeddedGraph() must be cross-linked.
 *
 *  - BCTree{Node,Edge} in BCTree and EmbeddedBCTree{Node,Edge} 
 *    must be cross-linked.
 *
 *  - ExplorationNode in ExplorationTree and EmbeddedBCTreeNode
 *    must be cross-linked.
 *
 *
 *
 * =====================
 * Output data structure
 * =====================
 * VisRepFinder::find() does the following.
 *
 *   EmbeddedBCTree::ExplorationTree
 *
 *     ||     x- and y-coordinates of each
 *     ||     node and the edges. A node is
 *     ||     representation by a horizontal
 *     ||     line segment, and an edge is
 *     ||     by a vertical one. A node label
 *     ||     can be placed at the center of
 *     ||     the line segment. Edge label
 *     ||     can be placed along the vertical
 *    \||/    line segment at an appropriate location.
 *
 *   VRRectLabelGraph[Node,Edge]
 *
 *
 * ============
 * Classes List
 * ============
 * - VisRepFinder : The main class the user uses.
 * 
 *
 * - VRRectLabelGraph       : The input graph the user specifies with the
 *                          geometric information.
 * - VRRectLabelGraphNode : The input graph node with the rectangular label
 *                          dimensions.
 * - VRRectLabelGraphEdge : The input graph edge with the rectangular label
 *                          dimensions.
 * 
 * Following 5 classes are used internally in VisRepFinder
 *
 *   - EGNodeAux  : Internal data structure used to augument EmbeddedNode
 *                  in the embedding of the blocks in the BCTree.
 *
 *   - EGEdgeAux  : Internal data structure used to augument EmbeddedEdge
 *                  in the embedding of the blocks in the BCTree. 
 *
 *   - DGFaceAux  : Internal data structure used to augument EmbeddedFace
 *                  in the dual of the embedding of the blocks in the BCTree
 *
 *   - DGEdgeAux  : Internal data structure used to augument DualEdge
 *                  in the dual of the embedding of the blocks in the BCTree
 *
 *   - ETNodeAux  : Internal data structure used to augument BCTreeNode
 *                  in the BCTree.

 * =================================
 * Snippets of sample implementation
 * =================================
 * 
 *
 *
 *
 * @reference
 * [TT86] "A Unified Approach to Visibility Representation of Planar Graphs"
 *  R. Tamassia and I.G. Tollis, Discrete Comput Geom 1:321-341 (1986)
 *
 */


/** @class VRRectLabelGraph
 *
 *  @brief The graph for which a visibility representation is generated.
 *         It contains rectangular label dimensions for nodes and edges.
 *         The node labels are placed at the center of the node.
 *         There are three types of edge labels: one close to node 1, 
 *         one close to node 2, and the other in the middle.
 *         For the edge labels the location relative to the edge curve 
 *         is specified in any of the 5 positions:
 *
 *         1 placed on the curver
 *
 *         2 attached to the curve on clockwise side when the edge is 
 *           oriented from node 1 to 2.
 *
 *         3 attached to the curve on counter-clockwise side when the edge is 
 *           oriented from node 1 to 2.
 *
 *
 *       Edge labels          Edge labels            Edge labels
 *    placed on center     placed on CW side      placed on CCW side
 *  
 *            +-----+               +-----+            +-----+
 *      Node1 |     |               |     |            |     |
 *      Label +-----+               +-----+            +-----+
 *               |                     |                  |
 *      Edge   +---+              +---+|                  |+---+
 *      Label  |   |              |   ||                  ||   |
 *      Node1  +---+              +---+|                  |+---+
 *               |                     |                  |
 *               |                     |                  |
 *               |                     |                  |
 *      Edge   +---+              +---+|                  |+---+
 *      Label  |   |              |   ||                  ||   |
 *      Middle +---+              +---+|                  |+---+
 *               |                     |                  |
 *               |                     |                  |
 *               |                     |                  |
 *      Edge   +---+              +---+|                  |+---+
 *      Label  |   |              |   ||                  ||   |
 *      Node1  +---+              +---+|                  |+---+
 *               |                     |                  |
 *            +-----+               +-----+            +-----+
 *      Node2 |     |               |     |            |     |
 *      Label +-----+               +-----+            +-----+
 */


/** @brief this is the unit used for geometric coordinates.
 *         this must be real as there are some divisions involved.
 */
using coord_t = double;

/** @class VRRectLabelGraph
 *
 *  @brief this is the input and the output for the visibility representation.
 *         1. The user specify the sizes of the labels of the nodes and edges
 *            in VRRectLabelNode and VRRectLabelEdge.           
 *            The user also specifies the horizontal and vertical gaps.
 *              
 *         2. The user gives it to VisRepFinder.
 *
 *         3. VisRepFinder assigns the coordinates to the nodes and edges
 *            in VRRectLabelNode and VRRectLabelEdge. It also finds the total
 *            rectangular area for the visibility representation.
 */
class VRRectLabelGraph : public PlanarizableGraph {

  public:
    inline void    setHorizontalGap(coord_t v) { mHGap = v; }
    inline void    setVerticalGap(coord_t v)   { mVGap = v; }
    inline coord_t horizontalGap() const       { return mHGap; }
    inline coord_t verticalGap() const         { return mVGap; }
    inline coord_t width() const               { return mWidth; }
    inline coord_t height() const              { return mHeight; }

  private:

    /** @brief horizontal gap between two features.
     */
    coord_t mHGap;

    /** @brief vertical gap between two features
     */
    coord_t mVGap;

    /** @brief the width of the area of the generated visibility representation
     */
    coord_t mWidth;

    /** @brief the height of the area of the generated visibility 
     *         representation
     */
    coord_t mHeight;

friend class VisRepFinder;

};


/** @class VRRectLabelNode
 *
 *  @brief graph node with rectangular label dimensions
 */
class VRRectLabelNode : public PlanarizableNode {

  public:

    /** @brief default constructor w/o any label
     */
    VRRectLabelNode():mWidth(0), mHeight(0){;}
                                 

    /** @brief default constructor with label dimensions
     *  @param W        (in): Width of the label
     *  @param H        (in): Height of the label
     */
    VRRectLabelNode(coord_t W, coord_t H):mWidth(W), mHeight(H){;}

    /* accessors */
    inline bool     hasLabel()  { return mWidth > 0 && mHeight > 0;}
    inline coord_t  width()     { return mWidth; }
    inline coord_t  height()    { return mHeight; }

    inline coord_t  xLeft()     { return mXleft; }
    inline coord_t  xRight()    { return mXright; }
    inline coord_t  y()         { return mY; }

  private:
    coord_t           mWidth;
    coord_t           mHeight;

    bool              mXset;
    coord_t           mXleft;
    coord_t           mXright;

    coord_t           mY;

friend class VisRepFinder;

#ifdef USE_TEST_GRAPH_GENERATOR
  public:
    long              mDebugNum;
#endif
};


/** @class VRRectLabelEdge
 *
 *  @brief graph edge with rectangular labels dimensions
 */
class VRRectLabelEdge : public PlanarizableEdge {

  public:
    /** @brief orientation is defined as follows:
     *             counter-clockwise
     *        N1 -------------------->   N2
     *                clockwise    
     */
    enum posType { POS_CENTER, POS_COUNTER_CLOCKWISE, POS_CLOCKWISE };


    /** @brief constructor w/o any labels.
     */
    VRRectLabelEdge():
        mTypeNode1(POS_CENTER),
        mWidthNode1(0),
        mHeightNode1(0),
        mTypeNode2(POS_CENTER),
        mWidthNode2(0),
        mHeightNode2(0),
        mTypeMiddle(POS_CENTER),
        mWidthMiddle(0),
        mHeightMiddle(0),
        mFlipped(false){;}

    void setLabelNode1Side(posType type, coord_t W, coord_t H){
        mTypeNode1   = type;
        mWidthNode1  = W;
        mHeightNode1 = H;
    }

    void setLabelNode2Side(posType type, coord_t W, coord_t H){
        mTypeNode2   = type;
        mWidthNode2  = W;
        mHeightNode2 = H;
    }

    void setLabelMiddle(posType type, coord_t W, coord_t H){
        mTypeMiddle   = type;
        mWidthMiddle  = W;
        mHeightMiddle = H;
    }

    void setFlipped() {mFlipped=true;}

    /* accessors */
    bool hasNode1Label()      { return mWidthNode1 > 0 && mHeightNode1 > 0;}
    coord_t widthNode1()      { return mWidthNode1;}
    coord_t heightNode1()     { return mHeightNode1;}
    enum posType typeNode1()  { return mTypeNode1;}
    bool hasNode2Label()      { return mWidthNode2 > 0 && mHeightNode2 > 0;}
    coord_t widthNode2()      { return mWidthNode2;}
    coord_t heightNode2()     { return mHeightNode2;}
    enum posType typeNode2()  { return mTypeNode2;}
    bool hasMiddleLabel()     { return mWidthMiddle > 0 && mHeightMiddle > 0;}
    coord_t widthMiddle()     { return mWidthMiddle;}
    coord_t heightMiddle()    { return mHeightMiddle;}
    enum posType typeMiddle() { return mTypeMiddle;}

    coord_t x()               { return mX; }
    coord_t y1()              { return mY1; }
    coord_t y2()              { return mY2; }

    bool    isFlipped()       { return mFlipped; }

  private:

    enum posType      mTypeNode1;
    coord_t           mWidthNode1;
    coord_t           mHeightNode1;
    enum posType      mTypeNode2;
    coord_t           mWidthNode2;
    coord_t           mHeightNode2;
    enum posType      mTypeMiddle;
    coord_t           mWidthMiddle;
    coord_t           mHeightMiddle;

    coord_t           mX;
    coord_t           mY1;
    coord_t           mY2;

    bool              mFlipped;

friend class VisRepFinder;

};




/** @brief temporarily auguments EmbeddedNode in EmbeddedBCTreeNode
 */
class EGNodeAux {

  public:
    EGNodeAux() { reset(); }

  private:
    void reset() {
//        mSTNum                = 0;
        mNumOutgoingProcessed = 0;
        mLabelHeight          = 0.0;
        mCost                 = 0.0;
        mEarlyStart           = 0.0;
        mY                    = 0.0;
    }

    /** @brief ST-numbering which gives partial order among EmbeddedNodes.
     */
    long mSTNum;

    /** @brief incoming EmbeddedEdges after the orientation is determined.
     *         NOTE: the ordering is not aligned with the planar embedding.
     */
    vector<edge_list_it_t> mEdgesIn;

    /** @brief outgoing EmbeddedEdges after the orientation is determined.
     *         NOTE: the ordering is not aligned with the planar embedding.
     */
    vector<edge_list_it_t> mEdgesOut;

    /** @brief the left face of this node
     *         if the block containing it is in the normal orientation
     */
    node_list_it_t mLeftFace;

    /** @brief the right face of this node
     *         if the block containing it is in the normal orientation
     */
    node_list_it_t mRightFace;

    /** @brief number of outgoing edges processed. Used in scheduling
     *         algorithm.
     */
    long mNumOutgoingProcessed;

    /** @brief stores the node label height.
     */
    coord_t mLabelHeight;

    /** @brief accummulated cost used for scheduling.
     */
    coord_t mCost;

    /** @brief earliest start time by the scheduling algorithm
     */
    coord_t mEarlyStart;

    /** @brief Y-coordinate value. This is in the local coordinate system
     *         in pass 1, it will be transformed to the global coordinate
     *         system in pass 2.
     */
    coord_t mY;

friend class VisRepFinder;

#ifdef UNIT_TESTS
    std::ostream& debugPrint(std::ostream& os, size_t indent, long nodeNum);
#endif

};


/** @brief temporarily auguments EmbeddedEdge in EmbeddedBCTreeNode
 */
class EGEdgeAux {

  public:
    EGEdgeAux(){ reset(); }

  private:
    void reset() {
//        mOriented1to2             = false;
        mLabelHeightUpper         = 0.0;
        mLabelHeightMiddle        = 0.0;
        mLabelHeightLower         = 0.0;
        mLabelHeight              = 0.0;
        mChildrenHeightUpperRight = 0.0;
        mChildrenHeightLowerRight = 0.0;
        mChildrenHeightUpperLeft  = 0.0;
        mChildrenHeightLowerLeft  = 0.0;
        mCost                     = 0.0;
        mX                        = 0.0;
        mFlipped                  = false;
    }

    /** @brief specifies orientation of the edge
     */
    bool    mOriented1to2;

    /** @brief height of the edge label on the upper node side
     */
    coord_t mLabelHeightUpper;

    /** @brief height of the edge label on the lower node side
     */
    coord_t mLabelHeightLower;

    /** @brief height of the edge label in the middle
     */
    coord_t mLabelHeightMiddle;

    /** @brief height required to accommodate the edge labels along the edge.
     */
    coord_t mLabelHeight;

    /** @brief height of the children VR on the upper node on the right face
     */
    coord_t mChildrenHeightUpperRight;

    /** @brief height of the children VR on the lower node on the right face
     */
    coord_t mChildrenHeightLowerRight;

    /** @brief height of the children VR on the upper node on the left face
     */
    coord_t mChildrenHeightUpperLeft;

    /** @brief height of the children VR on the lower node on the left face
     */
    coord_t mChildrenHeightLowerLeft;

    /** @brief cost calculated for this edge.
     */
    coord_t mCost;

    /** @brief X-coordinate value. This is in the local coordinate system
     *         in pass 1, it will be transformed to the global coordinate
     *         system in pass 2.
     */
    coord_t mX;

    /** @brief indicates the edge is flipped horizontally, as a result of
     *         the block it blongs to has been flipped.
     */
    bool    mFlipped;

friend class VisRepFinder;

#ifdef UNIT_TESTS
  public:
    std::ostream& debugPrint(std::ostream& os, size_t indent);
#endif
};


/** @brief temporarily auguments EmbeddedFace in EmbeddedBCTreeNode
 */
class DGFaceAux {

  public:
    DGFaceAux(){ reset(); }

  private:
    void reset() {
        mNumIncomingProcessed = 0;
        mCost                 = 0;
        mEarlyStart           = 0;
        mX                    = 0;
    }

    /** @brief incoming DualEdges after the orientation is determined.
     *         NOTE: the ordering is not aligned with the planar embedding.
     */
    vector<edge_list_it_t> mEdgesIn;

    /** @brief outgoing DualEdges after the orientation is determined.
     *         NOTE: the ordering is not aligned with the planar embedding.
     */
    vector<edge_list_it_t> mEdgesOut;

    /** @brief number of incoming edges processed. Used in scheduling
     *         algorithm.
     */
    long    mNumIncomingProcessed;

    /** @brief accummulated cost used for scheduling.
     */
    coord_t mCost;

    /** @brief earliest start time by the scheduling algorithm
     */
    coord_t mEarlyStart;

    /** @brief the center position of this face.
     */
    coord_t mX;

friend class VisRepFinder;

#ifdef UNIT_TESTS
    std::ostream& debugPrint(std::ostream& os, size_t indent, long nodeNum);
#endif

};


/** @brief temporarily auguments DualEdge in EmbeddedBCTreeNode
 */
class DGEdgeAux {

  public:
    DGEdgeAux(){ reset();}

  private:
    void reset() {
//        mOriented1to2            = false;
        mWidthLabelShareUpper    = 0.0;
        mWidthLabelShareLower    = 0.0;
        mLabelWidthUpperRight    = 0.0;
        mLabelWidthMiddleRight   = 0.0;
        mLabelWidthLowerRight    = 0.0;
        mLabelWidthRight         = 0.0;
        mLabelWidthUpperLeft     = 0.0;
        mLabelWidthMiddleLeft    = 0.0;
        mLabelWidthLowerLeft     = 0.0;
        mLabelWidthLeft          = 0.0;
        mChildrenWidthUpperRight = 0.0;
        mChildrenWidthLowerRight = 0.0;
        mChildrenWidthUpperLeft  = 0.0;
        mChildrenWidthLowerLeft  = 0.0;
        mLeftInset               = 0.0;
        mRightInset              = 0.0;
        mCountMeOut              = false;
        mCost                    = 0.0;
    }

    /** @brief specifies orientation of the edge
     */
    bool    mOriented1to2;

    /** @brief its share of top node label width.
     */
    coord_t mWidthLabelShareUpper;

    /** @brief its share of bottom node label width.
     */
    coord_t mWidthLabelShareLower;

    /** @brief width of the edge label on the upper node 
     *         on the right face.
     */
    coord_t mLabelWidthUpperRight;

    /** @brief width of the edge label in the middle
     *         on the right face.
     */
    coord_t mLabelWidthMiddleRight;

    /** @brief width of the edge label on the lower node 
     *         on the right face.
     */
    coord_t mLabelWidthLowerRight;

    /** @brief width of the edge label on the upper node 
     *         on the left face.
     */
    coord_t mLabelWidthUpperLeft;

    /** @brief width of the edge label in the middle
     *         on the left face.
     */
    coord_t mLabelWidthMiddleLeft;

    /** @brief width of the edge label on the lower node 
     *         on the left face.
     */
    coord_t mLabelWidthLowerLeft;

    /** @brief width of the children VR on the upper node on the right face.
     */
    coord_t mChildrenWidthUpperRight;

    /** @brief width of the children VR on the lower node on the right face.
     */
    coord_t mChildrenWidthLowerRight;

    /** @brief width of the children VR on the upper node on the left face.
     */
    coord_t mChildrenWidthUpperLeft;

    /** @brief width of the children VR on the lower node on the left face.
     */
    coord_t mChildrenWidthLowerLeft;

    /** @brief max width of the label widths to the right.
     */
    coord_t mLabelWidthRight;

    /** @brief max width of the label widths to the left.
     */
    coord_t mLabelWidthLeft;

    /** @brief the prohibited extent from the left face where the correponding
     *         edge  can't be drawin.
     */
    coord_t mLeftInset;

    /** @brief the prohibited extent from the right face where the correponding
     *         edge  can't be drawin.
     */
    coord_t mRightInset;

    /** @brief true if this edge is excluded from scheduling algorithm.
     */
    bool    mCountMeOut;

    /** @brief earliest start time by the scheduling algorithm
     */
    coord_t mCost;

friend class VisRepFinder;

#ifdef UNIT_TESTS
    std::ostream& debugPrint(std::ostream& os, size_t indent);
#endif

};


/** @brief temporarily augument EmbeddedBCTreeNode
 */
class ETNodeAux {

  public:
    ETNodeAux(){ reset(); }

  private:
    void reset() {
        mExtraHeightBottom   = 0.0;
        mExtraWidthBottom    = 0.0;
        mExtraHeightRight    = 0.0;
        mExtraWidthRight     = 0.0;
        mBlockHeight         = 0.0;
        mBlockWidth          = 0.0;
        mWidth               = 0.0;
        mHeight              = 0.0;
        mXOffsetBlock        = 0.0;
        mYOffsetBlock        = 0.0;
        mXOffsetRight        = 0.0;
        mYOffsetRight        = 0.0;
        mXOffsetBottom       = 0.0;
        mYOffsetBottom       = 0.0;

        mAnchorX1            = 0.0;
        mAnchorX2            = 0.0;
        mAnchorY1            = 0.0;
        mAnchorY2            = 0.0;
        mFlippedVertically   = false;
        mFlippedHorizontally = false;
    }

    /** @brief auxiliary data structures for embedded graph and dual graph.
     */
    vector<EGNodeAux> mEGNodeAux;
    vector<EGEdgeAux> mEGEdgeAux;
    vector<DGFaceAux> mDGFaceAux;
    vector<DGEdgeAux> mDGEdgeAux;

    /** @brief extra height required to accommodate childs on the outer
     *         face at the bottom node.
     */
    coord_t mBlockHeight;

    /** @brief width required to accommodate childs on the outer
     *         face at the bottom node.
     */
    coord_t mBlockWidth;

    /** @brief extra height required to accommodate childs on the outer
     *         face at the bottom node.
     */
    coord_t mExtraHeightBottom;

    /** @brief width required to accommodate childs on the outer
     *         face at the bottom node.
     */
    coord_t mExtraWidthBottom;

    /** @brief heigth required to accommodate childs on the outer
     *         face at the top node.
     */
    coord_t mExtraHeightRight;

    /** @brief width required to accommodate childs on the outer
     *         face at the top node.
     */
    coord_t mExtraWidthRight;

    /** @brief width of the visibility representation for this tree node
     *         and its children.
     */
    coord_t mWidth;

    /** @brief height of the visibility representation for this tree node
     *         and its children.
     */
    coord_t mHeight;

    /** @brief X offset for the bottom left corner for this block.
     */
    coord_t mXOffsetBlock;

    /** @brief Y offset for the bottom left corner for this block.
     */
    coord_t mYOffsetBlock;

    /** @brief X offset for the bottom left corner for the childs on the
     *         outer face at the top node.
     */
    coord_t mXOffsetRight;

    /** @brief Y offset for the bottom left corner for the childs on the
     *         outer face at the top node.
     */
    coord_t mYOffsetRight;

    /** @brief X offset for the bottom left corner for the childs on the
     *         outer face at the bottom node.
     */
    coord_t mXOffsetBottom;

    /** @brief Y offset for the bottom left corner for the childs on the
     *         outer face at the bottom node.
     */
    coord_t mYOffsetBottom;

    /** @brief the X coordinate for the left corner of the visiblity 
     *         representation of this block in the global coordinate system.
     */
    coord_t mAnchorX1;
    coord_t mAnchorX2;

    /** @brief the Y coordinate for the top corner of the visibility
     *         representation of this block in the global coordinate system.
     */
    coord_t mAnchorY1;
    coord_t mAnchorY2;



    /** @brief true if the visibility representation must be flipped 
     *         vertically along the Y axis in the global coordinate system.
     */
    bool    mFlippedVertically;

    /** @brief true if the visibility representation must be flipped 
     *         horizontally along the Y axis in the global coordinate system.
     */
    bool    mFlippedHorizontally;

friend class VisRepFinder;

#ifdef UNIT_TESTS
    std::ostream& debugPrint(std::ostream& os, size_t indent);
#endif

};


/** @class VisRepFinder
 *
 *  @brief main class
 */
class VisRepFinder {

  public:

    /** @brief constructor with the input EmbeddedBCTree
     *
     *  @param tree (in): EmbeddedBCTree as input.
     */
#ifdef UNIT_TESTS
    inline VisRepFinder(EmbeddedBCTree& tree):mDebugPrint(false),mEBCTree(tree){;}
#else
    inline VisRepFinder(EmbeddedBCTree& tree):mEBCTree(tree){;}
#endif
    /** @brief main function. Generates a visibility representation.
     */
    void find();

#ifdef UNIT_TESTS
    bool mDebugPrint;
#endif

  private:

    static constexpr const coord_t kSplitPointAlpha = 1.0;

    /** @brief rooted directed exploration tree of EmbeddedBCTree
     *         set by the constructor.
     */        
    EmbeddedBCTree&   mEBCTree;

    /** @brief holds temporary auxiliary data structure to augument
     *         EmbeddedBCTreeNode,
     *         EmbeddedNode, EmbeddedEdge, EmbeddedFace, and DualEdge.
     *         They are indexed by Node::utility() and Edge::utility().
     */
    vector<ETNodeAux> mAux;

    /** @brief hozirontal gap between labels. Copied from VRRectLabelGraph.
     */
    coord_t           mHGap;

    /** @brief vertical gap between labels. Copied from VRRectLabelGraph.
     */
    coord_t           mVGap;

    /** utility shorthand notation for max of three vlues.
     */
    inline coord_t max(coord_t v1, coord_t v2, coord_t v3);

    /** @brief create aux data for EmbeddedBCTreeNode,
     *         EmbeddedNode, EmbeddedEdge, EmbeddedFace, and DualEdge.
     */
    void prepare();

    /******************************/
    /* FUNCTIONS FOR PASS 1 BEGIN */
    /******************************/

    /** @brief exploring through the EmbeddedBCTree::ExplorationTree in the, 
     *         bottom-up manner, and find visibility representation for each 
     *         block in its local coordinates.
     */
    void performPass1BottomUp();

    /** @brief top-level dispatch function for each node of pass 1.
     *
     *  @param  N       (in): EmbeddedBCTree::ExplorationNode for the
     *                        current block.
     */
    void processOneTreeNodePass1(EmbeddedBCTree::ExplorationNode& N);

    /** @brief find the bottom node (t), an edge {s,t}, the other face adjacent
     *         to outer face t* , and the dual edge {s*,t*},
     *         where s is the top node and s* is the outer face.
     *        
     *  @param  ETN     (in): EmbeddedBCTreeNode for the current block.
     *
     *  @param bottomIt (out):Bottom node (t)
     *
     *  @param rightIt  (out):The adjacent face of the outer face (t*)
     *
     *  @param stIt     (out): {s,t}
     *
     *  @param stDualIt (out): {s*,t*}
     */
    void findBottomNodeAndLeftMostFace(
        EmbeddedBCTreeNode& ETN,
        node_list_it_t&     bottomIt,
        node_list_it_t&     rightIt,
        edge_list_it_t&     stIt,
        edge_list_it_t&     stDualIt
    );

    /** @brief orient the edges and the dual edges based on the (t,s)-numbering
     *        
     *  @param  ETN     (in): EmbeddedBCTreeNode for the current block.
     *
     *  @param bottomIt (in): Bottom node (t)
     *
     *  @param rightIt  (in): The left-most face in DualGraph of the block
     *                        after the cycle gets broken at {s*,t*}
     *                        The name 'right' comes from the fact that it is
     *                        to the right of the outer face if it is placed
     *                        to the left most location. After the cycle gets
     *                        broken by excluding {s*,t*}, it becomes the 
     *                        left most node, and the outer face (s*) becomes
     *                        the right most face.
     *
     *  @param stIt     (in): {s,t}
     *
     *  @param stDualIt (in): {s*,t*}. The dual of {s,t}
     */
    void orientEGandDG(
        EmbeddedBCTreeNode& ETN,
        node_list_it_t      bottomIt,
        node_list_it_t      rightIt,
        edge_list_it_t      stIt,
        edge_list_it_t      stDualIt
    );


    /** @brief resets the width and height values in the Aux structures in
     *         preparation for the cost caluculation.
     *  @param  ETN     (in): EmbeddedBCTree::ExplorationNodeEmbedded foor the
     *                        current block.
     */
    void assignDefaultCosts(EmbeddedBCTreeNode& ETN);

    /** @brief assigns the heights and the widths of the nodes to the Aux
     *         structures.
     *        VRRectLabelNode::height() => EGNodeAux::mLabelHeight
     *        VRRectLabelNode::width()  => DGEdgeAux::mWidthLabelShareUpper
     *        VRRectLabelNode::width()  => DGEdgeAux::mWidthLabelShareLower
     *
     *  @param    N        (in): EmbeddedBCTreeNode for the current block.
     *
     *  @param    stDualIt (in): STDual edge in DualGraph.
     */
    void handleNodeLabels(EmbeddedBCTreeNode& N, edge_list_it_t stDualIt);


    /** @brief sort the incident edges of EmbeddedNode into two: upper and
     *         lower according to the (t,s)-numbering.
     *         A supporting function to handleNodeLabels()
     *
     *  @param embNode    (in):  EmbeddedNode 
     *
     *  @param embNode    (in):  The Aux structure for embNode.
     *
     *  @param upperEdges (in):  List of incident edges higher than embNode
     *                           with respect to (t,s)-numbering.
     *
     *  @param lowerEdges (in):  List of incident edges lower than embNode
     *                           with respect to (t,s)-numbering.
     */
    void findUpperAndLowerEdges(
        EmbeddedNode&           embNode,
        ETNodeAux&              aux,
        vector<edge_list_it_t>& upperEdges,
        vector<edge_list_it_t>& lowerEdges
    );


    /** @brief find the left and right margins for node label width.
     *         We usually add mHGap/2.0 to the left and to the right
     *         of the label width so that two node labels are at least
     *         apart by mHGap.
     *         However, if the node is incident to the outer face, we don't
     *         need those gaps.
     *
     *  @param leftGap    (out): left gap to be added. Either 0.0 or mHGap/2.0.
     *
     *  @param rightGap   (out): right gap to be added.Either 0.0 or mHGap/2.0.
     *
     *  @param upperEdges (in):  List of incident edges higher than embNode
     *                           with respect to (t,s)-numbering.
     *
     *  @param lowerEdges (in):  List of incident edges lower than embNode
     *                           with respect to (t,s)-numbering.
     *
     *  @param outerFaceIt (in): Outer face.
     *
     *  @param rightFaceIt (in): Right face.
     */
    void findLeftAndRightGaps(
        coord_t&                leftGap,
        coord_t&                rightGap,
        vector<edge_list_it_t>& upperEdges,
        vector<edge_list_it_t>& lowerEdges,
        node_list_it_t          outerFaceIt,
        edge_list_it_t          stDualIt
    );


    /** @brief assigns the heights and widths of the edges to the Aux 
     *         structures
     *         VRRectLabelEdge::height*() => EGEdgeAux::mLabelHeight
     *         VRRectLabelEdge::width*()  => DGEdgeAux::mLabelWidthRight 
     *                                       DGEdgeAux::mLabelWidthLeft
     *
     *  @param  N     (in): EmbeddedBCTreeNode for the current block.
     */
    void handleEdgeLabels(EmbeddedBCTreeNode& N);

    /** @brief finds the left and the right extent (width) of the rectangular 
     *         area to accommodate the edge labels.
     *         A supporting function to handleEdgeLabels()
     *
     *  @param labelEdge (in): the edge in the original VRREctLabelGraph.
     *
     *  @param cwIsLeft  (in): true if VRRectLabelEdge::POSCLOCKWISE is
     *                         to the left of the edge in the current 
     *                         embedding of the block with its orientation
     *                         fixed (unflipped).
     *
     *  @param  aux_de   (out):the Aux data structure to which the label
     *                         information is stored.
     */
    void assignEdgeWidths(
        VRRectLabelEdge& labelEdge,
        bool             cwIsLeft,
        DGEdgeAux&       aux_de
    );

    /** @brief finds the left and the right extent (width) of the each
     *         individual edge label based on their positioning.
     *         A helper function to handleEdgeLabels()
     *         
     *  @param  type       (in): position of the label along the edge.
     *
     *  @param  width      (in): width of the label.
     *
     *  @param  widthLeft  (out):the left extent of the width of the label.
     *
     *  @param  widthRight (out):the right extent of the width of the label.
     */
    void assignEdgeWidth(
        VRRectLabelEdge::posType  type,
        coord_t                   width,
        bool                      cwIsLeft,
        coord_t&                  widthLeft,
        coord_t&                  widthRight
    );

    /** @brief a dispatch function to handle the heights and widhts of
     *         the absorbee blocks.
     *
     *  @param  N       (in): EmbeddedBCTree::ExplorationNode for the
     */
    void handleChildBlocks(EmbeddedBCTree::ExplorationNode& expNode);


    /** @brief represents how the absorbor accommodates absorbee blocks.
     *         found in findAbsorbingType() and used by handleChildBlocks().
     */
    enum absorbingType {
        INNER_FACE_TOP_CV,
        INNER_FACE_BOTTOM_CV,
        INNER_FACE_LEFT_CV,
        INNER_FACE_RIGHT_CV,
        OUTER_FACE_TOP_CV,
        OUTER_FACE_BOTTOM_CV,
        OUTER_FACE_LEFT_CV,
        OUTER_FACE_RIGHT_CV,
        K2_TOP_CV,
        K2_BOTTOM_CV,
    };

    /** @brief find the type of the absorber face and the incident
     *         CW/CCW edges based on their inner/outer classification of 
     *         the face and the (t,s) number of the nodes incident to 
     *         the edges.
     *
     *  @param   UF  (in): UnificationFace in which the absorber information
     *                     is stored.
     *  @return type of the absorber.
     */
    enum absorbingType findAbsorbingType(UnificationFace& UF);


    /** @brief finds the edge incident to the unification face and the cut
     *         vertex on the clockwise side relative to the embedding of the 
     *         block.
     *  @param   UF  (in): UnificationFace in which the absorber information
     *                     is stored.
     *  @return  EmbeddedEdge on the clockwise size in the block embedding.
     */
    EmbeddedEdge& blockEdgeCW(UnificationFace& UF);


    /** @brief finds the edge incident to the unification face and the cut
     *         vertex on the counter-clockwise side relative to the embedding
     *         of the block.
     *        
     *  @param   UF  (in): UnificationFace in which the absorber information
     *                     is stored.
     *  @return  EmbeddedEdge on the counter-clockwise size in the block
     *           embedding.
     */
    EmbeddedEdge& blockEdgeCCW(UnificationFace& UF);


    /** @brief the following 10 functions allocate spaces for the absorbees
     *         based on absorbingType of the absorber.
     *         
     *   ETNodeAux::mHeight      
     *   EGNodeAux::mLabelHeight ==>
     *                          EGEdgeAux::mChildrenHeightUpperRight
     *                          EGEdgeAux::mChildrenHeightLowerRight
     *                          EGEdgeAux::mChildrenHeightUpperLeft
     *                          EGEdgeAux::mChildrenHeightLowerLeft
     *                          ETNodeAux::mExtraHeightBottom
     *
     *   ETNodeAux::mWidth  ==>
     *                          DGEdgeAux::mChildrenWidthUpperRight
     *                          DGEdgeAux::mChildrenWidthLowerRight
     *                          DGEdgeAux::mChildrenWidthUpperLeft
     *                          DGEdgeAux::mChildrenWidthLowerLeft
     *                          ETNodeAux::mExtraWidthBottom
     *
     *   @param  UG (in): sorted unification group to be processed.
     */
    void pass1ChildBlocksInnerFaceTopCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksInnerFaceBottomCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksInnerFaceRightCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksInnerFaceLeftCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksOuterFaceTopCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksOuterFaceBottomCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksOuterFaceRightCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksOuterFaceLeftCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksK2TopCV(SortedUnificationGroup& UG);
    void pass1ChildBlocksK2BottomCV(SortedUnificationGroup& UG);


    /** @brief finds the required width and the height of the minimum 
     *         rectangular area to accommodate the absorbee blocks
     *         arranged horizontally under the cut vertex.
     *         The absorbee blocks are specified in the range between the start
     *         index and the end index.
     * 
     *   @param  UG         (in):  sorted unification group to be processed.
     *
     *   @param  startIndex (in):  start index (inclusive) of the absorbee
     *                             in the unification faces list.
     *
     *   @param  endIndex   (in):  end index (exclusive) of the absorbee.
     *                             in the unification faces list.
     *
     *   @param sumWidths   (out): the width of the rectangular area.
     * 
     *   @param maxHeight   (out): the height of the rectangular area.
     */
    void findSumWidthsAndMaxHeight(
        SortedUnificationGroup& UG,
        long                    startIndex,
        long                    endIndex,
        coord_t&                sumWidths,
        coord_t&                maxHeight
    );

    /** @brief finds the best split point in the ordered absorbees list
     *         to minimize the aspect ratio and the area of the resultant
     *         rectangular area to accommodate the absorbee blocks in two rows
     * 
     *   @param  UG         (in):  sorted unification group to be processed.
     */
    size_t findBestSplitPointInChildren(SortedUnificationGroup& UG);

    /** @brief support functions to get all the aux structures of the specified
     *         unification face.
     *
     *  @param   UF        (in):  UnificationFace in which the absorber 
     *                            information is stored.
     *  @return EGNodeAux for the cut vertex
     *  @return EGEdgeAux for the edge on CW side of UF
     *  @return DGEdgeAux for the edge on CW side of UF
     *  @return EGEdgeAux for the edge on CCW side of UF
     *  @return DGEdgeAux for the edge on CCW side of UF
     *  @return DGFaceAux for the unification face
     */
    EGNodeAux& findAuxsCV(UnificationFace& UF);
    EGEdgeAux& findAuxsEECW(UnificationFace& UF);
    EGEdgeAux& findAuxsEECCW(UnificationFace& UF);
    DGEdgeAux& findAuxsDECW(UnificationFace& UF);
    DGEdgeAux& findAuxsDECCW(UnificationFace& UF);
    DGFaceAux& findAuxsFace(UnificationFace& UF);


    /** @brief based on the parameters stores in the Aux structures, calculate
     *         the edge and node costs of the block embedding (EmbeddedGraph)
     *         and its dual (DualGraph) for the scheduling algorithm.
     *
     *      EGNodeAux::mLabelHeight                => EGNodeAux::mCost
     *
     *      EGEdgeAux::mLabelHeight
     *      EGEdgeAux::mChildrenHeightUpperRight
     *      EGEdgeAux::mChildrenHeightLowerRight
     *      EGEdgeAux::mChildrenHeightUpperLeft
     *      EGEdgeAux::mChildrenHeightLowerLeft    => EGEdgeAux::mCost
     *                                        
     *      DGEdgeAux::mChildrenWidthUpperLeft
     *      DGEdgeAux::mChildrenWidthLowerLeft
     *      DGEdgeAux::mLabelWidthLeft             => DGEdgeAux::mLeftInset
     *
     *      DGEdgeAux::mChildrenWidthUpperRight
     *      DGEdgeAux::mChildrenWidthLowerRight
     *      DGEdgeAux::mLabelWidthRight            => DGEdgeAux::mRightInset
     *
     *      DGEdgeAux::mLeftInset
     *      DGEdgeAux::mRightInset,
     *      DGEdgeAux::mWidthLabelShareUpper
     *      DGEdgeAux::mWidthLabelShareLower       =>  DGEdgeAux::mCost
     *
     *  @param N   (in): EmbeddedBCTreeNode that contains the current block.
     *
     *  @param stDualIt (in): {s*,t*}, the dual edge of {s,t}.  It is excluded
     *         from the scheduling algorithm.
     *
     */
    void calculateCosts(
        EmbeddedBCTreeNode& N,
        edge_list_it_t stDualIt
    );

    /** @brief a helper function to handleEdgeLabels() to calculate the overall
     *         height required to accommodate all the edge labels along an
     *         edge.
     *        
     *  @param upper   (in): the height of the upper label
     *
     *  @param middle  (in): the height of the middle label
     *
     *  @param lower   (in): the height of the lower label
     *
     *  @return overall height
     */
    coord_t getEdgeLabelHeight(coord_t upper, coord_t middle, coord_t lower);


    /** @brief perform BFS-based 1-pass scheduling algorithm to find the
     *         early start time for each node in the block embedding.
     *         After the algorithm, EGNodeAux::mStart contains a valid
     *         value.
     *
     *  @param N        (in): EmbeddedBCTreeNode that contains the current 
     *                        block.
     *
     *  @param bottomit (in): the bottom node t.
     */
    void scheduleEG(EmbeddedBCTreeNode& N, node_list_it_t bottomit);


    /** @brief perform BFS-based 1-pass scheduling algorithm to find the
     *         early start time for each node of the dual graph.
     *         After the algorithm, DGFaceAux::mStart contains a valid
     *         value.
     *
     *  @param N             (in): EmbeddedBCTreeNode that contains the current
     *                             block.
     *
     *  @param rightIt       (in): the start face {t*} for the algorithm.
     *  @param stDualrightIt (in): the edge {s*,t*} to be excluded.
     */
    void scheduleDG(
        EmbeddedBCTreeNode& N,
        node_list_it_t      rightIt,
        edge_list_it_t      stDualIt
    );


    /** @brief assign X and Y coordinate values to the nodes and the edges
     *         based on the start values and the other parameters.
     *         It also sets ETNodeAux::mWidth and mHeight, which are the 
     *         minimal required rectangular area for the block.
     *
     *      EGNodeAux::mEarlyStart => EGNodeAux::mY
     *      DGFaceAux::mEarlyStart => DGFaceAux::mX
     *
     *      
     *      DGFaceAux::mEarlyStart
     *      DGEdgeAux::mLeftInset
     *      DGEdgeAux::mRightInset => EGEdgeAux::mX
     *
     *  @param embTreeNode   (in): EmbeddedBCTreeNode that contains the current
     *
     *  @param stIt          (in): {s,t}
     *
     *  @param stDualIt      (in): {s*,t*}
     *
     *  @param bottomIt      (in): the bottom node t.
     */
    void assignCoordinates(
        EmbeddedBCTreeNode& embTreeNode,
        edge_list_it_t      stIt,
        edge_list_it_t      stDualIt,
        node_list_it_t      bottomIt
    );


    /** @brief determins the layout type based on the following:
     *        mBlockWidth
     *        mBlockHeight
     *        mExtraWidthBottom
     *        mExtraHeightBottom
     *        mExtraWidthRight
     *        mExtraHeightRight
     *
     *  @param aux      (in/out): Tree node aux structure
     */
    void determineTopLevelLayoutType(ETNodeAux& aux);


    /** @brief normalize the local coordinates inthe block and determine
     *         the width and the height of this local VR.
     *
     *  @param embTreeNode   (in): EmbeddedBCTreeNode that contains the current
     *
     *  @param bottomLabelHeight (in): Height of the bottom node label
     */
    void normalizeBlockCoordinates(
        EmbeddedBCTreeNode& embTreeNode,
        coord_t             bottomLabelHeight
    );


    void normalizeBlockCoordinatesNoExtra(
        EmbeddedBCTreeNode& embTreeNode
    );


    void normalizeBlockCoordinatesExtraRight(
        EmbeddedBCTreeNode& embTreeNode
    );


    void normalizeBlockCoordinatesExtraBottom(
        EmbeddedBCTreeNode& embTreeNode,
        coord_t             bottomLabelHeight
    );


    void normalizeBlockCoordinatesBothExtras(
        EmbeddedBCTreeNode& embTreeNode,
        coord_t             bottomLabelHeight
    );



    /******************************/
    /*  FUNCTIONS FOR PASS 1 END  */
    /******************************/

    /******************************/
    /* FUNCTIONS FOR PASS 2 BEGIN */
    /******************************/

    /** @brief exploring through the EmbeddedBCTree::ExplorationTree in the, 
     *         top-down manner, fix the orientation of each block and set
     *         the X and Y coordinates of the nodes and the edges in the global
     *         coordinate system.
     */
    void performPass2TopDown();


    /** @brief top-level dispatch function for each node of pass 2.
     *
     *  @param  N       (in): EmbeddedBCTree::ExplorationNode for the
     *                        current block.
     */
    void processOneTreeNodePass2(EmbeddedBCTree::ExplorationNode& N);


    /** @brief the first step of pass 2. Translate the X and Y coordinates of
     *         the nodes and the edges according to ETNodeAux::mAnchorX1 and
     *         mAnchorY1.
     *
     *  @param  expNode (in): EmbeddedBCTree::ExplorationNode for the
     *                        current block.
     */
    void translate(EmbeddedBCTree::ExplorationNode& expNode);


    /** @brief the following 10 functions set 
     *         ETNodeAux::mAnchorX1, ETNodeAux::mAnchorX2, 
     *         ETNodeAux::mAnchorY1, and ETNodeAux::mAnchorY2
     *         in the global coordinate system based on absorbingType of the
     *         absorber assuming the absorber's block embedding is in the
     *         normal orientation.
     *
     *  @param  UG (in): sorted unification group to be processed.
     */
    void pass2ChildBlocksInnerFaceTopCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksInnerFaceBottomCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksInnerFaceRightCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksInnerFaceLeftCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksOuterFaceTopCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksOuterFaceBottomCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksOuterFaceRightCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksOuterFaceLeftCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksK2TopCV(SortedUnificationGroup& UG);
    void pass2ChildBlocksK2BottomCV(SortedUnificationGroup& UG);

    /** @brief this specifies the horizontal direction of arrangement of the
     *         absorbees under or over the cut vertex.
     *
     *         LtoR:  The order of the absorbees is from left to right.
     *
     *         RtoL:  The order of the absorbees is from right to left and
     *                the embedding of the absorbees are flipped horizontally
     *                relative to the absorber.
     */
    enum assignDirectionType   { LtoR, RtoL   };

    /** @brief this specifies the vertical orientation and the location of the
     *         cut vertex in the absorbees. 
     *         TOP:    The cut vertex is at the top (higher coordinate) of
     *                 the embedding of the absorbees.
     *         BOTTOM: The cut vertex is at the bottom (lower coordinate) of
     *                 the embedding of the absorbees, and the embedding
     *                 of the absorbees are vertically flipped relative to 
     *                 the absorber.
     */
    enum topNodeLocationType   { TOP,  BOTTOM };

    /** @brief this specifies how the horizontal gaps at the both ends of the
     *         absorbee list.
     */
    enum terminalType          {
                                 FullGapLeftFullGapRight,
                                 FullGapLeftHalfGapRight,
                                 FullGapLeftNoGapRight,
                                 HalfGapLeftFullGapRight,
                                 NoGapLeftNoGapRight,
                               };


    /** @brief helper function to pass2ChildBlocks****().
     *         This assigns ETNodeAux::mAnchorX1, ETNodeAux::mAnchorX2, 
     *         ETNodeAux::mAnchorY1, and ETNodeAux::mAnchorY2 in 
     *         the global coordinate system, and
     *         sets ETNodeAux::mFlippedHorzontally and 
     *         ETNodeAux::mFlippedVertically relative to the abosrber's
     *         block embedding.
     *
     *  @param  UG         (in): sorted unification group to be processed.
     *
     *  @param  startIndex (in): start index in the absorbee list
     *
     *  @param  endIndex   (in): end index in the absorbee list
     *
     *  @param  dirType    (in): horizontal direction along which the absorbees
     *                           are arranged
     *  @param  topNodeLocation 
     *                     (in): vertical location of the cut vertex and also
     *                           the vertical orientation of the absorbee
     *                           blocks relative to the absorber.
     *
     *  @param  terminal   (in): how to assign the gaps at both ends of the 
     *                           absorbee list
     *
     *  @param  posL       (in): the left most position in the global
     *                           coordinate system allowed to the list
     *                           of the absorber blocks
     *
     *  @param  posR       (in): the right most position in the global
     *                           coordinate system  allowed to the list
     *                           of the absorber blocks
     *
     *  @param  cvY        (in): the vertical position in the global coordinate
     *                           system for the cut vertex. It is either the
     *                           highest or lowest position allowed to the list
     *                           of the absorber blocks.
     */
    void assignXYToAbsorbees(
        SortedUnificationGroup&  UG,
        long                     startIndex,
        long                     endIndex,
        enum assignDirectionType dirType,
        enum topNodeLocationType topNodeLocation,
        enum terminalType        terminal,
        coord_t                  posL,
        coord_t                  posR,
        coord_t                  cvY
    );

    /** @brief a helper function for assignXYToAbsorbees to find the 
     *         sum of widhts of the absorbee blocks.
     *
     *  @param  UG         (in): sorted unification group to be processed.
     *
     *  @param  startIndex (in): start index in the absorbee list
     *
     *  @param  endIndex   (in): end index in the absorbee list
     *
     *  @return the sum of the widths.
     */
    coord_t findSumWidthsWithoutGaps(
        SortedUnificationGroup& UG,
        long                    startIndex,
        long                    endIndex
    );


    /** @brief updates the orientation of the block embedding.
     *         Update the coodinates for the edges and nodes.
     *         Update ETNodeAux::mAnchorX1, ETNodeAux::mAnchorX2 
     *         ETNodeAux::mAnchorY1, and ETNodeAux::mAnchorY2 
     *         for all the absorbees under the current exploration node.
     *         Update ETNodeAux::mHorizontallyFlipped and
     *         ETNodeAux::mHorizontallyFlipped according to the absorber's
     *         real orientation.
     *
     *  @param  expNode (in): EmbeddedBCTree::ExplorationNode for the
     *                        current block.
     */
    void correctOrientations(EmbeddedBCTree::ExplorationNode& expNode);

    /******************************/
    /*  FUNCTIONS FOR PASS 2 END  */
    /******************************/


    /** @brief assigns VRRectLabelNode::mXleft, 
     *                 VRRectLabelNode::mXright, 
     *                 VRRectLabelNode::mY,
     *                 VRRectLabelEdge::mX,
     *                 VRRectLabelEdge::mY1, and
     *                 VRRectLabelEdge::mY2
     *                 VRRectLabelNode::mY is for the center of the label
     */
    void findVRCoordinates();


    /** @brief helper function for findVRCoordinates().
     *         It assigns VRRectLabelNode::mY, VRRectLabelNode::mY
     *         for the nodes and edges of the original VRRectLabelGraph.
     */
    void findVRCoordinatesOfTreeNode(EmbeddedBCTree::ExplorationNode& expNode);


    /** @brief cleanup utilities in EmbeddedBCTreeNode,
     *         EmbeddedNode, EmbeddedEdge, EmbeddedFace, and DualEdge.
     */
    void cleanUp();


#ifdef UNIT_TESTS
  public:
    void debugPrint(EmbeddedBCTree::ExplorationNode& N, size_t indent);

    void debugPrintFindBottomNodeAndLeftMostFace(
        EmbeddedBCTreeNode& ETN,
        node_list_it_t&     bottomIt,
        node_list_it_t&     rightIt,
        edge_list_it_t&     stIt,
        edge_list_it_t&     stDualIt,
        size_t              indent
    );

    void debugPrintOrientEGandDG(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintHandleNodeLabels(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintHandleEdgeLabels(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintHandleChildBlocks(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintCalculateCosts(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintScheduleEG(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintScheduleDG(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintAssignCoordinates(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );

    void debugPrintTranslate(
        EmbeddedBCTreeNode& ETN,
        size_t              indent
    );
 

#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_VIS_REP_FINDER_HPP_*/
