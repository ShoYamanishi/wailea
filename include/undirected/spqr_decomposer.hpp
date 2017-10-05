#ifndef _WAILEA_UNDIRECTED_SPQRTREE_HPP_
#define _WAILEA_UNDIRECTED_SPQRTREE_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

#include "undirected/base.hpp"
#include "undirected/dfs.hpp"

#include "undirected/spqr_dfs_elements.hpp"
#include "undirected/spqr_tree.hpp"
#include "undirected/spqr_proc1.hpp"
#include "undirected/spqr_proc2.hpp"
#include "undirected/spqr_proc3.hpp"
#include "undirected/spqr_proc4.hpp"

#ifdef UNIT_TESTS
#include "gtest/gtest_prod.h"
#endif

/**
 * @file undirected/spqrtree.hpp
 *
 * @brief triconnected decomposition and SPQR-tree.
 *     
 * @reference:
 * [1] Gutwenger, Carsten; Mutzel Petra (2001), "A linear time implementation
 *     of SPQR trees", Proc. 8th International Symposium on Graph Drawing
 *     (GD 2000), Lecture Nodes in Computer Science 1984, Springer-Verlag,
 *     pp. 77-90, doi:10.1007/3-540-44541-2_8
 *
 * [2] Hopcroft, John; Tarjan, Robert (1973), "Dividing a graph into 
 *     triconnected components', SIAM Journal on Computing 2 (3): 135-158,
 *     doi:10.1137/0202012
 *
 * @details
 *    The main component of this file is TriconnectedDecomposer, which
 *    takes a simple bi-connected graph and generates an SPQR-tree as a result
 *    of the SPQR decomposition.
 *    An SPQR-tree is represented by the class SPQRTree, whcih is a derived 
 *    class of Graph, and which contains SPQRTreeNode and SPQRTreeEdge as its
 *    nodes and edges respectively.
 *
 *    An SPQR-tree can be any of S, P, or R type.
 *    If it is of S type, the underlying graph is a disjoint cycle.
 *    If it is of P type, the underlying graph is a dipole multi-edge graph,
 *    or a bond.
 *    If it is of R type, the underlying graph is a triconnected component.
 *    A minimal R (triconnected) component is  a K4.
 *    A minimal S component is a cycle with 3 edges, or a triangle.
 *    A minimal P component is 3 parralel edges, or 3-bond, 
 *    in which at least two are virtual edges.
 *
 *    If the input graph is empty, the resultant SPQR-tree is empty.
 *    If the input graph is a single isolated node, the resultant SPQR-tree
 *    consists of a single R-node that contains a copy of the isolated node.
 *    If the input graph is neither of the above nor simple bi-connected, 
 *    the behavior is undefined.
 *
 *    The TriconnectedDecomposer uses the DFS-based algorithm by Hopcroft & 
 *    Tarjan, and later revised by Gutwenger & Mutzel.
 *    The DFS algorithm is implemented with DFSExplorer with the following 
 *    subcomponents.
 *    - SPQRStrategy is a derived class of DFSStrategy to implement algorithm
 *      using hook points invoked by DFSExplorer.
 *    - SPQR_DFS_Node and SPQR_DFS_Edge are graph node and edge to be explored
 *      by DFSExplorer and SPQRStrategy. Those two classes hold values used by 
 *      SPQRStrategy such as lowpt1 and lowpt2.
 *      SPQR_DFS_Node and SPQR_DFS_Edge are further decoreated to 
 *      DFSNode<SPQR_DFS_Node,SPQR_DFS_Edge>, and
 *      DFSEdge<SPQR_DFS_Node,SPQR_DFS_Edge>  for DFSExplorer.
 *      DFSNode<> and DFSEdge<> hold necessary data rquired to perform DFS walk
 *      in DFSExplorer. See compbinatorial/undirected/dfs.hpp for details.
 *      Before performing a DFS walk, TriconnectedDecomposer created a copy of 
 *      the original input graph using DFSNode<SPQR_DFS_Node,SPQR_DFS_Edge>,
 *      and DFSEdge<SPQR_DFS_Node,SPQR_DFS_Edge>. The copy is destroyed after
 *      the walk is finished.
 *
 *  @remarks class diagram
 *
 *    TriconnectedDecomposer 
 *                defines->SPQRStrategy
 *                defines->SPQR_DFS_Node
 *                defines->SPQR_DFS_Edge
 *
 *                owns->DFSExplorer<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
 *                        explores->DFSGraph<Graph,SPQR_DFS_Node,SPQR_DFS_Edge>
 *                                  owns->DFSGraph<SPQR_DFS_Node,SPQR_DFS_Edge>
 *                                  owns->DFSGraph<SPQR_DFS_Node,SPQR_DFS_Edge>
 *                owns->SPQRStrategy
 *                        generates->SPQRTree
 *                                      owns->SPQRTreeNode
 *                                              owns->SPQRComponent
 *                                                      owns->SQPRComponentNode
 *                                                      owns->SQPRComponentEdge
 *                                      owns->SPQRTreeEdge
 * 
 * 
 * 
 * @remarks on algorithm.
 *
 * Overview
 *
 * The decomposition algorithm consists of several steps.
 * 1. Forming a palm-tree. This is the 1st DFS.
 * 2. Determine the visiting orders of neighbors at each node.
 *    Two orderes are made: one for Step 3 and the other for Step 4.
 * 3. Calculate numbers for nodes and edges in the palm tree.
 *    This is the 2nd DFS
 * 4. Find triconnected components (R-nodes) and triangles (minimal S-nodes),
 *    and bonds (minimal P-nodes). This is the 3rd DFS.
 * 5. Merge adjacent P-nodes in the tree to make them maximal. Merge adjacent
 *    S-nodes in the tree to make them maximal.
 *       
 * NOTE1: The implementation does not have any recursive function calls to 
 *        privent unpredictable growth of the runtime calling stack.
 *
 * NOTE2: The algorithm details have been slightly changed from the ones above,
 *        as this implementation does not allow multi-edges at any time
 *        including the execution of PathSearch() described in [1].
 *        Hence, handling of the cases for w = PARENT(v), and
 *        NODE(LOWPT1(w))=PARENT(v) are ommitted.
 *
 * The following explains each step.
 *
 * Step 1. Forming a palm-tree. The first step is to form a palm tree.
 *         A DFS exploration gives rise to a palm-tree, in which the edge from
 *         the currently visiting node to an unexplored adjacent node is 
 *         called 'tree arc', and the edge from the current node to an already
 *         visited adjacent node is called 'frond'. A palm-tree is unique up
 *         to the same initial node, and the order in the adjacency list of 
 *         each node.
 *         It also calculates two numbers called lowpt1 and lowpt2 for each
 *         node, which are used by Step 2.
 *
 * Step1(G, s)
 * comment: G is the graph to be explored, and s is the start node.
 * begin
 *
 *     integer n 
 *     comment: n is the last number assigned to a vertex.
 *     procedure DFSProc1(v)
 *     begin
 *         NUM(v) := n
 *         LOWPT1(v) := n
 *         LOWPT2(v) := n
 *         n := n + 1
 *         ND(v) := 1
 *         
 *         comment: ADJ is the adjacency list.
 *         for w in ADJ(v) do
 *         begin
 *             if VISITED((v,w)) = false then
 *             begin
 *                 if NUM(w) = 0 then
 *                 begin
 *                     comment: w is a new vertex
 *                     TYPE((v,w)) := TREE_ARC
 *                     VISITED((v,w)) := true
 *                     DFSProc1(w)
 *                 
 *                     if LOWPT1(w) < LOWPT1(v) then
 *                     begin
 *                         LOWPT2(v) := min{LOWPT1(v), LOWPT2(w)}
 *                         LOWPT1(v) := LOWPT1(w)
 *                     end
 *                     else if LOWPT1(w) = LOWPT1(v) then
 *                         LOWPT2(v) := min{LOWPT2(v), LOWPT2(w)}
 *                     else
 *                         LOWPT2(v) := min{LOWPT2(v), LOWPT1(w)}
 *                     ND(v) := ND(v) + ND(w)
 *                     PARENT(w) := v
 *                 end
 *                 else
 *                 begin
 *                     TYPE((v,w)) := FROND
 *                     if NUM(w) < LOWPT1(v) then
 *                     begin
 *                         LOWPT2(v) := LOWPT1(v)
 *                         LOWPT1(v) := NUM(w)
 *                     end
 *                     else if NUM(w) > LOWPT1(v) then
 *                         LOWPT2(v) := min{LOWPT2(v), NUM(w)}
 *                 end
 *             end
 *         end
 *     end
 *
 *     n := 1
 *     comment: N is the set of nodes in G.
 *     for v in N do
 *         NUM(v) := 0
 *
 *     comment: E is the set of edges in G.
 *     for e in E do
 *         VISITED(e) := false
 *     
 *     comment: s is the start node.
 *     DFSProc1(s)
 *
 * Step 2. make two types of adjacency lists for each node.
 *         
 *         Type 1 is used for Step 3 to calculate the updated node number
 *         lowpt1, lowpt2, and a list called high points for each node.
 *         Type 2 is basically in the same order as Type 1 except for the
 *         positions for the out-going fronds. It is used at Step 4, the 
 *         main algorithm.
 *
 *         The order in each incidence list is by the descending order of the
 *         lowpt1 numbers of adjacent nodes. The ties are broken by lowpt2
 *         and the type of the edge (tree arc or frond).
 * 
 *         NOTE on the roll of ADJ_TYPE_X_BACK.
 *         Those list named _BACK are used to store the fronds.
 *         Each frond appears in two incidence lists: One as an out-going
 *         edge, and the other as an incoming one.
 *         To maintain the same palm-tree as found in Step 1, we want to 
 *         avoid visiting fronds as incoming edges in the node incidence lists.
 *         Those should be visited from the out-going side. Ex. A frond
 *         (w->v) should be visited from w (out-going side), not from v
 *         (in-coming side).
 *         To push back the fronds of incoming edges in the incidence list
 *         The _BACK lists are used to Type 1.
 *         For Type 2, we want to visit the out-going edges after visiting
 *         all the tree arcs (and their descendants). This is required to 
 *         make the high points lists of nodes in the descending order
 *         of the node number.
 *
 *         NOTE on the reason for Gutwenger & Mutzel's modification, which is
 *         to insert  fronds (v,w) between tree arcs (v,x) with 
 *         LOWPT2(x) < v and tree arcs (v,y) with v <= LOWPT2(y), 
 *         is to explore and push (v,w) to ESTACK before exploring (v,y) When
 *         the DFS is coming back from y to v along (v->y), and it has found a
 *         type1 split component with the pair (v,lowpt1(y)), and if 
 *         lowpt1(y) = w, then the frond (v[_>w) will become the top on ESTACK
 *         after popping all the edges in the component. The presence or 
 *         absense of frond (v[_>w), which is an edge between the separation 
 *         pair, will be detected checking the top of ESTACK.
 *
 * Step1(G)
 * comment: calculate phi value for each edge, and make two types of incidence
 *          list of each node based on the phi values. This is used for a DFS
 *          at Step 3 and 4.
 *         
 * begin
 *
 *     for v in N do
 *         ADJ(v) := empty list
 *    
 *     buckets := array of 3|N| + 2 elements
 *
 *     for b in buckets do
 *         b := empty list
 *
 *     for (v,w) in E do
 *     comment: E is the edge set of G
 * 
 *              
 *     begin
 *         if TYPE((v,w)) = FROND then
 *             PHI((v,w)) := 3 * LOWPT1(w) + 1
 *         else if LOWPT2(w) < NUM(v) then
 *             PHI((v,w)) := 3 * LOWPT1(w)
 *         else
 *             PHI((v,w)) := 3 * LOWPT1(w) + 2
 *         buckets[ PHI((v,w)) ].append((v,w))
 * 
 *     end
 *
 *     comment: order the incidence list of each node in the ascening
 *              order of phi value.
 *              The incoming side of the fronds are pushed back to the end
 *              using ADJBACK below to maintain the same frond as the one 
 *              at Step 1.
 *
 *     for b in buckets do
 *         for (v,w) in b do
 *         begin
 *             if TYPE((v,w)) = FROND then
 *             begin
 *                 ADJ_TYPE2_BACK(v).append(w)
 *                 ADJ_TYPE2_BACK(w).append(v)
 *                 if NUM(v) < NUM(w) then
 *                 begin
 *                     ADJ_TYPE1_BACK(v).append(w)
 *                     ADJ_TYPE1_FRONT(w).append(v)
 *                 end
 *                 else
 *                 begin
 *                     ADJ_TYPE1_FRONT(v).append(w)
 *                     ADJ_TYPE1_BACK(w).append(v)
 *                 end
 *             end
 *             else
 *             begin
 *                 ADJ_TYPE1_FRONT(v).append(w)
 *                 ADJ_TYPE1_FRONT(w).append(v)
 *                 ADJ_TYPE2_FRONT(v).append(w)
 *                 ADJ_TYPE2_FRONT(w).append(v)
 *             end
 *         end
 *           
 *     for v in N do
 *     comment: concatenate ADJBACK to the end of ADJ.
 *     begin
 *         ADJ_TYPE1(v) := ADJ_TYPE1_FRONT(v) + ADJ_TYPE1_BACK(v)
 *         ADJ_TYPE2(v) := ADJ_TYPE2_FRONT(v) + ADJ_TYPE2_BACK(v)
 *     end
 * end
 *
 * Step 3. Explore the graph again with the reordered adjacency lists, and
 *         recalculate NUM(v), LOWPT1(v), and LOWPT2(v) for each node. 
 *         NUM(v) is calculated such that NUM(v) < NUM(w) if v->*->w, and
 *         NUM(v) > NUM(w) if u->*->v & u->*->w & v is visited erlier than w.
 *         It also finds START_PATH(e) for each edge, which indicates if the 
 *         edge starts a new path in the DFS exploration or not, and HIGHPTS(v)
 *         for each node, which contains the highest numbered nodes reachable
 *         from v in the reverse orientation of arc and fronds.
 *         The list is in descending order of numbers of nodes.
 *         
 *
 * Step3(G, s)
 * comment: G is the graph to be explored, and s is the same start node.
 * begin
 *
 *     integer m
 *
 *     procedure DFSProc3(v)
 *     begin
 *         NUM(v) := m - ND(v) + 1
 *         LOWPT1(v) := NUM(v)
 *         LOWPT2(v) := NUM(v)
 *
 *         for w in ADJ(v) do
 *         begin
 *             if VISITED((v,w)) = false then
 *             begin
 *
 *                 if NUM(w) = 0 then
 *                 begin
 *                     comment: w is a new vertex and (v,w) is TREE_ARC
 *                     VISITED((v,w)) := true
 *                     DFSProc3(w)
 *                     m := m - 1
 *
 *                     if LOWPT1(w) < LOWPT1(v) then
 *                     begin
 *                         LOWPT2(v) := min{LOWPT1(v), LOWPT2(w)}
 *                         LOWPT1(v) := LOWPT1(w)
 *                     end
 *                     else if LOWPT1(w) = LOWPT1(v) then
 *                         LOWPT2(v) := min{LOWPT2(v), LOWPT2(w)}
 *                     else
 *                         LOWPT2(v) := min{LOWPT2(v), LOWPT1(w)}
 *                     ND(v) := ND(v) + ND(w)
 *                     PARENT(w) := v
 *                 end
 *                 else
 *                 comment: (v,w) is FROND
 *                 begin
 *
 *                     if NUM(w) < LOWPT1(v) then
 *                     begin
 *                         LOWPT2(v) := LOWPT1(v)
 *                         LOWPT1(v) := NUM(w)
 *                     end
 *                     else if NUM(w) > LOWPT1(v) then
 *                         LOWPT2(v) := min{LOWPT2(v), NUM(w)}
 *
 *                     HIGH_PTS(w).append(v)
 *
 *                 end
 *             end
 *         end
 *     end
 *
 *     m := |N(G)|
 *
 *     for v in N(G) do
 *     begin
 *         ADJ(v) := ADJ_TYPE1(v)
 *         HIGH_PTS(v) := empty list
 *         NUM(v) := 0
 *     end
 *
 *     for e in E(G) do
 *     begin
 *         VISITED(e) := false
 *     end
 *     
 *     comment: s is the start node. The same one in Step 1.
 *     DFSProc3(s)
 * end
 *
 * Step 4. The core of the algorithm. Explore the graph again, and find the 
 *         triconnected components, triangles, and 3-bonds.
 *         During the DFS exploration, it finds separation pairs with a 
 *         data structure called TSTACK.
 *
 * Step4(G, s)
 * comment: G is the graph to be explored, and s is the same start node.
 * begin
 *
 *     comment: true as long as the exploration keeps going along the direction
 *              of tree arc and frond.  It turns false once the exploration 
 *              has come back from a node.
 *     boolean outgoing_path
 *
 *     procedure DFSProc4(v)
 *     begin
 *         for w in ADJ(v) do
 *         begin
 *             if TYPE((v,w)) = TREE_ARC then
 *             begin
 *                 boolean start_path
 *
 *                 if outgoing_path = false then
 *                 comment: the first edge of a new path.
 *                 begin
 *                     outgoing_path := true
 *                     start_path := true
 *                     ProcessTStackArc((v,w))
 *                 end
 *                 DFSProc4(w)
 *
 *                 EStack.push((v,w))
 *                 CheckForType2s((v,w))
 *                 CheckForType1((v,w))
 *
 *                 comment: at this point, (v,w) and w may no longer exist.
 *
 *                 if start_path = true then
 *                     TStack.popUntilAndIncludingEOS()
 *
 *                 ProcessTStackUpdateH(v)
 *                 outgoing_path := false
 *             end
 *             else
 *             begin
 *                 if outgoing_path = false then
 *                 comment: the first edge (and the last edge) of a new path.
 *                 begin
 *                     ProcessTStackFrond((v,w))
 *                     outgoing_path := false
 *                 end
 *                 EStack.push((v,w))
 *             end
 *         end 
 *     end
 *    
 *     procedure ProcessTStackArc((v,w))
 *     comment: this is called when a new path starts during the exploration.
 *              this updates the stack elements for the path underneath next
 *              time the exploration goes back to it. The elemensts with 
 *              a > lowpt1(w) are all removed. This is because if the subtree
 *              under w belongs to a type2 split component later, then a must
 *              be lower than lowpt1(w), as any frond from the component must
 *              go above a.
 *     begin
 *         (h,a,b) := TSTACK.top()
 *         num max_h  := NUM(w) + ND(w) -1
 *         num last_b := v
 *         while a > LOWPT1(w) do
 *         begin
 *             TSTACK.pop()
 *             max_h   := max{max_h, h}
 *             last_b  := b
 *             (h,a,b) := TSTACK.top()
 *         end
 *         TSTACK.push((max_h,LOWPT1(w),last_b))
 *         TSTACK.push(EOS)
 *     end
 *     
 *     procedure ProcessTStackFrond((v,w))
 *     comment: this is called when a new path starts during the exploration.
 *              this updates the stack elements for the path underneath next
 *              time the exploration goes back to it. The elemensts with 
 *              a > w are all removed. This is because if the subtree
 *              under w belongs to a type2 split component later, then a must
 *              be lower than w, as any frond from the component must
 *              go above a.
 *     begin
 *         (h,a,b) := TSTACK.top()
 *         num max_h  := v
 *         num last_b := v
 *         while a > w do
 *         begin
 *             TSTACK.pop()
 *             max_h   := max{max_h, h}
 *             last_b  := b
 *             (h,a,b) := TSTACK.top()
 *         end
 *         TSTACK.push((max_h,w,last_b))
 *         TSTACK.push(EOS)
 *     end
 *     
 *     procedure ProcessTStackUpdateH(v)
 *     comment: this is called to expand h on the current stack leftward, 
 *              if v has a frond whose neighbor hpt is higher than the h on 
 *              the stack, and if v is not a or b on the current stack,
 *              then the formation is like hpt > h > b > v > a.
 *              This stack top (h,a,b) is no longer valid, because the frond
 *              whose source node is higher than h is supposed to go down to
 *              a node higher than or equal to b or lower than or equal to a,
 *              but hpt goes to v, which is between a and b.
 *     begin
 *         hpt := HIGH_PTS(v).first_element
 *         (h,a,b) := TSTACK.top()
 *         while a != v AND b != v AND h < NUM(hpt) do
 *         begin
 *             TSTACK.pop()
 *             (h,a,b) := TSTACK.top()
 *         end
 *     end
 *
 *     procedure CheckForType2s((v,w))
 *     comment: this is called to try to find type 2 split components and
 *              triangles.
 *              This is a descructive process to the current edge and the 
 *              adjacent node in DFS.
 *     begin
 *         (h,a,b) := TSTACK.top()
 *         (w,x)   := one of w's edges other edge than (v,w).
 *         while NUM(v) != 1 AND (a = v OR (Deg(w) = 2 AND NUM(w) < NUM(x)) do
 *         begin
 *             (h,a,b) := TSTACK.top()
 *             while a = v AND PARENT(b) = a do
 *             begin
 *                 TSTACK.pop()
 *                 (h,a,b) := TSTACK.top()
 *             end
 *             if Deg(w) = 2 AND NUM(w) < NUM(x)) then
 *                 (v,w) := processUpwardTriangle((v,w))
 *             else
 *                 (v,w) := processSeparationComponentByHAB((v,w))
 *             comment: at this point (v,w) and w may have been updated.
 *         end
 *     end
 *
 *     procedure processUpwardTriangle((v,w))
 *     comment: Split (v,w), (w,x) from G, and add a tree arc (v->x).
 *              NOTE: DFS is currently visiting (v,w), which will be replaced
 *                    with (v,x) in the current exploration.
 *     begin
 *         edges_found := empty list
 *         comment: the two edges popped here are (v,w) and (w,x).
 *
 *         comment: replace_edge replaces the existing edge (first param)
 *                  with a new one (second param), and returns the reference
 *                  to the new edge.
 *         edge_ref e_xv := G.replace_edge(ESTACK.top(), (v->x))
 *         edge_ref e_xv_extra := nil
 *         G.set_virtual(e_xv)
 *         edges_found.append(ESTACK.pop())
 *         G.remove_edge(ESTACK.top())
 *         edges_found.append(ESTACK.pop())
 *
 *         if ESTACK.top() = (x,v) then
 *         begin
 *             G.remove_edge(ESTACK.top())
 *             e_xv_extra := ESTACK.pop()
 *         end
 *         make_tree_components(v, x, edges_found, e_xv, e_xv_extra)
 *         return (v,x)
 *     end
 *
 *
 *     procedure processSeparationComponentByHAB((v,w))
 *     comment: Split (x,y) such that a<=x<=h and a<=y<=h, and add a tree arc
 *              (a->b) where (a,b,h) is the current top on TSTACK.
 *     begin
 *         edges_found := empty list
 *         (h,a,b) := TSTACK.top()
 *         (x,y) := ESTACK.top()
 *         edge_ref e_ab
 *         edge_ref e_ab_extra := nil
 *         while a<=NUM(x)<=h AND a<=NUM(y)<=h do
 *         begin
 *             if (x,y) = (a,b) then
 *             begin
 *                 if VIRTUAL((x,y)) = true then
 *                 begin
 *                     e_ab_pair := PAIR((x,y))
 *                     unlink_virtual_edges((x,y),e_ab_pair)
 *                     e_ab := (x,y)
 *                     e_ab_extra := e_ab_pair
 *                 end
 *                 else
 *                 begin
 *                     e_ab := (x,y)
 *                     G.set_virtual(e_ab)
 *                 end
 *             end 
 *             else if (x,y) = (a,w) then
 *             begin
 *                 comment: replace_edge replaces the existing edge (first
 *                          param) with a new one (second param), and returns
 *                          the reference to the new edge.
 *                 e_ab := G.replace_edge((x->y),(a->b))
 *                 G.set_virtual(e_ab)
 *                 edges_found.append((x,y))
 *             end
 *             else
 *             begin
 *                 G.remove_edge((x,y))
 *                 edges_found.append((x,y))
 *             end
 *         end
 *
 *         make_tree_components(a, b, edges_found, e_ab, e_ab_extra)
 *                                                  
 *         return e_ab
 *     end
 *
 *     procedure make_tree_components(a, b, edges, e_ab, e_ab_extra)
 *     begin
 *         C1 := new_component()
 *         for e in edges do
 *             C1.add_edge(e)
 *         edge_ref e_ab_c1 := C1.make_virtual_edge(a,b)
 *
 *         if e_ab_extra = nil then
 *             linkup_virtual_edges(e_ab,e_ab_c1)
 *         else
 *         begin
 *             comment: C2 is a P node.
 *             C2 := new_component()
 *             C2.add_edge(e_ab_extra)
 *             e1 := C2.make_virtual_edge(a,b)
 *             e2 := C2.make_virtual_edge(a,b)
 *             linkup_virtual_edges(e1,e_ab)
 *             linkup_virtual_edges(e2,e_ab_c1)
 *         end
 *     end
 *
 *     procedure CheckForType1((v,w))
 *     comment: this is called to try to find type 1 split component including
 *              downward triangles (i.e. {(v->w),(w[_>a),(a-*->v)} where "[_>"
 *              denotes a frond).
 *              This is a descructive process to the currentl edge and the 
 *              adjacent node in DFS. If a component has been detected, 
 *              there will no longer be (v,w).
 *     begin
 *         if LOWPT2(w) >= NUM(v) AND LOWPT1(w) < NUM(v) AND
 *            ( NUM(PARENT(v)) != 1 OR PARENT(v) has an unexplored edge) then
 *
 *         comment: The 3rd clause above means if V's parent is not the root,
 *                  or if the parent is the root, but v has at least one
 *                  unexplored edge.
 *         begin
 *         node_ref a := LOWPT1(w)
 *         node_ref b := v
 *         edge_ref e_ab
 *         edge_ref e_ab_extra := nil
 *         edges_found = empty list
 *         (x,y) := ESTACK.top()
 *         while ( NUM(w) <= NUM(x) < NUM(w) + NW(w) ) OR 
 *               ( NUM(w) <= NUM(y) < NUM(w) + NW(w) )   do
 *             edges_found.append((x,y))
 *             if (x,y) = (v,w) AND PARENT(b) != a then
 *             begin
 *                 e_ab := G.replace_edge((x->y),(b[_>a))
 *                 G.set_virtual(e_ab)
 *             end
 *             else
 *                 G.remove_edge((x,y))
 *             ESTACK.pop()
 *             (x,y) := ESTACK.top()
 *         end
 *         if (x,y) = (a,b) then
 *         comment: there is a frond between a and b.
 *         begin
 *             e_ab_extra := (x,y)
 *             G.remove_edge((x,y))
 *         end
 *
 *         if PARENT(b) = a then
 *         comment: the following returns the tree arc (a->b), which
 *                  is under exploration.
 *         begin
 *             e_ab = VISITING_EDGE(a)
 *             if VIRTUAL(e_ab) = true then
 *             begin
 *                 e_ab_extra = PAIR(e_ab)
 *                 unlink_virtual_edges(e_ab,e_ab_extra)
 *             end
 *         end
 *         make_tree_components(a, b, edges_found, e_ab, e_ab_extra)
 *     end
 *
 *     for v in N(G) do
 *     begin
 *         ADJ(v) := ADJ_TYPE2(v)
 *     end
 *
 *     for e in E(G) do
 *     begin
 *         VISITED(e) := false
 *         START_PATH(e) := false
 *     end
 *     
 *     start_path := false
 *     comment: s is the start node. The same one in Step 1.
 *     DFSProc3(s)
 * end
 *        
 */
 
