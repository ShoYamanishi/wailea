#ifndef _WAILEA_UNDIRECTED_BCTREE_HPP_
#define _WAILEA_UNDIRECTED_BCTREE_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/bctree.hpp
 *
 * @brief biconnected decomposition and BC-tree.
 *     
 * @details
 *    The main component of this file is BiconnectedDecomposer, which 
 *    takes a simple connected graph and generates a BC-tree as a result
 *    of the binconnected decomposition.
 *    A path in a BC-tree always forms an alternating sequence of block and
 *    and cut vertex.
 *    A BC-tree is represented by the class BCTree, which is a derived 
 *    class of Graph, and which contains BCTreeNode and BCTreeEdge as its
 *    nodes and edges respectively.
 * 
 *    A BCTreeNode can be either of a cut vertex or a block.
 *    If it is a block, it contains Block object, which is a derived class
 *    of Graph to represent a copy of the subgraph (block) of the original
 *    input graph. A Block has BlockNode and BlockEdge as its nodes and edges.
 *    A minimal Block is an edge with two vertices, at least one of which
 *    is a cut vertex, except for the three cases:
 *    1. If the input graph is empty, the resultant BC-tree is empty.
 *    2. If the input graph has only a single isolated vertex, the resultant
 *    BC-tree consists of a block node, which contains a copy of the single
 *    vertex.
 *    3. If the input graph has only an edge with two incident vertices,
 *    the resultant BC-tree consists of a block node, which contains copies
 *    of the edge and the vertices.
 *
 *    The BiconnectedDecomposer uses the DFS-based algorithm by Hopcroft
 *    and Tarjan (1973).
 *    The DFS algorithm is implemented with DFSExplorer with the following
 *    subcomponents.
 *    - BCStrategy is a derived class of DFSStrategy to implement algorithm
 *      ussing hook points called by DFSExplorer.
 *    - BCDFSNode and BCDFSEdge are graph node and edge to be explored by
 *      DFSExplorer and BCStrategy. Those two classes hold values used by
 *      BCStrategy such as lowpt number.
 *      BCDFSNode and BCDFSEdge are further decorated to
 *      DFSNode<Graph,BCDFSNode,BCDFSEdge> and 
 *      DFSEdge<Graph,BCDFSNode,BCDFSEdge>
 *      for DFSExplorer. DFSNode<> and DFSEdge<> hold necessary data required
 *      to perform DFS walk in DFSExplorer.
 *      See undirected/dfs.hpp for details.
 *      Before performing a DFS walk, BiconnectedDecomposer created a copy
 *      of the original input graph using DFSNode<Graph,BCDFSNode,BCDFSEdge> 
 *      and DFSEdge<Graph, BCDFSNode,BCDFSEdge>.
 *      The copy is destroyed after the walk is finished.
 *    
 *
 *  @remarks class diagram
 *    
 *    BiconnectedDecomposer
 *               defines->BCStrategy
 *               defines->BCDFSNode
 *               defines->BCDFSEdge
 *         
 *               owns->DFSExplorer<Graph,BCDFSNode,BCDFSEdge>
 *                       explores->DFSGraph<Graph,BCDFSNode,BCDFSEdge>
 *                                     owns->DFSNode<Graph,BCDFSNode,BCDFSEdge>
 *                                     owns->DFSEdge<Graph,BCDFSNode,BCDFSEdge>
 *
 *               owns->BCStrategy
 *                       generates->BCTree
 *                                     owns->BCTreeNode
 *                                             owns->Block
 *                                                     owns->BlockNode
 *                                                     owns->BlockEdge
 *                                     owns->BCTreeEdge
 *
 */

/*   [SCRATCH]
 *    It generates a BC-tree of the given connected component of the graph.
 *    A BC-tree is implemented as BCTree object, and it has BCTreeNode
 *    and BCTreeEdge as its nodes and edges.
 *    A BCTreeNode is either of BlockType or CutVertexType.
 *
 *    A BlockType contains Block object to represent the corresponding subgraph
 *    of the original graph.  Block is a Graph object that contains
 *    BlockNode and BlockEdge as its nodes and edges.
 *    A BlockNode is either of OrdinaryVertexType or a CutVertexType.
 *    An OrdinaryVertexType is a copy of a node that is not a cut vertex
 *    in the original graph. A CutVertexType is a copy of a cut vertex in the
 *    original grpah. A BlockNode of CutVertexType has an iterator to 
 *    the corresponding edge in the BC-tree.
 *    A BlockNode and a BlockEdge both contain an iterator to the Node
 *    and the Edge object in the original graph respectively.
 *
 *    A BCTreeNode of CutVertexType represents a cut vertex. It contains an
 *    iterator to the Node in the original Graph.
 *
 *    A BCTreeEdge is an edge in the BC-tree. It connects a copy of the 
 *    cut vertex in a block to the corresponding BCTreeNode of CutVertexType.
 *    One incidedent node is a BlockType and the other is a CutVertexType
 * 
 */

