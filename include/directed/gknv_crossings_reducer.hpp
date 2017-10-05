#ifndef _WAILEA_DIRECTED_GKNV_CROSSINGS_REDUCER_HPP_
#define _WAILEA_DIRECTED_GKNV_CROSSINGS_REDUCER_HPP_

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
 * @file directed/gknv_crossing_reducer.hpp
 *
 * @brief it tries to reduce the number of crossings for a given acyclic
 *        digraph whose nodes are given acyclic ranks
 *
 *  @reference [GKNV] E. R. Gansner, E. Koutsofios, S. C. North, and
 *             K.-P Vo. A technique for drawing directed graphs. IEEE
 *             Transactions on Software Engineering, 19(3):214–230, 
 *             March 1993.
 */
namespace Wailea {

namespace Directed {

using namespace std;

using namespace Wailea::Directed;



/** @class  GKNVcrossingsReducer
 *
 *  @brief
 */
class GKNVcrossingsReducer {

  public:

    /** @brief constructor
     *
     *  @param   G (in/out): The inpug digraph
     */
    inline GKNVcrossingsReducer(DiGraph& G):mG(G){;}

    inline virtual ~GKNVcrossingsReducer(){;}


    /** @brief main function to attempt to reduce the number of crossings
     *         of edges using GKNV heuristics for a given node rank assignment
     *         
     *  @param ranks  (in/out): outer vector represents the ranks and the inner
     *                  vector at each rank holds the nodes in that rank.
     *                  After a call, the nodes in an inner vector are arranged
     *
     *  @param numIterations (in): number of iterations to apply the heuristics
     *
     *  @return number of crossings with the resultant node arrangement.
     *
     */
    long reduce(vector<vector<node_list_it_t> >& ranks, long numIterations);

  private:


    /** @brief reorder the nodes in each rank based on DFS.
     *
     *  @param ranks (in/out): ranks. Each rank has an ordered list of nodes.
     */
    void initialOrderByDFS(vector<vector<node_list_it_t> >& ranks);


    /** @brief one iteration of median heuristic either forward or backward
     *
     *  @param  oldRanks (in):  current node arrangement.
     *
     *  @param  newRanks (out): new node arrangement
     *
     *  @param  forward  (in): true if the direction is from rank 0 forward.
     *                         false if the direction is down to 0 backward.
     */
    void applyMedianHeuristic(
        vector<vector<node_list_it_t> >& oldRanks,
        vector<vector<node_list_it_t> >& newRanks,
        bool forward
    );


    /** @brief applies transpose heuristic. Swap two adjacent nodes in the 
     *         the same rank and see if it reduces the crossings.
     *
     *  @param  ranks (in/out): node arrangement
     */
    void applyTransposeHeuristic(vector<vector<node_list_it_t> >& ranks);


    /** @brief try swapping the adjacent nodes on the left hand side.
     *         Used for the first two adjacent ranks
     *
     *  @param   leftRank    (in): rank[0]
     *
     *  @param   rightRank   (in): rank[1]
     */
    void tryTransposeLeft(
        vector<node_list_it_t>& leftRank,
        vector<node_list_it_t>& rightRank
    );


    /** @brief try swapping the adjacent nodes on the mid rank.
     *
     *  @param   leftRank    (in): rank[i-1]
     *
     *  @param   midRank     (in): rank[i]
     *
     *  @param   rightRank   (in): rank[i+1]
     */
    void tryTransposeMid(
        vector<node_list_it_t>& leftRank,
        vector<node_list_it_t>& midRank,
        vector<node_list_it_t>& rightRank
    );


    /** @brief try swapping the adjacent nodes on the left hand side.
     *         Used for the last two adjacent ranks
     *
     *  @param   leftRank    (in): rank[last -1]
     *
     *  @param   rightRank   (in): rank[last]
     */
    void tryTransposeRight(
        vector<node_list_it_t>& leftRank,
        vector<node_list_it_t>& rightRank
    );


    /** @brief calculates the number of crossings in the input graph in mG.
     *
     *  @param ranks (in): ranks. Each rank has an ordered list of nodes.
     *
     *  @return number of crossings
     */
    long calculateNumberOfCrossings(vector<vector<node_list_it_t> >& ranks);


    /** @brief calculates the number of crossings between
     *         two consecutive ranks as a bipartite graph
     *
     *  @param leftNodes (in): ordered list of nodes on the left hand side,
     *                         assuming the graph flows from left to right.
     *
     *  @param rightNodes (in): ordered list of nodes on the right hand side.
     *
     *  @return number of crossings
     */
    long calculateNumberOfCrossings(
        vector<node_list_it_t>& leftNodes,
        vector<node_list_it_t>& rightNodes
    );


    /** @brief reorder the right hand side of the nodes based on the median
     *         heuristic described in GKNV.
     *
     *  @param leftNodes  (in): ordered list of left nodes.
     *
     *  @param rightNodes (in): ordered list of right nodes.
     *
     *  @param newRightNodes (out): reordered list of right nodes.
     */
    void reorderRightSideByMedianHeuristic(
        vector<node_list_it_t>& leftNodes,
        vector<node_list_it_t>& rightNodes,
        vector<node_list_it_t>& newRightNodes
    );


    /** @brief reorder the left hand side of the nodes based on the median
     *         heuristic described in GKNV.
     *
     *  @param leftNodes  (in): ordered list of left nodes.
     *
     *  @param rightNodes (in): ordered list of right nodes.
     *
     *  @param newLeftNodes (out): reordered list of left nodes.
     */
    void reorderLeftSideByMedianHeuristic(
        vector<node_list_it_t>& leftNodes,
        vector<node_list_it_t>& rightNodes,
        vector<node_list_it_t>& newLeftNodes
    );


    /** @brief based on the current node ordering in each rank,
     *         reorder the incident edge lists of the nodes.
     *
     *  @param ranks (in): ranks. Each rank has an ordered list of nodes.
     */
    void rearrangeIncidenceLists(
        vector<vector<node_list_it_t> >& ranks
    );


    /** @brief the input graph */
    DiGraph& mG;


    /** @brief utility functions for std::sort() to rearrange the
     *         incident edge list of nodes.
     */
    static bool nodeCompSrc(edge_list_it_t it1, edge_list_it_t it2);
    static bool nodeCompDst(edge_list_it_t it1, edge_list_it_t it2);


#ifdef UNIT_TESTS
friend class GKNVcrossingsReducerTests;
#endif
};


}// namespace Directed

}// namespace Wailea

#endif /* _WAILEA_DIRECTED_GKNV_CROSSINGS_REDUCER_HPP_*/
