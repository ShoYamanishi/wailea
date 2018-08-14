#include "directed/network_simplex.hpp"
#include "undirected/connected_decomposer.hpp"
#include "directed/tot_unimod_simul_eqs_solver.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file directed/network_simplex.cpp
 *
 * @brief network simplex linear optimization algorithm
 */
namespace Wailea {

namespace Directed { 

using namespace std;

using namespace Wailea::Directed;


void NetworkSimplex::prepareZeroBGraph()
{
    long perturbation = 0;

    auto ePair = mG.edges();
    for(auto eit = ePair.first; eit != ePair.second; eit++) {

        auto& E = dynamic_cast<NetworkSimplexEdge&>(*(*eit));

        E.mArtificial    = false;
        E.mC             = E.mCreal;
        E.mCperturbation = perturbation++;
    }
}


void NetworkSimplex::makeInitialZeroBSpanningTree()
{
    auto nPair = mG.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));

        auto np = make_unique<NSSpanningTreeNode>();
        auto& Nst = dynamic_cast<NSSpanningTreeNode&>(
                                         mSpanningTree.addNode(std::move(np)));
        Nst.pushIGBackwardLink(Ng.backIt());
        Ng.pushIGForwardLink(Nst.backIt());
    }

    mNsGeneration++;
    list<node_list_it_t> nodesToBeChecked;

    auto& Rg  = dynamic_cast<NetworkSimplexNode&>(*(*nPair.first));
    auto& Rst = dynamic_cast<NSSpanningTreeNode&>(Rg.IGForwardLinkRef());
    Rst.mNsGeneration = mNsGeneration;
    nodesToBeChecked.push_back(Rg.backIt());

    while (nodesToBeChecked.size() > 0) {

        // Pick an arbitrary node from the list.
        auto nit = *(nodesToBeChecked.rbegin());
        nodesToBeChecked.pop_back();

        auto& Ng  = dynamic_cast<NetworkSimplexNode&>(*(*nit));
                                                        
        auto iPairIn = Ng.incidentEdgesIn();
        for (auto iit = iPairIn.first; iit != iPairIn.second; iit++) {
            auto& Eg =  dynamic_cast<NetworkSimplexEdge&>(*(*(*iit)));
            auto& Ag =  dynamic_cast<NetworkSimplexNode&>(Eg.adjacentNode(Ng));
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                        Ag.IGForwardLinkRef());
            if (Ast.mNsGeneration < mNsGeneration) {
                Ast.mNsGeneration = mNsGeneration;
                createSpanningTreeEdge(Eg);
                nodesToBeChecked.push_front(Ag.backIt());
            }
        }

        auto iPairOut = Ng.incidentEdgesOut();
        for (auto iit = iPairOut.first; iit != iPairOut.second; iit++) {
            auto& Eg =  dynamic_cast<NetworkSimplexEdge&>(*(*(*iit)));
            auto& Ag =  dynamic_cast<NetworkSimplexNode&>(Eg.adjacentNode(Ng));
                                                          
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                        Ag.IGForwardLinkRef());
            if (Ast.mNsGeneration < mNsGeneration) {
                Ast.mNsGeneration = mNsGeneration;
                createSpanningTreeEdge(Eg);
                nodesToBeChecked.push_front(Ag.backIt());
            }
        }
    }

}


void NetworkSimplex::findInitialZeroBX() {

    auto ePair = mG.edges();
    for (auto eit = ePair.first; eit != ePair.second; eit++) {

        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        Eg.mX = 0;
    }

}


void NetworkSimplex::makeAuxGraph()
{

    long perturbation = 0;

    // Save the cost of all the edges and temporarily set it to 0.
    auto ePair = mG.edges();
    for(auto eit = ePair.first; eit != ePair.second; eit++) {

        auto& E = dynamic_cast<NetworkSimplexEdge&>(*(*eit));

        E.mArtificial    = false;
        E.mC             = 0;
        E.mCperturbation = perturbation++;
    }

    // Create an artificial node.
#ifdef UNIT_TESTS
    auto  np = make_unique<NetworkSimplexNode>(-1);
#else
    auto  np = make_unique<NetworkSimplexNode>();
#endif
    auto& Na = dynamic_cast<NetworkSimplexNode&>(mG.addNode(std::move(np)));
                    
    Na.mArtificial    = true;
    Na.mB             = 0;// Artificial node's supply/demand is 0.
    mArtificialNodeIt = Na.backIt();

    // Create artificial edges.
    auto nPair = mG.nodes();
    for(auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        if (Ng.mB != 0) {

            auto  ep = make_unique<NetworkSimplexEdge>();
            auto& Ea = dynamic_cast<NetworkSimplexEdge&>(mG.addEdge(
                             std::move(ep), (Ng.mB>0)?Ng:Na, (Ng.mB>0)?Na:Ng));
            Ea.mArtificial    = true;
            Ea.mC             = 1; // Temporary artificial cost.
            Ea.mCperturbation = perturbation++;
            mArtificialEdges.push_back(Ea.backIt());
        }
    }
}


NSSpanningTreeEdge& NetworkSimplex::createSpanningTreeEdge(
    NetworkSimplexEdge& Eg
) {

    auto& Ngs  = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
    auto& Ngd  = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
    auto& Nsts = dynamic_cast<NSSpanningTreeNode&>(Ngs.IGForwardLinkRef());
    auto& Nstd = dynamic_cast<NSSpanningTreeNode&>(Ngd.IGForwardLinkRef());

    auto  ep  = make_unique<NSSpanningTreeEdge>();
    auto& Est = dynamic_cast<NSSpanningTreeEdge&>(mSpanningTree.addEdge(
                        std::move(ep),
                        Nsts,
                        Nstd,
                        Nsts.incidentEdges().first,
                        Nstd.incidentEdges().first
                ));
    Est.pushIGBackwardLink(Eg.backIt());
    Eg.pushIGForwardLink(Est.backIt());
    Eg.mInBasis = true;
    return Est;
}        


