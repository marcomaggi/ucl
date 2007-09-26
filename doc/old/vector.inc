@c ------------------------------------------------------------
@node Vector
@section The vector structure

@cindex Vector container


@noindent
The @UCL{} vector container is an implementation of the classic array
with redundant memory allocation.

@deftp Struct ucl_vector_t
The data type of the base structure. It must be allocated by the user
code, on the stack or in a block of dynamically allocated memory.
@end deftp

This module was inspired by the book on C++ by Bjarne Stroustrup and by
the STL C++ (Standard Template Library).


@menu
* Vector Creation::             Creating and destroying vectors.
* Vector Indexes::              Converting indexes to pointers.
* Vector Adding::               Adding elements to a vector.
* Vector Removing::             Removing elements from a vector.
* Vector Ops::                  Various operations on a vector.
* Vector Iteration::            Iteration over a vector.
* Vector Memory::               Allocating and freeing memory.
* Vector as PQueue::            Using a vector as a priority queue.
@end menu


@c ------------------------------------------------------------
@node Vector Creation
@subsection Creating and destroying vectors


@deftypefun {void *} ucl_vector_constructor1 (@var{this}, @var{size}, @var{step_up}, @var{step_down}, @var{pad}, @var{dim})

Initialise a structure to build a new vector container.

Arguments:

@table @code
@item ucl_vector_t *this
pointer to an already allocated @code{ucl_vector_t} structure;

@item size_t size
the number of slots to allocate for the new vector, it must be a
non--null positive integer;

@item size_t step_up
select the number of slots to add when reallocating the array, it must
be a non--null positive integer;

@item size_t step_down
the number of free slots that will cause the array to be reallocated and
restricted, it must be a non--null positive integer;

@item size_t pad
the numer of free slots to keep at the beginning of the array when
moving elements, it must be a positive or null integer no lesser than
@var{size};

@item size_t dim
the size in bytes of the element that the vector will hold, this should
be the result of the @code{sizeof()} operator applied to the data type.
@end table

Returns a pointer to the first allocated slot, or NULL if an error
allocating memory occurs or if the selected values are incorrect.
@end deftypefun


@defmac ucl_vector_constructor (@var{this}, @var{dim})
A wrapper for @code{ucl_vector_constructor1} that initialises the
structure with the default values (declared in @file{ucl.h}):

@table @code
@item UCL_VECTOR_DEFAULT_SIZE (16)
@item UCL_VECTOR_DEFAULT_STEP_UP (8)
@item UCL_VECTOR_DEFAULT_STEP_DOWN (10)
@item UCL_VECTOR_DEFAULT_PAD (3)
@end table
@end defmac


@deftypefun void ucl_vector_destructor (ucl_vector_t * @var{this})
Destroys the structure pointed by @var{this}. The structure itself is
not deallocated. Any block of dynamically allocated memory is freed.
All the structure fields are reset to zero.
@end deftypefun


@c ------------------------------------------------------------
@node Vector Indexes
@subsection Converting indexes to pointers


@deftypefun {void *} ucl_vector_index (ucl_vector_t *@var{this}, size_t @var{index})
Converts @var{index} into the corresponding pointer in the array.
Returns a pointer to the selected slot, or @code{NULL} if the selected
index is out of range.
@end deftypefun


@deftypefun {void *} ucl_vector_newindex (ucl_vector_t *@var{this}, size_t @var{index})
Converts @var{index} into the corresponding pointer in the array.

This is different from @code{ucl_vector_index()} in that the requested
position can be one unit greater than the last position in the vector:
that way the returned value can be used with @code{ucl_vector_insert()}
to append an element to the end of the vector.

Returns a pointer to the selected slot, or @code{NULL} if the selected
index is out of range.
@end deftypefun


Example:

@example
ucl_vector_t *  vectPtr;
DataType *    ptr;
DataType      val;

ptr = ucl_vector_index(vectPtr, 13);
val = *ptr;
@end example


@c ------------------------------------------------------------
@node Vector Adding
@subsection Adding elements to a vector


