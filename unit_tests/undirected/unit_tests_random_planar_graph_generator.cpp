#include "gtest/gtest.h"
#include "undirected/random_planar_graph_generator.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/bl_planarity_tester.hpp"

namespace Wailea {

namespace Undirected { 

class RandomPlanarGraphGeneratorTests : public ::testing::Test {

  protected:  

    RandomPlanarGraphGeneratorTests(){;};
    virtual ~RandomPlanarGraphGeneratorTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


/**  @brief DFS for an empty graph.
 */
TEST_F(RandomPlanarGraphGeneratorTests, Test1) {

    RandomPlanarGraphGenerator gen;
//    gen.generateGraphs(20,20,0.4);
    gen.generateGraphs(10,10,0.4);
    cerr << "Number of graphs: " << gen.numGraphs() << "\n";
    for (size_t i = 0; i < gen.numGraphs(); i++ ) {
        if (gen.numVertices(i) > 4) {
            cerr << "Graph[" << i << "]\n";
            cerr << "    Number of nodes: " << gen.numVertices(i) << "\n";
            vector<pair<size_t,size_t> >& edgePairs = gen.edges(i);

            Graph g;
            vector<node_list_it_t> nodes;

            for (size_t j = 1; j <= gen.numVertices(i); j++) {
                auto& N = g.addNode(make_unique<NumNode>(j));
                nodes.push_back(N.backIt());
            }

            cerr << "    Number of edges: " << edgePairs.size() << "\n";
            for (auto& p : edgePairs) {
//            cerr << "    {" << p.first << "," << p.second << "}\n";
                auto& N1 = dynamic_cast<NumNode&>(*(*(nodes[p.first-1])));
                auto& N2 = dynamic_cast<NumNode&>(*(*(nodes[p.second-1])));
                g.addEdge(make_unique<Edge>(), N1, N2);
            }
//            cerr << "\n";


            vector<node_list_it_t> TList;
            for (auto tIt = g.nodes().first; tIt != g.nodes().second; tIt++){
                TList.push_back(tIt);
            }

            size_t success = 0;
            size_t failure = 0;
            for (size_t i = 0; i < g.numNodes() ; i++ ) {
                for (size_t j = 0; j < g.numNodes() ; j++ ) {
                    if (i == j) {
                        continue;
                    }
//            cerr << "Testing [" << i << "][" << j << "]\n";
                    STNumbering st;
                    vector<node_list_it_t> st_list =
                         st.getBipolarOrientation(g, **TList[i], **TList[j]);
                    BLPlanarityTester tester;
                    size_t index = 0;
//                    bool planar = tester.isPlanarOldAlg(g, st_list, index);
                    bool planar = tester.isPlanar(g, st_list);

//                    if (!planar) {
//                        cerr << "Failed at iteration: " << index << "\n";
//                        failure++;
//                    }
//                    else {
//                        cerr << "Succeeded.\n";
//                        success++;
//                    }
                    EXPECT_EQ(planar, true);
                }
            }
//            cerr << "Success: " << success << " Failure: " << failure << "\n";
        }
    }

}

} // namespace Undirected

} // namespace Wailea
