#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/spqr_tree.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


/**
 * @file undirected/spqr_tree.cpp
 *
 * @brief
 *     
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

const utility_t SPQRTree::kForwardEdgeTypeSeparatingEdge = 0;
const utility_t SPQRTree::kForwardEdgeTypeOrdinaryEdge   = 1;

vector<node_list_it_t> SPQRTree::RTypeNodes()
{
    vector<node_list_it_t> sameTypeList;

    for (auto nit  = nodes().first; nit != nodes().second; nit++ ) {
        SPQRTreeNode& n = dynamic_cast<SPQRTreeNode&>(*(*nit));
        if (n.type() == SPQRTreeNode::RType) {
            sameTypeList.push_back(nit);
        }
    }
    return sameTypeList; // rvo
}


vector<node_list_it_t> SPQRTree::STypeNodes()
{
    vector<node_list_it_t> sameTypeList;

    for (auto nit  = nodes().first; nit != nodes().second; nit++ ) {
        SPQRTreeNode& n = dynamic_cast<SPQRTreeNode&>(*(*nit));
        if (n.type() == SPQRTreeNode::SType) {
            sameTypeList.push_back(nit);
        }
    }
    return sameTypeList; // rvo
}


vector<node_list_it_t> SPQRTree::PTypeNodes()
{
    vector<node_list_it_t> sameTypeList;

    for (auto nit  = nodes().first; nit != nodes().second; nit++ ) {
        SPQRTreeNode& n = dynamic_cast<SPQRTreeNode&>(*(*nit));
        if (n.type() == SPQRTreeNode::PType) {
            sameTypeList.push_back(nit);
        }
    }
    return sameTypeList; // rvo
}


void SPQRTree::determineTypes()
{
    for (auto nit  = nodes().first; nit != nodes().second; nit++ ) {
        auto& n = dynamic_cast<SPQRTreeNode&>(*(*nit));
        auto& c = n.component();
        if (c.numNodes() == 2) {
            n.setType(SPQRTreeNode::PType);
        }
        else {
            bool foundR = false;
            for (auto cnit = c.nodes().first; cnit != c.nodes().second;
                                                                 cnit++ ) {
                auto& cn = *(*cnit);
                if (cn.degree() != 2) {
                    foundR = true;
                    break;
                }
            }
            if (foundR) {
                n.setType(SPQRTreeNode::RType);
            }
            else {
                n.setType(SPQRTreeNode::SType);
            }
        }
    }
}


void SPQRTree::mergeTwoNodesS(
                   edge_list_it_t treeEdgeIt, node_list_it_t nodeToBeRemovedIt)
{
    auto& treeEdge = dynamic_cast<SPQRTreeEdge&>(*(*treeEdgeIt));
    auto& n1  = dynamic_cast<SPQRTreeNode&>(treeEdge.incidentNode1());
    auto& n2  = dynamic_cast<SPQRTreeNode&>(treeEdge.incidentNode2());
    auto& c1  = n1.component();
    auto& c2  = n2.component();

    node_list_it_t nodeRemovedIt;
    node_list_it_t nodeExpandedIt;
    edge_list_it_t virtualEdgeRIt;
    edge_list_it_t virtualEdgeEIt;
    if (nodeToBeRemovedIt == n1.backIt()) {

        nodeRemovedIt  = n1.backIt();
        nodeExpandedIt = n2.backIt();
        virtualEdgeRIt = (*treeEdge.virtualEdge1())->backIt();
        virtualEdgeEIt = (*treeEdge.virtualEdge2())->backIt();

    }
    else {

        nodeRemovedIt  = n2.backIt();
        nodeExpandedIt = n1.backIt();
        virtualEdgeRIt = (*treeEdge.virtualEdge2())->backIt();
        virtualEdgeEIt = (*treeEdge.virtualEdge1())->backIt();

    }

    auto& nodeRemoved  = dynamic_cast<SPQRTreeNode&>(*(*nodeRemovedIt));
    auto& nodeExpanded = dynamic_cast<SPQRTreeNode&>(*(*nodeExpandedIt));
    auto& componentRemoved  = nodeRemoved.component();
    auto& componentExpanded = nodeExpanded.component();
    auto& virtualEdgeR = dynamic_cast<SPQRComponentEdge&>(*(*virtualEdgeRIt));
    auto& virtualEdgeE = dynamic_cast<SPQRComponentEdge&>(*(*virtualEdgeEIt));

    removeEdge(treeEdge);

    auto& node1R = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeR.incidentNode1());
    auto& node2R = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeR.incidentNode2());
    auto& node1Eprep = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeE.incidentNode1());
    auto& node2Eprep = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeE.incidentNode2());
    node_list_it_t node1EIt;
    node_list_it_t node2EIt;

    if (node1R.IGBackwardLink() == node1Eprep.IGBackwardLink()) {
        node1EIt = node1Eprep.backIt();
        node2EIt = node2Eprep.backIt();
    }
    else {
        node1EIt = node2Eprep.backIt();
        node2EIt = node1Eprep.backIt();
    }

    auto& node1E = dynamic_cast<SPQRComponentNode&>(*(*node1EIt));
    auto& node2E = dynamic_cast<SPQRComponentNode&>(*(*node2EIt));
                                                         
    c1.removeEdge(virtualEdgeR);
    c2.removeEdge(virtualEdgeE);

    vector<edge_list_it_t> edgesMerged;
    for (auto eit = componentRemoved.edges().first;
              eit != componentRemoved.edges().second; eit++) {
        edgesMerged.push_back(eit);
    }                       

    componentRemoved.moveEdgeInducedSubgraph(edgesMerged, componentExpanded);

    // Find the two end edges in (former) compRemoved.
    auto& edge1R = dynamic_cast<SPQRComponentEdge&>(
                                       *(*(*(node1R.incidentEdges().first))));
    auto& edge2R = dynamic_cast<SPQRComponentEdge&>(
                                       *(*(*(node2R.incidentEdges().first))));

    auto& edge1E = dynamic_cast<SPQRComponentEdge&>(
                                       *(*(*(node1E.incidentEdges().first))));
    auto& edge2E = dynamic_cast<SPQRComponentEdge&>(
                                       *(*(*(node2E.incidentEdges().first))));

    auto& theOtherNodeEdge1R = dynamic_cast<SPQRComponentNode&>(
                                                  edge1R.adjacentNode(node1R));

    auto& theOtherNodeEdge2R = dynamic_cast<SPQRComponentNode&>(
                                                  edge2R.adjacentNode(node2R));

    // Link the end edges of the c1 path to the nodes in c2.
    componentExpanded.moveEdge(edge1R, node1E, theOtherNodeEdge1R);
    componentExpanded.moveEdge(edge2R, node2E, theOtherNodeEdge2R);

    c2.removeNode(node1R);
    c2.removeNode(node2R);

    // Relink the virtual edges that were in componentRemoved.
    auto prevEit = edge1E.backIt();
    auto thisEit = edge1R.backIt();
    while (thisEit != edge2E.backIt()) {

        auto& edgeThisSide = dynamic_cast<SPQRComponentEdge&>(*(*thisEit));
        if (edgeThisSide.type() == SPQRComponentEdge::VirtualType) {
            auto& edgeThatSide     = edgeThisSide.pairVirtualEdge();
            edgeThatSide.setVirtualPair(nodeExpanded.backIt(),
                                                       edgeThisSide.backIt());
            relinkTreeEdge(edgeThisSide.treeEdge(), 
                                   nodeRemoved, nodeExpanded, edgeThisSide);
        }


        auto newEit = advanceEdgeItOnPath(prevEit, thisEit);
        prevEit = thisEit;
        thisEit = newEit;
    }

    removeNode(nodeRemoved);

    return;

}


void SPQRTree::relinkTreeEdge(SPQRTreeEdge& e, SPQRTreeNode& nodeFrom,
                     SPQRTreeNode& nodeTo, SPQRComponentEdge& newVirtualEdge)
{
    auto& n1 = dynamic_cast<SPQRTreeNode&>(e.incidentNode1());
    auto& n2 = dynamic_cast<SPQRTreeNode&>(e.incidentNode2());
    if (n1.backIt() == nodeFrom.backIt()) {
        moveEdge(e, nodeTo, n2);
        e.setVirtualEdge1(newVirtualEdge.backIt());
    }
    else if (n2.backIt() == nodeFrom.backIt()) {
        moveEdge(e, n1, nodeTo);
        e.setVirtualEdge2(newVirtualEdge.backIt());
    }
}


edge_list_it_t SPQRTree::advanceEdgeItOnPath(
                               edge_list_it_t prevEit, edge_list_it_t thisEit)
{
    auto& n1 = (*(*thisEit)).incidentNode1();
    auto& n2 = (*(*thisEit)).incidentNode2();
    auto n1eit = n1.incidentEdges().first;
    if (*n1eit == thisEit) {
        n1eit++;
    }
    auto& e1 = *(*(*n1eit));

    auto n2eit = n2.incidentEdges().first;
    if (*n2eit == thisEit) {
        n2eit++;
    }
    auto& e2 = *(*(*n2eit));

    if (e1.backIt() == prevEit) {
        return e2.backIt();
    }
    else {
        return e1.backIt();
    }
}


void SPQRTree::mergeTwoNodesP(
                   edge_list_it_t treeEdgeIt, node_list_it_t nodeToBeRemovedIt)
{

    auto& treeEdge = dynamic_cast<SPQRTreeEdge&>(*(*treeEdgeIt));
    auto& n1  = dynamic_cast<SPQRTreeNode&>(treeEdge.incidentNode1());
    auto& n2  = dynamic_cast<SPQRTreeNode&>(treeEdge.incidentNode2());

    node_list_it_t nodeRemovedIt;
    node_list_it_t nodeExpandedIt;
    edge_list_it_t virtualEdgeRIt;
    edge_list_it_t virtualEdgeEIt;

    if (nodeToBeRemovedIt == n1.backIt()) {

        nodeRemovedIt  = n1.backIt();
        nodeExpandedIt = n2.backIt();
        virtualEdgeRIt = (*treeEdge.virtualEdge1())->backIt();
        virtualEdgeEIt = (*treeEdge.virtualEdge2())->backIt();

    }
    else {

        nodeRemovedIt  = n2.backIt();
        nodeExpandedIt = n1.backIt();
        virtualEdgeRIt = (*treeEdge.virtualEdge2())->backIt();
        virtualEdgeEIt = (*treeEdge.virtualEdge1())->backIt();

    }

    auto& nodeRemoved  = dynamic_cast<SPQRTreeNode&>(*(*nodeRemovedIt));
    auto& nodeExpanded = dynamic_cast<SPQRTreeNode&>(*(*nodeExpandedIt));
    auto& componentRemoved  = nodeRemoved.component();
    auto& componentExpanded = nodeExpanded.component();
    auto& virtualEdgeR = dynamic_cast<SPQRComponentEdge&>(*(*virtualEdgeRIt));
    auto& virtualEdgeE = dynamic_cast<SPQRComponentEdge&>(*(*virtualEdgeEIt));

    removeEdge(treeEdge);

    auto& node1R = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeR.incidentNode1());

    auto& node2R = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeR.incidentNode2());

    auto& node1Eprep = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeE.incidentNode1());

    auto& node2Eprep = dynamic_cast<SPQRComponentNode&>(
                                                virtualEdgeE.incidentNode2());

    node_list_it_t node1EIt;
    node_list_it_t node2EIt;

    if (node1R.IGBackwardLink() == node1Eprep.IGBackwardLink()) {

        node1EIt = node1Eprep.backIt();
        node2EIt = node2Eprep.backIt();
    }
    else {

        node1EIt = node2Eprep.backIt();
        node2EIt = node1Eprep.backIt();
    }

    auto& node1E = dynamic_cast<SPQRComponentNode&>(*(*node1EIt));
    auto& node2E = dynamic_cast<SPQRComponentNode&>(*(*node2EIt));

    componentRemoved.removeEdge(virtualEdgeR);

    componentExpanded.removeEdge(virtualEdgeE);

    vector<edge_list_it_t> edgesMerged;
    for (auto eit = componentRemoved.edges().first;
              eit != componentRemoved.edges().second; eit++) {

        edgesMerged.push_back(eit);

    }                       

    componentRemoved.moveEdgeInducedSubgraph(edgesMerged, componentExpanded);

    node_incidence_t node1RincidentEdgesCopy;
    for (auto eit = node1R.incidentEdges().first;
              eit != node1R.incidentEdges().second; eit++) {

        node1RincidentEdgesCopy.push_back(*eit);
        
    }

    for (auto eit :node1RincidentEdgesCopy) {

        auto& movedEdge = dynamic_cast<SPQRComponentEdge&>(*(*eit));

        auto& nodeA = movedEdge.incidentNode1();

        if (&nodeA == &node1R) {

            componentExpanded.moveEdge(movedEdge, node1E, node2E);

        }
        else {

            componentExpanded.moveEdge(movedEdge, node2E, node1E);

        }

        if (movedEdge.type() == SPQRComponentEdge::VirtualType) {

            auto& edgeThatSide     = movedEdge.pairVirtualEdge();

            edgeThatSide.setVirtualPair(nodeExpanded.backIt(),
                                                       movedEdge.backIt());

            relinkTreeEdge(movedEdge.treeEdge(), 
                                      nodeRemoved, nodeExpanded, movedEdge);
        }

    }

    componentExpanded.removeNode(node1R);

    componentExpanded.removeNode(node2R);

    removeNode(nodeRemoved);

    return;
}


/** @brief creates the final remaining component and a tree node.
 *  
 *  @param G                  (in): the graph being explored. it contains
 *                                  SPQR_DFS_Node and SPQR_DFS_Edge as its
 *                                  nodes and edges.
 *                           
 *  @param edgesInGToBeCopied (in): list of edges in G to be copied to the
 *                                  split component.
 */
