#ifndef _WAILEA_UNDIRECTED_ST_NUMBERING_HPP_
#define _WAILEA_UNDIRECTED_ST_NUMBERING_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"
#include "undirected/spqr_dfs_elements.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/st_numbering.hpp
 *
 * @brief finding st-numbers of a simple graph between 
 *        two particular nodes s and t. The numbers are implicitly given as
 *        a node list of bipolar orientation between s and t.
 *        If the given graph has {s,t}, it must be biconnected.
 *        If the given graph G does not have {s,t}, then G + {s,t} must be
 *        biconnected.
 *     
 * @reference:
 * [1] ftp://ftp.cs.princeton.edu/techreports/1985/013.pdf
 *     Two Streamlined Depth-First Search Algorthms
 *     Robert Endre Tarjan Computer Science Department
 *     Princeton University Princeton, NJ 08544 and AT&T Bell Laboratories
 *     Murray Hill, NJ 07974 July, 1985 CS-TR-013-86
 *     
 * [2] https://www.cs.princeton.edu/courses/archive/fall03/cs528/handouts/
 *                                                        two_stremlined.pdf
 *     Two Streamlined Depth-First Search Algorithms
 *     Robert Endre Tarjan, Computer Science Department, Princeton University
 *     Princeton, NJ 08544, U.S.A. and 
 *     AT&T Bell Laboratories, Murray Hill, NJ 07974, U.S.A.
 *     Fundamental Informaticae IX (1986) 85-94 North-Holland
 *
 * @supplement to understanding the algorithm.
 *
 *  A key to understand the algorithm is the following operation in the 
 *  reference paper.
 *  
 *      Add a vertex. If sign(low(v))=plus, insert v after p(v) in L and
 *      set sign(p(v))=minus; If sign(low(v))=minus, insert v before p(v) in L
 *      and set sign(p(v))=plus.
 *        
 *  Another key is the following invariant.
 *
 *      During the (second DFS) traversal, each vertex u that is a proper 
 *      ancestor of the current vertex v has a sign that is minus if u precedes
 *      v in L and plus if u follows v in L. Initially L={s,t} and s has sign
 *      minus. 
 *
 *  The correctness of the algorishm is briefly given in Theorem 1, but it is
 *  still not clear why the algorithm works.
 *  Let's define 'ta-path(w)' for each leaf w, as a directed path from
 *  low(w) to w.
 *  The following is a set of ta-paths for the sample graph given in [1]
 * 
 *      t->g->h
 *      s->t->g->f->b->a->e
 *      s->t->g->f->c->d
 *
 *  Now let's reduce the problem to finding a st-numbering for a ta-path.
 *  An invariant is that once a sign is set to a vertex, it will not change.
 *  The sign can change at a starting node of a new path, but within the DFS
 *  exploration of a ta-path, they won't change.
 *  And if you look at the descendants from an ancestor, the following 
 * invariant holds.
 *  - If ancestor v has sign plus, then all the descendants are to the left
 *    of v in L, and the tree arc from v is directed to the left in L.
 *  - If ancestor v has sign minus, then all the descendants are to the right
 *    of v in L, and the tree arc from v is directed to the right in L.
 *
 *  Ex.)   -    -      +
 *        [s<..]g->h..>t
 *               <----/
 *
 *         <......\
 *             <---\
 *        -   -     +  +  +  +
 *        s<..a->e  b<-f<-g<-t
 *         \------\--------->
 *                 \.....>
 *
 *      Legend:
 *      --> : tree arc
 *      ..> : back edge
 *
 *  For example, for the second case above, s is minus, all the descendants
 *  on the ta-path are to the right of s, and (s->t) is directed to the right.
 *  Vertex t is plus,  all the descendants on the ta-path are to the left of t,
 *  and t->g is directed to the left.
 *  The direction is determined to the consecutive vertices that share the same
 *  low() value. For the second case, it's (t->g->f->g->a). Since they are
 *  chanined we know the vertices that not at either end are adjacentnt to
 *  both lower and higher vertices in L.
 *
 *  The descendent end of the chain (w) is incident to a back edge to low(w)=x,
 *  and the direction is   decided to the opposite of the x (=low(w)) so that
 *  the tree arc from w and the back edge from w are in the different direction
 *  and hence w has both lower and higher adjacent vertices in L.
 *  This way if v's sign is different from p(v)'s, then the descendants of (v)
 *  will be housed between v and p(v) in L. See the location of vertex e above.
 *
 *  If the ancestor end of the consecutive vertices is not t, then it can be
 *  considered a child of a vertex y, which is the descendent end of the 
 *  previous consecutive chain explored (v1->v2->...->y).
 *  
 *  The argument above explans the bipolar orientation of a single ta-path.
 *  next let's extend it to the general case with tree arc branches.
 *  When the DFS enters the first unvisited tree arc of a new ta-path, for
 *  example in the case above, (g->f) or (f->c), the initial direction of such
 *  arc is determined by low(v) of the branching vertex v, which works as
 *  the first vertex in the consecutive chain above and the path from v
 *  is processed in the same way as described above for the single ta-path
 *  case.
 *
 *  We can see that once we  have placed the vertices of a ta-path on L, their
 *  relative positions among them will not change during the DFS exploration.
 *  This ensures all the vertices except for s and t will have both higher
 *  and lower adjacent vertices in L.
 *  
 *
 *
 * @overview
 *
 * The algorithm consists of two DFS steps.
 * 1. Forming a palm-tree and calculate th pre-numbers and low point
 *    for each node.
 *
 * 2. Form a node list L by inserting the visiting edges at the appropriate
 *    locations.
 *       
 * NOTE1: The implementation does not have any recursive function calls to 
 *        privent unpredictable growth of the runtime calling stack.
 *
 * Step 1. The first step is to form a palm tree.
 *         A DFS exploration gives rise to a palm-tree, in which the edge from
 *         the currently visiting node to an unexplored adjacent node is 
 *         called 'tree arc', and the edge from the current node to an already
 *         visited adjacent node is called 'frond'. A 'frond' is also called 
 *         'feedback edge' in some papers. 
 *         to the same initial node, and the order in the adjacency list of 
 *         each node.
 *         It assigns a number to each node in pre-order travarsal, find 
 *         a low point.
 *
 * Step1(G, s, {s,t})
 * comment: G is the graph to be explored, and s is the start node, and
 *          {s,t} is the first edge to be explored in DFS.
 * begin
 *
 *     integer n 
 *     comment: n is the last number assigned to a vertex.
 *     procedure DFSProc1(v)
 *     begin
 *         PRE(v) := n
 *         LOW(v) := n
 *         n := n + 1
 *
 *         comment: ADJ is the adjacency list.
 *         for w in ADJ(v) do
 *         begin
 *             if VISITED((v,w)) = false then
 *             begin
 *                 if PRE(w) = 0 then
 *                 begin
 *                     comment: w is a new vertex
 *                     TYPE((v,w)) := TREE_ARC
 *                     VISITED((v,w)) := true
 *                     DFSProc1(w)
 *                 
 *                     if LOW(w) < LOW(v) then
 *                     begin
 *                         LOW(v) := LOW(w)
 *                     end
 *                     PARENT(w) := v
 *                 end
 *                 else
 *                 begin
 *                     TYPE((v,w)) := FROND
 *                     if PRE(w) < LOW(v) then
 *                     begin
 *                         LOW(v) := PRE(w)
 *                     end
 *                 end
 *             end
 *         end
 *     end
 *
 *     n := 1
 *     comment: N is the set of nodes in G.
 *     for v in N do
 *         PRE(v) := 0
 *
 *     comment: E is the set of edges in G.
 *     for e in E do
 *         VISITED(e) := false
 *     
 *     comment: s is the start node.
 *
 *     comment: Reorder the incident edges list of s such that {s,t}
 *              gets the first position in the list.
 * 
 *     DFSProc1(s)
 *
 * Step 2. make the list L.
 *
 *     procedure DFSProc2(v)
 *     begin
 *         if v != s AND v != t then
 *         begin
 *             lowp := NodeMap[LOW(v)]
 *             if SIGN(lowp) =  plus then
 *             begin
 *                 L.insertBefore(PARENT(v), v)
 *                 SIGN(PARENT(v)) := minus
 *             end
 *             else
 *             begin
 *                 L.insertAfter(PARENT(v), v)
 *                 SIGN(PARENT(v)) := plus
 *             end
 *         end
 *         for w in ADJ(v) do
 *             if VISITED((v,w)) = false AND TYPE((v,w)) = TREE_ARC then
 *                 DFSProc2(w)
 *     end
 *
 *     NodeMap := array of (N + 1) elements
 *     comment: NodeMap is a mapping from the PRE number to (the reference to)
 *              the node.
 *     for v in N do
 *         NodeMap[ NUM(v) ] := v
 *         
 *     comment: E is the set of edges in G.
 *     for e in E do
 *         VISITED(e) := false
 *
 *     L := empty doubly linked list
 *     L.append(s)
 *     L.append(t)
 *     SIGN(s) = minus
 *     DFSProc2(s, L)
 *
 *  
 */
 
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @class ST_DFS_Node
 *
 *  @brief a node edge to be explored by STStrategyProc1 and.STStrategyProc2.
 *         This class is augumented to DFSNode<ST_DFS_Node,ST_DFS_Edge>
 *         for DFSExplorer<Graph,ST_DFS_Node,ST_DFS_Edge>.
 */