@deftypefun {void *} ucl_vector_insert (ucl_vector_t *@var{this}, void *@var{target})
Inserts an empty slot at a selected position.  This function assumes
that the array has a free slot to hold the new element: to make sure
that this is true a call to this function must be preceeded by a call to
@code{ucl_vector_enlarge()}.

The pointer to the slot must be the return value of a previous
invocation to @code{ucl_vector_newindex()}.

This function tries to move as less elements as possible to create an
empty slot at the selected position. The return value is a pointer to
the empty slot: this could be different from the value of the
@var{target} argument.
@end deftypefun


@deftypefun {void *} ucl_vector_insertsort (@var{this}, @var{elmPtr}, @var{compar})
Finds the position in the vector in which an element must be inserted to
keep the array sorted and creates a new slot there. This function
assumes that: there's room in the vector to insert a new element; the
array is sorted.

Arguments:

@table @code
@item ucl_vector_t * @var{this}
pointer to the base structure;

@item void * @var{elmPtr}
pointer to a variable holding a copy of the element to be inserted, it's
used as first argument to the @code{compar} function;

@item int (*@var{compar})(const void *a, const void *b)
pointer to a @code{strcmp()}--like function that's used to compare
elements.
@end table

The return value is a pointer to the empty slot.
@end deftypefun


The correct sequence of function calls required to insert a new element
is: enlarge the vector, acquire the pointer, make a free slot, copy the
value. Example:

@example
ucl_vector_t *    vectPtr;
DataType *      ptr;
size_t          index;

...

ptr = ucl_vector_enlarge(vectPtr);
if (ptr == NULL)
  @{
    handle_the_error();
  @}

ptr  = ucl_vector_newindex(vectPtr, index);
ptr  = ucl_vector_insert(vectPtr, ptr);
*ptr = data_value;
@end example


@c ------------------------------------------------------------
@node Vector Removing
@subsection Removing elements from a vector


@deftypefun void ucl_vector_erase (ucl_vector_t *@var{this}, void *@var{target})
Erases an element at a selected position: the slot is overwritten by
moving as less elements as possible to cover it.

After  the invocation  the pointer  represented by  the @var{target}
argument value, it's  not guaranteed to be a  pointer to a valid
slot in the vector.
@end deftypefun

After  the invocation  of  this  function, it's  possible  to attempt  a
reallocation of  the array  to free  some unused memory  with a  call to
@code{ucl_vector_restrict()}.

Example:

@example
ucl_vector_t *    vectPtr;
DataType *      ptr;
size_t          index;

...

ptr  = ucl_vector_index(vectPtr, index);
data_value = *ptr;
ucl_vector_erase(vectPtr, ptr);

ptr = ucl_vector_restrict(vectPtr);
if (ptr == NULL)
  @{
    handle_the_error();
  @}
@end example


@c ------------------------------------------------------------
@node Vector Ops
@subsection Various operations on a vector


@defmac ucl_vector_size (ucl_vector_t *@var{this})
Returns a @code{size_t} value representing the number of elements in the
container.
@end defmac


@defmac ucl_vector_front (ucl_vector_t *@var{this})
A wrapper for @code{ucl_vector_index()} thath returns a pointer to the
first element in the array.
@end defmac


@defmac ucl_vector_back (ucl_vector_t *@var{this})
A wrapper for @code{ucl_vector_index()} that returns a pointer to the
last element in the array.
@end defmac


@deftypefun void ucl_vector_qsort (this, compar)
Quick sort the vector using the C library function @code{qsort()}.

@table @code
@item ucl_vector_t *this
pointer to the structure;
@item int (*compar)(const void *a, const void *b)
pointer to the function to use to compare elements.
@end table
@end deftypefun


@deftypefun {void *} ucl_vector_find (@var{this}, @var{elmPtr}, @var{compar})
Finds an element in the array with a linear search.

Arguments:

@table @code
@item ucl_vector_t * @var{this}
pointer to the base structure;

@item void * @var{elmPtr}
pointer to a variable holding a copy of the element to be found, it's
used as first argument to the @code{compar} function;

@item int (*@var{compar})(const void *a, const void *b)
pointer to a @code{strcmp()}--like function that's used to compare
elements.
@end table