namespace Wailea {

namespace Undirected { 

using namespace std;

class Block;
class BCTreeNode;
class BlockNode;


class ConstantsBCTree {

  public:
    static const char*       kExceptionBCTreeInvalidNodeType;
    static const utility_t   kForwardTypeCutVertex;
    static const utility_t   kForwardTypeBlockNode;
};


/** @class BCTree
 *
 *  @brief   implementation of bc-tree as a result of biconnected decomposition
 *
 *  @details this is a derived class of Graph. This class has BCTreeNode
 *           and BCTreeEdge objects as its nodes and edges.
 *           There are three additional functionalities on top of Graph.
 *           1. original() : reference to the original graph. 
 *              BC-tree forms as a result of a decomposition of a simple
 *              connected graph. 
 *           2. cutVertexNodes() : list of tree nodes for cut vertices.
 *           3. blockNodes() : list of tree nodes for blocks.
 */
class BCTree : public Graph {

  public:

    // Default constructor is prohibited.
    inline BCTree():Graph() {;}

    // Constructor with the reference to the original graph.
    inline BCTree(Graph& original) noexcept;

    inline virtual ~BCTree() noexcept;

    // Move operations allowed.
    inline BCTree(BCTree&& rhs) noexcept;
    inline BCTree& operator=(BCTree&& rhs) noexcept;


    // Copy operations are not allowed.
    BCTree(const BCTree& rhs) = delete;
    BCTree& operator=(const BCTree& rhs) = delete;


    /** @brief reference to the original graph.
     *
     *  @return reference to the original graph.
     */
    inline Graph& original()  noexcept;


    /** @brief list of tree nodes for cut vertices.
     *
     *  @return list of tree nodes for cut vertices.
     * 
     *  @throws  bad_alloc() if the underlyind vector::push_back() 
     *                       throws it.
     *
     *  @remarks on complexity:
     *           this function runs in O(|N|) where N is the set of 
     *           BCtree nodes.
     */
    vector<node_list_it_t> cutVertexNodes();


    /** @brief list of tree nodes for blocks.
     *
     *  @return list of tree nodes for blocks.
     * 
     *  @throws  bad_alloc() if the underlyind vector::push_back() 
     *                       throws it.
     *
     *  @remarks on complexity:
     *           this function runs in O(|N|) where N is the set of 
     *           BCtree nodes.
     */
    vector<node_list_it_t> blockNodes();

  private:

    /** @brief original graph on which the BC-tree decomposition is made.
     */
    Graph* mOriginal;

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif

};


/** @class  BCTreeNode
 *
 *  @brief  node used in BCTree.
 *          It represents a cut vertex or a block.
 *
 */
class BCTreeNode : public Node {

  public:

    enum type {
        BlockType,
        CutVertexType
    };

    /** @brief constructor for a cut vertex.
     *
     *  @param  cv (in): the node (cut vertex) in the original graph.
     */
    BCTreeNode(node_list_it_t cv) noexcept;

    /** @brief constructor for a block
     *
     *  @param  block (in): the ownership of the block.
     */
    BCTreeNode(unique_ptr<Block> block);

    // move operations are not allowed.
    BCTreeNode(BCTreeNode&& rhs) = delete;
    BCTreeNode& operator=(BCTreeNode&& rhs) = delete;

    // copy operations are not allowed.
    BCTreeNode(const BCTreeNode& rhs) = delete;
    BCTreeNode& operator=(const BCTreeNode& rhs) = delete;
        
    /** @brief specifies the type of this node: cut vertex or block.
     *
     *  @return type
     */
    inline enum type type();

