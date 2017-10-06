#include "gtest/gtest.h"
#include "directed/dfs_di.hpp"

namespace Wailea {

namespace Directed { 

using DDiExplorer = DFSDiExplorer <DiGraph,NumDiNode,DiEdge>;
using DDiStrategy = DFSDiStrategy <DiGraph,NumDiNode,DiEdge>;
using DDiGraph    = DFSDiGraph    <DiGraph,NumDiNode,DiEdge>;
using DDiNode     = DFSDiNode     <DiGraph,NumDiNode,DiEdge>;
using DDiEdge     = DFSDiEdge     <DiGraph,NumDiNode,DiEdge>;

class DFSDiTests : public ::testing::Test {

  protected:  

    std::unique_ptr<DDiGraph> generate_graph(){
        return std::make_unique<DDiGraph>();
    }
 
    std::unique_ptr<DDiNode> generate_node(int n){
        return std::make_unique<DDiNode>(n);
    }

    std::unique_ptr<DDiEdge> generate_edge(){
        return std::make_unique<DDiEdge>();
    }

    // DFSDiExploer delegates.
    node_list_it_t currentNodeIt(DDiExplorer& d){
        return d.mCurrentNodeIt;
    }

    void reset(DDiExplorer& d,DDiGraph& g, bool reversed){
        d.reset(g, reversed);
    }

    void setStartNode(DDiExplorer& d,DDiNode& s){
        d.setStartNode(s);
    }

    bool oneStep(DDiExplorer& d){
        return d.oneStep();
    }

    // DFSDiNode delegates.
    node_list_it_t parentInDFS(DDiNode& n){
        return n.mParentInDFS;
    }

    node_incidence_it_t edgeVisiting(DDiNode& n){
        return n.mEdgeVisiting;
    }

    node_incidence_it_t edgeToBeVisited(DDiNode& n){
        return n.mEdgeToBeVisited;
    }

    bool waitingForChild(DDiNode& n){
        return n.mWaitingForChild;
    }

    bool visited(DDiNode& n){
        return n.mVisited;
    }

    // DFSDiEdge delegates.
    bool visited(DDiEdge& e){
        return e.mVisited;
    }


    DFSDiTests(){;};
    virtual ~DFSDiTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


class DFSDiStrategySetNextEdge: public DFSDiStrategy <DiGraph,NumDiNode,DiEdge> {

  public:
    DFSDiStrategySetNextEdge():mSet(false){;}
    virtual ~DFSDiStrategySetNextEdge(){;}
    void setNextEdgeAtNextCall(
        node_list_it_t nodeIt,
        node_incidence_it_t incidenceIt)
    {
        mNodeIt = nodeIt;
        mIncidenceIt = incidenceIt;
        mSet = true;
    }

