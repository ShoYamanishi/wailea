#ifndef _WAILEA_UNDIRECTED_EMBEDDED_BCTREE_HPP_
#define _WAILEA_UNDIRECTED_EMBEDDED_BCTREE_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/planar_dual_graph_maker.hpp"
#include "undirected/bctree.hpp"

#include "directed/di_base.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif


namespace Wailea {

namespace Undirected {

/**
 * @file undirected/embedded_bctree.hpp
 *
 * @brief
 *
 * @remark
 * This file handles a combinatorial and geometric embedding of a connected
 * graph. Here we define a combinatorial embedding as a set of permissible
 * ordering of the incident edges around each node, and a geometric embedidng
 * as a combinatorial embedding with its outer face and the orientation fixed.
 * A geometric embedding is used to generate a visibility representation of 
 * the connected graph.
 *
 * A connected graph can be decomposed into block-cut-tree, or BC-tree.
 * Each node of a BC-tree is either a cut vertex or a block.
 * A biconnected decomposition into a BC-tree can be considered to be a 2-tier
 * structure, where the higher level is the rootless tree with its nodes
 * cutvertices and blocks, and each block as the lower level is a biconnected
 * subgraph.
 * A combinatorial embedding of a biconnected component is usually found by 
 * some variants of planarity testing algorithm such as Booth & Lueker.
 * On the other hand, a combinatorial embedding of a tree is always planar, 
 * and sometimes the combinatorial embedding is given externally.
 *
 * ===============================================================
 * How to represent a combinatorial embedding of a connected graph
 * ===============================================================
 * Our representation is based on the BC-tree decomposion.
 * We assume a connected component has been decomposed into a
 * BC-tree and for each block, a combinatorial embedding and its dual graph
 * have been already found independently.
 * The main task is then unifying the embeddings of the blocks incident to
 * each cut vertex. 
 * A convenient way to think about the unification is first to form some groups
 * of faces incident to the cut vertex. 
 * We denote such a group by face unification group. Each unification
 * face group consists of zero or one face from each incident block.
 * No face can be part of more than one unification group for the same cut
 * vertex.
 * Multiple faces from a single block can participate in unification groups.
 * At least one face from a block must participate in a unification group
 * for the cut vertex.
 * If we form a graph with its nodes for the incident blocks, all the 
 * participating faces, and all the face unification groups for the cut vertex,
 * and connect two nodes with an edge if: 
 * - one is a block and the other is a participating face from the block
 * - one is a face unification group and the other is the participating face
 * then the graph must not contain a cycle. I.e, the graph must be a tree.
 *
 * Each face unification group specifies a circular ordering and orientation
 * of the participating block around the cut vertex. 
 * In each face unification group, each participating block nominates a face
 * to be unified in the face unification group.
 * If the block is not k2, then the block also specifes the 
 * ordering of two incident edges to the nominated face. This ordering 
 * specifies one of the two orientations (like the two sides of a coin)
 * of the block relative to the unification group.
 *
 * An easy way to visualize this unification of a face unification group is
 * temporarily making the nominated faces outer face of each block.
 *
 * Consider a situation where three blocks B1, B2, and B3 are incident to a 
 * cut vertex cv1 and B2, B4, and B5 are incident to a cut vertex cv2
 * as follows.
 *
 *         B1                 B2                 B3
 *   _______________    _______________    _______________ 
 *  |e6\       e1/  |  |  \e12    e7/  |  |  \e18   e13/  |
 *  |   \       /   |  |   \       /   |  |   \       /   | 
 *  |    \  f1 /    |  |    \  f7 /    |  |    \  f13/    |
 *  | f6  \   /  f2 |  | f12 \   /  f8 |  | f18 \   /  f14| 
 *  |      \ /      |  |      \ /      |  |      \ /      |
 *  |------cv1------|  |------cv1------|  |------cv1------|
 *  |e6    / \   e2 |  |e11   / \   e8 |  |e17   / \   e14|
 *  |     /   \     |  |     /   \     |  |     /   \     |
 *  | f5 /     \ f3 |  | f11/     \ f9 |  | f17/     \ f15|
 *  |   /  f4   \   |  |   /  f10  \   |  |   /  f16  \   | 
 *  |e4/       e3\  |  |  /e10    e9\  |  |  /e16   e15\  |
 *   ---------------   -cv2-------------   ---------------
 *
 *         B4                 B5
 *   _______________    _______________
 *  |e24      e19/  |  |  \e30   e25/  |
 *  |   \       /   |  |   \       /   |
 *  |    \  f19/    |  |    \  f25/    |
 *  | f24 \   /  f20|  | f30 \   /  f26|
 *  |      \ /      |  |      \ /      |
 *  |------cv2------|  |------cv2------|
 *  |e23   / \   e20|  |e29   / \   e26|
 *  |     /   \     |  |     /   \     |
 *  | f23/     \ f21|  | f29/     \ f27|
 *  |   /  f22  \   |  |   /  f28  \   |
 *  |e22      e21\  |  |  /e28   e27\  |
 *   ---------------   -----------------
 *
 * Assume we want to embedded the blocks in the circular ordering of
 * (B1, B3, B2).
 * The faces  f4, f10, and f16 are nominated as the faces to be unified.
 * The orientations of the blocks are: ( (e4, e3), (e16, e15), (e9, e10) ).
 *
 * This can be visualized as follows.
 *
 *           _________
 *           \       /
 *            \  B1 /
 *           e3\   /e4  Unified faces: (f4, f16, and f10)
 *              \ /
 *      __e16___cv1___e10__
 *      \       / \       /
 *       \  B3 /   \e9   /
 *        \   /e15  \ B2/
 *         \_/       \_/
 *
 * Please note that one UF group around one cut vertex may be further 
 * unified with other UF groups around another cut vertices as follows.
 * We denote such a set of UF groups by UF group tree.
 *
 *
 *           _________      _____
 *           \       /     |    /
 *            \  B1 /      |B5 /
 *           e3\   /e4  e30|  /e25
 *              \ /        | /
 *      __e16___cv1___e10__cv2__e24__
 *      \       / \       / \       /
 *       \  B3 /   \e9   /   \ B4  /
 *        \   /e15  \ B2/  e19\   /
 *         \_/       \_/       \_/
 *
 *  face unification group 1 : (f4, f16, f10)
 *  face unification group 2 : (f10 f25, f19)
 *  face unification group tree T(V,E) = ( {group 1, group 2}, {(cv1,cv2)} )
 *  Unified faces: (f4, f16, f10, f25, and f19)
 *
 * To formalize our representation of a combinatorial embedding of a
 * connected graph G, we need the following.
 * - BC-tree of G
 * - For each block B, combinatorial embedding Gamma(B), and its Dual(B)
 * - For each cut vertex CV, set S(CV) of face unification groups.
 * - Each face unification group G has a circular ordering of the incident 
 *   edges of the nominated faces of the participating blocks.
 *  
 * This can be implemented by augumenting BC-tree. The nodes of block type
 * contain combinatorial embeddings and their duals, and the nodes of 
 * cut vertex type and the tree edges contain the UF groups described above.
 * This is implemented into EmbeddedBCTree[Node,Edge] below.
 *
 * ================================================================
 * Finding a geometric embedding of a connected graph and
 * a permissible classification of inner/outer faces of each block
 * ================================================================
 * We define a geometric embedding as a combinatorial embedding with its
 * outer face and the orientation determined.
 * Any face of any block can be part of the outer face of the resultant 
 * embedding for the connected graph.
 *
 * First, in a geometric embedding, we observe that among the faces in a 
 * face unification group tree, all except for one have to be the outer faces
 * of corresponding blocks when decomposed.
 * This comes from the fact that two inner faces can not be unified without
 * making crossings in a geometric embedding. Please see the example below.
 *
 *               B4                        B5
 *       f20                       f28
 *       ________cv2_______        ________cv2_______
 *      |     / /   \ \    |      |     / /   \ \    |
 *      |f19 /  |   |  \f21|      | f29/  |   |  \f27|
 *      |   /   |   |   \  |      |   /   |   |   \  |
 *      |  / f24|f23| f22\ |      |  / f30|f25|f26 \ |
 *      |------------------|      |------------------|
 *      |\/\/\/\/\/\/\/\/\/|      |\/\/\/\/\/\/\/\/\/|
 *      |\/\/\/\/\/\/\/\/\/|      |\/\/\/\/\/\/\/\/\/|
 *       ------------------        ------------------
 *
 * - f20 and f28 can be unified. In this case B4 and B5 will be placed
 *   side-by-side.
 * - f20 and f30 can be unified. In this case B4 will be enclosed by f30.
 * - f23 and f28 can be unified. In this case B5 will be enclosed by f23.
 * - f24 and f30 can not be unified without making a crossing, as both are
 *   inner faces.
 *
 * We have to first determine the outer face and the top node for 
 * each block.
 * There are two constraints in the way each face is classified into 
 * outer or inner face.
 * - For each block, exactly one face will be outer face, and the rest
 *   will be inner faces.
 * - For each face participating in a face unification group for a cut vertex,
 *   at most one face can be inner face, the rest must be outer faces.
 * Please note that this classification is valid if we look at the embedding
 * of each block individually. An outer face of a block may be unified 
 * with an inner face of another block, in which case the former will be
 * enclosed in the latter and the former will not be part of the outer face
 * of the entire connected graph.
 * 
 * To meet those contraints, we nominate one face in each unification group
 * as 'the absorber', and the rest as 'the absorbees'.
 * The absorber can be an inner face, but the absorbees must be outer faces
 * in order to keep the planarity.
 * If we take apart the connected graph at the cut vertex, then we will get
 * several connected subgraphs and each of them are represented by the faces
 * participating to the unification groups at the cut vertex.
 * The sugraphs (with their embeddings) identified by the absorbees can be 
 * considered absorbed in their absorber face at each unification group.
 * Please note that there can be multiple face unification groups at a cut
 * vertex.
 * At each block, we can have at most one absorbee and all the other faces
 * that participate to face unification groups must be absorbor, as they
 * have to be inner faces. This means only the outer face can be an absorbee.
 *
 * The above observation about the constraints and the absorber/absorbee
 * relationships among the blocks give a partial order among the block nodes
 * of the BC-tree, which can be represented in a rooted directed tree.
 * Such a tree is represented by EmbeddedBCTree::ExplorationTree.
 * Please note that ExplorationTree is not a directed version of BC-tree.
 *
 * For any given combinatorial embedding of a connected graph represented
 * as above, there is following freedom to make inner/outer classification.
 * - What block to be the root block?
 * - What face to be the outer face of the root block?
 * - What node incident to the face to be the top node? : (This is needed
 *   for the visibility representation algorithm)
 * Once those are determined, the outer/inner face classification will be done
 * from the root block down to the leaf blocks recursively as follows.
 *
 * - For each block node:
 *   - If it is a root node, make the outer face the absorbee.
 *     Otherwise, the absorbee has been already determined before reaching here
 *   - For each cut vertex do:
 *         for each face unificaton group do:
 *             for each face in the face unification group do:
 *                 - if the face blongs to this block, make it absorber.
 *                 - else make it absorbee and recursively process the block.
 * 
 *
 * For that purpose, EmbeddedBCTree provides findGeometricEmbedding(), which
 * classifies the faces and generates EmbeddedBCTree::ExplorationTree.
 */


class ConstantsEmbeddedBCTree {

