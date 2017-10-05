#ifndef _WAILEA_UNDIRECTED_SPQR_TREE_HPP_
#define _WAILEA_UNDIRECTED_SPQR_TREE_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"
#include "undirected/spqr_dfs_elements.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqr_tree.hpp
 *
 * @brief 
 *     
 */
namespace Wailea {

namespace Undirected { 

using namespace std;

class SPQRTreeNode;
class SPQRTreeEdge;
class SPQRComponentEdge;

/** @class SPQRTree
 *
 *  @brief  implementation of spqr-tree as a result of triconnected
 *          decomposition
 *
 *  @details this is a derived class of Graph. This class has SPQRTreeNode
 *           and SPQRTreeEdge objects as its nodes and edges.
 *           1. original() : reference to the original graph.
 *              BC-tree forms as a result of a decomposition of a simple
 *              biconnected graph.
 *           2. RNodes() : list of tree nodes of R type.
 *           3. SNodes() : list of tree nodes of S type.
 *           4. PNodes() : list of tree nodes of P type.
 *           A SPQRTreeNode has a component SPQRComponent as a copy of the
 *           subgraph of the original graph.
 *
 */
class SPQRTree : public Graph {

  public:

    static const utility_t   kForwardEdgeTypeSeparatingEdge;
    static const utility_t   kForwardEdgeTypeOrdinaryEdge;


    // Default constructor is prohibited.
    SPQRTree() = delete;

    // Constructor with the reference to the original graph.
    inline SPQRTree(Graph& original) noexcept;
    inline virtual ~SPQRTree() noexcept;

    // Move operations allowed.
    inline SPQRTree(SPQRTree&& rhs) noexcept;
    inline SPQRTree& operator=(SPQRTree&& rhs) noexcept;

    // Copy operations are not allowed.
    SPQRTree(const SPQRTree& rhs) = delete;
    SPQRTree& operator=(const SPQRTree& rhs) = delete;


    /** @brief reference to the original graph.
     *
     *  @return reference to the original graph.
     */
    inline Graph& original() noexcept;


    /** @brief list of tree nodes of R type.
     *
     *  @return list of tree nodes of R type.
     *
     *  @throws bad_alloc() if the underlying vector::push_back() throws it.
     *                 
     *  @remarks on complexity:
     *           this function runs in O(|N|) where N is the set of 
     *           SPQRTreeNodes.
     */
    vector<node_list_it_t> RTypeNodes();

    /** @brief list of tree nodes of S type.
     *
     *  @return list of tree nodes of S type.
     *
     *  @throws bad_alloc() if the underlying vector::push_back() throws it.
     *                 
     *  @remarks on complexity:
     *           this function runs in O(|N|) where N is the set of 
     *           SPQRTreeNodes.
     */
    vector<node_list_it_t> STypeNodes();


    /** @brief list of tree nodes of P type.
     *
     *  @return list of tree nodes of P type.
     *
     *  @throws bad_alloc() if the underlying vector::push_back() throws it.
     *                 
     *  @remarks on complexity:
     *           this function runs in O(|N|) where N is the set of 
     *           SPQRTreeNodes.
     */
    vector<node_list_it_t> PTypeNodes();

    /** @brief determines the type of each tree node.
     *         If the component has two nodes, it's of P Type.
     *         If all the degrees of all the component nodes are 2, 
     *         it's of S Type.
     *         Otherwise, it's of R Type.
     */
    void determineTypes();

    /** @brief creates the final remaining component and a tree node.
     *
     *  @param G                  (in): the graph being explored. it contains
     *                                  SPQR_DFS_Node and SPQR_DFS_Edge as its
     *                                  nodes and edges.
     *
     *  @param edgesInGToBeCopied (in): list of edges in G to be copied to the
     *                                  split component.
     */
    void makeFinalTreeNodeAndComponent(
        Graph&                  G,
        vector<edge_list_it_t>& edgesInGToBeCopied
    );


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
    void makeNewNodesEdgesAndComponents(

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

    );