void NetworkSimplex::removeSpanningTreeEdge(NetworkSimplexEdge& Eg)
{
    auto& Est = dynamic_cast<NSSpanningTreeEdge&>(Eg.IGForwardLinkRef());
    Eg.mInBasis = false;
    Eg.popIGForwardLink();
    Est.popIGBackwardLink();
    mSpanningTree.removeEdge(Est);

}


void NetworkSimplex::makeInitialAuxSpanningTree() {

    list<node_list_it_t> nodesToBeChecked;

    mNsGeneration++;

    // Create a copy of each node in mG in the spanning tree. 
    auto nPair = mG.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++) {

        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));

        auto np = make_unique<NSSpanningTreeNode>();
        auto& Nst = dynamic_cast<NSSpanningTreeNode&>(
                                         mSpanningTree.addNode(std::move(np)));
        Nst.pushIGBackwardLink(Ng.backIt());
        Ng.pushIGForwardLink(Nst.backIt());
        
        // If the node's supply/demand is non-zero, it is already reachable
        // from artificial edges. Put them in a queue, from which the 
        // unreachable nodes will be discovered later by BFS on mG.
        if (Ng.mB != 0 ) {
            nodesToBeChecked.push_front(nit);
            Nst.mNsGeneration = mNsGeneration;
        }
        else if (nit == mArtificialNodeIt) {
            Nst.mNsGeneration = mNsGeneration;
        }
    }

    for (auto eit : mArtificialEdges) {
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        createSpanningTreeEdge(Eg);
    }

    while (nodesToBeChecked.size() > 0) {

        // Pick an arbitrary node from the list.
        auto nit = *(nodesToBeChecked.rbegin());
        nodesToBeChecked.pop_back();

        auto& Ng  = dynamic_cast<NetworkSimplexNode&>(*(*nit));
                                                        
        auto iPairIn = Ng.incidentEdgesIn();
        for (auto iit = iPairIn.first; iit != iPairIn.second; iit++) {
            auto& Eg =  dynamic_cast<NetworkSimplexEdge&>(*(*(*iit)));
            auto& Ag =  dynamic_cast<NetworkSimplexNode&>(Eg.adjacentNode(Ng));
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                        Ag.IGForwardLinkRef());
            if (Ast.mNsGeneration < mNsGeneration) {
                Ast.mNsGeneration = mNsGeneration;
                createSpanningTreeEdge(Eg);
                nodesToBeChecked.push_front(Ag.backIt());
            }
        }

        auto iPairOut = Ng.incidentEdgesOut();
        for (auto iit = iPairOut.first; iit != iPairOut.second; iit++) {
            auto& Eg =  dynamic_cast<NetworkSimplexEdge&>(*(*(*iit)));
            auto& Ag =  dynamic_cast<NetworkSimplexNode&>(Eg.adjacentNode(Ng));
                                                          
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                        Ag.IGForwardLinkRef());
            if (Ast.mNsGeneration < mNsGeneration) {
                Ast.mNsGeneration = mNsGeneration;
                createSpanningTreeEdge(Eg);
                nodesToBeChecked.push_front(Ag.backIt());
            }
        }
    }
}


void NetworkSimplex::createInitialInternalStructureForSpanningTree()
{
    auto invalidNodeInST = mSpanningTree.nodes().second;
    auto invalidEdgeInST = mSpanningTree.edges().second;

    // Pick an arbitrary node for the root.
    auto nPair = mG.nodes();
    mRootIt = nPair.second;
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& Ng  = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        auto& Ns  = dynamic_cast<NSSpanningTreeNode&>(Ng.IGForwardLinkRef());
        if (mRootIt == nPair.second && nit != mArtificialNodeIt) {
            mRootIt = nit;
        }
        Ns.mThreadPrev = invalidNodeInST;
        Ns.mThreadNext = invalidNodeInST;
    }

    // From the root, create depth, prec, and thread structures.
    auto& Rg = dynamic_cast<NetworkSimplexNode&>(*(*mRootIt));
    auto& Rs = dynamic_cast<NSSpanningTreeNode&>(Rg.IGForwardLinkRef());

    mNsGeneration++;

    Rs.initForDFS(mNsGeneration, invalidNodeInST);
    Rs.mDepth      = 0;
    Rs.mPrec       = invalidEdgeInST;

    auto nodeBeingVisited = Rs.backIt();
    auto nodePrevThread   = Rs.backIt();

    while(nodeBeingVisited != invalidNodeInST) {

        auto& Ns = dynamic_cast<NSSpanningTreeNode&>(*(*(nodeBeingVisited)));

        if (Ns.hasNextChild()) {

            auto& Es = Ns.nextChild();
            auto& As = dynamic_cast<NSSpanningTreeNode&>(Es.adjacentNode(Ns));
                                                          
            As.mDepth      = Ns.mDepth + 1;
            As.mPrec       = Es.backIt();

            auto& Np = dynamic_cast<NSSpanningTreeNode&>(*(*nodePrevThread));
            Np.mThreadNext = As.backIt();
            As.mThreadPrev = Np.backIt();
            nodePrevThread = As.backIt();

            As.initForDFS(mNsGeneration, Ns.backIt());
            nodeBeingVisited = As.backIt();

        }
        else {

            nodeBeingVisited = Ns.dfsParent();
        }
    }
}


