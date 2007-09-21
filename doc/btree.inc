@c btree.inc --
@c 
@c Part of: Useless Containers Library
@c Contents: documentation for the "UCL_BTree" container
@c Date: Mon Nov 25, 2002
@c 
@c Abstract
@c 
@c 
@c 
@c Copyright (c) 2002, 2003 Marco Maggi
@c 
@c This library is free software;  you can redistribute it and/or modify
@c it  under the  terms  of the  GNU  Lesser General  Public License  as
@c published by the Free Software  Foundation; either version 2.1 of the
@c License, or (at your option) any later version.
@c 
@c This library is  distributed in the hope that it  will be useful, but
@c WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
@c MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
@c Lesser General Public License for more details.
@c 
@c You  should have received  a copy  of the  GNU Lesser  General Public
@c License along with  this library; if not, write  to the Free Software
@c Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
@c USA
@c 
@c $Id: btree.inc,v 1.1.1.10 2003/03/01 10:04:22 marco Exp marco $


@c ------------------------------------------------------------
@node BTree
@section The binary tree container


@cindex Binary tree container
@cindex BTree container


@noindent
The @UCL{} btree container provides an implementation of a binary tree.


@deftp {Struct Typedef} ucl_btree_node_t
The data type of the nodes. Nodes must be allocated and freed by the
user code and a pointer to them passed as arguments to the @UCL{} btree
functions: that way the container module doesn't have to deal with
memory allocation and the user code can implement a memory allocator.
@end deftp

The btree container is built as a chain of structures; each link has a
brother, a son and a parent.

@example
                 -----    bro     -----
                | no1 |--------->| no2 |
                 ----- <--------- -----
                 |  ^     dad
             son |  | dad
                 v  |
                 ----- 
                | no3 |
                 ----- 
@end example

@noindent
each node data structure is just a collection of pointers used to keep
trace of the tree hierarchy. There's no data field: to use a tree to
store data we have to include the btree node structure in another
structure; that's what the @UCL{} map container does.

Each of the three pointers in the structure can be @code{NULL}: that
means that the node has no parent, brother or son. Functions and macros
are provided: to append nodes to other nodes; to extract nodes from the
tree; to traverse the hierarchy and to find particularly interesting
nodes.


@menu
* BTree Creation::              Building btree hierarchies.
* BTree Inspection::            Accessing nodes.
* BTree Removing::              Removing elements from a btree.
* BTree Find::                  Finding special nodes.
* BTree Iteration::             Iterations in a btree hierarchy.
@end menu

@c ------------------------------------------------------------
@node BTree Creation
@subsection Building btrees hierarchies


@deftypefun void ucl_btree_constructor (ucl_btree_node_t *@var{this}, ucl_btree_node_t *@var{dadPtr})
Initialises an already allocated btree structure referenced by
@var{this}. @var{dadPtr} is a pointer to the parent node of @var{this},
or @code{NULL} if @var{this} has no parent node.

We can use this function to initialise a new node and append it to an
existing btree in a single step.
@end deftypefun


@defmac ucl_btree_init (UCL_BTree_Node *@var{this})
Resets all the node fields to NULL. It's a wrapper for the
@code{memset()} function.
@end defmac


@defmac ucl_btree_destructor (UCL_BTree_Node *@var{this})
Just a placeholder for a destructor. It's defined to the empty string.
@end defmac


@defmac ucl_btree_setdad (UCL_BTree_Node *@var{this}, UCL_BTree_Node *@var{dad})
Select a new parent node for @var{this}.
@end defmac


@defmac ucl_btree_setbro (UCL_BTree_Node *@var{this}, UCL_BTree_Node *@var{bro})
Select a new brother node for @var{this}.
@end defmac


@defmac ucl_btree_setson (UCL_BTree_Node *@var{this}, UCL_BTree_Node *@var{son})
Select a new child node for @var{this}.
@end defmac


@defmac ucl_btree_dadson (UCL_BTree_Node *@var{dad}, UCL_BTree_Node *@var{son})
Links @var{dad} and @var{son} to be the parent and the son respectively.
@end defmac


@defmac ucl_btree_dadbro (UCL_BTree_Node *@var{dad}, UCL_BTree_Node *@var{bro})
Links @var{dad} and @var{bro} to be the parent and the bro respectively.
@end defmac


Example of btree creation (let's ignore memory allocation errors for
simplicity): we want @code{a} to be the root node; @code{b} to be the
son of @code{a}; @code{c} to be the brother of @code{a}; @code{d} to be
the son of @code{c}.