    /** @brief returns the reference to the block if this node is for a block.
     *
     *  @return reference to the block.
     *
     *  @throws ConstantsBCTree::kExceptionBCTreeInvalidNodeType
     *          if this node is not for a block.
     */
    inline Block& block();

  private:

    /** @brief type of this node: Block or Cut vertex.
     */
    enum type mType;

    /** @brief if mType == BlockType
     *         pointer to the block (Graph).
     *         This node also owns this block.
     */
    unique_ptr<Block>    mBlock;

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif

};


/** @class  BCTreeEdge
 *
 *  @brief  edge used in BCTree.
 *          One incident node is a cut vertex, while the other is
 *          a block.
 */
class BCTreeEdge : public Edge {

  public:

    /** @brief constructor with the indicator to which side of the incident
     *         nodes is for cut vertex.
     *  @param cutVertexOnNode1 (in): true if the node1 is for cut vertex.
     */
    inline BCTreeEdge(bool cutVertexOnNode1) noexcept;


    // move operations are not allowed.
    BCTreeEdge(BCTreeEdge&& rhs) = delete;
    BCTreeEdge& operator=(BCTreeEdge&& rhs) = delete;

    // copy operations are not allowed.
    BCTreeEdge(const BCTreeEdge& rhs) = delete;
    BCTreeEdge& operator=(const BCTreeEdge& rhs) = delete;

    /** @brief returns the incident node for blcok.
     */
    inline BCTreeNode& blockNode() noexcept;

    /** @brief returns the incident node for cut vertex.
     */
    inline BCTreeNode& cutVertexNode() noexcept;

    /** @brief returns the corresponding copy of the cut vertex
     *         in the block in the block side of the incident tree node.
     *
     *  @return the reference to the copy of the cut vertex in the block.
     */
    inline BlockNode& cutVertexInBlock() noexcept;

    /** @brief sets the corresponding copy of the cut vertex
     *         in the block in the block side of the incident tree node.
     *
     *  @param the interator to the copy of the cut vertex in the block.
     *
     *  @remarks this should not be called by the user of BCTreeEdge
     *           other than BCStrategy.
     */
    inline void setCutVertexInBlock(node_list_it_t it) noexcept;

    /** @brief true if the cut vertex is on node 1 of this edge.
     *
     *  @return true if the cut vertex is on node 1.
     *          false if the cut vertex is on node 2.
     */
    inline bool isCutVertexOnNode1() const noexcept;

  private:

    /** @brief true if  Node1 is the cut vertex
     *         false if Node2 is the cut vertex
     */
    bool mCutVertexOnNode1;

    /** @brief (copy of) cutvertex in the block.
     */
    node_list_it_t  mCutVertexInBlock;

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif

};


/** @class  Block
 *
 *  @brief  represents a block. This belongs to (is owned by) a BCTreeNode.
 *          A block means a bi-connected component.
 */
class Block : public Graph {

  public:

    inline Block() noexcept;
    inline virtual ~Block() noexcept;

    // move operations are allowed.
    inline Block(Block&& rhs) noexcept;

    inline Block& operator=(Block&& rhs) noexcept;

    // copy operations are allowed.
    Block(const Block& rhs) = delete;
    Block& operator=(const Block& rhs) = delete;


    /** @brief set the iterator to the BCTreeNode that contains this block.
     *
     *  @param it (in): iterator to hte tree node.
     *
     *  @remarks this should not be called outside BCStrategy.
     */
    inline void setBackTreeNode(node_list_it_t it)noexcept;


    /** @brief lists the copies of cut vertices in the block.
     *
     *  @return list of the copies of cut vertices in the block.
     */
    vector<node_list_it_t> cutVertices();

    /** @brief reference to the corresponding BC tree node.
     *
     *  @return reference to the corresponding BC tree node.
     */
    inline BCTreeNode& BCTreeNode() noexcept;

  private:

    /** @brief  the corresponding node in the BCTree.
     */
    node_list_it_t mBCTreeNode;

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif

#ifdef USE_TEST_GRAPH_GENERATOR
  public:
    long mDebugNum;
#endif
};


/** @class  BlockNode
 *
 *  @brief  represents a node in a block.
 */
class BlockNode : public Node {

  public:

    enum type {
        CutVertexType,
        OrdinaryNodeType
    };

    // Default constructor is prohibited.
    BlockNode() = delete;