void NetworkSimplex::findInitialY() {
//cerr << "findInitialY() BEGIN\n";
    // From the root, find mY in DFS on the spanning tree.
    auto& Rg  = dynamic_cast<NetworkSimplexNode&>(*(*mRootIt));
    auto& Rst = dynamic_cast<NSSpanningTreeNode&>(Rg.IGForwardLinkRef());
//cerr << "findInitialY() 1\n";
    mNsGeneration++;
    auto invalidNodeInST = mSpanningTree.nodes().second;

    Rst.initForDFS(mNsGeneration, invalidNodeInST);
    Rg.mY = 0;
    node_list_it_t nodeBeingVisited = Rst.backIt();
//cerr << "findInitialY() 2\n";   
    while(nodeBeingVisited != invalidNodeInST) {
//cerr << "findInitialY() 3\n";
        auto& Ns = dynamic_cast<NSSpanningTreeNode&>(*(*(nodeBeingVisited)));
        auto& Ng = dynamic_cast<NetworkSimplexNode&>(Ns.IGBackwardLinkRef());
//cerr << "findInitialY() 4\n";
        if (Ns.hasNextChild()) {
//cerr << "findInitialY() 5\n";
            auto& Es = Ns.nextChild();
            auto& Eg = dynamic_cast<NetworkSimplexEdge&>(
                                                       Es.IGBackwardLinkRef());
            auto& As = dynamic_cast<NSSpanningTreeNode&>(Es.adjacentNode(Ns));
//cerr << "findInitialY() 6\n";
            if (Eg.incidentNodeSrc().backIt() == Ng.backIt()) {
                // Discovered(Ng) ====> New (Nd)
                auto& Nd = dynamic_cast<NetworkSimplexNode&>(
                                                         Eg.incidentNodeDst());
                Nd.mY = Ng.mY + Eg.mC;
            }
            else {
                // New (Ns) ====> Discovered(Ng)
                auto& Ns = dynamic_cast<NetworkSimplexNode&>(
                                                         Eg.incidentNodeSrc());
                Ns.mY = Ng.mY - Eg.mC;
            }
//cerr << "findInitialY() 7\n";
            As.initForDFS(mNsGeneration, Ns.backIt());
            nodeBeingVisited = As.backIt();
//cerr << "findInitialY() 8\n";
        }
        else {
//cerr << "findInitialY() 9\n";
            // No children. Leaving.
            nodeBeingVisited = Ns.dfsParent();
//cerr << "findInitialY() 10\n";
        }
//cerr << "findInitialY() 11\n";
    }
//cerr << "findInitialY() END\n";
}


void NetworkSimplex::findInitialZ() {
//cerr << "findInitialZ() BEGIN\n";
    auto ePair = mG.edges();
    for (auto eit = ePair.first; eit != ePair.second; eit++) {
//cerr << "findInitialZ() 1\n";
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));

        if (Eg.mInBasis) {
//cerr << "findInitialZ() 2\n";
            Eg.mZ = 0;
        }
        else {
            auto& Ns = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
            auto& Nd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());

            Eg.mZ = Eg.mC - Nd.mY + Ns.mY;
//cerr << "findInitialZ() 3: ]" << Eg.mZ << "\n";
            if (Eg.mZ < 0) {
//cerr << "findInitialZ() 4\n";
                Eg.mNZEbackIt = mNegativeZEdges.insert(
                                           mNegativeZEdges.end(), Eg.backIt());
            }
        }
    }
//cerr << "findInitialZ() END\n";
}


void NetworkSimplex::findInitialAuxX() {

    auto ePair = mG.edges();
    for (auto eit = ePair.first; eit != ePair.second; eit++) {

        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        auto& Ns = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
        auto& Nd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());

        if (Eg.mArtificial) {
            if (Ns.backIt() == mArtificialNodeIt) {
                Eg.mX = -1 * Nd.mB;
            }
            else {
                Eg.mX = Ns.mB;
            }
        }
        else {
            Eg.mX = 0;
        }
    }
}


bool NetworkSimplex::pivot() {

    if (mNegativeZEdges.empty()) {
        // There is no edge with negative Z. At an optimum.
        return false;
    }

    // Find a non-basis edge that has negative Z.
    auto& Eenter = dynamic_cast<NetworkSimplexEdge&>(
                                             *(*(*(mNegativeZEdges.begin()))));
#ifdef UNIT_TESTS_DEBUG_PRINT
    {
        auto& Ngs =dynamic_cast<NetworkSimplexNode&>(Eenter.incidentNodeSrc());
        auto& Ngd =dynamic_cast<NetworkSimplexNode&>(Eenter.incidentNodeDst());
        cerr << "Entering: (" << Ngs.mDebugId << "," << Ngd.mDebugId << ")\n";
    }
#endif

    vector<edge_list_it_t> cycle = findCycle(Eenter);

#ifdef UNIT_TESTS_DEBUG_PRINT
    cerr << "Cycle: ";
    for (auto cit : cycle) {
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*cit));
        auto& Ngs = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
        auto& Ngd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
        cerr << "(" << Ngs.mDebugId << "," << Ngd.mDebugId << ") ";
    }
    cerr << "\n";
#endif

    auto& Eleave = findLeavingEdge(Eenter, cycle);

#ifdef UNIT_TESTS_DEBUG_PRINT
    {
        auto& Ngs =dynamic_cast<NetworkSimplexNode&>(Eleave.incidentNodeSrc());
        auto& Ngd =dynamic_cast<NetworkSimplexNode&>(Eleave.incidentNodeDst());
        cerr << "Leaving: (" << Ngs.mDebugId << "," << Ngd.mDebugId << ")\n";
    }
#endif

    vector<node_list_it_t> subtreeNodes = findSubtree(Eleave);

