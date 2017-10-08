import sys
import re
from subprocess import call
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import matplotlib.transforms as mtransforms
import matplotlib.text as mtext
import matplotlib.patches as patches
from matplotlib.patches import Polygon



# Utility class to implement enum equivalent
class enum(set):
    def __getattr__(self, name):
        if name in self:
            return name
        raise AttributeError

# Internal states used by parses
parseState = enum(["INIT", 
                   "ERROR", 
                   "IN_TOP_NODE",
                   "IN_GAPS",
                   "IN_NODES",
                   "IN_VIRTUAL_NODES",
                   "IN_EDGES",
                   "IN_CUT_VERTICES",
                   "IN_BLOCK_BEGIN",
                   "IN_BLOCK_CUT_VERTICES",
                   "IN_BLOCK_ORDINARY_VERTICES",
                   "IN_BLOCK_EDGES",
                   "IN_BLOCK_END",
                   "IN_INCIDENCES",
                   "IN_FACES",
                   "IN_AREA",
                   "END"])

# Position of edge labels: Center, Clockwise side, and Counter-clockwise side
edgeLabelPos = enum(["CENTER", "CW", "CCW"])


# Base parser class.
class parser(object):

    IN_DELIM  = ' '
    OUT_DELIM = ' '

    def __init__(self, filePath):
        self.error   = None
        self.state = parseState.INIT
        with open(filePath, "r") as ins:
            lineno = 0
            for line in ins:
                line = line.strip()
                lineno = lineno + 1
                comment = self.isComment(line)
                if comment:
                    continue
                changed, self.error = self.updateState(line)
                if changed:
                    continue
                if self.error:
                    self.emitError(lineno, line)
                    break
                self.error = self.parseLine(line)
                if self.error:
                    self.emitError(lineno, self.error)
                    break

    def isComment(self, line):
        if re.match(r'^#.*', line):
            return True
        if re.match(r'^\s*$', line):
            return True
        return False

    def updateState(self, line):
        return ERROR, False

    def parseLine(self, line):
        return True

    def emitError(self, lineno, mess):
        sys.stderr.write('Syntax error on line' + str(lineno))
        sys.stderr.write(parser.OUT_DELIM)
        sys.stderr.write('[' + mess + ']\n')


# Represents an edge with label dimensions
class labelEdge(object):

    @classmethod
    def fromString(cls, line):
        inst = cls()
        inst.error = None
        fields = line.split(parser.IN_DELIM)
        if len(fields) != 11:
            inst.error = 'wrong edge syntax'
        else:
            inst.n1    = int          (fields[ 0])
            inst.n2    = int          (fields[ 1])
            inst.p1    = cls.parsePos(fields[ 2])
            inst.w1    = float        (fields[ 3])
            inst.h1    = float        (fields[ 4])
            inst.pm    = cls.parsePos(fields[ 5])
            inst.wm    = float        (fields[ 6])
            inst.hm    = float        (fields[ 7])
            inst.p2    = cls.parsePos(fields[ 8])
            inst.w2    = float        (fields[ 9])
            inst.h2    = float        (fields[10])
        return inst

    @classmethod
    def parsePos(cls, field):
        if field == 'CENTER':
            return edgeLabelPos.CENTER
        elif field == 'CW':
            return edgeLabelPos.CW
        elif field == 'CCW':
            return edgeLabelPos.CCW
        else:
            self.error = 'wrong position token'



    @classmethod
    def fromNodes(cls, n1, n2):
        inst = cls()
        inst.error = None
        inst.n1    = n1
        inst.n2    = n2
        return inst

    @classmethod
    def fromAnother(cls, e):
        inst = cls()
        inst.error = e.error
        inst.n1    = e.n1
        inst.n2    = e.n2
        inst.p1    = e.p1
        inst.w1    = e.w1
        inst.h1    = e.h1
        inst.pm    = e.pm
        inst.wm    = e.wm
        inst.hm    = e.hm
        inst.p2    = e.p2
        inst.w2    = e.w2
        inst.h2    = e.h2
        return inst

    def __init__(self):
        self.error = None
        self.n1    = -1
        self.n2    = -1
        self.p1    = 'CENTER'
        self.w1    = 0.0
        self.h1    = 0.0
        self.pm    = 'CENTER'
        self.wm    = 0.0
        self.hm    = 0.0
        self.p2    = 'CENTER'
        self.w2    = 0.0
        self.h2    = 0.0

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
        else:
            out_str = out_str + str(self.n1) + parser.OUT_DELIM
            out_str = out_str + str(self.n2) + parser.OUT_DELIM
            out_str = out_str + str(self.p1) + parser.OUT_DELIM
            out_str = out_str + str(self.w1) + parser.OUT_DELIM
            out_str = out_str + str(self.h1) + parser.OUT_DELIM
            out_str = out_str + str(self.pm) + parser.OUT_DELIM
            out_str = out_str + str(self.wm) + parser.OUT_DELIM
            out_str = out_str + str(self.hm) + parser.OUT_DELIM
            out_str = out_str + str(self.p2) + parser.OUT_DELIM
            out_str = out_str + str(self.w2) + parser.OUT_DELIM
            out_str = out_str + str(self.h2) + '\n'
        return out_str

    def setLabel1(self, e):
        self.p1 = e.p1
        self.w1 = e.w1
        self.h1 = e.h1

    def setLabelM(self, e):
        self.pm = e.pm
        self.wm = e.wm
        self.hm = e.hm

    def setLabel2(self, e):
        self.p2 = e.p2
        self.w2 = e.w2
        self.h2 = e.h2