@example
UCL_BTree_Node *a, *b, *c, *d;

a = malloc(sizeof(UCL_BTree_Node));
b = malloc(sizeof(UCL_BTree_Node));
c = malloc(sizeof(UCL_BTree_Node));
d = malloc(sizeof(UCL_BTree_Node));

ucl_btree_constructor(a, NULL);

ucl_btree_constructor(b, a);
ucl_btree_setson(a, b);

ucl_btree_constructor(c, a);
ucl_btree_setbro(a, c);

ucl_btree_constructor(d, c);
ucl_btree_setson(c, d);
@end example

@noindent
now the hierarchy is:

@example
       ---  bro  ---
      | a |---->| c |
       ---       ---
        |         |
    son v     son v
       ---       ---
      | b |     | d |
       ---       ---
@end example

@noindent
the following expressions are true:

@example
(a == ucl_btree_getdad(b)) && (b == ucl_btree_getson(a))
(a == ucl_btree_getdad(c)) && (c == ucl_btree_getbro(a))
(c == ucl_btree_getdad(d)) && (d == ucl_btree_getson(c))
ucl_btree_getdad(a) == NULL
ucl_btree_getbro(c) == NULL
(ucl_btree_getson(b) == NULL) && (ucl_btree_getbro(b) == NULL)
(ucl_btree_getson(d) == NULL) && (ucl_btree_getbro(d) == NULL)
@end example

@c ------------------------------------------------------------
@node BTree Inspection
@subsection Accessing nodes


@defmac ucl_btree_getdad (UCL_BTree_Node *@var{this})
Returns a pointer to the parent of @var{this} or @code{NULL} if the node
has no parent.
@end defmac


@defmac ucl_btree_getbro (UCL_BTree_Node *@var{this})
Returns a pointer to the brother of @var{this} or @code{NULL} if the
node has no brother.
@end defmac


@defmac ucl_btree_getson (UCL_BTree_Node *@var{this})
Returns a pointer to the son of @var{this} or @code{NULL} if the node
has no son.
@end defmac

@c ------------------------------------------------------------
@node BTree Removing
@subsection Removing elements from a btree


@noindent
It's a matter of setting pointers to @code{NULL}. Care must be taken not
to loose reference to subtrees.

We just use the @code{ucl_btree_setdad()}, @code{ucl_btree_setbro()} and
@code{ucl_btree_setson()} macros.


Example: detach a subtree. The brother of the node @code{a} is detached
and made an independent tree.

@example
UCL_BTree_Node *   a;
UCL_BTree_Node *   b;

a = ...;
b = ucl_btree_getbro(a);

ucl_btree_setbro(a, NULL);
ucl_btree_setdad(b, NULL);
@end example


@c ------------------------------------------------------------
@node BTree Find
@subsection Finding special nodes



@deftypefun {UCL_BTree_Node *} ucl_btree_find_leftmost (UCL_BTree_Node *@var{this})
Find the leftmost node in the subtree of the supplied node. To do this,
we traverse the tree choosing always the son of the current node.

Example:

@example   
      5-------10----12
      |        |     |
      1--3--4  7--9 11
         |     |  |
         2     6  8
@end example

@noindent
starting from @code{5} the selected node is @code{1}, starting from
@code{10} the selected node is @code{6}.

Returns a pointer to the leftmost node in the @var{this} sub--hierarchy
or to @var{this} itself if it has no son.
@end deftypefun


@deftypefun {UCL_BTree_Node *} ucl_btree_find_rightmost (UCL_BTree_Node *@var{this})
Find the rightmost node in the subtree of the supplied node. To do this,
we traverse the tree choosing always the brother of the current node.

Example:

@example   
      5-------10----12
      |        |     |
      1--3--4  7--9 11
         |     |  |
         2     6  8
@end example

@noindent
starting from @code{5} the selected node is @code{12}, starting from
@code{7} the selected node is @code{9}.

Returns a pointer to the rightmost node in the @var{this} sub--hierarchy
or to @var{this} itself if it has no brother.
@end deftypefun


@deftypefun {UCL_BTree_Node *} ucl_btree_find_deepest_son (UCL_BTree_Node *@var{this})
Find the deepest leftmost son in a subtree. This is different from
@code{ucl_btree_find_leftmost()}.

Example:

@example   
      5-------10----12
      |        |     |
      1--3--4  7--9 11
         |     |  |
         2     6  8
@end example

@noindent
starting from @code{5} the selected node is @code{2}, starting from
@code{10} the selected node is @code{6}.

