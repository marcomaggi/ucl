@c ------------------------------------------------------------
@node Hash
@section The hash table structure


@cindex Hash table container


@noindent
A hash table is a structure that maps keys to values in a way that
allows the search operation to be performed with constant for all the
keys.

@deftp {Struct Typedef} ucl_hash_t
The base structure. Two interesting fields are: @code{dim}, the number
of buckets allocated; @code{used}, the number of used buckets.
@end deftp

@deftp {Struct Typedef} ucl_hash_entry_t
The type of table elements. Both the key and the value stored in a hash
table entry are of type @code{ucl_value_t} (@ref{Typedefs}).
@end deftp

@deftypefn {Function Typedef} size_t ucl_hashfun_t (ucl_value_t @var{val})
The prototype  of the hash function.  The hash container  code will take
care of  mapping the @code{size_t}  value to an  index in the  vector of
buckets.
@end deftypefn

The hash was inspired by the book on C++ by Bjarne Stroustrup and the
hash structure in the @acronym{TCL} (Tool Command Language) source code,
by John Ousterhout and others (@url{http://www.tcl.tk/} for more about
@acronym{TCL}).


@menu
* Hash Implementation::         How it's done.
* Hash Creation::               Creating and destroying hash tables.
* Hash Adding::                 Adding elements to a hash table.
* Hash Removing::               Removing elements from a hash table.
* Hash Ops::                    Various operations on a hash table.
* Hash Resizing::               Resizing a hash table.
* Hash Iterator::               Visiting elements in the table.
* Hash Functions::              Provided hash function.
@end menu


@c ------------------------------------------------------------
@node Hash Implementation
@subsection How it's done


@noindent
A vector of pointers is allocated by the constructor. Each pointer,
called ``bucket'' in this document, can be @code{NULL} (empty bucket) or
referencing an entry structure. Entry structures are chained in a linked
list.

Let's say that the vector has @code{dim} buckets.

When inserting a new entry in the table, the hash function converts the
keys to integers in the range 0@dots{}@code{dim}-1, selecting a bucket;
then the bucket is examined: if it's @code{NULL}, it's set to the entry
pointer; if it's not @code{NULL}, the referenced entry is appended to
the new entry and the bucket is set to a pointer to the new entry.

@example
 ----                          ----
|    |                        |    |
 ----     ---     ---          ----     ---     ---     ---
|    |-->|en1|-->|en2|   ->   |    |-->|new|-->|en1|-->|en2|
 ----     ---     ---          ----     ---     ---     ---
|    |                        |    |
 ----                          ----
@end example

It's obvious how the extraction operation works.

If the keys are such that the hash function distributes entries
uniformly over all the buckets, the time spent to find an entry is (more
or less) constant.

When the hash table is resized, say to @code{newdim}, the vector is
enlarged. Now the hash function converts keys in indexes in the range
0@dots{}@code{newdim}-1, so all the entries already in the table are in
the wrong position. They must be extracted and rehashed. This is
expensive.

Can the vector be restricted? Sure! But all the entries must be rehashed
with a lesser @code{dim} before the reallocation. This operation is
currently not supported, but the algorithm is not different from the one
used to enlarge (and you have the code).

@c ------------------------------------------------------------
@node Hash Creation
@subsection Creating and destroying hash tables


@deftypefun {void *} ucl_hash_constructor (@var{this}, @var{dim}, @var{keycmp}, @var{hash})
Initialises a hash table. Arguments:

@table @code
@item ucl_hash_t * @var{this}
pointer to the already allocated structure;

@item size_t @var{dim}
the initial number of buckets;

@item ucl_valcmp_t * @var{keycmp}
pointer to a @code{strcmp()}--like function used to compare keys;

@item ucl_hashfun_t * @var{hash}
pointer to the hash function.
@end table

The vector of buckets is allocated with a call to @code{ucl_malloc()}.

Returns @code{NULL} if an error allocating the vector of buckets
occurred.
@end deftypefun


@deftypefun void ucl_hash_destructor (ucl_hash_t * @var{this})
Destroys a hash table. The vector of buckets is freed with a call to
@code{ucl_free()}. Before invoking this function all the entries must be
extracted from the table.
@end deftypefun

@c ------------------------------------------------------------
@node Hash Adding
@subsection Adding elements to a hash table


@deftypefun void ucl_hash_insert (ucl_hash_t *@var{this}, ucl_hash_entry_t *@var{entPtr})
Inserts a new entry in the table. The entry structure must be allocated
and filled with key and value by the user code. Inserting an entry with
a key that already exists in the table will work, but future invocations
of @code{ucl_hash_find()} will return one or the other: nobody knows
which.
@end deftypefun

To avoid collision of keys, we have to check the existence of a key with
@code{ucl_hash_find()} before attempting to insert a new entry.

Example:

@example
ucl_hash_t *         hashPtr;
ucl_hash_entry_t *   entryPtr;
ucl_value_t            value;

entryPtr = malloc(sizeof(ucl_hash_entry_t));
if (!entryPtr)
  @{
  @}

value = ...;
ucl_hash_setkey(entryPtr, value);
value = ...;
ucl_hash_setval(entryPtr, value);

ucl_hash_insert(hashPtr, entryPtr);
@end example


@c ------------------------------------------------------------
@node Hash Removing
@subsection Removing elements from a hash table



@deftypefun void ucl_hash_extract (ucl_hash_t *@var{this}, ucl_hash_entry_t *@var{entPtr})
Extracts an entry from the table. @var{entPtr}, a pointer to the entry
to be removed, must be the return value of a previous invocation to
@code{ucl_hash_find()}. The entry structure is not destroyed nor freed,
just extracted.
@end deftypefun


Example:

@example
ucl_hash_t *         hashPtr;
ucl_hash_entry_t *   entryPtr;
ucl_value_t            value, key, key1;

hashPtr  = ...;
key      = ...;
entryPtr = ucl_hash_find(hashPtr, key);
ucl_hash_extract(hashPtr, entPtr);

key1  = ucl_hash_getkey(entryPtr);
/* destroy key1 */

value = ucl_hash_getval(entryPtr);
/* destroy value */

free(entryPtr);
@end example

@c ------------------------------------------------------------
@node Hash Ops
@subsection Various operations on a hash table


@deftypefun {ucl_hash_entry_t *} ucl_hash_find (ucl_hash_t *@var{this}, ucl_value_t @var{key})
Search in the table an entry associated with the selected key. Returns a
pointer to the entry or @code{NULL} if the key was not found.
@end deftypefun


@defmac ucl_hash_size (ucl_hash_t *@var{this})
Returns a @code{size_t} value representing the number of entries in the
table.
@end defmac


@defmac ucl_hash_setkey (ucl_hash_entry_t *@var{entPtr}, ucl_value_t @var{newkey})
Stores a new key in an entry structure. The old key, if present, is
overwritten.
@end defmac


@defmac ucl_hash_getkey (ucl_hash_entry_t *@var{entPtr})
Returns the key stored in an entry structure.
@end defmac


@defmac ucl_hash_setval (entPtr, newval)
Stores a new value in an entry structure. The old avlue, if present, is
overwritten.
@end defmac


@defmac ucl_hash_getval (entPtr)
Returns the value stored in an entry structure.
@end defmac


@c ------------------------------------------------------------
@node Hash Resizing
@subsection Resizing a hash table


@noindent
At present the hash table is not enlarged automatically. The decision is
delegated to the user code; we have to examine the @code{dim} and
@code{used} members of the base structure and implement a reallocation
policy ourselves.

@code{dim} is the current number of allocated buckets: the second
argument to @code{ucl_hash_constructor()}. @code{used} is the number of
used buckets.


@deftypefun {void *} ucl_hash_enlarge (ucl_hash_t *@var{this}, size_t @var{newdim})
Enlarge the table. The new number of buckets is @var{newdim}: it must be
greater (not equal) to the current dimension.  This is an expensive
operation because it requires rehashing all the entries.

Returns @code{NULL} if an error reallocating the vector occurred. In
this case the table is still usable, unless you redefined the
@code{ucl_realloc()} macro to a function that messes with the input.
@end deftypefun


@c ------------------------------------------------------------
@node Hash Iterator
@subsection Visiting elements in the table


@deftypefun void ucl_hash_iterator (ucl_hash_t *@var{this}, ucl_iterator_t *@var{iterator})
Initialises the table iterator. The iterator pointer references the
entries in the table. The order in which the entries are visited is
unknown.
@end deftypefun


Example:

@example
ucl_hash_t *         hashPtr;
ucl_hash_entry_t *   entryPtr;
ucl_value_t          key, value;
ucl_iterator_t       iterator;

hashPtr  = ...;

for (ucl_hash_iterator(hashPtr, &iterator);
     ucl_iterator_more(&iterator);
     ucl_iterator_next(&iterator))
  @{
    entryPtr = ucl_iterator_ptr(&iterator);
    key      = ucl_hash_getkey(entryPtr);
    ...
  @}
@end example

@c ------------------------------------------------------------
@node Hash Functions
@subsection Provided hash function


@deftypefun size_t ucl_hash_string (ucl_value_t @var{val})
Returns an unsigned integer representing the hash value for the string
in @var{val}. The @code{str} member of @var{val} must be a pointer to a
@code{NULL}--terminated string of characters.

The hashing algorithm comes from a C++ book by Bjarne Stroustrup
(@ref{References}).
@end deftypefun




@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
