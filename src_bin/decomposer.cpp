#include "undirected/base.hpp"
#include "undirected/bctree.hpp"

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
 * CUT_VERTICES
 * [Index] [Nod Num] [Block Num1] ... [Block NumX]
 *
 * BLOCK_BEGIN 
 * [Block Num]
 * 
 * BLOCK_CUT_VERTICES
 * [Nod Num] [Cut vertex index]
 *
 * BLOCK_ORDINARY_VERTICES
 * [Nod Num] 
 *
 * BLOCK_EDGES
 * [Node Num1] [Node Num2]
 *
 * BLOCK_END
 */

class CommandLineDecomposer {

  public:

    void parseSpec(const char* filename);
    void generateBCTree();
    void emitBCTreeSpec(const char* filename);

    class _node {public:long n; node_list_it_t it;};
    class _edge {public:long n1; long n2; edge_list_it_t it;};

    vector<_node> mNodes;
    vector<_edge> mEdges;

  private:

    static const std::string NODES;
    static const std::string EDGES;
    static const std::string CUT_VERTICES;
    static const std::string BLOCK_BEGIN;
    static const std::string BLOCK_CUT_VERTICES;
    static const std::string BLOCK_ORDINARY_VERTICES;
    static const std::string BLOCK_EDGES;
    static const std::string BLOCK_END;

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
    BCTree mBCTree;

};


const std::string CommandLineDecomposer::NODES = "NODES";
const std::string CommandLineDecomposer::EDGES = "EDGES";
const std::string CommandLineDecomposer::CUT_VERTICES = "CUT_VERTICES";
const std::string CommandLineDecomposer::BLOCK_BEGIN = "BLOCK_BEGIN";
const std::string CommandLineDecomposer::BLOCK_CUT_VERTICES
                                                        = "BLOCK_CUT_VERTICES";
const std::string CommandLineDecomposer::BLOCK_ORDINARY_VERTICES
                                                   = "BLOCK_ORDINARY_VERTICES";
const std::string CommandLineDecomposer::BLOCK_EDGES = "BLOCK_EDGES";
const std::string CommandLineDecomposer::BLOCK_END = "BLOCK_END";


void CommandLineDecomposer::parseSpec(const char* filename)
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


