#include "unit_tests/undirected/vis_rep_test_graph_generator.hpp"
#include <math.h>

namespace Wailea {

namespace Undirected { 

const std::string VisRepTestGraphParser::GAPS               = "GAPS";
const std::string VisRepTestGraphParser::NODES              = "NODES SPEC";
const std::string VisRepTestGraphParser::EDGES              = "EDGES SPEC";
const std::string VisRepTestGraphParser::INCIDENCES         = "INCIDENCES";
const std::string VisRepTestGraphParser::BLOCKS             = "BLOCKS";
const std::string VisRepTestGraphParser::FACES              = "FACES";
const std::string VisRepTestGraphParser::UNIFICATION_GROUPS = "UNIFICATION GROUPS";
const std::string VisRepTestGraphParser::ROOT               = "ROOT";
const std::string VisRepTestGraphParser::CENTER             = "CENTER";
const std::string VisRepTestGraphParser::CCW                = "CCW";
const std::string VisRepTestGraphParser::CW                 = "CW";
const std::string VisRepTestGraphParser::AREA_EXPECTED      = "AREA EXPECTED";
const std::string VisRepTestGraphParser::NODES_EXPECTED     = "NODES EXPECTED";
const std::string VisRepTestGraphParser::EDGES_EXPECTED     = "EDGES EXPECTED";

const float VisRepTestGraphGenerator::TOLERANCE_EPS   = 0.01;

void VisRepTestGraphParser::parseSpec(const char* filename)
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

          case IN_AREA_EXPECTED:
            handleAreaExpected(line, filename, lineNumber, error);
            break;

          case IN_NODES_EXPECTED:
            handleNodesExpected(line, filename, lineNumber, error);
            break;

          case IN_EDGES_EXPECTED:
            handleEdgesExpected(line, filename, lineNumber, error);
            break;

          case END:
            emitError(filename, lineNumber, "", error);
            break;
        }
    }        
}


bool VisRepTestGraphParser::isSectionHeader(std::string line, enum parseState& state)
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
    else if (line.compare(0, AREA_EXPECTED.size(), AREA_EXPECTED)==0) {
        state = IN_AREA_EXPECTED;
        return true;
    }
    else if (line.compare(0, NODES_EXPECTED.size(), NODES_EXPECTED)==0) {
        state = IN_NODES_EXPECTED;
        return true;
    }
    else if (line.compare(0, EDGES_EXPECTED.size(), EDGES_EXPECTED)==0) {
        state = IN_EDGES_EXPECTED;
        return true;
    }

    return false;
}


