#include "undirected/base.hpp"
#include "directed/di_base.hpp"
#include "directed/acyclic_ordering_finder.hpp"
#include "directed/network_simplex.hpp"
#include "directed/gknv_crossings_reducer.hpp"
#include "directed/sugiyama_digraph.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

namespace Wailea {

namespace Directed { 

/**
 *
 * @brief find the horizontal and vertical arrangement of the nodes of the digraph for drawing
 *
 * =================
 * Input file format
 * =================
 *
 * NODES
 * [Node Num]
 *
 * EDGES
 * [Node Src] [Node Dst] [Flipping Cost] [Extending Cost]
 *
 * ==================
 * Output file format
 * ==================
 * 
 * NODES
 * [Node Num]
 * ...
 *
 * VIRTUAL_NODES
 * [Node Num]
 * ... 
 *
 * EDGES
 * [Node Src] [Virtual Node 1] ... [Virtual Node X] [Node Dst]
 * ...
 *
 * RANKS
 * [Node Num for rank 1 pos 1] ... [Node Num for rank 1 pos X1] 
 * [Node Num for rank 2 pos 1] ... [Node Num for rank 1 pos X2] 
 * ...
 *
 * INCIDENCE_LEFT
 * [Node Num] [Incident Node Num1] ... [Incident Node Num X]
 * ...
 *
 * INCIDENCE_RIGHT
 * [Node Num] [Incident Node Num1] ... [Incident Node Num X]
 * ...
 *
 */
namespace DiGraphArranger { 

/** @class DGANode
 *
 *  @brief parsed node information are realized into DGANodes internally.
 *         This is used as the basis for further processing and finally
 *         emission of resultant node arrangement.
 */
class DGANode : public DiNode {
  public:
    DGANode(long num, bool isVirtual):mNum(num), mVirtual(isVirtual){;}
    long mNum;
    bool mVirtual;
};


/** @class DGAEdge
 *
 *  @brief parsed node information are realized into DGAEdge internally.
 *         This is used as the basis for further processing and finally
 *         emission of resultant node arrangement.
 */
class DGAEdge : public DiEdge {
  public:
    DGAEdge(long flippingCost, long extendingCost):
        mFlippingCost(flippingCost),
        mExtendingCost(extendingCost){;}

    long mFlippingCost;
    long mExtendingCost;
};

/** @class Parser
 * 
 *  @brief parsing input graph informatin and construct a basis digraph.
 */
class Parser {

  public:

    /** @brief constructor
     *
     *  @param  G (in/out) graph to which nodes and edges are to be added
     */
    Parser(DiGraph& G):mG(G){;}

    /** @brief
     *
     *  @param  filename (in): name of the file to be opened and parsed.
     *
     */
    void parseSpec(const char* filename);

    static const std::string NODES;
    static const std::string EDGES;

  private:

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

    DiGraph& mG;

