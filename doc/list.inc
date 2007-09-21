@c ------------------------------------------------------------
@node List
@section The linked list structure


@noindent
The @UCL{} list container is an implementation of the classic double
linked list: elements are stored in little structures chained together
with pointers that allow the user code to iterate forward and backward.

@deftp Struct ucl_list_t
The data type of the base structure. It must be allocated by the user
code, on the stack or in a block of dynamically allocated memory.
@end deftp

@deftp Struct ucl_list_link_t
The data type of the nodes. Nodes must be allocated and freed by the
user code and a pointer to them passed as arguments to the @UCL{} list
functions: that way the container module doesn't have to deal with
memory allocation and the user code can implement a memory allocator.
@end deftp

The handling of  list links is derived from the  handling of elements in
the @acronym{TCL} (Tool Command  Language) hash table by John Ousterhout
and others (@url{http://www.tcl.tk} for more about @acronym{TCL}).


@menu
* List Creation::               Creating and destroying lists.
* List Adding::                 Adding elements to a list.
* List Removing::               Removing elements from a list.
* List Ops::                    Various operations on a list.
* List Iteration::              Iteration over a list.
@end menu


@c ------------------------------------------------------------
@node List Creation
@subsection Creating and destroying lists


@deftypefun void ucl_list_constructor (ucl_list_t *@var{this})
Initialises an already allocated structure to build a new list
container.
@end deftypefun


@deftypefun void ucl_list_destructor (ucl_list_t *@var{this})
Destroys the structure pointed by @var{this}. The structure itself is
not deallocated. Before calling this the user code has to make sure that
all the links are extracted from the list.

All the structure fields are reset to zero.  This concept of
``destruction'' is still in development in the head of the author, so it
may change in future releases; it's better, though, to have the code
invoke a destructor, so if things will change we have it in place. It
may even be replaced by a do--nothing @code{#define}.
@end deftypefun


To extract all the links from a list, we can do:

@example
ucl_list_t *       listPtr;
ucl_list_link_t *  linkPtr;
ucl_value_t        val;

...

while(ucl_list_size(listPtr))
 @{
    linkPtr = ucl_list_popfront(listPtr);
    val     = ucl_list_getval(linkPtr);

    /* insert here the code to destroy the object */
    /* insert here the code to free the link memory */
 @}
@end example

@noindent
popping elements from the front or from the back no difference.

@c ------------------------------------------------------------
@node List Adding
@subsection Adding elements to a list


@deftypefun void ucl_list_insertafter (@var{this}, @var{linkPtr}, @var{newPtr})
Inserts an element after a link.  The new link is inserted after the
selected one; if the selected link is the last in the chain, the new
link becomes the last in the chain.

This function can't be used to insert an element in an empty list.

Arguments:

@table @code
@item ucl_list_t *@var{this}
pointer to the base structure;

@item ucl_list_link_t *@var{linkPtr}
pointer to a link in the list, it must be the return value of a previous
invocation of @code{ucl_list_index()};

@item ucl_list_link_t *@var{newPtr}
pointer to a new list link.
@end table
@end deftypefun


@deftypefun void ucl_list_insertbefore (@var{this}, @var{linkPtr}, @var{newPtr})
Inserts an element before a link.  The new link is inserted before the
selected one; if the selected link is the first in the chain, the new
link becomes the first in the chain.

This function can't be used to insert an element in an empty list.

Arguments:

@table @code
@item ucl_list_t *@var{this}
pointer to the base structure;

@item ucl_list_link_t *@var{linkPtr}
pointer to a link in the list, it must be the return value of a previous
invocation of @code{ucl_list_index()};

@item ucl_list_link_t *@var{newPtr}
pointer to a new list link.
@end table
@end deftypefun


@deftypefun void ucl_list_pushfront (ucl_list_t *@var{this}, ucl_list_link_t *@var{linkPtr})
Inserts an element at the beginning of the list. This function can be
used to insert an element in an empty list.
@end deftypefun


@deftypefun void ucl_list_pushback (ucl_list_t *@var{this}, ucl_list_link_t *@var{linkPtr})
Inserts an element at the end of the list. This function can be used to
insert an element in an empty list.
@end deftypefun


@c ------------------------------------------------------------
@node List Removing
@subsection Removing elements from a list


@deftypefun void ucl_list_extract (ucl_list_t *@var{this}, ucl_list_link_t *@var{linkPtr})
Extracts a selected link. @var{linkPtr} is pointer to a list link, it
must be the return value of a previous invocation of
@code{ucl_list_index()}.
@end deftypefun


Example of element removal:

@example
ucl_list_t *      listPtr;
ucl_list_link_t * linkPtr;
size_t            position;

position = ...

linkPtr = ucl_list_index(this, position);
if (linkPtr == NULL)
  @{
    ...
  @}
ucl_list_extract(this, linkPtr);
@end example


@defmac ucl_list_popfront (ucl_list_t *@var{this})
Removes an element at the beginning of the list. This is a wrapper for
@code{ucl_list_extract()}.
@end defmac


@defmac ucl_list_popback (ucl_list_t *@var{this})
Removes an element at the end of the list. This is a wrapper for
@code{ucl_list_extract()}.
@end defmac

@c ------------------------------------------------------------
@node List Ops
@subsection Various operations on a list



@defmac ucl_list_size (ucl_list_t *@var{this})
Returns a @code{size_t} value representing the number of elements in the
container.
@end defmac


@defmac ucl_list_index (ucl_list_t *@var{this}, size_t @var{index})
Converts a position in a pointer to the corresponding list link.
Returns a pointer to the selected link, or @code{NULL} if the index is
out of range.
@end defmac


@defmac ucl_list_getval (ucl_list_link_t *@var{linkPtr})
Returns a @code{ucl_value_t} value representing the data of a link.
@end defmac


@defmac ucl_list_setval (ucl_list_link_t *@var{linkPtr}, ucl_value_t @var{val})
Stores a new value in a link.
@end defmac


@defmac ucl_list_front (const ucl_list_t *@var{this})
Returns a pointer to the first link in the list or @code{NULL} if the
list is empty.
@end defmac


@defmac ucl_list_back (const ucl_list_t *@var{this})
Returns a pointer to the last link in the list or @code{NULL} if the
list is empty.
@end defmac


@defmac ucl_list_next (ucl_list_link_t * @var{linkPtr})
Returns a pointer to the link next to @var{linkPtr} or @code{NULL} if
@var{linkPtr} is the last in the chain.
@end defmac


@defmac ucl_list_prev (ucl_list_link_t * @var{linkPtr})
Returns a pointer to the previous link of @var{linkPtr} or @code{NULL}
if @var{linkPtr} is the first in the chain.
@end defmac


@c ------------------------------------------------------------
@node List Iteration
@subsection Iteration over a list


@noindent
Example of forward iteration:

@example
ucl_list_t *       this;
ucl_list_link_t *  linkPtr;
ucl_value_t        val;

...

for (linkPtr = ucl_list_front(listPtr);
     linkPtr != NULL;
     linkPtr = ucl_list_next(linkPtr))
  @{
    val = ucl_list_getval(linkPtr);
    ...
  @}
@end example


Example of backward iteration:

@example
ucl_list_t *       this;
ucl_list_link_t *  linkPtr;
ucl_value_t        val;

...

for (linkPtr = ucl_list_back(listPtr);
     linkPtr != NULL;
     linkPtr = ucl_list_prev(linkPtr))
  @{
    val = ucl_list_data(linkPtr);
    ...
  @}
@end example

@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
