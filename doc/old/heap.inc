@c ------------------------------------------------------------
@node Heap
@section The heap structure


@cindex Heap container


@noindent
The heap container allows us to insert a bunch of values and extract
them sorted, from the lesser to the greater. The concept of ``lesser''
and ``greater'' are established by a user--provided function, so they
are customisable.

The heap is implemented as a binary tree. The heap node structure is
compatible with the link structure of the @code{ucl_list_t} container
(@ref{List}), so it's possible to sort a list converting it to a heap
and then rebuilding it, using list links as heap nodes directly.


@deftp {Struct Typedef} ucl_heap_t
The base structure for the heap container.
@end deftp


@deftp {Struct Typedef} ucl_heap_node_t
The type of heap nodes.
@end deftp



@menu
* Heap Creation::             Creating and destroying heaps.
* Heap Adding::               Adding elements to a heaps.
* Heap Removing::             Removing elements from a heaps.
* Heap Ops::                  Various operations on a heaps.
@end menu



@c ------------------------------------------------------------
@node Heap Creation
@subsection Creating and destroying heap


@deftypefun void ucl_heap_constructor (ucl_heap_t *@var{this}, ucl_valcmp_t *@var{valcmp})
Initialises the heap structure. @var{this} is a pointer to an already
allocated heap structure, @var{valcmp} is a pointer to a
@code{strcmp()}--like function used to compare values.
@end deftypefun


@deftypefun void ucl_heap_destructor (ucl_heap_t *@var{this})
Resets a heap structure. Before invoking this function all the nodes
must be extracted and freed by the user code.
@end deftypefun


Before invoking the destructor function, all the elements must be
extracted from the heap. Sample code:

@example
ucl_heap_t *      this;
ucl_heap_node_t * nodePtr;

while (ucl_heap_size(this))
@{
  nodePtr = ucl_heap_extract(this);
  ... frees the node structure ...
@}
ucl_heap_destructor(this);
@end example

@c ------------------------------------------------------------
@node Heap Adding
@subsection Adding elements to a heap


@deftypefun void ucl_heap_insert (ucl_heap_t *@var{this}, ucl_heap_node_t *@var{node})
Inserts a new node in the heap. @var{node} is a pointer to an already
allocated heap node structure.
@end deftypefun


Example of node insertion:

@example
ucl_heap_t *       heapPtr;
ucl_heap_node_t *  nodePtr;
ucl_value_t          value;


nodePtr = malloc(sizeof(ucl_heap_node_t));
if (nodePtr == NULL)
  @{
    /* ... */
  @}

heapPtr = ...;
value.integer = ...;
ucl_heap_setval(nodePtr, value);
ucl_heap_insert(heapPtr, nodePtr);
@end example


@c ------------------------------------------------------------
@node Heap Removing
@subsection Removing elements from a heap


@deftypefun {ucl_heap_node_t *} ucl_heap_extract (ucl_heap_t *@var{this})
Extracts a node from the heap. The node is the one with the smallest
value. Returns a pointer to the node.
@end deftypefun


Example of node extraction:

@example
ucl_heap_t *       heapPtr;
ucl_heap_node_t *  nodePtr;
ucl_value_t          value;

heapPtr = ...;
nodePtr = ucl_heap_extract(heapPtr);
value   = ucl_heap_getval(nodePtr);
free(nodePtr);
@end example


@c ------------------------------------------------------------
@node Heap Ops
@subsection Various operations on a heap


@deftypefun size_t ucl_heap_size (ucl_heap_t *@var{this})
Returns a value of type @code{size_t} representing the number of nodes
in the heap.
@end deftypefun


@deftypefun void ucl_heap_setval (ucl_heap_node_t *@var{nodePtr}, ucl_value_t @code{newval})
Stores a new value in a heap node.
@end deftypefun


@deftypefun ucl_value_t ucl_heap_getval (ucl_heap_node_t *@var{nodePtr})
Returns a copy of the value, of type @code{ucl_value_t}, stored in a heap
node.
@end deftypefun


@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
