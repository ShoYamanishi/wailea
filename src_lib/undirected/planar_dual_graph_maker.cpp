#include "undirected/planar_dual_graph_maker.hpp"
#include <map>
#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file undirected/planar_dual_graph_maker.cpp
 *
 * @brief
 */
namespace Wailea {

namespace Undirected {

using namespace std;


void PlanarDualGraphMaker::makeDualGraph(
    Graph&         src,
    EmbeddedGraph& emb,
    DualGraph&     dual
) {

    // Step 1. Make an EmbeddedGraph as a copy of the input graph.
    copyInputGraph(src, emb);

    // Step 2. Connect half edges to form face cycles.
    findFaces(emb, dual);

    // Step 3. Make dual edges to connecte faces.
    findDualEdges(emb, dual);

    // Step 4. Make inter-graph forward links to src.
    makeForwardLinks(emb, dual);
}


void PlanarDualGraphMaker::copyInputGraph(
    Graph&         src,
    EmbeddedGraph& emb
) {

    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    auto nitPair = src.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {

        auto np = make_unique<EmbeddedNode>();
        np->pushIGBackwardLink(nit);
        nodePairs.push_back(make_pair(nit,std::move(np)));

    }

    auto eitPair = src.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto ep = make_unique<EmbeddedEdge>();
        ep->pushIGBackwardLink(eit);
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }

    src.copySubgraph(nodePairs, edgePairs, emb);

