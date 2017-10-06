#ifndef _WAILEA_UNDIRECTED_PLANARIZABLE_GRAPH_HPP_
#define _WAILEA_UNDIRECTED_PLANARIZABLE_GRAPH_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/planarizable_graph.hpp
 */

namespace Wailea {

namespace Undirected {

using namespace std;


class PlanarizableGraph;

class PlanarizableNode : public Node {

  public:
      inline PlanarizableNode():mVirtual(false){;}
      inline virtual ~PlanarizableNode(){;}

      inline bool isVirtual() const  {return mVirtual;}
      inline void setVirtual(bool v) {mVirtual = v;}
  private:
    /** @brief true if this is introduced to make 
     *         the graph planar.
     *         false otherwise.
     */
    bool            mVirtual;

friend class PlanarizableGraph;
};


class PlanarizableEdge : public Edge {

  public:
    inline PlanarizableEdge():mTerminalN1(true),mTerminalN2(true){;}
    inline edge_list_it_t chainN1()      const {return mChainN1;}
    inline edge_list_it_t chainN2()      const {return mChainN2;}
    inline bool           isTerminalN1() const {return mTerminalN1;}
    inline bool           isTerminalN2() const {return mTerminalN2;}
    inline void setChainN1(edge_list_it_t eit) {
                                         mChainN1 = eit; mTerminalN1 = false; }
    inline void setChainN2(edge_list_it_t eit) {
                                         mChainN2 = eit; mTerminalN2 = false; }

    /** @brief if incidenceNode1() is virtual,
     *         then this points to the continuation
     *         of this edge that represents the 
     *         original (unsplit) edge.
     */
    edge_list_it_t  mChainN1;

    /** @brief if incidenceNode2() is virtual,
     *         then this points to the continuation
     *         of this edge that represents the 
     *         original (unsplit) edge.
     */
    edge_list_it_t  mChainN2;
  private:
    /** @brief incidentNode1() is the terminal of 
     *         the chain of edges across virtual nodes.
     */
    bool            mTerminalN1;

    /** @brief incidentNode2() is the terminal of 
     *         the chain of edges across virtual nodes.
     */
    bool            mTerminalN2;

friend class PlanarizableGraph;
};


/**
 * @class PlanarizableGraph
 *
 * @brief 
 */
class PlanarizableGraph : public Graph {

  public:
    /** @brief actually constructs this graph as a copy of the given graph.
     *         This Graph must be empty before calling createFrom().
     *
     *  @param g      (in): input graph
     *
     *  @param nodes  (in): nodes in input graph to be copied
     *
     *  @param edges  (in): edges in input graph to be copied
     *
     *  @throw   std::invalid_argument(Constants::kExceptionNotEmpty)
     *           if the Graph is not empty.
     *
     *  @remark on inter-graph links. After the call, g and this Graph
     *           are interconnected as follows.
     *           - Nodes & Edges in g are cross-linked with PlanarizableNodes
     *             and PlanarizableEdges respectively.
     */
    void createFrom(
        Graph&                  g,
        vector<node_list_it_t>& Nlist,
        vector<edge_list_it_t>& Elist
    );


