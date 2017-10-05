#include "undirected/base.hpp"
#include "undirected/planar_insertion_path_finder.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/planar_dual_graph_maker.hpp"



#ifdef UNIT_TESTS
#include "undirected/bctree.hpp"
#include "undirected/gmw_skeleton_generator.hpp"
#endif

/**
 * @file undirected/planar_insertion_path_finder.cpp
 *
 */
namespace Wailea {

namespace Undirected {

using namespace std;


void PlanarInsertionPathFinder::findDualGraph()
{

    // Step 1. Find an st ordering
    STNumbering st;
    auto nit1 = mG.nodes().first;
    auto nit2 = mG.nodes().first;
    nit2++;

    vector<node_list_it_t> stOrdering = 
                              st.getBipolarOrientation(mG, *(*nit1), *(*nit2));

    // Step 2. Find an embedding
    BLPlanarityTester tester;
    tester.findEmbedding(mG, stOrdering);

    // Step 3. Find the dual graph for the embedding
    PlanarDualGraphMaker maker;    
    maker.makeDualGraph(mG, mGemb, mGdual);

}




list<edge_list_it_t> PlanarInsertionPathFinder::findInsertionPath(
    node_list_it_t nIt1,
    edge_list_it_t eIt1,
    node_list_it_t nIt2,
    edge_list_it_t eIt2
) {

    // Step 1. Find a dual graph.
    findDualGraph();

//    cerr << "mGemb nodes:" << mGemb.numNodes() << "\n";
//    cerr << "mGemb edges:" << mGemb.numEdges() << "\n";
//    cerr << "mGdual nodes:" << mGdual.numNodes() << "\n";
//    cerr << "mGaudl edges:" << mGdual.numEdges() << "\n";

//    for (auto tmpIt1 = mGemb.nodes().first; 
//                                tmpIt1 != mGemb.nodes().second;tmpIt1++ ) {
//        auto& N  = dynamic_cast<EmbeddedNode&>(*(*tmpIt1));
//        auto& SN = dynamic_cast<GMWSkeletonNode&>(*(*(N.IGBackwardLink())));
//        auto& BN = dynamic_cast<BlockNode&>(*(*(SN.IGBackwardLink())));
//        auto& ON = dynamic_cast<NumNode&>(*(*(BN.IGBackwardLink())));
//        cerr << "N: " << ON.num() << ": ";
//        for (auto tmpIt2 = N.incidentEdges().first ;
//                   tmpIt2 != N.incidentEdges().second; tmpIt2++) {
//            auto& E = dynamic_cast<EmbeddedEdge&>(*(**(tmpIt2)));
//            auto& AN = dynamic_cast<EmbeddedNode&>(E.adjacentNode(N));
//            auto& SAN = dynamic_cast<GMWSkeletonNode&>(*(*(AN.IGBackwardLink())));
//            auto& BAN = dynamic_cast<BlockNode&>(*(*(SAN.IGBackwardLink())));
//            auto& OAN = dynamic_cast<NumNode&>(*(*(BAN.IGBackwardLink())));
//            cerr << "AN: " << OAN.num() << " ";
//        }
//        cerr << "\n";
//    }

    // Step 2. Find the corresponding end faces.
    node_list_it_t NembIt1;
    node_list_it_t NembIt2;
    if (nIt1 != mG.nodes().second|| nIt2 != mG.nodes().second) {
        for (auto nit = mGemb.nodes().first;
                                        nit != mGemb.nodes().second; nit++ ) {
            auto& EN = dynamic_cast<EmbeddedNode&>(*(*nit));
            if (EN.IGBackwardLink() == nIt1) {
                NembIt1 = nit;
            }
            if (EN.IGBackwardLink() == nIt2) {
                NembIt2 = nit;        
            }
        }
    }

    edge_list_it_t EembIt1;
    edge_list_it_t EembIt2;
    if (eIt1 != mG.edges().second|| eIt2 != mG.edges().second) {
        for (auto eit = mGemb.edges().first;
                                         eit != mGemb.edges().second; eit++ ) {
            auto& EE = dynamic_cast<EmbeddedEdge&>(*(*eit));
            if (EE.IGBackwardLink() == eIt1) {
                EembIt1 = eit;
            }
            if (EE.IGBackwardLink() == eIt2) {
                EembIt2 = eit;        
            }
        }
    }

    vector<node_list_it_t> Faces1 = (nIt1!=mG.nodes().second)?
                                 incidentFaces(NembIt1):incidentFaces(EembIt1);
    //cerr << "Faces1:" << Faces1.size() << "\n";
    vector<node_list_it_t> Faces2 = (nIt2!=mG.nodes().second)?
                                 incidentFaces(NembIt2):incidentFaces(EembIt2);
    //cerr << "Faces2:" << Faces2.size() << "\n";

    // Find the shortest path between two sets of faces and then find the
    // crossing edges.
    list<edge_list_it_t> crossingEdges = performDijkstra(Faces1, Faces2);

    return crossingEdges; // rvo
}


list<edge_list_it_t> PlanarInsertionPathFinder::performDijkstra(
    vector<node_list_it_t> faces1,
    vector<node_list_it_t> faces2
) {
    list<edge_list_it_t>   crossingEdges;

    vector<size_t>         distances;
    vector<edge_list_it_t> backPointers;

    size_t index = 0;

    // Initialize the BFS data structures.
    // The nodes in mGdual are enhanced with distance and backPointers
    // and Node::utility works as glue between nodes to those two arrays.
    edge_list_it_t nullIt = mGdual.edges().second;
    for (auto nit = mGdual.nodes().first;
                                        nit != mGdual.nodes().second; nit++ ) {
        auto& N = dynamic_cast<EmbeddedFace&>(*(*nit));

        distances.push_back(std::numeric_limits<utility_t>::max());
        backPointers.push_back(nullIt);
        N.pushUtility(index++);
//        cerr << "F"  << N.utility() << ": [";
//        for (auto tit : N.mCycleEdges) {
//            auto& E = dynamic_cast<EmbeddedEdge&>(*(*tit));
//            auto& OE = dynamic_cast<Edge&>(*(*(E.mOriginal)));
//            auto& N1 = dynamic_cast<NumNode&>(OE.incidentNode1());
//            auto& N2 = dynamic_cast<NumNode&>(OE.incidentNode2());
//            cerr << "{" << N1.num() << "," << N2.num() << "} ";
//        }
//        cerr << "]\n";        
    }

    // Set the distance of the initial nodes to 0, and put them to the queue.
    list<node_list_it_t> Q;
    for (auto fit : faces1) {

        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
//        cerr << "init: " << F.utility() << "\n";
        distances[F.utility()] = 0;
        Q.push_back(fit);
    }


    while (Q.size()>0) {

        auto& F = dynamic_cast<EmbeddedFace&>(*(*(*(Q.begin()))));
//        cerr << "loop F: " << F.utility() << " dist: "
//                           << distances[F.utility()] << "\n";

        Q.pop_front();

        for (auto deit = F.incidentEdges().first; 
                                   deit != F.incidentEdges().second; deit++ ) {
            auto& DE = dynamic_cast<DualEdge&>(*(*(*deit)));
            auto& AF = dynamic_cast<EmbeddedFace&>(DE.adjacentNode(F));

//            auto& OE = dynamic_cast<Edge&>(*(*(DE.mOriginal)));
//            auto& N1 = dynamic_cast<NumNode&>(OE.incidentNode1());
//            auto& N2 = dynamic_cast<NumNode&>(OE.incidentNode2());
//            cerr << "{" << N1.num() << "," << N2.num() << "} ";

//            cerr << "AF: " << AF.utility() << " dist: "
//                           << distances[AF.utility()] << "\n";

            if (distances[AF.utility()] > distances[F.utility()] + 1) {

                distances[AF.utility()] = distances[F.utility()] + 1;
                backPointers[AF.utility()] = DE.backIt();



                Q.push_back(AF.backIt());
//                cerr << "Pushing AF:\n";
            }
        }
    }

//    for (auto n : distances) {
//        cerr << "n: " << n << "\n";
//    }

    // Find the face with the minimum distance in faces2.
    utility_t cur_min = std::numeric_limits<utility_t>::max();
    auto      cur_eit  = nullIt;
    auto      cur_fit  = mGdual.nodes().second;

    for (auto fit : faces2) {

        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
//        cerr << "m: " << F.utility() << "\n";
//        cerr << "cur_min: " << cur_min << "\n";
//        cerr << "distance: " << distances[F.utility()] << "\n";
        if (cur_min > distances[F.utility()]) {
//            cerr << "updating\n";
            cur_min = distances[F.utility()];
            cur_fit = fit;
            cur_eit = backPointers[F.utility()];

        }
    }


    // From the face in face2 back to face1, explore the min-path 
    // using backPointers.
    while (cur_eit != nullIt) {

        auto& F  = dynamic_cast<EmbeddedFace&>(*(*cur_fit));
//cerr << "f: " << F.utility() ;
        auto& DE = dynamic_cast<DualEdge&>(*(*cur_eit));
        auto& AF = dynamic_cast<EmbeddedFace&>(DE.adjacentNode(F));
//cerr << " af: " << AF.utility();
        crossingEdges.push_front(DE.IGBackwardLink());

//        auto& OE = dynamic_cast<Edge&>(*(*(DE.mOriginal)));
//        auto& N1 = dynamic_cast<NumNode&>(OE.incidentNode1());
//        auto& N2 = dynamic_cast<NumNode&>(OE.incidentNode2());
//        cerr << "{" << N1.num() << "," << N2.num() << "}\n";

        cur_fit = AF.backIt();
        cur_eit  = backPointers[AF.utility()];

    }

    mGdual.popUtilityFromNodes();

    return crossingEdges; // rvo

}


vector<node_list_it_t> PlanarInsertionPathFinder::incidentFaces(
    node_list_it_t nit
) {
    vector<node_list_it_t> faces;

    auto& Nemb = dynamic_cast<EmbeddedNode&>(*(*nit));

    for (auto eit = Nemb.incidentEdges().first;
                                  eit != Nemb.incidentEdges().second; eit++) {
        auto& Eemb = dynamic_cast<EmbeddedEdge&>(*(*(*eit)));
        if (Eemb.mHalfEdge1.mSrcNode == nit) {
            faces.push_back(Eemb.mHalfEdge1.mEmbeddedFace);
        }
        else {
            faces.push_back(Eemb.mHalfEdge2.mEmbeddedFace);
        }
    }

    return faces;
}


vector<node_list_it_t> PlanarInsertionPathFinder::incidentFaces(
    edge_list_it_t eit
) {

    vector<node_list_it_t> faces;

    auto& Eemb = dynamic_cast<EmbeddedEdge&>(*(*eit));
    faces.push_back(Eemb.mHalfEdge1.mEmbeddedFace);
    faces.push_back(Eemb.mHalfEdge2.mEmbeddedFace);

    return faces;
}

}// namespace Undirected

}// namespace Wailea
