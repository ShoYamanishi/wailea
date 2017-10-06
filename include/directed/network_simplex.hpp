#ifndef _WAILEA_DIRECTED_NETWORK_SIMPLEX_HPP_
#define _WAILEA_DIRECTED_NETWORK_SIMPLEX_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "directed/di_base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file directed/network_simplex.hpp
 *
 * @brief network simplex linear optimization algorithm
 * 
 * @remark 
 *      ===================================
 *      Network Simplex Problem Formulation
 *      ===================================
 *        The problem of a network simplex is formulates as follows:
 *          
 *        For a given directed graph G(N,E):
 *
 *          Minimize    sum(c_(i,j)*x_(i,j)) over (i,j) \in E
 *
 *          Subject to  sum(x_(i,k)) - sum(x_(k,j)) = -b_{k}  for k \in N
 *
 *                      x_(i,j) >= 0                          for (i,j) \in E
 *
 *          where c_(i,j) : edge cost to carry a unit amount from i to j
 *                b_{k}   : supply (positive) or demand (negative) at node k.
 *                x_(i,j) : flow on edge (i,j) to be found.
 *
 *
 *        The dual of the problem above is as follows:
 *
 *          Maximize   -sum(b_{k}*y_{k})
 *
 *          Subject to  y_{j} - y_{i} + z_(i,j) = c_(i,j) for (i,j) \in E
 *
 *                      z_(i,j) >= 0                      for (i,j) \in E
 *        
 *
 *           where y_{k}  : dual variable
 *                 z_(i,j): dual slack
 *
 *           We also assume the node 0 is the root, and y_{0} is anchored at 0.
 *
 *        At an optimal solutions: x_(i,j) * z_(i,j) = 0
 *
 *        Prerequisites on the input digraph (network).
 *        - the digraph must be connected.
 *        - sum(b_{k}) must be equal to zero.
 *        - c_(i,j} must be non-negative.
 *
 *        Ex. ) A rank assignment problem with minimum total edge length for
 *        an acyclic digraph is formulated as a dual problem as follows.
 *
 *          b_{k}:   - sum(cost(k,j)) + sum(cost(i,k))
 *          c_(j,i): minimum length required for (i,j) * -1
 *          y_{k}:   rank for k
 *          z_(j,i): slack at edge (i,j)
 *          x_(j,i): no specific meaning.
 *
 *
 *      ======================
 *      Implemetation Overview
 *      ======================
 *
 *        The high level overview.
 *        ------------------------         
 *            1. Make an auxiliary problem. 
 *            2. Find an initial feasible solution for the auxiliary problem
 *            3. Solve it to an optimum. (Phase 1)
 *            4. If the optimum basis contains any artificial edge, 
 *               declare infeasibility.
 *            5. Use the basis found at step 3 as an initial feasible basis.
 *            6. Solve it to an optimum (Phase 2)
 *
 *        Process
 *        -------
 *
 *        1. Make an auxiliary problem. 
 *
 *          Place an artificial node v on the digraph with b_{v} = 0.
 *          For each node n such that b_{n} > 0, add an artificial edge (n,v).
 *          For each node n such that b_{n} < 0, add an artificial edge (v,n).
 *          For each non-artificial edge (i,j), set c_(i,j) = 0.
 *          For each artificial edge (i,j), set c_(i,j) = 1.
 *
 *        2. Find an initial feasible solution for the auxiliary problem
 *
 *        2.1. Find an initial feasible spanning tree ST(Ns,Es).
 *
 *          Place all the artificial edge to Es.
 *          At this point all the nodes n succh that b_{n}==0 are not reachable
 *          on ST. 
 *          For a non-reachable node n, pick an arbitrary edge (n,i) or (i,n)
 *          where node i is already reachable in ST.
 *          This becomes the initial feasible basis for the auxiliary problem
 *          Also, pick an arbitrary node r as the root of the spanning tree.
 *
 *        2.2. Find x, y, and z values for the initial basis.
 *
 *          For artificial edges (i,v) in Es, x_(i,v) := b_{i}.
 *          For artificial edges (v,i) in Es, x_(v,i) := - b_{i}.
 *          For non-artifical edges (i,j), x_(i,j) := 0
 *
 *          Explore the spanning tree in DFS from r:
 *              - set y_{r} := 0
 *              - For node n being visited with its parent p:
 *                  y_{n} = y_{p} + c_(n,p)
 *                  y_{n} = y_{p} - c_(n,p)
 *
 *        3. Solve the auxiliary problem to an optimum.
 *          
 *          Pivotting until there is no non-basis edge with negative Z.
 *          One pivot goes as follows:
 *
 *          P1. Select a non-basis edge (ve1, ve2) with z_(ve1,ve2) < 0.
 *
 *          P2. Find a (ve2,ve1)-path P in ST. P and (ve1,ve2) form a cycle.
 *
 *          P3. Find the leaving edge (vl1, vl2). It is found as follows.
 *              - (vl1,vl2) is oriented against the direction of (ve2, ve1).
 *              - x_(vl1,vl2) has the minimum value (including perturbation).
 *
 *          P4. Find the subtree STnr that would form if (vl1, vl2) were 
 *              removed from ST. Removal of (vl1, vl2) would make two disjoint
 *              subtrees of ST, and let STnr be the one that does not contain
 *              r, which is the root of the spanning tree.
 *
 *          P5. Find the cut set C (set of edges) in the graph that spans
 *              STnr and ST\STnr.
 *
 *          P6. Update x_(i,j). Update on X will be needed only for edges in 
 *              P and (ve1, ve2). Let t := x_(vl1, vl2).
 *              
 *               x_(ve1, ve2) := t
 *               If the orientation of (i,j) and the direction of 
 *               (ve2,ve1)-path are the same, then 
 *                   x_(i,j) := x_(i,j) + t.
 *               Otherwise 
 *                   x_(i,j) := x_(i,j) - t.
 *
 *          P7. Update y_{n}. Update on Y will be needed only for the nodes in
 *              STnr. 
 *                  y_{n} := y_{n} - z_(ve1,ve2)
 *
 *          P8. Update z_(i,j). Update on Z will be needed only for the edges 
 *              in C (cutset). Let t := z_(ve1, ve2).
 *
 *              z_(ve1,ve2) := 0
 *              If the spanning direction of (i,j) is the same as (ve1,ve2),
 *              i.e. if those two edges are oriented from STnr to ST\STnr, 
 *              or from ST\STnr to STnr,
 *                  z_(i,j) = z_(i,j) - t
 *              Otherwise (i.e., one edge is from STnr to ST\STnr and the other
 *              is from ST\STnr to STnr)
 *                  z_(i,j) = z_(i,j) + t
 *
 *          P9. Remove (vl1, vl2) from ST and add (ve1, ve2) to it.
 *
 *        4. If the optimum basis contains any artificial edge, 
 *           declare infeasibility.
 *
 *        5. Use the basis found at step 3 as an initial feasible basis.
 *
 *        5.1. Remove the artificial edges and the artificial node.
 *
 *        5.2. Restore the real cost c_(i,j) for each edge.
 * 
 *        5.3. Recalculate y_{n} based on the current spanning tree and the
 *             restored cost. This is done by DFS from r.
 *
 *        5.4. Recalculate z_(i,j) for the non-basis edges based on the 
 *             updated y_{n}.
 *
 *        6. Solve it to an optimum (Phase 2)
 *           Using the same pivotting described at step3 until there is no
 *           non-basis edge with negative Z.
 *
 *        Efficient implementation issues
 *        -------------------------------
 *
 *        - mNegativeZEdges
 *          The non-basis edges that have negative Z are kept in the list
 *          so that an entering edge can be found in O(1) time without scanning
 *          the edes in the digraph.
 *
 *        - Orientation of the spanning tree from a designated root.
 *          Orienting the spanning tree from the root and adding some
 *          additional attributes will make the discovery of (ve2,ve1)-path,
 *          and STnr efficient. Each spanning tree node has the following 
 *          attributes.
 *           - mDepth           : distance from the root on the spanning tree
 *           - mPrec            : the parent edge toward the root
 *           - mThreadPrev&Next : list of nodes in the pre-order traversal of
 *                                DFS from the root.
 *
 *          (ve2,ve1)-path is found from ve2 and ve1 forming two paths by 
 *          following the parent link mPrec from those two nodes.
 *          Those two paths eventually merge at a node.
 *          To avoid one from passing the other path, mDepth is used such that
 *          only the deeper of the two nodes is advanced.
 *
 *                      -----------------  Depth  --------------->
 *
 *                   mPrec     mPrec     mPrec                  mPrec
 *           merging <----n2 <------ n3 <---...     <------ nx <----- ve2
 *           node ^                                                     ^
 *                |                                                     |
 *                 ------ n1 <------ n2 <-...--  ve1 - - - - - - - - - -
 *                  mPrec     mPrec                      (ve1,ve2) entering
 *
 *          STnr is found by mThreadPrev/Next. The root of STnr is the deeper
 *          of vl1 or vl2. Starting from the root, follow mThreadNext until
 *          the depth of the nodes gets shallower than the root.
 *          This gives the nodes in STnr.
 */
