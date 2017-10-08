#include <iostream>
#include <fstream>
#include <sstream>
#include "undirected/base.hpp"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/bl_tree.hpp"
#include "undirected/bl_tree_node.hpp"
#include "undirected/bl_inline.hpp"
#include "undirected/st_numbering.hpp"

using namespace std;
using namespace Wailea::Undirected;

#ifndef UNIT_TESTS
class NumNode : public Node {
  public:
    NumNode(long n):mNum(n){;}
    virtual ~NumNode(){;}
    inline long num(){return mNum;};
  private:
        long mNum;
};
#endif

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " InputGraphFile\n"
              << "    The results are in |N|<TAB>|E|<TAB>Total attempts<TAB>num failed<LN>";
}


int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        show_usage(argv[0]);
        return 1;
    }


    //std::srand( time( NULL ) );

    string   filename(argv[1]);
    ifstream gs;
    string   oneLine;

    size_t   numNodes = 0;
    size_t   numEdges = 0;
    gs.open (filename);;
    if (gs.is_open()) {

        getline(gs, oneLine);
        stringstream lineStream(oneLine);
        lineStream >> numNodes >> numEdges;
    }        

    Graph G;
    vector <node_list_it_t> nodes;
    for (size_t i = 0; i < numNodes; i++) {
        auto& N = G.addNode(make_unique<NumNode>(i+1));
        nodes.push_back(N.backIt());
    }
    size_t cycle = 0;
    while (getline(gs, oneLine)) {
        if(oneLine.length()>0) {
            cycle++;
            size_t I1;
            size_t I2;
            stringstream lineStream(oneLine);
            lineStream >> I1 >> I2;
            auto& N1 = dynamic_cast<NumNode&>(*(*nodes[I1-1]));
            auto& N2 = dynamic_cast<NumNode&>(*(*nodes[I2-1]));
            G.addEdge(make_unique<Edge>(), N1, N2);
        }
    }
    gs.close();

    size_t fail    = 0;
    size_t success = 0;
    if (cycle != numEdges) {
        return 1;
    }

    vector<node_list_it_t> TList;
    for (auto tIt = G.nodes().first; tIt != G.nodes().second; tIt++){
        TList.push_back(tIt);
    }
    if (numNodes <= 32) {
        for (size_t i = 0; i < G.numNodes() ; i++ ) {
            for (size_t j = 0; j < G.numNodes() ; j++ ) {
                if (i == j) {
                    continue;
                }
                STNumbering st;
                vector<node_list_it_t> st_list =
                       st.getBipolarOrientation(G, **TList[i], **TList[j]);
                BLPlanarityTester tester;
                size_t index = 0;
                bool result1 = tester.isPlanarOldAlg(G, st_list, index);
                bool result2 = tester.isPlanar(G, st_list);
                if (!result2) {
                    cerr << "Failed at iteration: " << index << "\n";
                    return 1;
                }
                if (result1) {
                    success++;
                }
                else {
                    fail++;
                }
            }
        }
    }
    else {
        size_t kMax = numNodes>10000?100:1000;
        for (size_t k = 0; k < kMax ; k++) {
            size_t i = 0;
            size_t j = 0;
            while (i == j) {    
                i = static_cast<size_t>(std::rand() % numNodes);
                j = static_cast<size_t>(std::rand() % numNodes);
            }
            STNumbering st;
            vector<node_list_it_t> st_list =
                       st.getBipolarOrientation(G, **TList[i], **TList[j]);
            //auto& N1 = dynamic_cast<NumNode&>(*(*TList[i]));
            //auto& N2 = dynamic_cast<NumNode&>(*(*TList[j]));
            //cerr << "Trying :[" << N1.num() << "],[" << N2.num() << "]\n";

            BLPlanarityTester tester;
            size_t index = 0;
            bool result1 = tester.isPlanarOldAlg(G, st_list, index);
            bool result2 = tester.isPlanar(G, st_list);
            bool result3 = tester.findEmbedding(G, st_list);
            if (!result2 || !result3) {
                cerr << "Failed at iteration: " << index << "\n";
                return 1;
            }
            if (result1) {
                success++;
            }
            else {
                fail++;
            }
        }
    }
    cerr << numNodes << "\t" << numEdges << "\t" << fail+success << "\t" << fail << "\n";
    return 0;
}


