#include "gtest/gtest.h"
#include "undirected/jts_planarizer.hpp"
#include "undirected/jts_tree.hpp"
#include "undirected/jts_tree_node.hpp"
#include "undirected/jts_inline.hpp"
#include "undirected/st_numbering.hpp"

namespace Wailea {

namespace Undirected {

class JTSPlanarizerTests : public ::testing::Test {

  protected:

    JTSPlanarizerTests(){;};
    virtual ~JTSPlanarizerTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    JTSTree& getPQTree();

    JTSTree  mTree;
    void copyInputGraph(
        JTSPlanarizer& planarizer,
        Graph&                  src,
        vector<node_list_it_t>& stOrderSrc,
        JTSGraph&               dst,
        vector<node_list_it_t>& stOrderDst,
        JTSTree&                pqTree);

    node_list_it_t makeInitialAttachmentP(JTSTree& tr);

    void fanOutLeavesFromAttachment(
        JTSTree&              tr,
        JTSTreeNode&          A,
	list<edge_list_it_t>& edgeList
    );

    void fanOutLeavesFromDummyAttachment(
        JTSTree&              tr,
        JTSTreeNode&          A,
        list<edge_list_it_t>& edgeList
    );


    JTSTreeNode& toNodeRef(JTSTree& tr, node_list_it_t nIt);

    vector<node_list_it_t> fromGraphEdgesToPQLeaves(
        JTSPlanarizer& planarizer,
        JTSTree& pqTree,
        list<edge_list_it_t>& edgeList
    );

    void initializeForOneIteration(JTSTree& pqTree);

    node_list_it_t bubbleUp(
        JTSPlanarizer& planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves
    );

    void calculateWHAC(JTSTreeNode& tn);

    void findWHA(
        JTSPlanarizer&          planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t          pertinentRoot
    );


    void determineRootNodeType(
        JTSTreeNode& tn
    );

    void determineWHABCTypeForChildren(
        JTSTreeNode& tn
    );


    void determineWHABCNodeTypes(
        JTSPlanarizer& planarizer,
        JTSTree&       pqTree,
        node_list_it_t pertinentRoot
    );


    void removeNonconsecutiveNodes(
        JTSPlanarizer&          planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         pertinentRoot
    );


    bool templateL1(JTSTree& tr, JTSTreeNode& X);
    bool templateP1(JTSTree& tr, JTSTreeNode& X);
    bool templateP2(JTSTree& tr,JTSTreeNode& X,node_list_it_t& pertinentRoot);
    bool templateP3(JTSTree& tr, JTSTreeNode& X);
    bool templateP4(JTSTree& tr,JTSTreeNode& X,node_list_it_t& pertinentRoot);
    bool templateP5(JTSTree& tr, JTSTreeNode& X);
    bool templateP6(JTSTree& tr,JTSTreeNode& X,node_list_it_t& pertinentRoot);
    bool templateP7(JTSTree& tr,JTSTreeNode& X);
    bool templateP8(JTSTree& tr,JTSTreeNode& X);
    bool templateQ1(JTSTree& tr, JTSTreeNode& X);
    bool templateQ2(JTSTree& tr, JTSTreeNode& X);
    bool templateQ3(JTSTree& tr, JTSTreeNode& X);
    bool templateQ4(JTSTree& tr, JTSTreeNode& X);
    bool templateQ5(JTSTree& tr, JTSTreeNode& X);

    node_list_it_t applyTemplates(
        JTSPlanarizer&          planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t          pertinentRoot
    );


    node_list_it_t removePertinentTree(
        JTSPlanarizer& planarizer,
        JTSTree&       pqTree,
        node_list_it_t pertinentRoot
    );