    /** @brief one S node into another adjacent S node.
     *         
     *  @param eit (in): the tree edge whose incident nodes are type S
     *
     *  @param nodeToBeRemovedIt (in): the incident node of tree edge that
     *                                 will be removed after the merger.
     */
    void mergeTwoNodesS(
                  edge_list_it_t treeEdgeIt, node_list_it_t nodeToBeRemovedIt);


    /** @brief advance the edge along the path. Used in mergeTwoNodesS().
     *
     *  @param prevEit  (in): iterator to the previous edge on the path.
     * 
     *  @param thisEit  (in): iterator to the current edge on the path.
     *
     *  @return iterator to the next edge on the path.
     */
    edge_list_it_t advanceEdgeItOnPath(
                               edge_list_it_t prevEit, edge_list_it_t thisEit);

    /** @brief one P node into another adjacent P node.
     *         
     *  @param treeEdgeIt (in): the tree edge whose incident nodes are type P
     *
     *  @param nodeToBeRemovedIt (in): the incident node of tree edge that
     *                                 will be removed after the merger.
     */
    void mergeTwoNodesP(
                  edge_list_it_t treeEdgeIt, node_list_it_t nodeToBeRemovedIt);

    /** @brief change one incident node of tree edge to the new one
     *         after two tree nodes are merged.
     *
     *  @param e (in): the tree edge in which one incident node is updated.
     *
     *  @param nodeFrom (in): the old incident node to be removed.
     *
     *  @param nodeTo   (in): new incident node.
     *
     *  @param newVirtualEdge (in): the new virtual edge that corresponds in
     *                             the component of the updated tree node.
     */
    void relinkTreeEdge(SPQRTreeEdge& e, SPQRTreeNode& nodeFrom,
                     SPQRTreeNode& nodeTo, SPQRComponentEdge& newVirtualEdge);


  private:

    /** @brief fill the empty component of the newly create SPQR tree node
     *         with the split components.
     *  
     *  @param    G    (in): the graph being explored by DFS.
     *
     *  @param    comp (in): component of an SPQR tree node.
     *
     *  @param    nodesInGToBeCopied (in): split component nodes in G  
     *
     *  @param    edgesInGToBeCopied (in): split component edges in G  
     *
     *  @param    nodeAInGit  (in): a node in the split pair.
     *
     *  @param    nodeBInGit  (in): the other node in the split pair.
     *
     *  @param    nodeAInComp1It (out): the node in the component that  
     *                           corresponds to nodeAInGit.
     *
     *  @param    nodeBInComp1It (out): the node in the component that  
     *                           corresponds to nodeBInGit.
     */
    void fillInComponent(
        Graph&                  G,
        Graph&                  comp, 
        vector<node_list_it_t>& nodesInGToBeCopied, 
        vector<edge_list_it_t>& edgesInGToBeCopied,
        node_list_it_t          nodeAInGIt,
        node_list_it_t          nodeBInGIt,
        node_list_it_t&         nodeAInComp1It,
        node_list_it_t&         nodeBInComp1It );

    /** @brief scan the component of the given tree node, find the 
     *         unlinked virtual egge, and peir them up with their buddies, and
     *         make a tree edge in the SPQR tree.
     */
    void patchUpVirtualEdges(SPQRTreeNode& tnThisSide);


  private:

    /** @brief original graph on which the triconnected decomposition is made.
     */
    Graph* mOriginal;

#ifdef UNIT_TESTS
  friend class SPQRTreeTests;
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};


/** @class  SPQRComponent
 *
 *  @brief  represents a component graph for this node.
 *          It is either of the following three:
 *          R type: triconnected subgraph with some virtual edges.
 *          S type: cycle with some virtual edges.
 *          P type: dipole multi-graph with virtual edges and at most.
 *                  one non-virtual edge.
 */
class SPQRComponent : public Graph {

  public:

    inline SPQRComponent() noexcept;
    inline virtual ~SPQRComponent() noexcept;

    // move operations are allowed.
    inline SPQRComponent(SPQRComponent&& rhs) noexcept;
    inline SPQRComponent& operator=(SPQRComponent&& rhs) noexcept;

    // copy operations are not allowed.
    SPQRComponent(const SPQRComponent& rhs) = delete;
    SPQRComponent& operator=(const SPQRComponent& rhs) = delete;

