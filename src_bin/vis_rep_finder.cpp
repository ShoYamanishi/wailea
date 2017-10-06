#include "undirected/base.hpp"
#include "undirected/bctree.hpp"
#include "undirected/embedded_bctree.hpp"
#include "undirected/vis_rep_finder.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Wailea {

namespace Undirected { 
/**
 *
 * @brief quick and dirly command line interface to visibility representation
 *
 * =================
 * Input file format
 * =================
 *
 * GAPS
 * [W] [H]
 *
 * NODES
 * [Node Num] [Label W] [Label H]
 *
 * EDGES
 * [Node 1] [Node 2] CENTER/CW/CCW [Label1 W] [Label1 H] \
 *                   CENTER/CW/CCW [LabelM W] [LabelM H] \
 *                   CENTER/CW/CCW [Label2 W] [Label2 H] \
 *
 * INCIDENCES
 * [Node Num] [Incident Node 1]... [Incident Node X]
 *
 * BLOCKS
 * [Block Num] [Node Num 1] ... [Node Num X]
 *
 * FACES
 * [Face Num] [Node Num 1] ... [Node Num X]
 *
 * UNIFICATION GROUPS
 * [Group Num] [Cut Vertex Num] [Block Num1] [Face Num1] [Node CCW1] [Node CW1]
 *                              ... 
 *                              [Block NumN] [Face NumN] [Node CCWN] [Node CWN]
 *
 * ROOT
 * [Block Num] [Top Node Num] [Face Num]
 *
 *
 * ==================
 * Output file format
 * ==================
 * AREA
 * [W] [H]
 * 
 * NODES
 * [Node Num] [X Left] [X Right] [Y]
 *
 * EDGES
 * [Node 1] [Node 2] [X] [Y1] [Y2] FLIPPED/NOTFLIPPED
 *
 */
class CommandLineVisRepFinder {

  public:

    void parseSpec(const char* filename);
    void generateVisRep();
    void emitCoordSpec(const char* filename);

  private:

    using pType = enum Wailea::Undirected::VRRectLabelEdge::posType;

    static const std::string GAPS;
    static const std::string NODES;
    static const std::string EDGES;
    static const std::string INCIDENCES;
    static const std::string BLOCKS;
    static const std::string FACES;
    static const std::string UNIFICATION_GROUPS;
    static const std::string ROOT;
    static const std::string CENTER;
    static const std::string CCW;
    static const std::string CW;

    enum parseState {
        INIT,
        IN_GAPS,
        IN_NODES,
        IN_EDGES,
        IN_INCIDENCES,
        IN_BLOCKS,
        IN_FACES,
        IN_UNIFICATION_GROUPS,
        IN_ROOT,
        END
    };

    class _node {
      public:
        long n;
        long w;
        long h;
        node_list_it_t it;
    };

    class _edge {
      public:
        long n1;
        long n2;
        pType p1; long w1; long h1;
        pType pm; long wm; long hm;
        pType p2; long w2; long h2;
        edge_list_it_t it;
    };

    class _incidence {
      public:
        long n;
        vector<long> incNodes;
    };

    class _block {
      public:
        long b;
        vector<long> nodes;
        node_list_it_t it;
    };

    class _face {
      public:
        long f;
        vector<long> nodes;
        node_list_it_t it;
    };

    class _unificationFace {
      public:
        long b;
        long f;
        long nccw;
        long ncw;
    };

    class _unificationGroup {
      public:
        long g;
        long c;
        vector<_unificationFace> ufs;
    };


    long                      mVgap;
    long                      mHgap;
    vector<_node>             mNodes;
    vector<_edge>             mEdges;
    vector<_incidence>        mIncidences;
    vector<_block>            mBlocks;
    vector<_face>             mFaces;
    vector<_unificationGroup> mUnificationGroups;
    long                      mRootBlock;
    long                      mRootTopNode;
    long                      mRootOuterFace;

    VRRectLabelGraph          mLG;

    bool isSectionHeader(std::string line, enum parseState& state);

    bool isCommentLine(std::string line);

    size_t splitLine(
        const std::string&        txt,
        std::vector<std::string>& strs,
        char                      ch
    );