# Represents a node with label dimensions
class labelNode:

    @classmethod
    def fromString(cls, line):
        inst = cls()
        inst.error = None
        fields = line.split(parser.IN_DELIM)
        if len(fields) != 3:
            inst.error = 'wrong node syntax'
        else:
            inst.n       = int(fields[ 0])
            inst.w       = int(fields[ 1])
            inst.h       = int(fields[ 2])
            inst.virtual = False
        return inst

    def __init__(self):
        self.n = -1
        self.w = 0.0
        self.h = 0.0

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
        else:
            out_str = out_str + str(self.n) + parser.OUT_DELIM
            out_str = out_str + str(self.w) + parser.OUT_DELIM
            out_str = out_str + str(self.h) + '\n'
        return out_str

# Parses the original input graph and create internal representation of it.
class originalInputParser(parser):

    def __init__(self, filePath):
        self.topNode = None
        self.gaps    = None
        self.nodes   = {}
        self.edges   = {}
        self.nodeNumMax = -1
        super(originalInputParser, self).__init__(filePath)

    def updateState(self, line):
        if self.state == parseState.INIT:
            if line == 'TOP NODE':
                self.state = parseState.IN_TOP_NODE
                return True, None
            else:
                return False, None

        elif self.state == parseState.IN_TOP_NODE:
            if line == 'GAPS':
                self.state = parseState.IN_GAPS
                return True, None
            else:
                return False, None

        elif self.state == parseState.IN_GAPS:
            if line == 'NODES':
                self.state = parseState.IN_NODES
                return True, None
            else:
                return False, None

        elif self.state == parseState.IN_NODES:
            if line == 'EDGES':
                self.state = parseState.IN_EDGES
                return True, None
            else:
                return False, None

        elif self.state == parseState.IN_EDGES:
                return False, None
        else:
            return False, 'wrong state'

    def parseLine(self, line):
        if self.state == parseState.INIT:
            return 'wrong state'

        elif self.state == parseState.IN_TOP_NODE:
            fields = line.split(parser.IN_DELIM)
            if self.topNode:
                return 'Duped top node'
            elif len(fields) != 1:
                return 'Wrong top node syntax'
            else:
                self.topNode = int(fields[0])
                return None

        elif self.state == parseState.IN_GAPS:
            fields = line.split(parser.IN_DELIM)
            if self.gaps:
                return 'Duped gaps'
            elif len(fields) != 2:
                return 'Wrong gaps syntax'
            else:
                self.gaps = (float(fields[0]), float(fields[1]))
                return None

        elif self.state == parseState.IN_NODES:
            n = labelNode.fromString(line)
            if n.error:
                return n.error
            else:
                self.nodes[n.n] = n
                if self.nodeNumMax < n.n:
                    self.nodeNumMax = n.n
                return None

        elif self.state == parseState.IN_EDGES:
            e = labelEdge.fromString(line)
            if e.error:
                return e.error
            else:
                self.edges[(e.n1, e.n2)] = e
                return None
        else:
            return False, 'wrong state'

    def emitForDecomposition(self, filePath):
        with open(filePath, "w") as outs:
            outs.write('NODES\n')
            for n in self.nodes:
                outs.write(str(self.nodes[n].n) + '\n')
            outs.write('\n')
            outs.write('EDGES\n')
            for e in self.edges:
                outs.write(str(e[0]) + ' ' + str(e[1]) + '\n')
            outs.write('\n')

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
            return out_str
        if self.topNode:
            out_str = out_str + 'TOP NODE\n'
            out_str = out_str + str(self.topNode) + '\n'
        out_str = out_str + '\n'
        if self.gaps:
            out_str = out_str + 'GAPS\n'
            out_str = out_str + str(self.gaps[0]) + parser.OUT_DELIM
            out_str = out_str + str(self.gaps[1]) + '\n'
        out_str = out_str + '\n'
        out_str = out_str + 'NODES\n'
        for n in self.nodes:
            out_str = out_str + str(self.nodes[n])
        out_str = out_str + '\n'
        out_str = out_str + 'EDGES\n'
        for e in self.edges:
            out_str = out_str + str(self.edges[e])
        return out_str