    /** @brief rererence to the corresponding SPQRTreeNode.
     *
     *  @return rererence to the corresponding SPQRTreeNode.
     */
    inline SPQRTreeNode& SPQRTreeNode() const;

    /** @brief set the node owning this component.
     */
    inline void setBackLink(node_list_it_t it) noexcept;

  private:

    /** @brief the corresponding node in the SPQRTree.
     */
    node_list_it_t mTreeNode;

#ifdef UNIT_TESTS
  friend class SPQRTreeTests;
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};


/** @class SPQRTreeNode
 *
 *  @brief node used in SPQRTree.
 */
class SPQRTreeNode : public Node {
  public:

    enum type {Unknown, RType, PType, SType};

    /** @brief constructor
     */
    inline SPQRTreeNode();
    inline virtual ~SPQRTreeNode();

    // Move operations are not allowed.
    SPQRTreeNode(SPQRTreeNode&& rhs) = delete;
    SPQRTreeNode& operator=(SPQRTreeNode&& rhs) = delete;

    // copy operations are not allowed.
    SPQRTreeNode(const SPQRTreeNode& rhs) = delete;
    SPQRTreeNode& operator=(const SPQRTreeNode& rhs) = delete;

    /** @brief specifies the type of this node.
     *  @return type
     */
    inline enum type type() const noexcept;

    inline void setType(enum type t) noexcept;

    /** @brief returns a reference to the underlying component
     *  @return reference.
     */
    inline SPQRComponent& component() noexcept;

    /** @brief link this node to the component after this node is
     *         added to an SPQRTree.
     */
    inline void linkComponent() noexcept;

  private:

    /** @brief type of this node.
     */
    enum type      mType;

    /** @brief underlying component (skeleton graph)
     */
    SPQRComponent mComponent;

#ifdef UNIT_TESTS
  friend class SPQRTreeTests;
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};


/** @class SPQRTreeEdge
 *
 *  @brief edge used in SPQRTree.
 *         An edge represents a connection between two virtual edges in 
 *         two different components.
 */
class SPQRTreeEdge : public Edge {

  public:

    SPQRTreeEdge() = delete;

    /** @brief constructor with two virtual edges.
     *
     *  @param virtualEdge1 (in): virtual edge 1.
     *
     *  @param virtualEdge2 (in): virtual edge 2.
     */
    inline SPQRTreeEdge(
            edge_list_it_t virtualEdge1, edge_list_it_t virtualEdge2) noexcept;

    inline virtual ~SPQRTreeEdge();

    // move operations are not allowed.
    SPQRTreeEdge(SPQRTreeEdge&& rhs) = delete;
    SPQRTreeEdge& operator=(SPQRTreeEdge&& rhs) = delete;

    // copy operations are not allowed.
    SPQRTreeEdge(const SPQRTreeEdge& rhs) = delete;
    SPQRTreeEdge& operator=(const SPQRTreeEdge& rhs) = delete;

    /** @brief returns the virtual edge in the component of tree node 1.
     */
    inline edge_list_it_t virtualEdge1() const noexcept;

    /** @brief returns the virtual edge in the component of tree node 2.
     */
    inline edge_list_it_t virtualEdge2() const noexcept;

    /** @brief sets the new virtual edge.
     */
    inline void setVirtualEdge1(edge_list_it_t it) noexcept;

    /** @brief sets the new virtual edge.
     */
    inline void setVirtualEdge2(edge_list_it_t it) noexcept;

  private:

    /** @brief the virtual edge in the component of tree node 1.
     */
    edge_list_it_t mVirtualEdge1;

    /** @brief the virtual edge in the component of tree node 2.
     */
    edge_list_it_t mVirtualEdge2;

#ifdef UNIT_TESTS
  friend class SPQRTreeTests;
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};



/** @class SPQRComponentNode
 *
 *  @brief represents a node in a SPQRComponent
 */
class SPQRComponentNode : public Node {

  public:

    // Default constructor is prohibited.
    SPQRComponentNode() = delete;

    /** @brief constructor
     *
     *  @param org  (in): corresponding node in the original graph.
     */
    inline SPQRComponentNode(node_list_it_t org) noexcept;
    inline virtual ~SPQRComponentNode() noexcept;

