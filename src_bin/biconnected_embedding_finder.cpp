#include "undirected/base.hpp"
#include "undirected/bctree.hpp"
#include "undirected/st_numbering.hpp"
#include "undirected/bl_planarity_tester.hpp"
#include "undirected/planar_dual_graph_maker.hpp"
#include "undirected/planarizable_graph.hpp"

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
 * VIRTUAL_NODES
 * [Nod Num]
 *
 * EDGES 
 * [Node1] [Virtual Node1] ... [Virtual NodeX] [Node2]
 *
 * Output file format:
 *
 * INCIDENCES
 * [Node Num] [Incident Node 1] ... [ Incident Node X]
 *
 * FACES
 * [Face Index] [Incident Node 1] ... [ Incident Node X]
 *
 */
class CommandLineBiconnectedEmbeddingFinder {

  public:

    void parseSpec(const char* filename);
    void generateEmbedding();
    void emitEmbeddingSpec(const char* filename);

    class _node {public:long n; node_list_it_t it;};
    class _edge {public:vector<long> ns; vector<edge_list_it_t> its;};

    vector<_node> mNodes;
    vector<_node> mVirtualNodes;
    vector<_edge> mEdges;

  private:

    static const std::string NODES;
    static const std::string VIRTUAL_NODES;
    static const std::string EDGES;
    static const std::string INCIDENCES;
    static const std::string FACES;

    enum parseState {
        INIT,
        IN_NODES,
        IN_VIRTUAL_NODES,
        IN_EDGES,
        END
    };

    bool isSectionHeader(std::string line, enum parseState& state);

    bool isCommentLine(std::string line);

    size_t splitLine(
              const std::string& txt, std::vector<std::string>& strs, char ch);
    void handleNode(
      std::string line, const char* filename, long lineNumber,bool& errorFlag);

    void handleVirtualNode(
      std::string line, const char* filename, long lineNumber,bool& errorFlag);
        
    void handleEdge(
     std::string line, const char* filename, long lineNumber, bool& errorFlag);

    void emitError(
     const char* filename, long lineNumber, const char* mess, bool& errorFlag);

    Node& toNodeRef(long num);
    long toNodeNum(node_list_it_t it);

    PlanarizableGraph mG;
    EmbeddedGraph     mEmb;
    DualGraph         mDual;

};

const std::string CommandLineBiconnectedEmbeddingFinder::NODES = "NODES";
const std::string CommandLineBiconnectedEmbeddingFinder::VIRTUAL_NODES = 
                                                               "VIRTUAL_NODES";
const std::string CommandLineBiconnectedEmbeddingFinder::EDGES = "EDGES";
const std::string CommandLineBiconnectedEmbeddingFinder::
                                                     INCIDENCES = "INCIDENCES";
const std::string CommandLineBiconnectedEmbeddingFinder::FACES = "FACES";


void CommandLineBiconnectedEmbeddingFinder::parseSpec(const char* filename)
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
          case IN_VIRTUAL_NODES:
            handleVirtualNode(line, filename, lineNumber, error);
            break;
          case IN_EDGES:
            handleEdge(line, filename, lineNumber, error);
            break;
        }
    }
}


bool CommandLineBiconnectedEmbeddingFinder::isSectionHeader(
                                      std::string line, enum parseState& state)
{
    if (line.compare(0, NODES.size(), NODES)==0) {
        state = IN_NODES;
        return true;
    }
    else if (line.compare(0, VIRTUAL_NODES.size(), VIRTUAL_NODES)==0) {
        state = IN_VIRTUAL_NODES;
        return true;
    }
    else if (line.compare(0, EDGES.size(), EDGES)==0) {
        state = IN_EDGES;
        return true;
    }

    return false;
}


