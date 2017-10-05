#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "../src/directed/dfs_di.cpp"

namespace Wailea {

namespace Directed {

/** @brief list of classes with actual template parameter types.
 *         this is needed to let the compiler know what actual
 *         classes will be instantiated.
 *         We could avoid this file by moving all the implementation of 
 *         dfs.cpp to dfs.hpp, but this may cause a code bloat depending
 *         on the compiler.
 */


#ifdef UNIT_TESTS
template class DFSDiExplorer<DiGraph, NumDiNode, DiEdge>;
template class DFSDiStrategy<DiGraph, NumDiNode, DiEdge>;
template class DFSDiGraph   <DiGraph, NumDiNode, DiEdge>;
template class DFSDiNode    <DiGraph, NumDiNode, DiEdge>;
template class DFSDiEdge    <DiGraph, NumDiNode, DiEdge>;
#else
template class DFSDiExplorer<DiGraph, DiNode, DiEdge>;
template class DFSDiStrategy<DiGraph, DiNode, DiEdge>;
template class DFSDiGraph   <DiGraph, DiNode, DiEdge>;
template class DFSDiNode    <DiGraph, DiNode, DiEdge>;
template class DFSDiEdge    <DiGraph, DiNode, DiEdge>;
#endif

}// namespace Directed

}// namespace Wailea



