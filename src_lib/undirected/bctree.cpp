#include "undirected/bctree.hpp"

namespace Wailea {

namespace Undirected { 

using namespace std;

const utility_t ConstantsBCTree::kForwardTypeCutVertex = 0;
const utility_t ConstantsBCTree::kForwardTypeBlockNode = 1;
const char*     ConstantsBCTree::kExceptionBCTreeInvalidNodeType
                      = "BCTreeNode is not a valid type for this operation";


vector<node_list_it_t> BCTree::cutVertexNodes()
{
    vector<node_list_it_t> vec;

    auto itPair = nodes();
    for (auto it = itPair.first; it != itPair.second; it++) {

        BCTreeNode& n = dynamic_cast<BCTreeNode&>(*(*it));
        if (n.type() == BCTreeNode::CutVertexType) {
            vec.push_back(it);
        }

    }

    return vec; // rvo
}


vector<node_list_it_t> BCTree::blockNodes()
{
    vector<node_list_it_t> vec;

    auto itPair = nodes();
    for (auto it = itPair.first; it != itPair.second; it++) {

        BCTreeNode& n = dynamic_cast<BCTreeNode&>(*(*it));
        if (n.type() == BCTreeNode::BlockType) {
            vec.push_back(it);
        }

    }

    return vec; // rvo
}


vector<node_list_it_t> Block::cutVertices() {

    vector<node_list_it_t> cvs;
    auto itPair = nodes();
    for (auto it = itPair.first;it != itPair.second; it++) {
        auto& n = dynamic_cast<BlockNode&>(*(*it));
        if (n.type() == BlockNode::CutVertexType) {
            cvs.push_back(it);
        }
    }
    return cvs; // rvo

}


BiconnectedDecomposer::BCStrategy::BCStrategy(
    node_list_it_t s,
    BCTree&        bcTree
) noexcept : mI(0),mRootNode(s),mBCTree(bcTree){;}


BCTree BiconnectedDecomposer::decompose(Graph& g, Node& s)
{
    if (s.degree() == 0) {

        return processSingleIsolatedVertex(g, s);

    }
    else {

        DFSGraph<Graph,BCDFSNode,BCDFSEdge> gCopy;
        auto sCopy = copyInputGraph(g, gCopy, s.backIt());
        BCTree     bcTree(g); 
        BCStrategy strategy(sCopy, bcTree);
        DFSExplorer<Graph,BCDFSNode,BCDFSEdge> mExplorer(gCopy,strategy);
        mExplorer.reset(gCopy);
        mExplorer.run(dynamic_cast<DFSNode<Graph,BCDFSNode,BCDFSEdge>&>
                                                               (*(*sCopy)));
        return bcTree; // rvo

    }
} 


BCTree BiconnectedDecomposer::decompose(Graph& g)
{
    if (g.numNodes()==0) {
        // Handle empty graph.
        BCTree bcTree(g);      
        return bcTree; // rvo
    }
    else{
        Node& s = *(*g.nodes().first);
        return decompose(g,s);// rvo
    }
} 


BCTree BiconnectedDecomposer::processSingleIsolatedVertex(Graph& g, Node& n)
{
    BCTree bcTree(g);
    auto  bp    = make_unique<Block>();
    auto& tn    = dynamic_cast<BCTreeNode&>(
                          bcTree.addNode(make_unique<BCTreeNode>(move(bp))) );
    auto& block = tn.block();
    block.setBackTreeNode(tn.backIt());
    auto& bn = block.addNode(make_unique<BlockNode>(
                                    n.backIt(), BlockNode::OrdinaryNodeType ));

    n.pushIGForwardLink(bn.backIt());
    n.pushUtility(ConstantsBCTree::kForwardTypeBlockNode);

    return bcTree; // rvo
}


node_list_it_t BiconnectedDecomposer::copyInputGraph(
    Graph&         src,
    Graph&         dst,
    node_list_it_t startNode
){
    // Make room for forward pointers in the original graph.
    src.pushDefaultIGForwardLinkToNodes();
    src.pushDefaultUtilityToNodes();
    src.pushDefaultIGForwardLinkToEdges();

    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;       
    Node* startNodeCopyGraph;
    auto nitPair = src.nodes();
    for (auto nit = nitPair.first; nit != nitPair.second; nit++) {
        auto np = make_unique<DFSNode<Graph,BCDFSNode,BCDFSEdge>>();
        np->pushIGBackwardLink(nit);
        if (nit == startNode) {
            startNodeCopyGraph = &(*np);
        }
        nodePairs.push_back(make_pair(nit,std::move(np)));
    }
    auto eitPair = src.edges();
    for (auto eit = eitPair.first; eit != eitPair.second; eit++) {
        auto ep = make_unique<DFSEdge<Graph,BCDFSNode,BCDFSEdge>>();
        ep->pushIGBackwardLink(eit);
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }
    src.copySubgraph(nodePairs, edgePairs, dst);

    return startNodeCopyGraph->backIt();        
}


void BiconnectedDecomposer::BCStrategy::enterNode(DNode& n) noexcept
{
//    NumNode& NN = dynamic_cast<NumNode&>(*(*(n.mOriginal)));
//    cerr << "enterNode: " << NN.num() << " mNum: ";

    mI++;
    n.mNum = mI;
    n.mLowPt = mI;
//    cerr << n.mNum << "\n";
}

void BiconnectedDecomposer::BCStrategy::leaveNode(DNode& n) noexcept {
//    NumNode& NN = dynamic_cast<NumNode&>(*(*(n.mOriginal)));
//    cerr << "leaveNode: " << NN.num() << "\n";
}

void BiconnectedDecomposer::BCStrategy::processFrond(DNode& n, DEdge& c)
{
//    auto& NN = dynamic_cast<NumNode&>(*(*(n.mOriginal)));
//    auto& DA = dynamic_cast<DNode&>(c.adjacentNode(n));
//    auto& NN2 = dynamic_cast<NumNode&>(*(*(DA.mOriginal)));
//    cerr << "processFrond: (" << NN.num() << "," << NN2.num() << ") lowPt: ";

    auto& adj = dynamic_cast<DNode&>(c.adjacentNode(n));        
    mStack.push_back(c.backIt());
    n.mLowPt = std::min(n.mLowPt, adj.mNum);
//    cerr << n.mLowPt << "\n";
}


void BiconnectedDecomposer::BCStrategy::beforeVisitingChild(DNode& n, DEdge& c)
{
//    auto& NN = dynamic_cast<NumNode&>(*(*(n.mOriginal)));
//    auto& DA = dynamic_cast<DNode&>(c.adjacentNode(n));
//    auto& NN2 = dynamic_cast<NumNode&>(*(*(DA.mOriginal)));
//    cerr << "beforeVisitingChild: (" << NN.num() << "," << NN2.num() << ")\n";

    mStack.push_back(c.backIt());
}


void BiconnectedDecomposer::BCStrategy::afterVisitingChild(DNode& n, DEdge& c)
{
//    auto& NN = dynamic_cast<NumNode&>(*(*(n.mOriginal)));
//    auto& DA = dynamic_cast<DNode&>(c.adjacentNode(n));
//    auto& NN2 = dynamic_cast<NumNode&>(*(*(DA.mOriginal)));
//    cerr << "afterVisitingChild: (" << NN.num() << "," << NN2.num() << ") lowPt: ";

    auto& adj = dynamic_cast<DNode&>(c.adjacentNode(n));
    n.mLowPt = std::min(n.mLowPt,adj.mLowPt);
//    cerr << n.mLowPt << "\n";
    if (adj.mLowPt >= n.mNum) {
        if (n.backIt() != mRootNode) {
            // Not a root node. n is a cut vertex.
            if (n.mCutVertex == false) {
                processNewlyFoundCutVertex(n);
            }
            processNewlyFoundBlock(n, c,  mStack);
        }                        
        else if (isThereUnexplored(n)) {
            // n is a root but this is a cut vertex.
            if (n.mCutVertex == false) {
                processNewlyFoundCutVertex(n);
            }
            processNewlyFoundBlock(n, c, mStack);
        }
        else if (!mStack.empty()) { 
            // Processing the final block. n may not be a cut vertex.
            processNewlyFoundBlock(n, c, mStack);
        }
    }
}


void BiconnectedDecomposer::BCStrategy::processNewlyFoundCutVertex
                                                                (BCDFSNode& n)
{
    auto& cv = mBCTree.addNode(make_unique<BCTreeNode>(n.IGBackwardLink()));
    n.mBCTreeNodeCutVertex = cv.backIt();
    n.mCutVertex = true;
    (*n.IGBackwardLink())->setIGForwardLink(cv.backIt());
    (*n.IGBackwardLink())->setUtility(ConstantsBCTree::kForwardTypeCutVertex);
}


void BiconnectedDecomposer::BCStrategy::processNewlyFoundBlock(
                 BCDFSNode& n, BCDFSEdge& e, vector<edge_list_it_t>& edgeStack)
{
    vector<edge_list_it_t> blockEdges;
    while (edgeStack.size() > 0) {
        auto seit = *(edgeStack.rbegin());
        edgeStack.pop_back();
        blockEdges.push_back(seit);
        if (e.backIt() == seit) {
            break;
        }
    }
    auto bp = make_unique<Block>();
    auto& tn = mBCTree.addNode(make_unique<BCTreeNode>(move(bp)));
    fillBlockAndConnectBCTreeNodes(dynamic_cast<BCTreeNode&>(tn),blockEdges);
}


void BiconnectedDecomposer::BCStrategy::fillBlockAndConnectBCTreeNodes(
                                 BCTreeNode& tn, vector<edge_list_it_t>& eCopy)
{
    auto& block = tn.block();
    block.setBackTreeNode(tn.backIt());
    auto& gCopy = (*(*(eCopy[0]))).graph();
    vector<node_list_it_t> nCopy = gCopy.edgeInducedNodes(eCopy);
    vector<pair<node_list_it_t,node_ptr_t>> nodePairs;
    for (auto nit : nCopy) {
        auto& n = dynamic_cast<BCDFSNode&>(*(*nit));
        if (n.mCutVertex) {
            auto np = make_unique<BlockNode>(n.IGBackwardLink(),
                                                     BlockNode::CutVertexType);
            // Cut vertex on Node1 of ep.
            auto ep = make_unique<BCTreeEdge>(true);
            auto& e = mBCTree.addEdge(
                                    move(ep),*(*(n.mBCTreeNodeCutVertex)),tn);
            np->setBCTreeEdge(e.backIt());
            nodePairs.push_back(make_pair(nit,std::move(np)));
        }
        else {
            auto np = make_unique<BlockNode>(n.IGBackwardLink(),
                                                  BlockNode::OrdinaryNodeType);
            nodePairs.push_back(make_pair(nit,std::move(np)));
        }
    }
    vector<pair<edge_list_it_t,edge_ptr_t>> edgePairs;       
    for (auto eit : eCopy) {
        auto& e = dynamic_cast<BCDFSEdge&>(*(*eit));
        auto ep = make_unique<BlockEdge>(e.IGBackwardLink());
        edgePairs.push_back(make_pair(eit,std::move(ep)));
    }
    gCopy.copySubgraph(nodePairs, edgePairs, block);
    // Patch up BCTreeEdge's iterator.
    auto bnPair = block.nodes();
    for (auto bnit = bnPair.first ; bnit != bnPair.second ; bnit++) {
        auto&  n = dynamic_cast<BlockNode&>(*(*bnit));
        if (n.type()==BlockNode::CutVertexType) {
            auto&  e = n.treeEdge();
            e.setCutVertexInBlock(n.backIt());
        }
        else {
            n.IGBackwardLinkRef().setIGForwardLink(bnit);
            n.IGBackwardLinkRef().setUtility(ConstantsBCTree::kForwardTypeBlockNode);
        }
    }

    auto bePair = block.edges();
    for (auto beit = bePair.first ; beit != bePair.second ; beit++) {

        auto&  e = dynamic_cast<BlockEdge&>(*(*beit));
        e.IGBackwardLinkRef().setIGForwardLink(beit);
    }
}

}// namespace Undirected

}// namespace Wailea