#ifdef UNIT_TESTS_DEBUG_PRINT
    cerr << "Subtree nodes: ";
    for (auto nit : subtreeNodes) {
        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        cerr << Ng.mDebugId << " ";
    }
    cerr << "\n";
#endif

    vector<edge_list_it_t> cut =findCutSet(subtreeNodes);

#ifdef UNIT_TESTS_DEBUG_PRINT
    cerr << "Cut: ";
    for (auto eit : cut) {
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));  
        auto& Ngs = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
        auto& Ngd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
        cerr << "(" << Ngs.mDebugId << "," << Ngd.mDebugId << ") ";
    }
    cerr << "\n";
#endif


    updateX(cycle, Eenter, Eleave);
    updateY(subtreeNodes, Eenter);
    updateZ(cut);
    updateSpanningTree(Eenter, Eleave);


#ifdef UNIT_TESTS_DEBUG_PRINT
    printDebug();
#endif

    return true;

}


vector<edge_list_it_t> NetworkSimplex::findCycle(NetworkSimplexEdge& Eg)
{
    list<edge_list_it_t> edgeListInG1;
    list<edge_list_it_t> edgeListInG2;
    auto& Ngs = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
    auto& Ngd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
    auto NsIt1 = Ngs.IGForwardLink();
    auto NsIt2 = Ngd.IGForwardLink();
    while (NsIt1!= NsIt2) {
        auto& Ns1 = dynamic_cast<NSSpanningTreeNode&>(*(*NsIt1));
        auto& Ns2 = dynamic_cast<NSSpanningTreeNode&>(*(*NsIt2));
        if (Ns1.mDepth == Ns2.mDepth) {
            auto& Es1 = dynamic_cast<NSSpanningTreeEdge&>(*(*(Ns1.mPrec)));
            auto& Es2 = dynamic_cast<NSSpanningTreeEdge&>(*(*(Ns2.mPrec)));
            auto& As1 = dynamic_cast<NSSpanningTreeNode&>(
                                                        Es1.adjacentNode(Ns1));
            auto& As2 = dynamic_cast<NSSpanningTreeNode&>(
                                                        Es2.adjacentNode(Ns2));
            edgeListInG1.push_front(Es1.IGBackwardLink());
            edgeListInG2.push_back(Es2.IGBackwardLink());

            NsIt1 = As1.backIt();
            NsIt2 = As2.backIt();
        }
        else if (Ns1.mDepth > Ns2.mDepth) {
            auto& Es1 = dynamic_cast<NSSpanningTreeEdge&>(*(*(Ns1.mPrec)));
            auto& As1 = dynamic_cast<NSSpanningTreeNode&>(
                                                        Es1.adjacentNode(Ns1));
            edgeListInG1.push_front(Es1.IGBackwardLink());
            NsIt1 = As1.backIt();
        }
        else { //(Ns1.mDepth < Ns2.mDepth)
            auto& Es2 = dynamic_cast<NSSpanningTreeEdge&>(*(*(Ns2.mPrec)));
            auto& As2 = dynamic_cast<NSSpanningTreeNode&>(
                                                        Es2.adjacentNode(Ns2));
            edgeListInG2.push_back(Es2.IGBackwardLink());

            NsIt2 = As2.backIt();
        }        
    }

    vector<edge_list_it_t> combinedListInG;

    for (auto e : edgeListInG2) {
        combinedListInG.push_back(e);
    }    

    for (auto e : edgeListInG1) {
        combinedListInG.push_back(e);
    }    

    return combinedListInG;
}


NetworkSimplexEdge& NetworkSimplex::findLeavingEdge(
    NetworkSimplexEdge&     Eg,
    vector<edge_list_it_t>& cycle
) {
    auto prevIt = Eg.incidentNodeDst().backIt();
    long minX  = 0;
    long pert  = 0;
    auto minIt = mG.edges().second;
    for (auto eit : cycle) {
        auto& Cg  = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        if (Cg.incidentNodeSrc().backIt() == prevIt) {
            prevIt = Cg.incidentNodeDst().backIt();
        }
        else {
            if (minIt == mG.edges().second) {
                minX  = Cg.mX;
                pert  = Cg.mCperturbation;
                minIt = Cg.backIt();
            }
            else if (Cg.mX < minX) {
                minX  = Cg.mX;
                pert  = Cg.mCperturbation;
                minIt = Cg.backIt();
            }
            else if (Cg.mX == minX && Cg.mCperturbation < pert) {

                minX  = Cg.mX;
                pert  = Cg.mCperturbation;
                minIt = Cg.backIt();
            }
            prevIt = Cg.incidentNodeSrc().backIt();
        }
    }

    return dynamic_cast<NetworkSimplexEdge&>(*(*minIt));
}


NSSpanningTreeNode& NetworkSimplex::deeperSpanningTreeNode(
    NetworkSimplexEdge& Eg
) {
    auto& N1g = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNode1());
    auto& N2g = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNode2());
    auto& N1st = dynamic_cast<NSSpanningTreeNode&>(N1g.IGForwardLinkRef());
    auto& N2st = dynamic_cast<NSSpanningTreeNode&>(N2g.IGForwardLinkRef());
    return (N1st.mDepth > N2st.mDepth)?N1st:N2st;
}



vector<node_list_it_t> NetworkSimplex::findSubtree(NetworkSimplexEdge& Eleave)
{

    mNsGeneration++;        

    vector<node_list_it_t> subtreeNodes;

    auto& Rst  = deeperSpanningTreeNode(Eleave);

    auto nit = Rst.backIt();

    while(nit != mSpanningTree.nodes().second) {

        auto& Nst = dynamic_cast<NSSpanningTreeNode&>(*(*nit));
        auto& Ng  = dynamic_cast<NetworkSimplexNode&>(Nst.IGBackwardLinkRef());

        if (Nst.mDepth <= Rst.mDepth && nit != Rst.backIt()) {
            break;
        }
        Nst.mNsGeneration = mNsGeneration;
        subtreeNodes.push_back(Ng.backIt());
        nit = Nst.mThreadNext;
    }
    return subtreeNodes;
}


