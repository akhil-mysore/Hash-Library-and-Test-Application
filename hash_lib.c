
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
  hash_ret_e (*hash_function)(p_key, bucket_t);
  hash_ret_e (*compare_function)(p_key, p_key);
  void (*dump_function)(p_key, p_data, bucket_t index);
  dll_t **buckets;
} hash_table_t;

static bucket_t def_hash_function(p_key key, bucket_t table_size);
static hash_ret_e def_compare_function(p_key compare_this, p_key with_this);

static bucket_t def_hash_function(p_key key, bucket_t table_size)
{
  /*
   * Just treat p_key as sizeof(p_key) and mod with table_size.
   * As we do not know the type of key, I dont think we can do 
   * anything better than this
   */
  return (bucket_t)(((bucket_t)key) % table_size); 
}

static hash_ret_e def_compare_function(p_key compare_this, p_key with_this)
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
                bucket_t (*compare_function)(p_key, p_key),
                void (*dump_function)(p_key, p_data, bucket_t index))
{
  hash_table_t *hptr;
  bucket_t index;

  hptr = (hash_table_t *) malloc (sizeof (hash_table_t));

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
    hptr->hash_function = def_hash_function;

  if(compare_function)
    hptr->compare_function = compare_function;
  else
    hptr->compare_function = def_compare_function;

  hptr->dump_function = dump_function;

  return (hasht) hptr;
}

hash_ret_e hash_insert(hasht hash_ptr, p_key pkey, p_data data)
{
  bucket_t index;
  dll_t *node;
  hash_table_t *hptr = (hash_table_t *) hash_ptr;

  index = hptr->hash_function(pkey, hptr->num_buckets);

  for(node = hptr->buckets[index]; node != NULL; node = node->next) {
    if(HASH_OK == hptr->compare_function(pkey, node->key)) {
      return HASH_DUP;
    }
  }

  node = (dll_t *) malloc (sizeof(dll_t));
  if(node == NULL)
    return HASH_ERROR;

  node->key = pkey;
  node->data = data;

  if(hptr->buckets[index] == NULL) {
    /* First item in linked list */
    node->next = NULL;
    node->prev = NULL;
    hptr->buckets[index] = node;
  } else {
    /* Insert at head */
    hptr->buckets[index]->prev = node;
    node->next = hptr->buckets[index];
    node->prev = NULL;
    hptr->buckets[index] = node;
  }

  return HASH_OK; 
}

hash_ret_e hash_remove(hasht hash_ptr, p_key pkey)
{
  bucket_t index;
  dll_t *node;
  hash_table_t *hptr = (hash_table_t *) hash_ptr;

  index = hptr->hash_function(pkey, hptr->num_buckets);

  /*
   * Go th the linked list and by comparing the keys find the
   * item to be removed and remove/free it from the linked list.
   */
  for(node = hptr->buckets[index]; node != NULL; node = node->next) {
    if(HASH_OK == hptr->compare_function(pkey, node->key)) {
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

hash_ret_e hash_lookup(hasht hash_ptr, p_key pkey, p_data *pp_data)
{
  bucket_t index;
  dll_t *node;
  hash_table_t *hptr = (hash_table_t *) hash_ptr;

  *pp_data = NULL;
  index = hptr->hash_function(pkey, hptr->num_buckets);

  /*
   * Got the index.
   * Go th the linked list and by comparing the keys find the
   * item to be that matches the key and return the data.
   */
  for(node = hptr->buckets[index]; node != NULL; node = node->next) {
    if(HASH_OK == hptr->compare_function(pkey, node->key)) {
      *pp_data = node->data;
      return HASH_OK;
    }
  }
  return HASH_NOT_FOUND;
}


hash_ret_e hash_dump(hasht hash_ptr)
{
  bucket_t      index;
  dll_t         *node;
  hash_table_t  *hptr = (hash_table_t *) hash_ptr;

  if(hptr->dump_function == NULL)
        return HASH_OK;

  for(index=0; index<hptr->num_buckets; index++)
  {
    /* Go over the linked list in each bucket and dump the entries. */  
    node = hptr->buckets[index];
    while(node)
    {
      hptr->dump_function(node->key, node->data, index);
      node = node->next;
    }
  }

  return HASH_OK;
  
}

