#include "gtest/gtest.h"
#include "undirected/dfs.hpp"

namespace Wailea {

namespace Undirected { 

using DExplorer = DFSExplorer <Graph,Node,Edge>;
using DStrategy = DFSStrategy <Graph,Node,Edge>;
using DGraph    = DFSGraph    <Graph,Node,Edge>;
using DNode     = DFSNode     <Graph,Node,Edge>;
using DEdge     = DFSEdge     <Graph,Node,Edge>;

class DFSTests : public ::testing::Test {

  protected:  

    std::unique_ptr<DGraph> generate_graph(){
        return std::make_unique<DGraph>();
    }
 
    std::unique_ptr<DNode> generate_node(){
        return std::make_unique<DNode>();
    }

    std::unique_ptr<DEdge> generate_edge(){
        return std::make_unique<DEdge>();
    }

    // DFSExploer delegates.
    node_list_it_t currentNodeIt(DExplorer& d){
        return d.mCurrentNodeIt;
    }

    void reset(DExplorer& d,DGraph& g){
        d.reset(g);
    }

    void setStartNode(DExplorer& d,DNode& s){
        d.setStartNode(s);
    }

    bool oneStep(DExplorer& d){
        return d.oneStep();
    }

    // DFSNode delegates.
    node_list_it_t parentInDFS(DNode& n){
        return n.mParentInDFS;
    }

    node_incidence_it_t edgeVisiting(DNode& n){
        return n.mEdgeVisiting;
    }

    node_incidence_it_t edgeToBeVisited(DNode& n){
        return n.mEdgeToBeVisited;
    }

    bool waitingForChild(DNode& n){
        return n.mWaitingForChild;
    }

    bool visited(DNode& n){
        return n.mVisited;
    }

    // DFSEdge delegates.
    bool visited(DEdge& e){
        return e.mVisited;
    }


    DFSTests(){;};
    virtual ~DFSTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

};


class DFSStrategySetNextEdge: public DFSStrategy <Graph,Node,Edge> {

  public:
    DFSStrategySetNextEdge():mSet(false){;}
    virtual ~DFSStrategySetNextEdge(){;}
    void setNextEdgeAtNextCall(
        node_list_it_t nodeIt,
        node_incidence_it_t incidenceIt)
    {
        mNodeIt = nodeIt;
        mIncidenceIt = incidenceIt;
        mSet = true;
    }

