#include "undirected/random_planar_graph_generator.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " M N PROP OUT_DIR\n"
              << "    M (int) Number of rows in the triangular mesh.\n"
              << "    N (int) Number of columns in the triangular mesh.\n"
              << "    PROP (real 0.0-1.0): Proportion of mesh edges to be removed.\n"
              << "    OUT_DIR: Output directory where the generated graphs are stored.\n"
              << "\n"
              << "    Ex. " << name << "100 200 0.3 .\n"
              << "    Mesh of 100x200 is created and 30% of the edges will be removed.\n";
}


int main(int argc, char *argv[]) {
    
    if (argc < 5) {
        show_usage(argv[0]);
        return 1;
    }

    size_t M = atoi(argv[1]);
    size_t N = atoi(argv[2]);
    float  P = atof(argv[3]);
    std::string out_dir(argv[4]);

    Wailea::Undirected::RandomPlanarGraphGenerator gen;
    gen.generateGraphs(M,N,P);

    for (size_t i = 0; i < gen.numGraphs(); i++ ) {
        if (gen.numVertices(i) > 4) {

            vector<pair<size_t,size_t> >& edgePairs = gen.edges(i);
            std::ostringstream stringStream;
            stringStream << out_dir 
                         << "/planar_graph_" 
                         << gen.numVertices(i) 
                         << "_" 
                         << edgePairs.size()
                         << ".txt";
            ofstream gs;
            gs.open (stringStream.str());

            gs << gen.numVertices(i) << "\t" << edgePairs.size() << "\n";
            for (auto& p : edgePairs) {
                gs << p.first << "\t" << p.second << "\n";
            }
            gs.close();
        }
    }

    return 0;
}