void SPQRTree::makeFinalTreeNodeAndComponent(
    Graph&                  G,
    vector<edge_list_it_t>& edgesInGToBeCopied
){
    vector<node_list_it_t> nodesInGToBeCopied
                                     = G.edgeInducedNodes(edgesInGToBeCopied);

    auto  treeNode1Ptr = make_unique<SPQRTreeNode>();
    auto& treeNode1 =
            dynamic_cast<SPQRTreeNode&>(addNode(move(treeNode1Ptr)));
    auto& comp1        = treeNode1.component();
    treeNode1.linkComponent();

    vector<pair<node_list_it_t, node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t, edge_ptr_t>> edgePairs;

    for (auto nit : nodesInGToBeCopied) {

        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*nit));
        auto newNodePtr = make_unique<SPQRComponentNode>(n.IGBackwardLink());
        nodePairs.push_back(make_pair(nit,move(newNodePtr)));

    }

    for (auto eit : edgesInGToBeCopied) {
        SPQR_DFS_Edge& e = dynamic_cast<SPQR_DFS_Edge&>(*(*eit));

        if (e.mIsVirtual) {
            edgePairs.push_back(make_pair(eit,
                                          make_unique<SPQRComponentEdge>(
                                              e.mVirtualPairEdge, 
                                              e.mVirtualPairTreeNode     )));
        }
        else {
            edgePairs.push_back(make_pair(
                      eit,make_unique<SPQRComponentEdge>(e.IGBackwardLink())));
        }

    }

    G.copySubgraph(nodePairs, edgePairs, comp1);

    patchUpVirtualEdges(treeNode1);

}