    void enterNode(DNode& n)
    {
        DFSStrategy<Graph,Node,Edge>::enterNode(n);
        if(mSet){
            (dynamic_cast<DNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void leaveNode(DNode& n)
    {
        DFSStrategy<Graph,Node,Edge>::leaveNode(n);
        if(mSet){
            (dynamic_cast<DNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void processFrond(DNode& n, DEdge& c )
    {
        DFSStrategy<Graph,Node,Edge>::processFrond(n,c);
        if(mSet){
            (dynamic_cast<DNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void beforeVisitingChild(DNode& n, DEdge& c )
    {
        DFSStrategy<Graph,Node,Edge>::beforeVisitingChild(n,c);
        if(mSet){
            (dynamic_cast<DNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
            mSet=false;
        }
    }

    void afterVisitingChild(DNode& n, DEdge& c )
    {
        DFSStrategy<Graph,Node,Edge>::afterVisitingChild(n,c);
        if(mSet){
            (dynamic_cast<DNode&>(*(*mNodeIt))).setNextDFSEdge(mIncidenceIt);
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
TEST_F(DFSTests, Test1) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    DStrategy dstr;
    DExplorer dexpr(g1,dstr);
    reset(dexpr,g1);

    EXPECT_EQ(visited(n1), false);
    setStartNode(dexpr,n1);
    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr);
    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
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
TEST_F(DFSTests, Test2) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    DStrategy dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr);
    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);
    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
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
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(dstr.enterNodeCnt,1);

    bool step3 = oneStep(dexpr);
    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);
    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);
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
TEST_F(DFSTests, Test3) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    DStrategy dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(e1), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // n1->n2

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // n2->n1 (already visited)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // n2->n1 (going back)

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // n1 come back from n2.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step7 = oneStep(dexpr); // n1 has no more edge to visit.

    EXPECT_EQ(step7,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


/**  @brief DFS for two nodes with two multi-edges.
 */
TEST_F(DFSTests, Test4) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n1,n2));

    std::unique_ptr<DEdge> ep3 = generate_edge();
    DEdge& e3 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep3),n1,n1));

    DStrategy dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    bool step1 = oneStep(dexpr); // Enter n1: call enterNode().

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2: call enterNode().

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check first edge (parent tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // check second edge (frond).

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.processFrondCnt,1);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step6 = oneStep(dexpr); // leaveNode and move to n1.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step7 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step8 = oneStep(dexpr); // check the second (frond already visited)

    EXPECT_EQ(step8,true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdges().first;
    n1_edge++; n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step9 = oneStep(dexpr); // check the third (self-loop)

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.processFrondCnt,2);

    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdges().first;
    n1_edge++; n1_edge++; n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step10 = oneStep(dexpr);//check the fourth(self-loop,already visited)

    EXPECT_EQ(step10,true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step11 = oneStep(dexpr);// leave n1.

    EXPECT_EQ(step11,false);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdges().first;
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS for three nodes with two edges from the leaf.
TEST_F(DFSTests, Test5) {

    DGraph&   g1 = *(generate_graph());

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    DStrategy dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check the first edge (parent arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // check the second edge (new tree arc)

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // enter n3.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step7 = oneStep(dexpr); // n3 check the first edge (parent)

    EXPECT_EQ(step7,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // leaving n3.

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step10 = oneStep(dexpr); // leaving n2.

    EXPECT_EQ(step10,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step11 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    bool step12 = oneStep(dexpr); // leaving n1.

    EXPECT_EQ(step12,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS for three edges with two edges from the middle point.
TEST_F(DFSTests, Test6) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n1,n3));

    DStrategy dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

    EXPECT_EQ(visited(n1), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(visited(e1), false);
    EXPECT_EQ(visited(e2), false);

    setStartNode(dexpr,n1);

    EXPECT_EQ(currentNodeIt(dexpr),n1.backIt());
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // call beforeVisitingChild()
                                 // Move to n2.
    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check the first edge (parent arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // leaving n2

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step6 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge =  n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step7 = oneStep(dexpr); // check the next edge (new)

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);

    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step9 = oneStep(dexpr); // check the first edge (parent arc)

    EXPECT_EQ(step9,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step10 = oneStep(dexpr); // leaving n3

    EXPECT_EQ(step10,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge =  n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step11 = oneStep(dexpr); // has come back from n3 to n1.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step12 = oneStep(dexpr); // leave n1

    EXPECT_EQ(step12,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with good sized realistic graph.
//     including hasexplored.
TEST_F(DFSTests, Test7) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DNode> np4 = generate_node();
    DNode& n4 = dynamic_cast<DNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DNode> np5 = generate_node();
    DNode& n5 = dynamic_cast<DNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n3));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n1,n2));

    std::unique_ptr<DEdge> ep3 = generate_edge();
    DEdge& e3 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep3),n1,n4));

    std::unique_ptr<DEdge> ep4 = generate_edge();
    DEdge& e4 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep4),n2,n3));

    std::unique_ptr<DEdge> ep5 = generate_edge();
    DEdge& e5 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep5),n2,n4));

