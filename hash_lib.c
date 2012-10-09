


/* The Brain */

#include <hash_lib.h>

typedef unsigned int u_int32_t;
typedef unsigned short u_int16_t;
typedef unsigned char u_int8_t;

typedef dll_s
{
  void *data;
  struct dll_s *prev;
  struct dll_s *next;
} dll_t;

/* this is defined here so as to abstract it from app */
typedef struct hash_table_s 
{
  u_int32_t num_buckets;
  u_int32_t (*hash_function(pa, pa));
  u_int32_t (*compare_function));
  dll_t **buckets;
} hash_table_t;

/* 
 * hash_function: to calculate the index/bucket.
 * compare_function: to compare the items which have collided.
 */
hasht hash_init(u_int32_t num_buckets, 
		u_int32_t (*hash_function(pa, pa)),
		u_int32_t (*compare_function(pa, pa)))
{
  hasht hptr;

  hptr = (hasht) malloc (sizeof (*hptr));

  if(hptr == NULL)
    return NULL;

  /* Allocate number of buckets in this hash table */
  (hptr->buckets) = (dll_t **) malloc (num_buckets * sizeof(dll_t));

  if(hptr->buckets == NULL)
    return NULL;
  
  /* Initialize head pointers of all the buckets to NULL */
  for(index=0; index<num_buckets; index++)
    hptr->buckets[index] = NULL;

  hptr->num_buckets = num_buckets;

  if(hash_function)
    hptr->hash_function = hash_function;
  else
    hptr->hash_function = def_hash_fucntion;

  if(compare_function)
    hptr->compare_function = compare_function;
  else
    hptr->compare_function = def_compare_function;

  return hptr;
}

hash_insert(hasht hptr, )
hash_remove()
hash_lookup()


