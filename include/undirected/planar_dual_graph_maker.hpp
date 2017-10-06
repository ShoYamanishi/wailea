#ifndef _WAILEA_UNDIRECTED_PLANAR_DUAL_GRAPH_MAKER_HPP_
#define _WAILEA_UNDIRECTED_PLANAR_DUAL_GRAPH_MAKER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file undirected/planar_dual_graph_maker.hpp
 *
 * @brief It takes as input a simple bi-connected planar graph with a planar 
 *        embedding, and generates two graph structures useful to draw the 
 *        graph.
 *        As an input, it receives the planar embedding as in an ordered list
 *        of incident edges in each node.
 *        It then generates data required for the following.
 *
 *        - Enumerate all the faces
 *        - Explore the edges incident to a face in a given orientation.
 *        - Find the adjacent face across an edge of a given face.
 *
 *        The data come in two graph objects: EmbeddedGraph and DualGraph.
 *
 *        EmbeddedGraph is isomorphic to the original graph but has additional
 *        information in its edges. Its node and edge objects are of 
 *        EmbeddedNode and EmbeddedEdge respectively. EmbeddedNode has a 
 *        pointer to the corresponding node in the original graph.
 *        EmbeddedEdge has a pointer to the corresponding edge in the original
 *        graph. It also has two HalfEdge orjects and a pointer to the 
 *        corresponding dual edge in DualGraph.
 *        A HalfEdge is an oriented edge with designated source and sink nodes.
 *        A HalfEdge comes in a pair, which belong to the same edge.
 *        Each in the pair is oriented in the opposite of each other.
 *        A HalfEdge has a link to the previous HalfEdge of the same face, and
 *        another link to the next. The link data structure forms the oriented
 *        incident edges of the face, which is of EmbeddedFace object in
 *        DualGraph.
 *
 *        DualGraph represents the dual graph of the given input graph with
 *        EmbeddedFace objects as its nodes, and DualEdges as its edges.
 *        An EmbeddedFace's incidentEdges() is an ordered list of pointers to
 *        DualEdges. Its ordering is in accordance with the surrounding 
 *        HalfEdges in EmbeddedGraph. It also has a list of surrounding
 *        HalfEdges represented by two lists:
 *        mCycleEdges and mCycleHalfEdgesOn1.
 *        A DualEdge has a pointer to the corresponding EmbeddedEdge in
 *        EmbeddedGraph.
 *
 *        It seems use of raw pointer is strongly discouraged circa C++14.
 *        A HalfEdge is pointed to not by a pointer, but by two variables:
 *        an interator to the owning EmbeddedEdge in edge_list_it_t, and
 *        a bool which indicates which of two HaldEdges in EmbeddedEdge is it.
 *
 *        For example a HalfEdge's previous HalfEdge is described in the
 *        following two variables.
 *              edge_list_it_t mPrevEdge;
 *              bool           mPrevHalfEdgeOn1;
 *
 *
 *  @remark on construction:
 *
 *       To achieve O(|N|) processing time and space, we make use of two
 *       types of lists: a list of nodes (nodesPending in 
 *       PlanarDualGraphMaker::initializeUnprocessedQueues() ), and a list of
 *        edges per node (EmbeddedNode::mEdgesPending).
 *       Those lists keep track of nodes and edges that have not bee completely
 *       processed.
 *
 *       Initially, all the nodes are in nodesPending. And all the edges
 *       are in mEdgesPending of all the incident nodes. Every edge occurs
 *       twice in total, as it has two incident ndoes.
 *       
 *       We start exploring the surrounding edges of a face from an edge of 
 *       a node. Everytime an edge is explored, it is removed from 
 *       mEdgesPending of its source node. If a node's mEdgesPending becomes
 *       empty, then all the edges (and hence faces incident to the node)
 *       have been explored, and it is removed from nodesPending.
 *       An iteration starts with finding an initial edge, and ends with
 *       finding it again, then all the edges discorvered during the iteration
 *       form a face cycle in the orientation of exploration.
 *
 *       A new iteration starts with the end edge of the previous iteration
 *       but in the opposite direction if it has not been processed.
 *       If the node has no unprocessed edge, then it is removed from 
 *       nodesPending, and a new unprocessed node is picked from the list.
 *       The processes continues until nodesPending becomes empty.
 */

