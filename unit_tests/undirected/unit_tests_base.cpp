#include "gtest/gtest.h"
#include "undirected/base.hpp"

namespace Wailea {

namespace Undirected { 

class NodeTests : public ::testing::Test {

  protected:

    NodeTests():mNode1(){;};
    virtual ~NodeTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    Node& Node1(){return mNode1;}

    generation_t mGeneration(Node& n){
        return n.mGeneration;
    };

    Graph* mGraph(Node&n){
        return n.mGraph;
    };

    node_incidence_t& mIncidence(Node&n){
        return n.mIncidence;
    }        

    Node mNode1;

};

class EdgeTests : public ::testing::Test {

  protected:

    EdgeTests():mEdge1(){;};
    virtual ~EdgeTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    Edge& Edge1(){return mEdge1;}

    generation_t mGeneration(Edge& n){
        return n.mGeneration;
    };

    Graph* mGraph(Edge&n){
        return n.mGraph;
    };

    Edge mEdge1;

};

 
class GraphTests : public ::testing::Test {
 
  protected:

    GraphTests():mGraph1(),mGraph2(){;};
    virtual ~GraphTests(){;};
    virtual void SetUp() {;};
    virtual void TearDown() {;};

    Graph& Graph1(){return mGraph1;}
    Graph& Graph2(){return mGraph2;}
    generation_t mGeneration(Node& n){ return n.mGeneration;};
    void setGeneration(Node&n, generation_t v){n.mGeneration=v;};
    Graph* mGraph(Node&n){ return n.mGraph; };
    node_incidence_t& mIncidence(Node&n){ return n.mIncidence; }        
    node_list_it_t mBackIt(Node&n){ return n.mBackIt; }
    Graph* mGraph(Edge&e){ return e.mGraph; };
    edge_list_it_t& mBackIt(Edge&e){ return e.mBackIt;};
    node_list_it_t& mIncidentNode1(Edge&e){ return e.mIncidentNode1;};
    node_list_it_t& mIncidentNode2(Edge&e){ return e.mIncidentNode2;};
    node_incidence_it_t& mBackItNode1(Edge&e){ return e.mBackItNode1;};
    node_incidence_it_t& mBackItNode2(Edge&e){ return e.mBackItNode2;};
    generation_t mGeneration(Edge& e){ return e.mGeneration;};
    void setGeneration(Edge&e, generation_t v){e.mGeneration=v;};

    list<node_ptr_t>& mNodes(Graph&g){ return g.mNodes;};
    list<edge_ptr_t>& mEdges(Graph&g){ return g.mEdges; }
    generation_t mGeneration(Graph& g){ return g.mGeneration; };
    void setGeneration(Graph& g, generation_t v){ g.mGeneration=v;};

    Graph mGraph1;
    Graph mGraph2;

    std::unique_ptr<Graph> generate_graph(){
        return std::make_unique<Graph>();
    }
 
    std::unique_ptr<Node> generate_node(){
        return std::make_unique<Node>();
    }

    std::unique_ptr<Edge> generate_edge(){
        return std::make_unique<Edge>();
    }

};
 

TEST_F(NodeTests, DefaultConstructor) {

    /** @brief tests the default constructor.
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mGraph is nullptr.
     *           - mIncidence is empty, meaning begin() is equal to end().
     *         Inteface:
     *         - degree() is 0.
     *         - incidentEdges() must return a pair of begin()==end().
     */
    Node& node = Node1();
    EXPECT_EQ(mGeneration(node),0) << "mGeneration is incorrect."; 
    EXPECT_EQ(mGraph(node),nullptr) << "mGraph is incorrect.";  
    node_incidence_t incidence = mIncidence(node);
    EXPECT_EQ(incidence.size(),0) << "mIncidence is incorrect.";; 
    bool caught = false;
    try{node.degree();}catch(exception& e){caught=true;}
    EXPECT_EQ(caught,true) << "degree() is incorrect.";  
    caught = false;
    try{node.incidentEdges();}catch(exception& e){caught=true;}
    EXPECT_EQ(caught,true) << "incidentEdges() is incorrect.";
}


TEST_F(EdgeTests, DefaultConstructor) {

    /** @brief tests the default constructor.
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mGraph is nullptr.
     *         Inteface:
     *           none
     */
    Edge& edge = Edge1();
    EXPECT_EQ(mGeneration(edge),0) << "mGeneration is incorrect."; 
    EXPECT_EQ(mGraph(edge),nullptr)<< "mGraph is incorrect."; 
    bool caught = false;
    try{edge.incidentNode1();}catch(exception& e){caught=true;}
    EXPECT_EQ(caught,true) << "incidentNode1() is incorrect.";  
    caught = false;
    try{edge.incidentNode2();}catch(exception& e){caught=true;}
    EXPECT_EQ(caught,true) << "incidentNode2() is incorrect.";  


}

 
TEST_F(GraphTests, DefaultConstructor) {

    /** @brief tests the default constructor.
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mEdges is empty.
     *           - mNodes is empty.
     *         Inteface:
     *           - nodes() returns empty.
     *           - edges() returns empty.
     *           - numNodes() returns 0.
     *           - numEdges() returns 0.
     */
    Graph& graph = Graph1();
    EXPECT_EQ(mGeneration(graph),0) << "mGeneration is incorrect";
    list<node_ptr_t>& graph_mNodes = mNodes(graph);
    list<edge_ptr_t>& graph_mEdges = mEdges(graph);
    EXPECT_EQ(graph_mNodes.size(),0) << "mNodes is incorrect.";    
    EXPECT_EQ(graph_mEdges.size(),0) << "mEdges is incorrect.";    

    pair< node_list_it_t,node_list_it_t > graph_nodes = graph.nodes();
    EXPECT_EQ(graph_nodes.first, graph_nodes.second) 
                                                   << "nodes() is incorrect.";
    pair< edge_list_it_t,edge_list_it_t > graph_edges = graph.edges();
    EXPECT_EQ(graph_edges.first, graph_edges.second)
                                                   << "edges() is incorrect.";

    EXPECT_EQ(graph.numNodes(),0) << "numNodes() is incorrect.";
    EXPECT_EQ(graph.numEdges(),0) << "numEdges() is incorrect.";
}


TEST_F(GraphTests, AddNodeToEmptyGraph) {

    /** @brief tests addNode() to an empty graph.
     *     Graph
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mEdges is empty.
     *           - mNodes has one Node.
     *         Inteface:
     *           - nodes() returns the added node only.
     *           - edges() returns empty.
     *           - numNodes() returns 1.
     *           - numEdges() returns 0.
     *
     *     Node
     *         Internal:
     *           - mGraph points to the graph.
     *           - mBackIt points to the first element in mNodes of the graph.
     */
    std::unique_ptr<Node> np = generate_node();
    Graph& graph = Graph1();
    auto& n1 = graph.addNode(std::move(np));

    EXPECT_EQ(mGeneration(graph),0) << "mGeneration is incorrect";
    list<node_ptr_t>& graph_mNodes = mNodes(graph);
    list<edge_ptr_t>& graph_mEdges = mEdges(graph);
    EXPECT_EQ(graph_mNodes.size(),1) << "mNodes is incorrect.";    
    EXPECT_EQ(graph_mEdges.size(),0) << "mEdges is incorrect.";    
    Node& n2 = *(*(graph_mNodes.begin()));
    EXPECT_EQ(&n1,&n2) << "The node in mNodes is incorrect.";

    pair< node_list_it_t,node_list_it_t > graph_nodes = graph.nodes();
    node_list_it_t it1 = graph_nodes.first;
    it1++;
    EXPECT_EQ(it1, graph_nodes.second) << "nodes() is incorrect.";
    it1--;
    Node& n3 = *(*it1);
    EXPECT_EQ(&n1,&n3) << "The node in mNodes is incorrect.";

    pair< edge_list_it_t,edge_list_it_t > graph_edges = graph.edges();
    EXPECT_EQ(graph_edges.first, graph_edges.second)
                                                << "edges() is incorrect.";    

    EXPECT_EQ(graph.numNodes(),1) << "numNodes() is incorrect.";
    EXPECT_EQ(graph.numEdges(),0) << "numEdges() is incorrect.";

    node_list_it_t node_backIt = mBackIt(n1);
    EXPECT_EQ(node_backIt,graph_nodes.first) << "node's mBackIt is incorrect.";
    EXPECT_EQ(mGraph(n1), &graph) << "node's mGraph is incorrect.";

}


TEST_F(GraphTests, RemoveNodeFromGraph) {

    /** @brief tests removeNode() to an empty graph.
     *     Graph
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mEdges is empty.
     *           - mNodes has one Node.
     *         Inteface:
     *           - nodes() returns the added node only.
     *           - edges() returns empty.
     *           - numNodes() returns 1.
     *           - numEdges() returns 0.
     *
     *     Node
     *         Internal:
     *           - mGraph points to the graph.
     *           - mBackIt points to the first element in mNodes of the graph.
     */
    std::unique_ptr<Node> np1 = generate_node();
    Graph& g1 = Graph1();
    Node& n1 = g1.addNode(std::move(np1));

    std::unique_ptr<Node> np2 = g1.removeNode(n1);

    EXPECT_EQ(mGeneration(g1),0)     << "mGeneration is incorrect";
    list<node_ptr_t>& g1_mNodes = mNodes(g1);
    list<edge_ptr_t>& g1_mEdges = mEdges(g1);
    EXPECT_EQ(g1_mNodes.size(),0)    << "mNodes is incorrect.";
    EXPECT_EQ(g1_mEdges.size(),0)    << "mEdges is incorrect.";

    pair< node_list_it_t,node_list_it_t > g1_nodes = g1.nodes();
    EXPECT_EQ(g1_nodes.first, g1_nodes.second)
                                     << "nodes() is incorrect.";
    pair< edge_list_it_t,edge_list_it_t > g1_edges = g1.edges();
    EXPECT_EQ(g1_edges.first, g1_edges.second)
                                     << "edges() is incorrect.";
    EXPECT_EQ(g1.numNodes(),0)       << "numNodes() is incorrect.";
    EXPECT_EQ(g1.numEdges(),0)       << "numEdges() is incorrect.";

    Node& n2 = *np2;
    EXPECT_EQ(mGeneration(n2),0)     << "mGeneration is incorrect."; 
    EXPECT_EQ(mGraph(n2),nullptr)    << "mGraph is incorrect.";  
    node_incidence_t n2_incidence = mIncidence(n2);
    EXPECT_EQ(n2_incidence.size(),0) << "mIncidence is incorrect.";; 
    bool caught = false;
    try{n2.incidentEdges();}catch(exception&e){caught=true;}
    EXPECT_EQ(caught,true)           << "incidentEdges() is incorrect.";  
    caught = false;
    try{n2.degree();}catch(exception&e){caught=true;}
    EXPECT_EQ(caught,true)           << "degree() is incorrect.";  

}


TEST_F(GraphTests, AddEdgeToGraph) {

    /** @brief tests addEdge() to an empty graph.
     *     Graph
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mEdges has the edge just added.
     *           - mNodes has two Nodes.
     *         Inteface:
     *           - nodes() returns two nodes.
     *           - edges() returns one node.
     *           - numNodes() returns 2.
     *           - numEdges() returns 1.
     *
     *     Node
     *         Internal:
     *           - Node 1 has just one incidence to Edge 1.
     *           - Node 2 has just one incidence to Edge 1.
     *           - Edge 1 has Node 1 and 2 as the incident nodes.
     *           - mGraph of Edge 1 points to the graph.
     *           - mBackIt of Edge 1 points to the first element in mEdges
     *             Nodes of the graph.
     */

    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep1),n1, n2);

    EXPECT_EQ(mGeneration(g1),0) << "mGeneration is incorrect";
    list<node_ptr_t>& g1_mNodes = mNodes(g1);
    list<edge_ptr_t>& g1_mEdges = mEdges(g1);
    EXPECT_EQ(g1_mNodes.size(),2) << "mNodes is incorrect.";    
    EXPECT_EQ(g1_mEdges.size(),1) << "mEdges is incorrect.";    

    node_incidence_t& n1_mIncidence = mIncidence(n1);
    EXPECT_EQ(n1_mIncidence.size(),1) << "N1's incidence is incorrect.";
    node_incidence_t& n2_mIncidence = mIncidence(n2);
    EXPECT_EQ(n2_mIncidence.size(),1) << "N2's incidence is incorrect.";
    pair<node_incidence_it_t,node_incidence_it_t>
                                     n1_incidence_pair = n1.incidentEdges(); 
    pair<node_incidence_it_t,node_incidence_it_t>
                                     n2_incidence_pair = n2.incidentEdges(); 
    Edge& n1_e =  *(*(*n1_incidence_pair.first));
    Edge& n2_e =  *(*(*n2_incidence_pair.first));

    EXPECT_EQ(&e2,&n1_e) << "N1's incidence edge is incorrect.";    
    EXPECT_EQ(&e2,&n2_e) << "N1's incidence edge is incorrect.";    

    Edge& g1_e = *(*g1_mEdges.begin());
    EXPECT_EQ(&e2,&g1_e) << "G1's edge is incorrect.";        

    Graph*               g1_e_mGraph         = mGraph(g1_e);
    EXPECT_EQ(g1_e_mGraph, &g1) << "E1's mGraph is incorrect.";
    edge_list_it_t&      g1_e_mBackIt        = mBackIt(g1_e);
    EXPECT_EQ(g1_e_mBackIt, g1_mEdges.begin()) << "E1's mBackIt is incorrect.";
    node_list_it_t&      g1_e_mIncidentNode1 = mIncidentNode1(g1_e);
    EXPECT_EQ(&(*(*g1_e_mIncidentNode1)), &n1);
    node_list_it_t&      g1_e_mIncidentNode2 = mIncidentNode2(g1_e);
    EXPECT_EQ(&(*(*g1_e_mIncidentNode2)), &n2);
    node_incidence_it_t& g1_e_mBackItNode1   = mBackItNode1(g1_e);
    EXPECT_EQ(g1_e_mBackItNode1, n1_mIncidence.begin());
    node_incidence_it_t& g1_e_mBackItNode2   = mBackItNode2(g1_e);
    EXPECT_EQ(g1_e_mBackItNode2, n2_mIncidence.begin());
    generation_t         g1_e_mGeneration    = mGeneration(g1_e);
    EXPECT_EQ(g1_e_mGeneration,0);
}


TEST_F(GraphTests, RemoveEdgeFromGraph) {

    /** @brief tests removeEdge() from a graph.
     *     Graph
     *         Internal:
     *           - mGeneration is initialized to 0.
     *           - mUtitlity is 0.
     *           - mEdges has no edges.
     *           - mNodes has two nodes.
     *
     *     Removed edge
     *         Internal:
     *           - mGraph points to nullptr.
     *           - mGeneration is 0.
     */

    Graph& g1 = Graph1();

    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));

    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1, n2);

    std::unique_ptr<Edge> ep1_rtn = g1.removeEdge(e1);

    auto g1_mGeneration = mGeneration(g1);
    EXPECT_EQ(g1_mGeneration, 0) << "g1's mGeneration is incorrect.";
    auto& g1_mEdges     = mEdges(g1);
    EXPECT_EQ(g1_mEdges.size(), 0) << "g1's mEdges is incorrect.";
    auto& g1_mNodes     = mNodes(g1);
    EXPECT_EQ(g1_mNodes.size(), 2) << "g1's mNodes is incorrect.";
    auto g1_mNodes_it = g1_mNodes.begin();
    EXPECT_EQ(&(*(*g1_mNodes_it)), &n1)
                      << "g1's mNodes's first node is incorrect.";
    g1_mNodes_it++;
    EXPECT_EQ(&(*(*g1_mNodes_it)), &n2)
                      << "g1's mNodes's second node is incorrect.";
    auto e1_mGraph = mGraph(e1);
    EXPECT_EQ(e1_mGraph, nullptr) << "e1's mGraph is incorrect.";
    auto e1_mGeneration = mGeneration(e1);
    EXPECT_EQ(e1_mGeneration, 0) << "e1's mGeneration is incorrect.";
}


TEST_F(GraphTests, addNodeToGraphSpecificPos) {

    /** @brief tests addNode() to a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...n4, and 0 edges.
     *       n5 is an independent node created.
     *
     *     - Operation
     *       Node& addNode(n5, <before n3>);
     *
     *     - Results to be checked.
     *       g1.mGeneration == 0
     *       g1.mNodes.size()==5
     *       g1.mEdges.size()==0
     *       g1.mNodes == [n1,n2,n5,n3,n4]
     *
     *       n5.mGraph == g1
     *       n5.mGeneration == 0
     *       n5.mIncidence.size() == 0
     *       n5.mBackIt points to the 3rd element in g1.mNodes.
     */
    Graph& g1 = Graph1();

    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));

    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5),mBackIt(n3));

    EXPECT_EQ(mGeneration(g1),0)  << "g1's mGeneration is incorrect.";
    auto& g1_mNodes = mNodes(g1);
    EXPECT_EQ(g1_mNodes.size(),5) << "g1's mNodes.size() is incorrect.";
    auto& g1_mEdges = mEdges(g1);
    EXPECT_EQ(g1_mEdges.size(),0) << "g1's mEdges.size() is incorrect.";    

    auto g1_mNodes_it = g1_mNodes.begin();
    Node& g1_n1 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n2 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n3 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n4 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n5 = *(*g1_mNodes_it);
    EXPECT_EQ(&n1,&g1_n1) << "g1's n1 is incorrect.";    
    EXPECT_EQ(&n2,&g1_n2) << "g1's n2 is incorrect.";    
    EXPECT_EQ(&n5,&g1_n3) << "g1's n3 is incorrect.";    
    EXPECT_EQ(&n3,&g1_n4) << "g1's n4 is incorrect.";    
    EXPECT_EQ(&n4,&g1_n5) << "g1's n5 is incorrect.";    

    EXPECT_EQ(mGraph(n5),&g1)    << "n5's graph is incorrect.";
    EXPECT_EQ(mGeneration(n5),0) << "n5's generation is incorrect.";
    auto& n5_mIncidence = mIncidence(n5);
    EXPECT_EQ(n5_mIncidence.size(),0) << "n5's incidence is incorrect.";
    auto n5_mBackIt = mBackIt(n5);
    g1_mNodes_it--;
    g1_mNodes_it--;
    EXPECT_EQ(n5_mBackIt,g1_mNodes_it);

}