  public:
    static constexpr const char* kExceptionBCTreeRootNotSet
                    = "The root is not set for the tree or the tree is empty.";
    static constexpr const char* kExceptionEmbeddingNotSet
                    = "The embedding is not set for this edge.";
};


class EmbeddedBCTreeNode;
class EmbeddedBCTreeEdge;
class UnificationFace;
class UnificationFaceRef;
class UnificationGroup;
class SortedUnificationGroup;

/** @class EmbeddedBCTree 
 *
 *  @brief BC-tree with a combinatorial embedding.
 *         The combinatorial embeding is specified by the following.
 *         - embedding (and its dual) of each block
 *         - face unification groups of each cut vertex
 *         The details about the face unification groups is described
 *         above at "How to represent a combinatorial embedding of 
 *         a connected graph".
 *
 *         The input graph is assumed to be planar. If it is not planar, the
 *         behavior is undefined.
 *         It provides a convenience function, makeDefaultEmbedding, which
 *         finds an embedding for each block using Booth-Lueker planarity
 *         algorithm, and a single face unification group is created at 
 *         each cut vertex. From each block the biggest incident face is 
 *         selected and those are places in one group in the order found in the
 *         BC-tree.
 *
 *         If the user does not use make DefaultEmbedding, s/he must provide
 *         the following:
 *
 *         For each EmbeddedBCTreeNode of type BCTreeNode::BlockType
 *             The incident edges of each node in Block must represent
 *             planar combinatorial embedding.
 *             An EmbeddedGraph and a DualGraph that refer to 
 *             the underlying planar Block in the BC-tree.
 *             Those are set by EmbeddedBCTreeNode::setEmbedding().
 *         
 *         For each EmbeddedBCTreeEdge, which spans a block and a cut vertex,
 *             fit:  The face nominated from the block to be unified at
 *                   the cut vertex.
 *             eCW:  EmbeddedEdge incident to the face and the copy of
 *                   the cut vertex above. On the clockwise side to be unified
 *                   around the cut vertex.
 *             eCCW: EmbeddedEdge incident to the face and the copy of
 *                   the cut vertex above. On the counter-clockwise side.
 *             If eCW and eCCW are not in the same orientation as in the
 *             orientation of the half edges around the face, then the 
 *             block in which the face reside will be flipped when unified.
 *
 *         For each EmbeddedBCTreeNode of type BCTreeNode::CutVertexType
 *             face unification groups. A face unification group is a
 *             vector<edge_list_it_t>, and the ordering in the vector 
 *             specifies the counter-clockwise ordering of the embedding.
 *             of the blocks (faces) specified in EmbeddedBCTreeEdge.
 *             Those are set by EmbeddedBCTreeNode::pushBackUnificationGroup()
 *
 *         It provides a function findGeometricEmbedding() to find a 
 *         geometric embedding by classifying outer face/inner faces of the
 *         embedding of each block, and by generating a rooted tree structure
 *         among block nodes.
 *         The function can take the following three parameters:
 *         - Root block
 *         - Outer face of the root block
 *         - Top node of the outer face of the root block
 *         The top node is used to find a visibility representation.
 *
 *  @dependencies
 *         It depends on BCTree in bctree.hpp. An EmbeddedBCTree references to
 *         the nodes, edges, blocks, and cut vertices of BCTree.
 *         ExplorationTree is derived from Directed::DiGraph.
 *  
 */
class EmbeddedBCTree : public Graph {