namespace Wailea {

namespace Directed { 

using namespace std;

using namespace Wailea::Directed;


/** @class NetworkSimplexNode
 *
 *  @brief the main node class for network simplex.
 *         The user of NetworkSimplex must set supply/demand with setB().
 */
class NetworkSimplexNode : public DiNode {

  public:

#ifdef UNIT_TESTS
    inline NetworkSimplexNode(long id):
                              mB(0),mY(0),mArtificial(false),mDebugId(id){;}
#endif
    inline NetworkSimplexNode():mB(0),mY(0),mArtificial(false){;}
    inline virtual ~NetworkSimplexNode(){;}
    inline void setB(long b){mB = b;}
    inline long y(){return mY;}

  private:

    /** @brief supply (positive) or demand (negative) set by the user
     */
    long mB;

    /** @brief dual variable
     */
    long mY;

    /** @brief true if this is an artificial node added to find
     *         the intial feasible solution.
     */
    bool mArtificial;

  friend class NetworkSimplex;

#ifdef UNIT_TESTS
  public:
    long mDebugId;
#endif
};


/** @class NetworkSimplexEdge
 *
 *  @brief the main edge class for network simplex.
 *         The user of NetworkSimplex must set the cost with setC().
 *
 */
class NetworkSimplexEdge : public DiEdge {