class bcTreeParser(parser):

    class cutVertex:

        def __init__(self, line):
            self.error = None
            fields = line.split(parser.IN_DELIM)
            if len(fields) < 4:
                self.error = 'wrong cut vertex syntax'
            else:
                self.cvIndex      = int(fields[ 0])
                self.nodeNum      = int(fields[ 1])
                self.blockIndices = [ int(c) for c in fields[2:] ]

        def __str__(self):
            out_str = ''
            if self.error:
                out_str = out_str + 'ERROR: [' + self.error + ']\n'
            else:
                out_str = out_str + str(self.cvIndex) + parser.OUT_DELIM
                out_str = out_str + str(self.nodeNum)
                for n in self.blockIndices:
                    out_str = out_str + parser.OUT_DELIM
                    out_str = out_str + str(n)
                out_str = out_str + '\n'
            return out_str

    class block:

        def __init__(self):
            self.error = None
            self.blockIndex       = -1
            self.cvNodeNums       = {} # Key:NodeNum, Val:CutVertex Index
            self.ordinaryNodeNums = []
            self.edges            = set()

        def addCV(self, nodeNum, cvIndex):
            self.cvNodeNums[nodeNum] = cvIndex

        def addOrdinaryNum(self, n):
            self.ordinaryNodeNums.append(n)

        def addEdge(self, n1, n2):
            self.edges.add((n1, n2))

        def __str__(self):
            out_str = 'BLOCK_BEGIN\n'
            out_str = out_str + str(self.blockIndex) + '\n'
            out_str = out_str + 'BLOCK_CUT_VERTICES\n'
            for c in self.cvNodeNums:
                out_str = out_str + str(c) + ' ' + str(self.cvNodeNums[c]) + '\n'
            out_str = out_str + 'BLOCK_ORDINARY_VERTICES\n'
            for o in self.ordinaryNodeNums:
                out_str = out_str + str(o) + '\n'
            out_str = out_str + 'BLOCK_EDGES\n'
            for e in self.edges:
                out_str = out_str + str(e[0]) + ' ' + str(e[1]) + '\n'
            out_str = out_str + 'BLOCK_END\n'
            return out_str

        def emitForPlanarization(self, filePath, virtualNodeStart):
            with open(filePath, "w") as outs:
                outs.write('NODES\n')
                for n in self.cvNodeNums:
                    outs.write(str(n) + '\n')
                for n in self.ordinaryNodeNums:
                    outs.write(str(n) + '\n')
                outs.write('\n')
                outs.write('VIRTUAL NODE START\n')
                outs.write(str(virtualNodeStart) + '\n')
                outs.write('EDGES\n')
                for e in self.edges:
                    outs.write(str(e[0]) + ' ' + str(e[1]) + '\n')
                outs.write('\n')

    def __init__(self, filePath):
        self.cutVertices           = {}
        self.blocks                = {}
        self.planarizedBlocks      = {}
        self.blockIndex            = 1
        super(bcTreeParser, self).__init__(filePath)

    def updateState(self, line):
        if self.state == parseState.INIT:
            if line == 'CUT_VERTICES':
                self.state = parseState.IN_CUT_VERTICES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_CUT_VERTICES:
            if line == 'BLOCK_BEGIN':
                self.blockBeingParsed = self.block()
                self.state = parseState.IN_BLOCK_BEGIN

                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_BLOCK_BEGIN:
            if line == 'BLOCK_CUT_VERTICES':
                self.state = parseState.IN_BLOCK_CUT_VERTICES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_BLOCK_CUT_VERTICES:
            if line == 'BLOCK_ORDINARY_VERTICES':
                self.state = parseState.IN_BLOCK_ORDINARY_VERTICES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_BLOCK_ORDINARY_VERTICES:
            if line == 'BLOCK_EDGES':
                self.state = parseState.IN_BLOCK_EDGES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_BLOCK_EDGES:
            if line == 'BLOCK_END':
                self.blocks[self.blockIndex] = self.blockBeingParsed
                self.blockIndex = self.blockIndex + 1
                self.state = parseState.IN_BLOCK_END
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_BLOCK_END:
            if line == 'BLOCK_BEGIN':
                self.blockBeingParsed = self.block()
                self.state = parseState.IN_BLOCK_BEGIN
                return True, None
            else:
                return False, None
        else:
            return False, 'wrong state'

    def parseLine(self, line):
        if self.state == parseState.INIT:
            return 'wrong state'

        elif self.state == parseState.IN_CUT_VERTICES:
            c = self.cutVertex(line)            
            if c.error:
                return c.error
            else:
                self.cutVertices[c.nodeNum] = c
                return None

        elif self.state == parseState.IN_BLOCK_BEGIN:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 1:
                return 'Wrong top block num syntax'
            else:
                self.blockBeingParsed.blockIndex = int(fields[0])
                return None

        elif self.state == parseState.IN_BLOCK_CUT_VERTICES:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 2:
                return 'Wrong top block cut vertex syntax'
            else:
                #                             NodeNum        CVIndex(unused)
                self.blockBeingParsed.addCV( int(fields[0]), int(fields[1]))
                return None

        elif self.state == parseState.IN_BLOCK_ORDINARY_VERTICES:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 1:
                return 'Wrong top block ordinary vertex syntax'
            else:
                self.blockBeingParsed.addOrdinaryNum(int(fields[0]))
                return None

        elif self.state == parseState.IN_BLOCK_EDGES:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 2:
                return 'Wrong top block edge syntax'
            else:
                self.blockBeingParsed.addEdge(int(fields[0]), int(fields[1]))
                return None
        else:
            return False, 'wrong state'

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
            return out_str
        out_str = out_str + 'CUT_VERTICES\n'
        for c in self.cutVertices:
            out_str = out_str + str(self.cutVertices[c])
        for b in self.blocks:
            out_str = out_str + str(self.blocks[b])
        return out_str


