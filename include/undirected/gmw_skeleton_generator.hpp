#ifndef _WAILEA_UNDIRECTED_GMW_SKELETON_GENERATOR_HPP_
#define _WAILEA_UNDIRECTED_GMW_SKELETON_GENERATOR_HPP_

#include "undirected/base.hpp"
#include "undirected/bctree.hpp"
#include "undirected/spqr_tree.hpp"


/**
 * @file undirected/gmw_skeleton_generator.hpp
 *
 * @brief It takes a set of edges along an SPQR-tree, and finds
 *        the corresponding expanded skeleton along it.
 *        It uses TreeSplitter to find the set of 
 *        component tree nodes for each node along the SPQR-path.
 *        It uses SPQRComponentNode::utility to indicate the
 *        generation it belongs to to identify the skeleton
 *                             
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/** @class GMWSkeletonNode
 *
 *  @brief copied node in the skeleton
 */
class GMWSkeletonNode : public Node {};


/** @class GMWSkeletonEdge
 *
 *  @brief copied node in the skeleton
 */
class GMWSkeletonEdge : public Edge {

  public:

    bool           mVirtual;  // Set if this is a virtual edge.

};


/** @class GMWSkeleton
 *
 *  @brief it holds necessary information to represent a skeleton of an 
 *         SPQR-tree R node.
 *         It holds the pointers (iterators) to the nodes in the biconnected 
 *         block represented by Block.
 *         It also holds the two terminal features, which are either node or 
 *         an edge.
 *         If a terminal feature is an edge, it is a virtual edge and hence
 *         there is no corresponding edge in the original block.
 *         (If there is a real edge between those incident nodes, then it will
 *          be in a P-node, which will be ignored in the skeleton generation.)
 */
class GMWSkeleton {

  public:
    enum type {
                Unknown,
                END1_NODE_END2_NODE,
                END1_NODE_END2_EDGE,
                END1_EDGE_END2_NODE,
                END1_EDGE_END2_EDGE  
              };


    /** @brief  default constructor
     */
    inline GMWSkeleton() noexcept 
        :mType(Unknown),mSkel(make_unique<Graph>()){;};

    /** @brief copy constructor is not allowed.
     */
    GMWSkeleton(const GMWSkeleton& rhs) = delete;


    /** @brief  move-constructs to this.
     *
     *  @param  rhs  (in): GMWSkeleton whose contents are swapped with this.
     */
    inline GMWSkeleton(GMWSkeleton&& rhs) noexcept;


    /** @brief copy assignment operator is not allowed.
     */
    GMWSkeleton& operator=(const GMWSkeleton& rhs) = delete;


    /** @brief  move-assigns to this.
     *
     *  @param  rhs  (in): GMWSKeleton whose contents are swapped with this.
     *
     *  @return reference to this.
     */
    inline GMWSkeleton& operator=(GMWSkeleton&& rhs) noexcept;


    /** @brief  destructs this object
     */
    inline virtual ~GMWSkeleton() noexcept {;};


  private:
    enum type              mType;


    /* @brief this contains the component nodes in the block.
     */
    vector<node_list_it_t> mBlockNodes;

    /** @brief if type is END1_NODE_*, it points to the
     *         terminal node 1 in the block.
     *         if type is END1_EDGE_* is points to the
     *         first incident node of the terminal edge 1.
     */
    node_list_it_t         mBlockNit11;

    /** @brief if type is END1_EDGE*, it points to the
     *         second incident node of the terminal edge 1.
     */
    node_list_it_t         mBlockNit12;

    /** @brief if type is *_END2_NODE, it points to the
     *         terminal node 2 in the block.
     *         if type is *_END2_EDGE is points to the
     *         first incident node of the terminal edge 2.
     */
    node_list_it_t         mBlockNit21;

    /** @brief if type is *_END2_EDGE, it points to the
     *         second incident node of the terminal edge 2.
     */
    node_list_it_t         mBlockNit22;


    /** @brief generate a skeleton as a copy of subgraph in Block.
     */
    void generateSkeleton();


    /** @brief skeleton graph. A copy of the subgraph specified by mBlockNodes.
     */
    std::unique_ptr<Graph> mSkel;

  public:

    inline Graph& graph();

    /** @brief terminal node 1 in mSkel.
     */
    node_list_it_t         mSkelNit1;

    /** @brief terminal edge 1 in mSkel.
     */
    edge_list_it_t         mSkelEit1;

    /** @brief terminal node 2 in mSkel.
     */
    node_list_it_t         mSkelNit2;

    /** @brief terminal edge 2 in mSkel.
     */
    edge_list_it_t         mSkelEit2;

  friend class GMWSkeletonGenerator;

#ifdef UNIT_TESTS
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};


Graph& GMWSkeleton::graph(){ return *mSkel; }


