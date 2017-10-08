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
import math

# Utility class to implement enum equivalent
class enum(set):
    def __getattr__(self, name):
        if name in self:
            return name
        raise AttributeError

# Internal states used by parses
parseState = enum(["INIT", 
                   "ERROR", 
                   "IN_NODES",
                   "IN_VIRTUAL_NODES",
                   "IN_EDGES",
                   "IN_RANKS",
                   "IN_INCIDENCE_LEFT",
                   "IN_INCIDENCE_RIGHT",
                   "END"])

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
        sys.stderr.write('[' + str(mess) + ']\n')


# Represents a node with label dimensions
class node:
    @classmethod
    def fromString(cls, line, v):
        inst = cls()
        inst.error = None
        fields = line.split(parser.IN_DELIM)
        if len(fields) != 1:
            inst.error = 'wrong node syntax'
        else:
            inst.nodeNum = int(fields[ 0])
            inst.isVirtual = v
        return inst

    def __init__(self):
        self.nodeNum =   -1
        self.rank      = -1
        self.pos       = -1
        self.x         = 0.0
        self.y         = 0.0
        self.isVirtual = False

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
        else:
            out_str = out_str + str(self.nodeNum) + '\n'
        return out_str

class edge:

    @classmethod
    def fromString(cls, line):
        inst = cls()
        inst.error = None
        fields = line.split(parser.IN_DELIM)
        if len(fields) < 2:
            inst.error = 'wrong edge syntax'
        else:
            inst.nodeList = [int(f) for f in fields]
        return inst

    def __init__(self):
        self.nodeList = []

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
        else:
            out_str = (' '.join(self.nodeList))  + '\n'
        return out_str

class rank:

    @classmethod
    def fromString(cls, line):
        inst = cls()
        inst.error = None
        fields = line.split(parser.IN_DELIM)
        if len(fields) < 1:
            inst.error = 'wrong rank syntax'
        else:
            inst.positions    = [int(f) for f in fields]
            inst.numPositions = len(fields)
        return inst

    def __init__(self):
        self.positions    = []
        self.numPositions = 0

    def __str__(self):
        out_str = ''
        if self.error:
            out_str = out_str + 'ERROR: [' + self.error + ']\n'
        else:
            out_str = (' '.join(self.positions))  + '\n'
        return out_str