    void enterNode(DDiNode& n)
    {
        DFSDiStrategy<DiGraph,NumDiNode,DiEdge>::enterNode(n);
        if(mSet){
            (dynamic_cast<DDiNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void leaveNode(DDiNode& n)
    {
        DFSDiStrategy<DiGraph,NumDiNode,DiEdge>::leaveNode(n);
        if(mSet){
            (dynamic_cast<DDiNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void processFrond(DDiNode& n, DDiEdge& c )
    {
        DFSDiStrategy<DiGraph,NumDiNode,DiEdge>::processFrond(n,c);
        if(mSet){
            (dynamic_cast<DDiNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void beforeVisitingChild(DDiNode& n, DDiEdge& c )
    {
        DFSDiStrategy<DiGraph,NumDiNode,DiEdge>::beforeVisitingChild(n,c);
        if(mSet){
            (dynamic_cast<DDiNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void afterVisitingChild(DDiNode& n, DDiEdge& c )
    {
        DFSDiStrategy<DiGraph,NumDiNode,DiEdge>::afterVisitingChild(n,c);
        if(mSet){
            (dynamic_cast<DDiNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

  private:
    bool mSet;
    node_incidence_it_t mIncidenceIt;
    node_list_it_t mNodeIt;

};


/**  @brief DFS for a single node.
 */
TEST_F(DFSDiTests, Test1) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);
    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    setStartNode(dexpr,n1);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr);
    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    bool step2 = oneStep(dexpr);
    EXPECT_EQ(step2,false);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(currentNodeIt(dexpr),g1.nodes().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

}


/**  @brief DFS for two isolated nodes.
 */
TEST_F(DFSDiTests, Test2) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr);
    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(dstr.leaveNodeCnt,0);

    bool step2 = oneStep(dexpr);
    EXPECT_EQ(step2,false);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(currentNodeIt(dexpr),g1.nodes().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    setStartNode(dexpr,n2);

    EXPECT_EQ(currentNodeIt(dexpr),n2.backIt());
    EXPECT_EQ(parentInDFS(n2), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);

    bool step3 = oneStep(dexpr);
    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);
    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr),n2.backIt());
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    bool step4 = oneStep(dexpr);
    EXPECT_EQ(step4,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);
    EXPECT_EQ(currentNodeIt(dexpr),g1.nodes().second);

    EXPECT_EQ(dstr.enterNodeCnt,2);
    EXPECT_EQ(dstr.leaveNodeCnt,2);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

}


/**  @brief DFS for two nodes with one edge.
 */
TEST_F(DFSDiTests, Test3) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(e1), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // n1->n2

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // n2->n1 (going back)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step5 = oneStep(dexpr); // n1 come back from n2.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // n1 has no more edge to visit.

    EXPECT_EQ(step6,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


/**  @brief DFS for two nodes with two multi-edges.
 */
TEST_F(DFSDiTests, Test4) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n1));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n1,n1));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());

    // N1
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());


    bool step4 = oneStep(dexpr); // check the first edge (frond)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // leaveNode and move to n1.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step7 = oneStep(dexpr); // check the third (self-loop)

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.processFrondCnt,2);

    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++; n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step8 = oneStep(dexpr);// leave n1.

    EXPECT_EQ(step8,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS for three nodes with two edges from the leaf.
TEST_F(DFSDiTests, Test5) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check the second edge (new tree arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // enter n3.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // leaving n3.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step8 = oneStep(dexpr); // leaving n2.

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    bool step10 = oneStep(dexpr); // leaving n1.

    EXPECT_EQ(step10,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS for three edges with two edges from the middle point.
TEST_F(DFSDiTests, Test6) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n1,n3));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // leaving n2

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step5 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge =  n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // check the next edge (new)

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);

    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step7 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // leaving n3

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // has come back from n3 to n1.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step10 = oneStep(dexpr); // leave n1

    EXPECT_EQ(step10,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with good sized realistic graph.
//     including hasexplored.
TEST_F(DFSDiTests, Test7) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n3));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n1,n2));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n1,n4));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n2,n3));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n2,n4));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n3,n4));

    std::unique_ptr<DDiEdge> ep7 = generate_edge();
    DDiEdge& e7 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep7),n3,n5));

    std::unique_ptr<DDiEdge> ep8 = generate_edge();
    DDiEdge& e8 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep8),n4,n5));


    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);
    EXPECT_EQ(visited(e7), false);
    EXPECT_EQ(visited(e8), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 new arc.

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step3 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step4 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step5 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step6 = oneStep(dexpr); // Check e8 (new arc)

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e8), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step7 = oneStep(dexpr); // Enter n5

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step8 = oneStep(dexpr); // no more edge going from n5

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    auto n4_edge = n4.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // come back from n5 to n4 on e8.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step10 = oneStep(dexpr); // no more edge going from n4

    EXPECT_EQ(step10,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step11 = oneStep(dexpr); // come back from n4 to n3 on e6.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    auto n3_edge = n3.incidentEdgesOut().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step12 = oneStep(dexpr); // Check e3 (frond)

    EXPECT_EQ(step12,true);
    EXPECT_EQ(dstr.processFrondCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step13 = oneStep(dexpr); // no more edge going from n3

    EXPECT_EQ(step13,true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step14 = oneStep(dexpr); // come back from n3 to n1 on e1.

    EXPECT_EQ(step14,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step15 = oneStep(dexpr); // Check e2 new arc.

    EXPECT_EQ(step15,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step16 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step16,true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step17 = oneStep(dexpr); // Check e4 (frond)

    EXPECT_EQ(step17, true);
    EXPECT_EQ(dstr.processFrondCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step18 = oneStep(dexpr); // Check e5 (frond)

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.processFrondCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // no more edge going from n2

    EXPECT_EQ(step19,true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step20 = oneStep(dexpr); // come back from n2 to n1 on e2.

    EXPECT_EQ(step20,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesOut().first;
    n1_edge++;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // Check e3 (frond)

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.processFrondCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),    n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // no more edge going from n1

    EXPECT_EQ(step22,false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);
}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the first node right after entering n2.
// Set the next node while visiting other nodes.
TEST_F(DFSDiTests, Test8) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiNode> np7 = generate_node(7);
    DDiNode& n7 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DDiNode> np8 = generate_node(8);
    DDiNode& n8 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np8)));


    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n2,n4));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n2,n5));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n2,n6));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n2,n7));

    std::unique_ptr<DDiEdge> ep7 = generate_edge();
    DDiEdge& e7 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep7),n2,n8));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(visited(n8), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);
    EXPECT_EQ(visited(e7), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // set the next visiting edge to e4 (n5).
    // This has not effect on the first edge to be explored
    // right after entering.
    auto n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge);
    // remove n3 e2, n4, and e3 on the fly.
    g1.removeNode(n3);
    g1.removeNode(n4);
    
    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check e4 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e4), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step5 = oneStep(dexpr); // Enter n5.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    // Remove n6, n7, and n8 while visiting n5.
    g1.removeNode(n6);
    g1.removeNode(n7);
    g1.removeNode(n8);
    n2_edge = n2.incidentEdgesOut().second;    
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge);

    bool step6 = oneStep(dexpr); // Leaving n5.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // Come back from n5 to n2.

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step8 = oneStep(dexpr); // Leaving n2.

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step10 = oneStep(dexpr); // Leave n1.

    EXPECT_EQ(step10,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge to e6(n7) in beforeVisitingChild(n2,e3).
TEST_F(DFSDiTests, Test9) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiNode> np7 = generate_node(7);
    DDiNode& n7 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n2,n4));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n2,n5));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n2,n6));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n2,n7));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(visited(n7), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // Check e2 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step5, true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step7, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // Set the next edge to e6(n7) in beforeVisitingChild(n2,e3).
    auto n2_edge_set = n2.incidentEdgesOut().first;
    n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge_set);

    // remove e4, n5 e5, and n6 on the fly.
    g1.removeNode(n5);
    g1.removeNode(n6);

    bool step8 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step8, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);// Setting new edge to e6 .
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step9, true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step10 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step10, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step11 = oneStep(dexpr); // has come back from n4 to n2.

    EXPECT_EQ(step11, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step15 = oneStep(dexpr); // Check e6 (new arc)

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step16 = oneStep(dexpr); // Enter n7

    EXPECT_EQ(step16, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step18 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // has come back from n7 to n2.

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step22, false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge to e6(n7) in afterVisitingChild(n2,e3).
TEST_F(DFSDiTests, Test10) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiNode> np7 = generate_node(7);
    DDiNode& n7 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n2,n4));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n2,n5));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n2,n6));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n2,n7));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(visited(n7), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // Check e2 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step5, true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step7, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step9, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step11, true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step13 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // Set the next edge to e6(n7) in afterVisitingChild(n2,e3).
    auto n2_edge_set = n2.incidentEdgesOut().first;
    n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge_set);

    // remove e4, n5 e5, and n6 on the fly.
    g1.removeNode(n5);
    g1.removeNode(n6);

    bool step14 = oneStep(dexpr); // has come back from n4 to n2.

    EXPECT_EQ(step14, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2_edge_set);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step15 = oneStep(dexpr); // Check e6 (new arc)

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step16 = oneStep(dexpr); // Enter n7

    EXPECT_EQ(step16, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step18 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // has come back from n7 to n2.

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step22, false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge in processFrond()
TEST_F(DFSDiTests, Test11) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n3,n4));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n3,n1));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n3,n5));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n3,n6));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, false);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (new arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // enter n2.

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check e1 (new arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // enter n3

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // check e3 (new arc)

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    auto n3_edge = n3.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // no more edge.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step12 = oneStep(dexpr); // has come from n4 to n3.

    EXPECT_EQ(step12, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    // Set the next edge to e4(n6) in processFrond(n3,e4).
    auto n3_edge_set = n3.incidentEdgesOut().first;
    n3_edge_set++;n3_edge_set++;n3_edge_set++;
    dstr.setNextEdgeAtNextCall(n3.backIt(),n3_edge_set);

    bool step13 = oneStep(dexpr); // edge e4 (frond)

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.processFrondCnt,1);
    EXPECT_EQ(visited(e4), true);   

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step14 = oneStep(dexpr); // edge e6 (new arc)

    EXPECT_EQ(step14, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);   

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step15 = oneStep(dexpr); // Enter n6.

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step17 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step17, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesOut().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    auto n6_edge =  n6.incidentEdgesOut().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step18 = oneStep(dexpr); // has come from n6 to n3.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdgesOut().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step19 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesOut().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // has come from n3 to n2
    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step21 = oneStep(dexpr); // no more edge
    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // has come from n2 to n1

    EXPECT_EQ(step22, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step24 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step24, false);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesOut().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesOut().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesOut().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


/**  @brief DFS for a single node. Reversed.
 */
TEST_F(DFSDiTests, Test12) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);
    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    setStartNode(dexpr,n1);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr);
    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    bool step2 = oneStep(dexpr);
    EXPECT_EQ(step2,false);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(currentNodeIt(dexpr),g1.nodes().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

}


