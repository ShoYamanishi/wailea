#include "undirected/base.hpp"
#include "undirected/planarizable_graph.hpp"
#include "undirected/jts_bl_gmw_planarizer.hpp"

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
 * VIRTUAL NODE START
 * [num]
 *
 * Output file format:
 * NODES
 * [Nod Num]
 *
 * VIRTUAL NODES
 * [Nod Num]
 *
 * EDGES
 * [Node1] [Virtual Node2]... [Virtual NodeX-1] [NodeX]
 *
 */
class CommandLinePlanarizer {

  public:
    inline CommandLinePlanarizer():mVirtualNodeStart(0){;}
    inline virtual ~CommandLinePlanarizer(){;}
    void parseUnplanarizedSpec(const char* filename);
    void generateGraphs();
    void emitPlanarizedSpec(const char* filename);

    class _node {public:long n; node_list_it_t it;};
    class _edge {public:long n1; long n2; edge_list_it_t it;};

    vector<_node> mNodes;
    vector<_edge> mEdges;

  private:

    static const std::string NODES;
    static const std::string EDGES;
    static const std::string VIRTUAL_NODE_START;

    enum parseState {INIT, IN_NODES, IN_EDGES, IN_VIRTUAL_NODE_START, END};

    bool isSectionHeader(std::string line, enum parseState& state);

    bool isCommentLine(std::string line);

    size_t splitLine(
              const std::string& txt, std::vector<std::string>& strs, char ch);
    void handleNode(
      std::string line, const char* filename, long lineNumber,bool& errorFlag);
        
    void handleEdge(
     std::string line, const char* filename, long lineNumber, bool& errorFlag);

    void handleVirtualNodeStart(
     std::string line, const char* filename, long lineNumber, bool& errorFlag);


    void emitError(
     const char* filename, long lineNumber, const char* mess, bool& errorFlag);

    PlanarizableNode& toNodeRef(long num);
    long toNodeNum(node_list_it_t it);

    list<node_list_it_t> getChainedNodes(edge_list_it_t eit);

    long              mVirtualNodeStart;
    PlanarizableGraph mGunplanarized;
    PlanarizableGraph mGplanarized;

};


const std::string CommandLinePlanarizer::NODES = "NODES";
const std::string CommandLinePlanarizer::EDGES = "EDGES";
const std::string CommandLinePlanarizer::VIRTUAL_NODE_START = "VIRTUAL NODE START";

void CommandLinePlanarizer::parseUnplanarizedSpec(const char* filename)
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
          case IN_VIRTUAL_NODE_START:
            handleVirtualNodeStart(line, filename, lineNumber, error);
            break;
        }
    }
}


bool CommandLinePlanarizer::isSectionHeader(
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
    else if (line.compare(0, VIRTUAL_NODE_START.size(), VIRTUAL_NODE_START)==0) {
        state = IN_VIRTUAL_NODE_START;
        return true;
    }

    return false;
}