GMWSkeleton::GMWSkeleton(GMWSkeleton&& rhs) noexcept :
    mType(rhs.mType),
    mBlockNodes(std::move(rhs.mBlockNodes)),
    mBlockNit11(rhs.mBlockNit11),
    mBlockNit12(rhs.mBlockNit12),
    mBlockNit21(rhs.mBlockNit21),
    mBlockNit22(rhs.mBlockNit22),
    mSkel(std::move(rhs.mSkel)),
    mSkelNit1(rhs.mSkelNit1),
    mSkelEit1(rhs.mSkelEit1),
    mSkelNit2(rhs.mSkelNit2),
    mSkelEit2(rhs.mSkelEit2) {;}

GMWSkeleton& GMWSkeleton::operator=(GMWSkeleton&& rhs) noexcept
{
    if (this != &rhs) {

        mType       = rhs.mType;
        mBlockNodes = std::move(rhs.mBlockNodes);
        mBlockNit11 = rhs.mBlockNit11;
        mBlockNit12 = rhs.mBlockNit12;
        mBlockNit21 = rhs.mBlockNit21;
        mBlockNit22 = rhs.mBlockNit22;
        mSkel       = std::move(rhs.mSkel);
        mSkelNit1   = rhs.mSkelNit1;
        mSkelEit1   = rhs.mSkelEit1;
        mSkelNit2   = rhs.mSkelNit2;
        mSkelEit2   = rhs.mSkelEit2;

    }
    return *this;
}



/** @class GMWSkeletonGenerator
 *
 *  @brief it takes a planar biconnected block B and its two nodes BN1& BN2,
 *         and find an ordered list of expanded skeletons along the tree path 
 *         between BN1 and BN2.
 *
 *         1. decomposes the block B to an SPQR-tree T.
 *
 *         2. finds a minimal path (TN1,TE1, TN2, TE2, ... TNm-1, TEm-1, TNm)
 *            between BN1 and BN2.
 *
 *         3. decompose T into components with (TE1,TE2,...TEm-1) as the cut
 *            cut set. The components are denoted by (C1, C2, ..., C2m).
 *            The component Ci contains TNi, and probably some other tree
 *            nodes.
 *            If the path consists of a single tree node TN1, then:
 *              The underlying SPQR component of C1 contains both BN1 and BN2.
 *            Otherwise:
 *              - C1's underlying SPQRComponent contains BN1 and  
 *                the virtual edge that corresponds to TE1.
 *              - The underlying SPQR component of Cm contains the virtual edge
 *                that corresponds to TEm-1, and BN2.
 *              - The underlying SPQR component of Ci (i!=1 AND i!=m), contains
 *                two virtual edges that correspond to TEi-1 and TEi.
 *
 *         4. for each component Ci:
 *             Find the subset of the nodes BSi in B that corresponds to Ci.
 *             If the path consists of a single tree node TN1, then:
 *               BN1 and BN2 are in BS1, and they are returned together with 
 *               BS1.
 *             Otherwise:
 *               - BS1 corresponds to C1, and BN1 and the incident nodes to 
 *                 the virtual edge are also returned together with BS1.
 *               - BSm corresponds to Cm, and BNm and the incident nodes to 
 *                 the virtual edge are also returned together with BSm.
 *               - Any other BSi corresponds to Ci, and the 4 incident nodes
 *                 to the 2 virtual edges are also returned together with BSi.
 *             Please note that there is no corresponding edge in B to a
 *             virtual edge. This is the reason why two incident nodes are
 *             returned above.
 *
 *  @remark It uses BlockNode::utility to find the set of skeleton nodes
 *          for each tree component. The previous value held is destroyed.
 */
class GMWSkeletonGenerator {


  public:            

    GMWSkeletonGenerator(){;}
    ~GMWSkeletonGenerator(){;}

    /** @brief main function . It takes a block with two terminal nodes in it.
     *
     *  @param  B (in):     block
     *
     *  @param  bnit1 (in): pointer to the terminal node 1.
     *
     *  @param  bnit2 (in): pointer to the terminal node 2.
     *
     *  @remark This destroys Block::utility value.
     */
    void processBlock(
        Block&         B,
        node_list_it_t bnit1,
        node_list_it_t bnit2
    );


    /** @brief returns the number of skeletons generated.
     */
    size_t numSkeletons() const;


    /** @brief returns the skeleton specified by i
     */
    GMWSkeleton& skeleton(size_t i);

  private:

    void findTreeNodesFromBlockNodes(
        SPQRTree&       T,
        node_list_it_t  bnit1,
        node_list_it_t  bnit2,
        node_list_it_t& tnit1,
        node_list_it_t& tnit2
    );


    void findMinimalTreePath(
        list<node_list_it_t>& treePathNodes,
        list<edge_list_it_t>& treePathEdges,
        node_list_it_t        bnit1,
        node_list_it_t        bnit2
    );

    vector<node_list_it_t> fromTreeNodesToSkeletonNodes(
                                               list<node_list_it_t> treeNodes);

    bool isVirtualEdge(
        node_list_it_t tnit,
        node_list_it_t bnit1,
        node_list_it_t bnit2
    );

    vector<GMWSkeleton> mSkeletons;

#ifdef UNIT_TESTS
  friend class GMWSkeletonTests;
  friend class GMWSkeletonGeneratorTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_GMW_SKELETON_GENERATOR_HPP_*/