TEST_F(GraphTests, addEdgeToGraphSpecificPos) {

    /** @brief tests addEdge() to a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n4, and 4 edges e1,...,e4.
     *         n1-e1-n2-e2-n3-e3-n4-e4-n1
     *         n1-> e1,e4
     *         n2-> e1,e2
     *         n3-> e2,e3
     *         n4-> e3,e4
     *         e5 is an isoloated edge.
     *
     *     - Operation
     *       Node& addEdge(e5, n2, n4, <before e2 in n2> <before e4 in n4>
     *                                                           <before e3>);
     *
     *     - Results to be checked.
     *       g1.mGeneration == 0
     *       g1.mNodes.size()==4
     *       g1.mEdges.size()==5
     *       g1.mNodes == [n1,n2,n3,n4]
     *       g1.mEdges == [e1,e2,e5,e3,e4]
     *
     *       e5.mGraph == g1
     *       e5.mGeneration == 0
     *       e5.mIncidentNode1 == n2
     *       e5.mIncidentNode2 == n4
     *       e5.mBackItNode1 == <1st element of n2.mIncidence >
     *       e5.mBackItNode2 == <2nd element of n4.mIncidence >
     *       e5.mBackIt points to the 3rd element in g1.mEdges.
     *       n2.mIncidence == [e5,e1,e2]
     *       n4.mIncidence == [e3,e5,e4]
     *       n1.mIncidence.size() == 2
     *       n2.mIncidence.size() == 3
     *       n3.mIncidence.size() == 2
     *       n4.mIncidence.size() == 3
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n2);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n2,n3);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n3,n4);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n4,n1);
    edge_list_t& g1_mEdges = mEdges(g1);
    edge_list_it_t g1_mEdges_it = g1_mEdges.begin();
    g1_mEdges_it++;
    g1_mEdges_it++;
    node_incidence_t& n2_mIncidence = mIncidence(n2);
    node_incidence_it_t n2_incidence_it = n2_mIncidence.begin();
    node_incidence_t& n4_mIncidence = mIncidence(n4);
    node_incidence_it_t n4_incidence_it = n4_mIncidence.begin();
    n4_incidence_it++;
    std::unique_ptr<Edge> ep5 = generate_edge();

    // Target operation.
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4,n2_incidence_it, 
                                               n4_incidence_it, g1_mEdges_it);

    // Check.
    EXPECT_EQ(mGeneration(g1),0)  << "g1's mGeneration is incorrect.";
    auto& g1_mNodes = mNodes(g1);
    EXPECT_EQ(g1_mNodes.size(),4) << "g1's mNodes.size() is incorrect.";
    EXPECT_EQ(g1_mEdges.size(),5) << "g1's mEdges.size() is incorrect.";    

    auto g1_mNodes_it = g1_mNodes.begin();
    Node& g1_n1 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n2 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n3 = *(*g1_mNodes_it);
    g1_mNodes_it++;
    Node& g1_n4 = *(*g1_mNodes_it);
    EXPECT_EQ(&n1,&g1_n1) << "g1's n1 is incorrect.";    
    EXPECT_EQ(&n2,&g1_n2) << "g1's n2 is incorrect.";    
    EXPECT_EQ(&n3,&g1_n3) << "g1's n3 is incorrect.";    
    EXPECT_EQ(&n4,&g1_n4) << "g1's n4 is incorrect.";    

    g1_mEdges_it = g1_mEdges.begin();
    Edge& g1_e1 = *(*g1_mEdges_it);
    g1_mEdges_it++;
    Edge& g1_e2 = *(*g1_mEdges_it);
    g1_mEdges_it++;
    Edge& g1_e3 = *(*g1_mEdges_it);
    g1_mEdges_it++;
    Edge& g1_e4 = *(*g1_mEdges_it);
    g1_mEdges_it++;
    Edge& g1_e5 = *(*g1_mEdges_it);
    EXPECT_EQ(&e1,&g1_e1) << "g1's e1 is incorrect.";    
    EXPECT_EQ(&e2,&g1_e2) << "g1's e2 is incorrect.";    
    EXPECT_EQ(&e5,&g1_e3) << "g1's e3 is incorrect.";    
    EXPECT_EQ(&e3,&g1_e4) << "g1's e4 is incorrect.";    
    EXPECT_EQ(&e4,&g1_e5) << "g1's e5 is incorrect.";    

    EXPECT_EQ(mGraph(e5),&g1)    << "e5's graph is incorrect.";
    EXPECT_EQ(mGeneration(e5),0) << "e5's generation is incorrect.";

    auto  e5_n1_it =mIncidentNode1(e5);
    EXPECT_EQ(&(*(*e5_n1_it)),&n2)   << "e5's n1 is incorrect.";
    auto  e5_n2_it =mIncidentNode2(e5);
    EXPECT_EQ(&(*(*e5_n2_it)),&n4)   << "e5's n1 is incorrect.";

    auto e5_n1_backIt = mBackItNode1(e5);
    n2_incidence_it = n2_mIncidence.begin();
    EXPECT_EQ(e5_n1_backIt,n2_incidence_it)
                                   << "e5's backit for node 1 is incorrect.";
    auto e5_n2_backIt = mBackItNode2(e5);
    n4_incidence_it = n4_mIncidence.begin();
    n4_incidence_it++;
    EXPECT_EQ(e5_n2_backIt,n4_incidence_it)
                                   << "e5's backit for node 2 is incorrect.";

    n2_incidence_it = n2_mIncidence.begin();
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5)
                                   << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e1)
                                   << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e2) << "n2's incidence is incorrect.";

    n4_incidence_it = n4_mIncidence.begin();
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e3) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e4) << "n4's incidence is incorrect.";

    node_incidence_t& n1_mIncidence = mIncidence(n1);
    node_incidence_t& n3_mIncidence = mIncidence(n3);

    EXPECT_EQ(n1_mIncidence.size(),2);
    EXPECT_EQ(n2_mIncidence.size(),3);
    EXPECT_EQ(n3_mIncidence.size(),2);
    EXPECT_EQ(n4_mIncidence.size(),3);
}


TEST_F(GraphTests, checkmIncidence) {

    /** @brief tests addEdge() to a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *       Node::incidentEdges() for all the nodes.
     *
     *     - Results to be checked.
     *       n1.degree() == 3
     *       n2.degree() == 3
     *       n3.degree() == 4
     *       n4.degree() == 4
     *       n5.degree() == 2
     *
     *       n1.incidentEdges() == [e1,e2,e3]
     *       n2.incidentEdges() == [e3,e4,e5]
     *       n3.incidentEdges() == [e1,e4,e6,e7]
     *       n4.incidentEdges() == [e2,e5,e6,e8]
     *       n5.incidentEdges() == [e7,e8]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operations.
    EXPECT_EQ(n1.degree(),3) << "n1's degree is incorrect.";
    EXPECT_EQ(n2.degree(),3) << "n2's degree is incorrect.";
    EXPECT_EQ(n3.degree(),4) << "n3's degree is incorrect.";
    EXPECT_EQ(n4.degree(),4) << "n4's degree is incorrect.";
    EXPECT_EQ(n5.degree(),2) << "n5's degree is incorrect.";

    pair<node_incidence_it_t,node_incidence_it_t> n1_incidence_pair
                                                         = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n2_incidence_pair
                                                         = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n3_incidence_pair
                                                         = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n4_incidence_pair
                                                         = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n5_incidence_pair
                                                         = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    // Checks.
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3) << "n1's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5) << "n2's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e6) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7) << "n3's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e6) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8) << "n4's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7) << "n5's incidence is incorrect.";
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8) << "n5's incidence is incorrect.";
}

TEST_F(GraphTests, removeEdgeFromGraph) {

    /** @brief tests removeEdge() from a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *       Graph::removeEdge(e1)
     *
     *     - Results to be checked.
     *       n1.degree() == 2
     *       n2.degree() == 3
     *       n3.degree() == 3
     *       n4.degree() == 4
     *       n5.degree() == 2
     *
     *       n1.incidentEdges() == [e2,e3]
     *       n2.incidentEdges() == [e3,e4,e5]
     *       n3.incidentEdges() == [e4,e6,e7]
     *       n4.incidentEdges() == [e2,e5,e6,e8]
     *       n5.incidentEdges() == [e7,e8]
     *
     *       e1.mGraph == nullptr
     *       e1.mGeneration == 0
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operations.
    std::unique_ptr<Edge> ep9 = g1.removeEdge(e1);

    // Checks.

    EXPECT_EQ(n1.degree(),2) << "n1's degree is incorrect.";
    EXPECT_EQ(n2.degree(),3) << "n2's degree is incorrect.";
    EXPECT_EQ(n3.degree(),3) << "n3's degree is incorrect.";
    EXPECT_EQ(n4.degree(),4) << "n4's degree is incorrect.";
    EXPECT_EQ(n5.degree(),2) << "n5's degree is incorrect.";

    pair<node_incidence_it_t,node_incidence_it_t> n1_incidence_pair
                                                         = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n2_incidence_pair
                                                         = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n3_incidence_pair
                                                         = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n4_incidence_pair
                                                         = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n5_incidence_pair
                                                         = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3) << "n1's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5) << "n2's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e6) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7) << "n3's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e6) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8) << "n4's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7) << "n5's incidence is incorrect.";
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8) << "n5's incidence is incorrect.";

    Edge& e9 = *ep9;
    EXPECT_EQ(mGraph(e9),nullptr) << "e9's mGraph is incorrect.";
    EXPECT_EQ(mGeneration(e9),0)  << "e9's mGeneration is incorrect.";

}


TEST_F(GraphTests, removeNodeFromGraph) {

    /** @brief tests removeEdge() from a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *       Graph::removeNode(n3)
     *
     *     - Results to be checked.
     *       n1.degree() == 2
     *       n2.degree() == 2
     *       n4.degree() == 3
     *       n5.degree() == 1
     *
     *       n1.incidentEdges() == [e2,e3]
     *       n2.incidentEdges() == [e3,e5]
     *       n4.incidentEdges() == [e2,e5,e8]
     *       n5.incidentEdges() == [e8]
     *
     *       e1.mGraph == nullptr
     *       e1.mGeneration == 0
     *
     *       e4.mGraph == nullptr
     *       e4.mGeneration == 0
     *
     *       e6.mGraph == nullptr
     *       e6.mGeneration == 0
     *
     *       e7.mGraph == nullptr
     *       e7.mGeneration == 0
     *
     *       n3.mGraph == nullptr
     *       n3.mGeneration == 0
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operations.
    vector<edge_ptr_t> removedEdges;
    std::unique_ptr<Node> np6 = g1.removeNode(n3,removedEdges);

    // Checks.
    EXPECT_EQ(n1.degree(),2) << "n1's degree is incorrect.";
    EXPECT_EQ(n2.degree(),2) << "n2's degree is incorrect.";
    EXPECT_EQ(n4.degree(),3) << "n4's degree is incorrect.";
    EXPECT_EQ(n5.degree(),1) << "n5's degree is incorrect.";

    pair<node_incidence_it_t,node_incidence_it_t> n1_incidence_pair
                                                         = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n2_incidence_pair
                                                         = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n4_incidence_pair
                                                         = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n5_incidence_pair
                                                         = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3) << "n1's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5) << "n2's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8) << "n4's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8) << "n5's incidence is incorrect.";

    Node& n6 = *np6;
    EXPECT_EQ(mGraph(n6),nullptr) << "n3's mGraph is incorrect.";
    EXPECT_EQ(mGeneration(n6),0)  << "n3's mGeneration is incorrect.";
    
    EXPECT_EQ(removedEdges.size(),4) << "removedEdge is incorrect.";
    for (auto& i : removedEdges) {

        EXPECT_EQ(mGraph(*i),nullptr) << "removed edge's mGraph is incorrect.";
        EXPECT_EQ(mGeneration(*i),0)
                                 << "removed edge's mGeneration is incorrect.";

    }

}


TEST_F(GraphTests, moveEdgeinGraph) {

    /** @brief tests moveEdge() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *       Graph::moveNode(e4,n1,n5)
     *
     *     - Results to be checked.
     *       n1.degree() == 4
     *       n2.degree() == 2
     *       n3.degree() == 3
     *       n4.degree() == 4
     *       n5.degree() == 3
     *
     *       n1.incidentEdges() == [e1,e2,e3,e4]
     *       n2.incidentEdges() == [e3,e5]
     *       n3.incidentEdges() == [e1,e6,e7]
     *       n4.incidentEdges() == [e2,e5,e6,e8]
     *       n5.incidentEdges() == [e7,e8,e4]
     *
     *       e4.mGraph == g1
     *       e4.mBackIt == g1.mEdges[3]
     *       e4.mIncidentNode1 == n1
     *       e4.mIncidentNode2 == n5
     *       e4.mBackItNode1 == n1.mIncidence[3]
     *       e4.mBackItNode2 == n5.mIncidence[2]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operations.
    g1.moveEdge(e4,n1,n5);
    EXPECT_EQ(n1.degree(),4) << "n1's degree is incorrect.";
    EXPECT_EQ(n2.degree(),2) << "n2's degree is incorrect.";
    EXPECT_EQ(n3.degree(),3) << "n3's degree is incorrect.";
    EXPECT_EQ(n4.degree(),4) << "n4's degree is incorrect.";
    EXPECT_EQ(n5.degree(),3) << "n5's degree is incorrect.";

    pair<node_incidence_it_t,node_incidence_it_t> n1_incidence_pair
                                                         = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n2_incidence_pair
                                                         = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n3_incidence_pair
                                                         = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n4_incidence_pair
                                                         = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n5_incidence_pair
                                                         = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e4) << "n1's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5) << "n2's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e6) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7) << "n3's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e6) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8) << "n4's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7) << "n5's incidence is incorrect.";
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8) << "n5's incidence is incorrect.";
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e4) << "n5's incidence is incorrect.";

    EXPECT_EQ(mGraph(e4),&g1)     << "g1's mGraph is incorrect.";
    auto& g1_mEdges = mEdges(g1);
    auto g1_mEdges_it = g1_mEdges.begin();
    g1_mEdges_it++;
    g1_mEdges_it++;
    g1_mEdges_it++;
    EXPECT_EQ(mBackIt(e4),g1_mEdges_it) << "e4's mBackIt is incorrect.";
    auto  e4_n1_it =mIncidentNode1(e4);
    EXPECT_EQ(&(*(*e4_n1_it)),&n1)   << "e4's n1 is incorrect.";
    auto  e4_n2_it =mIncidentNode2(e4);
    EXPECT_EQ(&(*(*e4_n2_it)),&n5)   << "e4's n2 is incorrect.";

    auto e4_n1_backIt = mBackItNode1(e4);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    n1_incidence_it++;
    n1_incidence_it++;
    EXPECT_EQ(e4_n1_backIt,n1_incidence_it)
                                   << "e4's backit for node 1 is incorrect.";
    auto e4_n2_backIt = mBackItNode2(e4);
    n5_incidence_it = n5_incidence_pair.first;
    n5_incidence_it++;
    n5_incidence_it++;
    EXPECT_EQ(e4_n2_backIt,n5_incidence_it)
                                   << "e4's backit for node 2 is incorrect.";
}



TEST_F(GraphTests, moveEdgeinGraphSpecificLocations) {

    /** @brief tests moveEdge() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *       Graph::moveNode(e4,n1,n5)
     *
     *     - Results to be checked.
     *       n1.degree() == 4
     *       n2.degree() == 2
     *       n3.degree() == 3
     *       n4.degree() == 4
     *       n5.degree() == 3
     *
     *       n1.incidentEdges() == [e1,e4,e2,e3]
     *       n2.incidentEdges() == [e3,e5]
     *       n3.incidentEdges() == [e1,e6,e7]
     *       n4.incidentEdges() == [e2,e5,e6,e8]
     *       n5.incidentEdges() == [e4,e7,e8]
     *
     *       e4.mGraph == g1
     *       e4.mBackIt == g1.mEdges[3]
     *       e4.mIncidentNode1 == n1
     *       e4.mIncidentNode2 == n5
     *       e4.mBackItNode1 == n1.mIncidence[3]
     *       e4.mBackItNode2 == n5.mIncidence[2]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);


    // Target operations.
    pair<node_incidence_it_t,node_incidence_it_t> n1_incidence_pair
                                                         = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n5_incidence_pair
                                                         = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;
    n1_incidence_it++;

    g1.moveEdge(e4, n1, n1_incidence_it, n5, n5_incidence_it);

    // Checks.
    EXPECT_EQ(n1.degree(),4) << "n1's degree is incorrect.";
    EXPECT_EQ(n2.degree(),2) << "n2's degree is incorrect.";
    EXPECT_EQ(n3.degree(),3) << "n3's degree is incorrect.";
    EXPECT_EQ(n4.degree(),4) << "n4's degree is incorrect.";
    EXPECT_EQ(n5.degree(),3) << "n5's degree is incorrect.";
    n1_incidence_pair = n1.incidentEdges();
    n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n2_incidence_pair
                                                         = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n3_incidence_pair
                                                         = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t> n4_incidence_pair
                                                         = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    n5_incidence_pair = n5.incidentEdges();
    n5_incidence_it = n5_incidence_pair.first;

    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e4) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2) << "n1's incidence is incorrect.";
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3) << "n1's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3) << "n2's incidence is incorrect.";
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5) << "n2's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e6) << "n3's incidence is incorrect.";
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7) << "n3's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e6) << "n4's incidence is incorrect.";
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8) << "n4's incidence is incorrect.";

    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e4) << "n5's incidence is incorrect.";
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7) << "n5's incidence is incorrect.";
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8) << "n5's incidence is incorrect.";

    EXPECT_EQ(mGraph(e4),&g1)     << "g1's mGraph is incorrect.";
    auto& g1_mEdges = mEdges(g1);
    auto g1_mEdges_it = g1_mEdges.begin();
    g1_mEdges_it++;
    g1_mEdges_it++;
    g1_mEdges_it++;
    EXPECT_EQ(mBackIt(e4),g1_mEdges_it) << "e4's mBackIt is incorrect.";
    auto  e4_n1_it =mIncidentNode1(e4);
    EXPECT_EQ(&(*(*e4_n1_it)),&n1)   << "e4's n1 is incorrect.";
    auto  e4_n2_it =mIncidentNode2(e4);
    EXPECT_EQ(&(*(*e4_n2_it)),&n5)   << "e4's n2 is incorrect.";

    auto e4_n1_backIt = mBackItNode1(e4);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    EXPECT_EQ(e4_n1_backIt,n1_incidence_it)
                                   << "e4's backit for node 1 is incorrect.";
    auto e4_n2_backIt = mBackItNode2(e4);
    n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(e4_n2_backIt,n5_incidence_it)
                        << "e4's backit for node 2 is incorrect.";
}


// Split edge.
TEST_F(GraphTests, splitEdgeInGraph) {

    /** @brief tests splitEdgeInGraph() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *         n6, e9, e10 : unmanaged objects.
     *
     *     - Operation
     *        n6ref = Grpah::splitEdge(
     *                  e6, e6ptr, e9ptr, e10ptr, n6ptr, before n3)
     *
     *     - Results to be checked.
     *       g1.mNodes.size() == 6
     *       g1.mNodes == [n1,n2,n6,n3,n4,n5]
     *       g1.mEdges.size() == 9
     *       g1.mEdges == [e1,e2,e3,e4,e5,e9,e10,e7,e8]
     *
     *       n1.degree() == 3
     *       n2.degree() == 3
     *       n3.degree() == 4
     *       n4.degree() == 4
     *       n5.degree() == 2
     *       n6.degree() == 2
     *
     *       n1.incidentEdges() == [e1,e2,e3]
     *       n2.incidentEdges() == [e3,e4,e5]
     *       n3.incidentEdges() == [e1,e4,e9,e7]
     *       n4.incidentEdges() == [e2,e5,e10,e8]
     *       n5.incidentEdges() == [e7,e8]
     *       n6.incidentEdges() == [e9,e10]
     *
     *       e9.mGraph  == g1
     *       e9.mBackIt == g1.mEdges[6]
     *       e9.mIncidentNode1 == n3
     *       e9.mIncidentNode2 == n6
     *       e9.mBackItNode1 == n3.mIncidence[2]
     *       e9.mBackItNode2 == n6.mIncidence[0]
     *
     *       e10.mGraph  == g1
     *       e10.mBackIt == g1.mEdges[7]
     *       e10.mIncidentNode1 == n6
     *       e10.mIncidentNode2 == n4
     *       e10.mBackItNode1 == n6.mIncidence[1]
     *       e10.mBackItNode2 == n4.mIncidence[2]
     *
     *       n6.mGraph == g1
     *       n6.mBackIt == g1.mNodes[2]
     *       n6.mIncidence == [e9,e10]
     *
     *       e6.mGraph == nullptr
     *
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Edge> ep9 = generate_edge();
    std::unique_ptr<Edge> ep10= generate_edge();
    std::unique_ptr<Node> np6 = generate_node();

    // Target operation
    std::unique_ptr<Edge> ep6_removed;
    auto nit = mNodes(g1).begin();
    nit++;
    nit++;
    std::tuple<Node&,Edge&,Edge&> tpl
        = g1.splitEdge(e6, 
                       ep6_removed, 
                       std::move(ep9), 
                       std::move(ep10), 
                       std::move(np6), 
                       nit++);
    Node& n6  = std::get<0>(tpl);
    Edge& e9  = std::get<1>(tpl);
    Edge& e10 = std::get<2>(tpl);

    // Checks
    EXPECT_EQ(mNodes(g1).size(),6);
    nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n1);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n6);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n4);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);

    EXPECT_EQ(mEdges(g1).size(),9);
    auto eit = mEdges(g1).begin();
    EXPECT_EQ(&(*(*eit)),&e1);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e3);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e4);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e5);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e9);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e10);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e7);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e8);

    EXPECT_EQ(n1.degree(),3);
    EXPECT_EQ(n2.degree(),3);
    EXPECT_EQ(n3.degree(),4);
    EXPECT_EQ(n4.degree(),4);
    EXPECT_EQ(n5.degree(),2);
    EXPECT_EQ(n6.degree(),2);

    pair<node_incidence_it_t,node_incidence_it_t> n1_incidence_pair
                                                         = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3);
    
    pair<node_incidence_it_t,node_incidence_it_t> n2_incidence_pair
                                                         = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5);

    pair<node_incidence_it_t,node_incidence_it_t> n3_incidence_pair
                                                         = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e9);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7);

    pair<node_incidence_it_t,node_incidence_it_t> n4_incidence_pair
                                                         = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e10);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8);

    pair<node_incidence_it_t,node_incidence_it_t> n5_incidence_pair
                                                         = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7);
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8);

    pair<node_incidence_it_t,node_incidence_it_t> n6_incidence_pair
                                                         = n6.incidentEdges();
    node_incidence_it_t n6_incidence_it = n6_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n6_incidence_it))),&e9);
    n6_incidence_it++;
    EXPECT_EQ(&(*(*(*n6_incidence_it))),&e10);

    EXPECT_EQ(mGraph(e9),&g1);
    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e9),eit);    
    EXPECT_EQ(&(*(*(mIncidentNode1(e9)))),&n3);
    EXPECT_EQ(&(*(*(mIncidentNode2(e9)))),&n6);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n6_incidence_it = n6_incidence_pair.first;
    EXPECT_EQ(mBackItNode1(e9),n3_incidence_it);
    EXPECT_EQ(mBackItNode2(e9),n6_incidence_it);

    EXPECT_EQ(mGraph(e10),&g1);
    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e10),eit);    
    EXPECT_EQ(&(*(*(mIncidentNode1(e10)))),&n6);
    EXPECT_EQ(&(*(*(mIncidentNode2(e10)))),&n4);
    n6_incidence_it = n6_incidence_pair.first;
    n6_incidence_it++;
    n4_incidence_it = n4_incidence_pair.first;
    n4_incidence_it++;
    n4_incidence_it++;
    EXPECT_EQ(mBackItNode1(e10),n6_incidence_it);
    EXPECT_EQ(mBackItNode2(e10),n4_incidence_it);

    EXPECT_EQ(mGraph(n6),&g1);
    nit = mNodes(g1).begin();
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n6),nit);
    EXPECT_EQ(mGraph(e6),nullptr);

}


// Split edge.
TEST_F(GraphTests, contractEdgeInGraph) {

    /** @brief tests constractEdgeInGraph() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Results to be checked.
     *       g1.mNodes.size() == 4
     *       g1.mNodes == [n1,n2,n3,n5]
     *       g1.mEdges.size() == 7
     *       g1.mEdges == [e1,e2,e3,e4,e5,e7,e8]
     *
     *       n1.degree() == 3
     *       n2.degree() == 3
     *       n3.degree() == 6
     *       n5.degree() == 2
     *
     *       n1.incidentEdges() == [e1,e2,e3]
     *       n2.incidentEdges() == [e3,e4,e5]
     *       n3.incidentEdges() == [e1,e4,e7,e2,e5,e8]
     *       n5.incidentEdges() == [e7,e8]
     *
     *       e1.mIncidentNode1 = n1
     *       e1.mIncidentNode2 = n3
     *       e1.mBackItNode1 = n1.mIncidence[0]
     *       e1.mBackItNode2 = n3.mIncidence[0]
     *
     *       e2.mIncidentNode1 = n1
     *       e2.mIncidentNode2 = n3
     *       e2.mBackItNode1 = n1.mIncidence[1]
     *       e2.mBackItNode2 = n3.mIncidence[3]
     *
     *       e3.mIncidentNode1 = n1
     *       e3.mIncidentNode2 = n2
     *       e3.mBackItNode1 = n1.mIncidence[2]
     *       e3.mBackItNode2 = n2.mIncidence[0]
     *
     *       e4.mIncidentNode1 = n2
     *       e4.mIncidentNode2 = n3
     *       e4.mBackItNode1 = n2.mIncidence[1]
     *       e4.mBackItNode2 = n3.mIncidence[1]
     *
     *       e5.mIncidentNode1 = n2
     *       e5.mIncidentNode2 = n3
     *       e5.mBackItNode1 = n2.mIncidence[2]
     *       e5.mBackItNode2 = n3.mIncidence[4]
     *
     *       e7.mIncidentNode1 = n3
     *       e7.mIncidentNode2 = n5
     *       e7.mBackItNode1 = n3.mIncidence[2]
     *       e7.mBackItNode2 = n5.mIncidence[0]
     *
     *       e8.mIncidentNode1 = n3
     *       e8.mIncidentNode2 = n5
     *       e8.mBackItNode1 = n3.mIncidence[5]
     *       e8.mBackItNode2 = n5.mIncidence[1]
     *
     *       ep6->mGraph == nullptr
     *       np6->mGraph == nullptr
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);


    // Target operation
    std::unique_ptr<Node> np4_2(nullptr);
    std::unique_ptr<Edge> ep6_2 = g1.contractEdge(e6,np4_2); 


    // Checks
    EXPECT_EQ(mNodes(g1).size(),4);
    auto nit = mNodes(g1).begin();
    nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n1);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);

    EXPECT_EQ(mEdges(g1).size(),7);
    auto eit = mEdges(g1).begin();
    EXPECT_EQ(&(*(*eit)),&e1);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e3);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e4);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e5);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e7);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e8);

    EXPECT_EQ(n1.degree(),3);
    EXPECT_EQ(n2.degree(),3);
    EXPECT_EQ(n3.degree(),6);
    EXPECT_EQ(n5.degree(),2);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n1_incidence_pair = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n2_incidence_pair = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_incidence_pair = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e2);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e5);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e8);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n5_incidence_pair = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7);
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8);

    auto  e1_n1_it =mIncidentNode1(e1);
    EXPECT_EQ(&(*(*e1_n1_it)),&n1);
    auto  e1_n2_it =mIncidentNode2(e1);
    EXPECT_EQ(&(*(*e1_n2_it)),&n3);
    auto e1_n1_backIt = mBackItNode1(e1);
    n1_incidence_it = n1_incidence_pair.first;
    EXPECT_EQ(e1_n1_backIt,n1_incidence_it);
    auto e1_n2_backIt = mBackItNode2(e1);
    n3_incidence_it = n3_incidence_pair.first;
    EXPECT_EQ(e1_n2_backIt,n3_incidence_it);

    auto  e2_n1_it =mIncidentNode1(e2);
    EXPECT_EQ(&(*(*e2_n1_it)),&n1);
    auto  e2_n2_it =mIncidentNode2(e2);
    EXPECT_EQ(&(*(*e2_n2_it)),&n3);
    auto e2_n1_backIt = mBackItNode1(e2);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    EXPECT_EQ(e2_n1_backIt,n1_incidence_it);
    auto e2_n2_backIt = mBackItNode2(e2);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e2_n2_backIt,n3_incidence_it);

    auto  e3_n1_it =mIncidentNode1(e3);
    EXPECT_EQ(&(*(*e3_n1_it)),&n1);
    auto  e3_n2_it =mIncidentNode2(e3);
    EXPECT_EQ(&(*(*e3_n2_it)),&n2);
    auto e3_n1_backIt = mBackItNode1(e3);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    n1_incidence_it++;
    EXPECT_EQ(e3_n1_backIt,n1_incidence_it);
    auto e3_n2_backIt = mBackItNode2(e3);
    n2_incidence_it = n2_incidence_pair.first;
    EXPECT_EQ(e3_n2_backIt,n2_incidence_it);

    auto  e4_n1_it =mIncidentNode1(e4);
    EXPECT_EQ(&(*(*e4_n1_it)),&n2);
    auto  e4_n2_it =mIncidentNode2(e4);
    EXPECT_EQ(&(*(*e4_n2_it)),&n3);
    auto e4_n1_backIt = mBackItNode1(e4);
    n2_incidence_it = n2_incidence_pair.first;
    n2_incidence_it++;
    EXPECT_EQ(e4_n1_backIt,n2_incidence_it);
    auto e4_n2_backIt = mBackItNode2(e4);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    EXPECT_EQ(e4_n2_backIt,n3_incidence_it);

    auto  e5_n1_it =mIncidentNode1(e5);
    EXPECT_EQ(&(*(*e5_n1_it)),&n2);
    auto  e5_n2_it =mIncidentNode2(e5);
    EXPECT_EQ(&(*(*e5_n2_it)),&n3);
    auto e5_n1_backIt = mBackItNode1(e5);
    n2_incidence_it = n2_incidence_pair.first;
    n2_incidence_it++;
    n2_incidence_it++;
    EXPECT_EQ(e5_n1_backIt,n2_incidence_it);
    auto e5_n2_backIt = mBackItNode2(e5);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e5_n2_backIt,n3_incidence_it);

    auto  e7_n1_it =mIncidentNode1(e7);
    EXPECT_EQ(&(*(*e7_n1_it)),&n3);
    auto  e7_n2_it =mIncidentNode2(e7);
    EXPECT_EQ(&(*(*e7_n2_it)),&n5);
    auto e7_n1_backIt = mBackItNode1(e7);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e7_n1_backIt,n3_incidence_it);
    auto e7_n2_backIt = mBackItNode2(e7);
    n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(e7_n2_backIt,n5_incidence_it);

    auto  e8_n1_it =mIncidentNode1(e8);
    EXPECT_EQ(&(*(*e8_n1_it)),&n3);
    auto  e8_n2_it =mIncidentNode2(e8);
    EXPECT_EQ(&(*(*e8_n2_it)),&n5);
    auto e8_n1_backIt = mBackItNode1(e8);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e8_n1_backIt,n3_incidence_it);
    auto e8_n2_backIt = mBackItNode2(e8);
    n5_incidence_it = n5_incidence_pair.first;
    n5_incidence_it++;
    EXPECT_EQ(e8_n2_backIt,n5_incidence_it);

    EXPECT_EQ(mGraph(*np4_2),nullptr);
    EXPECT_EQ(mGraph(*ep6_2),nullptr);

}


// Split edge without getting removed node back.
TEST_F(GraphTests, contractEdgeInGraph2) {

    /** @brief tests constractEdgeInGraph() in a graph
     *         without getting removed node back.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Results to be checked.
     *       g1.mNodes.size() == 4
     *       g1.mNodes == [n1,n2,n3,n5]
     *       g1.mEdges.size() == 7
     *       g1.mEdges == [e1,e2,e3,e4,e5,e7,e8]
     *
     *       n1.degree() == 3
     *       n2.degree() == 3
     *       n3.degree() == 6
     *       n5.degree() == 2
     *
     *       n1.incidentEdges() == [e1,e2,e3]
     *       n2.incidentEdges() == [e3,e4,e5]
     *       n3.incidentEdges() == [e1,e4,e7,e2,e5,e8]
     *       n5.incidentEdges() == [e7,e8]
     *
     *       e1.mIncidentNode1 = n1
     *       e1.mIncidentNode2 = n3
     *       e1.mBackItNode1 = n1.mIncidence[0]
     *       e1.mBackItNode2 = n3.mIncidence[0]
     *
     *       e2.mIncidentNode1 = n1
     *       e2.mIncidentNode2 = n3
     *       e2.mBackItNode1 = n1.mIncidence[1]
     *       e2.mBackItNode2 = n3.mIncidence[3]
     *
     *       e3.mIncidentNode1 = n1
     *       e3.mIncidentNode2 = n2
     *       e3.mBackItNode1 = n1.mIncidence[2]
     *       e3.mBackItNode2 = n2.mIncidence[0]
     *
     *       e4.mIncidentNode1 = n2
     *       e4.mIncidentNode2 = n3
     *       e4.mBackItNode1 = n2.mIncidence[1]
     *       e4.mBackItNode2 = n3.mIncidence[1]
     *
     *       e5.mIncidentNode1 = n2
     *       e5.mIncidentNode2 = n3
     *       e5.mBackItNode1 = n2.mIncidence[2]
     *       e5.mBackItNode2 = n3.mIncidence[4]
     *
     *       e7.mIncidentNode1 = n3
     *       e7.mIncidentNode2 = n5
     *       e7.mBackItNode1 = n3.mIncidence[2]
     *       e7.mBackItNode2 = n5.mIncidence[0]
     *
     *       e8.mIncidentNode1 = n3
     *       e8.mIncidentNode2 = n5
     *       e8.mBackItNode1 = n3.mIncidence[5]
     *       e8.mBackItNode2 = n5.mIncidence[1]
     *
     *       ep6->mGraph == nullptr
     *       np6->mGraph == nullptr
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operation
    std::unique_ptr<Edge> ep6_2 = g1.contractEdge(e6);

    // Checks
    EXPECT_EQ(mNodes(g1).size(),4);
    auto nit = mNodes(g1).begin();
    nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n1);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);

    EXPECT_EQ(mEdges(g1).size(),7);
    auto eit = mEdges(g1).begin();
    EXPECT_EQ(&(*(*eit)),&e1);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e3);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e4);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e5);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e7);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e8);

    EXPECT_EQ(n1.degree(),3);
    EXPECT_EQ(n2.degree(),3);
    EXPECT_EQ(n3.degree(),6);
    EXPECT_EQ(n5.degree(),2);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n1_incidence_pair = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n2_incidence_pair = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_incidence_pair = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e2);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e5);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e8);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n5_incidence_pair = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7);
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8);

    auto  e1_n1_it =mIncidentNode1(e1);
    EXPECT_EQ(&(*(*e1_n1_it)),&n1);
    auto  e1_n2_it =mIncidentNode2(e1);
    EXPECT_EQ(&(*(*e1_n2_it)),&n3);
    auto e1_n1_backIt = mBackItNode1(e1);
    n1_incidence_it = n1_incidence_pair.first;
    EXPECT_EQ(e1_n1_backIt,n1_incidence_it);
    auto e1_n2_backIt = mBackItNode2(e1);
    n3_incidence_it = n3_incidence_pair.first;
    EXPECT_EQ(e1_n2_backIt,n3_incidence_it);

    auto  e2_n1_it =mIncidentNode1(e2);
    EXPECT_EQ(&(*(*e2_n1_it)),&n1);
    auto  e2_n2_it =mIncidentNode2(e2);
    EXPECT_EQ(&(*(*e2_n2_it)),&n3);
    auto e2_n1_backIt = mBackItNode1(e2);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    EXPECT_EQ(e2_n1_backIt,n1_incidence_it);
    auto e2_n2_backIt = mBackItNode2(e2);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e2_n2_backIt,n3_incidence_it);

    auto  e3_n1_it =mIncidentNode1(e3);
    EXPECT_EQ(&(*(*e3_n1_it)),&n1);
    auto  e3_n2_it =mIncidentNode2(e3);
    EXPECT_EQ(&(*(*e3_n2_it)),&n2);
    auto e3_n1_backIt = mBackItNode1(e3);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    n1_incidence_it++;
    EXPECT_EQ(e3_n1_backIt,n1_incidence_it);
    auto e3_n2_backIt = mBackItNode2(e3);
    n2_incidence_it = n2_incidence_pair.first;
    EXPECT_EQ(e3_n2_backIt,n2_incidence_it);

    auto  e4_n1_it =mIncidentNode1(e4);
    EXPECT_EQ(&(*(*e4_n1_it)),&n2);
    auto  e4_n2_it =mIncidentNode2(e4);
    EXPECT_EQ(&(*(*e4_n2_it)),&n3);
    auto e4_n1_backIt = mBackItNode1(e4);
    n2_incidence_it = n2_incidence_pair.first;
    n2_incidence_it++;
    EXPECT_EQ(e4_n1_backIt,n2_incidence_it);
    auto e4_n2_backIt = mBackItNode2(e4);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    EXPECT_EQ(e4_n2_backIt,n3_incidence_it);

    auto  e5_n1_it =mIncidentNode1(e5);
    EXPECT_EQ(&(*(*e5_n1_it)),&n2);
    auto  e5_n2_it =mIncidentNode2(e5);
    EXPECT_EQ(&(*(*e5_n2_it)),&n3);
    auto e5_n1_backIt = mBackItNode1(e5);
    n2_incidence_it = n2_incidence_pair.first;
    n2_incidence_it++;
    n2_incidence_it++;
    EXPECT_EQ(e5_n1_backIt,n2_incidence_it);
    auto e5_n2_backIt = mBackItNode2(e5);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e5_n2_backIt,n3_incidence_it);

    auto  e7_n1_it =mIncidentNode1(e7);
    EXPECT_EQ(&(*(*e7_n1_it)),&n3);
    auto  e7_n2_it =mIncidentNode2(e7);
    EXPECT_EQ(&(*(*e7_n2_it)),&n5);
    auto e7_n1_backIt = mBackItNode1(e7);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e7_n1_backIt,n3_incidence_it);
    auto e7_n2_backIt = mBackItNode2(e7);
    n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(e7_n2_backIt,n5_incidence_it);

    auto  e8_n1_it =mIncidentNode1(e8);
    EXPECT_EQ(&(*(*e8_n1_it)),&n3);
    auto  e8_n2_it =mIncidentNode2(e8);
    EXPECT_EQ(&(*(*e8_n2_it)),&n5);
    auto e8_n1_backIt = mBackItNode1(e8);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e8_n1_backIt,n3_incidence_it);
    auto e8_n2_backIt = mBackItNode2(e8);
    n5_incidence_it = n5_incidence_pair.first;
    n5_incidence_it++;
    EXPECT_EQ(e8_n2_backIt,n5_incidence_it);

    EXPECT_EQ(mGraph(*ep6_2),nullptr);

}


// Node-induced edges.
TEST_F(GraphTests, nodeInducedEdgesInGraph) {

    /** @brief tests nodeInducedEdges() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *         elist1 = nodeInducedEdges([])
     *         elist2 = nodeInducedEdges([n1])
     *         elist3 = nodeInducedEdges([n1, n2, n5])
     *         elist4 = nodeInducedEdges([n3, n4, n5])
     *         elist5 = nodeInducedEdges([n1,n2,n3,n4,n5])
     *
     *     - Results to be checked.
     *       elist1.size() == 0
     *       elist2.size() == 0
     *       elist3.size() == 1
     *       elist3 == [e3]
     *       elist4.size() == 3
     *       elist4 == [e6,e7,e8]
     *       elist5.size() == 8
     *       elist5 == [e1,e2,e3,e4,e5,e6,e7,e8]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operation
    vector<node_list_it_t>nlist1;
    vector<edge_list_it_t>elist1 = g1.nodeInducedEdges(nlist1);

    vector<node_list_it_t>nlist2;
    nlist2.push_back(mBackIt(n1));
    vector<edge_list_it_t>elist2 = g1.nodeInducedEdges(nlist2);

    vector<node_list_it_t>nlist3;
    nlist3.push_back(mBackIt(n1));
    nlist3.push_back(mBackIt(n2));
    nlist3.push_back(mBackIt(n5));
    vector<edge_list_it_t>elist3 = g1.nodeInducedEdges(nlist3);

    vector<node_list_it_t>nlist4;
    nlist4.push_back(mBackIt(n3));
    nlist4.push_back(mBackIt(n4));
    nlist4.push_back(mBackIt(n5));
    vector<edge_list_it_t>elist4 = g1.nodeInducedEdges(nlist4);

    vector<node_list_it_t>nlist5;
    nlist5.push_back(mBackIt(n1));
    nlist5.push_back(mBackIt(n2));
    nlist5.push_back(mBackIt(n3));
    nlist5.push_back(mBackIt(n4));
    nlist5.push_back(mBackIt(n5));
    vector<edge_list_it_t>elist5 = g1.nodeInducedEdges(nlist5);

    // Checks.
    bool e1_found = false;
    bool e2_found = false;
    bool e3_found = false;
    bool e4_found = false;
    bool e5_found = false;
    bool e6_found = false;
    bool e7_found = false;
    bool e8_found = false;

    EXPECT_EQ(elist1.size(),0);
    EXPECT_EQ(elist2.size(),0);
    EXPECT_EQ(elist3.size(),1);
    EXPECT_EQ(elist3[0],mBackIt(e3));
    EXPECT_EQ(elist4.size(),3);

    e1_found = false;
    e2_found = false;
    e3_found = false;
    e4_found = false;
    e5_found = false;
    e6_found = false;
    e7_found = false;
    e8_found = false;
    for(auto e : elist4 ){
        if (e == mBackIt(e1)){
            e1_found = true;
        }
        if (e == mBackIt(e2)){
            e2_found = true;
        }
        if (e == mBackIt(e3)){
            e3_found = true;
        }
        if (e == mBackIt(e4)){
            e4_found = true;
        }
        if (e == mBackIt(e5)){
            e5_found = true;
        }
        if (e == mBackIt(e6)){
            e6_found = true;
        }
        if (e == mBackIt(e7)){
            e7_found = true;
        }
        if (e == mBackIt(e8)){
            e8_found = true;
        }
    }
    EXPECT_EQ(!e1_found &&
              !e2_found &&
              !e3_found &&
              !e4_found &&
              !e5_found &&
               e6_found &&
               e7_found &&
               e8_found,   true);

    EXPECT_EQ(elist5.size(),8);
    e1_found = false;
    e2_found = false;
    e3_found = false;
    e4_found = false;
    e5_found = false;
    e6_found = false;
    e7_found = false;
    e8_found = false;
    for(auto e : elist5 ){
        if (e == mBackIt(e1)){
            e1_found = true;
        }
        if (e == mBackIt(e2)){
            e2_found = true;
        }
        if (e == mBackIt(e3)){
            e3_found = true;
        }
        if (e == mBackIt(e4)){
            e4_found = true;
        }
        if (e == mBackIt(e5)){
            e5_found = true;
        }
        if (e == mBackIt(e6)){
            e6_found = true;
        }
        if (e == mBackIt(e7)){
            e7_found = true;
        }
        if (e == mBackIt(e8)){
            e8_found = true;
        }
    }
    EXPECT_EQ(e1_found && e2_found && e3_found && e4_found && e5_found 
                              && e6_found && e7_found && e8_found, true);
}


// Edge-induced nodes.
TEST_F(GraphTests, edgeInducedNodesInGraph) {

    /** @brief tests edgeInducedNodes() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *         nlist1 = nodeInducedEdges([])
     *         nlist2 = nodeInducedEdges([e1])
     *         nlist3 = nodeInducedEdges([e3, e6])
     *         nlist4 = nodeInducedEdges([e1, e3, e4])
     *         nlist5 = nodeInducedEdges([e1, e2, e6, e8])
     *
     *     - Results to be checked.
     *       nlist1.size() == 0
     *       nlist2.size() == 2
     *       nlist2 == [n1, n3]
     *       nlist3.size() == 4
     *       nlist3 == [n1, n2, n3, n4]
     *       nlist4.size() == 3
     *       nlist4 == [n1, n2, n3]
     *       nlist5.size() == 4
     *       nlist5 == [n1, n3, n4, n5]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    // Target operation
    vector<edge_list_it_t>elist1;
    vector<node_list_it_t>nlist1 = g1.edgeInducedNodes(elist1);

    vector<edge_list_it_t>elist2;
    elist2.push_back(mBackIt(e1));
    vector<node_list_it_t>nlist2 = g1.edgeInducedNodes(elist2);

    vector<edge_list_it_t>elist3;
    elist3.push_back(mBackIt(e3));
    elist3.push_back(mBackIt(e6));
    vector<node_list_it_t>nlist3 = g1.edgeInducedNodes(elist3);

    vector<edge_list_it_t>elist4;
    elist4.push_back(mBackIt(e1));
    elist4.push_back(mBackIt(e3));
    elist4.push_back(mBackIt(e4));
    vector<node_list_it_t>nlist4 = g1.edgeInducedNodes(elist4);

    vector<edge_list_it_t>elist5;
    elist5.push_back(mBackIt(e1));
    elist5.push_back(mBackIt(e2));
    elist5.push_back(mBackIt(e6));
    elist5.push_back(mBackIt(e8));
    vector<node_list_it_t>nlist5 = g1.edgeInducedNodes(elist5);

    // Checks
    bool n1_found = false;
    bool n2_found = false;
    bool n3_found = false;
    bool n4_found = false;
    bool n5_found = false;

    EXPECT_EQ(nlist1.size(),0);

    EXPECT_EQ(nlist2.size(),2);
    n1_found = false;
    n2_found = false;
    n3_found = false;
    n4_found = false;
    n5_found = false;
    for(auto n : nlist2 ){
        if (n == mBackIt(n1)){
            n1_found = true;
        }
        if (n == mBackIt(n2)){
            n2_found = true;
        }
        if (n == mBackIt(n3)){
            n3_found = true;
        }
        if (n == mBackIt(n4)){
            n4_found = true;
        }
        if (n == mBackIt(n5)){
            n5_found = true;
        }
    }
    EXPECT_EQ( n1_found && 
              !n2_found &&
               n3_found && 
              !n4_found &&
              !n5_found,   true);
    
    EXPECT_EQ(nlist3.size(),4);
    n1_found = false;
    n2_found = false;
    n3_found = false;
    n4_found = false;
    n5_found = false;
    for(auto n : nlist3 ){
        if (n == mBackIt(n1)){
            n1_found = true;
        }
        if (n == mBackIt(n2)){
            n2_found = true;
        }
        if (n == mBackIt(n3)){
            n3_found = true;
        }
        if (n == mBackIt(n4)){
            n4_found = true;
        }
        if (n == mBackIt(n5)){
            n5_found = true;
        }
    }
    EXPECT_EQ(n1_found && n2_found && n3_found && n4_found && !n5_found, true);

    EXPECT_EQ(nlist4.size(),3);
    n1_found = false;
    n2_found = false;
    n3_found = false;
    n4_found = false;
    n5_found = false;
    for(auto n : nlist4 ){
        if (n == mBackIt(n1)){
            n1_found = true;
        }
        if (n == mBackIt(n2)){
            n2_found = true;
        }
        if (n == mBackIt(n3)){
            n3_found = true;
        }
        if (n == mBackIt(n4)){
            n4_found = true;
        }
        if (n == mBackIt(n5)){
            n5_found = true;
        }
    }
    EXPECT_EQ( n1_found &&
               n2_found &&
               n3_found &&
              !n4_found &&
              !n5_found,    true);

    EXPECT_EQ(nlist5.size(),4);
    n1_found = false;
    n2_found = false;
    n3_found = false;
    n4_found = false;
    n5_found = false;
    for(auto n : nlist5 ){
        if (n == mBackIt(n1)){
            n1_found = true;
        }
        if (n == mBackIt(n2)){
            n2_found = true;
        }
        if (n == mBackIt(n3)){
            n3_found = true;
        }
        if (n == mBackIt(n4)){
            n4_found = true;
        }
        if (n == mBackIt(n5)){
            n5_found = true;
        }
    }
    EXPECT_EQ(n1_found && !n2_found && n3_found && n4_found && n5_found, true);
}


// Find cutset.
TEST_F(GraphTests, findCutSetInGraph) {

    /** @brief tests findCutSet() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *         cutset1 = nodeInducedEdges([],[])
     *         cutset2 = nodeInducedEdges([e1],[])
     *         cutset3 = nodeInducedEdges([],[n1])
     *         cutset4 = nodeInducedEdges([e1,e7],[n1,n3,n5]])
     *         cutset5 = nodeInducedEdges([e1,e3,e5,e6][n1,n2,n3,n4])
     *         cutset6 = nodeInducedEdges([e1,e2,e3,e4,e5,e6,e7,e8],
     *                                    [n1,n2,n3,n4,n5])
     *
     *     - Results to be checked.
     *       cutset1 == []
     *       cutset2 == []
     *       cutset3 == [e1,e2,e3]
     *       cutset4 == [e3,e2,e4,e6,e8]
     *       cutset5 == [e2,e4,e7,e8]
     *       cutset6 == []
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Edge> ep9 = generate_edge();
    std::unique_ptr<Edge> ep10= generate_edge();
    std::unique_ptr<Node> np6 = generate_node();


    // Target operation
    vector<node_list_it_t> nlist1;
    vector<edge_list_it_t> elist1;
    vector<edge_list_it_t> cutset1 = g1.findCutSet(elist1,nlist1);

    vector<node_list_it_t> nlist2;
    vector<edge_list_it_t> elist2;
    elist2.push_back(mBackIt(e1));
    vector<edge_list_it_t> cutset2 = g1.findCutSet(elist2,nlist2);

    vector<node_list_it_t> nlist3;
    vector<edge_list_it_t> elist3;
    nlist3.push_back(mBackIt(n1));
    vector<edge_list_it_t> cutset3 = g1.findCutSet(elist3,nlist3);

    vector<node_list_it_t> nlist4;
    vector<edge_list_it_t> elist4;
    elist4.push_back(mBackIt(e1));
    elist4.push_back(mBackIt(e7));
    nlist4.push_back(mBackIt(n1));
    nlist4.push_back(mBackIt(n3));
    nlist4.push_back(mBackIt(n5));
    vector<edge_list_it_t> cutset4 = g1.findCutSet(elist4,nlist4);

    vector<node_list_it_t> nlist5;
    vector<edge_list_it_t> elist5;
    elist5.push_back(mBackIt(e1));
    elist5.push_back(mBackIt(e3));
    elist5.push_back(mBackIt(e5));
    elist5.push_back(mBackIt(e6));
    nlist5.push_back(mBackIt(n1));
    nlist5.push_back(mBackIt(n2));
    nlist5.push_back(mBackIt(n3));
    nlist5.push_back(mBackIt(n4));
    vector<edge_list_it_t> cutset5 = g1.findCutSet(elist5,nlist5);

    vector<node_list_it_t> nlist6;
    vector<edge_list_it_t> elist6;
    elist6.push_back(mBackIt(e1));
    elist6.push_back(mBackIt(e2));
    elist6.push_back(mBackIt(e3));
    elist6.push_back(mBackIt(e4));
    elist6.push_back(mBackIt(e5));
    elist6.push_back(mBackIt(e6));
    elist6.push_back(mBackIt(e7));
    elist6.push_back(mBackIt(e8));
    nlist6.push_back(mBackIt(n1));
    nlist6.push_back(mBackIt(n2));
    nlist6.push_back(mBackIt(n3));
    nlist6.push_back(mBackIt(n4));
    nlist6.push_back(mBackIt(n5));
    vector<edge_list_it_t> cutset6 = g1.findCutSet(elist6,nlist6);

    // Checks
    bool e1_found = false;
    bool e2_found = false;
    bool e3_found = false;
    bool e4_found = false;
    bool e5_found = false;
    bool e6_found = false;
    bool e7_found = false;
    bool e8_found = false;

    EXPECT_EQ(cutset1.size(),0);
    EXPECT_EQ(cutset2.size(),0);

    e1_found = false;
    e2_found = false;
    e3_found = false;
    e4_found = false;
    e5_found = false;
    e6_found = false;
    e7_found = false;
    e8_found = false;
    EXPECT_EQ(cutset3.size(),3);
    for(auto e : cutset3 ){
        if (e == mBackIt(e1)){
            e1_found = true;
        }
        if (e == mBackIt(e2)){
            e2_found = true;
        }
        if (e == mBackIt(e3)){
            e3_found = true;
        }
        if (e == mBackIt(e4)){
            e4_found = true;
        }
        if (e == mBackIt(e5)){
            e5_found = true;
        }
        if (e == mBackIt(e6)){
            e6_found = true;
        }
        if (e == mBackIt(e7)){
            e7_found = true;
        }
        if (e == mBackIt(e8)){
            e8_found = true;
        }
    }
    EXPECT_EQ( e1_found &&
               e2_found &&
               e3_found &&
              !e4_found && 
              !e5_found && 
              !e6_found && 
              !e7_found && 
              !e8_found    , true);

    e1_found = false;
    e2_found = false;
    e3_found = false;
    e4_found = false;
    e5_found = false;
    e6_found = false;
    e7_found = false;
    e8_found = false;
    EXPECT_EQ(cutset3.size(),3);
    for(auto e : cutset3 ){
        if (e == mBackIt(e1)){
            e1_found = true;
        }
        if (e == mBackIt(e2)){
            e2_found = true;
        }
        if (e == mBackIt(e3)){
            e3_found = true;
        }
        if (e == mBackIt(e4)){
            e4_found = true;
        }
        if (e == mBackIt(e5)){
            e5_found = true;
        }
        if (e == mBackIt(e6)){
            e6_found = true;
        }
        if (e == mBackIt(e7)){
            e7_found = true;
        }
        if (e == mBackIt(e8)){
            e8_found = true;
        }
    }
    EXPECT_EQ( e1_found && 
               e2_found && 
               e3_found &&
              !e4_found &&
              !e5_found &&
              !e6_found &&
              !e7_found &&
              !e8_found,    true);

    e1_found = false;
    e2_found = false;
    e3_found = false;
    e4_found = false;
    e5_found = false;
    e6_found = false;
    e7_found = false;
    e8_found = false;
    EXPECT_EQ(cutset4.size(),5);
    for(auto e : cutset4 ){
        if (e == mBackIt(e1)){
            e1_found = true;
        }
        if (e == mBackIt(e2)){
            e2_found = true;
        }
        if (e == mBackIt(e3)){
            e3_found = true;
        }
        if (e == mBackIt(e4)){
            e4_found = true;
        }
        if (e == mBackIt(e5)){
            e5_found = true;
        }
        if (e == mBackIt(e6)){
            e6_found = true;
        }
        if (e == mBackIt(e7)){
            e7_found = true;
        }
        if (e == mBackIt(e8)){
            e8_found = true;
        }
    }
    EXPECT_EQ(!e1_found && 
               e2_found && 
               e3_found &&
               e4_found &&
              !e5_found &&
               e6_found &&
              !e7_found &&
               e8_found,    true);


    e1_found = false;
    e2_found = false;
    e3_found = false;
    e4_found = false;
    e5_found = false;
    e6_found = false;
    e7_found = false;
    e8_found = false;
    EXPECT_EQ(cutset5.size(),4);
    for(auto e : cutset5 ){
        if (e == mBackIt(e1)){
            e1_found = true;
        }
        if (e == mBackIt(e2)){
            e2_found = true;
        }
        if (e == mBackIt(e3)){
            e3_found = true;
        }
        if (e == mBackIt(e4)){
            e4_found = true;
        }
        if (e == mBackIt(e5)){
            e5_found = true;
        }
        if (e == mBackIt(e6)){
            e6_found = true;
        }
        if (e == mBackIt(e7)){
            e7_found = true;
        }
        if (e == mBackIt(e8)){
            e8_found = true;
        }
    }
    EXPECT_EQ(!e1_found && 
               e2_found && 
              !e3_found &&
               e4_found &&
              !e5_found &&
              !e6_found &&
               e7_found &&
               e8_found,    true);

    EXPECT_EQ(cutset6.size(),0);
}


// Remove cutset.
TEST_F(GraphTests, removeCutSetInGraph) {

    /** @brief tests removeCutSet() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *     - Operation
     *         cutset1 = removeCutSet([e1,e3,e4],[n1,n2,n3])
     *
     *     - Results to be checked.
     *         cutset1 == [e2,e5,e6,e7]
     *         g1.mNodes == [n1,n2,n3,n4,n5]
     *         g1.mEdges == [e1,e3,e4,e8]
     *         n1.mIncidence == [e1,e3]
     *         n2.mIncidence == [e3,e4]
     *         n3.mIncidence == [e1,e4]
     *         n4.mIncidence == [e8]
     *         n5.mIncidence == [e8]
     *
     *         e2.mGraph == nullptr
     *         e5.mGraph == nullptr
     *         e6.mGraph == nullptr
     *         e7.mGraph == nullptr
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);


    // Target operation
    vector<node_list_it_t> nlist1;
    vector<edge_list_it_t> elist1;
    elist1.push_back(mBackIt(e1));
    elist1.push_back(mBackIt(e3));
    elist1.push_back(mBackIt(e4));
    nlist1.push_back(mBackIt(n1));
    nlist1.push_back(mBackIt(n2));
    nlist1.push_back(mBackIt(n3));
    vector<edge_ptr_t> cutset1 = g1.removeCutSet(elist1,nlist1);

    // Checks

    bool e1_found = false;
    bool e2_found = false;
    bool e3_found = false;
    bool e4_found = false;
    bool e5_found = false;
    bool e6_found = false;
    bool e7_found = false;
    bool e8_found = false;

    EXPECT_EQ(cutset1.size(),4);
    for(auto& e : cutset1 ){
        if (&(*e) == &e1) {
            e2_found = true;
        }
        if (&(*e) == &e2) {
            e2_found = true;
        }
        if (&(*e) == &e3) {
            e2_found = true;
        }
        if (&(*e) == &e4) {
            e2_found = true;
        }
        if (&(*e) == &e5) {
            e5_found = true;
        }
        if (&(*e) == &e6) {
            e6_found = true;
        }
        if (&(*e) == &e7) {
            e7_found = true;
        }
        if (&(*e) == &e8) {
            e7_found = true;
        }
    }
    EXPECT_EQ(!e1_found && 
               e2_found && 
              !e3_found &&
              !e4_found &&
               e5_found &&
               e6_found &&
               e7_found &&
              !e8_found,    true);

    EXPECT_EQ(mNodes(g1).size(),5);
    auto nit = mNodes(g1).begin();
    nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n1);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n4);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);
    EXPECT_EQ(mEdges(g1).size(),4);
    auto eit = mEdges(g1).begin();
    eit = mEdges(g1).begin();
    EXPECT_EQ(&(*(*eit)),&e1);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e3);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e4);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e8);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n1_incidence_pair = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;

    pair<node_incidence_it_t,node_incidence_it_t>
                           n2_incidence_pair = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_incidence_pair = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n4_incidence_pair = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n5_incidence_pair = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    node_incidence_t& n1_mIncidence = mIncidence(n1);
    node_incidence_t& n2_mIncidence = mIncidence(n2);
    node_incidence_t& n3_mIncidence = mIncidence(n3);
    node_incidence_t& n4_mIncidence = mIncidence(n4);
    node_incidence_t& n5_mIncidence = mIncidence(n5);

    EXPECT_EQ(n1_mIncidence.size(),2);
    EXPECT_EQ(n2_mIncidence.size(),2);
    EXPECT_EQ(n3_mIncidence.size(),2);
    EXPECT_EQ(n4_mIncidence.size(),1);
    EXPECT_EQ(n5_mIncidence.size(),1);

    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3);

    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);

    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8);

    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8);

    EXPECT_EQ(mGraph(e2), nullptr);
    EXPECT_EQ(mGraph(e5), nullptr);
    EXPECT_EQ(mGraph(e6), nullptr);
    EXPECT_EQ(mGraph(e7), nullptr);

}


// Move Node-induced subgraph. several cases
TEST_F(GraphTests, moveNodeInducedSubgraph) {

    /** @brief tests moveEdgeInducedSubgraph() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *       g2 has 2 nodes n6, n7, and 1 edge e9.
     *         n6 e9 n7
     *         n6-> e9(n7)
     *         n7-> e9(n6)
     *
     *     - Operation
     *         nodes = [n1,n2,n3]
     *         cutset1 = moveNodeInducedSubgraph(nodes, g2)
     *
     *     - Results to be checked.
     *         cutset1 == [e2,e5,e6,e7]
     *         g1.mNodes == [n4,n5]
     *         g1.mEdges == [e8]
     *         n4.mIncidence == [e8]
     *         n5.mIncidence == [e8]
     *
     *         g2.mNodes == [n6,n7,n1,n2,n3]
     *         g2.mEdges == [e9,e1,e3,e4]
     *         n6.mIncidence == [e9]    
     *         n7.mIncidence == [e9]    
     *         n1.mIncidence == [e1,e3]
     *         n2.mIncidence == [e3,e4]
     *         n3.mIncidence == [e1,e4]
     *
     *         e2.mGraph == nullptr
     *         e5.mGraph == nullptr
     *         e6.mGraph == nullptr
     *         e7.mGraph == nullptr
     *
     *         nodes == [n1(g2.mNodes[2]), n2(g2.mNodes[3]), n3(g2.mNodes[3])]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    Graph& g2 = Graph2();

    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Node> np6 = generate_node();
    Node& n6 = g2.addNode(std::move(np6));
    std::unique_ptr<Node> np7 = generate_node();
    Node& n7 = g2.addNode(std::move(np7));

    std::unique_ptr<Edge> ep9 = generate_edge();
    Edge& e9 = g2.addEdge(std::move(ep9),n6,n7);

    // Target operation
    vector<node_list_it_t> nlist1;
    nlist1.push_back(mBackIt(n1));
    nlist1.push_back(mBackIt(n2));
    nlist1.push_back(mBackIt(n3));

    vector<edge_ptr_t> cutset1 = g1.moveNodeInducedSubgraph(nlist1,g2);

    // Checks
    bool e1_found = false;
    bool e2_found = false;
    bool e3_found = false;
    bool e4_found = false;
    bool e5_found = false;
    bool e6_found = false;
    bool e7_found = false;
    bool e8_found = false;
    bool e9_found = false;
    bool ex_found = false;

    EXPECT_EQ(cutset1.size(),4);
    for(auto& e : cutset1 ){
        if (&(*e) == &e1) {
            e2_found = true;
        }
        else if (&(*e) == &e2) {
            e2_found = true;
        }
        else if (&(*e) == &e3) {
            e3_found = true;
        }
        else if (&(*e) == &e4) {
            e4_found = true;
        }
        else if (&(*e) == &e5) {
            e5_found = true;
        }
        else if (&(*e) == &e6) {
            e6_found = true;
        }
        else if (&(*e) == &e7) {
            e7_found = true;
        }
        else if (&(*e) == &e8) {
            e8_found = true;
        }
        else if (&(*e) == &e9) {
            e9_found = true;
        }
        else{
            ex_found = true;
        }
    }
    EXPECT_EQ(!e1_found && 
               e2_found && 
              !e3_found &&
              !e4_found &&
               e5_found &&
               e6_found &&
               e7_found &&
              !e8_found && 
              !e9_found &&
              !ex_found,    true);

    EXPECT_EQ(mNodes(g1).size(),2);
    auto nit = mNodes(g1).begin();
    nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n4);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);
    EXPECT_EQ(mEdges(g1).size(),1);
    auto eit = mEdges(g1).begin();
    eit = mEdges(g1).begin();
    EXPECT_EQ(&(*(*eit)),&e8);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n4_incidence_pair = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n5_incidence_pair = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    node_incidence_t& n4_mIncidence = mIncidence(n4);
    node_incidence_t& n5_mIncidence = mIncidence(n5);

    EXPECT_EQ(n4_mIncidence.size(),1);
    EXPECT_EQ(n5_mIncidence.size(),1);

    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8);
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8);

    EXPECT_EQ(mNodes(g2).size(),5);
    auto nit2 = mNodes(g2).begin();
    EXPECT_EQ(&(*(*nit2)),&n6);
    nit2++;
    EXPECT_EQ(&(*(*nit2)),&n7);
    nit2++;
    EXPECT_EQ(&(*(*nit2)),&n1);
    nit2++;
    EXPECT_EQ(&(*(*nit2)),&n2);
    nit2++;
    EXPECT_EQ(&(*(*nit2)),&n3);

    EXPECT_EQ(mEdges(g2).size(),4);
    auto eit2 = mEdges(g2).begin();
    eit2 = mEdges(g2).begin();
    EXPECT_EQ(&(*(*eit2)),&e9);
    eit2++;
    EXPECT_EQ(&(*(*eit2)),&e1);
    eit2++;
    EXPECT_EQ(&(*(*eit2)),&e3);
    eit2++;
    EXPECT_EQ(&(*(*eit2)),&e4);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n6_incidence_pair = n6.incidentEdges();
    node_incidence_it_t n6_incidence_it = n6_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n7_incidence_pair = n7.incidentEdges();
    node_incidence_it_t n7_incidence_it = n7_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n1_incidence_pair = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n2_incidence_pair = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_incidence_pair = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;

    node_incidence_t& n6_mIncidence = mIncidence(n6);
    node_incidence_t& n7_mIncidence = mIncidence(n7);
    node_incidence_t& n1_mIncidence = mIncidence(n1);
    node_incidence_t& n2_mIncidence = mIncidence(n2);
    node_incidence_t& n3_mIncidence = mIncidence(n3);

    EXPECT_EQ(n6_mIncidence.size(),1);
    EXPECT_EQ(n7_mIncidence.size(),1);
    EXPECT_EQ(n1_mIncidence.size(),2);
    EXPECT_EQ(n2_mIncidence.size(),2);
    EXPECT_EQ(n3_mIncidence.size(),2);

    EXPECT_EQ(&(*(*(*n6_incidence_it))),&e9);
    EXPECT_EQ(&(*(*(*n7_incidence_it))),&e9);
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3);
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);

    EXPECT_EQ(mGraph(e2) , nullptr);
    EXPECT_EQ(mGraph(e5) , nullptr);
    EXPECT_EQ(mGraph(e6) , nullptr);
    EXPECT_EQ(mGraph(e7) , nullptr);

    nit = mNodes(g2).begin();
    nit++;
    nit++;
    EXPECT_EQ(&(*(*nit)), &n1);
    nit++;
    EXPECT_EQ(&(*(*nit)), &n2);
    nit++;
    EXPECT_EQ(&(*(*nit)), &n3);

    for (auto nit1=g1.nodes().first; nit1!=g1.nodes().second; nit1++) {
        EXPECT_EQ((*nit1)->utilitySize(), 0);
    }
    for (auto eit1=g1.edges().first; eit1!=g1.edges().second; eit1++) {
        EXPECT_EQ((*eit1)->utilitySize(), 0);
    }
    for (auto nit2=g2.nodes().first; nit2!=g2.nodes().second; nit2++) {
        EXPECT_EQ((*nit2)->utilitySize(), 0);
    }
    for (auto eit2=g2.edges().first; eit2!=g2.edges().second; eit2++) {
        EXPECT_EQ((*eit2)->utilitySize(), 0);
    }

}


// Move Edge-induced subgraph. several cases
TEST_F(GraphTests, moveEdgeInducedSubgraph) {

    /** @brief tests moveNodeInducedSubgraph() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *       g2 has 2 nodes n6, n7, and 1 edge e9.
     *         n6 e9 n7
     *         n6-> e9(n7)
     *         n7-> e9(n6)
     *
     *     - Operation
     *         edges = [e4,e5,e6]
     *         cutset1 = moveEdgeInducedSubgraph(edges, g2)
     *
     *     - Results to be checked.
     *         cutset1 == [e1,e2,e3,e7,e8]
     *         g1.mNodes == [n1,n5]
     *         g1.mEdges == []
     *         n1.mIncidence == []
     *         n5.mIncidence == []
     *
     *         g2.mNodes == [n6,n7,n2,n3,n4]
     *         g2.mEdges == [e9,e4,e5,e6]
     *         n6.mIncidence == [e9]
     *         n7.mIncidence == [e9]
     *         n2.mIncidence == [e4,e5]
     *         n3.mIncidence == [e4,e6]
     *         n4.mIncidence == [e5,e6]
     *
     *         e1.mGraph == nullptr
     *         e2.mGraph == nullptr
     *         e3.mGraph == nullptr
     *         e7.mGraph == nullptr
     *         e8.mGraph == nullptr
     *
     *         edges = [e4(g2.mEdges[1]), e5(g2.mEdges[2]), e6(g2.mEdges[3])]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    Graph& g2 = Graph2();

    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Node> np6 = generate_node();
    Node& n6 = g2.addNode(std::move(np6));
    std::unique_ptr<Node> np7 = generate_node();
    Node& n7 = g2.addNode(std::move(np7));

    std::unique_ptr<Edge> ep9 = generate_edge();
    Edge& e9 = g2.addEdge(std::move(ep9),n6,n7);

    // Target operation
    vector<edge_list_it_t> elist1;
    elist1.push_back(mBackIt(e4));
    elist1.push_back(mBackIt(e5));
    elist1.push_back(mBackIt(e6));
    vector<edge_ptr_t> cutset1 = g1.moveEdgeInducedSubgraph(elist1,g2);

    // Checks
    bool e1_found = false;
    bool e2_found = false;
    bool e3_found = false;
    bool e4_found = false;
    bool e5_found = false;
    bool e6_found = false;
    bool e7_found = false;
    bool e8_found = false;
    bool e9_found = false;
    bool ex_found = false;

    EXPECT_EQ(cutset1.size(),5);
    for(auto& e : cutset1 ){
        if (&(*e) == &e1) {
            e1_found = true;
        }
        else if (&(*e) == &e2) {
            e2_found = true;
        }
        else if (&(*e) == &e3) {
            e3_found = true;
        }
        else if (&(*e) == &e4) {
            e4_found = true;
        }
        else if (&(*e) == &e5) {
            e5_found = true;
        }
        else if (&(*e) == &e6) {
            e6_found = true;
        }
        else if (&(*e) == &e7) {
            e7_found = true;
        }
        else if (&(*e) == &e8) {
            e8_found = true;
        }
        else if (&(*e) == &e9) {
            e9_found = true;
        }
        else {
            ex_found = true;
        }
    }
    EXPECT_EQ( e1_found && 
               e2_found && 
               e3_found &&
              !e4_found &&
              !e5_found &&
              !e6_found &&
               e7_found &&
               e8_found &&
              !e9_found &&
              !ex_found,    true);

    EXPECT_EQ(mNodes(g1).size(),2);
    auto nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n1);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);

    EXPECT_EQ(mEdges(g1).size(),0);

//    pair<node_incidence_it_t,node_incidence_it_t>
//                           n1_incidence_pair = n1.incidentEdges();
//    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
//    pair<node_incidence_it_t,node_incidence_it_t>
//                           n5_incidence_pair = n5.incidentEdges();
//    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    node_incidence_t& n1_mIncidence = mIncidence(n1);
    node_incidence_t& n5_mIncidence = mIncidence(n5);

    EXPECT_EQ(n1_mIncidence.size(),0);
    EXPECT_EQ(n5_mIncidence.size(),0);

    EXPECT_EQ(mNodes(g2).size(),5);
    nit = mNodes(g2).begin();
    EXPECT_EQ(&(*(*nit)),&n6);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n7);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n4);

    EXPECT_EQ(mEdges(g2).size(),4);
    auto eit = mEdges(g2).begin();
    EXPECT_EQ(&(*(*eit)),&e9);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e4);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e5);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e6);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n6_incidence_pair = n6.incidentEdges();
    node_incidence_it_t n6_incidence_it = n6_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n7_incidence_pair = n7.incidentEdges();
    node_incidence_it_t n7_incidence_it = n7_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n2_incidence_pair = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_incidence_pair = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n4_incidence_pair = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;

    node_incidence_t& n6_mIncidence = mIncidence(n6);
    node_incidence_t& n7_mIncidence = mIncidence(n7);
    node_incidence_t& n2_mIncidence = mIncidence(n2);
    node_incidence_t& n3_mIncidence = mIncidence(n3);
    node_incidence_t& n4_mIncidence = mIncidence(n4);

    EXPECT_EQ(n6_mIncidence.size(),1);
    EXPECT_EQ(n7_mIncidence.size(),1);
    EXPECT_EQ(n2_mIncidence.size(),2);
    EXPECT_EQ(n3_mIncidence.size(),2);
    EXPECT_EQ(n4_mIncidence.size(),2);

    EXPECT_EQ(&(*(*(*n6_incidence_it))),&e9);
    EXPECT_EQ(&(*(*(*n7_incidence_it))),&e9);
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5);
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e6);
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e6);

    eit = mEdges(g2).begin();
    eit++;
    EXPECT_EQ(&(*(*eit)), &e4);
    eit++;
    EXPECT_EQ(&(*(*eit)), &e5);
    eit++;
    EXPECT_EQ(&(*(*eit)), &e6);

    EXPECT_EQ(mGraph(e1) , nullptr);
    EXPECT_EQ(mGraph(e2) , nullptr);
    EXPECT_EQ(mGraph(e3) , nullptr);
    EXPECT_EQ(mGraph(e7) , nullptr);
    EXPECT_EQ(mGraph(e8) , nullptr);

    for (auto nit1=g1.nodes().first; nit1!=g1.nodes().second; nit1++) {
        EXPECT_EQ((*nit1)->utilitySize(), 0);
    }
    for (auto eit1=g1.edges().first; eit1!=g1.edges().second; eit1++) {
        EXPECT_EQ((*eit1)->utilitySize(), 0);
    }
    for (auto nit2=g2.nodes().first; nit2!=g2.nodes().second; nit2++) {
        EXPECT_EQ((*nit2)->utilitySize(), 0);
    }
    for (auto eit2=g2.edges().first; eit2!=g2.edges().second; eit2++) {
        EXPECT_EQ((*eit2)->utilitySize(), 0);
    }

}


// copy Node-induced subgraph. several cases
TEST_F(GraphTests, copySubgraph) {

    /** @brief tests copySubgraph() in a graph.
     *
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *
     *       g2 has 2 nodes n6, n7, and 1 edge e9.
     *         n6 e9 n7
     *         n6-> e9(n7)
     *         n7-> e9(n6)
     *
     *       isolated nodes and edges:
     *         n1_2, n3_2, n4_2, n5_2
     *         e1_2, e2_2, e7_2, e8_2
     *
     *     - Operation
     *         node_pairs = [n1, n3, n4, n5]
     *         edge_pairs = [e1, e2, e7, e8]
     *         copySubgraph(node_pairs, edge_pairs, g2)
     *
     *     - Results to be checked.
     *         g1.mNodes     == [n1,n2,n3,n4,n5]
     *         g1.mEdges     == [e1,e2,e3,e4,e5,e6,e7,e8]
     *         n1.mIncidence == [e1, e2, e3]
     *         n2.mIncidence == [e3, e4, e5]
     *         n3.mIncidence == [e1, e4, e6, e7]
     *         n4.mIncidence == [e2, e5, e6, e8]
     *         n5.mIncidence == [e7, e8]
     *
     *         g2.mNodes       == [n6,n7,n1_2,n3_2,n4_2,n5_2]
     *         g2.mEdges       == [e9,e1_2,e2_2,e7_2,e8_2]
     *         n6.mIncidence   == [e9]
     *         n7.mIncidence   == [e9]
     *         n1_2.mIncidence == [e1_2,e2_2]
     *         n3_2.mIncidence == [e1_2,e7_2]
     *         n4_2.mIncidence == [e2_2,e8_2]
     *         n5_2.mIncidence == [e7_2,e8_2]
     *
     *         n6.mBackIt   == g2.mNodes[0]
     *         n7.mBackIt   == g2.mNodes[1]
     *         n1_2.mBackIt == g2.mNodes[2]
     *         n3_2.mBackIt == g2.mNodes[3]
     *         n4_2.mBackIt == g2.mNodes[4]
     *         n5_2.mBackIt == g2.mNodes[5]
     *
     *         n6.mGraph   == g2
     *         n7.mGraph   == g2
     *         n1_2.mGraph == g2
     *         n3_2.mGraph == g2
     *         n4_2.mGraph == g2
     *         n5_2.mGraph == g2
     *
     *         e9.mIncidentNode1   == g2.mNodes[0] n6
     *         e9.mIncidentNode2   == g2.mNodes[1] n7
     *         e1_2.mIncidentNode1 == g2.mNodes[2] n1
     *         e1_2.mIncidentNode2 == g2.mNodes[3] n3
     *         e2_2.mIncidentNode1 == g2.mNodes[2] n1
     *         e2_2.mIncidentNode2 == g2.mNodes[4] n4
     *         e7_2.mIncidentNode1 == g2.mNodes[3] n3
     *         e7_2.mIncidentNode2 == g2.mNodes[5] n5
     *         e8_2.mIncidentNode1 == g2.mNodes[4] n4
     *         e8_2.mIncidentNode2 == g2.mNodes[5] n5
     *
     *         e9.mBackItNode1     == n6.mIncidence[0]
     *         e9.mBackItNode2     == n7.mIncidence[0]
     *         e1_2.mBackItNode1   == n1.mIncidence[0]
     *         e1_2.mBackItNode2   == n3.mIncidence[0]
     *         e2_2.mBackItNode1   == n1.mIncidence[1]
     *         e2_2.mBackItNode2   == n4.mIncidence[0]
     *         e7_2.mBackItNode1   == n3.mIncidence[1]
     *         e7_2.mBackItNode2   == n5.mIncidence[0]
     *         e8_2.mBackItNode1   == n4.mIncidence[1]
     *         e8_2.mBackItNode2   == n5.mIncidence[1]
     * 
     *         e9.mBackIt   == g2.mEdges[0]
     *         e1_2.mBackIt == g2.mEdges[1]
     *         e2_2.mBackIt == g2.mEdges[2]
     *         e7_2.mBackIt == g2.mEdges[3]
     *         e8_2.mBackIt == g2.mEdges[4]
     *
     *         e9.mGraph   == g2
     *         e1_2.mGraph == g2
     *         e2_2.mGraph == g2
     *         e7_2.mGraph == g2
     *         e8_2.mGraph == g2
     */

    // Preparation.    
    Graph& g1 = Graph1();
    Graph& g2 = Graph2();

    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Node> np6 = generate_node();
    Node& n6 = g2.addNode(std::move(np6));
    std::unique_ptr<Node> np7 = generate_node();
    Node& n7 = g2.addNode(std::move(np7));

    std::unique_ptr<Edge> ep9 = generate_edge();
    Edge& e9 = g2.addEdge(std::move(ep9),n6,n7);

    std::unique_ptr<Node> np1_2 = generate_node();
    Node& n1_2 = *np1_2;
    std::unique_ptr<Node> np3_2 = generate_node();
    Node& n3_2 = *np3_2;
    std::unique_ptr<Node> np4_2 = generate_node();
    Node& n4_2 = *np4_2;
    std::unique_ptr<Node> np5_2 = generate_node();
    Node& n5_2 = *np5_2;

    std::unique_ptr<Edge> ep1_2 = generate_edge();
    Edge& e1_2 = *ep1_2;
    std::unique_ptr<Edge> ep2_2 = generate_edge();
    Edge& e2_2 = *ep2_2;
    std::unique_ptr<Edge> ep7_2 = generate_edge();
    Edge& e7_2 = *ep7_2;
    std::unique_ptr<Edge> ep8_2 = generate_edge();
    Edge& e8_2 = *ep8_2;

    // Target operation
    pair<node_list_it_t,node_ptr_t>nodePair1(mBackIt(n1),std::move(np1_2));
    pair<node_list_it_t,node_ptr_t>nodePair2(mBackIt(n3),std::move(np3_2));
    pair<node_list_it_t,node_ptr_t>nodePair3(mBackIt(n4),std::move(np4_2));
    pair<node_list_it_t,node_ptr_t>nodePair4(mBackIt(n5),std::move(np5_2));
    vector < pair< node_list_it_t, node_ptr_t > > nodePairs;
    nodePairs.push_back(std::move(nodePair1));
    nodePairs.push_back(std::move(nodePair2));
    nodePairs.push_back(std::move(nodePair3));
    nodePairs.push_back(std::move(nodePair4));

    pair<edge_list_it_t,edge_ptr_t>edgePair1(mBackIt(e1),std::move(ep1_2));
    pair<edge_list_it_t,edge_ptr_t>edgePair2(mBackIt(e2),std::move(ep2_2));
    pair<edge_list_it_t,edge_ptr_t>edgePair3(mBackIt(e7),std::move(ep7_2));
    pair<edge_list_it_t,edge_ptr_t>edgePair4(mBackIt(e8),std::move(ep8_2));
    vector < pair< edge_list_it_t, edge_ptr_t > > edgePairs;
    edgePairs.push_back(std::move(edgePair1));
    edgePairs.push_back(std::move(edgePair2));
    edgePairs.push_back(std::move(edgePair3));
    edgePairs.push_back(std::move(edgePair4));

    g1.copySubgraph(nodePairs, edgePairs, g2);

    // Checks

    // g1
    EXPECT_EQ(mNodes(g1).size(),5);
    auto nit = mNodes(g1).begin();
    EXPECT_EQ(&(*(*nit)),&n1);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n4);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5);

    EXPECT_EQ(mEdges(g1).size(),8);
    auto eit = mEdges(g1).begin();
    EXPECT_EQ(&(*(*eit)),&e1);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e3);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e4);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e5);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e6);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e7);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e8);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n1_incidence_pair = n1.incidentEdges();
    node_incidence_it_t n1_incidence_it = n1_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n2_incidence_pair = n2.incidentEdges();
    node_incidence_it_t n2_incidence_it = n2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_incidence_pair = n3.incidentEdges();
    node_incidence_it_t n3_incidence_it = n3_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n4_incidence_pair = n4.incidentEdges();
    node_incidence_it_t n4_incidence_it = n4_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n5_incidence_pair = n5.incidentEdges();
    node_incidence_it_t n5_incidence_it = n5_incidence_pair.first;

    node_incidence_t& n1_mIncidence = mIncidence(n1);
    node_incidence_t& n2_mIncidence = mIncidence(n2);
    node_incidence_t& n3_mIncidence = mIncidence(n3);
    node_incidence_t& n4_mIncidence = mIncidence(n4);
    node_incidence_t& n5_mIncidence = mIncidence(n5);

    EXPECT_EQ(mGraph(n1),&g1);
    EXPECT_EQ(mGraph(n2),&g1);
    EXPECT_EQ(mGraph(n3),&g1);
    EXPECT_EQ(mGraph(n4),&g1);
    EXPECT_EQ(mGraph(n5),&g1);
    EXPECT_EQ(mGraph(e1),&g1);
    EXPECT_EQ(mGraph(e2),&g1);
    EXPECT_EQ(mGraph(e3),&g1);
    EXPECT_EQ(mGraph(e4),&g1);
    EXPECT_EQ(mGraph(e5),&g1);
    EXPECT_EQ(mGraph(e6),&g1);
    EXPECT_EQ(mGraph(e7),&g1);
    EXPECT_EQ(mGraph(e8),&g1);

    nit = mNodes(g1).begin();
    EXPECT_EQ(mBackIt(n1),nit);
    EXPECT_EQ(n1_mIncidence.size(),3);
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e1);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e2);
    n1_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_incidence_it))),&e3);

    nit = mNodes(g1).begin();
    nit++;
    EXPECT_EQ(mBackIt(n2),nit);
    EXPECT_EQ(n2_mIncidence.size(),3);
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e3);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e4);
    n2_incidence_it++;
    EXPECT_EQ(&(*(*(*n2_incidence_it))),&e5);

    nit = mNodes(g1).begin();
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n3),nit);
    EXPECT_EQ(n3_mIncidence.size(),4);
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e1);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e4);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e6);
    n3_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_incidence_it))),&e7);

    nit = mNodes(g1).begin();
    nit++;
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n4),nit);
    EXPECT_EQ(n4_mIncidence.size(),4);
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e2);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e5);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e6);
    n4_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_incidence_it))),&e8);

    nit = mNodes(g1).begin();
    nit++;
    nit++;
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n5),nit);
    EXPECT_EQ(n5_mIncidence.size(),2);
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e7);
    n5_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_incidence_it))),&e8);

    eit = mEdges(g1).begin();
    EXPECT_EQ(mBackIt(e1),eit);
    auto  e1_n1_it =mIncidentNode1(e1);
    EXPECT_EQ(&(*(*e1_n1_it)),&n1);
    auto  e1_n2_it =mIncidentNode2(e1);
    EXPECT_EQ(&(*(*e1_n2_it)),&n3);
    auto e1_n1_backIt = mBackItNode1(e1);
    n1_incidence_it = n1_incidence_pair.first;
    EXPECT_EQ(e1_n1_backIt,n1_incidence_it);
    auto e1_n2_backIt = mBackItNode2(e1);
    n3_incidence_it = n3_incidence_pair.first;
    EXPECT_EQ(e1_n2_backIt,n3_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    EXPECT_EQ(mBackIt(e2),eit);
    auto  e2_n1_it =mIncidentNode1(e2);
    EXPECT_EQ(&(*(*e2_n1_it)),&n1);
    auto  e2_n2_it =mIncidentNode2(e2);
    EXPECT_EQ(&(*(*e2_n2_it)),&n4);
    auto e2_n1_backIt = mBackItNode1(e2);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    EXPECT_EQ(e2_n1_backIt,n1_incidence_it);
    auto e2_n2_backIt = mBackItNode2(e2);
    n4_incidence_it = n4_incidence_pair.first;
    EXPECT_EQ(e2_n2_backIt,n4_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e3),eit);
    auto  e3_n1_it =mIncidentNode1(e3);
    EXPECT_EQ(&(*(*e3_n1_it)),&n1);
    auto  e3_n2_it =mIncidentNode2(e3);
    EXPECT_EQ(&(*(*e3_n2_it)),&n2);
    auto e3_n1_backIt = mBackItNode1(e3);
    n1_incidence_it = n1_incidence_pair.first;
    n1_incidence_it++;
    n1_incidence_it++;
    EXPECT_EQ(e3_n1_backIt,n1_incidence_it);
    auto e3_n2_backIt = mBackItNode2(e3);
    n2_incidence_it = n2_incidence_pair.first;
    EXPECT_EQ(e3_n2_backIt,n2_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e4),eit);
    auto  e4_n1_it =mIncidentNode1(e4);
    EXPECT_EQ(&(*(*e4_n1_it)),&n2);
    auto  e4_n2_it =mIncidentNode2(e4);
    EXPECT_EQ(&(*(*e4_n2_it)),&n3);
    auto e4_n1_backIt = mBackItNode1(e4);
    n2_incidence_it = n2_incidence_pair.first;
    n2_incidence_it++;
    EXPECT_EQ(e4_n1_backIt,n2_incidence_it);
    auto e4_n2_backIt = mBackItNode2(e4);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    EXPECT_EQ(e4_n2_backIt,n3_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e5),eit);
    auto  e5_n1_it =mIncidentNode1(e5);
    EXPECT_EQ(&(*(*e5_n1_it)),&n2);
    auto  e5_n2_it =mIncidentNode2(e5);
    EXPECT_EQ(&(*(*e5_n2_it)),&n4);
    auto e5_n1_backIt = mBackItNode1(e5);
    n2_incidence_it = n2_incidence_pair.first;
    n2_incidence_it++;
    n2_incidence_it++;
    EXPECT_EQ(e5_n1_backIt,n2_incidence_it);
    auto e5_n2_backIt = mBackItNode2(e5);
    n4_incidence_it = n4_incidence_pair.first;
    n4_incidence_it++;
    EXPECT_EQ(e5_n2_backIt,n4_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e6),eit);
    auto  e6_n1_it =mIncidentNode1(e6);
    EXPECT_EQ(&(*(*e6_n1_it)),&n3);
    auto  e6_n2_it =mIncidentNode2(e6);
    EXPECT_EQ(&(*(*e6_n2_it)),&n4);
    auto e6_n1_backIt = mBackItNode1(e6);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e6_n1_backIt,n3_incidence_it);
    auto e6_n2_backIt = mBackItNode2(e6);
    n4_incidence_it = n4_incidence_pair.first;
    n4_incidence_it++;
    n4_incidence_it++;
    EXPECT_EQ(e6_n2_backIt,n4_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e7),eit);
    auto  e7_n1_it =mIncidentNode1(e7);
    EXPECT_EQ(&(*(*e7_n1_it)),&n3);
    auto  e7_n2_it =mIncidentNode2(e7);
    EXPECT_EQ(&(*(*e7_n2_it)),&n5);
    auto e7_n1_backIt = mBackItNode1(e7);
    n3_incidence_it = n3_incidence_pair.first;
    n3_incidence_it++;
    n3_incidence_it++;
    n3_incidence_it++;
    EXPECT_EQ(e7_n1_backIt,n3_incidence_it);
    auto e7_n2_backIt = mBackItNode2(e7);
    n5_incidence_it = n5_incidence_pair.first;
    EXPECT_EQ(e7_n2_backIt,n5_incidence_it);

    eit = mEdges(g1).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e8),eit);
    auto  e8_n1_it =mIncidentNode1(e8);
    EXPECT_EQ(&(*(*e8_n1_it)),&n4);
    auto  e8_n2_it =mIncidentNode2(e8);
    EXPECT_EQ(&(*(*e8_n2_it)),&n5);
    auto e8_n1_backIt = mBackItNode1(e8);
    n4_incidence_it = n4_incidence_pair.first;
    n4_incidence_it++;
    n4_incidence_it++;
    n4_incidence_it++;
    EXPECT_EQ(e8_n1_backIt,n4_incidence_it);
    auto e8_n2_backIt = mBackItNode2(e8);
    n5_incidence_it = n5_incidence_pair.first;
    n5_incidence_it++;
    EXPECT_EQ(e8_n2_backIt,n5_incidence_it);

    // g2
    EXPECT_EQ(mNodes(g2).size(),6);
    nit = mNodes(g2).begin();
    EXPECT_EQ(&(*(*nit)),&n6);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n7);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n1_2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n3_2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n4_2);
    nit++;
    EXPECT_EQ(&(*(*nit)),&n5_2);

    EXPECT_EQ(mEdges(g2).size(),5);
    eit = mEdges(g2).begin();
    EXPECT_EQ(&(*(*eit)),&e9);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e1_2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e2_2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e7_2);
    eit++;
    EXPECT_EQ(&(*(*eit)),&e8_2);

    pair<node_incidence_it_t,node_incidence_it_t>
                           n6_incidence_pair = n6.incidentEdges();
    node_incidence_it_t n6_incidence_it = n6_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n7_incidence_pair = n7.incidentEdges();
    node_incidence_it_t n7_incidence_it = n7_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n1_2_incidence_pair = n1_2.incidentEdges();
    node_incidence_it_t n1_2_incidence_it = n1_2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n3_2_incidence_pair = n3_2.incidentEdges();
    node_incidence_it_t n3_2_incidence_it = n3_2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n4_2_incidence_pair = n4_2.incidentEdges();
    node_incidence_it_t n4_2_incidence_it = n4_2_incidence_pair.first;
    pair<node_incidence_it_t,node_incidence_it_t>
                           n5_2_incidence_pair = n5_2.incidentEdges();
    node_incidence_it_t n5_2_incidence_it = n5_2_incidence_pair.first;

    node_incidence_t& n6_mIncidence = mIncidence(n6);
    node_incidence_t& n7_mIncidence = mIncidence(n7);
    node_incidence_t& n1_2_mIncidence = mIncidence(n1_2);
    node_incidence_t& n3_2_mIncidence = mIncidence(n3_2);
    node_incidence_t& n4_2_mIncidence = mIncidence(n4_2);
    node_incidence_t& n5_2_mIncidence = mIncidence(n5_2);

    EXPECT_EQ(mGraph(n6),&g2);
    EXPECT_EQ(mGraph(n7),&g2);
    EXPECT_EQ(mGraph(n1_2),&g2);
    EXPECT_EQ(mGraph(n3_2),&g2);
    EXPECT_EQ(mGraph(n4_2),&g2);
    EXPECT_EQ(mGraph(n5_2),&g2);

    EXPECT_EQ(mGraph(e9),&g2);
    EXPECT_EQ(mGraph(e1_2),&g2);
    EXPECT_EQ(mGraph(e2_2),&g2);
    EXPECT_EQ(mGraph(e7_2),&g2);
    EXPECT_EQ(mGraph(e8_2),&g2);

    nit = mNodes(g2).begin();
    EXPECT_EQ(mBackIt(n6),nit);
    EXPECT_EQ(n6_mIncidence.size(),1);
    n6_incidence_it = n6_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n6_incidence_it))),&e9);

    nit = mNodes(g2).begin();
    nit++;
    EXPECT_EQ(mBackIt(n7),nit);
    EXPECT_EQ(n7_mIncidence.size(),1);
    n7_incidence_it = n7_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n7_incidence_it))),&e9);

    nit = mNodes(g2).begin();
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n1_2),nit);
    EXPECT_EQ(n1_2_mIncidence.size(),2);
    n1_2_incidence_it = n1_2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n1_2_incidence_it))),&e1_2);
    n1_2_incidence_it++;
    EXPECT_EQ(&(*(*(*n1_2_incidence_it))),&e2_2);

    nit = mNodes(g2).begin();
    nit++;
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n3_2),nit);
    EXPECT_EQ(n3_2_mIncidence.size(),2);
    n3_2_incidence_it = n3_2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n3_2_incidence_it))),&e1_2);
    n3_2_incidence_it++;
    EXPECT_EQ(&(*(*(*n3_2_incidence_it))),&e7_2);

    nit = mNodes(g2).begin();
    nit++;
    nit++;
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n4_2),nit);
    EXPECT_EQ(n4_2_mIncidence.size(),2);
    n4_2_incidence_it = n4_2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n4_2_incidence_it))),&e2_2);
    n4_2_incidence_it++;
    EXPECT_EQ(&(*(*(*n4_2_incidence_it))),&e8_2);

    nit = mNodes(g2).begin();
    nit++;
    nit++;
    nit++;
    nit++;
    nit++;
    EXPECT_EQ(mBackIt(n5_2),nit);
    EXPECT_EQ(n5_2_mIncidence.size(),2);
    n5_2_incidence_it = n5_2_incidence_pair.first;
    EXPECT_EQ(&(*(*(*n5_2_incidence_it))),&e7_2);
    n5_2_incidence_it++;
    EXPECT_EQ(&(*(*(*n5_2_incidence_it))),&e8_2);

    eit = mEdges(g2).begin();
    EXPECT_EQ(mBackIt(e9),eit);
    auto  e9_n1_it =mIncidentNode1(e9);
    EXPECT_EQ(&(*(*e9_n1_it)),&n6);
    auto  e9_n2_it =mIncidentNode2(e9);
    EXPECT_EQ(&(*(*e9_n2_it)),&n7);
    auto e9_n1_backIt = mBackItNode1(e9);
    n6_incidence_it = n6_incidence_pair.first;
    EXPECT_EQ(e9_n1_backIt,n6_incidence_it);
    auto e9_n2_backIt = mBackItNode2(e9);
    n7_incidence_it = n7_incidence_pair.first;
    EXPECT_EQ(e9_n2_backIt,n7_incidence_it);

    eit = mEdges(g2).begin();
    eit++;
    EXPECT_EQ(mBackIt(e1_2),eit);
    auto  e1_2_n1_it =mIncidentNode1(e1_2);
    EXPECT_EQ(&(*(*e1_2_n1_it)),&n1_2);
    auto  e1_2_n2_it =mIncidentNode2(e1_2);
    EXPECT_EQ(&(*(*e1_2_n2_it)),&n3_2);
    auto e1_2_n1_backIt = mBackItNode1(e1_2);
    n1_2_incidence_it = n1_2_incidence_pair.first;
    EXPECT_EQ(e1_2_n1_backIt,n1_2_incidence_it);
    auto e1_2_n2_backIt = mBackItNode2(e1_2);
    n3_2_incidence_it = n3_2_incidence_pair.first;
    EXPECT_EQ(e1_2_n2_backIt,n3_2_incidence_it);

    eit = mEdges(g2).begin();
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e2_2),eit);
    auto  e2_2_n1_it =mIncidentNode1(e2_2);
    EXPECT_EQ(&(*(*e2_2_n1_it)),&n1_2);
    auto  e2_2_n2_it =mIncidentNode2(e2_2);
    EXPECT_EQ(&(*(*e2_2_n2_it)),&n4_2);
    auto e2_2_n1_backIt = mBackItNode1(e2_2);
    n1_2_incidence_it = n1_2_incidence_pair.first;
    n1_2_incidence_it++;
    EXPECT_EQ(e2_2_n1_backIt,n1_2_incidence_it);
    auto e2_2_n2_backIt = mBackItNode2(e2_2);
    n4_2_incidence_it = n4_2_incidence_pair.first;
    EXPECT_EQ(e2_2_n2_backIt,n4_2_incidence_it);

    eit = mEdges(g2).begin();
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e7_2),eit);
    auto  e7_2_n1_it =mIncidentNode1(e7_2);
    EXPECT_EQ(&(*(*e7_2_n1_it)),&n3_2);
    auto  e7_2_n2_it =mIncidentNode2(e7_2);
    EXPECT_EQ(&(*(*e7_2_n2_it)),&n5_2);
    auto e7_2_n1_backIt = mBackItNode1(e7_2);
    n3_2_incidence_it = n3_2_incidence_pair.first;
    n3_2_incidence_it++;
    EXPECT_EQ(e7_2_n1_backIt,n3_2_incidence_it);
    auto e7_2_n2_backIt = mBackItNode2(e7_2);
    n5_2_incidence_it = n5_2_incidence_pair.first;
    EXPECT_EQ(e7_2_n2_backIt,n5_2_incidence_it);

    eit = mEdges(g2).begin();
    eit++;
    eit++;
    eit++;
    eit++;
    EXPECT_EQ(mBackIt(e8_2),eit);
    auto  e8_2_n1_it =mIncidentNode1(e8_2);
    EXPECT_EQ(&(*(*e8_2_n1_it)),&n4_2);
    auto  e8_2_n2_it =mIncidentNode2(e8_2);
    EXPECT_EQ(&(*(*e8_2_n2_it)),&n5_2);
    auto e8_2_n1_backIt = mBackItNode1(e8_2);
    n4_2_incidence_it = n4_2_incidence_pair.first;
    n4_2_incidence_it++;
    EXPECT_EQ(e8_2_n1_backIt,n4_2_incidence_it);
    auto e8_2_n2_backIt = mBackItNode2(e8_2);
    n5_2_incidence_it = n5_2_incidence_pair.first;
    n5_2_incidence_it++;
    EXPECT_EQ(e8_2_n2_backIt,n5_2_incidence_it);


    for (auto nit1=g1.nodes().first; nit1!=g1.nodes().second; nit1++) {
        EXPECT_EQ((*nit1)->utilitySize(), 0);
    }
    for (auto eit1=g1.edges().first; eit1!=g1.edges().second; eit1++) {
        EXPECT_EQ((*eit1)->utilitySize(), 0);
    }
    for (auto nit2=g2.nodes().first; nit2!=g2.nodes().second; nit2++) {
        EXPECT_EQ((*nit2)->utilitySize(), 0);
    }
    for (auto eit2=g2.edges().first; eit2!=g2.edges().second; eit2++) {
        EXPECT_EQ((*eit2)->utilitySize(), 0);
    }

}