Returns a pointer to the deepest son in the @var{this} sub--hierarchy,
or @var{this} itself if it has no son.
@end deftypefun


@c ------------------------------------------------------------
@node BTree Iteration
@subsection Iterations in a btree hierarchy


@menu
* BTree Inorder Iteration::     Inorder iteration.
* BTree Preorder Iteration::    Preorder iteration.
* BTree Postorder Iteration::   Postorder iteration.
* BTree Level Iteration::       Level order iteration.
@end menu


@c ------------------------------------------------------------
@node BTree Inorder Iteration
@subsubsection Inorder iteration


@deftypefun {UCL_BTree_Node *} ucl_btree_step_inorder (UCL_BTree_Node *@var{this})
Advance an inorder iteration.  Inorder iteration: visit all the nodes
from the leftmost to the rightmost. Given a node in a btree, this
function performs a single step.  Returns a pointer to the next node in
the iteration, or @code{NULL} if the iteration is over.

Example:
   
@example
      5-------10----12
      |        |     |
      1--3--4  7--9 11
      |  |     |  |
      0  2     6  8
@end example

@noindent
the order is: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12.

To perform a complete iteration, we have to start from the leftmost node
(@code{0} in the picture), already visited, and begin from
there. Example:

@example
UCL_BTree_Node *        cur;

cur     = select_a_btree(...);
cur     = ucl_btree_find_leftmost(cur);

while (cur != NULL);
  @{
    makes_use_of_node(cur);
    cur = ucl_btree_step_inorder(cur);
  @}
@end example

To restrict the iteration to a subtree of a tree or to a range of nodes
in a tree, we have to select the first and last nodes and check when the
iterator reaches the last.

Example of subtree restriction:

@example
UCL_BTree_Node *        cur, end;

cur     = select_a_node(...);
end     = ucl_btree_find_rightmost(cur);

cur     = ucl_btree_find_leftmost(cur);
makes_use_of_node(cur);

while (cur != end)
  @{
    cur = ucl_btree_step_inorder(cur);
    makes_use_of_node(cur);
  @}
@end example

@code{cur} can't be @code{NULL} because @code{end} is in the subtree of
the top node; this code should work even if: @code{cur == end}.

Example of range restriction:

@example
UCL_BTree_Node *	root, cur, end;

root	= select_a_node(...);
cur	= select_first(root, ...);
end	= ucl_btree_find_rightmost(root);

makes_use_of_node(cur);

while (cur != end)
  @{
    cur = ucl_btree_step_inorder(cur);
    makes_use_of_node(cur);
  @}
@end example

@code{cur} can't be @code{NULL} since we selected the first and last
nodes in a subtree; this code should work even if: @code{root == cur ==
end}.
@end deftypefun



@deftypefun {UCL_BTree_Node *} ucl_btree_step_inorder_backward (UCL_BTree_Node *@var{this})
Advance a backward inorder iteration.  Backward inorder iteration: visit
all the nodes from the rightmost to the leftmost.  Given a node in a
btree, this function performs a single step. Returns a pointer to the
next node in the iteration, or @code{NULL} if the iteration is over.

Example:

@example   
      5-------10----12
      |        |     |
      1--3--4  7--9 11
      |  |     |  |
      0  2     6  8
@end example

@noindent
the order is: 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.

To perform a complete iteration, we have to start from the rightmost
node (@code{12} in the picture), already visited, and begin from
there. Example:

@example
UCL_BTree_Node *        cur;

cur     = select_a_btree(...);
cur     = ucl_btree_find_rightmost(cur);

while (cur != NULL);
  @{
    makes_use_of_node(cur);
    cur = ucl_btree_step_inorder_backward(cur);
  @}
@end example

To restrict the iteration to a subtree of a tree or to a range of nodes
in a tree, we have to select the first and last nodes and check when the
iterator reaches the last.

Example of subtree restriction:

@example
UCL_BTree_Node *        cur, end;

cur     = select_a_node(...);
end     = ucl_btree_find_leftmost(cur);
cur     = ucl_btree_find_rightmost(cur);

makes_use_of_node(cur);

while (cur != end)
  @{
    cur = ucl_btree_step_inorder_backward(cur);
    makes_use_of_node(cur);
  @}
@end example

@noindent
@code{cur} can't be @code{NULL} because @code{end} is in the subtree of
the top node; this code should work even if: @code{cur == end}.

Example of range restriction:

@example
UCL_BTree_Node *        root, cur, end;

root    = select_a_node(...);
cur     = select_first(root, ...);
end     = ucl_btree_find_leftmost(root);

