@c ------------------------------------------------------------
@node Tree
@section The tree structure


@noindent
The tree container is basically different from the others in the
@acronym{UCL}.  There is no ``base struct'', like @code{ucl_list_t} and
@code{ucl_map_t}, that holds fundamental data and that can be used to
``keep a grab'' on the structure itself.

Instead, the tree is just a collection of nodes linked together, and we
control the struct keeping a pointer to one of the nodes.  Which one
doesn't matter. Nothing prevents us from storing somewhere a pointer to
the ``root node'', but this is not required.

At any time we must take care of holding a pointer to at least one tree
node; if we loose it, we will not be able to free the struct. Keeping
two or more references to nodes is possible but dangerous: if we delete
a subtree we may end up with a pointer to a memory block already freed.

The referenced node is called ``current''. To handle the tree we use a
``walking'' pattern. We can add/remove nodes relative to the ``current'' and
move step by step from the ``current'' to adjacent nodes.

Care must be taken to distinguish between the node's classification from
the ``logic'' point of view and the ``implementation'' point of view. A
tree is implemented as a binary tree.  Here we talk about the logic
point of view; so: the ``dad'' of a node is the uplevel node in the
hierarchy, the ``brother'' of a node is in the same level of the
``current'' node.


@deftp {Struct Typedef} ucl_tree_node_t
The type of the tree nodes.
@end deftp


@menu
* Tree Implementation::         How it's done.
* Tree Creation::               Creating tree nodes.
* Tree Testing::                Testing relationships between nodes.
* Tree Relatives::              Accessing or setting the relatives of
                                a node.
* Tree Adding::                 Adding elements to a tree.
* Tree Removing::               Removing elements from a tree.
* Tree Iterators::              Traversing a tree.
@end menu


@c ------------------------------------------------------------
@node Tree Implementation
@subsection How it's done


@noindent
The implementation is a binary tree in which: the son of the node A is
the first child of the node A, the brother of the child is the second
son of the node A, and so on.

@example
   -----
  | dad |
   -----
    ^ |son
dad | v
   -----  bro   -----  bro   -----  bro   -----  bro
  | 1st |----->| 2nd |----->| 3rd |----->| 4th |----->NULL
   -----        -----        -----        -----
    |            |
    v son        v son
   -----        -----
  | nep |      |     |
   -----        -----
@end example

Pointers condition meaning:

@table @code
@item node.broPtr == NULL
the node has no brothers, so it's the last brother between the children
of its father;

@item node.sonPtr == NULL
the node has no children;

@item node.dadPtr == NULL
the node is the root node of a tree;

@item A.dadPtr == B && B.sonPtr == A
A is the first between the children of node B;

@item A.dadPtr == B && B.broPtr == A 
A and B are brothers, and children of the same parent node.
@end table


@c ------------------------------------------------------------
@node Tree Creation
@subsection Creating tree nodes


@noindent
The tree node structures must be allocated by the user code, none of the
@acronym{UCL} tree methods will invoke memory allocation/release
functions.


@defmac ucl_tree_constructor (ucl_tree_node_t * @var{nodPtr})
A wrapper for the @code{memset()} function. Initialises to zero all the
members of the structure referenced by @var{nodPtr}. This is recommended
before inserting a node into a tree.
@end defmac


@c ------------------------------------------------------------
@node Tree Testing
@subsection Testing relationships between nodes


@deftypefun int ucl_tree_is_dad (ucl_tree_node_t *@var{dadPtr}, ucl_tree_node_t *@var{cldPtr})
Returns true if the node referenced by @var{dadPtr} is the father of the
node referenced by @var{cldPtr}, otherwise returns false.
@end deftypefun


@deftypefun int ucl_tree_is_bro (ucl_tree_node_t *@var{nodPtr}, ucl_tree_node_t *@var{broPtr})
Returns true if the node referenced by @var{nodPtr} is a brother of the
node referenced by @var{broPtr}, otherwise returns false.
@end deftypefun


@deftypefun int	ucl_tree_has_dad (ucl_tree_node_t *@var{this})
Returns true if the node referenced by @var{this} has a parent,
otherwise returns false.
@end deftypefun


@deftypefun int	ucl_tree_has_prev (ucl_tree_node_t *@var{this})
Returns true if the node referenced by @var{this} has a brother to the
left, otherwise returns false.
@end deftypefun


@defmac ucl_tree_has_next (ucl_tree_node_t *@var{this})
Returns true (a non--@code{NULL} pointer) if the node referenced by
@var{this} has a brother to the right, otherwise returns false.
@end defmac


@defmac ucl_tree_has_son (ucl_tree_node_t *@var{this})
Returns true (a non--@code{NULL} pointer) if the node referenced by
@var{this} has a son, otherwise returns false.
@end defmac


@c ------------------------------------------------------------
@node Tree Relatives
@subsection Accessing or setting the relatives of a node