    /** @brief inserts an edge between two nodes along a specific route
     *         specified by the ordered list of edges
     *
     *  @param insertionPath (in/out):
     *                             Before the call, this is the ordered list of
     *                             the edge from N1 to N2 in this graph along
     *                             which a new edge is inserted. Those edges
     *                             are split by virtual edges.
     *                             Upon return, this is filled with the 
     *                             new edges inserted from N1 to N2.
     *
     *  @param N1            (in): Node 1 of the new edge(s) inserted.
     *
     *  @param N2            (in): Node 2 of the new edge(s) inserted.
     *
     *  @param original      (in): If this is not edges().second, then it is
     *                             the original edge in the graph from which
     *                             this PlanarizableGraph has been created.
     *                             This value is set to the new edges along the
     *                             path from N1 to N2 in IGBackwardLink.
     *                             Also, the new edge incident to N1 is pushed
     *                             to IGForwardLink of the original edge.
     *                              
     *                             
     *
     *  @return  removed edges that have been split by the new path from N1 to
     *            N2.
     *
     * @remark 
     *        The insertion is done by splitting some edges with 
     *        virtual nodes, and connecting those nodes.
     *        Such an ordered list of edges can be found by GMWEdgeInserter.
     *
     *         e1   e2   e3   e4      ex               e'1 e'2 e'3 e'4   e'x
     *         |    |    |    |       |                |   |   |   |     |
     *         |    |    |    |       |             e*0|e*1|e*2|e*3|e*4  |e*x
     *  N1 ... |    |    |    |  ...  |   N2  => N1 ---+---+---+---+-...-+-- N2
     *         |    |    |    |       |                |n1 |n2 |n3 |n4   |nx
     *         |    |    |    |       |                |   |   |   |     |
     *                                                 e"1 e"2 e"3 e"4   e"x
     *  ex  : edges to be split and removed.
     *        It is assumed that IGBackwardLink of ex points to a original edge
     *        org_x.
     *
     *  e'x : edges to be created for ex as a result of splitting.
     *        IGBackwardLink of e'x will point to org_x, and 
     *        IGForwardLink of org_x will point to e'x.
     *
     *  e"x : Another set of edges to be created for ex as a result of 
     *        splitting.
     *        IGBackwardLink of e'x will point to org_x.
     *
     *  e*x : edges to be inserted between N1 and N2.
     *        IGBackwardLink of e'x will point to the egde specified by the
     *        the param 'original', whose IGForwardLink will then point back
     *        to e*0.
     *
     *  nx  : Virtual nodes introduced along the path from N1 to N2.
     *        IGBackwardLink of nx points to a dummy value (nodes().second).
     */
    vector<edge_ptr_t> insertEdge(
        list<edge_list_it_t>& insertionPath,
        PlanarizableNode&     N1,
        PlanarizableNode&     N2,
        edge_list_it_t        original
    );


    /** @brief  remove unnecessary virtual nodes after an emdedding is found.
     *          This must be called after a planar embedding has been found 
     *          into the incidence list of each node.
     *         
     *  @remark an unnecessary virtual node is found is the incident edge pair
     *          for the same original edge are arranged consecutively as 
     *          follows.
     *
     *          N1      N3             N1      N3              N1        N3
     *           \     /                \     /                |         |
     *            \   /  an embedding    \   /                 |         |
     *             \ /    is found        \ /    remove VN     |         |
     *              VN       =>            VN       =>         |         |
     *             / \                    / \                  |         |
     *            /   \                  /   \                 |         |
     *           /     \                /     \                |         |
     *          N4      N2             N2      N4              N2        N4
     *
     *  @return list of tuples each of which contains the following:
     *          - removed edge 1
     *          - removed edge 2
     *          - new edge 
     */
    vector<std::tuple<edge_ptr_t, edge_ptr_t, edge_list_it_t>>
                                               removeUnnecessaryVirtualNodes();
  private:
    vector<edge_list_it_t> findConsecutiveChains(PlanarizableNode& N);


    std::tuple<edge_ptr_t, edge_ptr_t, edge_list_it_t>
    mergeTwoEdges(
        PlanarizableNode& N,
        PlanarizableEdge& E1,
        PlanarizableEdge& E2
    );

    void splitInsertionPath (
        list<edge_list_it_t>&   insertionPath,
        vector<edge_ptr_t>&     removedEdges,
        vector<node_list_it_t>& newNodes
    );

    vector<node_list_it_t> splitInsertionPath (
        list<edge_list_it_t>& insertionPath
    );

};

}// namespace Undirected

}// namespace Wailea

#endif /* _WAILEA_UNDIRECTED_PLANARIZABLE_GRAPH_HPP_*/