makes_use_of_node(cur);

while (cur != end)
  @{
    cur = ucl_btree_step_inorder_backward(cur);
    makes_use_of_node(cur);
  @}
@end example

@noindent
@code{cur} can't be @code{NULL} since we selected the first and last
nodes in a subtree; this code should work even if: @code{root == cur ==
end}.
@end deftypefun


@c ------------------------------------------------------------
@node BTree Preorder Iteration
@subsubsection Preorder iteration


@deftypefun {UCL_BTree_Node *} ucl_btree_step_preorder (UCL_BTree_Node *@var{this})
Advance a preorder iteration.  Preorder iteration: visit the current
node then the son then the brother.  Given a node in a btree this
function performs a single step.  Returns a pointer to the next node in
the preorder iteration, or @code{NULL} if the iteration is over.

Example:

@example
      5-------10----12
      |        |     |
      1--3--4  7--9 11
      |  |     |  |
      0  2     6  8
@end example

@noindent  
the order is: 5, 1, 0, 3, 2, 4, 10, 7, 6, 9, 8, 12, 11. It's a worm that
always turns ``right''.

To perform a complete iteration, we have to start from the top node of
the tree (@code{5} in the example), already visited, and begin from
there. Example:

@example
UCL_BTree_Node *        cur;

cur     = select_a_btree(...);

while (cur != NULL);
  @{
    makes_use_of_node(cur);
    cur = ucl_btree_step_pre(cur);
  @}
@end example

To restrict the iteration to a subtree of a tree, we have to check when
the iterator reaches it the top node. Example:

@example
UCL_BTree_Node *        cur, end;

end = cur = select_a_node(...);

makes_use_of_node(cur);
cur = ucl_btree_step_preorder(cur);

while (cur != end);
  @{
    makes_use_of_node(cur);
    cur = ucl_btree_step_preorder(cur);
  @}
@end example
@end deftypefun

@c ------------------------------------------------------------
@node BTree Postorder Iteration
@subsubsection Postorder iteration


@deftypefun {UCL_BTree_Node *} ucl_btree_step_postorder (UCL_BTree_Node *@var{this})
Advance a postorder iteration.  Postorder iteration: visit the son, then
the brother, then the parent node.  Returns a pointer to the next node
in the postorder iteration, or @code{NULL} if the iteration is over.

Example:

@example
      5-------10----12
      |        |     |
      1--3--4  7--9 11
         |     |  |
         2     6  8
@end example  

@noindent
the order is: 2, 4, 3, 1, 6, 8, 9, 7, 11, 12, 10, 5.

To perform a complete iteration, we have to select the deepest leftmost
son in the tree (@code{2} in the example) and begin from there. Example:

@example
UCL_BTree_Node *        cur;

cur     = select_a_node(...);
cur     = ucl_btree_find_deepest_son(cur);

while (cur != NULL);
  @{
    makes_use_of_node(cur);
    cur = ucl_btree_step_postorder(cur);
  @}
@end example

To restrict the iteration to a subtree of a tree, we have to check when
the iterator reaches the top node. Example:

@example
UCL_BTree_Node *        cur, end;

cur = end = select_a_node(...);

makes_use_of_node(cur);
cur = ucl_btree_step_postorder(cur);

while (cur != end);
  @{
    cur = ucl_btree_step_postorder(cur);
    makes_use_of_node(cur);
  @}
@end example

@noindent
@code{cur} can't be null in the loop.
@end deftypefun


@c ------------------------------------------------------------
@node BTree Level Iteration
@subsubsection Level order iteration


@deftypefun {UCL_BTree_Node *} ucl_btree_step_levelorder (UCL_BTree_Node *@var{this})
Advance a level--order iteration.  Level order iteration: visit the tree
level by level.  Returns a pointer to the next node in the levelorder
iteration, or @code{NULL} if the iteration is over.

Example:

@example  
      5-------10----12
      |        |     |
      1--3--4  7--9 11
      |  |     |  |
      0  2     6  8
@end example

@noindent
the order is: 5, 1, 10, 0, 3, 7, 12, 2, 4, 6, 9, 11, 8

We need a moving cursor that always ``turns right'' keeping the count of
the level.

To perform a complete iteration, we just call this function until it
returns @code{NULL}. Example:

@example
UCL_BTree_Node *        cur;

cur     = select_a_btree(...);

while (cur != NULL);
  @{
    makes_use_of_node(cur);
    cur = ucl_btree_step_levelorder(cur);
  @}
@end example
@end deftypefun




@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