  public:
    inline NetworkSimplexEdge():
               mC(0),
               mCreal(0),
               mCperturbation(0),
               mX(0),
               mZ(0),
               mInBasis(false),
               mArtificial(false){;}

    inline virtual ~NetworkSimplexEdge(){;}

    /** @brief sets the cost of this edge
     */
    inline void setC(long c){mCreal = c;}

    /** @brief returns the calculated x() value by NetworkSimplex::solve().
     */
    inline long x(){return mX;}

  private:

    /** @brief cost of the edge to transfer a unit from source node to the 
     *         destination.
     *         It is 0 or 1 if an auxiliary problem is being solved.
     *         It is equal to mCreal when the actual problem is being solved.
     */
    long mC;

    /** @brief the real cost value set by the user is stored here.
     */
    long mCreal;

    /** @brief the perturbation to make ordering among edges with the same 
     *         cost.
     */
    long mCperturbation;

    /** @brief primary variable
     */
    long mX;

    /** @brief dual slack
     */
    long mZ;

    /** @brief true if this edge is in the basis (spanning tree)
     */
    bool mInBasis;

    /** @brief true if this is an artificial edge added to find
     *         the intial feasible solution.
     */
    bool mArtificial;

    /** @brief back link to the element in mNegativeZEdges if mZ of this edge
     *         is negative.
     */
    list<edge_list_it_t>::iterator mNZEbackIt;