// findEdges.
TEST_F(GraphTests, findMultiEdgesInGraph) {

    /** @brief tests findMultiEdges() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         elist12 = g1.findMultiEdges(n1,n2)
     *         elist21 = g1.findMultiEdges(n2,n1)
     *         elist13 = g1.findMultiEdges(n1,n3)
     *         elist31 = g1.findMultiEdges(n3,n1)
     *         elist15 = g1.findMultiEdges(n1,n5)
     *         elist51 = g1.findMultiEdges(n5,n1)
     *         elist23 = g1.findMultiEdges(n2,n3)
     *         elist32 = g1.findMultiEdges(n3,n2)
     *         elist25 = g1.findMultiEdges(n2,n5)
     *         elist52 = g1.findMultiEdges(n5,n2)
     *         elist35 = g1.findMultiEdges(n3,n5)
     *         elist53 = g1.findMultiEdges(n5,n3)
     *
     *     - Results to be checked.
     *         elist12 == [e3]
     *         elist21 == [e3]
     *         elist13 == [e1,e2]
     *         elist31 == [e1,e2]
     *         elist15 == []
     *         elist51 == []
     *         elist23 == [e4]
     *         elist32 == [e4]
     *         elist25 == []
     *         elist52 == []
     *         elist35 == [e7,e8]
     *         elist53 == [e7,e8]
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Node> np4_2(nullptr);
    std::unique_ptr<Edge> ep6_2 = g1.contractEdge(e6,np4_2); 

    std::unique_ptr<Edge> ep9 = generate_edge();
    Edge& e9 = g1.addEdge(std::move(ep9),n3,n5);

    // Target operation
    vector<edge_list_it_t> elist12 = g1.findMultiEdges(n1,n2);
    vector<edge_list_it_t> elist21 = g1.findMultiEdges(n2,n1);
    vector<edge_list_it_t> elist13 = g1.findMultiEdges(n1,n3);
    vector<edge_list_it_t> elist31 = g1.findMultiEdges(n3,n1);
    vector<edge_list_it_t> elist15 = g1.findMultiEdges(n1,n5);
    vector<edge_list_it_t> elist51 = g1.findMultiEdges(n5,n1);
    vector<edge_list_it_t> elist23 = g1.findMultiEdges(n2,n3);
    vector<edge_list_it_t> elist32 = g1.findMultiEdges(n3,n2);
    vector<edge_list_it_t> elist25 = g1.findMultiEdges(n2,n5);
    vector<edge_list_it_t> elist52 = g1.findMultiEdges(n5,n2);
    vector<edge_list_it_t> elist35 = g1.findMultiEdges(n3,n5);
    vector<edge_list_it_t> elist53 = g1.findMultiEdges(n5,n3);

    EXPECT_EQ(elist12.size(),1);
    EXPECT_EQ(elist12[0] ,mBackIt(e3));
    EXPECT_EQ(elist21.size(),1);
    EXPECT_EQ(elist21[0] ,mBackIt(e3));
    EXPECT_EQ(elist13.size(),2);
    EXPECT_EQ(elist13[0] ,mBackIt(e1));
    EXPECT_EQ(elist13[1] ,mBackIt(e2));
    EXPECT_EQ(elist31.size(),2);
    EXPECT_EQ(elist31[0] ,mBackIt(e1));
    EXPECT_EQ(elist31[1] ,mBackIt(e2));
    EXPECT_EQ(elist15.size(),0);
    EXPECT_EQ(elist51.size(),0);
    EXPECT_EQ(elist23.size(),2);
    EXPECT_EQ(elist23[0] ,mBackIt(e4));
    EXPECT_EQ(elist23[1] ,mBackIt(e5));
    EXPECT_EQ(elist32.size(),2);
    EXPECT_EQ(elist32[0] ,mBackIt(e4));
    EXPECT_EQ(elist32[1] ,mBackIt(e5));
    EXPECT_EQ(elist25.size(),0);
    EXPECT_EQ(elist52.size(),0);
    EXPECT_EQ(elist35.size(),3);
    EXPECT_EQ(elist35[0] ,mBackIt(e7));
    EXPECT_EQ(elist35[1] ,mBackIt(e8));
    EXPECT_EQ(elist35[2] ,mBackIt(e9));
    EXPECT_EQ(elist53.size(),3);
    EXPECT_EQ(elist53[0] ,mBackIt(e7));
    EXPECT_EQ(elist53[1] ,mBackIt(e8));
    EXPECT_EQ(elist53[2] ,mBackIt(e9));

}


// numEdges()
TEST_F(GraphTests, numEdgesInGraph) {

    /** @brief tests numEdges() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         g1.numEdges()
     *
     *     - Results to be checked.
     *         g1.numEdges() == 8
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    /*Edge& e1 = */g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    /*Edge& e2 = */g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    /*Edge& e3 = */g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    /*Edge& e4 = */g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    /*Edge& e5 = */g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    /*Edge& e7 = */g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    /*Edge& e8 = */g1.addEdge(std::move(ep8),n4,n5);

    std::unique_ptr<Node> np4_2(nullptr);
    std::unique_ptr<Edge> ep6_2 = g1.contractEdge(e6,np4_2); 

    std::unique_ptr<Edge> ep9 = generate_edge();
    /*Edge& e9 = */g1.addEdge(std::move(ep9),n3,n5);

    // Target operation
    size_t numEdges = g1.numEdges();
    EXPECT_EQ(numEdges, 8);
}