    /** @brief used during parsing to find a node from a node number.*/
    map<long,node_list_it_t> mNodeMap;      


};

const std::string Parser::NODES          = "NODES";
const std::string Parser::EDGES          = "EDGES";

void Parser::parseSpec(const char* filename)
{
    std::ifstream   is(filename);
    long            lineNumber = 0;
    bool            error      = false;
    enum parseState state      = INIT;

    while (!is.eof() && !error) {

        std::string line;

        std::getline(is, line);

        if (!line.empty() && line[line.size() - 1] == '\n'){
            line.erase(line.size() - 1);
        }
        if (!line.empty() && line[line.size() - 1] == '\r'){
            line.erase(line.size() - 1);
        }

        lineNumber++;

        if (line.empty()) {
            continue;
        }
        if (isCommentLine(line)) {
            continue;
        }
        if(isSectionHeader(line, state)){
            continue;
        }

        switch(state) {

          case INIT:

            emitError(filename, lineNumber, "", error);
            break;

          case IN_NODES:

            handleNode(line, filename, lineNumber, error);
            break;

          case IN_EDGES:

            handleEdge(line, filename, lineNumber, error);
            break;

          case END:
          default:
            emitError(filename, lineNumber, "", error);
            break;

        }

    }

}


bool Parser::isSectionHeader(
    std::string line,
    enum parseState& state
) {
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


void Parser::emitError(
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


bool Parser::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t Parser::splitLine(
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


void Parser::handleNode(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 1) {
        emitError(filename, lineNumber, "Invalid Node", errorFlag);
    }

    long nodeNum = std::stol(fields[0]);

    auto  np = std::make_unique<DGANode>(nodeNum, false);
    auto& N  = dynamic_cast<DGANode&>(mG.addNode(std::move(np)));
    mNodeMap[nodeNum] = N.backIt();
}


void Parser::handleEdge(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 4) {
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }

    auto N1num = std::stol(fields[0]);
    auto N2num = std::stol(fields[1]);
    auto Fcost = std::stol(fields[2]);
    auto Ecost = std::stol(fields[3]);

    auto& N1 = dynamic_cast<DGANode&>(*(*(mNodeMap[N1num])));
    auto& N2 = dynamic_cast<DGANode&>(*(*(mNodeMap[N2num])));

    auto  ep = std::make_unique<DGAEdge>(Fcost, Ecost);
    mG.addEdge(std::move(ep), N1, N2);
}


class Generator {

  public:
    Generator(DiGraph& G):mG(G),mSG(G){;}

    void generate(const char* filename);
  private:

    void makeAcyclicOrdering();
    void findRanksAndPos();
    void updateBasisGraph();
    void emitNodesSpec(std::ofstream& os);
    void emitEdgesSpec(std::ofstream& os);
    void emitRanksSpec(std::ofstream& os);
    void emitIncidenceInSpec(std::ofstream& os);
    void emitIncidenceOutSpec(std::ofstream& os);

    DiGraph&        mG;
    SugiyamaDiGraph mSG;

    vector<vector<node_list_it_t> > mRanks;
};


void Generator::updateBasisGraph()
{
    mSG.pushDefaultUtilityToNodes();

    long vNum = 0;
    for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
        auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
        if (!Ns.isVirtual()) {
            auto& Norg = dynamic_cast<DGANode&>(Ns.IGBackwardLinkRef());
            Ns.setUtility(Norg.mNum);
            vNum = std::max(vNum, Norg.mNum);
        }
    }

    vNum++;
    for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
        auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
        if (Ns.isVirtual()) {
            Ns.setUtility(vNum++);
        }
    }
}


void Generator::generate(const char* filename)
{
    std::ofstream   os(filename);
    makeAcyclicOrdering();
    findRanksAndPos();
    updateBasisGraph();
    emitNodesSpec(os);
    emitEdgesSpec(os);
    emitRanksSpec(os);
    emitIncidenceInSpec(os);
    emitIncidenceOutSpec(os);
}


void Generator::emitNodesSpec(std::ofstream& os)
{
    if (mSG.numNodes() > 0) {
        os << "NODES\n";
        for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
            auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
            if (!Ns.isVirtual()) {
                os << Ns.utility() << "\n";
            }
        }

        bool first = true;
        for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
            auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
            if (Ns.isVirtual()) {
                if (first) {
                    os << "\n";
                    os << "VIRTUAL_NODES\n";
                    first = false;
                }
                os << Ns.utility() << "\n";
            }
        }
    }
}


