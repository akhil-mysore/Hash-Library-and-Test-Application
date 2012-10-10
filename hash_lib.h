
#ifndef __INCLUDE_HASH__
#define __INCLUDE_HASH__

typedef unsigned int u_int32_t;
typedef unsigned short u_int16_t;
typedef unsigned char u_int8_t;

typedef void * hasht;
typedef void * p_key;
typedef void * p_data;
typedef u_int32_t bucket_t;

typedef enum {
  HASH_OK = 0,
  HASH_NOTOK,
  HASH_NOT_FOUND,
  HASH_DUP,
  HASH_ERROR,
} hash_ret_e;

hasht hash_init(bucket_t num_buckets,
		bucket_t (*hash_function)(p_key, bucket_t),
		bucket_t (*compare_function)(p_key, p_key));
hash_ret_e hash_insert(hasht hptr, p_key key, p_data data);
hash_ret_e hash_remove(hasht hptr, p_key key);
hash_ret_e hash_lookup(hasht hptr, p_key, p_data *pp_data);

#endif