namespace Wailea {

namespace Undirected { 

using namespace std;


/** @class  SPQRDecomposer
 *  @brief  takes a simple biconnected graph and creates an SPQR-tree as a
 *          result of biconnected decomposition.
 *          
 *  @details 
 *      The main top level class that performs the decomposition.
 *      It depends on DFSExplorer, DFSNode, and DFSEdge defined in 
 *      undirected/dfs.hpp.
 *      It takes a Graph object of a simple biconnected graph as its input.
 *      If the input graph is not connected, only the connected component
 *      from the starting node is explored, but the processing time
 *      is still in the order of O(|N|+|E|), where N and E represent the
 *      nodes and edges of the entire input graph.
 *      If the input graph is not simple (i.e. if it has multi-edges and/or 
 *      self-loops), the behavior is undefined.
 *      If the input graph is not biconnected the behavior is undefined.
 *      A minimal biconnected graph is a triangle.
 *      It has a single main member function decompose(), which takes the 
 *      input graph and a starting node, and then generates an SPQR-tree, and
 *      subordinate components contained in the SPQR-tree nodes.
 *     
 *      Complexities:
 *        It uses heap memory in the order of O(|N|+|E|).
 *        It runs in the order of  O(|N|+|E|).
 */
class SPQRDecomposer {

