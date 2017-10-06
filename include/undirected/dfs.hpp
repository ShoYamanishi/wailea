#ifndef _WAILEA_UNDIRECTED_DFS_HPP_
#define _WAILEA_UNDIRECTED_DFS_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/** @file  undirected/dfs.hpp
 *
 *  @brief implementation of base DFS for undirected graph.
 *
 *  A DFS-based algorithm is implemented by inheriting some of 
 *  the following 5 classes.
 *
 *  - DFSExplorer
 *        This class implement the basic functionality of walking through
 *        the nodes and edges in depth-first way. Durint the walk, at 
 *        specific points, the functions provided in DFSStrategy are called.
 *
 *  - DFSStrategy
 *        This class is a collection of functions called by DFSExplorer
 *        during the DFS exploration.
 *        A DFS-based algorithm inherits this class and implement their own
 *        member functions for the algorithsm.
 *
 *  - DFSGraph
 *        This is a Graph class for the base DFSExplorer.
 *        A DFS-based algorithm can inherit this class and add its specific
 *        members required for their algorithm.
 *
 *  - DFSNode
 *        This is a Node class for the base DFSExploerer.
 *        A DFS-based algorithm can inherit this class and add its specific
 *        members required for their algorithm. An example is to store
 *        an integer for lowpt for Wailea's palm-tree based algorithms.
 *
 *  - DFSEdge
 *        This is an Edge class for the base DFSExploerer.
 *        A DFS-based algorithm can inherit this class and add its specific
 *        members required for their algorithm.
 *
 *  @remark on lifecycle in the exploration of a Node using recursion.
 *
 *   HOOK indicates the point where Strategy's function is called.
 *
 *     proc run(startNode)
 *
 *         startNode.visited <- false
 *         call ProcessOneNode(startNode)
 *
 *     proc ProcessOneNode(curNode)
 *
 *         HOOK[ enterNode(curNode) ]
 *
 *         curNode.visited <- true
 *         curNode.waitingForChild <- false
 *
 *         curEdge <- [first edge in curNode.incidentEdges]
 *         while ( not curNode.hasExploredAllEdges()) {
 *
 *             if (adjacentNode.visited) ) {
 *
 *                 HOOK[ processFrond(curNode, curEdge) ]
 *
 *                 curEdge.Visited <- true
 *
 *             }
 *             else {
 *
 *                 HOOK[ beforeVisitingChild(curNode, curEdge) ]
 *
 *                 curEdge.visited <- true
 *                 curNode.waitingForChild <- true
 *                 call ProcessOneNode(adjacentNode)
 *
 *                 HOOK[ afterVisitingChild(curNode, curEdge) ]
 *
 *                 curNode.WaitingForChild<-false               
 *             }
 *             else{
 *
 *             }
 *             curEdge = curNode.advanceEdge()
 *         }
 *
 *         HOOK[ leaveNode(curNode) ]
 *
 */