void Generator::emitEdgesSpec(std::ofstream& os)
{
    if (mSG.numEdges() > 0) {

        os << "\n";
        os << "EDGES\n";

        for (auto eit = mSG.edges().first; eit != mSG.edges().second; eit++) {
            auto& Es   = dynamic_cast<SugiyamaDiEdge&>(*(*eit));
            if (Es.isFlipped()) {
                if (Es.isTerminalNdst()) {
                    auto& Ndst = dynamic_cast<SugiyamaDiNode&>(
                                                         Es.incidentNodeDst());
                    os << Ndst.utility() << " ";
                    auto eit2 = Es.backIt();
                    while (true) {
                        auto& Es2   = dynamic_cast<SugiyamaDiEdge&>(*(*eit2));
                        auto& Nsrc  = dynamic_cast<SugiyamaDiNode&>(
                                                        Es2.incidentNodeSrc());
                        os << Nsrc.utility();
                        if (Es2.isTerminalNsrc()) {
                            break;
                        }
                        else {
                            os << " ";
                            eit2 = Es2.chainNsrc();
                        }
                    }
                    os << "\n";
                }
            }
            else {
                if (Es.isTerminalNsrc()) {
                    auto& Nsrc = dynamic_cast<SugiyamaDiNode&>(
                                                         Es.incidentNodeSrc());
                    os << Nsrc.utility() << " ";

                    auto eit2 = Es.backIt();
                    while (true) {
                        auto& Es2   = dynamic_cast<SugiyamaDiEdge&>(*(*eit2));
                        auto& Ndst  = dynamic_cast<SugiyamaDiNode&>(
                                                        Es2.incidentNodeDst());
                        os << Ndst.utility();
                        if (Es2.isTerminalNdst()) {
                            break;
                        }
                        else {
                            os << " ";
                            eit2 = Es2.chainNdst();
                        }
                    }
                    os << "\n";
                }
            }
        }
    }
}


void Generator::emitRanksSpec(std::ofstream& os)
{
    if (mRanks.size() > 0) {
        os << "\n";
        os << "RANKS\n";
        for (auto& rank : mRanks) {
            bool first = true;
            for (auto nit : rank) {
                auto& Ns = dynamic_cast<SugiyamaDiNode&>(*(*nit));
                if (first){
                    first = false;
                }
                else {
                    os << " ";
                }
                os << Ns.utility();
            }
            os << "\n";
        }
    }
}


void Generator::emitIncidenceInSpec(std::ofstream& os)
{
    if (mSG.numEdges() > 0) {    
        os << "\n";
        os << "INCIDENCE_LEFT\n";
        for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
            auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
            if (Ns.degreeIn() > 0) {
                os << Ns.utility();
                auto iPair = Ns.incidentEdgesIn();
                for (auto iit = iPair.first; iit != iPair.second; iit++) {
                    auto& E = dynamic_cast<SugiyamaDiEdge&>(*(*(*iit)));
                    auto& A = dynamic_cast<SugiyamaDiNode&>(
                                                           E.adjacentNode(Ns));
                    os << " " << A.utility();
                }
                os << "\n";
            }
        }
    }
}


void Generator::emitIncidenceOutSpec(std::ofstream& os)
{
    if (mSG.numEdges() > 0) {
        os << "\n";
        os << "INCIDENCE_RIGHT\n";
        for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
            auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
            if (Ns.degreeOut() > 0) {
                os << Ns.utility();
                auto iPair = Ns.incidentEdgesOut();
                for (auto iit = iPair.first; iit != iPair.second; iit++) {
                    auto& E = dynamic_cast<SugiyamaDiEdge&>(*(*(*iit)));
                    auto& A = dynamic_cast<SugiyamaDiNode&>(
                                                           E.adjacentNode(Ns));
                    os << " " << A.utility();
                }
                os << "\n";
            }
        }
    }
}


