

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

typedef struct hash_table_s 
{
  dll_t **buckets;
  
} hash_table_t;

/* 
 * hash_function: to calculate the index/bucket.
 * compare_function: to compare the items which have collided.
 */
hasht hash_init(u_int32_t buckets, hash_function, compare_function)
{
  hast hptr;

  hptr = (hast) malloc (sizeof (*hptr));

  if(hptr == NULL)
    return NULL;

  *(hptr->buckets) = (dll_t **) malloc (num_buckets * sizeof(dll_t));
  // check for malloc error.

  // bzero((*(hptr->buckets))

  return hptr;
}

hash_insert()
hash_remove()
hash_lookup()


