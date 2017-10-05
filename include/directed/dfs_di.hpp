#ifndef _WAILEA_DIRECTED_DFS_DI_HPP_
#define _WAILEA_DIRECTED_DFS_DI_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "directed/di_base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/** @file  undirected/dfs_di.hpp
 *
 *  @brief implementation of base DFS for directed graph.
 *
 *  A DFS-based algorithm is implemented by inheriting some of 
 *  the following 5 classes.
 *
 *  - DFSDiExplorer
 *        This class implement the basic functionality of walking through
 *        the nodes and edges in depth-first way. During the walk, at 
 *        specific points, the functions provided in DFSDiStrategy are called.
 *
 *  - DFSDiStrategy
 *        This class is a collection of functions called by DFSDiExplorer
 *        during the DFS exploration.
 *        A DFS-based algorithm inherits this class and implement their own
 *        member functions for the algorithsm.
 *
 *  - DFSDiGraph
 *        This is a Graph class for the base DFSDiExplorer.
 *        A DFS-based algorithm can inherit this class and add its specific
 *        members required for their algorithm.
 *
 *  - DFSDiNode
 *        This is a DiNode class for the base DFSDiExploerer.
 *        A DFS-based algorithm can inherit this class and add its specific
 *        members required for their algorithm.  An example is found in 
 *        the PERT critical path algorithm implemented in XXX.
 *
 *  - DFSDiEdge
 *        This is an Edge class for the base DFSDiExploerer.
 *        A DFS-based algorithm can inherit this class and add its specific
 *        members required for their algorithm.
 *
 *
 *  @remark on lifecycle in the exploration of a DiNode using recursion.
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