// numNodes() several cases.
TEST_F(GraphTests, numNodesInGraph) {

    /** @brief tests numNodes() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         g1.numNodes()
     *
     *     - Results to be checked.
     *         g1.numNodes() == 8
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    /*Edge& e1 = */g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    /*Edge& e2 = */g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    /*Edge& e3 = */g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    /*Edge& e4 = */g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    /*Edge& e5 = */g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    /*Edge& e6 = */g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    /*Edge& e7 = */g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    /*Edge& e8 = */g1.addEdge(std::move(ep8),n4,n5);

    // Target operation
    size_t numNodes = g1.numNodes();
    EXPECT_EQ(numNodes, 5);
}

// edges() several cases.
TEST_F(GraphTests, edgesInGraph) {

    /** @brief tests edges() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         g1.edges()
     *
     *     - Results to be checked.
     *         g1.edges() == [it1=e1,e2,e3,e4,e5,e6,e7,e8,it2];
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);


    // Target operation
    auto it_pair = g1.edges();
    size_t index = 0;
    for(auto eit = it_pair.first; eit != it_pair.second; eit++) {
        switch (index) {        
          case 0:
             EXPECT_EQ(&(*(*eit)),&e1);
          break;
          case 1:
             EXPECT_EQ(&(*(*eit)),&e2);
          break;
          case 2:
             EXPECT_EQ(&(*(*eit)),&e3);
          break;
          case 3:
             EXPECT_EQ(&(*(*eit)),&e4);
          break;
          case 4:
             EXPECT_EQ(&(*(*eit)),&e5);
          break;
          case 5:
             EXPECT_EQ(&(*(*eit)),&e6);
          break;
          case 6:
             EXPECT_EQ(&(*(*eit)),&e7);
          break;
          case 7:
             EXPECT_EQ(&(*(*eit)),&e8);
          break;
          default:
             EXPECT_EQ(false,true);
        }
        index++;
    }
    EXPECT_EQ(index,8);
}


// nodes() several cases.
TEST_F(GraphTests, nodesInGraph) {

    /** @brief tests nodes() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         g1.edges()
     *
     *     - Results to be checked.
     *         g1.edges() == [it1=e1,e2,e3,e4,e5,e6,e7,e8,it2];
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    /*Edge& e1 = */g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    /*Edge& e2 = */g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    /*Edge& e3 = */g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    /*Edge& e4 = */g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    /*Edge& e5 = */g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    /*Edge& e6 = */g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    /*Edge& e7 = */g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    /*Edge& e8 = */g1.addEdge(std::move(ep8),n4,n5);


    // Target operation
    auto it_pair = g1.nodes();
    size_t index = 0;
    for(auto nit = it_pair.first; nit != it_pair.second; nit++) {
        switch (index) {        
          case 0:
             EXPECT_EQ(&(*(*nit)),&n1);
          break;
          case 1:
             EXPECT_EQ(&(*(*nit)),&n2);
          break;
          case 2:
             EXPECT_EQ(&(*(*nit)),&n3);
          break;
          case 3:
             EXPECT_EQ(&(*(*nit)),&n4);
          break;
          case 4:
             EXPECT_EQ(&(*(*nit)),&n5);
          break;
          default:
             EXPECT_EQ(false,true);
        }
        index++;
    }
    EXPECT_EQ(index,5);
}