    for (auto eit = emb.edges().first; eit != emb.edges().second; eit++) {

        auto& E  = dynamic_cast<EmbeddedEdge&>(*(*eit));
        auto& N1 = E.incidentNode1();
        auto& N2 = E.incidentNode2();

        auto& HE1 = E.mHalfEdge1;
        auto& HE2 = E.mHalfEdge2;

        HE1.mEmbeddedEdge    = E.backIt();
        HE2.mEmbeddedEdge    = E.backIt();
        HE1.mTheOtherHalfOn1 = false;
        HE2.mTheOtherHalfOn1 = true;
        HE1.mSrcNode         = N1.backIt();
        HE1.mDstNode         = N2.backIt();
        HE2.mSrcNode         = N2.backIt();
        HE2.mDstNode         = N1.backIt();

    }
}


list<node_list_it_t> PlanarDualGraphMaker::initializeUnprocessedQueues(
    EmbeddedGraph& emb
) {

    list<node_list_it_t> nodesPending;

    for (auto nit = emb.nodes().first; nit != emb.nodes().second; nit++) {

        auto& N = dynamic_cast<EmbeddedNode&>(*(*nit));


        /** Place the node into 'pending' queue.
         * During the processing loop far below, if a node has no more half
         * edge to explore, it will be removed from the middle of the queue.
         *
         * mItIntoNodesPending is used to remember the location in the queue
         * to remove it.
         */
        if (N.degree() > 0) {
            N.mItIntoNodesPending
                        = nodesPending.insert(nodesPending.end(), N.backIt());

            for (auto eit = N.incidentEdges().first;
                                     eit != N.incidentEdges().second; eit++) {

                auto& E   = dynamic_cast<EmbeddedEdge&>(*(*(*eit)));

                auto& he1 = E.mHalfEdge1;
                auto& he2 = E.mHalfEdge2;

                auto pos = N.mEdgesPending.insert(N.mEdgesPending.end(), *eit);

                if ( he1.mSrcNode == N.backIt()) {
                    he1.mItIntoEdgesPending = pos;
                }
                else {
                    he2.mItIntoEdgesPending = pos;
                }
            }
        }
    }
    return nodesPending; //rvo
}


void PlanarDualGraphMaker::findFaces(
    EmbeddedGraph& emb,
    DualGraph&     dual
) {
    // Cope with k1.
    if (emb.numNodes()==1 && emb.numEdges()==0) {
        dual.addNode(make_unique<EmbeddedFace>());
    }

    list<node_list_it_t> nodesPending = initializeUnprocessedQueues(emb);

    while (nodesPending.size() > 0) {

        // Find an unprocessed node.
        auto& N = dynamic_cast<EmbeddedNode&>(*(*(*(nodesPending.begin()))));

        while (N.mEdgesPending.size() > 0 ) {


            // Find an unprocessed halfedge

            auto& E   = dynamic_cast<EmbeddedEdge&>(
                                            *(*(*(N.mEdgesPending.begin()))));

            auto& AN  = dynamic_cast<EmbeddedNode&>(E.adjacentNode(N));

            /** The initial edge with adjacent nodes look like the following:
             *
             *
             *       <-- mSrcNode:he:mDstNode -->
             * 
             *     N    <--->     E      <--->    AN
             *    sit         eit/HEOn1           dit
             */
            node_list_it_t sit   = N.backIt();  // source node
            edge_list_it_t eit   = E.backIt();  // edge
            bool           HEOn1 = E.mHalfEdge1.mSrcNode == N.backIt();
            node_list_it_t dit   = AN.backIt(); // dest node

            /* The half edges around a face will be
             *  accumulated in the followinglists.
             */
            list<edge_list_it_t> cycleEdges;
            list<bool>           cycleHalfEdgesOn1;

            cycleEdges.push_back(eit);
            cycleHalfEdgesOn1.push_back(HEOn1);            
            if (HEOn1) {
                N.mEdgesPending.erase(E.mHalfEdge1.mItIntoEdgesPending);
            }
            else {
                N.mEdgesPending.erase(E.mHalfEdge2.mItIntoEdgesPending);
            }

            findNextHalfEdge(sit, eit, HEOn1, dit);
#ifdef UNIT_TESTS
            map<Node*,long> nodeMap;
            nodeMap[&N] = 1;
#endif
            // Explore the half edges and form a face cycle
            while (sit != N.backIt()) {

                auto& S  = dynamic_cast<EmbeddedNode&>(*(*sit));
                auto& E  = dynamic_cast<EmbeddedEdge&>(*(*eit));
#ifdef UNIT_TESTS
                if (nodeMap.find(&S)!=nodeMap.end()) {
                    auto& N = dynamic_cast<NumNode&>(S.IGBackwardLinkRef());
                    cerr << "!!! ERROR: Duplicate node [" << N.num()
                         << "] found during dual graph generation. !!!\n";
//                    mDupFound = true;
                }
                else {
                    nodeMap[&S] = 1;
                }
#endif
                cycleEdges.push_back(eit);
                cycleHalfEdgesOn1.push_back(HEOn1);

                if (HEOn1) {
                    S.mEdgesPending.erase(E.mHalfEdge1.mItIntoEdgesPending);
                }
                else {
                    S.mEdgesPending.erase(E.mHalfEdge2.mItIntoEdgesPending);
                }

                if (S.mEdgesPending.size()==0) {
                    nodesPending.erase(S.mItIntoNodesPending);
                }

                findNextHalfEdge(sit, eit, HEOn1, dit);

            }

            if (N.mEdgesPending.size()==0) {

                nodesPending.erase(N.mItIntoNodesPending);
            }

            // Now we have a face cycle. Create an EmbeddedFace.
            makeOneFace(
                    dual, std::move(cycleEdges), std::move(cycleHalfEdgesOn1));

        }
    }
}


void PlanarDualGraphMaker::makeOneFace(
    DualGraph&             dual,
    list<edge_list_it_t>&& cycleEdges,
    list<bool>&&           cycleHalfEdgesOn1
) {
    auto& F = dynamic_cast<EmbeddedFace&>(
                                    dual.addNode(make_unique<EmbeddedFace>()));

    auto ceIt       = cycleEdges.begin();
    auto cheIt      = cycleHalfEdgesOn1.begin();
    auto ceItPrev   = cycleEdges.begin();
    auto cheItPrev  = cycleHalfEdgesOn1.begin();

    for (; ceIt != cycleEdges.end(); ceIt++,cheIt++) {

        auto& E  = dynamic_cast<EmbeddedEdge&>(*(*(*ceIt)));
        auto& HE = (*cheIt)?E.mHalfEdge1:E.mHalfEdge2;
        HE.mEmbeddedFace = F.backIt();

        if (ceIt != cycleEdges.begin()) {

            auto& Eprev = dynamic_cast<EmbeddedEdge&>(*(*(*ceItPrev)));
            auto& HEprev = (*cheItPrev)?Eprev.mHalfEdge1:Eprev.mHalfEdge2;

            HE.mPrevEdge = Eprev.backIt();
            HE.mPrevHalfEdgeOn1 = (*cheItPrev);
            HEprev.mNextEdge = E.backIt();
            HEprev.mNextHalfEdgeOn1 = (*cheIt);

            ceItPrev  = ceIt;
            cheItPrev = cheIt;
        }
    }

    ceIt  = cycleEdges.begin();
    cheIt = cycleHalfEdgesOn1.begin();
    auto& E  = dynamic_cast<EmbeddedEdge&>(*(*(*ceIt)));
    auto& HE = (*cheIt)?E.mHalfEdge1:E.mHalfEdge2;
    auto& Eprev = dynamic_cast<EmbeddedEdge&>(*(*(*ceItPrev)));
    auto& HEprev = (*cheItPrev)?Eprev.mHalfEdge1:Eprev.mHalfEdge2;

    HE.mPrevEdge = Eprev.backIt();
    HE.mPrevHalfEdgeOn1 = (*cheItPrev);
    HEprev.mNextEdge = E.backIt();
    HEprev.mNextHalfEdgeOn1 = (*cheIt);

    F.mCycleEdges = std::move(cycleEdges);
    F.mCycleHalfEdgesOn1 = std::move(cycleHalfEdgesOn1);

}


void PlanarDualGraphMaker::findNextHalfEdge(
    node_list_it_t&  sit,   // (io): source node pointer
    edge_list_it_t&  eit,   // (io): edge pointer
    bool&            HEOn1, // (io): half edge is mHaldEdge1
    node_list_it_t&  dit    // (io): destination node pointer
) {

    auto& E  = dynamic_cast<EmbeddedEdge&>(*(*eit));
    auto& D  = dynamic_cast<EmbeddedNode&>(*(*dit));

    node_incidence_it_t iit;

    if (E.incidentNode1().backIt()==D.backIt()) {

        iit = E.incidentBackItNode1();
    }
    else {

        iit = E.incidentBackItNode2();
    }

    if (iit == D.incidentEdges().first) {
        iit = D.incidentEdges().second;
    }

    iit--;

    auto& Snext = D;

    auto& Enext = dynamic_cast<EmbeddedEdge&>(*(*(*(iit))));

    auto& Dnext = dynamic_cast<EmbeddedNode&>(Enext.adjacentNode(Snext));

    HEOn1 = Enext.mHalfEdge1.mSrcNode == Snext.backIt();

    sit = Snext.backIt();
    eit = Enext.backIt();
    dit = Dnext.backIt();

}


void PlanarDualGraphMaker::findDualEdges(
    EmbeddedGraph& emb,
    DualGraph&     dual
) {

    /** For each EmbeddedEdge, create a dual edge and connect two
     *  Embedded faces.
     */
    for (auto eIt = emb.edges().first; eIt != emb.edges().second; eIt++) {

        auto& E   = dynamic_cast<EmbeddedEdge&>(*(*eIt));
        auto& HE1 = E.mHalfEdge1;
        auto& HE2 = E.mHalfEdge2;
        auto& F1  = dynamic_cast<EmbeddedFace&>(*(*HE1.mEmbeddedFace));
        auto& F2  = dynamic_cast<EmbeddedFace&>(*(*HE2.mEmbeddedFace));

        auto& DE  = dynamic_cast<DualEdge&>(
                     dual.addEdge(make_unique<DualEdge>(), F1, F2));
        E.mDualEdge = DE.backIt();
        DE.mEmbeddedEdge = E.backIt();
        DE.pushIGBackwardLink(E.IGBackwardLink());

    }

    /** Reorder incident dual edges of each face according
     *  to the surrounding half edges.
     */
    for (auto fIt = dual.nodes().first; fIt != dual.nodes().second; fIt++) {

        auto& F = dynamic_cast<EmbeddedFace&>(*(*fIt));

        list<edge_list_it_t> orderedDualEdges;

        for (auto eIt : F.mCycleEdges) {

            auto& E = dynamic_cast<EmbeddedEdge&>(*(*eIt));
            orderedDualEdges.push_back(E.mDualEdge);

        }

        F.reorderIncidence(std::move(orderedDualEdges));

    }
}


void PlanarDualGraphMaker::makeForwardLinks(
    EmbeddedGraph& emb,
    DualGraph&     dual
) {

    for (auto nit = emb.nodes().first; nit != emb.nodes().second; nit++) {
        auto& N = dynamic_cast<EmbeddedNode&>(*(*nit));
        N.IGBackwardLinkRef().pushIGForwardLink(nit);
    }

    for (auto eit = emb.edges().first; eit != emb.edges().second; eit++) {
        auto& EE = dynamic_cast<EmbeddedEdge&>(*(*eit));
        EE.IGBackwardLinkRef().pushIGForwardLink(eit);
    }

}

}// namespace Undirected

}// namespace Wailea