    void makePlanarSubgraphAndRemovedEdges(
        JTSPlanarizer&          planarizer,
        Graph&                  originalGraph,
        JTSGraph&               internalGraph,
        PlanarizableGraph&      planarSubgraph,
        vector<edge_list_it_t>& removedEdges
    );



};


void JTSPlanarizerTests::makePlanarSubgraphAndRemovedEdges(
    JTSPlanarizer&          planarizer,
    Graph&                  originalGraph,
    JTSGraph&               internalGraph,
    PlanarizableGraph&      planarSubgraph,
    vector<edge_list_it_t>& removedEdges
) {
    planarizer.makePlanarSubgraphAndRemovedEdges(originalGraph, internalGraph, planarSubgraph, removedEdges);
}

node_list_it_t JTSPlanarizerTests::removePertinentTree(
    JTSPlanarizer& planarizer,
    JTSTree&       pqTree,
    node_list_it_t pertinentRoot
) {
    return planarizer.removePertinentTree(pqTree, pertinentRoot);
}


node_list_it_t JTSPlanarizerTests::applyTemplates(
    JTSPlanarizer&          planarizer,
    JTSTree&                pqTree,
    vector<node_list_it_t>& pertinentLeaves,
    node_list_it_t          pertinentRoot
) {
    return planarizer.applyTemplates(pqTree, pertinentLeaves, pertinentRoot);
}


bool JTSPlanarizerTests::templateL1(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateL1(X);
}

bool JTSPlanarizerTests::templateP1(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateP1(X);
}

bool JTSPlanarizerTests::templateP2(
    JTSTree& tr,
    JTSTreeNode& X,
    node_list_it_t& pertinentRoot
) {
    return tr.templateP2(X, pertinentRoot);
}

bool JTSPlanarizerTests::templateP3(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateP3(X);
}

bool JTSPlanarizerTests::templateP4(
    JTSTree& tr,
    JTSTreeNode& X,
    node_list_it_t& pertinentRoot
) {
    return tr.templateP4(X, pertinentRoot);
}

bool JTSPlanarizerTests::templateP5(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateP5(X);
}

bool JTSPlanarizerTests::templateP6(
    JTSTree& tr,
    JTSTreeNode& X,
    node_list_it_t& pertinentRoot
) {
    return tr.templateP6(X, pertinentRoot);
}

bool JTSPlanarizerTests::templateP7(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateP7(X);
}

bool JTSPlanarizerTests::templateP8(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateP8(X);
}

bool JTSPlanarizerTests::templateQ1(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateQ1(X);
}

bool JTSPlanarizerTests::templateQ2(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateQ2(X);
}

bool JTSPlanarizerTests::templateQ3(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateQ3(X);
}


bool JTSPlanarizerTests::templateQ4(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateQ4(X);
}


bool JTSPlanarizerTests::templateQ5(
    JTSTree& tr,
    JTSTreeNode& X
) {
    return tr.templateQ5(X);
}


void JTSPlanarizerTests::removeNonconsecutiveNodes(
        JTSPlanarizer&          planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t&         pertinentRoot
) {
    planarizer.removeNonconsecutiveNodes(pqTree, pertinentLeaves, pertinentRoot);
}


void JTSPlanarizerTests::determineWHABCNodeTypes(
    JTSPlanarizer& planarizer,
    JTSTree&       pqTree,
    node_list_it_t pertinentRoot
){
    planarizer.determineWHABCNodeTypes(pqTree, pertinentRoot);
}


void JTSPlanarizerTests:: determineRootNodeType(
    JTSTreeNode& tn
) {
    tn.determineRootNodeType();
}


void JTSPlanarizerTests:: determineWHABCTypeForChildren(
    JTSTreeNode& tn
) {
    tn.determineWHABCTypeForChildren();
}


void JTSPlanarizerTests::findWHA(
        JTSPlanarizer&          planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves,
        node_list_it_t          pertinentRoot
) {
    planarizer.findWHA(pqTree, pertinentLeaves, pertinentRoot);
}


void JTSPlanarizerTests::calculateWHAC(JTSTreeNode& tn)
{
    tn.calculateWHAC();
}


void JTSPlanarizerTests::initializeForOneIteration(JTSTree& pqTree)
{
    pqTree.initializeForOneIteration();
}


node_list_it_t JTSPlanarizerTests::bubbleUp(
        JTSPlanarizer& planarizer,
        JTSTree&                pqTree,
        vector<node_list_it_t>& pertinentLeaves
){
    return planarizer.bubbleUp(pqTree, pertinentLeaves);
}


vector<node_list_it_t> JTSPlanarizerTests::fromGraphEdgesToPQLeaves(
    JTSPlanarizer& planarizer,
    JTSTree& pqTree,
    list<edge_list_it_t>& edgeList
) {
    return planarizer.fromGraphEdgesToPQLeaves(pqTree, edgeList);
}


JTSTreeNode& JTSPlanarizerTests::toNodeRef(JTSTree& tr, node_list_it_t nIt)
{
    return tr.toNodeRef(nIt);
}

void JTSPlanarizerTests::fanOutLeavesFromAttachment(
    JTSTree&              tr,
    JTSTreeNode&          A,
    list<edge_list_it_t>& edgeList
)
{
    return tr.fanOutLeavesFromAttachment(A, edgeList);
}


void JTSPlanarizerTests::fanOutLeavesFromDummyAttachment(
    JTSTree&              tr,
    JTSTreeNode&          A,
    list<edge_list_it_t>& edgeList
) {
    for(auto eIt : edgeList) {

        auto& E = dynamic_cast<JTSGraphEdge&>(*(*eIt));

        auto  np  = make_unique<JTSTreeNode>(tr);
        auto  nIt = tr.addNode(std::move(np)).backIt();
        auto& N   = tr.toNodeRef(nIt);

        N.mNodeType  = JTSTreeNode::LType;
        N.mGraphEdge = eIt;
        E.mPQLeaf    = nIt;
        E.mRemoved   = false;

        N.linkToParent(A);
    }
}



JTSTree& JTSPlanarizerTests::getPQTree()
{
    return mTree;
}


void JTSPlanarizerTests::copyInputGraph(
    JTSPlanarizer& planarizer,
    Graph&                  src,
    vector<node_list_it_t>& stOrderSrc,
    JTSGraph&               dst,
    vector<node_list_it_t>& stOrderDst,
    JTSTree&                pqTree)
{
    planarizer.copyInputGraph(src,stOrderSrc,dst,stOrderDst,pqTree);
}


node_list_it_t JTSPlanarizerTests::makeInitialAttachmentP(JTSTree& tr)
{
    return tr.makeInitialAttachmentP();
}


static bool cmp_edges_unordered(
    list<edge_list_it_t>& cmp1,
    list<edge_list_it_t>& cmp2
) {
    if( cmp1.size() != cmp2.size() ){
        return false;
    }
    vector<int> checkFlags;
    for (size_t i = 0; i < cmp1.size(); i++) {
        checkFlags.push_back(0);
    }
    size_t index = 0;
    for (auto c1 : cmp1) {
        for (auto c2 : cmp2) {
            if (c1==c2) {
                 checkFlags[index]++;
            }
        }
        index++;
    }
    for (auto f : checkFlags) {
        if (f != 1) {
            return false;
        }
    }

    return true;
}


static bool cross_check(
    list<edge_list_it_t>& edges,
    list<node_list_it_t>& treeLeaves
) {
    if (edges.size() != treeLeaves.size()) {
        return false;
    }

    vector<int> checkFlags;
    for (size_t i = 0; i < edges.size(); i++) {
        checkFlags.push_back(0);
    }
    size_t index = 0;
    for (auto eIt : edges) {
        auto& e = dynamic_cast<JTSGraphEdge&>(*(*eIt));
        for (auto tlIt : treeLeaves) {
            auto& tl = dynamic_cast<JTSTreeNode&>(*(*tlIt));
            if (e.mPQLeaf == tlIt && tl.mGraphEdge == eIt) {
                checkFlags[index]++;
            }
        }
        index++;
    }

    for (auto f : checkFlags) {
        if (f != 1) {
            return false;
        }
    }

    return true;
}


static bool cross_check(
    list<edge_list_it_t>&   edges,
    vector<node_list_it_t>& treeLeaves
) {
    if (edges.size() != treeLeaves.size()) {
        return false;
    }

    vector<int> checkFlags;
    for (size_t i = 0; i < edges.size(); i++) {
        checkFlags.push_back(0);
    }
    size_t index = 0;
    for (auto eIt : edges) {
        auto& e = dynamic_cast<JTSGraphEdge&>(*(*eIt));
        for (auto tlIt : treeLeaves) {
            auto& tl = dynamic_cast<JTSTreeNode&>(*(*tlIt));
            if (e.mPQLeaf == tlIt && tl.mGraphEdge == eIt) {
                checkFlags[index]++;
            }
        }
        index++;
    }

    for (auto f : checkFlags) {
        if (f != 1) {
            return false;
        }
    }

    return true;
}


static bool parent_check(JTSTreeNode& N)
{
    for (auto cIt = N.mChildren.begin(); cIt != N.mChildren.end(); cIt++) {
        auto& C = dynamic_cast<JTSTreeNode&>(*(*(*cIt)));
        if (C.mChildIt != cIt) {
            return false;
        }
        if (C.mParent != N.backIt()){
            return false;
        }
    }
    return true;
}


static JTSTreeNode& createP(JTSTree& pqTree, list<node_list_it_t>& children)
{
    auto  np    = make_unique<JTSTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& P     = pqTree.toNodeRef(nIt);
    P.mNodeType = JTSTreeNode::PType;
    for (auto cIt : children) {
        auto& C = dynamic_cast<JTSTreeNode&>(*(*cIt));
        C.mChildIt = P.mChildren.insert(P.mChildren.end(),cIt);
        C.mParent = nIt;
    }
    return P;
}


static JTSTreeNode& createQ(JTSTree& pqTree, list<node_list_it_t>& children)
{
    auto  np    = make_unique<JTSTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& Q     = pqTree.toNodeRef(nIt);
    Q.mNodeType = JTSTreeNode::QType;
    for (auto cIt : children) {
        auto& C = dynamic_cast<JTSTreeNode&>(*(*cIt));
        C.mChildIt = Q.mChildren.insert(Q.mChildren.end(),cIt);
        C.mParent = nIt;
    }
    return Q;
}


static JTSTreeNode& createL(JTSTree& pqTree)
{
    auto  np    = make_unique<JTSTreeNode>(pqTree);
    auto  nIt   = pqTree.addNode(std::move(np)).backIt();
    auto& L     = pqTree.toNodeRef(nIt);
    L.mNodeType = JTSTreeNode::LType;
    return L;
}


static JTSGraphEdge& createE(JTSGraph& G, JTSTreeNode& L)
{
    auto  np1   = make_unique<JTSGraphNode>();
    auto& n1    = G.addNode(std::move(np1));
    auto  np2   = make_unique<JTSGraphNode>();
    auto& n2    = G.addNode(std::move(np2));
    auto  ep    = make_unique<JTSGraphEdge>();
    auto& e     = dynamic_cast<JTSGraphEdge&>(G.addEdge(std::move(ep),n1, n2));
    e.mRemoved  = false;
    e.mPQLeaf   = L.backIt();
    L.mGraphEdge= e.backIt();
    return e;
}

#include "unit_tests/undirected/unit_tests_jts_planarizer_copyInputGraph.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_bubbleup.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_calculateWHAC.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_determineWHABCTypeForChildren.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_removeNonConsecutiveNodes.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_templates.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_rest.inc"
#include "unit_tests/undirected/unit_tests_jts_planarizer_templates2.inc"

} // namespace Undirected

} // namespace Wailea