class planarizedParser(parser):


    # Represents a chain of edges derived from a single original edge.
    # A chain is made as a result of planarization operation with virtual
    # nodes.
    class edgeChain(object):

        def __init__(self, line):
            self.error = None
            fields = line.split(parser.IN_DELIM)
            if len(fields) < 2:
                self.error = 'wrong edge syntax'
            else:
                self.n1           = int(fields[ 0])
                self.n2           = int(fields[-1])
                self.virtualNodes = [ int(c) for c in fields[1:-1] ]

        def __str__(self):
            out_str = ''
            if self.error:
                out_str = out_str + 'ERROR: [' + self.error + ']\n'
            else:
                out_str = out_str + str(self.n1)
                for v in self.virtualNodes:
                    out_str = out_str + parser.OUT_DELIM + str(v)
                out_str = out_str + parser.OUT_DELIM + str(self.n2)
            return out_str

        def emitForEmbedding(self):
            out_str = ''
            if self.error:
                out_str = out_str + 'ERROR: [' + self.error + ']\n'
            else:
                if len(self.virtualNodes) == 0:
                    out_str = out_str + str(self.n1) + parser.OUT_DELIM 
                    out_str = out_str + str(self.n2) + '\n'
                else:
                    out_str = out_str + str(self.n1) + parser.OUT_DELIM
                    out_str = out_str + str(self.virtualNodes[0]) + '\n'
                    if len(self.virtualNodes) >= 2:
                        for i in range (0, len(self.virtualNodes)-1):
                            out_str = out_str + str(self.virtualNodes[i])
                            out_str = out_str + parser.OUT_DELIM
                            out_str = out_str + str(self.virtualNodes[i+1]) + '\n'
                    out_str = out_str + str(self.virtualNodes[-1]) + parser.OUT_DELIM
                    out_str = out_str + str(self.n2) + '\n'
            return out_str

        def flip(self):
            tmp = self.n1
            self.n1 = self.n2
            self.n2 = tmp
            self.virtualNodes.reverse()

        def generateSplitEdges(self, orgE):
            if len(self.virtualNodes) == 0:
                return [ labelEdge.fromAnother(orgE) ]

            elif len(self.virtualNodes) == 1:
                e1 = labelEdge.fromNodes(self.n1, self.virtualNodes[0])
                e2 = labelEdge.fromNodes(self.virtualNodes[0], self.n2)
                e1.setLabel1(orgE)
                e1.setLabelM(orgE)
                e2.setLabel2(orgE)
                return [e1, e2]

            else:
                eList = []
                e1 = labelEdge.fromNodes(self.n1, self.virtualNodes[0])
                e1.setLabel1(orgE)
                eList.append(e1)
                halfP = len(self.virtualNodes)/2
                for i in range(0,len(self.virtualNodes)-1):
                    e = labelEdge.fromNodes(self.virtualNodes[i],self.virtualNodes[i+1])
                    if i == halfP:
                        e.setLabelM(orgE)
                    eList.append(e)
                el = labelEdge.fromNodes(self.virtualNodes[-1], self.n2)
                el.setLabel2(orgE)
                eList.append(el)
                return eList

    def __init__(self, filePath, orgEdges):
        self.nodes         = set()
        self.virtualNodes  = set()
        self.originalEdges = orgEdges # original Edges with label info
        self.edgeChains    = {}       # edge chains with key (n1,n2)
        self.splitEdges    = {}       # split edges with label info distributed
        self.nodeNumMax    = -1       # holds the highest node num + 1
        super(planarizedParser, self).__init__(filePath)

    def updateState(self, line):
        if self.state == parseState.INIT:
            if line == 'NODES':
                self.state = parseState.IN_NODES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_NODES:
            if line == 'VIRTUAL_NODES':
                self.state = parseState.IN_VIRTUAL_NODES
                return True, None
            elif line == 'EDGES':
                self.state = parseState.IN_EDGES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_VIRTUAL_NODES:
            if line == 'EDGES':
                self.state = parseState.IN_EDGES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_EDGES:
            return False, None
        else:
            return False, 'wrong state'

    def parseLine(self, line):
        if self.state == parseState.INIT:
            return 'wrong state'
        elif self.state == parseState.IN_NODES:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 1:
                return 'Wrong node syntax'
            else:
                val = int(fields[0])
                if self.nodeNumMax < val:
                    self.nodeNumMax = val
                self.nodes.add(val)
                return None
        elif self.state == parseState.IN_VIRTUAL_NODES:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 1:
                return 'Wrong node syntax'
            else:
                val = int(fields[0])
                if self.nodeNumMax < val:
                    self.nodeNumMax = val
                self.virtualNodes.add(val)
                return None
        elif self.state == parseState.IN_EDGES:
            e = self.edgeChain(line)            
            if e.error:
                return e.error
            else:
                if (e.n1, e.n2) not in self.originalEdges:
                    e.flip()
                self.edgeChains[(e.n1, e.n2)] = e
                ses = e.generateSplitEdges(self.originalEdges[(e.n1, e.n2)])
                for se in ses:
                    self.splitEdges[(se.n1, se.n2)] = se
                return None
        else:
            return False, 'wrong state'

    def __str__(self):
        out_str = ''
        out_str = out_str + 'NODES\n'
        for n in self.nodes:
            out_str = out_str + str(n) + '\n'
        out_str = out_str + 'VIRTUAL_NODES\n'
        for n in self.virtualNodes:
            out_str = out_str + str(n) + '\n'
        out_str = out_str + 'EDGES\n'
        for e in self.edgeChains:
            out_str = out_str + str(self.edges[e]) + '\n'
        return out_str

    def emitForEmbedding(self, filePath):
        with open(filePath, "w") as outs:
            outs.write('NODES\n')
            for n in self.nodes:
                outs.write(str(n) + '\n')
            for n in self.virtualNodes:
                outs.write(str(n) + '\n')
            outs.write('\n')
            outs.write('EDGES\n')
            for e in self.edgeChains:
               outs.write(self.edgeChains[e].emitForEmbedding())

 