// pair<node_incidence_it_t,node_incidence_it_t> Node::incidentEdges();
TEST_F(GraphTests, Node_incidentEdges) {

    /** @brief tests Node::incidentEdges() in a graph.
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n2);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n3);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n4);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n1,n5);


    // Target operation
    auto it_pair = n1.incidentEdges();
    auto eit = it_pair.first;
    EXPECT_EQ(&(*(*(*eit))),&e1);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e2);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e3);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e4);
    eit++;
    EXPECT_EQ(eit,it_pair.second);
}


//    inline void reorderIncidence(vector<edge_list_it_t>&& reorderedEdges);
TEST_F(GraphTests, Node_reorderIncidence_1) {

    /** @brief tests Node::reorderIncidence(vector<edge_list_it_t>&&)
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n2,n1);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n3);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n4);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n1,n5);

    // Target operation
    vector<edge_list_it_t> reorderedEdges;
    reorderedEdges.push_back(e2.backIt());
    reorderedEdges.push_back(e3.backIt());
    reorderedEdges.push_back(e1.backIt());
    reorderedEdges.push_back(e4.backIt());

    n1.reorderIncidence(std::move(reorderedEdges));

    auto it_pair = n1.incidentEdges();
    auto eit = it_pair.first;
    EXPECT_EQ(&(*(*(*eit))),&e2);
    auto bn1 = mBackItNode1(*(*(*eit)));
    EXPECT_EQ(bn1,eit);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e3);
    bn1 = mBackItNode1(*(*(*eit)));
    EXPECT_EQ(bn1,eit);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e1);
    auto bn2 = mBackItNode2(*(*(*eit)));
    EXPECT_EQ(bn2,eit);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e4);
    bn1 = mBackItNode1(*(*(*eit)));
    EXPECT_EQ(bn1,eit);
    eit++;
    EXPECT_EQ(eit,it_pair.second);
}


//    inline void reorderIncidence(list<edge_list_it_t>&& edges);
TEST_F(GraphTests, Node_reorderIncidence_2) {

    /** @brief tests Node::reorderIncidence(list<edge_list_it_t>&&)
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n2);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n3,n1);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n4);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n1,n5);

    // Target operation
    list<edge_list_it_t> reorderedEdges;
    reorderedEdges.push_back(e4.backIt());
    reorderedEdges.push_back(e3.backIt());
    reorderedEdges.push_back(e2.backIt());
    reorderedEdges.push_back(e1.backIt());

    n1.reorderIncidence(std::move(reorderedEdges));

    auto it_pair = n1.incidentEdges();
    auto eit = it_pair.first;
    EXPECT_EQ(&(*(*(*eit))),&e4);
    auto bn1 = mBackItNode1(*(*(*eit)));
    EXPECT_EQ(bn1,eit);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e3);
    bn1 = mBackItNode1(*(*(*eit)));
    EXPECT_EQ(bn1,eit);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e2);
    auto bn2 = mBackItNode2(*(*(*eit)));
    EXPECT_EQ(bn2,eit);
    eit++;
    EXPECT_EQ(&(*(*(*eit))),&e1);
    bn1 = mBackItNode1(*(*(*eit)));
    EXPECT_EQ(bn1,eit);
    eit++;
    EXPECT_EQ(eit,it_pair.second);
}


//    Node::degree()
TEST_F(GraphTests, Node_degree) {

    /** @brief tests Node::degree()
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));
    std::unique_ptr<Node> np6 = generate_node();
    Node& n6 = g1.addNode(std::move(np6));


    std::unique_ptr<Edge> ep1 = generate_edge();
    /*Edge& e1 = */g1.addEdge(std::move(ep1),n1,n2);
    std::unique_ptr<Edge> ep2 = generate_edge();
    /*Edge& e2 = */g1.addEdge(std::move(ep2),n1,n3);
    std::unique_ptr<Edge> ep3 = generate_edge();
    /*Edge& e3 = */g1.addEdge(std::move(ep3),n1,n4);
    std::unique_ptr<Edge> ep4 = generate_edge();
    /*Edge& e4 = */g1.addEdge(std::move(ep4),n1,n5);

    // Target operation
    auto deg1 = n1.degree();
    EXPECT_EQ(deg1,4);
    auto deg6 = n6.degree();
    EXPECT_EQ(deg6,0);
    auto deg2 = n2.degree();
    EXPECT_EQ(deg2,1);

}