  friend class NetworkSimplex;

};


/** Following two are used by the spanning tree.
 */
class NSSpanningTreeEdge : public Edge {};
class NSSpanningTreeNode : public Node {

  public:

    inline NSSpanningTreeNode():mNsGeneration(0){;}
    inline virtual ~NSSpanningTreeNode(){;}

    /** @brief called if this node is to be explored by DFS for the first time
     *   
     *  @param gen       (in): generation value to be set.
     *  
     *  @param dfsParent (in): parent of this node for the current DFS 
     *                         exploration
     */
    inline void initForDFS(long gen, node_list_it_t dfsParent) {

        mNsGeneration = gen;
        mDfsParent     = dfsParent;

        for (mNextEdge = incidentEdges().first;
                            mNextEdge != incidentEdges().second; mNextEdge++) {
            auto& E = dynamic_cast<NSSpanningTreeEdge&>(*(*(*mNextEdge)));
            auto& A = dynamic_cast<NSSpanningTreeNode&>(E.adjacentNode(*this));
            if (A.mNsGeneration < mNsGeneration) {
                break;
            }
        }             
    }

    /** @brief returns true if the current node has an incident edge to be
     *         explored.
     */
    bool hasNextChild() {return mNextEdge != incidentEdges().second; }

    /** @brief returns the next incident edge to be explored during the
     *         DFS exploration.
     */
    NSSpanningTreeEdge& nextChild() { 
        // Save the current edge to be returned first.
        edge_list_it_t eit = *mNextEdge;

        // Advance mNextEdge to the next edge to be explored for the future
        // call to nextChild().
        mNextEdge++;
        for ( ; mNextEdge != incidentEdges().second; mNextEdge++) {
            auto& E = dynamic_cast<NSSpanningTreeEdge&>(*(*(*mNextEdge)));
            auto& A = dynamic_cast<NSSpanningTreeNode&>(E.adjacentNode(*this));
            if (A.mNsGeneration < mNsGeneration) {
                break;
            }
        }

        return dynamic_cast<NSSpanningTreeEdge&>(*(*eit));
    }


    node_list_it_t dfsParent() { return mDfsParent; }

  private:

    /** @brief used to construct the first spanning tree.
     *
     *  @remark The parent class 'Node' has mGeneration but 
     *          it is not used to avoid interference with 
     *          other operations defined Node or other classes.
     */
    long                mNsGeneration;

    /** @brief points to the next incident edge to be explored by DFS.
     */
    node_incidence_it_t mNextEdge;

    /** @brief holds the pointer to the parent during a DFS exploration.
     */
    node_list_it_t      mDfsParent;

    /** @brief depth of this node in the current spanning tree.
     */
    long                mDepth;

    /** @brief the predecessor (parent) edge of this node in the current 
     *         spanning tree.
     */
    edge_list_it_t      mPrec;

    /** @brief thread links of the nodes in the current spanning tree.
     *         A thread is an ordered list of nodes according to the DFS
     *         pre-traversal visiting ordering.
     */
    node_list_it_t      mThreadPrev;
    node_list_it_t      mThreadNext;

  friend class NetworkSimplex;
};


/** @class  NetworkSimplex
 *
 *  @brief  main class to perform network simplex algorithm
 *
 */
class NetworkSimplex {

  public:
    /** @brief constructor
     *
     *  @param   G  (in): The digraph (network) to solve. 
     *                    It must has NetworkSimplexNode and NetworkSimplexEdge
     *                    as its nodes and edges.
     *                    It must be connected.
     *                    Sum of supply/cost over all the nodes must be 0.
     *                    There must be at least one positive supply node.
     *                    The edge costs must be non-zero.
     */
    inline NetworkSimplex(DiGraph& G):mG(G),mNsGeneration(0){;}
    inline virtual ~NetworkSimplex(){;}

    /** @brief checks the validity of the input graph.
     *
     *  @return true : if the given digraph (network) is valid.
     *          false: if the given digraph (network) is not valid.
     *
     *  @remark on validity:
     *          - Must be connected
     *          - Sum of B (supply/demand) must be 0
     *          - At least one non-zero supply/demand.
     *          - Cost must be non-negative.
     */
    bool checkValidity();