    std::unique_ptr<DEdge> ep6 = generate_edge();
    DEdge& e6 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep6),n3,n4));

    std::unique_ptr<DEdge> ep7 = generate_edge();
    DEdge& e7 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep7),n3,n5));

    std::unique_ptr<DEdge> ep8 = generate_edge();
    DEdge& e8 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep8),n4,n5));


    DStrategy dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 new arc.

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step3 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step4 = oneStep(dexpr); // Check e1 (parent arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    auto n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step5 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e4), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step6 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step7 = oneStep(dexpr); // Check e2 (frond)

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.processFrondCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step8 = oneStep(dexpr); // Check e4 (parent arc)

    EXPECT_EQ(step8,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // Check e5 (new arc)

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e5), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step10 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step10,true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // Check e3 (frond)

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.processFrondCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    auto n4_edge = n4.incidentEdges().first;
    n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step12 = oneStep(dexpr); // Check e5 (parent arc)

    EXPECT_EQ(step12,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step13 = oneStep(dexpr); // Check e6 (frond)

    EXPECT_EQ(step13,true);
    EXPECT_EQ(dstr.processFrondCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step14 = oneStep(dexpr); // Check e8 (new arc)

    EXPECT_EQ(step14,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e8), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step15 = oneStep(dexpr); // n5 enter

    EXPECT_EQ(step15,true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step16 = oneStep(dexpr); // check e7 (frond)

    EXPECT_EQ(step16,true);
    EXPECT_EQ(dstr.processFrondCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    auto n5_edge = n5.incidentEdges().first;
    n5_edge++;
    EXPECT_EQ(edgeVisiting(n5), n5_edge);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step17 = oneStep(dexpr); // check e8 (parent arc)

    EXPECT_EQ(step17,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    n5_edge = n5.incidentEdges().first;
    n5_edge++; n5_edge++;
    EXPECT_EQ(edgeVisiting(n5), n5_edge);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step18 = oneStep(dexpr); // no more edge going from n5

    EXPECT_EQ(step18,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), true);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step19 = oneStep(dexpr); // come back from n5 to n4 on e8.

    EXPECT_EQ(step19,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    n4_edge = n4.incidentEdges().first;
    n4_edge++; n4_edge++; n4_edge++; n4_edge++;
    EXPECT_EQ(edgeVisiting(n4), n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step20 = oneStep(dexpr); // no more edge going from n4

    EXPECT_EQ(step20,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step21 = oneStep(dexpr); // come back from n4 to n2 on e5.

    EXPECT_EQ(step21,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++; n2_edge++; n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step22 = oneStep(dexpr); // no more edge from n2.

    EXPECT_EQ(step22,true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step23 = oneStep(dexpr); // come back from n2 to n3 on e4.

    EXPECT_EQ(step23,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step24 = oneStep(dexpr); // check e6 (frond already visited)

    EXPECT_EQ(step24,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step25 = oneStep(dexpr); // check e7 (frond already visited)

    EXPECT_EQ(step25,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step26 = oneStep(dexpr); // no more edge from n3.

    EXPECT_EQ(step26,true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step27 = oneStep(dexpr); // come back from n3 to n1 on e1.

    EXPECT_EQ(step27,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step28 = oneStep(dexpr); // check e2 (frond already visited.)

    EXPECT_EQ(step28,true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdges().first;
    n1_edge++;n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step29 = oneStep(dexpr); // check e3 (frond already visited.)

    EXPECT_EQ(step29,true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdges().first;
    n1_edge++;n1_edge++;n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step30 = oneStep(dexpr); // no more edge to visit.

    EXPECT_EQ(step30,false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n1.backIt());
    EXPECT_EQ(edgeVisiting(n3),n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n3.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4), n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n4.backIt());
    EXPECT_EQ(edgeVisiting(n5),  n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}



// DFS with realistic graph with on-the-fly incidence modification.
// Set the first node right after entering n2.
// Set the next node while visiting other nodes.
TEST_F(DFSTests, Test8) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DNode> np4 = generate_node();
    DNode& n4 = dynamic_cast<DNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DNode> np5 = generate_node();
    DNode& n5 = dynamic_cast<DNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DNode> np6 = generate_node();
    DNode& n6 = dynamic_cast<DNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DNode> np7 = generate_node();
    DNode& n7 = dynamic_cast<DNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DNode> np8 = generate_node();
    DNode& n8 = dynamic_cast<DNode&>(g1.addNode(std::move(np8)));


    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DEdge> ep3 = generate_edge();
    DEdge& e3 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep3),n2,n4));

    std::unique_ptr<DEdge> ep4 = generate_edge();
    DEdge& e4 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep4),n2,n5));

    std::unique_ptr<DEdge> ep5 = generate_edge();
    DEdge& e5 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep5),n2,n6));

    std::unique_ptr<DEdge> ep6 = generate_edge();
    DEdge& e6 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep6),n2,n7));

    std::unique_ptr<DEdge> ep7 = generate_edge();
    DEdge& e7 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep7),n2,n8));

    DFSStrategySetNextEdge dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check e4 (tree arc)

    EXPECT_EQ(step4,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e4), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), false);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step5 = oneStep(dexpr); // Enter n5.

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    // Remove n6, n7, and n8 while visiting n5.
    g1.removeNode(n6);
    g1.removeNode(n7);
    g1.removeNode(n8);
    n2_edge = n2.incidentEdges().second;    
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge);

    bool step6 = oneStep(dexpr); // Check e4 (parent arc)

    EXPECT_EQ(step6,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    n2_edge = n2.incidentEdges().second;
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    auto n5_edge = n5.incidentEdges().first;
    n5_edge++;
    EXPECT_EQ(edgeVisiting(n5), n5_edge);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n5.backIt());

    bool step7 = oneStep(dexpr); // Leaving n5.

    EXPECT_EQ(step7,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    n2_edge = n2.incidentEdges().second;
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step8 = oneStep(dexpr); // Come back from n5 to n2.

    EXPECT_EQ(step8,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // Leaving n2.

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step10 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step10,true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step11 = oneStep(dexpr); // Leave n1.

    EXPECT_EQ(step11,false);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2),n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n5), false);
    EXPECT_EQ(visited(n5), true);
    EXPECT_EQ(parentInDFS(n5), n2.backIt());
    EXPECT_EQ(edgeVisiting(n5), n5.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n5), n5.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge to e6(n7) in beforeVisitingChild(n2,e3).
