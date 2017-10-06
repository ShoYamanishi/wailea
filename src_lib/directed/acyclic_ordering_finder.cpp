#include <algorithm>
#include "directed/acyclic_ordering_finder.hpp"
#include "directed/network_simplex.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file directed/acyclic_ordering_finder.cpp
 */
namespace Wailea {

namespace Directed { 

using namespace std;

using namespace Wailea::Directed;


long AcyclicOrderingFinder::findCyclicRanks(long numFeedbackEdges)
{
//cerr << "findCyclicRanks() BEGIN\n";
    // Network used for network simplex
    DiGraph network;

    mSupCost = 0;
    mInfCost = 0;

    // Create nodes in the network
    auto nPair = mG.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
//cerr << "findCyclicRanks() 1\n";
        auto& Ng = dynamic_cast<AOFNode&>(*(*nit));
#ifdef UNIT_TESTS
        auto& Nn = dynamic_cast<NetworkSimplexNode&>(
           network.addNode(std::make_unique<NetworkSimplexNode>(Ng.mDebugId)));
#else
        auto& Nn = dynamic_cast<NetworkSimplexNode&>(
                      network.addNode(std::make_unique<NetworkSimplexNode>()));
#endif
        Ng.pushIGForwardLink(Nn.backIt());
        Nn.pushIGBackwardLink(Ng.backIt());
//cerr << "findCyclicRanks() 2\n";
        long cost = 0;
        auto iPairIn = Ng.incidentEdgesIn();
        for (auto iit = iPairIn.first; iit != iPairIn.second; iit++) {
            auto& E = dynamic_cast<AOFEdge&>(*(*(*iit)));
            cost -= E.mCost;
            if (E.mFeedbackEdge) {
                mInfCost += E.mCost;
            }
        }
//cerr << "findCyclicRanks() 3\n";
        auto iPairOut = Ng.incidentEdgesOut();
        for (auto iit = iPairOut.first; iit != iPairOut.second; iit++) {
            auto& E = dynamic_cast<AOFEdge&>(*(*(*iit)));
            cost +=  E.mCost;
            if (E.mFeedbackEdge) {
                mSupCost -= E.mCost;
            }
        }
//cerr << "findCyclicRanks() 4: " << cost << "\n";
        Ng.mCost = cost;
        Nn.setB(cost * -1);
    }
//cerr << "findCyclicRanks() 5\n";
    // Create two artificial extremal nodes.

#ifdef UNIT_TESTS
    auto& Nsup = dynamic_cast<NetworkSimplexNode&>(
             network.addNode(std::make_unique<NetworkSimplexNode>(10)));
    auto& Ninf = dynamic_cast<NetworkSimplexNode&>(
             network.addNode(std::make_unique<NetworkSimplexNode>(-2)));
#else
    auto& Nsup = dynamic_cast<NetworkSimplexNode&>(
                      network.addNode(std::make_unique<NetworkSimplexNode>()));
    auto& Ninf = dynamic_cast<NetworkSimplexNode&>(
                      network.addNode(std::make_unique<NetworkSimplexNode>()));
#endif
    Nsup.setB(mSupCost * -1);
    Ninf.setB(mInfCost * -1);
//cerr << "findCyclicRanks() 6\n";
    // Initialize the edges
    auto ePair = mG.edges();
    for (auto eit = ePair.first; eit != ePair.second; eit++) {
//cerr << "findCyclicRanks() 7\n";
        auto& Eg = dynamic_cast<AOFEdge&>(*(*eit));

        auto& Ngs = dynamic_cast<AOFNode&>(Eg.incidentNodeSrc());
        auto& Ngd = dynamic_cast<AOFNode&>(Eg.incidentNodeDst());

        auto& Nns = dynamic_cast<NetworkSimplexNode&>(Ngs.IGForwardLinkRef());
        auto& Nnd = dynamic_cast<NetworkSimplexNode&>(Ngd.IGForwardLinkRef());

        if(Eg.mFeedbackEdge) {
//cerr << "findCyclicRanks() 8\n";

            auto  ep1 = std::make_unique<NetworkSimplexEdge>();
            auto  ep2 = std::make_unique<NetworkSimplexEdge>();
            auto& En1 = dynamic_cast<NetworkSimplexEdge&>(
                                   network.addEdge(std::move(ep1), Nsup, Nns));
            auto& En2 = dynamic_cast<NetworkSimplexEdge&>(
                                   network.addEdge(std::move(ep2), Nnd, Ninf));
            En1.setC(-1);
            En2.setC(-1);
        }
        else {
//cerr << "findCyclicRanks() 9\n";
            auto  ep = std::make_unique<NetworkSimplexEdge>();
            auto& En = dynamic_cast<NetworkSimplexEdge&>(
                                     network.addEdge(std::move(ep), Nnd, Nns));
            En.setC(-1);
        }
    }
//cerr << "findCyclicRanks() 10\n";
    if (numFeedbackEdges==0) {
//cerr << "findCyclicRanks() 11\n";
        // Remove the artificial extremal nodes if there is no feedback edge.
        network.removeNode(Nsup);
        network.removeNode(Ninf);
    }
//cerr << "findCyclicRanks() 12\n";
    NetworkSimplex ns(network);
    ns.solve();
//cerr << "findCyclicRanks() 13\n";
    // Set the rank
    long minRank = 0;
    long maxRank = 0;
//cerr << "findCyclicRanks() 14\n";
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
//cerr << "findCyclicRanks() 15\n";
        auto& Ng = dynamic_cast<AOFNode&>(*(*nit));
        auto& Nn = dynamic_cast<NetworkSimplexNode&>(Ng.IGForwardLinkRef());
        Ng.mRank = Nn.y();
//cerr << "findCyclicRanks() 16:" << Ng.mRank << "\n";
        if (nit == nPair.first) {
            minRank = Ng.mRank;
            maxRank = Ng.mRank;
        }
        else {
            if (minRank > Ng.mRank) {
                minRank = Ng.mRank;
            }
            if (maxRank < Ng.mRank) {
                maxRank = Ng.mRank;
            }
        }
//cerr << "findCyclicRanks() 17\n";
        Ng.popIGForwardLink();
//cerr << "findCyclicRanks() 18\n";
    }
//cerr << "findCyclicRanks() 19\n";
    // Normalize the rank with zero origin.
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
//cerr << "findCyclicRanks() 20\n";
        auto& Ng = dynamic_cast<AOFNode&>(*(*nit));
        Ng.mRank -= minRank;
    }
