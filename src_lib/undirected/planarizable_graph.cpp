#include "undirected/planarizable_graph.hpp"

/**
 * @file undirected/planarizable_graph.cpp
 *
 * @brief this is a graph data structure used to insert edge one by one
 *        to a planar base graph.
 *        The insertion is done by splitting some edges with 
 *        virtual nodes, and connecting those nodes.
 *        Such an ordered list of edges can be found by GMWEdgeInserter.
 */

namespace Wailea {

namespace Undirected {

using namespace std;


void PlanarizableGraph::createFrom(
    Graph&                  g,
    vector<node_list_it_t>& Nlist,
    vector<edge_list_it_t>& Elist
)
{
    if (numNodes() > 0) {
        throw std::invalid_argument(Constants::kExceptionNotEmpty);
    }

    vector<node_list_it_t> EInodes;

    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    for (auto nit : Nlist) {

        auto np = make_unique<PlanarizableNode>();
	np->pushIGBackwardLink(nit);
        np->mVirtual = false;
        nodePairs.push_back(make_pair(nit,std::move(np)));

    }

    for (auto eit : Elist) {

        auto ep = make_unique<PlanarizableEdge>();
	ep->pushIGBackwardLink(eit);
        edgePairs.push_back(make_pair(eit,std::move(ep)));

    }

    g.copySubgraph(nodePairs, edgePairs, *this);

    for (auto nit = nodes().first; nit != nodes().second; nit++) {
        auto& PN = dynamic_cast<PlanarizableNode&>(*(*nit));
        PN.IGBackwardLinkRef().pushIGForwardLink(nit);
    }

    for (auto eit = edges().first; eit != edges().second; eit++) {
        auto& PE = dynamic_cast<PlanarizableEdge&>(*(*eit));
        PE.IGBackwardLinkRef().pushIGForwardLink(eit);
    }
}


vector<edge_list_it_t> PlanarizableGraph::findConsecutiveChains(
    PlanarizableNode& N
) {
    vector<edge_list_it_t> edgePairs;

    auto prevIit = N.incidentEdges().second;
    prevIit--;// last element.
    for (auto iit = N.incidentEdges().first; 
                                      iit != N.incidentEdges().second; iit++) {
        auto&Eprev = dynamic_cast<PlanarizableEdge&>(*(*(*prevIit)));
        auto&Ecur  = dynamic_cast<PlanarizableEdge&>(*(*(*iit)));
        if (!Ecur.isTerminalN1() && Ecur.chainN1()==Eprev.backIt()) {
            edgePairs.push_back(*prevIit);
            edgePairs.push_back(*iit);
        }
        if (!Ecur.isTerminalN2() && Ecur.chainN2()==Eprev.backIt()) {
            edgePairs.push_back(*prevIit);
            edgePairs.push_back(*iit);
        }
        prevIit = iit;
    }
    return edgePairs;
}


std::tuple<edge_ptr_t, edge_ptr_t, edge_list_it_t> 
PlanarizableGraph::mergeTwoEdges(
    PlanarizableNode& N,
    PlanarizableEdge& E1,
    PlanarizableEdge& E2
) {

    Edge& Eorg = E1.IGBackwardLinkRef();

    auto& N1   = dynamic_cast<PlanarizableNode&>(E1.adjacentNode(N));

    auto& N2   = dynamic_cast<PlanarizableNode&>(E2.adjacentNode(N));

    auto  pos1 = (E1.incidentNode1().backIt()==N1.backIt())?
                  E1.incidentBackItNode1():E1.incidentBackItNode2();

    auto  pos2 = (E2.incidentNode1().backIt()==N2.backIt())?
                  E2.incidentBackItNode1():E2.incidentBackItNode2();

    auto  E1oppIt = edges().second;
    auto  E2oppIt = edges().second;

    bool shouldUpdateForwardLinkOfEorg = false;
    bool shouldUpdateN1OfE1opp         = false;
    bool shouldUpdateN2OfE1opp         = false;
    bool shouldUpdateN1OfE2opp         = false;
    bool shouldUpdateN2OfE2opp         = false;

    if (Eorg.IGForwardLink()==E1.backIt()||Eorg.IGForwardLink()==E2.backIt()) {
        shouldUpdateForwardLinkOfEorg = true;
    }

    if (E1.incidentNode1().backIt()==N1.backIt() && !E1.isTerminalN1()) {
        E1oppIt = E1.chainN1();
        auto& E1opp = dynamic_cast<PlanarizableEdge&>(*(*E1oppIt));
        if (!E1opp.isTerminalN1() && E1opp.chainN1()==E1.backIt()) {
            shouldUpdateN1OfE1opp = true;
        }
        if (!E1opp.isTerminalN2() && E1opp.chainN2()==E1.backIt()) {
            shouldUpdateN2OfE1opp = true;
        }
    }  
    else if (E1.incidentNode2().backIt()==N1.backIt() && !E1.isTerminalN2()) {
        E1oppIt = E1.chainN2();
        auto& E1opp = dynamic_cast<PlanarizableEdge&>(*(*E1oppIt));
        if (!E1opp.isTerminalN1() && E1opp.chainN1()==E1.backIt()) {
            shouldUpdateN1OfE1opp = true;
        }
        if (!E1opp.isTerminalN2() && E1opp.chainN2()==E1.backIt()) {
            shouldUpdateN2OfE1opp = true;
        }
    }

    if (E2.incidentNode1().backIt()==N2.backIt() && !E2.isTerminalN1()) {
        E2oppIt = E2.chainN1();
        auto& E2opp = dynamic_cast<PlanarizableEdge&>(*(*E2oppIt));
        if (!E2opp.isTerminalN1() && E2opp.chainN1()==E2.backIt()) {
            shouldUpdateN1OfE2opp = true;
        }
        if (!E2opp.isTerminalN2() && E2opp.chainN2()==E2.backIt()) {
            shouldUpdateN2OfE2opp = true;
        }
    }  
    else if (E2.incidentNode2().backIt()==N2.backIt() && !E2.isTerminalN2()) {
        E2oppIt = E2.chainN2();
        auto& E2opp = dynamic_cast<PlanarizableEdge&>(*(*E2oppIt));
        if (!E2opp.isTerminalN1() && E2opp.chainN1()==E2.backIt()) {
            shouldUpdateN1OfE2opp = true;
        }
        if (!E2opp.isTerminalN2() && E2opp.chainN2()==E2.backIt()) {
            shouldUpdateN2OfE2opp = true;
        }
    }

    auto  ep   = make_unique<PlanarizableEdge>();
    auto& Enew = dynamic_cast<PlanarizableEdge&>(
                                  addEdge(std::move(ep), N1, N2, pos1, pos2));

    auto epE1 = removeEdge(E1);
    auto epE2 = removeEdge(E2);

    Enew.pushIGBackwardLink(Eorg.backIt());

    if (shouldUpdateForwardLinkOfEorg) {
        Eorg.setIGForwardLink(Enew.backIt());
    }

    if (shouldUpdateN1OfE1opp||shouldUpdateN2OfE1opp) {
        auto& E1opp = dynamic_cast<PlanarizableEdge&>(*(*E1oppIt));
        Enew.setChainN1(E1opp.backIt());
        if (shouldUpdateN1OfE1opp) {
            E1opp.setChainN1(Enew.backIt());
        }
        else {
            E1opp.setChainN2(Enew.backIt());
        }
    }

    if (shouldUpdateN1OfE2opp||shouldUpdateN2OfE2opp) {
        auto& E2opp = dynamic_cast<PlanarizableEdge&>(*(*E2oppIt));
        Enew.setChainN2(E2opp.backIt());
        if (shouldUpdateN1OfE2opp) {
            E2opp.setChainN1(Enew.backIt());
        }
        else {
            E2opp.setChainN2(Enew.backIt());
        }
    }
    return {std::move(epE1), std::move(epE2), Enew.backIt()};
}
                
vector<std::tuple<edge_ptr_t, edge_ptr_t, edge_list_it_t>>
PlanarizableGraph::removeUnnecessaryVirtualNodes()
{
    vector<std::tuple<edge_ptr_t, edge_ptr_t, edge_list_it_t>> edgeTuples;

    vector<node_list_it_t> nodesToBeRemoved;

    for (auto nit = nodes().first; nit != nodes().second; nit++) {
        auto& N = dynamic_cast<PlanarizableNode&>(*(*nit));
        if (N.isVirtual()) {
            vector<edge_list_it_t> edgePairs = findConsecutiveChains(N);
            for (size_t index = 0; index < edgePairs.size(); index +=2) {
                auto& E1 = dynamic_cast<PlanarizableEdge&>(
                                                       *(*(edgePairs[index])));
                auto& E2 = dynamic_cast<PlanarizableEdge&>(
                                                     *(*(edgePairs[index+1])));
                std::tuple<edge_ptr_t, edge_ptr_t, edge_list_it_t> tpl = 
                                                      mergeTwoEdges(N, E1, E2);
                edgeTuples.push_back(std::move(tpl));
            }
        }
        if (N.degree()==0) {
            nodesToBeRemoved.push_back(N.backIt());        
        }
    }

    for (auto nit : nodesToBeRemoved) {
        auto& N = dynamic_cast<PlanarizableNode&>(*(*nit));
        removeNode(N);
    }
    return edgeTuples;
}



/** @brief split the edges along the insertion path edges.
 *
 *                     | Eopp1              | Eopp1
 *                     |                    |
 *                     N1 of E              N1 of E   
 *                     |     IGBackwardLink |
 *     | <--------------------------------- |
 *     | ---------------------------------> | Enew1
 *     |               |                    |
 *     | (new) IGForwardLink                |
 *     | <------------ |                    |
 * Eorg|IGBackwardLink |                    |
 *     | ------------> | E      =====>      Nnew
 *     |IGForwardLink  |                    |
 *     |               |     IGBackwardLink | Enew2
 *     | <--------------------------------- |
 *     | ---------------------------------> |
 *     | (new) IGForwardLink                |
 *     |               |                    |
 *                     N2 of E              N2 of E
 *                     |                    |
 *                     | Eopp2              | Eopp2
 *
 *
 *  @param insertionPath  (in): the edges along the insertion path
 *
 *  @return the new virtual nodes created along the insertion path
 */
void PlanarizableGraph::splitInsertionPath (
    list<edge_list_it_t>&   insertionPath,
    vector<edge_ptr_t>&     removedEdges,
    vector<node_list_it_t>& newNodes
) {
    for (auto eit : insertionPath) {

        auto& E  = dynamic_cast<PlanarizableEdge&>(*(*eit));
        auto& N1 = dynamic_cast<PlanarizableNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<PlanarizableNode&>(E.incidentNode2());

        // Check the chain and link structures and store them to flags.
        bool shouldUpdateChainN1OfEopp1        = false;
        bool shouldUpdateChainN2OfEopp1        = false;
        bool shouldUpdateChainN1OfEopp2        = false;
        bool shouldUpdateChainN2OfEopp2        = false;
        bool shouldUpdateForwardLinkOfEorgToN1 = false;
        bool shouldUpdateForwardLinkOfEorgToN2 = false;

        if (!E.mTerminalN1) {

            auto& Eopp1 = dynamic_cast<PlanarizableEdge&>(*(*(E.mChainN1)));

            if (!Eopp1.mTerminalN1 && Eopp1.mChainN1 == E.backIt()) {

                shouldUpdateChainN1OfEopp1 = true;
            }
            else {

                shouldUpdateChainN2OfEopp1 = true;
            }
        }

        if (!E.mTerminalN2) {
            auto& Eopp2 = dynamic_cast<PlanarizableEdge&>(*(*(E.mChainN2)));
            if (!Eopp2.mTerminalN1 && Eopp2.mChainN1 == E.backIt()) {
                shouldUpdateChainN1OfEopp2 = true;
            }
            else {
                shouldUpdateChainN2OfEopp2 = true;
            }
        }

        auto& Eorg = E.IGBackwardLinkRef();
        if (Eorg.IGForwardLink()==E.backIt()) {
            if (!N1.mVirtual) {
                shouldUpdateForwardLinkOfEorgToN1 = true;
            }
            else {
                shouldUpdateForwardLinkOfEorgToN2 = true;
            }
        }

        // Split the edge with a new virtual node and two new edges.
        auto  np   = make_unique<PlanarizableNode>();
        auto  ep1  = make_unique<PlanarizableEdge>();
        auto  ep2  = make_unique<PlanarizableEdge>();

        edge_ptr_t removedE;
        auto newTuple = splitEdge(
               E, removedE, move(ep1), move(ep2), move(np), nodes().second);

        auto& Nnew  = dynamic_cast<PlanarizableNode&>(get<0>(newTuple));
        auto& Enew1 = dynamic_cast<PlanarizableEdge&>(get<1>(newTuple));
        auto& Enew2 = dynamic_cast<PlanarizableEdge&>(get<2>(newTuple));

        Nnew.mVirtual = true;
        Nnew.pushIGBackwardLink(nodes().second);

        // Update the links and chains on the edges N1 side.

        Enew1.pushIGBackwardLink(Eorg.backIt());

        if (shouldUpdateForwardLinkOfEorgToN1) {
            Eorg.setIGForwardLink(Enew1.backIt());
        }

        if (Enew1.incidentNode1().backIt()==N1.backIt()) {
            Enew1.mChainN1    = E.mChainN1;
            Enew1.mTerminalN1 = E.mTerminalN1;
            Enew1.mChainN2    = Enew2.backIt();
            Enew1.mTerminalN2 = false;
        }
        else {
            Enew1.mChainN2    = E.mChainN1;
            Enew1.mTerminalN2 = E.mTerminalN1;
            Enew1.mChainN1    = Enew2.backIt();
            Enew1.mTerminalN1 = false;
        }

        if (shouldUpdateChainN1OfEopp1) {
            auto& Eopp1 = dynamic_cast<PlanarizableEdge&>(*(*(E.mChainN1)));
            Eopp1.mChainN1     = Enew1.backIt();
            Eopp1.mTerminalN1 = false;
        }

        if (shouldUpdateChainN2OfEopp1) {
            auto& Eopp1 = dynamic_cast<PlanarizableEdge&>(*(*(E.mChainN1)));
            Eopp1.mChainN2     = Enew1.backIt();
            Eopp1.mTerminalN2 = false;
        }

        // Update the links and chains on the edges N2 side.

        Enew2.pushIGBackwardLink(Eorg.backIt());

        if (shouldUpdateForwardLinkOfEorgToN2) {
            Eorg.setIGForwardLink(Enew2.backIt());
        }

        if (Enew2.incidentNode2().backIt()==N2.backIt()) {
            Enew2.mChainN1    = Enew1.backIt();
            Enew2.mTerminalN1 = false;
            Enew2.mChainN2    = E.mChainN2;
            Enew2.mTerminalN2 = E.mTerminalN2;
        }
        else {
            Enew2.mChainN2    = Enew1.backIt();
            Enew2.mTerminalN2 = false;
            Enew2.mChainN1    = E.mChainN2;
            Enew2.mTerminalN1 = E.mTerminalN2;
        }

        if (shouldUpdateChainN1OfEopp2) {
            auto& Eopp2 = dynamic_cast<PlanarizableEdge&>(*(*(E.mChainN2)));
            Eopp2.mChainN1     = Enew2.backIt();
            Eopp2.mTerminalN1 = false;
        }

        if (shouldUpdateChainN2OfEopp2) {
            auto& Eopp2 = dynamic_cast<PlanarizableEdge&>(*(*(E.mChainN2)));
            Eopp2.mChainN2     = Enew2.backIt();
            Eopp2.mTerminalN2 = false;
        }

        newNodes.push_back(Nnew.backIt());

        removedEdges.push_back(std::move(removedE));

    }

}


vector<edge_ptr_t> PlanarizableGraph::insertEdge (
    list<edge_list_it_t>& insertionPath,
    PlanarizableNode&     N1,
    PlanarizableNode&     N2,
    edge_list_it_t        originalEdge
) {
    vector<edge_ptr_t>     removedEdges;
    list<edge_list_it_t>   newEdges;
    if (insertionPath.size() == 0) {
        // The edge can be inserted without breaking planarity.
        auto  ep   = make_unique<PlanarizableEdge>();
        auto& Enew = dynamic_cast<PlanarizableEdge&>(addEdge(move(ep),N1,N2));
        Enew.mChainN1    = edges().second;
        Enew.mTerminalN1 = true;
        Enew.mChainN2    = edges().second;
        Enew.mTerminalN2 = true;
        newEdges.push_back(Enew.backIt());
        Enew.pushIGBackwardLink(originalEdge);
        if (originalEdge != edges().second) {
            (*originalEdge)->setIGForwardLink(Enew.backIt());
        }

        insertionPath = std::move(newEdges);

        return removedEdges;
    }
    // Split the edges on the insertionPath
    vector<node_list_it_t> newNodes;

    splitInsertionPath(insertionPath, removedEdges, newNodes);;
    edge_list_it_t         prevIt;
    for (size_t i = 0; i <= newNodes.size() ; i++) {
        if (i == 0) {
            auto& Ni_2 = dynamic_cast<PlanarizableNode&>(*(*(newNodes[i])));
            auto  ep   = make_unique<PlanarizableEdge>();
            auto& Enew = dynamic_cast<PlanarizableEdge&>(
                                                  addEdge(move(ep), N1, Ni_2));
            Enew.mChainN1    = edges().second;
            Enew.mTerminalN1 = true;

            newEdges.push_back(Enew.backIt());
            Enew.pushIGBackwardLink(originalEdge);
            if (originalEdge != edges().second) {
                (*originalEdge)->setIGForwardLink(Enew.backIt());
            }
            prevIt = Enew.backIt();
        }
        else if (i < newNodes.size()) {
            auto& Ni_1  = dynamic_cast<PlanarizableNode&>(*(*(newNodes[i-1])));
            auto& Ni_2  = dynamic_cast<PlanarizableNode&>(*(*(newNodes[i])));
            auto& Eprev = dynamic_cast<PlanarizableEdge&>(*(*prevIt));
            auto  ep    = make_unique<PlanarizableEdge>();
            auto& Enew  = dynamic_cast<PlanarizableEdge&>(
                                                addEdge(move(ep), Ni_1, Ni_2));
            Eprev.mChainN2    = Enew.backIt();
            Eprev.mTerminalN2 = false;
            Enew.mChainN1     = prevIt;
            Enew.mTerminalN1  = false;

            Enew.pushIGBackwardLink(originalEdge);
            newEdges.push_back(Enew.backIt());
            prevIt = Enew.backIt();
        }
        else /*if (i == newNodes.size())*/{
            auto& Ni_1  = dynamic_cast<PlanarizableNode&>(*(*(newNodes[i-1])));
            auto& Eprev = dynamic_cast<PlanarizableEdge&>(*(*prevIt));
            auto  ep    = make_unique<PlanarizableEdge>();
            auto& Enew  = dynamic_cast<PlanarizableEdge&>(
                                                  addEdge(move(ep), Ni_1, N2));
            Eprev.mChainN2    = Enew.backIt();
            Eprev.mTerminalN2 = false;
            Enew.mChainN1     = prevIt;
            Enew.mTerminalN1  = false;
            Enew.mChainN2     = edges().second;
            Enew.mTerminalN2  = true;

            Enew.pushIGBackwardLink(originalEdge);
            newEdges.push_back(Enew.backIt());
        }
    }
    insertionPath = std::move(newEdges);
    return removedEdges;

}


}// namespace Undirected

}// namespace Wailea