@deftypefun {ucl_tree_node_t *} ucl_tree_get_dad (ucl_tree_node_t *@var{this})
Returns a pointer to the father of the node referenced by @var{this}; if
the node has no parent, it returns @code{NULL}.
@end deftypefun


@deftypefun {ucl_tree_node_t *} ucl_tree_get_prev (ucl_tree_node_t *@var{this})
Returns a pointer to the left brother of the node referenced by
@var{this}; if the node has no left brother, it returns @code{NULL}.
@end deftypefun


@defmac ucl_tree_get_next (ucl_tree_node_t *@var{this})
Returns a pointer to the right brother of the node referenced by
@var{this}; if the node has no right brother, it returns @code{NULL}.
@end defmac


@defmac ucl_tree_get_son (ucl_tree_node_t *@var{this})
Returns a pointer to the son of the node referenced by @var{this}; if
the node has no child, returns @code{NULL}
@end defmac


@deftypefun {ucl_tree_node_t *} ucl_tree_get_first (ucl_tree_node_t *@var{this})
Returns a pointer to the first between the brothers of the node
referenced by @var{this}; it can be a pointer to @var{this} itself, it
@var{this} is the first.
@end deftypefun


@deftypefun {ucl_tree_node_t *} ucl_tree_get_last (ucl_tree_node_t *@var{this})
Returns a pointer to the last between the brothers of the node
referenced by @var{this}; it can be a pointer to @var{this} itself, it
@var{this} is the last.
@end deftypefun

@c ------------------------------------------------------------
@node Tree Adding
@subsection Adding elements to a tree


@noindent
The @code{ucl_tree_set_*()} macros are used to append a new subtree to
a tree.


@defmac ucl_tree_set_son (ucl_tree_node_t * @var{nodPtr}, ucl_tree_node_t * @var{sonPtr})
Links together the nodes referenced by @var{nodPtr} and @var{sonPtr} so
that one is the son of the other. The old reference to the son of
@var{nodPtr} is lost; the old reference to the dad of @var{sonPtr} is
lost.
@end defmac


@defmac ucl_tree_set_bro (ucl_tree_node_t * @var{nodPtr}, ucl_tree_node_t * @var{broPtr})
Links together the nodes referenced by @var{nodPtr} and @var{broPtr} so
that one is the brother of the other. The node referenced by
@var{broPtr} becomes the right brother of the node referenced by
@var{nodPtr}. The old references to the brothers of the two nodes are
lost.
@end defmac


The following functions are used to insert subtrees in a tree. None of
the nodes in the target tree are detached. The links in the new subtrees
that are not interested by the relations in these functions, are left
untouched.


@deftypefun void ucl_tree_insert_dad (ucl_tree_node_t *@var{nodPtr}, ucl_tree_node_t *@var{dadPtr})
Inserts a new dad for a node. The dad node becomes the father of the
node and the first son of the old dad (if any).

@example	
 ---       ---
| A |     | A |
 ---       ---
  |         |
 ---       ---
|nod| ->  |dad|
 ---       ---
            |
           ---
          |nod|
           ---
@end example
@end deftypefun


Example of dad insertion:

@example
ucl_tree_insert_dad( 1, A )

0               D         0
|               |         |
1--2--3--4  +   A--C  =   A--C
|  |            |         |
5  6            B         1--2--3--4
                          |  |
                          5  6
@end example

@noindent
the @code{D} and @code{B} nodes are detached and will be lost if we
don't keep a reference to them.


@deftypefun void ucl_tree_insert_son (ucl_tree_node_t *@var{nodPtr}, ucl_tree_node_t *@var{sonPtr})
Inserts a new child for a node. The node referenced by @var{sonPtr}
becomes the last between the children of the node referenced by
@var{nodPtr}.

@example
 ---         ---
|nod|       |nod|
 ---         ---
  |     ->    |
 ---         ---     ---
| A |       | A |-->|son|
 ---         ---     ---
@end example
@end deftypefun


Example of son insertion:

@example
ucl_tree_insert_son( 0, A )

0               D         0
|               |         |
1--2--3--4  +   A--C  =   1--2--3--4--A--C
|  |            |         |  |        |
5  6            B         5  6        B
@end example

@noindent
the node @code{D} is detached and will be lost if we don't keep a
reference to it.


@deftypefun void ucl_tree_insert_prev (ucl_tree_node_t *@var{nodPtr}, ucl_tree_node_t *@var{broPtr})
Inserts a new brother for a node. The node referenced by @var{broPtr}
becomes the left brother of the node referenced by @var{nodPtr}.

@example
 ---    ---          ---    ---    ---
| A |--|nod|   ->   | A |--|bro|--|nod|
 ---    ---          ---    ---    ---
@end example
@end deftypefun


Example of prev node insertion:

@example
ucl_tree_insert_prev( 2, A )

0               D         0
|               |         |
1--2--3--4  +   A--C  =   1--A--2--3--4
|  |            |         |  |  |
5  6            B         5  B  6
@end example

