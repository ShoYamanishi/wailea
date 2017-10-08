import sys
import fileinput
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib.colors as colors

def parseFile(filename):

    lineno = 0

    X = []
    Y = []

    with open(filename) as fp:
        for line in fp:
            line = line.strip()
            fields = line.split()
            if len(fields)== 3:
                N        = float(fields[0])
                E        = float(fields[1])
                millisec = float(fields[2])
                X.append(N + E)
                Y.append(millisec)
            lineno = lineno+1
    return X, Y


def draw(X, Y, filename):

    ax = plt.gca()
    plt.scatter(X,Y,s=5, lw=0, alpha=0.8)
    ax.set_xscale('log')
    ax.set_yscale('log')
    plt.xlabel('|V|+|E|')
    plt.ylabel('Time')
    #plt.ylim(1.0,3.0)
    #plt.xlim(1.0,1000.0)
    plt.savefig(filename, format='eps', dpi=1000)
#    plt.show()



def main(argv):

    X, Y = parseFile(argv[1])

    draw(X, Y, argv[2])

if __name__ == "__main__":
    main(sys.argv)
