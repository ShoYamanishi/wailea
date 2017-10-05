#ifndef _WAILEA_UNDIRECTED_GMW_BC_PATH_HPP_
#define _WAILEA_UNDIRECTED_GMW_BC_PATH_HPP_

#include "undirected/base.hpp"
#include "undirected/bctree.hpp"

/**
 * @file undirected/gmw_bc_path.hpp
 *
 * @brief
 *
 * @remark
 *
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/** @class GMWBCPath
 *
 *  @brief it takes a simple connected graph G in
 *         with two designated nodes N1 and N2 in it, and then
 *         returns one or two blocks to which the nodes belong.
 *
 *  1. It decomposes G into biconnected components in a BC-tree.
 *  2. It finds a path on BC-tree that corresponds to a path from N1 to N2.
 *  There are several cases to consider.
 *  - If both N1 and N2 are cut vertices, then {N1,N2} can be added to G
 *    without breaking planarity.
 *  - If neither  N1 or N2 is a cut vertex, and if both belong to a same
 *    block, then it returns the block with the pointers to the nodes in it
 *    that correspond to N1 and N2.
 *    
 *  - If N1 (N2) belongs to a block B1 (B2), then the path leaves B1 (B2) at
 *    a cutvertex C1 (C2) toward N2 (N1).
 *    It returns B1 (B2) with the pointers to the node that corresponds to N1
 *    (N2)  and the node to C1 (C2).
 */
class GMWBCPath {

  public:

    enum pathType {

        BC_PATH_TYPE_UNKNOWN,
        BC_PATH_BOTH_CUT_VERTICES,
        BC_PATH_BOTH_IN_THE_SAME_BLOCK,        
        BC_PATH_N1_IS_BLOCK_N2_IS_BLOCK,
        BC_PATH_N1_IS_BLOCK_N2_IS_CUT_VERTEX,
        BC_PATH_N1_IS_CUT_VERTEX_N2_IS_BLOCK,

    };


    /** @brief constructor
     *
     *  @param G    (in): simple connected graph to be examined.
     */
    GMWBCPath(Graph&G):mG(G),mBCTree(G){;}
    virtual ~ GMWBCPath(){;}

    /** @brief it takes a simple connected graph G in with 
     *        two nodes N1 and N2, decompose G into biconnected components
     *        in BC-tree, find a path in the tree from N1 to N2,
     *        determines if N1 and N2 are cutvertices or not, and if not,
     *        then find the blocks they belong to, and set the pathType.
     *
     *  @param n1It (in): pointer to N1
     *
     *  @param n2It (in): pointer to N2
     *
     */
    void processConnectedGraph(node_list_it_t n1It, node_list_it_t n2It);


    /** @brief returns the type of the path and the decomposition described
     *         in the class description.
     *
     *  @return type
     */
    pathType getPathType();
 

    /** @brief returns the block if both N1 and N2 belong to the same
     *         block.
     *
     *  @brief n1It (out): the pointer to the node in the block that 
     *                     corresponds to N1.
     *
     *  @brief n2It (out): the pointer to the node in the block that 
     *                     corresponds to N2.
     *
     *  @return block
     */
    Block& getSingleBlock(
        node_list_it_t& n1It,
        node_list_it_t& n2It
    );


    /** @brief returns the block to which N1 belongs.
     *
     *  @brief n1It (out): the pointer to the node in the block that 
     *                     corresponds to N1.
     *
     *  @brief cvIt (out): the pointer to the node in the block that 
     *                     corresponds to the cut vertexm at which
     *                     the path leaves toward N2.
     *
     *  @return block
     */
    Block& getBlock1(
        node_list_it_t& n1It,
        node_list_it_t& cvIt
    );


    /** @brief returns the block to which N2 belongs.
     *
     *  @brief n2It (out): the pointer to the node in the block that 
     *                     corresponds to N2.
     *
     *  @brief cvIt (out): the pointer to the node in the block that 
     *                     corresponds to the cut vertexm at which
     *                     the path leaves toward N1.
     *
     *  @return block
     */
    Block& getBlock2(
        node_list_it_t& n2It,
        node_list_it_t& cvIt
    );

  private:
    Graph&         mG;

    BCTree         mBCTree;

    pathType       mPathType;

    node_list_it_t mTNIt1;    // BC-tree node that corresponds to N1.
    node_list_it_t mTNIt2;    // BC-tree node that corresponds to N2.
    node_list_it_t mBNIt11;   // Component node in block 1 for N1.
    node_list_it_t mBNIt12;   // Component node in block 1 for the cut vertex.
    node_list_it_t mBNIt21;   // Component node in block 2 for the cut vertex.
    node_list_it_t mBNIt22;   // Component node in block 2 for N2.

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_GMW_BC_PATH_HPP_*/