//cerr << "findCyclicRanks() 21\n";
    maxRank -= minRank;
//cerr << "findCyclicRanks() 22\n";
    return maxRank;
}


// Compare utility for std::sort.
bool AOFNode::nodeComp(node_list_it_t it1, node_list_it_t it2)
{
    auto& N1 = dynamic_cast<AOFNode&>(*(*it1));
    auto& N2 = dynamic_cast<AOFNode&>(*(*it2));
    return N1.mCost < N2.mCost;
}


vector<node_list_it_t> AcyclicOrderingFinder::serializeCyclicRanks(
    long maxRank
) {
    vector<node_list_it_t> flattenedRanks;
    vector<vector<node_list_it_t> >ranks;    

    for (size_t i = 0 ; i <= maxRank ; i++) {
        vector<node_list_it_t> r;
        ranks.push_back(std::move(r));
    }

    auto nPair = mG.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& N = dynamic_cast<AOFNode&>(*(*nit));
        ranks[N.mRank].push_back(nit);

    }

    size_t index = 0;
    for (auto& rank : ranks) {

        std::sort (rank.begin(), rank.end(), AOFNode::nodeComp);
        for (auto nit : rank) {

            auto& N = dynamic_cast<AOFNode&>(*(*nit));
            flattenedRanks.push_back(nit);
            N.mRank = index;
            index++;

        }
    }

    return flattenedRanks;    
}


size_t AcyclicOrderingFinder::findMinimumRankGap(
    vector<node_list_it_t>& cyclicRanks
) {
//cerr << "findMinimumRankGap()\n";
    // Sweeping enter and leave alrogithm.
    auto   numActiveEdges    = mInfCost;
    auto   minNumActiveEdges = mInfCost;
    size_t minIndex          = 0;
//cerr << numActiveEdges << "\n";
    for (size_t i = 0; i < cyclicRanks.size(); i++) {    

        auto& N = dynamic_cast<AOFNode&>(*(*(cyclicRanks[i])));
        numActiveEdges += N.mCost;
//cerr << numActiveEdges << "\n";
        if (numActiveEdges < minNumActiveEdges) {

            minNumActiveEdges = numActiveEdges;
            minIndex          = i + 1;

        }
    }

    return minIndex;

}