class embeddingParser(parser):

    class incidence(object):

        def __init__(self, line):
            self.error = None
            fields = line.split(parser.IN_DELIM)
            self.nodeNum   = int(fields[ 0])
            self.incidence = [ int(c) for c in fields[1:] ]

        def __str__(self):
            out_str = ''
            if self.error:
                out_str = out_str + 'ERROR: [' + self.error + ']\n'
            else:
                out_str = out_str + str(self.nodeNum)
                for n in self.incidence:
                    out_str = out_str + parser.OUT_DELIM + str(n)
            return out_str

    class face(object):

        def __init__(self, line, index):
            self.error = None
            fields = line.split(parser.IN_DELIM)
            self.index         = index
            self.incidentNodes = [ int(c) for c in fields[1:] ]

        def __str__(self):
            out_str = ''
            if self.error:
                out_str = out_str + 'ERROR: [' + self.error + ']\n'
            else:
                out_str = out_str + str(self.index)
                for n in self.incidentNodes:
                    out_str = out_str + parser.OUT_DELIM + str(n)
            return out_str

    def __init__(self, filePath, faceIndexStart):
        self.incidences   = {}
        self.faces        = {}
        self.faceIndex    = faceIndexStart
        super(embeddingParser, self).__init__(filePath)

    def updateState(self, line):
        if self.state == parseState.INIT:
            if line == 'INCIDENCES':
                self.state = parseState.IN_INCIDENCES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_INCIDENCES:
            if line == 'FACES':
                self.state = parseState.IN_FACES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_FACES:
            return False, None
        else:
            return False, 'wrong state'

    def parseLine(self, line):
        if self.state == parseState.INIT:
            return 'wrong state'
        elif self.state == parseState.IN_INCIDENCES:
            i = self.incidence(line) 
            if i.error:
                return i.error
            else:
                self.incidences[i.nodeNum] = i
                return None
        elif self.state == parseState.IN_FACES:
            f = self.face(line, self.faceIndex)
            self.faceIndex = self.faceIndex + 1
            if f.error:
                return f.error
            else:
                self.faces[f.index] = f
                return None
        else:
            return False, 'wrong state'

    def __str__(self):
        out_str = ''
        out_str = out_str + 'INCIDENCES\n'
        for n in self.incidences:
            out_str = out_str + str(self.incidences[n]) + '\n'
        out_str = out_str + 'FACES\n'
        for f in self.faces:
            out_str = out_str + str(self.faces[f]) + '\n'
        return out_str

    # Finds a biggest face incident to the given cut vertex.
    def biggestFace(self, cv):
        maxIdx  = -1
        maxSize = -1
        
        for fk in self.faces:
            f= self.faces[fk]
            if cv in f.incidentNodes:
                if len(f.incidentNodes) > maxSize:
                    maxIdx = fk
                    maxSize = len(f.incidentNodes)
        if maxIdx == -1:
            # No face incident to cv found.
            return (-1, -1, -1)

        fMax = self.faces[maxIdx]
        for cvPos in range(0, len(fMax.incidentNodes)):
            if cv == fMax.incidentNodes[cvPos]:
                break;
        n1Pos = cvPos + 1
        if n1Pos >= len(fMax.incidentNodes):
            n1Pos = 0
        n2Pos = cvPos - 1
        if n2Pos < 0:
            n2Pos = len(fMax.incidentNodes)-1
        return (maxIdx, fMax.incidentNodes[n1Pos], fMax.incidentNodes[n2Pos])


class visRepParser(parser):

    class node(object):

        def __init__(self, line):
            self.error = None
            fields = line.split(parser.IN_DELIM)
            self.n      = int(  fields[0])
            self.xLeft  = float(fields[1])
            self.xRight = float(fields[2])
            self.y      = float(fields[3])

        def __str__(self):
            out_str = ''
            out_str = out_str + str(self.n) + ' ' + str(self.xLeft) + ' '
            out_str = out_str + str(self.xRight) + ' ' + str(self.y)
            return out_str

    class edge(object):

        def __init__(self, line):
            self.error = None
            fields = line.split(parser.IN_DELIM)
            self.n1   = int(  fields[0])
            self.n2   = int(  fields[1])
            self.x    = float(fields[2])
            self.y1   = float(fields[3])
            self.y2   = float(fields[4])
            self.fl   =       fields[5]

        def __str__(self):
            out_str = ''
            out_str = out_str + str(self.n1) + ' ' + str(self.n2) + ' '
            out_str = out_str + str(self.x) + ' ' + str(self.y1) + ' '
            out_str = out_str + str(self.y2) + ' ' + self.fl
            return out_str

    def __init__(self, filePath):
        self.nodes  = {}
        self.edges  = {}
        self.width  = 0.0
        self.height = 0.0
        super(visRepParser, self).__init__(filePath)

    def updateState(self, line):
        if self.state == parseState.INIT:
            if line == 'AREA':
                self.state = parseState.IN_AREA
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_AREA:
            if line == 'NODES':
                self.state = parseState.IN_NODES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_NODES:
            if line == 'EDGES':
                self.state = parseState.IN_EDGES
                return True, None
            else:
                return False, None
        elif self.state == parseState.IN_EDGES:
            return False, None
        else:
            return False, 'wrong state'

    def parseLine(self, line):
        if self.state == parseState.INIT:
            return 'wrong state'
        elif self.state == parseState.IN_AREA:
            fields = line.split(parser.IN_DELIM)
            if len(fields) != 2:
                return 'Wrong area syntax'
            else:
                self.width  = float(fields[0])
                self.height = float(fields[1])
                return None
        elif self.state == parseState.IN_NODES:
            n = self.node(line)
            if n.error:
                return n.error
            else:
                self.nodes[n.n] = n
                return None
        elif self.state == parseState.IN_EDGES:
            e = self.edge(line)
            if e.error:
                return e.error
            else:
                self.edges[(e.n1, e.n2)] = e
                return None
        else:
            return False, 'wrong state'

    def __str__(self):
        out_str = ''
        out_str = out_str + 'AREA\n'
        out_str = out_str + str(self.width) + ' ' + str(self.height) + '\n'
        out_str = out_str + '\n'
        out_str = out_str + 'NODES\n'
        for n in self.nodes:
            out_str = out_str + str(self.nodes[n]) + '\n'
        out_str = out_str + '\n'
        out_str = out_str + 'EDGES\n'
        for e in self.edges:
            out_str = out_str + str(self.edges[e]) + '\n'
        return out_str