TEST_F(DFSTests, Test9) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DNode> np4 = generate_node();
    DNode& n4 = dynamic_cast<DNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DNode> np5 = generate_node();
    DNode& n5 = dynamic_cast<DNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DNode> np6 = generate_node();
    DNode& n6 = dynamic_cast<DNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DNode> np7 = generate_node();
    DNode& n7 = dynamic_cast<DNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DEdge> ep3 = generate_edge();
    DEdge& e3 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep3),n2,n4));

    std::unique_ptr<DEdge> ep4 = generate_edge();
    DEdge& e4 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep4),n2,n5));

    std::unique_ptr<DEdge> ep5 = generate_edge();
    DEdge& e5 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep5),n2,n6));

    std::unique_ptr<DEdge> ep6 = generate_edge();
    DEdge& e6 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep6),n2,n7));

    DFSStrategySetNextEdge dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());
    
    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // Check e1 (parent arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // Check e2 (tree arc)

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step7 = oneStep(dexpr); // Check e2 (parent arc)

    EXPECT_EQ(step7, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    auto n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step8, true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step9, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // Set the next edge to e6(n7) in beforeVisitingChild(n2,e3).
    auto n2_edge_set = n2.incidentEdges().first;
    n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;
    dstr.setNextEdgeAtNextCall(n2.backIt(),n2_edge_set);

    // remove e4, n5 e5, and n6 on the fly.
    g1.removeNode(n5);
    g1.removeNode(n6);

    bool step10 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step10, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step11, true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step12 = oneStep(dexpr); // Check e2 (parent arc)

    EXPECT_EQ(step12, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step13 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2_edge_set);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step14 = oneStep(dexpr); // has come back from n4 to n2.

    EXPECT_EQ(step14, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step15 = oneStep(dexpr); // Check e6 (new arc)

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step16 = oneStep(dexpr); // Enter n7

    EXPECT_EQ(step16, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step17 = oneStep(dexpr); // Check e6 (parent arc)

    EXPECT_EQ(step17, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step18 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // has come back from n7 to n2.

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step22, false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge to e6(n7) in afterVisitingChild(n2,e3).
TEST_F(DFSTests, Test10) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DNode> np4 = generate_node();
    DNode& n4 = dynamic_cast<DNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DNode> np5 = generate_node();
    DNode& n5 = dynamic_cast<DNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DNode> np6 = generate_node();
    DNode& n6 = dynamic_cast<DNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DNode> np7 = generate_node();
    DNode& n7 = dynamic_cast<DNode&>(g1.addNode(std::move(np7)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DEdge> ep3 = generate_edge();
    DEdge& e3 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep3),n2,n4));

    std::unique_ptr<DEdge> ep4 = generate_edge();
    DEdge& e4 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep4),n2,n5));

    std::unique_ptr<DEdge> ep5 = generate_edge();
    DEdge& e5 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep5),n2,n6));

    std::unique_ptr<DEdge> ep6 = generate_edge();
    DEdge& e6 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep6),n2,n7));

    DFSStrategySetNextEdge dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (tree arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());
    
    bool step3 = oneStep(dexpr); // Enter n2

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // Check e1 (parent arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // Check e2 (tree arc)

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // Enter n3

    EXPECT_EQ(step6, true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3), n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step7 = oneStep(dexpr); // Check e2 (parent arc)

    EXPECT_EQ(step7, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    auto n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3), n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step8, true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step9 = oneStep(dexpr); // has come back from n3 to n2.

    EXPECT_EQ(step9, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());


    bool step10 = oneStep(dexpr); // Check e3 (new arc)

    EXPECT_EQ(step10, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step11, true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step12 = oneStep(dexpr); // Check e2 (parent arc)

    EXPECT_EQ(step12, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step13 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    // Set the next edge to e6(n7) in afterVisitingChild(n2,e3).
    auto n2_edge_set = n2.incidentEdges().first;
    n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;n2_edge_set++;
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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2_edge_set);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step15 = oneStep(dexpr); // Check e6 (new arc)

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), false);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step16 = oneStep(dexpr); // Enter n7

    EXPECT_EQ(step16, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step17 = oneStep(dexpr); // Check e6 (parent arc)

    EXPECT_EQ(step17, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n7.backIt());

    bool step18 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step19 = oneStep(dexpr); // has come back from n7 to n2.

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;n2_edge++;n2_edge++;n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step21 = oneStep(dexpr); // has come back from n2 to n1.

    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // No more edge.

    EXPECT_EQ(step22, false);
    EXPECT_EQ(dstr.leaveNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n2.backIt());
    EXPECT_EQ(edgeVisiting(n4),  n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n7), false);
    EXPECT_EQ(visited(n7), true);
    EXPECT_EQ(parentInDFS(n7), n2.backIt());
    EXPECT_EQ(edgeVisiting(n7),  n7.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n7), n7.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}