namespace Directed {

template <class BASE_GRAPH,class BASE_NODE,class BASE_EDGE> class DFSDiEdge;

template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
                                                           class DFSDiStrategy;

template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
                                                           class DFSDiExplorer;


template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSDiNode : public BASE_NODE {

    using DDiEdge = DFSDiEdge<BASE_GRAPH, BASE_NODE, BASE_EDGE>;

  public:
#ifdef UNIT_TESTS
    DFSDiNode(int n):
        BASE_NODE(n),mWaitingForChild(false),mVisited(false), mReversed(false)
    {
        checkBase();
    }
#else
    DFSDiNode():
        BASE_NODE(),mWaitingForChild(false),mVisited(false), mReversed(false)
    {
        checkBase();
    }
#endif
    virtual ~DFSDiNode(){;}
    DFSDiNode(const DFSDiNode& rhs) = delete;
    DFSDiNode(DFSDiNode&& rhs) = delete;
    DFSDiNode& operator=(const DFSDiNode& rhs) = delete;
    DFSDiNode& operator=(DFSDiNode&& rhs) = delete;

    inline bool isVisited()const{return mVisited;}
    inline bool setVisited(){return mVisited=true;}
    inline bool resetVisited(){return mVisited=false;}

    inline void resetForDFS(bool reversed){
                                    mVisited = false; mReversed = reversed; }

    inline void initExploration(const node_list_it_t& parentNode)
    {
        mParentInDFS        = parentNode;
        mWaitingForChild    = false;
        mVisited            = false;
        if (mReversed) {
            mEdgeVisiting       = this->incidentEdgesIn().first;
            mEdgeToBeVisited    = this->incidentEdgesIn().second;
        }
        else {
            mEdgeVisiting       = this->incidentEdgesOut().first;
            mEdgeToBeVisited    = this->incidentEdgesOut().second;
        }
        mEdgeToBeVisitedSet = false;
    }

    inline bool isWaitingForChild()const noexcept {return mWaitingForChild;}
    inline void resetWaitingForChild() noexcept {mWaitingForChild = false;}
    inline void setWaitingForChild() noexcept {mWaitingForChild = true;}

    inline bool hasExploredAllEdges() noexcept {
        if (mWaitingForChild) {
            if (mEdgeToBeVisitedSet) {
                if (mReversed) {
                    return mEdgeToBeVisited == this->incidentEdgesIn().second;
                }
                else {
                    return mEdgeToBeVisited == this->incidentEdgesOut().second;
                }
            }
            else{
                auto it = mEdgeVisiting;
                it++;
                if (mReversed) {
                    return it == this->incidentEdgesIn().second;
                }
                else {
                    return it == this->incidentEdgesOut().second;
                }
            }
        }
        else {
            if (mReversed) {
                return mEdgeVisiting == this->incidentEdgesIn().second;
            }
            else {
                return mEdgeVisiting == this->incidentEdgesOut().second;
            }
        }
    }

    inline bool isThereUnexplored() noexcept {
        if (mWaitingForChild) {
            if (mEdgeToBeVisitedSet) {
                if (mReversed) {
                    return mEdgeToBeVisited != this->incidentEdgesIn().second;
                }
                else {
                    return mEdgeToBeVisited != this->incidentEdgesOut().second;
                }

            }
            else{
                auto it = mEdgeVisiting;
                it++;
                if (mReversed) {
                    for(;it != this->incidentEdgesIn().second; it++) {
                        DDiEdge& e = dynamic_cast<DDiEdge&>(*(*(*it)));
                        if (e.isVisited()==false) {
                            return true;
                        }
                    }
                }
                else {
                    for(;it != this->incidentEdgesOut().second; it++) {
                        DDiEdge& e = dynamic_cast<DDiEdge&>(*(*(*it)));
                        if (e.isVisited()==false) {
                            return true;
                        }
                    }
                }
                return false;
            }
        }
        else {
            if (mReversed) {
                for(auto it = mEdgeVisiting;
                                  it != this->incidentEdgesIn().second; it++) {
                                                                      
                    DDiEdge& e = dynamic_cast<DDiEdge&>(*(*(*it)));
                    if (e.isVisited()==false) {
                        return true;
                    }
                }
            }
            else {
                for(auto it = mEdgeVisiting;
                                 it != this->incidentEdgesOut().second; it++) {
                                                                      
                    DDiEdge& e = dynamic_cast<DDiEdge&>(*(*(*it)));
                    if (e.isVisited()==false) {
                        return true;
                    }
                }
            }
            return false;
        }
    }


    inline DDiEdge& currentDFSEdge() noexcept {

        return dynamic_cast<DDiEdge&>(*(*(*mEdgeVisiting)));

    }

    inline void advanceDFSEdge(){

        if ( mEdgeToBeVisitedSet ){

            mEdgeVisiting = mEdgeToBeVisited;
            mEdgeToBeVisitedSet = false;
            if (mReversed) { 
                mEdgeToBeVisited = this->incidentEdgesIn().second;
            }
            else {
                mEdgeToBeVisited = this->incidentEdgesOut().second;
            }
        }
        else if (mReversed) {
            if (mEdgeVisiting != this->incidentEdgesIn().second) {
                mEdgeVisiting++;
            }
        }
        else {
            if (mEdgeVisiting != this->incidentEdgesOut().second) {
                mEdgeVisiting++;
            }
        }
    }

    /**  @remark notes on the timing to set the 'next' DFS edge.
     *           setNextDFSEdge() is called from the functions of 
     *           DFSDiStrategy. Following describes the details about
     *           how the next edge to be explored is set.
     *           - If the node n is not visited, then call to this function
     *             has no effect.
     *
     *           - If the node n is being visited but is not the current node
     *             (i.e. the node is on the DFS stack, but not on top.)
     *             the edge set in setNextDFSEdge will be visited next
     *             once DFSDiExplorer has come back to n and it becomes the 
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
        static_assert(std::is_base_of<DiNode, BASE_NODE>::value,
                                           "BASE_NODE must be a Node class");
        static_assert(std::is_base_of<DiEdge, BASE_EDGE>::value,
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
    bool                mWaitingForChild;


    /**  @brief true if DFSDiExplorer is exploring or has explored this node.
     */
    bool                mVisited;


    /** @brief indicates this DFS for di-graph is in the reversed orientatoin
     *         i.e.,  following in-neibors rather than out-neighbors.
     */
    bool                mReversed;


#ifdef UNIT_TESTS
  friend class DFSDiTests;
#endif

};


template <class BASE_GRAPH,class BASE_NODE,class BASE_EDGE>
class DFSDiEdge : public BASE_EDGE {

