#include "undirected/random_planar_graph_generator.hpp"
#include <ctime>
#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/random_planar_graph_generator.cpp
 *
 * @brief
 *     
 * @details
 */

namespace Wailea {

namespace Undirected { 

using namespace std;



class RandGenNode : public Node {
  public:
    RandGenNode(long n):mNum(n){;}
    virtual ~RandGenNode(){;}
    inline long num(){return mNum;};
  private:
        long mNum;
};



void RandomPlanarGraphGenerator::createInducedGraph(
    Graph&                  src,
    Graph&                  dst,
    vector<node_list_it_t>& nodes
) {
    vector<edge_list_it_t> edges = src.nodeInducedEdges(nodes);

    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;

    for (auto nit : nodes) {
        auto& N = dynamic_cast<RandGenNode&>(*(*nit));
        auto np = make_unique<RandGenNode>(N.num());
        nodePairs.push_back(make_pair(nit,std::move(np)));
    }

    for (auto eit : edges) {
        auto ep = make_unique<Edge>();
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }

    src.copySubgraph(nodePairs, edgePairs, dst);
}


vector<node_list_it_t> RandomPlanarGraphGenerator::findNonTransitiveDeg2Nodes(
    Graph& g
) {
    vector<node_list_it_t> deg2Nodes;
    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {

        auto& N = *(*nit);
        if (N.degree()==2) {

            auto eit = N.incidentEdges().first;
            auto& E1 = *(*(*eit));
            auto& A1 = E1.adjacentNode(N);
            eit++;
            auto& E2 = *(*(*eit));
            auto& A2 = E2.adjacentNode(N);
            bool transitive = false;
            for (auto eit = A1.incidentEdges().first; 
                                     eit != A1.incidentEdges().second; eit++) {
                auto& A1E = *(*(*eit));
                auto& A1A = A1E.adjacentNode(A1);
                if (A1A.backIt() == A2.backIt()) {
                    transitive = true;
                }
            }
            if (!transitive) {
                // N is a candidite for removal.
                deg2Nodes.push_back(nit);
            }
        }
    }
    return deg2Nodes;
}


void RandomPlanarGraphGenerator::removeDeg2Node(
    Graph&         g,
    Node&          N
) {
    if (N.degree()!=2) {
        // Previous call may have made the degree less than 2.
        return;
    }
    auto eit = N.incidentEdges().first;
    auto& E1 = *(*(*eit));
    auto& A1 = E1.adjacentNode(N);
    eit++;
    auto& E2 = *(*(*eit));
    auto& A2 = E2.adjacentNode(N);
    if (A1.backIt() == A2.backIt()) {
        // Previous call may have made A1 == A2.
        g.removeNode(N);
        return;
    }

    bool A1A2exists = false;
    for (auto a1it = A1.incidentEdges().first;
                                 a1it != A1.incidentEdges().second; a1it++) {
        auto& A1E = *(*(*a1it));
        auto& A1A = A1E.adjacentNode(A1);
        if (A1A.backIt() == A2.backIt()) {
            A1A2exists = true;
            break;
        }
    }
    g.removeNode(N);
    if (!A1A2exists) {
        g.addEdge(make_unique<Edge>(), A1, A2);
    }
}


void RandomPlanarGraphGenerator::generateTriangularMesh(
    Graph&        G,
    size_t        M,
    size_t        N
) {
    size_t numNodes = M*N;


    std::srand( time( NULL ) );

    vector<node_list_it_t> nodesArray;

    for (size_t i = 0; i < numNodes; i++) {
        auto& N = G.addNode(make_unique<RandGenNode>(i+1));
        nodesArray.push_back(N.backIt());
    }

    for (size_t i = 0; i < M; i++) {

        for (size_t j = 0; j < N; j++) {

            if (i < M-1){

                // (i,j)->(i+1,j)
                auto&N1 = *(*(nodesArray[ i*N     + j ]));
                auto&N2 = *(*(nodesArray[ (i+1)*N + j ]));
                G.addEdge(make_unique<Edge>(), N1, N2);
            }
            if (j < N-1){

                // (i,j)->(i,j+1)
                auto&N1 = *(*(nodesArray[ i*N +   j ]));
                auto&N2 = *(*(nodesArray[ i*N + j+1 ]));
                G.addEdge(make_unique<Edge>(), N1, N2);
            }
            if (i < M-1 && j < N-1) {

                // (i,j)->(i+1,j+1)
                auto&N1 = *(*(nodesArray[ i*N     + j   ]));
                auto&N2 = *(*(nodesArray[ (i+1)*N + j+1 ]));
                G.addEdge(make_unique<Edge>(), N1, N2);
            }
        }
    }
}


void RandomPlanarGraphGenerator::removeEdgesRandomly(
    Graph& G,
    float  prop
) {
    vector<edge_list_it_t> edgesArray;
    vector<bool>           edgesRemoved;
    for (auto eit = G.edges().first; eit != G.edges().second; eit++) {
        edgesArray.push_back(eit);
        edgesRemoved.push_back(false);
    }

    size_t numEdges   = G.numEdges();
    size_t numRemoved = (size_t)((float)numEdges * prop);

    for (size_t i = 0 ; i < numRemoved; i++) {
        size_t randIndex = static_cast<size_t>(std::rand() % numEdges);

        if (!edgesRemoved[randIndex]) {
            edgesRemoved[randIndex] = true;
            auto& E = *(*(edgesArray[randIndex]));
            G.removeEdge(E);
        }

    }

}


void RandomPlanarGraphGenerator::removeDeg2NodesRandomly(
    Graph& G, 
    long   thresholdPercent,
    long   rangePercent
) {

    long randomizedRange = (std::rand() % rangePercent * 2) - rangePercent;
    thresholdPercent = thresholdPercent + randomizedRange;

    if (thresholdPercent < 0) {
        thresholdPercent = 0;
    }
    if (thresholdPercent > 100) {
        thresholdPercent = 100;
    }

    long threshold = ((long)(G.numNodes()) * thresholdPercent) / 100;

    vector<node_list_it_t> deg2Nodes = findNonTransitiveDeg2Nodes(G);
    vector<bool>           deg2NodesRemoved;


    for(size_t i = 0; i < deg2Nodes.size(); i++) {
        deg2NodesRemoved.push_back(false);
    }    
    long excess = deg2Nodes.size() - threshold;
    for (long i = 0; i < excess; i++) {

        size_t randIndex = static_cast<size_t>(std::rand() % deg2Nodes.size());

        if (!deg2NodesRemoved[randIndex]) {

            deg2NodesRemoved[randIndex] = true;

            removeDeg2Node(G, *(*(deg2Nodes[randIndex])));

        }

    }

}


void RandomPlanarGraphGenerator::emitGraph(Block& G)
{
    mNumVertices.push_back(G.numNodes());
    size_t index = 1;
    for (auto nit = G.nodes().first; nit!= G.nodes().second; nit++) {
        auto& BN = dynamic_cast<BlockNode&>(*(*nit));
//        auto& N  = dynamic_cast<NumNode&>(BN.IGBackwardLinkRef());
//        BN.pushUtility(N.num());
        BN.pushUtility(index++);

    }

    vector<pair<size_t,size_t> > edgeSet;
    for (auto eit = G.edges().first; eit != G.edges().second; eit++) {

        auto& E = *(*eit);
        auto  i1 = static_cast<size_t>(E.incidentNode1().utility());
        auto  i2 = static_cast<size_t>(E.incidentNode2().utility());
        edgeSet.push_back(make_pair(i1, i2));

    }

    mEdgeSets.push_back(std::move(edgeSet));

    G.popUtilityFromNodes();

}


void RandomPlanarGraphGenerator::printInducedGraph(Graph& G)
{
    cerr << "Nodes: ";
    for (auto nit = G.nodes().first; nit != G.nodes().second; nit++) {
        auto& N = dynamic_cast<RandGenNode&>(*(*nit));
        cerr << N.num() << " ";
    }
    cerr << "\n";
    cerr << "Edges: ";
    for (auto eit = G.edges().first; eit != G.edges().second; eit++) {
        auto& E = dynamic_cast<Edge&>(*(*eit));
        auto& N1 = dynamic_cast<RandGenNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<RandGenNode&>(E.incidentNode2());
        cerr << "{" << N1.num() << "," << N2.num() << "} ";
    }
    cerr << "\n";
}


void RandomPlanarGraphGenerator::generateGraphs(
    size_t        M,
    size_t        N,
    float         prop
)
{

    // Create a dense triangular mesh.
    Graph G;  
    generateTriangularMesh(G, M, N);

    // Randomly remove edges
    removeEdgesRandomly(G, prop);

    // Randomly remove non-transitive deg2-nodes.
    removeDeg2NodesRandomly(G, 30, 20);

    // Find connected components.
    ConnectedDecomposer decomp;
    vector<vector<node_list_it_t> >connectedComponents;
    decomp.decompose(G, connectedComponents);

    // Find bi-connected components.
    for (auto& connectedComponent :  connectedComponents) {
//        cerr << "conn:" << connectedComponent.size() << "\n";
        if (connectedComponent.size() >= 3) {

            Graph CG;
            createInducedGraph(G, CG, connectedComponent);
//            cerr << "    conn: [" << CG.numNodes() << "," << CG.numEdges() << "]\n";
//            printInducedGraph(CG);
            BiconnectedDecomposer bidecomp;
            BCTree bcTree = bidecomp.decompose(CG);

            for (auto bnit = bcTree.nodes().first; 
                                       bnit != bcTree.nodes().second; bnit++) {

                auto& TN = dynamic_cast<BCTreeNode&>(*(*bnit));

                if (TN.type() == BCTreeNode::BlockType) {

                    auto& B = TN.block();
//                    cerr << "        biconn: [" << B.numNodes() << "," << B.numEdges() << "]\n";
                    if (B.numNodes() >= 3) {
//                        cerr << "emit\n";
                        emitGraph(B);
                    }
                }
            }
        }
    }
}

}// namespace Undirected

}// namespace Wailea