namespace Wailea {

namespace Undirected {

/** @class EmbeddedGraph
 *
 *  @brief represent a copy of the given simple biconnected planar graph
 *         with an embedding. It has (directed) half edges that form
 *         directed cycle around the faces.
 *
 *         It has nodes and edges in EmbeddedNode and EMbeddedNode class
 *         respectively.
 */
class EmbeddedGraph : public Graph {
    // EmbeddedNode
    // EmbeddedEdge

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests ;
#endif

};


/** @class EmbeddedNode
 *
 *  @brief represents a copied node in EmbeddedGraph.
 */
class EmbeddedNode : public Node {

#ifdef UNIT_TESTS
public:
#else
private:
#endif

    /** @brief list of unprocessed edges. Used in constuction.
     */
    list<edge_list_it_t> mEdgesPending;

    /** @brief iterator into the list of pending nodes. Used in construction.
     */
    list<node_list_it_t>::iterator mItIntoNodesPending;

    friend class PlanarDualGraphMaker;

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests ;
#endif

};


/** @class HalfEdge
 *
 *  @brief represents a directed half edge.
 *         It has the designated source and destination nodes.
 *         It has linkes to the previous and the next half edges along the
 *         face cycle.
 *         It has a pointer to the face it blongs to.
 */
class HalfEdge {

public:
    /** @brief back pointer to the owner of the half edge
     */
    edge_list_it_t mEmbeddedEdge;

    /** @brief the other half edge
     */
    bool           mTheOtherHalfOn1;

    /** @brief source node
     */
    node_list_it_t mSrcNode;

    /** @brief destination node
     */
    node_list_it_t mDstNode;

    /** @brief pointer to the corresponding face (node) in the dual graph.
     */
    node_list_it_t mEmbeddedFace;

    /** @brief previous edge of this half edge on the face cycle
     */
    edge_list_it_t mPrevEdge;

    /** @brief true if the corresponding half edge is HalfEdge1 of mPrevEdge
     *         false  if the corresponding half edge is HalfEdge2 of it.
     */
    bool           mPrevHalfEdgeOn1;

    /** @brief next edge of this half edge on the face cycle
     */
    edge_list_it_t mNextEdge;

    /** @brief true if the corresponding half edge is HalfEdge1 of mNextEdge
     *         false  if the corresponding half edge is HalfEdge2 of it.
     */
    bool           mNextHalfEdgeOn1;

#ifdef UNIT_TESTS
public:
#else
private:
#endif

    /** @brief iterator into the list of pending edges of the src node.
     *         Used in construction.
     */
    list<edge_list_it_t>::iterator
                   mItIntoEdgesPending;

    friend class PlanarDualGraphMaker;

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests ;
#endif

};


/** @class EmbeddedEdge
 *
 *  @brief represents a copied edge in EmbeddedGraph.
 *         It contains two half edges in the opposite direction of each other.
 *         It points to the corresponding dual edge in the dual graph.
 */
class EmbeddedEdge : public Edge {

public:
    /** @brief directed half edge 1
     */
    HalfEdge       mHalfEdge1;

    /** @brief directed half edge 2
     */
    HalfEdge       mHalfEdge2 ;

    /** @brief pointer to the corresponding dual edge crossing this edge.
     */
    edge_list_it_t mDualEdge;

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests ;
#endif

};


/** @class DualGraph
 *
 *  @brief represents a dual graph of a given biconnected plane graph.
 *         It contains faces and dual edges.
 */
class DualGraph : public Graph {
    // EmbeddedFace
    // DualEdge

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests ;
#endif

};


/** @class EmbeddedFace
 *
 *  @brief represents a face in a dual graph.
 *         It contains an ordered list of surrounding half edges.
 *         The incident dual edge list is ordered in accordance with the
 *         surrounding half edges.
 */
class EmbeddedFace : public Node {
public:
    /** @brief ordered list of surrounding half edges in EmbeddedGraph.
     *         the order is aligned with the order if incident dual edge list
     */
    list<edge_list_it_t> mCycleEdges;