  public:


    /** @brief performs the decomposition from a specific node.
     *
     *  @param   g (in): input graph. It must be simple and biconnected.
     *  @param   s (in): start node for DFS walk.
     *
     *  @return  an SPQR-tree.
     *
     *  @remark on inter-graph links:
     *         - SPQRComponentNode::IGBackwardLink() in SPQR-tree points to the
     *           corresponding Node in g.
     *         - SPQRComponentEdge::IGBackwardLink() in SPQR-tree points to the
     *           corresponding Edge in g.
     *         - Edge::IGForwardLink() in g points to the corresponding 
     *           SPQRComponentEdge of OrdinaryType in SPQR-tree.
     *         - Edge::utility() in g has either of the following values:
     *               SPQRTree::kForwardEdgeTypeSeparatingEdge;
     *               SPQRTree::kForwardEdgeTypeOrdinaryEdge;
     *
     *  @throws bad_alloc()
     *           if there is a memory shortage during a call.
     *
     *  @remark exception safety
     *           If an exception is thrown, all the data structures internally
     *           allocated during a call will be freed.
     *
     *  @remark undefined behavior
     *           If the input graph is not simple, the behavior is undefined.
     *           If the input graph is not biconnected, empty or an isolated
     *           single vertex, the bahaviro is undefined.
     *           the required memory and running time will be still in the 
     *           order of O(|N|+|E|).
     */
    SPQRTree decompose(Graph& g, Node& s);