# Parses the output of digraph_arranger
class inputParser(parser):

    def __init__(self, filePath):
        self.nodes    = {}
        self.edges    = []
        self.ranks    = []
        self.maxPos   = 0
        super(inputParser, self).__init__(filePath)

    def updateState(self, line):
        if line == 'NODES':
            self.state = parseState.IN_NODES
            return True, None
        elif line == 'VIRTUAL_NODES':
            self.state = parseState.IN_VIRTUAL_NODES
            return True, None
        elif line == 'EDGES':
            self.state = parseState.IN_EDGES
            return True, None
        elif line == 'RANKS':
            self.state = parseState.IN_RANKS
            return True, None
        elif line == 'INCIDENCE_LEFT':
            self.state = parseState.IN_INCIDENCE_LEFT
            return True, None
        elif line == 'INCIDENCE_RIGHT':
            self.state = parseState.IN_INCIDENCE_RIGHT
            return True, None
        else:
            return False, None

    def parseLine(self, line):
        if self.state == parseState.INIT:
            return 'wrong state'
        elif self.state == parseState.IN_NODES:
            n = node.fromString(line, False)
            if n.error:
                return n.error
            else:
                self.nodes[n.nodeNum] = n
                return None
        elif self.state == parseState.IN_VIRTUAL_NODES:
            n = node.fromString(line, True)
            if n.error:
                return n.error
            else:
                self.nodes[n.nodeNum] = n
                return None
        elif self.state == parseState.IN_EDGES:
            e = edge.fromString(line)
            if e.error:
                return e.error
            else:
                self.edges.append(e)
                return None
        elif self.state == parseState.IN_RANKS:
            r = rank.fromString(line)
            if r.error:
                return e.error
            else:
                self.ranks.append(r)
                return None
        elif self.state == parseState.IN_INCIDENCE_LEFT:
            return None
        elif self.state == parseState.IN_INCIDENCE_RIGHT:
            return None
        else:
            return False, 'wrong state'

    def assignRanksAndPos(self):
        numRanks = len(self.ranks)
        for i in range(0, numRanks):
            rank= self.ranks[i]          

            if self.maxPos < rank.numPositions:
                self.maxPos = rank.numPositions
            for j in range(0, rank.numPositions):
                nodeNum   = rank.positions[j]
                node      = self.nodes[nodeNum]
                node.rank = i
                node.pos  = j
                node.x    = (float(i) + 1.0) / (float(numRanks) + 1.0)
                node.y    = (float(j) + 1.0) / (float(rank.numPositions) + 1.0)

        if self.maxPos > numRanks:
            self.labelRadius = 1.5 / (float(self.maxPos) * 6.0)
        else:
            self.labelRadius = 1.5 / (float(numRanks) * 6.0)
        self.textOffset = (self.labelRadius /4.0)
        
    def drawLabel(self, node, ax):
        if not node.isVirtual:
            circ = plt.Circle((node.x, node.y), self.labelRadius, color='skyblue', fill=False)
            ax.add_artist(circ)
            ax.text(node.x - self.textOffset, node.y - self.textOffset, str(node.nodeNum), fontsize=10, zorder=10)

    def drawLine(self, edge, ax):
        if len(edge.nodeList) > 2:
            for i in range(0,len(edge.nodeList)-2):
                n1 = self.nodes[edge.nodeList[i]]
                n2 = self.nodes[edge.nodeList[i+1]]
                if i == 0:
                    theta = math.atan2(n2.y - n1.y, n2.x - n1.x)
                    diff_x = math.cos(theta) * self.labelRadius
                    diff_y = math.sin(theta) * self.labelRadius
                    line  = lines.Line2D([n1.x + diff_x, n2.x], [n1.y + diff_y, n2.y], linewidth=1.5, zorder=0, color='black')
                else:
                    line  = lines.Line2D([n1.x, n2.x], [n1.y, n2.y], linewidth=1.5, zorder=0, color='black')
                ax.add_line(line)
            na = self.nodes[edge.nodeList[len(edge.nodeList) -2]]
            nb = self.nodes[edge.nodeList[len(edge.nodeList) -1]]
            hLength = 0.4*self.labelRadius
            theta = math.atan2(nb.y - na.y, nb.x - na.x)
            diff_x = nb.x - na.x - math.cos(theta) * (self.labelRadius + hLength)
            diff_y = nb.y - na.y - math.sin(theta) * (self.labelRadius + hLength)
            ax.arrow(na.x, na.y, diff_x, diff_y, head_width=(0.6*self.labelRadius), head_length=hLength, linewidth=1.0, zorder=0)
        else:
            na = self.nodes[edge.nodeList[0]]
            nb = self.nodes[edge.nodeList[1]]
            hLength = 0.4*self.labelRadius
            theta = math.atan2(nb.y - na.y, nb.x - na.x)
            start_x  = na.x + math.cos(theta) * (self.labelRadius)
            start_y  = na.y + math.sin(theta) * (self.labelRadius)
            diff_x = nb.x - na.x - math.cos(theta) * (2.0 * self.labelRadius + hLength)
            diff_y = nb.y - na.y - math.sin(theta) * (2.0 * self.labelRadius + hLength)
            ax.arrow(start_x, start_y, diff_x, diff_y, head_width=(0.6*self.labelRadius), head_length=hLength, linewidth=1.0, zorder=0)


def draw(p):
    fig, ax = plt.subplots()
    ax.set_xlim([0.0,1.0])
    ax.set_ylim([0.0,1.0])

    for n in p.nodes:
        p.drawLabel(p.nodes[n], ax)
    for e in p.edges:
        p.drawLine(e, ax)

    plt.show()


def main(argv):

    arrangedNodesFileName = 'digraph_drawer_tmp_output.txt'
    call(['digraph_arranger', argv[1], arrangedNodesFileName])
    p = inputParser(arrangedNodesFileName)
    p.assignRanksAndPos()
    call(['rm' , '-f', arrangedNodesFileName])
    draw(p)

if __name__ == "__main__":
    main(sys.argv)
