#include "undirected/connected_decomposer.hpp"

namespace Wailea {

namespace Undirected {

using namespace std;

void ConnectedDecomposer::decompose(
    Graph& g,
    vector<vector<node_list_it_t> >& nodeSets
) {
    nodeSets.clear();

    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {
        auto& N = *(*nit);
        N.pushUtility(0);
    }

    for (auto nit = g.nodes().first; nit != g.nodes().second; nit++) {

        auto& N = *(*nit);

        if (N.utility() == 0) {

            vector<node_list_it_t> nodes;
            list<node_list_it_t> BFSStack;
            BFSStack.push_back(nit);
            N.setUtility(1);

            while (BFSStack.size()>0) {

                auto& N2 = *(*(*(BFSStack.begin())));
                BFSStack.pop_front();

                nodes.push_back(N2.backIt());
                for (auto eit = N2.incidentEdges().first; 
                                   eit != N2.incidentEdges().second; eit++ ) {
                    auto& E = *(*(*eit));
                    auto& A = E.adjacentNode(N2);
                    if (A.utility()==0) {
                        A.setUtility(1);
                        BFSStack.push_back(A.backIt());
                    }
                }

            }

            nodeSets.push_back(std::move(nodes));
        }
    }

    g.popUtilityFromNodes();

}

}// namespace Undirected

}// namespace Wailea