long AcyclicOrderingFinder::findFeedbackEdges()
{
//cerr << "findFeedbackEdges() BEGIN\n";
    long numFeedbackEdges = 0;
    list<node_list_it_t> unvisitedNodes;
    long unvisitedNodesSize = 0;
//cerr << "findFeedbackEdges() 1\n";
    // Initialize the nodes.
    auto nPair = mG.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
//cerr << "findFeedbackEdges() 2\n";
        auto& N = dynamic_cast<AOFNode&>(*(*nit));
        N.mVisited = false;
        N.mActive  = false;
        N.mNodeListBackIt = unvisitedNodes.insert(
                                             unvisitedNodes.end(), N.backIt());
        unvisitedNodesSize++;
    }
//cerr << "findFeedbackEdges() 3\n";
    // Initialize the edges
    auto ePair = mG.edges();
    for (auto eit = ePair.first; eit != ePair.second; eit++) {
//cerr << "findFeedbackEdges() 4\n";
        auto& E = dynamic_cast<AOFEdge&>(*(*eit));
        E.mFeedbackEdge = false;
    }

    while(unvisitedNodesSize > 0) {
//cerr << "findFeedbackEdges() 5\n";
        auto&R = dynamic_cast<AOFNode&>(*(*(*(unvisitedNodes.begin()))));
        R.initForDFS(mG.nodes().second);
        R.mActive = true;
        unvisitedNodes.erase(R.mNodeListBackIt);
        unvisitedNodesSize--;
        auto nodeBeingVisited = R.backIt();

        while(nodeBeingVisited != mG.nodes().second) {
//cerr << "findFeedbackEdges() 6\n";
            auto& N = dynamic_cast<AOFNode&>(*(*(nodeBeingVisited)));

            if (N.hasNextChild()) {
//cerr << "findFeedbackEdges() 7\n";
                auto& E = N.nextChild();
                auto& A = dynamic_cast<AOFNode&>(E.adjacentNode(N));
                if (!A.mVisited) {
//cerr << "findFeedbackEdges() 8\n";
                    unvisitedNodes.erase(A.mNodeListBackIt);
                    unvisitedNodesSize--;
                    A.initForDFS(N.backIt());
                    N.mActive = true;
                    nodeBeingVisited = A.backIt();
                }
                else if (A.mActive) {
//cerr << "findFeedbackEdges() 9\n";
                    E.mFeedbackEdge = true;
                    numFeedbackEdges++;
                }
            }
            else {
//cerr << "findFeedbackEdges() 10\n";
                N.mActive = false;
                nodeBeingVisited = N.dfsParent();
            }
//cerr << "findFeedbackEdges() 11\n";
        }
//cerr << "findFeedbackEdges() 12\n";
    }
//cerr << "findFeedbackEdges() END: " << numFeedbackEdges << "\n";
    return numFeedbackEdges;
}


vector<node_list_it_t> AcyclicOrderingFinder::find()
{
    long numFeedbackEdges                 = findFeedbackEdges();

    long maxRank                          = findCyclicRanks(numFeedbackEdges);

    vector<node_list_it_t> cyclicOrdering = serializeCyclicRanks(maxRank);
                                            
    if (mInfCost ==0) {
        // Already acyclic.
        return cyclicOrdering;
    }

    size_t minIndex = findMinimumRankGap(cyclicOrdering);

    vector<node_list_it_t> acyclicOrdering;
    for (size_t i = minIndex; i < cyclicOrdering.size(); i++) {
        acyclicOrdering.push_back(cyclicOrdering[i]);
    }
    for (size_t i = 0 ; i < minIndex; i++) {
        acyclicOrdering.push_back(cyclicOrdering[i]);
    }

    return acyclicOrdering;
}

}// namespace Directed

}// namespace Wailea