    // move operations are not allowed.
    SPQRComponentNode(SPQRComponentNode&& rhs) = delete;
    SPQRComponentNode& operator=(SPQRComponentNode&& rhs) = delete;

    // copy operations are not allowed.
    SPQRComponentNode(const SPQRComponentNode& rhs) = delete;
    SPQRComponentNode& operator=(const SPQRComponentNode& rhs) = delete;


  private:

#ifdef UNIT_TESTS
  friend class SPQRTreeTests;
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};


/** @class SPQRComponentEdge
 *
 *  @brief represents an edge in a component.
 */
class SPQRComponentEdge : public Edge {

  public:

    enum type {VirtualType, OrdinaryType};    

    /** @brief constructor for an ordinary edge
     *
     *  @param org (in): corresponding edge in the original graph.
     */
    inline SPQRComponentEdge(edge_list_it_t org) noexcept;

    /** @brief constructor for a virtual edge
     *
     *  @param virtualEdgePair (in): virtual edge in the pair.
     *
     *  @param treeNodePair (in): tree node in which virtualEdgePair resides.
     */
    inline SPQRComponentEdge(
       edge_list_it_t virtualPairEdge, node_list_it_t pairTreeNode) noexcept ;


    inline virtual ~SPQRComponentEdge() noexcept;

    inline void setType(enum type t) noexcept;

    inline enum type type() const noexcept;


    /** @brief sets the other virtual edge in the pair.
     *
     *  @param  vc (in): the other SPQRTreeNode
     *  @param  ve (in): the other virtual edge
     */
    inline void setVirtualPair(node_list_it_t vc, edge_list_it_t ve) noexcept;

    /** @brief sets the corresponding SPQR tree edge for the virtual 
     *         edge pair.
     */
    inline void setTreeEdge(edge_list_it_t te) noexcept;


    /** @brief returns the corresponding SPQR tree edge for the virtual 
     *         edge pair.
     */
    inline SPQRTreeEdge& treeEdge() const;


    /** @brief  returns the tree node in which the other virtual edge resides.
     *
     *  @return the tree node in which the other virtual edge resides.
     *     
     *  @throws invalid_argument(Constants::kExceptionWrongType)
     *          if this edge is not virtual.
     */
    SPQRTreeNode& pairTreeNode() const;

    /** @brief returns the other virtual edge.
     *
     *  @return the other virtual edge.
     *
     *  @throws invalid_argument(Constants::kExceptionWrongType)
     *          if this edge is not virtual.
     */      
    SPQRComponentEdge& pairVirtualEdge() const;

  private:

    /** @brief type of the edge: ordinary or virtual.
     */
    enum type       mType;

    /** @brief corresponding tree edge in the SPQRTree.
     */
    edge_list_it_t  mTreeEdge;

    /** @brief the pair virtual edge in a different component if this is 
     *         a virtual edge.
     */
    edge_list_it_t  mVirtualPairEdge;