    /** @brief constructor
     * 
     *  @param org  (in): corresponding node in the original graph.
     *  @param type (in): specifies if this is a cut vertex or not.
     */
    inline BlockNode(node_list_it_t org, enum type type) noexcept;

    // move operations are not allowed.
    BlockNode(BlockNode&& rhs) = delete;
    BlockNode& operator=(BlockNode&& rhs) = delete;

    // copy operations are not allowed.
    BlockNode(const BlockNode& rhs) = delete;
    BlockNode& operator=(const BlockNode& rhs) = delete;

    /** @brief specifies if this is a copy of a cut vertex or not.
     *
     *  @return type 
     */
    inline enum type type() const noexcept;

    /** @brief sets the back iterator to the BC tree edge to which
     *         this node corresponds to if this is a copy of a cut vertex.
     *         This must not be called other than BCStrategy.
     *
     *  @param it (in): the iterator to the BC tree edge.
     */
    inline void setBCTreeEdge(edge_list_it_t it) noexcept;

     /** @brief returns the back iterator to the BC tree edge to which
      *         this node corresponds to if this is a copy of a cut vertex.
      *  @return iterator
      */
    inline BCTreeEdge& treeEdge() noexcept;

  private:

    /** @brief type of this node: cut vertex or not.
     */
    enum type mType;

    /** @brief corresponding edge in the BC-tree if this is of CutVertexType
     */
    edge_list_it_t mBCEdge;

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif
   
};


/** @class  BlockEdge
 *
 *  @brief  represents an edge in a block.
 */
class BlockEdge : public Edge {

  public:

    // Consructor
    BlockEdge(edge_list_it_t org);

    // move operations are not allowed.
    BlockEdge(BlockEdge&& rhs) = delete;
    BlockEdge& operator=(BlockEdge&& rhs) = delete;

    // copy operations are not allowed.
    BlockEdge(const BlockEdge& rhs) = delete;
    BlockEdge& operator=(const BlockEdge& rhs) = delete;

  private:

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif

};


/** @class  BiconnectedDecomposer
 *  @brief  takes a simple connected graph and creates a BC-tree as a result
 *          of biconnected decomposition.
 *  @details 
 *      The main top level class that performs the decomposition.
 *      It depends on DFSExplorer, DFSNode, and DFSEdge defined in 
 *      undirected/dfs.hpp.
 *      It takes a Graph object of a simple connected graph as its input.
 *      If the input graph is not connected, only the connected component
 *      from the starting node is explored, but the processing time
 *      is still in the order of O(|N|+|E|), where N and E represent the
 *      nodes and edges of the entire input graph.
 *      If the input graph is not simple (i.e. if it has multi-edges and/or 
 *      self-loops), the behavior is undefined.
 * 
 *      It has a single main member function decompose(), which takes the 
 *      input graph and a starting node, and then generates a BC-tree, and
 *      subordinate blocks contained in the BC-tree nodes.
 *     
 *      Complexities:
 *        It uses heap memory in the order of O(|N|+|E|).
 *        It runs in the order of  O(|N|+|E|).
 */
class BiconnectedDecomposer {

  public:


    /** @brief performs the decomposition from a specific node.
     *
     *  @param   g (in): input graph. It must be simple and connected.
     *  @param   s (in): start node for DFS walk.
     *
     *  @return  a BC-tree.
     *
     *  @remark  On inter-graph linking structure.
     *           After decompose() is performed:
     *           - Nodes of g have a new forward link and a new utility value
     *             If the new utility value is 
     *             ConstantsBCTree::kForwardTypeCutVertex, then the new forward
     *             link is to the corresponding BlockNode.
     *             If the new utility value is 
     *             ConstantsBCTree::kForwardTypeBlockNode, then the new forward
     *             link is to the corresponding BCTreeNode of cut vertex type.
     *
     *           - Edges of g have a new forward link pointing to new 
     *             BlockEdge.
     *          
     *           NOTE: g.popIGBackwardLinkFromNodes(), g.popUtilityFromNodes(),
     *           and g.popIGBackwardLinkFromEdges() can be called to remove them
     *           from g.
     *
     *           - BCTreeNodes of CutVertexType have a backward link to the
     *             original Nodes in g.
     *
     *           - BlockNodes of OrdinaryNodeType have a backward link to the
     *             original Nodes in g.
     *
     *           - BlockEdges have a backward link to the original Edges in g.
     *
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed.
     *
     *  @remark undefined behavior
     *           If the input graph is not simple, the behavior is undefined.
     *           If the input graph is not connected, only the connected 
     *           component from the start node will be explored, but
     *           the required memory and running time will be still in the 
     *           order of O(|N|+|E|).
     */
    BCTree decompose(Graph& g, Node& s);