  public:

    /** @brief this is a directed tree as the result of orient() operation
     *         by which the outer face is determined for each block.
     *         this tree is rooted and oriented.
     *         The tree nodes represent the block nodes of the BC-tree.
     *         This tree is used to generate a visibility
     *         representation of the connected graph the BC-tree represents.
     *         This tree is different from any oriented BC-tree in the sense
     *         there are no nodes that represent cut vertices.
     */
    class ExplorationNode;
    class ExplorationTree : public Directed::DiGraph {

      public:

        // Move constructor and assignment are allowed.
        inline ExplorationTree(ExplorationTree&& rhs) noexcept;
        inline ExplorationTree& operator=(ExplorationTree&& rhs) noexcept;

        /** @brief  returns the root ExplorationNode that corresponds to
         *          the root block node in EmbeddedBCTree.
         *
         *  @throws invalid_argument(ConstantsEmbeddedBCTree::
         *                                          kExceptionBCTreeRootNotSet)
         *          if the tree is empty.
         */
        inline ExplorationNode& root();

      private:
        inline ExplorationTree();
        inline void setRoot(node_list_it_t root);

        node_list_it_t  mRoot;

    friend class EmbeddedBCTree;
#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif
    };


    class ExplorationNode : public Directed::DiNode {

      public:

        /** @brief reset the internal counter for bottom-up tree traversal.
         */
        inline void resetNumChildrenProcessed();

        /** @brief increment the internal counter for bottom-up tree traversal.
         */
        inline void incrementNumChildrenProcessed();

        /** @brief returns the current value of the internal counter
         *         for bottom-up tree traversal.
         */
        inline long numChildrenProcessed();

        /** @brief returns the sorted unification groups with the absorber
         *         absorbees classification.
         */
        inline vector<SortedUnificationGroup>& sortedUnificationGroups();

      private:
        inline ExplorationNode(EmbeddedBCTreeNode& org);

        /** @brief set of subordinate face unification groups whose absorber
         *         faces are in this block.
         *         This is used when generating visibility representation.
         */
        vector<SortedUnificationGroup> mSortedUnificationGroups;

        /** @brief internal counter for bottom-up tree traversal.
         */
        long mNumChildrenProcessed;

    friend class EmbeddedBCTree;
    friend std::unique_ptr<ExplorationNode> std::make_unique<ExplorationNode,
                                     EmbeddedBCTreeNode&>(EmbeddedBCTreeNode&);
#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif



    };

    class ExplorationEdge : public Directed::DiEdge {

      public:
  
      private:
        inline ExplorationEdge();

    friend class EmbeddedBCTree;
    friend std::unique_ptr<ExplorationEdge> std::make_unique<ExplorationEdge>();
#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif

    };


    /** @brief constructor from a BCTree.
     *
     *  @param tree (in): BC-tree.
     */
    EmbeddedBCTree(BCTree& tree);

    /** @brief generates a default combinatorial embedding.
     *         It generates an embedding and its dual for each block
     *         and then one face unification group per cut vertex.
     *         the face to be unified is chosen to be the largest one
     *         incident to the cut vertex. The ordering of the unified faces
     *         is taken from the ordering of the incident blocks around the
     *         cut vertex in the BC-tree.
     *
     *         BlockNodes & BlockEdges of each Block in BCTreeNode of 
     *         BlockType and EmbeddedNodes & EmbeddedEdges in 
     *         EmbeddedBCTreeNode::embeddedGraph() are cross-linked with
     *         IGForwardLink and IGBackwardLink.
     */
    void makeDefaultEmbedding();


    /** @brief find the geometric embedding based on the given block node
     *         in the BC-tree and the outer face of the block.
     *         The root top node can be considered a dummy cut vertex to
     *         make the root block consistent with the others.
     *         Also, the top node is used to find visibility representation.
     *
     *  @param rootBlock (in): The top level block from which absorber/
     *                         absorbee classification starts.
     *
     *  @param rootOuterFace (in):
     *                         The face that will become the outer face of the
     *                         geometric embedding in the rootBlock
     *
     *  @param rootTopNode (in):
     *                         The node in EmbeddedNode for the block that will
     *                         become the top node to generate a visibility 
     *                         representation for the block.
     *
     *  @remark if rootTopNode is not specified, an arbitrary node incident
     *          to the outer face is chosen.
     *          If rootOuterFace is not specified, an arbitrary biggest face
     *          is chosen.
     *          If rootBlock is not specified, an arbitrary largest block is
     *          chosen.
     *
     *  @remark 
     *
     *
     */
    void findGeometricEmbedding(
        node_list_it_t rootBlock,
        node_list_it_t rootOuterFace,
        node_list_it_t rootTopNode
    );
    void findGeometricEmbedding(
                       node_list_it_t rootBlock, node_list_it_t rootOuterFace);
    void findGeometricEmbedding(node_list_it_t rootBlock);
    void findGeometricEmbedding();

    /** @brief returns the underlying BCTree that holds all the Blocks.
     */
    inline BCTree& bcTree();

    /** @brief returns the root block in EmbeddedBCTree.
     *
     *  @throws ConstantsEmbeddedBCTree::kExceptionBCTreeRootNotSet if
     *          the tree is empty, or if findGeometricEmbedding has not been
     *          called.
     */
    inline EmbeddedBCTreeNode& root();

    /** @brief returns the ExplorationTree generated by 
     *         findGeometricEmbedding()
     */
    inline ExplorationTree& explorationTree();
    
  private:

    /** @brief underlying BCTre specified in the constructor.
     */
    BCTree*         mTree;

    /** @brief the root block specified to or determined in 
     *         findGeometricEmbedding.
     */
    node_list_it_t  mRoot;

    /** @brief ExplorationTree generated by findGeometricEmbedding.
     */
    ExplorationTree mExpTree;

    /** @brief finds the biggest face around the given node (EmbeddedNode)
     *
     *  @param  TN      (in):  EmbeddedNode
     *  @param  cCWIt   (out): EmbeddedEdge incident to TN and the face
     *                         clockwise side of the block.
     *  @param  cCCWIt  (out): EmbeddedEdge incident to TN and the face
     *                         counter-clockwise side of the block.
     *
     *  @return iterator to the face found
     */
    node_list_it_t findBiggestFace(
        EmbeddedNode&   TN,
        edge_list_it_t& eCWIt,
        edge_list_it_t& eCCWIt
    );

#ifdef UNIT_TESTS
  friend class EmbeddedBCTreeTests;
#endif

};


/** @class EmbeddedBCTreeNode
 *
 *  @brief Augumentation of BCTreeNode with the necessary embedding
 *         information by composition (as opposed to inheritance)
 *         For the block nodes:
 *         - Combinatorial embedding of block and its dual.
 *
 *         For the cut-vertex nodes:
 *         - Face unification groups as in a set of ordered list of
 *           edge pointers to EmbeddedBCTreeEdge.
 *
 *         After calling findGeometricEmbedding():
 *         For the block nodes:
 *         - Outer face
 *         - Top node (cut vertex) used to make a visibility representation.
 *         - If the block is flipped or not
 */
class EmbeddedBCTreeNode : public Node {