//    Node::resetGeneration()
TEST_F(GraphTests, Node_resetGeneration) {

    /** @brief tests Node::resetGeneration()
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));

    setGeneration(n1, 5938);
    // Target operation
    n1.resetGeneration();

    EXPECT_EQ(mGeneration(n1),0);
}


//    inline node_list_it_t backIt() const noexcept;
TEST_F(GraphTests, Node_backIt) {

    /** @brief tests Node::resetGeneration()
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));

    auto node_it_pair = g1.nodes();

    // Target operation
    auto n1_it = n1.backIt();

    EXPECT_EQ(n1_it, node_it_pair.first);
}


//    inline Graph& graph();
TEST_F(GraphTests, Node_graph) {

    /** @brief tests Node::graph()
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));

    // Target operation
    auto& n1_graph = n1.graph();

    EXPECT_EQ(&n1_graph, &g1);
}

// Edge primitive tests
//    inline Node& incidentNode1() const;
//    inline Node& incidentNode2() const;
//    inline Node& adjacentNode(Node& v) const;
//    inline void resetGeneration() noexcept;
//    inline edge_list_it_t backIt() const noexcept;
//    inline Graph& graph()
TEST_F(GraphTests, Edge_primitives_1) {

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n2);

    setGeneration(e1, 1234);
    auto edge_it_pair = g1.edges();


    // Target operation
    Node& e1_n1 = e1.incidentNode1();
    Node& e1_n2 = e1.incidentNode2();
    Node& e1_n1_adj = e1.adjacentNode(n1);
    Node& e1_n2_adj = e1.adjacentNode(n2);

    e1.resetGeneration();
    auto e1_it = e1.backIt();
    auto& e1_graph = e1.graph();

    // Checks
    EXPECT_EQ(&e1_n1,     &n1);
    EXPECT_EQ(&e1_n2,     &n2);
    EXPECT_EQ(&e1_n1_adj, &n2);
    EXPECT_EQ(&e1_n2_adj, &n1);
    EXPECT_EQ(mGeneration(e1),0);
    EXPECT_EQ(e1_it, edge_it_pair.first);
    EXPECT_EQ(&e1_graph, &g1);

}


//  Graph
//    add multi-edges
//    add self-loops
TEST_F(GraphTests, Graph_multiedges_selfloops) {

    /** @brief tests Node::degree()
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));

    // Target operation
    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n2);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n2);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n1,n1);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n1,n1);

    auto nit = g1.nodes().first;
    EXPECT_EQ(&(*(*nit)), &n1);
    nit++;
    EXPECT_EQ(&(*(*nit)), &n2);
    nit++;
    EXPECT_EQ(nit, g1.nodes().second);

    EXPECT_EQ(g1.numNodes(), 2);

    auto eit = g1.edges().first;
    EXPECT_EQ(&(*(*eit)), &e1);
    eit++;
    EXPECT_EQ(&(*(*eit)), &e2);
    eit++;
    EXPECT_EQ(&(*(*eit)), &e3);
    eit++;
    EXPECT_EQ(&(*(*eit)), &e4);
    eit++;
    EXPECT_EQ(&(*(*eit)), &e5);
    eit++;
    EXPECT_EQ(eit, g1.edges().second);

    EXPECT_EQ(g1.numEdges(), 5);

    EXPECT_EQ(&(e1.incidentNode1()),&n1);
    EXPECT_EQ(&(e1.incidentNode2()),&n2);
    EXPECT_EQ(&(e2.incidentNode1()),&n1);
    EXPECT_EQ(&(e2.incidentNode2()),&n2);
    EXPECT_EQ(&(e3.incidentNode1()),&n1);
    EXPECT_EQ(&(e3.incidentNode2()),&n2);
    EXPECT_EQ(&(e4.incidentNode1()),&n1);
    EXPECT_EQ(&(e4.incidentNode2()),&n1);
    EXPECT_EQ(&(e5.incidentNode1()),&n1);
    EXPECT_EQ(&(e5.incidentNode2()),&n1);

    EXPECT_EQ(n1.degree(), 7);
    EXPECT_EQ(n2.degree(), 3);

    auto n1_edge = n1.incidentEdges().first;
    EXPECT_EQ(&(*(*(*n1_edge))),&e1);
    n1_edge++;
    EXPECT_EQ(&(*(*(*n1_edge))),&e2);
    n1_edge++;
    EXPECT_EQ(&(*(*(*n1_edge))),&e3);
    n1_edge++;
    EXPECT_EQ(&(*(*(*n1_edge))),&e4);
    n1_edge++;
    EXPECT_EQ(&(*(*(*n1_edge))),&e4);
    n1_edge++;
    EXPECT_EQ(&(*(*(*n1_edge))),&e5);
    n1_edge++;
    EXPECT_EQ(&(*(*(*n1_edge))),&e5);
    n1_edge++;
    EXPECT_EQ(n1_edge, n1.incidentEdges().second);

    auto n2_edge = n2.incidentEdges().first;
    EXPECT_EQ(&(*(*(*n2_edge))),&e1);
    n2_edge++;
    EXPECT_EQ(&(*(*(*n2_edge))),&e2);
    n2_edge++;
    EXPECT_EQ(&(*(*(*n2_edge))),&e3);
    n2_edge++;
    EXPECT_EQ(n2_edge, n2.incidentEdges().second);

}


// Graph::resetGeneration.
TEST_F(GraphTests, Graph_resetGeneration) {

    /** @brief tests edges() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         g1.resetGeneration()
     *
     *     - Results to be checked.
     *         mGeneration == 0 for all nodes, edges, and the graph.
     */

    // Preparation.    
    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));
    std::unique_ptr<Node> np3 = generate_node();
    Node& n3 = g1.addNode(std::move(np3));
    std::unique_ptr<Node> np4 = generate_node();
    Node& n4 = g1.addNode(std::move(np4));
    std::unique_ptr<Node> np5 = generate_node();
    Node& n5 = g1.addNode(std::move(np5));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n3);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n4);
    std::unique_ptr<Edge> ep3 = generate_edge();
    Edge& e3 = g1.addEdge(std::move(ep3),n1,n2);
    std::unique_ptr<Edge> ep4 = generate_edge();
    Edge& e4 = g1.addEdge(std::move(ep4),n2,n3);
    std::unique_ptr<Edge> ep5 = generate_edge();
    Edge& e5 = g1.addEdge(std::move(ep5),n2,n4);
    std::unique_ptr<Edge> ep6 = generate_edge();
    Edge& e6 = g1.addEdge(std::move(ep6),n3,n4);
    std::unique_ptr<Edge> ep7 = generate_edge();
    Edge& e7 = g1.addEdge(std::move(ep7),n3,n5);
    std::unique_ptr<Edge> ep8 = generate_edge();
    Edge& e8 = g1.addEdge(std::move(ep8),n4,n5);

    setGeneration(n1, 1234);
    setGeneration(n2, 2345);
    setGeneration(n3, 3456);
    setGeneration(n4, 4567);
    setGeneration(n5, 5678);
    setGeneration(e1, 6789);
    setGeneration(e2, 7890);
    setGeneration(e3, 8901);
    setGeneration(e4, 9012);
    setGeneration(e5, 4321);
    setGeneration(e6, 5432);
    setGeneration(e7, 6543);
    setGeneration(e8, 7654);
    setGeneration(g1, 8765);

    g1.resetGeneration();

    EXPECT_EQ(mGeneration(n1),0);
    EXPECT_EQ(mGeneration(n2),0);
    EXPECT_EQ(mGeneration(n3),0);
    EXPECT_EQ(mGeneration(n4),0);
    EXPECT_EQ(mGeneration(n5),0);
    EXPECT_EQ(mGeneration(e1),0);
    EXPECT_EQ(mGeneration(e2),0);
    EXPECT_EQ(mGeneration(e3),0);
    EXPECT_EQ(mGeneration(e4),0);
    EXPECT_EQ(mGeneration(e5),0);
    EXPECT_EQ(mGeneration(e6),0);
    EXPECT_EQ(mGeneration(e7),0);
    EXPECT_EQ(mGeneration(e8),0);
    EXPECT_EQ(mGeneration(g1),0);
}