    /** @brief performs the network simplex.
     *         The solution can be retrieved by NetworkSimplexNode::y() and
     *         NetworkSimplexEdge::x().
     *
     *  @return true : the problem is feasible and an optimum has been found.
     *          false: the problem is infeasible.
     */
    bool solve();

  private:

    /** @brief checks if all the supply/demand values are zero, in which case
     *         the X values in a solution are trivially zero.
     */
    bool areAllBZero();

    /** @brief sets the cost of all the edges with the real cost, and sets
     *         the perturbation to the edges.
     */
    void prepareZeroBGraph();

    /** @brief finds an arbitrary spanning tree as the initial feasible tree.
     */
    void makeInitialZeroBSpanningTree();

    /** @brief set X values of all the edges to zero.
     */
    void findInitialZeroBX();

    /** @brief make a solution if |E| = |V| -1, where there is only one
     *         trivial basis.
     *  @return true if the solution is feasible.
     *          false if the solution is infeasible.
     */
    bool makeTrivialSolution();

    /** @brief  utility routine to create an edge in the spanning tree.
     *
     *  @param  Eg (in): the original edge in mG.
     *
     *  @return edge created in the spanning tree.
     */
    NSSpanningTreeEdge& createSpanningTreeEdge(NetworkSimplexEdge& Eg);

    /** @brief utility routine to remove the specified spanning tree edge.
     */
    void removeSpanningTreeEdge(NetworkSimplexEdge& Eg);

    /** @brief it adds an artificial node and some artificial edges from
     *         the nodes whose supply/demand are non-zero.
     *         Such an augmented graph is used to solve an auxiliary problem
     *         to find an initial feasble solution for the original problem.
     *         It also sets the cost of non-artificial edge to zero, and 
     *         artificial edge to one so that at an optimum of the auxiliary
     *         solution the basis edges's costs are all zero, i.e., they are
     *         non-artificial edges.
     */
    void makeAuxGraph();

    /** @brief constructs the initial spanning tree for the auxiliary problem
     *         it includes all the artificial edges and possibly some
     *         non-artificial ones.     
     */
    void makeInitialAuxSpanningTree();

    /** @brief it selects an arbitrary node of the spanning tree as it root,
     *         orient the tree from it, and then sets the deptsh, predecessor,
     *         and the thread (ordered node list in pre-traversal of DFS).
     */
    void createInitialInternalStructureForSpanningTree();

    /** @brief it finds the y (dual) value of each node from scratch with DFS
     *         starting from he spanning tree root with y = 0.
     */
    void findInitialY();

    /** @brief it finds the z (dual slack) value of each edge based on 
     *         the y values of incident nodes.
     */
    void findInitialZ();

    /** @brief it finds the initial X value of each node by exploiting the
     *         characters of the artificial edges and nodes, and the initial
     *         spanning tree.
     */
    void findInitialAuxX();

    /** @brief perform one pivot.
     *
     *  @return true : a pivot has been performed.
     *          false: a pivot not performed as the current solution is optimum
     */
    bool pivot();

    /** @brief finds (ve2,ve1)-path where (ve1, ve2) is the entering edge.
     *
     *  @param Eg  (in): entering edge
     *
     *  @return list of edges on (ve2,ve1)-path in this ordering.
     */
    vector<edge_list_it_t> findCycle(NetworkSimplexEdge& Eg);

    /** @brief finds the leaving edge in the (ve2,ve1)-path.
     *
     *  @param Eg (in):    enteirng edge
     *
     *  @param cycle (in): (ve2,ve1)-path
     *
     *  @return the leaving edge
     */
    NetworkSimplexEdge& findLeavingEdge(
        NetworkSimplexEdge&     Eg,
        vector<edge_list_it_t>& cycle
    );