/** @brief creates one or two SPQR tree node(s) and its (their)
 *         component(s).
 *
 *  @param G                  (in): the graph being explored. it contains
 *                                  SPQR_DFS_Node and SPQR_DFS_Edge as its
 *                                  nodes and edges.
 *
 *  @param edgesInGToBeCopied (in): list of edges in G to be copied to the
 *                                  split component.
 *
 *  @param nodeAInG           (in): split node A in G.
 *
 *  @param nodeBInG           (in): split node B in G.
 *
 *  @param hasEdgeABInG       (in): true if edgeABInG below specifies an
 *                                  an edge.
 *
 *  @param edgeABInG          (in): edge AB in G if it exists.
 *                                  It maybe either real edge or a virtual
 *                                  edge.
 *
 *  @param hasVirtualEdgeABInTree (in): true if virtualEdgeABInTree below
 *                                  specifies an edge.
 *
 *  @param virtualEdgeABInTree(in): existing virtual edge AB in a tree node
 *
 *  @param treeNodeOfVirtualEdgeAB (in): the tree node in which the edge
 *                                  virtualEdgeABINTree exists.    
 *
 *  @param treeNodeToBeLinkedToG (in): The SPQR tree node whose component
 *                                  has the virtual edge to be linked to
 *                                  the one in G.
 *
 *  @param virtualEdgeToBeLinkedToG (in): the virtual edge in the SPQR tree
 *                                  node, which is to be paired to the one
 *                                  in G.
 */