def emitVisRepInput(filePath, pOrg, pDecomp, planarizedBlocks, incidences, embeddings):
    hGap = pOrg.gaps[0]
    vGap = pOrg.gaps[1]
    with open(filePath, "w") as outs:
        outs.write('GAPS\n')
        outs.write(str(hGap) + ' ' + str(vGap) + '\n')
        outs.write('\n')
        outs.write('NODES\n')
        for n in pOrg.nodes:
            outs.write(str(pOrg.nodes[n]))
        for bk in planarizedBlocks:
            for vn in planarizedBlocks[bk].virtualNodes:
                outs.write(str(vn) + ' ' + str(hGap*2.0) + ' ' + str(vGap*2.0) + '\n')
        outs.write('EDGES\n')
        for bk in planarizedBlocks:
            block = planarizedBlocks[bk]
            for e in block.splitEdges:
                outs.write(str(block.splitEdges[e]))
        outs.write('INCIDENCES\n')
        for i in incidences:
            outs.write(str(i))
            [outs.write(' ' + str(n)) for n in incidences[i]]
            outs.write('\n')    
        outs.write('BLOCKS\n')
        for blockIndex in pDecomp.blocks:
            block = pDecomp.blocks[blockIndex]
            outs.write(str(blockIndex))
            for n in block.cvNodeNums:
                outs.write(' ' + str(n))
            for n in block.ordinaryNodeNums:
                outs.write(' ' + str(n))
            outs.write('\n')       
        outs.write('FACES\n')
        for blockIndex in embeddings:
            emb = embeddings[blockIndex]
            for f in emb.faces:
                outs.write(str(emb.faces[f]) + '\n')
        outs.write('UNIFICATION GROUPS\n')
        ugIdx = 1
        for cvIndex in pDecomp.cutVertices:
            cvInfo = pDecomp.cutVertices[cvIndex]
            outs.write(str(ugIdx) + ' ' + str(cvInfo.nodeNum))
            for blockIndex in cvInfo.blockIndices:
                faceIdx, nCW, nCCW = embeddings[blockIndex].biggestFace(cvInfo.nodeNum)
                outs.write(' ' + str(blockIndex) + ' ' + str(faceIdx) + ' ' + str(nCW) + ' ' + str(nCCW))
            outs.write('\n')
            ugIdx = ugIdx + 1
        outs.write('ROOT\n')
        maxBlockIndex = 0
        maxSum   = 0
        for blockIndex in pDecomp.blocks:
            block = pDecomp.blocks[blockIndex]
            inCVS = False
            for c in block.cvNodeNums:
                if pOrg.topNode == c:
                    inCVS = True
                    break
            if pOrg.topNode in block.cvNodeNums or pOrg.topNode in block.ordinaryNodeNums:
                sum = len(block.cvNodeNums) + len(block.ordinaryNodeNums) + len(block.edges)
                if maxSum < sum:
                    maxSum        = sum
                    maxBlockIndex = blockIndex
        faceIdx, nCW, nCCW = embeddings[maxBlockIndex].biggestFace(pOrg.topNode)

        outs.write(str(maxBlockIndex) + ' ' + str(pOrg.topNode) + ' ' + str(faceIdx) + '\n')



class renderNode:

    def __init__(self, n, w, h, xLeft, xRight, y, virtual):
        self.num     = n
        self.w       = w
        self.h       = h
        self.xLeft   = xLeft
        self.xRight  = xRight
        self.y       = y
        self.virtual = virtual
        self.degree  = 0
        self.xAccum  = 0.0

    def __str__(self):
        return "Node: " + str(self.num) + "\tW: " + str(self.w) + "\tH: " + \
              str(self.h) + "\txLeft: " + str(self.xLeft) + "\txRight: " + \
              str(self.xRight) + "\ty: " + str(self.y) + "\tvirtual: " +\
              str(self.virtual)

    def xMid(self):
        mid = self.xAccum / float(self.degree)
        if (mid - (self.w/2.0)) < self.xLeft:
            return self.xLeft + (self.w/2.0)
        elif (mid + (self.w/2.0)) > self.xRight:
            return self.xRight - (self.w/2.0)
        else:
            return mid

    def drawLabel(self, ax):
        ax.add_patch(
            patches.Rectangle(
                (self.xMid()-self.w/2.0, self.y - self.h/2.0),
                self.w,
                self.h,
                edgecolor="skyblue",
                facecolor="skyblue",
                zorder=1
            )
        )
        ax.text(self.textX(), self.textY(), self.text(), fontsize=10, zorder=2)

    def lineX(self):
        xMid = self.xMid()
        wh   = self.w/2.0
        return [(xMid-wh)/1.0, (xMid+wh)/1.0, (xMid+wh)/1.0, (xMid-wh)/1.0, (xMid-wh)/1.0]

    def lineY(self):
        hh   = self.h/2.0
        return [(self.y+hh)/1.0, (self.y+hh)/1.0, (self.y-hh)/1.0, (self.y-hh)/1.0, (self.y+hh)/1.0 ]

    def textX(self):
        return self.xMid()

    def textY(self):
        return self.y

    def text(self):
        return str(self.num)