class ST_DFS_Node : public Node {

  public:
    inline ST_DFS_Node():mPre(0),mLow(0),mSignPlus(false){;}

    inline virtual ~ST_DFS_Node(){;}

    /** @brief node number in the pre-order travarsal of DFS.
     */
    size_t               mPre;
    /** @brief low point reachable from this node walking along tree arcs and
     *        at most one frond in the end of the walk.
     */
    size_t               mLow;


    /** @brief parent edge of this node.
     */
    edge_list_it_t       mParentEdge;


    /** @brief false:minus / true:plus
     */
    bool                 mSignPlus;


    /** @brief corresponding node in the input graph.
     */
    node_list_it_t mOriginal;


    /** @brief the insersion point in the list L.
     */
    list<node_list_it_t>::iterator mItForL;

};


/** @class ST_DFS_Edge
 *
 *  @brief a edge edge to be explored by STStrategyProc1 and.STStrategyProc2.
 *         This class is augumented to DFSNode<ST_DFS_Node,ST_DFS_Edge>
 *         for DFSExplorer<Grah,ST_DFS_Node,ST_DFS_Edge>.
 */
class ST_DFS_Edge : public Edge {

  public:
    enum type {
        Unknown,
        TreeArcType,
        FrondType,
    };

    inline ST_DFS_Edge(){;}