vector<edge_list_it_t> NetworkSimplex::findCutSet(
    vector<node_list_it_t>& subtreeNodes
) {

    vector<edge_list_it_t> cutSet;

    for (auto nit : subtreeNodes) {

        auto& Ng  = dynamic_cast<NetworkSimplexNode&>(*(*nit));

        auto iPairIn = Ng.incidentEdgesIn();
        for (auto iit = iPairIn.first; iit != iPairIn.second; iit++) {
            auto& IEg = dynamic_cast<NetworkSimplexEdge&>(*(*(*iit)));
            auto& Ag  = dynamic_cast<NetworkSimplexNode&>(
                                                         IEg.adjacentNode(Ng));
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                        Ag.IGForwardLinkRef());
            if (Ast.mNsGeneration < mNsGeneration) {
                cutSet.push_back(*iit);
            }
        }

        auto iPairOut = Ng.incidentEdgesOut();
        for (auto iit = iPairOut.first; iit != iPairOut.second; iit++) {
            auto& IEg = dynamic_cast<NetworkSimplexEdge&>(*(*(*iit)));
            auto& Ag  = dynamic_cast<NetworkSimplexNode&>(
                                                         IEg.adjacentNode(Ng));
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                        Ag.IGForwardLinkRef());
            if (Ast.mNsGeneration < mNsGeneration) {
                cutSet.push_back(*iit);
            }
        }
    }

    return cutSet;
}


void NetworkSimplex::updateX(
    vector<edge_list_it_t>& cycle,
    NetworkSimplexEdge&     Eenter,
    NetworkSimplexEdge&     Eleave
) {

    auto prevIt = Eenter.incidentNodeDst().backIt();
    auto t      = Eleave.mX;

    for (auto eit : cycle) {
        auto& Cg  = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
                                                         
        if (Cg.incidentNodeSrc().backIt() == prevIt) {
            Cg.mX += t;
            prevIt = Cg.incidentNodeDst().backIt();
        }
        else {
            Cg.mX -= t;
            prevIt = Cg.incidentNodeSrc().backIt();
        }
    }
    Eenter.mX = t;
    Eleave.mX = 0;
}


void NetworkSimplex::updateY(
    vector<node_list_it_t>& subtreeNodes,
    NetworkSimplexEdge&     Eenter
) {
    auto& Ngs  = dynamic_cast<NetworkSimplexNode&>(Eenter.incidentNodeSrc());
    auto& Nsts = dynamic_cast<NSSpanningTreeNode&>(Ngs.IGForwardLinkRef());
    auto t = Eenter.mZ;
    for (auto nit : subtreeNodes) {
        auto& N = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        if (Nsts.mNsGeneration < mNsGeneration) {
            // Source node is on the root side.
            N.mY += t;
        }
        else {
            // Destination node is on the root side.
            N.mY -= t;
        }
    }
}


void NetworkSimplex::updateZ(vector<edge_list_it_t>& cut)
{

    for (auto eit : cut) {

        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        auto& Ns = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
        auto& Nd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());

        auto oldZ = Eg.mZ;

        Eg.mZ = Eg.mC - Nd.mY + Ns.mY;

        if (oldZ >= 0 && Eg.mZ < 0) {
            Eg.mNZEbackIt = mNegativeZEdges.insert(
                                          mNegativeZEdges.end(), Eg.backIt());
        }
        else if  (oldZ < 0 && Eg.mZ >= 0) {
            mNegativeZEdges.erase(Eg.mNZEbackIt);
        }
    }
}


NSSpanningTreeNode& NetworkSimplex::rootSideOfEnter(NetworkSimplexEdge& Eg)
{
    auto& Ngs  = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
    auto& Nsts = dynamic_cast<NSSpanningTreeNode&>(Ngs.IGForwardLinkRef());

    auto& Ngd  = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
    auto& Nstd = dynamic_cast<NSSpanningTreeNode&>(Ngd.IGForwardLinkRef());

    return (Nsts.mNsGeneration < mNsGeneration)?Nsts:Nstd;
}


NSSpanningTreeNode& NetworkSimplex::nonRootSideOfEnter(
    NetworkSimplexEdge& Eg
) {
    auto& Ngs  = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
    auto& Nsts = dynamic_cast<NSSpanningTreeNode&>(Ngs.IGForwardLinkRef());

    auto& Ngd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
    auto& Nstd = dynamic_cast<NSSpanningTreeNode&>(Ngd.IGForwardLinkRef());

    return (Nsts.mNsGeneration < mNsGeneration)?Nstd:Nsts;
}