    /** @brief true if the half edge is HalfEdge1 of the corresponding
     *         EMbeddedEdge in mCycleEdges.
     *         false if the half edge is HalfEdge2.
     */
    list<bool>           mCycleHalfEdgesOn1;

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests;
#endif

#ifdef USE_TEST_GRAPH_GENERATOR
  public:
    long mDebugNum;
#endif
};


/** @class DualEdge
 *
 *  @brief represents a dual edge in a dual graph.
 *         It contains a pointer to the corresponding edge to an EmbeddedEdge
 *         in EmbeddedGraph.
 */
class DualEdge : public Edge {

public:

    /** @brief pointer to the corresponding EmbeddedEdge that cuts across
     *         this dual edge
     */
    edge_list_it_t mEmbeddedEdge;

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests ;
#endif

};


class PlanarDualGraphMaker{

public:
    /** @brief make a dual graph representation of the given simple
     *         biconnected planar graph. It also makes a data structure
     *         as an augumented input graph with additional circularly
     *         ordered half edges and associated faces.
     *
     *  @param src (in):    input graph. It must be simple, biconnected
     *                      planar grpah, and the incident edges of each
     *                      node must be arranged to represent a combinatorial
     *                      planar embedding.
     *
     *  @param emb (out):   copy of the input graph in EmbeddedGraph,
     *                      EmbeddedNode, and EmbeddedEdge objects
     *
     *  @param dual(out):   dual graph of the given input graph. For each node,
     *                      the incident dual edges are arranged such that the
     *                      corresponding half edges in emb form the face cycle
     *
     *  @remark on inter-graph links.
     *          - Nodes and Edges in src have IGForwardLink() that point to
     *            the corresponding EmbeddedNodes and EmbeddedEdges in emb.
     *          - EmbeddedNodes and EmbeddedEdges in emb have IGBackwardLink()
     *            that point to the corresponding Nodes and Edges in src.
     *
     *  @remark if the input graph is not simple, biconnected, or planar, or
     *          if the incident edges of each node are not arranged to
     *          represent a planar embedding, the behavior of this function is
     *          undefined.
     * 
     *  @remark on complexity:
     *          This runs in O(|N|) time and space.
     *
     */
    void makeDualGraph(Graph& src, EmbeddedGraph& emb, DualGraph& dual);

private:

    void copyInputGraph(Graph& src, EmbeddedGraph& emb);

    void findNextHalfEdge(
        node_list_it_t&  sit,   // (io): source node pointer
        edge_list_it_t&  eit,   // (io): edge pointer
        bool&            HEOn1, // (io): half edge is mHaldEdge1
        node_list_it_t&  dit    // (io): destination node pointer
    );

    HalfEdge* findNextHalfEdge(
        HalfEdge&       HE,        // (in):   Current Half Edge
        EmbeddedNode&   D,         // (in):   Current Dest Node
        bool&           nextHEOn1  // (out):  Next HalfEdge is on HalfEdge1
                                   // return: Next HalfEdge
    );
        
    void findFaces(EmbeddedGraph& emb, DualGraph& dual);

    void makeOneFace(
        DualGraph&             dual,
        list<edge_list_it_t>&& cycleEdges,
        list<bool>&&           cycleHalfEdgesOn1
    );

    list<node_list_it_t> initializeUnprocessedQueues(EmbeddedGraph& emb);

    void findDualEdges(EmbeddedGraph& emb, DualGraph& dual);

    void makeForwardLinks(EmbeddedGraph& emb, DualGraph& dual);

#ifdef UNIT_TESTS
    friend class PlanarDualGraphMakerTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_PLANAR_DUAL_GRAPH_MAKER_HPP_*/
