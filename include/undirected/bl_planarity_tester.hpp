#ifndef _WAILEA_UNDIRECTED_BL_PLANARITY_TESTER_HPP_
#define _WAILEA_UNDIRECTED_BL_PLANARITY_TESTER_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/bl_planarity_tester.hpp
 *
 * @brief the main header file the users of BLPlanarityTester include.
 *        It defines BLPlanarityTester class and its subordinate classes.
 *        BLPlanarityTester has two main functions.
 *        1. Planarity test by isPlanar().
 *          It takes a graph G and G's st-ordering, and tests if G is planar or
 *          not. G must be a simple bi-connected graph, or if {s,t} is not in G
 *          then G + {s,t} is bi-connected.
 *
 *        2. Find a combinatorial embedding by findEmbedding()
 *          It takes a graph G and G's st-ordering, and finds a combinatorial
 *          embedding as in the ordered incident edge list for each node.
 *
 *        It is based on [BL76] planarity test algorithm using PQ-tree.
 *
 * @remark on the complexity
 *    The planarity test runs in O(|N|+|E|) as in [BL76].
 * 
 *   dependency: undirected/base.{hpp,cpp}
 *
 *   file organization:
 *     * undirected/bl_planarity_tester.hpp
 *                                - The users of BLPlanarityTester are to 
 *                                  include this file.
 *
 *     * undirected/bl_planarity_tester.cpp
 *         BLPlanarityTester      - top level implementation class
 *
 *         BLGraph[Node,Edge]     - used internally in BLPlanarityTester
 *                                  as a copy of the input graph.
 *
 *     * undirected/bl_tree.{hpp,cpp}
 *         BLTree                - implements PQ-tree operations.
 *
 *     * undirected/bl_tree_node.{hpp,cpp}
 *         BLTreeNode            - implements PQ-tree node data structure
 *                                  and operations.
 *     * undirected/bl_inline.hpp
 *                                - inline function definitions for BLTree
 *                                  and BLTreeNode.
 *
 *
 * @usage for a planarity test
 *
 *   BLPlanarityTester pt;
 *   Graph         G;  // The input graph to be tested.
 *
 *   Node&         s;  // source node.
 *   Node&         t;  // sink node.
 *   STNumbering st;
 *   vector<node_list_it_t> stOrder = st.getBipolarOrientation(G,s,t);
 *                     // {s,t}-orientation of the nodes of G.
 *
 *   bool planar = pt.isPlanar(G, stOrder);
 * 
 * @usage for finding a planar embedding
 *
 *   BLPlanarityTester pt;
 *   Graph         G;  // The input planar graph for which
 *                     // a planar embedding is to be found.
 *   Node&         s;  // source node.
 *   Node&         t;  // sink node.
 *   vector<node_list_it_t> stOrder = st.getBipolarOrientation(G,s,t);
 *                     // {s,t}-orientation of the nodes of G.
 *
 *   pt.findEmbedding(G, stOrder);
 *   // After the call, the nodes in G have their incident edges rearranged
 *   // according to the combinatorial embedding found.
 *
 *
 * @references
 *  [BL76] Testing for the consecutive ones property, interval graphs, and
 *       graph planarity using PQ-tree algorithms
 *       Kellogg S. Booth & George S. Lueker
 *       Journal of Computer and System Sciences archive Volume 13 Issue 3,
 *       December, 1976 Pages 335-379 Academic Press, Inc. Orlando, FL, USA
 */

namespace Wailea {

namespace Undirected {

using namespace std;

/**
 * @class BLGraph
 *
 * @brief the input graph is augumented to this class internally
 *        to be processed by PQ-tree.
 */
class BLGraph : public Graph {
  public:
    /** @brief pointer to the original target graph.
     */
    Graph*  mOriginal;
};


/**
 * @class BLGraphEdge
 *
 * @brief the input graph edges are augumented to this class internally
 *        to be processed by BLPlanarityTester.
 */
class BLGraphEdge : public Edge {

  public:

    /** @brief pointer to the corresponding leaf in the PQ-tree.
     */
    node_list_it_t mPQLeaf;
};


/**
 * @class BLGraphNode
 *
 * @brief the input graph nodes are augumented to this class internally
 *        to be processed by BLPlanarityTester.
 */
class BLGraphNode : public Node {

  public:

    /** @brief st-number of this node.
     */
    size_t               mStNumber;