void CommandLineBiconnectedEmbeddingFinder::emitError(
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


bool CommandLineBiconnectedEmbeddingFinder::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t CommandLineBiconnectedEmbeddingFinder::splitLine(
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


void CommandLineBiconnectedEmbeddingFinder::handleNode(
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


void CommandLineBiconnectedEmbeddingFinder::handleVirtualNode(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 1) {
        emitError(filename, lineNumber, "Invalid Virtual Node", errorFlag);
    }

    _node n;
    n.n = std::stol(fields[0]);
    mVirtualNodes.push_back(n);
}


void CommandLineBiconnectedEmbeddingFinder::handleEdge(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;
    size_t numFields = splitLine(line, fields, ' ');

    if (numFields < 2) {
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }

    _edge e;
    for (long index = 0; index < numFields; index++) {    
        e.ns.push_back(std::stol(fields[index]));
    }
    mEdges.push_back(e);

}


void CommandLineBiconnectedEmbeddingFinder::generateEmbedding()
{

    for (auto& n : mNodes) {
        auto  np  = std::make_unique<PlanarizableNode>();
        auto& nr  = dynamic_cast<PlanarizableNode&>(
                                                    mG.addNode(std::move(np)));
        n.it = nr.backIt();
    }
    for (auto& n : mVirtualNodes) {
        auto  np  = std::make_unique<PlanarizableNode>();
        auto& nr  = dynamic_cast<PlanarizableNode&>(
                                                    mG.addNode(std::move(np)));
        n.it = nr.backIt();
        nr.setVirtual(true);
    }

    for (auto& e : mEdges) {
        edge_list_it_t prevIt;
        for (size_t index = 0; index < e.ns.size() - 1 ; index++) {
            auto  ep    = std::make_unique<PlanarizableEdge>();
            auto& Ecur  = dynamic_cast<PlanarizableEdge&>(mG.addEdge(
                                                std::move(ep),
                                                toNodeRef(e.ns[index]),
                                                toNodeRef(e.ns[index+1])) );
            Ecur. pushIGBackwardLink(mG.edges().second);
            if (index > 0) {
                auto& Eprev = dynamic_cast<PlanarizableEdge&>(*(*prevIt));
                Eprev.setChainN2(Ecur.backIt());
                Ecur. setChainN1(Eprev.backIt());
            }
            prevIt = Ecur.backIt();

            e.its.push_back(Ecur.backIt());
        }
    }

    if (mG.numNodes() > 3) {    
        STNumbering st;

        auto eit = mG.edges().first;
        auto& E = *(*eit);
        auto& N1 = E.incidentNode1();
        auto& N2 = E.incidentNode2();

        vector<node_list_it_t> stNum = st.getBipolarOrientation(mG, N1, N2);
        BLPlanarityTester tester;
        bool planar = tester.findEmbedding(mG, stNum);
        if (!planar) {
            return;
        }
        mG.removeUnnecessaryVirtualNodes();
    }
    

    PlanarDualGraphMaker maker;
    maker.makeDualGraph(mG, mEmb, mDual);
    return;
}


Node& CommandLineBiconnectedEmbeddingFinder::toNodeRef(long num)
{
    for (auto& n : mNodes) {
        if (n.n == num) {
            return dynamic_cast<Node&>(*(*n.it));
        }
    }
    for (auto& n : mVirtualNodes) {
        if (n.n == num) {
            return dynamic_cast<Node&>(*(*n.it));
        }
    }
    cerr << "ERROR\n";
    return dynamic_cast<Node&>(*(*(mNodes[0].it)));
}


long CommandLineBiconnectedEmbeddingFinder::toNodeNum(node_list_it_t it)
{
    for (auto& n : mNodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    for (auto& n : mVirtualNodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    cerr << "ERROR\n";
    return -1;
}


void CommandLineBiconnectedEmbeddingFinder::emitEmbeddingSpec(
    const char* filename
) {

    std::ofstream   os(filename);

    os << INCIDENCES << "\n";
    os << "#[Node Num] [Incident Node 1] ... [ Incident Node X]\n";

    for (auto nit = mG.nodes().first; nit != mG.nodes().second; nit++) {
        auto& N = *(*nit);
        os << toNodeNum(nit);
        for (auto iit = N.incidentEdges().first ; 
                                      iit != N.incidentEdges().second; iit++) {
            auto& E = *(*(*iit));
            auto& A = E.adjacentNode(N);
            os << " " << toNodeNum(A.backIt());
        }
        os << "\n";
    }
    os << "\n";

    os << FACES << "\n";
    os << "#[Face Index] [Incident Node 1] ... [ Incident Node X]\n";

    long faceIndex = 0;
    for (auto fit = mDual.nodes().first; fit != mDual.nodes().second; fit++ ) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        os << ++faceIndex;
        auto bit = F.mCycleHalfEdgesOn1.begin();
        for (auto cit = F.mCycleEdges.begin(); 
                                    cit != F.mCycleEdges.end(); cit++, bit++) {
            node_list_it_t enit;
            auto& EE = dynamic_cast<EmbeddedEdge&>(*(*(*cit)));
            if (*bit) {            
                enit = EE.mHalfEdge1.mSrcNode;
            }
            else {
                enit = EE.mHalfEdge2.mSrcNode;
            }
            auto& EN = dynamic_cast<EmbeddedNode&>(*(*enit));
            os << " " << toNodeNum(EN.IGBackwardLink());
        }
        os << "\n";
    }
}

} // namespace Undirected

} // namespace Wailea


static void print_usage()
{

    std::cerr << "biconnected_embedding_finder : finds a planar embedding of the given biconnected graph\n";
    std::cerr << "    Usage : biconnected_embedding_finder <input_file> <output_file>\n";
    std::cerr << "\n";
    std::cerr << "    Input file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        VIRTUAL_NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node1] [Virtual Node1] ... [Virtual NodeX] [Node2]\n";
    std::cerr << "\n";
    std::cerr << "    Output file format:\n";
    std::cerr << "        INCIDENCES\n";
    std::cerr << "        [Node Num] [Incident Node 1] ... [ Incident Node X]\n";
    std::cerr << "\n";
    std::cerr << "        FACES\n";
    std::cerr << "        [Face Index] [Incident Node 1] ... [ Incident Node X]\n";
    std::cerr << "\n";

}

int main(int argc, char *argv[])
{
    if (argc!=3) {
        print_usage();
        return 1;
    }
    Wailea::Undirected::CommandLineBiconnectedEmbeddingFinder p;
    p.parseSpec(argv[1]);
    p.generateEmbedding();
    p.emitEmbeddingSpec(argv[2]);
    return 0;
}