bool CommandLineDecomposer::isSectionHeader(
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


void CommandLineDecomposer::emitError(
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


bool CommandLineDecomposer::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t CommandLineDecomposer::splitLine(
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


void CommandLineDecomposer::handleNode(
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


void CommandLineDecomposer::handleEdge(
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


void CommandLineDecomposer::generateBCTree()
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

    BiconnectedDecomposer decomposer;
    mBCTree = decomposer.decompose(mG);
    return;
}


Node& CommandLineDecomposer::toNodeRef(long num)
{
    for(auto& n : mNodes) {
      if (n.n == num) {
            return dynamic_cast<Node&>(*(*n.it));
	    }
    }
    cerr << "ERROR\n";
    return dynamic_cast<Node&>(*(*(mNodes[0].it)));
}


long CommandLineDecomposer::toNodeNum(node_list_it_t it)
{
    for(auto& n : mNodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    cerr << "ERROR\n";
    return -1;
}


void CommandLineDecomposer::emitBCTreeSpec(const char* filename)
{
   
    std::ofstream   os(filename);

    utility_t cutVertexIndex = 0;
    utility_t blockIndex = 0;

    for (auto tnit = mBCTree.nodes().first; tnit != mBCTree.nodes().second; 
                                                                      tnit++) {
        auto& TN = dynamic_cast<BCTreeNode&>(*(*tnit));
        if (TN.type() == BCTreeNode::CutVertexType) {
            TN.pushUtility(++cutVertexIndex);
        }
        else {
            TN.pushUtility(++blockIndex);
        }
    }

    os << CUT_VERTICES << "\n";

    os << "#[Index] [Node Num] [Incindent Block Num1]"
          "...[Incindent Block NumX]\n";

    for (auto tnit = mBCTree.nodes().first; tnit != mBCTree.nodes().second; 
                                                                      tnit++) {
        auto& TN = dynamic_cast<BCTreeNode&>(*(*tnit));
        if (TN.type() == BCTreeNode::CutVertexType) {
            os << TN.utility() << " " 
               << toNodeNum(TN.IGBackwardLink());
            for (auto iit = TN.incidentEdges().first; 
                                     iit != TN.incidentEdges().second; iit++) {
                auto& TE  = dynamic_cast<BCTreeEdge&>(*(*(*iit)));
                auto& TNB = TE.blockNode();

                os << " " << TNB.utility();
                
            }
            os << "\n";
        }
    }
    os << "\n";
    bool commentShown = false;
    for (auto tnit = mBCTree.nodes().first; tnit != mBCTree.nodes().second; 
                                                                      tnit++) {
        auto& TN = dynamic_cast<BCTreeNode&>(*(*tnit));
        if (TN.type() == BCTreeNode::BlockType) {

            os << BLOCK_BEGIN << "\n" << TN.utility() << "\n";

            auto& B = TN.block();

            os << BLOCK_CUT_VERTICES << "\n";
            if (!commentShown) {
                os <<"#[Node Num] [Cut vertex index]\n";
            }
            for (auto bnit=B.nodes().first; bnit!=B.nodes().second; bnit++) {
                auto& BN = dynamic_cast<BlockNode&>(*(*(bnit)));

               if (BN.type()==BlockNode::CutVertexType) {
                    auto& TE = BN.treeEdge();
                    auto& TN = TE.cutVertexNode();
                    os << toNodeNum(BN.IGBackwardLink()) << " " 
                       << TN.utility() << "\n";
                }
            }

            os << BLOCK_ORDINARY_VERTICES << "\n";
            if (!commentShown) {
                os <<"#[Node Num]\n";
            }
            for (auto bnit=B.nodes().first; bnit!=B.nodes().second; bnit++) {
                auto& BN = dynamic_cast<BlockNode&>(*(*(bnit)));

                if (BN.type()==BlockNode::OrdinaryNodeType) {
                    os << toNodeNum(BN.IGBackwardLink()) << "\n";
                }
            }

            os << BLOCK_EDGES << "\n";
            if (!commentShown) {
                os << "#[Node Num 1] [Node Num 2]\n";
            }
            for (auto beit=B.edges().first; beit!=B.edges().second; beit++) {

                auto& BE = dynamic_cast<BlockEdge&>(*(*(beit)));
                auto& N1 = dynamic_cast<BlockNode&>(BE.incidentNode1());
                auto& N2 = dynamic_cast<BlockNode&>(BE.incidentNode2());
                os << toNodeNum(N1.IGBackwardLink()) << " ";
                os << toNodeNum(N2.IGBackwardLink()) << "\n";
            }
            os << BLOCK_END << "\n";
            os << "\n";
            commentShown = true;
        }
    }
}

} // namespace Undirected

} // namespace Wailea

static void print_usage()
{
    std::cerr << "decomposer : decomposes the given connected graph into BC-tree\n";
    std::cerr << "    Usage : decomposer <input_file> <output_file>\n";
    std::cerr << "\n";
    std::cerr << "    Input file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Node2]\n";
    std::cerr << "\n";
    std::cerr << "    Output file format:\n";
    std::cerr << "        CUT_VERTICES\n";
    std::cerr << "        [Index] [Nod Num] [Block Num1] ... [Block NumX]\n";
    std::cerr << "\n";
    std::cerr << "        BLOCK_BEGIN\n";
    std::cerr << "        [Block Num]\n";
    std::cerr << "\n";
    std::cerr << "        BLOCK_CUT_VERTICES\n";
    std::cerr << "        [Node Num] [Cut Vertex Index]\n";
    std::cerr << "\n";
    std::cerr << "        BLOCK_ORDINARY_VERTICES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        BLOCK_EDGES\n";
    std::cerr << "        [Node Num1] [Node Num2]\n";
    std::cerr << "\n";
    std::cerr << "        BLOCK_END\n";
    std::cerr << "\n";
}


int main(int argc, char *argv[])
{
    if (argc!=3) {
        print_usage();
        return 1;
    }
    Wailea::Undirected::CommandLineDecomposer p;
    p.parseSpec(argv[1]);
    p.generateBCTree();
    p.emitBCTreeSpec(argv[2]);
    return 0;
}