    inline virtual ~ST_DFS_Edge(){;}

    enum type mType;

    /** @brief corresponding edge in the input graph.
     */
    edge_list_it_t mOriginal;

};


/** @class  STNumbering
 *  @brief  generates bipolar orientaion of a given biconnected graph 
 *          in a vertex list.
 *          
 *  @details 
 *      Complexities:
 *        It uses heap memory in the order of O(|N|+|E|).
 *        It runs in the order of  O(|N|+|E|).
 */
class STNumbering {

  public:

    using DNode = DFSNode<Graph, ST_DFS_Node, ST_DFS_Edge>;
    using DEdge = DFSEdge<Graph, ST_DFS_Node, ST_DFS_Edge>;

    /** @brief generates bipolar orientation
     *
     *  @param   g (in): input graph. It must be simple and biconnected.
     *  @param   s (in): source node
     *  @param   t (in): destination node
     *
     *  @return  list of nodes in the bipolar orientation from s to t.
     *
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed.
     *
     *  @remark undefined behavior
     *           If the input graph is not simple, the behavior is undefined.
     *           If the input graph + {s,t} is not biconnected, empty or an 
     *           isolated single vertex, the bahavior is undefined.
     *           The required memory and running time will be still in the 
     *           order of O(|N|+|E|).
     */
    vector<node_list_it_t> getBipolarOrientation(Graph& g, Node& s, Node& t);

  private:

    /** @brief   creates a copy of the input graph for DFS exploration.
     *
     *  @param   src (in):  the input graph whose copy is created.
     *  @param   dst (out): the copy graph to which nodes and edges are added.
     *  @param   startNode (in): the start node in the input graph.
     *
     *  @return  the start node in the copied graph.
     */
    void copyInputGraph(
            Graph& src,           Graph& dst, 
            node_list_it_t srcIt, node_list_it_t& srcCopyIt, 
            node_list_it_t dstIt, node_list_it_t& dstCopyIt );


#ifdef UNIT_TESTS
  friend class STNumberingTests;
#endif
        
};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_ST_NUMBERING_HPP_*/
