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
    H = []

    with open(filename) as fp:
        for line in fp:
            line = line.strip()
            fields = line.split()
            if len(fields)== 4:
                N     = float(fields[0])
                E     = float(fields[1])
                Total = float(fields[2])
                Fail  = float(fields[3])
                if N > 0.0 and Total > 0.0 :
                    X.append(N)
                    Y.append(E/N)
                    H.append(Fail/Total)
            lineno = lineno+1
    return X, Y, H


def draw(X, Y, H, filename):

    jet_map = plt.get_cmap('jet')
    trunc_range = [cm.jet(i) for i in xrange(20, 235)]
    new_map = colors.LinearSegmentedColormap.from_list('new_map', trunc_range, N=215)
    ax = plt.gca()
    plt.scatter(X,Y,c=H,s=5, lw=0, alpha=0.8, cmap=new_map)
    ax.set_xscale('log')
    plt.xlabel('|V|')
    plt.ylabel('|E|/|V|')
    plt.ylim(1.0,3.0)
    #plt.xlim(1.0,1000.0)
    plt.colorbar()
    plt.savefig(filename, format='eps', dpi=1000)
    #plt.show()



def main(argv):

    X, Y, H = parseFile(argv[1])

    draw(X, Y, H, argv[2])

if __name__ == "__main__":
    main(sys.argv)