void NetworkSimplex::updateSpanningTree(
    NetworkSimplexEdge& Eenter,
    NetworkSimplexEdge& Eleave
) {

    // Identify the new thread's link points.
    
    auto& Ast  = rootSideOfEnter(Eenter);    // Root side
    auto& Rst  = nonRootSideOfEnter(Eenter); // STnr side
    auto& LDst = deeperSpanningTreeNode(Eleave);

#ifdef UNIT_TESTS_DEBUG_PRINT
    {
        auto& Ag = dynamic_cast<NetworkSimplexNode&>(Ast.IGBackwardLinkRef());
        auto& Rg = dynamic_cast<NetworkSimplexNode&>(Rst.IGBackwardLinkRef());
        auto& LDg= dynamic_cast<NetworkSimplexNode&>(LDst.IGBackwardLinkRef());
        cerr << "Ag:" << Ag.mDebugId << "\n";
        cerr << "Rg:" << Rg.mDebugId << "\n";
        cerr << "LDg:" << LDg.mDebugId << "\n";
    }
#endif

    // New subtree will be inserted between Ast and astNextIt.

    auto ldPrevIt  = LDst.mThreadPrev;


#ifdef UNIT_TESTS_DEBUG_PRINT
    {
        auto& LDstPrev = dynamic_cast<NSSpanningTreeNode&>(*(*(ldPrevIt)));
        auto& LDgPrev  = dynamic_cast<NetworkSimplexNode&>(
                                                 LDstPrev.IGBackwardLinkRef());
        cerr << "LDgPrev:" << LDgPrev.mDebugId << "\n";
    }
#endif

    // From LDst, follow the thread until the node that is shallower or 
    // at the same level as LDst.
    auto invalidNodeInST = mSpanningTree.nodes().second;
    auto ldNextIt = LDst.mThreadNext;
    while (ldNextIt != invalidNodeInST) {

        auto& N = dynamic_cast<NSSpanningTreeNode&>(*(*ldNextIt));
        if (N.mDepth <= LDst.mDepth) {
            break;
        }        

        ldNextIt = N.mThreadNext;
    }

#ifdef UNIT_TESTS_DEBUG_PRINT
    {
        if (ldNextIt == invalidNodeInST) {
            cerr << "LDgNext: (Null)\n";
        }
        else {
            auto& LDstNext = dynamic_cast<NSSpanningTreeNode&>(*(*(ldNextIt)));
            auto& LDgNext  = dynamic_cast<NetworkSimplexNode&>(
                                                 LDstNext.IGBackwardLinkRef());
            cerr << "LDgNext:" << LDgNext.mDebugId << "\n";
        }
    }
#endif
    // Update the thread link on the leaving side
    if (ldPrevIt != invalidNodeInST) {
          auto& LDprev = dynamic_cast<NSSpanningTreeNode&>(*(*ldPrevIt));
          LDprev.mThreadNext = ldNextIt;
    }

    if (ldNextIt != invalidNodeInST) {
          auto& LDnext = dynamic_cast<NSSpanningTreeNode&>(*(*ldNextIt));
          LDnext.mThreadPrev = ldPrevIt;
    }

    // Remove Eleave from the spanning tree.
    removeSpanningTreeEdge(Eleave);

    // DFS into the subtree and update values.
    mNsGeneration++;

    Rst.mDepth = Ast.mDepth + 1;

    Rst.initForDFS(mNsGeneration, invalidNodeInST);

    node_list_it_t nodeBeingVisited = Rst.backIt();
    node_list_it_t nodePrevThread   = Rst.backIt();

    auto astNextItsaved = Ast.mThreadNext;

    Ast.mThreadNext = Rst.backIt();
    Rst.mThreadPrev = Ast.backIt();

    while(nodeBeingVisited != invalidNodeInST) {

        auto& Ns = dynamic_cast<NSSpanningTreeNode&>(*(*(nodeBeingVisited)));

        if (Ns.hasNextChild()) {

            auto& Es = Ns.nextChild();
            auto& As = dynamic_cast<NSSpanningTreeNode&>(Es.adjacentNode(Ns));

            As.mDepth      = Ns.mDepth + 1;
            As.mPrec       = Es.backIt();

            auto& Np = dynamic_cast<NSSpanningTreeNode&>(*(*nodePrevThread));
            Np.mThreadNext = As.backIt();
            As.mThreadPrev = Np.backIt();
            nodePrevThread = As.backIt();

            As.initForDFS(mNsGeneration, Ns.backIt());
            nodeBeingVisited = As.backIt();

        }
        else {

            // No children. Leaving.
            nodeBeingVisited = Ns.dfsParent();

        }

    }

#ifdef UNIT_TESTS_DEBUG_PRINT
    {
        if (astNextItsaved != invalidNodeInST) {
            auto& AstNext =dynamic_cast<NSSpanningTreeNode&>(
                                                        *(*(astNextItsaved)));
            auto& AgNext  =dynamic_cast<NetworkSimplexNode&>(
                                                  AstNext.IGBackwardLinkRef());
            cerr << "AgNext:" << AgNext.mDebugId << "\n";
        }
        else {
            cerr << "AgNext: (Null)\n";
        }
    }
#endif

    if (nodePrevThread != invalidNodeInST) {

        auto& Ns = dynamic_cast<NSSpanningTreeNode&>(*(*nodePrevThread));

        Ns.mThreadNext = astNextItsaved;

    }

    if (astNextItsaved != invalidNodeInST) {

        auto& AstNext = dynamic_cast<NSSpanningTreeNode&>(*(*astNextItsaved));

        AstNext.mThreadPrev = nodePrevThread;

    }

    createSpanningTreeEdge(Eenter);

    Rst.mPrec  = Eenter.IGForwardLink();


}


void NetworkSimplex::restoreOriginalProblem() {

    auto& Ag  = dynamic_cast<NetworkSimplexNode&>(*(*mArtificialNodeIt));
    auto& Ast = dynamic_cast<NSSpanningTreeNode&>(Ag.IGForwardLinkRef());

    mG.removeNode(Ag);
    mSpanningTree.removeNode(Ast);

    auto ePair = mG.edges();
    for (auto eit = ePair.first; eit != ePair.second; eit++) {
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        Eg.mC = Eg.mCreal;
    }

    createInitialInternalStructureForSpanningTree();
}


bool NetworkSimplex::areAllBZero()
{
    auto nPair = mG.nodes();
    for(auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        if (Ng.mB != 0) {
            return false;
        }
    }
    return true;
}