    /** @brief incoming incident edge set w.r.t. the st-ordering.
     */
    list<edge_list_it_t> mIncomingEdges;


    /** @brief outgoing incident edge set w.r.t. the st-ordering.
     */
    list<edge_list_it_t> mOutgoingEdges;


    /** @brief ordered incoming incident edges of the enbedding being found.
     */
    list<edge_list_it_t> mIncomingEdgesOrdered;


    /** @brief true if the embedding has the ordered incident edges in the
     *         reverse order of the list mIncomingEdgesOrdered.
     */
    bool                 mIncomingEdgesInReverse;


    /** @brief ordered outgoing incident edges of the enbedding being found.
     */
    list<edge_list_it_t> mOutgoingEdgesOrdered;


    /** @brief true if the embedding has the ordered incident edges in the
     *         reverse order of the list mOutgoingEdgesOrdered.
     */
    bool                 mOutgoingEdgesInReverse;

};


class BLTree;
class BLTreeNode;


/** @class BLPlanarityTester
 *
 *  @brief top level class to test the planarity
 *         of a given G and an st-ordering.
 */
class BLPlanarityTester {

  public:

    /** @brief tests if the given graph is planar or not
     *
     *  @param G              (in):  simple connected graph to be planarized
     *                               G or G + {s,t} must be biconnected
     *  @param stOrder        (in):  (s,t)-ordering of the graph G.
     *     
     *  @return   true: G is planar
     *            false:G is not planar
     * 
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark on exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed, and the input graph
     *           G, and the list stOrder will be unchanged.
     *
     *  @remark on undefined behavior
     *           If G or G + {s,t} is not simple biconnected, 
     *           or if stOrder is not a
     *           valid (s,t)-ordering, then the behavior is undefined.
     *
     *  @remark on complexity
     *          The required memory and running time will be in the
     *          order of O(|N| + |E|).
     */
    bool isPlanar(
        Graph&                  G,
        vector<node_list_it_t>& stOrder
    );


    /** @brief isPlanar without new templates for experiments.    
     */
    bool isPlanarOldAlg(
        Graph&                  G,
        vector<node_list_it_t>& stOrder,
        size_t&                 index
    );


    /** @brief finds an embedding of the given planar graph
     *
     *  @param G          (in/out):  simple connected graph to be planarized
     *                               G or G + {s,t} must be biconnected
     *                               On return, the incident edge list
     *                               of each node in G is ordered to represent
     *                               the embedding.
     *
     *  @param stOrder        (in):  (s,t)-ordering of the graph g.
     *     
     *  @return   true: G is planar
     *            false:G is not planar
     *
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark on exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed, and the input graph
     *           G, and the list stOrder will be unchanged, except that
     *           the incident edges list of some nodes in G may have been
     *           rearranged, but their integrity is kept.
     *
     *  @remark on undefined behavior
     *           If G or G + {s,t} is not simple biconnected, 
     *           or if stOrder is not a
     *           valid (s,t)-ordering, then the behavior is undefined.
     *
     *  @remark on complexity
     *          The required memory and running time will be in the
     *          order of O(|N| + |E|).
     */
    bool findEmbedding(
        Graph&                  G,
        vector<node_list_it_t>& stOrder
    );


#ifdef UNIT_TESTS
  public:
#else
  private:
#endif

    /** @brief make an internal copy of the input graph.
     *         The copy is in BLGraph[Node,Edge]
     *         It also makes a copy st-ordering (bipolar orientation)
     *         of the copied nodes.
     *
     *  @param   srcGraph      (in):  input graph G
     *
     *  @param   stOrderSrc    (in):  bipolar orientation for G
     *
     *  @param   dstGraph      (out): copy of G
     *
     *  @param   stOrderDst    (out): bipolar orientation for the nodes in
     *                                dstGraph
     */
    void copyInputGraph(
        Graph&                  srcGraph,
        vector<node_list_it_t>& stOrderSrc,
        BLGraph&                dstGraph, 
        vector<node_list_it_t>& stOrderDst
    );


    /** @brief   find a set of PQ-nodes of L type that correspond to
     *           the given set of graph edges.
     *
     *  @param   edgeList        (in): graph edges in BLGraphEdge 
     *                                 of the copied graph.
     *
     *  @return  corresponding PQ-tree nodes of L type.
     *
     */
    vector<node_list_it_t> fromGraphEdgesToPQLeaves(
        list<edge_list_it_t>& edgeList
    );