  private:


    /** @brief   creates a copy of the input graph for DFS exploration.
     *
     *  @param   src (in):  the input graph whose copy is created.
     *  @param   dst (out): the copy graph to which nodes and edges are added.
     *  @param   startNode (in): the start node in the input graph.
     *
     *  @return  the start node in the copied graph.
     */
    node_list_it_t copyInputGraph(
                            Graph& src, Graph& dst, node_list_it_t startNode);


    /** @brief sort the adjacency list of each node based on Phi number
     *         by Gutwenger and Mutzel.
     */
    void sortAdjacency(Graph& g);

    /** @brief calculate mPhi of edge based on its incident mNum, mLowPt1,
     *         and mLowPt2.
     */
    void calculatePhi(SPQR_DFS_Edge&e);


    /** @brief creates an internal copy of the input SPQRTree for DFS 
     *         exploration.
     *
     *  @param tree (in): the input graph whose copy is created.
     *
     *  @param copiedGraph (in): copy of the spqr tree for DFS Proc4.
     */
    void copySPQRTree(SPQRTree& tree, Graph& copiedGraph);


    /** @brief helper to copySPQRTree(). Converts the type of SPQR_TREE_Node to
     *         the same type of SPQR_PROC4_Node.
     */
    SPQR_PROC4_Node::type convertNodeType(node_list_it_t nit);


    /** @brief merges the clusters of S and P types and form a proper 
     *         SPQR tree.
     *
     *  @param  clusters (in): list of tre edges of clusters in a list.
     *
     *  @param  tree (in/out): SPQR tree.
     */
    void mergeSPClusters(vector<vector<edge_list_it_t>>& clusters, 
                                                            SPQRTree& tree);




#ifdef UNIT_TESTS
  friend class SPQRDecomposerTests;
  friend class SPQRProc1Tests;
  friend class SPQRProc2Tests;
  friend class SPQRTreeTests;
  friend class SPQRProc3Tests;
#endif
        
};


}// namespace Undirected

}// namespace Wailea

#endif /*_WAILEA_UNDIRECTED_SPQRTREE_HPP_*/