bool NetworkSimplex::solve()
{
    if (mG.numNodes() == 0) {
        return true;
    }

    if (mG.numNodes() == 1) {
        auto& N = dynamic_cast<NetworkSimplexNode&>(*(*(mG.nodes().first)));
        N.mY = 0;
        return true;
    }

    if (mG.numEdges() == mG.numNodes() -1) {
        bool res = makeTrivialSolution();
#ifdef UNIT_TESTS_DEBUG_PRINT
        cerr << "Already a spanning tree Find solution with simultaneous equations.\n";
        printDebug();
#endif
        return res;
    }

    if (areAllBZero()) {
        prepareZeroBGraph();
        makeInitialZeroBSpanningTree();
        createInitialInternalStructureForSpanningTree();
        findInitialZeroBX();
        findInitialY();
        findInitialZ();
#ifdef UNIT_TESTS_DEBUG_PRINT
        cerr << "All zero, before pivotting.\n";
        printDebug();
#endif
        while(pivot()){;}

#ifdef UNIT_TESTS_DEBUG_PRINT
        cerr << "All zero, after pivotting.\n";
        printDebug();
#endif

    }
    else {
        makeAuxGraph();

        makeInitialAuxSpanningTree();

        createInitialInternalStructureForSpanningTree();

        findInitialAuxX();
        findInitialY();
        findInitialZ();
#ifdef UNIT_TESTS_DEBUG_PRINT
        printDebug();
#endif
        while(pivot()){;}
      
        long numArtificialBasis = 0;
        for (auto eit : mArtificialEdges) {
            auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
            if (Eg.mInBasis) {
                if (Eg.mX != 0) {
                    return false;
                }
                else {
                    numArtificialBasis++;
                }
            }
        }
        if (numArtificialBasis != 1) {
            return false;
        }

        restoreOriginalProblem();

        mNegativeZEdges.clear();

        findInitialY();
        findInitialZ();
#ifdef UNIT_TESTS_DEBUG_PRINT
        printDebug();
#endif
        while(pivot()){;}


    }

    return true;
}


bool NetworkSimplex::checkValidity() {

    Undirected::ConnectedDecomposer cd;
    vector<vector<node_list_it_t> > nodeSets;
    vector<vector<edge_list_it_t> > edgeSets;
    cd.decompose(mG, nodeSets, edgeSets);
    if (nodeSets.size() > 1) {
        return false;
    }

    long totalB = 0;
    auto nPair = mG.nodes();
    for (auto nit = nPair.first; nit != nPair.second; nit++ ) {
        auto& N = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        totalB += N.mB;
    }
    if (totalB != 0) {
        return false;
    }

//    auto ePair = mG.edges();
//    for (auto eit = ePair.first; eit != ePair.second; eit++ ) {
//        auto& E = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
//        if (E.mCreal < 0) {
//            return false;
//        }
//    }

    return true;
}


bool NetworkSimplex::makeTrivialSolution() {

    // Find X values with simultaneous equations solver.
    TotUnimodSimulEqsSolver ts(mG.numNodes()-1);

    auto nPair = mG.nodes();
    size_t index = 0;
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));


        if (nit != nPair.first) {      
            ts.setRHS(index, -1 * Ng.mB);
            Ng.pushUtility(index++);
        }
        else {
            Ng.pushUtility(-1);// Dummy value
        }
    }

    auto ePair = mG.edges();
    index = 0;
    for (auto eit = ePair.first; eit != ePair.second; eit++) {

        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        auto& Ns = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
        if (Ns.backIt() != nPair.first) {
            ts.setCoefficient(Ns.utility(), index, -1);
        }

        auto& Nd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
        if (Nd.backIt() != nPair.first) {
            ts.setCoefficient(Nd.utility(), index, 1);
        }
        Eg.pushUtility(index++);

    }

    ts.solve();

    for (auto eit = ePair.first; eit != ePair.second; eit++) {
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        Eg.mX = ts.getSolution(Eg.utility());
        if (Eg.mX < 0) {
            return false;
        }
    }

    mG.popUtilityFromNodes();
    mG.popUtilityFromEdges();

    // Find Y values with DFS
    for (auto nit = nPair.first; nit != nPair.second; nit++) {
        auto& Ng = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        auto np = make_unique<NSSpanningTreeNode>();
        auto& Nst = dynamic_cast<NSSpanningTreeNode&>(
                                         mSpanningTree.addNode(std::move(np)));
        Nst.pushIGBackwardLink(Ng.backIt());
        Ng.pushIGForwardLink(Nst.backIt());

        // Following fields are not used.
        Nst.mThreadNext = mSpanningTree.nodes().second;
        Nst.mThreadPrev = mSpanningTree.nodes().second;
        Nst.mPrec       = mSpanningTree.edges().second;
        Nst.mDepth      = 0;

    }

    for (auto eit = ePair.first; eit != ePair.second; eit++) {
        auto& Eg = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        createSpanningTreeEdge(Eg);
    }

    auto invalidNodeInST = mSpanningTree.nodes().second;

    // From the root, create depth, prec, and thread structures.
    auto& Rg = dynamic_cast<NetworkSimplexNode&>(*(*(mG.nodes().first)));
    auto& Rs = dynamic_cast<NSSpanningTreeNode&>(Rg.IGForwardLinkRef());

    mNsGeneration++;

    Rs.initForDFS(mNsGeneration, invalidNodeInST);
    Rg.mY = 0;

    auto nodeBeingVisited = Rs.backIt();

    while(nodeBeingVisited != invalidNodeInST) {

        auto& Ns = dynamic_cast<NSSpanningTreeNode&>(*(*(nodeBeingVisited)));
        auto& Ng = dynamic_cast<NetworkSimplexNode&>(Ns.IGBackwardLinkRef());
                                                      
        if (Ns.hasNextChild()) {

            auto& Es = Ns.nextChild();
            auto& Eg = dynamic_cast<NetworkSimplexEdge&>(
                                                       Es.IGBackwardLinkRef());
            auto& As = dynamic_cast<NSSpanningTreeNode&>(Es.adjacentNode(Ns));
                                                          
            if (Eg.incidentNodeSrc().backIt() == Ng.backIt()) {
                // Discovered(Ng) ====> New (Nd)
                auto& Nd = dynamic_cast<NetworkSimplexNode&>(
                                                         Eg.incidentNodeDst());
                Nd.mY = Ng.mY + Eg.mCreal;
            }
            else {
                // New (Ns) ====> Discovered(Ng)
                auto& Ns = dynamic_cast<NetworkSimplexNode&>(
                                                         Eg.incidentNodeSrc());
                Ns.mY = Ng.mY - Eg.mCreal;
            }
            As.initForDFS(mNsGeneration, Ns.backIt());
            nodeBeingVisited = As.backIt();
        }
        else {
            // No children. Leaving.
            nodeBeingVisited = Ns.dfsParent();
        }

    }
    return true;
}

