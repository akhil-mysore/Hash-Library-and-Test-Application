
/* Public Functions */

typedef void * hasht;

hasht hash_init(u_int32_t buckets, u_int32_t (*hash_function(pa, pa)), u_int32_t (*compare_function(pa, pa)), );