void SPQRTree::makeNewNodesEdgesAndComponents(
    Graph&                  G,
    vector<edge_list_it_t>& edgesInGToBeCopied,
    node_list_it_t          nodeAInG,
    node_list_it_t          nodeBInG,
    bool                    hasEdgeABInG,
    edge_list_it_t          edgeABInGIt,
    bool                    hasVirtualEdgeABInComp3,
    edge_list_it_t          virtualEdgeABInComp3It,
    node_list_it_t          treeNodeComp3,
    node_list_it_t&         treeNodeToBeLinkedToG,
    edge_list_it_t&         virtualEdgeToBeLinkedToG
){
    vector<node_list_it_t> nodesInGToBeCopied
                                     = G.edgeInducedNodes(edgesInGToBeCopied);
    auto  treeNode1Ptr = make_unique<SPQRTreeNode>();
    auto& treeNode1 =
            dynamic_cast<SPQRTreeNode&>(addNode(move(treeNode1Ptr)));
    auto& comp1        = treeNode1.component();
    treeNode1.linkComponent();

    node_list_it_t nodeAInComp1It;
    node_list_it_t nodeBInComp1It;
    fillInComponent(G, comp1, nodesInGToBeCopied, edgesInGToBeCopied, 
                           nodeAInG, nodeBInG, nodeAInComp1It, nodeBInComp1It);
    patchUpVirtualEdges(treeNode1);
    auto  virtualEdgeABInComp1Ptr
                           = make_unique<SPQRComponentEdge>(G.edges().second);
    auto& virtualEdgeABInComp1 = dynamic_cast<SPQRComponentEdge&>(
                            comp1.addEdge( move(virtualEdgeABInComp1Ptr),
                            *(*nodeAInComp1It),
                            *(*nodeBInComp1It)                              ));
    if (hasVirtualEdgeABInComp3) {

        auto  treeNode2Ptr = make_unique<SPQRTreeNode>();
        auto& treeNode2 =
                dynamic_cast<SPQRTreeNode&>(addNode(move(treeNode2Ptr)));
        auto& comp2        = treeNode2.component();
        treeNode2.linkComponent();

        auto& virtualEdgeABInComp3 = dynamic_cast<SPQRComponentEdge&>(
                                                  *(*virtualEdgeABInComp3It));

        vector<edge_list_it_t> edgesInGextra;
        vector<node_list_it_t> nodesInGextra;
        nodesInGextra.push_back(nodeAInG);
        nodesInGextra.push_back(nodeBInG);

        node_list_it_t nodeAInComp2It;
        node_list_it_t nodeBInComp2It;
        fillInComponent(G, comp2, nodesInGextra, edgesInGextra, 
                           nodeAInG, nodeBInG, nodeAInComp2It, nodeBInComp2It);

        auto  virtualEdge1InComp2Ptr
                           = make_unique<SPQRComponentEdge>(G.edges().second);
        auto& virtualEdge1InComp2 = dynamic_cast<SPQRComponentEdge&>(
                            comp2.addEdge( move(virtualEdge1InComp2Ptr),
                            *(*nodeAInComp2It),
                            *(*nodeBInComp2It)                              ));

        auto  virtualEdge2InComp2Ptr
                           = make_unique<SPQRComponentEdge>(G.edges().second);
        auto& virtualEdge2InComp2 = dynamic_cast<SPQRComponentEdge&>(
                            comp2.addEdge( move(virtualEdge2InComp2Ptr),
                            *(*nodeAInComp2It),
                            *(*nodeBInComp2It)                              ));

        auto  virtualEdge3InComp2Ptr
                           = make_unique<SPQRComponentEdge>(G.edges().second);
        auto& virtualEdge3InComp2 = dynamic_cast<SPQRComponentEdge&>(
                            comp2.addEdge( move(virtualEdge3InComp2Ptr),
                            *(*nodeAInComp2It),
                            *(*nodeBInComp2It)                              ));

        virtualEdgeABInComp1.setVirtualPair(
                            treeNode2.backIt(), virtualEdge1InComp2.backIt());

        virtualEdge1InComp2.setVirtualPair(
                            treeNode1.backIt(), virtualEdgeABInComp1.backIt());

        auto newTreeEdge1Ptr = make_unique<SPQRTreeEdge>(
                  virtualEdgeABInComp1.backIt(), virtualEdge1InComp2.backIt());
        auto& newTreeEdge1 =
                          addEdge(move(newTreeEdge1Ptr), treeNode1, treeNode2);
            
        virtualEdgeABInComp1.setTreeEdge(newTreeEdge1.backIt());
        virtualEdge1InComp2.setTreeEdge(newTreeEdge1.backIt());

        virtualEdgeABInComp3.setVirtualPair(
                            treeNode2.backIt(), virtualEdge2InComp2.backIt());

        virtualEdge2InComp2.setVirtualPair(
                            treeNodeComp3, virtualEdgeABInComp3.backIt());

        auto newTreeEdge2Ptr = make_unique<SPQRTreeEdge>(
                  virtualEdge2InComp2.backIt(), virtualEdgeABInComp3.backIt());
        auto& newTreeEdge2 =
                addEdge(move(newTreeEdge2Ptr), treeNode2, *(*treeNodeComp3));
            
        virtualEdge2InComp2.setTreeEdge(newTreeEdge2.backIt());
        virtualEdgeABInComp3.setTreeEdge(newTreeEdge2.backIt());

        treeNodeToBeLinkedToG = treeNode2.backIt();
        virtualEdgeToBeLinkedToG = virtualEdge3InComp2.backIt();
        virtualEdge3InComp2.setType(SPQRComponentEdge::VirtualType);
    }
    else if (hasEdgeABInG) {

        auto  treeNode2Ptr = make_unique<SPQRTreeNode>();
        auto& treeNode2 =
                dynamic_cast<SPQRTreeNode&>(addNode(move(treeNode2Ptr)));
        auto& comp2        = treeNode2.component();
        treeNode2.linkComponent();
        vector<edge_list_it_t> edgesInGextra;
        vector<node_list_it_t> nodesInGextra;
        edgesInGextra.push_back(edgeABInGIt);
        nodesInGextra.push_back(nodeAInG);
        nodesInGextra.push_back(nodeBInG);

        node_list_it_t nodeAInComp2It;
        node_list_it_t nodeBInComp2It;
        fillInComponent(G, comp2, nodesInGextra, edgesInGextra, 
                           nodeAInG, nodeBInG, nodeAInComp2It, nodeBInComp2It);
        patchUpVirtualEdges(treeNode2);

        auto  virtualEdge1InComp2Ptr
                           = make_unique<SPQRComponentEdge>(G.edges().second);
        auto& virtualEdge1InComp2 = dynamic_cast<SPQRComponentEdge&>(
                            comp2.addEdge( move(virtualEdge1InComp2Ptr),
                            *(*nodeAInComp2It),
                            *(*nodeBInComp2It)                              ));

        auto  virtualEdge2InComp2Ptr
                           = make_unique<SPQRComponentEdge>(G.edges().second);
        auto& virtualEdge2InComp2 = dynamic_cast<SPQRComponentEdge&>(
                            comp2.addEdge( move(virtualEdge2InComp2Ptr),
                            *(*nodeAInComp2It),
                            *(*nodeBInComp2It)                              ));
        virtualEdgeABInComp1.setVirtualPair(
                             treeNode2.backIt(), virtualEdge1InComp2.backIt());

        virtualEdge1InComp2.setVirtualPair(
                            treeNode1.backIt(), virtualEdgeABInComp1.backIt());

        auto newTreeEdgePtr = make_unique<SPQRTreeEdge>(
                  virtualEdgeABInComp1.backIt(), virtualEdge1InComp2.backIt());
        auto& newTreeEdge =
                 addEdge(move(newTreeEdgePtr), treeNode1, treeNode2);
            
        virtualEdgeABInComp1.setTreeEdge(newTreeEdge.backIt());
        virtualEdge1InComp2.setTreeEdge(newTreeEdge.backIt());

        treeNodeToBeLinkedToG = treeNode2.backIt();
        virtualEdgeToBeLinkedToG = virtualEdge2InComp2.backIt();
        virtualEdge2InComp2.setType(SPQRComponentEdge::VirtualType);

    }
    else {

        treeNodeToBeLinkedToG = treeNode1.backIt();
        virtualEdgeToBeLinkedToG = virtualEdgeABInComp1.backIt();
        virtualEdgeABInComp1.setType(SPQRComponentEdge::VirtualType);
    }

}