  public:

    /** @brief returns the type of the node based on the underlying 
     *         BCTreeNode: BlockType or CutVertexType.
     */
    inline enum BCTreeNode::type type();

    /** @brief returns the embedding of the block.
     *         valid if this is BlockType.
     */
    inline EmbeddedGraph&    embeddedGraph();

    /** @brief returns the dual graph of the embedding of the block
     *         valid if this is BlockType.
     */
    inline DualGraph&        dualGraph();

    /** @brief returns the number of face unification groups.
     *         valid if this is CutVertexType.
     */
    inline size_t            numUnificationGroups();

    /** @brief returns the face unification group specified by the index.
     *         valid if this is CutVertexType.
     */
    inline UnificationGroup& unificationGroup(size_t index);

    /** @brief returns the outer face of this block.
     *         valid if this is BlockType, and
     *         EmbeddedBCTree::findGeometricEmbedding() has been called.
     */
    inline EmbeddedFace&     outerFace();

    /** @brief returns the top node of this block.
     *         valid if this is BlockType, and
     *         EmbeddedBCTree::findGeometricEmbedding() has been called.
     */
    inline EmbeddedNode&     topNode();

    /** @brief called manually if the user of EmbeddedBCTree wants to 
     *         specify the embedding of the block manually.
     *
     *  @param eg (in): embedding
     *  @param dg (in): dual graph
     *
     *  @throws ConstantsBCTree::kExceptionBCTreeInvalidNodeType
     *          if this node is not for a block.
     *
     *  @remark IMPORTANT
     *         BlockNodes & BlockEdges of each Block in BCTreeNode of 
     *         BlockType and EmbeddedNodes & EmbeddedEdges in 
     *         EmbeddedBCTreeNode::embeddedGraph() must be
     *         cross-linked with IGForwardLink and IGBackwardLink.
     */
    inline void setEmbedding(EmbeddedGraph&& eg, DualGraph&& dg);

    /** @brief called manually if the user of EmbeddedBCTree wants to
     *         specify the face unification groups manually.
     *         The specified unification group is pushed back internally
     *         to a vector which is indexed by a number.
     *
     *  @param ug (in): unification group.
     *
     *  @throws ConstantsBCTree::kExceptionBCTreeInvalidNodeType
     *          if this node is not for a cut vertex.
     */
    inline void pushBackUnificationGroup(UnificationGroup&& ug);
    inline void pushBackUnificationGroup(const UnificationGroup& ug);

    inline list<UnificationFaceRef>& unexploredFaces();

  private:

    EmbeddedBCTreeNode()=delete;

    /** @brief constructor with the corresponding BCTreeNode
     */
    inline EmbeddedBCTreeNode(BCTreeNode& original);

    /** @brief aggregate setter for outer face and top node
     *         EmbeddedBCTree::findGeometricEmbedding().
     */
    inline void setOrientation(
                           node_list_it_t outerFace, node_list_it_t topNode);

    /** @brief type of the underlying BCTreeNode.
     *         we keep the copy of the type here as IGBackwardLink stack
     *         may change over time.
     */
    enum BCTreeNode::type mType;

    /** @brief embedding of the block if this is BlockType.
     */
    EmbeddedGraph  mEG;

    /** @brief dual of the embedding of the block if this is BlockType.
     */
    DualGraph      mDG;

    /** @brief unification groups if this node is of CutVertexType.
     */
    vector<UnificationGroup> mUnificationGroups;

    /** @brief outer face 
     */
    node_list_it_t mOuterFace;

    /** @brief top node of the visibility representation in embedded graph
     *         its the copy of the cut vertex at the unification.
     */
    node_list_it_t mTopNode;

    /** @remark
     */
    list<UnificationFaceRef> mUnexploredFaces;


friend class EmbeddedBCTree;    
friend std::unique_ptr<EmbeddedBCTreeNode> 
                std::make_unique<EmbeddedBCTreeNode, BCTreeNode&>(BCTreeNode&);
friend class UnificationFace;

#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif

};


/** @class EmbeddedBCTreeEdge
 *
 *  @brief Augumentation of BCTreeEdge with the necessary embedding
 *         information by composition (as opposed to inheritance)
 *         - Copy of the cut vertex (top node) in the block
 *         - Face to be unified in the specified face unification group.
 *           in the dual graph of the embedding of the block.
 *         - Two edges incident to the top node and the face on the 
 *           block, both counter-clockwise side and clockwise side around
 *           the top node.
 *         - Indices into the face unification group in the incident 
 *           EmbededBCTreeNode of CutVertexType.
 */
class EmbeddedBCTreeEdge : public Edge {

  public:

    /** @brief returns the incident node of the block type.
     */
    inline EmbeddedBCTreeNode& incidentNodeBlockType();

  private:

    EmbeddedBCTreeEdge()=delete;

