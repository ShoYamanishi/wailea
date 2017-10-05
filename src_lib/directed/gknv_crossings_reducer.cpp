#include "directed/gknv_crossings_reducer.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file directed/gknv_crossings_reducer.cpp
 *
 * @brief
 *
 */
namespace Wailea {

namespace Directed {

using namespace std;

using namespace Wailea::Directed;


void GKNVcrossingsReducer::initialOrderByDFS(
    vector<vector<node_list_it_t> >& ranks
) {

    /** @class GknvDfsNode
     *
     *  @brief internal DiNode to perform DFS in initialOrderByDFS().
     */
    class GknvDfsNode :public DiNode {

        /** @brief called if this node is to be explored by DFS for the first
         *         time
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
        inline bool hasNextChild() {
                                return mNextEdge != incidentEdgesOut().second;}

        /** @brief returns the next incident edge to be explored during the
         *         DFS exploration.
         */
        inline DiEdge& nextChild() {
            edge_list_it_t eit = *mNextEdge;
            mNextEdge++;
            return dynamic_cast<DiEdge&>(*(*eit));
        }

        inline node_list_it_t dfsParent() { return mDfsParent; }

        /** @brief indicates if this node has been visited.
         */
        bool                mVisited;

        /** @brief rank for this node.
         */
        long                mRank;

        /** @brief back link to the element in unvisitedNodes
         */
        list<node_list_it_t>::iterator mDFSbackIt;

        /** @brief holds the pointer to the parent during a DFS exploration.
         */
        node_list_it_t      mDfsParent;

        /** @brief points to the next incident edge to be explored by DFS.
         */
        node_incidence_it_t mNextEdge;

    friend class GKNVcrossingsReducer;
#ifdef UNIT_TESTS
    friend class GKNVcrossingsReducerTests;
#endif
    };

    list<node_list_it_t> unvisitedNodes;
    long unvisitedNodesSize = 0;

    DiGraph mDFS;

    mG.pushDefaultIGForwardLinkToNodes();

    for (long i = 0; i < ranks.size(); i++) {

        for (auto nit : ranks[i]) {

            auto& Norg = dynamic_cast<DiNode&>(*(*nit));

            auto& Ndfs = dynamic_cast<GknvDfsNode&>(
                                mDFS.addNode(std::make_unique<GknvDfsNode>()));
            Ndfs.mRank = i;
            Ndfs.mVisited = false;
            Norg.setIGForwardLink(Ndfs.backIt());
            Ndfs.pushIGBackwardLink(Norg.backIt());

            Ndfs.mDFSbackIt = unvisitedNodes.insert(
                                          unvisitedNodes.end(), Ndfs.backIt());
            unvisitedNodesSize++;
        }
    }

    for (auto eit = mG.edges().first; eit != mG.edges().second; eit++) {

        auto& Eorg = dynamic_cast<DiEdge&>(*(*eit));

        auto& NsrcOrg = dynamic_cast<DiNode&>(Eorg.incidentNodeSrc());

        auto& NdstOrg = dynamic_cast<DiNode&>(Eorg.incidentNodeDst());

        auto& NsrcDfs = dynamic_cast<GknvDfsNode&>(NsrcOrg.IGForwardLinkRef());

        auto& NdstDfs = dynamic_cast<GknvDfsNode&>(NdstOrg.IGForwardLinkRef());

        auto  ep = std::make_unique<DiEdge>();

        auto& Edfs = dynamic_cast<DiEdge&>(
                                mDFS.addEdge(std::move(ep), NsrcDfs, NdstDfs));

        Eorg.pushIGForwardLink(Edfs.backIt());

        Edfs.pushIGBackwardLink(Eorg.backIt());

    }

    for (auto& rank : ranks) {
        if (rank.size() > 0) {
            rank.clear();
        }
    }

    while(unvisitedNodesSize > 0) {

        auto&Rdfs = dynamic_cast<GknvDfsNode&>(
                                              *(*(*(unvisitedNodes.begin()))));
        auto& Rorg = dynamic_cast<DiNode&>(Rdfs.IGBackwardLinkRef());

        ranks[Rdfs.mRank].push_back(Rorg.backIt());
//        cerr << "Pushing " << Rorg.num() << "to rank " << Rdfs.mRank << "\n";
        Rdfs.initForDFS(mDFS.nodes().second);

        unvisitedNodes.erase(Rdfs.mDFSbackIt);

        unvisitedNodesSize--;

        auto nodeBeingVisited = Rdfs.backIt();

        while(nodeBeingVisited != mDFS.nodes().second) {

            auto& Ndfs = dynamic_cast<GknvDfsNode&>(*(*(nodeBeingVisited)));

            if (Ndfs.hasNextChild()) {

                auto& Edfs = Ndfs.nextChild();

                auto& Adfs = dynamic_cast<GknvDfsNode&>(
                                                      Edfs.adjacentNode(Ndfs));

                if (!Adfs.mVisited) {

                    unvisitedNodes.erase(Adfs.mDFSbackIt);

                    unvisitedNodesSize--;

                    Adfs.initForDFS(Ndfs.backIt());

                    nodeBeingVisited = Adfs.backIt();

                    auto& Aorg = dynamic_cast<DiNode&>(
                                                     Adfs.IGBackwardLinkRef());

                    ranks[Adfs.mRank].push_back(Aorg.backIt());
//                    cerr << "Pushing " << Aorg.num() << "to rank "
//                         << Adfs.mRank << "\n";
                }

            }
            else {

                nodeBeingVisited = Ndfs.dfsParent();

            }

        }

    }

    mG.popIGForwardLinkFromNodes();

}


long GKNVcrossingsReducer::calculateNumberOfCrossings(
    vector<node_list_it_t>& leftNodes,
    vector<node_list_it_t>& rightNodes
) {
//cerr << "E 1\n";
    long index = 0;
    for(auto nit : rightNodes) {
//cerr << "E 2.1\n";
        auto& N = dynamic_cast<DiNode&>(*(*nit));
//cerr << "E 2.2\n";
        N.pushUtility(index++);
//cerr << "E 2.3\n";
    }
//cerr << "E 3\n";
    long numCrossings = 0;

    for(long i = 0; i < long(leftNodes.size()) - 1; i++) {
//cerr << "E 4\n";
        auto& Ni = dynamic_cast<DiNode&>(*(*(leftNodes[i])));
//cerr << "E 5\n";
        for(long j = i + 1; j < long(leftNodes.size()) ; j++) {
//cerr << "E 6\n";
            auto& Nj = dynamic_cast<DiNode&>(*(*(leftNodes[j])));
//cerr << "E 7\n";
            for (auto sit = Ni.incidentEdgesOut().first;
                                  sit != Ni.incidentEdgesOut().second; sit++) {
//cerr << "E 8\n";
                auto& Es = dynamic_cast<DiEdge&>(*(*(*sit)));
//cerr << "E 9\n";
                auto& Ns = dynamic_cast<DiNode&>(Es.adjacentNode(Ni));
//cerr << "E 10\n";
                auto  s  = Ns.utility();
//cerr << "E 11\n";
                for (auto tit = Nj.incidentEdgesOut().first;
                                  tit != Nj.incidentEdgesOut().second; tit++) {
//cerr << "E 12\n";
                    auto& Et = dynamic_cast<DiEdge&>(*(*(*tit)));
//cerr << "E 13\n";
                    auto& Nt = dynamic_cast<DiNode&>(Et.adjacentNode(Nj));
//cerr << "E 14\n";
                    auto  t  = Nt.utility();
//cerr << "E 15\n";
                    // Invariant: i < j. If s > t, (i,s) and (j,t) crosses.
                    if (s > t) {
//cerr << "E 16\n";
                        numCrossings++;
//cerr << "E 17\n";
                    }
//cerr << "E 18\n";
                }
//cerr << "E 19\n";
            }
//cerr << "E 20\n";
        }
//cerr << "E 21\n";
    }
//cerr << "E 22\n";
    for(auto nit : rightNodes) {
//cerr << "E 23\n";
        auto& N = dynamic_cast<DiNode&>(*(*nit));
//cerr << "E 24\n";
        N.popUtility();
//cerr << "E 25\n";
    }
//cerr << "E 26\n";
    return numCrossings;
}


void GKNVcrossingsReducer::reorderRightSideByMedianHeuristic(
    vector<node_list_it_t>& leftNodes,
    vector<node_list_it_t>& rightNodes,
    vector<node_list_it_t>& newRightNodes
) {

    vector<vector<node_list_it_t> >buckets;

    long index = 0;
    for(auto nit : leftNodes) {
        auto& N = dynamic_cast<DiNode&>(*(*nit));
        N.pushUtility(index++);
        vector<node_list_it_t> vec;
        buckets.push_back(std::move(vec));
    }
    float leftRightRatio =  float(leftNodes.size()) / float(rightNodes.size());
                                         
    float findex = 0.0;
    for(auto nit : rightNodes) {
        auto& N = dynamic_cast<DiNode&>(*(*nit));
        if (N.degreeIn()==0) {
            buckets[long(findex*leftRightRatio)].push_back(nit);

        }
        else {
            vector<long> incidentNodes;
            for (auto iit = N.incidentEdgesIn().first;
                                    iit != N.incidentEdgesIn().second; iit++) {
                auto& E = dynamic_cast<DiEdge&>(*(*(*iit)));
                auto& A = dynamic_cast<DiNode&>(E.adjacentNode(N));
                incidentNodes.push_back(A.utility());
            }
            std::sort(incidentNodes.begin(), incidentNodes.end());
            long degree = N.degreeIn();
            if (degree % 2 ==1) {
                buckets[incidentNodes[degree/2]].push_back(nit);

            }
            else if (degree==2) {
                buckets[(incidentNodes[0]+incidentNodes[1])/2].push_back(nit);

            }
            else {
                float leftPos   = float(incidentNodes[degree/2]);
                float leftDiff  = leftPos - float(incidentNodes[0]);
                float rightPos  = float(incidentNodes[degree/2+1]);
                float rightDiff = float(incidentNodes[degree-1]) - rightPos;
                float newPos    = (leftPos * leftDiff + rightPos * rightDiff) /
                                                       (leftDiff + rightDiff);
                buckets[size_t(newPos)].push_back(nit);

            }
        }
        findex += 1.0;
    }
    for(auto nit : leftNodes) {
        auto& N = dynamic_cast<DiNode&>(*(*nit));
        N.popUtility();
    }
    newRightNodes.clear();
    for (auto& v : buckets) {
        for (auto nit : v) {
            newRightNodes.push_back(nit);
        }
    }
}


void GKNVcrossingsReducer::reorderLeftSideByMedianHeuristic(
    vector<node_list_it_t>& leftNodes,
    vector<node_list_it_t>& rightNodes,
    vector<node_list_it_t>& newLeftNodes
) {

    vector<vector<node_list_it_t> >buckets;

    long index = 0;
    for(auto nit : rightNodes) {
        auto& N = dynamic_cast<DiNode&>(*(*nit));
        N.pushUtility(index++);
        vector<node_list_it_t> vec;
        buckets.push_back(std::move(vec));
    }
    float rightLeftRatio = float(rightNodes.size()) / float(leftNodes.size());
    float findex = 0.0;
    for(auto nit : leftNodes) {
        auto& N = dynamic_cast<DiNode&>(*(*nit));
        if (N.degreeOut()==0) {
            buckets[long(findex*rightLeftRatio)].push_back(nit);

        }
        else {
            vector<long> incidentNodes;
            for (auto iit = N.incidentEdgesOut().first;
                                   iit != N.incidentEdgesOut().second; iit++) {
                auto& E = dynamic_cast<DiEdge&>(*(*(*iit)));
                auto& A = dynamic_cast<DiNode&>(E.adjacentNode(N));
                incidentNodes.push_back(A.utility());
            }
            std::sort(incidentNodes.begin(), incidentNodes.end());
            long degree = N.degreeOut();
            if (degree % 2 ==1) {
                buckets[incidentNodes[degree/2]].push_back(nit);

            }
            else if (degree==2) {
                buckets[(incidentNodes[0]+incidentNodes[1])/2].push_back(nit);

            }
            else {
                float leftPos   = float(incidentNodes[degree/2]);
                float leftDiff  = leftPos - float(incidentNodes[0]);
                float rightPos  = float(incidentNodes[degree/2+1]);
                float rightDiff = float(incidentNodes[degree-1]) - rightPos;
                float newPos    = (leftPos * leftDiff + rightPos * rightDiff) /
                                                       (leftDiff + rightDiff);
                buckets[size_t(newPos)].push_back(nit);
            }
        }
        findex += 1.0;
    }
    for(auto nit : rightNodes) {
        auto& N = dynamic_cast<DiNode&>(*(*nit));
        N.popUtility();
    }
    newLeftNodes.clear();
    for (auto& v : buckets) {
        for (auto nit : v) {
            newLeftNodes.push_back(nit);
        }
    }
}


long GKNVcrossingsReducer::reduce(
    vector<vector<node_list_it_t> >& ranks,
    long numIterations
) {
    long stagnant = 0;
//cerr << "C 1\n";
    initialOrderByDFS(ranks);
//cerr << "C 2.1\n";
    auto numCrossings = calculateNumberOfCrossings(ranks);
//cerr << "C 2.2\n";
//    cerr << "Initial: " << numCrossings << "\n";
    if (numCrossings == 0) {
//cerr << "C 2.3\n";
        rearrangeIncidenceLists(ranks);
//cerr << "C 2.4\n";
        return 0;
    }
//cerr << "C 3\n";
    for (long i = 0; i < numIterations; i++) {
//        cerr << "Iteration: " << i << "\n";
        vector<vector<node_list_it_t> > updatedRanks;

        applyMedianHeuristic(ranks, updatedRanks, i%2==0);

        applyTransposeHeuristic(updatedRanks);

        auto updatedNumCrossings = calculateNumberOfCrossings(updatedRanks);
//        cerr << "Crossings: " << updatedNumCrossings << "\n";
        if (updatedNumCrossings < numCrossings) {
            ranks = std::move(updatedRanks);
            numCrossings = updatedNumCrossings;
            if (numCrossings == 0) {
                break;
            }
            stagnant = 0;
        }
        else {
            stagnant++;
            if (stagnant >= 4) {
//                cerr << "Stagnant. Giving up\n";
                break;
            }
        }
    }
//cerr << "C 4\n";
    rearrangeIncidenceLists(ranks);
//cerr << "C 5\n";
    return numCrossings;
}


void GKNVcrossingsReducer::rearrangeIncidenceLists(
    vector<vector<node_list_it_t> >& ranks
) {
    mG.pushDefaultUtilityToNodes();

    for (auto& rank : ranks) {
        long index = 0;
        for (auto nit : rank) {
            auto& N = dynamic_cast<DiNode&>(*(*nit));
            N.setUtility(index++);
        }
    }

    for (long i = 0; i < long(ranks.size()) - 1; i++) {
        auto& leftRank  = ranks[i];

        for (auto nit : leftRank) {
            auto&N = dynamic_cast<DiNode&>(*(*nit));
            vector<edge_list_it_t> incidence(N.incidentEdgesOut().first,
                                             N.incidentEdgesOut().second );

            std::sort(incidence.begin(), incidence.end(), nodeCompDst);
            list<edge_list_it_t> incidenceList(incidence.begin(),
                                               incidence.end()   );
            N.reorderIncidenceOut(std::move(incidenceList));
        }
    }

    for (long i = 1; i < long(ranks.size()); i++) {
        auto& rightRank  = ranks[i];

        for (auto nit : rightRank) {
            auto&N = dynamic_cast<DiNode&>(*(*nit));
            vector<edge_list_it_t> incidence(N.incidentEdgesIn().first,
                                             N.incidentEdgesIn().second );

            std::sort(incidence.begin(), incidence.end(), nodeCompSrc);
            list<edge_list_it_t> incidenceList(incidence.begin(),
                                               incidence.end()   );
            N.reorderIncidenceIn(std::move(incidenceList));
        }
    }

    mG.popUtilityFromNodes();
}


void GKNVcrossingsReducer::applyMedianHeuristic(
    vector<vector<node_list_it_t> >& oldRanks,
    vector<vector<node_list_it_t> >& newRanks,
    bool forward
) {
    newRanks.clear();
    if (oldRanks.size()==0) {
        return;
    }
    long lastIndex = long(oldRanks.size()) - 1;

    for (long i = 0; i <= lastIndex; i++) {
        vector<node_list_it_t> emptyRank;
        newRanks.push_back(std::move(emptyRank));
    }

    if(forward) {
        newRanks[0].insert(
                    newRanks[0].end(), oldRanks[0].begin(), oldRanks[0].end());
    }
    else {

        newRanks[lastIndex].insert(
                              newRanks[lastIndex].end(),
                              oldRanks[lastIndex].begin(),
                              oldRanks[lastIndex].end()
                           );
    }

    if (forward) {
        for (long j = 0; j < lastIndex; j++) {
            reorderRightSideByMedianHeuristic(
                                    oldRanks[j], oldRanks[j+1], newRanks[j+1]);
        }
    }
    else {
        for (long j = lastIndex -1 ; j >= 0; j--) {
            reorderLeftSideByMedianHeuristic(
                                      oldRanks[j], oldRanks[j+1], newRanks[j]);
        }
    }
}


void GKNVcrossingsReducer::applyTransposeHeuristic(
    vector<vector<node_list_it_t> >& ranks
) {
    for (long i = 0; i < long(ranks.size()); i++) {
        if (i == 0) {
            tryTransposeLeft(ranks[0], ranks[1]);
        }
        else if (i == long(ranks.size()) - 1) {
            tryTransposeRight(ranks[i-1], ranks[i]);
        }
        else {
            tryTransposeMid(ranks[i-1], ranks[i], ranks[i+1]);
        }
    }
}


void GKNVcrossingsReducer::tryTransposeLeft(
    vector<node_list_it_t>& leftRank,
    vector<node_list_it_t>& rightRank
) {

    for (long i = 0; i < long(leftRank.size()) - 1; i++) {
        auto numOrg = calculateNumberOfCrossings(leftRank, rightRank);

        // Swap i and i + 1 and see how the crossings changes
        std::swap(leftRank[i], leftRank[i+1]);

        auto numSwapped = calculateNumberOfCrossings(leftRank, rightRank);

        if (numSwapped >= numOrg) {
            // Swap back to the original
            std::swap(leftRank[i], leftRank[i+1]);
        }
    }
}


void GKNVcrossingsReducer::tryTransposeMid(
    vector<node_list_it_t>& leftRank,
    vector<node_list_it_t>& midRank,
    vector<node_list_it_t>& rightRank
) {

    for (long i = 0; i < long(midRank.size()) - 1; i++) {
        auto numOrg = calculateNumberOfCrossings(leftRank, midRank);
        numOrg += calculateNumberOfCrossings(midRank, rightRank);

        // Swap i and i + 1 and see how the crossings changes
        std::swap(midRank[i], midRank[i+1]);

        auto numSwapped = calculateNumberOfCrossings(leftRank, midRank);
        numSwapped += calculateNumberOfCrossings(midRank, rightRank);

        if (numSwapped >= numOrg) {
            // Swap back to the original
            std::swap(midRank[i], midRank[i+1]);
        }
    }

}


void GKNVcrossingsReducer::tryTransposeRight(
    vector<node_list_it_t>& leftRank,
    vector<node_list_it_t>& rightRank
) {

    for (long i = 0; i < long(rightRank.size()) - 1; i++) {
        auto numOrg = calculateNumberOfCrossings(leftRank, rightRank);

        // Swap i and i + 1 and see how the crossings changes
        std::swap(rightRank[i], rightRank[i+1]);

        auto numSwapped = calculateNumberOfCrossings(leftRank, rightRank);

        if (numSwapped >= numOrg) {
            // Swap back to the original
            std::swap(rightRank[i], rightRank[i+1]);
        }
    }
}


long GKNVcrossingsReducer::calculateNumberOfCrossings(
    vector<vector<node_list_it_t> >& ranks
) {
//cerr << "D 1\n";
    long numCrossings = 0;
//cerr << "D 2\n";
    for (long i = 0; i < long(ranks.size()) - 1; i++) {
//cerr << "D 3\n";
        auto& leftRank  = ranks[i];
//cerr << "D 4\n";
        auto& rightRank = ranks[i+1];
//cerr << "D 5\n";
        numCrossings += calculateNumberOfCrossings(leftRank, rightRank);
//cerr << "D 6\n";
    }
//cerr << "D 7\n";
    return numCrossings;
}


bool GKNVcrossingsReducer::nodeCompDst(edge_list_it_t it1, edge_list_it_t it2)
{
    auto& E1 = dynamic_cast<DiEdge&>(*(*it1));
    auto& E2 = dynamic_cast<DiEdge&>(*(*it2));
    auto& N1 = dynamic_cast<DiNode&>(E1.incidentNodeDst());
    auto& N2 = dynamic_cast<DiNode&>(E2.incidentNodeDst());
    return N1.utility() > N2.utility();
}


bool GKNVcrossingsReducer::nodeCompSrc(edge_list_it_t it1, edge_list_it_t it2)
{
    auto& E1 = dynamic_cast<DiEdge&>(*(*it1));
    auto& E2 = dynamic_cast<DiEdge&>(*(*it2));
    auto& N1 = dynamic_cast<DiNode&>(E1.incidentNodeSrc());
    auto& N2 = dynamic_cast<DiNode&>(E2.incidentNodeSrc());
    return N1.utility() < N2.utility();
}


}// namespace Directed

}// namespace Wailea
