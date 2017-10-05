#ifndef _WAILEA_DIRECTED_ACYCLIC_ORDERING_FINDER_HPP_
#define _WAILEA_DIRECTED_ACYCLIC_ORDERING_FINDER_HPP_

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
 * @file directed/acyclic_ordering_finder.hpp
 *
 * @brief find an acyclic ordering among the nodes of a given digraph that may
 *        contain some cycles.
 * 
 *        Such an ordering can be used to make a digraph acyclic by flipping
 *        the direction of the arcs that are against the ordering.
 *
 *        The the edges in the given graph has a non-negative cost value.
 *        The higher the cost, more resistant to be flipped.
 *
 * @remark
 *        Process
 *
 *        1. Find the feedback edges with a variant of DFS
 *
 *        proc FindFeedbackEdges
 *        Input:  G(N,E)
 *        Output: F (set of feedback edges)
 *        -  For v in N  visited(v) := false and put v to a set L
 *        -  For v in L:
 *            2.1. visited(v) := true
 *            2.2. visit(v)
 *
 *        func visit (v)
 *            active(v) := true
 *            for n in Nout(n) do:
 *                if visited(n) == false then
 *                    remove n from L
 *                    visited(n) := true
 *                if active(n) == true then
 *                    put (v,n) in FeedbackEdges
 *                else
 *                    visit(n)
 *            active(v) := false
 *            
 *        NOTE: Flipping the edges in F would make G acyclic.
 *              However, it is not a good set. It does not consider
 *              the edge cots either.
 * 
 *        2. If |F| > 0 then
 *            add Nsup and Ninf to G
 *            for each (v,w) in F:
 *                remove (v,w) from G
 *                add (v, Nsup) to G
 *                add (Ninf, w) to G
 *     
 *        At this point the altered graph G is acyclic.
 *
 *        NOTE: The alteration above is conceptual. 
 *              Theimplementation does not alter the input graph.
 *
 *        3. Assign acyclic ranks for the nodes in G minimizing the total
 *           edge length. (Here we use network simplex)
 *
 *        4. For each rank for which there are multiple nodes, order them
 *           based on the value in-degree(n) - out-degree(n).
 *
 *        At this point we have a ordered set of nodes in G, possibly including
 *        Ninf and Nsup.
 *
 *        5. With a line sweep algorithm with enter and leave, find the gap
 *           between two nodes in the ordered set that has the minimum total
 *           cost of the edges that run between two nodes.
 *
 *        6. Remove Ninf and Nsup, concatenate two ends of the ordered list
 *           into a cyclic ordered list, and then break the cycle at the 
 *           minimum gap. This gives an acyclic ordering of the nodes in G.
 *
 *        Ex. Consider the following graph as an input
 *            The number on the edge is the cost of it.
 *            (1,2), (1,3), (4,1), (2,3), (3,4), (3,5), (4,5), (5,2)
 *
 *                              10
 *                [1] ----------------------> [2]
 *                /|\\                      / /|\
 *                 |  \                    /   |
 *                 |   \   8            2 /    |
 *                1|    ------->[3]<------     |7
 *                 |            / \            |
 *                 |           /   \           |
 *                 |     1    /     \    4     |
 *                [4]<--------   12  -------->[5]
 *                   ------------------------->
 *
 *             1. Find the feedback edge set.
 *                [1] ---> [2] ---> [3] ---> [4] ===> [1]  
 *                                              (4,1) is a feedback edge
 *                                  [3] ---> [5] ===> [2] 
 *                                              (5,2) is a feedback edge
 *                [1] ---> [3]
 * 
 *             2. Augment the graph with inf, sup and the following edges
 *                Removed: (4,1), (5,2)
 *                Added:   (inf, 1), (inf, 2), (4, sup), (5, sup)
 *
 *                  --------------------------------->
 *                 |  ---->[1] ----------------------> [2]
 *                 | |       \                      /
 *                 | |        \                    /   
 *                 | |         \                  /    
 *                [inf]         ------->[3]<------      ------>[sup]
 *                                      / \            |   --->
 *                                     /   \           |  |
 *                                    /     \          |  |
 *                        [4]<--------       -------->[5] |
 *                         | ------------------------->   |
 *                         |                              |
 *                          ------------------------------
 *
 *             3. Rank the graph with network simplex
 *
 *             Rank:   0       1      2      3      4      5       6
 *
 *                        ---------->
 *                   [inf]--->[1]--->[2]--->          ---------->[sup]
 *                               ---------->[3]--->[4]--->[5]--->
 *                                             ---------->
 *             4. If a rank has multiple nodes, order them.
 *
 *
 *             5. Find the gap that has the minimum edge cost.
 *
 *                  ---------------------- sweep ------------------->
 *
 *              Gap cost   8     25     10     >5<     17     8
 *
 *                             7
 *                        ---------->
 *                         1      10     2                 1
 *                   [inf]--->[1]--->[2]--->          ---------->[sup]
 *                                    8         1      12     7
 *                               ---------->[3]--->[4]--->[5]--->
 *                                                  4
 *                                             ---------->
 *               In this case, the gap between rank 3 and 4 has the minimum
 *               edge cost 5. This is between node 3 and 4.
 *
 *             6. Remove inf, sup, and make an acyclic ordering by breaking
 *                the gap.
 *               
 *               Acyclic ordering: 4, 5, 1, 2, 3
 *
 *               Eventually, (3,4) and (3,5) will be flipped to make the
 *               graph acyclic.
 *
 */