    /** @brief constructor with the corresponding BCTreeEdge
     */
    inline EmbeddedBCTreeEdge(BCTreeEdge& original);

friend class EmbeddedBCTree;
friend class EmbeddedBCTreeNode;
friend std::unique_ptr<EmbeddedBCTreeEdge> 
                std::make_unique<EmbeddedBCTreeEdge, BCTreeEdge&>(BCTreeEdge&);
#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif

};


/** @brief internal class to reference a unification face in a unification 
 *         group from an embedded bc-tree node of block type to another 
 *         of cut-vertex.
 */
class UnificationFaceRef {

  public:

    inline UnificationFaceRef(
              EmbeddedBCTreeNode& CVNode, size_t groupIndex, size_t faceIndex);

    inline UnificationFace& unificationFace();

    inline EmbeddedBCTreeNode& cvNode();

    inline UnificationGroup& group();

    inline size_t faceIndex();

  private:

    EmbeddedBCTreeNode& mCVNodeRef;
    size_t              mGroupIndex;
    size_t              mFaceIndex;

};


class UnificationFace {

  public:

    inline UnificationFace(

        node_list_it_t    nodeInEmbeddedBCTree,
        node_list_it_t    faceInDG,
        node_list_it_t    cutVertexInEG,
        edge_list_it_t    edgeCWInEG,
        edge_list_it_t    edgeCCWInEG

    );

    inline virtual ~UnificationFace();

    inline EmbeddedBCTreeNode& treeNode();

    inline EmbeddedFace&  faceInDG();
    inline node_list_it_t faceInDGIt();

    inline EmbeddedNode&  cutVertexInEG();
    inline node_list_it_t cutVertexInEGIt();

    inline EmbeddedEdge&  edgeCCWInEG();
    inline edge_list_it_t edgeCCWInEGIt();

    inline EmbeddedEdge&  edgeCWInEG();
    inline edge_list_it_t edgeCWInEGIt();

    inline bool           roleOfECWReversed();

    inline bool operator==(const UnificationFace& rhs) const;

  private:

    inline void markExplored();

    inline void setOrientation();
    
    inline void setBackIt(list<UnificationFaceRef>::iterator it);

    /** @brief default constructor used by SortedUnificationGroup */
    inline UnificationFace();

    /** @brief pointer to the node in embedded bctree  */
    node_list_it_t mNodeInEmbeddedBCTree;

    /** @brief pointer to embedded face to be unified */
    node_list_it_t mFaceInDG;

    /** @brief pointer to the cut vertex (EmbeddedNode) */
    node_list_it_t mCutVertexInEG;

    /** @brief the incident edge to the face and the cut vertex
     *         on the clockwise side to be unified around the cut vertex.
     */
    edge_list_it_t mEdgeCWInEG;

    /** @brief the incident edge to the face and the cut vertex
     *         on the counter-clockwise side to be unified around the cut 
     *         vertex.
     */
    edge_list_it_t mEdgeCCWInEG;

    /** @brief indicates if the orientation of mEdgeCCWInEG -> mEdgeCWInEG
     *         is different from their natural orientation in the embedding
     *         of the block.
     */
    bool           mRoleOfECWReversed;

    /** @brief back pointer to the embedded bctree node */
    list<UnificationFaceRef>::iterator mUnexploredBackIt;

friend class EmbeddedBCTree;
friend class UnificationGroup;
friend class SortedUnificationGroup;

#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif
};


/** @class UnificationGroup
 *
 *  @brief represents a face unification group.
 *         It specifies a subset of the incident blocks around the cut vertex
 *         in the counter-clockwise ordering.
 */
class UnificationGroup {

  public:

    inline UnificationGroup();

    inline virtual ~UnificationGroup();

    /** @brief set a face to this face unification group.
     */
    inline void push(UnificationFace&& uf);
    inline void push(const UnificationFace&  uf);

    /** @brief get the face at the specified index.
     */
    inline UnificationFace& at(size_t index);

    /** @brief number of faces stored
     */
    inline size_t size();

    inline void markExplored();

  private:

    vector<UnificationFace> mUnificationFaces;

#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif

};


/** @class SortedUnificationGroup
 *
 *  @brief represents a face unification group with the absorber/absorbee
 *         classification.
 */
class SortedUnificationGroup : public UnificationGroup {

  public:

    inline SortedUnificationGroup(UnificationGroup& UG, size_t absorberIndex);

    inline virtual ~SortedUnificationGroup();

    /** @returns the absorber face.
     */
    inline UnificationFace& absorber();

    /** @brief get the absorbee face at the specified index.
     */
    inline UnificationFace& absorbeeAt(size_t index);

    /** @brief number of absorbee faces stored
     */
    inline size_t absorbeesSize();

    /** @brief used in VisRepFinder to specify the first index
     *         in the second row after split.
     */
    size_t mIndexSecondStart;