    /** @brief performs the decomposition from an arbitrary node.
     *         it allows an empty graph as the input.
     *  @param   g (in): input graph. It must be simple and connected.
     *
     *  @return  a BC-tree.
     *
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed.
     *
     *  @remark undefined behavior
     *           If the input graph is not simple, the behavior is undefined.
     *           If the input graph is not connected, only the connected 
     *           component from the start node will be explored, but
     *           the required memory and running time will be still in the 
     *           order of O(|N|+|E|).
     */

    BCTree decompose(Graph& g);

  private:


    /** @class BCDFSNode
     *
     *  @brief a graph node to be explored by BCStrategy.
     *         This class is augumented to DFSNode<Graph,BCDFSNode,BCDFSEdge>
     *         for DFSExplorer<Graph,BCDFSNode,BCDFSEdge>.
     */
    class BCDFSNode : public Node {
 
      public:
        inline BCDFSNode():mNum(0),mLowPt(0),mCutVertex(false){;}

        size_t         mNum;
        size_t         mLowPt;

        /** @brief true if this is found to be a cut vertex.
         */
        bool           mCutVertex;

        /** @brief corresponding node (cut vertex) in BCTree
         */
        node_list_it_t mBCTreeNodeCutVertex;

    };


    class BCDFSEdge : public Edge {

      public:
        inline BCDFSEdge(){;}

    };


    /** @class BCStrategy
     *
     *  @brief implements Hopcroft & Tarjan's DFS-based algorithm.
     */
    class BCStrategy : public DFSStrategy<Graph,BCDFSNode,BCDFSEdge> {

      public:

        /// Constructor requires an empty BCtree
        BCStrategy(node_list_it_t s, BCTree& bcTree) noexcept;

        // The following 5 functions are the hook points to DFSExplorer
        void enterNode(DNode& n) noexcept;
        void leaveNode(DNode& n) noexcept;
        void processFrond(DNode& n, DEdge& c);        // throws bad_alloc()
        void beforeVisitingChild(DNode& n, DEdge& c); // throws bad_alloc()
        void afterVisitingChild(DNode& n, DEdge& c);  // throws bad_alloc()

      private:

        /**  @brief  creates a cut vertex in BC-tree.
         *   @param  n (in): the node currently being explored, which 
         *                   has been found to be a new cut vertex.
         *   @throws bad_alloc()
         */
        void processNewlyFoundCutVertex(BCDFSNode& n);

        /**  @brief  creates a block node in BC-tree.
         *   @param  blockEdges (in): the edges built-up during DFS 
         *                            exploration. They represent a block.
         *   @throws bad_alloc()
         */
        void processNewlyFoundBlock(
               BCDFSNode& n, BCDFSEdge& e, vector<edge_list_it_t>& blockEdges);


        /**  @brief takes an empty block node, fill it with a copy of
         *          the subgraph, and then connects the relevant cut vertices
         *          in BCtree to the block with tree edges.
         *     
         *   @param   tn    (in): empty BCTreeNode for a block.
         *   @param   eCopy (in): the edges built-up during DFS 
         *                        exploration. They represent a block.
         *   @throws bad_alloc()
         */
        void fillBlockAndConnectBCTreeNodes(
                             BCTreeNode& tn, vector<edge_list_it_t>& eCopy);


        /**  @brief the next number assigned to the visiting node.
         */
        size_t                 mI;

        /**  @brief edge stack that represents the next block to be found.
         */
        vector<edge_list_it_t> mStack;

        /**  @brief start node.
         */
        node_list_it_t         mRootNode;

        /**  @brief BCtree to which nodes and edges are added.
         */
        BCTree&                mBCTree;
    };


    /**  @brief  BC-tree that consists of a single block node that has
     *           block of a single isolated vertex.
     *           
     *   @param  n (in): the node (copy) of the single vertex.
     *
     *   @return BCTree constructoed.
     *
     *   @throws bad_alloc()
     */
    BCTree processSingleIsolatedVertex(Graph& g, Node& n);