    /** @brief the tree node to which the pair virtual edge belongs to
     *         if this is a virtual edge.
     */
    node_list_it_t  mVirtualPairTreeNode;


#ifdef UNIT_TESTS
  friend class SPQRTreeTests;
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};


inline SPQRTreeNode::SPQRTreeNode():mType(Unknown){;}


inline SPQRTreeNode::~SPQRTreeNode(){;}


inline enum SPQRTreeNode::type SPQRTreeNode::type() const noexcept { return mType; }


inline void SPQRTreeNode::setType(enum type t) noexcept { mType = t; }


inline SPQRComponent& SPQRTreeNode::component() noexcept { return mComponent; }


inline void SPQRTreeNode::linkComponent() noexcept
                                    { mComponent.setBackLink(this->backIt()); }

inline SPQRTreeEdge:: SPQRTreeEdge(
     edge_list_it_t virtualEdge1, edge_list_it_t virtualEdge2) noexcept :
     mVirtualEdge1(virtualEdge1), mVirtualEdge2(virtualEdge2) {;}


inline SPQRTreeEdge::~SPQRTreeEdge(){;}


inline edge_list_it_t SPQRTreeEdge::virtualEdge1() const noexcept 
                                                     { return mVirtualEdge1; }

inline edge_list_it_t SPQRTreeEdge::virtualEdge2() const noexcept 
                                                     { return mVirtualEdge2; }

inline void SPQRTreeEdge::setVirtualEdge1(edge_list_it_t it) noexcept
{
    mVirtualEdge1 = it;
}

inline void SPQRTreeEdge::setVirtualEdge2(edge_list_it_t it) noexcept
{
    mVirtualEdge2 = it;
}


inline SPQRComponent::SPQRComponent() noexcept {;}


inline SPQRComponent::~SPQRComponent() noexcept {;}


inline SPQRComponent::SPQRComponent(SPQRComponent&& rhs) noexcept :
                           Graph(move(rhs)), mTreeNode(move(rhs.mTreeNode)){;}

inline SPQRComponent& SPQRComponent::operator=(SPQRComponent&& rhs) noexcept {
    Graph::operator=(move(rhs));
    mTreeNode = move(rhs.mTreeNode);
   return *this;
}


inline SPQRTreeNode& SPQRComponent::SPQRTreeNode() const {
    return dynamic_cast<class SPQRTreeNode&>(*(*(mTreeNode)));
}


inline void SPQRComponent::setBackLink(node_list_it_t it) noexcept 
                                                           { mTreeNode = it; }

inline SPQRTree::SPQRTree(Graph& original) noexcept : mOriginal(&original){;}


inline SPQRTree::~SPQRTree() noexcept {;}


inline SPQRTree::SPQRTree(SPQRTree&& rhs) noexcept :
                                 Graph(move(rhs)), mOriginal(rhs.mOriginal){;}

inline SPQRTree& SPQRTree::operator=(SPQRTree&& rhs) noexcept
{
    Graph::operator = (move(rhs));
    mOriginal = move(rhs.mOriginal);
    return *this;
}


inline Graph& SPQRTree::original() noexcept {return *mOriginal;}


inline SPQRComponentNode::SPQRComponentNode(node_list_it_t org) noexcept
{ 
    pushIGBackwardLink(org);
}


inline SPQRComponentNode::~SPQRComponentNode() noexcept {;}



inline SPQRComponentEdge::SPQRComponentEdge(edge_list_it_t org) noexcept :
                                                           mType(OrdinaryType)
{
    pushIGBackwardLink(org);
}

inline SPQRComponentEdge:: SPQRComponentEdge(
       edge_list_it_t virtualPairEdge, node_list_it_t pairTreeNode) noexcept :
       mType(VirtualType),
       mVirtualPairEdge(virtualPairEdge), 
       mVirtualPairTreeNode(pairTreeNode) {;}


inline SPQRComponentEdge::~SPQRComponentEdge() noexcept {;}
                                                                         

inline void SPQRComponentEdge::setType(enum type t) noexcept { mType = t; }


inline enum SPQRComponentEdge::type SPQRComponentEdge::type() const noexcept
                                                              { return mType; }


inline void SPQRComponentEdge::setVirtualPair(
                          node_list_it_t vc, edge_list_it_t ve) noexcept
{ 
    mType = VirtualType ;
    mVirtualPairEdge = ve;
    mVirtualPairTreeNode = vc;
}

inline SPQRComponentEdge& SPQRComponentEdge::pairVirtualEdge() const {
    if (mType != VirtualType) {
        throw std::invalid_argument(Constants::kExceptionWrongType);
    }
    return dynamic_cast<SPQRComponentEdge&>(*(*mVirtualPairEdge));
}


inline SPQRTreeNode& SPQRComponentEdge::pairTreeNode() const {
    if (mType != VirtualType) {
        throw std::invalid_argument(Constants::kExceptionWrongType);
    }
    return dynamic_cast<SPQRTreeNode&>(*(*mVirtualPairTreeNode));
}


inline void SPQRComponentEdge::setTreeEdge(edge_list_it_t te) noexcept {
    mTreeEdge = te;
}

inline SPQRTreeEdge& SPQRComponentEdge::treeEdge() const {
    if (mType != VirtualType) {
        throw std::invalid_argument(Constants::kExceptionWrongType);
    }
    return dynamic_cast<SPQRTreeEdge&>(*(*mTreeEdge));
}

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQR_TREE_HPP_*/