void SPQRTree::fillInComponent(
    Graph&                  G, 
    Graph&                  comp, 
    vector<node_list_it_t>& nodesInGToBeCopied, 
    vector<edge_list_it_t>& edgesInGToBeCopied,
    node_list_it_t          nodeAInGIt,
    node_list_it_t          nodeBInGIt,
    node_list_it_t&         nodeAInComp1It,
    node_list_it_t&         nodeBInComp1It )
{
    vector<pair<node_list_it_t, node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t, edge_ptr_t>> edgePairs;

    for (auto nit : nodesInGToBeCopied) {

        SPQR_DFS_Node& n = dynamic_cast<SPQR_DFS_Node&>(*(*nit));
        auto newNodePtr = make_unique<SPQRComponentNode>(n.IGBackwardLink());
        nodePairs.push_back(make_pair(nit,move(newNodePtr)));

    }

    for (auto eit : edgesInGToBeCopied) {

        SPQR_DFS_Edge& e = dynamic_cast<SPQR_DFS_Edge&>(*(*eit));

        if (e.mIsVirtual) {
            edgePairs.push_back(make_pair(eit,
                                          make_unique<SPQRComponentEdge>(
                                              e.mVirtualPairEdge, 
                                              e.mVirtualPairTreeNode     )));
        }
        else {
            edgePairs.push_back(make_pair(
                            eit,make_unique<SPQRComponentEdge>(e.IGBackwardLink())));
        }

    }

    SPQR_DFS_Node& nodeAInG = dynamic_cast<SPQR_DFS_Node&>(*(*nodeAInGIt));
    SPQR_DFS_Node& nodeBInG = dynamic_cast<SPQR_DFS_Node&>(*(*nodeBInGIt));

    G.copySubgraph(nodePairs, edgePairs, comp);

    for (auto nit = comp.nodes().first ; nit != comp.nodes().second; nit++) {

        SPQRComponentNode& n = dynamic_cast<SPQRComponentNode&>(*(*nit));

        if (n.IGBackwardLink() == nodeAInG.IGBackwardLink()) {
            nodeAInComp1It = n.backIt();
        }
        if (n.IGBackwardLink() == nodeBInG.IGBackwardLink()) {
            nodeBInComp1It = n.backIt();
        }
    }    
}