class renderEdge:
    def __init__(self, n1, n2, t1, w1, h1, tm, wm, hm, t2, w2, h2, x, y1, y2, fl):
        self.n1 = n1
        self.n2 = n2
        self.t1 = t1
        self.w1 = w1
        self.h1 = h1
        self.tm = tm
        self.wm = wm
        self.hm = hm
        self.t2 = t2
        self.w2 = w2
        self.h2 = h2
        self.x  = x
        self.y1 = y1
        self.y2 = y2
        self.f  = fl

    def __str__(self):
      return "Edge: {" + str(self.n1) + "," + str(self.n2) + "}\t"\
      + str(self.t1) + "\tW: " + str(self.w1) + "\tH: " + str(self.h1) + "\t"\
      + str(self.tm) + "\tW: " + str(self.wm) + "\tH: " + str(self.hm) + "\t"\
      + str(self.t2) + "\tW: " + str(self.w2) + "\tH: " + str(self.h2) + "\t"\
      + "x: " + str(self.x) + "\ty1: " + str(self.y1) + "\ty2: " + str(self.y2) + "\t"\
      + self.f

    def drawLabel1(self, ax, n1):
        ax.add_patch(
            patches.Rectangle(
                (self.labelX1()[3], self.labelY1(n1)[3]),
                self.w1,
                self.h1,
                edgecolor="None",
                facecolor="pink",
                zorder=1
            )
        )
        ax.text(self.textX1(), self.textY1(n1), self.text1(), fontsize=10,zorder=2)

    def drawLabelm(self, ax, n1,n2):
        ax.add_patch(
            patches.Rectangle(
                (self.labelXm()[3], self.labelYm(n1,n2)[3]),
                self.wm,
                self.hm,
                edgecolor="None",
                facecolor="pink",
                zorder=1
            )
        )
        ax.text(self.textXm(), self.textYm(n1,n2), self.textm(), fontsize=10,zorder=2)

    def drawLabel2(self, ax, n2):
        ax.add_patch(
            patches.Rectangle(
                (self.labelX2()[3], self.labelY2(n2)[3]),
                self.w2,
                self.h2,
                edgecolor="None",
                facecolor="pink",
                zorder=1
            )
        )
        ax.text(self.textX2(), self.textY2(n2), self.text2(), fontsize=10,zorder=2)

    def drawLine(self, ax, n1, n2):
        line  = lines.Line2D(self.lineX(n1,n2), self.lineY(n1,n2), color='black', linewidth=2.0, zorder=0)
        ax.add_line(line)

    def labelY1(self, n1):
        if self.y1 > self.y2:
            ry = n1.y - n1.h/2.0;
            h  = self.h1
            return [ry, ry, ry-h, ry-h, ry]
        else:
            ry = n1.y + n1.h/2.0;
            h  = self.h1
            return [ry+h, ry+h, ry, ry, ry+h]

    def labelY2(self, n2):
        if self.y1 < self.y2:
            ry = n2.y - n2.h/2.0;
            h  = self.h2
            return [ry, ry, ry-h, ry-h, ry]
        else:
            ry = n2.y + n2.h/2.0;
            h  = self.h2
            return [ry+h, ry+h, ry, ry, ry+h]

    def labelYm(self, n1, n2):
        if self.y1 > self.y2:
            cy = ((n1.y - n1.h/2.0 - self.h1) + (n2.y + n2.h/2.0 + self.h2))/2.0
        else:
            cy = ((n1.y + n1.h/2.0 + self.h1) + (n2.y - n2.h/2.0 - self.h2))/2.0
        hh = self.hm /2.0
        return [cy + hh, cy + hh, cy - hh, cy - hh, cy + hh]

    def labelX1(self):
        return self.labelX(self.w1, self.t1, self.f)

    def labelXm(self):
        return self.labelX(self.wm, self.tm, self.f)

    def labelX2(self):
        return self.labelX(self.w2, self.t2, self.f)

    def labelX(self, w, t, f):
        hw = w/2.0
        x  = self.x
        if t == 'CENTER':
            return [x - hw, x + hw, x + hw, x - hw, x - hw]
        if f == 'NOTFLIPPED':
            if (t == 'CCW' and self.y1 >  self.y2) or\
               (t == 'CW' and self.y1 <= self.y2):
                return [x, x + w, x + w, x, x]
            else:
                return [x - w, x, x, x - w, x - w]
        else:
            if (t == 'CCW' and self.y1 >  self.y2) or\
               (t == 'CW' and self.y1 <= self.y2):
                return [x - w, x, x, x - w, x - w]
            else:
                return [x, x + w, x + w, x, x]

    def lineX(self, n1, n2):

        return [n1.xMid(), self.x, self.x, n2.xMid()]

    def lineY(self, n1, n2):
        if n1.y > n2.y:
            return [n1.y, n1.y - n1.h/2.0, n2.y + n2.h/2.0, n2.y]
        else:        
            return [n1.y, n1.y + n1.h/2.0, n2.y - n2.h/2.0, n2.y]

    def textX1(self):
        return self.textX(self.w1, self.t1, self.f)

    def textXm(self):
        return self.textX(self.wm, self.tm, self.f)

    def textX2(self):
        return self.textX(self.w2, self.t2, self.f)

    def textX(self, w, t, f):
        hw = w/2.0
        x  = self.x
        if t == 'CENTER':
            return x
        if f == 'NOTFLIPPED':
            if (t == 'CCW' and self.y1 <= self.y2) or (t == 'CW'  and self.y1 > self.y2):
                return x - hw
            else:
                return x + hw
        else:
            if (t == 'CCW' and self.y1 <= self.y2) or (t == 'CW'  and self.y1 > self.y2):
                return x + hw
            else:
                return x - hw

    def textY1(self, n1):
        if self.y1 > self.y2:
            ry = n1.y - n1.h/2.0;
            h  = self.h1/2.0
            return ry-h
        else:
            ry = n1.y + n1.h/2.0;
            h  = self.h1/2.0
            return ry+h

    def textY2(self, n2):
        if self.y2 > self.y1:
            ry = n2.y - n2.h/2.0;
            h  = self.h2/2.0
            return ry-h
        else:
            ry = n2.y + n2.h/2.0;
            h  = self.h2/2.0
            return ry+h

    def textYm(self, n1,n2):
        if self.y1 > self.y2:
            return ((n1.y - n1.h/2.0 - self.h1) + (n2.y + n2.h/2.0 + self.h2))/2.0
        else:
            return ((n1.y + n1.h/2.0 + self.h1) + (n2.y - n2.h/2.0 - self.h2))/2.0
    
    def text1(self):
         return "{" + str(self.n1) + "," + str(self.n2) + "} Side 1"

    def text2(self):
         return "{" + str(self.n1) + "," + str(self.n2) + "} Side 2"

    def textm(self):
         return "{" + str(self.n1) + "," + str(self.n2) + "} Mid"