void Generator::makeAcyclicOrdering()
{
    DiGraph AG;
    for (auto nit = mG.nodes().first; nit != mG.nodes().second; nit++) {    
        auto& Norg = dynamic_cast<DGANode&>(*(*nit));
        auto  np = std::make_unique<AOFNode>();
        auto& Naof  = dynamic_cast<AOFNode&>(AG.addNode(std::move(np)));
        Norg.pushIGForwardLink(Naof.backIt());
        Naof.pushIGBackwardLink(Norg.backIt());
    }
    for (auto eit = mG.edges().first; eit != mG.edges().second; eit++) {    
        auto& Eorg    = dynamic_cast<DGAEdge&>(*(*eit));
        auto& NorgSrc = dynamic_cast<DGANode&>(Eorg.incidentNodeSrc());
        auto& NorgDst = dynamic_cast<DGANode&>(Eorg.incidentNodeDst());
        auto& NaofSrc = dynamic_cast<AOFNode&>(NorgSrc.IGForwardLinkRef());
        auto& NaofDst = dynamic_cast<AOFNode&>(NorgDst.IGForwardLinkRef());

        auto  ep = std::make_unique<AOFEdge>();
        auto& Eaof = dynamic_cast<AOFEdge&>(
                                  AG.addEdge(std::move(ep), NaofSrc, NaofDst));
        Eaof.setCost(Eorg.mFlippingCost);
    }
    AcyclicOrderingFinder  finder(AG);
    vector<node_list_it_t> ListAOF = finder.find();
    vector<node_list_it_t> ListOrg;
    for (auto nit : ListAOF) {
        auto& Naof = dynamic_cast<AOFNode&>(*(*nit));
        ListOrg.push_back(Naof.IGBackwardLink());
        Naof.popIGBackwardLink();
    }     
    mSG.generateFromAcyclicOrdering(ListOrg);

//    { 
//        for (auto eit = mSG.edges().first; eit != mSG.edges().second; eit++) {
//            auto& Es = dynamic_cast<SugiyamaDiEdge&>(*(*eit));
//            auto& Nsrc = dynamic_cast<SugiyamaDiNode&>(Es.incidentNodeSrc());
//            auto& Ndst = dynamic_cast<SugiyamaDiNode&>(Es.incidentNodeDst());
//            auto& Nsrc2 = dynamic_cast<DGANode&>(Nsrc.IGBackwardLinkRef());
//            auto& Ndst2 = dynamic_cast<DGANode&>(Ndst.IGBackwardLinkRef());
//            cerr << "(" << Nsrc2.mNum << "," << Ndst2.mNum << ")\n";
//        }
//    }
}


void Generator::findRanksAndPos()
{
    DiGraph NG;
    for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
        auto& Ns = dynamic_cast<SugiyamaDiNode&>(*(*nit));
#ifdef UNIT_TESTS
        auto& Ndga = dynamic_cast<DGANode&>(Ns.IGBackwardLinkRef());
        auto  np = std::make_unique<NetworkSimplexNode>(Ndga.mNum);
#else
        auto  np = std::make_unique<NetworkSimplexNode>();
#endif
        auto& Nn = dynamic_cast<NetworkSimplexNode&>(
                                                    NG.addNode(std::move(np)));
        Nn.pushIGBackwardLink(Ns.backIt());
        Ns.pushIGForwardLink(Nn.backIt());
    }
    for (auto eit = mSG.edges().first; eit != mSG.edges().second; eit++) {
        auto& Es    = dynamic_cast<SugiyamaDiEdge&>(*(*eit));
        auto& Nssrc = dynamic_cast<SugiyamaDiNode&>(Es.incidentNodeSrc());
        auto& Nsdst = dynamic_cast<SugiyamaDiNode&>(Es.incidentNodeDst());
        auto& Nnsrc = dynamic_cast<NetworkSimplexNode&>(
                                                    Nssrc.IGForwardLinkRef());
        auto& Nndst = dynamic_cast<NetworkSimplexNode&>(
                                                    Nsdst.IGForwardLinkRef());
        auto  ep = std::make_unique<NetworkSimplexEdge>();
        auto& En = dynamic_cast<NetworkSimplexEdge&>(
                                      NG.addEdge(std::move(ep), Nndst, Nnsrc));
        En.setC(-1);
    }
    for (auto nit = mSG.nodes().first; nit != mSG.nodes().second; nit++) {
        auto& Ns   = dynamic_cast<SugiyamaDiNode&>(*(*nit));
        auto& Norg = dynamic_cast<DGANode&>(Ns.IGBackwardLinkRef());
        auto& Nn   = dynamic_cast<NetworkSimplexNode&>(Ns.IGForwardLinkRef());

        Ns.popIGForwardLink();

        long totalCostIn = 0;
        auto iPairIn = Norg.incidentEdgesIn();
        for (auto iit = iPairIn.first; iit != iPairIn.second; iit++) {
            auto& Eorg = dynamic_cast<DGAEdge&>(*(*(*iit)));
            auto& Esug = dynamic_cast<SugiyamaDiEdge&>(Eorg.IGForwardLinkRef());
            if (!Esug.isFlipped()) {
                totalCostIn += (Eorg.mExtendingCost);
            }
            else {
                totalCostIn -= (Eorg.mExtendingCost);
            }
        }

        long totalCostOut = 0;
        auto iPairOut = Norg.incidentEdgesOut();
        for (auto iit = iPairOut.first; iit != iPairOut.second; iit++) {
            auto& Eorg = dynamic_cast<DGAEdge&>(*(*(*iit)));
            auto& Esug = dynamic_cast<SugiyamaDiEdge&>(Eorg.IGForwardLinkRef());
            if (!Esug.isFlipped()) {
                totalCostIn -= (Eorg.mExtendingCost);
            }
            else {
                totalCostIn += (Eorg.mExtendingCost);
            }
        }
        Nn.setB(totalCostIn + totalCostOut);

    }
    NetworkSimplex ns(NG);
    ns.solve();
    long minY;
    long maxY;
    for (auto nit = NG.nodes().first; nit != NG.nodes().second; nit++) {
        auto& Nn = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        if (nit == NG.nodes().first) {
            minY = Nn.y();
            maxY = Nn.y();
        }
        else {
            minY = std::min(minY, Nn.y());
            maxY = std::max(maxY, Nn.y());
        }
    }
    vector<vector<node_list_it_t> > ranksIn;
    for (long i = 0 ; i < maxY + 1 - minY; i++) {
        vector<node_list_it_t> rank;
        ranksIn.push_back(std::move(rank));
    }
    for (auto nit = NG.nodes().first; nit != NG.nodes().second; nit++) {

        auto& Nn = dynamic_cast<NetworkSimplexNode&>(*(*nit));
        ranksIn[Nn.y() - minY].push_back(Nn.IGBackwardLink());
    }
    mRanks = mSG.rankNodesAndAddVirtualNodes(ranksIn);

    GKNVcrossingsReducer reducer(mSG);
    /*long numCrossings = */reducer.reduce(mRanks, 20);
}

} // namespace DiGraphArranger 

} // namespace Directed

} // namespace Wailea