Returns a pointer to the found element in the array, or @code{NULL} if
the element is not present.
@end deftypefun


@deftypefun {void *} ucl_vector_sortfind (@var{this}, @var{elmPtr}, @var{compar})
Finds an element in the array with a binary search.  This function
assumes that the array is sorted.  If there are few elements in the
vector, a linear search is performed.

Arguments:

@table @code
@item ucl_vector_t * @var{this}
pointer to the base structure;

@item void * @var{elmPtr}
pointer to a variable holding a copy of the element to be found, it's
used as first argument to the @code{compar} function;

@item int (*@var{compar})(const void *a, const void *b)
pointer to a @code{strcmp()}--like function that's used to compare
elements.
@end table

Returns a pointer to the found element in the array, or @code{NULL} if
the element is not present.
@end deftypefun



@c ------------------------------------------------------------
@node Vector Iteration
@subsection Iteration over a vector


@noindent
It's easy to iterate over all the elements of a vector.

Example of forward iteration:

@example
ucl_vector_t     this;
DataType *     ptr

...

for (ptr = ucl_vector_front(vectPtr);
     ptr <= (DataType *) ucl_vector_back(vectPtr);
     ++ptr)
  @{
    /* do something with "*ptr" */
  @}
@end example

Example of backward iteration:

@example
ucl_vector_t     this;
DataType *     ptr

...

for (ptr = ucl_vector_back(vectPtr);
     ptr >= (DataType *) ucl_vector_front(vectPtr);
     --ptr)
  @{
    /* do something with "*ptr" */
  @}
@end example


@c ------------------------------------------------------------
@node Vector Memory
@subsection Allocating and freeing memory


@noindent
The allocation behaviour for a vector container are ruled by the
arguments handed to the constructor (@ref{Vector Creation}). The rules
are:

@itemize @bullet
@item
an initial number of element slots is selected and an array of such size
is allocated;

@item
a number of slots to be kept free at the beginning of the array is
selected, so it can be used to move elements during the insert
operation, saving some time when new elements are added at the
beginning;

@item
a ``step up'' number is selected: when the array is reallocated to be
enlarged, the new size is the minimum multiple of this number greater
than the number of elements in the container;

@item
a ``step down'' number is selected: when the code has to determine if
the array has to be reallocated for restriction, the operation is
performed if there are that number of free slots; 
@end itemize

@noindent
the step up and step down numbers provide the hysteresis required to
minimise the number of reallocations.


@deftypefun {void *} ucl_vector_enlarge (ucl_vector_t *@var{this})
Checks if there are free slots in the allocated memory: if there are,
nothing happens; else the array is reallocated and enlarged according to
the reallocation rules.

Returns a pointer to the first element in the array, or @code{NULL} if
the reallocation failed. An error doesn't invalidate the array.
@end deftypefun


@deftypefun {void *} ucl_vector_restrict (ucl_vector_t *@var{this})
Checks if there are enough free slots in the allocated memory so that
it's correct to restrict the array: if there aren't, nothing happens;
else the array is reallocated and restricted according to the
reallocation rules.

Returns a pointer to the first element in the array, or @code{NULL} if
the reallocation failed. An error doesn't invalidate the array.
@end deftypefun


@c ------------------------------------------------------------
@node Vector as PQueue
@subsection Using a vector as a priority queue


@noindent
The vector structure provides all the functions required to implement a
priority queue.  This is a structure in which elements are associated
with keys: when an element is added and the structure is kept sorted
comparing its key with the keys of the elements already in the
container.

Let's say we have declared a structure like this:

@example
typedef struct Pair @{
  Key   key;
  Val   val;
@} Pair;
@end example

@noindent
and a function used to compare @code{Pair}'s keys:

@example
int keycmp (Key *a, Key *b);
@end example

If any time a @code{Pair} must be inserted in the vector we use the
@code{ucl_vector_insertsort()} function to determine the insertion
position, the elements will be kept sorted according to the key values
and @code{keycmp()} algorithm.

Then @code{ucl_vector_front()} or @code{ucl_vector_back()} can be used
to extract the element with lesser or greater key.



@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