namespace Wailea {

namespace Directed { 

using namespace std;

using namespace Wailea::Directed;


class AOFEdge : public DiEdge {

  public:
    inline AOFEdge():mFeedbackEdge(false),mCost(1){;}

    /** @brief set the cost of the edge
     *         the higer the value the more difficult to flip
     *         this edge to make the graph acyclic.
     */
    inline void setCost(long c){ mCost = c; }

  private:
    bool mFeedbackEdge;
    long mCost;

friend class AcyclicOrderingFinder;

#ifdef UNIT_TESTS
friend class AcyclicOrderingFinderTests;
#endif

};


class AOFNode : public DiNode {

  public:

    /** @brief returns the rank in the acyclic ordering
     */
    inline long rank() { return mRank; }

#ifdef UNIT_TESTS
    long mDebugId;
#endif

  private:

    /** @brief called if this node is to be explored by DFS for the first time
     *   
     *  @param dfsParent (in): parent of this node for the current DFS 
     *                         exploration
     */
    inline void initForDFS(node_list_it_t dfsParent) {
        mVisited       = true;
        mDfsParent     = dfsParent;
        mNextEdge      = incidentEdgesOut().first;
    }

    /** @brief returns true if the current node has an incident edge to be
     *         explored.
     */
    inline bool hasNextChild() {return mNextEdge != incidentEdgesOut().second;}

    /** @brief returns the next incident edge to be explored during the
     *         DFS exploration.
     */
    inline AOFEdge& nextChild() { 
        edge_list_it_t eit = *mNextEdge;
        mNextEdge++;
        return dynamic_cast<AOFEdge&>(*(*eit));
    }

    inline node_list_it_t dfsParent() { return mDfsParent; }

    /** @brief indicates if this node has been visited.
     */
    bool                mVisited;

    /** @brief indicates if the node is on the DFS node stack being explored
     */
    bool                mActive;

    /** @brief points to the next incident edge to be explored by DFS.
     */
    node_incidence_it_t mNextEdge;

    /** @brief holds the pointer to the parent during a DFS exploration.
     */
    node_list_it_t      mDfsParent;

    /** @brief back pointer into the list of unvisited nodes.
     *         This is makes the removal of this node from a list in O(1).
     */
    list<node_list_it_t>::iterator mNodeListBackIt;

    /** @brief rank found by network simplex
     */
    long                mRank;

    /** @brief (sum of cost of outgoing edges)-(sum of cost of incoming edges) 
     *
     *         If this is positive, the pull from the outgoing edges is 
     *         strong, and this node wants to move upward to higher rank.
     *
     *         If this is negative, the pull from the incoming edges is 
     *         strong, and this node wants to move upward to higher rank.
     *
     *         This is also used by sweeping leave/enter algorithm in
     *         findMinimumRankGap().
     */
    long                mCost;

    /** @brief utility routine for std::sort for vector<node_list_it_t>.
     */
    static bool nodeComp(node_list_it_t it1, node_list_it_t it2);

friend class AcyclicOrderingFinder;

#ifdef UNIT_TESTS
friend class AcyclicOrderingFinderTests;
#endif

};


/** @class  AcyclicOrderingFinder
 *
 *  @brief  main class to find an acyclic ordering
 *
 */
class AcyclicOrderingFinder {

  public:
    /** @brief constructor
     *
     *  @param   G (in/out): The inpug graph for which an acyclic ordering
     *                       is to be found.
     */
    inline AcyclicOrderingFinder(DiGraph& G):mG(G),mSupCost(0),mInfCost(0){;}

    inline virtual ~AcyclicOrderingFinder(){;}

    /** @brief finds the acyclic ordering. Also it sets AOFNode::rank()
     *         accordingly.
     *
     *  @return acyclic ordering of the node.  
     */
    vector<node_list_it_t> find();

  private:

    /** @brief find feedback edges using DFS.
     *
     *  @return number of feedback edges found.
     */
    long                   findFeedbackEdges();

    /** @brief find a cyclic ordering among nodes
     *
     *  @param  numFeedbackEdges (in): number of feedback edges found in
     *                                                      findFeedbackEdges()
     *
     *  @return maximum rank number (with zero origin)
     */
    long                   findCyclicRanks(long numFeedbackEdges);

    /** @brief serialize the nodes ordered in the ranks found in 
     *         findCyclicRanks() by ordering the nodes in the same ranks.
     *         ordering.
     *
     *  @param maximum rank number (with zero origin)     
     *
     *  @return serialized cyclic ordering of nodes.
     */
    vector<node_list_it_t> serializeCyclicRanks(long maxRank);


    /** @brief find the gap between two ranks in the cyclic ranks.
     *         in which the cost of the edges that run across is the minimum.
     *
     *  @param  cyclicRanks (in): serialized cyclic ranks
     *
     *  @return the index i for the gap [i-1][i].
     */
    size_t                 findMinimumRankGap(
                                          vector<node_list_it_t>& cyclicRanks);

    /** @brief the input graph */
    DiGraph& mG;

    /** @brief total cost of the edges coming into the sup node.*/
    long     mSupCost;

    /** @brief total cost of the edges going out from the inf node.*/
    long     mInfCost;

#ifdef UNIT_TESTS
friend class AcyclicOrderingFinderTests;
#endif
};


}// namespace Directed

}// namespace Wailea

#endif /*_WAILEA_DIRECTED_ACYCLIC_ORDERING_FINDER_HPP_*/
