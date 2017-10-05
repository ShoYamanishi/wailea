#ifndef _WAILEA_DIRECTED_SUGIYAMA_DIGRAPH_HPP_
#define _WAILEA_DIRECTED_SUGIYAMA_DIGRAPH_HPP_

#include "directed/di_base.hpp"

/**
 * @file directed/sugiyama_digraph.hpp
 */

namespace Wailea {

namespace Directed {

using namespace std;


class SugiyamaDiNode : public DiNode {

  public:
      inline SugiyamaDiNode():mVirtual(false),mRank(0),mPos(0){;}
      inline virtual ~SugiyamaDiNode(){;}
      inline bool isVirtual() const  {return mVirtual;}
      inline long rank() const {return mRank;}
      inline long pos() const {return mPos;}
  private:

    /** @brief true if this is introduced to make 
     *         the graph planar.
     *         false otherwise.
     */
    bool            mVirtual;

    /** @brief rank of this node
     */
    long            mRank;

    /** @brief position in the rank
     */
    long            mPos;

friend class SugiyamaDiGraph;

#ifdef UNIT_TESTS
friend class SugiyamaDiGraphTests;
#endif
};


class SugiyamaDiEdge : public DiEdge {

  public:
    inline                SugiyamaDiEdge()
                    :mTerminalNsrc(true),mTerminalNdst(true),mFlipped(false){;}

    inline edge_list_it_t chainNsrc()      const {return mChainNsrc;}
    inline edge_list_it_t chainNdst()      const {return mChainNdst;}
    inline bool           isTerminalNsrc() const {return mTerminalNsrc;}
    inline bool           isTerminalNdst() const {return mTerminalNdst;}
    inline bool           isFlipped() { return mFlipped; }

  private:

    inline void           setChainNsrc(edge_list_it_t eit) {
                                     mChainNsrc = eit; mTerminalNsrc = false; }
    inline void           setChainNdst(edge_list_it_t eit) {
                                     mChainNdst = eit; mTerminalNdst = false;}

    /** @brief if incidenceNodeSrc() is virtual,
     *         then this points to the continuation
     *         of this edge that represents the 
     *         original (unsplit) edge.
     */
    edge_list_it_t  mChainNsrc;

    /** @brief if incidenceNodeDst() is virtual,
     *         then this points to the continuation
     *         of this edge that represents the 
     *         original (unsplit) edge.
     */
    edge_list_it_t  mChainNdst;



    /** @brief incidentNodeDrc() is the terminal of 
     *         the chain of edges across virtual nodes.
     */
    bool            mTerminalNsrc;

    /** @brief incidentNodeDst() is the terminal of 
     *         the chain of edges across virtual nodes.
     */
    bool            mTerminalNdst;

    /** @brief true if this edge is flipped to make the graph acyclic.
     */
    bool            mFlipped;

friend class SugiyamaDiGraph;

#ifdef UNIT_TESTS
friend class SugiyamaDiGraphTests;
#endif
};


class SugiyamaDiGraph : public DiGraph {

  public:

    /** @brief constructor with an original digraph, which could contain
     *         cycles (non-trivial strongly connected components), and from
     *         which an acyclic SugiyamaDiGraph will be generated contructed.
     *
     *  @param  G  (in): Connected digraph.
     */
    inline SugiyamaDiGraph(DiGraph& G):mG(G){;};
    inline virtual ~SugiyamaDiGraph(){;};


    /** @brief generates a copy of the input graph given in mG to this graph
     *         with Sugiyama[Node,Edge].
     *         The orientaion of some of the edges may be flipped to make
     *         it acyclic according to the given acyclic ordering.
     *
     *  @param  ao (in): ordered list of the nodes in mG according to which 
     *                   the edges are oriented.
     */
    void generateFromAcyclicOrdering(vector<node_list_it_t>& ao);


    /** @brief assign ranks to the nodes and if the length 
     *         ( rank(dst) - rank(src) ) of an edge becomes longer than one,
     *         then split it into multiple edges of length one with some 
     *         virtual nodes.
     *
     *  @param  ranks (in): rank[i] contains the nodes in this graph 
     *                      for rank i.
     *
     *  @param return  ranks of the nodes in this updated graph.
     *                 It may contain some virtual nodes.
     *                 Order of the nodes in each inner vector is arbitrary.
     */
    vector<vector<node_list_it_t> >rankNodesAndAddVirtualNodes(
                                    vector<vector<node_list_it_t> >& ranks);

    /** @brief sets the mPos in the nodes based on the given ordering.
     *
     *  @param ranks (in): outer vector specifies the ranks.
     *                     each inner vector specifies the specific ordering
     *                     (positions) of the ndoes at the rank.
     */
    void setPositionsForEachRank(vector<vector<node_list_it_t> >& ranks);

  private:

    /** @brief utility routines for std::sort for vector<edge_list_it_t>.
     */
    static bool nodeCompDst(edge_list_it_t it1, edge_list_it_t it2);
    static bool nodeCompSrc(edge_list_it_t it1, edge_list_it_t it2);


    DiGraph& mG;


#ifdef UNIT_TESTS
friend class SugiyamaDiGraphTests;
#endif
};

}// namespace Directed

}// namespace Wailea

#endif /*_WAILEA_DIRECTED_SUGIYAMA_DIGRAPH_HPP_*/
