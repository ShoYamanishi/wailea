#ifndef _WAILEA_UNDIRECTED_RAMDOM_PLANAR_GRAPH_GENERATOR_HPP_
#define _WAILEA_UNDIRECTED_RAMDOM_PLANAR_GRAPH_GENERATOR_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/connected_decomposer.hpp"
#include "undirected/bctree.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/random_planar_graph_generator.hpp
 *
 * @brief
 *     
 * @details
 */

namespace Wailea {

namespace Undirected { 

using namespace std;

/** @class ConnectedDecomposer
 *
 *  @brief it takes a simple graph and decompose into connected components.
 *         each component is given in the set of nodes.
 *         1. Mesh M*N
 *         2. Remove edges randomly
 *         3. Remove nodes of degree 2 randomly
 *         4. Find connected, and then biconnected components and emit them.
 *
 */
class RandomPlanarGraphGenerator {

  public:
    /** @brief try to generate planar biconnected graphs.
     *
     *  @param  M (in): Number of rows in the triangular mesh
     *
     *  @param  N (in): Number of columns in the triangular mesh
     *
     *  @param  prop (in): proportion of edges removed [0.0 -1.0]
     *
     *  @param  os (out): the stream to which the generated graphs in text
     *                    representation will be emitted.
     *
     *  @remark steps to generate graphs.
     *   
     *  1. Generate mesh of size (M,N)
     *     
     *     (1,1)-(1,2)-(1,3)-...(1,N)
     *       |\    |\    |\       |
     *       |  \  |  \  |  \     |
     *       |    \|    \|    \   |
     *     (2,1)-(2,2)-(2,3)-...(2,N)
     *       |\    |\    |\       |
     *       |  \  |  \  |  \     |
     *       |    \|    \|    \   |
     *     (3,1)-(3,2)-(3,3)-...(3,N)
     *       .     .     .        .
     *       .     .     .        .
     *       |    \|    \|    \   |
     *     (M,1)-(M,2)-(M,3)-...(M,N)
     *
     *  2. Randomly remove edges from the mesh
     *
     *  3. Randomly remove vertices of degree 2 to avoid exccessive number
     *     of vertices of degree 2. If deg(v) is 2, then v, {a,v}, and {v,b}
     *     are removed, and {a,b} is added.
     *
     *  4. Find connected components.
     *
     *  5. For each connected component, find bi-connected components.
     *
     *  @remark text format of the emitted graph
     *      <Num Nodes><TAB><Num Edges><RTN>
     *      <Node ID 1><RTN>               // Node 1
     *      ...
     *      <Node ID M><RTN>               // Node M
     *      <Node ID><TAB><Node ID><RTN>   // Edge 1
     *      ...
     *      <Node ID><TAB><Node ID><RTN>   // Edge N
     */
    void generateGraphs(
        size_t        M,
        size_t        N,
        float         prop
    );

    inline size_t numGraphs() {return mNumVertices.size();}

    inline size_t numVertices(size_t i) {return mNumVertices[i];}

    inline vector<pair<size_t,size_t> >& edges(size_t i) {return mEdgeSets[i];}

  private:

    void generateTriangularMesh(
        Graph&        G,
        size_t        M,
        size_t        N
    );

    void removeEdgesRandomly(
        Graph& G,
        float  prop
    );

    void removeDeg2NodesRandomly(
        Graph& G,
        long   thresholdPercent,
        long   rangePercent
    );

    void removeDeg2Node(Graph& g, Node& N);

    vector<node_list_it_t> findNonTransitiveDeg2Nodes(Graph& g);

    void createInducedGraph(
        Graph&                  src,
        Graph&                  dst,
        vector<node_list_it_t>& nodes
    );

    void emitGraph(Block& G);
    void printInducedGraph(Graph& G);

    vector<size_t>                        mNumVertices;
    vector<vector<pair<size_t,size_t> > > mEdgeSets;

#ifdef UNIT_TESTS
  friend class RandomPlanarGraphGeneratorTests;
#endif

};

}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_RAMDOM_PLANAR_GRAPH_GENERATOR_HPP_*/

