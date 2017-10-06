#ifndef _WAILEA_DIRECTED_CRITICAL_PATH_HPP_
#define _WAILEA_DIRECTED_CRITICAL_PATH_HPP_

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
 * @file directed/critical_path.hpp
 *
 * @brief 
 * 
 *  @remark This is an extention of PERT critical path
 *          algorithm used in the project management.
 *          In PERT the task dependencies are described in a digraph.
 *          A node specifies a task with a given length in time.
 *          A directed edge from node 1 to node 2 means the task
 *          of node 2 can start only when the task of node 1 has finished.
 *          There are two artificial nodes in PERT. One is a source node
 *          that specifies the beginning of the project, and a destination
 *          node that specifies the end of the whole project.
 *          In PERT, we try to find the earliest possible start time
 *          and the latest possible start time for each node (task) by
 *          forward and backward BFSes. If the earliest and latest times
 *          match at a node, that will be on a critical path.
 *          The difference between the latest and the earliest time is called
 *          slack.
 *
 *          The extentions are as follows:
 *          1. It allows multiple sources and destinaton nods.
 *          2. It admits additional delay time between two tasks
 *             the value is given in the directed edge.
 *          
 *          The implementation is done by 3 BFSes.
 *          1. Forward BFS. Assume the earliest time for all the source nodes
 *             are at 0, and find the earliest time for the destination nodes.
 *
 *          2. Backward BFS. Assume the latest time for the destination nodes
 *             are equal to their earliest time found in the previous BFS,
 *             find the latest time for each node including the source nodes.
 *
 *          3. Forward BFS again. Assume the earliest time for all the source
 *             nodes are equal to their latest time found in the previous BFS,
 *             find the earliest time for each node.
 */

namespace Wailea {

namespace Directed { 

using namespace std;

using namespace Wailea::Undirected;

class CriticalPath;

/** @class  CPDiNode
 *  @brief  This class extends DiNode to perform ciritical path operation
 */
class CPDiNode : public DiNode {

  public:

    inline CPDiNode(long length):
        mLength(length),
        mEarliestStart(0),
        mLatestStart(0),
        mNumNeighborsProcessed(0){;}

    inline virtual ~CPDiNode(){;}

    inline long earliestStart()  const noexcept {return mEarliestStart;}
    inline long latestStart()   const noexcept {return mLatestStart;}
    inline long earliestFinish() const noexcept {
                                              return mEarliestStart + mLength;}
    inline long latestFinish()  const noexcept {return mLatestStart + mLength;}


  protected:

    /** @ brief minimum length of the time required 
     *          perform this task
     */
    long           mLength;

    /** @brief the earlies possible start time of this task.   
     */
    long           mEarliestStart;

    /** @brief the latest possible start time of this task.
     */
    long           mLatestStart;

    /** @brief number of neighbors that have checked this node
     *         used in BFS to enqueue this node.
     */
    size_t         mNumNeighborsProcessed;

    friend class CriticalPath;
#ifdef UNIT_TESTS
  friend class CriticalPathTests;
#endif

};


/** @class  CPDiEdge
 *  @brief  This class extends DiEdge to perform ciritical path operation
 */
class CPDiEdge : public DiEdge {

  public:

    inline CPDiEdge(long length):mLength(length){;}

    inline virtual ~CPDiEdge(){;}

  protected:

    /** @ brief minimum length of the time required 
     *          perform this task
     */
    long           mLength;

    friend class CriticalPath;
#ifdef UNIT_TESTS
  friend class CriticalPathTests;
#endif

};


/** @class  CriticalPath
 *
 *  @brief  main class to perform critical path calculation
 *
 */
class CriticalPath {

  public:
    /** @brief finds critical path in the given digraph.
     *
     *  @param D (in/out) : DiGraph with CPDiNode and CPDiEdges as 
     *                      its nodes and edges.
     *  @remark   Before the call, it is assumed that length values of 
     *            the nodes in CPDiNode and edges in CPDiEdge are set
     *            by the 2nd parameter to their constructors.
     *
     *            After the call, the results can be found with the following.
     *            - CPDiNode::earliestStart()
     *            - CPDiNode::lastestStart()
     *            - CPDiNode::earliestFinish()
     *            - CPDiNode::lastestFinish()
     */
    void findCriticalPaths(DiGraph& D);

  private:

    /** @brief find the source and destination nodes.
     */
    void findTerminals(DiGraph& D);

    /** @brief it sets earliest time of the nodes to 0.
     */
    void resetForForwardPath1(DiGraph& D);

    /** @brief it sets the latest time of the nodes to the specified value
     *         except for the destinations for which it is set to their 
     *         earliest time.
     *
     *  @param latestStart (in): the value set to the latest time of the nodes
     */
    void resetForBackwardPath(DiGraph& D, long latestStart);

    /** @brief it sets the earliest time of the nodes to 0
     *         except for the source nodes for which it is set to their
     *         latest time.
     */
    void resetForForwardPath2(DiGraph& D);

    /** @brief performs a forward BFS and find the earliest time for each
     *         node. It also finds the latest (greatest) start time found.
     *
     *  @return  the latest start time found
     */
    long performForwardPath(DiGraph&   D);

    /** @brief performs a backward BFS and find the latest time for each
     *         node.
     */
    void performBackwardPath(DiGraph&  D);


    /** @brief stores the source nodes of the graph.
     */
    vector<node_list_it_t> mSources;

    /** @brief stores the destination nodes of the graph.
     */
    vector<node_list_it_t> mDestinations;

};

}// namespace Directed

}// namespace Wailea

#endif /* _WAILEA_DIRECTED_CRITICAL_PATH_HPP_*/
