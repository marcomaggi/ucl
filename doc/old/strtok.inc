@c ------------------------------------------------------------
@node Strtok
@section The string structure


@cindex Strtok container


@noindent
The @UCL{} string token container is an implementation of the classic
array with redundant memory allocation.

@deftp {Struct Typedef} ucl_strtok_t
The data type of the base structure.
@end deftp

This data type represents the string with a null--terminated array of
characters and can allocate more memory than strictly required, so that
the number of reallocations when appending data to a string can be
minimised. The @UCL{} string functions will use @code{ucl_malloc()},
@code{ucl_realloc()} and @code{ucl_free()} to handle memory allocation.

The structure has the following data members:

@table @code
@item size_t size
the number of bytes actually allocated for the array of characters;
being the string representation null--terminated, this is one unit
higher than the maximum possible string length;

@item size_t len
the actual length of the string in the array of character; this is the
index of the terminating null character;

@item char * ptr
pointer to the array, we use it to access the data.
@end table


This module was inspired by the book on C++ by Bjarne Stroustrup, by the
STL C++ (Standard Template Library) and the TCL (Tool Command Language)
string manipulation functions.


@menu
* Strtok Creation::             Creating and destroying strings.
* Strtok Adding::               Adding elements to a string.
* Strtok Removing::             Removing elements from a string.
* Strtok Access::               Accessing string data.
* Strtok Ops::                  Various operations on a string.
* Strtok Memory::               Allocating and freeing memory.
@end menu



@c ------------------------------------------------------------
@node Strtok Creation
@subsection Creating and destroying strings


@deftypefun {char *} ucl_strtok_constructor (ucl_strtok_t *@var{this}, size_t @var{size})
Initialises an already allocated string token. Returns a pointer to the
allocated buffer or @code{NULL} if an error occurred allocating
memory. @var{size} is the exact size of the allocated data buffer.
@end deftypefun


@deftypefun void ucl_strtok_destructor (ucl_strtok_t *@var{this})
Destroys the string pointed by @var{this}. The block of dynamically
allocated memory is freed.
@end deftypefun


@deftypefun {char *} ucl_strtok_duplicate (ucl_strtok_t *@var{dst}, ucl_strtok_t *@var{src})
Duplicates a the string in @var{src} into the string @var{dst}.  The
size of the new string buffer may be different from the size of the
original one. Returns a pointer to the new buffer in @var{dst}, or
@code{NULL} if an error allocating memory occurs.
@end deftypefun


Example of @code{ucl_strtok_t} allocation:

@example
char *         ptr;
ucl_strtok_t *   strPtr;

ptr = ucl_strtok_constructor(strPtr, 23);
if (! ptr)
  @{
    /* handle the error */
  @}
/* do something with strPtr */
ucl_strtok_destructor(strPtr);
@end example

@c ------------------------------------------------------------
@node Strtok Adding
@subsection Adding elements to a string


@deftypefun void ucl_strtok_insert (@var{this}, @var{position}, @var{c})
Inserts an element at a selected position.  It assumes that there's a
free slot to hold the new element, to make sure that it's there we have
to call @code{ucl_strtok_enlarge()} (@ref{Strtok Memory}).

Arguments:

@table @code
@item ucl_strtok_t *this
pointer to the base structure;
@item size_t position
the position of the new element;
@item const char c
the element to insert.
@end table
@end deftypefun


@defmac ucl_strtok_pushfront (ucl_strtok_t *@var{this}, const char @var{val})
Inserts an element at the beginning of the string. This is a wrapper for
@code{ucl_strtok_insert()} and so it returns the same value.
@end defmac


@defmac ucl_strtok_pushback (ucl_strtok_t *@var{this}, const char @var{val})
Inserts an element at the end of the string. This is a wrapper for
@code{ucl_strtok_insert()} and so it returns the same error value.
@end defmac


@c ------------------------------------------------------------
@node Strtok Removing
@subsection Removing elements from a string


@deftypefun void ucl_strtok_erase (ucl_strtok_t *@var{this}, size_t @var{position})
Removes an element at a selected @var{position}: the slot is overwritten
moving the characters following it. The memory is not reallocated to
restrict the block: to do this we have to invoke
@code{ucl_strtok_restrict()} (@ref{Strtok Memory}).
@end deftypefun


