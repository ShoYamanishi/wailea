#include "gtest/gtest.h"
#include "undirected/base.hpp"
#include "undirected/embedded_bctree.hpp"
#include "undirected/vis_rep_finder.hpp"
#include "undirected/planar_dual_graph_maker.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Wailea {

namespace Undirected { 


/**
 * GAPS
 * [Horizontal GAP] [Virtical Gap]
 *
 * NODES
 * [Nod Num] [W] [H]
 *
 * EDGES 
 * [Node1] [Node2] [P1] [W1] [H1] [Pm] [Wm] [Hm] [P2] [W2] [H2]
 * # Px: CENTER, CCW, CW
 *
 * INCIDENCES
 * [Node] [Inc Node1] [Inc Node2]...[Inc NodeN]
 *
 * BLOCKS
 * [Block Num] [Node1] [Node2] ... [NodeN-1]
 *
 * FACES
 * [Face Num] [Node1] [Node2] ... [NodeN-1] in counter-clockwise
 *
 * UNIFICATION GROUPS
 * [Group Num] [Cut Vertex Num] [Block Num1] [Face Num1] [Node CCW1] [Node CW1]
 *                           ...[Block NumN] [Face NumN] [Node CCWN] [Node CWN]
 *
 * ROOT
 * [Block Num] [Top Node Num] [Face Num]
 *
 * AREA_EXPECTED:
 * [Width] [Height]
 *
 * NODES_EXPECTED
 * [NodeNum] [xLeft][xRight] [y]
 *
 * EDGES EXPECTED
 * [Node1] [Node2] [x] [y1] [y2]
 */
class VisRepTestGraphParser {

  public:

    void parseSpec(const char* filename);

    void debugPrint();

    using pType = 
            enum Wailea::Undirected::VRRectLabelEdge::posType;

    const std::string toStr(pType type);

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


    class _node_expected {
      public:
        long  n;
        float y;
        float x_left;
        float x_right;
    };

    class _edge_expected {
      public:
        long  n1;
        long  n2;
        float x;
        float y1;
        float y2;
    };

    long                      h_gap;
    long                      v_gap;
    vector<_node>             nodes;
    vector<_edge>             edges;
    vector<_incidence>        incidences;
    vector<_block>            blocks;
    vector<_face>             faces;
    vector<_unificationGroup> unificationGroups;
    long                      rootBlock;
    long                      rootTopNode;
    long                      rootOuterFace;

    float                     width_expected;
    float                     height_expected;
    vector<_node_expected>    nodes_expected;
    vector<_edge_expected>    edges_expected;

  private:

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
    static const std::string AREA_EXPECTED;
    static const std::string NODES_EXPECTED;
    static const std::string EDGES_EXPECTED;


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
        IN_AREA_EXPECTED,
        IN_NODES_EXPECTED,
        IN_EDGES_EXPECTED,
        END
    };

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

    void handleAreaExpected(
        std::string line,
        const char* filename, 
        long        lineNumber,
        bool&       errorFlag
    );

    void handleNodesExpected(
        std::string line,
        const char* filename, 
        long        lineNumber,
        bool&       errorFlag
    );

    void handleEdgesExpected(
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

};


class VisRepTestGraphGenerator {

  public:
    static const float TOLERANCE_EPS;

    VisRepTestGraphGenerator(VisRepTestGraphParser& parser);

    void                  generateGraphs();
    EmbeddedBCTree&       getEBCTree()    { return mEBCTree; }
    BCTree&               getBCTree()     { return mBCTree; }
    VRRectLabelGraph&     getLabelGraph() { return mLG; }
    void                  debugPrint();
    void                  emitUnitTestExpectation();
    bool                  inspect();

  private:

    VRRectLabelNode&      toVRRectLabelNodeRef(long n);
    long                  toVRRectLabelNodeNum(node_list_it_t it);
    edge_list_it_t        toVRRectLabelEdgeIt(long num1, long num2);
    void                  findBlock(Block& b);
    vector<long>          findBlockNodes(EmbeddedBCTreeNode& N);
    bool                  orderAgnosticCompare(
                                       vector<long>& vec1, vector<long>& vec2);
    bool                  cyclicCompare(
                                       vector<long>& vec1, vector<long>& vec2);
    vector<vector<long> > findEmbeddingIncidencesBlock(Block& b);
    bool                  isIn(long n, vector<long>& vec);
    void                  rearrangeIncidences(
                                          Block& b, vector<vector<long>>&incs);
    BlockNode&            toBlockNodeRef(Block& b, long num);
    BlockEdge&            toBlockEdge(Block& b, BlockNode& N1, BlockNode& N2);
    long                  toVRRectLabelNodeNum(EmbeddedNode& EN);
    long                  toFaceNum(vector<long>& surroundingNodes);
    long                  toFaceNum(EmbeddedFace& F);
    long                  toBlockNum(EmbeddedBCTreeNode& N);
    long                  toBlockNum(vector<long>& nodes);
    BCTreeNode&           toBCTreeNodeRefBlock(long num);
    EmbeddedBCTreeNode&   toEBCTreeNodeRefBlock(
                                               EmbeddedBCTree& tree, long num);
    EmbeddedBCTreeNode&   toEBCTreeNodeRefCV(EmbeddedBCTree& tree, long num);
    EmbeddedFace&         toFaceRef(long num);
    EmbeddedEdge&         toEmbeddedEdgeRef(
                                      EmbeddedGraph& EG, long num1, long num2);
    EmbeddedNode&         toEmbeddedNodeRef(EmbeddedGraph& EG, long num);
    vector<long>          findIncidentNodes(EmbeddedFace& F);
    void                  findFace(EmbeddedFace& F);
    VisRepTestGraphParser::_node_expected& toNodeExpected(long num);
    VisRepTestGraphParser::_edge_expected& toEdgeExpected(long n1, long n2);

    void                  debugPrint(long inc, EmbeddedGraph& T);
    void                  debugPrint(long inc, DualGraph& DG);
    void                  debugPrint(long inc, UnificationGroup& UG);
    void                  debugPrint(long inc, UnificationFace& UF);
    void                  debugPrintEdges(EmbeddedBCTreeNode& N);
    void                  debugPrint(
                                 long inc, EmbeddedBCTree::ExplorationNode& N);
    void                  emitInc(long inc);


    VisRepTestGraphParser& mParser;
    VRRectLabelGraph       mLG;
    Graph                  mDummy;
    BCTree                 mBCTree;
    EmbeddedBCTree         mEBCTree;

};

} // namespace Undirected

} // namespace Wailea