  private:
    UnificationFace mAbsorber;

#ifdef UNIT_TESTS
    friend class EmbeddedBCTreeTests;
#endif

};

/** @brief inline function definitions */
 
EmbeddedBCTree::ExplorationTree& EmbeddedBCTree::explorationTree(){
                                                             return mExpTree; }

EmbeddedBCTree::ExplorationTree::ExplorationTree(){;}

EmbeddedBCTree::ExplorationTree::ExplorationTree(ExplorationTree&& rhs)
    noexcept: Directed::DiGraph(std::move(rhs)), mRoot(std::move(rhs.mRoot)){;}

EmbeddedBCTree::ExplorationTree&
EmbeddedBCTree::ExplorationTree::operator=(ExplorationTree&& rhs) noexcept 
{
    mRoot = std::move(rhs.mRoot);
    DiGraph::operator=(std::move(rhs));
    return *this;
}

void EmbeddedBCTree::ExplorationTree::setRoot(node_list_it_t root){
                                                                mRoot = root; }

EmbeddedBCTree::ExplorationNode& EmbeddedBCTree::ExplorationTree::root(){
    if (numNodes()==0||mRoot==nodes().second) {
        throw std::invalid_argument(
                         ConstantsEmbeddedBCTree::kExceptionBCTreeRootNotSet);
    }                            
    return dynamic_cast<ExplorationNode&>(*(*mRoot));
}

EmbeddedBCTree::ExplorationNode::ExplorationNode(EmbeddedBCTreeNode& org):
                                                       mNumChildrenProcessed(0)
{
    pushIGBackwardLink(org.backIt());
}

void EmbeddedBCTree::ExplorationNode::resetNumChildrenProcessed()
{
    mNumChildrenProcessed = 0;
}

void EmbeddedBCTree::ExplorationNode::incrementNumChildrenProcessed()
{
    mNumChildrenProcessed++;
}

long EmbeddedBCTree::ExplorationNode::numChildrenProcessed()
{
    return mNumChildrenProcessed;
}

vector<SortedUnificationGroup>& 
                     EmbeddedBCTree::ExplorationNode::sortedUnificationGroups()
{
    return mSortedUnificationGroups;
}

EmbeddedBCTree::ExplorationEdge::ExplorationEdge(){;}

EmbeddedBCTreeNode::EmbeddedBCTreeNode(BCTreeNode& original):
    mType(original.type())
{
    pushIGBackwardLink(original.backIt());

}

enum BCTreeNode::type EmbeddedBCTreeNode::type(){ return mType; }


EmbeddedGraph& EmbeddedBCTreeNode::embeddedGraph(){ 
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return mEG; 
}

DualGraph& EmbeddedBCTreeNode::dualGraph(){
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return mDG; 
}


void EmbeddedBCTreeNode::pushBackUnificationGroup(UnificationGroup&& ug)
{
    if (type() != BCTreeNode::CutVertexType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    mUnificationGroups.push_back(std::move(ug));
}


void EmbeddedBCTreeNode::pushBackUnificationGroup(const UnificationGroup& ug)
{
    if (type() != BCTreeNode::CutVertexType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    mUnificationGroups.push_back(ug);
}


size_t EmbeddedBCTreeNode::numUnificationGroups()
{
    if (type() != BCTreeNode::CutVertexType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return mUnificationGroups.size();
}

UnificationGroup& EmbeddedBCTreeNode::unificationGroup(size_t index)
{
    if (type() != BCTreeNode::CutVertexType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return mUnificationGroups[index];
}

void EmbeddedBCTreeNode::setOrientation(
              node_list_it_t outerFace, node_list_it_t topNode)
{
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    mOuterFace = outerFace;
    mTopNode   = topNode;
}

EmbeddedFace& EmbeddedBCTreeNode::outerFace()
{
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return dynamic_cast<EmbeddedFace&>(*(*mOuterFace));
}

EmbeddedNode& EmbeddedBCTreeNode::topNode()
{
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return dynamic_cast<EmbeddedNode&>(*(*mTopNode));
}


void EmbeddedBCTreeNode::setEmbedding(EmbeddedGraph&& eg, DualGraph&& dg)
{
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    mEG = std::forward<EmbeddedGraph&&>(eg);
    mDG = std::forward<DualGraph&&>(dg);
}


list<UnificationFaceRef>& EmbeddedBCTreeNode::unexploredFaces() {
    if (type() != BCTreeNode::BlockType){
        throw std::invalid_argument(
                            ConstantsBCTree::kExceptionBCTreeInvalidNodeType);
    }
    return mUnexploredFaces;
}


EmbeddedBCTreeEdge::EmbeddedBCTreeEdge(BCTreeEdge& original)
{
    pushIGBackwardLink(original.backIt());
}


EmbeddedBCTreeNode& EmbeddedBCTreeEdge::incidentNodeBlockType()
{
    auto& N1 = dynamic_cast<EmbeddedBCTreeNode&>(incidentNode1());
    if (N1.type() == BCTreeNode::BlockType) {
        return N1;
    }
    else {
        return dynamic_cast<EmbeddedBCTreeNode&>(incidentNode2());
    }
}


BCTree& EmbeddedBCTree::bcTree(){ return *mTree; }

EmbeddedBCTreeNode& EmbeddedBCTree::root(){
    if (numNodes()==0||mRoot==nodes().second) {
        throw std::invalid_argument(
                         ConstantsEmbeddedBCTree::kExceptionBCTreeRootNotSet);
    }                            
    return dynamic_cast<EmbeddedBCTreeNode&>(*(*mRoot)); 
}

UnificationGroup::UnificationGroup(){;}

UnificationGroup::~UnificationGroup(){;}

void UnificationGroup::markExplored() {
    for (size_t i = 0; i < size(); i++) {
        auto& uf = at(i);
        uf.markExplored();
    }
}

void UnificationGroup::push(UnificationFace&& uf){
               mUnificationFaces.push_back(std::forward<UnificationFace>(uf));}


void UnificationGroup::push(const UnificationFace& uf){
                                              mUnificationFaces.push_back(uf);}

UnificationFace& UnificationGroup::at(size_t index){
                                              return mUnificationFaces[index];}

size_t UnificationGroup::size(){ return mUnificationFaces.size(); }


SortedUnificationGroup::SortedUnificationGroup(
    UnificationGroup& UG,
    size_t absorberIndex     ):UnificationGroup()
{
    for (size_t i = absorberIndex+1; i < UG.size(); i++) {
        push(UG.at(i));
    }
    for (size_t i = 0; i < absorberIndex; i++) {
        push(UG.at(i));
    }
    mAbsorber = UG.at(absorberIndex);
}


SortedUnificationGroup::~SortedUnificationGroup(){;}


UnificationFace& SortedUnificationGroup::absorber(){ return mAbsorber; }


UnificationFace& SortedUnificationGroup::absorbeeAt(size_t index){
                                          return UnificationGroup::at(index); }


size_t SortedUnificationGroup::absorbeesSize(){
                                             return UnificationGroup::size(); }


UnificationFaceRef::UnificationFaceRef(
    EmbeddedBCTreeNode& CVNode,
    size_t              groupIndex,
    size_t              faceIndex
):mCVNodeRef(CVNode), mGroupIndex(groupIndex), mFaceIndex(faceIndex){;}


UnificationFace& UnificationFaceRef::unificationFace() {
    return mCVNodeRef.unificationGroup(mGroupIndex).at(faceIndex());
}


EmbeddedBCTreeNode& UnificationFaceRef::cvNode() {
    return mCVNodeRef;
}


UnificationGroup& UnificationFaceRef::group() {
    return mCVNodeRef.unificationGroup(mGroupIndex);
}

size_t UnificationFaceRef::faceIndex() { return mFaceIndex; }


UnificationFace::UnificationFace(

    node_list_it_t    prmNodeInEmbeddedBCTree,
    node_list_it_t    prmFaceInDG,
    node_list_it_t    prmCutVertexInEG,
    edge_list_it_t    prmEdgeCWInEG,
    edge_list_it_t    prmEdgeCCWInEG

):  mNodeInEmbeddedBCTree(prmNodeInEmbeddedBCTree),
    mFaceInDG(prmFaceInDG),
    mCutVertexInEG(prmCutVertexInEG),
    mEdgeCWInEG(prmEdgeCWInEG),
    mEdgeCCWInEG(prmEdgeCCWInEG)
{
    auto& ecw = edgeCWInEG();
    if (prmEdgeCWInEG == prmEdgeCCWInEG) {
        mRoleOfECWReversed = false;
    }
    else if (ecw.mHalfEdge1.mEmbeddedFace == mFaceInDG) {
        mRoleOfECWReversed = (ecw.mHalfEdge1.mSrcNode!=mCutVertexInEG);
    }
    else {
        mRoleOfECWReversed = (ecw.mHalfEdge2.mSrcNode!=mCutVertexInEG);
    }
}

UnificationFace::UnificationFace(){;}

UnificationFace::~UnificationFace(){;} 

EmbeddedBCTreeNode& UnificationFace::treeNode(){
    return dynamic_cast<EmbeddedBCTreeNode&>(*(*mNodeInEmbeddedBCTree));
}


void UnificationFace::setBackIt(list<UnificationFaceRef>::iterator it) {
    mUnexploredBackIt = it;
}

void UnificationFace::markExplored() {
    auto& BN = treeNode();
    BN.unexploredFaces().erase(mUnexploredBackIt);
}

void UnificationFace::setOrientation() {
    auto& BN = treeNode();  
    BN.setOrientation(mFaceInDG, mCutVertexInEG);
}

EmbeddedFace& UnificationFace::faceInDG() {
                           return dynamic_cast<EmbeddedFace&>(*(*mFaceInDG)); }
node_list_it_t UnificationFace::faceInDGIt() { return mFaceInDG; }

EmbeddedNode& UnificationFace::cutVertexInEG() {
                      return dynamic_cast<EmbeddedNode&>(*(*mCutVertexInEG)); }
node_list_it_t UnificationFace::cutVertexInEGIt() { return mCutVertexInEG; }

EmbeddedEdge& UnificationFace::edgeCCWInEG() {
                        return dynamic_cast<EmbeddedEdge&>(*(*mEdgeCCWInEG)); }

edge_list_it_t UnificationFace::edgeCCWInEGIt() { return mEdgeCCWInEG; }

EmbeddedEdge& UnificationFace::edgeCWInEG() {
                         return dynamic_cast<EmbeddedEdge&>(*(*mEdgeCWInEG)); }

edge_list_it_t UnificationFace::edgeCWInEGIt() { return mEdgeCWInEG; }

bool UnificationFace::roleOfECWReversed() { return mRoleOfECWReversed; }


bool UnificationFace::operator==(const UnificationFace& rhs) const {
    return   mFaceInDG          == rhs.mFaceInDG          &&
             mCutVertexInEG     == rhs.mCutVertexInEG     &&
             mEdgeCWInEG        == rhs.mEdgeCWInEG        &&
             mEdgeCCWInEG       == rhs.mEdgeCCWInEG       &&
             mRoleOfECWReversed == rhs.mRoleOfECWReversed ;
}

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_EMBEDDED_BCTREE_HPP_*/