def constructGeomInfo(pOrg, planarizedBlocks, pVisRep):

    hGap = pOrg.gaps[0]
    vGap = pOrg.gaps[1]

    rNodes = {}
    rEdges = {}

    for nodeNum in pVisRep.nodes:
        nVisRep = pVisRep.nodes[nodeNum]
        if nodeNum in pOrg.nodes:
            nOrg = pOrg.nodes[nodeNum]
            rNodes[nodeNum] = renderNode(nodeNum, nOrg.w, nOrg.h, nVisRep.xLeft, nVisRep.xRight, nVisRep.y, False)
        else:
            # virtual node
            rNodes[nodeNum] = renderNode(nodeNum, hGap*2.0, vGap*2.0, nVisRep.xLeft, nVisRep.xRight, nVisRep.y, True)

    for blockIndex in planarizedBlocks:
        sEdges = planarizedBlocks[blockIndex].splitEdges
        for e in sEdges:
            es = sEdges[e]
            ev = pVisRep.edges[e]
            rEdges[e] = renderEdge(e[0], e[1], es.p1, es.w1, es.h1, es.pm, es.wm, es.hm, es.p2, es.w2, es.h2, ev.x, ev.y1, ev.y2, ev.fl)
            n1 = rNodes[e[0]]
            n2 = rNodes[e[1]]
            n1.xAccum = n1.xAccum + ev.x
            n2.xAccum = n2.xAccum + ev.x
            n1.degree = n1.degree + 1
            n2.degree = n2.degree + 1
    return rNodes, rEdges

def draw(w, h, rNodes, rEdges):
    fig, ax = plt.subplots()
    ax.set_xlim([-10,w+10])
    ax.set_ylim([-10,h+10])

    for e in rEdges:
        edge = rEdges[e]
        n1   = rNodes[e[0]]
        n2   = rNodes[e[1]]
        edge.drawLine(ax,n1,n2)
        if edge.w1 > 0.0 and edge.h1 > 0.0:
            edge.drawLabel1(ax,n1)
        if edge.wm > 0.0 and edge.hm > 0.0:
            edge.drawLabelm(ax,n1,n2)
        if edge.w2 > 0.0 and edge.h2 > 0.0:
            edge.drawLabel2(ax,n2)

    for n in rNodes:
        node = rNodes[n]
        if not node.virtual:
            node.drawLabel(ax);
    plt.show()


def main(argv):

    work_dir = './sample_graph_drawer_tmp/'
    call(['mkdir', '-p', work_dir])
    decompInputFileName  = work_dir + 'decomp_input.txt'
    decompOutputFileName = work_dir + 'decomp_output.txt'
    pOrg = originalInputParser(argv[1])
    pOrg.emitForDecomposition(decompInputFileName)
    call(['decomposer', decompInputFileName, decompOutputFileName])

    nodeMax = pOrg.nodeNumMax
    faceIndexStart = 1
    pDecomp = bcTreeParser(decompOutputFileName)

    planarizedBlocks = {} # Key is the block number
    embeddings       = {} # Key is the block number
    incidences       = {} # Key is the node number

    for blockIndex in pDecomp.blocks:

        block = pDecomp.blocks[blockIndex]
        planarizationInputFileName  = work_dir + 'unplanarized_'     + str(blockIndex) + '.txt'
        planarizationOutputFileName = work_dir + 'planarized_'       + str(blockIndex) + '.txt'
        embeddingInputFileName      = work_dir + 'embedding_input_'  + str(blockIndex) + '.txt'
        embeddingOutputFileName     = work_dir + 'embedding_output_' + str(blockIndex) + '.txt'

        block.emitForPlanarization(planarizationInputFileName, nodeMax+1)
        call(['planarizer', planarizationInputFileName, planarizationOutputFileName])
        pPlanarized = planarizedParser(planarizationOutputFileName, pOrg.edges);
        pPlanarized.emitForEmbedding(embeddingInputFileName)
        call(['biconnected_embedding_finder', 
              embeddingInputFileName, 
              embeddingOutputFileName])
        pEmbedding = embeddingParser(embeddingOutputFileName, faceIndexStart)
        faceIndexStart = pEmbedding.faceIndex
        planarizedBlocks[blockIndex] = pPlanarized
        embeddings[blockIndex]       = pEmbedding
        for nodeNum in pEmbedding.incidences.keys():
            if nodeNum not in incidences:
                incidences[nodeNum] = []
            incidences[nodeNum] = incidences[nodeNum] + pEmbedding.incidences[nodeNum].incidence
        if nodeMax < pPlanarized.nodeNumMax:
            nodeMax = pPlanarized.nodeNumMax

    visRepInputFileName  = work_dir + 'vis_rep_input.txt'
    visRepOutputFileName = work_dir + 'vis_rep_output.txt'
    emitVisRepInput(visRepInputFileName, pOrg, pDecomp, planarizedBlocks, incidences, embeddings)
    call(['vis_rep_finder', visRepInputFileName, visRepOutputFileName])
    pVisRep = visRepParser(visRepOutputFileName)

    rNodes, rEdges = constructGeomInfo(pOrg, planarizedBlocks, pVisRep)
    call(['rm', '-fr', work_dir])     
    draw(pVisRep.width, pVisRep.height, rNodes, rEdges)


if __name__ == "__main__":
    main(sys.argv)