void VisRepTestGraphParser::emitError(
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


bool VisRepTestGraphParser::isCommentLine(std::string line)
{
    return line.at(0) == '#';
}


size_t VisRepTestGraphParser::splitLine(
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


void VisRepTestGraphParser::handleGaps(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 2) { 
        emitError(filename, lineNumber, "Invalid Gaps", errorFlag);
    }

    h_gap = std::stol(fields[0]);
    v_gap = std::stol(fields[1]);
} 


void VisRepTestGraphParser::handleNode(
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
    nodes.push_back(n);
} 


bool VisRepTestGraphParser::strToPosType(
    std::string& str, 
    enum Wailea::Undirected::VRRectLabelEdge::posType &type)
{
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


void VisRepTestGraphParser::handleEdge(
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

    edges.push_back(e);
} 


void VisRepTestGraphParser::handleIncidence(
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

    incidences.push_back(inc);
} 


void VisRepTestGraphParser::handleBlock(
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
    blocks.push_back(blk);
}


void VisRepTestGraphParser::handleFace(
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
    faces.push_back(fc);
}


void VisRepTestGraphParser::handleUnificationGroup(
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

    unificationGroups.push_back(ug);
} 


void VisRepTestGraphParser::handleAreaExpected(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
){

    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 2) { 
        emitError(filename, lineNumber, "Invalid value", errorFlag);
    }

    width_expected  = std::stof(fields[0]);
    height_expected = std::stof(fields[1]);

}


void VisRepTestGraphParser::handleNodesExpected(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {
    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 4) { 
        emitError(filename, lineNumber, "Invalid Node Expected", errorFlag);
    }

    _node_expected n;
    n.n       = std::stol(fields[0]);
    n.x_left  = std::stof(fields[1]);
    n.x_right = std::stof(fields[2]);
    n.y = std::stof(fields[3]);
    nodes_expected.push_back(n);

}


void VisRepTestGraphParser::handleEdgesExpected(
    std::string line,
    const char* filename,
    long        lineNumber,
    bool&       errorFlag
) {
    vector<std::string> fields;

    if (splitLine(line, fields, ' ')!= 5) { 
        emitError(filename, lineNumber, "Invalid Edge Expected", errorFlag);
    }

    _edge_expected e;
    e.n1  = std::stol(fields[0]);
    e.n2  = std::stol(fields[1]);
    e.x   = std::stof(fields[2]);
    e.y1  = std::stof(fields[3]);
    e.y2  = std::stof(fields[4]);
    edges_expected.push_back(e);
}


const std::string VisRepTestGraphParser::toStr(
    enum Wailea::Undirected::VRRectLabelEdge::posType type
) {
    if(type == VRRectLabelEdge::POS_CENTER){
        return CENTER;
    }
    else if(type == VRRectLabelEdge::POS_COUNTER_CLOCKWISE){
        return CCW;
    }
    else if(type == VRRectLabelEdge::POS_CLOCKWISE){
        return CW;
    }
    return CW;  
}


void VisRepTestGraphParser::debugPrint()
{
    std::cerr << "GAPS\n";
    std::cerr << h_gap << " " << v_gap << "\n";
    
    std::cerr << "NODES\n";
    for (auto& n : nodes) {
        std::cerr << n.n << " " << n.w << " " << n.h << "\n";
    }

    std::cerr << "EDGES\n";
    for (auto& e : edges) {
        std::cerr << e.n1 << " " << e.n2 << " "
                  << toStr(e.p1) << " " << e.w1 << " " << e.h1 << " "
                  << toStr(e.pm) << " " << e.wm << " " << e.hm << " "
                  << toStr(e.p2) << " " << e.w2 << " " << e.h2 << "\n";
    }

    std::cerr << "INCIDENCES\n";
    for (auto& i : incidences) {
        std::cerr << i.n << " ";
        for (auto& incn : i.incNodes) {
            std::cerr << incn << " ";
        }
        std::cerr << "\n";
    }

    std::cerr << "BLOCKS\n";
    for (auto& b : blocks) {
        std::cerr << b.b << " ";
        for (auto& nds : b.nodes) {
            std::cerr << nds << " ";
        }
        std::cerr << "\n";
    }

    std::cerr << "FACES\n";
    for (auto& f : faces) {
        std::cerr << f.f << " ";
        for (auto& nds : f.nodes) {
            std::cerr << nds << " ";
        }
        std::cerr << "\n";
    }

    std::cerr << "UNIFICATION GROUPS\n";
    for (auto& g : unificationGroups) {
        std::cerr << g.g << " ";
        std::cerr << g.c << " ";
        for (auto& fs : g.ufs) {
            std::cerr << fs.b << " ";
            std::cerr << fs.f << " ";
            std::cerr << fs.nccw << " ";
            std::cerr << fs.ncw << " ";
        }
        std::cerr << "\n";
    }

    std::cerr << "ROOT\n";
    std::cerr << rootBlock << " " 
              << rootTopNode << " " 
              << rootOuterFace << "\n";

}


void VisRepTestGraphParser::handleRoot(
    std::string line,
    const char* filename, 
    long        lineNumber,
    bool&       errorFlag
) {

    vector<std::string> fields;

    if (splitLine(line, fields, ' ') != 3) { 
        emitError(filename, lineNumber, "Invalid root", errorFlag);
    }

    rootBlock     = std::stol(fields[0]);
    rootTopNode   = std::stol(fields[1]);
    rootOuterFace = std::stol(fields[2]);
} 


VRRectLabelNode& VisRepTestGraphGenerator::toVRRectLabelNodeRef(long num)
{
    for(auto& n : mParser.nodes) {
        if (n.n == num) {
            return dynamic_cast<VRRectLabelNode&>(*(*n.it));
        }
    }
    cerr << "CKP XXX\n";
    return dynamic_cast<VRRectLabelNode&>(*(*(mParser.nodes[0].it)));
}


BlockNode& VisRepTestGraphGenerator::toBlockNodeRef(Block& b, long num)
{
    for (auto nit = b.nodes().first; nit != b.nodes().second; nit++) {
        auto& B = dynamic_cast<BlockNode&>(*(*nit));
        auto& N = dynamic_cast<VRRectLabelNode&>(B.IGBackwardLinkRef());
        auto nodeNum = toVRRectLabelNodeNum(N.backIt());
        if ( nodeNum == num) {
            return B;
        }
    }
    cerr << "ERROR\n";
    return dynamic_cast<BlockNode&>(*(*(b.nodes().first)));
}


BCTreeNode& VisRepTestGraphGenerator::toBCTreeNodeRefBlock(long num)
{
   for (auto& bSpec : mParser.blocks) {
       if (bSpec.b == num) {
          return dynamic_cast<BCTreeNode&>(*(*(bSpec.it)));
        }
    }
    cerr << "ERROR\n";
    return dynamic_cast<BCTreeNode&>(*(*(mParser.blocks[0].it)));
}


EmbeddedBCTreeNode& VisRepTestGraphGenerator::toEBCTreeNodeRefBlock(
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
    cerr << "ERROR\n";
    return dynamic_cast<EmbeddedBCTreeNode&>(*(*(tree.nodes().first)));
}        


EmbeddedBCTreeNode& VisRepTestGraphGenerator::toEBCTreeNodeRefCV(
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

    cerr << "ERROR\n";

    return dynamic_cast<EmbeddedBCTreeNode&>(*(*(tree.nodes().first)));
}        


long VisRepTestGraphGenerator::toVRRectLabelNodeNum(node_list_it_t it)
{
    for(auto& n : mParser.nodes) {
        if (n.it == it) {
            return n.n;
        }
    }
    cerr << "ERROR\n";
    return mParser.nodes[0].n;
}


edge_list_it_t VisRepTestGraphGenerator::toVRRectLabelEdgeIt(long num1, long num2)
{
    for(auto& e : mParser.edges) {
        if ( (e.n1 == num1 && e.n2 == num2) ||
             (e.n1 == num2 && e.n2 == num1)    ) {
            return e.it;
        }
    }
    cerr << "ERROR\n";
    return mParser.edges[0].it;
}
 

EmbeddedFace& VisRepTestGraphGenerator::toFaceRef(long num)
{
    for (auto& f : mParser.faces) {
        if (f.f == num) {
            return dynamic_cast<EmbeddedFace&>(*(*(f.it)));
        }
    }
    cerr << "ERROR\n";
    return dynamic_cast<EmbeddedFace&>(*(*(mParser.faces[0].it)));
}


EmbeddedEdge& VisRepTestGraphGenerator::toEmbeddedEdgeRef(
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
    cerr << "ERROR\n";
    return dynamic_cast<EmbeddedEdge&>(*(*(EG.edges().first)));
}


EmbeddedNode& VisRepTestGraphGenerator::toEmbeddedNodeRef(
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
    cerr << "ERROR\n";
    return dynamic_cast<EmbeddedNode&>(*(*(EG.nodes().first)));
}


bool VisRepTestGraphGenerator::orderAgnosticCompare(
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


bool VisRepTestGraphGenerator::cyclicCompare(vector<long>& vec1, vector<long>& vec2)
{

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


void VisRepTestGraphGenerator::findBlock(Block& b)
{
    vector<long> blockNums;
    for (auto nit = b.nodes().first; nit != b.nodes().second; nit++) {
        auto& B = dynamic_cast<BlockNode&>(*(*nit));
        auto& N = dynamic_cast<VRRectLabelNode&>(B.IGBackwardLinkRef());
        blockNums.push_back(toVRRectLabelNodeNum(N.backIt()));
    }

    for (auto& blk : mParser.blocks) {
        if (orderAgnosticCompare(blk.nodes, blockNums)) {
            blk.it = b.BCTreeNode().backIt();
#ifdef USE_TEST_GRAPH_GENERATOR
            b.mDebugNum = blk.b;
#endif
        }
    }
}


bool VisRepTestGraphGenerator::isIn(long n, vector<long>& vec) {
    for (auto val : vec) {
        if (n==val) {
            return true;
        }
    }
    return false;
}


vector<vector<long>> VisRepTestGraphGenerator::findEmbeddingIncidencesBlock(Block& b)
{
    vector<long> nodeNums;
    vector<vector<long>> incs;

    for (auto bnit = b.nodes().first; bnit != b.nodes().second; bnit++) {

        auto& BN  = dynamic_cast<BlockNode&>(*(*bnit));
        auto& LN  = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());

        nodeNums.push_back(toVRRectLabelNodeNum(LN.backIt()));

    }
    
    for (auto& inc : mParser.incidences) {
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


BlockEdge& VisRepTestGraphGenerator::toBlockEdge(
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
    cerr << "ERROR\n";
    return dynamic_cast<BlockEdge&>(*(*(b.edges().first)));
}


void VisRepTestGraphGenerator::rearrangeIncidences(
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


vector<long> VisRepTestGraphGenerator::findBlockNodes(EmbeddedBCTreeNode& N)
{
    auto& N2 = dynamic_cast<BCTreeNode&>(N.IGBackwardLinkRef());
    auto& B  = N2.block();

    vector<long> blockNums;

    for (auto nit = B.nodes().first; nit != B.nodes().second; nit++) {
        auto& BN = dynamic_cast<BlockNode&>(*(*nit));
        auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
        blockNums.push_back(toVRRectLabelNodeNum(LN.backIt()));
    }

    return blockNums;
}


vector<long> VisRepTestGraphGenerator::findIncidentNodes(EmbeddedFace& F)
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


void VisRepTestGraphGenerator::findFace(EmbeddedFace& F)
{
    vector<long> nodeNums = findIncidentNodes(F);

    for (auto& fspec : mParser.faces) {
        if (cyclicCompare(fspec.nodes, nodeNums)) {
            fspec.it = F.backIt();
#ifdef USE_TEST_GRAPH_GENERATOR
            F.mDebugNum = fspec.f;
#endif
        }
    }
}

            
VisRepTestGraphGenerator::VisRepTestGraphGenerator(VisRepTestGraphParser& parser):
                           mParser(parser),mBCTree(mDummy),mEBCTree(mBCTree){;}



void VisRepTestGraphGenerator::generateGraphs()
{
//cerr << "generateGraphs() 1\n";
    mLG.setHorizontalGap(mParser.h_gap);
    mLG.setVerticalGap(mParser.v_gap);

    for (auto& n : mParser.nodes) {

        auto  np  = std::make_unique<VRRectLabelNode>(n.w, n.h);
                                                
        auto& nr  = dynamic_cast<VRRectLabelNode&>(mLG.addNode(std::move(np)));
#ifdef USE_TEST_GRAPH_GENERATOR
        nr.mDebugNum = n.n;
#endif
        n.it = nr.backIt();

    }
//cerr << "generateGraphs() 2\n";
    for (auto& e : mParser.edges) {

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
//cerr << "generateGraphs() 3: " << mLG.numNodes() << "," << mLG.numEdges() << "\n";
    for (auto& i : mParser.incidences) {
        vector<edge_list_it_t> incIts;
//cerr << "incidence: " << i.n << ":";
        for (auto a :  i.incNodes) {
//cerr << a << " ";
            incIts.push_back(toVRRectLabelEdgeIt(i.n, a));
        }
//cerr << "\n";
        toVRRectLabelNodeRef(i.n).reorderIncidence(std::move(incIts));
    }

    BiconnectedDecomposer decomp;
    mBCTree = decomp.decompose(mLG, *(*(mLG.nodes().first)));
//cerr << "generateGraphs() 4: " << mBCTree.numNodes() << "," << mBCTree.numEdges() << "\n";
    for (auto bnit  = mBCTree.nodes().first; 
              bnit != mBCTree.nodes().second; bnit++) {

        auto& bn = dynamic_cast<BCTreeNode&>(*(*bnit));
        if (bn.type()==BCTreeNode::BlockType) {
//cerr << "generateGraphs() block type\n";
            auto& b = bn.block();
            findBlock(b);
            vector<vector<long>> incs = findEmbeddingIncidencesBlock(b);
            rearrangeIncidences(b, incs);
        }
        else {
//cerr << "generateGraphs() cv type\n";
        }
    }
//cerr << "generateGraphs() 5\n";
    EmbeddedBCTree EmbBcTree(mBCTree);
//cerr << "generateGraphs() 6\n";
    // Rearrange incidence for each block in emb bc tree.
    for (auto nit  = EmbBcTree.nodes().first; 
              nit != EmbBcTree.nodes().second; nit++) {
//cerr << "generateGraphs() 6.1\n";
        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
//cerr << "generateGraphs() 6.2\n";
        auto& TN  = dynamic_cast<BCTreeNode&>(ETN.IGBackwardLinkRef());
//cerr << "generateGraphs() 6.3\n";
        if (TN.type()==BCTreeNode::BlockType) {
//cerr << "generateGraphs() 6.4\n";
            auto& B   = TN.block();
//cerr << "generateGraphs() 6.5\n";
            EmbeddedGraph eg;
//cerr << "generateGraphs() 6.6\n";
            DualGraph     dg;
//cerr << "generateGraphs() 6.7\n";
            PlanarDualGraphMaker maker;
//cerr << "generateGraphs() 6.8\n";
            maker.makeDualGraph(B, eg, dg);
//cerr << "generateGraphs() 6.9\n";
            ETN.setEmbedding(std::move(eg), std::move(dg));
//cerr << "generateGraphs() 6.10\n";
        }
//cerr << "generateGraphs() 6.11\n";
    }
//cerr << "generateGraphs() 7\n";
    // Find faces.
    for (auto nit  = EmbBcTree.nodes().first;
              nit != EmbBcTree.nodes().second; nit++) {
//cerr << "generateGraphs() 7.1\n";
        auto& ETN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
//cerr << "generateGraphs() 7.2\n";
        if (ETN.type()==BCTreeNode::BlockType) {
//cerr << "generateGraphs() 7.3\n";
            auto& DG  = ETN.dualGraph();
//cerr << "generateGraphs() 7.4\n";
            for (auto nit2  = DG.nodes().first; 
                      nit2 != DG.nodes().second; nit2++) {
//cerr << "generateGraphs() 7.5\n";
                auto& F = dynamic_cast<EmbeddedFace&>(*(*nit2));
//cerr << "generateGraphs() 7.6\n";
                findFace(F);
//cerr << "generateGraphs() 7.7\n";
            }
        }
    }
//cerr << "generateGraphs() 8\n";
    // Create Unification Groups.
    for (auto& ugSpec : mParser.unificationGroups) {
//cerr << "generateGraphs() 8.1\n";
        UnificationGroup ug;
//cerr << "generateGraphs() 8.2\n";
        for (auto& ufSpec : ugSpec.ufs) {
//cerr << "generateGraphs() 8.3\n";
            auto& ETN  = toEBCTreeNodeRefBlock(EmbBcTree, ufSpec.b);
//cerr << "generateGraphs() 8.4 f:"<< ufSpec.f << "\n";
            auto& F    = toFaceRef(ufSpec.f);
//cerr << "generateGraphs() 8.5\n";
            auto& EG   = ETN.embeddedGraph();
//cerr << "generateGraphs() 8.6\n";
            auto& ECCW = toEmbeddedEdgeRef(EG, ugSpec.c, ufSpec.nccw);
//cerr << "generateGraphs() 8.7\n";
            auto& ECW  = toEmbeddedEdgeRef(EG, ugSpec.c, ufSpec.ncw);
//cerr << "generateGraphs() 8.8\n";
            auto& CV   = toEmbeddedNodeRef(EG, ugSpec.c);
//cerr << "generateGraphs() 8.9\n";
            UnificationFace uf(
                ETN.backIt(),
                F.backIt(),
                CV.backIt(),
                ECW.backIt(),
                ECCW.backIt()
            );
//cerr << "generateGraphs() 8.10\n";
            ug.push(std::move(uf));
//cerr << "generateGraphs() 8.11\n";
        }
//cerr << "generateGraphs() 8.12 c: " << ugSpec.c << "\n";
        auto& ETNCV = toEBCTreeNodeRefCV(EmbBcTree, ugSpec.c);
//cerr << "generateGraphs() 8.13\n";
        ETNCV.pushBackUnificationGroup(std::move(ug));
//cerr << "generateGraphs() 8.14\n";
    }    
//cerr << "generateGraphs() 9\n";
    auto& RB = toEBCTreeNodeRefBlock(EmbBcTree, mParser.rootBlock);
    auto& EG = RB.embeddedGraph();
//cerr << "EG (" << EG.numNodes() << "," << EG.numEdges() << "\n";
    auto& OF = toFaceRef(mParser.rootOuterFace);
    auto& TN = toEmbeddedNodeRef(EG, mParser.rootTopNode);

    EmbBcTree.findGeometricEmbedding(
        RB.backIt(),
        OF.backIt(),
        TN.backIt()
    );
//cerr << "generateGraphs() 9: (" << EmbBcTree.numNodes() << "," << EmbBcTree.numEdges() << ")\n";
//    auto& ExpTree = EmbBcTree.explorationTree();
//cerr << "expTree(" << ExpTree.numNodes() << "," << ExpTree.numEdges() << ")\n";

    mEBCTree = std::move(EmbBcTree);

}


bool VisRepTestGraphGenerator::inspect()
{
    if(mLG.width()!=mParser.width_expected) {
        cerr << "Wrong width.\n";
        return false;
    }
    if(mLG.height()!=mParser.height_expected) {
        cerr << "Wrong height.\n";
        return false;
    }

    for (auto nit = mLG.nodes().first; nit != mLG.nodes().second; nit++) {
        auto& N = dynamic_cast<VRRectLabelNode&>(*(*nit));
        long  Nnum = toVRRectLabelNodeNum(nit);
        auto& exp = toNodeExpected(Nnum);
        if (fabs(N.y() - exp.y)> TOLERANCE_EPS) {
            cerr << "Wrong node y.\n";
            return false;
        }
        if (fabs(N.xLeft() - exp.x_left) > TOLERANCE_EPS) {
            cerr << "Wrong node x left.\n";
            return false;
        }
        if (fabs(N.xRight() - exp.x_right) > TOLERANCE_EPS) {
            cerr << "Wrong node x right.\n";
            cerr << "N.xRight(): " << N.xRight() << "\n";
            cerr << "exp.x_right: " << exp.x_right << "\n";
            return false;
        }
    }

    for (auto eit = mLG.edges().first; eit != mLG.edges().second; eit++) {
        auto& E = dynamic_cast<VRRectLabelEdge&>(*(*eit));
        long N1num = toVRRectLabelNodeNum(E.incidentNode1().backIt());
        long N2num = toVRRectLabelNodeNum(E.incidentNode2().backIt());
        auto& exp = toEdgeExpected(N1num, N2num);
        if (fabs(E.x() - exp.x) > TOLERANCE_EPS) {
            cerr << "Wrong edge x.\n";
            return false;
        }
        if (fabs(E.y1() - exp.y1) > TOLERANCE_EPS) {
            cerr << "Wrong edge y1.\n";
            return false;
        }
        if (fabs(E.y2() - exp.y2) > TOLERANCE_EPS) {
            cerr << "Wrong edge y2.\n";
            return false;
        }
    }

    return true;
}

void VisRepTestGraphGenerator::emitInc(long inc)
{
    for (size_t i = 0; i < inc; i++) {
        cerr << " ";
    }
}


VisRepTestGraphParser::_node_expected& VisRepTestGraphGenerator::toNodeExpected(long num)
{
    for (auto& n : mParser.nodes_expected) {
        if (n.n == num){
            return n;
        }
    }
    return mParser.nodes_expected[0];
}



VisRepTestGraphParser::_edge_expected& VisRepTestGraphGenerator::toEdgeExpected(long num1, long num2)
{
    for (auto& e : mParser.edges_expected) {
        if (num1 == e.n1 && num2 == e.n2) {
            return e;
        }
    }
    return mParser.edges_expected[0];
}


long VisRepTestGraphGenerator::toVRRectLabelNodeNum(EmbeddedNode& EN)
{
    auto& BN = dynamic_cast<BlockNode&>(EN.IGBackwardLinkRef());
    auto& LN = dynamic_cast<VRRectLabelNode&>(BN.IGBackwardLinkRef());
    return toVRRectLabelNodeNum(LN.backIt());
}


void VisRepTestGraphGenerator::debugPrint(long inc, EmbeddedGraph& EG)
{
    emitInc(inc);
    cerr << "EmbeddedGraph(N,E)=(" << EG.numNodes() << ","
         << EG.numEdges() << ")\n";

    for (auto nit = EG.nodes().first; nit != EG.nodes().second; nit++) {

        auto& EN = dynamic_cast<EmbeddedNode&>(*(*nit));

        emitInc(inc+4);
        cerr << "Node: [" << toVRRectLabelNodeNum(EN) << "] Incidence: ";
        for (auto iit  = EN.incidentEdges().first; 
                  iit != EN.incidentEdges().second; iit++) {

            auto& EE = dynamic_cast<EmbeddedEdge&>(*(*(*iit)));
            auto& AN = dynamic_cast<EmbeddedNode&>(EE.adjacentNode(EN));
            cerr << toVRRectLabelNodeNum(AN) << " ";
        }
        cerr << "\n";
    }
}


long VisRepTestGraphGenerator::toFaceNum(vector<long>& surroundingNodes)
{

    for (auto& fspec : mParser.faces) {
        if (cyclicCompare(fspec.nodes, surroundingNodes)) {
            return fspec.f;
        }
    }

    return -1;
}


long VisRepTestGraphGenerator::toBlockNum(EmbeddedBCTreeNode& N)
{
    vector<long> nodes = findBlockNodes(N);
    return toBlockNum(nodes);
}


long VisRepTestGraphGenerator::toBlockNum(vector<long>& nodes)
{

    for (auto& bspec : mParser.blocks) {
        if (orderAgnosticCompare(bspec.nodes, nodes)) {
            return bspec.b;
        }
    }

    return -1;
}


long VisRepTestGraphGenerator::toFaceNum(EmbeddedFace& F)
{

    vector<long> surroundingNodes = findIncidentNodes(F);
    return toFaceNum(surroundingNodes);
}


void VisRepTestGraphGenerator::debugPrint(long inc, DualGraph& DG)
{
    emitInc(inc);
    cerr << "DualGraph(F,DE)=(" << DG.numNodes() << ","
         << DG.numEdges() << ")\n";

    for (auto fit = DG.nodes().first; fit != DG.nodes().second; fit++) {
        auto& F = dynamic_cast<EmbeddedFace&>(*(*fit));
        emitInc(inc+4);
        vector<long> surroundingNodes = findIncidentNodes(F);
        cerr << "Face: [" 
             << toFaceNum(surroundingNodes)
             << "] Surrouding nodes: ";
        for (auto n :  surroundingNodes) {
            cerr << n << " ";
        }
        cerr << "\n";
    }
}


void VisRepTestGraphGenerator::debugPrint(long inc, UnificationGroup& UG)
{
    emitInc(inc);    
    cerr << "Unification Group:\n";
    for (size_t i = 0 ; i < UG.size() ; i++) {
        debugPrint(inc + 4, UG.at(i));

    }
}


void VisRepTestGraphGenerator::debugPrint(long inc, UnificationFace& UF)
{
    emitInc(inc);    
    cerr << "Unification Face: ";
    cerr << "Block: " << toBlockNum(UF.treeNode());
    cerr << " Face: " << toFaceNum(UF.faceInDG());
    cerr << " Cut Vertex: " << toVRRectLabelNodeNum(UF.cutVertexInEG());
    auto& ECCW  = UF.edgeCCWInEG();
    auto& NCCW1 = dynamic_cast<EmbeddedNode&>(ECCW.incidentNode1());
    auto& NCCW2 = dynamic_cast<EmbeddedNode&>(ECCW.incidentNode2());
    cerr << " Edge CCW: {" << toVRRectLabelNodeNum(NCCW1) << "," 
         << toVRRectLabelNodeNum(NCCW2) << "} ";
    auto& ECW  = UF.edgeCWInEG();
    auto& NCW1 = dynamic_cast<EmbeddedNode&>(ECW.incidentNode1());
    auto& NCW2 = dynamic_cast<EmbeddedNode&>(ECW.incidentNode2());
    cerr << " Edge CW: {" << toVRRectLabelNodeNum(NCW1) << "," 
         << toVRRectLabelNodeNum(NCW2) << "} ";
    if (UF.roleOfECWReversed()) {
        cerr << "Role reversed";
    }
    else {
        cerr << "Role not reversed";
    }
    cerr << "\n";
}


void VisRepTestGraphGenerator::debugPrintEdges(EmbeddedBCTreeNode& N)
{
    auto& N2 = dynamic_cast<BCTreeNode&>(N.IGBackwardLinkRef());
    auto& B  = N2.block();
    for(auto eit = B.edges().first; eit != B.edges().second; eit++) {
        auto& BE  = dynamic_cast<BlockEdge&>(*(*eit));
        auto& BN1 = dynamic_cast<BlockNode&>(BE.incidentNode1());
        auto& LN1 = dynamic_cast<VRRectLabelNode&>(BN1.IGBackwardLinkRef());
        auto& BN2 = dynamic_cast<BlockNode&>(BE.incidentNode2());
        auto& LN2 = dynamic_cast<VRRectLabelNode&>(BN2.IGBackwardLinkRef());
        cerr << "{" << toVRRectLabelNodeNum(LN1.backIt()) << ","
             << toVRRectLabelNodeNum(LN2.backIt()) << "} ";
    }
}


void VisRepTestGraphGenerator::debugPrint(
    long                             inc,
    EmbeddedBCTree::ExplorationNode& N
) {

    auto& Gs = N.sortedUnificationGroups();
    emitInc(inc);
    cerr << "Unification Groups\n";
    for (auto& G : Gs) {
        emitInc(inc+4);
        cerr << "Unification Group\n";
        emitInc(inc+8);cerr << "Absorber\n";
        debugPrint(inc+8, G.absorber());
        for (size_t i = 0; i < G.absorbeesSize(); i++) {
            emitInc(inc+8);cerr << "Absorbee " << i << "\n";
            debugPrint(inc+8, G.absorbeeAt(i));
        }
    }

}


void VisRepTestGraphGenerator::emitUnitTestExpectation()
{
    cerr << "\n";
    cerr << "# [Width] [Height]\n";
    cerr << "AREA EXPECTED\n";
    cerr << mLG.width() << " " << mLG.height() << "\n";
    cerr << "\n";
    cerr << "# [NodeNum] [xLeft] [xRight] [y]\n";
    cerr << "NODES EXPECTED\n";
    for (auto nit = mLG.nodes().first; nit != mLG.nodes().second; nit++) {

        auto& N = dynamic_cast<VRRectLabelNode&>(*(*nit));

        cerr << toVRRectLabelNodeNum(nit) << " ";
        cerr << N.xLeft() << " ";
        cerr << N.xRight() << " ";
        cerr << N.y() << "\n";

    }
    cerr << "# [Node1] [Node2] [x] [y1] [y2]\n";
    cerr << "EDGES EXPECTED\n";
    for (auto eit = mLG.edges().first; eit != mLG.edges().second; eit++) {

        auto& E  = dynamic_cast<VRRectLabelEdge&>(*(*eit));
        auto& N1 = dynamic_cast<VRRectLabelNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<VRRectLabelNode&>(E.incidentNode2());

        cerr << toVRRectLabelNodeNum(N1.backIt()) << " ";
        cerr << toVRRectLabelNodeNum(N2.backIt()) << " ";
        cerr << E.x()  << " ";
        cerr << E.y1() << " ";
        cerr << E.y2() << "\n";

    }
}


void VisRepTestGraphGenerator::debugPrint()
{
    cerr << "EmbeddedBCTree(N,E)=(" << mEBCTree.numNodes() << ","
        << mEBCTree.numEdges() << ")\n";
    for (auto nit  = mEBCTree.nodes().first; 
              nit != mEBCTree.nodes().second; nit++) {
         emitInc(4);
         cerr << "EmbeddedBCTreeNode ";
         auto& EBTN = dynamic_cast<EmbeddedBCTreeNode&>(*(*nit));
         if (EBTN.type()==BCTreeNode::BlockType){
             cerr << "BlockType: Block Num: " << toBlockNum(EBTN) << " ";
             cerr << " Outer Face: ";
             cerr << toFaceNum(EBTN.outerFace()) << " ";
             cerr << "Top Node: " 
                  << toVRRectLabelNodeNum(EBTN.topNode()) << "\n";
             emitInc(8);
             cerr << "Block Edges: ";
             debugPrintEdges(EBTN);
             cerr << "\n";
             debugPrint(8, EBTN.embeddedGraph());
             debugPrint(8, EBTN.dualGraph());
         }
         else {
             cerr << "CutVertexType\n";
             for (size_t i = 0; i < EBTN.numUnificationGroups(); i++) {
                 debugPrint(8, EBTN.unificationGroup(i));
             }
         }

    }

    auto& expT = mEBCTree.explorationTree();
    cerr << "ExplorationTree(N,E)=(" << expT.numNodes() << ","
         << expT.numEdges() << ")\n";
    auto& root = expT.root();
    list<node_list_it_t> Q;
    Q.push_back(root.backIt());
    while (Q.size() > 0) {
        auto& N = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                           *(*(*(Q.begin()))));
        Q.pop_front();
        cerr << "ExplorationNode ";
        cerr << "Block Num: " << toBlockNum(
                   dynamic_cast<EmbeddedBCTreeNode&>(N.IGBackwardLinkRef()))
             << " ";
                                                
        cerr << "\n";

        debugPrint(4, N);

        emitInc(4);    
        if (N.degreeOut()>0) {
            cerr << "Edges: ";
            for (auto iit = N.incidentEdgesOut().first; 
                      iit != N.incidentEdgesOut().second; iit++){
                auto& E = dynamic_cast<EmbeddedBCTree::ExplorationEdge&>(
                                                                   *(*(*iit)));
                auto& A = dynamic_cast<EmbeddedBCTree::ExplorationNode&>(
                                                            E.adjacentNode(N));
                cerr << "(" << toBlockNum(
                    dynamic_cast<EmbeddedBCTreeNode&>(N.IGBackwardLinkRef()))
                     << "->" 
                     << toBlockNum(
                    dynamic_cast<EmbeddedBCTreeNode&>(N.IGBackwardLinkRef()))
                     << ") ";
                Q.push_back(A.backIt());
            }
        }                
        cerr << "\n";
    }

    cerr << "VRRectLabelGraph(N,E)=(" << mLG.numNodes() << ","
         << mLG.numEdges() << ")\n";
    emitInc(4);
    cerr << "AREA:\tW: " << mLG.width() << "\tH: " << mLG.height() << "\n";
    for (auto nit = mLG.nodes().first; nit != mLG.nodes().second; nit++) {
        auto& N = dynamic_cast<VRRectLabelNode&>(*(*nit));
        emitInc(4);
        cerr << "Node: " << toVRRectLabelNodeNum(nit) << "\t";
        cerr << "W: " << N.width() << "\t";
        cerr << "H: " << N.height() << "\t";
        cerr << "xLeft: " << N.xLeft() << "\t";
        cerr << "xRight: " << N.xRight() << "\t";
        cerr << "y: " << N.y() << "\n";
    }

    for (auto eit = mLG.edges().first; eit != mLG.edges().second; eit++) {
        auto& E  = dynamic_cast<VRRectLabelEdge&>(*(*eit));
        auto& N1 = dynamic_cast<VRRectLabelNode&>(E.incidentNode1());
        auto& N2 = dynamic_cast<VRRectLabelNode&>(E.incidentNode2());
        emitInc(4);
        cerr << "Edge: {" << toVRRectLabelNodeNum(N1.backIt()) << ","
             << toVRRectLabelNodeNum(N2.backIt()) << "}\t"
             << mParser.toStr(E.typeNode1()) << "\t"
             << "W: " << E.widthNode1() << "\t"
             << "H: " << E.heightNode1() << "\t"
             << mParser.toStr(E.typeMiddle()) << "\t"
             << "W: " << E.widthMiddle() << "\t"
             << "H: " << E.heightMiddle() << "\t"
             << mParser.toStr(E.typeNode2()) << "\t"
             << "W: " << E.widthNode2() << "\t"
             << "H: " << E.heightNode2() << "\t"
             << "x: " << E.x() << "\t"
             << "y1: " << E.y1() << "\t"
             << "y2: " << E.y2() << "\t";
         if (E.isFlipped()) {
             cerr << "FLIPPED";
         }
         else {
             cerr << "NOTFLIPPED";
         }
         cerr << "\n";
    }
    cerr << "VRRectLabelGraph END\n";
}


} // namespace Undirected

} // namespace Wailea