namespace Wailea {

namespace Undirected {

template <class BASE_GRAPH,class BASE_NODE,class BASE_EDGE> class DFSEdge;
template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
                                                           class DFSStrategy;

template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
                                                           class DFSExplorer;

template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSNode : public BASE_NODE {

    using DEdge = DFSEdge<BASE_GRAPH, BASE_NODE, BASE_EDGE>;

  public:
    DFSNode():BASE_NODE(),mWaitingForChild(false),mVisited(false){
        checkBase();
    }
    virtual ~DFSNode(){;}
    DFSNode(const DFSNode& rhs) = delete;
    DFSNode(DFSNode&& rhs) = delete;
    DFSNode& operator=(const DFSNode& rhs) = delete;
    DFSNode& operator=(DFSNode&& rhs) = delete;

    inline bool isVisited()const{return mVisited;}
    inline bool setVisited(){return mVisited=true;}
    inline bool resetVisited(){return mVisited=false;}

    inline void resetForDFS(){ mVisited = false; }

    inline void initExploration(const node_list_it_t& parentNode)
    {
        mParentInDFS        = parentNode;
        mWaitingForChild    = false;
        mVisited            = false;
        mEdgeVisiting       = this->incidentEdges().first;
        mEdgeToBeVisited    = this->incidentEdges().second;
        mEdgeToBeVisitedSet = false;
    }

    inline bool isWaitingForChild()const noexcept {return mWaitingForChild;}
    inline void resetWaitingForChild() noexcept {mWaitingForChild = false;}
    inline void setWaitingForChild() noexcept {mWaitingForChild = true;}

    inline bool hasExploredAllEdges() noexcept {
        if (mWaitingForChild) {
            if (mEdgeToBeVisitedSet) {
                return mEdgeToBeVisited == this->incidentEdges().second;
            }
            else{
                auto it = mEdgeVisiting;
                it++;
                return it == this->incidentEdges().second;
            }
        }
        else {
            return mEdgeVisiting == this->incidentEdges().second;
        }
    }

    inline bool isThereUnexplored() noexcept {
        if (mWaitingForChild) {
            if (mEdgeToBeVisitedSet) {
                return mEdgeToBeVisited != this->incidentEdges().second;
            }
            else{
                auto it = mEdgeVisiting;
                it++;
                for(;it != this->incidentEdges().second; it++) {
                    DEdge& e = dynamic_cast<DEdge&>(*(*(*it)));
                    if (e.isVisited()==false) {
                        return true;
                    }
                }
                return false;
            }
        }
        else {
            for(auto it = mEdgeVisiting;it != this->incidentEdges().second;
                                                                       it++) {
                DEdge& e = dynamic_cast<DEdge&>(*(*(*it)));
                if (e.isVisited()==false) {
                    return true;
                }
            }
            return false;
        }
    }


    inline DEdge& currentDFSEdge() noexcept {

        return dynamic_cast<DEdge&>(*(*(*mEdgeVisiting)));

    }

    inline void advanceDFSEdge(){

        if ( mEdgeToBeVisitedSet ){

            mEdgeVisiting = mEdgeToBeVisited;
            mEdgeToBeVisitedSet = false;
            mEdgeToBeVisited = this->incidentEdges().second;
        }
        else if (mEdgeVisiting != this->incidentEdges().second) {

            mEdgeVisiting++;
        }
    }

    /**  @remark notes on the timing to set the 'next' DFS edge.
     *           setNextDFSEdge() is called from the functions of 
     *           DFSStrategy. Following describes the details about
     *           how the next edge to be explored is set.
     *           - If the node n is not visited, then call to this function
     *             has no effect.
     *
     *           - If the node n is being visited but is not the current node
     *             (i.e. the node is on the DFS stack, but not on top.)
     *             the edge set in setNextDFSEdge will be visited next
     *             once DFSExplorer has come back to n and it becomes the 
     *             top node on the DFS stack.
     *
     *           - If the node is being visited and it's the current node
     *                 - from enterNode(): the first edge to be visited from 
     *                   the current node will be set.
     *                 - from leaveNode(): it has no effect.
     *                 - from beforeVisitingChild(): the next edge after
     *                   'currentEdge' is set.
     *                 - from afterVisitingChild(): the next edge after
     *                   'currentEdge' is set.
     *                 - from processFrond(): the next edge after the 
     *                   'currentEdge' (frond) is set.
     */
    inline void setNextDFSEdge(node_incidence_it_t it) noexcept {
        if (mWaitingForChild) {

            mEdgeToBeVisited    = it;
            mEdgeToBeVisitedSet = true;

        }
        else {

            mEdgeVisiting = it;
            mEdgeToBeVisitedSet = false;

        }
    }

    inline node_list_it_t parentInDFS()const noexcept {return mParentInDFS;}


    void checkBase(){
        static_assert(std::is_base_of<Node, BASE_NODE>::value,
                                           "BASE_NODE must be a Node class");
        static_assert(std::is_base_of<Edge, BASE_EDGE>::value,
                                          "BASE_EDGE must be an Edge class");
    }

    /**  @brief back pointer to the parent node being visited.
     *          the chain of nodes following this pointer together with
     *          mEdgeVisiting below form the current DFS stack.
     */
    node_list_it_t      mParentInDFS;

    /**  @brief represents the visiting edge.
     */
    node_incidence_it_t mEdgeVisiting;

    /**  @brief holds the next edge to be visited set eternally.
     */
    node_incidence_it_t mEdgeToBeVisited;

    /**  @brief specifies if mEdgeToBeVisited is set or not.
     */
    bool                mEdgeToBeVisitedSet;

    /**  @brief indicates the current state of exploring.
     *          false: the next edge is going to be explored.
     *          true:  the current edge is being explored, and
     *                 coming back from the child.
     */
    bool mWaitingForChild;

    /**  @brief true if DFSExplorer is exploring or has explored this node.
     */
    bool mVisited;

#ifdef UNIT_TESTS
  friend class DFSTests;
#endif

};


template <class BASE_GRAPH,class BASE_NODE,class BASE_EDGE>
class DFSEdge : public BASE_EDGE {