static void print_usage()
{
    std::cerr << "digraph_arranger : finds horizontal and vertical arrangement of the nodes of the given digraph for drawing.\n";
    std::cerr << "    Usage : digraph_arranger <input_file> <output_file>\n";
    std::cerr << "\n";
    std::cerr << "    Input file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node Src] [Node Dst] [Flipping Cost] [Extending Cost]\n";
    std::cerr << "\n";
    std::cerr << "    Output file format:\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "        ...\n";  
    std::cerr << "\n";
    std::cerr << "        VIRTUAL_NODES\n";
    std::cerr << "        [Node Num]\n";
    std::cerr << "        ...\n";  
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node Src] [Virtual Node 1] ... [Virtual Node X] [Node Dst]\n";
    std::cerr << "        ...\n";  
    std::cerr << "\n";
    std::cerr << "        RANKS\n";
    std::cerr << "        [Node Num for rank 1 pos 1] ... [Node Num for rank 1 pos X1]\n";
    std::cerr << "        [Node Num for rank 2 pos 1] ... [Node Num for rank 2 pos X2]\n";
    std::cerr << "        ...\n";  
    std::cerr << "        [Node Num for rank Y pos 1] ... [Node Num for rank Y pos XY]\n";
    std::cerr << "\n";
    std::cerr << "        INCIDENCE_LEFT\n";
    std::cerr << "        [Node Num] [Incident Node Num1] ... [Incident Node Num X]\n";
    std::cerr << "        ...\n";  
    std::cerr << "\n";
    std::cerr << "        INCIDENCE_RIGHT\n";
    std::cerr << "        [Node Num] [Incident Node Num1] ... [Incident Node Num X]\n";
    std::cerr << "        ...\n";  
    std::cerr << "\n";

}


int main(int argc, char *argv[])
{
    if (argc!=3) {

        print_usage();
        return 1;

    }

    Wailea::Directed::DiGraph DGA;

    Wailea::Directed::DiGraphArranger::Parser p(DGA);

    p.parseSpec(argv[1]);

    Wailea::Directed::DiGraphArranger::Generator g(DGA);

    g.generate(argv[2]);

    return 0;
}