  public:
    DFSDiEdge():BASE_EDGE(),mVisited(false){ checkBase(); }
    virtual ~DFSDiEdge(){;}
    DFSDiEdge(const DFSDiEdge& rhs) = delete;
    DFSDiEdge(DFSDiEdge&& rhs) = delete;
    DFSDiEdge& operator=(const DFSDiEdge& rhs) = delete;
    DFSDiEdge& operator=(DFSDiEdge&& rhs) = delete;

    inline void resetForDFS(){mVisited = false;}
    inline bool isVisited()const{return mVisited;}
    inline bool setVisited(){return mVisited=true;}
    inline bool resetVisited(){return mVisited=false;}

    inline DFSDiNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>& adjacentDFSDiNode(
        DFSDiNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>& n
    ) const noexcept {
        return dynamic_cast<DFSDiNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>&>
                   (this->adjacentNode(n));
    }

  private:

    void checkBase(){
        static_assert(std::is_base_of<DiEdge, BASE_EDGE>::value, 
                                                  "BASE must be Graph class");
    }

    bool mVisited;

#ifdef UNIT_TESTS
  friend class DFSDiTests;
#endif

};


template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSDiGraph : public BASE_GRAPH  {};


/** @class DFSDiStrategy
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
class DFSDiStrategy {

  public:

    using DDiEdge = DFSDiEdge<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DDiNode = DFSDiNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>;

#ifndef UNIT_TESTS
    DFSDiStrategy():mEarlyOut(false){;}
#else
    DFSDiStrategy():
      mEarlyOut(false),
      enterNodeCnt(0),
      leaveNodeCnt(0),
      processFrondCnt(0),
      isVisitingChildCnt(0),
      notVisitingChildCnt(0),
      beforeVisitingChildCnt(0),
      afterVisitingChildCnt(0){;}
#endif

//    virtual ~DFSDiStrategy() = default;
    virtual ~DFSDiStrategy(){;}
    DFSDiStrategy(const DFSDiStrategy& rhs) = default;
    DFSDiStrategy(DFSDiStrategy&& rhs) = default;
    DFSDiStrategy& operator=(const DFSDiStrategy& rhs) = default;
    DFSDiStrategy& operator=(DFSDiStrategy&& rhs) = default;

    /** @brief called when the DFSDiExplorer is exploring a non-leaf node.
     *         DFSDiExplorer::run calls this.
     */
    virtual void enterNode(DDiNode& n){
#ifdef UNIT_TESTS
        enterNodeCnt++
#endif
    ;}


    /** @brief called when the DFSDiExplorer has been staying at the node
     *         and it leaving it
     *         DFSDiExplorer::run calls this.
     */
    virtual void leaveNode(DDiNode& n){
#ifdef UNIT_TESTS
        leaveNodeCnt++
#endif
    ;}


    /** @brief called when the DFSDiExplorer finds a frond or a self-loop,
     *         which is an unexplored arc but its adjacent node has been 
     *         visited.
     */
    virtual void processFrond(DDiNode& n, DDiEdge& c ){
#ifdef UNIT_TESTS
        processFrondCnt++
#endif
    ;}


    /** @brief called when the DFSDiExplorer wants to know if the given edge
     *         should be visited or not. This is called if the edge is
     *         going to be a tree arc, i.e. the other incident node of c
     *         has not been entered.
     *         If c is going to be a frond, i.e. the other incident ndoe of c
     *         has aleady been entered, this is not called, and processFrond()
     *         is called.
     */       
    virtual bool isVisitingChild(DDiNode& n, DDiEdge& c ){
#ifdef UNIT_TESTS
        isVisitingChildCnt++;
#endif
        return true;
    }


    /** @brief called when the DFSDiExplorer does not visit the child 
     *         with the given edge.
     *         This is called when isVisitingChild() returned false.
     */
    virtual void notVisitingChild(DDiNode& n, DDiEdge& c ){
#ifdef UNIT_TESTS
        notVisitingChildCnt++;
#endif
    ;}



    /** @brief called when the DFSDiExplorer is staying at node n, and
     *         is about to explore the new child node c.
     *         DFSDiExplorer::run calls this.
     */
    virtual void beforeVisitingChild(DDiNode& n, DDiEdge& c ){
#ifdef UNIT_TESTS
        beforeVisitingChildCnt++
#endif
    ;}


