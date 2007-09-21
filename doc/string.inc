@c ------------------------------------------------------------
@node String
@section The string structure


@cindex String container


@noindent
The @UCL{} string container is an implementation of the classic array
with redundant memory allocation.

@deftp {Struct Typedef} ucl_string_t
The data type of the base structure. With this type of string, the
metadata (length, buffer size) is stored in the same dynamically
allocated memory block as the characters data. It is supposed to be used
in the same fashion of a normal array of characters.
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
* String Creation::             Creating and destroying strings.
* String Adding::               Adding elements to a string.
* String Removing::             Removing elements from a string.
* String Access::               Accessing string data.
* String Ops::                  Various operations on a string.
* String Memory::               Allocating and freeing memory.
@end menu



@c ------------------------------------------------------------
@node String Creation
@subsection Creating and destroying strings


@deftypefun {ucl_string_t *} ucl_string_constructor (size_t @var{size})
Allocates and initialises an empty string. Returns a pointer to the
allocated structure or @code{NULL} if an error occurred allocating
memory. @var{size} is the exact size of the allocated data buffer.
@end deftypefun


@defmac ucl_string_destructor (ucl_string_t *@var{this})
Destroys the structure pointed by @var{this}. The block of dynamically
allocated memory is freed.  After the call to this function the pointer
is no more valid.
@end defmac


@deftypefun {ucl_string_t *} ucl_string_duplicate (ucl_string_t *@var{this})
Duplicates a string. A new memory block is allocated and the string
referenced by @var{this} is duplicated into it. The size of the new
string buffer may be different from the size of the original string.

Returns a pointer to the new string structure or @code{NULL} if an error
allocating memory occurs.
@end deftypefun


Example of @code{ucl_string_t} allocation:

@example
ucl_string_t *   strPtr;

strPtr = ucl_string_constructor(23);
if (! strPtr)
  @{
    /* handle the error */
  @}
/* do something with strPtr */
ucl_string_destructor(strPtr);
@end example

@c ------------------------------------------------------------
@node String Adding
@subsection Adding elements to a string


@deftypefun void ucl_string_insert (@var{this}, @var{position}, @var{c})
Inserts an element at a selected position.  It assumes that there's a
free slot to hold the new element, to make sure that it's there we have
to call @code{ucl_string_enlarge()} (@ref{String Memory}).

Arguments:

@table @code
@item ucl_string_t *this
pointer to the base structure;
@item size_t position
the position of the new element;
@item const char c
the element to insert.
@end table
@end deftypefun


@defmac ucl_string_pushfront (ucl_string_t *@var{this}, const char @var{val})
Inserts an element at the beginning of the string. This is a wrapper for
@code{ucl_string_insert()} and so it returns the same value.
@end defmac


@defmac ucl_string_pushback (ucl_string_t *@var{this}, const char @var{val})
Inserts an element at the end of the string. This is a wrapper for
@code{ucl_string_insert()} and so it returns the same error value.
@end defmac


@c ------------------------------------------------------------
@node String Removing
@subsection Removing elements from a string


@deftypefun void ucl_string_erase (ucl_string_t *@var{this}, size_t @var{position})
Removes an element at a selected @var{position}: the slot is overwritten
moving the characters following it. The memory is not reallocated to
restrict the block: to do this we have to invoke
@code{ucl_string_restrict()} (@ref{String Memory}).
@end deftypefun


@defmac ucl_string_popfront (ucl_string_t *@var{this})
Removes an element at the beginning of the string. This is a wrapper for
@code{ucl_string_erase()} and so it returns the same value.
@end defmac


@defmac ucl_string_popback (ucl_string_t *@var{this})
Removes an element at the end of the string. This is a wrapper for
@code{ucl_string_insert()} and so it returns the same value.
@end defmac


@c ------------------------------------------------------------
@node String Access
@subsection Accessing string data.


@defmac ucl_string_data (ucl_string_t *@var{this})
Returns a pointer to the first character in the string.
@end defmac


@defmac ucl_string_back (ucl_string_t *@var{this})
Accesses the last element in the string. It supposes that the container
is not empty, so that the element it's there; we have to make sure of
this, for example, by checking the size of the string first.
@end defmac


@defmac ucl_string_front (ucl_string_t *@var{this})
Accesses the first element in the string. It supposes that the container
is not empty, so that the element it's there; we have to make sure of
this, for example, by checking the size of the string first.
@end defmac


@defmac ucl_string_getval (ucl_string_t *@var{this}, size_t @var{position})
Accesses the element at @var{position}. It supposes that @var{position}
is a valid index for the string, we can check this with
@code{ucl_string_isindex()}.
@end defmac


@defmac ucl_string_setval (ucl_string_t *@var{this}, size_t @var{position}, char @var{c})
Stores a new element at @var{position}. It supposes that @var{position}
is a valid index for the string, we can check this with
@code{ucl_string_isindex()}. The old element is overwritten.
@end defmac


@defmac ucl_string_size (ucl_string_t *@var{this})
Returns a @code{size_t} value represenging the buffer size.
@end defmac


@defmac ucl_string_len (ucl_string_t *@var{this})
Returns a @code{size_t} value representing the number of characters in
the string.
@end defmac


@defmac ucl_string_isindex (ucl_string_t *@var{this}, size_t @var{position})
Returns true (an integer) if @var{position} is a valid index for the
string referenced by @var{this}. It must be a positive integer greater
or equal to zero and lesser than the number of elements in the
container (@code{size_t} should be an unsigned type).
@end defmac

@c ------------------------------------------------------------
@node String Ops
@subsection Various operations on a string



@deftypefun void ucl_string_randomise (ucl_string_t *@var{this})
Shuffles the string characters. @code{rand()} is used to generate the
entropy.
@end deftypefun


@deftypefun void ucl_string_swap (ucl_string_t *@var{this}, size_t @var{a}, size_t @var{b})
Swaps two elements in the string.  This function assumes that the
indexes are not out of range.
@end deftypefun


@defmac ucl_string_compare (ucl_string_t *@var{one}, ucl_string_t *@var{two})
A wrapper for @code{strcmp()} that applies to string structures.
@end defmac

@c ------------------------------------------------------------
@node String Memory
@subsection Allocating and freeing memory


@defmac ucl_string_isfull (ucl_string_t *@var{this})
Returns true if the string buffer is full.
@end defmac


@deftypefun {void *} ucl_string_resize (ucl_string_t *@var{this}, size_t @var{size})
Reallocates a string to resize the buffer. If the new size is lesser
than the length of the string, the tail is truncated.  Returns the new
pointer to the string, or @code{NULL} if an error occurred reallocating
memory.
@end deftypefun


@deftypefun {void *} ucl_string_enlarge (ucl_string_t *@var{this})
If the string is full, enlarge the buffer size to the next multiple of
@code{UCL_STRING_STEP_UP} starting from the buffer size. Returns the new
pointer to the string, or @code{NULL} if an error occurred reallocating
memory.
@end deftypefun


@deftypefun {void *} ucl_string_restrict (ucl_string_t *@var{this})
Restrict the buffer size to the next multiple of
@code{UCL_STRING_STEP_UP} starting from the string length. Returns the
new pointer to the string, or @code{NULL} if an error occurred
reallocating memory.
@end deftypefun



@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