#ifdef UNIT_TESTS
void NetworkSimplex::printDebug()
{
    cerr << "ID\t Type\tB\tY\tGen\tDepth\tPrec\tPrev\tNext\n";
    for (auto nit = mG.nodes().first; nit != mG.nodes().second; nit++) {
        auto& N = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        printDebugNode(N);
    }
    cerr << "\n\n";
    cerr << "(ID,ID)\tmC\tmCreal\tmCpert\tmX\tmZ\tType\n";
    for (auto eit = mG.edges().first; eit != mG.edges().second; eit++) {
        auto& E = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        printDebugEdge(E);
    }
    cerr << "\n\n";

    cerr << "NegativeZEdges: ";
    for (auto eit : mNegativeZEdges) {
        auto& Eg  = dynamic_cast<NetworkSimplexEdge&>(*(*eit));
        auto& Ngs = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
        auto& Ngd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());
        cerr << "(" << Ngs.mDebugId << "," << Ngd.mDebugId << ") ";
    }
    cerr << "\n";
}


void NetworkSimplex::printDebugEdge(NetworkSimplexEdge& Eg)
{
    auto& Ngs = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeSrc());
    auto& Ngd = dynamic_cast<NetworkSimplexNode&>(Eg.incidentNodeDst());

    cerr << "(" << Ngs.mDebugId << "," << Ngd.mDebugId << ")\t";
    cerr << Eg.mC << "\t";
    cerr << Eg.mCreal << "\t";
    cerr << Eg.mCperturbation << "\t";
    cerr << Eg.mX << "\t";
    cerr << Eg.mZ << "\t";
    if (Eg.mInBasis) {
        cerr << "B";
    }
    if (Eg.mArtificial) {
        cerr << "A";
    }
    cerr << "\n";
}


void NetworkSimplex::printDebugNode(NetworkSimplexNode& Ng)
{
    if (Ng.IGForwardLinksSize()==1) {

        auto& Nst = dynamic_cast<NSSpanningTreeNode&>(Ng.IGForwardLinkRef());
        cerr << Ng.mDebugId << "\t";
        if (mRootIt == Ng.backIt()) {
            cerr << "R";
        }
        if (mArtificialNodeIt == Ng.backIt()) {
            cerr << "A";
        }
        cerr << "\t";
        cerr << Ng.mB << "\t";
        cerr << Ng.mY << "\t";
        cerr << Nst.mNsGeneration << "\t";
        cerr << Nst.mDepth << "\t";
        if (Nst.mPrec != mSpanningTree.edges().second){
            auto& Est = dynamic_cast<NSSpanningTreeEdge&>(*(*(Nst.mPrec)));
            auto& Ast = dynamic_cast<NSSpanningTreeNode&>(
                                                       Est.adjacentNode(Nst));
            auto& Ag  = dynamic_cast<NetworkSimplexNode&>(
                                                      Ast.IGBackwardLinkRef());
            cerr << Ag.mDebugId << "\t";
        }
        else {
            cerr << "N/A\t";
        }
        if (Nst.mThreadPrev != mSpanningTree.nodes().second) {
            auto& Pst = dynamic_cast<NSSpanningTreeNode&>(
                                                        *(*(Nst.mThreadPrev)));
            auto& Pg  = dynamic_cast<NetworkSimplexNode&>(
                                                      Pst.IGBackwardLinkRef());
            cerr << Pg.mDebugId << "\t";
        }
        else {
            cerr << "N/A\t";
        }

        if (Nst.mThreadNext != mSpanningTree.nodes().second) {
            auto& Pst = dynamic_cast<NSSpanningTreeNode&>(
                                                        *(*(Nst.mThreadNext)));
            auto& Pg  = dynamic_cast<NetworkSimplexNode&>(
                                                      Pst.IGBackwardLinkRef());
            cerr << Pg.mDebugId << "\n";
        }
        else {
            cerr << "N/A\n";
        }
    }
    else {

        cerr << Ng.mDebugId << "\t";
        if (mRootIt == Ng.backIt()) {
            cerr << "R";
        }
        if (mArtificialNodeIt == Ng.backIt()) {
            cerr << "A";
        }
        cerr << "\t";
        cerr << Ng.mB << "\t";
        cerr << Ng.mY << "\t";
        cerr << "N/A\t";
        cerr << "N/A\t";
        cerr << "N/A\t";
        cerr << "N/A\t";
        cerr << "N/A\n";
    }
}
#endif

}// namespace Directed

}// namespace Wailea