// Graph::resetGeneration.
TEST_F(GraphTests, Graph_linkAndUtilityStacks) {

    /** @brief tests edges() in a graph.
     *     - Prerequisite:
     *       g1 has 4 nodes n1,...,n5, and 4 edges e1,...,e8.
     *         n1 e1 n3
     *         n1 e2 n4
     *         n1 e3 n2
     *         n2 e4 n3
     *         n2 e5 n4
     *         n3 e6 n4
     *         n3 e7 n5
     *         n4 e8 n5
     *         n1-> e1(n3), e2(n4) ,e3(n2)
     *         n2-> e3(n1), e4(n3), e5(n4)
     *         n3-> e1(n1), e4(n2), e6(n4), e7(n5)
     *         n4-> e2(n1), e5(n2), e6(n3), e8(n5)
     *         n5-> e7(n3), e8(n4)
     *         ep6 = g1.contractEdge(e6,np6)
     *
     *     - Operation
     *         g1.resetGeneration()
     *
     *     - Results to be checked.
     *         mGeneration == 0 for all nodes, edges, and the graph.
     */

    Graph& g1 = Graph1();
    std::unique_ptr<Node> np1 = generate_node();
    Node& n1 = g1.addNode(std::move(np1));
    std::unique_ptr<Node> np2 = generate_node();
    Node& n2 = g1.addNode(std::move(np2));

    std::unique_ptr<Edge> ep1 = generate_edge();
    Edge& e1 = g1.addEdge(std::move(ep1),n1,n2);
    std::unique_ptr<Edge> ep2 = generate_edge();
    Edge& e2 = g1.addEdge(std::move(ep2),n1,n2);


    e1.pushUtility(1234);
    e2.pushUtility(4321);
    EXPECT_EQ(e1.utility(), 1234);
    e1.setUtility(2345);
    EXPECT_EQ(e1.utility(), 2345);
    EXPECT_THROW(e1.utility(1), std::invalid_argument);
    e2.pushUtility(1111);
    e2.pushUtility(2222);
    EXPECT_EQ(e2.utility(),  2222);
    EXPECT_EQ(e2.utility(1), 1111);
    EXPECT_EQ(e2.utility(2), 4321);
    EXPECT_EQ(e1.utilitySize(), 1);
    EXPECT_EQ(e2.utilitySize(), 3);
    e2.popUtility();
    e2.popUtility();
    g1.popUtilityFromEdges();
    EXPECT_THROW(e1.utility(), std::invalid_argument);
    EXPECT_THROW(g1.popUtilityFromEdges(), std::invalid_argument);


    e1.pushIGForwardLink(e2.backIt());
    EXPECT_EQ(e1.IGForwardLink(), e2.backIt());
    e2.pushIGForwardLink(e1.backIt());
    EXPECT_EQ(e2.IGForwardLinksSize(),1);
    EXPECT_EQ(e1.IGForwardLinksSize(),1);
    g1.popIGForwardLinkFromEdges();
    EXPECT_EQ(e2.IGForwardLinksSize(),0);
    EXPECT_EQ(e1.IGForwardLinksSize(),0);
    EXPECT_THROW(e1.IGForwardLink(), std::invalid_argument);
    EXPECT_THROW(g1.popIGForwardLinkFromEdges(), std::invalid_argument);


    e1.pushIGBackwardLink(e2.backIt());
    EXPECT_EQ(e1.IGBackwardLink(), e2.backIt());
    e2.pushIGBackwardLink(e1.backIt());
    EXPECT_EQ(e2.IGBackwardLinksSize(),1);
    EXPECT_EQ(e1.IGBackwardLinksSize(),1);
    g1.popIGBackwardLinkFromEdges();
    EXPECT_EQ(e2.IGBackwardLinksSize(),0);
    EXPECT_EQ(e1.IGBackwardLinksSize(),0);
    EXPECT_THROW(e1.IGBackwardLink(), std::invalid_argument);
    EXPECT_THROW(g1.popIGBackwardLinkFromEdges(), std::invalid_argument);


    n1.pushUtility(1234);
    n2.pushUtility(4321);
    EXPECT_EQ(n1.utility(), 1234);
    n1.setUtility(2345);
    EXPECT_EQ(n1.utility(), 2345);
    EXPECT_THROW(n1.utility(1), std::invalid_argument);
    n2.pushUtility(1111);
    n2.pushUtility(2222);
    EXPECT_EQ(n2.utility(),  2222);
    EXPECT_EQ(n2.utility(1), 1111);
    EXPECT_EQ(n2.utility(2), 4321);
    EXPECT_EQ(n1.utilitySize(), 1);
    EXPECT_EQ(n2.utilitySize(), 3);
    n2.popUtility();
    n2.popUtility();
    g1.popUtilityFromNodes();
    EXPECT_THROW(n1.utility(), std::invalid_argument);
    EXPECT_THROW(g1.popUtilityFromEdges(), std::invalid_argument);

    n1.pushIGForwardLink(n2.backIt());
    EXPECT_EQ(n1.IGForwardLink(), n2.backIt());
    n2.pushIGForwardLink(n1.backIt());
    EXPECT_EQ(n2.IGForwardLinksSize(),1);
    EXPECT_EQ(n1.IGForwardLinksSize(),1);
    g1.popIGForwardLinkFromNodes();
    EXPECT_EQ(n2.IGForwardLinksSize(),0);
    EXPECT_EQ(n1.IGForwardLinksSize(),0);
    EXPECT_THROW(n1.IGForwardLink(), std::invalid_argument);
    EXPECT_THROW(g1.popIGForwardLinkFromNodes(), std::invalid_argument);

    n1.pushIGBackwardLink(n2.backIt());
    EXPECT_EQ(n1.IGBackwardLink(), n2.backIt());
    n2.pushIGBackwardLink(n1.backIt());
    EXPECT_EQ(n2.IGBackwardLinksSize(),1);
    EXPECT_EQ(n1.IGBackwardLinksSize(),1);
    g1.popIGBackwardLinkFromNodes();
    EXPECT_EQ(n2.IGBackwardLinksSize(),0);
    EXPECT_EQ(n1.IGBackwardLinksSize(),0);
    EXPECT_THROW(n1.IGBackwardLink(), std::invalid_argument);
    EXPECT_THROW(g1.popIGBackwardLinkFromNodes(), std::invalid_argument);
}

} // namespace Undirected


} // namespace Wailea