@defmac ucl_strtok_popfront (ucl_strtok_t *@var{this})
Removes an element at the beginning of the string. This is a wrapper for
@code{ucl_strtok_erase()} and so it returns the same value.
@end defmac


@defmac ucl_strtok_popback (ucl_strtok_t *@var{this})
Removes an element at the end of the string. This is a wrapper for
@code{ucl_strtok_insert()} and so it returns the same value.
@end defmac


@c ------------------------------------------------------------
@node Strtok Access
@subsection Accessing string data.


@defmac ucl_strtok_data (ucl_strtok_t *@var{this})
Returns a pointer to the first character in the string.
@end defmac


@defmac ucl_strtok_back (ucl_strtok_t *@var{this})
Accesses the last element in the string. It supposes that the container
is not empty, so that the element it's there; we have to make sure of
this, for example, by checking the size of the string first.
@end defmac


@defmac ucl_strtok_front (ucl_strtok_t *@var{this})
Accesses the first element in the string. It supposes that the container
is not empty, so that the element it's there; we have to make sure of
this, for example, by checking the size of the string first.
@end defmac


@defmac ucl_strtok_getval (ucl_strtok_t *@var{this}, size_t @var{position})
Accesses the element at @var{position}. It supposes that @var{position}
is a valid index for the string, we can check this with
@code{ucl_strtok_isindex()}.
@end defmac


@defmac ucl_strtok_setval (ucl_strtok_t *@var{this}, size_t @var{position}, char @var{c})
Stores a new element at @var{position}. It supposes that @var{position}
is a valid index for the string, we can check this with
@code{ucl_strtok_isindex()}. The old element is overwritten.
@end defmac


@defmac ucl_strtok_size (ucl_strtok_t *@var{this})
Returns a @code{size_t} value represenging the buffer size.
@end defmac


@defmac ucl_strtok_len (ucl_strtok_t *@var{this})
Returns a @code{size_t} value representing the number of characters in
the string.
@end defmac


@defmac ucl_strtok_isindex (ucl_strtok_t *@var{this}, size_t @var{position})
Returns true (an integer) if @var{position} is a valid index for the
string referenced by @var{this}. It must be a positive integer greater
or equal to zero and lesser than the number of elements in the
container (@code{size_t} should be an unsigned type).
@end defmac

@c ------------------------------------------------------------
@node Strtok Ops
@subsection Various operations on a string



@deftypefun void ucl_strtok_randomise (ucl_strtok_t *@var{this})
Shuffles the string characters. @code{rand()} is used to generate the
entropy.
@end deftypefun


@deftypefun void ucl_strtok_swap (ucl_strtok_t *@var{this}, size_t @var{a}, size_t @var{b})
Swaps two elements in the string.  This function assumes that the
indexes are not out of range.
@end deftypefun


@defmac ucl_strtok_compare (ucl_strtok_t *@var{one}, ucl_strtok_t *@var{two})
A wrapper for @code{strcmp()} that applies to string structures.
@end defmac

@c ------------------------------------------------------------
@node Strtok Memory
@subsection Allocating and freeing memory


@defmac ucl_strtok_isfull (ucl_strtok_t *@var{this})
Returns true if the string buffer is full.
@end defmac


@deftypefun {char *} ucl_strtok_resize (ucl_strtok_t *@var{this}, size_t @var{size})
Reallocates a string to resize the buffer. If the new size is lesser
than the length of the string, the tail is truncated.  Returns the new
pointer to the string, or @code{NULL} if an error occurred reallocating
memory.
@end deftypefun


@deftypefun {void *} ucl_strtok_enlarge (ucl_strtok_t *@var{this})
If the string is full, enlarge the buffer size to the next multiple of
@code{UCL_STRTOK_STEP_UP} starting from the buffer size. Returns the new
pointer to the string, or @code{NULL} if an error occurred reallocating
memory.
@end deftypefun


@deftypefun {void *} ucl_strtok_restrict (ucl_strtok_t *@var{this})
Restrict the buffer size to the next multiple of
@code{UCL_STRTOK_STEP_UP} starting from the string length. Returns the
new pointer to the string, or @code{NULL} if an error occurred
reallocating memory.
@end deftypefun



@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