  public:
    DFSEdge():BASE_EDGE(),mVisited(false){ checkBase(); }
    virtual ~DFSEdge(){;}
    DFSEdge(const DFSEdge& rhs) = delete;
    DFSEdge(DFSEdge&& rhs) = delete;
    DFSEdge& operator=(const DFSEdge& rhs) = delete;
    DFSEdge& operator=(DFSEdge&& rhs) = delete;

    inline void resetForDFS(){mVisited = false;}
    inline bool isVisited()const{return mVisited;}
    inline bool setVisited(){return mVisited=true;}
    inline bool resetVisited(){return mVisited=false;}

    inline DFSNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>& adjacentDFSNode(
        DFSNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>& n
    ) const noexcept {
        return dynamic_cast<DFSNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>&>
                   (this->adjacentNode(n));
    }

  private:

    void checkBase(){
        static_assert(std::is_base_of<Edge, BASE_EDGE>::value, 
                                                  "BASE must be Graph class");
    }

    bool mVisited;

#ifdef UNIT_TESTS
  friend class DFSTests;
#endif

};


template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSGraph : public BASE_GRAPH  {};


/** @class DFSStrategy
 *
 *  @brief base class for a DFS-based algorithm.
 *         DFS-based algorithms inherit from this providing the
 *         actual functionalities in the following.
 *             enterNode()
 *             leaveNode()
 *             shouldExplore()
 *             beforeVisitingChild()
 *             afterVisitingChild()
 *             notVisitingChild()
 *
 */
template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSStrategy {

  public:

    using DEdge = DFSEdge<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DNode = DFSNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>;

#ifndef UNIT_TESTS
    DFSStrategy():mEarlyOut(false){;}
#else
    DFSStrategy():
      mEarlyOut(false),
      enterNodeCnt(0),
      leaveNodeCnt(0),
      processFrondCnt(0),
      isVisitingChildCnt(0),
      notVisitingChildCnt(0),
      beforeVisitingChildCnt(0),
      afterVisitingChildCnt(0){;}
#endif

//    virtual ~DFSStrategy() = default;
    virtual ~DFSStrategy(){;}
    DFSStrategy(const DFSStrategy& rhs) = default;
    DFSStrategy(DFSStrategy&& rhs) = default;
    DFSStrategy& operator=(const DFSStrategy& rhs) = default;
    DFSStrategy& operator=(DFSStrategy&& rhs) = default;

    /** @brief called when the DFSExplorer is exploring a non-leaf node.
     *         DFSExplorer::run calls this.
     */
    virtual void enterNode(DNode& n){
#ifdef UNIT_TESTS
        enterNodeCnt++
#endif
    ;}


    /** @brief called when the DFSExplorer has been staying at the node
     *         and it leaving it
     *         DFSExplorer::run calls this.
     */
    virtual void leaveNode(DNode& n){
#ifdef UNIT_TESTS
        leaveNodeCnt++
#endif
    ;}


    /** @brief called when the DFSExplorer finds a frond or a self-loop,
     *         which is an unexplored arc but its adjacent node has been 
     *         visited.
     */
    virtual void processFrond(DNode& n, DEdge& c ){
#ifdef UNIT_TESTS
        processFrondCnt++
#endif
    ;}


    /** @brief called when the DFSExplorer wants to know if the given edge
     *         should be visited or not. This is called if the edge is
     *         going to be a tree arc, i.e. the other incident node of c
     *         has not been entered.
     *         If c is going to be a frond, i.e. the other incident ndoe of c
     *         has aleady been entered, this is not called, and processFrond()
     *         is called.
     */       
    virtual bool isVisitingChild(DNode& n, DEdge& c ){
#ifdef UNIT_TESTS
        isVisitingChildCnt++;
#endif
        return true;
    }


    /** @brief called when the DFSExplorer does not visit the child 
     *         with the given edge.
     *         This is called when isVisitingChild() returned false.
     */
    virtual void notVisitingChild(DNode& n, DEdge& c ){
#ifdef UNIT_TESTS
        notVisitingChildCnt++;
#endif
    ;}



    /** @brief called when the DFSExplorer is staying at node n, and
     *         is about to explore the new child node c.
     *         DFSExplorer::run calls this.
     */
    virtual void beforeVisitingChild(DNode& n, DEdge& c ){
#ifdef UNIT_TESTS
        beforeVisitingChildCnt++
#endif
    ;}


    /** @brief called when the DFSExplorer is staying at node n, and
     *         has just come back from the visit to the child node c.
     *         DFSExplorer::run calls this.
     */
    virtual void afterVisitingChild(DNode& n, DEdge& c ){
#ifdef UNIT_TESTS
        afterVisitingChildCnt++
#endif
    ;}
                                    

    /** @brief returns if there is any unexplored child for the node n.
     *         Some DFS-based algorithms such as Wailea's SPQR decomposition
     *         use this.
     */
    virtual bool isThereUnexplored(DNode& n){
        return n.isThereUnexplored();
    }

    inline void resetEarlyOut() {mEarlyOut = false;}
    inline void setEarlyOut() {mEarlyOut = true;}
    inline bool isEarlyOut() const {return mEarlyOut;}

  private:

    void checkBase(){
        static_assert(std::is_base_of<Node, BASE_NODE>::value,
                                            "BASE_NODE must be a Node class");
        static_assert(std::is_base_of<Edge, BASE_EDGE>::value,
                                            "BASE_EDGE must be an Edge class");
    }

    bool mEarlyOut;

#ifdef UNIT_TESTS
  public:
    size_t enterNodeCnt;
    size_t leaveNodeCnt;
    size_t processFrondCnt;
    size_t isVisitingChildCnt;
    size_t notVisitingChildCnt;
    size_t beforeVisitingChildCnt;
    size_t afterVisitingChildCnt;

  friend class DFSTests;
#endif


};


/** @class DFSExplorer
 *
 *  @brief performs depth-first exploration of the given graph from
 *         the specified node.
 *
 */
template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSExplorer {

    using DGraph    = DFSGraph<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DEdge     = DFSEdge<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DNode     = DFSNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DStrategy = DFSStrategy<BASE_GRAPH,BASE_NODE,BASE_EDGE>;

  public:

    /** @brief  default constructor is prohibited.
     */
    DFSExplorer()=delete;


    /** @brief  constructor
     *
     *  @param  g        (in): graph in which the DFS exploration is performed.
     *
     *          strategy (in): coleection of routines for the DFS performed at
     *                         the specific hooking points.
     */
    inline DFSExplorer(DGraph& g, DStrategy& s):mG(g),mStrategy(s){
        mCurrentNodeIt=g.nodes().second;
    }

    virtual ~DFSExplorer(){;}

    DFSExplorer(const DFSExplorer& rhs) = default;
    DFSExplorer(DFSExplorer&& rhs) = default;
    DFSExplorer& operator=(const DFSExplorer& rhs) = default;
    DFSExplorer& operator=(DFSExplorer&& rhs) = default;


    /** @brief  inizialize all the nodes and edges in the given graph
     *          before performing DFS.
     *
     *  @param  g  (in): graph whose nodes and edges are initialized.
     *
     *  @remarks on complexity:
     *           this function runs in O(|N|+|E|). It does not use any
     *           extra memory.
     *  @remarks this is a static convenience function.
     */
    void reset(DGraph& g) noexcept;


    /** @brief  performs a DFS from a given node.  
     *
     *  @param  start    (in): the starting node from which the DFS is
     *                         performed.
     *                         Note if the graph is not connected, then not all
     *                         the nodes are explored.
     *
     *  @throws  any exception that the accompanying DFSStrategy's hook 
     *           functions can throw.
     *
     *  @remarks lifecycle in the exploration of a Node using recursion.
     *           HOOK indicates the point where Strategy's function is called.
     *           The member function 'oneStep()' performs one small step of
     *           ProcessOneNode() below.
     *
     *     proc run(startNode)
     *         startNode.Visited<-false
     *         call ProcessOneNode(startNode)
     *
     *     proc ProcessOneNode(curNode)
     *         HOOK[ enterNode(curNode) ]
     *         curNode.Visited <- true
     *         curNode.WaitingForChild <- false
     *         curEdge <- [first edge in curNode.incidentEdges]
     *         while (not curNode.hasExploredAllEdges()) {
     *             if(HOOK[ shouldExplore(curNode,curEdge) ]) {
     *                 HOOK[ beforeVisitingChild(curNode, curEdge) ]
     *                 curEdge.Visited <- true
     *                 curNode.WaitingForChild <- true
     *                 call ProcessOneNode(adjacentNode)
     *                 HOOK[ afterVisitingChild(curNode, curEdge) ]
     *                 curNode.WaitingForChild<-false               
     *             }
     *             else{
     *                 HOOK[ notVisitingChild(curNode, curEdge) ]
     *             }
     *             curEdge = curNode.advanceEdge()
     *         }
     *         HOOK[ leaveNode(curNode) ]
     *
     *  @remarks on exception:
     *           this function itself does not throw any exception, neither
     *           does it allocate any memory dynamically.
     *           if an exception is thrown in any of the hook functions of
     *           DFSStrategy, this function does not catch it.
     *           It is the caller's responsibility to take necessary action
     *           upon exception.
     *    
     *  @remarks on complexity:
     *           this function runs in O(|N|+|E|). It does not use any
     *           extra memory.
     */
    void run(DNode& st) noexcept;

  private:

    /** @brief indicates the node currently explored in DFS.
     */
    node_list_it_t mCurrentNodeIt;

    /** @brief DFSGraph object in which DFS is performed.
     */
    DGraph&        mG;

    /** @brief DFSStrategy whose functions are called during a DFS.
     */
    DStrategy&     mStrategy;

    /** @brief sets the start node for a DFS
     */
    void setStartNode(DNode& s) noexcept;


    /**  @brief performs a step of DFS.  One execution of DFS exploration, 
     *          which is often expressed in recursion like ProcessOneNode
     *          in the description of run(), is split into small steps
     *          and each step is executed in one call to oneStep for 
     *          easier debugging.
     *
     *   @see   run().
     */
    bool oneStep();

#ifdef UNIT_TESTS
  friend class DFSTests;
  friend class SPQRProc3Tests;
  friend class SPQRTreeTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_DFS_HPP_*/