    void handleGaps(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleNode(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleEdge(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleIncidence(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleBlock(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleFace(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleUnificationGroup(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    void handleRoot(
        std::string line,
        const char* filename,
        long        lineNumber,
        bool&       errorFlag
    );

    bool strToPosType(std::string& str, pType &type);


    void emitError(
        const char* filename,
        long        lineNumber,
        const char* mess ,
        bool&       errorFlag
    );

    VRRectLabelNode& toVRRectLabelNodeRef(long num);

    vector<vector<long>> findEmbeddingIncidencesBlock(Block& b);

    bool isIn(long n, vector<long>& vec);

    void rearrangeIncidences(Block& b, vector<vector<long>>& incs);

    BlockEdge& toBlockEdge(Block& b, BlockNode& N1, BlockNode& N2);

    BlockNode& toBlockNodeRef(Block& b, long num);

    void findFace(EmbeddedFace& F);

    bool cyclicCompare(vector<long>& vec1, vector<long>& vec2);

    EmbeddedBCTreeNode& toEBCTreeNodeRefBlock(EmbeddedBCTree& tree, long num);

    BCTreeNode& toBCTreeNodeRefBlock(long num);

    EmbeddedFace& toFaceRef(long num);

    EmbeddedEdge& toEmbeddedEdgeRef(EmbeddedGraph& EG, long num1, long num2);

    long toVRRectLabelNodeNum(node_list_it_t it);

    edge_list_it_t toVRRectLabelEdgeIt(long num1, long num2);

    EmbeddedBCTreeNode& toEBCTreeNodeRefCV(EmbeddedBCTree& tree, long num);

    EmbeddedNode& toEmbeddedNodeRef(EmbeddedGraph& EG, long num);

    vector<long> findIncidentNodes(EmbeddedFace& F);

    bool orderAgnosticCompare(vector<long>& vec1, vector<long>& vec2);

    void findBlock(Block& b);

    long toVRRectLabelNodeNum(EmbeddedNode& EN);

};

const std::string CommandLineVisRepFinder::GAPS        = "GAPS";
const std::string CommandLineVisRepFinder::NODES       = "NODES";
const std::string CommandLineVisRepFinder::EDGES       = "EDGES";
const std::string CommandLineVisRepFinder::INCIDENCES  = "INCIDENCES";
const std::string CommandLineVisRepFinder::BLOCKS      = "BLOCKS";
const std::string CommandLineVisRepFinder::FACES       = "FACES";
const std::string CommandLineVisRepFinder::UNIFICATION_GROUPS 
                                                       = "UNIFICATION GROUPS";
const std::string CommandLineVisRepFinder::ROOT        = "ROOT";
const std::string CommandLineVisRepFinder::CENTER      = "CENTER";
const std::string CommandLineVisRepFinder::CCW         = "CCW";
const std::string CommandLineVisRepFinder::CW          = "CW";


void CommandLineVisRepFinder::parseSpec(const char* filename)
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

          case IN_GAPS:

            handleGaps(line, filename, lineNumber, error);
            break;


          case IN_NODES:

            handleNode(line, filename, lineNumber, error);
            break;

          case IN_EDGES:

            handleEdge(line, filename, lineNumber, error);
            break;

          case IN_INCIDENCES:

            handleIncidence(line, filename, lineNumber, error);
            break;

          case IN_BLOCKS:

            handleBlock(line, filename, lineNumber, error);
            break;

          case IN_FACES:

            handleFace(line, filename, lineNumber, error);
            break;

          case IN_UNIFICATION_GROUPS:

            handleUnificationGroup(line, filename, lineNumber, error);
            break;

          case IN_ROOT:

            handleRoot(line, filename, lineNumber, error);
            break;

          case END:

            emitError(filename, lineNumber, "", error);
            break;
        }

    }
}


bool CommandLineVisRepFinder::isSectionHeader(std::string line, enum parseState& state)
{
    if (line.compare(0, GAPS.size(), GAPS)==0) {
        state = IN_GAPS;
        return true;
    }
    else if (line.compare(0, NODES.size(), NODES)==0) {
        state = IN_NODES;
        return true;
    }
    else if (line.compare(0, EDGES.size(), EDGES)==0) {
        state = IN_EDGES;
        return true;
    }
    else if (line.compare(0, INCIDENCES.size(), INCIDENCES)==0) {
        state = IN_INCIDENCES;
        return true;
    }
    else if (line.compare(0, BLOCKS.size(), BLOCKS)==0) {
        state = IN_BLOCKS;
        return true;
    }
    else if (line.compare(0, FACES.size(), FACES)==0) {
        state = IN_FACES;
        return true;
    }
    else if (line.compare(0,UNIFICATION_GROUPS.size(), UNIFICATION_GROUPS)==0){
        state = IN_UNIFICATION_GROUPS;
        return true;
    }
    else if (line.compare(0, ROOT.size(), ROOT)==0) {
        state = IN_ROOT;
        return true;
    }

    return false;
}


void CommandLineVisRepFinder::emitError(
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


bool CommandLineVisRepFinder::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t CommandLineVisRepFinder::splitLine(
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


void CommandLineVisRepFinder::handleGaps(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 2) {
        emitError(filename, lineNumber, "Invalid Gaps", errorFlag);
    }

    mHgap = std::stol(fields[0]);
    mVgap = std::stol(fields[1]);
}


void CommandLineVisRepFinder::handleNode(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 3) {
        emitError(filename, lineNumber, "Invalid Node", errorFlag);
    }

    _node n;
    n.n = std::stol(fields[0]);
    n.w = std::stol(fields[1]);
    n.h = std::stol(fields[2]);
    mNodes.push_back(n);
}


bool CommandLineVisRepFinder::strToPosType(
    std::string& str,
    enum Wailea::Undirected::VRRectLabelEdge::posType &type
) {
    if (str.compare(0, CENTER.size(), CENTER)==0) {
        type = VRRectLabelEdge::POS_CENTER;
        return true;
    }
    else if (str.compare(0, CCW.size(), CCW)==0) {
        type = VRRectLabelEdge::POS_COUNTER_CLOCKWISE;
        return true;
    }
    else if (str.compare(0, CW.size(), CW)==0) {
        type = VRRectLabelEdge::POS_CLOCKWISE;
        return true;
    }
    else{
        return false;
    }
}


void CommandLineVisRepFinder::handleEdge(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 11) {
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }

    _edge e;
    enum Wailea::Undirected::VRRectLabelEdge::posType t;

    e.n1  = std::stol(fields[0]);
    e.n2  = std::stol(fields[1]);

    if (strToPosType(fields[2], t)){
        e.p1  = t;
    }
    else{
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }
    e.w1  = std::stol(fields[3]);
    e.h1  = std::stol(fields[4]);

    if (strToPosType(fields[5], t)){
        e.pm  = t;
    }
    else{
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }
    e.wm  = std::stol(fields[6]);
    e.hm  = std::stol(fields[7]);

    if (strToPosType(fields[8], t)){
        e.p2  = t;
    }
    else{
        emitError(filename, lineNumber, "Invalid Edge", errorFlag);
    }
    e.w2  = std::stol(fields[9]);
    e.h2  = std::stol(fields[10]);

    mEdges.push_back(e);
}


void CommandLineVisRepFinder::handleIncidence(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ') < 2) {
        emitError(filename, lineNumber, "Invalid Incidence", errorFlag);
    }

    _incidence inc;

    inc.n  = std::stol(fields[0]);
    for (size_t i = 1; i < fields.size();i++) {
        inc.incNodes.push_back(std::stol(fields[i]));
    }

    mIncidences.push_back(inc);
}


void CommandLineVisRepFinder::handleBlock(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {
    vector<std::string> fields;
    if (splitLine(line, fields, ' ') < 3) {
        emitError(filename, lineNumber, "Invalid block", errorFlag);
    }

    _block blk;
    blk.b  = std::stol(fields[0]);
    for (size_t i = 1; i < fields.size();i++) {
        blk.nodes.push_back(std::stol(fields[i]));
    }
    mBlocks.push_back(blk);
}


void CommandLineVisRepFinder::handleFace(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;
    if (splitLine(line, fields, ' ') < 3) {
        emitError(filename, lineNumber, "Invalid Face", errorFlag);
    }

    _face fc;
    fc.f  = std::stol(fields[0]);
    for (size_t i = 1; i < fields.size();i++) {
        fc.nodes.push_back(std::stol(fields[i]));
    }
    mFaces.push_back(fc);
}


void CommandLineVisRepFinder::handleUnificationGroup(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;
    if (splitLine(line, fields, ' ') < 6) {

        emitError(filename, lineNumber, "Invalid UG", errorFlag);
    }
    if ((fields.size() - 2) % 4 != 0) {

        emitError(filename, lineNumber, "Invalid UG", errorFlag);
    }

    _unificationGroup ug;
    ug.g  = std::stol(fields[0]);
    ug.c  = std::stol(fields[1]);

    for (size_t i = 2; i < fields.size();i+=4) {

        _unificationFace uf;
        uf.b = std::stol(fields[i]);
        uf.f = std::stol(fields[i+1]);
        uf.nccw = std::stol(fields[i+2]);
        uf.ncw = std::stol(fields[i+3]);
        ug.ufs.push_back(uf);
    }

    mUnificationGroups.push_back(ug);

}


void CommandLineVisRepFinder::handleRoot(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ') != 3) {
        emitError(filename, lineNumber, "Invalid root", errorFlag);
    }

    mRootBlock     = std::stol(fields[0]);
    mRootTopNode   = std::stol(fields[1]);
    mRootOuterFace = std::stol(fields[2]);
}


VRRectLabelNode& CommandLineVisRepFinder::toVRRectLabelNodeRef(long num)
{
    for(auto& n : mNodes) {
        if (n.n == num) {
            return dynamic_cast<VRRectLabelNode&>(*(*n.it));
        }
    }
    cerr << "ERROR toVRRectLabelNodeRef()\n";
    return dynamic_cast<VRRectLabelNode&>(*(*(mNodes[0].it)));
}


bool CommandLineVisRepFinder::isIn(long n, vector<long>& vec) {
    for (auto val : vec) {
        if (n==val) {
            return true;
        }
    }
    return false;
}


vector<vector<long>> CommandLineVisRepFinder::
                                        findEmbeddingIncidencesBlock(Block& b)
{
    vector<long> nodeNums;
    vector<vector<long>> incs;

    for (auto bnit = b.nodes().first; bnit != b.nodes().second; bnit++) {

        auto& BN  = dynamic_cast<BlockNode&>(*(*bnit));
        auto& LN  = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());

        nodeNums.push_back(toVRRectLabelNodeNum(LN.backIt()));
    }

    for (auto& inc : mIncidences) {
        if (isIn(inc.n, nodeNums)) {
            vector<long> vec;
            vec.push_back(inc.n);
            for (auto i : inc.incNodes) {
                if (isIn(i, nodeNums)) {
                    vec.push_back(i);
                }
            }
            incs.push_back(std::move(vec));
        }
    }
    return incs;

}


void CommandLineVisRepFinder::rearrangeIncidences(
    Block&                b,
    vector<vector<long>>& incs
) {
    for (auto bnit = b.nodes().first; bnit != b.nodes().second; bnit++) {
        auto& BN  = dynamic_cast<BlockNode&>(*(*bnit));
        auto& LN  = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        auto nodeNum = toVRRectLabelNodeNum(LN.backIt());
        for (auto& vec :  incs) {
            if (nodeNum==vec[0]) {
                vector<edge_list_it_t> incIts;

                for (size_t i = 1; i < vec.size(); i++) {
                    auto& IN = toBlockNodeRef(b, vec[i]);
                    auto& E  = toBlockEdge(b, BN, IN);
                    incIts.push_back(E.backIt());
                }
                BN.reorderIncidence(std::move(incIts));
                break;
            }
        }
    }
}


BlockEdge& CommandLineVisRepFinder::toBlockEdge(
    Block&     b,
    BlockNode& N1,
    BlockNode& N2
) {
    for (auto beit = b.edges().first; beit != b.edges().second; beit++) {
        auto& BE = dynamic_cast<BlockEdge&>(*(*beit));
        if ( (&(BE.incidentNode1())==&N1 && &(BE.incidentNode2())==&N2)||
             (&(BE.incidentNode1())==&N2 && &(BE.incidentNode2())==&N1)  ) {
            return BE;
        }
    }
    cerr << "ERROR toBlockEdge()\n";
    return dynamic_cast<BlockEdge&>(*(*(b.edges().first)));
}


BlockNode& CommandLineVisRepFinder::toBlockNodeRef(Block& b, long num)
{
    for (auto nit = b.nodes().first; nit != b.nodes().second; nit++) {
        auto& B = dynamic_cast<BlockNode&>(*(*nit));
        auto& N = dynamic_cast<VRRectLabelNode&>(B.IGBackwardLinkRef());
        auto nodeNum = toVRRectLabelNodeNum(N.backIt());
        if ( nodeNum == num) {
            return B;
        }
    }
    cerr << "ERROR toBlockNodeRef()\n";
    return dynamic_cast<BlockNode&>(*(*(b.nodes().first)));
}


long CommandLineVisRepFinder::toVRRectLabelNodeNum(EmbeddedNode& EN)
{
    auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
    auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
    return toVRRectLabelNodeNum(LN.backIt());
}


vector<long> CommandLineVisRepFinder::findIncidentNodes(EmbeddedFace& F)
{
    vector<long> surroundingNodes;
    auto cbit = F.mCycleHalfEdgesOn1.begin();
    for (auto ceit  = F.mCycleEdges.begin();
              ceit != F.mCycleEdges.end();   ceit++) {
        auto& EE = dynamic_cast<EmbeddedEdge&>(*(*(*ceit)));
        if (*cbit) {
            auto& N =dynamic_cast<EmbeddedNode&>(*(*(EE.mHalfEdge1.mSrcNode)));
	        surroundingNodes.push_back(toVRRectLabelNodeNum(N));
        }
        else {
            auto& N =dynamic_cast<EmbeddedNode&>(*(*(EE.mHalfEdge2.mSrcNode)));
            surroundingNodes.push_back(toVRRectLabelNodeNum(N));
	    }
        cbit++;
    }
    return surroundingNodes;
}


void CommandLineVisRepFinder::findFace(EmbeddedFace& F)
{
    vector<long> nodeNums = findIncidentNodes(F);

    for (auto& fspec : mFaces) {
        if (cyclicCompare(fspec.nodes, nodeNums)) {
            fspec.it = F.backIt();
        }
    }
}


bool CommandLineVisRepFinder::cyclicCompare(
    vector<long>& vec1,
    vector<long>& vec2
) {
    if (vec1.size()!=vec2.size()) {
      return false;
    }
    for(size_t start1 = 0; start1 < vec1.size();start1++) {
        bool match = true;
        for (size_t rep = 0; rep < vec1.size(); rep++) {
            long pos1 = (start1+rep)%(vec1.size());
            if (vec1[pos1]!=vec2[rep]) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }
    return false;
}


EmbeddedBCTreeNode& CommandLineVisRepFinder::toEBCTreeNodeRefBlock(
    EmbeddedBCTree& tree,
    long            num
) {
    auto& TN = toBCTreeNodeRefBlock(num);
    for (auto nit = tree.nodes().first; nit != tree.nodes().second; nit++) {
        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (ETN.type()==BCTreeNode::BlockType) {
	      if (TN.backIt() == ETN.IGBackwardLink()) {
                return ETN;
            }
        }
    }
    cerr << "ERROR toEBCTreeNodeRefBlock()\n";
    return dynamic_cast<EmbeddedBCTreeNode&>(*(*(tree.nodes().first)));
}


BCTreeNode& CommandLineVisRepFinder::toBCTreeNodeRefBlock(long num)
{
   for (auto& bSpec : mBlocks) {
       if (bSpec.b == num) {
          return dynamic_cast<BCTreeNode&>(*(*(bSpec.it)));
        }
    }
    cerr << "ERROR toBCTreeNodeRefBlock()\n";
    return dynamic_cast<BCTreeNode&>(*(*(mBlocks[0].it)));
}


EmbeddedFace& CommandLineVisRepFinder::toFaceRef(long num)
{
    for (auto& f : mFaces) {
        if (f.f == num) {
            return dynamic_cast<EmbeddedFace&>(*(*(f.it)));
        }
    }
    cerr << "ERROR toFaceRef()\n";
    return dynamic_cast<EmbeddedFace&>(*(*(mFaces[0].it)));
}


EmbeddedEdge& CommandLineVisRepFinder::toEmbeddedEdgeRef(
    EmbeddedGraph& EG,
    long           num1,
    long           num2
) {
    edge_list_it_t lit = toVRRectLabelEdgeIt(num1, num2);

    for (auto eit = EG.edges().first; eit != EG.edges().second; eit++) {
        auto& EE = dynamic_cast<EmbeddedEdge&>(*(*eit));
        auto& BE = dynamic_cast<BlockEdge&>(EE.IGBackwardLinkRef());
        auto& LE = dynamic_cast<VRRectLabelEdge&>(BE.IGBackwardLinkRef());
        if (lit == LE.backIt()) {
            return EE;
        }
    }
    cerr << "ERROR toEmbeddedEdgeRef()\n";
    return dynamic_cast<EmbeddedEdge&>(*(*(EG.edges().first)));
}


long CommandLineVisRepFinder::toVRRectLabelNodeNum(node_list_it_t it)
{
    for(auto& n : mNodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    cerr << "ERROR toVRRectLabelNodeNum()\n";
    return mNodes[0].n;
}


edge_list_it_t CommandLineVisRepFinder::toVRRectLabelEdgeIt(
                                                          long num1, long num2)
{
//    cerr << "CKP1: " << mEdges.size() << "\n";
//    cerr << "CKP2: " << num1 << "," << num2 << "\n";
    for(auto& e : mEdges) {
        if ( (e.n1 == num1 && e.n2 == num2) ||
             (e.n1 == num2 && e.n2 == num1)    ) {
            return e.it;
        }
    }
    cerr << "ERROR toVRRectLabelEdgeIt()\n";
    return mEdges[0].it;
}


EmbeddedBCTreeNode& CommandLineVisRepFinder::toEBCTreeNodeRefCV(
    EmbeddedBCTree& tree,
    long            num
) {
    auto& LN = toVRRectLabelNodeRef(num);
    for (auto nit = tree.nodes().first; nit != tree.nodes().second; nit++) {
        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (ETN.type()==BCTreeNode::CutVertexType) {
            auto& TN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
            auto& LN2 = dynamic_cast<VRRectLabelNode&>(TN.IGBackwardLinkRef());
            if (LN.backIt() == LN2.backIt()) {
                return ETN;
            }
        }
    }
    cerr << "ERROR toEBCTreeNodeRefCV()\n";
    return dynamic_cast<EmbeddedBCTreeNode&>(*(*(tree.nodes().first)));
}


EmbeddedNode& CommandLineVisRepFinder::toEmbeddedNodeRef(
    EmbeddedGraph& EG,
    long           num
) {
    auto& LN = toVRRectLabelNodeRef(num);
    for (auto nit = EG.nodes().first; nit != EG.nodes().second; nit++) {
        auto& EN  = dynamic_cast<EmbeddedNode&>(*(*nit));
        auto& BN  = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        if (LN.backIt() == LN2.backIt()) {
            return EN;
        }
    }
    cerr << "ERROR toEmbeddedNodeRef()\n";
    return dynamic_cast<EmbeddedNode&>(*(*(EG.nodes().first)));
}


bool CommandLineVisRepFinder::orderAgnosticCompare(
    vector<long>& vec1,
    vector<long>& vec2
) {
    vector<long> found;
    for (size_t i = 0; i < vec1.size(); i++) {
        found.push_back(0);
    }
    size_t i = 0;
    for (auto v1 : vec1) {
        for (auto v2 : vec2) {
            if (v1 == v2){
                found[i] = found[i] + 1;
            }
        }
        i++;
    }
    for (auto f : found) {
        if (f != 1){
            return false;
        }
    }
    return true;
}


void CommandLineVisRepFinder::findBlock(Block& b)
{
    vector<long> blockNums;
    for (auto nit = b.nodes().first; nit != b.nodes().second; nit++) {
      auto& B = dynamic_cast<BlockNode&>(*(*nit));
        auto& N = dynamic_cast<VRRectLabelNode&>(B.IGBackwardLinkRef());
        blockNums.push_back(toVRRectLabelNodeNum(N.backIt()));
    }
    for (auto& blk : mBlocks) {
        if (orderAgnosticCompare(blk.nodes, blockNums)) {
            blk.it = b.BCTreeNode().backIt();
        }
    }
}


void CommandLineVisRepFinder::generateVisRep()
{
    mLG.setHorizontalGap(mHgap);
    mLG.setVerticalGap(mVgap);

    for (auto& n : mNodes) {
        auto  np  = std::make_unique<VRRectLabelNode>(n.w, n.h);
        auto& nr  = dynamic_cast<VRRectLabelNode&>(mLG.addNode(std::move(np)));
        n.it = nr.backIt();
    }

    for (auto& e : mEdges) {
        auto  ep  = std::make_unique<VRRectLabelEdge>();
        auto& er  = dynamic_cast<VRRectLabelEdge&>(
                                mLG.addEdge( std::move(ep),
                                toVRRectLabelNodeRef(e.n1),
                                toVRRectLabelNodeRef(e.n2) ));
        er.setLabelNode1Side(e.p1, e.w1, e.h1);
        er.setLabelMiddle   (e.pm, e.wm, e.hm);
        er.setLabelNode2Side(e.p2, e.w2, e.h2);
        e.it = er.backIt();
    }

    for (auto& i : mIncidences) {
        vector<edge_list_it_t> incIts;
        for (auto a :  i.incNodes) {
            incIts.push_back(toVRRectLabelEdgeIt(i.n, a));
        }
        toVRRectLabelNodeRef(i.n).reorderIncidence(std::move(incIts));
    }

    BiconnectedDecomposer decomp;
    BCTree bcTree = decomp.decompose(mLG, *(*(mLG.nodes().first)));

    for (auto bnit  = bcTree.nodes().first; 
              bnit != bcTree.nodes().second; bnit++) {

        auto& bn = dynamic_cast<BCTreeNode&>(*(*bnit));
        if (bn.type()==BCTreeNode::BlockType) {
            // Rearrange incidence for each block in bc tree.
            auto& b = bn.block();
            findBlock(b);
            vector<vector<long>> incs = findEmbeddingIncidencesBlock(b);
            rearrangeIncidences(b, incs);
        }
        else {

        }
    }

    EmbeddedBCTree embBcTree(bcTree);
    for (auto nit  = embBcTree.nodes().first; 
              nit != embBcTree.nodes().second; nit++) {
        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        auto& TN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
        if (TN.type()==BCTreeNode::BlockType) {
            auto& B   = TN.block();
            EmbeddedGraph eg;
            DualGraph     dg;
            PlanarDualGraphMaker maker;
            maker.makeDualGraph(B, eg, dg);
            ETN.setEmbedding(std::move(eg), std::move(dg));
        }
    }

    // Find faces.
    for (auto nit  = embBcTree.nodes().first;
              nit != embBcTree.nodes().second; nit++) {
        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
        if (ETN.type()==BCTreeNode::BlockType) {
            auto& DG  = ETN.dualGraph();
            for (auto nit2  = DG.nodes().first; 
                      nit2 != DG.nodes().second; nit2++) {
                auto& F = dynamic_cast<EmbeddedFace&>(*(*nit2));
                findFace(F);
            }
        }
    }

    // Create Unification Groups.
    for (auto& ugSpec : mUnificationGroups) {
        UnificationGroup ug;
        for (auto& ufSpec : ugSpec.ufs) {
            auto& ETN  = toEBCTreeNodeRefBlock(embBcTree, ufSpec.b);
            auto& F    = toFaceRef(ufSpec.f);
            auto& EG   = ETN.embeddedGraph();
            auto& ECCW = toEmbeddedEdgeRef(EG, ugSpec.c, ufSpec.nccw);
            auto& ECW  = toEmbeddedEdgeRef(EG, ugSpec.c, ufSpec.ncw);
            auto& CV   = toEmbeddedNodeRef(EG, ugSpec.c);

            UnificationFace uf(
            ETN.backIt(), F.backIt(), CV.backIt(), ECW.backIt(), ECCW.backIt()
            );
            ug.push(std::move(uf));
        }
        auto& ETNCV = toEBCTreeNodeRefCV(embBcTree, ugSpec.c);
        ETNCV.pushBackUnificationGroup(std::move(ug));
    }    

    auto& RB = toEBCTreeNodeRefBlock(embBcTree, mRootBlock);
    auto& EG = RB.embeddedGraph();

    auto& OF = toFaceRef(mRootOuterFace);
    auto& TN = toEmbeddedNodeRef(EG, mRootTopNode);

    embBcTree.findGeometricEmbedding(
        RB.backIt(),
        OF.backIt(),
        TN.backIt()
    );

    VisRepFinder vr(embBcTree);
    vr.find();


}


void CommandLineVisRepFinder::emitCoordSpec(
    const char* filename
) {
    std::ofstream   os(filename);

    os << "AREA\n";
    os << mLG.width() << " " << mLG.height() << "\n";
    os << "\n";
    os << "NODES\n";
    for (auto nit = mLG.nodes().first; nit != mLG.nodes().second; nit++) {
        auto& N = dynamic_cast<VRRectLabelNode&>(*(*nit));
        os << toVRRectLabelNodeNum(nit) << " ";
        os << N.xLeft()  << " ";
        os << N.xRight() << " ";
        os << N.y() << "\n";
    }
    os << "\n";
    os << "EDGES\n";
    for (auto eit = mLG.edges().first; eit != mLG.edges().second; eit++) {
        auto& E  = dynamic_cast<VRRectLabelEdge&>(*(*eit));
        auto& N1 = dynamic_cast<VRRectLabelNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<VRRectLabelNode&>(E.incidentNode2());
        os << toVRRectLabelNodeNum(N1.backIt()) << " ";
        os << toVRRectLabelNodeNum(N2.backIt()) << " ";
        os << E.x()  << " ";
        os << E.y1() << " ";
        os << E.y2() << " ";
        if (E.isFlipped()) {
            os << "FLIPPED";
        }
        else {
            os << "NOTFLIPPED";
        }
        os << "\n";
    }
}

} // namespace Undirected

} // namespace Wailea

static void print_usage()
{

    std::cerr << "vis_rep_finder : finds a visibility representation of the given connected graph\n";
    std::cerr << "    Usage : vis_rep_finder <input_file> <output_file>\n";
    std::cerr << "\n";
    std::cerr << "    Input file format:\n";
    std::cerr << "        GAPS\n";
    std::cerr << "        [W] [H]\n";
    std::cerr << "\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num] [Label W] [Label H]\n";
    std::cerr << "\n";
    std::cerr << "        EDGESn";
    std::cerr << "        [Node 1] [Node 2] CENTER/CW/CCW [Label1 W] [Label1 H] CENTER/CW/CCW [LabelM W] [LabelM H] CENTER/CW/CCW [Label2 W] [Label2 H]\n";
    std::cerr << "\n";
    std::cerr << "        INCIDENCES\n";
    std::cerr << "        [Node Num] [Incident Node 1] ... [ Incident Node X]\n";
    std::cerr << "\n";
    std::cerr << "        BLOCKS\n";
    std::cerr << "        [Block Num] [Node Num 1] ... [Node Num X]\n";
    std::cerr << "\n";
    std::cerr << "        FACES\n";
    std::cerr << "        [Face NUm] [Node Num1 ] ... [Node Num X]\n";
    std::cerr << "\n";
    std::cerr << "        UNIFICATION GROUPS\n";
    std::cerr << "        [Group Num] [Cut Vertex Num] [Block Num1] [Face Num1] [Node CCW1] [Node CW1] ... [Block NumN] [Face NumN] [Node CCWN] [Node CWN]\n";
    std::cerr << "\n";
    std::cerr << "        ROOT\n";
    std::cerr << "        [Block Num] [Top Node Num] [Outer Face Num]\n";
    std::cerr << "\n";
    std::cerr << "    Output file format:\n";
    std::cerr << "        AREA\n";
    std::cerr << "        [W] [H]\n";
    std::cerr << "\n";
    std::cerr << "        NODES\n";
    std::cerr << "        [Node Num] [X Left] [X Right] [Y]\n";
    std::cerr << "\n";
    std::cerr << "        EDGES\n";
    std::cerr << "        [Node 1] [Node 2] [X] [Y1] [Y2] FLIPPED/NOTFLIPPED\n";
    std::cerr << "\n";

}


int main(int argc, char *argv[])
{
    if (argc!=3) {
        print_usage();
        return 1;
    }
    Wailea::Undirected::CommandLineVisRepFinder p;
    p.parseSpec(argv[1]);
    p.generateVisRep();
    p.emitCoordSpec(argv[2]);
    return 0;
}
