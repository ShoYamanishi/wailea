#include "undirected/base.hpp"
#include "undirected/connected_decomposer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Wailea {

namespace Undirected { 

/**
 * Input file format:
 * NODES
 * [Nod Num]
 *
 * EDGES 
 * [Node1] [Node2]
 *
 * Output file format:
 * GRAPH 1
 * NODES
 * [Nod Num]
 *
 * EDGES 
 * [Node1] [Node2]
 *
 * GRAPH 2
 * NODES
 * [Nod Num]
 *
 * EDGES 
 * [Node1] [Node2]
 *
 * ...
 *
 * GRAPH X
 * NODES
 * [Nod Num]
 *
 * EDGES 
 * [Node1] [Node2]
 */

class CommandLineConnectedDecomposer {

  public:

    void parseSpec(const char* filename);
    void decompose();
    void emitComponentsSpec(const char* filename);

    class _node {public:long n; node_list_it_t it;};
    class _edge {public:long n1; long n2; edge_list_it_t it;};

    vector<_node> mNodes;
    vector<_edge> mEdges;

  private:

    static const std::string NODES;
    static const std::string EDGES;

    enum parseState {
        INIT,
        IN_NODES,
        IN_EDGES,
        END
    };

    bool isSectionHeader(std::string line, enum parseState& state);

    bool isCommentLine(std::string line);

    size_t splitLine(
              const std::string& txt, std::vector<std::string>& strs, char ch);
    void handleNode(
      std::string line, const char* filename, long lineNumber,bool& errorFlag);
        
    void handleEdge(
     std::string line, const char* filename, long lineNumber, bool& errorFlag);

    void emitError(
     const char* filename, long lineNumber, const char* mess, bool& errorFlag);

    Node& toNodeRef(long num);
    long toNodeNum(node_list_it_t it);

    Graph  mG;
    vector<vector<node_list_it_t> > mNodeSets;
    vector<vector<edge_list_it_t> > mEdgeSets;
};


const std::string CommandLineConnectedDecomposer::NODES = "NODES";
const std::string CommandLineConnectedDecomposer::EDGES = "EDGES";

void CommandLineConnectedDecomposer::parseSpec(const char* filename)
{
    std::ifstream   is(filename);
    long            lineNumber = 0;
    bool            error      = false;
    enum parseState state      = INIT;
    while (!is.eof() && !error) {
        std::string line;
        std::getline(is, line);
        if (line.empty()) {continue;}
        if ((line[line.size()-1] == '\n')||(line[line.size()- 1] == '\r')) {
            line.erase(line.size() - 1);
        }
	lineNumber++;
        if (isCommentLine(line)) {continue;}
        if(isSectionHeader(line, state)){continue;}
        switch(state) {
          case INIT:
          case END:
            emitError(filename, lineNumber, "", error);
            break;
          case IN_NODES:
            handleNode(line, filename, lineNumber, error);
            break;
          case IN_EDGES:
            handleEdge(line, filename, lineNumber, error);
            break;
        }
    }
}


bool CommandLineConnectedDecomposer::isSectionHeader(
                                      std::string line, enum parseState& state)
{
    if (line.compare(0, NODES.size(), NODES)==0) {
        state = IN_NODES;
        return true;
    }
    else if (line.compare(0, EDGES.size(), EDGES)==0) {
        state = IN_EDGES;
        return true;
    }

    return false;
}


void CommandLineConnectedDecomposer::emitError(
    const char* filename,
    long        lineNumber,
    const char* message ,
    bool&       errorFlag
) {

    std::cerr << "Syntax Error: "
              << filename
              << " at line: "
              << lineNumber
              << " "
              << message
              << "\n";

    errorFlag = true;
}


bool CommandLineConnectedDecomposer::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t CommandLineConnectedDecomposer::splitLine(
    const std::string&        txt,
    std::vector<std::string>& strs,
    char                      ch
) {

    auto   pos        = txt.find( ch );
    size_t initialPos = 0;

    strs.clear();

    while( pos != std::string::npos && initialPos < txt.size()) {

        if (pos > initialPos) {
            strs.push_back( txt.substr( initialPos, pos - initialPos) );
        }

        initialPos = pos + 1;

        if (initialPos < txt.size()) {
            pos = txt.find( ch, initialPos );
        }

    }

    if(initialPos < txt.size()) {
        strs.push_back( txt.substr( initialPos, txt.size() - initialPos) );
    }

    return strs.size();
}


void CommandLineConnectedDecomposer::handleNode(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 1) {
        emitError(filename, lineNumber, "Invalid Node", errorFlag);
    }

