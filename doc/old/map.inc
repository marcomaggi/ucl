@c ------------------------------------------------------------
@node Map
@section The map structure


@cindex Map container


@noindent
The @code{ucl_map_t} structure can be used to implement an associative
array; it's implemented as an AVL tree.


@deftp Struct ucl_map_t
Base structure for the container. It must be allocated by the user code,
on the stack or in a block of dynamically allocated memory.
@end deftp


@deftp Struct ucl_map_link_t
The data type of the nodes. Nodes must be allocated and freed by the
user code and a pointer to them passed as arguments to the @UCL{} map
functions: that way the container module doesn't have to deal with
memory allocation and the user code can implement a memory allocator.
@end deftp


The map/multimap idea was inspired by the book on C++ by Bjarne
Stroustrup and by the STL C++ (Standard Template Library) by Stepanov
and Lee.

The handling of nodes is influenced by the handling of elements in the
@acronym{TCL} (Tool Command Language) hash table by John Ousterhout and
others (@url{http://www.tcl.tk/} for more about @acronym{TCL}).

@menu
* Map Intro::                   Introduction to the operations and the
                                selected implementation.
* Map Creation::                Creating and destroying maps.
* Map Adding/Removing::         Adding and removing elements to a map.
* Map Accessors::               Accessing values and keys.
* Map Ops::                     Various operations on a map.
* Map Iterators::               Iteration over a map.
* Set Iterators::               Composing map iterators.
@end menu


@c ------------------------------------------------------------
@node Map Intro
@subsection Introduction to the operations and the selected implementation


@noindent
Maps are often used as associative arrays, that is: as collections of
key/value pairs.  The operations we want to do on a map are:

@itemize
@item
add a key/value pair, if the element already exists replace the old
value with the new one or add a new key/value pair;

@item
find a key/value pair with a specified key;

@item
remove all key/value pairs with a selected key, or the one with a
selected value among the ones having the same key;

@item
traverse the collection of key/value pairs from the lesser key to the
greater;

@item
traverse the collection of key/value pairs from the greater key to the
lesser.
@end itemize

Clearly there are two sub--types of map container: the one that allows
multiple values to be associated to the same key, and the one that
doesn't. We call the first a @dfn{multimap} and the second a @dfn{simple
map}.


@c ------------------------------------------------------------
@node Map Creation
@subsection Creating and destroying maps


@deftypefun void ucl_map_constructor (@var{this}, @var{flags}, @var{keycmp})
Initialises an already allocated map structure.

Arguments:

@table @code
@item ucl_map_t * @var{this}
pointer to the already allocated map structure;

@item unsigned int @var{flags}
map behaviour flags;

@item ucl_valcmp_t * @var{keycmp}
pointer to the @code{strcmp()}--like function used to compare keys.
@end table

Map configuration @var{flags} can be an ORed combination of:

@table @code
@item UCL_ALLOW_MULTIPLE_OBJECTS
allows more than one object to be associated to the same key, with this
the map behaves like a multimap.
@end table
@end deftypefun


@deftypefun void ucl_map_destructor (ucl_map_t *@var{this})
Destroys a map structure, the structure itself is not deallocated. The
elements are reset to zero.

Before invoking this function the user code has to extract all the nodes
from the map.  If the elements are not extracted before this function is
invoked, at least memory leakage will happen.
@end deftypefun


@c ------------------------------------------------------------
@node Map Adding/Removing
@subsection Adding and removing elements to a map


@deftypefun void ucl_map_insert (ucl_map_t *@var{this}, ucl_map_link_t *@var{linkPtr})
Given an already allocated and constructed map link, inserts it in the
map. This functions always succeeds if @var{this} references a multimap,
can lead to an error if @var{this} is a simple map and an element with
the same key already exists. If such a condition occurs, this function
@strong{silently does nothing}; it's our responsibility to use
@code{ucl_map_find()} to check for the element existence.
@end deftypefun

Example of link insertion in a simple map:

@example
ucl_map_t *      this;
ucl_map_link_t * link;

ucl_value_t        key;
ucl_value_t        obj;

/* ... */

key = ...;
obj = ...;

link = ucl_map_find(this, key);
if (link == NULL)
  @{
    link = (ucl_map_link_t *) malloc(sizeof(ucl_map_link_t));
    ucl_map_setkey(link, key);
    ucl_map_setval(link, obj);
    ucl_map_insert(this, link);
  @}
@end example


@deftypefun {ucl_map_link_t *} ucl_map_remove (ucl_map_t *@var{this}, ucl_map_link_t *@var{linkPtr})
Removes from the map the key/value pair in the element referenced by
@var{linkPtr}. This must be the return value of a previous invocation of
@code{ucl_map_find()}. This function only removes the link from the map:
it's our responsibility to free the key, the object and the link
structure itself.

Returns a pointer to the extracted link.
@end deftypefun


Example of link removal:

@example
ucl_map_t *      this;
ucl_map_link_t * link;

ucl_value_t        key;
ucl_value_t        obj;

/* ... */

link = ucl_map_find(this, key);
if (link != NULL)
  @{
    ucl_map_remove(this, link);
    key = ucl_map_getkey(link);
    obj = ucl_map_getval(link);
    free(link);

    /* free "key" and "obj" */
  @}
@end example

@c ------------------------------------------------------------
@node Map Accessors
@subsection Accessing values and keys


@deftypefun void ucl_map_setkey (ucl_map_link_t *@var{linkPtr}, ucl_value_t @var{newKey})
Stores a key in a link.
@end deftypefun


@deftypefun void ucl_map_setval (ucl_map_link_t *@var{linkPtr}, ucl_value_t @var{newObj})
Stores an object in a link.
@end deftypefun


@deftypefun ucl_value_t ucl_map_getkey (ucl_map_link_t *@var{linkPtr})
Returns the key in a map link.
@end deftypefun


@deftypefun ucl_value_t ucl_map_getval (ucl_map_link_t *@var{linkPtr})
Returns the object in a map link.
@end deftypefun


@c ------------------------------------------------------------
@node Map Ops
@subsection Various operations on a map


@deftypefun size_t ucl_map_count (const ucl_map_t *@var{this}, const ucl_value_t @var{key})
Returns the number of elements with @var{key}; this is always 0 or 1 for
simple maps
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_find (const ucl_map_t *@var{this}, const ucl_value_t @var{key});
Returns a pointer to the (first found) element associated with
@var{key}. The return value is @code{NULL} if such an element doesn't
exist.

For a multimap, this function returns a pointer to the first element
with the selected key, so that the others can be found with repeated
invocations of @code{ucl_map_next()}.
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_first (const ucl_map_t *@var{this})
Returns a pointer to the element with lesser key in the map, or
@code{NULL} if the map is empty.
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_last (const ucl_map_t *@var{this})
Returns a pointer to the element with greater key in the map, or
@code{NULL} if the map is empty.
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_next (ucl_map_link_t *@var{linkPtr})
Returns a pointer to the element adjacent to the one referenced by
@var{linkPtr} with greater key, or @code{NULL} if @var{linkPtr} has the
greater key in the map.
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_prev (ucl_map_link_t *@var{linkPtr})
Returns a pointer to the element adjacent to the one referenced by
@var{linkPtr} with lesser key, or @code{NULL} if @var{linkPtr} has the
lesser key in the map.
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_find_or_next (ucl_map_t *@var{this}, ucl_value_t * @var{key})
Given a key find the element in the map associated with it, or the
element with the lesser key greater than the selected one.  Returns a
pointer to the requested link or @code{NULL} if all the keys in the map
are lesser than the selected one.
@end deftypefun


@deftypefun {ucl_map_link_t *} ucl_map_find_or_prev (ucl_map_t *@var{this}, ucl_value_t * @var{key})
Given a key find the element in the map associated with it, or the
element with the greater key lesser than the selected one.  Returns a
pointer to the requested link or @code{NULL} if all the keys in the map
are greater than the selected one.
@end deftypefun


@deftypefun size_t ucl_map_size (const ucl_map_t *@var{this})
Returns the number of elements in the map.
@end deftypefun


@c ------------------------------------------------------------
@node Map Iterators
@subsection Iteration over a map


@noindent
For the map, the iteration is over the map links: the macro
@code{ucl_iterator_ptr()} returns a pointer to the current map link.
We can use the @code{ucl_map_getkey()} and @code{ucl_map_setkey()} to
acquire the keys and values.


@deftypefun void ucl_map_iterator_inorder (@var{this}, @var{iterator})
Initalises an inorder iteration.

@table @code
@item const ucl_map_t *@var{this}
pointer to the map structure;

@item ucl_iterator_t *@var{iterator}
pointer to the iterator structure.
@end table
@end deftypefun


@deftypefun void ucl_map_iterator_preorder (@var{this}, @var{iterator})
Initialises a preorder iteration.

@table @code
@item const ucl_map_t *@var{this}
pointer to the map structure;

@item ucl_iterator_t *@var{iterator}
pointer to the iterator structure.
@end table
@end deftypefun


@deftypefun void ucl_map_iterator_postorder (@var{this}, @var{iterator})
Initialises a postorder iteration.

@table @code
@item const ucl_map_t *@var{this}
pointer to the map structure;

@item ucl_iterator_t *@var{iterator}
pointer to the iterator structure.
@end table
@end deftypefun


@deftypefun void ucl_map_iterator_levelorder (@var{this}, @var{iterator})
Initialises a level order iteration.

@table @code
@item const ucl_map_t *@var{this}
pointer to the map structure;

@item ucl_iterator_t *@var{iterator}
pointer to the iterator structure.
@end table
@end deftypefun


@deftypefun void ucl_map_lower_bound (@var{this}, @var{iterator}, @var{key})
Initialises an iteration over the elements with the selected key,
starting with the first element.

@table @code
@item const ucl_map_t *@var{this}
pointer to the map structure;

@item ucl_iterator_t *@var{iterator}
pointer to the iterator structure;

@item const ucl_value_t @var{key}
the selected key.
@end table
@end deftypefun



@deftypefun void ucl_map_upper_bound (@var{this}, @var{iterator}, @var{key})
Initialises an iteration over the elements with the selected key,
starting with the last element.

@table @code
@item const ucl_map_t *@var{this}
pointer to the map structure;

@item ucl_iterator_t *@var{iterator}
pointer to the iterator structure;

@item const ucl_value_t @var{key}
the selected key.
@end table
@end deftypefun


@c ------------------------------------------------------------
@node Set Iterators
@subsection Composing map iterators


@cindex Set iterators


@noindent
It's possible to compose map iterators to implement set operations: the
keys from a map are used as set elements. The values are not
considered. A set operation is implemented as an iterator that visits
one by one the result of the operation itself.

The key values must be of the same data type. That means that the
compare function used by both the maps, must accept the same type of
values and return the same values when called with the same arguments.

For all the set iterators, the arguments are:

@table @code
@item ucl_iterator_t * @var{it1}
pointer to an in--order iterator over set 1, already initialised;

@item ucl_iterator_t * @var{it2}
pointer to an in--order iterator over set 2, already initialised;

@item ucl_iterator_t * @var{iter}
pointer to the set iterator structure.
@end table

The input map iterators must be of in--order type: if the sequences are
visited from the lesser to the greater key, the minimum amount of key
comparison is performed.

If the sequences are not visited with the in--order iterator, the result
is not defined.

The set iterators are used in the same fashion of all the other
iterators in the @acronym{UCL} (@ref{Iterators}). The value retrieved
with the @code{ucl_iterator_ptr()} macro is the pointer to the
referenced map link.

@deftypefun void ucl_map_iterator_union (@var{it1}, @var{it2}, @var{iter})
Initialises the iteration over all the elements from both the
sequences. Elements present in both sequences are included twice.
@end deftypefun


@deftypefun void ucl_map_iterator_intersection (@var{it1}, @var{it2}, @var{iter})
Initialises  the iteration  over all  the elements  present in  both the
sequences. Elements included in only one sequence are discarded.
@end deftypefun


@deftypefun void ucl_map_iterator_complintersect (@var{it1}, @var{it2}, @var{iter})
Initialises the iteration over all the elements present in only one of
the two sequences.
@end deftypefun


@deftypefun void ucl_map_iterator_subtraction (@var{it1}, @var{it2}, @var{iter})
Initialises the iteration over all the elements from sequence 1 that are
not present in sequence 2.
@end deftypefun


Example:

@example
Sequence 1: 0 1 2 3 4 5 6
Sequence 2: 4 5 6 7 8 9
Union: 0 1 2 3 4 4 5 5 6 6 7 8 9
Intersection: 4 5 6
Complementary intersection: 0 1 2 3 7 8 9
Subtraction: 0 1 2 3
@end example



@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