void CommandLinePlanarizer::emitError(
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


bool CommandLinePlanarizer::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t CommandLinePlanarizer::splitLine(
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


void CommandLinePlanarizer::handleNode(
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


void CommandLinePlanarizer::handleEdge(
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


void CommandLinePlanarizer::handleVirtualNodeStart(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {
    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 1) {
        emitError(filename, lineNumber, "Invalid Virtual Node Start", errorFlag);
    }

    mVirtualNodeStart = std::stol(fields[0]);
}



void CommandLinePlanarizer::generateGraphs()
{
    for (auto& n : mNodes) {
        auto  np  = std::make_unique<PlanarizableNode>();
        auto& nr  = dynamic_cast<PlanarizableNode&>(
                                        mGunplanarized.addNode(std::move(np)));
        n.it = nr.backIt();
    }
    for (auto& e : mEdges) {
        auto  ep  = std::make_unique<PlanarizableEdge>();
        auto& er  = dynamic_cast<PlanarizableEdge&>(
                    mGunplanarized.addEdge(
                               std::move(ep),toNodeRef(e.n1),toNodeRef(e.n2)));
        e.it = er.backIt();
    }
    JTS_BL_GMW_Planarizer planarizer;
    planarizer.planarizeConnectedGraph(mGunplanarized, mGplanarized);
    return;
}


PlanarizableNode& CommandLinePlanarizer::toNodeRef(long num)
{
    for(auto& n : mNodes) {
      if (n.n == num) {
            return dynamic_cast<PlanarizableNode&>(*(*n.it));
	    }
    }
    cerr << "ERROR toNodeRef()\n";
    return dynamic_cast<PlanarizableNode&>(*(*(mNodes[0].it)));
}


long CommandLinePlanarizer::toNodeNum(node_list_it_t it)
{
    for(auto& n : mNodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    cerr << "ERROR toNodeNum()\n";
    return -1;
}


void CommandLinePlanarizer::emitPlanarizedSpec(const char* filename)
{
    std::ofstream   os(filename);

    vector<_node> newNodes;

    for (auto& n : mNodes) {
        mVirtualNodeStart = std::max(mVirtualNodeStart, n.n + 1);
    }
    long vNum = mVirtualNodeStart;

    for (auto nit = mGplanarized.nodes().first; 
                                   nit != mGplanarized.nodes().second; nit++) {
        auto& N = dynamic_cast<PlanarizableNode&>(*(*nit));
        _node n;
        n.it = nit;

        if (N.isVirtual()) {
            n.n  = vNum++;
        }
        else {
            n.n  = toNodeNum(N.IGBackwardLink());
        }
        newNodes.push_back(n);
    }

    class _splitEdge {public:list<long> ns;};
    vector<_splitEdge> newEdges;

    mGplanarized.pushDefaultUtilityToEdges();

    for (auto eit = mGplanarized.edges().first; 
                                   eit != mGplanarized.edges().second; eit++) {

        list<node_list_it_t> nodeList = getChainedNodes(eit);

        if (nodeList.size()>0) {

            _splitEdge e;
            for(auto nit : nodeList) {

                for (auto& n : newNodes) {

                    if (n.it == nit) {

                        e.ns.push_back(n.n);

                    }

                }

            }

            newEdges.push_back(std::move(e));

        }

    }

    mGplanarized.popUtilityFromEdges();

    if (newNodes.size() > 0) {
        os << "NODES\n";
 
    }

    for (auto& n : newNodes) {
        if (n.n == mVirtualNodeStart) {
            os << "VIRTUAL_NODES\n";
        }
        os << n.n << "\n";
    }

    if (newEdges.size() > 0) {
        os << "EDGES\n";
    }

    for (auto& e : newEdges) {
        bool start = true;
        for (auto n : e.ns) {
            if (start) {
                start = false;
            }
            else {
                os << " ";
            }
            os << n;
        }
        os << "\n";
    }

}


list<node_list_it_t> CommandLinePlanarizer::getChainedNodes(
    edge_list_it_t eit
) {

    list<node_list_it_t> nList;
    edge_list_it_t prevIt = mGplanarized.edges().second;

    while (eit != mGplanarized.edges().second) {

        auto& E = dynamic_cast<PlanarizableEdge&>(*(*eit));

        if (E.utility()==1) {
            return nList;
        }

        if (E.isTerminalN1() && E.isTerminalN2()) {

            E.setUtility(1);
            nList.push_back(E.incidentNode1().backIt());
            nList.push_back(E.incidentNode2().backIt());
            return nList;
        }
        else if (E.isTerminalN1() && !E.isTerminalN2()) {

            if (E.chainN2()==prevIt){
                prevIt = mGplanarized.edges().second;
                break;
            }
            else {
                prevIt = eit;
                eit = E.chainN2();
            }
        }
        else if (!E.isTerminalN1() && E.isTerminalN2()) {

            if (E.chainN1()==prevIt){
                prevIt = mGplanarized.edges().second;
                break;
            }
            else {
                prevIt = eit;
                eit = E.chainN1();
            }
        }
        else if (!E.isTerminalN1() && !E.isTerminalN2()) {

            if (prevIt == E.chainN1()) {
                prevIt = eit;
                eit    = E.chainN2();
            }
            else {
                prevIt = eit;
                eit    = E.chainN1();
            }
        }

    }

    while (eit != mGplanarized.edges().second) {

        auto& E = dynamic_cast<PlanarizableEdge&>(*(*eit));

        E.setUtility(1);

        if (E.isTerminalN1() && !E.isTerminalN2()) {

            if (E.chainN2()==prevIt){
                prevIt = mGplanarized.edges().second;
                nList.push_back(E.incidentNode1().backIt());
                break;
            }
            else {
                prevIt = eit;
                eit = E.chainN2();
                nList.push_back(E.incidentNode1().backIt());
                nList.push_back(E.incidentNode2().backIt());
            }
        }
        else if (!E.isTerminalN1() && E.isTerminalN2()) {

            if (E.chainN1()==prevIt){
                prevIt = mGplanarized.edges().second;
                nList.push_back(E.incidentNode2().backIt());
                break;
            }
            else {
                prevIt = eit;
                eit = E.chainN1();
                nList.push_back(E.incidentNode2().backIt());
                nList.push_back(E.incidentNode1().backIt());
            }
        }
        else {

            if (prevIt == E.chainN1()) {
                prevIt = eit;
                eit = E.chainN2();
                nList.push_back(E.incidentNode2().backIt());
            }
            else {
                prevIt = eit;
                eit = E.chainN1();
                nList.push_back(E.incidentNode1().backIt());
            }
        }

    }

    return nList;
}

} // namespace Undirected

} // namespace Wailea

static void print_usage()
{
    std::cerr << "planarizer : planarize the given connected graph with vertual nodes\n";
    std::cerr << "    Usage : planarizer <input_file> <output_file>\n";
    std::cerr << "\n";
    std::cerr << "    Input file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Nod Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Node2]\n";
    std::cerr << "\n";
    std::cerr << "        VIRTUAL NODE START\n";
    std::cerr << "        [num]\n";
    std::cerr << "\n";
    std::cerr << "    Output file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Nod Num]\n";
    std::cerr << "\n";
    std::cerr << "        VIRTUAL NODES\n";
    std::cerr << "        [Nod Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Virtual Node2]... [Virtual NodeX-1] [NodeX]\n";
}


int main(int argc, char *argv[])
{
    if (argc!=3) {
        print_usage();
        return 1;
    }
    Wailea::Undirected::CommandLinePlanarizer p;
    p.parseUnplanarizedSpec(argv[1]);
    p.generateGraphs();
    p.emitPlanarizedSpec(argv[2]);

    return 0;
}