    /** @brief peforms BubbleUp() in [BL76]. It find a superset of the
     *         pertinent tree.
     *         After a call, following holds for the pertinent nodes.
     *         - mGeneration is up-to-date.
     *         - mPertinentChildrenCount has the correct value.
     *         - mParent points to the correct parent.
     *
     *         And following holds for the non-pertinent nodes.
     *         - mGeneration is NOT up-to-date.
     *
     *  @param pqTree          (in): PQ-tree (BLTree)
     *
     *  @param pertinentLeaves (in): Set of PQ-tree leaves that are to be
     *                               consecutively arranged.
     *
     *  @param virtualRoot     (out): Virtual root if one is created.
     *                                pqTree.nil() if no virtual root is
     *                                created.
     *
     *  @return true :  bubble up is successful
     *          false : the given graph is not planar
     *
     *  @ramarks If there is a remaining block of blocked nodes, it means
     *           they are children of an unknown Q node located in the 
     *           middle of its sibling list.
     *           In this case,  it creates a dummy tree node of 
     *           VirtualRootType and it lets those orphans point to it.
     *           The virtual root is used to hold the following values.
     *               mPertinentChildrenCount <= Set in bubbleUp()
     *               mFullChild
     *               mSinglyPartialChild1
     *               mSinglyPartialChild2
     *               mPertinentLeavesCount
     *               (mCDPartialChild: In theory this does not occur.) 
     *           Those values are used in templateQ3() in applyTemplates().
     *
     */
    bool bubbleUp(
        BLTree&                 pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         virtualRoot
    );


    /** @brief put the parent of the current node to the FIFO queue
     *         if it has not been done so.
     *
     *  @param Q         (in):     FIFO queue
     *
     *  @param pqTree    (in):     PQ tree
     *
     *  @param parentIt  (in):     pointer to the parent
     *
     *  @param offTheTop (in/Out)  flag to indicate if the tree root is found.
     */
    void queueParentIfNecessaryBubbleUp(
        list<node_list_it_t>& Q,
        size_t&               Qsize,
        BLTree&               pqTree,
        node_list_it_t        parentIt,
        size_t&               offTheTop
    );


    /** @brief checks if the node can be unblocked and if so, it unblocks
     *         it and its siblings as much as possible.
     *
     *  @param  pqTree     (in):    PQ-tree
     *
     *  @param  X          (in):    the current node
     *
     *  @param  blockCount (in/out):current number of blocks of 'blocked'
     *                              children
     *  @param Q           (in):   FIFO queue
     *
     *  @param offTheTop  (in/Out) flag to indicate if the tree node (not
     *                             necessarily the pertinent tree node)
     *                             is found.
     */
    void tryUnblockingPertinentNodes(
        BLTree&               pqTree,
        BLTreeNode&           X,
        size_t&               blockCount,
        list<node_list_it_t>& Q,        
        size_t&               Qsize,
        size_t&               offTheTop
    );


    /** @brief tries to unblock the siblings of the unblocked node
     *
     *  @param  pqTree      (in):      PQ-tree
     *
     *  @param  X           (in):      the current node
     *
     *  @param  sibIt       (in):      the sibling of the current node
     *                                 to be tried
     *
     *  @param  blockCount  (in/out):  current number of blocks of 'blocked'
     *                                 children
     */
    void unblockSiblings(
        BLTree&        pqTree,
        BLTreeNode&    X,
        node_list_it_t sibIt,
        size_t&        blockCount
    );


    /** @brief makes a virtual root and put the orphans point to it.
     *
     *  @param  pqTree      (in):      PQ-tree
     *
     *  @param  offQ       (in):       list of all the pertinent nodes that 
     *                                 have been processed. The blocked nodes
     *                                 are somewhere in this list.
     *
     *  @return the pointer to the newly created virtual root.
     */
    node_list_it_t makeVirtualRootAndPutOrphansUnder(
        BLTree&               pqTree,
        list<node_list_it_t>& offQ
    );