    /** @brief   creates a copy of the input graph for DFS exploration.
     *
     *  @param   src (in):  the input graph whose copy is created.
     *  @param   dst (out): the copy graph to which nodes and edges are added.
     *  @param   startNode (in): the start node in the input graph.
     *
     *  @return  the start node in the copied graph.
     */
    node_list_it_t copyInputGraph(
                            Graph& src, Graph& dst, node_list_it_t startNode);

#ifdef UNIT_TESTS
  friend class BCTreeTests;
#endif
        
};


inline BCTree::BCTree(Graph& original) noexcept
                                            :Graph(),mOriginal(&original){;}

inline BCTree::~BCTree() noexcept {;}


inline BCTree::BCTree(BCTree&& rhs) noexcept
                              :Graph(move(rhs)), mOriginal(rhs.mOriginal){;}

inline BCTree& BCTree::operator=(BCTree&& rhs) noexcept 
{
    Graph::operator = (move(rhs));
    mOriginal = rhs.mOriginal;
    return *this;
}


inline Graph& BCTree::original() noexcept { return *mOriginal; }


inline BCTreeNode::BCTreeNode(node_list_it_t cv) noexcept:
                                        mType(CutVertexType),mBlock(nullptr)
{
    pushIGBackwardLink(cv);
}

inline BCTreeNode::BCTreeNode(unique_ptr<Block> block):
                                     mType(BlockType),mBlock(move(block)){;}

inline enum BCTreeNode::type BCTreeNode::type() {return mType;}


inline Block& BCTreeNode::block()
{
    if (mType != BlockType) {
        throw std::invalid_argument(
                           ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return *mBlock;
}


inline BCTreeEdge::BCTreeEdge(bool cutVertexOnNode1) noexcept:
                                 Edge(),mCutVertexOnNode1(cutVertexOnNode1){;}


inline BCTreeNode& BCTreeEdge::blockNode() noexcept
{
    if (mCutVertexOnNode1) {
        return dynamic_cast<BCTreeNode&>(incidentNode2());
    }
    else {
        return dynamic_cast<BCTreeNode&>(incidentNode1());
    }
};

inline BCTreeNode& BCTreeEdge::cutVertexNode() noexcept
{
    if (mCutVertexOnNode1) {
        return dynamic_cast<BCTreeNode&>(incidentNode1());
    }
    else {
        return dynamic_cast<BCTreeNode&>(incidentNode2());
    }
};


inline BlockNode& BCTreeEdge::cutVertexInBlock() noexcept 
{
    return dynamic_cast<BlockNode&>(*(*mCutVertexInBlock));
};


inline void BCTreeEdge::setCutVertexInBlock(node_list_it_t it) noexcept
{
    mCutVertexInBlock = it;
}

inline bool BCTreeEdge::isCutVertexOnNode1() const noexcept {
    return mCutVertexOnNode1;
}


inline Block::Block() noexcept :Graph(){;}


inline Block::~Block() noexcept {;}


inline Block::Block(Block&& rhs) noexcept
                      :Graph(move(rhs)), mBCTreeNode(move(rhs.mBCTreeNode)){;}


inline Block& Block::operator=(Block&& rhs) noexcept
{
    Graph::operator = (move(rhs));
    mBCTreeNode = move(rhs.mBCTreeNode);
    return *this;
}

inline void Block::setBackTreeNode(node_list_it_t it) noexcept
{
    mBCTreeNode = it;
}

inline BCTreeNode& Block::BCTreeNode() noexcept
{
    return dynamic_cast<class BCTreeNode&>(*(*mBCTreeNode));
}


inline BlockNode::BlockNode(node_list_it_t org, enum type type) noexcept:
                                                      Node(),mType(type)
{
    pushIGBackwardLink(org);
}


inline enum BlockNode::type BlockNode::type() const noexcept {return mType;}


inline void BlockNode::setBCTreeEdge(edge_list_it_t it) noexcept
{
    mBCEdge = it;
}


inline BCTreeEdge& BlockNode::treeEdge() noexcept
{
    return dynamic_cast<BCTreeEdge&>(*(*mBCEdge));
}


inline BlockEdge::BlockEdge(edge_list_it_t org)
{
    pushIGBackwardLink(org);
}

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_BCTREE_HPP_*/