/** @brief takes a tree component, scan the component edges and find
 *         unlinked virtual edges, then link them to the pairs and 
 *         add tree edges to SPQRTree.
 *  @param   tnThisSide (in): tree node newly created.
 */
void SPQRTree::patchUpVirtualEdges(SPQRTreeNode& tnThisSide) {

    Graph& compThisSide = tnThisSide.component();

    for (auto eit = compThisSide.edges().first ;
                               eit != compThisSide.edges().second; eit++) {
        SPQRComponentEdge& eThisSide 
                                  = dynamic_cast<SPQRComponentEdge&>(*(*eit));
        if (eThisSide.type() == SPQRComponentEdge::VirtualType) {
            SPQRComponentEdge& eTheOtherSide = eThisSide.pairVirtualEdge();
            SPQRTreeNode& tnTheOtherSide = eThisSide.pairTreeNode();
            eTheOtherSide.setVirtualPair(tnThisSide.backIt(),
                                                          eThisSide.backIt());
            auto newTreeEdgePtr = make_unique<SPQRTreeEdge>(
                                   eThisSide.backIt(), eTheOtherSide.backIt());
            auto& newTreeEdge =
                     addEdge(move(newTreeEdgePtr), tnThisSide, tnTheOtherSide);
            
            eThisSide.setTreeEdge(newTreeEdge.backIt());
            eTheOtherSide.setTreeEdge(newTreeEdge.backIt());
        }
    }    
}

}// namespace Undirected

}// namespace Wailea

