#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "directed/dfs_di.hpp"

namespace Wailea {

namespace Directed {

template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
void DFSDiExplorer<BASE_GRAPH,BASE_NODE,BASE_EDGE>::setStartNode(
    DDiNode& s
) noexcept
{
    mCurrentNodeIt = s.backIt();
    s.initExploration(mG.nodes().second);             
}


template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
void DFSDiExplorer<BASE_GRAPH,BASE_NODE,BASE_EDGE>::reset(
    DDiGraph& g,
    bool      reversed
) noexcept
{

    auto nodeItPair = g.nodes();
    for (auto nit = nodeItPair.first; nit != nodeItPair.second; nit++) {
        dynamic_cast<DDiNode&>(*(*nit)).resetForDFS(reversed);
    }

    auto edgeItPair = g.edges();
    for (auto eit = edgeItPair.first; eit != edgeItPair.second; eit++) {
        dynamic_cast<DDiEdge&>(*(*eit)).resetForDFS();
    }

}


template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
void DFSDiExplorer<BASE_GRAPH,BASE_NODE,BASE_EDGE>::run(DDiNode& st) noexcept 
{
    setStartNode(st);
    mStrategy.resetEarlyOut();
    while (oneStep());
}


template <class BASE_GRAPH, class BASE_NODE, class BASE_EDGE>
bool DFSDiExplorer<BASE_GRAPH,BASE_NODE,BASE_EDGE>::oneStep()
{

    DDiNode& currentNode = dynamic_cast<DDiNode&>(*(*mCurrentNodeIt));

    if ( !currentNode.isVisited() ) {

         // It has just entered this node.
         mStrategy.enterNode(currentNode);
         currentNode.setVisited();
         currentNode.resetWaitingForChild();

    }
    else {

        if (!currentNode.isWaitingForChild()) {

            // Ready to explore a new edge if there is one.
            if (!currentNode.hasExploredAllEdges()) {

                // It has at least an unexplored edge. 
                DDiEdge& currentEdge = currentNode.currentDFSEdge();

                if (currentEdge.isVisited()) {

                    // The edge has been already processed as a frond
                    // when the adjacent node was explored. Skipping.
                    currentNode.advanceDFSEdge();
                }
                else {

                    DDiNode& adjacentNode = dynamic_cast<DDiNode&>(
                                    currentEdge.adjacentNode(currentNode));

                    if (adjacentNode.isVisited()) {

                        // The current edge is a frond.

                        // Call setWaitingForChild  and resetWaitingForChild
                        // here to cope with the 
                        // situation where setNextDFSEdge() is called
                        // in processFrond below, in which case,
                        // the currentEdge is not changed but the next one
                        // to visit will be set by setNextDFSEdge().
                        currentNode.setWaitingForChild();
                        mStrategy.processFrond(currentNode,currentEdge);
                        currentNode.resetWaitingForChild();

                        currentEdge.setVisited();
                        // Move on to the next unexplored edge.
                        currentNode.advanceDFSEdge();

                    }
                    else {

                        if(mStrategy.isVisitingChild(currentNode,currentEdge)){

                            // Visit the current edge and the adjacent node.

                            // Call setWaitingForChild here to cope with the 
                            // situation where setNextDFSEdge() is called
                            // in beforeVisitingChild below, in which case,
                            // the currentEdge is not changed but the next one
                            // to visit will be set by setNextDFSEdge().
                            currentNode.setWaitingForChild();
                            mStrategy.beforeVisitingChild
                                                     (currentNode,currentEdge);
                            currentEdge.setVisited();

                            // Going to the adjacent node at the next itration.
                            adjacentNode.initExploration(mCurrentNodeIt);
                            mCurrentNodeIt = adjacentNode.backIt();

                        }
                        else {

                            mStrategy.notVisitingChild
                                                     (currentNode,currentEdge);
                            // Move on to the next unexplored edge.
                            currentNode.advanceDFSEdge();
                        }
                    }
                }
            }
            else {

                // There is no more unexplored edge.
                mStrategy.leaveNode(currentNode);

                // Going back to the parent node.
                mCurrentNodeIt = currentNode.parentInDFS();
            }
        }
        else {

            // It has come back from the last child.
            mStrategy.afterVisitingChild(currentNode,
                                                currentNode.currentDFSEdge());
            currentNode.resetWaitingForChild();
            currentNode.advanceDFSEdge();
        }
    }

    return !mStrategy.isEarlyOut() &&  mCurrentNodeIt != mG.nodes().second;
}

}// namespace Directed

}// namespace Wailea