    _node n;
    n.n = std::stol(fields[0]);
    mNodes.push_back(n);
}


void CommandLineConnectedDecomposer::handleEdge(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 2) {
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }

    _edge e;
    e.n1  = std::stol(fields[0]);
    e.n2  = std::stol(fields[1]);
    mEdges.push_back(e);
}


void CommandLineConnectedDecomposer::decompose()
{
    for (auto& n : mNodes) {
        auto  np  = std::make_unique<Node>();
        auto& nr  = dynamic_cast<Node&>(mG.addNode(std::move(np)));
        n.it = nr.backIt();
    }
    for (auto& e : mEdges) {

        auto  ep  = std::make_unique<Edge>();
        auto& er  = dynamic_cast<Edge&>(mG.addEdge(
                             std::move(ep), toNodeRef(e.n1), toNodeRef(e.n2)));
        e.it = er.backIt();
    }

    ConnectedDecomposer decomposer;
    decomposer.decompose(mG, mNodeSets, mEdgeSets);
    return;
}


Node& CommandLineConnectedDecomposer::toNodeRef(long num)
{
    for(auto& n : mNodes) {
      if (n.n == num) {
            return dynamic_cast<Node&>(*(*n.it));
	    }
    }
    cerr << "ERROR\n";
    return dynamic_cast<Node&>(*(*(mNodes[0].it)));
}


long CommandLineConnectedDecomposer::toNodeNum(node_list_it_t it)
{
    for(auto& n : mNodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    cerr << "ERROR\n";
    return -1;
}


void CommandLineConnectedDecomposer::emitComponentsSpec(const char* filename)
{
   
    std::ofstream   os(filename);

    for (size_t i = 0; i < mNodeSets.size(); i++) {
        os << "GRAPH " << i << "\n";
        auto& nodes = mNodeSets[i];   
        auto& edges = mEdgeSets[i];   

        os << "NODES\n";
        for (auto nit : nodes) {
            os << toNodeNum(nit) << "\n";
        }

        os << "EDGES\n";
        for (auto eit : edges) {
            auto& E = *(*eit);
            auto& N1 = E.incidentNode1();
            auto& N2 = E.incidentNode2();
            os << toNodeNum(N1.backIt()) << " ";
            os << toNodeNum(N2.backIt()) << "\n";
        }
    }
}

} // namespace Undirected

} // namespace Wailea

static void print_usage()
{
    std::cerr << "connected_decomposer : decomposes the given graph"
                 " graph into connected components\n";
    std::cerr << "    Usage : connected_decomposer "
                 " <input_file> <output_file>\n";
    std::cerr << "\n";
    std::cerr << "    Input file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Node2]\n";
    std::cerr << "\n";
    std::cerr << "    Output file format:\n";
    std::cerr << "        GRAPH 1\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Node2]\n";
    std::cerr << "\n";
    std::cerr << "        GRAPH 2\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Node2]\n";
    std::cerr << "\n";
    std::cerr << "        ...\n";
    std::cerr << "\n";
    std::cerr << "        GRAPH X\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Node2]\n";
    std::cerr << "\n";
}


int main(int argc, char *argv[])
{
    if (argc!=3) {
        print_usage();
        return 1;
    }
    Wailea::Undirected::CommandLineConnectedDecomposer p;
    p.parseSpec(argv[1]);
    p.decompose();
    p.emitComponentsSpec(argv[2]);
    return 0;
}