    /** @brief applies the templates from the pertinent leaves up to
     *         the pertinent root.
     *         After a call to it, all the pertinent leaves are circularly 
     *         consecutively arranged.
     *
     *  @param   pqTree          (in): PQ-tree
     *
     *  @param   pertinentLeaves (in): Pertinent leaves
     *
     *  @param   pertinentRoot   (out): Pertinent root
     *
     *  @return  true : if the application is successful.
     *           false : reduction not possible.
     *
     *
     *  @remarks After the call, the following are set in the pertinent nodes.
     *               - mPertinentChildrenCount 
     *               - mPertinentChildrenCountProcessed
     *                   At the beginning of the call, it was zero.
     *                   It gets incremented everytime its child is removed
     *                   from the FIFO and processed. 
     *                   When it becomes equal to  mPertinentChildrenCount,
     *                   it is put to FIFO.
     *                   This ensures all the children have been processed
     *                   before the parent gets processed.
     *
     *               - mPertinentType : Full, SinglyPartial, CDPartial, or
     *                                  Empty.
     *                                    
     *               - mFullChild     : Set to one full child if there is any.
     *               - mSinglyPartialChild1 
     *                                : Set to one partial child if there is
     *                                  any.
     *               - mSinglyPartialChild2
     *                                : Set to another partial child if there
     *                                  is any.
     *               - mCDPartialChild: Set to one complementarily doubly 
     *                                  partial child if there is any.
     */
    bool applyTemplates(
        BLTree&                 pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         pertinentRoot
    );


    /** @brief applyTemplates without new templates for experiments
     */
    bool applyTemplatesOldAlg(
        BLTree&                 pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         pertinentRoot
    );


    /** @brief   put the parent node of the given node to the queue
     *           if all of its pertinent children have been dequeued.
     *
     *  @param   Q         (in): FIFO queue
     *
     *  @param   pqTree    (in): PT-tree
     *
     *  @param   X         (in): PQ-tree node
     *
     *  @remarks the parent of a pertinent node is put to the queue when
     *           the all of its pertinent children have been dequeued and
     *           the last one is about to be processed.
     *           This condition is detected if 
     *           mPertinentChildrenCountProcessed becomes equal to 
     *           mPertinentChildrenCount.
     */
    void queueParentIfNecessaryApplyTemplates(
        list<node_list_it_t>& Q,
        size_t&               Qsize,
        BLTree&               pqTree,
        BLTreeNode&           X
    );


    /** @brief removes the pertinent subtree of the PQ-tree after the reduction
     *         have been made, and if the pertinent tree is not a
     *         complementarily doubly partial type. After a call to this, the
     *         subtree is replaced with an empty P-node, which will be used as
     *         the attachment node for the new L nodes.
     *
     *  @param   pqTree          (in): PQ-tree
     *
     *  @param   pertinentRoot   (in): root of the pertinent subtree to be
     *                                 removed
     *
     *  @return  attachment node for the new L nodes
     */
    node_list_it_t removePertinentTree(
        BLTree&          pqTree,
        node_list_it_t   pertinentRoot
    );


    /** @brief removes the pertinent children of the tree of complemenrarily
     *         doublyPartial type.
     *
     *  @param   pqTree     (in): PQ-tree
     *
     *  @param   treeRoot   (in): root of the PQ-tree
     *
     *  @return  attachment node for the new L nodes
     */
    node_list_it_t removePertinentTreeCDPartial(
        BLTree&        pqTree,
        BLTreeNode&    treeRoot
    );


    /** @brief removes the pertinent children of the root of QType or Virtual
     *         RootType.
     *
     *  @param   pqTree          (in): PQ-tree
     *
     *  @param   pertinentRoot   (in): root of the pertinent subtree to be
     *                                 removed
     *
     *  @return  attachment node for the new L nodes
     */
    node_list_it_t removePertinentTreeQType(
        BLTree&               pqTree,
        BLTreeNode&           R
    );

    node_list_it_t removePertinentTreePType(
        BLTree&        pqTree,
        BLTreeNode&    pertinentRoot
    );


    node_list_it_t removePertinentTreeQTypeMiddle(
        BLTree&        pqTree,
        BLTreeNode&    pertinentRoot
    );


    /** @brief removes the full node and its descendands.
     *
     *  @param   pqTree          (in): PQ-tree
     *
     *  @param   topLevelNodes   (in): list of full nodes to be removed.
     */
    void removePertinentNodesAndDescendants(
        BLTree&               pqTree,
        list<node_list_it_t>& topLevelNodes
    );


