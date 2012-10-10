
#include "hash_lib.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct dll_s
{
  p_key key;
  p_data data;
  struct dll_s *prev;
  struct dll_s *next;
} dll_t;

/* this is defined here so as to abstract it from app */
typedef struct hash_table_s 
{
  bucket_t num_buckets;
  u_int32_t (*hash_function)(p_key, bucket_t);
  u_int32_t (*compare_function)(p_key, p_key);
  dll_t *buckets[];
} hash_table_t;

bucket_t def_hash_function(p_key key, bucket_t table_size)
{
  /*
   * Just treat p_key as sizeof(p_key) and mod with table_size.
   * As we do not know the type of key, I dont think we can do 
   * anything better than this
   */
  return (bucket_t)(((bucket_t)key) % table_size); 
}

hash_ret_e def_compare_function(p_key compare_this, p_key with_this)
{
  if(((bucket_t)compare_this) == ((bucket_t)with_this))
    return HASH_OK;
  else
    return HASH_NOTOK;
}

/* 
 * hash_function: to calculate the index/bucket.
 * compare_function: to compare the items which have collided.
 */
hasht hash_init(bucket_t num_buckets, 
		bucket_t (*hash_function)(p_key, bucket_t),
		bucket_t (*compare_function)(p_key, p_key))
{
  hasht hptr;

  hptr = (hasht) malloc (sizeof (*hptr));

  if(hptr == NULL)
    return NULL;

  /* Allocate number of buckets in this hash table */
  hptr->buckets = (dll_t **) malloc (num_buckets * sizeof(dll_t *));

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

hash_ret_e hash_insert(hasht hptr, p_key key, p_data data)
{
  bucket_t index;
  dll_t *node;

  index = hptr->hash_function(pkey, hptr->num_buckets);

  node = (dll_t *) malloc (sizeof(dll_t));
  if(node == NULL)
    HASH_ERROR;

  node->data = data;

  if(hptr->buckets[index] == NULL) {
    /* First item in linked list */
    node->next = NULL;
    node->prev = NULL;
    hptr->buckets[index] = node;
  } else {
    hptr->buckets[index]->prev = node;
    node->next = hptr->buckets[index];
    node->prev = NULL;
  }

  return HASH_OK; 
}

hash_ret_e hash_remove(hasht hptr, p_key key)
{
  bucket_t index;
  dll_t *node;

  index = hptr->hash_function(pkey, hptr->num_buckets);

  /*
   * Go th the linked list and by comparing the keys find the
   * item to be removed and remove/free it from the linked list.
   */
  for(node = hptr->buckets[index]; node != NULL; node = node->next) {
    if(HASH_OK == hptr->compare_function(key, node->key)) {
      if(node->prev) {
	node->prev->next = node->next;
      }
      else {
	/* Removing the head, re initialize head */
	hptr->buckets[index] = node->next;
      }
      if(node->next)
	node->next->prev = node->prev;      

      free(node);
      return HASH_OK;
    }
  }

  return HASH_NOT_FOUND;
}

hash_ret_e hash_lookup(hasht hptr, p_key, p_data *pp_data)
{
  bucket_t index;
  dll_t *node;

  *pp_data = NULL;
  index = hptr->hash_function(pkey, hptr->num_buckets);

  /*
   * Got the index.
   * Go th the linked list and by comparing the keys find the
   * item to be that matches the key and return the data.
   */
  for(node = hptr->buckets[index]; node != NULL; node = node->next) {
    if(HASH_OK == hptr->compare_function(key, node->key)) {
      *pp_data = node->data;
      return HASH_OK;
    }
  }
  return HASH_NOT_FOUND;
}


