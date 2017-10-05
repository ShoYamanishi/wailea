#ifndef _WAILEA_UNDIRECTED_JTS_TREE_HPP_
#define _WAILEA_UNDIRECTED_JTS_TREE_HPP_

#include "undirected/base.hpp"

/**
 * @file undirected/jts_tree.hpp
 *
 * @brief an implementation of O(|N|^2) PQ-tree used by
 *        JTSPlanarizer.
 *        In this implementation, a tree node, whether it is of type P or Q,
 *        has all the children in std::list, and
 *        all the children point to the correct parent all the time wheather
 *        they are pertinent or not.
 */
namespace Wailea {

namespace Undirected {

using namespace std;


/**
 * @class JTSTree
 *
 * @brief represents a PQ-tree.
 */
class JTSTree : public Graph {


#ifdef UNIT_TESTS
  public:
#else
  private:
#endif

    inline JTSTree();
    inline virtual ~JTSTree();


    /**
     *  @brief utilities to access the tree node.
     */
    inline JTSTreeNode& toNodeRef(node_list_it_t nIt);
    inline bool isNil(node_list_it_t nIt);
    inline node_list_it_t nil();


    node_list_it_t makeInitialAttachmentP();


    inline void initializeForOneIteration();

  /**
   * ======== JTSPlanarizer::bubbleUp() specific. =========
   */


    /** @brief explore from a pertinent leaf, find the pertinent root.
     *         The pertinent root is the node that has multiple pertinent
     *         children, but its parent has onlye one pertinent child.
     */
    node_list_it_t findPertinentRoot(node_list_it_t leaf);


  /**
   * ======== JTSPlanarizer::removeNonconsecutiveNodes() specific. =========
   */


    /** @brief if there is only one child in the given node, bring the contents
     *         of the child to the node. The contents are:
     *           mNodeType
     *           mChildren
     *           mPertinentType
     *           mPertinentChildrenCount;
     *           mGraphEdge
     *         This is called in JTSPlanarizer::removePertinentTree(),
     *         and hence mPartialChild1, mPartialChild2, and 
     *         mFullChildrenCount are not yet set.
     */
    void transferContentsOfOnlyChildAndRemove(JTSTreeNode& X);


    /** @brief performs the following:
     *         - if X is W and L, then mark the graph edge removed, and
     *           then remove the node, which decreases mPertinentChildrenCount
     *           in its parent.
     *         - if X has no child, then remove the node, which descreases
     *           mPertinentChildrenCount.
     *         - If X has only one child, then the contents of it are brought
     *           up to this node
     *         - If X is not removed, and its mPertinentChildrenCount becomes
     *           0, then it changes mPErtinentType to Empty, and then 
     *           decreases mPertinentChildrenCount in its parent.
     */
    void removePertinentNodeIfNecessary(JTSTreeNode& X);


  /**
   * ======== JTSPlanarizer::applyTemplates() specific. =========
   */

    bool templateL1(JTSTreeNode& X);
    bool templateP1(JTSTreeNode& X);
    bool templateP2(JTSTreeNode& X, node_list_it_t& pertinentRoot);
    bool templateP3(JTSTreeNode& X);
    bool templateP4(JTSTreeNode& X, node_list_it_t& pertinentRoot);
    bool templateP5(JTSTreeNode& X);
    bool templateP6(JTSTreeNode& X, node_list_it_t& pertinentRoot);
        void concatenateOnePartialToTheOtherP6(
            JTSTreeNode& M,
            JTSTreeNode& A
        );
    bool templateP7(JTSTreeNode& X);
        void concatenateOnePartialToTheOtherP7(
            JTSTreeNode& M,
            JTSTreeNode& A
        );
    bool templateP8(JTSTreeNode& X);

    bool templateQ1(JTSTreeNode& X);
    bool templateQ2(JTSTreeNode& X);
        bool checkSequenceQ2(JTSTreeNode& X);
    bool templateQ3(JTSTreeNode& X);
        bool checkSequenceQ3(JTSTreeNode& X);
        void unlinkFromParent(JTSTreeNode& P, list<node_list_it_t>& children);
        node_list_it_t makePNode(list<node_list_it_t>& fullChildren);

    bool templateQ4(JTSTreeNode& X);
        bool checkSequenceQ4(JTSTreeNode& X);

        void flattenPartialChildToMiddleOfPartialNode(
            JTSTreeNode&   P,
            node_list_it_t cIt,
            bool fullInTheMiddle
        );

    bool templateQ5(JTSTreeNode& X);
    

  /**
   * ======== JTSPlanarizer::removePertinentTree() specific. =========
   */

    void resetNodeToPAttachment(JTSTreeNode& X);

    void removeNodeAndDescendants(JTSTreeNode& X);

    node_list_it_t makePAttachmentBefore(
        JTSTreeNode& P,
        list<node_list_it_t>::iterator insertBefore
    );



    void fanOutLeavesFromAttachment(
        JTSTreeNode&          A,
        list<edge_list_it_t>& edgeList
    );


    /** @brief indicates the current iteration of PQ-tree reduction.
     *         This is incremented after every bubbleup and reduction.
     *         If JTSTreeNode's mGeneration agrees with it,
     *         it is a pertinent node.
     */
    unsigned long long   mGeneration;



friend class JTSPlanarizer;
friend class JTSTreeNode;

#ifdef UNIT_TESTS
    void printTree(ostream& os);
  friend class JTSPlanarizerTests;
#endif

};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_JTS_TREE_HPP_*/