    /** @brief called when the DFSDiExplorer is staying at node n, and
     *         has just come back from the visit to the child node c.
     *         DFSDiExplorer::run calls this.
     */
    virtual void afterVisitingChild(DDiNode& n, DDiEdge& c ){
#ifdef UNIT_TESTS
        afterVisitingChildCnt++
#endif
    ;}
                                    

    /** @brief returns if there is any unexplored child for the node n.
     *         Some DFS-based algorithms such as Wailea's SPQR decomposition
     *         use this.
     */
    virtual bool isThereUnexplored(DDiNode& n){
        return n.isThereUnexplored();
    }

    inline void resetEarlyOut() {mEarlyOut = false;}
    inline void setEarlyOut() {mEarlyOut = true;}
    inline bool isEarlyOut() const {return mEarlyOut;}

  private:

    void checkBase(){
        static_assert(std::is_base_of<DiNode, BASE_NODE>::value,
                                            "BASE_NODE must be a Node class");
        static_assert(std::is_base_of<DiEdge, BASE_EDGE>::value,
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

  friend class DFSDiTests;
#endif


};





/** @class DFSDiExplorer
 *
 *  @brief performs depth-first exploration of the given graph from
 *         the specified node.
 *
 */
template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
class DFSDiExplorer {

    using DDiGraph    = DFSDiGraph<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DDiEdge     = DFSDiEdge<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DDiNode     = DFSDiNode<BASE_GRAPH,BASE_NODE,BASE_EDGE>;
    using DDiStrategy = DFSDiStrategy<BASE_GRAPH,BASE_NODE,BASE_EDGE>;

  public:

    /** @brief  default constructor is prohibited.
     */
    DFSDiExplorer()=delete;


    /** @brief  constructor
     *
     *  @param  g        (in): graph in which the DFS exploration is performed.
     *
     *          strategy (in): coleection of routines for the DFS performed at
     *                         the specific hooking points.
     */
    inline DFSDiExplorer(DDiGraph& g, DDiStrategy& s):mG(g),mStrategy(s){
        mCurrentNodeIt=g.nodes().second;
    }

    virtual ~DFSDiExplorer(){;}

    DFSDiExplorer(const DFSDiExplorer& rhs) = default;
    DFSDiExplorer(DFSDiExplorer&& rhs) = default;
    DFSDiExplorer& operator=(const DFSDiExplorer& rhs) = default;
    DFSDiExplorer& operator=(DFSDiExplorer&& rhs) = default;


    /** @brief  inizialize all the nodes and edges in the given graph
     *          before performing DFS.
     *
     *  @param  g  (in): graph whose nodes and edges are initialized.
     *
     *  @param  reversed (in): set to true if the exploration will be
     *                   in the reversed direction i.e., following in-
     *                   neighbors instead of out-neibors.
     *
     *  @remarks on complexity:
     *           this function runs in O(|N|+|E|). It does not use any
     *           extra memory.
     *  @remarks this is a static convenience function.
     */
    void reset(DDiGraph& g, bool reversed) noexcept;


    /** @brief  performs a DFS from a given node.  
     *
     *  @param  start    (in): the starting node from which the DFS is
     *                         performed.
     *                         Note if the graph is not connected, then not all
     *                         the nodes are explored.
     *
     *  @throws  any exception that the accompanying DFSDiStrategy's hook 
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
     *           DFSDiStrategy, this function does not catch it.
     *           It is the caller's responsibility to take necessary action
     *           upon exception.
     *    
     *  @remarks on complexity:
     *           this function runs in O(|N|+|E|). It does not use any
     *           extra memory.
     */
    void run(DDiNode& st) noexcept;

  private:

    /** @brief indicates the node currently explored in DFS.
     */
    node_list_it_t mCurrentNodeIt;

    /** @brief DFSGraph object in which DFS is performed.
     */
    DDiGraph&        mG;

    /** @brief DFSDiStrategy whose functions are called during a DFS.
     */
    DDiStrategy&     mStrategy;

    /** @brief sets the start node for a DFS
     */
    void setStartNode(DDiNode& s) noexcept;


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
  friend class DFSDiTests;
#endif

};

}// namespace Directed

}// namespace Wailea

#endif /*_WAILEA_DIRECTED_DFS_DI_HPP_*/