    /** @brief first pass for PQ-tree reductions.
     *         It finds the ordering of the incoming edges of each node.
     *
     *  @param   graphCopy     (in):  internal graph
     *
     *  @param   stOrderCopy   (in):  bipolar orientation for graphCopy
     *
     *  @return  true : operation successful
     *           false: graph is not planar
     *
     *  @remarks if the call is successful, the nodes in grpahCopy 
     *           have orientation-agnostic ordered list of incoming incident
     *           edges in BLGraphNode::mIncomingEdgesOrdered.
     */
    bool findEmbeddingFirstPass(
        BLGraph&                graphCopy,
        vector<node_list_it_t>& stOrderCopy
    );


    /** @brief second backward pass for PQ-tree reductions.
     *         It finds the ordering of the outgoing edges of each node, and
     *         its orientation.
     *
     *  @param   graphCopy     (in):  interna graph
     *
     *  @param   stOrderCopy   (in):  bipolar orientation for graphCopy
     *
     *  @return  true : operation successful
     *           false: graph is not planar
     *
     *  @remarks if the call is successful, the nodes in grpahCopy 
     *           have orientation-agnostic ordered list of outgoing incident
     *           edges in BLGraphNode::mOutgoingEdgesOrdered, and
     *           the orientations of 
     *               BLGraphNode::mIncomingEdgesOrdered and
     *               BLGraphNode::mOutgoingEdgesOrdered.
     *           to
     *               mIncomingEdgesInReverse and
     *               mOutgoingEdgesInReverse.
     */
    bool findEmbeddingSecondPass(
        BLGraph&                graphCopy,
        vector<node_list_it_t>& stOrderCopy
    );


    enum edgeCollectionType {
        INCOMING,
        OUTGOING
    };

    enum scanDirectionType {
        NORMAL_DIRECTION,
        REVERSED_DIRECTION
    };


    void collectEdgeOrdering(
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        edgeCollectionType    collectionType
    );


    void collectEdgeOrderingCDPartialRoot(
        BLTree&               pqTree,
        node_list_it_t        treeRoot,
        BLGraphNode&          graphNode,
        edgeCollectionType    collectionType
    );


    void collectEdgeOrderingQTypeMiddle(
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        edgeCollectionType    collectionType
    );


    void collectEdgeOrderingQType(
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        edgeCollectionType    collectionType
    );


    void collectEdgeOrderingPType(
        BLTree&               pqTree,
        node_list_it_t        pertinentRoot,
        BLGraphNode&          graphNode,
        edgeCollectionType    collectionType
    );


    void collectEdgeOrderingFromTopLevelNodes(
        BLTree&                   pqTree,
        list<node_list_it_t>&     topPertinentNodes,
        list<edge_list_it_t>&     edgeList,
        scanDirectionType scanDirection
    );


    /** @brief arrange the incident edges of each node in the original graph.
     *
     *  @param   graph     (in/out):   original graph
     *
     *  @param   graphCopy (in):  internal copy of the graph
     *
     *  @remarks it rearranges the incident edges list of the original graph
     *           using the following in graphCopy
     *
     *           edges in BLGraphNode::mOutgoingEdgesOrdered, and
     *           the orientations of 
     *               BLGraphNode::mIncomingEdgesOrdered,
     *               BLGraphNode::mOutgoingEdgesOrdered,
     *               mIncomingEdgesInReverse, and
     *               mOutgoingEdgesInReverse.
     */
    void rearrangeIncidentEdges(
        Graph&                  graph,
        BLGraph&                graphCopy
    );


    void cleanUpInternalData();

    /** @brief the following two lists keep track of the orientation of
     *         the lists of the incoming edges emitted from an attachment
     *         Q node for each graph node during the reduction.
     *         When a Q-node without a specific orientation gets removed
     *         from the tree, the lists are moved here
     */
    list<node_list_it_t>           mOrientInNorm;
    list<node_list_it_t>           mOrientInReversed;

    /** @brief the following two lists keep track of the orientation of
     *         the lists of the outgoing edges collected for each node
     *         during the reduction.
     *         When a Q-node without a specific orientation gets removed
     *         from the tree, the lists are moved here
     */
    list<node_list_it_t>           mOrientOutNorm;
    list<node_list_it_t>           mOrientOutReversed;


#ifdef UNIT_TESTS
  friend class BLPlanarityTesterTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_BL_PLANARITY_TESTER_HPP_*/