    /** @brief utility routine to find the deeper of two incident nodes 
     *         of an edge. Such a node is used to find the subtree root
     *         of STnr.
     *
     *  @param Eleave  (in): edge
     *
     *  @return the deeper incident node
     */
    NSSpanningTreeNode& deeperSpanningTreeNode(NetworkSimplexEdge& Eleave);


    /** @brief utility to find the root side of the incident node of 
     *         the entering edge.
     */
    NSSpanningTreeNode& rootSideOfEnter(NetworkSimplexEdge& Eg);


    /** @brief utility to find the subtree side of the incident node of 
     *         the entering edge.
     */
    NSSpanningTreeNode& nonRootSideOfEnter(NetworkSimplexEdge& Eg);


    /** @brief finds STnr as in the list of nodes.   
     *
     *  @param Eleave            (in):  leaving edge
     *
     *  @param subtreeThreadPrev (out): the previous link in the thread of STnr
     *                                  STnr.
     *
     *  @param subtreeThreadPrev (out): the next link in the thread of STnr
     *
     *  @return list of nodes in STnr.
     *
     *  @remark it brings mNsGeneration of NSSpaningTreeNodes in STnr 
     *          up-to-date.
     */
    vector<node_list_it_t> findSubtree(NetworkSimplexEdge& Eleave);

    /** @brief finds the cut set between STnr and ST\STnr.
     *
     *  @param subtreeNodes (in): nodes in STnr.
     *
     *  @return list of edges in the cut
     *
     *  @remark it depends on mNsGeneration of NSSpaningTreeNodes
     *          to identify the nodes in STnr in O(1).
     */
    vector<edge_list_it_t> findCutSet(vector<node_list_it_t>& subtreeNodes);

    /** @brief updates the X values of the edges on (ve2,ve1)-path and
     *         (ve1,ve2).
     *
     *  @param cycle  (in): (ve2,ve1)-path
     *
     *  @param Eenter (in): entering edge   
     *
     *  @param Eleave (in): leaving edge   
     */
    void updateX(
        vector<edge_list_it_t>& cycle,
        NetworkSimplexEdge&     Eenter,
        NetworkSimplexEdge&     Eleave
    );

    /** @brief updates the Y values of the nodes in STnr.
     *
     *  @param subtreeNodes: (in) nodes in STnr.
     *
     *  @param Eenter:       (in) entering edge
     */
    void updateY(
             vector<node_list_it_t>& subtreeNodes, NetworkSimplexEdge& Eenter);

    /** @brief updates the Z values of the edges in the cut.
     *
     *  @param cut  (in) list of edges in cut.
     */
    void updateZ(vector<edge_list_it_t>& cut);

    /** @brief updates the spanning tree by removing the leaving edge and
     *         adding the entering one.
     *
     *  @param Eenter (in): entering edge   
     *
     *  @param Eleave (in): leaving edge   
     */
    void updateSpanningTree(
        NetworkSimplexEdge& Eenter,
        NetworkSimplexEdge& Eleave
    );

    /** @brief restores the original problem by removing the artificial node
     *         and the edges, restoring the real cost values for the edges.
     */
    void restoreOriginalProblem();

    /** @brief input digraph (network) */
    DiGraph&               mG;

    /** @brief root of the spanning tree as in the iterator into mG. */
    node_list_it_t         mRootIt;

    /** @brief the artificial node */
    node_list_it_t         mArtificialNodeIt;

    /** @brief the artificial edges */
    vector<edge_list_it_t> mArtificialEdges;

    /** @brief list of edges in mG that have negative Z values. */
    list<edge_list_it_t>   mNegativeZEdges;

    /** @brief generation value used for nodes to indicate if the node is
     *         up-to-date.
     */
    long                   mNsGeneration;

    /** @brief the spanning tree */
    Graph                  mSpanningTree;


#ifdef UNIT_TESTS
    void printDebug();
    void printDebugNode(NetworkSimplexNode& Ng);
    void printDebugEdge(NetworkSimplexEdge& Eg);
#endif
};

}// namespace Directed

}// namespace Wailea

#endif /*_WAILEA_DIRECTED_NETWORK_SIMPLEX_HPP_*/
