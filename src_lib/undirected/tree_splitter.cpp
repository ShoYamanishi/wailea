#include "undirected/tree_splitter.hpp"

namespace Wailea {

namespace Undirected {

using namespace std;


TreeSplitter::TreeSplitter(Graph& tree):mTree(tree){;}


TreeSplitter::~TreeSplitter(){;}


void TreeSplitter::prepareTree(list<edge_list_it_t>& cutSet)
{
    for (auto eItIt = mTree.edges().first; 
                                     eItIt != mTree.edges().second; eItIt++ ) {
        auto& E = dynamic_cast<Edge&>(*(*eItIt));
        E. pushUtility(0);
    }

    for (auto eit : cutSet) {
        auto& E = dynamic_cast<Edge&>(*(*eit));
        E.setUtility(1);
    }

    for (auto nItIt = mTree.nodes().first; 
                                      nItIt != mTree.nodes().second; nItIt++) {
        auto& N = dynamic_cast<Node&>(*(*nItIt));
        N.pushUtility(0);
    }  
}


list<node_list_it_t> TreeSplitter::findComponent(node_list_it_t startIt)
{
    list<node_list_it_t> nodesFound;

    list<node_list_it_t> Q;
    Q.push_back(startIt);

    while (Q.size() > 0) {

        auto qit = *(Q.rbegin());
        auto& N = dynamic_cast<Node&>(*(*qit));
        Q.pop_back();
        nodesFound.push_back(qit);

        N.setUtility(1);

        for (auto eit = N.incidentEdges().first; 
                                     eit != N.incidentEdges().second; eit++ ) {
            auto& E = dynamic_cast<Edge&>(*(*(*eit)));
            if (E.utility() == 0) {
                auto& A = E.adjacentNode(N);
                if (A.utility() == 0) {
                    Q.push_back(A.backIt());
                }
            }
        }

    }

    return nodesFound; // rvo    
}

void TreeSplitter::cleanUpTree()
{
    mTree.popUtilityFromEdges();
    mTree.popUtilityFromNodes();
}

}// namespace Undirected

}// namespace Wailea