// DFS with realistic graph with on-the-fly incidence modification.
// Set the next edge in processFrond()
TEST_F(DFSTests, Test11) {

    std::unique_ptr<DGraph> gp1 = generate_graph();
    DGraph&   g1 = *gp1;

    std::unique_ptr<DNode> np1 = generate_node();
    DNode& n1 = dynamic_cast<DNode&>(g1.addNode(std::move(np1)));

    std::unique_ptr<DNode> np2 = generate_node();
    DNode& n2 = dynamic_cast<DNode&>(g1.addNode(std::move(np2)));

    std::unique_ptr<DNode> np3 = generate_node();
    DNode& n3 = dynamic_cast<DNode&>(g1.addNode(std::move(np3)));

    std::unique_ptr<DNode> np4 = generate_node();
    DNode& n4 = dynamic_cast<DNode&>(g1.addNode(std::move(np4)));

    std::unique_ptr<DNode> np5 = generate_node();
    DNode& n5 = dynamic_cast<DNode&>(g1.addNode(std::move(np5)));

    std::unique_ptr<DNode> np6 = generate_node();
    DNode& n6 = dynamic_cast<DNode&>(g1.addNode(std::move(np6)));

    std::unique_ptr<DEdge> ep1 = generate_edge();
    DEdge& e1 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep1),n1,n2));

    std::unique_ptr<DEdge> ep2 = generate_edge();
    DEdge& e2 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep2),n2,n3));

    std::unique_ptr<DEdge> ep3 = generate_edge();
    DEdge& e3 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep3),n3,n4));

    std::unique_ptr<DEdge> ep4 = generate_edge();
    DEdge& e4 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep4),n3,n1));

    std::unique_ptr<DEdge> ep5 = generate_edge();
    DEdge& e5 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep5),n3,n5));

    std::unique_ptr<DEdge> ep6 = generate_edge();
    DEdge& e6 = dynamic_cast<DEdge&>(g1.addEdge(std::move(ep6),n3,n6));

    DFSStrategySetNextEdge dstr;
    DExplorer dexpr(g1,dstr);

    EXPECT_EQ(dstr.enterNodeCnt,0);
    EXPECT_EQ(dstr.leaveNodeCnt,0);
    EXPECT_EQ(dstr.processFrondCnt,0);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,0);
    EXPECT_EQ(dstr.afterVisitingChildCnt,0);

    reset(dexpr,g1);

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
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);
    EXPECT_EQ(dstr.enterNodeCnt,0);

    bool step1 = oneStep(dexpr); // Enter n1

    EXPECT_EQ(step1,true);
    EXPECT_EQ(dstr.enterNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step2 = oneStep(dexpr); // Check e1 (new arc)

    EXPECT_EQ(step2,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,1);
    EXPECT_EQ(visited(e1), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), false);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step3 = oneStep(dexpr); // enter n2.

    EXPECT_EQ(step3,true);
    EXPECT_EQ(dstr.enterNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2), n2.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step4 = oneStep(dexpr); // check e1 (parent arc)

    EXPECT_EQ(step4,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    auto n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step5 = oneStep(dexpr); // check e1 (new arc)

    EXPECT_EQ(step5,true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,2);
    EXPECT_EQ(visited(e2), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), false);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step6 = oneStep(dexpr); // enter n3

    EXPECT_EQ(step6,true);
    EXPECT_EQ(dstr.enterNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),  n3.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step7 = oneStep(dexpr); // check e2 (parent arc)

    EXPECT_EQ(step7,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    auto n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step8 = oneStep(dexpr); // check e3 (new arc)

    EXPECT_EQ(step8, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,3);
    EXPECT_EQ(visited(e3), true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), false);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step9 = oneStep(dexpr); // Enter n4

    EXPECT_EQ(step9,true);
    EXPECT_EQ(dstr.enterNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step10 = oneStep(dexpr); // check e3 (parent arc)

    EXPECT_EQ(step10,true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    auto n4_edge = n4.incidentEdges().first;
    n4_edge++;
    EXPECT_EQ(edgeVisiting(n4),   n4_edge);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n4.backIt());

    bool step11 = oneStep(dexpr); // no more edge.

    EXPECT_EQ(step11,true);
    EXPECT_EQ(dstr.leaveNodeCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step12 = oneStep(dexpr); // has come from n4 to n3.

    EXPECT_EQ(step12, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,1);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());


    // Set the next edge to e4(n6) in processFrond(n3,e4).
    auto n3_edge_set = n3.incidentEdges().first;
    n3_edge_set++;n3_edge_set++;n3_edge_set++;n3_edge_set++;
    dstr.setNextEdgeAtNextCall(n3.backIt(),n3_edge_set);

    bool step13 = oneStep(dexpr); // edge e4 (frond)

    EXPECT_EQ(step13, true);
    EXPECT_EQ(dstr.processFrondCnt,1);
    EXPECT_EQ(visited(e4), true);   

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step14 = oneStep(dexpr); // edge e6 (new arc)

    EXPECT_EQ(step14, true);
    EXPECT_EQ(dstr.beforeVisitingChildCnt,4);
    EXPECT_EQ(visited(e6), true);   

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), false);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step15 = oneStep(dexpr); // Enter n6.

    EXPECT_EQ(step15, true);
    EXPECT_EQ(dstr.enterNodeCnt,5);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step16 = oneStep(dexpr); // check e5 (parent arc)

    EXPECT_EQ(step16, true);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    auto n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n6.backIt());

    bool step17 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step17, true);
    EXPECT_EQ(dstr.leaveNodeCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), true);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    n3_edge = n3.incidentEdges().first;
    n3_edge++;n3_edge++;n3_edge++;n3_edge++;
    EXPECT_EQ(edgeVisiting(n3),  n3_edge);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step18 = oneStep(dexpr); // has come from n6 to n3.

    EXPECT_EQ(step18, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,2);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n3.backIt());

    bool step19 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step19, true);
    EXPECT_EQ(dstr.leaveNodeCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), true);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    n2_edge = n2.incidentEdges().first;
    n2_edge++;
    EXPECT_EQ(edgeVisiting(n2), n2_edge);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step20 = oneStep(dexpr); // has come from n3 to n2
    EXPECT_EQ(step20, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,3);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n2.backIt());

    bool step21 = oneStep(dexpr); // no more edge
    EXPECT_EQ(step21, true);
    EXPECT_EQ(dstr.leaveNodeCnt,4);

    EXPECT_EQ(waitingForChild(n1), true);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1), n1.incidentEdges().first);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step22 = oneStep(dexpr); // has come from n2 to n1

    EXPECT_EQ(step22, true);
    EXPECT_EQ(dstr.afterVisitingChildCnt,4);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    auto n1_edge = n1.incidentEdges().first;
    n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step23 = oneStep(dexpr); // check e4 (frond already visited)

    EXPECT_EQ(step23, true);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    n1_edge = n1.incidentEdges().first;
    n1_edge++;n1_edge++;
    EXPECT_EQ(edgeVisiting(n1), n1_edge);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    n6_edge =  n6.incidentEdges().first;
    n6_edge++;
    EXPECT_EQ(edgeVisiting(n6),   n6_edge);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), n1.backIt());

    bool step24 = oneStep(dexpr); // no more edge

    EXPECT_EQ(step24, false);

    EXPECT_EQ(waitingForChild(n1), false);
    EXPECT_EQ(visited(n1), true);
    EXPECT_EQ(parentInDFS(n1), g1.nodes().second);
    EXPECT_EQ(edgeVisiting(n1),  n1.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n1), n1.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n2), false);
    EXPECT_EQ(visited(n2), true);
    EXPECT_EQ(parentInDFS(n2), n1.backIt());
    EXPECT_EQ(edgeVisiting(n2),  n2.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n2), n2.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n3), false);
    EXPECT_EQ(visited(n3), true);
    EXPECT_EQ(parentInDFS(n3), n2.backIt());
    EXPECT_EQ(edgeVisiting(n3),   n3.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n3), n3.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n4), false);
    EXPECT_EQ(visited(n4), true);
    EXPECT_EQ(parentInDFS(n4), n3.backIt());
    EXPECT_EQ(edgeVisiting(n4),   n4.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n4), n4.incidentEdges().second);

    EXPECT_EQ(waitingForChild(n6), false);
    EXPECT_EQ(visited(n6), true);
    EXPECT_EQ(parentInDFS(n6), n3.backIt());
    EXPECT_EQ(edgeVisiting(n6),   n6.incidentEdges().second);
    EXPECT_EQ(edgeToBeVisited(n6), n6.incidentEdges().second);

    EXPECT_EQ(currentNodeIt(dexpr), g1.nodes().second);

}

} // namespace Undirected

} // namespace Tarjan
