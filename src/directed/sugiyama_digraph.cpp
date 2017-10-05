#include "directed/sugiyama_digraph.hpp"
/**
 * @file directed/sugiyama_digraph.cpp
 */

namespace Wailea {

namespace Directed {

using namespace std;


void SugiyamaDiGraph::generateFromAcyclicOrdering(
    vector<node_list_it_t>& ao
) {

    long index = 0;

    for(auto nit : ao) {

        auto& Norg = dynamic_cast<DiNode&>(*(*nit));
        auto np    = make_unique<SugiyamaDiNode>();
        auto& Nsug = dynamic_cast<SugiyamaDiNode&>(addNode(std::move(np)));
        // Temporarily assign the acyclic number to rank.
        Nsug.mRank = index;

        Norg.pushIGForwardLink(Nsug.backIt());
        Nsug.pushIGBackwardLink(Norg.backIt());
        index++;

    }

    for (auto eit = mG.edges().first; eit != mG.edges().second; eit++) {

        auto& Eorg    = dynamic_cast<DiEdge&>(*(*(eit)));
        auto& NorgSrc = dynamic_cast<DiNode&>(Eorg.incidentNodeSrc());
        auto& NorgDst = dynamic_cast<DiNode&>(Eorg.incidentNodeDst());
        auto& NsugSrc = dynamic_cast<SugiyamaDiNode&>(
                                                   NorgSrc.IGForwardLinkRef());
        auto& NsugDst = dynamic_cast<SugiyamaDiNode&>(
                                                   NorgDst.IGForwardLinkRef());
        auto  ep = make_unique<SugiyamaDiEdge>();

        bool flipped = NsugSrc.mRank > NsugDst.mRank;
        auto& Esug = dynamic_cast<SugiyamaDiEdge&>(addEdge(std::move(ep), 
                             flipped?NsugDst:NsugSrc,flipped?NsugSrc:NsugDst));
        Esug.mFlipped = flipped;

        Eorg.pushIGForwardLink(Esug.backIt());
        Esug.pushIGBackwardLink(Eorg.backIt());

    }
}


vector<vector<node_list_it_t>>
SugiyamaDiGraph::rankNodesAndAddVirtualNodes(
    vector<vector<node_list_it_t> >& ranks
) {
//cerr << "rankNodesAndAddVirtualNodes() BEGIN\n";
    long index = 0;
    for (auto& rank : ranks) {    
        for (auto nit : rank) {
            auto& N = dynamic_cast<SugiyamaDiNode&>(*(*nit));
            N.mRank = index;
        }
        index++;
    }
//cerr << "rankNodesAndAddVirtualNodes() 1\n";
    list<edge_list_it_t> edgesSplit;
    for (auto eit = edges().first; eit != edges().second; eit++) {
        edgesSplit.push_back(eit);
    }
//cerr << "rankNodesAndAddVirtualNodes() 2\n";
    for (auto eit : edgesSplit) {

        auto& E = dynamic_cast<SugiyamaDiEdge&>(*(*eit));
        auto& Nsrc = dynamic_cast<SugiyamaDiNode&>(E.incidentNodeSrc());
        auto& Ndst = dynamic_cast<SugiyamaDiNode&>(E.incidentNodeDst());
        long length = Ndst.mRank - Nsrc.mRank;
                                    
        if (length > 1) {
            auto& Eorg = dynamic_cast<DiEdge&>(E.IGBackwardLinkRef());

            long rank   = Nsrc.mRank + 1;

            node_list_it_t prevNit = Nsrc.backIt();
            edge_list_it_t prevEit;

            for (long i = 0; i < length -1 ; i++) {

                auto& Vsrc = dynamic_cast<SugiyamaDiNode&>(*(*prevNit));
                auto  np   = make_unique<SugiyamaDiNode>();
                auto& Vdst = dynamic_cast<SugiyamaDiNode&>(
                                                       addNode(std::move(np)));
                Vdst.mRank    = rank;
                Vdst.mVirtual = true;
                Vdst.pushIGBackwardLink(mG.nodes().second);

                auto  ep   = make_unique<SugiyamaDiEdge>();
                auto& Enew = dynamic_cast<SugiyamaDiEdge&>(addEdge(
                                                   std::move(ep), Vsrc, Vdst));
                Enew.pushIGBackwardLink(Eorg.backIt());
                if (i == 0 && !(E.mFlipped)) {
                    Eorg.setIGForwardLink(Enew.backIt());
                }
                Enew.mFlipped = E.mFlipped;
                if (i!=0) {
                    auto& Eprev = dynamic_cast<SugiyamaDiEdge&>(*(*prevEit));
                                                                  
                    Eprev.setChainNdst(Enew.backIt());
                    Enew.setChainNsrc(prevEit);
                }

                prevNit = Vdst.backIt();
                prevEit = Enew.backIt();
                rank++;
            }

            auto& Vsrc = dynamic_cast<SugiyamaDiNode&>(*(*prevNit));
            auto  ep   = make_unique<SugiyamaDiEdge>();
            auto& Enew = dynamic_cast<SugiyamaDiEdge&>(addEdge(
                                                   std::move(ep), Vsrc, Ndst));
            Enew.pushIGBackwardLink(Eorg.backIt());
            if (E.mFlipped) {
                Eorg.setIGForwardLink(Enew.backIt());
            }
            Enew.mFlipped = E.mFlipped;
            auto& Eprev = dynamic_cast<SugiyamaDiEdge&>(*(*prevEit));
            Eprev.setChainNdst(Enew.backIt());
            Enew.setChainNsrc(prevEit);

            // Remove the original edge
            removeEdge(E);
        }
    }
//cerr << "rankNodesAndAddVirtualNodes() 3\n";
    vector<vector<node_list_it_t> > newRanks;
    for (long i = 0; i < ranks.size(); i++) {
//cerr << "rankNodesAndAddVirtualNodes() 3.1\n";
        vector<node_list_it_t> vec;
        newRanks.push_back(std::move(vec));
    }
//cerr << "rankNodesAndAddVirtualNodes() 4\n";
    for (auto nit = nodes().first; nit != nodes().second; nit++) {
//cerr << "rankNodesAndAddVirtualNodes() 4.1\n";
        auto& N = dynamic_cast<SugiyamaDiNode&>(*(*nit));
//cerr << "rankNodesAndAddVirtualNodes() 4.2\n";
        newRanks[N.mRank].push_back(nit);
//cerr << "rankNodesAndAddVirtualNodes() 4.3\n";
    }
//cerr << "rankNodesAndAddVirtualNodes() END\n";
    return newRanks;
}


void SugiyamaDiGraph::setPositionsForEachRank(
    vector<vector<node_list_it_t> >& ranks
) {

    for (auto& rank : ranks) {
        long index = 0;
        for (auto nit : rank) {
            auto& N = dynamic_cast<SugiyamaDiNode&>(*(*nit));
            N.mPos = index++;
        }
    }

    for (auto nit = nodes().first; nit != nodes().second; nit++) {

        auto& N = dynamic_cast<SugiyamaDiNode&>(*(*nit));

        vector<edge_list_it_t> incidenceIn(N.incidentEdgesIn().first,
                                           N.incidentEdgesIn().second);
        std::sort(incidenceIn.begin(), incidenceIn.end(), nodeCompSrc);
        list<edge_list_it_t> listIncidenceIn(
                                       incidenceIn.begin(), incidenceIn.end());
        N.reorderIncidenceIn(std::move(listIncidenceIn));
        vector<edge_list_it_t> incidenceOut(N.incidentEdgesOut().first,
                                            N.incidentEdgesOut().second);
        std::sort(incidenceOut.begin(), incidenceOut.end(), nodeCompDst);
        list<edge_list_it_t> listIncidenceOut(
                                     incidenceOut.begin(), incidenceOut.end());
        N.reorderIncidenceOut(std::move(listIncidenceOut));

    }
}


bool SugiyamaDiGraph::nodeCompDst(edge_list_it_t it1, edge_list_it_t it2)
{
    auto& E1 = dynamic_cast<SugiyamaDiEdge&>(*(*it1));
    auto& E2 = dynamic_cast<SugiyamaDiEdge&>(*(*it2));
    auto& N1 = dynamic_cast<SugiyamaDiNode&>(E1.incidentNodeDst());
    auto& N2 = dynamic_cast<SugiyamaDiNode&>(E2.incidentNodeDst());
    return N1.mPos > N2.mPos;
}


bool SugiyamaDiGraph::nodeCompSrc(edge_list_it_t it1, edge_list_it_t it2)
{
    auto& E1 = dynamic_cast<SugiyamaDiEdge&>(*(*it1));
    auto& E2 = dynamic_cast<SugiyamaDiEdge&>(*(*it2));
    auto& N1 = dynamic_cast<SugiyamaDiNode&>(E1.incidentNodeSrc());
    auto& N2 = dynamic_cast<SugiyamaDiNode&>(E2.incidentNodeSrc());
    return N1.mPos < N2.mPos;
}

}// namespace Directed

}// namespace Wailea
