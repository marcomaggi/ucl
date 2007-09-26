@c ------------------------------------------------------------
@node Circular
@section The circular list


@noindent
The circular container provides a circular double linked list. It's
implemented as a chain of little node structures; a pointer to the
@emph{current} position is stored in a base structure. The current
position marker can be moved forward and backward as a cursor.

@deftp Struct ucl_circular_t
Base structure of the container. It must be allocated by the user code,
on the stack or in a block of dynamically allocated memory.
@end deftp

@deftp Struct ucl_circular_link_t
The data type of the nodes. Nodes must be allocated and freed by the
user code and a pointer to them passed as arguments to the @UCL{}
circular functions: that way the container module doesn't have to deal
with memory allocation and the user code can implement a memory
allocator.
@end deftp

The handling of list links is derived from the handling of elements in
the @acronym{TCL} (Tool Command Language) hash table by John Ousterhout
and others (@url{http://www.tcl.tk} for more about @acronym{TCL}).


@menu
* Circular Creation::           Creating and destroying circulars.
* Circular Adding::             Adding elements to a circular.
* Circular Removing::           Removing elements from a circular.
* Circular Ops::                Various operations on a circular.
* Circular Data::               Another usage for links.
@end menu



@c ------------------------------------------------------------
@node Circular Creation
@subsection Creating and destroying circulars


@deftypefun void ucl_circular_constructor (ucl_circular_t *@var{this})
Initialises an already allocated structure.
@end deftypefun


@deftypefun void ucl_circular_destructor (ucl_circular_t *@var{this})
Destroys the structure. All the members are reset to zero. Before
calling this function the user code has to make sure that all the links
are extracted from the list.  All the structure fields are reset to
zero.
@end deftypefun


To extract all the links from a circular list, we can do:

@example
ucl_value_t val;
ucl_circular_link_t *  circPtr;
ucl_circular_link_t *  linkPtr;

...

while (ucl_circular_size(circPtr))
  @{
    ucl_circular_stepforw(circPtr);
    linkPtr = ucl_circular_extract(circPtr);

    val = ucl_circular_getval(linkPtr);

    /* insert here the code to destroy the value */
    /* insert here the code to free the link memory */
  @}
@end example

@c ------------------------------------------------------------
@node Circular Adding
@subsection Adding elements to a circular


@deftypefun void ucl_circular_insert (ucl_circular_t *@var{this}, ucl_circular_link_t *@var{linkPtr})
Inserts an element at the current position. To do this the user code
have to allocate a new circular link structure, store the value into it
and hand a pointer to the node to this function.

The old current link becomes the next link.
@end deftypefun


Example of element insertion:

@example
ucl_circular_t *       circPtr;
ucl_circular_link_t *  linkPtr;
ucl_value_t val;

circPtr = ...;

linkPtr = malloc(sizeof(ucl_circular_link_t) + sizeof(DataType));
if (linkPtr == NULL)
  @{
    /* handle the error */
  @}

val = ...;
ucl_circular_setval(linkPtr, val);
ucl_circular_insert(circPtr, linkPtr);
@end example


@c ------------------------------------------------------------
@node Circular Removing
@subsection Removing elements from a circular


@deftypefun {ucl_circular_link_t *} ucl_circular_extract (ucl_circular_t *@var{this})
Extracts the current link. It returns a pointer to the link or
@code{NULL} if the list is empty.  The new current element is the next
in the forward direction.
@end deftypefun


Example of element removal:

@example
ucl_value_t             val;
ucl_circular_t *        circPtr;
ucl_circular_link_t *   linkPtr;

circPtr = ...;

linkPtr = ucl_circular_extract(circPtr);
if (linkPtr == NULL)
  @{
    /* handle the error */
  @}

val = ucl_circular_getval(linkPtr);
free(linkPtr);
/* do something with val */
@end example


@c ------------------------------------------------------------
@node Circular Ops
@subsection Various operations on a circular


@defmac ucl_circular_size (ucl_circular_t *@var{this})
Returns the number of elements in the container. The return value is of
@code{size_t} type.
@end defmac


@defmac ucl_circular_current (ucl_circular_t *@var{this})
Returns a pointer to the current link or @code{NULL} if the container is
empty. The return value is of type @code{ucl_circular_link_t *}.
@end defmac


@defmac ucl_circular_data (ucl_circular_link_t *@var{linkPtr})
@defmacx ucl_circular_getval (ucl_circular_link_t *@var{linkPtr})
Returns a the data value in the link. The return value is of
@code{ucl_value_t} type.
@end defmac


@defmac ucl_circular_setval (ucl_circular_link_t *@var{linkPtr}, ucl_value_t @var{val})
Stores a value in a link.
@end defmac


@deftypefun void ucl_circular_forward (ucl_circular_t *@var{this}, size_t @var{times})
Moves forward the current position, @var{times} is the forward
offset. If the container is empty or the offset is zero nothing happens.
@end deftypefun


@deftypefun void ucl_circular_backward (ucl_circular_t *@var{this}, size_t @var{times})
Moves backward the current position, @var{times} is the backward
offset. If the container is empty or the offset is zero nothing happens.
@end deftypefun


@defmac ucl_circular_stepforw (ucl_circular_t *@var{this})
Move the current position one step forward. This macro assumes that the
container is not empty. Returns the pointer to the new current link.
@end defmac


@defmac ucl_circular_stepback (ucl_circular_t *@var{this})
Move the current position one step backward. This macro assumes that the
container is not empty. Returns the pointer to the new current link.
@end defmac


@deftypefun {ucl_circular_link_t *} ucl_circular_find (@var{this}, @var{val}, @var{compar})
Moves the current position to the an element whose value is equal to a
selected one. This function performs a forward element--by--element
search comparing the values in the container to the given one, using the
@var{compar} function.

Arguments:

@table @code
@item ucl_circular_t * @var{this}
pointer to the base structure;

@item ucl_value_t @var{val}
the value to find;

@item ucl_valcmp_t @var{compar}
pointer to a @code{strcmp()}--like function that's used to compare the
given value with the contained ones; @var{val} is used as first argument
to this function.
@end table
@end deftypefun


@c ------------------------------------------------------------
@node Circular Data
@subsection Another usage for links


@noindent
It's possible to allocate memory for a value together with the link
structure.

@example
typedef struct MyLink @{
  ucl_circular_link_t   link;
  Data                bigdata;
@} MyLink;

MyLink *             myPtr;
ucl_circular_link_t *  linkPtr;

linkPtr = malloc(sizeof(MyData));
myPtr   = (MyLink *) linkPtr;
linkPtr->val.ptr = &(myPtr->bigdata);
@end example

Now we can use @code{linkPtr} as a normal circular list link and access
the @code{bigdata} member through the link @code{void *} pointer.

@example
Data *  ptr;

ptr = (Data *) ucl_circular_getval(linkPtr).ptr;
@end example



@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
