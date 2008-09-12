/* Stub header file for the "hash" module.
   This file must be included in the main stub header. */

#ifndef __UCL_HASH_H

/** ------------------------------------------------------------ 
 ** Module "hash".
 ** --------------------------------------------------------- */

/* Function prototypes for the "hash" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_hash_initialise (ucl_hash_t this, ucl_valcmp_t compar, ucl_hashcmp_t hash);
ucl_decl void ucl_hash_constructor (ucl_hash_t this);
ucl_decl void ucl_hash_destructor (ucl_hash_t this);
ucl_decl void ucl_hash_insert (ucl_hash_t this, ucl_hash_entry_t *entry_p);
ucl_decl void ucl_hash_extract (ucl_hash_t this, ucl_hash_entry_t *entry_p);
ucl_decl ucl_hash_entry_t * ucl_hash_find (const ucl_hash_t this, const ucl_value_t key);
ucl_decl ucl_hash_entry_t * ucl_hash_first (const ucl_hash_t this);
ucl_decl void ucl_hash_enlarge (ucl_hash_t this);
ucl_decl void ucl_hash_restrict (ucl_hash_t this);
ucl_decl void ucl_hash_iterator (const ucl_hash_t this, ucl_iterator_t iterator);
ucl_decl size_t ucl_hash_number_of_used_buckets (const ucl_hash_t this);
ucl_decl size_t ucl_hash_bucket_chain_length (const ucl_hash_t this, ucl_vector_index_t position);
ucl_decl double ucl_hash_average_search_distance (const ucl_hash_t this);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "hash" module.
   A typedef for each function in the module. */

typedef void ucl_hash_initialise_proto_t (ucl_hash_t this, ucl_valcmp_t compar, ucl_hashcmp_t hash);
typedef void ucl_hash_constructor_proto_t (ucl_hash_t this);
typedef void ucl_hash_destructor_proto_t (ucl_hash_t this);
typedef void ucl_hash_insert_proto_t (ucl_hash_t this, ucl_hash_entry_t *entry_p);
typedef void ucl_hash_extract_proto_t (ucl_hash_t this, ucl_hash_entry_t *entry_p);
typedef ucl_hash_entry_t * ucl_hash_find_proto_t (const ucl_hash_t this, const ucl_value_t key);
typedef ucl_hash_entry_t * ucl_hash_first_proto_t (const ucl_hash_t this);
typedef void ucl_hash_enlarge_proto_t (ucl_hash_t this);
typedef void ucl_hash_restrict_proto_t (ucl_hash_t this);
typedef void ucl_hash_iterator_proto_t (const ucl_hash_t this, ucl_iterator_t iterator);
typedef size_t ucl_hash_number_of_used_buckets_proto_t (const ucl_hash_t this);
typedef size_t ucl_hash_bucket_chain_length_proto_t (const ucl_hash_t this, ucl_vector_index_t position);
typedef double ucl_hash_average_search_distance_proto_t (const ucl_hash_t this);

/* Stub table type declaration for the "hash" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_hash_stub_table_t {
  ucl_hash_constructor_proto_t        * stub_ucl_hash_constructor;
  ucl_hash_insert_proto_t             * stub_ucl_hash_insert;
  ucl_hash_extract_proto_t            * stub_ucl_hash_extract;
  ucl_hash_find_proto_t               * stub_ucl_hash_find;
  ucl_hash_enlarge_proto_t            * stub_ucl_hash_enlarge;
  ucl_hash_iterator_proto_t           * stub_ucl_hash_iterator;
  ucl_hash_destructor_proto_t         * stub_ucl_hash_destructor;
  ucl_hash_initialise_proto_t         * stub_ucl_hash_initialise;
  ucl_hash_first_proto_t              * stub_ucl_hash_first;
  ucl_hash_restrict_proto_t           * stub_ucl_hash_restrict;
  ucl_hash_number_of_used_buckets_proto_t * stub_ucl_hash_number_of_used_buckets;
  ucl_hash_bucket_chain_length_proto_t * stub_ucl_hash_bucket_chain_length;
  ucl_hash_average_search_distance_proto_t * stub_ucl_hash_average_search_distance;
} ucl_hash_stub_table_t;

/* Stub table macros definition for the "hash" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_hash_stub_table_t * ucl_hash_stub_table_p;
ucl_decl const ucl_hash_stub_table_t   ucl_hash_stub_table;

#define ucl_hash_constructor       ((ucl_hash_stub_table_p)->stub_ucl_hash_constructor)
#define ucl_hash_insert            ((ucl_hash_stub_table_p)->stub_ucl_hash_insert)
#define ucl_hash_extract           ((ucl_hash_stub_table_p)->stub_ucl_hash_extract)
#define ucl_hash_find              ((ucl_hash_stub_table_p)->stub_ucl_hash_find)
#define ucl_hash_enlarge           ((ucl_hash_stub_table_p)->stub_ucl_hash_enlarge)
#define ucl_hash_iterator          ((ucl_hash_stub_table_p)->stub_ucl_hash_iterator)
#define ucl_hash_destructor        ((ucl_hash_stub_table_p)->stub_ucl_hash_destructor)
#define ucl_hash_initialise        ((ucl_hash_stub_table_p)->stub_ucl_hash_initialise)
#define ucl_hash_first             ((ucl_hash_stub_table_p)->stub_ucl_hash_first)
#define ucl_hash_restrict          ((ucl_hash_stub_table_p)->stub_ucl_hash_restrict)
#define ucl_hash_number_of_used_buckets  ((ucl_hash_stub_table_p)->stub_ucl_hash_number_of_used_buckets)
#define ucl_hash_bucket_chain_length  ((ucl_hash_stub_table_p)->stub_ucl_hash_bucket_chain_length)
#define ucl_hash_average_search_distance  ((ucl_hash_stub_table_p)->stub_ucl_hash_average_search_distance)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_HASH_H */

/* end of file */