/**  @brief DFS for two isolated nodes. Reversed.
 */
TEST_F(DFSDiTests, Test13) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr);
    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(dstr.leaveNodeCnt,0);

    bool step2 = oneStep(dexpr);
    EXPECT_EQ(step2,false);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(currentNodeIt(dexpr),g1.nodes().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    setStartNode(dexpr,n2);

    EXPECT_EQ(currentNodeIt(dexpr),n2.backIt());
    EXPECT_EQ(parentInDFS(n2), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);

    bool step3 = oneStep(dexpr);
    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);
    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr),n2.backIt());
    EXPECT_EQ(dstr.leaveNodeCnt,1);
    bool step4 = oneStep(dexpr);
    EXPECT_EQ(step4,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);
    EXPECT_EQ(currentNodeIt(dexpr),g1.nodes().second);

    EXPECT_EQ(dstr.enterNodeCnt,2);
    EXPECT_EQ(dstr.leaveNodeCnt,2);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

}


/**  @brief DFS for two nodes with one edge. Reversed.
 */
TEST_F(DFSDiTests, Test14) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(e1), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // n1->n2

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // n2->n1 (going back)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step5 = oneStep(dexpr); // n1 come back from n2.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // n1 has no more edge to visit.

    EXPECT_EQ(step6,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


/**  @brief DFS for two nodes with two multi-edges. Reversed.
 */
TEST_F(DFSDiTests, Test15) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n1,n2));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n1,n1));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());

    // N1
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());


    bool step4 = oneStep(dexpr); // check the first edge (frond)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // leaveNode and move to n1.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step7 = oneStep(dexpr); // check the third (self-loop)

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.processFrondCnt,2);

    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++; n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step8 = oneStep(dexpr);// leave n1.

    EXPECT_EQ(step8,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS for three nodes with two edges from the leaf. Reversed.
TEST_F(DFSDiTests, Test16) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n3,n2));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check the second edge (new tree arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // enter n3.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // leaving n3.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step8 = oneStep(dexpr); // leaving n2.

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    bool step10 = oneStep(dexpr); // leaving n1.

    EXPECT_EQ(step10,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS for three edges with two edges from the middle point. Reversed.
TEST_F(DFSDiTests, Test17) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n3,n1));

    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // leaving n2

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step5 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge =  n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // check the next edge (new)

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);

    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step7 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // leaving n3

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // has come back from n3 to n1.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step10 = oneStep(dexpr); // leave n1

    EXPECT_EQ(step10,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with good sized realistic graph.
//     including hasexplored. Reversed.
TEST_F(DFSDiTests, Test18) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n3,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n2,n1));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n4,n1));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n3,n2));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n4,n2));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n4,n3));

    std::unique_ptr<DDiEdge> ep7 = generate_edge();
    DDiEdge& e7 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep7),n5,n3));

    std::unique_ptr<DDiEdge> ep8 = generate_edge();
    DDiEdge& e8 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep8),n5,n4));


    DDiStrategy dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);
    EXPECT_EQ(visited(e7), false);
    EXPECT_EQ(visited(e8), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 new arc.

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step3 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step4 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step5 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step6 = oneStep(dexpr); // Check e8 (new arc)

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e8), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step7 = oneStep(dexpr); // Enter n5

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step8 = oneStep(dexpr); // no more edge going from n5

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    auto n4_edge = n4.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // come back from n5 to n4 on e8.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step10 = oneStep(dexpr); // no more edge going from n4

    EXPECT_EQ(step10,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step11 = oneStep(dexpr); // come back from n4 to n3 on e6.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    auto n3_edge = n3.incidentEdgesIn().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step12 = oneStep(dexpr); // Check e3 (frond)

    EXPECT_EQ(step12,true);
    EXPECT_EQ(dstr.processFrondCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step13 = oneStep(dexpr); // no more edge going from n3

    EXPECT_EQ(step13,true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step14 = oneStep(dexpr); // come back from n3 to n1 on e1.

    EXPECT_EQ(step14,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step15 = oneStep(dexpr); // Check e2 new arc.

    EXPECT_EQ(step15,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step16 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step16,true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step17 = oneStep(dexpr); // Check e4 (frond)

    EXPECT_EQ(step17, true);
    EXPECT_EQ(dstr.processFrondCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step18 = oneStep(dexpr); // Check e5 (frond)

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.processFrondCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // no more edge going from n2

    EXPECT_EQ(step19,true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step20 = oneStep(dexpr); // come back from n2 to n1 on e2.

    EXPECT_EQ(step20,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdgesIn().first;
    n1_edge++;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // Check e3 (frond)

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.processFrondCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),    n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // no more edge going from n1

    EXPECT_EQ(step22,false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);
}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the first node right after entering n2.
// Set the next node while visiting other nodes. Reversed.
TEST_F(DFSDiTests, Test19) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiNode> np7 = generate_node(7);
    DDiNode& n7 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DDiNode> np8 = generate_node(8);
    DDiNode& n8 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np8)));


    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n3,n2));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n4,n2));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n5,n2));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n6,n2));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n7,n2));

    std::unique_ptr<DDiEdge> ep7 = generate_edge();
    DDiEdge& e7 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep7),n8,n2));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(visited(n8), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);
    EXPECT_EQ(visited(e7), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // set the next visiting edge to e4 (n5).
    // This has not effect on the first edge to be explored
    // right after entering.
    auto n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge);
    // remove n3 e2, n4, and e3 on the fly.
    g1.removeNode(n3);
    g1.removeNode(n4);
    
    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check e4 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e4), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step5 = oneStep(dexpr); // Enter n5.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    // Remove n6, n7, and n8 while visiting n5.
    g1.removeNode(n6);
    g1.removeNode(n7);
    g1.removeNode(n8);
    n2_edge = n2.incidentEdgesIn().second;    
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge);

    bool step6 = oneStep(dexpr); // Leaving n5.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // Come back from n5 to n2.

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step8 = oneStep(dexpr); // Leaving n2.

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step10 = oneStep(dexpr); // Leave n1.

    EXPECT_EQ(step10,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge to e6(n7) in beforeVisitingChild(n2,e3). Reversed.
TEST_F(DFSDiTests, Test20) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiNode> np7 = generate_node(7);
    DDiNode& n7 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n3,n2));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n4,n2));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n5,n2));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n6,n2));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n7,n2));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(visited(n7), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // Check e2 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step5, true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step7, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // Set the next edge to e6(n7) in beforeVisitingChild(n2,e3).
    auto n2_edge_set = n2.incidentEdgesIn().first;
    n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge_set);

    // remove e4, n5 e5, and n6 on the fly.
    g1.removeNode(n5);
    g1.removeNode(n6);

    bool step8 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step8, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);// Setting new edge to e6 .
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step9, true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step10 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step10, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step11 = oneStep(dexpr); // has come back from n4 to n2.

    EXPECT_EQ(step11, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step15 = oneStep(dexpr); // Check e6 (new arc)

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step16 = oneStep(dexpr); // Enter n7

    EXPECT_EQ(step16, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step18 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // has come back from n7 to n2.

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step22, false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge to e6(n7) in afterVisitingChild(n2,e3). Reversed.
TEST_F(DFSDiTests, Test21) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiNode> np7 = generate_node(7);
    DDiNode& n7 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n3,n2));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n4,n2));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n5,n2));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n6,n2));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n7,n2));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(visited(n7), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // Check e2 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step5, true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step7, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step9, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step11, true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step13 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // Set the next edge to e6(n7) in afterVisitingChild(n2,e3).
    auto n2_edge_set = n2.incidentEdgesIn().first;
    n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge_set);

    // remove e4, n5 e5, and n6 on the fly.
    g1.removeNode(n5);
    g1.removeNode(n6);

    bool step14 = oneStep(dexpr); // has come back from n4 to n2.

    EXPECT_EQ(step14, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2_edge_set);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step15 = oneStep(dexpr); // Check e6 (new arc)

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step16 = oneStep(dexpr); // Enter n7

    EXPECT_EQ(step16, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step18 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // has come back from n7 to n2.

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step22, false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge in processFrond(). Reversed.
TEST_F(DFSDiTests, Test22) {

    std::unique_ptr<DDiGraph> gp1 = generate_graph();
    DDiGraph&   g1 = *gp1;

    std::unique_ptr<DDiNode> np1 = generate_node(1);
    DDiNode& n1 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DDiNode> np2 = generate_node(2);
    DDiNode& n2 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DDiNode> np3 = generate_node(3);
    DDiNode& n3 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DDiNode> np4 = generate_node(4);
    DDiNode& n4 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DDiNode> np5 = generate_node(5);
    DDiNode& n5 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DDiNode> np6 = generate_node(6);
    DDiNode& n6 = dynamic_cast<DDiNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DDiEdge> ep1 = generate_edge();
    DDiEdge& e1 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep1),n2,n1));

    std::unique_ptr<DDiEdge> ep2 = generate_edge();
    DDiEdge& e2 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep2),n3,n2));

    std::unique_ptr<DDiEdge> ep3 = generate_edge();
    DDiEdge& e3 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep3),n4,n3));

    std::unique_ptr<DDiEdge> ep4 = generate_edge();
    DDiEdge& e4 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep4),n1,n3));

    std::unique_ptr<DDiEdge> ep5 = generate_edge();
    DDiEdge& e5 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep5),n5,n3));

    std::unique_ptr<DDiEdge> ep6 = generate_edge();
    DDiEdge& e6 = dynamic_cast<DDiEdge&>(g1.addEdge(std::move(ep6),n6,n3));

    DFSDiStrategySetNextEdge dstr;
    DDiExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1, true);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(visited(n6), false);

    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);
    EXPECT_EQ(visited(e3), false);
    EXPECT_EQ(visited(e4), false);
    EXPECT_EQ(visited(e5), false);
    EXPECT_EQ(visited(e6), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (new arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // enter n2.

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check e1 (new arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // enter n3

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // check e3 (new arc)

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    auto n3_edge = n3.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // no more edge.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step12 = oneStep(dexpr); // has come from n4 to n3.

    EXPECT_EQ(step12, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    // Set the next edge to e4(n6) in processFrond(n3,e4).
    auto n3_edge_set = n3.incidentEdgesIn().first;
    n3_edge_set++;n3_edge_set++;n3_edge_set++;
    dstr.setNextEdgeAtNextCall(n3.backIt(),n3_edge_set);

    bool step13 = oneStep(dexpr); // edge e4 (frond)

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.processFrondCnt,1);
    EXPECT_EQ(visited(e4), true);   

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step14 = oneStep(dexpr); // edge e6 (new arc)

    EXPECT_EQ(step14, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);   

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step15 = oneStep(dexpr); // Enter n6.

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step17 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step17, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdgesIn().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    auto n6_edge =  n6.incidentEdgesIn().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step18 = oneStep(dexpr); // has come from n6 to n3.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdgesIn().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step19 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdgesIn().first;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // has come from n3 to n2
    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step21 = oneStep(dexpr); // no more edge
    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // has come from n2 to n1

    EXPECT_EQ(step22, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step24 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step24, false);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdgesIn().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdgesIn().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdgesIn().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}

} // namespace Directed

} // namespace Wailea