@noindent
the @code{D} and @code{C} nodes are detached and will be lost if we
don't keep a reference to them.


@deftypefun void ucl_tree_insert_next (ucl_tree_node_t *@var{nodPtr}, ucl_tree_node_t *@var{broPtr})
Inserts a new brother for a node. The node referenced by @var{broPtr}
becomes the right brother of the node referenced by @var{nodPtr}.

@example
 ---    ---          ---    ---    ---
|nod|--| A |   ->   |nod|--|bro|--| A |
 ---    ---          ---    ---    ---
@end example
@end deftypefun


Example of next brother insertion:

@example
ucl_tree_insert_next( 2, A )

0               D         0
|               |         |
1--2--3--4  +   A--C  =   1--2--A--3--4
|  |            |         |  |
5  6            B         5  6
@end example

@noindent
the @code{D}, @code{B} and @code{C} nodes are detached and will be lost
if we don't keep a reference to them.
@c ------------------------------------------------------------
@node Tree Removing
@subsection Removing elements from a tree


@noindent
These functions will extract a node from a tree, returning a pointer to
the extracted node.


@deftypefun {ucl_tree_node_t *} ucl_tree_extract_dad (ucl_tree_node_t *@var{nodPtr})
Extracts the dad of the node referenced by @var{nodPtr} from the tree.
The referenced node and all its brothers are inserted in place of the
extracted dad.

Returns a pointer to the extracted node, or @code{NULL} if the selected
node has nod dad. All the pointers in the extracted node structure are
reset to @code{NULL}.

@example
 ---    ---    ---       ---    ---    ---    ---
| A |--|dad|--| B |     | A |--|nod|--| C |--| B |
 ---    ---    ---       ---    ---    ---    ---
         |          ->
        ---    ---              ---
       |nod|--| C |            |dad|
        ---    ---              ---
@end example
@end deftypefun


@deftypefun {ucl_tree_node_t *} ucl_tree_extract_son (ucl_tree_node_t *@var{nodPtr})
Extracts the son of the node referenced by @var{nodPtr} from the tree.
The son of the selected node is extracted from the hierarchy. All of its
children becomes children of the selected node.

Returns a pointer to the extracted node or @code{NULL} if the selected
node has no son. All the pointers in the extracted node structure are
reset to @code{NULL}.

@example
 ---               ---
|nod|             |nod|
 ---               ---
  |                 |
 ---    ---        ---    ---    ---
|son|--| C |  ->  | A |--| B |--| C |
 ---    ---        ---    ---    ---
  |
 ---    ---            ---
| A |--| B |          |son|
 ---    ---            ---
@end example
@end deftypefun


@deftypefun {ucl_tree_node_t *} ucl_tree_extract_prev (ucl_tree_node_t *@var{nodPtr})
Extracts the left brother of the node referenced by @var{nodPtr}.  The
left brother of the selected node is extracted from the hierarchy. Its
children becomes left brothers of the selected node.

Returns a pointer to the extracted node, or @code{NULL} if the selected
node has no left brother.  All the pointers in the extracted node
structure are reset to @code{NULL}.

@example
 ---               ---
| A |             | A |
 ---               ---
  |                 |
 ---    ---        ---    ---
|prv|--|nod|  ->  | B |--|nod|
 ---    ---        ---    ---
  |
 ---                   ---
| B |                 |prv|
 ---                   ---
@end example
@end deftypefun


@deftypefun {ucl_tree_node_t *} ucl_tree_extract_next (ucl_tree_node_t *@var{nodPtr})
Extracts the right brother of the node referenced by @var{nodPtr}.  The
right brother of the selected node is extracted from the hierarchy. Its
children becomes right brothers of the selected node.

Returns a pointer to the extracted node, or @code{NULL} if the selected
node has no right brother.  All the pointers in the extracted node
structure are reset to @code{NULL}.

@example
 ---    ---    ---       ---    ---    ---
|nod|--|nxt|--| A |     |nod|--| B |--| A |
 ---    ---    ---       ---    ---    ---
         |           ->
        ---                 ---
       | B |               |nxt|
        ---                 ---
@end example
@end deftypefun


@c ------------------------------------------------------------
@node Tree Iterators
@subsection Traversing a tree


@noindent
For the tree iterators, the return value of @code{ucl_iterator_ptr()} is
a pointer to the current node.


@deftypefun void ucl_tree_iterator_inorder (ucl_tree_node_t *@var{nodPtr}, ucl_iterator_t *@var{iter})
Initialises an in--order iteration.
@end deftypefun


@deftypefun void ucl_tree_iterator_preorder (ucl_tree_node_t *@var{nodPtr}, ucl_iterator_t *@var{iter})
Initialises a pre--order iteration.
@end deftypefun


@deftypefun void ucl_tree_iterator_postorder (ucl_tree_node_t *@var{nodPtr}, ucl_iterator_t *@var{iter})
Initialises a post--order iteration.
@end deftypefun




@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
